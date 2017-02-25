#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "train.cpp"

using namespace dlib;

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
	int count=0;
	while(1){
		Face* tmp = new Face;
		if(tmp->faceNormalization(mw->frame)){
			mw->rect_face=tmp->rect_face;

			cv_image<bgr_pixel> cimg(mw->frame);
 			dlib::rectangle face(
 				tmp->rect_face.x,
 				tmp->rect_face.y,
 				tmp->rect_face.x + tmp->rect_face.width,
 				tmp->rect_face.y + tmp->rect_face.height);
 			full_object_detection shape;
 			shape = pose_model(cimg, face);

			float distance=3000;
			PCA_testing(mw->f, tmp, mw->trainCoef, mw->eigenFace, mw->meanFace, distance);
			std::cout<<count<<":\t"<<distance<<std::endl;
			if(distance>=1000)
				count++;
			else
				count = 0;
			if(count>10){
				std::cout<<"===========New User========\n";
				mw->f.push_back(tmp);
				mw->trainCoef = Eigen::MatrixXf(mw->f.size() - 1, mw->f.size() );
				mw->eigenFace = Eigen::MatrixXf(WIDTH*HEIGHT, mw->f.size() - 1);
				PCA_training(mw->f, mw->trainCoef, mw->eigenFace, mw->meanFace);
			}
			sleep(1);
		}
	}
};
