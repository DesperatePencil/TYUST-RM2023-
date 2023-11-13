#ifndef ARMOR_H
#define ARMOR_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/ml/ml.hpp>
#include "opencv2/highgui/highgui_c.h"
#include "Info/Info.h"
#include <fuhuo.h>

using namespace cv;
using namespace ml;//Ptr<SVM>
using namespace std;

/*
 *@brief: 装甲板两侧灯条的相关信息
 */
class LightBag
{
public:

    LightBag();
    LightBag(const RotatedRect& light);
    ~LightBag();

public:
    RotatedRect lightRect; //灯条的旋转矩形（椭圆拟合获得）
    float length;  //灯条长度
    Point2f center; //灯条中心
    float angle;  //灯条长度方向与竖直方向的夹角，左偏为0~90,右偏为0~-90
};


/*
 *@brief: 装甲板相关数据信息
 */
class ArmorBag
{
public:
    ArmorBag();
    ArmorBag(Mat &img,const LightBag& l_light, const LightBag& r_light);
    ~ArmorBag();

    // 灯条平行度——装甲板左右灯条角度差
    float lightAngleDiff() const;
    // 灯条错位度角
    float getDeviationAngle() const;
    // 灯条距离与灯条长度比——距离比/两灯条长度平均值
    float lengthDistanceProportion() const;
    // 左右灯条长度差比值
    float lengthDiffProportion() const;
    //灯条位置差距 两灯条中心x方向差距比值
    float getDislocationX() const;
    //灯条位置差距 两灯条中心Y方向差距比值
    float getDislocationY() const;
    //左右灯条长度差比值
    float getLengthRation() const;

    float lightCenterDeviation() const;// 中心连线水平度——两灯条中心连线与水平线夹角

    //初步判断是否是装甲板
    bool SuitableArmor() const;

    bool fuhuo(FuHuo &fuhuo,ArmorBag &armor) const;
    //获取装甲板图片
    void getWrap(Mat &img, Point2f vertices[4],int &middle);

    //gramma 矫正
    void gamma_correction(Mat &src, Mat &img, float K);

    //针对游离灯条导致的错误装甲板进行检测和删除
    void eraseErrorRepeatArmor(vector<ArmorBag> & armors);



public:
    int l_index, r_index; //the index of left and right light 左右灯条的下标(默认为-1，仅作为ArmorDetector类成员时生效)
    LightBag l_light, r_light;         //装甲板的左右灯条
    vector <Point2f> light_Vertices;   //装甲板灯条四个顶点 左上 右上 右下 左下
    Point2f armor_Vertices[4];         //装甲板图案四个顶点 左上 右上 右下 左下
    Point2f armor_center;              //装甲板中心
    ArmorSize armor_size;              //装甲板大小类型
    RobotType robotType;               //判断机器人类型（用SVM识别装甲板图案得到
      
//    ROI roi;                           //创建ROI
    float center_diff;                 //装甲板中心与图像中心距离
    float armor_area;                  //装甲板面积
    float armorAngle;                  //装甲板角度(灯条角度的平均值)
    int l_num, r_num;                  //左右灯条在所有已经找到的灯条中的下标
    Rect armor_roi;                    //装甲板的矩形获取roi用
    Mat armor_warp;                    //装甲板的图片（透视变换获得）
    int score;
//===========================直方图处理========================//
    Mat equalImg;
//=============================================================//
};

class MatchSVM
{
public:
   bool picMatch(ArmorBag &armor);

   Ptr<ml::SVM>svm = ml::SVM::load("/home/newmaker-08/桌面/sentry_svm.xml");
};
/*
 *@brief: 装甲板识别类
 *装甲板两侧灯条的检测，装甲板的灯条匹配，装甲板的条件筛选，选择目标等功能
 */
class ArmorDetector
{
public:
    ArmorDetector();
    ~ArmorDetector();

    void detect(Mat &img, Mat &final_img,int &middle,FuHuo &fuhuo);     //检测流程
    void findLights(Mat &img, Mat &final_img);             //寻找所有灯条
    void matchArmor(Mat &img,int &middle,FuHuo &fuhuo);                   //匹配所有装甲板
    void priority();                               //目标击打优先级
    void drew(Mat &img,vector<ArmorBag> &armors);//画出未用svm筛选的所有找到的装甲板，检查用
    //void specialUI(cv::Mat img);     //狙击镜



//private:

    Mat final_img;         //二值处理后的当前源图像，用this->让形参赋值
    vector<LightBag> lights; //找到的所有灯条
    vector<ArmorBag> armors; //识别到的所有装甲板
    ArmorBag targetArmor;    //当前图像帧对应的目标装甲板
    ArmorBag lastArmor;      //上一帧图像的目标装甲板
    FindResult findResult;   //装甲板检测器的状态，随着装甲板进程的执行而不断更新
    uint T;                  //掉帧保护
    uint T_max=5;            //掉帧容错数--最大丢失目标帧数
    MatchSVM svm;            //创建svm
};


void OutLights(vector<LightBag> & lights);
void OutArmors(vector<ArmorBag> & armors);

#endif // ARMOR_H
