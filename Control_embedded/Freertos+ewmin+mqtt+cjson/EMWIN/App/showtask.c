#include <showtask.h>


/************************ʵ����Ŀ��ֹʹ�ô�ӡ������Ϣ��Ӱ��ʵʱ�ԣ�************/
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;



/************************����Freertos����״̬����****************/
char* return_state(char* state ,int value)
{

switch(value)
{
	case 0: 
	strcpy(state,"Running");/* A task is querying the state of itself, so must be running. */
	break;
	case 1: 
	strcpy(state,"Ready");/* The task being queried is in a read or pending ready list. */
	break;
	case 2: 
	strcpy(state,"Blocked");/* The task being queried is in the Blocked state. */
	case 3:
	strcpy(state,"Suspended");/* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
	break;
	case 4:
	strcpy(state,"Deleted");/* The task being queried has been deleted, but its TCB has not yet been freed. */
	break;
	case 5:
	strcpy(state,"Invalid");/* Used as an 'invalid state' value. */
	break;
	}
	
	return state;
}

/****************************��������ʹ����**********************/
char* return_usingrate(unsigned long Percentage,char* buff )
{
	if( Percentage > 0UL )
					{
					
						
							sprintf( buff, "%lu%%", Percentage );
						

					}
					else
					{
							sprintf( buff, "<1%%");
					}

return buff;
			
}


extern TaskHandle_t MQTT_Publish_task_Handler;
/*********************************************************************************************************

										showtaskAPP����

**********************************************************************************************************/
#define ID_WINDOW_0        (GUI_ID_USER + 0x00)
#define ID_LISTVIEW_0        (GUI_ID_USER + 0x01)
#define ID_BUTTON_0   			(GUI_ID_USER + 0x02)
static const GUI_WIDGET_CREATE_INFO _taskFrameCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 0, 130, 800, 350, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button1", ID_BUTTON_0,700, 0, 100, 100, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/


/*********************************************************************
�����Զ���
**********************************************************************/
/*
*********************************************************************************************************
*	�� �� ��: _cbButtonBack
*	����˵��: ��ť�ص�����
*	��    ��: pMsg  ��Ϣָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbButtonBack(WM_MESSAGE * pMsg) 
{
	WM_HWIN  hWin;
	hWin  = pMsg->hWin;

	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			if (BUTTON_IsPressed(hWin)) 
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle(100,0,72);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&bmReturn, 40, 5);
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(100, 0, 72);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  
				GUI_DrawBitmap(&bmReturn, 40, 5);				
			}
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
	
}

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _showtaskcallback(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	WM_HTIMER hTimer;
	WM_HWIN hWin = pMsg->hWin;
	/*********����״̬��ȡ����********/
	TaskHandle_t TaskHandle;	
	TaskStatus_t TaskStatus;
	uint32_t TotalRunTime;
	UBaseType_t ArraySize;
	TaskStatus_t *StatusArray; 
	char number[2];
	char state[10];
	char CurrentPriority[2];
	char BasePriority[2];
	char Stack[10];
	char count[5];
	uint32_t percentage;
	char usingrate[8];
	char StrEmwinHeap[5];

	
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Listview'
    //

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
		BUTTON_SetText(hItem, "BACK");
		WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonBack);
	
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
		LISTVIEW_AddColumn(hItem, 88, "ID", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 88, "Name", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 88, "Status", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 88, "CurrentPriority", GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hItem, 88, "BasePriority", GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hItem, 88, "Remaining stack", GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hItem, 88, "Count", GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hItem, 88, "Using rate", GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hItem, 88, "Heap", GUI_TA_HCENTER | GUI_TA_VCENTER);
	
    LISTVIEW_AddRow(hItem, NULL);
		LISTVIEW_AddRow(hItem, NULL);
    LISTVIEW_SetGridVis(hItem, 1);
		hTimer= WM_CreateTimer(WM_GetClientWindow(hWin),1, 2000,0);//������ʱ��
		(void)hTimer;//��ֹ����
		
    break;
	
	case WM_TIMER:	
		WM_RestartTimer(pMsg->Data.v,2000); //���ڶ�ʱ����װ��
	/*********************************************************************************
	��ȡ������Ϣ
	�����������ӡ�ظ����������޷���ȡ�����ڵ����г��ֶ���
	��˵������û�д����ɹ�����Ҫ���ϵͳ�ܶѴ�С����freertosConfig.h�мӴ�ϵͳ�Ѵ�С
	**********************************************************************************/
	/********��ȡϵͳ����ʱ��*********/
	
	ArraySize=uxTaskGetNumberOfTasks();
	StatusArray=pvPortMalloc(ArraySize*sizeof(TaskStatus_t));//�����ڴ�
	ArraySize=uxTaskGetSystemState((TaskStatus_t* 	)StatusArray, 	//������Ϣ�洢����
	(UBaseType_t		)ArraySize, 	//������Ϣ�洢�����С
	(uint32_t*		)&TotalRunTime);//��ȡϵͳ����ʱ��  ->���ڽ�������������ʹ����
	
	vPortFree(StatusArray);	//�ͷ��ڴ�
	/* For percentage calculations. */
	TotalRunTime /= 100UL;
	

	/****************LED_task*******************/
	TaskHandle=xTaskGetHandle("led_task");	//�����������ƻ�ȡ���
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//������
				 (TaskStatus_t*	)&TaskStatus, 		//������Ϣ�ṹ
				 (BaseType_t	)pdTRUE,			//����ͳ�������ջ��ʷ��Сʣ����
			   (eTaskState	)eInvalid);			//����״̬
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//����һ������������
		LISTVIEW_AddRow(hItem, NULL);//���LISTVIEW��ʼ����û��������Ҫ��ʾ���У����������к�LISTVIEW_SetItemText����������Ч
		 
		sprintf(number,"%lu",TaskStatus.xTaskNumber);
		return_state(state,TaskStatus.eCurrentState);
	  sprintf(CurrentPriority,"%lu",TaskStatus.uxCurrentPriority);
		sprintf(BasePriority,"%lu",TaskStatus.uxBasePriority);
		sprintf(Stack,"%u",TaskStatus.usStackHighWaterMark);
		sprintf(count,"%d",TaskStatus.ulRunTimeCounter);
		
		percentage=TaskStatus.ulRunTimeCounter/TotalRunTime;
		return_usingrate(percentage,usingrate);
		
		LISTVIEW_SetItemText(hItem,0,0,number); 
		LISTVIEW_SetItemText(hItem,1,0,TaskStatus.pcTaskName);
	  LISTVIEW_SetItemText(hItem,2,0,state); 
		LISTVIEW_SetItemText(hItem,3,0,CurrentPriority); 
		LISTVIEW_SetItemText(hItem,4,0,BasePriority); 
		LISTVIEW_SetItemText(hItem,5,0,Stack); 
		LISTVIEW_SetItemText(hItem,6,0,count);
		LISTVIEW_SetItemText(hItem,7,0,usingrate);
	
		/****************emwindemo_task*******************/
	TaskHandle=xTaskGetHandle("emwin_task");	//�����������ƻ�ȡ���
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//������
				 (TaskStatus_t*	)&TaskStatus, 		//������Ϣ�ṹ
				 (BaseType_t	)pdTRUE,			//����ͳ�������ջ��ʷ��Сʣ����
			   (eTaskState	)eInvalid);			//����״̬
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//������һ������������
		LISTVIEW_AddRow(hItem, NULL);//���LISTVIEW��ʼ����û��������Ҫ��ʾ���У����������к�LISTVIEW_SetItemText����������Ч
		 
		sprintf(number,"%lu",TaskStatus.xTaskNumber);
		return_state(state,TaskStatus.eCurrentState);
	  sprintf(CurrentPriority,"%lu",TaskStatus.uxCurrentPriority);
		sprintf(BasePriority,"%lu",TaskStatus.uxBasePriority);
		sprintf(Stack,"%u",TaskStatus.usStackHighWaterMark);
		sprintf(count,"%d",TaskStatus.ulRunTimeCounter);
		
		percentage=TaskStatus.ulRunTimeCounter/TotalRunTime;
		return_usingrate(percentage,usingrate);
		sprintf(StrEmwinHeap,"%d",xPortGetFreeHeapSize());
		
		LISTVIEW_SetItemText(hItem,0,1,number); 
		LISTVIEW_SetItemText(hItem,1,1,TaskStatus.pcTaskName);
	  LISTVIEW_SetItemText(hItem,2,1,state); 
		LISTVIEW_SetItemText(hItem,3,1,CurrentPriority); 
		LISTVIEW_SetItemText(hItem,4,1,BasePriority); 
		LISTVIEW_SetItemText(hItem,5,1,Stack); 
		LISTVIEW_SetItemText(hItem,6,1,count);
		LISTVIEW_SetItemText(hItem,7,1,usingrate);
		LISTVIEW_SetItemText(hItem,8,1,StrEmwinHeap);
		LISTVIEW_SetItemBkColor(hItem,8,1,LISTVIEW_CI_UNSEL,GUI_LIGHTBLUE);
		if(xPortGetFreeHeapSize()<=0)
		{
		LISTVIEW_SetItemBkColor(hItem,8,1,LISTVIEW_CI_UNSEL,GUI_LIGHTRED);
		}
				
				/****************touch_task*******************/
	TaskHandle=xTaskGetHandle("touch_task");	//�����������ƻ�ȡ���
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//������
				 (TaskStatus_t*	)&TaskStatus, 		//������Ϣ�ṹ
				 (BaseType_t	)pdTRUE,			//����ͳ�������ջ��ʷ��Сʣ����
			   (eTaskState	)eInvalid);			//����״̬
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//������һ������������
		LISTVIEW_AddRow(hItem, NULL);//���LISTVIEW��ʼ����û��������Ҫ��ʾ���У����������к�LISTVIEW_SetItemText����������Ч
		 
		sprintf(number,"%lu",TaskStatus.xTaskNumber);
		return_state(state,TaskStatus.eCurrentState);
	  sprintf(CurrentPriority,"%lu",TaskStatus.uxCurrentPriority);
		sprintf(BasePriority,"%lu",TaskStatus.uxBasePriority);
		sprintf(Stack,"%u",TaskStatus.usStackHighWaterMark);
		sprintf(count,"%d",TaskStatus.ulRunTimeCounter);
		
		percentage=TaskStatus.ulRunTimeCounter/TotalRunTime;
		return_usingrate(percentage,usingrate);
		
		LISTVIEW_SetItemText(hItem,0,2,number); 
		LISTVIEW_SetItemText(hItem,1,2,TaskStatus.pcTaskName);
	  LISTVIEW_SetItemText(hItem,2,2,state); 
		LISTVIEW_SetItemText(hItem,3,2,CurrentPriority); 
		LISTVIEW_SetItemText(hItem,4,2,BasePriority); 
		LISTVIEW_SetItemText(hItem,5,2,Stack); 
		LISTVIEW_SetItemText(hItem,6,2,count);
		LISTVIEW_SetItemText(hItem,7,2,usingrate);
		
		
		/****************PWM_task*******************/
	TaskHandle=xTaskGetHandle("motor_task");	//�����������ƻ�ȡ���
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//������
				 (TaskStatus_t*	)&TaskStatus, 		//������Ϣ�ṹ
				 (BaseType_t	)pdTRUE,			//����ͳ�������ջ��ʷ��Сʣ����
			   (eTaskState	)eInvalid);			//����״̬
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//������һ������������
		LISTVIEW_AddRow(hItem, NULL);//���LISTVIEW��ʼ����û��������Ҫ��ʾ���У����������к�LISTVIEW_SetItemText����������Ч
		 
		sprintf(number,"%lu",TaskStatus.xTaskNumber);
		return_state(state,TaskStatus.eCurrentState);
	  sprintf(CurrentPriority,"%lu",TaskStatus.uxCurrentPriority);
		sprintf(BasePriority,"%lu",TaskStatus.uxBasePriority);
		sprintf(Stack,"%u",TaskStatus.usStackHighWaterMark);
		sprintf(count,"%d",TaskStatus.ulRunTimeCounter);
		
		percentage=TaskStatus.ulRunTimeCounter/TotalRunTime;
		return_usingrate(percentage,usingrate);
		
		LISTVIEW_SetItemText(hItem,0,3,number); 
		LISTVIEW_SetItemText(hItem,1,3,TaskStatus.pcTaskName);
	  LISTVIEW_SetItemText(hItem,2,3,state); 
		LISTVIEW_SetItemText(hItem,3,3,CurrentPriority); 
		LISTVIEW_SetItemText(hItem,4,3,BasePriority); 
		LISTVIEW_SetItemText(hItem,5,3,Stack); 
		LISTVIEW_SetItemText(hItem,6,3,count);
		LISTVIEW_SetItemText(hItem,7,3,usingrate);
		
		/****************ADC_task*******************/
	TaskHandle=xTaskGetHandle("key_task");	//�����������ƻ�ȡ���
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//������
				 (TaskStatus_t*	)&TaskStatus, 		//������Ϣ�ṹ
				 (BaseType_t	)pdTRUE,			//����ͳ�������ջ��ʷ��Сʣ����
			   (eTaskState	)eInvalid);			//����״̬
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//������һ������������
		LISTVIEW_AddRow(hItem, NULL);//���LISTVIEW��ʼ����û��������Ҫ��ʾ���У����������к�LISTVIEW_SetItemText����������Ч
		 
		sprintf(number,"%lu",TaskStatus.xTaskNumber);
		return_state(state,TaskStatus.eCurrentState);
	  sprintf(CurrentPriority,"%lu",TaskStatus.uxCurrentPriority);
		sprintf(BasePriority,"%lu",TaskStatus.uxBasePriority);
		sprintf(Stack,"%u",TaskStatus.usStackHighWaterMark);
		sprintf(count,"%d",TaskStatus.ulRunTimeCounter);
		
		percentage=TaskStatus.ulRunTimeCounter/TotalRunTime;
		return_usingrate(percentage,usingrate);
		
		LISTVIEW_SetItemText(hItem,0,4,number); 
		LISTVIEW_SetItemText(hItem,1,4,TaskStatus.pcTaskName);
	  LISTVIEW_SetItemText(hItem,2,4,state); 
		LISTVIEW_SetItemText(hItem,3,4,CurrentPriority); 
		LISTVIEW_SetItemText(hItem,4,4,BasePriority); 
		LISTVIEW_SetItemText(hItem,5,4,Stack); 
		LISTVIEW_SetItemText(hItem,6,4,count);
		LISTVIEW_SetItemText(hItem,7,4,usingrate);
		
		

		



				
				/****************UserIf*******************/
	TaskHandle=xTaskGetHandle("UserIf");	//�����������ƻ�ȡ���
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//������
				 (TaskStatus_t*	)&TaskStatus, 		//������Ϣ�ṹ
				 (BaseType_t	)pdTRUE,			//����ͳ�������ջ��ʷ��Сʣ����
			   (eTaskState	)eInvalid);			//����״̬
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//������һ������������
		LISTVIEW_AddRow(hItem, NULL);//���LISTVIEW��ʼ����û��������Ҫ��ʾ���У����������к�LISTVIEW_SetItemText����������Ч
		 
		sprintf(number,"%lu",TaskStatus.xTaskNumber);
		return_state(state,TaskStatus.eCurrentState);
	  sprintf(CurrentPriority,"%lu",TaskStatus.uxCurrentPriority);
		sprintf(BasePriority,"%lu",TaskStatus.uxBasePriority);
		sprintf(Stack,"%u",TaskStatus.usStackHighWaterMark);
		sprintf(count,"%d",TaskStatus.ulRunTimeCounter);
		
		percentage=TaskStatus.ulRunTimeCounter/TotalRunTime;
		return_usingrate(percentage,usingrate);
		
		LISTVIEW_SetItemText(hItem,0,5,number); 
		LISTVIEW_SetItemText(hItem,1,5,TaskStatus.pcTaskName);
	  LISTVIEW_SetItemText(hItem,2,5,state); 
		LISTVIEW_SetItemText(hItem,3,5,CurrentPriority); 
		LISTVIEW_SetItemText(hItem,4,5,BasePriority); 
		LISTVIEW_SetItemText(hItem,5,5,Stack); 
		LISTVIEW_SetItemText(hItem,6,5,count);
		LISTVIEW_SetItemText(hItem,7,5,usingrate);
		
		
						/****************limit*******************/
	TaskHandle=xTaskGetHandle("limit_task");	//�����������ƻ�ȡ���
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//������
				 (TaskStatus_t*	)&TaskStatus, 		//������Ϣ�ṹ
				 (BaseType_t	)pdTRUE,			//����ͳ�������ջ��ʷ��Сʣ����
			   (eTaskState	)eInvalid);			//����״̬
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//������һ������������
		LISTVIEW_AddRow(hItem, NULL);//���LISTVIEW��ʼ����û��������Ҫ��ʾ���У����������к�LISTVIEW_SetItemText����������Ч
		 
		sprintf(number,"%lu",TaskStatus.xTaskNumber);
		return_state(state,TaskStatus.eCurrentState);
	  sprintf(CurrentPriority,"%lu",TaskStatus.uxCurrentPriority);
		sprintf(BasePriority,"%lu",TaskStatus.uxBasePriority);
		sprintf(Stack,"%u",TaskStatus.usStackHighWaterMark);
		sprintf(count,"%d",TaskStatus.ulRunTimeCounter);
		
		percentage=TaskStatus.ulRunTimeCounter/TotalRunTime;
		return_usingrate(percentage,usingrate);
		
		LISTVIEW_SetItemText(hItem,0,6,number); 
		LISTVIEW_SetItemText(hItem,1,6,TaskStatus.pcTaskName);
	  LISTVIEW_SetItemText(hItem,2,6,state); 
		LISTVIEW_SetItemText(hItem,3,6,CurrentPriority); 
		LISTVIEW_SetItemText(hItem,4,6,BasePriority); 
		LISTVIEW_SetItemText(hItem,5,6,Stack); 
		LISTVIEW_SetItemText(hItem,6,6,count);
		LISTVIEW_SetItemText(hItem,7,6,usingrate);
	
		

		
		
		
		
		
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
	
    switch(Id) {
			
    case ID_LISTVIEW_0: // Notifications sent by 'Listview'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        break;
			    }
					break;
					
				
					
			case ID_BUTTON_0:
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
			break;
				
      case WM_NOTIFICATION_RELEASED:
			GUI_EndDialog(hWin, 0);
      break;
			
			
      }
			

      break;






  }//�ؼ�
		
	  default:
    WM_DefaultProc(pMsg);
    break;
	
 } //message id
} //_showtaskcallback

/*************************** End of file ****************************/


void App_showtask(WM_HWIN hWin)
{
	GUI_CreateDialogBox(_taskFrameCreate,
		GUI_COUNTOF(_taskFrameCreate),
		&_showtaskcallback,
		hWin,
		0,
		0);
}
