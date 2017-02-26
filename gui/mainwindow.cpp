#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "train.cpp"

using namespace dlib;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow){
	ui->setupUi(this);
	users[0]=ui->user_1 ;
	users[1]=ui->user_2 ;
	users[2]=ui->user_3 ;
	users[3]=ui->user_4 ;
	users[4]=ui->user_5 ;
	users[5]=ui->user_6 ;
	users[6]=ui->user_7 ;
	users[7]=ui->user_8 ;
	users[8]=ui->user_9 ;
	users[9]=ui->user_10;
	video = new workerThread(this);
	analyze = new analyzeThread(this);
	meanFace = new float[WIDTH*HEIGHT];
	connect(ui->connect_button, SIGNAL(clicked()), this, SLOT(connect_cam()));
	connect(ui->capture_button, SIGNAL(clicked()), video, SLOT(start()));
	connect(ui->stop_button,    SIGNAL(clicked()), video, SLOT(terminate()));
	connect(ui->capture_button, SIGNAL(clicked()), analyze, SLOT(start()));
	connect(ui->stop_button,    SIGNAL(clicked()), analyze, SLOT(terminate()));
	
	string LandMarkModelName = "shape_predictor_68_face_landmarks.dat";
	deserialize(LandMarkModelName.c_str()) >> pose_model;
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
	camera.set(CV_CAP_PROP_FPS,30);
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
	
			cv::circle(mw->frame,mw->Leye,3,CV_RGB(255,0,0),CV_AA);
			cv::circle(mw->frame,mw->Reye,3,CV_RGB(255,0,0),CV_AA);
 		}
		cv::resize(mw->frame,framesmall,cv::Size(640,480),CV_INTER_AREA);
		cv::cvtColor(framesmall,framesmall,cv::COLOR_BGR2RGB);
		mw->ui->monitor->setPixmap(QPixmap::fromImage(QImage((const uchar*)framesmall.data, framesmall.cols, framesmall.rows, QImage::Format_RGB888)));
		usleep(16667);
	}
};

cv::Point analyzeThread::calcEye(dlib::full_object_detection shape,int start, int end){
	//start end: 
	//left eye: 	36 41
	//right:  	42 47

	int x_mean=0,y_mean=0;
	for (int i =start;i<=end;i++)
	{
		x_mean+=shape.part(i).x();
		y_mean+=shape.part(i).y();	
	}
	x_mean=cvRound((double)x_mean/6);
	y_mean=cvRound((double)y_mean/6);

	return Point(x_mean,y_mean);
}

void analyzeThread::run(){
	int count=0;
	
	while(1){
		Face* tmp = new Face;
		tmp->setPoseModel(&(mw->pose_model));
		if(!mw->frame.rows)
			continue;
		if(tmp->faceNormalization(mw->frame)){
			mw->rect_face=tmp->rect_face;
			cout <<"helo"<<endl;
			/*cv_image<bgr_pixel> cimg(mw->frame);
 			dlib::rectangle face(
 				tmp->rect_face.x,
 				tmp->rect_face.y,
 				tmp->rect_face.x + tmp->rect_face.width,
 				tmp->rect_face.y + tmp->rect_face.height);
 			full_object_detection shape;
 			shape = mw->pose_model(cimg, face);*/

			//mw->Leye=calcEye(shape,36,41);
			//mw->Reye=calcEye(shape,42,47);

			mw->Leye=tmp->Leye;
			mw->Reye=tmp->Reye;			

			float distance=3000;
			for(int i=0;i<10;i++)
				mw->users[i]->setStyleSheet(styleSheet());
			int id_user=PCA_testing(mw->f, tmp, mw->trainCoef, mw->eigenFace, mw->meanFace, distance);
			mw->users[id_user]->setStyleSheet("border: 20px solid red");
			std::cout<<count<<":\t"<<distance<<std::endl;
			if(distance>=1000)
				count++;
			else
				count = 0;
			if(count>10){
				count=0;
				std::cout<<"===========New User==========\n";
				cv::Mat framesmall;
				cv::resize(tmp->get_Mat(),framesmall,cv::Size(128,128),CV_INTER_AREA);
				cv::cvtColor(framesmall,framesmall,cv::COLOR_BGR2RGB);
				mw->users[mw->f.size()]->setPixmap(QPixmap::fromImage(QImage((const uchar*)framesmall.data, framesmall.cols, framesmall.rows, QImage::Format_RGB888)));
				mw->f.push_back(tmp);
				mw->trainCoef = Eigen::MatrixXf(mw->f.size() - 1, mw->f.size() );
				mw->eigenFace = Eigen::MatrixXf(WIDTH*HEIGHT, mw->f.size() - 1);
				PCA_training(mw->f, mw->trainCoef, mw->eigenFace, mw->meanFace);
			}
			sleep(1);
		}
	}
};
