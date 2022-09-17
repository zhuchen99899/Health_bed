#include "timer.h"
#include "led.h"
#include "usart.h"
#include "malloc.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

//FreeRTOSʱ��ͳ�����õĽ��ļ�����
volatile unsigned long long FreeRTOSRunTimeTicks;

//��ʼ��TIM4ʹ��ΪFreeRTOS��ʱ��ͳ���ṩʱ��
void ConfigureTimeForRunTimeStats(void)
{
	//��ʱ��3��ʼ������ʱ��ʱ��Ϊ72M����Ƶϵ��Ϊ72-1�����Զ�ʱ��3��Ƶ��
	//Ϊ72M/72=1M���Զ���װ��Ϊ50-1����ô��ʱ�����ھ���50us
	FreeRTOSRunTimeTicks=0;
	TIM4_Int_Init(50-1,72-1);	//��ʼ��TIM3
}

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�4��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx					 
}


//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{
		FreeRTOSRunTimeTicks++;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
}





void Timer3_Init(u16 arr,u16 psc)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
NVIC_InitTypeDef NVIC_Initstrue;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //ͨ�ö�ʱ��3RCCʱ��ʹ��
	
	TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;          //��Ԥ��Ƶǰ����ʱ����Ӱ��Ԥ��Ƶϵ������������
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;   //���ϼ���
	TIM_TimeBaseInitStrue.TIM_Period=arr;         //�Զ�װ��ֵ
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc;            //ʱ��Ԥ��Ƶϵ��

TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);//��ʱ��3�Ĵ�����ʼ��

TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//��ʱ��3�ж����ã������жϣ���ĳ���Ĵ������£�����жϣ�

	
	NVIC_Initstrue.NVIC_IRQChannel= TIM3_IRQn;  //TIM3�ж�ͨ��
	NVIC_Initstrue.NVIC_IRQChannelCmd=ENABLE;	//IRQ�ж�ͨ��ʹ��
	NVIC_Initstrue.NVIC_IRQChannelPreemptionPriority=0; //��ռ���ȼ�Ϊ2
	NVIC_Initstrue.NVIC_IRQChannelSubPriority=3;	//�����ȼ�Ϊ2
	NVIC_Init(&NVIC_Initstrue); //��ʼ��NVIC�ж����ȼ��Ĵ���
	
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	
}




