#ifndef __INCLUDES_H
#define __INCLUDES_H	 





/***********************************
bsp 驱动
************************************/
#include "led.h"
#include "timer.h"
#include "ILI93xx.h"
#include "key.h"
#include "timer.h"
#include "sram.h"
#include "24cxx.h"
#include "touch.h"
#include "malloc.h"
#include "w25qxx.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "limits.h"
#include "dio.h" 
#include "pwm.h"
#include "wifi.h"
/***********************************
os应用
************************************/

#include "task_emwin.h"
#include "task_usrif.h"
#include "task_led.h"
#include "task_key.h"
#include "task_motor.h"
#include "task_limit.h"
#include "task_mqtt_connect.h"
#include "task_mqtt_rec.h"
#include "task_MQTT_Pingreq.h"


//任务间通讯机制
#include <OS_AppObjCreate.h>






/***********************************
功能库
************************************/



/***********系统功能************/
#include "sys.h"
#include "delay.h"
#include "usart.h"


/***********GUI相关***********/
#include "GUI.h"
#include "WM.h"

/***********MQTT库************/
#include "MQTTConfig.h"
#include "MQTT_init.h"
#include "MQTTconnect.h"




/**********CJSON库************/
#include "cJSON.h"







#endif




