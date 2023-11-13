#ifndef SERIAL_H
#define SERIAL_H

#include "Start/start.h"


//模式
#define CmdID0 0x00; //关闭视觉
#define CmdID1 0x01; //识别红色
#define CmdID2 0x02; //识别蓝色

//字节数为4的结构体
typedef union
{
    float f;
    unsigned char c[4];
} float2uchar;

typedef struct
{
    float2uchar yaw_angle;//偏航角
    float2uchar pitch_angle;//俯仰角
    float2uchar dis;//目标距离
    int openfire;  //当识别的图片范围内有目标且电控发来的信号不为0x00（关闭视觉）置为1，否则置0
    int isFindTarget;//当识别的图片范围内有目标且电控发来的信号不为0x00（关闭视觉）置为1，否则置0
    int istuoluokaiguan;
    int nearFace;
    int imuy;
    int imup;
    int imumode1;
    int imumode2;
    int imumode3;
} VisionData;

class UsbSerial
{
public:
    bool SerialInit();
    void SerialRecData(int16_t *serial_data);
    void TransformData(const VisionData &data);
    void closePort();
    void send();

private:
    int fd;
    int speed, databits, stopbits, parity;
    unsigned char rdata[18]; //raw_data
    unsigned char data[18];
    unsigned char Tdata[30];  //transfrom data
};

//双串口
class UsbSerial1
{
public:
    bool SerialInit1();
    void SerialRecData1(int16_t *serial_data);
    void TransformData1(const VisionData &data);
    void closePort1();
    void send1();
private:
    int fd1;
    int speed, databits, stopbits, parity;
    unsigned char rdata[18]; //raw_data
    unsigned char data[18];
    unsigned char Tdata[30];  //transfrom data
};


unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength, unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);

#endif // SERIAL_H
