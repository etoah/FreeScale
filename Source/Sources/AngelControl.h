#include "derivative.h"      /* derivative-specific definitions */

//  归一化的值
//#define CAR_GYRO_INIT  1.25
#define CAR_ACCE_INIT  0.14
//#define CAR_ACCE_RAT 0.3  //加速度反馈比例  角度补偿时间常数  
#define MOTOR_SPEED_MAX 1800
#define MOTOR_SPEED_MIN -1800
#define CV_ACCE_OFFSET -1152 //1106    不加后传感器
//#define ZACCE_OFFSET   -1155
//#define CV_GYRO_ZERO 1400     //1300
//#define CAR_D_RATE 20 //角速度比例
//#define CAR_P_RATE 147 //角度比例
extern float CAR_D_RATE;      //角速度比例 9
extern float CAR_P_RATE,CAR_GYRO_INIT,CAR_ACCE_RAT;     //角度比例65


extern int g_nCarGyroVal,nZvalue,DireGyroValOffSet,D_Acce;
extern int CV_GYRO_ZERO;
extern float g_fCarAcceVal;
extern float g_fCarAngle;
extern int AngelControlOutput;//,CV_ACCE_OFFSET;		//电机输出
extern long g_lnCarAngleSigma; //积分值


/*************************************************************/
/*                 读陀螺仪的从值并均值滤波                      */
/*************************************************************/
int AD_filter(int nChannel);



/*************************************************************/
/*                         车模倾角计算                      */
/*************************************************************/
void CountCarAngel(void); 




 /*************************************************************/
/*                       车模直立控制                        */
/*************************************************************/
void CarAdjust(void); 



/*************************************************************/
/*                      得到当前温度陀螺仪的零偏值                */
/*************************************************************/
void Get_Gyro_Offset(void);
