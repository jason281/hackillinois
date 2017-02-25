#ifndef FACE_H_
#define FACE_H_

#define HEIGHT 128
#define WIDTH 128
#define IDsize 7

// Include OpenCV's C++ Interface
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Face {
public:
	Face();
	~Face();
	Face(float* faceData);
	void reset();
	void copyFace(Face* sourceFace);
	void faceNormalization(string filename, Point leftEye, Point rightEye);
	void basicNormalization();	//set mean to zero and set variance to one
	void gammaCorrection(double gamma);
	void DoG(float innerGaussian, float outerGaussian);
	void contrastEqualization(double alpha, double threshold);
	void show(string windowname);
	void save(string filename);
	void saveBright(string filename);
	void setID(char* ID);
	char* getID();
	float* getdata();
	float** getdata2D();
	bool savedata(string filename);
	bool savedata2D(string filename);

private:
	/** member functions **/

	/** member variables **/
	char faceID[IDsize];
	float* data;
	float** data2D;
	
};

#endif /* FACE_H_ */
