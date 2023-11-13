#ifndef SOLVER_H
#define SOLVER_H
#include "Start/start.h"
#include "Communication/serial.h"
#include "Armor/Armor.h"

//09相机的畸变和角度结算   左
class AngleSolver
{
public:
    void SolveAngle(UsbSerial &usb_serial,double width,double height,ArmorBag &targetArmor);              //结算角度
    void Compensate();  //补偿
    void CompensateOffset();  //补偿（枪管和相机之间高度差）
    void CompensateGravity();  //重力补偿
    void P4P();              //P4P结算
    void PinHole();            //小孔成像r
    void SolveDataProcess();       //处理信息准备发送       未写
    double y_yaw = 0.0;
    double x_pitch = 0.0;
    double distance = 0.0;

private:
    Mat cam_matrix = (Mat_<double>(3,3)<<2114.080770, 0, 623.726144,  0,   2114.077122,   531.609282, 0, 0, 1);//1981.3, -6.2, 684, 0, 2006.7, 504, 0, 0, 1
    Mat distortion_coeff = (Mat_<double>(5,1)<<-0.120982, 0.266626, 0,0,-0.352917);//-0.1029, 0.0058, -0.0030, 0.0047,0
    std :: vector<cv::Point3f> Small_Armor_Points_3d;
    std :: vector<cv::Point3f> Big_Armor_Points_3d;
};

//01相机的畸变和角度结算    右
class AngleSolver1
{
public:
    void SolveAngle(UsbSerial1 &usb_serial1,double width,double height,ArmorBag &targetArmor);              //结算角度
    void Compensate();  //补偿
    void CompensateOffset();  //补偿（枪管和相机之间高度差）
    void CompensateGravity();  //重力补偿
    void P4P();              //P4P结算
    void PinHole();            //小孔成像r
    void SolveDataProcess();       //处理信息准备发送       未写
    double y_yaw = 0.0;
    double x_pitch = 0.0;
    double distance = 0.0;

private:
    Mat cam_matrix = (Mat_<double>(3,3)<<2120.288830, 0,  611.557532, 0,   2121.294642, 555.844902, 0, 0, 1);//1981.3, -6.2, 684, 0, 2006.7, 504, 0, 0, 1
    Mat distortion_coeff = (Mat_<double>(5,1)<<-0.182239, 1.471425, 0,0,-10.818255);//-0.1029, 0.0058, -0.0030, 0.0047,0
    std :: vector<cv::Point3f> Small_Armor_Points_3d;
    std :: vector<cv::Point3f> Big_Armor_Points_3d;

    double width=120;
    double height=85;
};

#endif // SOLVER_H
