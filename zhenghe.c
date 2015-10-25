///********************���ߵ�ʱ��ע��Ҫ���أ���ǧ��Ŀǰ������������������
/**********************p1�ڵ�1.2   1.3��Ҫ��0������GPS���׷�������***************************************/
#include <STC12C5A60S2.h>
#include <intrins.h>
#include<string.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
uint time;	   //�ӵ绰��ʱ
//uchar code table[8]={0xf1,0xf3,0xf2,0xf6,0xf4,0xfc,0xf8,0xf9}; //��ת
//uchar code table1[8]={0xf9,0xf8,0xfc,0xf4,0xf6,0xf2,0xf3,0xf1}; //��ת
uint n,m;
#define Buf_Max 100 //���泤��100
#define Delay_Time 3000  //��ʱ����
unsigned char i = 0;               //���建��ָ��
xdata unsigned  char Rec_Buf[Buf_Max];    //���建������
sbit beep=P3^7;
sbit LED1 = P1^0; //״ָ̬ʾ��1
sbit LED2 = P1^1; //״ָ̬ʾ��2
sbit LED3 = P1^2; //״ָ̬ʾ��3	//�������Ʋ����ã���������2��
sbit LED4 = P1^3; //״ָ̬ʾ��4	//�������Ʋ����ã���������2��
sbit LED5 = P1^4; //״ָ̬ʾ��5
sbit LED6 = P1^5; //״ָ̬ʾ��6
sbit LED7 = P1^6; //״ָ̬ʾ��7
sbit LED8 = P1^7; //״ָ̬ʾ��8

void Serial_Init(void);            //�������ڳ�ʼ������
void Send_Hex(unsigned char b);    //���������ַ�(ʮ������)����
void Send_ASCII(unsigned char *b); //���������ַ�(ASCII��)����
void Delay_ms(unsigned int time);  //������ʱ����
void DelaySec(int sec);			   //������ʱ������
//void delay(uint z);				   //�������������ʱ����Ҫ�ĳ������
void CLR_Buf(void);				   //��������������ݣ��Ա��´μ�����	
void gsmcsh(void);				   //GSMģ��ĳ�ʼ�������еĶ����ˣ��͵ȴ�������
bit  Hand(unsigned char *a);       //�����жϻ������Ƿ���ָ�����ַ�������
void fdx();						   //��������λ���ź���
void GPSWZ();					   //������˼�����ź���
void ddh();					   //������绰����
void door();				   //���������ź���
void GPSWZ_dx();
////////////////
#define S2_P4 0x10
#define Buf_Max2 72 //���泤��72
xdata unsigned  char Rec_Buf2[Buf_Max2];    //���建������
unsigned  char GPSJW[28]="NO,Receive-GPS!";    //����GPS��γ�Ȼ�������
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


/**********************************������******************************/
void main(void)
{			
			gsmcsh();
			UartInit2();//���ڳ�ʼ��
			UART2_SendStr(temp1,94);//������������ķ�ʽ���ͳ�ȥ
			DelaySec(1);			   		
			while(1)
				{ 	
	    			/*********************�ȴ����ն���********************/
					if(strstr(Rec_Buf,"CMTI")!=NULL)    //�������ַ����к���"+CMTI"�ͱ�ʾ���µĶ���
		        		
						{
							LED1 = 1;						//�����Ժ�ѵ�һ���ȵ���
							CLR_Buf();                   //������������ݣ��ȴ����ն�������
							while(!Hand("CMGR"))
							{
								Delay_ms(Delay_Time);        //��ʱ
								Send_ASCII("AT+CMGR=1");     //���Ͷ�ȡ��Ϣָ��
				    			Send_Hex(0x0d);               //���ͻس���
	 							Send_Hex(0x0a);               //���ͻ��з�
								DelaySec(1);  //������������ﲻ��ʱ���ܻᷢ�������������ԣ�����ͨ������Ϊ���滹û�浽������������
							}
							LED2 = 1;//��+CMGR: "REC UNREAD","15150218615","","15/05/17,20:06:44+32"
							DelaySec(1);//�ö���װ��������
		        			if(strstr(Rec_Buf,"DW")!=NULL) //�����Ϣ������"DW"
		            			{
								 LED7=1;
								 Delay_ms(Delay_Time);     //��ʱ
								  while(!Hand("CENG"))
								  {
								  		 Delay_ms(Delay_Time);     //��ʱ
										 Send_ASCII("AT+CENG?");  //������ģʽ����ȡ��λ��Ϣ
										 CLR_Buf(); 			  //���������������Ķ�������ֹ֮ǰ��������
										 Delay_ms(Delay_Time);
										 Send_Hex(0x0d);            //���ͻس���
							 			 Send_Hex(0x0a); 			 //���ͻ��з�
										 DelaySec(1);
										 DelaySec(1);          
								  }
								  LED6=1;
								  fdx();			  //�ҵ�Ŀ�������ⷢ���ţ��Ѷ�λ��Ϣ������
								  LED5=1;							
									CLR_Buf();                              //�����������
		 							DelaySec(1);
								}	                        
							
							
							if(strstr(Rec_Buf,"GPSWZ")!=NULL) //�����Ϣ������"GPSWZ"
		            			{
								 LED7=1;
								 Delay_ms(Delay_Time);     //��ʱ
								  GPSWZ_dx();
								  LED5=1;							
								CLR_Buf();
								DelaySec(1);
								}                          //�����������	
							
							door();
							
							while(!Hand("OK"))                    //�ȴ����óɹ�
							{
								Delay_ms(Delay_Time);                   //��ʱ
								Send_ASCII("AT+CMGD=1,4");                //ɾ����ȡ�����Ϣ
								Send_Hex(0x0d);                          //���ͻس���
	 							Send_Hex(0x0a);							 //���ͻ��з�
								DelaySec(1);                          
							}
							CLR_Buf();						          //�����������
							DelaySec(1); 
							P1=0x4f;                       
						} 
               /******************�ȴ����ն��Ž���***************************/
			   	
			  /*******************������绰����****************************/
			  if(strstr(Rec_Buf,"CLIP")!=NULL)//����⵽���绰��ʱ��
				{
					DelaySec(40);  //��С������40����
					Send_ASCII("ATA\r\n");
					CLR_Buf();
					DelaySec(2);
					
					while(!Hand("CARRIER"))       //����ⲻ���Է��ҶϾ�����ȴ�			
					{
					   DelaySec(60);
					   time++;
					   if(time>200)
					   	{
							time=0;
							break;
					   	}
					}//��һ�����ȥ����������
					CLR_Buf();                //�����������
					DelaySec(1);
					Send_ASCII("ATH\r\n");
					DelaySec(5);				//��ʱ5��

				}
			  /*******************������绰��������****************************/
			  door();
			  if(GPSJW[0]=='A')
			  {
			  	GPSWZ();
			  }
			}
}

/***************************���ڳ�ʼ������*****************************/
/*����ԭ��:void Serial_Init(void)
/*��������:���ڳ�ʼ��
/*�������:��
/*�������:��
/**********************************************************************/
void Serial_Init(void)//�����ʲ�����
{
 	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xDC;		//�趨��ʱ��ֵ
	TH1 = 0xDC;		//�趨��ʱ����װֵ
	TR1 = 1;		//������ʱ��1
	ES=1;
	EA=1;
}

/****************************�����жϴ�����**************************/
/*����ԭ��:void Serial_Int1(void)
/*��������:�����жϴ���
/*�������:��
/*�������:��
/**********************************************************************/
void Serial_Int1() interrupt 4 
{	
	ES = 0;	 //�ش����жϣ���ֹ�ж�Ƕ��
	if(TI)	 //����Ƿ����жϣ������κδ���
		{
			TI = 0;  //��������жϱ�־λ
		}
	if(RI)	 //����ǽ����жϣ�����д���
		{
			RI = 0; //��������жϱ�־λ
			Rec_Buf[i] = SBUF; //�����յ����ַ����浽������
			i++;               //����ָ������ƶ�
			if(i>Buf_Max-1)          //���������,������ָ��ָ�򻺴���׵�ַ
				{
					i=Buf_Max-1;
					//REN=1;//�����ٽ����ˣ�������������������
				}
		}
	ES = 1;	 //���������ж�
}
/****************************GSMģ���ʼ������***********************/
/*����ԭ��:void gsmcsh(void)
/*��������:�����еĳ�ʼ���������������ˣ��͵��Ž��ն�����
/*�������:��
/*�������:��
/**********************************************************************/
void gsmcsh(void)
{
	        Serial_Init();         //��ʼ������
			DelaySec(4);		  //��ʱ���룬�������ÿ�������
			CLR_Buf();             //�����������
	        DelaySec(1);        	//��ʱ1��
	        while(!Hand("OK"))     //�ж��Ƿ����ֳɹ�,������ɹ���ʱһ��,�ٷ���AT����ָ��
				{
					Send_ASCII("AT"); //��������ָ�� 
	                Send_Hex(0x0d);    //���ͻس���    
					Send_Hex(0x0a);    //���ͻ��з�
					P1=0x1c;
					DelaySec(1);   //��ʱ
				}
			DelaySec(1);			   //��ʱһ��
			CLR_Buf();                //�����������
			P1 = 0x4e;                 //�������ֳɹ�ָʾ��

		////////////////////////////////////
		 DelaySec(1);     			//��ʱ
			while(!Hand("OK"))       //�ȴ����óɹ�
			{
				Delay_ms(Delay_Time);     //��ʱ
				Send_ASCII("AT+CIPSHUT");//	�ر��ƶ�����
				Send_Hex(0x0d);            //���ͻس���
	 			Send_Hex(0x0a);            //���ͻ��з�
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //�����������
			//LED4=1;

			DelaySec(1);     			//��ʱ
			while(!Hand("OK"))       //�ȴ����óɹ�
			{
				Delay_ms(Delay_Time);     //��ʱ
				Send_ASCII("AT+CIPMUX=0");//
				Send_Hex(0x0d);            //���ͻس���
	 			Send_Hex(0x0a);            //���ͻ��з�
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //�����������
			LED5=1;

			DelaySec(1);     			//��ʱ
			while(!Hand("OK"))       //�ȴ����óɹ�
			{
				Delay_ms(Delay_Time);     //��ʱ
				Send_ASCII("AT+CIPRXGET=0");//
				Send_Hex(0x0d);            //���ͻس���
	 			Send_Hex(0x0a);            //���ͻ��з�
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //�����������
			LED5=0;

			DelaySec(1);     			//��ʱ
			while(!Hand("OK"))       //�ȴ����óɹ�
			{
				Delay_ms(Delay_Time);     //��ʱ
				Send_ASCII("AT+CIPQRCLOSE=1");//
				Send_Hex(0x0d);            //���ͻس���
	 			Send_Hex(0x0a);            //���ͻ��з�
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //�����������
			LED6=1;


			DelaySec(1);     			//��ʱ
			while(!Hand("OK"))       //�ȴ����óɹ�
			{
				Delay_ms(Delay_Time);     //��ʱ
				Send_ASCII("AT+CIPMODE=0");//
				Send_Hex(0x0d);            //���ͻس���
	 			Send_Hex(0x0a);            //���ͻ��з�
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //�����������
			LED7=0;

			DelaySec(1);     			//��ʱ
			while(!Hand("ALREADY"))       //�ȴ����óɹ�
			{
				Delay_ms(Delay_Time);     //��ʱ
				Send_ASCII("AT+CIPSTART=\"UDP\",\"128.199.225.127\",8080");//
				Send_Hex(0x0d);            //���ͻس���
	 			Send_Hex(0x0a);            //���ͻ��з�
				CLR_Buf();
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //�����������
			LED7=1;

		   ///////////////////////
		   DelaySec(1);     			//��ʱ
			while(!Hand("OK"))       //�ȴ����óɹ�
			{
				Delay_ms(Delay_Time);     //��ʱ
				Send_ASCII("AT+COLP=1");//�������к�����ʾ
				Send_Hex(0x0d);            //���ͻس���
	 			Send_Hex(0x0a);            //���ͻ��з�
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //�����������
			LED1 = 0;                 //�������óɹ�ָʾ�� 

			DelaySec(1);     			//��ʱ
			while(!Hand("OK"))       //�ȴ����óɹ�
			{
				Delay_ms(Delay_Time);     //��ʱ
				Send_ASCII("AT+CSCS=\"GSM\"");//����GSM�ַ���
				Send_Hex(0x0d);            //���ͻس���
	 			Send_Hex(0x0a);            //���ͻ��з�
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //�����������
			LED1 = 0;                 //�������óɹ�ָʾ��
			
			DelaySec(1);
			while(!Hand("OK"))       //�ȴ����óɹ�
			{
				Delay_ms(Delay_Time);     //��ʱ
				Send_ASCII("AT+CNMI=2,1");//���õ����¶��ŵ���ʱ��ʾ 
				Send_Hex(0x0d);            //���ͻس���
	  			Send_Hex(0x0a);            //���ͻ��з�
				DelaySec(1);
			}
			DelaySec(1);
			CLR_Buf();                //�����������
			LED2 = 0;                 //�������óɹ�ָʾ��
		
			DelaySec(1);
			while(!Hand("OK"))	      //�ȴ����óɹ�
			{
				Delay_ms(Delay_Time);     //��ʱ
				Send_ASCII("AT+CMGF=1");  //�ı�(TEXT)ģʽ 
				Send_Hex(0x0d);            //���ͻس���
				Send_Hex(0x0a);            //���ͻ��з�
				DelaySec(1);
			}
			DelaySec(1);    			 //��ʱ
			CLR_Buf();                //�����������
			

			DelaySec(1);     		//��ʱ
			while(!Hand("OK"))     //�ж��Ƿ����ֳɹ�,������ɹ���ʱһ��,�ٷ���ָ��
				{		   			//	������O����ΪERRORҲ��O
					DelaySec(3); 
					Send_ASCII("AT+CMGD=1,4");  //ɾ��������Ϣ
					Send_Hex(0x0d);            //���ͻس���
		 			Send_Hex(0x0a);            //���ͻ��з�
					DelaySec(1);     			//��ʱ
				}
			DelaySec(1);
			CLR_Buf();                //�����������
			

			DelaySec(1);
			while(!Hand("OK"))	      //�ȴ����óɹ�
			{
				Send_ASCII("AT+CENG=1,1");  //��������ģʽ
				Send_Hex(0x0d);            //���ͻس���
	 			Send_Hex(0x0a);            //���ͻ��з�
				DelaySec(1);     			//��ʱ
			}
			Delay_ms(Delay_Time);     //��ʱ
			CLR_Buf();                //�����������
			LED5=0;                 //�������óɹ�ָʾ��	
}

/*************************�����ַ�(ʮ������)����********************/
/*����ԭ��:void Send_Hex(unsigned char c)
/*��������:�����ַ�(ʮ������)
/*�������:unsigned char c
/******************************************************************/ 
void Send_Hex(unsigned char c)         
{
    ES = 0;               //�ش����ж�
    SBUF = c;
    while(TI!=1);         //�ȴ��������
    TI = 0;               //��������жϱ�־λ
	ES = 1;               //�������ж�
}/****************************�����ַ�(ASCII)����***********************/
/*����ԭ��:void Send_ASCII(unsigned char *b) 
/*��������:�����ַ�(ASCII��)
/*�������:unsigned char *b
/**********************************************************************/
void Send_ASCII(unsigned char *b)         
{
    ES = 0;                   //�ش����ж�
    for (b; *b!='\0';b++)
    	{
        	SBUF = *b;
        	while(TI!=1);     //�ȴ��������
			TI = 0;           //��������жϱ�־λ
		}	   
	ES = 1;                   //�������ж�
}
/**************************����������ݺ���****************************/
/*����ԭ��:void CLR_Buf(void)
/*��������:�����������
/**********************************************************************/
void CLR_Buf(void)
{
	unsigned char k;
	ES = 0;
    for(k=0;k<Buf_Max;k++)    //��������������
	    {
			Rec_Buf[k] = 0;
		}
    i = 0;                    //�����ַ�������ʼ�洢λ��
	//REN=0;
	ES = 1;
}
/*****************�жϻ������Ƿ���ָ�����ַ�������******************/
bit Hand(unsigned char *a)
{ 
    if(strstr(Rec_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}
/*************��ʱN��ĺ���********************/
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
/****************************time ms��ʱ����***************************/
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
void fdx()//����λ����
{
	
		Send_ASCII("AT+CMGS=\"15950973151\"\r\n");//�˴��޸�Ϊ�Է��ĵ绰��
		DelaySec(2);//��ʱ3��
		Send_ASCII(Rec_Buf);//�޸Ķ�������
		DelaySec(1); 
		Send_Hex(0x1a);
	
		DelaySec(10);//��ʱ15��
}
void GPSWZ_dx()//��GPS����
{
	
		Send_ASCII("AT+CMGS=\"15950973151\"\r\n");//�˴��޸�Ϊ�Է��ĵ绰��
		DelaySec(2);//��ʱ3��
		Send_ASCII(GPSJW);//�޸Ķ������� //�����Ժ�ĳ�GPS������
		DelaySec(1); 
		Send_Hex(0x1a);
	
		DelaySec(10);//��ʱ15��
}

void GPSWZ()//��GPS����
{
	
		Send_ASCII("AT+CIPSEND");//
		DelaySec(2);//��ʱ3��
		Send_ASCII(GPSJW);//�޸Ķ������� //�����Ժ�ĳ�GPS������
		DelaySec(1); 
		Send_Hex(0x1a);
	
		DelaySec(10);//��ʱ15��
}
void door()
{
	 if(strstr(Rec_Buf,"OPENDOOR")!=NULL) 
	   {
    	beep=0;	//�յ��ַ�1,������������������
		DelaySec(2);
		beep=1;	//�յ��ַ�1,������������������
		DelaySec(2);
		beep=0;
		DelaySec(2);
		beep=1;
		CLR_Buf();
		}	  
	  if(strstr(Rec_Buf,"CLOSEDOOR")!=NULL)
	  {
	  	 	beep=0;	//�յ��ַ�1,������������������
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
						 for (m=0; m<8; m++)       //һ������ת30��
						{
							P1 = table[m];  //ȡ����
							delay(50);                   //����ת�٣�ʱ�䲻��Ϊ5��Ҫ��ʱ��̫�죬��Ӧ������
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
				 for (m=0; m<8; m++)       //һ������ת30��
				{
					P1 = table1[m];  //ȡ����
					delay(50);                   //����ת�٣�ʱ�䲻��Ϊ5��Ҫ��ʱ��̫�죬��Ӧ������
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
	AUXR &= 0xF7;		//�����ʲ�����
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//���������ʷ�����ʱ��ΪFosc,��1T
	BRT = 0xDC;		//�趨���������ʷ�������װֵ
	AUXR |= 0x10;		//�������������ʷ�����
    IE2 =0x01;	//������2�ж�  ES2=1
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
			//UART2_SendStr(GPSJW,28);
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