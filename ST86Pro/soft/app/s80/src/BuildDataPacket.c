

/******************************************
**	FileName:
**	Time:
**	�����ϴ����ݰ�
*******************************************/

#include "include.h"


static unsigned char TmpBuf[512];
static unsigned char MsgLine;   						//�б�ʶ
static unsigned char MsgNum;
static unsigned short int MsgSerial;



/*********************************************
**	��������:
**	��������:
*********************************************/

unsigned short int BuildPositionPacket(unsigned char *source,unsigned char big)
{	
	static unsigned short int DataLen = 0;
	unsigned short int tmp = 0;
	
	MsgHeadStr TmpMsg;
	unsigned char buf[50];
	SysConfigStr *pSysCfg;
	
	MsgHeadStr 	 *pMsgHead;
	MsgGnssStr   *pMsgGnss;
	MsgFarmStr   *pMsgFarm;
	MsgDeviceStr *pMsgDevice;

	GNSSSTR 	 *pGnss;
	SysTimeStr 	 *pTime;

	CanWheatStr  		*pWheat;
	CanMaizeStr 		*pMaize;
	CanPaddyStr 		*pPaddy;
	CanTractorStr 		*pTractor;
	
	pSysCfg = GetSysCfgSpace();
	//MsgNum = 0;
	MsgLine++;
	if(MsgLine <= 1)
	{
		TmpMsg.frame_start[0] = 0xF1;
		TmpMsg.frame_start[1] = 0xF2;
		TmpMsg.frame_start[2] = 0xFF;
		TmpMsg.msg_id = 0x45;
		
		SL_Memcpy(&TmpMsg.device_id,pSysCfg->TerminalID,16);
		TmpMsg.blind_flag = 0x00;
		TmpMsg.msg_body_num = 0x00;
		TmpMsg.msg_len = 0x00;
		SL_Memcpy(TmpBuf,(unsigned char *)&TmpMsg,25);           			//
		DataLen = 25;

		pMsgDevice = (MsgDeviceStr *)buf;
		pMsgDevice->msg_device_type = 0x0001;               	 			//��Ϣ��������(�豸״̬��Ϣ��)
		pMsgDevice->msg_device_len = 7;                             		//��Ϣ�峤��
 
		pMsgDevice->msg_device_manu_num 		= 0x01;                  	//�豸������������
		pMsgDevice->msg_device_manu_type 		= 0x01;                 	//
		pMsgDevice->msg_device_user_num 		= 22;                   	//�û����ұ��
		pMsgDevice->msg_device_car_num 			= pSysCfg->CarType;         //����
		pMsgDevice->msg_device_protocol_ver 	= 18;                		//�ϴ�Э��汾��
		pMsgDevice->msg_device_firware_ver 		= GetVerNum();              //�̼�����汾��
		pMsgDevice->msg_device_hardware_ver 	= pSysCfg->HardWareNum;     //Ӳ���汾��
		
		SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr)); 
		DataLen += sizeof(MsgDeviceStr);
		MsgNum++;
	}

	//��λ��Ϣ��
	pMsgGnss = (MsgGnssStr *)buf;
	pGnss = GetGnssDataSpace();
	pTime = GetSysTimeSpace();
	
	pMsgGnss->msg_gps_type = 0x0100;                   				//
	pMsgGnss->msg_gps_len = 26;                                     //
	pMsgGnss->msg_gps_latitude = pGnss->latitude * 1000000;         //
	pMsgGnss->msg_gps_longitude = pGnss->longitude * 1000000;      	//
	pMsgGnss->msg_gps_speed = pGnss->speed * 10;					//
	pMsgGnss->msg_gps_azimuth  = pGnss->azimuth;         			//
	pMsgGnss->msg_gps_altitude = pGnss->altitude;        			//
	pMsgGnss->msg_gps_year = pTime->Year;							//
	pMsgGnss->msg_gps_mon = pTime->Mon;								//
	pMsgGnss->msg_gps_day = pTime->MDay;							//
	pMsgGnss->msg_gps_hour = pTime->Hour;							//
	pMsgGnss->msg_gps_min = pTime->Min;								//
	pMsgGnss->msg_gps_sec = pTime->Sec;								//
	pMsgGnss->msg_gps_satellite_num = pGnss->satellite_num;			//
	pMsgGnss->msg_gps_view_num = pGnss->satellite_view_num;			//
	pMsgGnss->msg_gps_ghdop_v = pGnss->ghdop_v;						//
	
		if(pGnss->status == 'A')
		pMsgGnss->msg_gps_status = pGnss->status | 0x02;
	else
		pMsgGnss->msg_gps_status = pGnss->status & 0xFD;
	
	pMsgGnss->msg_gps_status = pGnss->status;						//
	pMsgGnss->msg_gps_line = MsgLine;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgGnssStr)); 
	DataLen += sizeof(MsgGnssStr);
	MsgNum++;

	//�豸ʱ��
	pMsgFarm = (MsgFarmStr *)buf;

	pWheat = GetCanWheatSpace();     	//С���
	pPaddy = GetCanPaddySpace();     	//ˮ����
	pMaize = GetCanMaizeSpace();     	//���׻�  
	pTractor = GetCanTractorSpace();   	//������ 
	pMsgFarm->MsgFarmType = 0x0980;                                 //��Ϣ��������
	pMsgFarm->MsgFarmLen = 9;                                       //��Ϣ�������򳤶�
	switch(pSysCfg->CarType)
	{
		case 0x08:
			pMsgFarm->MsgFarmBig = pWheat->Rotate1;         		//����Ͳת��
			break;
		case 0x0E:
			pMsgFarm->MsgFarmBig = pWheat->EngineRotate / 8;    
			break;
		case 0x0A:
			pMsgFarm->MsgFarmBig = pMaize->Rotate1;
			break;
		case 0x09:
			pMsgFarm->MsgFarmBig = pPaddy->Rotate1;
			break;
		case 0x03:
			pMsgFarm->MsgFarmBig = 0;
			break;
		default:
			pMsgFarm->MsgFarmBig = 0;
			break;
	}
	pMsgFarm->MsgTimeYear = pTime->Year;							//
	pMsgFarm->MsgTimeMon = pTime->Mon;								//
	pMsgFarm->MsgTimeDay = pTime->MDay;								//
	pMsgFarm->MsgTimeHour = pTime->Hour;							//
	pMsgFarm->MsgTimeMin = pTime->Min;                              //
	pMsgFarm->MsgTimeSec = pTime->Sec;                              //
	pMsgFarm->MsgLine = MsgLine;                                    //
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgFarmStr)); 
	DataLen += sizeof(MsgFarmStr);
	MsgNum++;

	if(MsgLine >= 10 || big > 0)    								//�������                     
	{
		pMsgHead = (MsgHeadStr *)TmpBuf; 
		pMsgHead->msg_line = MsgLine;
		pMsgHead->msg_body_num = MsgNum;
		pMsgHead->msg_len = DataLen - 23; 	//���������2���ֽ�
		MsgSerial++;
		SL_Memcpy(TmpBuf + DataLen,(unsigned char *)&MsgSerial,2);
		DataLen += 2;
		*(TmpBuf + DataLen) = BccVerify(TmpBuf + 3,DataLen - 3);
		DataLen++;
		*(TmpBuf + DataLen) = 0x0D;
		DataLen++;
		SL_Memcpy(source,TmpBuf,DataLen);
		tmp = DataLen;
		SL_Print(">App:CompletePacket Finsh  1...... %d\r\n",DataLen);
		MsgNum = 0;
		DataLen = 0;
		MsgLine = 0;
	}
	return tmp;
}	



/******************************************************
**	��������:
**	��������:
******************************************************/

unsigned short int BuildCompletePacket(unsigned char *source,unsigned char big)
{
	MsgHeadStr TmpMsg;
	unsigned short int DataLen;
	unsigned char buf[50];
	SysConfigStr *pSysCfg;
	

	//��Ϣ��ָ��
	MsgHeadStr 	 		*pMsgHead;
	MsgDeviceStr 		*pMsgDevice;
	MsgInputStr  		*pMsgInput;
	MsgGnssStr   		*pMsgGnss;
	MsgFarmStr   		*pMsgFarm;
	MsgCanWheatStr 		*pMsgCanWheat;
	MsgCanPaddyStr		*pMsgCanPaddy;
	MsgCanMaizeStr		*pMsgCanMaize;
	MsgCanTractorStr	*pMsgCanTractor;
	
	//����ָ��
	GNSSSTR 			*pGnss;
	TerStateStr			*pTerState;
	SysRunDataStr 		*pSysRun;
	SysTimeStr 			*pSysTime;      //ϵͳʱ��
	CanWheatStr			*pCanWheat;     //С���
	CanPaddyStr			*pCanPaddy;     //ˮ����
	CanMaizeStr			*pCanMaize;     //���׻�
	CanTractorStr		*pCanTractor;   //������
	
	pSysCfg = GetSysCfgSpace();       	//��ȡ�豸������Ϣ
	
	DataLen = 0;                     	//��ʶ���ݳ���
	big = 0;
	MsgNum = 0;
	//��Ϣͷ
	TmpMsg.frame_start[0] = 0xF1;
	TmpMsg.frame_start[1] = 0xF2;
	TmpMsg.frame_start[2] = 0xFF;
	TmpMsg.msg_id = 0x45;
	SL_Memcpy(&TmpMsg.device_id,pSysCfg->TerminalID,16);            //�豸��
	TmpMsg.blind_flag = 0x00;                                       //
	TmpMsg.msg_body_num = 0x00;                                     //
	TmpMsg.msg_len = 0x00;                                          //
	SL_Memcpy(TmpBuf,(unsigned char *)&TmpMsg,25);          		//
	DataLen = 25;
	
	//�豸״̬��Ϣ��:

	
	pMsgDevice = (MsgDeviceStr *)buf;
	pMsgDevice->msg_device_type = 0x0001;               		 	//
	pMsgDevice->msg_device_len = 8;									//

	pMsgDevice->msg_device_manu_num = 0x01;                  		//
	pMsgDevice->msg_device_manu_type = 0x01;                  	 	//
	pMsgDevice->msg_device_user_num = 22;                    		//�û����
	pMsgDevice->msg_device_car_num = pSysCfg->CarType;				//pSysCfg->CarType;            //���ͱ��  
	pMsgDevice->msg_device_firware_ver = GetVerNum();
	pMsgDevice->msg_device_protocol_ver = 18;                		//
	pMsgDevice->msg_device_hardware_ver = pSysCfg->HardWareNum;		//
	
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr));    
	DataLen += sizeof(MsgDeviceStr); 
	*(TmpBuf + DataLen) = 1;
	DataLen += 1;
	MsgNum++;
	
	//����״̬����

	pTerState = GetTerStateDataSpace();
	pSysRun  = GetSysRunDataSpace();
	
	pMsgInput = (MsgInputStr *)buf;
	
	pMsgInput->msg_input_type = 0x0004;                             //��Ϣ��������(����״̬��Ϣ��)
	pMsgInput->msg_input_len = 24;                                  //��Ϣ�峤��
	pMsgInput->msg_input_io = 0x00;
	pMsgInput->msgAccState = 1;                						//ACC״̬
	pMsgInput->msgMotoState = 0;
	pMsgInput->msgFrq1 = 0;
	pMsgInput->msgFrq2 = 0;
	pMsgInput->msgPWM1 = 0;
	pMsgInput->msgPWM2 =  0;
	pMsgInput->msgInputVol1 = 0;
	pMsgInput->msgInputVol2 = 0;
	pMsgInput->msgPowVol = pTerState->PowVol * 100;//�ⲿ�����ѹ
	pMsgInput->msgBatteryVol = pTerState->Battery * 100;//�ڲ���ص�ѹ
	pMsgInput->msgACCTotalTime = pSysRun->ACCTotalHour * 10;                      			//ACC�ܿ���ʱ��
	pMsgInput->msgLine = 1;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgInputStr)); 	
	DataLen += sizeof(MsgInputStr);
	MsgNum++;
	
	
	//��λ��Ϣ��
	pGnss = GetGnssDataSpace();
	pSysTime = GetSysTimeSpace();
	pMsgGnss = (MsgGnssStr *)buf;
	pMsgGnss->msg_gps_type = 0x0100;                   														//
	pMsgGnss->msg_gps_len = 26;                                     						//
	pMsgGnss->msg_gps_latitude = pGnss->latitude * 1000000;        					//
	pMsgGnss->msg_gps_longitude = pGnss->longitude * 1000000;      					//
	pMsgGnss->msg_gps_speed = pGnss->speed * 10;									 						//?��?��
	pMsgGnss->msg_gps_azimuth  = pGnss->azimuth;         										//???����??��
	pMsgGnss->msg_gps_altitude = pGnss->altitude;        										//o??��??
	pMsgGnss->msg_gps_year = pSysTime->Year;
	pMsgGnss->msg_gps_mon = pSysTime->Mon;
	pMsgGnss->msg_gps_day = pSysTime->MDay;
	pMsgGnss->msg_gps_hour = pSysTime->Hour;
	pMsgGnss->msg_gps_min = pSysTime->Min;
	pMsgGnss->msg_gps_sec = pSysTime->Sec;
	pMsgGnss->msg_gps_satellite_num = pGnss->satellite_num;
	pMsgGnss->msg_gps_view_num = pGnss->satellite_view_num;
	pMsgGnss->msg_gps_ghdop_v = pGnss->ghdop_v;

	if(pGnss->status == 'A')
		pMsgGnss->msg_gps_status = pGnss->status | 0x02;
	else
		pMsgGnss->msg_gps_status = pGnss->status & 0xFD;
	
	pMsgGnss->msg_gps_line = 1;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgGnssStr)); 
	DataLen += sizeof(MsgGnssStr);
	MsgNum++;
	
	//�豸ʱ��
	pMsgFarm = (MsgFarmStr *)buf;

	pMsgFarm->MsgFarmType = 0x0980;
	pMsgFarm->MsgFarmLen = 9;

	pCanWheat = GetCanWheatSpace();     	//С���
	pCanPaddy = GetCanPaddySpace();     	//ˮ����
	pCanMaize = GetCanMaizeSpace();     	//���׻�  
	pCanTractor = GetCanTractorSpace();   	//������ 

	switch(pSysCfg->CarType)
	{
		case 0x08:
			pMsgFarm->MsgFarmBig = pCanWheat->Rotate1;   //С��� �¹��� 
			break;
		case 0x0E:
			pMsgFarm->MsgFarmBig = pCanWheat->EngineRotate / 8;
			break;
		case 0x0A:
			pMsgFarm->MsgFarmBig = pCanMaize->Rotate1;       //���׻�
			break;
		case 0x09:
			pMsgFarm->MsgFarmBig = pCanPaddy->Rotate1;       //ˮ����
			break;
		case 0x03:
			pMsgFarm->MsgFarmBig = 0;                        //������
			break;
		default:
			pMsgFarm->MsgFarmBig = 0;
			break;
	}
	
	pMsgFarm->MsgTimeYear = pSysTime->Year;
	pMsgFarm->MsgTimeMon = pSysTime->Mon;
	pMsgFarm->MsgTimeDay = pSysTime->MDay;
	pMsgFarm->MsgTimeHour = pSysTime->Hour;
	pMsgFarm->MsgTimeMin = pSysTime->Min;
	pMsgFarm->MsgTimeSec = pSysTime->Sec;
	pMsgFarm->MsgLine = 1;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgFarmStr)); 
	DataLen += sizeof(MsgFarmStr);
	MsgNum++;
	switch(pSysCfg->CarType)
	{
		case 0x08:  //С��� 
		case 0x0E:
			pMsgCanWheat = (MsgCanWheatStr *)buf;
			pMsgCanWheat->MsgCanWheatType = 0x000D;
			pMsgCanWheat->MsgCanWheatLen = sizeof(MsgCanWheatStr) - 4;
			pMsgCanWheat->MsgEngineRotate = pCanWheat->EngineRotate;      //������ת��
			pMsgCanWheat->MsgWaterTemp = pCanWheat->WaterTemp;    		//��ȴˮ�¶�
			pMsgCanWheat->MsgEngineOil = pCanWheat->EngineOil;    		//����ѹ��
			pMsgCanWheat->MsgFuelPercent = pCanWheat->FuelPercent;  		//ȼ�ϰٷֱ�
			pMsgCanWheat->MsgEngineWorkTime = pCanWheat->EngineWorkTime;    //����������ʱ��  
			pMsgCanWheat->MsgSysVal = pCanWheat->SysVal;            //ϵͳ��ѹ
			pMsgCanWheat->MsgTotalTravlled = pCanWheat->TotalTravlled;     //����ʻ���
			pMsgCanWheat->MsgTravlledSpeed = pCanWheat->TravlledSpeed;     //��ʻ�ٶ� 
			pMsgCanWheat->MsgRotate1 = pCanWheat->Rotate1;        	//��Ͳת�� 
			pMsgCanWheat->MsgRotate2 = pCanWheat->Rotate2;        	//������ת��
			pMsgCanWheat->MsgRotate3 = pCanWheat->Rotate3;        	//����������
			pMsgCanWheat->MsgRotate4 = pCanWheat->Rotate4;        	//��ѡ����ת��
			pMsgCanWheat->MsgRotate5 = pCanWheat->Rotate5;        	//ι�����ת��
			pMsgCanWheat->MsgRotate6 = pCanWheat->Rotate6;        	//����ת��
			pMsgCanWheat->MsgLossRatio = pCanWheat->LossRatio;         //��ѡ��ʧ��
			pMsgCanWheat->MsgCabTemp = pCanWheat->CabTemp;           //��ʻ���¶�
			pMsgCanWheat->MsgReapHigh = pCanWheat->ReapHigh;          //���߶�
			pMsgCanWheat->MsgAperture1 = pCanWheat->Aperture1;         //��ɸ����
			pMsgCanWheat->MsgAperture2 = pCanWheat->Aperture2;         //��ɸ����
			pMsgCanWheat->MsgAperture3 = pCanWheat->Aperture3;         //βɸ����
			pMsgCanWheat->MsgWarnValue = pCanWheat->WarnValue;         //��������
			pMsgCanWheat->MsgLine = 1;
			
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanWheatStr)); 
			DataLen += sizeof(MsgCanWheatStr);
			MsgNum++;
			//SL_Print(">APP:Xiao Mai Ji.........\r\n");
			break;
		case 0x0A:                                  //���׻�
			pCanMaize = GetCanMaizeSpace();
			pMsgCanMaize = (MsgCanMaizeStr *)buf;

			pMsgCanMaize->MsgCanMaizeType = 0x000C;
			pMsgCanMaize->MsgCanMaizeLen = sizeof(MsgCanMaizeStr) - 4;

			pMsgCanMaize->MsgEngineRotate = pCanMaize->EngineRotate;      		//������ת��
			pMsgCanMaize->MsgWaterTemp = pCanMaize->WaterTemp;    				//��ȴˮ�¶�
			pMsgCanMaize->MsgEngineOil = pCanMaize->EngineOil;    				//����ѹ��
			pMsgCanMaize->MsgFuelPercent = pCanMaize->FuelPercent;  			//ȼ�ϰٷֱ�
			pMsgCanMaize->MsgEngineWorkTime = pCanMaize->EngineWorkTime;    	//����������ʱ�� 
			pMsgCanMaize->MsgSysVol = pCanMaize->SysVol;            			//ϵͳ��ѹ
			pMsgCanMaize->MsgTotalTravlled = pCanMaize->TotalTravlled;     		//����ʻ���
			pMsgCanMaize->MsgRotate1 = pCanMaize->Rotate1;        				//��Ƥ��ת��
			pMsgCanMaize->MsgRotate2 = pCanMaize->Rotate2;        				//������ת��
			pMsgCanMaize->MsgTravlledSpeed = pCanMaize->TravlledSpeed;     		//��ʻ�ٶ�
			pMsgCanMaize->MsgReapHigh = pCanMaize->ReapHigh;          			//���߶�
			pMsgCanMaize->MsgWarnValue = pCanMaize->WarnValue;         			//��������
			pMsgCanMaize->MsgLine = 1;
			
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanMaizeStr)); 
			DataLen += sizeof(MsgCanMaizeStr);
			MsgNum++;
			//SL_Print(">APP:Yu Mi Ji..................\r\n");
			break;
		case 0x09:                                  							//ˮ����  �����Ϲ��� ˮ���� 
			pCanPaddy = GetCanPaddySpace();
			pMsgCanPaddy = (MsgCanPaddyStr *)buf;

			pMsgCanPaddy->MsgCanPaddyType = 0x000E;				   				//
			pMsgCanPaddy->MsgCanPaddyLen = sizeof(MsgCanPaddyStr) - 4;			//
			
			pMsgCanPaddy->MsgEngineRotate = pCanPaddy->EngineRotate;     		//������ת��
			pMsgCanPaddy->MsgWaterTemp = pCanPaddy->WaterTemp;    				//��ȴˮ�¶�
			pMsgCanPaddy->MsgRotate1 = pCanPaddy->Rotate1;        				//�����ٶ��ж�
			pMsgCanPaddy->MsgRotate2 = pCanPaddy->Rotate2;        				//������ת��
			pMsgCanPaddy->MsgSysVol = pCanPaddy->SysVol;            			//ϵͳ��ѹ
			pMsgCanPaddy->MsgEngineWorkTime = pCanPaddy->EngineWorkTime;    	//����������ʱ�� 
			pMsgCanPaddy->MsgWarnValue = pCanPaddy->WarnValue;         			//��������
			pMsgCanPaddy->MsgTravlledSpeed = pCanPaddy->TravlledSpeed;     		//��ʻ�ٶ�
			pMsgCanPaddy->MsgTotalTravlled = pCanPaddy->TotalTravlled;     		//����ʻ���
			pMsgCanPaddy->MsgLine = 1;
			
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanPaddyStr)); 
			DataLen += sizeof(MsgCanPaddyStr);
			MsgNum++;
			//SL_Print(">App:Shui Dao Ji.....................\r\n");
			break;								
		case 0x03:
			pCanTractor = GetCanTractorSpace();
			pMsgCanTractor = (MsgCanTractorStr *)buf;

			pMsgCanTractor->MsgCanTractorType = 0x000F; 				 		//��Ϣ��������  
			pMsgCanTractor->MsgCanTractorLen = sizeof(MsgCanTractorStr) - 4;	//��Ϣ�峤��
			
			pMsgCanTractor->MsgEngineRotate = pCanTractor->EngineRotate;		//������ת��
			pMsgCanTractor->MsgWaterTemp =	pCanTractor->WaterTemp; 			//��ȴˮ�¶�
			pMsgCanTractor->MsgEngineOil =  pCanTractor->EngineOil; 			//����ѹ��
			pMsgCanTractor->MsgFuelPercent = pCanTractor->FuelPercent;			//ȼ�ϰٷֱ�
			pMsgCanTractor->MsgEngineWorkTime = pCanTractor->EngineWorkTime;	//����������ʱ�� 
			pMsgCanTractor->MsgSysVol =	pCanTractor->SysVol;					//ϵͳ��ѹ
			pMsgCanTractor->MsgTravlledSpeed = pCanTractor->TravlledSpeed; 		//��ʻ�ٶ�
			pMsgCanTractor->MsgTotalTravlled =	pCanTractor->TotalTravlled; 	//����ʻ���
			pMsgCanTractor->MsgWarnValue = pCanTractor->WarnValue; 				//��������
			pMsgCanTractor->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanTractorStr)); 
			DataLen += sizeof(MsgCanTractorStr);
			MsgNum++;
			//SL_Print("M Tuo La ji..............................\r\n");
			break;								//������
		default:
			break;
	}
	
	pMsgHead = (MsgHeadStr *)TmpBuf;
	pMsgHead->msg_len = DataLen - 23;          //��������볤��  
	pMsgHead->msg_body_num = MsgNum;
	pMsgHead->msg_line = 1;
	MsgSerial++;
	SL_Memcpy(TmpBuf + DataLen,(unsigned char *)&MsgSerial,2); 
	
	DataLen += 2;
	
	
	*(TmpBuf + DataLen) = BccVerify(TmpBuf + 3,DataLen - 3);
	DataLen++;
	*(TmpBuf + DataLen) = 0x0D;
	DataLen++;
	SL_Memcpy(source,TmpBuf,DataLen);

	SL_Print(">App:CompletePacket Finsh 2...... %d\r\n",DataLen);

	return DataLen;
	
}



/************************************************************
**	��������:
**	��������:
************************************************************/



/**************************File End***************************/

