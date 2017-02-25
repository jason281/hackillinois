#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "train.cpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow){
	ui->setupUi(this);
	video = new workerThread(this);
	connect(ui->connect_button, SIGNAL(clicked()), this, SLOT(connect_cam()));
	connect(ui->capture_button, SIGNAL(clicked()), video, SLOT(start()));
	connect(ui->stop_button,    SIGNAL(clicked()), this, SLOT(ana()));
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
void MainWindow::ana(){
	imshow(std::string("Origin"), frame);
	char keypress = waitKey(40);
	f.faceNormalization(frame);
	char c[10]="abcd";
	f.setID(c);
	f.show("Show");
	f.save("face.bmp");
	f.savedata2D("face.txt");
}

void workerThread::run(){
	cv::Mat framesmall;
	while(1){
		if(!mw->camera.read(mw->frame)){
			mw->statusBar()->showMessage("camera error");
			return;
		}
		cv::resize(mw->frame,framesmall,cv::Size(640,480),CV_INTER_AREA);
		cv::cvtColor(framesmall,framesmall,cv::COLOR_BGR2RGB);
		mw->ui->monitor->setPixmap(QPixmap::fromImage(QImage((const uchar*)framesmall.data, framesmall.cols, framesmall.rows, QImage::Format_RGB888)));
		usleep(16667);
	}
};
