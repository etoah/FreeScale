#include"AngelControl.h"
#include<math.h>






/*************************************************************/
/*                 读陀螺仪的从值并均值滤波                      */
/*************************************************************/
int AD_filter(char nChannel)
{
	 long sum=0;
	 int result;

	  
		 ATD0CTL3 = 0x80;   //转换一个通道16次
		 ATD0CTL5 =nChannel;   //转换AD0
		 
		 while(!ATD0STAT0_SCF); 
		 sum+=ATD0DR0;
		 sum+=ATD0DR1;
		 sum+=ATD0DR2;
		 sum+=ATD0DR3;
		 sum+=ATD0DR4;
		 sum+=ATD0DR5;
		 sum+=ATD0DR6;
		 sum+=ATD0DR7;
		 sum+=ATD0DR8;
		 sum+=ATD0DR9;
		 sum+=ATD0DR10;
		 sum+=ATD0DR11;
		 sum+=ATD0DR12;
		 sum+=ATD0DR13;
		 sum+=ATD0DR14;
		 sum+=ATD0DR15;
	
     result=(int)(sum>>4);  //除16求平均值
     return result	 ;
	 
}


/*************************************************************/
/*                         车模倾角计算                      */
/*************************************************************/
void CountCarAngel() 
{
  float lnDeltaValue=0,CarAcceVal,CarGyroVal;   //加速度与角速度的偏差，加速度，角速度


  
  g_fCarAcceVal = (g_fCarAcceVal- CV_ACCE_OFFSET );   //去偏值
  CarAcceVal = (float)(g_fCarAcceVal * CAR_ACCE_INIT);     //归一化到0到90
  D_Acce=g_fCarAcceVal = CarAcceVal;				   //归一化后加速度值	        */
  
  
  
   
  
  g_nCarGyroVal = (int)(g_nCarGyroVal - CV_GYRO_ZERO);     //去零偏
  CarGyroVal = (float)(g_nCarGyroVal * CAR_GYRO_INIT);     //归一化到？？
  g_nCarGyroVal = (int)CarGyroVal;				   //归一化后舵螺仪值

  
  g_fCarAngle = (g_lnCarAngleSigma /1024);	   //车最终角度
   
  lnDeltaValue = g_fCarAcceVal- g_fCarAngle;	//加速度角速度偏差 = 加速度 - 角速度
  lnDeltaValue = lnDeltaValue * CAR_ACCE_RAT;   //加速度角速度偏差 = 加速度角速度偏差 * 加速度反馈比例
  
  g_lnCarAngleSigma += (g_nCarGyroVal + lnDeltaValue); //积分器
  
}



 /*************************************************************/
/*                       车模直立控制                        */
/*************************************************************/
void CarAdjust() 
{
  long nP,nD,nSpeed;	  //比例部分，微分部分，速度

  
  nP = g_fCarAngle;    	  //比例部分 = 车子最终角速度
  
  nP = (long)(nP * CAR_P_RATE);		 //最终比例部分
/*  if((g_nCarAngle<2)||(g_nCarAngle>-2))
  nP/=1.5;								   //比例优化 */
  
  nD = g_nCarGyroVal >> 5;			 //微分部分
  
  nD = (int)(nD*CAR_D_RATE);		 //最终微分部分
  
  nSpeed = nP + nD;					 //速度 = 比例 + 微分
  
  if(nSpeed > MOTOR_SPEED_MAX) nSpeed = MOTOR_SPEED_MAX;    // 限幅
  
  else if(nSpeed < MOTOR_SPEED_MIN) nSpeed = MOTOR_SPEED_MIN ;  // 限幅
  
  
  
 
  
  AngelControlOutput = (int)nSpeed; 
  

  
}


/*************************************************************/
/*                      得到当前温度陀螺仪的零偏值                */
/*************************************************************/
void Get_Gyro_Offset()
{
 
 long sum1=0,sum2=0;
 int i,j=1800;
 
 while(j--)						//延时
  delay();
  
 PTH=0x00;

 	
	 for(i=16384;i;i--)
	 {
	     //对16次的转换结果平均
	           sum2+= AD_filter(0x21);
		  sum1+= AD_filter(0x20);
	 }
	 CV_GYRO_ZERO=(int)(sum1>>14);   //陀螺仪零偏
	 DireGyroValOffSet=(int)(sum2>>14);   //陀螺仪零偏
	 

 
 
 PTH=0xF0;                    //转换结束，LED灭
  
 j=2500;                        //延时
 while(j--)
   delay();
 
 
}
  
