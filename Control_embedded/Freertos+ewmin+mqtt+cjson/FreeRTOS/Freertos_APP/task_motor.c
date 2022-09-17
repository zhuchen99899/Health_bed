#include "task_motor.h"





//���1����
void motor_task(void *p_arg)
{
extern SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//���1�������ı��Ķ�ֵ�ź������
extern QueueHandle_t BedControlKEY1_Queue;
extern QueueHandle_t Bedspeed1_Queue;
extern QueueHandle_t Bedlimit1_Queue;

	
u8 bedlimit1flag=0;
float bedspeed1=0;
BaseType_t err=pdFALSE;
	
	
BedCtrl1_t *BedCtrlKey1;
BedCtrl1_t  BedCtrl1_buff1;
BedCtrlKey1=&BedCtrl1_buff1;//��Ϣ���нṹ���ʼ��

	

	BedCtrlKey1->Negative=0;
	BedCtrlKey1->Positive=0;


	
		while(1)
		{		
/********************���1******************************/	
//��ע�⡿�������ӵ����xQueuePeek()��������ʹ��portMAX_DELAY����ס����		
err=xSemaphoreTake(BinarySemaphore_Motor1ValueChange,portMAX_DELAY);	//��ȡ���1�ź���	
			
if(err==pdTRUE)
{			
	xQueuePeek(Bedlimit1_Queue,&bedlimit1flag,portMAX_DELAY);			
	xQueuePeek(BedControlKEY1_Queue,&BedCtrlKey1,portMAX_DELAY);
	xQueuePeek(Bedspeed1_Queue,&bedspeed1,portMAX_DELAY);	
	TIM_SetCompare2(TIM3,(25000-bedspeed1*100));
	
	
	/*********δ��λ**************/
	if(bedlimit1flag==0)
	{
		//����1
			if((BedCtrlKey1->Negative==1)&&(BedCtrlKey1->Positive==0))
			{
				OC0=0;
				OC1=1;
			}
		//����2
			else if((BedCtrlKey1->Negative==0)&&(BedCtrlKey1->Positive==1))
			{
				OC0=1;
				OC1=0;
			}
		//������
			else if((BedCtrlKey1->Negative==1)&&(BedCtrlKey1->Positive==1))
			{
				OC0=0;
				OC1=0;
			}
	}
		
/*********������λ1**************/
	else if(bedlimit1flag==1)
	{
		
printf("��λ1");
		//����
			if((BedCtrlKey1->Negative==1)&&(BedCtrlKey1->Positive==0))
			{
				OC0=0;
				OC1=1;
			}
			else
			{
				//���������ܼ���ǰ��
				OC0=0;
				OC1=0;
			}
	}
	/*********������λ1**************/
	else if(bedlimit1flag==2)
	{
printf("��λ2");
			//����
			if((BedCtrlKey1->Negative==0)&&(BedCtrlKey1->Positive==1))
			{
				OC0=1;
				OC1=0;
			}
			else
			{
				//���������ܼ���ǰ��
				OC0=0;
				OC1=0;
			}
	
	
	}
	
}
			
			
			vTaskDelay(15);
		}
		
		
}

