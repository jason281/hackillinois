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

class MainWindow : public QMainWindow{
	Q_OBJECT
	friend class workerThread;
public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
	void resizeEvent(QResizeEvent*);
private:
	Ui::MainWindow* ui;
	cv::VideoCapture camera;
	cv::Mat frame;
	workerThread* video;
  Face f;
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

#endif
