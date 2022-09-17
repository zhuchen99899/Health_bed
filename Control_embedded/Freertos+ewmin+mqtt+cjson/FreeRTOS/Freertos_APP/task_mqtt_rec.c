#include "task_mqtt_rec.h"



void mqttrec_task(void *p_arg)
{
	/**************************句柄**************************/
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//二值信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE报文二值信号量句柄
  extern SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//电机1参数更改报文二值信号量句柄
	
	extern QueueHandle_t Wifi_buffer_Queue; //wifi消息队列
	extern QueueHandle_t PINGREQ_Queue;
	
	extern QueueHandle_t BedControlKEY1_Queue; //电机1控制消息队列句柄
	extern QueueHandle_t Bedspeed1_Queue;
	
	/**********************SUBACK解包参数**********************/
	unsigned short DeserializeSuback_packetid;
	int DeserializeSuback_Maxcount=subscribe_count;
	int DeserializeSuback_Count;
	int grantedQoSs[subscribe_count];
	
	/*********************服务器PUBLISH解包参数****************/
	unsigned char DeserializePublish_dup;
	int DeserializePublish_qos;
	unsigned char DeserializePublish_retained;
	unsigned short DeserializePublish_packetid;
	MQTTString DeserializePublish_topicName;
	unsigned char* DeserializePublish_payload;
	int DeserializePublish_payloadlen;
	
	/*********************循环变量****************/
	int i;
	
	
	//申请动态内存指针 存储主题与负载
	char *topic1;
	char *payload1;
			
	//JSON变量
	/* 创建JSON结构体指针*/
	cJSON *json = NULL; //json
	cJSON *node = NULL; //子节点
	cJSON *nodeSpeed =NULL;//子节点与*node做区分
	char *json_data = NULL;		


float bedSpeed1=0;

	/*******************消息队列结构体****************/
			BedCtrl1_t *BedCtrlKey1;
			BedCtrl1_t  BedCtrl1_buff1;



		
	/*****************DMAwifi缓冲区参数**************************/
	WIFIMSG *wifireceive;
	
	
	/****************信号***************************************/
	BaseType_t err=pdFALSE;
	u8 PINGREQ_FLAG=0;
	

	/***************变量初始化****************************************/
	BedCtrlKey1=&BedCtrl1_buff1;//消息队列结构体初始化
	BedCtrlKey1->Negative=0;
	BedCtrlKey1->Positive=0;
	
	
	

	

		
	while(1)
	{
		
	err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//获取串口2空闲中断二值信号量
	if(err==pdTRUE)										//获取USART2接收空闲中断信号量成功
	{
	
			xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
			
				
		/************************************************
		CONNACK报文检测
		**************************************************/
			//	首字节固定为0x20  剩余字节固定为0x02 可变报头部分为 连接确认标志：0x00 以及连接返回码
		if((wifireceive->buffer[0]==0x20)&&(wifireceive->buffer[1]==0x02)&&(wifireceive->buffer[2]==0x00))    //如果收到4个字节，且第一个字节是0x20，表示是connect ack报文，进入if
		{
			printf("已接收服务器CONNACK报文\r\n");
			switch(wifireceive->buffer[3])	                                                     //判断收到的第4个字节
			{					
				case 0x00 : printf("CONNECT报文成功\r\n");
										PINGREQ_FLAG=1;   //开始发送PINGREQ报文标志
										xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);
										vTaskDelay(3000);
										xSemaphoreGive(BinarySemaphore_MQTTsubscribe);//开始订阅MQTT主题过滤器
										vTaskDelay(3000);
				
							break;
				case 0x01 : printf("连接已拒绝，不支持的协议版本，准备重启\r\n");
				/***************E1错误重启*********************/		
										
						break;
				case 0x02 : printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n");
				/***************E2错误重启*********************/	
								
						break;
				case 0x03 : printf("连接已拒绝，服务端不可用，准备重启\r\n");
				/***************E3错误重启*********************/	

						break;
				case 0x04 : printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");
				/***************E4错误重启*********************/	

						break;
				case 0x05 : printf("连接已拒绝，未授权，准备重启\r\n");
				/***************E5错误重启*********************/	

						break;
				default   : printf("连接已拒绝，未知状态，准备重启\r\n");
				/***************E6错误重启*********************/	

				    break;
		
			}	//CONNACK报文检测
		} //connack报文
		
		
		/************************************************
		PINGRESP报文检测
		**************************************************/
		/*************收到PINGRESP报文******************************/
		//首字节固定为0xD0 剩余长度固定为 0x00 ，无可变报头
		else if((wifireceive->buffer[0]==0xD0)&&(wifireceive->buffer[1]==0x00)) 
		{
		
		printf("已接收服务器PINGRESP报文\r\n");

			
			
			
		}//else if PINGRESP报文
				
		
		/************************************************
		SUBACK报文检测
		**************************************************/
		else if(wifireceive->buffer[0]==0x90) 
		{
				printf("已接收服务器SUBACK报文\r\n");
			
						
			MQTTDeserialize_suback(&DeserializeSuback_packetid,DeserializeSuback_Maxcount,&DeserializeSuback_Count,grantedQoSs,(unsigned char*)(wifireceive->buffer),
									(int)(wifireceive->length));
			
			printf("包id为:%d\r\n",DeserializeSuback_packetid);
			printf("反回的报文主题过滤器计数%d\r\n",DeserializeSuback_Count);
			for (i=0;i<subscribe_count;i++)
			{
				printf("主题过滤器%d订阅成功,qos等级为:%d",i+1,grantedQoSs[i]);
			}
			

			printf("Suback消息接收完成\r\n");
		
				
		}//else if SUBSCRIBE报文
		
		
		/*************************************************
		服务器PUBLISH报文检测
		**************************************************/

		else if((wifireceive->buffer[0]&0xF0)==0x30)
		{

		printf("已接收服务器PUBLISH报文\r\n");
			/******解包PUBLISH报文*****/
		MQTTDeserialize_publish(&DeserializePublish_dup,&DeserializePublish_qos,
									&DeserializePublish_retained,&DeserializePublish_packetid,
									&DeserializePublish_topicName,&DeserializePublish_payload,
									&DeserializePublish_payloadlen,(unsigned char*)(wifireceive->buffer),
									(int)(wifireceive->length));	

				/*************主题/负载判断****************/						
									
					printf("DUP:%d\r\n",DeserializePublish_dup);
					printf("qos:%d\r\n",DeserializePublish_qos);
					printf("retained:%d\r\n",DeserializePublish_retained);
		
							
									
									
//申请动态内存保存主题和负载							
topic1=pvPortMalloc(DeserializePublish_topicName.len);
payload1=pvPortMalloc(DeserializePublish_payloadlen);
memcpy(payload1,DeserializePublish_payload,DeserializePublish_payloadlen);
memcpy(topic1,DeserializePublish_topicName.string,DeserializePublish_topicName.len);
//清零字符串最后一位，防止乱码
payload1[DeserializePublish_payloadlen]=0;
topic1[DeserializePublish_topicName.len]=0;		

									
									
			/***********电机1主题*******************/
			if((strcmp(topic1,topic1_compare_Universal)==0)||(strcmp(topic1,topic1_compare_monolayer)==0)||(strcmp(topic1,subscrible1_topicFilters1)==0))
			{
				printf("主题为电机1主题\r\n");							
				printf("主题为%s\r\n",topic1);	
				printf("负载为\r\n");
				
				
								/****************有效载荷Cjson解包******************/
			
			
									/**********	完整输出一个JSON对象结构************/
								json = cJSON_Parse(payload1);                         //从数据缓冲区解析JASON对象结构
								json_data = cJSON_Print(json);                    //将传入的JASON对象转化为JASON字符串
								printf("data: %s\n", json_data);                  //输出JASON打印结构
								vPortFree(json_data);                                  //释放JASON打印结构

									/****************JASON树中键值对解析***************/
									//根据键名再JASON中查找子节点，查找出Positive对象
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
													//从Positive中获取结果
												printf("value is :%d\r\n",node->valueint);
														if((node->valueint)==1)
														{
														BedCtrlKey1->Positive=1;	
														printf("Positive设置为1");
														}
														else if((node->valueint)==0)
														{
														BedCtrlKey1->Positive=0;	
														printf("Positive设置为0");
														}
											}//查找Positive值																																
									}//识别JASON树中Positive对象
			
									
									//根据键名再JASON中查找子节点，查找出Negative对象
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
													//从Negative中获取结果

												printf("value is :%d\r\n",node->valueint);
														if((node->valueint)==1)
														{
														BedCtrlKey1->Negative=1;	
														printf("Negative设置为1");
														}
														else if((node->valueint)==0)
														{
														BedCtrlKey1->Negative=0;	
														printf("Negative设置为0");
														}

											}//查找Speed值
													
									}//识别JASON树中Speed对象
			
									
									//根据键名再JASON中查找子节点，查找出Speed对象
									nodeSpeed = cJSON_GetObjectItem(json, "Speed");
									if (nodeSpeed == NULL)
									{
									printf("Speed: can't find\r\n");
										
									}	
									else
									{
							
									printf("Speed: find \r\n");

													//从Speed中获取结果
												printf("Speed valueI is :%d\r\n",nodeSpeed->valueint);
												printf("Speed valueS is :%s\r\n",nodeSpeed->valuestring);
												printf("Speed valueD is :%lf\r\n",nodeSpeed->valuedouble);
	
												bedSpeed1=(float)nodeSpeed->valuedouble;	


													
										}//识别JASON树中Speed对象
									
									
			//发送消息队列							
										
			if(node!=NULL)			
			{
			
					xQueueOverwrite(BedControlKEY1_Queue,&BedCtrlKey1);
					xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//发送电机1参数更改报文信号
			}				
									
									
			if (nodeSpeed!=NULL)
			{
				
					xQueueOverwrite(Bedspeed1_Queue,&bedSpeed1);
					xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//发送电机1参数更改报文信号
							
			}
							

									
				
			}//电机1主题确认
			
	
			
			/******状态查询主题，用于给Client 查询床的状态******/
			else if((strcmp(topic1,topic1_compare_Universal)==0)||(strcmp(topic1,topic1_compare_monolayer)==0)||(strcmp(topic1,subscrible1_topicFilters2)==0))
			{
				printf("主题为状态查询\r\n");							
				printf("主题为%s\r\n",topic1);	
				printf("负载为\r\n");
				
				
								/****************有效载荷Cjson解包******************/
			
			
							/**********	完整输出一个JSON对象结构************/
								json = cJSON_Parse(payload1);                         //从数据缓冲区解析JASON对象结构
								json_data = cJSON_Print(json);                    //将传入的JASON对象转化为JASON字符串
								printf("data: %s\n", json_data);                  //输出JASON结构
								vPortFree(json_data);                                  //释放JASON结构


										


			
			
				
			}//状态查询主题确认
			
			
			else
			{
			printf("已订阅的未知主题，请查看设置\r\n");
			}
			//主题确认
		
			
			//释放JASON树
			cJSON_Delete(json);
			
			//释放解包所申请的空间 
	vPortFree(payload1);	
	vPortFree(topic1);	
			


			
		};//else if publish报文	

		
	}//串口空闲信号量
			
			
				vTaskDelay(30);                           //延时3000ms 高优先级任务需要做较长时间延时.
	}//while(1)

	
}




