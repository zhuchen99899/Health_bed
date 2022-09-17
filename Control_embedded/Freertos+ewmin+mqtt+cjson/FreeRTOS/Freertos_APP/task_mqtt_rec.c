#include "task_mqtt_rec.h"



void mqttrec_task(void *p_arg)
{
	/**************************���**************************/
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//��ֵ�ź������
	extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE���Ķ�ֵ�ź������
  extern SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//���1�������ı��Ķ�ֵ�ź������
	
	extern QueueHandle_t Wifi_buffer_Queue; //wifi��Ϣ����
	extern QueueHandle_t PINGREQ_Queue;
	
	extern QueueHandle_t BedControlKEY1_Queue; //���1������Ϣ���о��
	extern QueueHandle_t Bedspeed1_Queue;
	
	/**********************SUBACK�������**********************/
	unsigned short DeserializeSuback_packetid;
	int DeserializeSuback_Maxcount=subscribe_count;
	int DeserializeSuback_Count;
	int grantedQoSs[subscribe_count];
	
	/*********************������PUBLISH�������****************/
	unsigned char DeserializePublish_dup;
	int DeserializePublish_qos;
	unsigned char DeserializePublish_retained;
	unsigned short DeserializePublish_packetid;
	MQTTString DeserializePublish_topicName;
	unsigned char* DeserializePublish_payload;
	int DeserializePublish_payloadlen;
	
	/*********************ѭ������****************/
	int i;
	
	
	//���붯̬�ڴ�ָ�� �洢�����븺��
	char *topic1;
	char *payload1;
			
	//JSON����
	/* ����JSON�ṹ��ָ��*/
	cJSON *json = NULL; //json
	cJSON *node = NULL; //�ӽڵ�
	cJSON *nodeSpeed =NULL;//�ӽڵ���*node������
	char *json_data = NULL;		


float bedSpeed1=0;

	/*******************��Ϣ���нṹ��****************/
			BedCtrl1_t *BedCtrlKey1;
			BedCtrl1_t  BedCtrl1_buff1;



		
	/*****************DMAwifi����������**************************/
	WIFIMSG *wifireceive;
	
	
	/****************�ź�***************************************/
	BaseType_t err=pdFALSE;
	u8 PINGREQ_FLAG=0;
	

	/***************������ʼ��****************************************/
	BedCtrlKey1=&BedCtrl1_buff1;//��Ϣ���нṹ���ʼ��
	BedCtrlKey1->Negative=0;
	BedCtrlKey1->Positive=0;
	
	
	

	

		
	while(1)
	{
		
	err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//��ȡ����2�����ж϶�ֵ�ź���
	if(err==pdTRUE)										//��ȡUSART2���տ����ж��ź����ɹ�
	{
	
			xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
			
				
		/************************************************
		CONNACK���ļ��
		**************************************************/
			//	���ֽڹ̶�Ϊ0x20  ʣ���ֽڹ̶�Ϊ0x02 �ɱ䱨ͷ����Ϊ ����ȷ�ϱ�־��0x00 �Լ����ӷ�����
		if((wifireceive->buffer[0]==0x20)&&(wifireceive->buffer[1]==0x02)&&(wifireceive->buffer[2]==0x00))    //����յ�4���ֽڣ��ҵ�һ���ֽ���0x20����ʾ��connect ack���ģ�����if
		{
			printf("�ѽ��շ�����CONNACK����\r\n");
			switch(wifireceive->buffer[3])	                                                     //�ж��յ��ĵ�4���ֽ�
			{					
				case 0x00 : printf("CONNECT���ĳɹ�\r\n");
										PINGREQ_FLAG=1;   //��ʼ����PINGREQ���ı�־
										xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);
										vTaskDelay(3000);
										xSemaphoreGive(BinarySemaphore_MQTTsubscribe);//��ʼ����MQTT���������
										vTaskDelay(3000);
				
							break;
				case 0x01 : printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");
				/***************E1��������*********************/		
										
						break;
				case 0x02 : printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n");
				/***************E2��������*********************/	
								
						break;
				case 0x03 : printf("�����Ѿܾ�������˲����ã�׼������\r\n");
				/***************E3��������*********************/	

						break;
				case 0x04 : printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");
				/***************E4��������*********************/	

						break;
				case 0x05 : printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");
				/***************E5��������*********************/	

						break;
				default   : printf("�����Ѿܾ���δ֪״̬��׼������\r\n");
				/***************E6��������*********************/	

				    break;
		
			}	//CONNACK���ļ��
		} //connack����
		
		
		/************************************************
		PINGRESP���ļ��
		**************************************************/
		/*************�յ�PINGRESP����******************************/
		//���ֽڹ̶�Ϊ0xD0 ʣ�೤�ȹ̶�Ϊ 0x00 ���޿ɱ䱨ͷ
		else if((wifireceive->buffer[0]==0xD0)&&(wifireceive->buffer[1]==0x00)) 
		{
		
		printf("�ѽ��շ�����PINGRESP����\r\n");

			
			
			
		}//else if PINGRESP����
				
		
		/************************************************
		SUBACK���ļ��
		**************************************************/
		else if(wifireceive->buffer[0]==0x90) 
		{
				printf("�ѽ��շ�����SUBACK����\r\n");
			
						
			MQTTDeserialize_suback(&DeserializeSuback_packetid,DeserializeSuback_Maxcount,&DeserializeSuback_Count,grantedQoSs,(unsigned char*)(wifireceive->buffer),
									(int)(wifireceive->length));
			
			printf("��idΪ:%d\r\n",DeserializeSuback_packetid);
			printf("���صı����������������%d\r\n",DeserializeSuback_Count);
			for (i=0;i<subscribe_count;i++)
			{
				printf("���������%d���ĳɹ�,qos�ȼ�Ϊ:%d",i+1,grantedQoSs[i]);
			}
			

			printf("Suback��Ϣ�������\r\n");
		
				
		}//else if SUBSCRIBE����
		
		
		/*************************************************
		������PUBLISH���ļ��
		**************************************************/

		else if((wifireceive->buffer[0]&0xF0)==0x30)
		{

		printf("�ѽ��շ�����PUBLISH����\r\n");
			/******���PUBLISH����*****/
		MQTTDeserialize_publish(&DeserializePublish_dup,&DeserializePublish_qos,
									&DeserializePublish_retained,&DeserializePublish_packetid,
									&DeserializePublish_topicName,&DeserializePublish_payload,
									&DeserializePublish_payloadlen,(unsigned char*)(wifireceive->buffer),
									(int)(wifireceive->length));	

				/*************����/�����ж�****************/						
									
					printf("DUP:%d\r\n",DeserializePublish_dup);
					printf("qos:%d\r\n",DeserializePublish_qos);
					printf("retained:%d\r\n",DeserializePublish_retained);
		
							
									
									
//���붯̬�ڴ汣������͸���							
topic1=pvPortMalloc(DeserializePublish_topicName.len);
payload1=pvPortMalloc(DeserializePublish_payloadlen);
memcpy(payload1,DeserializePublish_payload,DeserializePublish_payloadlen);
memcpy(topic1,DeserializePublish_topicName.string,DeserializePublish_topicName.len);
//�����ַ������һλ����ֹ����
payload1[DeserializePublish_payloadlen]=0;
topic1[DeserializePublish_topicName.len]=0;		

									
									
			/***********���1����*******************/
			if((strcmp(topic1,topic1_compare_Universal)==0)||(strcmp(topic1,topic1_compare_monolayer)==0)||(strcmp(topic1,subscrible1_topicFilters1)==0))
			{
				printf("����Ϊ���1����\r\n");							
				printf("����Ϊ%s\r\n",topic1);	
				printf("����Ϊ\r\n");
				
				
								/****************��Ч�غ�Cjson���******************/
			
			
									/**********	�������һ��JSON����ṹ************/
								json = cJSON_Parse(payload1);                         //�����ݻ���������JASON����ṹ
								json_data = cJSON_Print(json);                    //�������JASON����ת��ΪJASON�ַ���
								printf("data: %s\n", json_data);                  //���JASON��ӡ�ṹ
								vPortFree(json_data);                                  //�ͷ�JASON��ӡ�ṹ

									/****************JASON���м�ֵ�Խ���***************/
									//���ݼ�����JASON�в����ӽڵ㣬���ҳ�Positive����
									node = cJSON_GetObjectItem(json, "Positive");
									if (node == NULL)
									{
									printf("Positive: can't find\r\n");
										
									}	
									else
									{
						
									printf("Positive: find \r\n");
										
										 if( node->type == cJSON_Number )
											{
													//��Positive�л�ȡ���
												printf("value is :%d\r\n",node->valueint);
														if((node->valueint)==1)
														{
														BedCtrlKey1->Positive=1;	
														printf("Positive����Ϊ1");
														}
														else if((node->valueint)==0)
														{
														BedCtrlKey1->Positive=0;	
														printf("Positive����Ϊ0");
														}
											}//����Positiveֵ																																
									}//ʶ��JASON����Positive����
			
									
									//���ݼ�����JASON�в����ӽڵ㣬���ҳ�Negative����
									node = cJSON_GetObjectItem(json, "Negative");
									if (node == NULL)
									{
									printf("Negative: can't find\r\n");
										
									}	
									else
									{
								
									printf("Negative: find \r\n");
										 if( node->type == cJSON_Number )

											{
													//��Negative�л�ȡ���

												printf("value is :%d\r\n",node->valueint);
														if((node->valueint)==1)
														{
														BedCtrlKey1->Negative=1;	
														printf("Negative����Ϊ1");
														}
														else if((node->valueint)==0)
														{
														BedCtrlKey1->Negative=0;	
														printf("Negative����Ϊ0");
														}

											}//����Speedֵ
													
									}//ʶ��JASON����Speed����
			
									
									//���ݼ�����JASON�в����ӽڵ㣬���ҳ�Speed����
									nodeSpeed = cJSON_GetObjectItem(json, "Speed");
									if (nodeSpeed == NULL)
									{
									printf("Speed: can't find\r\n");
										
									}	
									else
									{
							
									printf("Speed: find \r\n");

													//��Speed�л�ȡ���
												printf("Speed valueI is :%d\r\n",nodeSpeed->valueint);
												printf("Speed valueS is :%s\r\n",nodeSpeed->valuestring);
												printf("Speed valueD is :%lf\r\n",nodeSpeed->valuedouble);
	
												bedSpeed1=(float)nodeSpeed->valuedouble;	


													
										}//ʶ��JASON����Speed����
									
									
			//������Ϣ����							
										
			if(node!=NULL)			
			{
			
					xQueueOverwrite(BedControlKEY1_Queue,&BedCtrlKey1);
					xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//���͵��1�������ı����ź�
			}				
									
									
			if (nodeSpeed!=NULL)
			{
				
					xQueueOverwrite(Bedspeed1_Queue,&bedSpeed1);
					xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//���͵��1�������ı����ź�
							
			}
							

									
				
			}//���1����ȷ��
			
	
			
			/******״̬��ѯ���⣬���ڸ�Client ��ѯ����״̬******/
			else if((strcmp(topic1,topic1_compare_Universal)==0)||(strcmp(topic1,topic1_compare_monolayer)==0)||(strcmp(topic1,subscrible1_topicFilters2)==0))
			{
				printf("����Ϊ״̬��ѯ\r\n");							
				printf("����Ϊ%s\r\n",topic1);	
				printf("����Ϊ\r\n");
				
				
								/****************��Ч�غ�Cjson���******************/
			
			
							/**********	�������һ��JSON����ṹ************/
								json = cJSON_Parse(payload1);                         //�����ݻ���������JASON����ṹ
								json_data = cJSON_Print(json);                    //�������JASON����ת��ΪJASON�ַ���
								printf("data: %s\n", json_data);                  //���JASON�ṹ
								vPortFree(json_data);                                  //�ͷ�JASON�ṹ


										


			
			
				
			}//״̬��ѯ����ȷ��
			
			
			else
			{
			printf("�Ѷ��ĵ�δ֪���⣬��鿴����\r\n");
			}
			//����ȷ��
		
			
			//�ͷ�JASON��
			cJSON_Delete(json);
			
			//�ͷŽ��������Ŀռ� 
	vPortFree(payload1);	
	vPortFree(topic1);	
			


			
		};//else if publish����	

		
	}//���ڿ����ź���
			
			
				vTaskDelay(30);                           //��ʱ3000ms �����ȼ�������Ҫ���ϳ�ʱ����ʱ.
	}//while(1)

	
}




