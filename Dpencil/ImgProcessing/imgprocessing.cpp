#include "imgprocessing.h"

using namespace std;
using namespace cv;


void ImgProcessing::Deal(Mat &img)
{

//     enemycolor = BLUE;
//    enemycolor = RED;
    split(img,bgr_channels);           // 原来的图像处理
    switch (enemycolor)
    {
    case RED:
         inRange(img,Scalar(B_R,G_R,R_R),Scalar(255,255,255),bright_img);
          morphologyEx(bright_img,bright_img,MORPH_OPEN,element2);
        // imshow("bright_img",bright_img);
        subtract(bgr_channels[2],(min_r*0.001)*bgr_channels[1],sub_img);
        threshold(sub_img,sub_img,min_R,255,THRESH_BINARY);
        dilate(sub_img,sub_img,element);
        final_img=sub_img & bright_img;
//        imshow("red_img",sub_img);
        break;
    case BLUE:
         inRange(img,Scalar(B_B,G_B,R_B),Scalar(255,255,255),bright_img);
          morphologyEx(bright_img,bright_img,MORPH_OPEN,element2);
        subtract(bgr_channels[0],(min_b*0.001)*bgr_channels[1],sub_img);
        threshold(sub_img,sub_img,min_B,255,THRESH_BINARY);
        dilate(sub_img,sub_img,element);
        final_img=sub_img & bright_img;
        break;
    }
}


