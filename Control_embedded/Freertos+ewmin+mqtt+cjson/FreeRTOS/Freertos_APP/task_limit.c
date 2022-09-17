#include "task_limit.h"





//限位任务
void limit_task(void *p_arg)
{

extern QueueHandle_t Bedlimit1_Queue;
extern SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//电机1参数更改报文二值信号量句柄



	
	u8 bedlimit1flag=0;
	static u8 IA5_up=1; //按键抬起
	static u8 IA6_up=1; //按键抬起	


	
		while(1)
		{
	

			/****翻身电机限位检测******/
//限位1
if(IA5_up&&IA5==0)	    
	{
		
		vTaskDelay(10);
		IA5_up=0;
			if(IA5==0)
			{
				 printf("限位标志1");
	bedlimit1flag=1;
	xQueueOverwrite(Bedlimit1_Queue,&bedlimit1flag);
	xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//发送电机1参数更改报文信号
			}			
	}
	else if(IA5==1) {		IA5_up=1;	}
	
//限位2
 if(IA6_up&&IA6==0)	
	{
		vTaskDelay(10);
		IA6_up=0;
			if(IA6==0)
			{
		 printf("限位标志2");			
	bedlimit1flag=2;
	xQueueOverwrite(Bedlimit1_Queue,&bedlimit1flag);
	xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//发送电机1参数更改报文信号
			}									
	}	
	else if(IA6==1) {IA6_up=1;} 

//正常无限位
	if((IA5_up&&IA5==1)&&(IA6_up&&IA6==1))
	{
	
		bedlimit1flag=0;
	xQueueOverwrite(Bedlimit1_Queue,&bedlimit1flag);
	xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//发送电机1参数更改报文信号
	}
	
	
	
	
	
	
	vTaskDelay(50);
	
	
		}//while(1)
		
		
}
