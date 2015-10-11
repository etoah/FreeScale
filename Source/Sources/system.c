#include "system.h"
/*************************************************************/
/*                      ��ʼ�����໷                           */
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

    REFDV = 0x81;         //PLLCLK=2��OSCCLK��(SYNR+1)/(REFDV+1)��64MHz ,fbus=32M
    PLLCTL =PLLCTL|0x70;  //Enable PLL circuit
    asm NOP;
    asm NOP;
    while(!(CRGFLG&0x08)); //PLLCLK is Locked already
    CLKSEL |= 0x80;        //set PLLCLK as sysclk
}



/*************************************************************/
/*                      IO��ʼ��                              */
/*************************************************************/
 void IO_init(void) 
{
    //PORTB=0x00;
    DDRB=0xEF; //����1110 1111  ����dataΪ���룬������Ϊ���
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
/*                      ��ʱ�в���1ms�ж�                         */
/*************************************************************/
void PIT_init(void)//��ʱ�жϳ�ʼ������ 1MS��ʱ�ж����� 
{
	PITCFLMT_PITE=0; //��ʱ�ж�ͨ��0��
	PITCE_PCE0=1;//��ʱ��ͨ��0ʹ��
	PITMTLD0=(BUS_CLOCK/200000)-1;   //56000000 8λ��ʱ����ֵ�趨,240��Ƶ����48MHzBusClock�£�Ϊ0.2MHz����5us
	PITLD0=150-1;//16λ��ʱ����ֵ�趨��100*0.005MS
	PITINTE_PINTE0=1;//��ʱ���ж�ͨ��0�ж�ʹ��
	PITCFLMT_PITE=1;//��ʱ��ͨ��0ʹ�� 
}

/*************************************************************/
/*                      ����������                         */
/************************************************************/
void Count_Init(void)
{   
    TCNT =0x00; //��ʱ�����ļĴ���
    TSCR1=0x80; //ʱ������  ��ʱ��ϵͳ���ƼĴ���1 
    TSCR2=0x04; //��ʱ��ϵͳ���ƼĴ���2 div by 16
    
    PACTL=0xc0; //�����ۼ���A���ƼĴ���,PT7 PIN,PACN32 16BIT,FALLing edge,NOT INTERRUPT
    
    TCTL3=0x80; //���ƼĴ��� c-���벶׽7�½�����Ч,             
    TCTL4=0x02; //���ƼĴ��� c-���벶׽0�½�����Ч, 
        
    TIE  =0x00; //ÿһλ��Ӧ��Ӧͨ���ж�����,0��ʾ��ֹ�ж�
    
    TIOS =0x7E; //ÿһλ��Ӧͨ����: 0���벶׽,1����Ƚ�  ����00Ҳ����       
    
    TCTL3_EDG7B=1; //��ʵ�������Ѿ������, 7�½�����Ч,            
    //TCTL3_EDG7x=1; //c-���벶׽7�κ�����Ч,�ñ�����
    TCTL4_EDG0B=1; //��ʵ�������Ѿ������, 0�½�����Ч,             
    //TCTL4_EDG0x=1;   c-���벶׽0�κ�����Ч   
}  


/*************************************************************/
/*                        PWM��ʼ��                         */
/*************************************************************/
void PWM_Init(void)
{   
         PWME=0x00;	    //��PWMͨ��
	PWMCTL=0xF0;    // ����ͨ��67��45��32��10����
	PWMPRCLK=0x22;  // Ԥ��Ƶ:A_CLK=busclk/2*2=12M B_CLK=BUSCLK/2*2=12M
	PWMSCLA=0x01;   //SA_CLK=A_CLK/(2*1)==6MHz
	PWMSCLB=0X01;   //SB_CLK=B_CLK/(2*1)==6MHz
	PWMPOL=0xFF;    //����ѡ����ʼΪ�ߵ�ƽ;
	PWMCLK=0x00;    //PWM01 45ѡ�� SA_CLK PWM67 23ѡ��SB_CLK
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
	PWMCAE=0x00;   //����뷽ʽ
	PWME=0xAA;	   //��PWMͨ��
}








/*************************************************************/
/*                        ��ʱ����                           */
/*************************************************************/
void delay(void) 
{
	 unsigned int i;
	 for(i=0;i<5000;i++);
}




/*************************************************************/
/*                 ��ʼ��ADģ��Ϊ12λ����                    */
/*************************************************************/
void INIT_AD12BIT(void) 
{
	 ATD0CTL2 = 0xc0;   //����A/Dת��,��������,��ֹ�ж�
	 ATD0CTL1_SRES=2;  //ѡ��12λģ��ת��
	 //ATD0CTL3 = 0x88;   //ÿ��ֻת��һ��ͨ��
	 ATD0CTL4 = 0x01;   //ADģ��ʱ��Ƶ��Ϊ8MHz
}



/*************************************************************/
/*                                          ������                                               */
/*************************************************************/
void MotorSpeedOut(void)
{
 
    int RightVal,LeftVal;
    
    //���ѹ = �Ƕȿ������ + �ٶȿ����� -���������+�����ٶȿ�����
  PWML= LeftVal = (int)(AngelControlOutput-  g_fSpeedMotorOut +g_fDirectionControlOut);//+DirectionSpeedControlOut;  
    //�ҵ�ѹ = �Ƕȿ������ + �ٶȿ����� + ��������� +�����ٶȿ�����
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
