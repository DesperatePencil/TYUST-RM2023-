#include "Armor.h"
#include <fuhuo.h>
//2500 3500
void eraseErrorRepeatArmor(vector<ArmorBag> & armors);     //吉大函数，用做筛选条件

ArmorDetector::ArmorDetector()
{
    T = T_max;
    targetArmor = ArmorBag();
    lastArmor = ArmorBag();

//     namedWindow("svm阈值",WINDOW_AUTOSIZE);
//     cvCreateTrackbar("middle","svm阈值",&middle,255);
}
ArmorDetector::~ArmorDetector(){}

void ArmorDetector::findLights(Mat &img,Mat &final_img)
{
    this->final_img=final_img;
    RotatedRect light_ellipse;
    vector<vector<Point>> lightContours;  //候选灯条轮廓
    Mat contourImg; //给findContours用的图像，防止findContours改变roi
    final_img.copyTo(contourImg);
    findContours(contourImg,lightContours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);//找到轮廓,耗时   

    for (uint i = 0;i < lightContours.size();i++)//这个循环的方式或许是个可以优化的点
    {
        if(lightContours[i].size()<6)continue;// 轮廓点数小于6，不可拟合椭圆

        if(contourArea(lightContours[i]) < 30.0||contourArea(lightContours[i]) > 8000.0)continue;//用面积简单筛选灯条   //30  5000
        light_ellipse = fitEllipse(lightContours[i]);//创建临时旋转矩阵

       //if(light_ellipse.size.height/light_ellipse.size.width<2.3 || light_ellipse.size.height/light_ellipse.size.width>13.3 )continue;//用框选的长宽比筛选灯条

        if(light_ellipse.angle > 90){light_ellipse.angle = light_ellipse.angle - 180;}
        if(abs(light_ellipse.angle) > 45)continue; //灯条倾斜度筛选  25

        llipse(img,light_ellipse,Scalar(50,250,50),2,8);//椭圆画灯条

        lights.emplace_back(light_ellipse);//比push_back好，节省冗余运算
    }
    if (lights.size() < 2) {
        findResult = NO_COUPLIE_LIGHT_FOUND; //灯条少于两条则为没找到灯条
        return; //exit
    }

    //将灯条从左到右排序
    sort(lights.begin(), lights.end(),[](LightBag & a1, LightBag & a2)
    {return a1.center.x < a2.center.x; });
//    for(uint i = 0; i < lights.size();i++)//根据lights里的灯条顺序标号，检查用
//    {
//        char string[2];
//        sprintf(string,"%d",i);
//        putText(img,string,lights[i].center, 2,1,Scalar(0,0,255));
//    }

    findResult = COUPLIE_LIGHT_FOUND;
    return;
}


void ArmorDetector::matchArmor(Mat &img, int &middle,FuHuo &fuhuo)           //筛选条件
{
    for (uint i = 0; i < lights.size(); i++)
    {
        for (uint j = i + 1; j <  lights.size(); j++)
        {
            ArmorBag armor = ArmorBag(img,lights[i],lights[j]);//用左右灯条初始化 armor

            if(armor.SuitableArmor())//几何条件筛选装甲板
            {
                armor.l_index = i; //set index of left light 左灯条的下标
                armor.r_index = j; //set index of right light 右灯条的下标
                //cout <<"几何通过"<<endl;
                armor.getWrap(img,armor.armor_Vertices,middle);//获取装甲板透视过的图案

                if(svm.picMatch(armor))//svm判断图案
                {
                   if( armor.fuhuo(fuhuo,armor) )       //复活机制

                    armors.emplace_back(armor); /*将匹配好的装甲板push入armors中*/
               }
            }
        }
            eraseErrorRepeatArmor(armors);//delete the error armor caused by error light 删除游离灯条导致的错误装甲板
    }
}

void eraseErrorRepeatArmor(vector<ArmorBag> & armors)         //吉大函数  删除游离灯条导致的错误装甲板
{
    int length = armors.size();
    vector<ArmorBag>::iterator it = armors.begin();
    for (size_t i = 0; i < length; i++)
        for (size_t j = i + 1; j < length; j++)
        {
            if (armors[i].l_index == armors[j].l_index ||
                armors[i].l_index == armors[j].r_index ||
                armors[i].r_index == armors[j].l_index ||
                armors[i].r_index == armors[j].r_index)
            {
                armors[i].getDeviationAngle() > armors[j].getDeviationAngle() ? armors.erase(it + i) : armors.erase(it + j);
            }
        }
}

void ArmorDetector::priority()
{
    if(T != 0)//T 为空 则重置上次目标为空
    {
        if(armors.size() != 0)//判断这次是否找到装甲板
        {
            if(lastArmor.score == 0)//判断上次目标是否为空
            {
                for(uint i = 0 ; i < armors.size();i++)//遍历装甲板，附分，找到得分最高的
                {
                    armors[i].score += (int)armors[i].armor_area/10;              //根据装甲板面积大小加分
                    armors[i].score += (int)armors[i].armor_size*10000;             //根据装甲板大小类型加分
                    armors[i].score -= (int)armors[i].center_diff/100;            //根据装甲板中心距图像正中心距离减分
                    if(armors[i].robotType == 2){armors[i].score -= 9990000000000*999*9;}       //若是工程，则减去对应得分
                    if(armors[i].robotType == 1) { armors[i].score += 9990000000000 * 999 * 9; }       //若是英雄，则猛猛加分
//                    cout<<i<<endl;
//                     cout<<armors[i].robotType<<"  "<<armors[i].score<<endl;
                }
                sort(armors.begin(), armors.end(),[](ArmorBag & a1, ArmorBag & a2)//根据装甲板得分从大到小排序
                {return a1.score > a2.score; });
                targetArmor = armors[0];//上次目标为空，赋值
                lastArmor = targetArmor;

            }
            else
            {
                for(uint i = 0 ; i < armors.size();i++)
                {
                    armors[i].score += (int)armors[i].armor_area/10;              //根据装甲板面积大小加分
                    armors[i].score += (int)armors[i].armor_size*10000;             //根据装甲板大小类型加分
                    armors[i].score -= (int)armors[i].center_diff/100;            //根据装甲板中心距图像正中心距离减分
                    if(armors[i].robotType == 2){armors[i].score -= 9990000000000*999*9;}       //若是工程，则减去对应得分
                    if (armors[i].robotType == 1) { armors[i].score += 9990000000000 * 999 * 9; }       //若是英雄，则猛猛加分

//                    cout<<i<<endl;
//                     cout<<armors[i].robotType<<"  "<<armors[i].score<<endl;
                }
                sort(armors.begin(), armors.end(),[](ArmorBag & a1, ArmorBag & a2)//根据装甲板得分从大到小排序
                {return a1.score > a2.score; });
                if(abs(armors[0].armor_center.x-lastArmor.armor_center.x)+abs(armors[0].armor_center.y-lastArmor.armor_center.y)>60)//上次目标不为空，判断与这次位置差
                {                                                                                                            //不必设置很大，这里是因为针对视频而设置大
                    if(lastArmor.robotType == armors[0].robotType)//判断这次目标装甲类型和上次是否一样
                    {
                        targetArmor = armors[0];//位置相差太大，但装甲板类型相同，则直接赋值
                        lastArmor = targetArmor;//跳变过大，预测时需将这个视为异常数据
                        T=T_max;
                    }
                    else
                    {
                        targetArmor = lastArmor;//位置相差太大，且装甲板类型不同，进入缓冲
                        T--;
                    }
                }
                else
                {
                    targetArmor = armors[0];//位置相差不大，直接赋值
                    lastArmor = targetArmor;
                    T=T_max;
                }
            }
        }
        else
        {
            if(lastArmor.score == 0){}//上次目标为空
            else
            {
                targetArmor = lastArmor;
                T--;
            }
        }
    }
    else
    {
        lastArmor = ArmorBag();
        targetArmor = ArmorBag();
        T=T_max;
    }

}

void ArmorDetector::detect(Mat &img, Mat &final_img,int &middle,FuHuo &fuhuo)
{
    lights.clear();
    armors.clear();


     if(lastArmor.score != 0)
     {
        char string[30];
        sprintf(string,"T:%d",T);
//        circle(img,lastArmor.armor_center,8,Scalar(0,250,250));//黄圈画出上一次目标中心
//        putText(img,string,lastArmor.armor_center, 1,1,Scalar(0,0,255));
     }

    findLights(img,final_img);     //发现灯条

    matchArmor(img,middle,fuhuo);    //匹配装甲板

    priority();   //优先级

    drew(img,armors);//画出筛选的所有找到的装甲板

    //OutArmors(armors);
}


void ArmorDetector::drew(Mat &img,vector<ArmorBag> &armors)       //以下这些都是在识别出的装甲板上圈圈画画，辅助调试用的
{
    for(uint i=0;i<armors.size();i++)
    {char string[10];sprintf(string,"%d",i);/*imshow(string,armors[i].armor_warp);*/}//显示找到的装甲板图案

    for(uint i = 0 ; i < armors.size(); i++)//画出所有找到的装甲板
    {
        line(img,armors[i].armor_Vertices[0],armors[i].armor_Vertices[1],Scalar(250,250,0));
        line(img,armors[i].armor_Vertices[2],armors[i].armor_Vertices[3],Scalar(250,250,0));
        line(img,armors[i].armor_Vertices[0],armors[i].armor_Vertices[3],Scalar(250,250,0));
        line(img,armors[i].armor_Vertices[1],armors[i].armor_Vertices[2],Scalar(250,250,0));
//        line(img,targetArmor.armor_Vertices[0],targetArmor.armor_Vertices[1],Scalar(0,250,0))  ;
//        line(img,targetArmor.armor_Vertices[2],targetArmor.armor_Vertices[3],Scalar(0,250,0))  ;
//        line(img,targetArmor.armor_Vertices[0],targetArmor.armor_Vertices[3],Scalar(0,250,0))  ;
//        line(img,targetArmor.armor_Vertices[1],targetArmor.armor_Vertices[2],Scalar(0,250,0))  ;

//        circle(img,armor.armor_Vertices[0],5,Scalar(250,250,250),2,8,0);
//        circle(img,armor.armor_Vertices[1],5,Scalar(250,250,250),2,8,0);
//        circle(img,armor.armor_Vertices[2],5,Scalar(250,250,250),2,8,0);
//        circle(img,armor.armor_Vertices[3],5,Scalar(250,250,250),2,8,0);

        char string[30];
        sprintf(string,"num:%d score:%d",armors[i].robotType,armors[i].score);
        sprintf(string,"num:%d",armors[i].robotType);
        putText(img,string,armors[i].armor_Vertices[0], 2,1,Scalar(0,0,255));
    }

//    circle(img,targetArmor.armor_center,5,Scalar(0,250,0));//画出targetarmor
//    putText(img,"target",targetArmor.armor_Vertices[3], 1,1,Scalar(0,255,0));

}

void OutLights(vector<LightBag> & lights)//输出找到灯条的中心、角度信息
{
    cout << "**************************** L I G H T S ******************************"<< endl;
    if (lights.empty()) {
        cout << "NO_COUPLIE_LIGHT_FOUND!" << endl;
    }
    else
    {
        cout << "COUPLIE_LIGHT_FOUND!" << endl;
//        for (size_t i = 0; i < lights.size(); i++)//输出找到灯条的中心、角度信息
//        {
//            cout << "Light " << i << " Center:" << lights[i].center << endl;
//            cout << "Light " << i << " Angle :" << lights[i].angle << endl;
//        }
    //cout << "疑似灯条数 ： " << lights.size() << endl;
        cout << "***********************************************************************" << endl;
    }
}

void OutArmors(vector<ArmorBag> & armors)//输出找到装甲板的中心、数字、匹配信息
{
//    cout << "**************************** A R M O R S ******************************" << endl;
//    if (armors.empty()) {cout << "ARMORS NOT FOUND!" << endl;}
//    else
//    {
//        cout << "ARMOR FOUND!" << endl;
/*
 * 为了方便系统之间的移植而定义的，不同的系统上，定义size_t 可能不一样。
 * size_t在32位系统上定义为 unsigned int，也就是32位无符号整型。在64位系统上定义为 unsigned long ，也就是64位无符号整形。
 * size_t 的目的是提供一种可移植的方法来声明与系统中可寻址的内存区域一致的长度。
 */
        //cout <<"装甲板个数 : " <<armors.size() << endl;
        for (size_t i = 0; i < armors.size(); i++)
        {
//            cout << "————————————————————————————" << endl;
//            cout << "Armor " << i << " Center: " << armors[i].armor_center << endl;
//            if (armors[i].armor_size == SMALL_ARMOR)    cout << "Armor Type: SMALL ARMOR" << endl;
//            else if(armors[i].armor_size ==  BIG_ARMOR) cout << "Armor Type: BIG ARMOR" << endl;

//            cout << "JD111_灯条平行度: " << armors[i].lightAngleDiff() << endl;
//            cout << "JD222_中心连线水平度: " << armors[i].getDeviationAngle() << endl;
//            cout << "JD333_灯条距离与灯条长度比: " << armors[i].lengthDistanceProportion() << endl;
//            cout << "JD444_左右灯条长度差比值: " << armors[i].lengthDiffProportion() << endl;
//            cout << "JD555_y轴方向上的距离比值（y轴距离与灯条平均值的比）: " << armors[i].getDislocationX() << endl;
//            cout << "JD666_灯条位置差距 两灯条中心Y方向差距比值: " << armors[i].getDislocationY() << endl;
//            cout << "JD777_左右灯条长度差比值: " << armors[i].getLengthRation() << endl;
        }
        //cout << "**********************************************************************" << endl;
    //}

}
