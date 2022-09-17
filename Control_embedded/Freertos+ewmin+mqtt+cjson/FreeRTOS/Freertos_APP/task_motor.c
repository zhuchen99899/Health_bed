#include "task_motor.h"





//电机1任务
void motor_task(void *p_arg)
{
extern SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//电机1参数更改报文二值信号量句柄
extern QueueHandle_t BedControlKEY1_Queue;
extern QueueHandle_t Bedspeed1_Queue;
extern QueueHandle_t Bedlimit1_Queue;

	
u8 bedlimit1flag=0;
float bedspeed1=0;
BaseType_t err=pdFALSE;
	
	
BedCtrl1_t *BedCtrlKey1;
BedCtrl1_t  BedCtrl1_buff1;
BedCtrlKey1=&BedCtrl1_buff1;//消息队列结构体初始化

	

	BedCtrlKey1->Negative=0;
	BedCtrlKey1->Positive=0;


	
		while(1)
		{		
/********************电机1******************************/	
//【注意】后续增加电机后，xQueuePeek()函数不能使用portMAX_DELAY阻塞住任务		
err=xSemaphoreTake(BinarySemaphore_Motor1ValueChange,portMAX_DELAY);	//获取电机1信号量	
			
if(err==pdTRUE)
{			
	xQueuePeek(Bedlimit1_Queue,&bedlimit1flag,portMAX_DELAY);			
	xQueuePeek(BedControlKEY1_Queue,&BedCtrlKey1,portMAX_DELAY);
	xQueuePeek(Bedspeed1_Queue,&bedspeed1,portMAX_DELAY);	
	TIM_SetCompare2(TIM3,(25000-bedspeed1*100));
	
	
	/*********未限位**************/
	if(bedlimit1flag==0)
	{
		//换向1
			if((BedCtrlKey1->Negative==1)&&(BedCtrlKey1->Positive==0))
			{
				OC0=0;
				OC1=1;
			}
		//换向2
			else if((BedCtrlKey1->Negative==0)&&(BedCtrlKey1->Positive==1))
			{
				OC0=1;
				OC1=0;
			}
		//防出错
			else if((BedCtrlKey1->Negative==1)&&(BedCtrlKey1->Positive==1))
			{
				OC0=0;
				OC1=0;
			}
	}
		
/*********到达限位1**************/
	else if(bedlimit1flag==1)
	{
		
printf("限位1");
		//反向
			if((BedCtrlKey1->Negative==1)&&(BedCtrlKey1->Positive==0))
			{
				OC0=0;
				OC1=1;
			}
			else
			{
				//此外电机不能继续前进
				OC0=0;
				OC1=0;
			}
	}
	/*********到达限位1**************/
	else if(bedlimit1flag==2)
	{
printf("限位2");
			//反向
			if((BedCtrlKey1->Negative==0)&&(BedCtrlKey1->Positive==1))
			{
				OC0=1;
				OC1=0;
			}
			else
			{
				//此外电机不能继续前进
				OC0=0;
				OC1=0;
			}
	
	
	}
	
}
			
			
			vTaskDelay(15);
		}
		
		
}

