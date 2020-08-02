
/**************************************************

***************************************************/

#include "include.h"

//#define BUF_NUM

static SendQueueStr 		SendQueue;               //���Ͷ��л�����

static FiFoStr				CmdData;                 //����ش�������


/***************************************************
**	��������:
**	��������:
***************************************************/

unsigned short int ReadSendQueue(unsigned char *Source)
{
	int i;
	unsigned char tmp;
	
	if(SendQueue.QNum == 0)
		return 0;
	
	SL_Print("Builid Num:%d \r\n",SendQueue.QNum);

	if(SendQueue.QData[SendQueue.QRead].DataBig > 0)     //˵����ä������
	{
		SL_Print("Ready Blind Data\r\n");
		tmp = CalcCrc8(SendQueue.QData[SendQueue.QRead].DataBuf,SendQueue.QData[SendQueue.QRead].DataLen);
		if(tmp != SendQueue.QData[SendQueue.QRead].DataCrc)
		{
			ClearSendQueue();
			return 0;
		}
		for(i = 0; i < SendQueue.QData[SendQueue.QRead].DataLen;i++)
		{
			*(Source + i) = SendQueue.QData[SendQueue.QRead].DataBuf[i];
		}
		SL_Print("Return Blind Data\r\n");
		return SendQueue.QData[SendQueue.QRead].DataLen;
			
	}
	else
	{
		if(SendQueue.QWrite >= SendQueue.QRead)
		{
			if(SendQueue.QNum != (SendQueue.QWrite - SendQueue.QRead))
			{
				SendQueue.QWrite = 0;
				SendQueue.QRead = 0;
				SendQueue.QNum = 0;
				return 0;
			}
		}

		if(SendQueue.QWrite < SendQueue.QRead)
		{
			if(SendQueue.QNum != (SendQueue.QWrite + BLIND_NUM - SendQueue.QRead))
			{
				SendQueue.QWrite = 0;
				SendQueue.QRead = 0;
				SendQueue.QNum = 0;
				return 0;
			}
		}
			
		for(i = 0;i < SendQueue.QData[SendQueue.QRead].DataLen;i++)
		{
			*(Source + i) = SendQueue.QData[SendQueue.QRead].DataBuf[i];
		}
		
		return SendQueue.QData[SendQueue.QRead].DataLen;
	}
}


/***************************************************
**	��������:unsigned char WriteSendQueue(unsigned char *Buf,unsigned short int DataLen)
**	��������:д���ͻ�����
***************************************************/

unsigned char WriteSendQueue(unsigned char *Buf,unsigned short int DataLen)
{
	int i;

	if(DataLen > 500)    // �������жϳ���
		return 0;
	
	if(SendQueue.QWrite >= BLIND_NUM)
		SendQueue.QWrite = 0;
	
	for(i = 0;i < DataLen;i++)
	{
		SendQueue.QData[SendQueue.QWrite].DataBuf[i] = *(Buf + i);
	}
	SendQueue.QData[SendQueue.QWrite].DataLen = DataLen;
	SendQueue.QData[SendQueue.QWrite].DataBig = 0;
	if(SendQueue.QNum < BLIND_NUM)         //���ͻ����������ڵ���������
		SendQueue.QNum++;
	SL_Print(">Write Send Queue Finsh....%d----%d\r\n",SendQueue.QWrite,SendQueue.QNum);
	SendQueue.QWrite++;
	
	
	return 1;
}


/***************************************************
**	��������:
**	��������:
***************************************************/

void ClearSendQueue(void)
{
	SendQueue.QData[SendQueue.QRead].DataLen = 0;
	SendQueue.QRead++;
	
	if(SendQueue.QRead >= BLIND_NUM)
		SendQueue.QRead = 0;
	
	if(SendQueue.QNum != 0)
		SendQueue.QNum--;
	
}


/************************************************
**	��������:
**	��������:
************************************************/

void WriteCmdDataBuf(unsigned char *Buf,unsigned short int DataLen)
{
	if(DataLen > 500)
		return;
	SL_Memcpy(CmdData.DataBuf,Buf,DataLen);
	CmdData.DataLen = DataLen;
}


/************************************************
**	��������:
**	��������:
**	�������:
************************************************/

unsigned short int ReadCmdDataBuf(unsigned char *buf)
{
	if(CmdData.DataLen > 0)
	{
		SL_Memcpy(buf,CmdData.DataBuf,CmdData.DataLen);
		return CmdData.DataLen;
	}
	
	return 0;
}

/************************************************
**	��������:
**	��������:
************************************************/

void ClearCmdData(void)
{
	CmdData.DataLen = 0;
}

/***************************************************
**	��������:unsigned char SlaveSendQueue(void)
**	��������:������е��ļ�ϵͳ
***************************************************/

void SaveSendQueue(void)
{	
	int i;
	unsigned char tmp = 0;
	MsgHeadStr *p;
	
	if(SendQueue.QNum > 0)
	{
		for(i = 0;i < SendQueue.QNum;i++)              //ֵä����ʶ
		{
			p = (MsgHeadStr *)SendQueue.QData[SendQueue.QRead].DataBuf;
			p->blind_flag = 1;           //ä����ʶ
			p->msg_id = 0x46;            //ä����ϢID;
			SendQueue.QData[SendQueue.QRead].DataBig = 1;
			tmp = SendQueue.QData[SendQueue.QRead].DataLen;
			SendQueue.QData[SendQueue.QRead].DataBuf[tmp - 2] = BccVerify(SendQueue.QData[SendQueue.QRead].DataBuf + 3,tmp - 4);
			SendQueue.QData[SendQueue.QRead].DataCrc = CalcCrc8(SendQueue.QData[SendQueue.QRead].DataBuf, tmp);
			SendQueue.QRead++;
			if(SendQueue.QRead >= BLIND_NUM)
				SendQueue.QRead = 0;
		}
		
		if(FileCheckOrGreate((unsigned char *)"SendQueueFile", (unsigned char *)&SendQueue, sizeof(SendQueue),1) > 0)
			SL_Print(">App:Save SendQueue Data.......OK\r\n");
	}
	
}



/*************************************************
**	��������:
**	��������:װ������
*************************************************/

void LoadSendQueue(void)
{
	unsigned short int tmp1,tmp2,tmp3;
	
	SL_Print("Start Load SendQueue Data............... %d\r\n",sizeof(SendQueue));
	if(FileCheckOrGreate((unsigned char *)"SendQueueFile", (unsigned char *)&SendQueue, sizeof(SendQueue),0) > 0)
	{
		if(SendQueue.QNum > BLIND_NUM || SendQueue.QRead > BLIND_NUM || SendQueue.QWrite > BLIND_NUM)
		{
			SendQueue.QNum = 0;
			SendQueue.QRead = 0;
			SendQueue.QWrite = 0;
		}
		
		//SL_Print(">App:Load SendQueue Data.......OK(%d)\r\n",SendQueue.QNum);
		if(SendQueue.QNum > 0)
		{
			tmp1 = SendQueue.QNum;
			tmp2 = SendQueue.QRead;
			tmp3 = SendQueue.QWrite;
			
			SendQueue.QNum = 0;
			SendQueue.QRead = 0;
			SendQueue.QWrite = 0;

			if(FileCheckOrGreate((unsigned char *)"SendQueueFile", (unsigned char *)&SendQueue, sizeof(SendQueue),1) > 0)
				SL_Print(">App:Claer Blind SendQueue Data.......OK\r\n");

			SendQueue.QNum = tmp1;
			SendQueue.QRead = tmp2;
			SendQueue.QWrite = tmp3;
		}
		
	}

	//SendQueue.QRead = 2;         //��Ϊ����ʹ�� 
	//SendQueue.QNum = 10;
}



/*********************************************
**	��������:
**	��������:���ä������
**********************************************/

void ClearBindData(void)
{
	SendQueue.QNum = 0;
	SendQueue.QRead = 0;
	SendQueue.QWrite = 0;
	SL_Print("Claer Bind Data Finsh..............\r\n");
		
}


/*****************************File End************************/

