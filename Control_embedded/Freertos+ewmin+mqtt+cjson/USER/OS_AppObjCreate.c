#include <OS_AppObjCreate.h>

WIFIMSG WIFI_MSG;
BedCtrl1_t BedCtrl1_Msg;

/***********************************************************
						 ������Ϣ����
************************************************************/


#define KEYMSG_Q_NUM    1  		//������Ϣ���е�����  
#define BedControlKEY1_Q_NUM   1     //��������Ϣ����
#define BedLimit1_Q_NUM 1 //������λ��Ϣ��������
#define WIFIMESSAGE_buffer_Q_NUM   1   	//wifi�������ݵ���Ϣ���е����� 
#define PINGREQ_Q_NUM 1 //ping ��Ϣ��������

//������Ϣ����
void Key_QueueCreat(void)
{
	extern QueueHandle_t Key_Queue;
	Key_Queue =xQueueCreate(KEYMSG_Q_NUM,sizeof(uint8_t));
	
	if (Key_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("������Ϣ���д����ɹ�");
	}
}


void BedControlKEY1_QueueCreat(void)
{
	extern QueueHandle_t BedControlKEY1_Queue;
	BedControlKEY1_Queue = xQueueCreate(BedControlKEY1_Q_NUM,sizeof(BedCtrl1_Msg ));
	if (BedControlKEY1_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
		
	printf("���1������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("���1������Ϣ���д����ɹ�");
	}
}

void Bedlimit1_QueueCreat(void)
{
	extern QueueHandle_t Bedlimit1_Queue;
	Bedlimit1_Queue =xQueueCreate(PINGREQ_Q_NUM,sizeof(uint8_t));
	
	if (Bedlimit1_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("Bedlimit1��Ϣ���д���ʧ��");
	}
	else 
	{
	printf("Bedlimit1��Ϣ���д����ɹ�");
	}
}

void Bedspeed1_QueueCreat(void)
{
	extern QueueHandle_t Bedspeed1_Queue;
	Bedspeed1_Queue =xQueueCreate(PINGREQ_Q_NUM,sizeof(float));
	
	if (Bedspeed1_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("Bedspeed1_Queue��Ϣ���д���ʧ��");
	}
	else 
	{
	printf("Bedspeed1_Queue��Ϣ���д����ɹ�");
		
	}
}

void WIFI_buffer_QueueCreat(void)
{
	extern QueueHandle_t Wifi_buffer_Queue;
	Wifi_buffer_Queue =xQueueCreate(WIFIMESSAGE_buffer_Q_NUM,sizeof(struct WIFI_MSG *));
	
	if (Wifi_buffer_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("wifi������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("wifi������Ϣ���д����ɹ�");
	}
}

void PINGREQ_QueueCreat(void)
{
	extern QueueHandle_t PINGREQ_Queue;
	PINGREQ_Queue =xQueueCreate(PINGREQ_Q_NUM,sizeof(uint8_t));
	
	if (PINGREQ_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("PINGREQ��Ϣ���д���ʧ��");
	}
	else 
	{
	printf("PINGREQ��Ϣ���д����ɹ�");
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
						 �ź�������
************************************************************/

extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2�����ж϶�ֵ�ź������ (wifi)
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET���Ķ�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE���Ķ�ֵ�ź������

extern SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//���1�������ı��Ķ�ֵ�ź������
void BinarySemaphoreCreate(void)
{
	BinarySemaphore_USART2ISR=xSemaphoreCreateBinary();	
	BinarySemaphore_MQTTconnect=xSemaphoreCreateBinary();
	BinarySemaphore_MQTTsubscribe=xSemaphoreCreateBinary();
	BinarySemaphore_Motor1ValueChange=xSemaphoreCreateBinary();
};




void OS_AppObjCreate(void)
{
//��Ϣ���д���
QueueCreate();
BinarySemaphoreCreate();

	
	
}

 
 
 
 
 
 
 
 
 
 
 
 







