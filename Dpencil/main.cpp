#include <algorithm>
#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <math.h>
#include "opencv2/highgui/highgui_c.h"
#include "Start/start.h"

using namespace std;
using namespace cv;


int main()
{
    Start start;    
    std::thread ImgProcessor(&Start::ImgProcess,&start);    //(&类::函数名,&对象）
    std::thread Reception(&Start::Recive,&start);
    std::thread Reception1(&Start::Recive1,&start);
    ImgProcessor.join();     //	阻塞等待到该线程结束,保证当前线程结束后再执行下一个线程
    Reception.join();
    Reception1.join();


    return 0;
}
