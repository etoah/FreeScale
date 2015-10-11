#include <hidef.h>      /* common defines and macros */
#include "system.h"
#include "AngelControl.h"
#include "DirectionControl.h"
#include "SpeedControl.h"
#include "Debug.h"



/***********************************/
/*        角度控制全局变量        */
/*********************************/


int g_nCarGyroVal, CV_GYRO_ZERO = 1460,
                   NowAngel, AngelControlOutput = 0, AD_Gyro = 0, nZvalue, DireGyroValOffSet;		//左右电机输出
long g_lnCarAngleSigma = 0;   //积分值
float CAR_D_RATE = 92, g_fCarAngle, g_fCarAcceVal;    //角速度比例
float CAR_P_RATE = 100, CAR_GYRO_INIT = 0.8, CAR_ACCE_RAT = 1.2; //角度比例



/*************************************************************/
/*                     方向控制全局变量                      */
/*************************************************************/
float g_fLeftVoltageSigma = 0, g_fRightVoltageSigma = 0, g_fDirectionControlOutOld = 0,
      g_fDirectionControlOutNew = 0, g_fDirectionControlOut = 0,
      g_nDireGyroVal = 0, g_fLastValue = 0, NowDirection = 0, DirectionSpeedControlOut = 0;
char g_cDirectionCount = 0;
float DIR_CONTROL_P = 2750, DIR_CONTROL_D = 1.73,DIR_SPEED_CONTROL = 650;
int L[6] = {0};
int VOLTAGE_LEFT = 0, VOLTAGE_RIGHT = 0;

char DP[7][7]=		     //方向自适应模糊PD控制规则P
{
 PB, PB, PM, PM, PS,   O,  PS,   
 PB, PB, PM,PS  ,PS,   O, PS,
 PM, PM, PM,PS,  O,   PS, PS,
 PM, PM, PS,O ,  PS,  PM, PM,
 PS,  PS,O, PS,  PS,  PM, PM,
 PS,  O, PS, PM,  PM,  PM, PB,
 O,   O, PM,PM,  PM,   PB,   PB    
} ;
char DD[7][7]=	            //方向自适应模糊PD控制规则D
{ 
   PB,PB, PM,PM,  PS,  O, O,
   PB,PB,  PM,PS,  PS, O, O,
   PB,PM, PS, PS, O, PS, PS, 
   PM,PM,PS,  O, PS, PM, PM, 
   PM,PS,  O,  PS, PS, PM, PB,
   O, O,   PS,PS,  PM, PB, PB,
   O,  O,  PS,PM ,PM,  PB, PB
} ;
///////////////////////////////////////////////////////////////////



/*************************************************************/
/*                     速度控制全局变量                      */
/*************************************************************/
unsigned char g_nSpeedControlPeriod = 0;
float KP_SPEED = 3.3;
float KI_SPEED = 0.0001;
float KD_SPEED = 22;
int SpeedSetCount = 0;
int g_nCarSpeedSet = 0, NowSpeed = 0, Last_Dist[2], PWMR, PWML;
float g_fSpeedControlInter = 0, g_fSpeedOutOld = 0, g_fSpeedOutNew = 0,g_fLastSpeedDiff[10]=0, g_fSpeedMotorOut = 0;

/*************************************************************/
/*                      调试程序变量定义专用区                   */
/*************************************************************/
 unsigned char tmp;
int D_Gyro, D_AcceAD, D_Acce,max=0;



/*************************************************************/
/*                     调试定义结束                          */
/*************************************************************/

unsigned char g_nTaskCount = 0, ControlFlag;               //任务计数





/*************************************************************/
/*                       速度给定                          */
/*************************************************************/
void SpeedSet()
{

    if((NowAngel > 4 || NowAngel < -4) || (NowDirection < 0.2))
        g_nCarSpeedSet = 155;

    else
        g_nCarSpeedSet = 165;


}




/*************************************************************/
/*                         主函数                            */
/*************************************************************/
void main(void)
{

    int i;
    /* put your own code here */
    INIT_PLL();		   //锁相环初始化
    IO_init();           //IO初始化
    PWM_Init();		   //PWM初始化
    INIT_AD12BIT();	   //12位AD初始化
    //Count_Init();		   //编码器初始化
    PUCR_PUPBE = 1;		 //所有的io管脚都上拉


    Get_Gyro_Offset();   //得到陀螺仪的零偏

    EnableInterrupts;
    PIT_init();          //定时中断初始化函数 1MS定时中断

    //BeginBalance();

    SpeedSetCount = 0; 
       i = 800;			 //延时
    while(i--)delay();
    ControlFlag = 1;
    for(;;)
    {
        _FEED_COP(); /* feeds the dog */
        if(!PORTA_PA5)OutRoadTest();
        SwitchDebug();

       switch((int)(max/50)) 
       {
        case 34:PTH=0xFF;break;
        case 33:PTH=0xEE;break;
        case 32:PTH=0xDD;break;
        case 31:PTH=0xCD;break;
        case 30:PTH=0xBF;break;
        case 29:PTH=0xAF;break;
        case 28:PTH=0x9F;break;
        case 27:PTH=0x8F;break;
        case 26:PTH=0x7F;break;
        case 25:PTH=0x6F;break;
        case 24:PTH=0x5F;break;
        case 23:PTH=0x4F;break;
        case 22:PTH=0x3F;break;
        case 21:PTH=0x2F;break;
        case 20:PTH=0x1F;break;
        
        default:PTH=0x0F;
        

        
              
        
             
       }
            
        
        // SpeedSet();
        //  CarTumble() ;// 跌倒检测

    } /* loop forever */
    /* please make sure that you never leave main */
}





/*************************************************************/
/*                                     中断服务程序                                            */
/*************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt 66 PIT0(void)
{


    PITTF_PTF0 = 1;          //清中断标志位
   // SpeedSetCount++ ;        //速度给定计数

    switch (g_nTaskCount++)
    {

    case 0:
    {


        g_nCarGyroVal = g_nCarGyroVal + AD_filter(0x20);			 //读陀螺仪的从值并均值滤波
        g_fCarAcceVal = g_fCarAcceVal - AD_filter(0x22);                       //读Z轴的值

        AD_Gyro = g_nCarGyroVal = g_nCarGyroVal >> 1;
        D_AcceAD = g_fCarAcceVal = (g_fCarAcceVal /2) ; //除2
        
        CountCarAngel();  //车模倾角计算
        CarAdjust();	    //车模直立控制
        if(!PORTA_PA7)MotorSpeedOut();
        break ;
    }

    case 1:
    {
        g_nCarGyroVal = AD_filter(0x20);			 //读陀螺仪的从值并均值滤波
        g_fCarAcceVal =0 - AD_filter(0x22);   //读Z轴的值
        
        g_nSpeedControlPeriod++;         //速度控制计数

        if(g_nSpeedControlPeriod == 9)
        {

       //    GetAllInductance();    //得到所有电感的值，
           //  DirectionVoltageSigma();//方向电压积分
            //    SpeedSet();     //判断赛道环境，并给定速度

            SpeedControl();			//速度控制
            g_nSpeedControlPeriod = 0;
        }


        else if(g_nSpeedControlPeriod % 3 == 1)
        {


	    GetAllInductance();    //得到所有电感的值? */
            DirectionVoltageSigma();//方向电压积分
   
	   	  if(g_nSpeedControlPeriod == 4)
	        {
		     NowSpeed = -CalcSpeed();      //获取当前速度
	        }

        }

        else   if(g_nSpeedControlPeriod % 3 == 2)
        {


            g_nDireGyroVal = AD_filter(0x21);
            
            
            
            DirectionControl();

        }
        else   if(g_nSpeedControlPeriod % 3 == 0)
        {


        }

           g_cDirectionCount++;
        if(g_cDirectionCount == DIRECTION_CONTROL_COUNT)g_cDirectionCount = 0;

       if(!PORTA_PA6)DirectionControlOutput();   			   //方向控制均匀输出
        SpeedControlOut();//速度均匀输出

        g_nTaskCount = 0;
        break ;
    }

    default: ;     //前LED灯亮:程序执行错误

    } 
  //  BangBangControl();





}

