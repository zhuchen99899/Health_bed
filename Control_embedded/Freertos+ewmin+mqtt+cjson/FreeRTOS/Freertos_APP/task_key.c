#include "task_key.h"







//统计任务（空闲任务）
void key_task(void *p_arg)
{

extern QueueHandle_t BedControlKEY1_Queue;
extern QueueHandle_t Bedspeed1_Queue;
 extern SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//电机1参数更改报文二值信号量句柄
u8 key=0;
float bedspeed1=0;
BedCtrl1_t *BedCtrlKey1;
BedCtrl1_t  BedCtrl1_buff1;
BedCtrlKey1=&BedCtrl1_buff1;//消息队列结构体初始化
	
	BedCtrlKey1->Negative=0;
	BedCtrlKey1->Positive=0;


	


		while(1)
		{

///*
//			！！！！[重要]
//后期这里要加入	xQueuePeek() 获取当前消息队列中的电机1的信息，防止其他设备修改后
//没有通知按键任务。				
//*/

xQueuePeek(BedControlKEY1_Queue,&BedCtrlKey1,10);

		
	key=KEY_Scan(0);

		if(key==WKUP_PRES)  					//加速
		{		 	

			if(0<=bedspeed1<250)bedspeed1+=50;

			xQueueOverwrite(Bedspeed1_Queue,&bedspeed1);
			xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//发送电机1参数更改报文信号
			
		}else if(key==KEY1_PRES)			//减速
		{

			if(bedspeed1>50)bedspeed1-=50;
			else bedspeed1=0;
			xQueueOverwrite(Bedspeed1_Queue,&bedspeed1);
			xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//发送电机1参数更改报文信号
		}
		
	if(key==KEY0_PRES)  					//反向
		{		 	
BedCtrlKey1->Negative=1;
BedCtrlKey1->Positive=0;
	xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//发送电机1参数更改报文信号
		}else if(key==KEY2_PRES)		//正向
		{
			
BedCtrlKey1->Negative=0;
BedCtrlKey1->Positive=1;
			
xQueueOverwrite(BedControlKEY1_Queue,&BedCtrlKey1);
xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//发送电机1参数更改报文信号

		}	


		vTaskDelay(20);
		
		
			

		}
		
		
  


}
