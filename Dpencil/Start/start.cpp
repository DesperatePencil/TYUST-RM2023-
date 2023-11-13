#include "start.h"
#include "CameraDriver/Open_camera.h"
#include "ImgProcessing/imgprocessing.h"
#include "Communication/serial.h"
#include "Armor/Armor.h"
#include "Solver/Solver.h"
#include <fuhuo.h>
#include <fstream>

#define ccc 

static UsbSerial usb_serial;        //最好用L和R把左右头区分开，而不是像我一样1 2，太容易搞混了
static UsbSerial1 usb_serial1;
static FuHuo fuhuo;

void Start::ImgProcess()
{
    open_camera camera;
    camera.openFrame2("/home/newmaker-08/newmaker/camera.ini");   //zuo       相机ini 调节曝光  由于是双相机，世界上没有绝对相同的两个物体
    camera.openFrame1("/home/newmaker-08/newmaker/camera.1.ini");   //you        包括相机，所以最好分别调用两个ini文件，一个也能用
    Mat image;
    Mat image1;
    ImgProcessing imgprocessing;
    ImgProcessing imgprocessing1;
    ArmorDetector armordetector;
    ArmorDetector armordetector1;
    AngleSolver anglesolver;
    AngleSolver1 anglesolver1;
    double time;
    while (1)
   {
    double start_time;

//---------------------------------------------------------------------
    image = camera.GetFrame2();
    image1 = camera.GetFrame1();

    if(image.empty())                   //防止相机掉线获取不到图像，好像没啥大用
    {
      cout<<"kkkkkkkkkkkkkkkkkkkkkkkkkkkk"<<endl;
      continue;}
    if(image1.empty())
    {
      cout << "jjjjjjjjjjjjjjjjjjjjjjjjjjjj"<< endl;
      continue;}

    start_time = getCPUTickCount();          //计算延迟

           if(image.size != nullptr)          //获取击打颜色
            {

                if(enemy_color == 1)
                {
                    imgprocessing.enemycolor =RED;
                }
                if (enemy_color == 2)
                {imgprocessing.enemycolor = BLUE;
                }
                img = image;
                imgprocessing.Deal(img);
            }
            if(image1.size != nullptr)
            {
                if(enemy_color == 1)
                {
                    imgprocessing1.enemycolor =RED;
                }
                if (enemy_color == 2)
                {imgprocessing1.enemycolor = BLUE;
                }
                img1 = image1;
                imgprocessing1.Deal(img1);
            }

            prepare_img = imgprocessing.final_img;
            prepare_img1 = imgprocessing1.final_img;
            
            armordetector.detect(img,prepare_img,imgprocessing.middle_l,fuhuo);      //遍历筛选条件，获取可击打装甲板
            armordetector1.detect(img1,prepare_img1,imgprocessing1.middle_r,fuhuo);

            if(armordetector.targetArmor.score != 0)           //若获取到目标
            {
                anglesolver.SolveAngle(usb_serial,12.0,5.3,armordetector.targetArmor);
            }
            else    //未获取到目标
            {
                VisionData   vdata = {{0},{0},{0},0,0,0,0};
                usb_serial.TransformData(vdata);
                usb_serial.send();
            }

            if(armordetector1.targetArmor.score != 0)
            {
                anglesolver1.SolveAngle(usb_serial1,12.0,5.3,armordetector1.targetArmor);
            }
            else
            {
                VisionData   vdata = {{0},{0},{0},0,0,0,0};
                usb_serial1.TransformData1(vdata);
                usb_serial1.send1();
            }
//--------------------------------------------------------------------------------------
            double end_time; end_time = getCPUTickCount();     
            time = (end_time-start_time)/getTickFrequency();
            char string[10];

            char stringJ[10];                 //以下都是用来在右上角打印distance，pitch，yaw信息的
            sprintf(stringJ,"%.2f",anglesolver.x_pitch);
            std::string fpsStringJJ("pitch:");
            fpsStringJJ += stringJ;

            char stringY[10];
            sprintf(stringY,"%.2f",anglesolver.y_yaw);
            std::string fpsStringYY("yaw");
            fpsStringYY += stringY;

            sprintf(string,"%.1f",1000*time);
            std::string fpsString("delay:");
            std::string unit("ms");
            fpsString += string;
            fpsString += unit;

            char stringz[10];
            sprintf(stringz,"%.2f",anglesolver.distance);
            std::string fpsStringzz("distance:");
            fpsStringzz += stringz;

            char stringQ[10];
            sprintf(stringQ,"%.2f",anglesolver1.distance);
            std::string fpsStringQQ("distance:");
            fpsStringQQ += stringQ;

            putText(image,fpsString,Point(50,50), 2,1,Scalar(0,0,255));
            cv::putText(image, fpsStringJJ,cv::Point(750, 150),cv::FONT_HERSHEY_SIMPLEX,1.0,cv::Scalar(0, 255, 0),2);
            cv::putText(image, fpsStringYY,cv::Point(750, 100),cv::FONT_HERSHEY_SIMPLEX,1.0,cv::Scalar(0, 255, 0),2);
            cv::putText(image, fpsStringzz,cv::Point(750, 50),cv::FONT_HERSHEY_SIMPLEX,1.0,cv::Scalar(0, 255, 0),2);
            cv::putText(image1, fpsStringQQ,cv::Point(750, 50),cv::FONT_HERSHEY_SIMPLEX,1.0,cv::Scalar(0, 255, 0),2);

            if(waitKey(1)==' ')//设置合适
                break;
        }
}

void Start::Recive()      //接收
{
     usb_serial.SerialInit();
     int16_t  rec_data[8];
     static int num = 0;
     while (true)
     {
             num = 20000;
             usb_serial.SerialRecData(rec_data);
             enemy_color = (int)rec_data[1];     //颜色
             int a[7];                         //以下都是复活机制
             //int huo[7];
             a[1] = (int)rec_data[4];   //英雄
             a[2] = (int)rec_data[5];  //工程
             a[3] = (int)rec_data[6];  //步兵3
             a[4] = (int)rec_data[7];  //步兵4
             a[5] = (int)rec_data[8];  //步兵5

             for (int i = 1; i < 6; i++)      //  可击打 数组里的i必须与兵种的数字对应
             {
                 if (a[i] == 0)
                 {
                     fuhuo.huo[i] = i;      //huo数组里的就是可以击打的兵种
                 }
                 else
                 {
                     fuhuo.huo[i] = 10;
                 }
             }


             qianshao = (int)rec_data[9];
          
             if(qianshao==0)  fuhuo.huo[6]=10;
             else   fuhuo.huo[6]=6;

             fuhuo.huo[1] = 1;           //国赛的复活机制只加了针对哨兵是否无敌的，所以其他兵种默认击打，后面有需求可以随时改
             fuhuo.huo[2] = 2;
             fuhuo.huo[3] = 3;
             fuhuo.huo[4] = 4;
             fuhuo.huo[5] = 5;
     }
}

void Start::Recive1()      //同上
{
    //cout<<"1111"<<endl;
     usb_serial1.SerialInit1();
     int16_t  rec_data[8];
     static int num = 0;
     while (true)
     {
             num = 20000;
             usb_serial1.SerialRecData1(rec_data);
             enemy_color = (int)rec_data[1];
             int a[7];
             //int huo[7];
             a[1] = (int)rec_data[4];   //英雄        
             a[2] = (int)rec_data[5];  //工程
             a[3] = (int)rec_data[6];  //步兵3
             a[4] = (int)rec_data[7];  //步兵4
             a[5] = (int)rec_data[8];  //步兵5

             for (int i = 1; i < 6; i++)
             {
               if (a[i] == 0)                     //  可击打 数组里的i必须与兵种的数字对应
               {
                 fuhuo.huo[i] = i;      //huo数组里的就是可以击打的兵种        1  3 4 5 6  或 10
               }
               else
               {
                  fuhuo.huo[i] = 10;        //除了 1--6  填啥都行

               }

             }
             if(qianshao==0)  fuhuo.huo[6]=10;
             else   fuhuo.huo[6]=6;

             fuhuo.huo[1] = 1;               //国赛的复活机制只加了针对哨兵是否无敌的，所以其他兵种默认击打，后面有需求可以随时改
             fuhuo.huo[2] = 2;
             fuhuo.huo[3] = 3;
             fuhuo.huo[4] = 4;
             fuhuo.huo[5] = 5;
     }
}

