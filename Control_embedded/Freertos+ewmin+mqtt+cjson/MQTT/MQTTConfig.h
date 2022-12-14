/******************MQTT配设**************/
#ifndef  __MQTT_CONFIG_H__
#define  __MQTT_CONFIG_H__


//MQTT发送缓冲区最大长度 (与DMA缓冲区长度匹配)
#define len_max  256



/****************************
CONNECT 配设 代码版本ID  = "MQTC"
						 代码版本    = 0
***************************/
#define MQTT_CONNCET_ID    "MQTC"
#define MQTT_CONNCET_version  0						



#define MQTT_Version  4										 //MQTT协议版本4=3.1.1，3=3.1
#define MQTT_clientID   "dMQTTClientBed1"   //客户端ID
#define keepAlive_Interval  60             //保持时间(s)，在这个这个时间内要与服务器通讯或者发送心跳包，否则断开连接，


/***cleansession******/
#define cleansession_FLAG   1     //删除会话标志

/*********************
Will 配设 代码版本ID = "MQTW"
					代码版本   =  0
**********************/
#define MQTT_WILL_ID        "MQTW"
#define MQTT_WILL_version    0

#define Will_Flag            1		 //遗嘱启用标志
#define will_qos						 0     //QOS等级 qos1=1,qos2=2 
#define will_retained				 0		 //保存启用标志
#define will_topicName      "lwt"		//遗嘱主题：设立遗嘱标志后必须有
#define will_message				"dMQTTClientBed1 died" //遗嘱消息：设立遗嘱标志后必须有
#define MQTT_username   		"zhuchen"								//用户名：若为空则无用户名
#define MQTT_password				"test"					//密码：若为空则无密码


/********************
Subscribe 订阅报文配设
主题过滤器与qos等级必须成对出现
报文标识符必须>0
*********************/
#define subscribe_header_dup  0  //固定头部首字节控制报文标志dup位 subscribe报文固定为0
#define subscribe_count       2 //报文过滤器计数

/*******订阅报文1*******/
#define subscribe_packetid   1   // 报文标识符
#define subscrible1_topicFilters1   "HealthyBed/Motor1"  //主题1
#define subscribe1_qos1        0   //主题1订阅等级


#define subscrible1_topicFilters2	 "HealthyBed/State"  //主题2
#define subscribe1_qos2				0		 //主题2订阅等级




//主题通配查找
#define topic1_compare_monolayer      "HealthyBed/+"    
#define topic1_compare_Universal      "HealthyBed/#"






/********************
Publish 报文配设

*********************/

/********发布报文1*******/
#define publish1_header_dup  0  //固定头部首字节控制报文标志dup位 
#define publish1_qos       0   //publish qos等级 
#define publish1_retained   0  //是否保留消息
#define publish1_packetid   2  //报文标识符 message id 
#define publish1_topicname   "STM32F103ZET6/temperature"  //主题名

#endif


