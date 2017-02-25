#include "face.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <math.h>
using namespace std;

Face::Face() {
	this->data = new float[HEIGHT * WIDTH];
	this->data2D = new float*[WIDTH];
	for (int i = 0; i < WIDTH; i++){
		data2D[i] = new float[HEIGHT];
	}
}

Face::~Face(){
	for (int i = 0; i < WIDTH; i++){
		delete [] data2D[i];
	}
	delete [] data, data2D;
}

Face::Face(float* faceData) {
	this->data = new float[HEIGHT * WIDTH];
	this->data2D = new float*[WIDTH];
	for (int i = 0; i < WIDTH; i++){
		data2D[i] = new float[HEIGHT];
	}
	for (int i = 0; i < HEIGHT*WIDTH; i++){
		this->data[i] = faceData[i];
	}
	for (int i = 0; i < WIDTH; i++){
		for (int j = 0; j < HEIGHT; j++){
			this->data2D[i][j] = faceData[i*HEIGHT + j];
		}
	}
}

void Face::reset() {

}

void Face::copyFace(Face* sourceFace){
	for (int i = 0; i < IDsize; i++) {
		this->faceID[i] = sourceFace->faceID[i];
	}
	for (int i = 0; i < HEIGHT*WIDTH; i++){
		this->data[i] = sourceFace->data[i];
	}
	for (int i = 0; i < WIDTH; i++){
		for (int j = 0; j < HEIGHT; j++){
			this->data2D[i][j] = sourceFace->data2D[i][j];
		}
	}
}

void Face::faceNormalization(string filename, Point leftEye, Point rightEye){
	Mat inputFace;
	Mat normalizedFace;
	int origin_x = (leftEye.x + rightEye.x) / 2;
	int origin_y = (leftEye.y + rightEye.y) / 2;
	int angle = 180 * atan(((float)leftEye.y - (float)rightEye.y) / ((float)leftEye.x - (float)rightEye.x)) / 3.1415926;
	int eyeDistance = sqrt((leftEye.y - rightEye.y) * (leftEye.y - rightEye.y) + (leftEye.x - rightEye.x) * (leftEye.x - rightEye.x));

	//cout << "Do normalization for the input face..." << endl;

	//open an image and load it to the Mat inputFace
	try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
		inputFace = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);   // Read the file
	}
	catch (cv::Exception &e) {}
	if (!inputFace.data){                              // Check for invalid input
		cout << "Could not open or find the image" << endl;
		exit(1);
	}

	//Cut the face region
	//cout << "Angle: " << angle << endl;
	Point2f src_center(origin_x, origin_y);
	Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
	warpAffine(inputFace, normalizedFace, rot_mat, inputFace.size());
	normalizedFace = cv::Mat(normalizedFace, cv::Rect((origin_x - eyeDistance * 1), (origin_y - eyeDistance * 0.5), eyeDistance * 2, eyeDistance * 2));
	cv::resize(normalizedFace, normalizedFace, cv::Size(HEIGHT, WIDTH), 0, 0, CV_INTER_LINEAR);

	//// Show the camera frame on the screen.
	//imshow("face", normalizedFace);
	//// IMPORTANT: Wait for at least 20 milliseconds, so that the image can be displayed on the screen!
	//// Also checks if a key was pressed in the GUI window. Note that it should be a "char" to support Linux.
	//char keypress = waitKey(20);  // This is needed if you want to see anything!
	
	//float* p;
	//for (int i = 0; i < WIDTH; i++) {
	//	p = normalizedFace.ptr<float>(i);
	//	for (int j = 0; j < HEIGHT; j++) {
	//		this->data[i * HEIGHT + j] = p[j];
	//	}
	//}

	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		this->data[i] = (float)normalizedFace.data[i];
	}

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			this->data2D[i][j] = (float)normalizedFace.data[i * HEIGHT + j];
		}
	}
}

void Face::basicNormalization(){
	float sum = 0;
	float mean;
	float variance = 0;
	for (int i = 0; i < WIDTH*HEIGHT; i++){
		sum += this->data[i];
	}
	mean = sum / WIDTH*HEIGHT;
	for (int i = 0; i < WIDTH*HEIGHT; i++){
		this->data[i] -= mean;
		variance += this->data[i] * this->data[i];
	}
	variance /= WIDTH*HEIGHT;
	for (int i = 0; i < WIDTH*HEIGHT; i++){
		this->data[i] /= sqrt(variance);
	}
}

void Face::gammaCorrection(double gamma){
	for (int i = 0; i < HEIGHT*WIDTH; i++){
		this->data[i] = (float)(pow(((double)this->data[i] / 256), gamma) * 256);
	}
	for (int i = 0; i < WIDTH; i++){
		for (int j = 0; j < HEIGHT; j++){
			this->data2D[i][j] = this->data[i * HEIGHT + j];
		}
	}
}

void Face::DoG(float innerGaussian, float outerGaussian){
	float face[WIDTH*HEIGHT];
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		face[i] = this->data[i];
	}
	Mat face_mat = Mat(WIDTH, HEIGHT, CV_32FC1, face);
	Mat innerGauss, outerGauss = Mat(WIDTH, HEIGHT, CV_32FC1);
	float result[WIDTH*HEIGHT];
	GaussianBlur(face_mat, innerGauss, Size(), innerGaussian, innerGaussian);
	GaussianBlur(face_mat, outerGauss, Size(), outerGaussian, outerGaussian);
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		result[i] = outerGauss.at<float>(i) - innerGauss.at<float>(i);
		if (result[i] < 0){
			result[i] *= -1;
		}
	}

	//string filename = "DoG.txt";
	//ofstream ff(filename.c_str());
	//if (!ff) {
	//	std::cout << "Cannot create " << filename << endl;
	//}
	//ff << "Face" << endl;
	//for (int i = 0; i < HEIGHT * WIDTH; i++) {
	//	ff << (float)result[i] << " ";
	//}
	//ff.close();

	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		this->data[i] = result[i];
	}

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			this->data2D[i][j] = result[i * HEIGHT + j];
		}
	}
}

void Face::contrastEqualization(double alpha, double threshold){
	double sum = 0;
	double result[WIDTH * HEIGHT];
	for (int i = 0; i < WIDTH * HEIGHT; i++){
		if (this->data[i] < threshold){
			sum += pow((double)this->data[i], alpha);
		}
		else{
			sum += threshold;
		}
	}
	sum /= WIDTH * HEIGHT;
	cout << "Scalar:" << pow(sum, (double)1 / alpha) << endl;
	for (int i = 0; i < WIDTH * HEIGHT; i++){
		result[i] = (double)this->data[i] / pow(sum, (double)1 / alpha);
		result[i] = (tanh(result[i] / threshold)*threshold);// + threshold)/2/threshold*255;
	}
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		this->data[i] = (float)result[i];
	}
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			this->data2D[i][j] = this->data[i * HEIGHT + j];
		}
	}
}

void Face::show(string windowname){
	Mat showFace = Mat(HEIGHT, WIDTH, CV_32FC1, data);

	// Show the camera frame on the screen.
	imshow(windowname, showFace/255);
	// IMPORTANT: Wait for at least 20 milliseconds, so that the image can be displayed on the screen!
	// Also checks if a key was pressed in the GUI window. Note that it should be a "char" to support Linux.
	char keypress = waitKey(40);  // This is needed if you want to see anything!
}

void Face::save(string filename){
	Mat saveFace = Mat(HEIGHT, WIDTH, CV_32FC1, this->data);
	imwrite(filename, saveFace);
}

void Face::saveBright(string filename){
	Mat saveFace = Mat(HEIGHT, WIDTH, CV_32FC1, this->data);
	imwrite(filename, saveFace * 5);
}

void Face::setID(char* ID){
	for (int i = 0; i < IDsize; i++) {
		this->faceID[i] = ID[i];
	}
}

char* Face::getID(){
	return this->faceID;
}

bool Face::savedata(string filename){
	ofstream ff(filename.c_str());
	if (!ff) {
		std::cout << "Cannot create " << filename << endl;
		return false;
	}
	ff << "Face" << endl;
	for (int i = 0; i < HEIGHT * WIDTH; i++) {
		ff << this->data[i] << " ";
	}
	ff.close();
	return true;
}

bool Face::savedata2D(string filename){
	ofstream ff(filename.c_str());
	if (!ff) {
		std::cout << "Cannot create " << filename << endl;
		return false;
	}
	ff << "Face" << endl;
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			ff << this->data2D[i][j] << " ";
		}
		ff << endl;
	}
	ff.close();
	return true;
}

float* Face::getdata(){
	return this->data;
}

float** Face::getdata2D(){
	return this->data2D;
}
