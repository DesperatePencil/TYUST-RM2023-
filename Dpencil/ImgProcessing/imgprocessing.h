#ifndef IMGPROCESSING_H
#define IMGPROCESSING_H
#include "Info/Info.h"
#include "Start/start.h"
#include "Armor/Armor.h"

#define THRE_GRAY_RED 25		// 红色装甲板会灰度图阈值
#define THRE_GRAY_BLUE 70		// 蓝色装甲板会灰度图阈值
#define THRE_SEPARATION_RED 60	// 红色装甲板红蓝通道相减之后的阈值
#define THRE_SEPARATION_BLUE 90 // 蓝色装甲板红蓝通道相减之后的阈值

class ImgProcessing
{
public:
    ImgProcessing()
    {
        //    namedWindow("Control", WINDOW_AUTOSIZE);

//    cvCreateTrackbar("B_R", "Control", &B_R, 255);
//    cvCreateTrackbar("G_R", "Control", &G_R, 255);
//    cvCreateTrackbar("R_R", "Control", &R_R, 255);
//    cvCreateTrackbar("B_B", "Control", &B_B, 255);
//    cvCreateTrackbar("G_B", "Control", &G_B, 255);
//    cvCreateTrackbar("R_B", "Control", &R_B, 255);
////    cvCreateTrackbar("BB", "Control", &BB, 255);
////    cvCreateTrackbar("GG", "Control", &GG, 255);
////    cvCreateTrackbar("RR", "Control", &RR, 255);
//    cvCreateTrackbar("min", "Control", &min, 255);
//    cvCreateTrackbar("min_r", "Control", &min_r, 1000);
//    cvCreateTrackbar("min_b", "Control", &min_b, 1000);

//    cvCreateTrackbar("min_R", "Control", &min_R, 255);
//    cvCreateTrackbar("min_B", "Control", &min_B, 255);
//    cvCreateTrackbar("middle_l", "Control", &middle_l, 255);
//    cvCreateTrackbar("middle_r", "Control", &middle_r, 255);
        enemycolor=RED;
        attackpattern=ARMOR;

    }


    Mat final_img;
    Mat final_img1;
    EnemyColor enemycolor;//敌方装甲颜色
    AttackPattern attackpattern;//击打目标类别

    void Deal (Mat &img);
    void img_process(uchar color_sp, Mat src);    // 图片预处理


    vector<Mat> bgr_channels;//通道
    Mat color_img;//颜色二值化
    Mat sub_img;  //通道相减
    Mat gray;
    Mat gray_img; //灰度图

    Mat bright_img;
    Mat element  = getStructuringElement(MORPH_RECT, Size(13,13));
    Mat dilate_element1 = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
 Mat element2  = getStructuringElement(MORPH_RECT, Size(3,3));
    int min=130;//灰度图二值化参数 红：B=0  G=0  R=170  蓝： B=200  G=110  R=150
    int min_r=650;
    int min_b=600;
    int min_R = 174;
    int min_B = 130;
    int B_R = 20;
    int G_R = 100;
    int R_R = 20;
    int B_B = 20;
    int G_B = 100;
    int R_B = 20;

    int middle_l =21;
    int middle_r =40;
};

#endif // IMGPROCESSING_H
