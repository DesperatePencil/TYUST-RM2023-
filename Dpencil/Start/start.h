#ifndef START_H
#define START_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/ml/ml.hpp>

#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */
#include <errno.h>   /* ERROR Number Definitions           */
#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include <sys/ioctl.h>
#include <mutex>

using namespace cv;
using namespace ml;//Ptr<SVM>
using namespace std;

class Start
{
public:
//    Start();

    //录视频
    std::string videoRecord();
    void ImgProcess();
    void TargetProcess();
    void Recive();
    void Recive1();
    void Send();

    Mat img;
    Mat img1;
    Mat prepare_img;
    Mat prepare_img1;
    uint8_t pc_data[10];
    uint enemy_color = 0;
    int hero = 0;
     int engineer = 0;
     int infantry3 = 0;
     int infantry4 = 0;
     int infantry5 = 0;
     int qianshao = 0;
//     int huo[7];
    Rect roi_rect;
    float mode;
    float TY;
    float TP;
    float TYv;
    float TPv;
};
#endif // START_H
