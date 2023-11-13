#ifndef OPEN_CAMERA_H
#define OPEN_CAMERA_H
#include <opencv2/opencv.hpp>
#include "DVPcamera.h"

using namespace std;

class open_camera{
    public:
        bool openFrame1(dvpStr camera_congig_file);
        cv::Mat GetFrame1();

       bool openFrame2(dvpStr camera_congig_file);
       cv::Mat GetFrame2();

        bool openFrame(dvpStr camera_congig_file);
        cv::Mat GetFrame();

        dvpFrame  m_pFrame;                   // 采集到的图像的结构体指针
        dvpFrame  m_pFrame2;
        void *    pBuffer;                    // 采集到的图像的内存首地址
        void *    pBuffer2;
        dvpStatus status;

        dvpStatus status2;

        dvpUint32 i = 0;
        dvpUint32 n = 0;
        dvpCameraInfo info[16];
        dvpHandle  m_handle;
        dvpHandle  m_handle2;
        dvpStreamState state;
        dvpDoubleDescr ExpDescr;
        dvpFloatDescr sAnalogGainDescr;
        bool bTrigStatus;
        bool SoftTriggerFlag;
};
#endif // OPEN_CAMERA_H
