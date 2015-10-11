/************************************************
	实现功能：外解码遥控程序
	硬件：STC89C52单片机，12MHZ晶振，红外接收管，车体及相关驱动
	作者：Duan
 	日期：2011年7月11日 19:27:54
	备注：用常见家用电视遥控器即可控制
************************************************/
#include <REG52.H>
sbit rightmotor2=P0^0;
sbit rightmotor1=P0^1;
sbit leftmotor1=P0^2;
sbit leftmotor2=P0^3;
sbit ena=P2^4;
sbit enb=P2^5;
sbit IR_IO=P3^2;  

/**********************************************

函数声明

**********************************************/
unsigned char ucIRTime;
unsigned char ucCodeContent[4];
unsigned char ucAllBits[32];
void driver(unsigned  char flag)  ;
bit bIRFlag;


void Timer0Init(void);
void EXT0Init(void);
void IR_DataProcess(void);

/******************************************************************************
函数名称：main
函数功能：主函数 

*******************************************************************************/
void main()
{
	EXT0Init();
	Timer0Init();

	while(1)
	{
		if(bIRFlag)
		{
			IR_DataProcess();
			bIRFlag=0;	
		}

		driver(ucCodeContent[2]);

	}

}
/******************************************************************************
函数名称：Timer0_IR
函数功能：中断函数入口

*******************************************************************************/
void Timer0_IR() interrupt 1		//定时器0，用来测量脉冲的宽度
{
	ucIRTime++;
}
/******************************************************************************
函数名称：EXT0_IR
函数功能：外部中断函数

*******************************************************************************/
void EXT0_IR() interrupt 0//外部中断1，用来处理接收信号
{
	static unsigned char i;
	static bit bFlag;//判断是否开始第一位

	if(bFlag)
	{
		if(ucIRTime<38&&ucIRTime>=29)	//TC9012的头码，即引导码
			i=0;
		ucAllBits[i]=ucIRTime; //一次性读取存储32位电平宽度
		ucIRTime=0;
		i++;
		if(i==33)
		{
			bIRFlag=1;
			i=0;
		}
	}
	else
	{
		ucIRTime=0;
		bFlag=1;//是否是第一次接收
	}
}
/******************************************************************************
函数名称：Timer0Init
函数功能：定时器0初始化函数

*******************************************************************************/
void Timer0Init(void)   //定时器0初始化
{
	TMOD=0x02;//定时器工作方式2，TH0是重装值，TL0是初值
	TH0=0x00;
	TL0=0x00;
	ET0=1;
	TR0=1;

}

/******************************************************************************
函数名称：EXT0Init
函数功能：外部中断初始化函数

*******************************************************************************/
void EXT0Init(void)	  //外部中断0的初始化
{
	IT0=1;
	EX0=1;
	EA=1;

}
/******************************************************************************
函数名称：IR_DataProcess
函数功能：红外数据处理函数

*******************************************************************************/

void IR_DataProcess(void)//红外码值处理函数
{
	unsigned char i,j,k;
	unsigned char ucCode,ucUserValue;
	k=1;
	for(i=0;i<4;i++)
	{
		for(j=1;j<=8;j++)
		{
			ucCode=ucAllBits[k];
			if(ucCode>5)
			{
				ucUserValue=ucUserValue|0x80;
			}
			else
			{
				ucUserValue=ucUserValue;
			}
			if(j<8)
			{
				ucUserValue=ucUserValue>>1;	
			}
			k++;
		}
		ucCodeContent[i]=ucUserValue;
		ucUserValue=0;
	}
}



/******************************************************************************
函数名称：Delay
函数功能：延时函数
入口参数：uiCount-延时参数，每加1增加0.5ms
*******************************************************************************/
void Delay(unsigned int uiCount)
{
	unsigned char j = 244;
	for(;uiCount > 0;uiCount--) while(--j);	
}
/******************************************************************************
函数名称：driver
函数功能:电动机驱动
*******************************************************************************/



void driver(unsigned  char flag)
{
	ena=enb=1;
	switch (flag)
	{
	
		case  4:		  //左快转
				{
				leftmotor1=0;
			 	rightmotor1=0;
			 	leftmotor2=1;
			 	rightmotor2=0;	
				break	;
				}
		case 1 :		//左慢转
				{
				leftmotor1=0;
			 	rightmotor1=0;
			 	leftmotor2=1;
			 	rightmotor2=0;
				Delay(2);
				rightmotor2=1;
				Delay(2);
				break	;

					
				}
	 	case 6:		  //	右快转
				{
			leftmotor1=0;
			 rightmotor1=0;
			 leftmotor2=0;
		 	rightmotor2=1;
				break	;
				}
		case 3:		  //右慢转
				{
				leftmotor1=0;
				rightmotor1=0;
			 	rightmotor2=1;
				leftmotor2=0;
				Delay(2)  ;
				leftmotor2=1;
				Delay(2);
				break	;
				}
		case 2:					//前进
				{
				 	leftmotor1=0;
			 		rightmotor1=0;
			 		leftmotor2=1;
					rightmotor2=1;
					break	;
				}
		case 8:					   //后退
				{
					leftmotor1=1;
			 		rightmotor1=1;
			 		leftmotor2=0;
					 rightmotor2=0;
					break	;
				}
		case 5:				//停止
				{
			leftmotor1=0;
			 rightmotor1=0;
			 leftmotor2=0;
			 rightmotor2=0;

		} 
	}
}

/****************end of line3**********************/
/****************end of line2**********************/
/****************end of line1**********************/