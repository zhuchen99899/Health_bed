#include <showtask.h>


/************************实际项目禁止使用打印任务信息（影响实时性）************/
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;



/************************返回Freertos任务状态函数****************/
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

/****************************返回任务使用率**********************/
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

										showtaskAPP程序

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
按键自定义
**********************************************************************/
/*
*********************************************************************************************************
*	函 数 名: _cbButtonBack
*	功能说明: 按钮回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
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
	/*********任务状态获取参数********/
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
		hTimer= WM_CreateTimer(WM_GetClientWindow(hWin),1, 2000,0);//创建定时器
		(void)hTimer;//防止警告
		
    break;
	
	case WM_TIMER:	
		WM_RestartTimer(pMsg->Data.v,2000); //窗口定时器重装填
	/*********************************************************************************
	获取任务信息
	若出现任务打印重复或任务句柄无法获取，串口调试中出现断言
	则说明任务没有创建成功，需要检查系统总堆大小，在freertosConfig.h中加大系统堆大小
	**********************************************************************************/
	/********获取系统运行时间*********/
	
	ArraySize=uxTaskGetNumberOfTasks();
	StatusArray=pvPortMalloc(ArraySize*sizeof(TaskStatus_t));//申请内存
	ArraySize=uxTaskGetSystemState((TaskStatus_t* 	)StatusArray, 	//任务信息存储数组
	(UBaseType_t		)ArraySize, 	//任务信息存储数组大小
	(uint32_t*		)&TotalRunTime);//获取系统运行时间  ->用于接下来计算任务使用率
	
	vPortFree(StatusArray);	//释放内存
	/* For percentage calculations. */
	TotalRunTime /= 100UL;
	

	/****************LED_task*******************/
	TaskHandle=xTaskGetHandle("led_task");	//根据任务名称获取句柄
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//任务句柄
				 (TaskStatus_t*	)&TaskStatus, 		//任务信息结构
				 (BaseType_t	)pdTRUE,			//允许统计任务堆栈历史最小剩余量
			   (eTaskState	)eInvalid);			//运行状态
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//往下一行中添加数据
		LISTVIEW_AddRow(hItem, NULL);//如果LISTVIEW初始化中没有添加需要显示的行，必须添加行后，LISTVIEW_SetItemText函数才能有效
		 
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
	TaskHandle=xTaskGetHandle("emwin_task");	//根据任务名称获取句柄
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//任务句柄
				 (TaskStatus_t*	)&TaskStatus, 		//任务信息结构
				 (BaseType_t	)pdTRUE,			//允许统计任务堆栈历史最小剩余量
			   (eTaskState	)eInvalid);			//运行状态
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//往第下一行中添加数据
		LISTVIEW_AddRow(hItem, NULL);//如果LISTVIEW初始化中没有添加需要显示的行，必须添加行后，LISTVIEW_SetItemText函数才能有效
		 
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
	TaskHandle=xTaskGetHandle("touch_task");	//根据任务名称获取句柄
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//任务句柄
				 (TaskStatus_t*	)&TaskStatus, 		//任务信息结构
				 (BaseType_t	)pdTRUE,			//允许统计任务堆栈历史最小剩余量
			   (eTaskState	)eInvalid);			//运行状态
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//往第下一行中添加数据
		LISTVIEW_AddRow(hItem, NULL);//如果LISTVIEW初始化中没有添加需要显示的行，必须添加行后，LISTVIEW_SetItemText函数才能有效
		 
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
	TaskHandle=xTaskGetHandle("motor_task");	//根据任务名称获取句柄
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//任务句柄
				 (TaskStatus_t*	)&TaskStatus, 		//任务信息结构
				 (BaseType_t	)pdTRUE,			//允许统计任务堆栈历史最小剩余量
			   (eTaskState	)eInvalid);			//运行状态
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//往第下一行中添加数据
		LISTVIEW_AddRow(hItem, NULL);//如果LISTVIEW初始化中没有添加需要显示的行，必须添加行后，LISTVIEW_SetItemText函数才能有效
		 
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
	TaskHandle=xTaskGetHandle("key_task");	//根据任务名称获取句柄
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//任务句柄
				 (TaskStatus_t*	)&TaskStatus, 		//任务信息结构
				 (BaseType_t	)pdTRUE,			//允许统计任务堆栈历史最小剩余量
			   (eTaskState	)eInvalid);			//运行状态
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//往第下一行中添加数据
		LISTVIEW_AddRow(hItem, NULL);//如果LISTVIEW初始化中没有添加需要显示的行，必须添加行后，LISTVIEW_SetItemText函数才能有效
		 
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
	TaskHandle=xTaskGetHandle("UserIf");	//根据任务名称获取句柄
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//任务句柄
				 (TaskStatus_t*	)&TaskStatus, 		//任务信息结构
				 (BaseType_t	)pdTRUE,			//允许统计任务堆栈历史最小剩余量
			   (eTaskState	)eInvalid);			//运行状态
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//往第下一行中添加数据
		LISTVIEW_AddRow(hItem, NULL);//如果LISTVIEW初始化中没有添加需要显示的行，必须添加行后，LISTVIEW_SetItemText函数才能有效
		 
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
	TaskHandle=xTaskGetHandle("limit_task");	//根据任务名称获取句柄
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//任务句柄
				 (TaskStatus_t*	)&TaskStatus, 		//任务信息结构
				 (BaseType_t	)pdTRUE,			//允许统计任务堆栈历史最小剩余量
			   (eTaskState	)eInvalid);			//运行状态
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	//往第下一行中添加数据
		LISTVIEW_AddRow(hItem, NULL);//如果LISTVIEW初始化中没有添加需要显示的行，必须添加行后，LISTVIEW_SetItemText函数才能有效
		 
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






  }//控件
		
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

