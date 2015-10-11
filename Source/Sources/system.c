#include "system.h"
/*************************************************************/
/*                      初始化锁相环                           */
/*************************************************************/
void INIT_PLL(void) 
{
    CLKSEL &= 0x7f;       //set OSCCLK as sysclk
    PLLCTL &= 0x8F;       //Disable PLL circuit
    CRGINT &= 0xDF;
    
    
    #if(BUS_CLOCK == 64000000) 
      SYNR = 0x47;
    #elif(BUS_CLOCK == 56000000) 
      SYNR = 0x46;
    #elif(BUS_CLOCK == 48000000) 
      SYNR = 0x45;
    #elif(BUS_CLOCK == 40000000) 
      SYNR = 0x44;
    #elif(BUS_CLOCK == 32000000)
      SYNR = 0x43;     
    #elif(BUS_CLOCK == 24000000)
      SYNR = 0x42;
    #endif 

    REFDV = 0x81;         //PLLCLK=2×OSCCLK×(SYNR+1)/(REFDV+1)＝64MHz ,fbus=32M
    PLLCTL =PLLCTL|0x70;  //Enable PLL circuit
    asm NOP;
    asm NOP;
    while(!(CRGFLG&0x08)); //PLLCLK is Locked already
    CLKSEL |= 0x80;        //set PLLCLK as sysclk
}



/*************************************************************/
/*                      IO初始化                              */
/*************************************************************/
 void IO_init(void) 
{
    //PORTB=0x00;
    DDRB=0xEF; //输入1110 1111  除了data为输入，其它均为输出
    DDRS=0xFF;
    DDRA=0x00;
    DDRH=0xF0;
    PTH=0xF0;
    CSn_L=1;
    CSn_R=1;
    CLK  = 1;
    DAT = 1;
}

/*************************************************************/
/*                      定时中产生1ms中断                         */
/*************************************************************/
void PIT_init(void)//定时中断初始化函数 1MS定时中断设置 
{
	PITCFLMT_PITE=0; //定时中断通道0关
	PITCE_PCE0=1;//定时器通道0使能
	PITMTLD0=(BUS_CLOCK/200000)-1;   //56000000 8位定时器初值设定,240分频，在48MHzBusClock下，为0.2MHz。即5us
	PITLD0=150-1;//16位定时器初值设定。100*0.005MS
	PITINTE_PINTE0=1;//定时器中断通道0中断使能
	PITCFLMT_PITE=1;//定时器通道0使能 
}

/*************************************************************/
/*                      编码器计数                         */
/************************************************************/
void Count_Init(void)
{   
    TCNT =0x00; //定时器核心寄存器
    TSCR1=0x80; //时钟允许  计时器系统控制寄存器1 
    TSCR2=0x04; //计时器系统控制寄存器2 div by 16
    
    PACTL=0xc0; //脉冲累加器A控制寄存器,PT7 PIN,PACN32 16BIT,FALLing edge,NOT INTERRUPT
    
    TCTL3=0x80; //控制寄存器 c-输入捕捉7下降沿有效,             
    TCTL4=0x02; //控制寄存器 c-输入捕捉0下降沿有效, 
        
    TIE  =0x00; //每一位对应相应通道中断允许,0表示禁止中断
    
    TIOS =0x7E; //每一位对应通道的: 0输入捕捉,1输出比较  这里00也可以       
    
    TCTL3_EDG7B=1; //其实在上面已经设好了, 7下降沿有效,            
    //TCTL3_EDG7x=1; //c-输入捕捉7任何沿有效,用编码器
    TCTL4_EDG0B=1; //其实在上面已经设好了, 0下降沿有效,             
    //TCTL4_EDG0x=1;   c-输入捕捉0任何沿有效   
}  


/*************************************************************/
/*                        PWM初始化                         */
/*************************************************************/
void PWM_Init(void)
{   
         PWME=0x00;	    //关PWM通道
	PWMCTL=0xF0;    // 设置通道67、45、32、10级连
	PWMPRCLK=0x22;  // 预分频:A_CLK=busclk/2*2=12M B_CLK=BUSCLK/2*2=12M
	PWMSCLA=0x01;   //SA_CLK=A_CLK/(2*1)==6MHz
	PWMSCLB=0X01;   //SB_CLK=B_CLK/(2*1)==6MHz
	PWMPOL=0xFF;    //极性选择起始为高电平;
	PWMCLK=0x00;    //PWM01 45选择 SA_CLK PWM67 23选择SB_CLK
	PWMCNT0=0x00;
	PWMCNT1=0x00;
	PWMCNT2=0x00;
	PWMCNT3=0x00;
	PWMCNT4=0x00;
	PWMCNT5=0x00;
	PWMCNT6=0x00;
	PWMCNT7=0x00;
	
	PWMPER01=2000; // F=12M/2000==6KHz
	PWMPER23=2000; // F=12M/2000==6KHz
	PWMPER45=2000; // F=12M/2000==6KHz
	PWMPER67=2000; // F=12M/2000==6KHz
	PWMCAE=0x00;   //左对齐方式
	PWME=0xAA;	   //开PWM通道
}








/*************************************************************/
/*                        延时函数                           */
/*************************************************************/
void delay(void) 
{
	 unsigned int i;
	 for(i=0;i<5000;i++);
}




/*************************************************************/
/*                 初始化AD模块为12位精度                    */
/*************************************************************/
void INIT_AD12BIT(void) 
{
	 ATD0CTL2 = 0xc0;   //启动A/D转换,快速清零,禁止中断
	 ATD0CTL1_SRES=2;  //选用12位模数转换
	 //ATD0CTL3 = 0x88;   //每次只转换一个通道
	 ATD0CTL4 = 0x01;   //AD模块时钟频率为8MHz
}



/*************************************************************/
/*                                          电机输出                                               */
/*************************************************************/
void MotorSpeedOut(void)
{
 
    int RightVal,LeftVal;
    
    //左电压 = 角度控制输出 + 速度控制量 -方向控制量+方向速度控制量
  PWML= LeftVal = (int)(AngelControlOutput-  g_fSpeedMotorOut +g_fDirectionControlOut);//+DirectionSpeedControlOut;  
    //右电压 = 角度控制输出 + 速度控制量 + 方向控制量 +方向速度控制量
   PWMR= RightVal = (int)(AngelControlOutput - g_fSpeedMotorOut -  g_fDirectionControlOut);//+DirectionSpeedControlOut; 
    
    

    
    if(RightVal>MOTOR_OUT_MAX)
          RightVal=MOTOR_OUT_MAX;
    else if(RightVal<MOTOR_OUT_MIN)
              RightVal=MOTOR_OUT_MIN;
    
    
    if(RightVal<0)
    {
      RightVal-=MOTOR_DEAD_VAL;
      PWMDTY23=0;
      PWMDTY01=-RightVal;
      
    } 
    else if(RightVal>0)
    {
      RightVal+=MOTOR_DEAD_VAL;
      PWMDTY23=RightVal;
      PWMDTY01=0;
      
    } 
    
     
    if(LeftVal<0)
    {
      LeftVal-=MOTOR_DEAD_VAL;
      PWMDTY45=0;
      PWMDTY67=-LeftVal;
      
    } 
    else if(LeftVal>0)
    {
     LeftVal+=MOTOR_DEAD_VAL;
      PWMDTY45=LeftVal;
      PWMDTY67=0;
      
    } 


}
