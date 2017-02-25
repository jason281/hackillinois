//g++ `pkg-config --libs opencv` -o FD FD.cpp -lwiringPi

//#include <Python.h>
#include<stdio.h>
#include<wiringPi.h>
#include<string.h>
#include<stdlib.h>
//#include<cv.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cctype>
#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <Eigen/Dense>
#include "face.h"

#define Width 320
#define Height 240 
#define MAX_( x, y ) ( ((x) > (y)) ? (x) : (y) ) 
#define MIN_( x, y ) ( ((x) < (y)) ? (x) : (y) ) 

using namespace cv;
using namespace std;
using namespace Eigen;

void FR_onlyPCA(int TRAINNO, int TESTNO, Face* trainFace, Face* testFace, int deleteEigenNo, bool deleteEigenOrder, MatrixXf* trainCoef, MatrixXf* testCoef){
	//dimension of trainCoef is (TRAINNO-1,TRAINNO), dimension of trainCoef is (TRAINNO-1,TESTNO)
	//initiate local variables
	float meanFace[HEIGHT*WIDTH];
	for (int i = 0; i < HEIGHT*WIDTH; i++){
		meanFace[i] = 0;
	}
	float** demeanedFace = new float*[TRAINNO];
	for (int i = 0; i < TRAINNO; i++){
		demeanedFace[i] = new float[HEIGHT*WIDTH];
	}
	for (int i = 0; i < TRAINNO; i++){
		for (int j = 0; j < HEIGHT*WIDTH; j++){
			meanFace[j] += trainFace[i].getdata()[j];
			demeanedFace[i][j] = trainFace[i].getdata()[j];
		}
	}

	for (int i = 0; i < HEIGHT*WIDTH; i++){
		meanFace[i] /= TRAINNO;
	}

	for (int i = 0; i < TRAINNO; i++){
		for (int j = 0; j < HEIGHT * WIDTH; j++){
			demeanedFace[i][j] -= meanFace[j];
		}
	}

	Face mean(meanFace);
	mean.show("meanFace");
	mean.save("./PCA/meanFace.bmp");
	mean.savedata("./PCA/meanFace.txt");

	//Save the training face matrix into the format used in Eigen library
	MatrixXf trainingset(WIDTH*HEIGHT, TRAINNO);
	for (int i = 0; i < TRAINNO; i++){
		for (int j = 0; j < HEIGHT * WIDTH; j++){
			trainingset(j, i) = demeanedFace[i][j];
		}
	}
	MatrixXf covFace(TRAINNO, TRAINNO);
	covFace = trainingset.transpose()*trainingset;
	//cout << "Here is the matrix m:\n" << covFace << std::endl;
	//cout << "The matrix m is of size " << covFace.rows() << "x" << covFace.cols() << std::endl;
	//Get the eigen value and eigen vector of the covFace
	SelfAdjointEigenSolver<MatrixXf> eigensolver(covFace);
	if (eigensolver.info() != Success) abort();
	//cout << "The eigenvalues of covFace are:\n" << eigensolver.eigenvalues() << endl;

	MatrixXf eigenValue(TRAINNO - 1, 1);
	MatrixXf eigenVector(WIDTH*HEIGHT, TRAINNO - 1);
	for (int i = 0; i < TRAINNO - 1; i++){
		eigenValue(i) = eigensolver.eigenvalues()(i + 1);
	}

	for (int i = 0; i < TRAINNO - 1; i++){
		eigenVector.col(i) = trainingset * eigensolver.eigenvectors().col(i + 1);
		eigenVector.col(i) /= real(sqrt(eigenValue(i)));
	}
	/*for (int i = 1; i < TRAINNO; i++){
	for (int j = 0; j < WIDTH*HEIGHT; j++){
	eigenVector(j, i) /= real(sqrt(eigensolver.eigenvalues()(i)));
	}
	}*/
	//for (int i = 0; i < TRAINNO; i++){
	//	cout << "The eigenvectors of covFace are:\n" << eigenVector(i, 0) << endl;
	//}

	MatrixXf testingset(WIDTH*HEIGHT, TESTNO);
	for (int i = 0; i < TESTNO; i++){
		for (int j = 0; j < HEIGHT * WIDTH; j++){
			testingset(j, i) = testFace[i].getdata()[j] - meanFace[j];
		}
	}

	if (deleteEigenOrder){		//if delete several eigenvectors of large eigenvalue
		*trainCoef = (eigenVector.transpose() * trainingset).block(0, 0, TRAINNO - 1 - deleteEigenNo, TRAINNO);
		*testCoef = (eigenVector.transpose() * testingset).block(0, 0, TRAINNO - 1 - deleteEigenNo, TESTNO);
	}
	else{						//if delete the first several eigenvectors of small eigenvalue
		*trainCoef = (eigenVector.transpose() * trainingset).block(deleteEigenNo, 0, TRAINNO - 1 - deleteEigenNo, TRAINNO);
		*testCoef = (eigenVector.transpose() * testingset).block(deleteEigenNo, 0, TRAINNO - 1 - deleteEigenNo, TESTNO);
	}

	delete demeanedFace;
}

float FR_euclideanDistance(int TRAINNO, int TESTNO, Face* trainFace, Face* testFace, MatrixXf* train, MatrixXf* test){

	if ((train->cols() != TRAINNO) || (test->cols() != TESTNO)){
		std::cout << "the column numbers of the matrice pass to the distance calculation is incorrect!" << endl;
		return 0;
	}
	if (test->rows() != train->rows()){
		std::cout << "the row numbers of the matrice pass to the distance calculation is incorrect!" << endl;
		return 0;
	}
	int coefNo = train->rows();
	cout << "Number of coefficients:" << coefNo << endl;

	//Calcuate the distance
	int* matchedFace = new int[TESTNO];
	int matchCounter = 0;
	float euclideanDistance_min;

	for (int i = 0; i < TESTNO; i++){
		euclideanDistance_min = 1000000000;
		for (int j = 0; j < TRAINNO; j++){
			VectorXf distance(coefNo);
			distance = test->col(i) - train->col(j);
			float euclideanDistance = 0;
			for (int k = 0; k < coefNo; k++){
				euclideanDistance += (distance(k) * distance(k));
			}
			euclideanDistance = sqrt(euclideanDistance);
			if (euclideanDistance < euclideanDistance_min){
				matchedFace[i] = j;
				euclideanDistance_min = euclideanDistance;
			}
		}
		if (!strcmp(trainFace[matchedFace[i]].getID(), testFace[i].getID())){
			matchCounter++;
		}
	}

	float accuracy = (float)matchCounter / TESTNO;
	std::cout << "Accuracy: " << accuracy << endl;

	string file = "./Info/ClassificationResults.txt";
	ofstream ff(file.c_str());
	if (!ff) {
		std::cout << "Cannot create " << file << endl;
	}
	ff << "Face" << endl;
	for (int i = 0; i < TESTNO; i++) {
		ff << "Test face " << i << " :\t" << testFace[i].getID() << "\t" << trainFace[matchedFace[i]].getID() << endl;
	}
	ff.close();

	delete matchedFace;
	return accuracy;
}


int main()
{
	//************************setting of camera*********************************************************
	//**choose the camera module(-1) or the webcam(0)												  **
	//**if can't switch between cammodule/webcam    use the folling command:sudo modprobe bcm2835-v4l2**
	//**************************************************************************************************
	VideoCapture cap(-1);			
	cap.set(CV_CAP_PROP_FPS,30);
	cap.set(CV_CAP_PROP_FRAME_WIDTH,320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
	if(!cap.isOpened())
		printf("Cannot open the video file\n");
	
	//
 	vector<Rect> faces1;
	int oldX,oldY;
	int maxW=0;// using the widest width to be the face.
	int maxH=0;
	int xFace;//center of face.
	int yFace;
	Mat image1;
	Mat image_roi;
	Mat frame;
	CvRect Rect;
	int xFace_OLD=0,yFace_OLD=0;
	
	int roix,roiy,roiw,roih;

	
	//time duration accounting.
	int time_s=0;				//start
	int time_e=0;				//end
	int time_no_show=0;		
	int time_det=0;
	
	//counter
	int frame_det=0,frame_process=0;
	
	
	//flag
	int The_Last_Time_Found=0;
	
	//cascader
  	CascadeClassifier face_cascade1; 
	face_cascade1.load("haarcascade_frontalface_alt.xml");
	//face_cascade1.load("lbpcascade_frontalface.xml"); //choice of model.
	
	
	//camera module specific variable.
	cv::Mat gray;
	vector<string> codes;
	Mat corners;	
	bool bSuccess1;
	for(;;)
	{

		//Time Counting Start
		time_s=micros();
		
		//Input Frame
		bSuccess1=cap.read(frame);
		if(!bSuccess1)
			cout<<"cannot read the frame from video file"<< endl;			
		
		//Setting ROI
		if(The_Last_Time_Found)
		{
			roix=xFace-MIN_((maxW>>1),xFace);
			roiy=yFace-MIN_((maxH>>1),yFace);
			roiw=MIN_((maxW>>1)+maxW,Width-xFace)-1+MIN_((maxW>>1),xFace);
			roih=MIN_((maxH>>1)+maxH,Height-yFace)-1+MIN_((maxH>>1),yFace);
			image_roi=frame(cvRect(roix,roiy,roiw,roih));				
			
		}
		//////////////////////////////Before Optimization
		//Rect=cvRect(xFace-MIN_(maxW,xFace),yFace-MIN_(maxH,yFace),MIN_(maxW,xFace)+(xFace>>1),MIN_(maxH,yFace)+(yFace>>1));
		//image_roi=image_roi(Rect);
		
		//Classification.
		face_cascade1.detectMultiScale((The_Last_Time_Found)?image_roi:frame,faces1,1.1,1,0|CV_HAAR_SCALE_IMAGE,cv::Size(50,50),cv::Size(100,100));		//set the size of the target face 
		
		
		//Figuring out the target face, which is the biggest.
		maxW=0;
		maxH=0;
		for (int i =0;i<faces1.size();i++)
		{		
			if (faces1[i].width>maxW)
			{
				maxW=faces1[i].width;
				maxH=faces1[i].height;
				xFace=faces1[i].x;
				yFace=faces1[i].y;
			}
		}
	

		findDataMatrix(gray, codes, corners);
		
		if(maxW!=0) //Face is found.
		{	
			frame_det++;
			if(The_Last_Time_Found){ 
				xFace+=roix;
				yFace+=roiy;
				//xFace=(xFace+xFace_OLD)>>1;//moving with 2.
				//yFace=(yFace+yFace_OLD)>>1;
			}
			//time_e=micros();
			//time_no_show+=(time_e-time_s)/1000;
				
			Rect=cvRect(xFace,yFace,maxW,maxH);
			image_roi=image_roi(Rect);
			Point leftEye,rightEye;
			detectAndRecognize(image_roi,leftEye,rightEye);
			cout<<a->x<<" "<<a.y<<","<<b.x<<" "<<b.y<<endl;


 			(The_Last_Time_Found)?rectangle( frame, Point(xFace,yFace),Point(xFace+maxW,yFace+maxH), cvScalar(255,64,64), 1,8 ):rectangle( frame, Point(xFace,yFace),Point(xFace+maxW,yFace+maxH), cvScalar(0,255,255), 1,8 );
			
			xFace_OLD=xFace;
			yFace_OLD=yFace;
			The_Last_Time_Found=1;

			
		}else{
			The_Last_Time_Found=0;
		}
		
		
			
		cv::imshow("test",frame);	
		waitKey(2);								
		time_e=micros();
		time_det+=(time_e-time_s)/1000.0;
		//delay N millis, usually long enough to display and capture input
		printf("\nafter detection time=%d\ncorrect=%d %d",time_det,frame_det*100/frame_process,frame_process);

		



		int TRAINNO = 20;
		int TESTNO = 20;

		// PCA + Euclidean Distance Calculation
		float accuracy[20];
		for (int i = 0; i < 20; i++){		//max: i < TRAINNO - 1
			//Get the low-dimenstional face images
			MatrixXf *trainCoef = new MatrixXf(TRAINNO - 1 - i, TRAINNO);
			MatrixXf *testCoef = new MatrixXf(TRAINNO - 1 - i, TESTNO);
			FR_onlyPCA(TRAINNO, TESTNO, trainFace, testFace, i, true, trainCoef, testCoef);
			accuracy[i] = FR_euclideanDistance(TRAINNO, TESTNO,trainFace, testFace, trainCoef, testCoef);
			delete trainCoef, testCoef;
		}
		string file = "./Info/Accuracy.txt";
		ofstream ff(file.c_str());
		if (!ff) {
			std::cout << "Cannot create " << file << endl;
		}
		ff << "Accuracy List" << endl;
		for (int i = 0; i < 20; i++){		//max: i < TRAINNO - 1
			ff << "delete eigen no: " << i << "       accuracy: " << accuracy[i] << endl;
		}
		ff.close();


	}//while			 		 
	
	return 0;	
}

