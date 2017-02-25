#include "face.h"
#include <Eigen/Dense>
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;
using namespace Eigen;

void PCA_training(vector<Face> trainFace, MatrixXf & trainCoef, MatrixXf & eigenFace, float* meanFace){
	int TRAINNO=trainFace.size();
	//dimension of trainCoef is (TRAINNO-1,TRAINNO), dimension of trainCoef is (TRAINNO-1,TESTNO)
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

	trainCoef = eigenVector.transpose() * trainingset;
	eigenFace = eigenVector;
	delete demeanedFace;
}

int PCA_testing(vector<Face> trainFace, Face* testFace, MatrixXf & trainCoef, MatrixXf & eigenFace, float* meanFace, float& euclideanDistance_min){

	int TRAINNO = trainFace.size();
	int coefNo = TRAINNO-1;
	MatrixXf testingset(WIDTH*HEIGHT, 1);
	for (int j = 0; j < HEIGHT * WIDTH; j++){
		testingset(j, 1) = testFace->getdata()[j] - meanFace[j];
	}
	MatrixXf testCoef(coefNo, 1);
	testCoef = (eigenFace.transpose() * testingset);

	//Calcuate the distance
	int matchedFace;
	bool correct = 0;

	euclideanDistance_min = 1000000000;
	for (int j = 0; j < TRAINNO; j++){
		VectorXf distance(coefNo);
		distance = testCoef - trainCoef.col(j);
		float euclideanDistance = 0;
		for (int k = 0; k < coefNo; k++){
			euclideanDistance += (distance(k) * distance(k));
		}
		euclideanDistance = sqrt(euclideanDistance);
		if (euclideanDistance < euclideanDistance_min){
			matchedFace = j;
			euclideanDistance_min = euclideanDistance;
		}
	}
	if (!strcmp(trainFace[matchedFace].getID(), testFace->getID())){
		correct = true;
	}

	cout << "Test face  :\t " << testFace->getID() << "\t" << trainFace[matchedFace].getID() << "\t Distance:\t" << euclideanDistance_min <<endl;

	return matchedFace;
}
