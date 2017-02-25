#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QtCore>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QDateTime>
#include "face.h"

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
	Face f;
private slots:
	void connect_cam();
	void ana();
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
