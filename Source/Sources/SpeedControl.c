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
  delay_us(2);  	//���ʱ���һ�°�
  
  for(k=0;k<10;k++)	   //��10λ����
   { CLK=0;
     delay_us(1);
     CLK=1;
     delay_us(1);
     dat1=(dat1<<1)+DAT;//C5
    }  
    
  for(k=0;k<6;k++)//  ���ҵ���λ
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
/*                       �����ٶ�                              */
/*************************************************************/
/***************************************************************************************
��ʱ������
|N_num[x]-L_num[x]|<512 ;  N_num[x]> L_num[x] ˳ʱ����ת
                           N_num[x]< L_num[x] ��ʱ����ת
                           
|N_num[x]-L_num[x]|>512 ;  N_num[x]> L_num[x] ��ʱ����ת
                           N_num[x]< L_num[x] ˳ʱ����ת
*******************************************************************************************/

int CalcSpeed(void)
{
  int Dist[2]=
  {0
  },VL,VR,Sp;

  Dist[0]=SSI(0);  //��ȡ��ǰλ������
 Dist[1]=SSI(1);  //��ȡ��ǰλ������
 
 
 VL = (Dist[0]-Last_Dist[0]);
  VR = (Last_Dist[1]-Dist[1]);

  
  
  
  
 Last_Dist[0]=Dist[0]; //�ϴ��ϴ�λ������
 Last_Dist[1]=Dist[1]; //�ϴ��ϴ�λ������
 
 
 
   
    if(VL>512) 
   VL = VL-1023;
  else if(VL<-512)
   VL = VL+1023;
    
    if(VR>512) 
   VR = VR-1023;
 else if(VR<-512)
   VR = VR+1023;
 
  Sp=(VR+VL);

 return Sp;					 //���ؽ��
  
}

 /*************************************************************/
/*                    Bang-Bang������                           */
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
/*                    �ٶȿ���ģ��PD                            */
/*************************************************************
void SFuzzyPD(SpeedDiff,fD)
{
    nPval=(int)SpeedDiff/120);
     if(nPval>PB)nPval=PB ;//ģ����
     else if(nPval<NB)nPval=NB;
	 
 /*    nDval=(int)(fDvalue/DIRE_DRAT) ;
     if(nDval>PB)nDval=PB; //ģ����
     else if(nDval<NB)nDval=NB; //ģ����
     nDval+=3;  
     nPval+=3;
     nP=DP[nDval][nPval];	         //��ѯ�����
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
/*                    �ٶȿ���PID                            */
/*************************************************************/

void SpeedControl(void)
{
    float SpeedDiff;        //�ֱ�Ϊ��ǰ�ٶȣ��ٶ�ƫ��
    float fP=0,fI=0,fD;		         //�ٶȱ������ٶȻ��� 
    char i;
   
 
           NowSpeed-=CalcSpeed();        //��ȡ��ǰ�ٶ�
         
            SpeedDiff=g_nCarSpeedSet-NowSpeed;   //�ٶ�ƫ�� = �趨�ٶ� -  ��ǰ�ٶ�
            fD= (SpeedDiff-g_fLastSpeedDiff[9]);
        //    SFuzzyPD(SpeedDiff,fD);
          
        
               fP = SpeedDiff * KP_SPEED;				 //�������� = �ٶ�ƫ�� * �ٶȻ��ֱ���
                fI = SpeedDiff * KI_SPEED;				 //���ֲ��� = �ٶ�ƫ�� * �ٶ�΢�ֱ���	    */
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
         
         g_fSpeedControlInter += fI;			 //����
         g_fSpeedOutOld = g_fSpeedOutNew;		 //���ٶ� = ���ٶ�
         g_fSpeedOutNew = g_fSpeedControlInter + fP;   //���ٶ� = �������� + ����
       
         
   
         
         g_nSpeedControlPeriod=0; 				 //�ٶȿ��Ƽ�����0
 
     
  
}





/*************************************************************/
/*                    �ٶȾ������                             */
/*************************************************************/

void SpeedControlOut()
{  
  float fvalue;
  fvalue = g_fSpeedOutNew - g_fSpeedOutOld;	 //�ٶȱ仯 = ���ٶ� - ���ٶ�= �������� + ����
  if(fvalue > 0)
       fvalue += SPEED_CONTROL_OFFSET;		 //�ٶȱ仯Ϊ�� ����һ��
  else if(fvalue < 0)
       fvalue -= SPEED_CONTROL_OFFSET;		 //�ٶȱ仯Ϊ�� ��ȥһ��
  //�ٶ���� = �ٶȱ仯 * ���ٶȿ��Ƽ��� + 1��/10 + ���ٶ� 
  g_fSpeedMotorOut = fvalue * (g_nSpeedControlPeriod+1) /9 + g_fSpeedOutOld;
  
  if(g_fSpeedMotorOut > SPEED_CONTROL_MAXVAL) g_fSpeedMotorOut =SPEED_CONTROL_MAXVAL;	      // �޷�
  else if(g_fSpeedMotorOut < SPEED_CONTROL_MINVAL) g_fSpeedMotorOut = SPEED_CONTROL_MINVAL;	   
  
}



