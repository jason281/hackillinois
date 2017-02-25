#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QtCore>
#include <QLabel>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QDateTime>
#include <Eigen/Dense>
#include <vector>
#include "face.h"

#include "../dlib-19.0 (vc14)/dlib/opencv.h"
#include "../dlib-19.0 (vc14)/dlib/image_processing/frontal_face_detector.h"
#include "../dlib-19.0 (vc14)/dlib/image_processing/render_face_detections.h"
#include "../dlib-19.0 (vc14)/dlib/image_processing.h"
#include "../dlib-19.0 (vc14)/dlib/gui_widgets.h"

namespace Ui{
	class MainWindow;
}
class workerThread;
class analyzeThread;

class MainWindow : public QMainWindow{
	Q_OBJECT
	friend class workerThread;
	friend class analyzeThread;
public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
	void resizeEvent(QResizeEvent*);
private:
	Ui::MainWindow* ui;
	cv::VideoCapture camera;
	cv::Mat frame;
	workerThread* video;
	analyzeThread* analyze;
	std::vector<Face*> f;
	Eigen::MatrixXf trainCoef;
	Eigen::MatrixXf eigenFace;
	float*	meanFace;
	cv::Rect rect_face;
	QLabel* users[10];
	
	dlib::shape_predictor pose_model;//landmark model;
	
private slots:
	void connect_cam();
};

class workerThread : public QThread{
	Q_OBJECT
public:
	workerThread(MainWindow* mw_):mw(mw_){};
	void run();
private:
	MainWindow* mw;
};
class analyzeThread : public QThread{
	Q_OBJECT
public:
	analyzeThread(MainWindow* mw_):mw(mw_){};
	void run();
private:
	MainWindow* mw;
};
#endif
