
/*******************************************************************************
**	FileName:
**	Time:
********************************************************************************/

#include "include.h"

HANDLE SysAppEnterTask;           //�������ʶ



HANDLE g_SLApp1 = HNULL;             //
HANDLE g_SLApp2 = HNULL;			 //
HANDLE g_SLApp3 = HNULL;             //
HANDLE g_SLApp4 = HNULL;       		 //




#define APP_TASK1_STACK_SIZE    (2 * 2048)
#define APP_TASK1_PRIORITY      (SL_APP_TASK_PRIORITY_BEGIN + 1)

#define APP_TASK2_STACK_SIZE    (2 * 2048)
#define APP_TASK2_PRIORITY      (APP_TASK1_PRIORITY + 1)

#define APP_TASK3_STACK_SIZE    (2 * 2048)
#define APP_TASK3_PRIORITY      (APP_TASK2_PRIORITY + 1)

#define APP_TASK4_STACK_SIZE    (2 * 2048)
#define APP_TASK4_PRIORITY      (APP_TASK3_PRIORITY + 1)



/*************************************************
**	��������:
**	��������:
**************************************************/

void SL_AppSendMsg(HANDLE stTask, unsigned int ulMsgId, unsigned int ulParam)
{
    SL_EVENT 					stEvnet;
    SL_TASK 					hTask;
	
    SL_Memset(&stEvnet, 0, sizeof(SL_EVENT));
    stEvnet.nEventId = ulMsgId;
    stEvnet.nParam1 = ulParam;
    hTask.element[0] = stTask;
    SL_SendEvents(hTask, &stEvnet);
}



/****************************************************
**	��������:
** 	��������:
*****************************************************/
void SL_AppTask1(void *pData)
{
    SL_EVENT ev = {0};
    SL_TASK stSltask;

    SL_Print("******* SL_AppTask1 *********\n");
    SL_Memset(&ev, 0, sizeof(SL_EVENT));
    stSltask.element[0] = g_SLApp1;
    while(1)
    {
        SL_FreeMemory((VOID*)ev.nParam1);
        SL_GetEvent(stSltask, &ev);

        //SL_Print("SLAPP: SL_AppTask1 get event[%d]\n", ev.nEventId);
        switch (ev.nEventId)
        {
            case EVT_APP_GPRS_READY:
				TickAdd();      			//Ӧ�ó���ʱ��   ������   
            	break;
            case EVT_APP_GPRS_STARTRECV:
            	break;
            case EVT_APP_TCP_CLOSE:
            	break;
            default:
            	break;
        }
    }
}


/***********************************************************
**	��������:
**	��������:
***********************************************************/
void SL_AppTask2(void *pData)
{
    SL_EVENT ev = {0};
    SL_TASK stSltask;

    SL_Print("******* SL_AppTask2 *********\n");
    SL_Memset(&ev, 0, sizeof(SL_EVENT));
    stSltask.element[0] = g_SLApp2;
    while(1)
    {
        SL_FreeMemory((VOID*)ev.nParam1);
        SL_GetEvent(stSltask, &ev);
		SL_Print("SLPP: SL_AppTask2 get event[%d]\n", ev.nEventId);
        switch (ev.nEventId)
        {
            case EVT_APP_GPRS_READY:
            	break;
            case EVT_APP_GPRS_STARTRECV:
            	break;
            case EVT_APP_TCP_CLOSE:
            	break;
            case EVT_APP_AGPS:
            	break;
            case EVT_APP_DTMF:
				break;
            default:
            	break;
        }
    }
}


/**************************************************
**	��������:
**	��������:
**************************************************/
void SL_AppTask3(void *pData)
{
    SL_EVENT ev = {0};
    SL_TASK stSltask;

    SL_Print("******* SL_AppTask3 *********\n");

    SL_Memset(&ev, 0, sizeof(SL_EVENT));
    stSltask.element[0] = g_SLApp3;

    while(1)
    {
        SL_FreeMemory((VOID*)ev.nParam1);
        SL_GetEvent(stSltask, &ev);
		SL_Print("SLPP: SL_AppTask3 get event[%d]\n", ev.nEventId);
        switch (ev.nEventId)
        {
            case EVT_APP_CREATE_FILE:
            	break;
            case EVT_APP_READ_FILE:
            	break;
            case EVT_APP_PLAY_FILE:
            	break;
            case EVT_APP_PLAY_TTS:
            	break;
            default:
           	 	break;
        }
    }
}


/*********************************************
**	��������:
** 	��������:
**********************************************/

void SL_AppTask4(void *pData)
{
    SL_EVENT ev = {0};
    SL_TASK stSltask;

    SL_Print("******* SL_AppTask4 *********\n");

    SL_Memset(&ev, 0, sizeof(SL_EVENT));
    stSltask.element[0] = g_SLApp4;

    while(1)
    {
        SL_FreeMemory((VOID*)ev.nParam1);
        SL_GetEvent(stSltask, &ev);

        SL_Print("SLPP: SL_AppTask4 get event[%d]\n", ev.nEventId);
        switch (ev.nEventId)
        {
            case SL_APP_FTPOP_REQ:
            	break;
            case SL_APP_UPDATE_REQ:
            	break;
            default:
             	break;
        }
    }
}


/*********************************************************
**	��������:
**	��������:
*********************************************************/
void SL_AppCreateTask()
{
   g_SLApp1 = SL_CreateTask(SL_AppTask1, APP_TASK1_STACK_SIZE, APP_TASK1_PRIORITY, "SL_AppTask1");
   SL_Print("g_SLApp1=%u", g_SLApp1);

   g_SLApp2 = SL_CreateTask(SL_AppTask2, APP_TASK2_STACK_SIZE, APP_TASK2_PRIORITY, "SL_AppTask2");
   SL_Print("g_SLApp2=%u", g_SLApp2);

   g_SLApp3 = SL_CreateTask(SL_AppTask3, APP_TASK3_STACK_SIZE, APP_TASK3_PRIORITY, "SL_AppTask3");
   SL_Print("g_SLApp3=%u", g_SLApp3);

   g_SLApp4 = SL_CreateTask(SL_AppTask4, APP_TASK4_STACK_SIZE, APP_TASK4_PRIORITY, "SL_AppTask4");
   SL_Print("g_SLApp4=%u", g_SLApp4);
}


/******************************************************
**	��������:
**	��������:
******************************************************/

void ProcessTest(void)
{
	static unsigned char step = 0;
	static struct stopwatch16 sw;
	unsigned char tmp;
	switch(step)
	{
		case 0:
			tmp = SL_FileSysFormat(SL_FS_TYPE_FAT,SL_FS_FORMAT_NORMAL);
			if(tmp == SL_RET_OK)
				SL_Print("FileSysFormat OK...........\r\n");
			step++;
			break;
		case 1:
			init_stopwatch16(&sw);
			SL_AppSendMsg(g_SLApp1,EVT_APP_GPRS_READY,0);   //������1���� 
			break;
		case 2:
			if(read_stopwatch16_value(&sw) < 40)
				break;
			SL_AppSendMsg(g_SLApp2,EVT_APP_GPRS_READY,0);
			init_stopwatch16(&sw);
			step++;
			break;
		case 3:
			if(read_stopwatch16_value(&sw) < 40)
				break;
			SL_AppSendMsg(g_SLApp3,EVT_APP_GPRS_READY,0);
			init_stopwatch16(&sw);
			step++;
			break;
		case 4:
			if(read_stopwatch16_value(&sw) < 40)
				break;
			SL_AppSendMsg(g_SLApp4,EVT_APP_GPRS_READY,0);
			init_stopwatch16(&sw);
			step++;
			break;	
		case 5:
			if(read_stopwatch16_value(&sw) < 40)
				break;
			step = 1;
			break;
		default:
			break;
	}
}


/************************************************
**	��������:
**	��������:
************************************************/

void APP_ENTRY_START SL_Entry(void *pData)
{
	SL_EVENT	   		ev = {0};
	SL_TASK		   		stSltask;

	PSL_UART_DATA  		pUartData;     //���ڽ�������  

	unsigned int 		cache_data_len;
	unsigned char 		*ftp_data;
    unsigned int 		app_up_st = 0;
	
    SL_Memset(&ev, 0, sizeof(SL_EVENT));
	SL_Sleep(1000);									//
	
	SL_StopWatchDog();  							//ֹͣ���Ź�

    while(SL_CoreInitFinish() == FALSE)				//�ȴ���ʼ���ں�
    {
        SL_Print("SLAPP: core init");
        SL_Sleep(1000);								//
    }

	SysAppEnterTask = SL_GetAppTaskHandle();
    stSltask.element[0] = SL_GetAppTaskHandle();	//��ȡӦ�����������������ʶ
		
	SL_Sleep(5000);
	SL_GpioSetDir(SL_GPIO_3, SL_GPIO_OUT);			//��ʼ����IO��
	SL_GpioSetDir(SL_GPIO_4, SL_GPIO_OUT);			//��ʼ����IO��  ����GNSS��Դ
	SL_UartSetAppTaskHandle(SL_UART_1, SL_GetAppTaskHandle());
	UART1_Init();                                   //��ʼ������1
    SL_UartSetAppTaskHandle(SL_UART_2, SL_GetAppTaskHandle());
	UART2_Init();
	Init_Led();
	SL_GpioWrite(SL_GPIO_4, SL_PIN_HIGH);			//GPSLED_ON;  
	SL_Sleep(100);
	
	//SL_Print("The Firmware Version Num:V25\r\n");
	//SL_Print("The Firmware Info:20161221_2\r\n");
	
	SL_AppSendMsg(stSltask.element[0], EVT_APP_READY, 0);
	
	while(1)
    {
        SL_FreeMemory((VOID*)ev.nParam1);
        SL_GetEvent(stSltask, &ev);
		
        switch (ev.nEventId)
        {
        	case SL_APP_UPDATE_REQ:                 //��������
        		cache_data_len = ev.nParam2;
                SL_Print("---> FTP get file len:%d\n", cache_data_len);
                if (cache_data_len < APP_CODE_MAXLEN) //app max 310K byte
                {
                    SL_Print("---> FTP get data in ram\n");
                    ftp_data = (unsigned char *)ev.nParam1;
                    app_up_st = SL_AppUpdateInit(ftp_data, cache_data_len);
                    SL_Print("---> SL APP update init ret:%d\n", app_up_st);

                    if (SL_APP_UPDATE_OK != app_up_st)
                    {
                        SL_Print("---> SL App update condition check fail, do not update!\n");
                    }
                    else
                    {
                    	SaveSendQueue();                //�������ݷ��Ͷ�������
                        SL_Print("---> SL App update condition check ok, reset system and update ... \n");
						SL_Sleep(10000);				//�ȴ�10��
						SL_HstSendEvt(0x20161034);      //������� 
                        SL_PowerDown(0); 				//����
                    }
                }
                else
                {
                    SL_Print("---> FTP get file size overflow:%d, do not download!\n", cache_data_len);

                }
				break;
           	case EVT_APP_GPRS_RECV:                 //����GPRS���������¼�  
           		 
		   		break;
			case EVT_APP_READY:                    				//Ӧ�ó���׼������¼�   
				
				InitSysTimer();                     			//��ʼ��ϵͳ��ʱ��  
				
				SL_Print("App Ready...........OK\r\n");  		//���������Ϣ   
				
				SL_Sleep(5000);	
				SL_AppCreateTask();
				break;
            case SL_EV_TIMER:                     	//��ʱ��ʱ�䵽�¼�
				if(ev.nParam1 == 3)				  	//һ�붨ʱ��
                {
				}
				
				if(ev.nParam1 == 4)
				{
					SL_AppSendMsg(g_SLApp1,EVT_APP_GPRS_READY,0);   //������1����
					ProcessGsmLed();
					ProcessTest();
				}
				
            	break;
			case SL_EV_UART_RECEIVE_DATA_IND:         //���ڽ����¼�
				if(ev.nParam2 == 1)
				{	
					pUartData = (PSL_UART_DATA)ev.nParam1;
					UART1_Recv_Data(pUartData->aucDataBuf,pUartData->ulDataLen);   //�������� ����1
				}
				else
				{
					pUartData = (PSL_UART_DATA)ev.nParam1;
					UART2_Recv_Data(pUartData->aucDataBuf,pUartData->ulDataLen);    //�������ݴ���2
				}
				break;
            default:
            	break;
        }
    }
}


