#include "stm32f10x.h"
#include "dio.h"
#include "sys.h" 
#include "delay.h"
/****************** 
限位器IO初始化
*******************/  
								    
//按键初始化函数
void DIO_Init_in(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTF时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOF 01234

}
void DIO_Init_out(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				 //C 0123 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);					//PC0123 输出高

}

//u8 DIO_Scan(u8 mode)
//{	 
//	static u8 DIO_up=1;//按键按松开标志
//	if(mode) DIO_up=1;  //支持连按		  
//	if(DIO_up&&(IF0==0||IF1==0||IF2==0||IF3==0))   
//	{
//		delay_ms(10);//去抖动 
//		DIO_up=0;
//		if(IF0==0)return IF0_PRES;
//		else if(IF1==0)return IF1_PRES;
//		else if(IF2==0)return IF2_PRES;
//		else if(IF3==0)return IF3_PRES;
//	}else if(IF0==1&&IF1==1&&IF2==1&&IF3==1)DIO_up=1; 	    
// 	return 0;// 无按键按下

//}
//u8 DIO_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//按键按松开标志
//	if(mode)key_up=1;  //支持连按		  
//	if(key_up&&(IF0==0||IF1==0||IF2==0))
//	{
//		delay_ms(10);//去抖动 
//		key_up=0;
//		if(IF0==0)return 1;
//		else if(IF1==0)return 2;
//		else if(IF2==0)return 3;
//	}else if(IF0!=0&&IF1!=0&&IF2!=0)key_up=1; 	    
// 	return 0;// 无按键按下
//}


