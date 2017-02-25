#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QDateTime>

namespace Ui{
	class MainWindow;
}

class MainWindow : public QMainWindow{
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
private:
	Ui::MainWindow* ui;
	cv::VideoCapture camera;
	cv::Mat frame;
	
};



#endif