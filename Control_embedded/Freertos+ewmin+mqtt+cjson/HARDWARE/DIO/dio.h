#ifndef __DIO_H
#define __DIO_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define WK_UP PAin(0)	//PA0  WK_UP

#define IA5  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//��ȡ����0
#define IA6  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//��ȡ����0



#define OC0 PCout(0)// PC0���
#define OC1 PCout(1)// PC1���
#define OC2 PCout(2)// PC2���
#define OC3 PCout(3)// PC3���


void DIO_Init_in(void);//IO��ʼ��
void DIO_Init_out(void);//IO��ʼ��
u8 DIO_Scan(u8 mode);
				    
#endif
