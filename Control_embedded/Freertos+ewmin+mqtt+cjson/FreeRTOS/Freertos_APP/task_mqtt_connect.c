#include "task_mqtt_connect.h"


extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET���Ķ�ֵ�ź������

/******MQTTconncet�ṹ��************/
MQTTPacket_connectData SETCONNECT;



//MQTT���ķ�������,�ɹ����Ӻ�����,ֱ�����������쳣
void mqttconnect_task(void *p_arg)
{
u8 mqtt_connect_buff1[len_max];
BaseType_t err=pdFALSE;
int len;
	/*********����20S����MQTT_CONNECT����*********/
vTaskDelay(20000);
conncect_init(); //MQTTconncet ����������ò�����ʼ��
	
		while(1)
		{
		err=xSemaphoreTake(BinarySemaphore_MQTTconnect,portMAX_DELAY);	//��ȡCONNECT�����ź���
			if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{		
			printf("WIFI���ڷ���CONNECT����\r\n");
				
			len=MQTT_Connect(mqtt_connect_buff1,len_max,&SETCONNECT);
			WIFI_send(mqtt_connect_buff1,len);


//					/*******��ӡwifi����*******/
//		
//		for (i=0;i<len;i++)
//		{
//		printf("%02x",temp_buff[i]);
//	
//		}
	
			
		printf("CONNECT���ķ������\r\n");
	

			}
			

			
			
			
			vTaskDelay(500);
		}
		
		
}

