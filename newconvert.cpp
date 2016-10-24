#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;
using namespace cv;

int convertpancake(string);
Vec3b pancake(int,int,int);
string pancake2(int,int,int);
int solvemins(int,int);
Mat group(Mat);
Mat bright(Mat);
Mat cont(Mat);
void savecode(string, string);

int main(int argc,char *argv[]){
	cout<<argv[1]<<endl;
	convertpancake(argv[1]);
	return 0;
}

int convertpancake(string filename){
	VideoCapture cap(filename);
	if(!cap.isOpened()){
		return -1;
	}

	namedWindow("gochiusa",WINDOW_AUTOSIZE);

	Mat frame, frame_gb;
	
	int framenum = 0; //frame number
	int count = 0;
	for(;;){
		cap >> frame;
		int x0 = 0; //left up in 8*8 block (x0,y0)
		int y0 = 0;
		int linenum = 10; //line number
		string ichigo = "NEW\n\n"; //ichigojam's program

		resize(frame, frame, Size(80,40), INTER_CUBIC);
		frame = cont(frame);
		while(x0 < 80 && y0 < 40){
			ichigo += to_string(linenum) + " ?\"PC STAMP HEX$(#" + to_string(x0) + ") HEX$(#" + to_string(y0) + ") 00 ";
			linenum += 10;
			for(int i=y0;i<y0+8;i++){
				Vec3b* src = frame.ptr<Vec3b>(i);
				for(int j=x0;j<x0+8;j++){
					Vec3b bgr = src[j];
					src[j] = pancake(bgr[0],bgr[1],bgr[2]);
					ichigo += pancake2(bgr[0],bgr[1],bgr[2]);
				}
			}
			ichigo += "\n";
			if(x0 != 72) x0 += 8;
			else{
				x0 = 0;
				y0 += 8;
			}
		}
		ichigo += "\nsave "+to_string(framenum+4)+"\n`Download\n?\"MJ GETS gyosib.github.io/"+to_string(framenum+1)+".txt";
		resize(frame, frame, Size(), 10, 10);
		if(frame.empty()) continue;
		imshow("gochiusa",frame);
		if(waitKey(30) >=0) break;
		if(count % 30 == 0){
			savecode("out/"+to_string(framenum)+".txt", ichigo);
			cout << ichigo << endl;
			framenum++;
		}
		count++;
	}
	return 0;
}

Mat cont(Mat img){
	Mat result;

	float a = 15.0;
	uchar lut[256];
	for(int i=0;i<256;i++)
		lut[i] = 255.0/(1+exp(-a*(i-128)/255));

	/*Mat p = original.reshape(0,1).clone();
	  for(int i=0;i<p.cols;i++){
	  p.at<uchar>(0,i) = lut[p.at<uchar>(0,i)];
	  }*/
	LUT(img,Mat(Size(256,1),CV_8U,lut),result);

	return result;
}

Vec3b pancake(int b,int g,int r){
	int color[16][3] = {
		{0,0,0},{255,255,255},{41,27,227},{139,104,255},
		{48,126,242},{61,203,255},{169,222,255},{43,74,107},
		{24,202,145},{55,77,28},{215,175,67},{208,74,38},
		{102,36,26},{187,55,99},{171,63,178},{204,204,204}
	};

	double mindis = 999; //distance in closest color
	int minnum = 0;

	for(int i=sizeof(color)/sizeof(color[0]);i>0;i--){
		int db = solvemins(color[i][0],b);
		int dg = solvemins(color[i][1],g);
		int dr = solvemins(color[i][2],r);
		double tmpdis = sqrt(db*db+dg*dg+dr*dr);
		if(tmpdis < mindis){
			mindis = tmpdis;
			minnum = i;
		}
	}

	return Vec3b(color[minnum][0],color[minnum][1],color[minnum][2]);
	//return Vec3b(color[10][0],color[10][1],color[10][2]);
}

string pancake2(int b,int g,int r){
	int color[16][3] = {
		{0,0,0},{255,255,255},{41,27,227},{139,104,255},
		{48,126,242},{61,203,255},{169,222,255},{43,74,107},
		{24,202,145},{55,77,28},{215,175,67},{208,74,38},
		{102,36,26},{187,55,99},{171,63,178},{204,204,204}
	};

	double mindis = 999; //distance in closest color
	int minnum = 0;

	for(int i=sizeof(color)/sizeof(color[0]);i>0;i--){
		int db = solvemins(color[i][0],b);
		int dg = solvemins(color[i][1],g);
		int dr = solvemins(color[i][2],r);
		double tmpdis = sqrt(db*db+dg*dg+dr*dr);
		if(tmpdis < mindis){
			mindis = tmpdis;
			minnum = i;
		}
	}

	switch(minnum){
		case 10:
			return "A";
			break;
		case 11:
			return "B";
			break;
		case 12:
			return "C";
			break;
		case 13:
			return "D";
			break;
		case 14:
			return "E";
			break;
		case 15:
			return "F";
			break;
	}

	return to_string(minnum);
	//return Vec3b(color[10][0],color[10][1],color[10][2]);
}

int solvemins(int l, int r){
	if(l > r){
		return l-r;
	}else{
		return r-l;
	}
	return 0;
}

void savecode(string filename, string code){
	ofstream writing_file;
	writing_file.open(filename, ios::out);
	writing_file << code << endl;
}
