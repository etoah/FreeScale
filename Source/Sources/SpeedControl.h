#include "derivative.h"      /* derivative-specific definitions */





#define BANGBANGTIME    300  
#define BNAGBANGVALUE   300
#define SPEED_CONTROL_MAXVAL 850
#define SPEED_CONTROL_MINVAL -1600

#define SPEEDINTERMAX   20 //积分项优化最小值

#define SPEED_CONTROL_PERIOD 100//速度控制周期
#define ENCODE_CONSTANT 100      // 编码器光栅数
#define CAR_SPEED_CONATANT  10000/SPEED_CONTROL_PERIOD/ENCODE_CONSTANT // 单位转化比例值
#define SPEED_CONTROL_OFFSET 30
#define uchar unsigned char
#define uint unsigned int
//引脚定义


//#define KP_SPEED  5.0
//#define KI_SPEED  0.08
//#define KD_SPEED  0.0
#define SPEED 340


extern float KP_SPEED;
extern float KI_SPEED,KD_SPEED;

extern int SpeedSetCount;

extern float g_fSpeedControlInter,g_fLastSpeedDiff[10];
extern unsigned char g_nSpeedControlPeriod,ControlFlag;
extern int g_nCarSpeedSet,NowSpeed,Last_Dist[2];

extern float g_fSpeedControlInter,g_fSpeedOutOld,g_fSpeedOutNew,g_fSpeedMotorOut;



void delay_us(unsigned int number) ;




 /*************************************************************/
/*                    Bang-Bang　控制                           */
/*************************************************************/
void BangBangControl(void);

/*********************************************************************
编码器SSI程序
函数功能：读取传感器位移数据
**********************************************************************/
unsigned int SSI(unsigned char add);


 /*************************************************************/
/*                       计算速度                              */
/*************************************************************/

int CalcSpeed(void);


 /*************************************************************/
/*                    速度控制PID                            */
/*************************************************************/

void SpeedControl(void);



/*************************************************************/
/*                    速度均匀输出                             */
/*************************************************************/

void SpeedControlOut(void);

/*************************************************************/
/*                       调试速度给定                          */
/*************************************************************/
void SpeedSet();
