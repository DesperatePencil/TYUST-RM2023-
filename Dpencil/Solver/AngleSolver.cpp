#include "Solver.h"

//09相机角度结算和畸变 右
void AngleSolver :: SolveAngle(UsbSerial &usb_serial,double width, double  height, ArmorBag &targetArmor)
{
    float small_x = 12.0 / 2.0;             //装甲板尺寸
    float small_y = 5.3 / 2.0;
    float big_x = 23.1 / 2.0;
    float big_y = 5.8 / 2.0;
    Mat r,tr,rot,trans,ttrans;
    vector <Point2f> ccc;
    Point2f a (targetArmor.armor_center.x-100,targetArmor.armor_center.y-50);
    Point2f b (targetArmor.armor_center.x+100,targetArmor.armor_center.y-50);
    Point2f c (targetArmor.armor_center.x+100,targetArmor.armor_center.y+50);
    Point2f d (targetArmor.armor_center.x-100,targetArmor.armor_center.y+50);
    ccc.emplace_back(a);ccc.emplace_back(b);ccc.emplace_back(c);ccc.emplace_back(d);

    switch (targetArmor.armor_size)
    {
    case SMALL_ARMOR :
        Small_Armor_Points_3d.push_back(Point3f(- small_x, small_y,0));         //特征点的世界坐标
        Small_Armor_Points_3d.push_back(Point3f(  small_x, small_y,0));
        Small_Armor_Points_3d.push_back(Point3f(  small_x, -small_y,0));
        Small_Armor_Points_3d.push_back(Point3f(- small_x, -small_y,0));
        solvePnP(Small_Armor_Points_3d , targetArmor.light_Vertices , cam_matrix , distortion_coeff , r , trans);
        //r  输出的旋转向量   trans 输出的平移矩阵                                                                                                     
        /*bool solvePnP(InputArray objectPoints, InputArray imagePoints, InputArray cameraMatrix, InputArray distCoeffs, OutputArray rvec, OutputArray tvec, bool useExtrinsicGuess = false, int flags = ITERATIVE)
        Small_Armor_Points_3d：特征点的世界坐标，坐标值需为float型，不能为double型，可以为mat类型，也可以直接输入vector<point3f>
         targetArmor.light_Vertices：特征点在图像中的像素坐标，可以输入mat类型，也可以直接输入vector<point3f>，注意输入点的顺序要与前面的特征点的世界坐标一一对应
         cam_matrix：相机内参矩阵
         distortion_coeff ：相机的畸变参数【Mat_<double>(5, 1)】
         r：输出的旋转向量
         trans：输出的平移矩阵*/
        Rodrigues(r, rot);                              //旋转向量转换旋转矩阵  应用罗德里格斯变换
        break;
    case BIG_ARMOR :
        Big_Armor_Points_3d.push_back(Point3f(- big_x, big_y,0));     //特征点的世界坐标
        Big_Armor_Points_3d.push_back(Point3f(  big_x, big_y,0));
        Big_Armor_Points_3d.push_back(Point3f(  big_x, -big_y,0));
        Big_Armor_Points_3d.push_back(Point3f(- big_x, -big_y,0));
        solvePnP(Big_Armor_Points_3d , targetArmor.light_Vertices , cam_matrix , distortion_coeff , r , trans);
        break;
    }
    Small_Armor_Points_3d.clear();//不及时清理会使数据异常
    Big_Armor_Points_3d.clear();

    double qx = trans.at<double>(0, 0);
    double qy = trans.at<double>(1, 0);
    double qz = trans.at<double>(2, 0);
    distance = sqrt(qx * qx + qy * qy + qz * qz);
    
    double tan_pitch = qy / sqrt(qx * qx + qz * qz);
    double tan_yaw = qx / qz;
    x_pitch = -atan(tan_pitch) * 180 / CV_PI;

    x_pitch += 0;       //手动给的补偿
    y_yaw += 0.9;

    VisionData   vdata = {{(float)y_yaw},{(float)x_pitch},{(float)distance},1,1,1,1};
    usb_serial.TransformData(vdata);
    usb_serial.send();
    //cout << "y_yaw  " << y_yaw << "   //x_pitch  " << x_pitch << "    //distance  " << distance << endl;
}


//01相机角度结算和畸变   左
void AngleSolver1 :: SolveAngle(UsbSerial1 &usb_serial1,double width, double  height, ArmorBag &targetArmor)
{
    float small_x = 12.0 / 2.0;
    float small_y = 5.3 / 2.0;
    float big_x = 23.1 / 2.0;
    float big_y = 5.8 / 2.0;
    Mat r,tr,rot,trans,ttrans;
    vector <Point2f> ccc;
    Point2f a (targetArmor.armor_center.x-100,targetArmor.armor_center.y-50);
    Point2f b (targetArmor.armor_center.x+100,targetArmor.armor_center.y-50);
    Point2f c (targetArmor.armor_center.x+100,targetArmor.armor_center.y+50);
    Point2f d (targetArmor.armor_center.x-100,targetArmor.armor_center.y+50);
    ccc.emplace_back(a);ccc.emplace_back(b);ccc.emplace_back(c);ccc.emplace_back(d);

    switch (targetArmor.armor_size)
    {
    case SMALL_ARMOR :
        Small_Armor_Points_3d.push_back(Point3f(- small_x, small_y,0));
        Small_Armor_Points_3d.push_back(Point3f(  small_x, small_y,0));
        Small_Armor_Points_3d.push_back(Point3f(  small_x, -small_y,0));
        Small_Armor_Points_3d.push_back(Point3f(- small_x, -small_y,0));     
        solvePnP(Small_Armor_Points_3d , targetArmor.light_Vertices , cam_matrix , distortion_coeff , r , trans);
        break;

    case BIG_ARMOR :
        Big_Armor_Points_3d.push_back(Point3f(- big_x, big_y,0));
        Big_Armor_Points_3d.push_back(Point3f(  big_x, big_y,0));
        Big_Armor_Points_3d.push_back(Point3f(  big_x, -big_y,0));
        Big_Armor_Points_3d.push_back(Point3f(- big_x, -big_y,0));
        solvePnP(Big_Armor_Points_3d , targetArmor.light_Vertices , cam_matrix , distortion_coeff , r , trans);
        break;
    }
    Small_Armor_Points_3d.clear();//不及时清理会使数据异常
    Big_Armor_Points_3d.clear();

    double qx = trans.at<double>(0, 0);
    double qy = trans.at<double>(1, 0);
    double qz = trans.at<double>(2, 0);
    distance = sqrt(qx * qx + qy * qy + qz * qz);

    double tan_pitch = qy / sqrt(qx * qx + qz * qz);
    double tan_yaw = qx / qz;
    x_pitch = -atan(tan_pitch) * 180 / CV_PI;
    y_yaw = atan(tan_yaw) * 180 / CV_PI;

    x_pitch += 0;
    y_yaw += 0.2;

    VisionData   vdata = {{(float)y_yaw},{(float)x_pitch},{(float)distance},1,1,1,1};
    usb_serial1.TransformData1(vdata);
    usb_serial1.send1();
}
//you

