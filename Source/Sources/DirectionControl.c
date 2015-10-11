#include"DirectionControl.h"
#include"system.h"
#include<math.h>
/*************************************************************/
/*                  ·½ÏòµçÑ¹»ý·Ö                             */
/************************************************************/
void DirectionVoltageSigma(void) 
{
    int nLeft, nRight;

/*
¡¡¡¡AD£¸¡¡×óºóµç¸Ð¡¡
¡¡¡¡AD£¹¡¡Ç°×óÐ±ÉÏµç¸Ð¡¡	
¡¡AD£±£°¡¡Ç°×óË®Æ½µç¸Ð¡¡
¡¡AD£±£±¡¡Ç°ÓÒË®Æ½µç¸Ð
¡¡AD£±£²¡¡Ç°ÓÒÐ±ÉÏµç¸Ð
¡¡AD£±£³¡¡ÓÒºóµç¸Ð*/
/*	

  */


  	
	if(VOLTAGE_LEFT > DIR_LEFT_OFFSET) nLeft = VOLTAGE_LEFT - DIR_LEFT_OFFSET;
	else nLeft = 0;
	if(VOLTAGE_RIGHT > DIR_RIGHT_OFFSET) nRight = VOLTAGE_RIGHT - DIR_RIGHT_OFFSET;
	else nRight = 0;	    

	g_fLeftVoltageSigma += nLeft; 		  //×ó±ß·½ÏòµçÑ¹»ý·Ö
	g_fRightVoltageSigma += nRight;		  //ÓÒ±ß·½ÏòµçÑ¹»ý·Ö   
	

/*	if(NowDirection>0.5||NowDirection<-0.5)
	{
		
	g_fLeftVoltageSigma =g_fLeftVoltageSigma+   L[2]; 		  //×ó±ß·½ÏòµçÑ¹»ý·Ö
	g_fRightVoltageSigma =	g_fRightVoltageSigma+L[4] ;
	}//ÓÒ±ß·½ÏòµçÑ¹»ý·Ö */	

}


/************************************************************/
/*                    µÃµ½ËùÓÐµç¸ÐµÄÖµ

µç¸ÐAD

¡¡¡¡AD£¸¡¡×óºóµç¸Ð¡¡
¡¡¡¡AD£¹¡¡Ç°×óË®Æ½µç¸Ð¡	
¡¡AD£±£°¡¡Ç°×óÐ±ÉÏµç¸Ð¡
¡¡AD£±£±¡¡Ç°ÓÒË®Æ½µç¸Ð
¡¡AD£±£²¡¡Ç°ÓÒÐ±ÉÏµç¸Ð
¡¡AD£±£³¡¡ÓÒºóµç¸Ð                                                                    

                                                           */
/************************************************************/


void GetAllInductance(void)
{

	int i;
	
	char tmp;
  	 //if(PORTA_PA5)
	 {
          ATD0CTL3 = 0x88;          //Ã¿´ÎÖ»×ª»»Ò»¸öÍ¨µÀ
            ATD0CTL5 = 0x09;          //×ª»»AD02
            while(!ATD0STAT2_CCF0);
           L[1]=VOLTAGE_LEFT = ATD0DR0;
            ATD0CTL5 = 0x0B;          //×ª»»AD03
            while(!ATD0STAT2_CCF0);
            L[3]=VOLTAGE_RIGHT = ATD0DR0;}//
        /*  else       
            {
            	
	ATD0CTL3 = 0x88;          //Ã¿´ÎÖ»×ª»»Ò»¸öÍ¨µÀ
	tmp=0x08;
	for(i=0;i<6;i++)
	{
		ATD0CTL5 = tmp++;          //×ª»»AD02
		while(!ATD0STAT2_CCF0);
		L[i]=ATD0DR0;*/

//	} 
	VOLTAGE_LEFT=L[1];//*(1-INDA_RAT)+L[0]*INDA_RAT;
	VOLTAGE_RIGHT=L[3];//*(1-INDA_RAT)+L[5]*INDA_RAT;
        //   }
}


/************************************************************/
/*                                  	×ÔÊÊÓ¦Ä£ºýPD                                    */
/************************************************************/
void DireFuzzyPD(float fDvalue) 
{
    int nPval,nDval,nP,nD;
	
     nPval=(int)(NowDirection*DIRE_PRAT);
     if(nPval>PB)nPval=PB ;//Ä£ºý»¯
     else if(nPval<NB)nPval=NB;
	 
     nDval=(int)(fDvalue/DIRE_DRAT) ;
     if(nDval>PB)nDval=PB; //Ä£ºý»¯
     else if(nDval<NB)nDval=NB; //Ä£ºý»¯
     nDval+=3;
     nPval+=3;
     
     	 
     nP=DP[nDval][nPval];	         //²éÑ¯¹æÔò±í
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
    //  DIR_CONTROL_P=1550+2560*(nP)/PB;	 //ÇåÎú»¯
      
      
      nD=DD[nDval][nPval];	         //²éÑ¯¹æÔò±í
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
  DIR_CONTROL_D=1.55+0.85*(nD)/PB;	 //ÇåÎú»¯   */
     
   }
									   
/************************************************************/
/*                    ·½Ïò¿ØÖÆPD                           */
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
	
	nLeft = (int)(g_fLeftVoltageSigma /= DIRECTION_CONTROL_COUNT);	 //DIRECTION_CONTROL_COUNT£º·½Ïò¿ØÖÆ¼ÆÊý 2
	nRight = (int)(g_fRightVoltageSigma /= DIRECTION_CONTROL_COUNT);
	g_fLeftVoltageSigma = 0;
	g_fRightVoltageSigma = 0;

	fLeftRightAdd = nLeft + nRight;	  //Yadd Á½±ßºÍ
	fLeftRightSub = nLeft - nRight;	  //Ysub Á½±ß²î
	
	g_fDirectionControlOutOld = g_fDirectionControlOutNew;	 //·½Ïò¿ØÖÆÀÏ = ·½Ïò¿ØÖÆÐÂ

	if(fLeftRightAdd < LEFT_RIGHT_MINIMUM)	   //×óÓÒ²îÖµ LEFT_RIGHT_MINIMUM=100
	      {
		  g_fDirectionControlOutNew=0;
	      }
	{
	    //Öµ = £¨Á½±ß²î * ·½Ïò±ÈÀý£©/ Á½±ßºÍ
/*	    	
	if(fLeftRightAdd < LEFT_RIGHT_MINIMUM)	   //×óÓÒ²îÖµ LEFT_RIGHT_MINIMUM=100
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
   
		if(fValue > 0) fValue += DIRECTION_CONTROL_DEADVALUE;		// ·½ÏòËÀÇø¿ØÖÆ
	    else if(fValue < 0) fValue -= DIRECTION_CONTROL_DEADVALUE;  // ·½ÏòËÀÇø¿ØÖÆ
		
		if(fValue > DIRECTION_CONTROL_OUT_MAX) fValue = DIRECTION_CONTROL_OUT_MAX;		  //·½ÏòÏÞ·ù¿ØÖÆ
	    else if(fValue < DIRECTION_CONTROL_OUT_MIN) fValue = DIRECTION_CONTROL_OUT_MIN;	  //·½ÏòÏÞ·ù¿ØÖÆ
		g_fDirectionControlOutNew = fValue;					// ·½Ïò¿ØÖÆÐÂÖµ
	}
}

/*************************************************************/
/*                 ·½Ïò¿ØÖÆ¾ùÔÈÊä³ö                             */
/*************************************************************/
void DirectionControlOutput(void) 
{
	float fValue;
	fValue = g_fDirectionControlOutNew - g_fDirectionControlOutOld;   //ËÙ¶ÈÆ«²î
	g_fDirectionControlOut = fValue * (g_cDirectionCount + 1) / DIRECTION_CONTROL_COUNT + g_fDirectionControlOutOld;
	
}
