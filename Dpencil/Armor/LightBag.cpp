#include "Armor.h"

LightBag::LightBag(){
    lightRect = RotatedRect();
    length = 0;
    center = Point2f();
    angle = 0;
}


LightBag::LightBag(const RotatedRect &lightRect){
    this->lightRect = lightRect;
    length = MAX(lightRect.size.height, lightRect.size.width);
    center = lightRect.center;
    if (lightRect.angle > 90)
        angle = lightRect.angle - 180;
    else
        angle = lightRect.angle;
}

LightBag::~LightBag(){}
