
/***********************************
**	FileName:
**	Time:
***********************************/

#include "include.h"

static unsigned char UpLoadBuf[512];
static unsigned char DataBig;
/*************************************
**	��������:
**	��������:
*************************************/

void ProcessTcpIpUpLoad(void)
{
	static unsigned char step = 0;
	
	unsigned short int len = 0;
	unsigned char  tmp;
	switch(step)
	{
		case 0:                 //ȡ�����ݣ�   
			if(ReadSendState() != 2)
				break;
			len = ReadCmdDataBuf(UpLoadBuf);     //������ �ͷ�������
			if(len > 0)
			{
				SL_Print(">App Start Send Cmd .......%d\r\n",len);
				if(AppSendTcpIpSocket(UpLoadBuf,len) == 1)
				{
					step++;           //�������� 
					DataBig = 0;
					break;
				}
			}
			len = ReadSendQueue(UpLoadBuf);    //������������ 
			if(len > 0)
			{
				SL_Print(">App Start Send UpLoad .......%d\r\n",len);
				if(AppSendTcpIpSocket(UpLoadBuf,len) == 1)
				{
					step++;           		  //�������� 
					DataBig = 1;			  //
					break;
				}
			}
			break;
		case 1:
			tmp = ReadSendState();    		//��⵽����״̬
			if(tmp == 2)
			{
				step++;
				break;
			}
			else if(tmp == 3 || tmp == 0)
			{
				step = 0;
				break;
			}
			break;
		case 2:               			//�������������
			if(DataBig == 0)
			{
				ClearCmdData();
			}
			else
			{
				ClearSendQueue();
				SL_Print(">App Clear SendQueue............\r\n");
			}
			step = 0;
			break;
		default:
			break;
	
	}
}

/********************File End**********************/


