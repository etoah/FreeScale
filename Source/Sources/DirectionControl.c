#include"DirectionControl.h"
#include"system.h"
#include<math.h>
/*************************************************************/
/*                  �����ѹ����                             */
/************************************************************/
void DirectionVoltageSigma(void) 
{
    int nLeft, nRight;

/*
����AD��������С�
����AD����ǰ��б�ϵ�С�	
��AD������ǰ��ˮƽ��С�
��AD������ǰ��ˮƽ���
��AD������ǰ��б�ϵ��
��AD�������Һ���*/
/*	

  */


  	
	if(VOLTAGE_LEFT > DIR_LEFT_OFFSET) nLeft = VOLTAGE_LEFT - DIR_LEFT_OFFSET;
	else nLeft = 0;
	if(VOLTAGE_RIGHT > DIR_RIGHT_OFFSET) nRight = VOLTAGE_RIGHT - DIR_RIGHT_OFFSET;
	else nRight = 0;	    

	g_fLeftVoltageSigma += nLeft; 		  //��߷����ѹ����
	g_fRightVoltageSigma += nRight;		  //�ұ߷����ѹ����   
	

/*	if(NowDirection>0.5||NowDirection<-0.5)
	{
		
	g_fLeftVoltageSigma =g_fLeftVoltageSigma+   L[2]; 		  //��߷����ѹ����
	g_fRightVoltageSigma =	g_fRightVoltageSigma+L[4] ;
	}//�ұ߷����ѹ���� */	

}


/************************************************************/
/*                    �õ����е�е�ֵ

���AD

����AD��������С�
����AD����ǰ��ˮƽ��С	
��AD������ǰ��б�ϵ�С
��AD������ǰ��ˮƽ���
��AD������ǰ��б�ϵ��
��AD�������Һ���                                                                    

                                                           */
/************************************************************/


void GetAllInductance(void)
{

	int i;
	
	char tmp;
  	 //if(PORTA_PA5)
	 {
          ATD0CTL3 = 0x88;          //ÿ��ֻת��һ��ͨ��
            ATD0CTL5 = 0x09;          //ת��AD02
            while(!ATD0STAT2_CCF0);
           L[1]=VOLTAGE_LEFT = ATD0DR0;
            ATD0CTL5 = 0x0B;          //ת��AD03
            while(!ATD0STAT2_CCF0);
            L[3]=VOLTAGE_RIGHT = ATD0DR0;}//
        /*  else       
            {
            	
	ATD0CTL3 = 0x88;          //ÿ��ֻת��һ��ͨ��
	tmp=0x08;
	for(i=0;i<6;i++)
	{
		ATD0CTL5 = tmp++;          //ת��AD02
		while(!ATD0STAT2_CCF0);
		L[i]=ATD0DR0;*/

//	} 
	VOLTAGE_LEFT=L[1];//*(1-INDA_RAT)+L[0]*INDA_RAT;
	VOLTAGE_RIGHT=L[3];//*(1-INDA_RAT)+L[5]*INDA_RAT;
        //   }
}


/************************************************************/
/*                                  	����Ӧģ��PD                                    */
/************************************************************/
void DireFuzzyPD(float fDvalue) 
{
    int nPval,nDval,nP,nD;
	
     nPval=(int)(NowDirection*DIRE_PRAT);
     if(nPval>PB)nPval=PB ;//ģ����
     else if(nPval<NB)nPval=NB;
	 
     nDval=(int)(fDvalue/DIRE_DRAT) ;
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
    //  DIR_CONTROL_P=1550+2560*(nP)/PB;	 //������
      
      
      nD=DD[nDval][nPval];	         //��ѯ�����
/*            switch(nD) 
     {
      case O:
            DIR_CONTROL_D=1.55;break;
      case PS:
            DIR_CONTROL_D=1.82;break;
      case PM:
            DIR_CONTROL_D=2.0;break;
      case PB:
            DIR_CONTROL_D=2.4;break;
            
     }//*/
  DIR_CONTROL_D=1.55+0.85*(nD)/PB;	 //������   */
     
   }
									   
/************************************************************/
/*                    �������PD                           */
/************************************************************/
void DirectionControl(void) 
{
	float fLeftRightAdd, fLeftRightSub, fValue,fDvalue=0;
	int nLeft, nRight;

/*	if(g_nDirectionControlFlag == 0) {
		g_fDirectionControlOut = 0;
		g_fDirectionControlOutOld = g_fDirectionControlOutNew = 0;
		return;
	}  */

	//--------------------------------------------------------------
	
	nLeft = (int)(g_fLeftVoltageSigma /= DIRECTION_CONTROL_COUNT);	 //DIRECTION_CONTROL_COUNT��������Ƽ��� 2
	nRight = (int)(g_fRightVoltageSigma /= DIRECTION_CONTROL_COUNT);
	g_fLeftVoltageSigma = 0;
	g_fRightVoltageSigma = 0;

	fLeftRightAdd = nLeft + nRight;	  //Yadd ���ߺ�
	fLeftRightSub = nLeft - nRight;	  //Ysub ���߲�
	
	g_fDirectionControlOutOld = g_fDirectionControlOutNew;	 //��������� = ���������

	if(fLeftRightAdd < LEFT_RIGHT_MINIMUM)	   //���Ҳ�ֵ LEFT_RIGHT_MINIMUM=100
	      {
		  g_fDirectionControlOutNew=0;
	      }
	{
	    //ֵ = �����߲� * ���������/ ���ߺ�
/*	    	
	if(fLeftRightAdd < LEFT_RIGHT_MINIMUM)	   //���Ҳ�ֵ LEFT_RIGHT_MINIMUM=100
	      {
	      	  if(NowDirection>0)NowDirection=1;
	      	  else NowDirection=-1
	      }
	else//*/  
	NowDirection= fLeftRightSub  / fLeftRightAdd;
	    fDvalue = (g_nDireGyroVal-DireGyroValOffSet);
	 //   if(fDvalue>max)max=fDvalue;
	     DireFuzzyPD(fDvalue);
	    
	    
		 fValue = NowDirection * DIR_CONTROL_P;
         fDvalue*=DIR_CONTROL_D;
  
	    fValue -= fDvalue;
   
		if(fValue > 0) fValue += DIRECTION_CONTROL_DEADVALUE;		// ������������
	    else if(fValue < 0) fValue -= DIRECTION_CONTROL_DEADVALUE;  // ������������
		
		if(fValue > DIRECTION_CONTROL_OUT_MAX) fValue = DIRECTION_CONTROL_OUT_MAX;		  //�����޷�����
	    else if(fValue < DIRECTION_CONTROL_OUT_MIN) fValue = DIRECTION_CONTROL_OUT_MIN;	  //�����޷�����
		g_fDirectionControlOutNew = fValue;					// ���������ֵ
	}
}

/*************************************************************/
/*                 ������ƾ������                             */
/*************************************************************/
void DirectionControlOutput(void) 
{
	float fValue;
	fValue = g_fDirectionControlOutNew - g_fDirectionControlOutOld;   //�ٶ�ƫ��
	g_fDirectionControlOut = fValue * (g_cDirectionCount + 1) / DIRECTION_CONTROL_COUNT + g_fDirectionControlOutOld;
	
}
