///********************接线的时候注意要共地！！千万目前服务器还不接受数据
/**********************p1口的1.2   1.3不要置0；否则GPS不首发数据了***************************************/
#include <STC12C5A60S2.h>
#include <intrins.h>
#include<string.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
uint time;	   //接电话计时
//uchar code table[8]={0xf1,0xf3,0xf2,0xf6,0xf4,0xfc,0xf8,0xf9}; //反转
//uchar code table1[8]={0xf9,0xf8,0xfc,0xf4,0xf6,0xf2,0xf3,0xf1}; //正转
uint n,m;
#define Buf_Max 100 //缓存长度100
#define Delay_Time 3000  //延时长度
unsigned char i = 0;               //定义缓存指针
xdata unsigned  char Rec_Buf[Buf_Max];    //定义缓存数组
sbit beep=P3^7;
sbit LED1 = P1^0; //状态指示灯1
sbit LED2 = P1^1; //状态指示灯2
sbit LED3 = P1^2; //状态指示灯3	//这两个灯不能用，留给串口2的
sbit LED4 = P1^3; //状态指示灯4	//这两个灯不能用，留给串口2的
sbit LED5 = P1^4; //状态指示灯5
sbit LED6 = P1^5; //状态指示灯6
sbit LED7 = P1^6; //状态指示灯7
sbit LED8 = P1^7; //状态指示灯8

void Serial_Init(void);            //声明串口初始化函数
void Send_Hex(unsigned char b);    //声明发送字符(十六进制)函数
void Send_ASCII(unsigned char *b); //声明发送字符(ASCII码)函数
void Delay_ms(unsigned int time);  //声明延时函数
void DelaySec(int sec);			   //声明延时多少秒
//void delay(uint z);				   //声明步进电机延时、、要改成上面的
void CLR_Buf(void);				   //声明清除缓存内容，以便下次继续用	
void gsmcsh(void);				   //GSM模块的初始化，所有的都有了，就等待短信了
bit  Hand(unsigned char *a);       //声明判断缓存中是否含有指定的字符串函数
void fdx();						   //声明发定位短信函数
void GPSWZ();					   //声明发思恋短信函数
void ddh();					   //声明打电话函数
void door();				   //声明开关门函数
void GPSWZ_dx();
////////////////
#define S2_P4 0x10
#define Buf_Max2 72 //缓存长度72
xdata unsigned  char Rec_Buf2[Buf_Max2];    //定义缓存数组
unsigned  char GPSJW[28]="NO,Receive-GPS!";    //定义GPS经纬度缓存数组
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


/**********************************主函数******************************/
void main(void)
{			
			gsmcsh();
			UartInit2();//串口初始化
			UART2_SendStr(temp1,94);//把命令以数组的方式发送出去
			DelaySec(1);			   		
			while(1)
				{ 	
	    			/*********************等待接收短信********************/
					if(strstr(Rec_Buf,"CMTI")!=NULL)    //若缓存字符串中含有"+CMTI"就表示有新的短信
		        		
						{
							LED1 = 1;						//进来以后把第一个等点亮
							CLR_Buf();                   //清除缓存内数据，等待接收短信内容
							while(!Hand("CMGR"))
							{
								Delay_ms(Delay_Time);        //延时
								Send_ASCII("AT+CMGR=1");     //发送读取信息指令
				    			Send_Hex(0x0d);               //发送回车符
	 							Send_Hex(0x0a);               //发送换行符
								DelaySec(1);  //！！！如果这里不延时，总会发两条命令来测试，才能通过，因为缓存还没存到缓存数组里了
							}
							LED2 = 1;//当+CMGR: "REC UNREAD","15150218615","","15/05/17,20:06:44+32"
							DelaySec(1);//让短信装进缓存里
		        			if(strstr(Rec_Buf,"DW")!=NULL) //如果信息内容是"DW"
		            			{
								 LED7=1;
								 Delay_ms(Delay_Time);     //延时
								  while(!Hand("CENG"))
								  {
								  		 Delay_ms(Delay_Time);     //延时
										 Send_ASCII("AT+CENG?");  //开工程模式，读取定位信息
										 CLR_Buf(); 			  //先清楚缓存数组里的东西，防止之前有所残余
										 Delay_ms(Delay_Time);
										 Send_Hex(0x0d);            //发送回车符
							 			 Send_Hex(0x0a); 			 //发送换行符
										 DelaySec(1);
										 DelaySec(1);          
								  }
								  LED6=1;
								  fdx();			  //我的目的是在这发短信，把定位信息发出来
								  LED5=1;							
									CLR_Buf();                              //清除缓存内容
		 							DelaySec(1);
								}	                        
							
							
							if(strstr(Rec_Buf,"GPSWZ")!=NULL) //如果信息内容是"GPSWZ"
		            			{
								 LED7=1;
								 Delay_ms(Delay_Time);     //延时
								  GPSWZ_dx();
								  LED5=1;							
								CLR_Buf();
								DelaySec(1);
								}                          //清除缓存内容	
							
							door();
							
							while(!Hand("OK"))                    //等待设置成功
							{
								Delay_ms(Delay_Time);                   //延时
								Send_ASCII("AT+CMGD=1,4");                //删除读取后的信息
								Send_Hex(0x0d);                          //发送回车符
	 							Send_Hex(0x0a);							 //发送换行符
								DelaySec(1);                          
							}
							CLR_Buf();						          //清除缓存内容
							DelaySec(1); 
							P1=0x4f;                       
						} 
               /******************等待接收短信结束***************************/
			   	
			  /*******************检测来电话接听****************************/
			  if(strstr(Rec_Buf,"CLIP")!=NULL)//当检测到来电话的时候
				{
					DelaySec(40);  //让小喇叭响40秒钟
					Send_ASCII("ATA\r\n");
					CLR_Buf();
					DelaySec(2);
					
					while(!Hand("CARRIER"))       //当检测不到对方挂断就在这等待			
					{
					   DelaySec(60);
					   time++;
					   if(time>200)
					   	{
							time=0;
							break;
					   	}
					}//这一句过不去，跳不下来
					CLR_Buf();                //清除缓存内容
					DelaySec(1);
					Send_ASCII("ATH\r\n");
					DelaySec(5);				//延时5秒

				}
			  /*******************检测来电话接听结束****************************/
			  door();
			  if(GPSJW[0]=='A')
			  {
			  	GPSWZ();
			  }
			}
}

/***************************串口初始化函数*****************************/
/*函数原型:void Serial_Init(void)
/*函数功能:串口初始化
/*输入参数:无
/*输出参数:无
/**********************************************************************/
void Serial_Init(void)//波特率产生器
{
 	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xDC;		//设定定时初值
	TH1 = 0xDC;		//设定定时器重装值
	TR1 = 1;		//启动定时器1
	ES=1;
	EA=1;
}

/****************************串口中断处理函数**************************/
/*函数原型:void Serial_Int1(void)
/*函数功能:串口中断处理
/*输入参数:无
/*输出参数:无
/**********************************************************************/
void Serial_Int1() interrupt 4 
{	
	ES = 0;	 //关串口中断，防止中断嵌套
	if(TI)	 //如果是发送中断，则不做任何处理
		{
			TI = 0;  //清除发送中断标志位
		}
	if(RI)	 //如果是接送中断，则进行处理
		{
			RI = 0; //清除接收中断标志位
			Rec_Buf[i] = SBUF; //将接收到的字符串存到缓存中
			i++;               //缓存指针向后移动
			if(i>Buf_Max-1)          //如果缓存满,将缓存指针指向缓存的首地址
				{
					i=Buf_Max-1;
					//REN=1;//不让再接收了，除非在清除数组里清除
				}
		}
	ES = 1;	 //开启串口中断
}
/****************************GSM模块初始化函数***********************/
/*函数原型:void gsmcsh(void)
/*函数功能:把所有的初始化都包括在这里了，就等着接收短信了
/*输入参数:无
/*输出参数:无
/**********************************************************************/
void gsmcsh(void)
{
	        Serial_Init();         //初始化串口
			DelaySec(4);		  //延时四秒，发现正好可以启动
			CLR_Buf();             //清除缓存内容
	        DelaySec(1);        	//延时1秒
	        while(!Hand("OK"))     //判断是否握手成功,如果不成功延时一会,再发送AT握手指令
				{
					Send_ASCII("AT"); //发送联机指令 
	                Send_Hex(0x0d);    //发送回车符    
					Send_Hex(0x0a);    //发送换行符
					P1=0x1c;
					DelaySec(1);   //延时
				}
			DelaySec(1);			   //延时一会
			CLR_Buf();                //清除缓存内容
			P1 = 0x4e;                 //点亮握手成功指示灯

		////////////////////////////////////
		 DelaySec(1);     			//延时
			while(!Hand("OK"))       //等待设置成功
			{
				Delay_ms(Delay_Time);     //延时
				Send_ASCII("AT+CIPSHUT");//	关闭移动场景
				Send_Hex(0x0d);            //发送回车符
	 			Send_Hex(0x0a);            //发送换行符
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //清除缓存内容
			//LED4=1;

			DelaySec(1);     			//延时
			while(!Hand("OK"))       //等待设置成功
			{
				Delay_ms(Delay_Time);     //延时
				Send_ASCII("AT+CIPMUX=0");//
				Send_Hex(0x0d);            //发送回车符
	 			Send_Hex(0x0a);            //发送换行符
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //清除缓存内容
			LED5=1;

			DelaySec(1);     			//延时
			while(!Hand("OK"))       //等待设置成功
			{
				Delay_ms(Delay_Time);     //延时
				Send_ASCII("AT+CIPRXGET=0");//
				Send_Hex(0x0d);            //发送回车符
	 			Send_Hex(0x0a);            //发送换行符
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //清除缓存内容
			LED5=0;

			DelaySec(1);     			//延时
			while(!Hand("OK"))       //等待设置成功
			{
				Delay_ms(Delay_Time);     //延时
				Send_ASCII("AT+CIPQRCLOSE=1");//
				Send_Hex(0x0d);            //发送回车符
	 			Send_Hex(0x0a);            //发送换行符
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //清除缓存内容
			LED6=1;


			DelaySec(1);     			//延时
			while(!Hand("OK"))       //等待设置成功
			{
				Delay_ms(Delay_Time);     //延时
				Send_ASCII("AT+CIPMODE=0");//
				Send_Hex(0x0d);            //发送回车符
	 			Send_Hex(0x0a);            //发送换行符
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //清除缓存内容
			LED7=0;

			DelaySec(1);     			//延时
			while(!Hand("ALREADY"))       //等待设置成功
			{
				Delay_ms(Delay_Time);     //延时
				Send_ASCII("AT+CIPSTART=\"UDP\",\"128.199.225.127\",8080");//
				Send_Hex(0x0d);            //发送回车符
	 			Send_Hex(0x0a);            //发送换行符
				CLR_Buf();
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //清除缓存内容
			LED7=1;

		   ///////////////////////
		   DelaySec(1);     			//延时
			while(!Hand("OK"))       //等待设置成功
			{
				Delay_ms(Delay_Time);     //延时
				Send_ASCII("AT+COLP=1");//开启被叫号码显示
				Send_Hex(0x0d);            //发送回车符
	 			Send_Hex(0x0a);            //发送换行符
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //清除缓存内容
			LED1 = 0;                 //点亮设置成功指示灯 

			DelaySec(1);     			//延时
			while(!Hand("OK"))       //等待设置成功
			{
				Delay_ms(Delay_Time);     //延时
				Send_ASCII("AT+CSCS=\"GSM\"");//设置GSM字符集
				Send_Hex(0x0d);            //发送回车符
	 			Send_Hex(0x0a);            //发送换行符
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //清除缓存内容
			LED1 = 0;                 //点亮设置成功指示灯
			
			DelaySec(1);
			while(!Hand("OK"))       //等待设置成功
			{
				Delay_ms(Delay_Time);     //延时
				Send_ASCII("AT+CNMI=2,1");//设置当有新短信到来时提示 
				Send_Hex(0x0d);            //发送回车符
	  			Send_Hex(0x0a);            //发送换行符
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //清除缓存内容
			LED2 = 0;                 //点亮设置成功指示灯
		
			DelaySec(1);
			while(!Hand("OK"))	      //等待设置成功
			{
				Delay_ms(Delay_Time);     //延时
				Send_ASCII("AT+CMGF=1");  //文本(TEXT)模式 
				Send_Hex(0x0d);            //发送回车符
				Send_Hex(0x0a);            //发送换行符
				DelaySec(1);
			}
			DelaySec(1);    			 //延时
			CLR_Buf();                //清除缓存内容
			

			DelaySec(1);     		//延时
			while(!Hand("OK"))     //判断是否握手成功,如果不成功延时一会,再发送指令
				{		   			//	不能是O，因为ERROR也有O
					DelaySec(3); 
					Send_ASCII("AT+CMGD=1,4");  //删除所有信息
					Send_Hex(0x0d);            //发送回车符
		 			Send_Hex(0x0a);            //发送换行符
					DelaySec(1);     			//延时
				}
			DelaySec(1);
			CLR_Buf();                //清除缓存内容
			

			DelaySec(1);
			while(!Hand("OK"))	      //等待设置成功
			{
				Send_ASCII("AT+CENG=1,1");  //开启工程模式
				Send_Hex(0x0d);            //发送回车符
	 			Send_Hex(0x0a);            //发送换行符
				DelaySec(1);     			//延时
			}
			Delay_ms(Delay_Time);     //延时
			CLR_Buf();                //清除缓存内容
			LED5=0;                 //点亮设置成功指示灯	
}

/*************************发送字符(十六进制)函数********************/
/*函数原型:void Send_Hex(unsigned char c)
/*函数功能:发送字符(十六进制)
/*输入参数:unsigned char c
/******************************************************************/ 
void Send_Hex(unsigned char c)         
{
    ES = 0;               //关串口中断
    SBUF = c;
    while(TI!=1);         //等待发送完成
    TI = 0;               //清除发送中断标志位
	ES = 1;               //开串口中断
}/****************************发送字符(ASCII)函数***********************/
/*函数原型:void Send_ASCII(unsigned char *b) 
/*函数功能:发送字符(ASCII码)
/*输入参数:unsigned char *b
/**********************************************************************/
void Send_ASCII(unsigned char *b)         
{
    ES = 0;                   //关串口中断
    for (b; *b!='\0';b++)
    	{
        	SBUF = *b;
        	while(TI!=1);     //等待发送完成
			TI = 0;           //清除发送中断标志位
		}	   
	ES = 1;                   //开串口中断
}
/**************************清除缓存数据函数****************************/
/*函数原型:void CLR_Buf(void)
/*函数功能:清除缓存数据
/**********************************************************************/
void CLR_Buf(void)
{
	unsigned char k;
	ES = 0;
    for(k=0;k<Buf_Max;k++)    //将缓存内容清零
	    {
			Rec_Buf[k] = 0;
		}
    i = 0;                    //接收字符串的起始存储位置
	//REN=0;
	ES = 1;
}
/*****************判断缓存中是否含有指定的字符串函数******************/
bit Hand(unsigned char *a)
{ 
    if(strstr(Rec_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}
/*************延时N秒的函数********************/
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
/****************************time ms延时函数***************************/
void Delay_ms(unsigned int time)
{
 unsigned char j;
 for(;time>0;time--)
 	{
  		j = 1500;
  		while(j--);
 	}
}
/*void delay(uint z)
{                           
	uchar x,y;
	for(x=z;x>0;x--)
		for(y=120;y>0;y--);
}*/
void fdx()//发定位短信
{
	
		Send_ASCII("AT+CMGS=\"15950973151\"\r\n");//此处修改为对方的电话号
		DelaySec(2);//延时3秒
		Send_ASCII(Rec_Buf);//修改短信内容
		DelaySec(1); 
		Send_Hex(0x1a);
	
		DelaySec(10);//延时15秒
}
void GPSWZ_dx()//发GPS数据
{
	
		Send_ASCII("AT+CMGS=\"15950973151\"\r\n");//此处修改为对方的电话号
		DelaySec(2);//延时3秒
		Send_ASCII(GPSJW);//修改短信内容 //这里以后改成GPS的数组
		DelaySec(1); 
		Send_Hex(0x1a);
	
		DelaySec(10);//延时15秒
}

void GPSWZ()//发GPS数据
{
	
		Send_ASCII("AT+CIPSEND");//
		DelaySec(2);//延时3秒
		Send_ASCII(GPSJW);//修改短信内容 //这里以后改成GPS的数组
		DelaySec(1); 
		Send_Hex(0x1a);
	
		DelaySec(10);//延时15秒
}
void door()
{
	 if(strstr(Rec_Buf,"OPENDOOR")!=NULL) 
	   {
    	beep=0;	//收到字符1,开锁，蜂鸣器响两声
		DelaySec(2);
		beep=1;	//收到字符1,开锁，蜂鸣器响两声
		DelaySec(2);
		beep=0;
		DelaySec(2);
		beep=1;
		CLR_Buf();
		}	  
	  if(strstr(Rec_Buf,"CLOSEDOOR")!=NULL)
	  {
	  	 	beep=0;	//收到字符1,开锁，蜂鸣器响两声
			DelaySec(4);
			beep=1;
			CLR_Buf();
	  }	
}
			
/*void door()
{
	 if(strstr(Rec_Buf,"OPENDOOR")!=NULL) 
			   {
		    	while(n<500)
					{ 
						 for (m=0; m<8; m++)       //一个周期转30度
						{
							P1 = table[m];  //取数据
							delay(50);                   //调节转速，时间不能为5，要不时间太快，反应不过来
						}
						n++;
					}
				n=0;
				CLR_Buf();
				}	  
	  if(strstr(Rec_Buf,"CLOSEDOOR")!=NULL)
	  {
	  	 while(n<500)
			{ 
				 for (m=0; m<8; m++)       //一个周期转30度
				{
					P1 = table1[m];  //取数据
					delay(50);                   //调节转速，时间不能为5，要不时间太快，反应不过来
				}
				n++;
			}
		n=0;
		CLR_Buf();
	  }	
}*/
////////////////////////////////////////////////////////////////////////
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
void Serial_Int2() interrupt 8 
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
			//UART2_SendStr(GPSJW,28);
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