#include "task_emwin.h"


//EMWINDEMO����
void emwin_task(void *pvParameters)
{

	MainTask(); 

}

//���������������
void touch_task(void *pvParameters)
{
	while(1)
	{
		GUI_TOUCH_Exec();	
		vTaskDelay(5);		//��ʱ5ms
	}
}





