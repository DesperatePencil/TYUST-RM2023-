#include "Armor.h"
#include <fuhuo.h>

ArmorBag::ArmorBag() {//用0初始化ArmorBag
    armor_area=1;
    score = 0;
    l_num = -1;
    r_num = -1;
    l_light = LightBag();
    r_light = LightBag();
    armor_size = SMALL_ARMOR;
    robotType = NO;
    armor_center = Point2f();
    armor_roi = Rect();
    armor_warp = Mat();
    equalImg = Mat();
    light_Vertices.clear();
    light_Vertices.resize(4);
}

ArmorBag::ArmorBag(Mat &img, const LightBag & l_light, const LightBag & r_light) {//用左右灯条初始化ArmorBag
    this->l_light = l_light;
    this->r_light = r_light;

    armorAngle = (l_light.angle + r_light.angle) / 2;

    Size L_Size1(float(l_light.lightRect.size.width), float(l_light.lightRect.size.height));//灯条尺寸
    Size R_Size1(float(r_light.lightRect.size.width), float(r_light.lightRect.size.height));
    Size L_Size2(float(l_light.lightRect.size.width), float(2.1*l_light.lightRect.size.height));//装甲板两边尺寸
    Size R_Size2(float(r_light.lightRect.size.width), float(2.1*r_light.lightRect.size.height));
    RotatedRect LLight(l_light.center, L_Size1, this->armorAngle),  LSide(l_light.center, L_Size2, this->armorAngle);
    RotatedRect RLight(r_light.center, R_Size1, this->armorAngle),  RSide(r_light.center, R_Size2, this->armorAngle);
    Point2f     ll[4],rr[4],                                        lside[4],rside[4];
                LLight.points(ll);RLight.points(rr);                LSide.points(lside);RSide.points(rside);
    Point2f     l_up = ll[2], l_low = ll[3],                        ls_up = lside[2], ls_low = lside[3];
    Point2f     r_up = rr[1], r_low = rr[0],                        rs_up = rside[1], rs_low = rside[0];
    light_Vertices.emplace_back(l_up);   armor_Vertices[0]=ls_up;
    light_Vertices.emplace_back(r_up);   armor_Vertices[1]=rs_up;
    light_Vertices.emplace_back(r_low);  armor_Vertices[2]=rs_low;
    light_Vertices.emplace_back(l_low);  armor_Vertices[3]=ls_low;

    armor_area = (int)(light_Vertices[1].x-light_Vertices[0].x)*(light_Vertices[3].y-light_Vertices[0].y);

    armor_center.x=(r_light.center.x + l_light.center.x)/2;
    armor_center.y=(r_light.center.y + l_light.center.y)/2;

    center_diff=(armor_center.x-0.5*img.cols)*(armor_center.x-0.5*img.cols)+(armor_center.y-0.5*img.rows)*(armor_center.y-0.5*img.rows);

    if (this->lengthDistanceProportion() > 3.1)     //用来判断大小装甲板
        armor_size = BIG_ARMOR;
    else
        armor_size = SMALL_ARMOR;
}

ArmorBag::~ArmorBag() {}

float ArmorBag::lightAngleDiff() const // 灯条平行度——装甲板左右灯条角度差
{
    float light_angle_diff = abs(l_light.angle-r_light.angle);
    return light_angle_diff;
}

float ArmorBag::getDeviationAngle() const// 灯条错位角          吉大
{
    float delta_x = r_light.center.x - l_light.center.x;
    float delta_y = r_light.center.y - l_light.center.y;
    float deviationAngle = abs(atan(delta_y / delta_x)) * 180 / CV_PI; //tanθ=Δy/Δx
    return deviationAngle;
}
/* 标准小装甲比--12.3/5.7=2.158      极限--   最小--6.00/5.7=1.05    最大--12.3/4.0=3.07
 * 标准大装甲比--23.1/5.8=3.982      极限--   最小--12.0/5.7=2.10    最大--23.1/4.0=5.77       理论值
 */
float ArmorBag::lengthDistanceProportion() const//灯条距离与灯条长度比——距离比/两灯条长度平均值
{
    float delta_x = r_light.center.x - l_light.center.x;
    float H_distance_proportion = abs(2*delta_x/(l_light.lightRect.size.height+r_light.lightRect.size.height));
    return H_distance_proportion;
}

float ArmorBag::lengthDiffProportion() const// 左右灯条长度差比值
{
    float H_diff = abs(l_light.lightRect.size.height-r_light.lightRect.size.height);
    float H_diff_proportion = 2*H_diff/(l_light.lightRect.size.height+r_light.lightRect.size.height);
    return H_diff_proportion;
}

//吉大集合筛选
float ArmorBag::getDislocationX() const    //灯条位置差距 两灯条中心x方向差距比值
{
    float meanLen = (l_light.length + r_light.length) / 2;
    float xDiff = abs(l_light.center.x - r_light.center.x); //x distance ration y轴方向上的距离比值（y轴距离与灯条平均值的比）
    float xDiff_ratio = xDiff / meanLen;
    return xDiff_ratio;
}

// dislocation judge Y:  r-l light center distance ration on the Y-axis 灯条位置差距 两灯条中心Y方向差距比值
float ArmorBag::getDislocationY() const
{
    float meanLen = (l_light.length + r_light.length) / 2;
    float yDiff = abs(l_light.center.y - r_light.center.y);  //y distance ration x轴方向上的距离比值（x轴距离与灯条平均值的比）
    float yDiff_ratio = yDiff / meanLen;
    return yDiff_ratio;
}

// length difference ration: the length difference ration r-l lights 左右灯条长度差比值
float ArmorBag::getLengthRation() const
{
    float length_diff = abs(l_light.length - r_light.length);
    float lengthDiffRation = length_diff / MAX(l_light.length, r_light.length);
    return lengthDiffRation;
}

float ArmorBag::lightCenterDeviation() const// 中心连线水平度——两灯条中心连线与水平线夹角
{
    float delta_x = r_light.center.x - l_light.center.x;
    float delta_y = r_light.center.y - l_light.center.y;
    float center_angle = abs(delta_y/delta_x);
    return center_angle;
}

bool ArmorBag::SuitableArmor() const//利用条件初步筛选装甲板
{
   uint i = 1;
   if(lightAngleDiff()>6)                                                       {i = 0;} // 灯条平行度——装甲板左右灯条角度差  吉大
   if(getDeviationAngle() > 50)                                             {i = 0;}  //0.2     吉大// 灯条错位角
   //吉大
   if(getDislocationX() > 4.5 )         {i = 0;}   //灯条位置差距 两灯条中心x方向差距比值
   if(getDislocationY() > 0.5)          {i = 0;}   //灯条位置差距 两灯条中心Y方向差距比值
   if(getLengthRation() > 0.5)          {i = 0;}   //左右灯条长度差比值

   if(lightAngleDiff() > 25 )                          {i = 0;}  //12  155// 灯条平行度——装甲板左右灯条角度差
   if(lightCenterDeviation() > 0.5)                                             {i = 0;}      // 中心连线水平度——两灯条中心连线与水平线夹角
   if(lengthDistanceProportion() < 1.05 || lengthDistanceProportion() > 5.55)   {i = 0;}//5.55//灯条距离与灯条长度比——距离比/两灯条长度平均值

   if(i == 0)
       return false;
   else
       return true;

//   cout << "JD111_灯条平行度: " << lightAngleDiff() << endl;
//   cout << "JD222_中心连线水平度: " << lightCenterDeviation() << endl;
//   cout << "JD333_灯条距离与灯条长度比: " << lengthDistanceProportion() << endl;
//   cout << "JD444_左右灯条长度差比值: " << lengthDiffProportion() << endl;

}

bool ArmorBag::fuhuo(FuHuo &fuhuo,ArmorBag &armor) const//复活筛选
{
    if ( armor.robotType== fuhuo.huo[1]|| armor.robotType == fuhuo.huo[2]|| armor.robotType == fuhuo.huo[3]|| armor.robotType == fuhuo.huo[4]||armor.robotType == fuhuo.huo[5]||armor.robotType == fuhuo.huo[6] )           //活着的兵种
        return true;
    else  return false;
}

void ArmorBag::getWrap(Mat &img, Point2f vertices[4],int &middle)//对装甲板图案作透视变换，20*20的大小，给svm分类
{
    Mat rotation;
    Point2f dst_points[4];
    dst_points[0] = Point2f(0.0,    0.0  );             //（）中的几十乘几十必须与SVM训练的数据集对应
    dst_points[1] = Point2f(50.0,   0.0  );
    dst_points[2] = Point2f(50.0,   50.0 );
    dst_points[3] = Point2f(0.0,    50.0 );
    rotation = getPerspectiveTransform(vertices,dst_points);//透视方式（最好写透视变换矩阵，才不显的low）
    warpPerspective(img,armor_warp,rotation,Size(50,50));

    multiply(armor_warp,5,armor_warp);  //6          图像整体变亮
    cvtColor(armor_warp,armor_warp,COLOR_BGR2GRAY);//以下为作二值化处理
     threshold(armor_warp,armor_warp,0,255,THRESH_BINARY_INV | THRESH_OTSU);   //大津法  自动调节阈值
}


