#include "includes.h"
/***********************************************************
						变量定义
************************************************************/



/********************************消息队列句柄声明*********************************/
QueueHandle_t Key_Queue;
QueueHandle_t BedControlKEY1_Queue;
QueueHandle_t Bedlimit1_Queue;
QueueHandle_t Wifi_buffer_Queue;
QueueHandle_t PINGREQ_Queue;
QueueHandle_t Bedspeed1_Queue;

/********************************信号量句柄声明*********************************/
SemaphoreHandle_t BinarySemaphore_USART2ISR;	
SemaphoreHandle_t BinarySemaphore_MQTTconnect;
SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;
SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//电机1参数更改报文二值信号量句柄

/***********************************************************
					任务函数声明
************************************************************/


void start_task(void *pvParameters);		//Start任务
void touch_task(void *pvParameters);		//TOUCH任务
void UserIf_task(void *pvParameters);		//空闲任务
void motor_task(void *pvParameters);		//PWM任务
void emwin_task(void *pvParameters); 	//emwin任务
void led_task(void *pvParameters);       //led任务
void key_task(void *pvParameters);			//key任务
void limit_task(void *pvParameters);    //limit任务
void mqttconnect_task(void *pvParameters);//mqtt connect任务
void mqttrec_task(void *pvParameters);   //mqtt rec任务
void MQTT_Pingreq_task(void *pvParameters);				// MQTTPINGREQ任务
void MQTT_Subscribe_task(void *pvParameters);				// MQTTSUBSCRIBE任务

/***********************************************************
						任务函数句柄
************************************************************/
//任务函数句柄
TaskHandle_t StartTask_Handler;			//Start任务
TaskHandle_t TouchTask_Handler;			//TOUCH任务
TaskHandle_t UserIfTask_Handler;		//空闲任务
TaskHandle_t MotorTask_Handler;		   //PWM任务
TaskHandle_t EmwinTask_Handler;		//emwin任务
TaskHandle_t LedTask_Handler;		//led任务
TaskHandle_t KeyTask_Handler;   //key任务 
TaskHandle_t Limit_Handler;			//limit任务
TaskHandle_t MqttConnect_Handler;			//mqtt connect任务
TaskHandle_t MqttRec_Handler;					//mqtt rec任务
TaskHandle_t MQTT_PINGREQ_task_Handler;		  // MQTTPINGREQ任务
TaskHandle_t MQTT_SUBSCRIBE_task_Handler;				// MQTTSUBSCRIBE任务


/***********************************************************
						任务堆栈大小
************************************************************/
#define START_STK_SIZE 			256  	//Start任务
#define TOUCH_STK_SIZE			128		//TOUCH任务
#define USERIF_STK_SIZE			128		//空闲任务
#define MOTOR_STK_SIZE			256		//空闲任务
#define EMWIN_STK_SIZE		512		//emwin任务
#define LED_STK_SIZE     128    //LED任务
#define KEY_STK_SIZE     128     //KEY任务
#define LIMIT_STK_SIZE     128    //limit任务
#define MqttConnect_SIZE     512    //mqtt connect任务
#define MqttRec_SIZE  			512    //mqtt rec任务
#define MQTT_PINGREQ_STK_SIZE		256	// MQTTPINGREQ任务
#define MQTT_SUBSCRIBE_STK_SIZE		256		// MQTTSUBSCRIBE任务


/***********************************************************
						 任务优先级(数值越小优先级越低)
************************************************************/
#define START_TASK_PRIO			5		//Start任务
#define TOUCH_TASK_PRIO			0		//TOUCH任务
#define USERIF_TASK_PRIO 		0	   //空闲任务
#define MOTOR_TASK_PRIO 		  2	    //PWM任务
#define EMWIN_TASK_PRIO		  0		  //emwin任务
#define LED_TASK_PRIO       1    //led任务
#define KEY_TASK_PRIO       1   //KEY任务
#define LIMIT_TASK_PRIO       2   //KEY任务
#define MQTTCONNECT_TASK_PRIO       3  //mqtt connect任务
#define MQTTREC_TASK_PRIO							1//mqtt rec 任务
#define MQTT_PINGREQ_TASK_PRIO		2		// MQTTPINGREQ任务
#define MQTT_SUBSCRIBE_TASK_PRIO		2		// MQTTSUBSCRIBE任务


/***********************************************************
						 主函数入口
************************************************************/

int main(void)
{
	
/*
 在启动调度前，为了防止初始化 STM32 外设时有中断服务程序执行，这里禁止全局中断(除了 NMI 和 HardFault)。
 这样做的好处是：
 1. 防止执行的中断服务程序中有 FreeRTOS 的 API 函数。
 2. 保证系统正常启动，不受别的中断影响。
 3. 关于是否关闭全局中断，大家根据自己的实际情况设置即可。
 在移植文件 port.c 中的函数 prvStartFirstTask 中会重新开启全局中断。通过指令 cpsie i 开启，__set_PRIMASK(1)
 和 cpsie i 是等效的。
 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 
	delay_init();	    				//延时函数初始化	 
	uart_init(115200);					//初始化串口
	wifi_init(115200);					//初始化wifi
	LED_Init();		  					//初始化LED
	KEY_Init();							//初始化按键
	DIO_Init_in();			 //输入初始化
	DIO_Init_out();				 //输出初始化

	TIM3_PWM_Init(24999,0);	//TIM1 PWM初始化, Fpwm=72M/25000=2.88Khz.
  TIM_SetCompare2(TIM3,25000);//初始值为0	
	TIM4_Int_Init(1799,0);//PWM初始化
	TFTLCD_Init();						//LCD初始化	
	FSMC_SRAM_Init();					//初始化SRAM
	TP_Init();							//触摸屏初始化

	
	my_mem_init(SRAMIN);            	//初始化内部内存池
	my_mem_init(SRAMEX);				//初始化外部内存池
	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄                
    vTaskStartScheduler();          //开启任务调度
/*
 如果系统正常启动是不会运行到这里的，运行到这里极有可能是用于定时器任务或者空闲任务的
 heap 空间不足造成创建失败，此要加大 FreeRTOSConfig.h 文件中定义的 heap 大小：
 #define configTOTAL_HEAP_SIZE ( ( size_t ) ( 17 * 1024 ) )
*/
	while(1);			
}

/***********************************************************
						 任务函数
************************************************************/

//开始任务任务函数
void start_task(void *pvParameters)
{
	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//开启CRC时钟
	GUI_Init();  					//STemWin初始化
	WM_SetCreateFlags(WM_CF_MEMDEV);
  taskENTER_CRITICAL();           //进入临界区
	

	
	OS_AppObjCreate();       //创建任务间通讯机制
	
	//发送MQTT任务信号量
	xSemaphoreGive(BinarySemaphore_MQTTconnect);//发送MQTT Connect报文信号

	
	//创建触摸任务
    xTaskCreate((TaskFunction_t )touch_task,             
                (const char*    )"touch_task",           
                (uint16_t       )TOUCH_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TOUCH_TASK_PRIO,        
                (TaskHandle_t*  )&TouchTask_Handler);   	
    //创建空闲任务
    xTaskCreate((TaskFunction_t )UserIf_task,             
                (const char*    )"UserIf",           
                (uint16_t       )USERIF_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )USERIF_TASK_PRIO,        
                (TaskHandle_t*  )&UserIfTask_Handler);  
   //创建PWM任务
    xTaskCreate((TaskFunction_t )motor_task,            
                (const char*    )"motor_task",           
                (uint16_t       )MOTOR_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MOTOR_TASK_PRIO,        
                (TaskHandle_t*  )&MotorTask_Handler);  
    //创建gui任务
    xTaskCreate((TaskFunction_t )emwin_task,             
                (const char*    )"emwin_task",           
                (uint16_t       )EMWIN_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EMWIN_TASK_PRIO,        
                (TaskHandle_t*  )&EmwinTask_Handler);   	

		//创建led 任务
    xTaskCreate((TaskFunction_t )led_task,             
                (const char*    )"led_task",           
                (uint16_t       )LED_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )LED_TASK_PRIO,        
                (TaskHandle_t*  )&LedTask_Handler); 

		//创建key 任务		
		xTaskCreate((TaskFunction_t )key_task,             
                (const char*    )"key_task",           
                (uint16_t       )KEY_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )KEY_TASK_PRIO,        
                (TaskHandle_t*  )&KeyTask_Handler);	
		//创建限位任务
		xTaskCreate((TaskFunction_t )limit_task,             
                (const char*    )"limit_task",           
                (uint16_t       )LIMIT_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )LIMIT_TASK_PRIO,        
                (TaskHandle_t*  )&Limit_Handler);	
								
		//创建MQTTconnect任务
		xTaskCreate((TaskFunction_t )mqttconnect_task,             
                (const char*    )"mqttconnect_task",           
                (uint16_t       )MqttConnect_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTTCONNECT_TASK_PRIO,        
                (TaskHandle_t*  )&MqttConnect_Handler);	
														
		//创建MQTTrec任务
		xTaskCreate((TaskFunction_t )mqttrec_task,             
                (const char*    )"mqttrec_task",           
                (uint16_t       )MqttRec_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTTREC_TASK_PRIO,        
                (TaskHandle_t*  )&MqttRec_Handler);	
																										

			 //创建MQTT PINGREQ任务			
		   xTaskCreate((TaskFunction_t )MQTT_Pingreq_task,             
                (const char*    )"MQTT_PINGREQ_Task",           
                (uint16_t       )MQTT_PINGREQ_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_PINGREQ_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_PINGREQ_task_Handler);
				
				//创建MQTT SUBSCRIBE任务			
		   xTaskCreate((TaskFunction_t )MQTT_Subscribe_task,             
                (const char*    )"MQTT_SUBSCRIBE_Task",           
                (uint16_t       )MQTT_SUBSCRIBE_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_SUBSCRIBE_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_SUBSCRIBE_task_Handler);
												
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

  

