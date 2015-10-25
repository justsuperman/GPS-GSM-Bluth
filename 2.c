/******************串口2中断来控制接收的数据保存在缓存数组中Rec_Buf2***********************************/
/**************************要用，直接调用这个数据就OK了GPSJW[28];*****************************************/
#include<12C5A60S2.h>
#include <intrins.h>
#include<string.h>
#include<stdio.h>
#define S2_P4 0x10
#define uchar unsigned char
#define uint unsigned int
#define Buf_Max2 72 //缓存长度72
xdata unsigned  char Rec_Buf2[Buf_Max2];    //定义缓存数组
xdata unsigned  char GPSJW[28];    //定义GPS经纬度缓存数组
uchar code temp1[94]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x46,0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x24,0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x31,0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x38,0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2A,0xB5,0x62,0x06,0x08,0x06,0x00,0x10,0x27,0x01,0x00,0x01,0x00,0x4D,0xDD};	
uchar u = 0;               //定义缓存指针
uchar f1 = 0;			   //收到70个数据的标志位
uchar f2=0;					//定义一个6次计数字符，这样就一分钟发一次
void Send_Hex2(unsigned char b);    //声明发送字符(十六进制)函数
void UART2_SendStr(uchar *str, uchar n); //声明发送数组，后面是数组的长度
void UartInit2(void);            //声明串口初始化函数
void GPRMC_send(void);
void clone(void);	   //拷贝出想要的数据
void CLR_Buf2(void);
void DelaySec(int sec);
void main()
{
	UartInit2();//串口初始化
	UART2_SendStr(temp1,94);//把命令以数组的方式发送出去
	DelaySec(1);
	//Timer1Init(); //定时器初始化,这样就固定1分钟读取发送一起缓存数组
	while(1)
	{
	   
	}	   
}

void UartInit2(void)		//9600bps@11.0592MHz
{
	AUXR &= 0xF7;		//波特率不倍速
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
	BRT = 0xDC;		//设定独立波特率发生器重装值
	AUXR |= 0x10;		//启动独立波特率发生器
    IE2 =0x01;	//开串口2中断  ES2=1
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
	 	u++;               //缓存指针向后移动
		if(u>Buf_Max2-10)          //如果缓存满,将缓存指针指向缓存的首地址
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
	 // S2CON=0x40;//不允许串口接收了
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
	IE2 =0x00;			    //关串口中断
	S2CON = S2CON & 0xFD; //B'11111101, 清零串口 2 发送完成中断请求标志
    S2BUF = c;
    do
	{
	temp = S2CON;
	temp = temp & 0x02;
	}while(temp==0);										
    S2CON = S2CON & 0xFD; //B'11111101, 清零串口 2 发送完成中断请求标志
	IE2 =0x01;	 //开启串口中断
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