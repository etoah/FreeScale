#include "derivative.h"      /* derivative-specific definitions */
/***********************************/
/*             方向控制宏         */
/*********************************/
#define DIR_LEFT_OFFSET 0 
#define DIR_RIGHT_OFFSET 0 
#define LEFT_RIGHT_MINIMUM 100
#define DIRECTION_CONTROL_COUNT 1		   //方向控制计数
#define DIRECTION_CONTROL_DEADVALUE 10
#define DIRECTION_CONTROL_OUT_MAX 720
#define DIRECTION_CONTROL_OUT_MIN -720
#define DIRE_PRAT  7
#define DIRE_DRAT  200
#define INDA_RAT 0
//#define DIR_CONTROL_P 0.01
//#define DIR_CONTROL_D 0
//#define DIR_GYRO_OFFSET 1400
//#define DireGyroValOffSet  1423
//////////////////////////////////////
/*************************************************************/
/*                     方向控制全局变量                      */
/*************************************************************/
extern float g_fLeftVoltageSigma,g_fRightVoltageSigma,g_fDirectionControlOutOld , 
      g_fDirectionControlOutNew,g_fDirectionControlOut,DirectionSpeedControlOut,
      g_nDireGyroVal,g_fLastValue,NowDirection;
extern char g_cDirectionCount;      
///////////////////////////////////  ////////////////////////////////     
      
     


extern int VOLTAGE_LEFT,VOLTAGE_RIGHT,L[6],DireGyroValOffSet,max;

extern float g_fSpeedControlInter,g_fSpeedOutOld,g_fSpeedOutNew,g_fSpeedMotorOut;

extern unsigned char g_ncount,g_nSpeedControlPeriod;

extern float DIR_CONTROL_P,DIR_CONTROL_D,DIR_CONTROL_D2,DIR_SPEED_CONTROL,DIR_GYRO_OFFSET;
extern char DP[7][7],DD[7][7];

/*************************************************************/
/*                  方向电压积分                             */
/************************************************************/
void DirectionVoltageSigma(void) ;
/************************************************************/
/*                    方向控制PID                           */
/************************************************************/
void DirectionControl(void); 


/*************************************************************/
/*                 方向控制均匀输出                             */
/*************************************************************/
void DirectionControlOutput(void) ;



/************************************************************/
/*                    得到所有电感的值

电感AD

　　AD８　左后电感　
　　AD９　前左水平电感�	
　AD１０　前左斜上电感�
　AD１１　前右水平电感
　AD１２　前右斜上电感
　AD１３　右后电感                                                                    

                                                           */
/************************************************************/


void GetAllInductance(void);