#include "task_limit.h"





//��λ����
void limit_task(void *p_arg)
{

extern QueueHandle_t Bedlimit1_Queue;
extern SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//���1�������ı��Ķ�ֵ�ź������



	
	u8 bedlimit1flag=0;
	static u8 IA5_up=1; //����̧��
	static u8 IA6_up=1; //����̧��	


	
		while(1)
		{
	

			/****��������λ���******/
//��λ1
if(IA5_up&&IA5==0)	    
	{
		
		vTaskDelay(10);
		IA5_up=0;
			if(IA5==0)
			{
				 printf("��λ��־1");
	bedlimit1flag=1;
	xQueueOverwrite(Bedlimit1_Queue,&bedlimit1flag);
	xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//���͵��1�������ı����ź�
			}			
	}
	else if(IA5==1) {		IA5_up=1;	}
	
//��λ2
 if(IA6_up&&IA6==0)	
	{
		vTaskDelay(10);
		IA6_up=0;
			if(IA6==0)
			{
		 printf("��λ��־2");			
	bedlimit1flag=2;
	xQueueOverwrite(Bedlimit1_Queue,&bedlimit1flag);
	xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//���͵��1�������ı����ź�
			}									
	}	
	else if(IA6==1) {IA6_up=1;} 

//��������λ
	if((IA5_up&&IA5==1)&&(IA6_up&&IA6==1))
	{
	
		bedlimit1flag=0;
	xQueueOverwrite(Bedlimit1_Queue,&bedlimit1flag);
	xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//���͵��1�������ı����ź�
	}
	
	
	
	
	
	
	vTaskDelay(50);
	
	
		}//while(1)
		
		
}
