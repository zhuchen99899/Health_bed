#include "task_key.h"







//ͳ�����񣨿�������
void key_task(void *p_arg)
{

extern QueueHandle_t BedControlKEY1_Queue;
extern QueueHandle_t Bedspeed1_Queue;
 extern SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//���1�������ı��Ķ�ֵ�ź������
u8 key=0;
float bedspeed1=0;
BedCtrl1_t *BedCtrlKey1;
BedCtrl1_t  BedCtrl1_buff1;
BedCtrlKey1=&BedCtrl1_buff1;//��Ϣ���нṹ���ʼ��
	
	BedCtrlKey1->Negative=0;
	BedCtrlKey1->Positive=0;


	


		while(1)
		{

///*
//			��������[��Ҫ]
//��������Ҫ����	xQueuePeek() ��ȡ��ǰ��Ϣ�����еĵ��1����Ϣ����ֹ�����豸�޸ĺ�
//û��֪ͨ��������				
//*/

xQueuePeek(BedControlKEY1_Queue,&BedCtrlKey1,10);

		
	key=KEY_Scan(0);

		if(key==WKUP_PRES)  					//����
		{		 	

			if(0<=bedspeed1<250)bedspeed1+=50;

			xQueueOverwrite(Bedspeed1_Queue,&bedspeed1);
			xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//���͵��1�������ı����ź�
			
		}else if(key==KEY1_PRES)			//����
		{

			if(bedspeed1>50)bedspeed1-=50;
			else bedspeed1=0;
			xQueueOverwrite(Bedspeed1_Queue,&bedspeed1);
			xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//���͵��1�������ı����ź�
		}
		
	if(key==KEY0_PRES)  					//����
		{		 	
BedCtrlKey1->Negative=1;
BedCtrlKey1->Positive=0;
	xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//���͵��1�������ı����ź�
		}else if(key==KEY2_PRES)		//����
		{
			
BedCtrlKey1->Negative=0;
BedCtrlKey1->Positive=1;
			
xQueueOverwrite(BedControlKEY1_Queue,&BedCtrlKey1);
xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//���͵��1�������ı����ź�

		}	


		vTaskDelay(20);
		
		
			

		}
		
		
  


}
