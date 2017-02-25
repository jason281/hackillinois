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

// Load the face and 1 or 2 eye detection XML classifiers.
void Face::initDetectors(CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2)
{
	// Cascade Classifier file, used for Face Detection.
	const char *faceCascadeFilename = "lbpcascade_frontalface.xml";     // LBP face detector.
	//const char *faceCascadeFilename = "haarcascade_frontalface_alt_tree.xml";  // Haar face detector.
	//const char *eyeCascadeFilename1 = "haarcascade_lefteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
	//const char *eyeCascadeFilename2 = "haarcascade_righteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
	//const char *eyeCascadeFilename1 = "haarcascade_mcs_lefteye.xml";       // Good eye detector for open-or-closed eyes.
	//const char *eyeCascadeFilename2 = "haarcascade_mcs_righteye.xml";       // Good eye detector for open-or-closed eyes.
	const char *eyeCascadeFilename1 = "haarcascade_eye.xml";               // Basic eye detector for open eyes only.
	const char *eyeCascadeFilename2 = "haarcascade_eye_tree_eyeglasses.xml"; // Basic eye detector for open eyes if they might wear glasses.

	// Load the Face Detection cascade classifier xml file.
	try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
		faceCascade.load(faceCascadeFilename);
	}
	catch (cv::Exception &e) {}
	if (faceCascade.empty()) {
		cerr << "ERROR: Could not load Face Detection cascade classifier [" << faceCascadeFilename << "]!" << endl;
		cerr << "Copy the file from your OpenCV data folder (eg: 'C:\\OpenCV\\data\\lbpcascades') into this WebcamFaceRec folder." << endl;
		exit(1);
	}
	//cout << "Loaded the Face Detection cascade classifier [" << faceCascadeFilename << "]." << endl;

	// Load the Eye Detection cascade classifier xml file.
	try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
		eyeCascade1.load(eyeCascadeFilename1);
	}
	catch (cv::Exception &e) {}
	if (eyeCascade1.empty()) {
		cerr << "ERROR: Could not load 1st Eye Detection cascade classifier [" << eyeCascadeFilename1 << "]!" << endl;
		cerr << "Copy the file from your OpenCV data folder (eg: 'C:\\OpenCV\\data\\haarcascades') into this WebcamFaceRec folder." << endl;
		exit(1);
	}
	//cout << "Loaded the 1st Eye Detection cascade classifier [" << eyeCascadeFilename1 << "]." << endl;

	// Load the Eye Detection cascade classifier xml file.
	try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
		eyeCascade2.load(eyeCascadeFilename2);
	}
	catch (cv::Exception &e) {}
	if (eyeCascade2.empty()) {
		cerr << "Could not load 2nd Eye Detection cascade classifier [" << eyeCascadeFilename2 << "]." << endl;
		// Dont exit if the 2nd eye detector did not load, because we have the 1st eye detector at least.
		//exit(1);
	}
	//else
	//	cout << "Loaded the 2nd Eye Detection cascade classifier [" << eyeCascadeFilename2 << "]." << endl;
}

// crop and resize the image in order to make the face image have the same size and the same location of eyes
void Face::detectAndRecognize(Mat inputFace, Point* originalLeftEye, Point* originalRightEye)
{
	CascadeClassifier faceCascade;
	CascadeClassifier eyeCascade1;
	CascadeClassifier eyeCascade2;

	Ptr<FaceRecognizer> model;
	vector<Mat> preprocessedFaces;
	vector<int> faceLabels;
	Mat old_prepreprocessedFace;
	double old_time = 0;

	// Load the face and 1 or 2 eye detection XML classifiers.
	initDetectors(faceCascade, eyeCascade1, eyeCascade2);

	// Find a face and preprocess it to have a standard size and contrast & brightness.
	Rect faceRect;  // Position of detected face.
	Rect searchedLeftEye, searchedRightEye; // top-left and top-right regions of the face, where eyes were searched.
	Point leftEye, rightEye;
	// Set the desired face dimensions. Note that "getPreprocessedFace()" will return a square face.
	int faceWidth = 70;
	int faceHeight = faceWidth;
	bool preprocessLeftAndRightSeparately = true;   // Preprocess left & right sides of the face separately, in case there is stronger light on one side.
	Mat preprocessedFace = getPreprocessedFace(inputFace, faceWidth, faceCascade, eyeCascade1, eyeCascade2, preprocessLeftAndRightSeparately, &faceRect, &leftEye, &rightEye, &searchedLeftEye, &searchedRightEye);
	*originalLeftEye = Point(faceRect.x + leftEye.x, faceRect.y + leftEye.y);
	*originalRightEye = Point(faceRect.x + rightEye.x, faceRect.y + rightEye.y);
	cout << "Position of left eye:" << originalLeftEye->x << "," << originalLeftEye->y << endl;
	cout << "Position of right eye:" << originalRightEye->x << "," << originalRightEye->y << endl;

	bool gotFaceAndEyes = false;
	if (preprocessedFace.data)
		gotFaceAndEyes = true;

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
