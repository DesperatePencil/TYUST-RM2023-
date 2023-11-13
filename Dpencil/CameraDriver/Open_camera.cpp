#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>
#include "opencv2/highgui/highgui_c.h"
#include "DVPcamera.h"
#include "Open_camera.h"

using namespace cv;
using namespace std;

//相机驱动是固定的，改不了，直接拿过去用，注意区分好左右头就可

bool open_camera::openFrame1(dvpStr camera_congig_file)
{
    status = dvpRefresh(&n);

    if (status == DVP_STATUS_OK)
    {
        // 枚举最多16台相机的信息
       {
            if (n > 16)
            n = 16;
        }
        cout <<"有"<<n<<"台相机"<< endl;
        for (i = 0; i < n; i++)
            // 逐个枚举出每个相机的信息
        {
            status = dvpEnum(i, &info[i]);
            if (status != DVP_STATUS_OK)
            {
               cout << "枚举NO"  << endl;
            }
            else
            {
               cout <<"枚举OK"<< endl;
            }
        }
    }
    status = dvpOpenByName(info[0].FriendlyName,OPEN_NORMAL,&m_handle);
    cout<<"info[0].FriendlyName  "<<info[0].FriendlyName<<endl;

    //status = dvpOpenByName(info[1].FriendlyName,OPEN_NORMAL,&m_handle);
    if (status == DVP_STATUS_OK)
    {
        cout<<"ok!!!"<<endl;//// QMessageBox::about(NULL,"About","Open the camera fail!");
    }
    status = dvpGetStreamState(m_handle,&state);
    status = dvpSetAwbOperation(m_handle, AWB_OP_CONTINUOUS);  //白平衡
    if (status != DVP_STATUS_OK)
    {
        cout<<"Get the stream state fail!"<<endl; //QMessageBox::about(NULL,"About","Get the stream state fail!");
    }
    if (state == STATE_STOPED)
    {
        status = dvpGetTriggerState(m_handle,&bTrigStatus);
        if (status != DVP_STATUS_FUNCTION_INVALID)
        {
            // 在启动视频流之前先设置为触发模式
            status = dvpSetTriggerState(m_handle,SoftTriggerFlag ? true : false);
            if (status != DVP_STATUS_OK)
            {
                 //QMessageBox::about(NULL,"About","Set status of trigger fail!");
            }
            else
            {
                cout<<"Set status of trigger"<<endl;
            }
        }
        else
        {
            cout<<"sss"<<endl;//ui->groupBox_trigger->setEnabled(false);
        }

        status = dvpStart(m_handle);
        if (status != DVP_STATUS_OK)
        {
        }
    }
     //cout<<"111status"<<status<<endl;
    dvpStr path= camera_congig_file;
    //dvpStr path1= "/home/newmaker/rm_task/Test/test/1280_1024.ini";
    dvpLoadConfig(m_handle,path);
    return true;
 }

cv::Mat open_camera::GetFrame1()
{
    cv::Mat Mat_src;
    status = dvpGetFrame(m_handle, &m_pFrame, &pBuffer,30000);
    //cout<<"111 status "<<status<<endl;
    Mat_src = cv::Mat(m_pFrame.iHeight, m_pFrame.iWidth, CV_8UC3, pBuffer);
//    imshow("3",Mat_src);
    return Mat_src;
}


bool open_camera::openFrame2(dvpStr camera_congig_file)
{
    status2 = dvpRefresh(&n);

    if (status2 == DVP_STATUS_OK)
    {
        // 枚举最多16台相机的信息
       {
            if (n > 16)
            n = 16;
        }
        cout <<"有"<<n<<"台相机"<< endl;
        for (i = 0; i < n; i++)
            // 逐个枚举出每个相机的信息
        {
             status2= dvpEnum(i, &info[i]);
            if (status2 != DVP_STATUS_OK)
            {
               cout << "枚举NO"  << endl;
            }
            else
            {
               cout <<"枚举OK"<< endl;
            }
        }
    }
    status2 = dvpOpenByName(info[1].FriendlyName,OPEN_NORMAL,&m_handle2);
    cout<<"222info[1].FriendlyName  "<<info[1].FriendlyName<<endl;

    if (status2 == DVP_STATUS_OK)
    {
        cout<<"ok!!!"<<endl;//// QMessageBox::about(NULL,"About","Open the camera fail!");
    }
    status2 = dvpGetStreamState(m_handle2,&state);
    status2 = dvpSetAwbOperation(m_handle2, AWB_OP_CONTINUOUS);  //白平衡

    if (status2 != DVP_STATUS_OK)
    {
        cout<<"Get the stream state fail!"<<endl; //QMessageBox::about(NULL,"About","Get the stream state fail!");
    }
    if (state == STATE_STOPED)
    {
        status2 = dvpGetTriggerState(m_handle2,&bTrigStatus);
        if (status2 != DVP_STATUS_FUNCTION_INVALID)
        {
            // 在启动视频流之前先设置为触发模式
            status2 = dvpSetTriggerState(m_handle2,SoftTriggerFlag ? true : false);
            if (status2 != DVP_STATUS_OK)
            {
                 //QMessageBox::about(NULL,"About","Set status2 of trigger fail!");
            }
            else
            {
                cout<<"Set status2 of trigger"<<endl;
            }
        }
        else
        {
            cout<<"sss"<<endl;//ui->groupBox_trigger->setEnabled(false);
        }

        status2 = dvpStart(m_handle2);
        if (status2 != DVP_STATUS_OK)
        {
        }
    }
    //cout<<"222status"<<status2<<endl;
    dvpStr path= camera_congig_file;
    //dvpStr path1= "/home/newmaker/rm_task/Test/test/1280_1024.ini";
    dvpLoadConfig(m_handle2,path);
    return true;
 }



cv::Mat open_camera::GetFrame2( )
{
    cv::Mat mat_src;
    status2 = dvpGetFrame(m_handle2, &m_pFrame2, &pBuffer2,30000);
     //cout<<"222 status2 "<<status2<<endl;
    mat_src = cv::Mat(m_pFrame2.iHeight, m_pFrame2.iWidth, CV_8UC3, pBuffer2);
//    imshow("4",mat_src);
    return mat_src;
}
