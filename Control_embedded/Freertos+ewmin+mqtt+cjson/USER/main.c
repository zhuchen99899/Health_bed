#include "includes.h"
/***********************************************************
						��������
************************************************************/



/********************************��Ϣ���о������*********************************/
QueueHandle_t Key_Queue;
QueueHandle_t BedControlKEY1_Queue;
QueueHandle_t Bedlimit1_Queue;
QueueHandle_t Wifi_buffer_Queue;
QueueHandle_t PINGREQ_Queue;
QueueHandle_t Bedspeed1_Queue;

/********************************�ź����������*********************************/
SemaphoreHandle_t BinarySemaphore_USART2ISR;	
SemaphoreHandle_t BinarySemaphore_MQTTconnect;
SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;
SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//���1�������ı��Ķ�ֵ�ź������

/***********************************************************
					����������
************************************************************/


void start_task(void *pvParameters);		//Start����
void touch_task(void *pvParameters);		//TOUCH����
void UserIf_task(void *pvParameters);		//��������
void motor_task(void *pvParameters);		//PWM����
void emwin_task(void *pvParameters); 	//emwin����
void led_task(void *pvParameters);       //led����
void key_task(void *pvParameters);			//key����
void limit_task(void *pvParameters);    //limit����
void mqttconnect_task(void *pvParameters);//mqtt connect����
void mqttrec_task(void *pvParameters);   //mqtt rec����
void MQTT_Pingreq_task(void *pvParameters);				// MQTTPINGREQ����
void MQTT_Subscribe_task(void *pvParameters);				// MQTTSUBSCRIBE����

/***********************************************************
						���������
************************************************************/
//���������
TaskHandle_t StartTask_Handler;			//Start����
TaskHandle_t TouchTask_Handler;			//TOUCH����
TaskHandle_t UserIfTask_Handler;		//��������
TaskHandle_t MotorTask_Handler;		   //PWM����
TaskHandle_t EmwinTask_Handler;		//emwin����
TaskHandle_t LedTask_Handler;		//led����
TaskHandle_t KeyTask_Handler;   //key���� 
TaskHandle_t Limit_Handler;			//limit����
TaskHandle_t MqttConnect_Handler;			//mqtt connect����
TaskHandle_t MqttRec_Handler;					//mqtt rec����
TaskHandle_t MQTT_PINGREQ_task_Handler;		  // MQTTPINGREQ����
TaskHandle_t MQTT_SUBSCRIBE_task_Handler;				// MQTTSUBSCRIBE����


/***********************************************************
						�����ջ��С
************************************************************/
#define START_STK_SIZE 			256  	//Start����
#define TOUCH_STK_SIZE			128		//TOUCH����
#define USERIF_STK_SIZE			128		//��������
#define MOTOR_STK_SIZE			256		//��������
#define EMWIN_STK_SIZE		512		//emwin����
#define LED_STK_SIZE     128    //LED����
#define KEY_STK_SIZE     128     //KEY����
#define LIMIT_STK_SIZE     128    //limit����
#define MqttConnect_SIZE     512    //mqtt connect����
#define MqttRec_SIZE  			512    //mqtt rec����
#define MQTT_PINGREQ_STK_SIZE		256	// MQTTPINGREQ����
#define MQTT_SUBSCRIBE_STK_SIZE		256		// MQTTSUBSCRIBE����


/***********************************************************
						 �������ȼ�(��ֵԽС���ȼ�Խ��)
************************************************************/
#define START_TASK_PRIO			5		//Start����
#define TOUCH_TASK_PRIO			0		//TOUCH����
#define USERIF_TASK_PRIO 		0	   //��������
#define MOTOR_TASK_PRIO 		  2	    //PWM����
#define EMWIN_TASK_PRIO		  0		  //emwin����
#define LED_TASK_PRIO       1    //led����
#define KEY_TASK_PRIO       1   //KEY����
#define LIMIT_TASK_PRIO       2   //KEY����
#define MQTTCONNECT_TASK_PRIO       3  //mqtt connect����
#define MQTTREC_TASK_PRIO							1//mqtt rec ����
#define MQTT_PINGREQ_TASK_PRIO		2		// MQTTPINGREQ����
#define MQTT_SUBSCRIBE_TASK_PRIO		2		// MQTTSUBSCRIBE����


/***********************************************************
						 ���������
************************************************************/

int main(void)
{
	
/*
 ����������ǰ��Ϊ�˷�ֹ��ʼ�� STM32 ����ʱ���жϷ������ִ�У������ֹȫ���ж�(���� NMI �� HardFault)��
 �������ĺô��ǣ�
 1. ��ִֹ�е��жϷ���������� FreeRTOS �� API ������
 2. ��֤ϵͳ�������������ܱ���ж�Ӱ�졣
 3. �����Ƿ�ر�ȫ���жϣ���Ҹ����Լ���ʵ��������ü��ɡ�
 ����ֲ�ļ� port.c �еĺ��� prvStartFirstTask �л����¿���ȫ���жϡ�ͨ��ָ�� cpsie i ������__set_PRIMASK(1)
 �� cpsie i �ǵ�Ч�ġ�
 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	 
	uart_init(115200);					//��ʼ������
	wifi_init(115200);					//��ʼ��wifi
	LED_Init();		  					//��ʼ��LED
	KEY_Init();							//��ʼ������
	DIO_Init_in();			 //�����ʼ��
	DIO_Init_out();				 //�����ʼ��

	TIM3_PWM_Init(24999,0);	//TIM1 PWM��ʼ��, Fpwm=72M/25000=2.88Khz.
  TIM_SetCompare2(TIM3,25000);//��ʼֵΪ0	
	TIM4_Int_Init(1799,0);//PWM��ʼ��
	TFTLCD_Init();						//LCD��ʼ��	
	FSMC_SRAM_Init();					//��ʼ��SRAM
	TP_Init();							//��������ʼ��

	
	my_mem_init(SRAMIN);            	//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);				//��ʼ���ⲿ�ڴ��
	
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������                
    vTaskStartScheduler();          //�����������
/*
 ���ϵͳ���������ǲ������е�����ģ����е����Ｋ�п��������ڶ�ʱ��������߿��������
 heap �ռ䲻����ɴ���ʧ�ܣ���Ҫ�Ӵ� FreeRTOSConfig.h �ļ��ж���� heap ��С��
 #define configTOTAL_HEAP_SIZE ( ( size_t ) ( 17 * 1024 ) )
*/
	while(1);			
}

/***********************************************************
						 ������
************************************************************/

//��ʼ����������
void start_task(void *pvParameters)
{
	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//����CRCʱ��
	GUI_Init();  					//STemWin��ʼ��
	WM_SetCreateFlags(WM_CF_MEMDEV);
  taskENTER_CRITICAL();           //�����ٽ���
	

	
	OS_AppObjCreate();       //���������ͨѶ����
	
	//����MQTT�����ź���
	xSemaphoreGive(BinarySemaphore_MQTTconnect);//����MQTT Connect�����ź�

	
	//������������
    xTaskCreate((TaskFunction_t )touch_task,             
                (const char*    )"touch_task",           
                (uint16_t       )TOUCH_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TOUCH_TASK_PRIO,        
                (TaskHandle_t*  )&TouchTask_Handler);   	
    //������������
    xTaskCreate((TaskFunction_t )UserIf_task,             
                (const char*    )"UserIf",           
                (uint16_t       )USERIF_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )USERIF_TASK_PRIO,        
                (TaskHandle_t*  )&UserIfTask_Handler);  
   //����PWM����
    xTaskCreate((TaskFunction_t )motor_task,            
                (const char*    )"motor_task",           
                (uint16_t       )MOTOR_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MOTOR_TASK_PRIO,        
                (TaskHandle_t*  )&MotorTask_Handler);  
    //����gui����
    xTaskCreate((TaskFunction_t )emwin_task,             
                (const char*    )"emwin_task",           
                (uint16_t       )EMWIN_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EMWIN_TASK_PRIO,        
                (TaskHandle_t*  )&EmwinTask_Handler);   	

		//����led ����
    xTaskCreate((TaskFunction_t )led_task,             
                (const char*    )"led_task",           
                (uint16_t       )LED_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )LED_TASK_PRIO,        
                (TaskHandle_t*  )&LedTask_Handler); 

		//����key ����		
		xTaskCreate((TaskFunction_t )key_task,             
                (const char*    )"key_task",           
                (uint16_t       )KEY_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )KEY_TASK_PRIO,        
                (TaskHandle_t*  )&KeyTask_Handler);	
		//������λ����
		xTaskCreate((TaskFunction_t )limit_task,             
                (const char*    )"limit_task",           
                (uint16_t       )LIMIT_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )LIMIT_TASK_PRIO,        
                (TaskHandle_t*  )&Limit_Handler);	
								
		//����MQTTconnect����
		xTaskCreate((TaskFunction_t )mqttconnect_task,             
                (const char*    )"mqttconnect_task",           
                (uint16_t       )MqttConnect_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTTCONNECT_TASK_PRIO,        
                (TaskHandle_t*  )&MqttConnect_Handler);	
														
		//����MQTTrec����
		xTaskCreate((TaskFunction_t )mqttrec_task,             
                (const char*    )"mqttrec_task",           
                (uint16_t       )MqttRec_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTTREC_TASK_PRIO,        
                (TaskHandle_t*  )&MqttRec_Handler);	
																										

			 //����MQTT PINGREQ����			
		   xTaskCreate((TaskFunction_t )MQTT_Pingreq_task,             
                (const char*    )"MQTT_PINGREQ_Task",           
                (uint16_t       )MQTT_PINGREQ_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_PINGREQ_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_PINGREQ_task_Handler);
				
				//����MQTT SUBSCRIBE����			
		   xTaskCreate((TaskFunction_t )MQTT_Subscribe_task,             
                (const char*    )"MQTT_SUBSCRIBE_Task",           
                (uint16_t       )MQTT_SUBSCRIBE_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_SUBSCRIBE_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_SUBSCRIBE_task_Handler);
												
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

  

