#include <hidef.h>      /* common defines and macros */
#include "system.h"
#include "AngelControl.h"
#include "DirectionControl.h"
#include "SpeedControl.h"
#include "Debug.h"



/***********************************/
/*        �Ƕȿ���ȫ�ֱ���        */
/*********************************/


int g_nCarGyroVal, CV_GYRO_ZERO = 1460,
                   NowAngel, AngelControlOutput = 0, AD_Gyro = 0, nZvalue, DireGyroValOffSet;		//���ҵ�����
long g_lnCarAngleSigma = 0;   //����ֵ
float CAR_D_RATE = 92, g_fCarAngle, g_fCarAcceVal;    //���ٶȱ���
float CAR_P_RATE = 100, CAR_GYRO_INIT = 0.8, CAR_ACCE_RAT = 1.2; //�Ƕȱ���



/*************************************************************/
/*                     �������ȫ�ֱ���                      */
/*************************************************************/
float g_fLeftVoltageSigma = 0, g_fRightVoltageSigma = 0, g_fDirectionControlOutOld = 0,
      g_fDirectionControlOutNew = 0, g_fDirectionControlOut = 0,
      g_nDireGyroVal = 0, g_fLastValue = 0, NowDirection = 0, DirectionSpeedControlOut = 0;
char g_cDirectionCount = 0;
float DIR_CONTROL_P = 2750, DIR_CONTROL_D = 1.73,DIR_SPEED_CONTROL = 650;
int L[6] = {0};
int VOLTAGE_LEFT = 0, VOLTAGE_RIGHT = 0;

char DP[7][7]=		     //��������Ӧģ��PD���ƹ���P
{
 PB, PB, PM, PM, PS,   O,  PS,   
 PB, PB, PM,PS  ,PS,   O, PS,
 PM, PM, PM,PS,  O,   PS, PS,
 PM, PM, PS,O ,  PS,  PM, PM,
 PS,  PS,O, PS,  PS,  PM, PM,
 PS,  O, PS, PM,  PM,  PM, PB,
 O,   O, PM,PM,  PM,   PB,   PB    
} ;
char DD[7][7]=	            //��������Ӧģ��PD���ƹ���D
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
/*                     �ٶȿ���ȫ�ֱ���                      */
/*************************************************************/
unsigned char g_nSpeedControlPeriod = 0;
float KP_SPEED = 3.3;
float KI_SPEED = 0.0001;
float KD_SPEED = 22;
int SpeedSetCount = 0;
int g_nCarSpeedSet = 0, NowSpeed = 0, Last_Dist[2], PWMR, PWML;
float g_fSpeedControlInter = 0, g_fSpeedOutOld = 0, g_fSpeedOutNew = 0,g_fLastSpeedDiff[10]=0, g_fSpeedMotorOut = 0;

/*************************************************************/
/*                      ���Գ����������ר����                   */
/*************************************************************/
 unsigned char tmp;
int D_Gyro, D_AcceAD, D_Acce,max=0;



/*************************************************************/
/*                     ���Զ������                          */
/*************************************************************/

unsigned char g_nTaskCount = 0, ControlFlag;               //�������





/*************************************************************/
/*                       �ٶȸ���                          */
/*************************************************************/
void SpeedSet()
{

    if((NowAngel > 4 || NowAngel < -4) || (NowDirection < 0.2))
        g_nCarSpeedSet = 155;

    else
        g_nCarSpeedSet = 165;


}




/*************************************************************/
/*                         ������                            */
/*************************************************************/
void main(void)
{

    int i;
    /* put your own code here */
    INIT_PLL();		   //���໷��ʼ��
    IO_init();           //IO��ʼ��
    PWM_Init();		   //PWM��ʼ��
    INIT_AD12BIT();	   //12λAD��ʼ��
    //Count_Init();		   //��������ʼ��
    PUCR_PUPBE = 1;		 //���е�io�ܽŶ�����


    Get_Gyro_Offset();   //�õ������ǵ���ƫ

    EnableInterrupts;
    PIT_init();          //��ʱ�жϳ�ʼ������ 1MS��ʱ�ж�

    //BeginBalance();

    SpeedSetCount = 0; 
       i = 800;			 //��ʱ
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
        //  CarTumble() ;// �������

    } /* loop forever */
    /* please make sure that you never leave main */
}





/*************************************************************/
/*                                     �жϷ������                                            */
/*************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt 66 PIT0(void)
{


    PITTF_PTF0 = 1;          //���жϱ�־λ
   // SpeedSetCount++ ;        //�ٶȸ�������

    switch (g_nTaskCount++)
    {

    case 0:
    {


        g_nCarGyroVal = g_nCarGyroVal + AD_filter(0x20);			 //�������ǵĴ�ֵ����ֵ�˲�
        g_fCarAcceVal = g_fCarAcceVal - AD_filter(0x22);                       //��Z���ֵ

        AD_Gyro = g_nCarGyroVal = g_nCarGyroVal >> 1;
        D_AcceAD = g_fCarAcceVal = (g_fCarAcceVal /2) ; //��2
        
        CountCarAngel();  //��ģ��Ǽ���
        CarAdjust();	    //��ģֱ������
        if(!PORTA_PA7)MotorSpeedOut();
        break ;
    }

    case 1:
    {
        g_nCarGyroVal = AD_filter(0x20);			 //�������ǵĴ�ֵ����ֵ�˲�
        g_fCarAcceVal =0 - AD_filter(0x22);   //��Z���ֵ
        
        g_nSpeedControlPeriod++;         //�ٶȿ��Ƽ���

        if(g_nSpeedControlPeriod == 9)
        {

       //    GetAllInductance();    //�õ����е�е�ֵ��
           //  DirectionVoltageSigma();//�����ѹ����
            //    SpeedSet();     //�ж������������������ٶ�

            SpeedControl();			//�ٶȿ���
            g_nSpeedControlPeriod = 0;
        }


        else if(g_nSpeedControlPeriod % 3 == 1)
        {


	    GetAllInductance();    //�õ����е�е�ֵ? */
            DirectionVoltageSigma();//�����ѹ����
   
	   	  if(g_nSpeedControlPeriod == 4)
	        {
		     NowSpeed = -CalcSpeed();      //��ȡ��ǰ�ٶ�
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

       if(!PORTA_PA6)DirectionControlOutput();   			   //������ƾ������
        SpeedControlOut();//�ٶȾ������

        g_nTaskCount = 0;
        break ;
    }

    default: ;     //ǰLED����:����ִ�д���

    } 
  //  BangBangControl();





}

