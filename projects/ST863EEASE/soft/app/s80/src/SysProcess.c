
/***************************************************************
**	FileName:SysProcess.c
**	Time:	2017.08.12
****************************************************************/


#include "include.h"

/**************����ȫ�ֱ���*******************/

static unsigned char 		DataPacketBuf[512];

static SysRunDataStr 		SysRunData;

static DistanceProStr 		DistancePro;

static	unsigned char 		AccStateBack = 0;     	//��ʶ�豸ACC״̬

static int 					NextTime = 0;			//

static int					NextTime1 = 0;    		//



/********************************************************
**	��������:ResetNexTime(void)
**	��������:
********************************************************/

void ResetNextTime(void)
{
	NextTime = 1;		//
	NextTime1 = 1;    	//
}



/********************************************************
**	��������:SysRunDataStr *GetSysRunDataSpace(void)
**	��������:	
********************************************************/

SysRunDataStr *GetSysRunDataSpace(void)
{
	return &SysRunData;
}


/********************************************************
**	��������:unsigned char ReadFarmStatus(void)
**	��������:�ж�ϵͳ��ҵ��־
*********************************************************/

unsigned char ReadFarmStatus(void)
{
	static unsigned short counter = 0;
	
	SysConfigStr		*pCfg;
	CanDataStr			*pCanData;
	
	pCfg 	= GetSysCfgSpace();
	pCanData = GetCanDataSpace();
	
	if(ReadAccState() == 0)
		return 0;
	
	switch(pCfg->CarType)
	{
		case 0x03:      					//������  ��������ҵ�ж�  ��Ҫ��Mƽ̨������
		case 0x0D:
			if(pCanData->EngineRotate > 8000)    
			{
				if(counter < 240)           //
					counter++;
			}
			else
			{
				counter = 0;
			}

			if(counter > 120)
				return 1;
			return 0;
		
		case 0x09:							//ˮ����  ����ת��
			if(pCanData->Rotate1 > 10)    
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
		case 0x0f:							//ˮ����  �Ϲ��� ת�ٴ���2000ת����λ��0.125����Ҫ����8
			if(pCanData->EngineRotate > 16000)    
			{
				if(counter < 240)           //
					counter++;
			}
			else
			{
				counter = 0;
			}

			if(counter > 120)
				return 1;
			return 0;
		default:
			return 0;
	}

}


/*****************************************************
**	��������:
**	��������:
*****************************************************/

void SaveSysRunData(unsigned char n)
{
	static unsigned short int counter = 0;

	SysTimeStr *pTime;
	GNSSSTR		*pGnss;
	
	if(n > 0)
		counter = 1000;
	
	if(counter++ >= 900)			  //û15���ӱ���һ��  ��Լ15����
	{	
		counter = 0;
		pTime = GetSysTimeSpace();
		SysRunData.TimeSec = pTime->TimeSec;	  //����ʱ�� 

		pGnss = GetGnssDataSpace();

		SysRunData.latitude = pGnss->latitude;									//ά��
		SysRunData.longitude = pGnss->longitude;								//����
		SysRunData.altitude = pGnss->altitude;      							//���θ߶�
		SysRunData.azimuth = pGnss->azimuth;	 								//����Ƕ�
		SysRunData.speed = pGnss->speed;		   								//�ٶ�
		SysRunData.ghdop_v = pGnss->ghdop_v;     								//��λ����
		SysRunData.satellite_num = pGnss->satellite_num;         	 			//ʹ����������
		SysRunData.satellite_view_num = pGnss->satellite_view_num;  			//������������
		SysRunData.status = pGnss->status;				            						//��λ״̬  
		SysRunData.TYear = pGnss->TYear;                                   		//GNSS-ʱ��-��
		SysRunData.TMon = pGnss->TMon;											//GNSS-ʱ��-��
		SysRunData.TDay = pGnss->TDay;											//GNSS-ʱ��-��
		SysRunData.THour = pGnss->THour;										//GNSS-ʱ��-ʱ
		SysRunData.TMin = pGnss->TMin;											//GNSS-ʱ��-��
		SysRunData.TSec = pGnss->TSec;                                     		//GNSS-ʱ��-��                           
		SysRunData.gnss_time = pGnss->gnss_time;              					//��λʱ��,��ʱ����2000��1��1�ţ�0ʱ0��0��Ϊ��׼��
		SysRunData.latitude_ns = pGnss->latitude_ns;           					//ά�ȱ�ʶ
		SysRunData.longitude_ew = pGnss->longitude_ew;            				//���ȱ�ʶ
		
		if(FileCheckOrGreate((unsigned char *)"SysRunDataFile", (unsigned char *)&SysRunData, sizeof(SysRunData),1) > 0)
			SL_Print(">App:Save Sys Run Data.......%d\r\n",n);
	}
		
}


/*****************************************************
**	��������:
**	��������:
******************************************************/

void ReadSysRunData(void)
{
	GNSSSTR *pGnss;
	
	if(FileCheckOrGreate((unsigned char *)"SysRunDataFile", (unsigned char *)&SysRunData, sizeof(SysRunData),0) > 0)
	{
		SL_Print(">App:Reads Run Data.......OK\r\n");
		if(SysRunData.TimeSec > 3155759999)        //ʱ��
			SysRunData.TimeSec = 0;                //�����ʱ��ظ�Ϊ0
		// ��ά�Ƚ�������
		pGnss = GetGnssDataSpace();

		pGnss->latitude = SysRunData.latitude;									//ά��
		pGnss->longitude = SysRunData.longitude;								//����
		pGnss->altitude = SysRunData.altitude;      							//���θ߶�
		pGnss->azimuth = SysRunData.azimuth;	 								//����Ƕ�
		pGnss->speed = SysRunData.speed;		   								//�ٶ�
		pGnss->ghdop_v = SysRunData.ghdop_v;     								//��λ����
		pGnss->satellite_num = SysRunData.satellite_num;         	 			//ʹ����������
		pGnss->satellite_view_num = SysRunData.satellite_view_num;  			//������������
		pGnss->status = 'V';				            						//��λ״̬  
		pGnss->TYear = SysRunData.TYear;                                   		//GNSS-ʱ��-��
		pGnss->TMon = SysRunData.TMon;											//GNSS-ʱ��-��
		pGnss->TDay = SysRunData.TDay;											//GNSS-ʱ��-��
		pGnss->THour = SysRunData.THour;										//GNSS-ʱ��-ʱ
		pGnss->TMin = SysRunData.TMin;											//GNSS-ʱ��-��
		pGnss->TSec = SysRunData.TSec;                                     		//GNSS-ʱ��-��                           
		pGnss->gnss_time = SysRunData.gnss_time;              					//��λʱ��,��ʱ����2000��1��1�ţ�0ʱ0��0��Ϊ��׼��
		pGnss->latitude_ns = SysRunData.latitude_ns;           					//ά�ȱ�ʶ
		pGnss->longitude_ew = SysRunData.longitude_ew;            				//���ȱ�ʶ
		
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
	static struct stopwatch16   sw;
	unsigned char 				tmp = 0;
	unsigned short int 			PacketLen = 0;
	SysConfigStr 				*pSysCfg;

	SysTimeStr					*pTime;             //����ʹ��
	
	pSysCfg = GetSysCfgSpace();						//ϵͳ����
	pTime = GetSysTimeSpace();                      //ϵͳʱ��
	tmp = ReadFarmStatus();              			//��ȡ����״̬

	if(pSysCfg->CfgFlag != 0x55 || ReadRunMode() > 0)
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
				//SL_Print("Build Data Time:%d,%d,%d\r\n",pTime->Hour,pTime->Min,pTime->Sec);
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
				PacketLen = BuildPositionPacket(DataPacketBuf,1);         	//�������
				if(PacketLen > 0)
				{
					WriteSendQueue(DataPacketBuf,PacketLen);   				//���뵽���Ͷ��� 
					step++;
					return;
				}

			}
			if(pTime->TimeSec < NextTime1)           						//
				break;
			NextTime1 = pTime->TimeSec + pSysCfg->WorkUpLoadTime;
			PacketLen = BuildPositionPacket(DataPacketBuf,0);
			if(PacketLen > 0)
			{
				WriteSendQueue(DataPacketBuf,PacketLen);   					//���뵽���Ͷ���
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
	switch(step1)                       		// ACC�仯�ϴ�����
	{
		case 0:
			if(tmp == AccStateBack)         	//���ACC�����ı�
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
				//SL_Print("Acc State Change:\r\n");             //������Ϣ
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

	TerStateStr 	*pTerState;
	GNSSSTR			*pGnss;

	pTerState = GetTerStateDataSpace();   		//
	
	if(pTerState->AccState > 0)               	//����ACC�Ĺ���ʱ��
	{
		SysRunData.ACCTotal++;                 	//�ú���1����ִ��һ��   ����һ��
		SysRunData.ACCTotalHour = SysRunData.ACCTotal / 3600.0;
	}
	

	pGnss = GetGnssDataSpace();             	//��λ�ռ�    
	
	switch(step2)                            	//ͨ����λ���ж���ʻ���
	{


		case 0:                        			//��û�����λ��  
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
		
			break;
		default:
			step2 = 1;
			break;
	
	}
	
	SaveSysRunData(0);            									//��������  
	
}


/********************************File End************************/




