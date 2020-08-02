

/******************************************
**	FileName:
**	Time:
**	�����ϴ����ݰ�
**
*******************************************/

#include "include.h"


static unsigned char 				TmpBuf[512];					//���ݻ�����
static unsigned char 				MsgLine;   						//�б�ʶ
static unsigned char 				MsgNum;							//��Ϣ������

static unsigned short int 			MsgSerial;						//��Ϣ������



/**********************************************************************************************
**	��������:unsigned short int BuildPositionPacket(unsigned char *source,unsigned char big)
**	��������:
***********************************************************************************************/

unsigned short int BuildPositionPacket(unsigned char *source,unsigned char big)
{	
	static unsigned short int DataLen = 0;
	unsigned short int tmp = 0;
	
	MsgHeadStr 			TmpMsg;
	unsigned char 		buf[50];
	SysConfigStr 		*pSysCfg;
	
	MsgHeadStr 	 		*pMsgHead;
	MsgGnssStr   		*pMsgGnss;
	MsgFarmStr   		*pMsgFarm;
	MsgDeviceStr 		*pMsgDevice;

	GNSSSTR 	 		*pGnss;
	SysTimeStr 	 		*pTime;
	TerStateStr			*pTerState;     //ϵͳ�豸״̬���ɼ���������
	CanDataStr 			*pCanData;
	
	pSysCfg = GetSysCfgSpace();
	pTerState = GetTerStateDataSpace();

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
		pMsgDevice->msg_device_protocol_ver 	= pTerState->McuFirNum;		//��Ƭ������汾
		pMsgDevice->msg_device_firware_ver 		= GetVerNum();              //�̼�����汾��
		pMsgDevice->msg_device_hardware_ver 	= pSysCfg->HardWareNum;     //Ӳ���汾��
		
		SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr)); 
		DataLen += sizeof(MsgDeviceStr);
		MsgNum = 1;
		//MsgNum++;
	}

	//��λ��Ϣ��
	pMsgGnss = (MsgGnssStr *)buf;
	pGnss = GetGnssDataSpace();
	pTime = GetSysTimeSpace();
	
	pMsgGnss->msg_gps_type = 0x0100;                   				//   ��Ϣ������
	pMsgGnss->msg_gps_len = 26;                                     //	��Ϣ�������򳤶�
	pMsgGnss->msg_gps_latitude = pGnss->latitude * 1000000;         //   ��λ��Ϣ-����
	pMsgGnss->msg_gps_longitude = pGnss->longitude * 1000000;      	//	��λ��Ϣ-ά��
	pMsgGnss->msg_gps_speed = pGnss->speed * 10;					//	��λ��Ϣ-�ٶ�
	pMsgGnss->msg_gps_azimuth  = pGnss->azimuth;         			//   ��λ��Ϣ-����Ƕ�
	pMsgGnss->msg_gps_altitude = pGnss->altitude;        			//	��λ��Ϣ-���θ߶�

	pMsgGnss->msg_gps_year = pTime->Year;							//   ��λ��Ϣ-ʱ��
	pMsgGnss->msg_gps_mon = pTime->Mon;								//   
	pMsgGnss->msg_gps_day = pTime->MDay;							//
	pMsgGnss->msg_gps_hour = pTime->Hour;							//
	pMsgGnss->msg_gps_min = pTime->Min;								//
	pMsgGnss->msg_gps_sec = pTime->Sec;								//

	pMsgGnss->msg_gps_satellite_num = pGnss->satellite_num;			//  ʹ����������
	pMsgGnss->msg_gps_view_num = pGnss->satellite_view_num;			//  ������������
	pMsgGnss->msg_gps_ghdop_v = pGnss->ghdop_v;						//  ˮƽ��λ����
	
	if(pGnss->status == 'A')                                    //��λ״̬��ʶ
		pMsgGnss->msg_gps_status = pGnss->status | 0x02;
	else
		pMsgGnss->msg_gps_status = pGnss->status & 0xFD;

	pMsgGnss->msg_gps_line = MsgLine;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgGnssStr)); 
	DataLen += sizeof(MsgGnssStr);
	MsgNum++;

	//�豸ʱ��
	pMsgFarm = (MsgFarmStr *)buf;

	pCanData = GetCanDataSpace();
	pMsgFarm->MsgFarmType = 0x0980;                                 //��Ϣ��������
	pMsgFarm->MsgFarmLen = 9;                                       //��Ϣ�������򳤶�
	
	switch(pSysCfg->CarType)
	{
		case 0x09:
			pMsgFarm->MsgFarmBig = pCanData->Rotate1;       //ˮ����
			break;
		case 0x0f:
			pMsgFarm->MsgFarmBig = pCanData->EngineRotate / 8;       //ˮ����,�Ϲ���
			break;
		case 0x03:
		case 0x0D:
			pMsgFarm->MsgFarmBig = pCanData->EngineRotate / 8;                        //������ 
			//SL_Print("(1)pMsgFarm->MsgFarmBig:%d\r\n",pMsgFarm->MsgFarmBig);
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
		//SL_Print("BuildPositionPacket:%d\r\n",pMsgHead->msg_body_num);
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
		//SL_Print(">App:BuildCompletePacket 1...... %d\r\n",DataLen);
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
	unsigned char buf[450];
	SysConfigStr *pSysCfg;

	//��Ϣ��ָ��
	MsgHeadStr 	 		*pMsgHead;              //��Ϣͷ
	MsgDeviceStr 		*pMsgDevice;            //�豸״̬��Ϣ��
	MsgInputStr  		*pMsgInput;             //������Ϣ��
	MsgGnssStr   		*pMsgGnss;              //��λ��Ϣ��
	MsgFarmStr   		*pMsgFarm;              //��ҵ��Ϣ��
	MsgCanPaddyStr		*pMsgCanPaddy;			//
	MsgCanTractorStr	*pMsgCanTractor;		//

	MsgOilStr			*pMsgOil;
	
	
	//����ָ��
	GNSSSTR 			*pGnss;         //��λģ������
	TerStateStr			*pTerState;     //ϵͳ�豸״̬���ɼ���������
	SysTimeStr 			*pSysTime;      //ϵͳʱ��
	CurrentFaultCodeStr	*pFaultCode1;   //��ǰ������
	CanDataStr 			*pCanData;
	
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

	pTerState = GetTerStateDataSpace();
	
	pMsgDevice = (MsgDeviceStr *)buf;
	pMsgDevice->msg_device_type = 0x0001;               		 	//
	pMsgDevice->msg_device_len = 8;									//
	pMsgDevice->msg_device_manu_num = 0x01;                  		//
	pMsgDevice->msg_device_manu_type = 0x01;                  	 	//
	pMsgDevice->msg_device_user_num = 22;                    		//�û����
	pMsgDevice->msg_device_car_num = pSysCfg->CarType;				//pSysCfg->CarType;            //���ͱ��  
	pMsgDevice->msg_device_firware_ver = GetVerNum();               //ST86����
	pMsgDevice->msg_device_protocol_ver = pTerState->McuFirNum;     //��Ƭ������
	pMsgDevice->msg_device_hardware_ver = pSysCfg->HardWareNum;		//Ӳ������
	
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr));    
	DataLen += sizeof(MsgDeviceStr); 
	*(TmpBuf + DataLen) = 1;
	DataLen += 1;
	MsgNum++;
	
	//����״̬����
	
	pMsgInput = (MsgInputStr *)buf;
	
	pMsgInput->msg_input_type = 0x0004;                             //��Ϣ��������(����״̬��Ϣ��)
	pMsgInput->msg_input_len = 24;                                  //��Ϣ�峤��
	pMsgInput->msg_input_io = 0x00;
	pMsgInput->msgAccState = ReadAccState();						//ACC״̬  
	pMsgInput->msgMotoState = 0;
	pMsgInput->msgFrq1 = 0;
	pMsgInput->msgFrq2 = 0;
	pMsgInput->msgPWM1 = 0;
	pMsgInput->msgPWM2 =  0;
	pMsgInput->msgInputVol1 = 0;
	pMsgInput->msgInputVol2 = 0;
	pMsgInput->msgPowVol = pTerState->PowVol * 100;					//�ⲿ�����ѹ
	pMsgInput->msgBatteryVol = pTerState->Battery * 100;			//�ڲ���ص�ѹ
	pMsgInput->msgWarningState = GetWarningValue();          			
	pMsgInput->msgLine = 1;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgInputStr)); 	
	DataLen += sizeof(MsgInputStr);
	MsgNum++;
	
	
	//��λ��Ϣ��
	pGnss = GetGnssDataSpace();
	pSysTime = GetSysTimeSpace();
	pMsgGnss = (MsgGnssStr *)buf;
	pMsgGnss->msg_gps_type = 0x0100;                   								//
	pMsgGnss->msg_gps_len = 26;                                     				//
	pMsgGnss->msg_gps_latitude = pGnss->latitude * 1000000;        					//
	pMsgGnss->msg_gps_longitude = pGnss->longitude * 1000000;      					//  
	pMsgGnss->msg_gps_speed = pGnss->speed * 10;									//
	pMsgGnss->msg_gps_azimuth  = pGnss->azimuth;         							//
	pMsgGnss->msg_gps_altitude = pGnss->altitude;        							//
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

	pCanData = GetCanDataSpace();          //��ȡ���л���CAN����
	switch(pSysCfg->CarType)
	{
		case 0x09:
			pMsgFarm->MsgFarmBig = pCanData->Rotate1;       //ˮ����
			break;
		case 0x0f:
			pMsgFarm->MsgFarmBig = pCanData->EngineRotate / 8;       //ˮ����,�Ϲ���
			break;
		case 0x03:                                           //�¹���Mƽ̨������
		case 0x0D:                                           //�Ϲ���Mƽ̨������
			pMsgFarm->MsgFarmBig = pCanData->EngineRotate / 8;                        //������  
			//SL_Print("(2)pMsgFarm->MsgFarmBig:%d\r\n",pMsgFarm->MsgFarmBig);
			break;
		default:
			pMsgFarm->MsgFarmBig = 0;
			break;
	}
	
	pMsgFarm->MsgTimeYear = pSysTime->Year;
	//SL_Print("The Year:%d\r\n",pMsgFarm->MsgTimeYear);
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
		case 0x09:  							// ˮ����
			pMsgCanPaddy = (MsgCanPaddyStr *)buf;

			pMsgCanPaddy->MsgCanPaddyType = 0x0014;				   				//
			pMsgCanPaddy->MsgCanPaddyLen = sizeof(MsgCanPaddyStr) - 4;			//
			
			pMsgCanPaddy->MsgEngineRotate = pCanData->EngineRotate;     		//������ת��
			pMsgCanPaddy->MsgWaterTemp = pCanData->WaterTemp;    				//��ȴˮ�¶�
			pMsgCanPaddy->MsgRotate1 = pCanData->Rotate1;        				//�����ٶ��ж�
			pMsgCanPaddy->MsgRotate2 = pCanData->Rotate2;        				//������ת��
			pMsgCanPaddy->MsgSysVol = pCanData->SysVol;            			//ϵͳ��ѹ
			pMsgCanPaddy->MsgEngineWorkTime = pCanData->EngineWorkTime;    	//����������ʱ�� 
			pMsgCanPaddy->MsgWarnValue = pCanData->WarnValue1;         			//��������
			pMsgCanPaddy->MsgTravlledSpeed = pCanData->TravlledSpeed;     		//��ʻ�ٶ�
			pMsgCanPaddy->MsgTotalTravlled = pCanData->TotalTravlled;     		//����ʻ���

			pMsgCanPaddy->MsgEngineOil =  pCanData->EngineOil; 			//����ѹ��
			pMsgCanPaddy->MsgFuelPercent = pCanData->FuelPercent;			//ȼ�ϰٷֱ�
//			SL_Print("Oil Data............%d\r\n",pMsgCanPaddy->MsgEngineOil);
			pMsgCanPaddy->MsgLine1 = 1;
			
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanPaddyStr)); 
			DataLen += sizeof(MsgCanPaddyStr);
			MsgNum++;

			pMsgOil = (MsgOilStr *)buf;

			pMsgOil->MsgOilType = 0x0013;
			pMsgOil->MsgOilLen = 9;

			pMsgOil->MsgOil = pCanData->OilConsumption;
			pMsgOil->MsgOilTotal = pCanData->TatalOilConsumption;
			pMsgOil->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgOilStr)); 
			DataLen += sizeof(MsgOilStr);
			//SL_Print("Oil Data.........................3\r\n");
			MsgNum++;
			break;
		case 0x0F:  //  �Ϲ���ˮ����
			pMsgCanPaddy = (MsgCanPaddyStr *)buf;

			pMsgCanPaddy->MsgCanPaddyType = 0x000E;				   				//
			pMsgCanPaddy->MsgCanPaddyLen = sizeof(MsgCanPaddyStr) - 6;			//
			
			pMsgCanPaddy->MsgEngineRotate = pCanData->EngineRotate;     		//������ת��
			pMsgCanPaddy->MsgWaterTemp = pCanData->WaterTemp;    				//��ȴˮ�¶�
			pMsgCanPaddy->MsgRotate1 = pCanData->Rotate1;        				//�����ٶ��ж�
			pMsgCanPaddy->MsgRotate2 = pCanData->Rotate2;        				//������ת��
			pMsgCanPaddy->MsgSysVol = pCanData->SysVol;            				//ϵͳ��ѹ
			pMsgCanPaddy->MsgEngineWorkTime = pCanData->EngineWorkTime;    		//����������ʱ�� 
			pMsgCanPaddy->MsgWarnValue = pCanData->WarnValue1;         			//��������
			pMsgCanPaddy->MsgTravlledSpeed = pCanData->TravlledSpeed;     		//��ʻ�ٶ�
			pMsgCanPaddy->MsgTotalTravlled = pCanData->TotalTravlled;     		//����ʻ���
			pMsgCanPaddy->MsgLine1 = 1;
			
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanPaddyStr) - 2);             //
			DataLen = DataLen + sizeof(MsgCanPaddyStr) - 2;
			MsgNum++;
			break;
		case 0x03:                                  //Mƽ̨������
			pMsgCanTractor = (MsgCanTractorStr *)buf;

			pMsgCanTractor->MsgCanTractorType = 0x000F; 				 		//��Ϣ��������  
			pMsgCanTractor->MsgCanTractorLen = sizeof(MsgCanTractorStr) - 4;	//��Ϣ�峤��
			
			pMsgCanTractor->MsgEngineRotate = pCanData->EngineRotate;		//������ת��
			pMsgCanTractor->MsgWaterTemp =	pCanData->WaterTemp; 			//��ȴˮ�¶�
			pMsgCanTractor->MsgEngineOil =  pCanData->EngineOil; 			//����ѹ��
			pMsgCanTractor->MsgFuelPercent = pCanData->FuelPercent;			//ȼ�ϰٷֱ�
			pMsgCanTractor->MsgEngineWorkTime = pCanData->EngineWorkTime;	//����������ʱ�� 
			pMsgCanTractor->MsgSysVol =	pCanData->SysVol;					//ϵͳ��ѹ
			pMsgCanTractor->MsgTravlledSpeed = pCanData->TravlledSpeed; 		//��ʻ�ٶ�
			pMsgCanTractor->MsgTotalTravlled =	pCanData->TotalTravlled; 	//����ʻ���
			pMsgCanTractor->MsgWarnValue = pCanData->WarnValue1; 				//��������
			pMsgCanTractor->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanTractorStr)); 
			DataLen += sizeof(MsgCanTractorStr);
			MsgNum++;

			pMsgOil = (MsgOilStr *)buf;

			pMsgOil->MsgOilType = 0x0013;
			pMsgOil->MsgOilLen = 9;

			pMsgOil->MsgOil = pCanData->OilConsumption;
			pMsgOil->MsgOilTotal = pCanData->TatalOilConsumption;
			pMsgOil->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgOilStr)); 
			DataLen += sizeof(MsgOilStr);
			//SL_Print("Oil Data.........................3\r\n");
			MsgNum++;
			break;
		case 0x0D:                              //Mƽ̨������ �Ϲ���
			pMsgCanTractor = (MsgCanTractorStr *)buf;

			pMsgCanTractor->MsgCanTractorType = 0x000F; 				 		//��Ϣ��������  
			pMsgCanTractor->MsgCanTractorLen = sizeof(MsgCanTractorStr) - 4;	//��Ϣ�峤��
			
			pMsgCanTractor->MsgEngineRotate = pCanData->EngineRotate;		//������ת��
			pMsgCanTractor->MsgWaterTemp =	pCanData->WaterTemp; 			//��ȴˮ�¶�
			pMsgCanTractor->MsgEngineOil =  pCanData->EngineOil; 			//����ѹ��
			pMsgCanTractor->MsgFuelPercent = pCanData->FuelPercent;			//ȼ�ϰٷֱ�
			pMsgCanTractor->MsgEngineWorkTime = pCanData->EngineWorkTime;	//����������ʱ�� 
			pMsgCanTractor->MsgSysVol =	pCanData->SysVol;					//ϵͳ��ѹ
			pMsgCanTractor->MsgTravlledSpeed = pCanData->TravlledSpeed; 		//��ʻ�ٶ�
			pMsgCanTractor->MsgTotalTravlled =	pCanData->TotalTravlled; 	//����ʻ���
			pMsgCanTractor->MsgWarnValue = pCanData->WarnValue1; 				//��������
			pMsgCanTractor->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanTractorStr)); 
			DataLen += sizeof(MsgCanTractorStr);
			MsgNum++;
			break;
		default:
			break;
	}

	//�����ǹ�����
	pFaultCode1 = GetCurrentFaultCodeSpace();     //��ǰ������

	if(pFaultCode1->FaultCodeNum > 0 && pFaultCode1->FaultCodeNum <= 160)     //�й��������ڵ�ǰ��Χ�ڣ����д��
	{
		IntToChar TmpInt16;

		TmpInt16.IntII = 0x0011;                      //���ͱ���
		buf[0] = TmpInt16.TTbyte[0];
		buf[1] = TmpInt16.TTbyte[1];

		TmpInt16.IntII = pFaultCode1->FaultCodeNum * 4 + 2;   //��Ϣ�峤��  ��Ϣ�峤�ȶ���8�ı���
				
		buf[2] = TmpInt16.TTbyte[0];
		buf[3] = TmpInt16.TTbyte[1];
				
		SL_Memcpy(TmpBuf + DataLen,buf,4);           //���Ⱥ������� 
		DataLen += 4;
				
		*(TmpBuf + DataLen) = pFaultCode1->FaultCodeNum;   //�ϴ�����������������������Ϊ
		DataLen++;
				
		SL_Memcpy(TmpBuf + DataLen,pFaultCode1->FaultCode,pFaultCode1->FaultCodeNum * 4);
		DataLen += pFaultCode1->FaultCodeNum * 4;

		*(TmpBuf + DataLen) = 0x01;
		DataLen += 1;
		MsgNum++;
			
	}
			
	pMsgHead = (MsgHeadStr *)TmpBuf;
	pMsgHead->msg_len = DataLen - 23;          //��������볤��  
	
	pMsgHead->msg_body_num = MsgNum;  
	//SL_Print("BuildCompletePacket:%d\r\n",pMsgHead->msg_body_num);
	pMsgHead->msg_line = 1;
	MsgSerial++;
	SL_Memcpy(TmpBuf + DataLen,(unsigned char *)&MsgSerial,2); 
	
	DataLen += 2;
	
	
	*(TmpBuf + DataLen) = BccVerify(TmpBuf + 3,DataLen - 3);
	DataLen++;
	*(TmpBuf + DataLen) = 0x0D;
	DataLen++;
	SL_Memcpy(source,TmpBuf,DataLen);

	SL_Print(">App:BuildCompletePacket 2...... %d\r\n",DataLen);

	return DataLen;
	
}



/**************************File End***************************/






