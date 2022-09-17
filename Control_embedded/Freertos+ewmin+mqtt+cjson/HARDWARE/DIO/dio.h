#ifndef __DIO_H
#define __DIO_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define WK_UP PAin(0)	//PA0  WK_UP

#define IA5  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//读取按键0
#define IA6  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//读取按键0



#define OC0 PCout(0)// PC0输出
#define OC1 PCout(1)// PC1输出
#define OC2 PCout(2)// PC2输出
#define OC3 PCout(3)// PC3输出


void DIO_Init_in(void);//IO初始化
void DIO_Init_out(void);//IO初始化
u8 DIO_Scan(u8 mode);
				    
#endif
