#include "derivative.h"      /* derivative-specific definitions */





#define BANGBANGTIME    300  
#define BNAGBANGVALUE   300
#define SPEED_CONTROL_MAXVAL 850
#define SPEED_CONTROL_MINVAL -1600

#define SPEEDINTERMAX   20 //�������Ż���Сֵ

#define SPEED_CONTROL_PERIOD 100//�ٶȿ�������
#define ENCODE_CONSTANT 100      // ��������դ��
#define CAR_SPEED_CONATANT  10000/SPEED_CONTROL_PERIOD/ENCODE_CONSTANT // ��λת������ֵ
#define SPEED_CONTROL_OFFSET 30
#define uchar unsigned char
#define uint unsigned int
//���Ŷ���


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
/*                    Bang-Bang������                           */
/*************************************************************/
void BangBangControl(void);

/*********************************************************************
������SSI����
�������ܣ���ȡ������λ������
**********************************************************************/
unsigned int SSI(unsigned char add);


 /*************************************************************/
/*                       �����ٶ�                              */
/*************************************************************/

int CalcSpeed(void);


 /*************************************************************/
/*                    �ٶȿ���PID                            */
/*************************************************************/

void SpeedControl(void);



/*************************************************************/
/*                    �ٶȾ������                             */
/*************************************************************/

void SpeedControlOut(void);

/*************************************************************/
/*                       �����ٶȸ���                          */
/*************************************************************/
void SpeedSet();
