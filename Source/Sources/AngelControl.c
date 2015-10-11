#include"AngelControl.h"
#include<math.h>






/*************************************************************/
/*                 �������ǵĴ�ֵ����ֵ�˲�                      */
/*************************************************************/
int AD_filter(char nChannel)
{
	 long sum=0;
	 int result;

	  
		 ATD0CTL3 = 0x80;   //ת��һ��ͨ��16��
		 ATD0CTL5 =nChannel;   //ת��AD0
		 
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
	
     result=(int)(sum>>4);  //��16��ƽ��ֵ
     return result	 ;
	 
}


/*************************************************************/
/*                         ��ģ��Ǽ���                      */
/*************************************************************/
void CountCarAngel() 
{
  float lnDeltaValue=0,CarAcceVal,CarGyroVal;   //���ٶ�����ٶȵ�ƫ����ٶȣ����ٶ�


  
  g_fCarAcceVal = (g_fCarAcceVal- CV_ACCE_OFFSET );   //ȥƫֵ
  CarAcceVal = (float)(g_fCarAcceVal * CAR_ACCE_INIT);     //��һ����0��90
  D_Acce=g_fCarAcceVal = CarAcceVal;				   //��һ������ٶ�ֵ	        */
  
  
  
   
  
  g_nCarGyroVal = (int)(g_nCarGyroVal - CV_GYRO_ZERO);     //ȥ��ƫ
  CarGyroVal = (float)(g_nCarGyroVal * CAR_GYRO_INIT);     //��һ��������
  g_nCarGyroVal = (int)CarGyroVal;				   //��һ���������ֵ

  
  g_fCarAngle = (g_lnCarAngleSigma /1024);	   //�����սǶ�
   
  lnDeltaValue = g_fCarAcceVal- g_fCarAngle;	//���ٶȽ��ٶ�ƫ�� = ���ٶ� - ���ٶ�
  lnDeltaValue = lnDeltaValue * CAR_ACCE_RAT;   //���ٶȽ��ٶ�ƫ�� = ���ٶȽ��ٶ�ƫ�� * ���ٶȷ�������
  
  g_lnCarAngleSigma += (g_nCarGyroVal + lnDeltaValue); //������
  
}



 /*************************************************************/
/*                       ��ģֱ������                        */
/*************************************************************/
void CarAdjust() 
{
  long nP,nD,nSpeed;	  //�������֣�΢�ֲ��֣��ٶ�

  
  nP = g_fCarAngle;    	  //�������� = �������ս��ٶ�
  
  nP = (long)(nP * CAR_P_RATE);		 //���ձ�������
/*  if((g_nCarAngle<2)||(g_nCarAngle>-2))
  nP/=1.5;								   //�����Ż� */
  
  nD = g_nCarGyroVal >> 5;			 //΢�ֲ���
  
  nD = (int)(nD*CAR_D_RATE);		 //����΢�ֲ���
  
  nSpeed = nP + nD;					 //�ٶ� = ���� + ΢��
  
  if(nSpeed > MOTOR_SPEED_MAX) nSpeed = MOTOR_SPEED_MAX;    // �޷�
  
  else if(nSpeed < MOTOR_SPEED_MIN) nSpeed = MOTOR_SPEED_MIN ;  // �޷�
  
  
  
 
  
  AngelControlOutput = (int)nSpeed; 
  

  
}


/*************************************************************/
/*                      �õ���ǰ�¶������ǵ���ƫֵ                */
/*************************************************************/
void Get_Gyro_Offset()
{
 
 long sum1=0,sum2=0;
 int i,j=1800;
 
 while(j--)						//��ʱ
  delay();
  
 PTH=0x00;

 	
	 for(i=16384;i;i--)
	 {
	     //��16�ε�ת�����ƽ��
	           sum2+= AD_filter(0x21);
		  sum1+= AD_filter(0x20);
	 }
	 CV_GYRO_ZERO=(int)(sum1>>14);   //��������ƫ
	 DireGyroValOffSet=(int)(sum2>>14);   //��������ƫ
	 

 
 
 PTH=0xF0;                    //ת��������LED��
  
 j=2500;                        //��ʱ
 while(j--)
   delay();
 
 
}
  
