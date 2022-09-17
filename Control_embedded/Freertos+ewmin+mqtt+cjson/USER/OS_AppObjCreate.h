#ifndef __OS_APPOBJCREATE_H
#define __OS_APPOBJCREATE_H
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
//wifi消息队列传递结构体
typedef struct WIFIMSG
{
u32 length;
u8  buffer[512];

} WIFIMSG;
	

//床控制消息队列结构体
typedef struct BedControl1
{
u8 Positive;
u8 Negative;
	
}BedCtrl1_t;






////按键消息队列
//void Key_QueueCreat(void);
//void BedControlKEY1_QueueCreate(void);

////消息队列创建
//void QueueCreate(void);


////信号量创建
//void BinarySemaphoreCreate(void);

////任务通讯机制创建
void OS_AppObjCreate(void);

#endif 







