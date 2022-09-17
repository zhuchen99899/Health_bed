/******************MQTT����**************/
#ifndef  __MQTT_CONFIG_H__
#define  __MQTT_CONFIG_H__


//MQTT���ͻ�������󳤶� (��DMA����������ƥ��)
#define len_max  256



/****************************
CONNECT ���� ����汾ID  = "MQTC"
						 ����汾    = 0
***************************/
#define MQTT_CONNCET_ID    "MQTC"
#define MQTT_CONNCET_version  0						



#define MQTT_Version  4										 //MQTTЭ��汾4=3.1.1��3=3.1
#define MQTT_clientID   "dMQTTClientBed1"   //�ͻ���ID
#define keepAlive_Interval  60             //����ʱ��(s)����������ʱ����Ҫ�������ͨѶ���߷���������������Ͽ����ӣ�


/***cleansession******/
#define cleansession_FLAG   1     //ɾ���Ự��־

/*********************
Will ���� ����汾ID = "MQTW"
					����汾   =  0
**********************/
#define MQTT_WILL_ID        "MQTW"
#define MQTT_WILL_version    0

#define Will_Flag            1		 //�������ñ�־
#define will_qos						 0     //QOS�ȼ� qos1=1,qos2=2 
#define will_retained				 0		 //�������ñ�־
#define will_topicName      "lwt"		//�������⣺����������־�������
#define will_message				"dMQTTClientBed1 died" //������Ϣ������������־�������
#define MQTT_username   		"zhuchen"								//�û�������Ϊ�������û���
#define MQTT_password				"test"					//���룺��Ϊ����������


/********************
Subscribe ���ı�������
�����������qos�ȼ�����ɶԳ���
���ı�ʶ������>0
*********************/
#define subscribe_header_dup  0  //�̶�ͷ�����ֽڿ��Ʊ��ı�־dupλ subscribe���Ĺ̶�Ϊ0
#define subscribe_count       2 //���Ĺ���������

/*******���ı���1*******/
#define subscribe_packetid   1   // ���ı�ʶ��
#define subscrible1_topicFilters1   "HealthyBed/Motor1"  //����1
#define subscribe1_qos1        0   //����1���ĵȼ�


#define subscrible1_topicFilters2	 "HealthyBed/State"  //����2
#define subscribe1_qos2				0		 //����2���ĵȼ�




//����ͨ�����
#define topic1_compare_monolayer      "HealthyBed/+"    
#define topic1_compare_Universal      "HealthyBed/#"






/********************
Publish ��������

*********************/

/********��������1*******/
#define publish1_header_dup  0  //�̶�ͷ�����ֽڿ��Ʊ��ı�־dupλ 
#define publish1_qos       0   //publish qos�ȼ� 
#define publish1_retained   0  //�Ƿ�����Ϣ
#define publish1_packetid   2  //���ı�ʶ�� message id 
#define publish1_topicname   "STM32F103ZET6/temperature"  //������

#endif

