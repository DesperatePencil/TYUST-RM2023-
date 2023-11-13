#ifndef INFO_H
#define INFO_H
enum EnemyColor//目标单位的颜色，图像处理用
{
    RED=0,
    BLUE
};
enum AttackPattern//目标类型
{
    ARMOR=0,
    FU
};
enum ArmorSize//装甲板大小
{
    SMALL_ARMOR=0,
    BIG_ARMOR
};
enum FindResult
{
    COUPLIE_LIGHT_FOUND=0,
    NO_COUPLIE_LIGHT_FOUND,
    ARMOR_FOUND,
    NO_ARMOR_FOUND
};
enum RobotType
{
    NO=0,
    HERO,
    TRUCK,
    INFANTRY_3,
    INFANTRY_4,
    INFANTRY_5,
    SENTRY

};
#endif // INFO_H
