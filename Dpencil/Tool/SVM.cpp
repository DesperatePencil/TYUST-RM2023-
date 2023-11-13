#include "Armor/Armor.h"

using namespace std;
using namespace cv;
using namespace ml;

bool MatchSVM::picMatch(ArmorBag &armor)        //哨兵的SVM训练集中千万别加基地
{/*

    std::cout<<"%%%*************************"<<armor.huo[1]<<endl;*/
   ArmorBag q = ArmorBag();
   q = armor;
   Mat p = q.armor_warp.reshape(1, 1);

   p.convertTo(p, CV_32FC1);
   int response =0;
   response = (int)svm->predict(p);
//   cout << "svm : " << response <<endl;
   switch (response)
   {
    case 0:
        armor.robotType = NO;
       break;
    case 1:
        armor.robotType = HERO;
       break;
    case 2:
        armor.robotType = TRUCK;
       break;
    case 3:
        armor.robotType = INFANTRY_3;
       break;
    case 4:
        armor.robotType = INFANTRY_4;
       break;
    case 5:
        armor.robotType = INFANTRY_5;
       break;
    case 6:
        armor.robotType = SENTRY;
       break;

   }

   if(armor.robotType == NO)
   {
//       cout << "no" << endl;
       return false;
   }
   else
   {
//       cout << "yes" << endl;
       return true;
   }



}
