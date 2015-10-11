#include"SpeedControl.h"
#include"system.h"
#include<math.h>




 /***********************************
delay_us
************************************/
void delay_us(unsigned int number) 
{
   uint j;
  for(j=0;j<number;j++);    
}
 


unsigned int SSI(unsigned char add)
{ uchar k;
  uint dat1=0;
 if(add==0) 
 {
   CSn_L=0;
 }
 else 
 { 
  CSn_R=0;
 }   
  delay_us(2);  	//这个时间调一下吧
  
  for(k=0;k<10;k++)	   //读10位数据
   { CLK=0;
     delay_us(1);
     CLK=1;
     delay_us(1);
     dat1=(dat1<<1)+DAT;//C5
    }  
    
  for(k=0;k<6;k++)//  厂家调试位
   { CLK=0;
     delay_us(1);
     CLK=1;
     delay_us(1);
    }
   CSn_R=1;
   CSn_L=1;
  return (dat1);
} 
 /*************************************************************/
/*                       计算速度                              */
/*************************************************************/
/***************************************************************************************
定时计数器
|N_num[x]-L_num[x]|<512 ;  N_num[x]> L_num[x] 顺时针旋转
                           N_num[x]< L_num[x] 逆时针旋转
                           
|N_num[x]-L_num[x]|>512 ;  N_num[x]> L_num[x] 逆时针旋转
                           N_num[x]< L_num[x] 顺时针旋转
*******************************************************************************************/

int CalcSpeed(void)
{
  int Dist[2]=
  {0
  },VL,VR,Sp;

  Dist[0]=SSI(0);  //读取当前位移数据
 Dist[1]=SSI(1);  //读取当前位移数据
 
 
 VL = (Dist[0]-Last_Dist[0]);
  VR = (Last_Dist[1]-Dist[1]);

  
  
  
  
 Last_Dist[0]=Dist[0]; //上传上次位移数据
 Last_Dist[1]=Dist[1]; //上传上次位移数据
 
 
 
   
    if(VL>512) 
   VL = VL-1023;
  else if(VL<-512)
   VL = VL+1023;
    
    if(VR>512) 
   VR = VR-1023;
 else if(VR<-512)
   VR = VR+1023;
 
  Sp=(VR+VL);

 return Sp;					 //返回结果
  
}

 /*************************************************************/
/*                    Bang-Bang　控制                           */
/*************************************************************/
void BangBangControl(void)
{

    int nSpeedDiff=0;
    
    nSpeedDiff=g_nCarSpeedSet-NowSpeed;
    if((nSpeedDiff>BNAGBANGVALUE||nSpeedDiff<0-BNAGBANGVALUE)&&(SpeedSetCount>BANGBANGTIME))
    {   
    
    	 if(g_fSpeedMotorOut >0)g_fSpeedMotorOut +=10000;
   		 else g_fSpeedMotorOut -=10000;
    
    	  
    	    SpeedSetCount=0;
    	    
   	}


							  
}




 /*************************************************************/
/*                    速度控制模糊PD                            */
/*************************************************************
void SFuzzyPD(SpeedDiff,fD)
{
    nPval=(int)SpeedDiff/120);
     if(nPval>PB)nPval=PB ;//模糊化
     else if(nPval<NB)nPval=NB;
	 
 /*    nDval=(int)(fDvalue/DIRE_DRAT) ;
     if(nDval>PB)nDval=PB; //模糊化
     else if(nDval<NB)nDval=NB; //模糊化
     nDval+=3;  
     nPval+=3;
     nP=DP[nDval][nPval];	         //查询规则表
     switch(nP) 
     {
      case O:
            DIR_CONTROL_P=1000;break;
      case PS:
            DIR_CONTROL_P=2500;break;
      case PM:
            DIR_CONTROL_P=3000;break;
      case PB:
            DIR_CONTROL_P=4000;break;
            
     }
}      */

 /*************************************************************/
/*                    速度控制PID                            */
/*************************************************************/

void SpeedControl(void)
{
    float SpeedDiff;        //分别为当前速度，速度偏差
    float fP=0,fI=0,fD;		         //速度比例，速度积分 
    char i;
   
 
           NowSpeed-=CalcSpeed();        //获取当前速度
         
            SpeedDiff=g_nCarSpeedSet-NowSpeed;   //速度偏差 = 设定速度 -  当前速度
            fD= (SpeedDiff-g_fLastSpeedDiff[9]);
        //    SFuzzyPD(SpeedDiff,fD);
          
        
               fP = SpeedDiff * KP_SPEED;				 //比例部分 = 速度偏差 * 速度积分比例
                fI = SpeedDiff * KI_SPEED;				 //积分部分 = 速度偏差 * 速度微分比例	    */
                fD*=KD_SPEED;
                for(i=9;i>0;i--)
                {
                	   g_fLastSpeedDiff[i]=  g_fLastSpeedDiff[i-1]   ;
                }
                g_fLastSpeedDiff[0]=SpeedDiff ;
                
         
         if(g_fSpeedControlInter>SPEED_CONTROL_MAXVAL)
            g_fSpeedControlInter=SPEED_CONTROL_MAXVAL;
         else if(g_fSpeedControlInter<SPEED_CONTROL_MINVAL)
            g_fSpeedControlInter=SPEED_CONTROL_MINVAL; 
          if(g_fSpeedOutNew>SPEED_CONTROL_MAXVAL)
            g_fSpeedOutNew=SPEED_CONTROL_MAXVAL;
         else if(g_fSpeedOutNew<SPEED_CONTROL_MINVAL)
            g_fSpeedOutNew=SPEED_CONTROL_MINVAL; 
         
         g_fSpeedControlInter += fI;			 //积分
         g_fSpeedOutOld = g_fSpeedOutNew;		 //老速度 = 新速度
         g_fSpeedOutNew = g_fSpeedControlInter + fP;   //新速度 = 比例部分 + 积分
       
         
   
         
         g_nSpeedControlPeriod=0; 				 //速度控制计数清0
 
     
  
}





/*************************************************************/
/*                    速度均匀输出                             */
/*************************************************************/

void SpeedControlOut()
{  
  float fvalue;
  fvalue = g_fSpeedOutNew - g_fSpeedOutOld;	 //速度变化 = 新速度 - 旧速度= 比例部分 + 积分
  if(fvalue > 0)
       fvalue += SPEED_CONTROL_OFFSET;		 //速度变化为正 加上一点
  else if(fvalue < 0)
       fvalue -= SPEED_CONTROL_OFFSET;		 //速度变化为负 减去一点
  //速度输出 = 速度变化 * （速度控制计数 + 1）/10 + 老速度 
  g_fSpeedMotorOut = fvalue * (g_nSpeedControlPeriod+1) /9 + g_fSpeedOutOld;
  
  if(g_fSpeedMotorOut > SPEED_CONTROL_MAXVAL) g_fSpeedMotorOut =SPEED_CONTROL_MAXVAL;	      // 限幅
  else if(g_fSpeedMotorOut < SPEED_CONTROL_MINVAL) g_fSpeedMotorOut = SPEED_CONTROL_MINVAL;	   
  
}



