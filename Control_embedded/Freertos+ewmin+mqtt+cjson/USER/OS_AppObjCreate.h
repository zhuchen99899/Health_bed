#ifndef __OS_APPOBJCREATE_H
#define __OS_APPOBJCREATE_H
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
//wifi��Ϣ���д��ݽṹ��
typedef struct WIFIMSG
{
u32 length;
u8  buffer[512];

} WIFIMSG;
	

//��������Ϣ���нṹ��
typedef struct BedControl1
{
u8 Positive;
u8 Negative;
	
}BedCtrl1_t;






////������Ϣ����
//void Key_QueueCreat(void);
//void BedControlKEY1_QueueCreate(void);

////��Ϣ���д���
//void QueueCreate(void);


////�ź�������
//void BinarySemaphoreCreate(void);

////����ͨѶ���ƴ���
void OS_AppObjCreate(void);

#endif 







