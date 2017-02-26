#ifndef FACE_H_
#define FACE_H_

#define HEIGHT 128
#define WIDTH 128
#define IDsize 7

#include "../dlib/dlib/opencv.h"
#include "../dlib/dlib/image_processing/frontal_face_detector.h"
#include "../dlib/dlib/image_processing/render_face_detections.h"
#include "../dlib/dlib/image_processing.h"
#include "../dlib/dlib/gui_widgets.h"

//#include <vector>

// Include OpenCV's C++ Interface
#include <opencv2/opencv.hpp>
#include "preprocessFace.h"     // Easily preprocess face images, for face recognition.

using namespace cv;
using namespace std;

class Face {
public:
	Face();
	~Face();
	Face(float* faceData);
	void reset();
	void copyFace(Face* sourceFace);
	void initDetectors(CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2);
	bool detectAndRecognize(Mat inputFace, Point* originalLeftEye, Point* originalRightEye);
	bool faceNormalization(Mat inputFace);
	void basicNormalization();	//set mean to zero and set variance to one
	void show(string windowname);
	void save(string filename);
	void saveBright(string filename);
	void setID(char* ID);
	char* getID();
	float* getdata();
	float** getdata2D();
	bool savedata(string filename);
	bool savedata2D(string filename);
	Rect rect_face;
	Mat get_Mat(){return colorFace;}

	cv::Point Leye;
	cv::Point Reye;
	void setPoseModel(dlib::shape_predictor* a){pose_model=a;cout<<"setting end"<<endl;};
private:
	/** member functions **/

	/** member variables **/
	char faceID[IDsize];
	float* data;
	float** data2D;
	Mat colorFace;

	bool getEyePoints(Mat inputFace, Point* originalLeftEye, Point* originalRightEye);
	cv::CascadeClassifier face_cascade1;
	cv::Mat ROI;
	bool IsFindFace;
	void calcROI(Mat InputFace);
	dlib::shape_predictor* pose_model;//landmark model;
	
	cv::Point calcEye(dlib::full_object_detection& shape, int start, int end);
};

#endif /* FACE_H_ */
