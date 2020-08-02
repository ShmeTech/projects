
/***************************************************************
**	FileName:
**	Time:
****************************************************************/


#include "include.h"

static unsigned char 		DataPacketBuf[512];

static SysRunDataStr 		SysRunData;

static DistanceProStr 		DistancePro;

static	unsigned char 		AccStateBack = 0;

/********************************************************
**	��������:
**	��������:
********************************************************/

SysRunDataStr *GetSysRunDataSpace(void)
{
	return &SysRunData;
}


/********************************************************
**	��������:
**	��������:�ж�ϵͳ��ҵ��־
*********************************************************/

unsigned char ReadFarmStatus(void)
{
	static unsigned short counter = 0;
	
	TerStateStr 		*p;
	SysConfigStr		*pCfg;
	CanWheatStr 		*pWheat;		//С���
	CanPaddyStr 		*pPaddy;		//ˮ����
	CanMaizeStr			*pMaize;		//���׻�
	GNSSSTR				*pGnss;         //GPS��λ��Ϣ
	
	pCfg 	= GetSysCfgSpace();
	p 		= GetTerStateDataSpace();
	pWheat 	= GetCanWheatSpace();
	pPaddy 	= GetCanPaddySpace();
	pMaize 	= GetCanMaizeSpace();
	//SL_Print("CarType:%d,%d\r\n",pCfg->CarType,pWheat->Rotate1);
	switch(pCfg->CarType)
	{
		case 0x03:      //������  ��������ҵ�ж�
			return 0;
		case 0x08:				//С���  ������Ͳת��  
			if(pWheat->Rotate1 > 10)    
			{
				if(counter < 5)             //
					counter++;
			}
			else
			{
				counter = 0;
			}
			
			if(counter > 3)
				return 1;
			return 0;
		case 0x09:				//ˮ����  ����ת��
			if(pPaddy->Rotate1 > 10)    
			{
				if(counter < 5)             //
					counter++;
			}
			else
			{
				counter = 0;
			}

			if(counter > 3)
				return 1;
			return 0;
		case 0x0A:				//���׻� ��Ƥ��ת��
			if(pMaize->Rotate1 > 10)    
			{
				if(counter < 200)             //
					counter++;
			}
			else
			{
				counter = 0;
			}

			if(counter > 120)
				return 1;
			return 0;
		case 0x0E:                  //С���  �Ϲ���
			pGnss = GetGnssDataSpace();
			if(pWheat->EngineRotate > 16000 && pGnss->speed > 1 && pGnss->speed  < 12)
			{
				if(counter < 240)
					counter++;
			}
			else
			{
				if(counter > 0)
					counter--;
			}
			
			if(counter > 120)
			{
				pWheat->WorkFlag = 1;
				
				return 1;
			}
			return 0;
		default:
			return 0;
	}
	
}


/*****************************************************
**	��������:
**	��������:
*****************************************************/

void SaveSysRunData(void)
{
	if(FileCheckOrGreate((unsigned char *)"SysRunDataFile", (unsigned char *)&SysRunData, sizeof(SysRunData),1) > 0)
		SL_Print(">App:Save Sys Run Data.......OK\r\n");
		
}


/*****************************************************
**	��������:
**	��������:
******************************************************/

void ReadSysRunData(void)
{
	if(FileCheckOrGreate((unsigned char *)"SysRunDataFile", (unsigned char *)&SysRunData, sizeof(SysRunData),0) > 0)
	{
		SL_Print(">App:Reads Run Data.......OK\r\n");
		if(SysRunData.TimeSec > 3155759999)
			SysRunData.TimeSec = 0;                //�����ʱ��ظ�Ϊ0
		SL_Print(">App RunData:%d",SysRunData.TimeSec);
	}
}

/**********************************************************
**	��������:
**	��������:��Ҫ������ϵͳ�߼�
***********************************************************/

void SysProcess(void)
{
	static unsigned char 		step = 0,step1 = 0;
	static unsigned int 		NextTime = 0,NextTime1 = 0;    //
	static struct stopwatch16   sw;
	unsigned char 				tmp = 0;
	unsigned short int 			PacketLen = 0;
	SysConfigStr 				*pSysCfg;

	SysTimeStr					*pTime;             //����ʹ��
	
	pSysCfg = GetSysCfgSpace();
	pTime = GetSysTimeSpace();
	tmp = ReadFarmStatus();              			//��ȡ����״̬

	if(pSysCfg->CfgFlag != 0x55)
		return;
	
	switch(step)
	{
		case 0:
			if(pTime->VerifyBit == 0)
				break;
			NextTime = pTime->TimeSec + pSysCfg->TravelUpLoadTime;
			NextTime1 = pTime->TimeSec + pSysCfg->WorkUpLoadTime;
			SL_Print(">App:Init UpLoadTime :%d\r\n",NextTime);
			step++;
			break;
		case 1:                    		 //�ǹ���״̬      ��ʻ �Ĺ�����   ����ҵ״̬  200���Ա�λ��   
			if(pTime->TimeSec >= NextTime)
			{
				NextTime =  pTime->TimeSec + pSysCfg->TravelUpLoadTime;
				PacketLen = BuildCompletePacket(DataPacketBuf,0);
				if(PacketLen > 0)
				{
					WriteSendQueue(DataPacketBuf,PacketLen);
					return;
				}
			}
			
			if(tmp > 0)                   //�ж���ҵ��ʶ  
				step++;
			break;
		case 2:                   		//����״̬    
			if(tmp == 0)                              //�������״̬�������������
			{
				PacketLen = BuildPositionPacket(DataPacketBuf,1);  //�������
				if(PacketLen > 0)
				{	
					WriteSendQueue(DataPacketBuf,PacketLen);   //���뵽���Ͷ��� 
					step = 1;
					return;
				}
			}
			if(pTime->TimeSec >= (NextTime - 1))
			{	
				NextTime1 = pTime->TimeSec + pSysCfg->WorkUpLoadTime;
				PacketLen = BuildPositionPacket(DataPacketBuf,1);         //�������
				if(PacketLen > 0)
				{
					WriteSendQueue(DataPacketBuf,PacketLen);   //���뵽���Ͷ��� 
					step++;
					return;
				}

			}
			if(pTime->TimeSec < NextTime1)           		//���촦��
				break;
			NextTime1 = pTime->TimeSec + pSysCfg->WorkUpLoadTime;
			PacketLen = BuildPositionPacket(DataPacketBuf,0);
			if(PacketLen > 0)
			{
				WriteSendQueue(DataPacketBuf,PacketLen);   	//���뵽���Ͷ���
			}
			return;
		case 3:
			if(pTime->TimeSec >= NextTime)
			{
				NextTime =  pTime->TimeSec + pSysCfg->TravelUpLoadTime;
				NextTime1 =  pTime->TimeSec + pSysCfg->WorkUpLoadTime;

				PacketLen = BuildCompletePacket(DataPacketBuf,0);
				if(PacketLen > 0)
				{
					WriteSendQueue(DataPacketBuf,PacketLen);
				}
				step = 2;
			}
			
			return;
		default: 
			step = 0;
			break;
	}

	tmp = ReadAccState();
	switch(step1)
	{
		case 0:
			if(tmp == AccStateBack)
				break;
			init_stopwatch16(&sw);
			step1++;
			break;
		case 1:
			if(tmp == AccStateBack)
			{
				step1 = 0;
				break;
			}
			if(read_stopwatch16_value(&sw) < 60)
				break;
			
			AccStateBack = tmp;
			PacketLen = BuildCompletePacket(DataPacketBuf,0);
			if(PacketLen > 0)
			{
				SL_Print("Acc State Change:\r\n");
				WriteSendQueue(DataPacketBuf,PacketLen);
			}
			step1 = 0;
			break;
		default:
			step1 = 0;
			break;
	}
}


/*******************************************************
**	��������:void ProcessRunData(void)
**	��������:������������
********************************************************/
void ProcessRunData(void)
{
	static unsigned char step2 = 0;
	static unsigned char counter2 = 0;
	
	TerStateStr 	*pTerState;
	GNSSSTR			*pGnss;
	SysTimeStr 		*pTime;
	//unsigned int 	Tmp = 0,Tmp1 = 0;

	pTerState = GetTerStateDataSpace();   //
	
	if(pTerState->AccState > 0)               //����ACC�Ĺ���ʱ��
	{
		SysRunData.ACCTotal++;                 //�ú���1����ִ��һ��   ����һ��
		SysRunData.ACCTotalHour = SysRunData.ACCTotal / 3600.0;
	}
	

	pGnss = GetGnssDataSpace();             //��λ�ռ�
	
	switch(step2)                            //ͨ����λ���ж���ʻ���
	{
		case 0:                        		//��û�����λ��  
			if(pGnss->status != 'A')
				break;
			if(pGnss->speed < 1)
				break;
			DistancePro.StartLat = pGnss->latitude;           //��ʼ����ʼ
			DistancePro.StartLong	 = pGnss->longitude;      //��ʼ����ʼ  
			
			step2++;
			break;
		case 1:
			if(pGnss->status != 'A' || pGnss->speed < 1)
			{
				step2 = 0;	
				break;
			}
			
			DistancePro.EndLat = pGnss->latitude;                   //����λ��
			DistancePro.EndLong = pGnss->longitude ;    			//����λ��   
			DistancePro.Distance = ComputeDistance(DistancePro.StartLat,DistancePro.StartLong,  DistancePro.EndLat,DistancePro.EndLong);
			SysRunData.TotalDistance += DistancePro.Distance;   	//�ۼ���ʻ����  
			
			DistancePro.StartLat = DistancePro.EndLat;       		//��ʼ����ʼ
			DistancePro.StartLong	 = DistancePro.EndLong;      	//��ʼ����ʼ   
			//Tmp = SysRunData.TotalDistance * 10;
			//Tmp1 = DistancePro.Distance * 1000;
			//SL_Print("The TotalDistance1:%d***********%d\r\n",Tmp,Tmp1);
			break;
		default:
			step2 = 1;
			break;
	
	}
	
	pTime = GetSysTimeSpace();
	
	if(counter2++ >= 900)             //û15���ӱ���һ��  ��Լ15����
	{	
		counter2 = 0;
		pTime = GetSysTimeSpace();
		SysRunData.TimeSec = pTime->TimeSec;
		SaveSysRunData();            //��������  
	}
}


/********************************File End************************/

