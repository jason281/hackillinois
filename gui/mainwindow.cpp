#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "train.cpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow){
	ui->setupUi(this);
	video = new workerThread(this);
	analyze = new analyzeThread(this);
	meanFace = new float[WIDTH*HEIGHT];
	connect(ui->connect_button, SIGNAL(clicked()), this, SLOT(connect_cam()));
	connect(ui->capture_button, SIGNAL(clicked()), video, SLOT(start()));
	connect(ui->stop_button,    SIGNAL(clicked()), video, SLOT(terminate()));
	connect(ui->capture_button, SIGNAL(clicked()), analyze, SLOT(start()));
	connect(ui->stop_button,    SIGNAL(clicked()), analyze, SLOT(terminate()));
}

MainWindow::~MainWindow(){
	delete ui;
}
void MainWindow::resizeEvent(QResizeEvent* event){
	QMainWindow::resizeEvent(event);
}

void MainWindow::connect_cam(){
	camera.open(0);
    if(!camera.isOpened()){
        statusBar()->showMessage("camera error");
    }
    else{
        camera.set( CV_CAP_PROP_FRAME_WIDTH,640);
        camera.set( CV_CAP_PROP_FRAME_HEIGHT,480);
    }
}

void workerThread::run(){
	cv::Mat framesmall;
	while(1){
		if(!mw->camera.read(mw->frame)){
			mw->statusBar()->showMessage("camera error");
			return;
		}
		if(mw->rect_face.x!=0)
 		{
 			cv::rectangle(mw->frame,cv::Point(mw->rect_face.x,mw->rect_face.y),cv::Point(mw->rect_face.x+mw->rect_face.width,mw->rect_face.y+mw->rect_face.height), cvScalar(255,64,64), 1,8);
 		}
		cv::resize(mw->frame,framesmall,cv::Size(640,480),CV_INTER_AREA);
		cv::cvtColor(framesmall,framesmall,cv::COLOR_BGR2RGB);
		mw->ui->monitor->setPixmap(QPixmap::fromImage(QImage((const uchar*)framesmall.data, framesmall.cols, framesmall.rows, QImage::Format_RGB888)));
		usleep(16667);
	}
};

void analyzeThread::run(){
	while(1){
		Face tmp;
		if(tmp.faceNormalization(mw->frame)){
			mw->rect_face=cv::Rect(0,0,0,0);
			float distance=100;
			cout<<"=========test==========\n";
			PCA_testing(mw->f, &tmp, mw->trainCoef, mw->eigenFace, mw->meanFace, distance);
			cout<<"=========HERE==========\n";
			if(distance>50){
				std::cout<<"==========train==========\n";
				mw->f.push_back(tmp);
				mw->trainCoef = Eigen::MatrixXf(mw->f.size() - 1, mw->f.size() );
				mw->eigenFace = Eigen::MatrixXf(WIDTH*HEIGHT, mw->f.size() - 1);
				PCA_training(mw->f, mw->trainCoef, mw->eigenFace, mw->meanFace);
			}
			cout<<"=========HERE==========\n";
			sleep(1);
		}
	}
};
