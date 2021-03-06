#include"DirectionControl.h"
#include"system.h"
#include<math.h>
/*************************************************************/
/*                  方向电压积分                             */
/************************************************************/
void DirectionVoltageSigma(void) 
{
    int nLeft, nRight;

/*
　　AD８　左后电感　
　　AD９　前左斜上电感　	
　AD１０　前左水平电感　
　AD１１　前右水平电感
　AD１２　前右斜上电感
　AD１３　右后电感*/
/*	

  */


  	
	if(VOLTAGE_LEFT > DIR_LEFT_OFFSET) nLeft = VOLTAGE_LEFT - DIR_LEFT_OFFSET;
	else nLeft = 0;
	if(VOLTAGE_RIGHT > DIR_RIGHT_OFFSET) nRight = VOLTAGE_RIGHT - DIR_RIGHT_OFFSET;
	else nRight = 0;	    

	g_fLeftVoltageSigma += nLeft; 		  //左边方向电压积分
	g_fRightVoltageSigma += nRight;		  //右边方向电压积分   
	

/*	if(NowDirection>0.5||NowDirection<-0.5)
	{
		
	g_fLeftVoltageSigma =g_fLeftVoltageSigma+   L[2]; 		  //左边方向电压积分
	g_fRightVoltageSigma =	g_fRightVoltageSigma+L[4] ;
	}//右边方向电压积分 */	

}


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


void GetAllInductance(void)
{

	int i;
	
	char tmp;
  	 //if(PORTA_PA5)
	 {
          ATD0CTL3 = 0x88;          //每次只转换一个通道
            ATD0CTL5 = 0x09;          //转换AD02
            while(!ATD0STAT2_CCF0);
           L[1]=VOLTAGE_LEFT = ATD0DR0;
            ATD0CTL5 = 0x0B;          //转换AD03
            while(!ATD0STAT2_CCF0);
            L[3]=VOLTAGE_RIGHT = ATD0DR0;}//
        /*  else       
            {
            	
	ATD0CTL3 = 0x88;          //每次只转换一个通道
	tmp=0x08;
	for(i=0;i<6;i++)
	{
		ATD0CTL5 = tmp++;          //转换AD02
		while(!ATD0STAT2_CCF0);
		L[i]=ATD0DR0;*/

//	} 
	VOLTAGE_LEFT=L[1];//*(1-INDA_RAT)+L[0]*INDA_RAT;
	VOLTAGE_RIGHT=L[3];//*(1-INDA_RAT)+L[5]*INDA_RAT;
        //   }
}


/************************************************************/
/*                                  	自适应模糊PD                                    */
/************************************************************/
void DireFuzzyPD(float fDvalue) 
{
    int nPval,nDval,nP,nD;
	
     nPval=(int)(NowDirection*DIRE_PRAT);
     if(nPval>PB)nPval=PB ;//模糊化
     else if(nPval<NB)nPval=NB;
	 
     nDval=(int)(fDvalue/DIRE_DRAT) ;
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
    //  DIR_CONTROL_P=1550+2560*(nP)/PB;	 //清晰化
      
      
      nD=DD[nDval][nPval];	         //查询规则表
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
  DIR_CONTROL_D=1.55+0.85*(nD)/PB;	 //清晰化   */
     
   }
									   
/************************************************************/
/*                    方向控制PD                           */
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
	
	nLeft = (int)(g_fLeftVoltageSigma /= DIRECTION_CONTROL_COUNT);	 //DIRECTION_CONTROL_COUNT：方向控制计数 2
	nRight = (int)(g_fRightVoltageSigma /= DIRECTION_CONTROL_COUNT);
	g_fLeftVoltageSigma = 0;
	g_fRightVoltageSigma = 0;

	fLeftRightAdd = nLeft + nRight;	  //Yadd 两边和
	fLeftRightSub = nLeft - nRight;	  //Ysub 两边差
	
	g_fDirectionControlOutOld = g_fDirectionControlOutNew;	 //方向控制老 = 方向控制新

	if(fLeftRightAdd < LEFT_RIGHT_MINIMUM)	   //左右差值 LEFT_RIGHT_MINIMUM=100
	      {
		  g_fDirectionControlOutNew=0;
	      }
	{
	    //值 = （两边差 * 方向比例）/ 两边和
/*	    	
	if(fLeftRightAdd < LEFT_RIGHT_MINIMUM)	   //左右差值 LEFT_RIGHT_MINIMUM=100
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
   
		if(fValue > 0) fValue += DIRECTION_CONTROL_DEADVALUE;		// 方向死区控制
	    else if(fValue < 0) fValue -= DIRECTION_CONTROL_DEADVALUE;  // 方向死区控制
		
		if(fValue > DIRECTION_CONTROL_OUT_MAX) fValue = DIRECTION_CONTROL_OUT_MAX;		  //方向限幅控制
	    else if(fValue < DIRECTION_CONTROL_OUT_MIN) fValue = DIRECTION_CONTROL_OUT_MIN;	  //方向限幅控制
		g_fDirectionControlOutNew = fValue;					// 方向控制新值
	}
}

/*************************************************************/
/*                 方向控制均匀输出                             */
/*************************************************************/
void DirectionControlOutput(void) 
{
	float fValue;
	fValue = g_fDirectionControlOutNew - g_fDirectionControlOutOld;   //速度偏差
	g_fDirectionControlOut = fValue * (g_cDirectionCount + 1) / DIRECTION_CONTROL_COUNT + g_fDirectionControlOutOld;
	
}
