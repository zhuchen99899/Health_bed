#include "MotorCtrl.h"





//消息队列句柄
extern QueueHandle_t BedControlKEY1_Queue;
extern QueueHandle_t Bedspeed1_Queue;
extern SemaphoreHandle_t BinarySemaphore_Motor1ValueChange;//电机1参数更改报文二值信号量句柄
 
//图片数据全局变量
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;
extern GUI_CONST_STORAGE GUI_BITMAP PNGLEDbutton_close;
extern GUI_CONST_STORAGE GUI_BITMAP PNGLEDbutton_open;

/*********************************************************************************************************

										ButtonController应用程序

**********************************************************************************************************/
#define ID_WINDOW_0   (GUI_ID_USER + 1)
#define ID_BUTTON_0   (GUI_ID_USER + 2)
#define ID_BUTTON_1   (GUI_ID_USER + 3)
#define ID_BUTTON_2   (GUI_ID_USER + 4)
#define ID_TEXT_0     (GUI_ID_USER + 5)
#define ID_TEXT_1     (GUI_ID_USER + 6)
#define ID_TEXT_2     (GUI_ID_USER + 7)
#define ID_TEXT_3     (GUI_ID_USER + 8)
#define ID_TEXT_4     (GUI_ID_USER + 9)
#define ID_SLIDER_0 	(GUI_ID_USER + 10)
/****************全局变量******************/
static u8 showkey1=0;
static u8 showkey2=0;
/*ButtonController对话框*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateMotorCtrl[] = {
{ WINDOW_CreateIndirect, "", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x64, 0 },
{ BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 700, 0, 100, 100, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 174, 97, 200, 60, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "Button", ID_BUTTON_2, 574, 97, 200, 60, 0, 0x0, 0 },
{ TEXT_CreateIndirect, "Positive", ID_TEXT_0, 	195, 160, 60, 60, 0, 0x0, 0 },
{ TEXT_CreateIndirect, "Negative", ID_TEXT_1, 	595, 160, 60, 60, 0, 0x0, 0 },
{ TEXT_CreateIndirect, "Speed", 	 ID_TEXT_2, 	400, 350, 60, 60, 0, 0x0, 0 },
{ TEXT_CreateIndirect, ""		, 		 ID_TEXT_3, 	380, 240, 60, 60, 0, 0x0, 0 },
{ TEXT_CreateIndirect, "%"	, 	 	 ID_TEXT_4, 	430, 240, 60, 60, 0, 0x0, 0 },
{ SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 300, 300, 241, 41, 0, 0x0, 0 },
};


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
				GUI_AA_FillCircle(100, 0, 72);
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


/*
*********************************************************************************************************
*	函 数 名: __cbLEDBUTTON1
*	功能说明: Motor按钮回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbLEDBUTTON1(WM_MESSAGE * pMsg) 
{

	switch (pMsg->MsgId) 
	{		
		case WM_PAINT:
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&PNGLEDbutton_close, 0, 0);
		

				if(showkey1==1)
				{
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&PNGLEDbutton_open, 0, 0);
				}
				else if(showkey1==0)
				{
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&PNGLEDbutton_close, 0, 0);

				}

			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}
/*
*********************************************************************************************************
*	函 数 名: __cbLEDBUTTON2
*	功能说明: Motor按钮回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbLEDBUTTON2(WM_MESSAGE * pMsg) 
{

	switch (pMsg->MsgId) 
	{		
		case WM_PAINT:
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&PNGLEDbutton_close, 0, 0);
		
		
				if(showkey2==1)
				{
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&PNGLEDbutton_open, 0, 0);
				}
				else if(showkey2==0)
				{
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&PNGLEDbutton_close, 0, 0);
				}
		
			
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
	
	
}




/*ButtonController回调函数*/
static void _cbCallbackMotorCtrl(WM_MESSAGE * pMsg) {
	
BedCtrl1_t *BedCtrlKey1;
BedCtrl1_t  BedCtrl1_buff1;

	WM_HWIN hItem;
	WM_HTIMER hTimer;
	WM_HWIN hWin = pMsg->hWin;
	int     NCode;
	int     Id;
	int SLIDER_value;
	char buf[20];
	
	float bedspeed1=0;
	// USER START (Optionally insert additional variables)
	// USER END
	BedCtrlKey1=&BedCtrl1_buff1;//消息队列结构体初始化
	BedCtrlKey1->Negative=0;
	BedCtrlKey1->Positive=0;


xQueuePeek(BedControlKEY1_Queue,&BedCtrlKey1,10);
xQueuePeek(Bedspeed1_Queue,&bedspeed1,10);
	
	//在程序开始获取消息队列，使接下来所有程序获得消息队列种的值
	
	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:

		//
		// Initialization of 'Button'
		//


if((BedCtrlKey1->Negative==1&&BedCtrlKey1->Positive==0))
	showkey1=1;
else showkey1=0;

if((BedCtrlKey1->Negative==0&&BedCtrlKey1->Positive==1))
		showkey2=1;
else showkey2=0;
	
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
		BUTTON_SetText(hItem, "BACK");
		WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonBack);

	
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
		BUTTON_SetText(hItem, "Positive");
		WM_SetCallback(hItem, _cbLEDBUTTON1);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
		BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
		BUTTON_SetText(hItem, "Negative");
		WM_SetCallback(hItem, _cbLEDBUTTON2);
		hTimer= WM_CreateTimer(WM_GetClientWindow(hWin),1, 1500,0);//创建定时器

TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_0),GUI_DARKMAGENTA);
TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_1),GUI_DARKMAGENTA);

hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0); 
SLIDER_SetNumTicks(hItem, 10);


(void)hTimer;//防止警告

	break;


	case WM_TIMER:
			
	WM_RestartTimer(pMsg->Data.v,1500); //窗口定时器重装填	
xQueuePeek(BedControlKEY1_Queue,&BedCtrlKey1,10);

/*设置按钮*/	
if((BedCtrlKey1->Negative==1&&BedCtrlKey1->Positive==0))
showkey1=1;
else showkey1=0;

if((BedCtrlKey1->Negative==0&&BedCtrlKey1->Positive==1))
		showkey2=1;
else showkey2=0;

/*设置滑块*/
SLIDER_SetValue(WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0),bedspeed1/2.5);

	WM_InvalidateWindow(hWin);

		break;

	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id) {
	
			case ID_BUTTON_0: // Notifications sent by 'Button'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)

				GUI_EndDialog(hWin, 0);
				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
			
		case ID_BUTTON_1: // Notifications sent by 'Button'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)

				
BedCtrlKey1->Negative=1;
BedCtrlKey1->Positive=0;
xQueueOverwrite(BedControlKEY1_Queue,&BedCtrlKey1);
xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//发送电机1参数更改报文信号
			
if((BedCtrlKey1->Negative==1&&BedCtrlKey1->Positive==0))
showkey1=1;
else showkey1=0;

if((BedCtrlKey1->Negative==0&&BedCtrlKey1->Positive==1))
		showkey2=1;
else showkey2=0;
WM_InvalidateWindow(hWin);

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;

			
			case ID_BUTTON_2: // Notifications sent by 'Button'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				
			// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

BedCtrlKey1->Negative=0;
BedCtrlKey1->Positive=1;
xQueueOverwrite(BedControlKEY1_Queue,&BedCtrlKey1);
xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//发送电机1参数更改报文信号			
			
if((BedCtrlKey1->Negative==1&&BedCtrlKey1->Positive==0))
	showkey1=1;
else showkey1=0;

if((BedCtrlKey1->Negative==0&&BedCtrlKey1->Positive==1))
		showkey2=1;
else showkey2=0;
	WM_InvalidateWindow(hWin);
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			
			break;
			
			
	case ID_SLIDER_0: // Notifications sent by 'Slider'
	switch(NCode) {
	case WM_NOTIFICATION_CLICKED:

	
	break;
	case WM_NOTIFICATION_RELEASED:

	
	break;
 
 case WM_NOTIFICATION_VALUE_CHANGED:
hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
SLIDER_value = SLIDER_GetValue(hItem);
 /*发送消息队列*/
 bedspeed1=SLIDER_value*2.5;

 xQueueOverwrite(Bedspeed1_Queue,&bedspeed1);
 xSemaphoreGive(BinarySemaphore_Motor1ValueChange);//发送电机1参数更改报文信号
 
sprintf(buf, "%d", SLIDER_value);
hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
TEXT_SetText(hItem, buf); //--------------(7)

 
		break;
		}
 
	break;//ID_SLIDER_0 

	
		}//switch (Id)
	
		break;//WM_NOTIFY_PARENT

		default:
			WM_DefaultProc(pMsg);
			break;
		
	}//MsgId
}//_cbCallbackMotorCtrl
/*App_MotorCtrl应用创建*/
void App_MotorCtrl(WM_HWIN hWin)
{
	GUI_CreateDialogBox(_aDialogCreateMotorCtrl,
		GUI_COUNTOF(_aDialogCreateMotorCtrl),
		&_cbCallbackMotorCtrl,
		hWin,
		0,
		0);
}





