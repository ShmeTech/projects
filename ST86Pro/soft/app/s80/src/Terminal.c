

/**************************************************
**	File Name:
**	Time:
**************************************************/
#include "include.h"


#define bitset(var, bitno) ((var) |= 1UL << (bitno))

#define bitclr(var, bitno) ((var) &= ~(1UL << (bitno)))


/************ STM32F�����ļ�������***************/

static UpgradeStatusStr 		UpgradeStatus;                //

static UpgradeFileStr			*UpgradeData;                 //          

/*****************���ݻ�����**********************/

static unsigned char 			TerRecvBuf[1200];           //���ջ�����
static unsigned char 			TerSendBuf[1200];			//���ͻ�����

static unsigned char 			RunMode;   //0:�������У�1��������Ƭ������2����Ƭ����������(ST87����)

/*******************����ȫ�ֱ���********************/


static CanWheatStr 			CanDataWheat;    		//С���
static CanPaddyStr			CanDataPaddy;     		//ˮ����
static CanMaizeStr			CanDataMaize;     		//���׻�
static CanTractorStr		CanDataTractor;		 	//������

static TerStateStr			TerStateData;           //�豸�ն�״̬
static TerSDCardStr			TerSDCardData;          //�豸SD��״̬��
static SysTimeStr 			TerTime;                //�豸ʱ��


/*********************����ȫ�ֱ���*************************/

static unsigned char 			CanProNumSendBig 	= 0;         //CANЭ��ŷ��ͱ�ʶ
static unsigned char 			GetStatusBig 		= 0;         //���豸״̬����
static unsigned char 			GetSDBig 			= 0;         //��ȡSD����ʶ
static unsigned char 			GetCanBig 			= 0;         //��CAN���ݱ�ʶ
static unsigned char 			GetTerTimeBig		= 0;         //��ȡMCUʱ���ʶ      



unsigned char GetCAN[17]    				= {0x5F,0x5F,0x0B,0x00,0x05,0x00,0x05,0x00,0xAA,0xFF,0x23,0x45,0x67,0x00,0x00,0x55,0xAA};
unsigned char GetACC[12]    				= {0x5F,0x5F,0x06,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};
unsigned char GetSD[12]    					= {0x5F,0x5F,0x06,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};
unsigned char GetIPandPOUR[12]  			= {0x5F,0x5F,0x06,0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};
unsigned char GetVIN[12]    				= {0x5F,0x5F,0x06,0x00,0x16,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};
unsigned char Lock_Vehicle[13]				= {0x5F,0x5F,0x07,0x00,0x22,0x00,0x01,0x00,0x00,0x00,0x00,0x55,0xAA};
unsigned char VCUUpdataStartCom[20]			= {0x5F,0x5F,0x0E,0x00,0x2F,0x00,0x08,0x00,0x01,0x00,0x34,0xB1,0x00,0x00,0x66,0x86,0xF9,0x01,0x55,0xAA};
unsigned char GetStatus[12]    				= {0x5F,0x5F,0x06,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};
unsigned char GetTerTime[12]  				= {0x5F,0x5F,0x06,0x00,0x01,0x01,0x00,0x00,0x01,0x01,0x55,0xAA};



/*************************************************
**	��������:unsigned char ReadAccState(void)
**	��������:����ACC״̬
**************************************************/

unsigned char ReadAccState(void)
{
	return TerStateData.AccState;
}

/**************************************************
**	��������:CanWheatStr *GetCanWheatSpace(void)
**	��������:����С���Can���ݵ�ַ
***************************************************/

CanWheatStr *GetCanWheatSpace(void)    //С��� 
{
	return &CanDataWheat;
}


/**************************************************
**	��������:CanPaddyStr *GetCanPaddySpace(void)
**	��������:����ˮ�������ݿռ��ַ
**************************************************/

CanPaddyStr	*GetCanPaddySpace(void) 
{
	return &CanDataPaddy;     //ˮ����  
}


/********************************************************
**	��������:CanMaizeStr *GetCanMaizeSpace(void)
**	��������:�������׻����ݿռ��ַ
*********************************************************/

CanMaizeStr *GetCanMaizeSpace(void)     //���׻�
{
	return	&CanDataMaize;     //���׻�
}


/********************************************************
**	��������:CanTractorStr *GetCanTractorSpace(void)
**	��������:�������������ݿռ��ַ
********************************************************/

CanTractorStr *GetCanTractorSpace(void)		 //������
{
	return &CanDataTractor;		 //������ 
}


/****************************************************
**	��������:TerStateStr	*GetTerStateDataSpace(void)
**	��������:
****************************************************/

TerStateStr	*GetTerStateDataSpace(void)
{
	return &TerStateData;
}


/****************************************************
**	��������:TerSDCardStr *GetTerSDCardSpace(void)
**	��������:�����豸SD����������ռ��ַ
****************************************************/

TerSDCardStr *GetTerSDCardSpace(void)
{
	return &TerSDCardData;
}


/******************************************************
**	��������:SysTimeStr *GetTerTimeSpace(void)
**	��������:�����豸ʱ��ռ�
******************************************************/
SysTimeStr *GetTerTimeSpace(void)
{
	return &TerTime;
}


/***************************************************
**	��������:void SendLockVehicleCmd(void)
**	��������:��������ָ��
***************************************************/

void SendLockVehicleCmd(void)
{
	SL_UartSendData(SL_UART_1,Lock_Vehicle,sizeof(Lock_Vehicle));    		//����
}


/**************************************************
**	��������:UpgradeStr *GetUpgradeDataSpace(void)
**	��������:
**************************************************/

UpgradeStatusStr *GetUpgradeDataSpace(void)
{
	return &UpgradeStatus;
}

/**************************************************
**	��������:
**	��������:
**************************************************/

unsigned char ReadRunMode(void)
{
	return RunMode;
}


/**************************************************
**
**
**************************************************/

void WriteRunMode(unsigned char n)
{
	if(n > 3)
		n = 0;
	RunMode = n;
}	


/****************************************************
**	��������:void SendTerSleepCmd(void)
**	��������:����˯��ָ�MCU
****************************************************/

void SendTerSleepCmd(void)
{
	SysConfigStr   *p;
	unsigned short tmp;
	int i;
	
	LongToChar     TmpData;
	IntToChar		Tmp;
	tmp = 0;

	p = GetSysCfgSpace();
	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	TerSendBuf[2] = 0x0A;
	TerSendBuf[3] = 0x00;
	TerSendBuf[4] = 0x10;
	TerSendBuf[5] = 0x00;
	TerSendBuf[6] = 0x04;
	TerSendBuf[7] = 0x00;
	TmpData.LongLL 	= p->SleepTime;
	TerSendBuf[8] 		= TmpData.TTbyte[0];        
	TerSendBuf[9] 		=TmpData.TTbyte[1];         
	TerSendBuf[10] 		= TmpData.TTbyte[2];         
	TerSendBuf[11] 		= TmpData.TTbyte[3];  
	for(i = 0; i < 8;i++)
		tmp += TerSendBuf[i + 4];
	Tmp.IntII = tmp;
	TerSendBuf[12] = Tmp.TTbyte[0];
	TerSendBuf[13] = Tmp.TTbyte[1];  
	TerSendBuf[14] = 0x55;
	TerSendBuf[15] = 0xAA;
	SL_UartSendData(SL_UART_1,TerSendBuf,16);
}

/****************************************************
**	��������:void SendVerifyTimeCmd(SysTimeStr *pTime)
**	��������:����ʱ��У����Ϣ��MCU
****************************************************/

void SendVerifyTimeCmd(SysTimeStr *pTime)
{
	unsigned short int Temp;

	Temp = 0;

	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	TerSendBuf[2] = 0x0C;
	TerSendBuf[3] = 0x00;
	TerSendBuf[4] = 0x20;
	TerSendBuf[5] = 0x00;
	TerSendBuf[6] = 0x06;
	TerSendBuf[7] = 0x00;
	TerSendBuf[8] = pTime->Year;          //��
	TerSendBuf[9] = pTime->Mon;           //��
	TerSendBuf[10] = pTime->MDay;         //��
	TerSendBuf[11] = pTime->Hour;         //ʱ
	TerSendBuf[12] = pTime->Min;          //��
	TerSendBuf[13] = pTime->Sec;          //��
	*(unsigned short int *)&TerSendBuf[14] = CheckSum16(TerSendBuf + 4,9);
	TerSendBuf[16] = 0x55;
	TerSendBuf[17] = 0xAA;

	SL_UartSendData(SL_UART_1, TerSendBuf,18);
}

/********************************************************
** ��������:void SetTerCanProNum(void)
** ��������:����CanЭ��汾�ŵ�MCU
********************************************************/

void SetTerCanProNum(void)
{
	unsigned short  Temp;

	SysConfigStr *pSysCfg;
	
	Temp = 0;

	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	TerSendBuf[2] = 0x10;
	TerSendBuf[3] = 0x00;
	TerSendBuf[4] = 0x14;
	TerSendBuf[5] = 0x00;
	TerSendBuf[6] = 0x0A;
	TerSendBuf[7] = 0x00;
	pSysCfg = GetSysCfgSpace();
	TerSendBuf[8] = pSysCfg->CanProtocolNum;
	TerSendBuf[9] = 0xF0;

	TerSendBuf[10] = 1;		//LockVehicleStatus;//����״̬1
	TerSendBuf[11] = 0;		//����2
	TerSendBuf[12] = 0;		//����3
	TerSendBuf[13] = 0;		//����4
	TerSendBuf[14] = 0;		//����5
	TerSendBuf[15] = 0;		//����6
	TerSendBuf[16] = 0;		//����7
	TerSendBuf[17] = 0;		//����8
	*(unsigned short int *)&TerSendBuf[18] = CheckSum16(TerSendBuf + 4,13);
	
	TerSendBuf[20] = 0x55;
	TerSendBuf[21] = 0xAA;
	//SL_Print("Send Set Ter Pro Num.......\r\n");
	SL_UartSendData(SL_UART_1, TerSendBuf,22);    		//����

}



/*********************************************************
**	��������:unsigned char ProTermRecvData(void)
**	��������:�����STM32���ص�����
*********************************************************/

unsigned char ProTermRecvData(void)
{
	unsigned short int 		DataLen;
	unsigned short int 		tmp;
	unsigned char 			*p = NULL;
	IntToChar       		TmpInt16;
	SysConfigStr			*pSysCfg;             //���� 
	TcpIPSocketStr			*pTcpIp;
	LongToChar				TmpInt32;
	unsigned char 			TmpStr[50];
	int i;

	if(RunMode >= 1)           //
		return 0; 
	
	SL_Memset(TerRecvBuf,0,sizeof(TerRecvBuf));
	DataLen = ReadComPkt(1,TerRecvBuf,sizeof(TerRecvBuf));    //

	if(DataLen == 0)
		return 0;
	
	if(TerRecvBuf[0] != 0x5F || TerRecvBuf[1] != 0x5F)                //�жϰ�ͷ
		return 0;
	TmpInt16.TTbyte[0] = *(TerRecvBuf + 2);                    //���ݳ��ȣ�����
	TmpInt16.TTbyte[1] = *(TerRecvBuf + 3);
	tmp = TmpInt16.IntII;
	
	if(*(TerRecvBuf + tmp + 4) != 0x55 || *(TerRecvBuf + tmp + 5) != 0xAA)   //�жϰ�β
		return 0;
	
	TmpInt16.TTbyte[0] = *(TerRecvBuf + tmp + 2);
	TmpInt16.TTbyte[1] = *(TerRecvBuf + tmp + 3);
	
	if(TmpInt16.IntII != CheckSum16(TerRecvBuf + 4,tmp - 2))
		return 0;
	
	pSysCfg = GetSysCfgSpace();
	
	TmpInt16.TTbyte[0] = *(TerRecvBuf +  4);
	TmpInt16.TTbyte[1] = *(TerRecvBuf +  5);
	tmp = TmpInt16.IntII;    //��ϢID  
	
	switch(tmp)
	{
		case 0x0014:                                   	 			//����CANͨѶЭ�飬
			CanProNumSendBig = 1;
			break;
		case 0x0001:                                     			//�����豸״̬
			p = TerRecvBuf + 8;
			TerStateData.AccState = *(p);                  			//�������ACC״̬
			TerStateData.MotoState = *(p + 1);			   			//Moto״̬
			TerStateData.SdCardState = *(p + 2);
			TerStateData.E2PROMState = *(p + 3);
			TerStateData.AntConState = *(p + 4);    				//��������״̬
			TerStateData.ShellState  = *(p + 5);  					//���״̬  
			//SL_Print("The Shell State :%d\r\n",TerStateData.ShellState);
			TerStateData.AntShortState = *(p + 6);   				//���߶�·״̬  
			TerStateData.CanStatus = *(p + 7);
			
			TmpInt16.TTbyte[0] 			= *(p + 18);
			TmpInt16.TTbyte[1] 			= *(p + 19);
			TerStateData.PowVol 		= TmpInt16.IntII;          //��Դ��ѹ
			TmpInt16.TTbyte[0] 			= *(p + 20);
			TmpInt16.TTbyte[1] 			= *(p + 21);
			TerStateData.Battery 		= TmpInt16.IntII;         //��ص�ѹ
			TmpInt16.TTbyte[0] 			= *(p + 22);
			TmpInt16.TTbyte[1] 			= *(p + 23);
			TerStateData.TerTemp 		= TmpInt16.IntII;         //��Ƭ��Ƭ���¶�
			GetStatusBig = 1;
			break;
		case 0x0018:
			GetSDBig = 1;                           			//
			p = TerRecvBuf + 8;
			TerSDCardData.SDCardCheck = 12;                		//SD���Ƿ����
			TerSDCardData.SDCardType = 12;           			//SD������
			TerSDCardData.SDCardTotalSpace = 12;    			//SD�������� 
			TerSDCardData.SDCardSurplusSpace = 12;         		//SD��ʣ������
			TerSDCardData.SDCardState = 12;                		//SD��״̬
			TerSDCardData.NC= 12;  
			break;
		case 0x0005:
			bitset(GetCanBig,0);            						//��
			switch(pSysCfg->CarType)
			{
				case 0x08:          										//С���  �¹���
				case 0x0E:                                          //С��� �Ϲ���
					p = TerRecvBuf + 12;
					
					TmpInt16.TTbyte[0] = *(p);
					TmpInt16.TTbyte[1] = *(p + 1);
					if(TmpInt16.IntII == 64127)
						ClearBindData();
					else
						CanDataWheat.EngineRotate = TmpInt16.IntII;          	//������ת��
						
					CanDataWheat.WaterTemp 		= *(p + 2);    				//��ȴˮ�¶�
					CanDataWheat.EngineOil 		= *(p + 3);    				//����ѹ��
					CanDataWheat.FuelPercent    = *(p + 4);  				//ȼ�ϰٷֱ�

					TmpInt32.TTbyte[0] 			= *(p + 5);
					TmpInt32.TTbyte[1] 			= *(p + 6);
					TmpInt32.TTbyte[2] 			= *(p + 7);
					TmpInt32.TTbyte[3] 			= *(p + 8);
					CanDataWheat.EngineWorkTime = TmpInt32.LongLL; 			//����������ʱ��  

										
					TmpInt16.TTbyte[0] 			= *(p + 9);
					TmpInt16.TTbyte[1] 			= *(p + 10);
					CanDataWheat.SysVal 		= TmpInt16.IntII;          //ϵͳ��ѹ
					
					TmpInt32.TTbyte[0] 			= *(p + 11);
					TmpInt32.TTbyte[1] 			= *(p + 12);
					TmpInt32.TTbyte[2] 			= *(p + 13);
					TmpInt32.TTbyte[3] 			= *(p + 14);
					CanDataWheat.TotalTravlled  = TmpInt32.LongLL;     		//����ʻ���

					TmpInt16.TTbyte[0] 			= *(p + 15);
					TmpInt16.TTbyte[1] 			= *(p + 16);
					CanDataWheat.TravlledSpeed  = TmpInt16.IntII;    		//��ʻ�ٶ� 

					TmpInt16.TTbyte[0] 			= *(p + 17);
					TmpInt16.TTbyte[1] 			= *(p + 18);
					CanDataWheat.Rotate1 	  	= TmpInt16.IntII;       	//��Ͳת�� 
					//SL_Print("APP:>%d\r\n",CanDataWheat.Rotate1);
					TmpInt16.TTbyte[0] 			= *(p + 19);
					TmpInt16.TTbyte[1] 			= *(p + 20);
					CanDataWheat.Rotate2		= TmpInt16.IntII;       	//������ת�� 

					TmpInt16.TTbyte[0] 			= *(p + 21);
					TmpInt16.TTbyte[1] 			= *(p + 22);
					CanDataWheat.Rotate3 		= TmpInt16.IntII;        	//����������

					TmpInt16.TTbyte[0] 			= *(p + 23);
					TmpInt16.TTbyte[1] 			= *(p + 24);
					CanDataWheat.Rotate4		= TmpInt16.IntII;        	//��ѡ����ת��

					TmpInt16.TTbyte[0] 			= *(p + 25);
					TmpInt16.TTbyte[1] 			= *(p + 26);
					CanDataWheat.Rotate5		= TmpInt16.IntII;        	//ι�����ת��  

					TmpInt16.TTbyte[0] 			= *(p + 27);
					TmpInt16.TTbyte[1] 			= *(p + 28);
					CanDataWheat.Rotate6		= TmpInt16.IntII;        	//����ת�� 

					TmpInt16.TTbyte[0] 			= *(p + 29);
					TmpInt16.TTbyte[1] 			= *(p + 30);
					CanDataWheat.LossRatio 		= TmpInt16.IntII;         	//��ѡ��ʧ��
					
					CanDataWheat.CabTemp        = *(p + 31);  				//��ʻ���¶�
					CanDataWheat.ReapHigh       = *(p + 32);  				//���߶�
					CanDataWheat.Aperture1      = *(p + 33);  				//��ɸ����
					CanDataWheat.Aperture2      = *(p + 34); 				//��ɸ����
					CanDataWheat.Aperture3      = *(p + 35); 				//βɸ����
					CanDataWheat.WarnValue      = *(p + 36); 				//��������  
					
					break;
				case 0x0A:			// ���׻�
					p = TerRecvBuf + 12;

					TmpInt16.TTbyte[0] 			= *(p);
					TmpInt16.TTbyte[1] 			= *(p + 1);
					
					if(TmpInt16.IntII == 64127)
						ClearBindData();               //�ж����ä��
					else
						CanDataMaize.EngineRotate 	= TmpInt16.IntII;      		//������ת��
					
					CanDataMaize.WaterTemp 		= *(p + 2);    				//��ȴˮ�¶�
					CanDataMaize.EngineOil 		= *(p + 3);    				//����ѹ��
					CanDataMaize.FuelPercent 	= *(p + 4);  				//ȼ�ϰٷֱ�

					TmpInt32.TTbyte[0] 			= *(p + 5);
					TmpInt32.TTbyte[1] 			= *(p + 6);
					TmpInt32.TTbyte[2] 			= *(p + 7);
					TmpInt32.TTbyte[3] 			= *(p + 8);
					CanDataMaize.EngineWorkTime = TmpInt32.LongLL;    		//����������ʱ��   

					TmpInt16.TTbyte[0] 			= *(p + 9);
					TmpInt16.TTbyte[1] 			= *(p + 10);
					CanDataMaize.SysVol 		= TmpInt16.IntII;     		//ϵͳ��ѹ

					TmpInt32.TTbyte[0] 			= *(p + 11);
					TmpInt32.TTbyte[1] 			= *(p + 12);
					TmpInt32.TTbyte[2] 			= *(p + 13);
					TmpInt32.TTbyte[3] 			= *(p + 14);
					CanDataMaize.TotalTravlled 	= TmpInt32.LongLL;     		//����ʻ���

					TmpInt16.TTbyte[0] 			= *(p + 15);
					TmpInt16.TTbyte[1] 			= *(p + 16);
					CanDataMaize.Rotate1 		= TmpInt16.IntII;       	//��Ƥ��ת��

					TmpInt16.TTbyte[0] 			= *(p + 17);
					TmpInt16.TTbyte[1] 			= *(p + 18);
					CanDataMaize.Rotate2 		= TmpInt16.IntII;       	//������ת��

					TmpInt16.TTbyte[0] 			= *(p + 19);
					TmpInt16.TTbyte[1] 			= *(p + 20);
					CanDataMaize.TravlledSpeed 	= TmpInt16.IntII;     		//��ʻ�ٶ�  
					
					TmpInt16.TTbyte[0] 			= *(p + 21);
					TmpInt16.TTbyte[1] 			= *(p + 22);
					CanDataMaize.ReapHigh 		= TmpInt16.IntII;    		//���߶�
					
					CanDataMaize.WarnValue 		= *(p + 23);         		//���������������״̬
					break;
				case 0x09:			//ˮ���� 
					p = TerRecvBuf + 12;

					TmpInt16.TTbyte[0] 			= *(p);
					TmpInt16.TTbyte[1] 			= *(p + 1);

					if(TmpInt16.IntII == 64127)
						ClearBindData();
					else
						CanDataPaddy.EngineRotate 	= TmpInt16.IntII;      		//������ת��
					
					CanDataPaddy.WaterTemp 		= *(p + 2);    				//��ȴˮ�¶�

					TmpInt16.TTbyte[0] 			= *(p + 3);
					TmpInt16.TTbyte[1] 			= *(p + 4);
					CanDataPaddy.Rotate1 			= TmpInt16.IntII;       //����ת��

					TmpInt16.TTbyte[0] 			= *(p + 5);
					TmpInt16.TTbyte[1] 			= *(p + 6);
					CanDataPaddy.Rotate2 		= TmpInt16.IntII;        	//������ת��

					TmpInt16.TTbyte[0] 			= *(p + 7);
					TmpInt16.TTbyte[1] 			= *(p + 8);
					CanDataPaddy.SysVol 		= TmpInt16.IntII;           //ϵͳ��ѹ

					TmpInt32.TTbyte[0] 			= *(p + 9);
					TmpInt32.TTbyte[1] 			= *(p + 10);
					TmpInt32.TTbyte[2] 			= *(p + 11);
					TmpInt32.TTbyte[3] 			= *(p + 12);
					CanDataPaddy.EngineWorkTime 	= TmpInt32.LongLL;    	//����������ʱ��  
					
					CanDataPaddy.WarnValue 		= *(p + 13);         		//��������

					TmpInt16.TTbyte[0] 			= *(p + 14);
					TmpInt16.TTbyte[1] 			= *(p + 15);
					CanDataPaddy.TravlledSpeed 	= TmpInt16.IntII;     		//��ʻ�ٶ�  
					
					TmpInt32.TTbyte[0] 			= *(p + 16);
					TmpInt32.TTbyte[1] 			= *(p + 17);
					TmpInt32.TTbyte[2] 			= *(p + 18);
					TmpInt32.TTbyte[3] 			= *(p + 19);					
					CanDataPaddy.TotalTravlled	= TmpInt32.LongLL;     		//����ʻ���	
					break;
				case 0x03:          //������  
					p = TerRecvBuf + 12;

					TmpInt16.TTbyte[0] 			= *(p);
					TmpInt16.TTbyte[1] 			= *(p + 1);

					if(TmpInt16.IntII == 64127)
						ClearBindData();
					else
						CanDataTractor.EngineRotate = TmpInt16.IntII;			//������ת��  
					
					CanDataTractor.WaterTemp 	= *(p + 2); 				//��ȴˮ�¶�
					CanDataTractor.EngineOil 	= *(p + 3); 				//����ѹ��
					CanDataTractor.FuelPercent 	= *(p + 4);					//ȼ�ϰٷֱ�

					TmpInt32.TTbyte[0] 			= *(p + 5);
					TmpInt32.TTbyte[1] 			= *(p + 6);
					TmpInt32.TTbyte[2] 			= *(p + 7);
					TmpInt32.TTbyte[3] 			= *(p + 8);
					CanDataTractor.EngineWorkTime = TmpInt32.LongLL;		//����������ʱ�� 

					TmpInt16.TTbyte[0] 			= *(p + 9);
					TmpInt16.TTbyte[1] 			= *(p + 10);
					CanDataTractor.SysVol 		= TmpInt16.IntII;			//ϵͳ��ѹ

					TmpInt16.TTbyte[0] 			= *(p + 11);
					TmpInt16.TTbyte[1] 			= *(p + 12);
					CanDataTractor.TravlledSpeed 	= TmpInt16.IntII; 		//��ʻ�ٶ�

					TmpInt32.TTbyte[0] 			= *(p + 13);
					TmpInt32.TTbyte[1] 			= *(p + 14);
					TmpInt32.TTbyte[2] 			= *(p + 15);
					TmpInt32.TTbyte[3] 			= *(p + 16);
					CanDataTractor.TotalTravlled 	= TmpInt32.LongLL; 		//����ʻ���
					
					CanDataTractor.WarnValue 		= *(p + 17); 			//��������
					break;   
				default:
					
					break;
			}
			
			break;
		case 0x0101:
			GetTerTimeBig = 1;
			p = TerRecvBuf + 8;
			//SL_Print(">App Recv TerTime \r\n");
			TerTime.Year 		= *(p);                 //
			TerTime.Mon 		= *(p + 1);
			TerTime.MDay 		= *(p + 2);
			TerTime.Hour 		= *(p + 3);
			TerTime.Min 		= *(p + 4);
			TerTime.Sec 		= *(p + 5);
			TerTime.VerifyBit 	= 1;
			TerTime.TimeSec     = CalendarToUnix(TerTime);    //�����ʱ����  
			//SL_Print(">App:The TerTime:%d\r\n",TerTime.TimeSec);
			break;
		case 0x00FE:
			//SL_MEMBLOCK(TerRecvBuf,resize,16);
			p = TerRecvBuf + 6;

			TmpInt16.TTbyte[0] = *p;
			TmpInt16.TTbyte[1] = *(p + 1);
			DataLen = TmpInt16.IntII;
			p = TerRecvBuf + 8;
			if(LookForStr(p,"AT+Test",sizeof("AT+Test")) >= 0)         			//������������
			{
				pSysCfg->CfgFlag = 0x56;
				SL_Print("AT+Test\r\n");
				break;
			}
			if(LookForStr(p,"HOMER3ETESTOVER!",sizeof("HOMER3ETESTOVER!")) >= 0)  		//�������ã��ص�����ģʽ
			{				  
				pSysCfg->CfgFlag = 0x55;
				SaveSysCfg();                                        			//��������  
				pTcpIp = GetTcpIpSocketSpce();
				pTcpIp->ResetLink = 1;                               			//��������
				SL_Print("BDWTESTOVER!\r\n");
				break;
			}
			if((i = LookForStr(p,"BDWMODIF",sizeof("BDWMODIF"))) >= 0)   		//����
			{
				p =p + i + 8;
				
				if(*(p) != ':')
					break;
				p += 1;
				for(i = 0;i < 16;i++)                                  			//ȥ�豸��ַ 
					pSysCfg->TerminalID[i] = *(p + i);
				
				pSysCfg->TerminalID[i] = '\0';
				p += 16;
				if(*p != ',')
					break;
				
				tmp = GetDataStr(1,2,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					SL_Memcpy(pSysCfg->ServerAddr,TmpStr,tmp + 1);          	//��������ַ  
					
				}
				
				tmp = GetDataStr(2,3,p,TmpStr,DataLen); 
				if(tmp > 0)
				{	
					TmpStr[tmp] = '\0';
					pSysCfg->ServerPort = StrToDouble(TmpStr);         			//�������˿�
				}
				
				tmp = GetDataStr(3,4,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->SleepTime = StrToDouble(TmpStr);          			//����ʱ��
				}
				
				tmp = GetDataStr(4,5,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->DistanceUpLoad = StrToDouble(TmpStr);    			//��λ��λ
				}


				tmp = GetDataStr(5,6,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->TravelUpLoadTime = StrToDouble(TmpStr);   			//��ʱ��λʱ��
				}

				tmp = GetDataStr(6,7,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->HardWareNum = StrToDouble(TmpStr);      			//Ӳ���汾��
				}
				
				tmp = GetDataStr(7,8,p,TmpStr,DataLen);                   		 	//CanЭ��汾��    
				//SL_Print("pSysCfg->CanProtocolNum:%d\r\n",tmp);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->CanProtocolNum = StrToDouble(TmpStr);   			//CanЭ��汾��  
					//SL_Print("pSysCfg->CanProtocolNum:%d\r\n",pSysCfg->CanProtocolNum);
					if(pSysCfg->CanProtocolNum == 0x01)                        //���׻�
						pSysCfg->CarType = 0x0A; 
					else if(pSysCfg->CanProtocolNum == 0x02)                   //С���  �¹���ˮ����
						pSysCfg->CarType = 0x08;
					else if(pSysCfg->CanProtocolNum == 0x03)                   //ˮ�����¹���
						pSysCfg->CarType = 0x09;
					else if(pSysCfg->CanProtocolNum == 0x04)                   //������
						pSysCfg->CarType = 0x03;    
					else if(pSysCfg->CanProtocolNum == 0x06)                   //�Ϲ��� С���
						pSysCfg->CarType = 0x0E;
				}

				tmp = GetDataStr(8,9,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					SL_Memcpy(pSysCfg->TerApn,TmpStr,tmp);           			//Apn ����㷽ʽ
					pSysCfg->TerApn[tmp] = '\0';
				}
			}
			
			break;
		default:
			
			return 0;
					
	}
	return 1;
}


/**********************************************************
**	��������:
**	��������:������STM32��������  ��������
**********************************************************/

void ProTerminalData(void)
{
	static unsigned char 		step = 0,step1 = 0,step2 = 0;
	static struct stopwatch16 	sw,sw1;

	SysTimeStr					Time;
	unsigned short int 			len;
	int 						tmp;
	SysConfigStr 				*pSys;                 //����
	IntToChar					TmpInt16;              //��ʱ���� 
	LongToChar					TmpInt32; 			   //��ʱ����
	unsigned char 				TmpStr[12];
	GNSSSTR 					*pGnss;                //��λ��Ϣ
	SysTimeStr					*pTime;                //ʱ��
	TcpIPSocketStr				*pTcpIp;

		
	pTime = GetSetTimeSpace();
	
	switch(RunMode)
	{
		case 0:
			switch(step)
			{
				case 0:                             //����CANЭ���  
					SetTerCanProNum();              //����CANЭ���   
					CanProNumSendBig = 0;
					init_stopwatch16(&sw);  
					step++;
					break;
				case 1:
					if(CanProNumSendBig == 0 && read_stopwatch16_value(&sw) < 20)      //��ʱһ��ʱ�䣬���� 
						break;
					step++;
					break;
				case 2:
					init_stopwatch16(&sw); 
					step++;
					break;
				case 3:
					if(read_stopwatch16_value(&sw) < 5)                       	     //��ʱ250
						break;
					SL_UartSendData(SL_UART_1,GetStatus,sizeof(GetStatus));    		//��ȡ�豸״̬��Ϣ ��Ҫ��ACC
					GetStatusBig = 0;
					init_stopwatch16(&sw); 
					step++;
					break;
				case 4:
					if(GetStatusBig == 0 && read_stopwatch16_value(&sw) < 20)   	//��ʱ 500ms
						break;
					step++;                                                         
					init_stopwatch16(&sw); 
					break;
				case 5:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					SL_UartSendData(SL_UART_1,GetCAN,sizeof(GetCAN));           	//��CAN��Ϣ
					 
					bitclr(GetCanBig,0);       							        	//
					init_stopwatch16(&sw);
					step++;
				case 6:
					if((GetCanBig & 0x01) == 0 && read_stopwatch16_value(&sw) < 20)    //�ȴ�  500
						break;
					if((GetCanBig & 0x01) == 0)
						bitset(GetCanBig,1);  
					else
						bitclr(GetCanBig,1);
					step++;
					init_stopwatch16(&sw);
					break;
				case 7:                                                            //��ȡ�豸ʱ��
					if(read_stopwatch16_value(&sw) < 5)             			   //��ʱ250ms��
						break;
					SL_UartSendData(SL_UART_1,GetTerTime,sizeof(GetTerTime));      //���豸ʱ��
					GetTerTimeBig = 0;
					init_stopwatch16(&sw);
					step++;
					break;
				case 8:
					if(GetTerTimeBig == 0 && read_stopwatch16_value(&sw) < 20)
						break;
					init_stopwatch16(&sw);
					
					if(pTime->VerifyBit > 0)
					{
						step = 10;
						pTime->VerifyBit = 0;
						break;
					}
					pSys = GetSysCfgSpace();          //��ȡ����
					if(pSys->CfgFlag == 0x55)
					{
						step = 2;                     //
						break;
					}
					step++;
					break;
				case 9:                               //����������Ϣ
					if(read_stopwatch16_value(&sw) < 10)
						break;
					pSys = GetSysCfgSpace();
					len = 0;
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0;//���ȣ��ܳ���-4
					TerSendBuf[len++] = 0;
					TerSendBuf[len++] = 0xFE;
					TerSendBuf[len++] = 0;
					TerSendBuf[len++] = 0;//���ݳ��ȣ��ܳ���-8
					TerSendBuf[len++] = 0;
					TerSendBuf[len++] = 'h';
					TerSendBuf[len++] = 'o';
					TerSendBuf[len++] = 'm';
					TerSendBuf[len++] = 'e';
					TerSendBuf[len++] = 'r';
					TerSendBuf[len++] = '3';
					TerSendBuf[len++] = 'x';
					TerSendBuf[len++] = ':';
					
					for(tmp = 0;tmp < 16;tmp++)
						TerSendBuf[len++] = pSys->TerminalID[tmp];   			//�豸��
					TerSendBuf[len] = ',';                               		//����1
					len += 1;
					pTcpIp = GetTcpIpSocketSpce();
					tmp = SL_Strlen(pTcpIp->SimIccId);
					SL_Memcpy(TerSendBuf + len,pTcpIp->SimIccId,tmp);  			//�豸ICC-ID��
					len += tmp;
					TerSendBuf[len] = ',';                                		//����2
					len += 1;
					
					tmp =SL_Strlen(pSys->ServerAddr);
					SL_Memcpy(TerSendBuf + len,pSys->ServerAddr,tmp);     //��������ַ
					len += tmp;
					TerSendBuf[len] = ',';                                //����3
					len += 1;
					SL_itoa(pSys->ServerPort,TmpStr,10);
					for(tmp = 0;tmp < SL_Strlen(TmpStr);tmp++)            //�������˿�
						TerSendBuf[len++] = TmpStr[tmp];
					TerSendBuf[len++] = ',';

					tmp = SL_Strlen(pSys->TerApn);
					SL_Memcpy(TerSendBuf + len,pSys->TerApn,tmp);        // APNGprs�����  
					len += tmp;
					TerSendBuf[len++] = ',';                             //����3
					
					pGnss = GetGnssDataSpace();
					if(pGnss->status == 'A')                             //��λ��ʶ
					{
						TerSendBuf[len] = '8';
						TerSendBuf[len + 1] = '0';
					}
					else
					{
						TerSendBuf[len] = '0';
						TerSendBuf[len + 1] = '0';
					}
					len += 2;
					
					tmp = pGnss->latitude * 1000000;                   //ά��
					SL_Memset(TmpStr,0x30,12);
					UInt32ToStr(tmp,TmpStr,12);
					SL_Memcpy(TerSendBuf + len,TmpStr,9);              //���Ƶ����ͻ�����
					len += 9;
					tmp = pGnss->longitude * 1000000;                  //����
					SL_Memset(TmpStr,0x30,12);
					UInt32ToStr(tmp,TmpStr,12);                        
					SL_Memcpy(TerSendBuf + len,TmpStr,9);              //���Ƶ����ͻ�����
					len += 9;

					SL_Memcpy(TerSendBuf + len,"00000000",8);         //�Խ�8��0
					len += 8;

					Time =  UnixToCalendar(pGnss->gnss_time - 28800);
					CharToStr(Time.Year,TmpStr,2);                  //GPSʱ��-��
					SL_Memcpy(TerSendBuf + len,TmpStr,2);
					len += 2;

					
					CharToStr(Time.Mon,TmpStr,2);                  //GPSʱ��-��
					SL_Memcpy(TerSendBuf + len,TmpStr,2);
					len += 2;

					CharToStr(Time.MDay,TmpStr,2);                  //GPSʱ��-��
					SL_Memcpy(TerSendBuf + len,TmpStr,2);
					len += 2;

					CharToStr(Time.Hour,TmpStr,2);                  //GPSʱ��-ʱ
					SL_Memcpy(TerSendBuf + len,TmpStr,2);
					len += 2;

					CharToStr(Time.Min,TmpStr,2);                  //GPSʱ��-��
					SL_Memcpy(TerSendBuf + len,TmpStr,2);
					len += 2;

					CharToStr(Time.Sec,TmpStr,2);                  //GPSʱ��-��
					SL_Memcpy(TerSendBuf + len,TmpStr,2);
					len += 2;
					TerSendBuf[len++] = ',';
					
					CharToStr(pGnss->satellite_num,TmpStr,2);       //������������
					SL_Memcpy(TerSendBuf + len,TmpStr,2);
					len += 2;
					TerSendBuf[len++] = ',';

					CharToStr(pTcpIp->GsmSign,TmpStr,2);
					SL_Memcpy(TerSendBuf + len,TmpStr,2);           //�ź�ֵ
					len += 2;
					TerSendBuf[len++] = ',';
					
					SL_Memset(TmpStr,0x30,12);
					tmp = UInt32ToStr(TerStateData.PowVol,TmpStr,12);
					SL_Memcpy(TerSendBuf + len,TmpStr,tmp);           //��Դ��ѹ
					len += tmp;
					TerSendBuf[len++] = ',';
					
					SL_Memset(TmpStr,0x30,12);
					tmp = UInt32ToStr(TerStateData.Battery,TmpStr,12);
					SL_Memcpy(TerSendBuf + len,TmpStr,tmp);           //��ص�ѹ 
					len += tmp;
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = TerStateData.AccState + 0x30;   //ACC ״̬
					TerSendBuf[len++] = ',';
						
					TerSendBuf[len++] = pTcpIp->LinkState + 0x30;      //GSM����״̬  0:δע��;1:��ע��;2:�����ӡ�
					TerSendBuf[len++] = ',';
						
					TerSendBuf[len++] = '2';                           //�з�������ڲ���Ϣ                
					TerSendBuf[len++] = '6';
					TerSendBuf[len++] = ',';

					SL_Memset(TmpStr,0x30,12);
					tmp = UInt32ToStr(pSys->SleepTime,TmpStr,12);      //˯��ʱ��
					SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
					len += tmp;
					TerSendBuf[len++] = ',';

					SL_Memset(TmpStr,0x30,12);
					tmp = UInt32ToStr(pSys->DistanceUpLoad,TmpStr,12);      //��λ�þ���
					SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
					len += tmp;
					TerSendBuf[len++] = ',';

					SL_Memset(TmpStr,0x30,12);
					tmp = UInt32ToStr(pSys->TravelUpLoadTime,TmpStr,12);      //ʱ�䱨λʱ��
					SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
					len += tmp;
					TerSendBuf[len++] = ',';

					SL_Memset(TmpStr,0x30,12);
					tmp = UInt32ToStr(pSys->HardWareNum,TmpStr,12);      //ʱ�䱨λʱ��
					SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
					len += tmp;
					TerSendBuf[len++] = ',';
					TerStateData.SdCardState = 1;  						// Ĭ��״̬SD״̬1
					TerSendBuf[len++] = TerStateData.SdCardState + 0x30;    //SD��״̬  1:����������0:�����쳣
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = TerStateData.E2PROMState + 0x30;	//EEPROM״̬ 1:��������
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = '1';								// �ⲿRTC״̬��1:��������        (ĿǰдΪ1)
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = TerStateData.AntConState + 0x30;   //����״̬  0:����
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = TerStateData.ShellState + 0x30; 	//����״̬
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = '1';                                //Key1״̬
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = '1';								//Key2״̬
					TerSendBuf[len++] = ',';

					tmp = GetVerNum();
					UInt32ToStr(tmp,TmpStr,12);      						//CanЭ��汾��
					SL_Memcpy(TerSendBuf + len,TmpStr,2);
					len += 2;
					tmp = sizeof("010101") - 1;                            //
					SL_Memcpy(TerSendBuf + len,"010101",tmp);
					len += tmp;
					TerSendBuf[len++] = ',';

					tmp = UInt32ToStr(pSys->CanProtocolNum,TmpStr,12);      //CanЭ��汾��
					SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
					len += tmp;
					TerSendBuf[len++] = ',';

					//�ⲿCAN����״̬
					if(TerStateData.CanStatus == 0)
						TerSendBuf[len++] = '0';                                //
					else
						TerSendBuf[len++] = '1';                                //
					
					TerSendBuf[len++] = ',';
						
					TerSendBuf[len++] = 0x0D;                               	//�س�
					TerSendBuf[len++] = 0x0A;
					
					TerSendBuf[len] = 0x00;
					TerSendBuf[len + 1] = 0x00;
					TerSendBuf[len + 2] = 0x55;
					TerSendBuf[len + 3] = 0xAA;
					len += 4;
					TmpInt16.IntII = len -6;                 				//�ܳ���
					TerSendBuf[2] = TmpInt16.TTbyte[0];
					TerSendBuf[3] = TmpInt16.TTbyte[1];
					TmpInt16.IntII	= len - 12;             				//���ݳ���
					TerSendBuf[6]	=TmpInt16.TTbyte[0];
					TerSendBuf[7]	=TmpInt16.TTbyte[1];
					
					SL_UartSendData(SL_UART_1,TerSendBuf,len);      		//
					
					step = 2;   											//
					init_stopwatch16(&sw);
					break;
				case 10:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					step = 2;
					SendVerifyTimeCmd(pTime);              //����У׼ʱ��  
					SL_Print("Verfy STM32 Time:%d\r\n",pTime->VerifyBit);
					init_stopwatch16(&sw);
					break;
					
				default:
					step = 0;
					break;
			}
			break;
		case 1:                                       //����STM32 ��Ƭ�� 
			switch(step1)
			{
				case 0:                                //������������  ��Ƭ����������ָ��    
					UpgradeData = (UpgradeFileStr *)SL_GetMemory(sizeof(UpgradeFileStr));
					if(UpgradeData == NULL)
					{
						SL_Print(">App:Get Sys Mem Fail......\r\n");       //����
						RunMode = 0;
						break;
					}
					
					pTcpIp = GetTcpIpSocketSpce();
					UpgradeData->FileTotalLen = pTcpIp->FtpFileLen;
					UpgradeData->FileVerfy = CheckSum16(pTcpIp->FtpFileSapce,pTcpIp->FtpFileLen);   //����У��  
					UpgradeData->FileTotalFram = UpgradeData->FileTotalLen / 1024;
					
					for(tmp = 0;tmp < UpgradeData->FileTotalFram;tmp++)
					{
						UpgradeData->FileLen[tmp] = 1024;       //���ݰ�����
						SL_Memcpy(UpgradeData->FileBuf[tmp],(pTcpIp->FtpFileSapce + 1024 * tmp),UpgradeData->FileLen[tmp]);  //
					}
					tmp = UpgradeData->FileTotalLen % 1024;
					
					if(tmp > 0)
					{
						UpgradeData->FileLen[UpgradeData->FileTotalFram] = tmp;
						SL_Memcpy(UpgradeData->FileBuf[UpgradeData->FileTotalFram],(pTcpIp->FtpFileSapce + 1024 * UpgradeData->FileTotalFram),UpgradeData->FileLen[UpgradeData->FileTotalFram]);
						UpgradeData->FileTotalFram++;
					}
					
					SL_Print(">App:Load Stm32 FirWare Finsh.....%d,%d,%d\r\n",UpgradeData->FileTotalLen,UpgradeData->FileTotalFram,UpgradeData->FileVerfy);     //���������Ϣ  
					SL_MEMBLOCK(UpgradeData->FileBuf[UpgradeData->FileTotalFram - 1],UpgradeData->FileLen[UpgradeData->FileTotalFram - 1],16);
					
					len = 0;
					SL_Memset(TerSendBuf,0,1200);          //
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0x0E;
					TerSendBuf[len++] = 0x00;            //����
					TerSendBuf[len++] = 0x1F;
					TerSendBuf[len++] = 0x00;
					TerSendBuf[len++] = 0x08;
					TerSendBuf[len++] = 0x00;           //
					TerSendBuf[len++] = 0x01;
					TerSendBuf[len++] = 0x00;           //�������򳤶� 
					TmpInt32.LongLL = UpgradeData->FileTotalLen;
					TerSendBuf[len++] = TmpInt32.TTbyte[0];
					TerSendBuf[len++] = TmpInt32.TTbyte[1];
					TerSendBuf[len++] = TmpInt32.TTbyte[2];
					TerSendBuf[len++] = TmpInt32.TTbyte[3];
					TmpInt16.IntII = UpgradeData->FileVerfy;
					TerSendBuf[len++] = TmpInt16.TTbyte[0];
					TerSendBuf[len++] = TmpInt16.TTbyte[1];
					TmpInt16.IntII = CheckSum16(TerSendBuf,len - 4);
					TerSendBuf[len++] = TmpInt16.TTbyte[0];
					TerSendBuf[len++] = TmpInt16.TTbyte[1];
					TerSendBuf[len++] = 0x55;
					TerSendBuf[len++] = 0xAA;
					SL_UartSendData(SL_UART_1,TerSendBuf,len);      //���ͳ�ȥ 
					init_stopwatch16(&sw1);
					step1++;
					break;
				case 1:                                                //��ʼ����  �����Ĺ�������Ҫ���  
					if(read_stopwatch16_value(&sw1) > 4800)        //1������Ҫ����������ȴ������
					{
						SL_FreeMemory(UpgradeData);
						RunMode = 0;         
						UpgradeStatus.UpgradeStatus = 1;     //����ʧ�� 
						//��������״̬ 
						return;
					}
					tmp = ReadComPkt(1,TerRecvBuf,sizeof(TerRecvBuf));    //
					if(tmp == 0)
						return ;
					if(TerRecvBuf[0] != 0x5F || TerRecvBuf[1] != 0x5F)                //�жϰ�ͷ
						return ;
					
					TmpInt16.TTbyte[0] = *(TerRecvBuf + 2);
					TmpInt16.TTbyte[1] = *(TerRecvBuf + 3);
					len = TmpInt16.IntII;
	
					if(*(TerRecvBuf + len + 4) != 0x55 || *(TerRecvBuf + len + 5) != 0xAA)   //�жϰ�β
						return;
					
					TmpInt16.TTbyte[0] = *(TerRecvBuf + len + 2);
					TmpInt16.TTbyte[1] = *(TerRecvBuf + len + 3);
					tmp = TmpInt16.IntII;
					if(tmp != CheckSum16(TerRecvBuf + 4,len - 2))
						return;
					
					TmpInt16.TTbyte[0] = TerRecvBuf[10];      //����֡����
					TmpInt16.TTbyte[1] = TerRecvBuf[11];
					/*
					if(TmpInt16.IntII >= UpgradeData->FileTotalFram)
					{	
						SL_FreeMemory(UpgradeData);
						RunMode = 0;         
						UpgradeStatus.UpgradeStatus = 1;     //����ʧ��
						//��������״̬ 
						return;
					}
					*/
					UpgradeData->FileCurrentFram = TmpInt16.IntII - 1;                  //��ǰ����֡����  
					SL_Print("UpgradeData->FileCurrentFram:%d\r\n",UpgradeData->FileCurrentFram);
					
					len = 0;
					SL_Memset(TerSendBuf,0,1200);    //�������
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0x5F;
					TmpInt16.IntII = UpgradeData->FileLen[UpgradeData->FileCurrentFram] + 10;
					TerSendBuf[len++] = TmpInt16.TTbyte[0];         //�ܳ���
					TerSendBuf[len++] =	TmpInt16.TTbyte[1];
					TerSendBuf[len++] = 0x1F;         //����������
					TerSendBuf[len++] = 0x00;
					TmpInt16.IntII    = UpgradeData->FileLen[UpgradeData->FileCurrentFram] + 6;   //���ȼ�
					TerSendBuf[len++] = TmpInt16.TTbyte[0];
					TerSendBuf[len++] = TmpInt16.TTbyte[1];       //�������򳤶�   
					
					TerSendBuf[len++] = 0x02;
					TerSendBuf[len++] = 0x00;
					TerSendBuf[len++] = 0x00;
					TerSendBuf[len++] = 0x00;
					
					SL_Memcpy(&TerSendBuf[len],UpgradeData->FileBuf[UpgradeData->FileCurrentFram],UpgradeData->FileLen[UpgradeData->FileCurrentFram]);
					
					len += UpgradeData->FileLen[UpgradeData->FileCurrentFram];
					SL_Print("UpgradeData->FileCurrentFram %d\r\n",UpgradeData->FileLen[UpgradeData->FileCurrentFram]);

					TmpInt16.IntII= CheckSum16(TerSendBuf + 12,len - 12);            //У�� 
					
					TerSendBuf[len++] = TmpInt16.TTbyte[0];
					TerSendBuf[len++] = TmpInt16.TTbyte[1];
					SL_Print("CheckSum16: %d\r\n",TmpInt16.IntII);
					TerSendBuf[len++] = 0x55;
					TerSendBuf[len++] = 0xAA;
					SL_UartSendData(SL_UART_1,TerSendBuf,len);      //
					SL_Print("Run This  %d\r\n",len);
					if(UpgradeData->FileCurrentFram >= UpgradeData->FileTotalFram)
					{
						SL_FreeMemory(UpgradeData);
						RunMode = 0;         
						UpgradeStatus.UpgradeStatus = 0;     //�������
						//��������״̬ 
						return;
						
					}
					break; 
				default:
					step1 = 0;
					break;
			}
			break;
		case 2:                                      //��Ƭ������ST86/ST87����
			switch(step2)
			{
				case 0:
					step2++;
					break;
				case 1:
					step2++;
					break;
				default:
					step2 = 0;
					break;
			}
			break;
	}
	
}




/**************************File End*************************/


