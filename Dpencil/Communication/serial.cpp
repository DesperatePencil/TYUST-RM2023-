#include "serial.h"
int k=0;

//串口注意要修改部分信息

/****************************************
* @funcName SerialInit
* @brief    打开串口权限
* @para     无
* @return   无
* @date     2021.4.27
* @author   wjl
*****************************************///public:    09相机
bool UsbSerial::SerialInit()
{
       cout<<"+--------------------------------------"<<endl;
       cout<<"|            Usb Serial Send Datattyusb1 jing0    |"<<endl;
       cout<<"+--------------------------------------"<<endl;
       fd = open("/dev/jing0",O_RDWR);         //注意把串口名改为重映射之后的
       if(-1 == fd)
       {
           cout<<"Error open Serial"<<endl;
           return false;
       }
       else
       {
           cout<<"Open Serial Successfully"<<endl;
       }

       struct termios SerialPortSettings;	/* Create the structure                          */
       tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

       cfsetispeed(&SerialPortSettings,B115200); /* Set Read  Speed as 9600                       */
       cfsetospeed(&SerialPortSettings,B115200); /* Set Write Speed as 9600                       */

       SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
       SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
       SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
       SerialPortSettings.c_cflag |= CS8;       /* Set the data bits = 8                                 */

       SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
       SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */


       SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */
       SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/
       SerialPortSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
       if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
       {
           cout<<"\n  ERROR ! in Setting attributes"<<endl;
       }
       else
       {
           cout<<"\n  BaudRate = 115200 \n  StopBits = 1 \n  Parity = none"<<endl;
           cout<<"Serial Start Work "<<endl;
       }
       tcflush(fd,TCIFLUSH);
       cout<<"+--------------------------------------"<<endl;
       /*------------------------------- Write data to serial port -----------------------------*/
       return true;
}

/****************************************
* @funcName TransformData
* @brief    发送内容
* @para     无
* @return   无
* @date     2022.4.2
* @author   cjh
*****************************************/
void UsbSerial::TransformData(const VisionData &data)
{
    Tdata[0] = 0xA5;
    Tdata[1] = CmdID1
    Append_CRC8_Check_Sum(Tdata, 3);
    Tdata[3] = data.pitch_angle.c[0];
    Tdata[4] = data.pitch_angle.c[1];
    Tdata[5] = data.pitch_angle.c[2];
    Tdata[6] = data.pitch_angle.c[3];
    Tdata[7] = data.yaw_angle.c[0];
    Tdata[8] = data.yaw_angle.c[1];
    Tdata[9] = data.yaw_angle.c[2];
    Tdata[10] = data.yaw_angle.c[3];
    Tdata[11] = data.dis.c[0];
    Tdata[12] = data.dis.c[1];
    Tdata[13] = data.dis.c[2];
    Tdata[14] = data.dis.c[3];
    Tdata[15] = data.openfire;   
    Append_CRC16_Check_Sum(Tdata, 18);
}

/****************************************
* @funcName send
* @brief    发送函数
* @para     无
* @return   无
* @date     2022.4.2
* @author   cjh
*****************************************/
void UsbSerial::send()
{
    write(fd, Tdata, 18);
//     for(int i = 0; i < 18; i++)
//        {
//           cout<<"ttyusb0  "<<endl;;
//           printf("%x  ",Tdata[i]);
//        }
//        cout << endl;
}

/****************************************
* @funcName closePort
* @brief    关闭通讯协议接口
* @para     无
* @return   无
* @date     2022.4.2
* @author   cjh
*****************************************/
void UsbSerial::closePort()
{
    close(fd);
}

/****************************************
* @funcName SerialRecData
* @brief    接受函数
* @para     无
* @return   无
* @date     2022.4.2
* @author   cjh
*****************************************/
void UsbSerial::SerialRecData(int16_t *serial_data)
{
    int bytes_read = 0;

    size_t bytes;
        char *name = ttyname(fd);
        if ((name = nullptr))printf("tty is null\n");
        if (name != nullptr)printf("device:%s\n",name);
        ioctl(fd, FIONREAD, &bytes);

     uint many = 16;
     bytes = read(fd,rdata,many);

     for(uint i = 0;i < many;i++)          //防串位用的
     {
         uint j = i+1;
         if(j == many){j=j-many;}
         if(rdata[i]==0xA5 && rdata[j]==0)
         {
             uint l = i;
             for(uint k = 0;k<many;k++)
             {
                 if(l == many){l=l-many;}
                 data[k]=rdata[l];
                 l++;
             }
         }
     }
     serial_data[1] = (int)data[4];//颜色
     serial_data[4] = (int)data[9]; //英雄  1
        serial_data[5] = (int)data[10];  //工程 2
        serial_data[6] = (int)data[11];   //步兵 3
        serial_data[7] = (int)data[12];   //步兵 4
        serial_data[8] = (int)data[13];   //步兵 5
        serial_data[9] = (int)data[14];   //前哨
}



//串口收发2
/****************************************
* @funcName SerialInit
* @brief    打开串口权限
* @para     无
* @return   无
* @date     2021.4.27
* @author   wjl
*****************************************///public:     01相机
bool UsbSerial1::SerialInit1()
{
    //cout<<"?????????????????????????????????????????????"<<endl;
       cout<<"+--------------------------------------"<<endl;
       cout<<"|            Usb Serial Send Datattyusb0 jing1    |"<<endl;
       cout<<"+--------------------------------------"<<endl;
       fd1 = open("/dev/jing1",O_RDWR);
       if(-1 == fd1)
       {
           cout<<"Error open Serial"<<endl;
           return false;
       }
       else
       {
           cout<<"Open Serial Successfully"<<endl;
       }

       struct termios SerialPortSettings;	/* Create the structure                          */
       tcgetattr(fd1, &SerialPortSettings);	/* Get the current attributes of the Serial port */

       cfsetispeed(&SerialPortSettings,B115200); /* Set Read  Speed as 9600                       */
       cfsetospeed(&SerialPortSettings,B115200); /* Set Write Speed as 9600                       */

       SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
       SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
       SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
       SerialPortSettings.c_cflag |= CS8;       /* Set the data bits = 8                                 */

       SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
       SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */


       SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */
       SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/
       SerialPortSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
       if((tcsetattr(fd1,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
       {
           cout<<"\n  ERROR ! in Setting attributes"<<endl;
       }
       else
       {
           cout<<"\n  BaudRate = 115200 \n  StopBits = 1 \n  Parity = none"<<endl;
           cout<<"Serial Start Work "<<endl;
       }
       tcflush(fd1,TCIFLUSH);
       cout<<"+--------------------------------------"<<endl;
       /*------------------------------- Write data to serial port -----------------------------*/
       return true;
}

/****************************************
* @funcName TransformData
* @brief    发送内容
* @para     无
* @return   无
* @date     2022.4.2
* @author   cjh
*****************************************/
void UsbSerial1::TransformData1(const VisionData &data)
{
    Tdata[0] = 0xA5;
    Tdata[1] = CmdID1
    Append_CRC8_Check_Sum(Tdata, 3);
    Tdata[3] = data.pitch_angle.c[0];
    Tdata[4] = data.pitch_angle.c[1];
    Tdata[5] = data.pitch_angle.c[2];
    Tdata[6] = data.pitch_angle.c[3];
    Tdata[7] = data.yaw_angle.c[0];
    Tdata[8] = data.yaw_angle.c[1];
    Tdata[9] = data.yaw_angle.c[2];
    Tdata[10] = data.yaw_angle.c[3];
    Tdata[11] = data.dis.c[0];
    Tdata[12] = data.dis.c[1];
    Tdata[13] = data.dis.c[2];
    Tdata[14] = data.dis.c[3];
   Tdata[15] = data.openfire;
    Append_CRC16_Check_Sum(Tdata, 18);
}

/****************************************
* @funcName send
* @brief    发送函数
* @para     无
* @return   无
* @date     2022.4.2
* @author   cjh
*****************************************/
void UsbSerial1::send1()
{
    write(fd1, Tdata, 18);
//    for(int i = 0; i < 18; i++)
//    {
//       cout<<"ttyusb1  "<<endl;
//       printf("%x  ",Tdata[i]);
//    }
//    cout << endl;

}

/****************************************
* @funcName closePort
* @brief    关闭通讯协议接口
* @para     无
* @return   无
* @date     2022.4.2
* @author   cjh
*****************************************/
void UsbSerial1::closePort1()
{
    close(fd1);
}

/****************************************
* @funcName SerialRecData
* @brief    接受函数
* @para     无
* @return   无
* @date     2022.4.2
* @author   cjh
*****************************************/
void UsbSerial1::SerialRecData1(int16_t *serial_data)
{
    int bytes_read = 0;

    size_t bytes;
        char *name = ttyname(fd1);
        if ((name = nullptr))printf("tty is null\n");
        if (name != nullptr)printf("device:%s\n",name);
        ioctl(fd1, FIONREAD, &bytes);

     uint many = 16;
     bytes = read(fd1,rdata,many);

     for(uint i = 0;i < many;i++)
     {
         uint j = i+1;
         if(j == many){j=j-many;}
         if(rdata[i]==0xA5 && rdata[j]==0)
         {
             uint l = i;
             for(uint k = 0;k<many;k++)
             {
                 if(l == many){l=l-many;}
                 data[k]=rdata[l];
                 l++;
             }
         }
     }
     serial_data[1] = (int)data[4];//颜色

     serial_data[4] = (int)data[9]; //英雄  1   0  1 2
     serial_data[5] = (int)data[10];  //工程 2
     serial_data[6] = (int)data[11];   //步兵 3
     serial_data[7] = (int)data[12];   //步兵 4
     serial_data[8] = (int)data[13];   //步兵 5
     serial_data[9] = (int)data[14];   //前哨
}
