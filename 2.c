/******************����2�ж������ƽ��յ����ݱ����ڻ���������Rec_Buf2***********************************/
/**************************Ҫ�ã�ֱ�ӵ���������ݾ�OK��GPSJW[28];*****************************************/
#include<12C5A60S2.h>
#include <intrins.h>
#include<string.h>
#include<stdio.h>
#define S2_P4 0x10
#define uchar unsigned char
#define uint unsigned int
#define Buf_Max2 72 //���泤��72
xdata unsigned  char Rec_Buf2[Buf_Max2];    //���建������
xdata unsigned  char GPSJW[28];    //����GPS��γ�Ȼ�������
uchar code temp1[94]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x46,0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x24,0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x31,0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x38,0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2A,0xB5,0x62,0x06,0x08,0x06,0x00,0x10,0x27,0x01,0x00,0x01,0x00,0x4D,0xDD};	
uchar u = 0;               //���建��ָ��
uchar f1 = 0;			   //�յ�70�����ݵı�־λ
uchar f2=0;					//����һ��6�μ����ַ���������һ���ӷ�һ��
void Send_Hex2(unsigned char b);    //���������ַ�(ʮ������)����
void UART2_SendStr(uchar *str, uchar n); //�����������飬����������ĳ���
void UartInit2(void);            //�������ڳ�ʼ������
void GPRMC_send(void);
void clone(void);	   //��������Ҫ������
void CLR_Buf2(void);
void DelaySec(int sec);
void main()
{
	UartInit2();//���ڳ�ʼ��
	UART2_SendStr(temp1,94);//������������ķ�ʽ���ͳ�ȥ
	DelaySec(1);
	//Timer1Init(); //��ʱ����ʼ��,�����͹̶�1���Ӷ�ȡ����һ�𻺴�����
	while(1)
	{
	   
	}	   
}

void UartInit2(void)		//9600bps@11.0592MHz
{
	AUXR &= 0xF7;		//�����ʲ�����
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//���������ʷ�����ʱ��ΪFosc,��1T
	BRT = 0xDC;		//�趨���������ʷ�������װֵ
	AUXR |= 0x10;		//�������������ʷ�����
    IE2 =0x01;	//������2�ж�  ES2=1
    EA = 1;
	
}
void Serial_Int() interrupt 8 
{	
	unsigned char k = 0;
	k = S2CON ;
	k = k & 0x01;
	 if(k==1)
	 {
		S2CON = S2CON & 0xFE; //1111,1110
		k = S2BUF;
		Rec_Buf2[u] = k;
		if(k=='$'){u=0;}
	 	u++;               //����ָ������ƶ�
		if(u>Buf_Max2-10)          //���������,������ָ��ָ�򻺴���׵�ַ
			{
				GPRMC_send();
			}
	 }	
	 else
	 {
	S2CON = S2CON & 0xFD; //1111,1101
	 }
}

void GPRMC_send()
{
	 // S2CON=0x40;//�������ڽ�����
	  if(strstr(Rec_Buf2,",E,")!=NULL)
	  {
	  	if((strstr(Rec_Buf2,"GPRMC")!=NULL))
		{
			//UART2_SendStr(Rec_Buf2,u);
			clone();
			UART2_SendStr(GPSJW,28);
			u=0;
	  	}
	  }			
}
void Send_Hex2(unsigned char c)         
{
    unsigned char temp = 0;
	IE2 =0x00;			    //�ش����ж�
	S2CON = S2CON & 0xFD; //B'11111101, ���㴮�� 2 ��������ж������־
    S2BUF = c;
    do
	{
	temp = S2CON;
	temp = temp & 0x02;
	}while(temp==0);										
    S2CON = S2CON & 0xFD; //B'11111101, ���㴮�� 2 ��������ж������־
	IE2 =0x01;	 //���������ж�
}

void UART2_SendStr(uchar *str,unsigned char n)   
{   
	unsigned char i;
	for(i=0;i<n;i++)
	{
	Send_Hex2(str[i]);	
	}   
}
void DelaySec(int sec)
{
	uint i , j= 0;

	for(i=0; i<sec; i++)
	{
		for(j=0; j<65535; j++)
		{	
		}
	}
} 
void clone()
{
	uchar i,j,k;
	for(i=0;i<22;i++)
	{
		if(Rec_Buf2[i]=='A')
		{j=i;}
	}
	i=j;
	for(k=0;k<28;k++)
	{
		GPSJW[k]=Rec_Buf2[i];
		i++;
	}
}