#include"Debug.h"




/*************************************************************/
/*                       车模跌倒停车                          */
/*************************************************************/
void CarTumble() 
{
      

    
       for(;;) {
       
        PITCFLMT_PITE=0;  //关中断
        PWMDTY67=0;
        PWMDTY01=0;
        PWMDTY23=0;
        PWMDTY45=0;
        if(PORTA_PA5)break; 
        } 
        PITCFLMT_PITE=1;   
      
   
    
  
}

/*************************************************************/
/*                  出赛道                                                                          */
/*************************************************************/
void OutRoadTest()
{
	int i,sum=0;
	//for(i=1;i<6;i++)
	sum+=L[1];
	sum+=L[3];
	if(sum<OUTROADMIN) 
	{
	
	//	if(SpeedSetCount>10)
		CarTumble() ;
		
	}
	
	
}





/***************************************** ********************/
/*              启动时找平衡点                                               */
/*************************************************************/ 
void BeginBalance()
{

	while((NowAngel> CAR_STAND_ANGLE_MIN )&&
		   (NowAngel < CAR_STAND_ANGLE_MAX )&&
		   (D_Gyro > CAR_STAND_ANGLE_MIN) &&
		   (D_Gyro < CAR_STAND_ANGLE_MAX)) ;
		   
		   
/*	while(SpeedSetCount<40) // 小车开始时停4s,找到平衡点
	{
	   g_nCarSpeedSet=0;
	}*/
}

/*************************************************************/
/*                       调试拨码开关                          */
/*************************************************************/
void SwitchDebug()	   
{
 unsigned char  ch;
 ch=PORTA;
 ch= ch&0x0F;
 
  switch(ch)
  {
  
  	
  		case 0x00:
	    {
	       g_nCarSpeedSet=0;
	       break;
	    }
	    case 0x01:
	    {
	       g_nCarSpeedSet=400;
	       break;
	    }
	    case 0x02:
	    {
	       g_nCarSpeedSet=480;
	       break;
	    }
	    case 0x03:
	    {
		    g_nCarSpeedSet=525;
		    break;
	    }
	    case 0x04:
	    {  
		     g_nCarSpeedSet=535;  
		    break;
	    }
	    	    case 0x05:
	    {  
		     g_nCarSpeedSet=545;  
		    break;
	    }
	    	    case 0x06:
	    {  
		     g_nCarSpeedSet=555;  
		    break;
	    }
	    	    case 0x07:
	    {  
		     g_nCarSpeedSet=565;  
		    break;
	    }
	    	    case 0x08:
	    {  
		     g_nCarSpeedSet=575;  
		    break;
	    }
	    	    case 0x09:
	    {  
		     g_nCarSpeedSet=585;  
		    break;
	    }
	    	    case 0x0a:
	    {  
		     g_nCarSpeedSet=595;  
		    break;
	    }

		  case 0x0B:
	    {  
		     g_nCarSpeedSet=605;  
		    break;
	    }
	     case 0x0C:
	    {  
		     g_nCarSpeedSet=615;  
		    break;
	    }
	     case 0x0D:
	    {  
		     g_nCarSpeedSet=625;  
		    break;
	    }
	     case 0x0E:
	    {  
		     g_nCarSpeedSet=635;  
		    break;
	    }
	    case 0x0F:
	    {  
		     g_nCarSpeedSet=645;  
		    break;
	    }
     }
  }



