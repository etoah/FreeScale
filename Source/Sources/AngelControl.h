#include "derivative.h"      /* derivative-specific definitions */

//  ��һ����ֵ
//#define CAR_GYRO_INIT  1.25
#define CAR_ACCE_INIT  0.14
//#define CAR_ACCE_RAT 0.3  //���ٶȷ�������  �ǶȲ���ʱ�䳣��  
#define MOTOR_SPEED_MAX 1800
#define MOTOR_SPEED_MIN -1800
#define CV_ACCE_OFFSET -1152 //1106    ���Ӻ󴫸���
//#define ZACCE_OFFSET   -1155
//#define CV_GYRO_ZERO 1400     //1300
//#define CAR_D_RATE 20 //���ٶȱ���
//#define CAR_P_RATE 147 //�Ƕȱ���
extern float CAR_D_RATE;      //���ٶȱ��� 9
extern float CAR_P_RATE,CAR_GYRO_INIT,CAR_ACCE_RAT;     //�Ƕȱ���65


extern int g_nCarGyroVal,nZvalue,DireGyroValOffSet,D_Acce;
extern int CV_GYRO_ZERO;
extern float g_fCarAcceVal;
extern float g_fCarAngle;
extern int AngelControlOutput;//,CV_ACCE_OFFSET;		//������
extern long g_lnCarAngleSigma; //����ֵ


/*************************************************************/
/*                 �������ǵĴ�ֵ����ֵ�˲�                      */
/*************************************************************/
int AD_filter(int nChannel);



/*************************************************************/
/*                         ��ģ��Ǽ���                      */
/*************************************************************/
void CountCarAngel(void); 




 /*************************************************************/
/*                       ��ģֱ������                        */
/*************************************************************/
void CarAdjust(void); 



/*************************************************************/
/*                      �õ���ǰ�¶������ǵ���ƫֵ                */
/*************************************************************/
void Get_Gyro_Offset(void);
