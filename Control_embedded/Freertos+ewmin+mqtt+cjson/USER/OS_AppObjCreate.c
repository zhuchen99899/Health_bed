#include <OS_AppObjCreate.h>

WIFIMSG WIFI_MSG;
BedCtrl1_t BedCtrl1_Msg;

/***********************************************************
						 创建消息队列
************************************************************/


#define KEYMSG_Q_NUM    1  		//按键消息队列的数量  
#define BedControlKEY1_Q_NUM   1     //翻身按键消息队列
#define BedLimit1_Q_NUM 1 //翻身限位消息队列数量
#define WIFIMESSAGE_buffer_Q_NUM   1   	//wifi接收数据的消息队列的数量 
#define PINGREQ_Q_NUM 1 //ping 消息队列数量

//按键消息队列
void Key_QueueCreat(void)
{
	extern QueueHandle_t Key_Queue;
	Key_Queue =xQueueCreate(KEYMSG_Q_NUM,sizeof(uint8_t));
	
	if (Key_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("按键消息队列创建失败");
	}
	else 
	{
	printf("按键消息队列创建成功");
	}
}


void BedControlKEY1_QueueCreat(void)
{
	extern QueueHandle_t BedControlKEY1_Queue;
	BedControlKEY1_Queue = xQueueCreate(BedControlKEY1_Q_NUM,sizeof(BedCtrl1_Msg ));
	if (BedControlKEY1_Queue==0)
	{
	/*消息创建失败处理机制*/
		
	printf("电机1控制消息队列创建失败");
	}
	else 
	{
	printf("电机1控制消息队列创建成功");
	}
}

void Bedlimit1_QueueCreat(void)
{
	extern QueueHandle_t Bedlimit1_Queue;
	Bedlimit1_Queue =xQueueCreate(PINGREQ_Q_NUM,sizeof(uint8_t));
	
	if (Bedlimit1_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("Bedlimit1消息队列创建失败");
	}
	else 
	{
	printf("Bedlimit1消息队列创建成功");
	}
}

void Bedspeed1_QueueCreat(void)
{
	extern QueueHandle_t Bedspeed1_Queue;
	Bedspeed1_Queue =xQueueCreate(PINGREQ_Q_NUM,sizeof(float));
	
	if (Bedspeed1_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("Bedspeed1_Queue消息队列创建失败");
	}
	else 
	{
	printf("Bedspeed1_Queue消息队列创建成功");
		
	}
}

void WIFI_buffer_QueueCreat(void)
{
	extern QueueHandle_t Wifi_buffer_Queue;
	Wifi_buffer_Queue =xQueueCreate(WIFIMESSAGE_buffer_Q_NUM,sizeof(struct WIFI_MSG *));
	
	if (Wifi_buffer_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("wifi接收消息队列创建失败");
	}
	else 
	{
	printf("wifi接收消息队列创建成功");
	}
}

void PINGREQ_QueueCreat(void)
{
	extern QueueHandle_t PINGREQ_Queue;
	PINGREQ_Queue =xQueueCreate(PINGREQ_Q_NUM,sizeof(uint8_t));
	
	if (PINGREQ_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("PINGREQ消息队列创建失败");
	}
	else 
	{
	printf("PINGREQ消息队列创建成功");
	}
}



void QueueCreate(void)
{

Key_QueueCreat();
BedControlKEY1_QueueCreat();
Bedlimit1_QueueCreat();
Bedspeed1_QueueCreat();
WIFI_buffer_QueueCreat();
PINGREQ_QueueCreat();
	
}

/***********************************************************
						 信号量创建
************************************************************/

extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2空闲中断二值信号量句柄 (wifi)
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET报文二值信号量句柄
extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE报文二值信号量句柄

extern SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//电机1参数更改报文二值信号量句柄
void BinarySemaphoreCreate(void)
{
	BinarySemaphore_USART2ISR=xSemaphoreCreateBinary();	
	BinarySemaphore_MQTTconnect=xSemaphoreCreateBinary();
	BinarySemaphore_MQTTsubscribe=xSemaphoreCreateBinary();
	BinarySemaphore_Motor1ValueChange=xSemaphoreCreateBinary();
};




void OS_AppObjCreate(void)
{
//消息队列创建
QueueCreate();
BinarySemaphoreCreate();

	
	
}

 
 
 
 
 
 
 
 
 
 
 
 







