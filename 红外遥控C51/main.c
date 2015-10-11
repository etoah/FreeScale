/************************************************
	ʵ�ֹ��ܣ������ң�س���
	Ӳ����STC89C52��Ƭ����12MHZ���񣬺�����չܣ����弰�������
	���ߣ�Duan
 	���ڣ�2011��7��11�� 19:27:54
	��ע���ó������õ���ң�������ɿ���
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

��������

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
�������ƣ�main
�������ܣ������� 

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
�������ƣ�Timer0_IR
�������ܣ��жϺ������

*******************************************************************************/
void Timer0_IR() interrupt 1		//��ʱ��0��������������Ŀ��
{
	ucIRTime++;
}
/******************************************************************************
�������ƣ�EXT0_IR
�������ܣ��ⲿ�жϺ���

*******************************************************************************/
void EXT0_IR() interrupt 0//�ⲿ�ж�1��������������ź�
{
	static unsigned char i;
	static bit bFlag;//�ж��Ƿ�ʼ��һλ

	if(bFlag)
	{
		if(ucIRTime<38&&ucIRTime>=29)	//TC9012��ͷ�룬��������
			i=0;
		ucAllBits[i]=ucIRTime; //һ���Զ�ȡ�洢32λ��ƽ���
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
		bFlag=1;//�Ƿ��ǵ�һ�ν���
	}
}
/******************************************************************************
�������ƣ�Timer0Init
�������ܣ���ʱ��0��ʼ������

*******************************************************************************/
void Timer0Init(void)   //��ʱ��0��ʼ��
{
	TMOD=0x02;//��ʱ��������ʽ2��TH0����װֵ��TL0�ǳ�ֵ
	TH0=0x00;
	TL0=0x00;
	ET0=1;
	TR0=1;

}

/******************************************************************************
�������ƣ�EXT0Init
�������ܣ��ⲿ�жϳ�ʼ������

*******************************************************************************/
void EXT0Init(void)	  //�ⲿ�ж�0�ĳ�ʼ��
{
	IT0=1;
	EX0=1;
	EA=1;

}
/******************************************************************************
�������ƣ�IR_DataProcess
�������ܣ��������ݴ�����

*******************************************************************************/

void IR_DataProcess(void)//������ֵ������
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
�������ƣ�Delay
�������ܣ���ʱ����
��ڲ�����uiCount-��ʱ������ÿ��1����0.5ms
*******************************************************************************/
void Delay(unsigned int uiCount)
{
	unsigned char j = 244;
	for(;uiCount > 0;uiCount--) while(--j);	
}
/******************************************************************************
�������ƣ�driver
��������:�綯������
*******************************************************************************/



void driver(unsigned  char flag)
{
	ena=enb=1;
	switch (flag)
	{
	
		case  4:		  //���ת
				{
				leftmotor1=0;
			 	rightmotor1=0;
			 	leftmotor2=1;
			 	rightmotor2=0;	
				break	;
				}
		case 1 :		//����ת
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
	 	case 6:		  //	�ҿ�ת
				{
			leftmotor1=0;
			 rightmotor1=0;
			 leftmotor2=0;
		 	rightmotor2=1;
				break	;
				}
		case 3:		  //����ת
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
		case 2:					//ǰ��
				{
				 	leftmotor1=0;
			 		rightmotor1=0;
			 		leftmotor2=1;
					rightmotor2=1;
					break	;
				}
		case 8:					   //����
				{
					leftmotor1=1;
			 		rightmotor1=1;
			 		leftmotor2=0;
					 rightmotor2=0;
					break	;
				}
		case 5:				//ֹͣ
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