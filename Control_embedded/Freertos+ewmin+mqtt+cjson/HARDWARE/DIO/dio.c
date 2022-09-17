#include "stm32f10x.h"
#include "dio.h"
#include "sys.h" 
#include "delay.h"
/****************** 
��λ��IO��ʼ��
*******************/  
								    
//������ʼ������
void DIO_Init_in(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTFʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOF 01234

}
void DIO_Init_out(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				 //C 0123 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);					//PC0123 �����

}

//u8 DIO_Scan(u8 mode)
//{	 
//	static u8 DIO_up=1;//�������ɿ���־
//	if(mode) DIO_up=1;  //֧������		  
//	if(DIO_up&&(IF0==0||IF1==0||IF2==0||IF3==0))   
//	{
//		delay_ms(10);//ȥ���� 
//		DIO_up=0;
//		if(IF0==0)return IF0_PRES;
//		else if(IF1==0)return IF1_PRES;
//		else if(IF2==0)return IF2_PRES;
//		else if(IF3==0)return IF3_PRES;
//	}else if(IF0==1&&IF1==1&&IF2==1&&IF3==1)DIO_up=1; 	    
// 	return 0;// �ް�������

//}
//u8 DIO_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//�������ɿ���־
//	if(mode)key_up=1;  //֧������		  
//	if(key_up&&(IF0==0||IF1==0||IF2==0))
//	{
//		delay_ms(10);//ȥ���� 
//		key_up=0;
//		if(IF0==0)return 1;
//		else if(IF1==0)return 2;
//		else if(IF2==0)return 3;
//	}else if(IF0!=0&&IF1!=0&&IF2!=0)key_up=1; 	    
// 	return 0;// �ް�������
//}


