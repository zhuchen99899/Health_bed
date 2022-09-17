#include "task_mqtt_connect.h"


extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET报文二值信号量句柄

/******MQTTconncet结构体************/
MQTTPacket_connectData SETCONNECT;



//MQTT报文发送任务,成功连接后阻塞,直到网络连接异常
void mqttconnect_task(void *p_arg)
{
u8 mqtt_connect_buff1[len_max];
BaseType_t err=pdFALSE;
int len;
	/*********开机20S后发送MQTT_CONNECT报文*********/
vTaskDelay(20000);
conncect_init(); //MQTTconncet 报文组包配置参数初始化
	
		while(1)
		{
		err=xSemaphoreTake(BinarySemaphore_MQTTconnect,portMAX_DELAY);	//获取CONNECT报文信号量
			if(err==pdTRUE)										//获取信号量成功
			{		
			printf("WIFI正在发送CONNECT报文\r\n");
				
			len=MQTT_Connect(mqtt_connect_buff1,len_max,&SETCONNECT);
			WIFI_send(mqtt_connect_buff1,len);


//					/*******打印wifi发送*******/
//		
//		for (i=0;i<len;i++)
//		{
//		printf("%02x",temp_buff[i]);
//	
//		}
	
			
		printf("CONNECT报文发送完成\r\n");
	

			}
			

			
			
			
			vTaskDelay(500);
		}
		
		
}

