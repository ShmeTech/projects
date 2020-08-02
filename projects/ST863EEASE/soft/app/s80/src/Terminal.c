

/**************************************************
**	File Name:
**	Time:
**************************************************/
#include "include.h"


#define bitset(var, bitno) ((var) |= 1UL << (bitno))

#define bitclr(var, bitno) ((var) &= ~(1UL << (bitno)))


extern HANDLE SysAppEnterTask;						//


/************ STM32F�����ļ�������***************/

static UpgradeStatusStr 		UpgradeStatus;                //Զ��������������

static UpgradeFileStr			*UpgradeData;                 //          

static UpgradeST86FileStr		*pUpgradeST86;			 	//ָ��Ҫ���������ݻ�����  

unsigned char 					*pUpgradeST86File;

/*****************���ݻ�����**********************/

static unsigned char 			TerRecvBuf[1024];           //���ջ�����

static unsigned char 			TerSendBuf[1200];			//���ͻ�����

static unsigned char 			RunMode;   					//0:�������У�1��������Ƭ������2����Ƭ����������(ST87����)

static unsigned char 			McuStatus;               	//��Ƭ������״̬

static unsigned char			ConfigFlag;					//����ģʽ��־

/*******************����ȫ�ֱ���********************/

static CanDataStr		    	CanData;

static CurrentFaultCodeStr		CurrentFaultCode;      //Pƽ̨��ǰ������   ������ ���ֻ���ͨ��

static TerStateStr				TerStateData;           //�豸�ն�״̬

//static TerSDCardStr				TerSDCardData;          //�豸SD��״̬��

static SysTimeStr 				TerTime;                //�豸ʱ��  ��Ƭ��ʱ��



/*********************����ȫ�ֱ���*************************/

static unsigned char 			CanProNumSendBig 	= 0;         //CANЭ��ŷ��ͱ�ʶ

static unsigned char 			GetStatusBig 		= 0;         //���豸״̬����  


unsigned char GetStatus[12]    				= {0x5F,0x5F,0x06,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};


/*************************************************
**	��������:unsigned char ReadAccState(void)
**	��������:����ACC״̬
**************************************************/

unsigned char ReadAccState(void)
{
	return TerStateData.AccState;
}

/**************************************************
**	��������:
**	��������:
***************************************************/
CanDataStr *GetCanDataSpace(void)
{
	return &CanData;
}

/****************************************************
**	��������:TerStateStr	*GetTerStateDataSpace(void)
**	��������:
****************************************************/

TerStateStr	*GetTerStateDataSpace(void)
{
	return &TerStateData;
}


/********************************************************
**	��������:
**	��������:
********************************************************/

CurrentFaultCodeStr *GetCurrentFaultCodeSpace(void)
{
	return &CurrentFaultCode;
}



/******************************************************
**	��������:SysTimeStr *GetTerTimeSpace(void)
**	��������:�����豸ʱ��ռ�
******************************************************/
SysTimeStr *GetTerTimeSpace(void)
{
	return &TerTime;
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
**	��������:
**	��������:
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
	SysConfigStr   			*p;
	unsigned short 			tmp;
	int 					i;
	LongToChar     			TmpData;
	IntToChar				Tmp;
	
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
	SL_UartSendData(SL_UART_2,TerSendBuf,16);
}

/****************************************************
**	��������:void SendVerifyTimeCmd(SysTimeStr *pTime)
**	��������:����ʱ��У����Ϣ��MCU
****************************************************/

void SendVerifyTimeCmd(SysTimeStr *pTime)
{
	LongToChar     TmpData;
	//IntToChar		Tmp;

	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	TerSendBuf[2] = 0x0A;
	TerSendBuf[3] = 0x00;
	TerSendBuf[4] = 0x20;
	TerSendBuf[5] = 0x00;
	TerSendBuf[6] = 0x04;
	TerSendBuf[7] = 0x00;
	TmpData.LongLL = pTime->TimeSec;
	TerSendBuf[8] = TmpData.TTbyte[0];
	TerSendBuf[9] = TmpData.TTbyte[1];           
	TerSendBuf[10] = TmpData.TTbyte[2];         
	TerSendBuf[11] = TmpData.TTbyte[3];        
	*(unsigned short int *)&TerSendBuf[12] = CheckSum16(TerSendBuf + 4,7);
	TerSendBuf[14] = 0x55;
	TerSendBuf[15] = 0xAA;

	SL_UartSendData(SL_UART_2, TerSendBuf,16);
}

/********************************************************
** 	��������:void SetTerCanProNum(void)
** 	��������:����CanЭ��汾�ŵ�MCU
**	���¶��幦��,���ͻ�����Ϣ����Ƭ��
**	�豸�š�CANЭ��š�����ʱ�䡣Ŀǰ�ȶ���������
**	
********************************************************/

void SetTerCanProNum(void)
{
	LongToChar     	TmpData;
	SysConfigStr 	*pSysCfg;
	
	unsigned char i,len;

	len = 0;
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0x10;
	TerSendBuf[len++] = 0x00;
	TerSendBuf[len++] = 0x14;
	TerSendBuf[len++] = 0x00;
	TerSendBuf[len++] = 0x0A;
	TerSendBuf[len++] = 0x00;
	pSysCfg = GetSysCfgSpace();
	TerSendBuf[len++] = pSysCfg->CanProtocolNum;    //CANЭ���  ����2�ֽ�
	TerSendBuf[len++] = 0xF0;

	TmpData.LongLL = pSysCfg->SleepTime;       		//����ʱ��  ����4�ֽ�
	TerSendBuf[len++] = TmpData.TTbyte[0];			
	TerSendBuf[len++] = TmpData.TTbyte[1];			
	TerSendBuf[len++] = TmpData.TTbyte[2];			
	TerSendBuf[len++] = TmpData.TTbyte[3];			

	for(i = 0;i < 18;i++)
		TerSendBuf[len++] = pSysCfg->TerminalID[i];     //�豸��
	
	*(unsigned short int *)&TerSendBuf[len] = CheckSum16(TerSendBuf + 4,len - 4);   //У��
	len += 2;
	TerSendBuf[len++] = 0x55;
	TerSendBuf[len++] = 0xAA;
	//SL_Print("Send Set Ter Pro Num.......\r\n");
	SL_UartSendData(SL_UART_2, TerSendBuf,len);    		//����

}



/******************************************************
**	��������:
**	��������:
**	����˵��:
********************************************************/
void GetBinFile(unsigned short int n,unsigned short int fn)
{
    IntToChar       		TmpInt16;
	unsigned short int 		len = 0;
	
   	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0;//���ȣ��ܳ���-4
	TerSendBuf[len++] = 0;
	TerSendBuf[len++] = 0x3F;
	TerSendBuf[len++] = 0;
	TerSendBuf[len++] = 0;//���ݳ��ȣ��ܳ���-8
	TerSendBuf[len++] = 0;

	TmpInt16.IntII = n;                     //����   2,����
    TerSendBuf[len++] = TmpInt16.TTbyte[0];
	TerSendBuf[len++] = TmpInt16.TTbyte[1];
	TmpInt16.IntII =fn;         
    TerSendBuf[len++] = TmpInt16.TTbyte[0];
	TerSendBuf[len++] = TmpInt16.TTbyte[1];
	
	TerSendBuf[len++] = 0;//У��
	TerSendBuf[len++] = 0;				//У��
	TerSendBuf[len++] = 0x55;
	TerSendBuf[len++] = 0xAA;

	TmpInt16.IntII = len - 6;								//�ܳ���
	TerSendBuf[2] = TmpInt16.TTbyte[0];
	TerSendBuf[3] = TmpInt16.TTbyte[1];
	TmpInt16.IntII	= len - 12; 							//���ݳ���
	TerSendBuf[6]	=TmpInt16.TTbyte[0];
	TerSendBuf[7]	=TmpInt16.TTbyte[1];
   	//SL_MEMBLOCK(TerSendBuf,len,16);
    SL_UartSendData(SL_UART_2, TerSendBuf,len);  
}


/********************************************************
**	��������:static void UpgradeST86Mode(void)
**	��������:����ST86ģ��Ӧ�ó���
**	ͨ����Ƭ������ST86����
********************************************************/

static unsigned char UpgradeST86Mode(unsigned char *p,unsigned short int len)
{
	IntToChar       		TmpInt16;
	LongToChar				TmpInt32;
	unsigned short int      FunID;
	int 					i;
					
	TmpInt16.TTbyte[0] = *(p + 0);
	TmpInt16.TTbyte[1] = *(p + 1);
	len = TmpInt16.IntII;               				//�������򳤶�
	
	TmpInt16.TTbyte[0] = *(p + 2);
	TmpInt16.TTbyte[1] = *(p + 3);        
	FunID = TmpInt16.IntII;
	//SL_Print("Enter ST86 Upgrad Mode:  %d\r\n",FunID);
	//FunID = 4;
	switch(FunID)
	{
		case 1:
			pUpgradeST86 = (UpgradeST86FileStr *)SL_GetMemory(sizeof(UpgradeST86FileStr));
			if(pUpgradeST86 == NULL)
			{
				SL_Print(">App:Get UpgradeST86FileStr Mem Fail......\r\n");       //����
				RunMode = 0;
				return 0;
			}
			
			pUpgradeST86File = (unsigned char *)GetSendQueueSpace();
			SL_Memset(pUpgradeST86,0,sizeof(UpgradeST86FileStr));    //������ൺ�����ݿռ�  
			SL_Memset(pUpgradeST86File,0,sizeof(SendQueueStr));    //������ൺ�����ݿռ�  
			TmpInt32.TTbyte[0] = *(p + 4); 
			TmpInt32.TTbyte[1] = *(p + 5); 
			TmpInt32.TTbyte[2] = *(p + 6); 
			TmpInt32.TTbyte[3] = *(p + 7); 
			pUpgradeST86->FileLen = TmpInt32.LongLL;		//�����ļ��ܳ��� 

			TmpInt16.TTbyte[0] = *(p + 8); 
			TmpInt16.TTbyte[1] = *(p + 9); 
			
			pUpgradeST86->FileVerfy = TmpInt16.IntII;    //�����ļ�У��ֵ 
			pUpgradeST86->CurrentFramNum++;  
		
			RunMode = 2;
			return 1;
		case 2:
			len -= 6;

			for(i = 0;i < len;i++)
			{
				*(pUpgradeST86File + pUpgradeST86->CurrentLen)= *(p + i + 6);
				pUpgradeST86->CurrentLen++;
			}	
			
			SL_Print("Recv Data:%d,%d,%d\r\n",len,pUpgradeST86->CurrentLen,pUpgradeST86->CurrentFramNum);
			pUpgradeST86->CurrentFramNum++;
			if(pUpgradeST86->CurrentLen >= pUpgradeST86->FileLen)
			{
				TmpInt16.IntII = CheckSum16(pUpgradeST86File,pUpgradeST86->FileLen);
				SL_Print("Recv Data Finsh.....%d,%d\r\n",TmpInt16.IntII,pUpgradeST86->FileVerfy);
				
				if(TmpInt16.IntII == pUpgradeST86->FileVerfy)
				{	
	    			return 2;
				}
				else
				{
					SL_FreeMemory(pUpgradeST86);       //�ͷ��ڴ�
					RunMode = 0;
					return 0;
				}
			}
			
			return 1;
		default:
			RunMode = 0;
			return 0;
	}
	
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
	unsigned char 			TmpStr[40];
	int i;

	if(RunMode >= 1)           //
		return 0; 

	SL_Memset(TerRecvBuf,0,sizeof(TerRecvBuf));
	DataLen = ReadComPkt(2,TerRecvBuf,sizeof(TerRecvBuf));    //��ȡҪ���յ�������

	if(DataLen == 0)
		return 0;
	
	if(TerRecvBuf[0] != 0x5F || TerRecvBuf[1] != 0x5F)                //�жϰ�ͷ
		return 0;
	TmpInt16.TTbyte[0] = *(TerRecvBuf + 2);                    //���ݳ��ȣ�����
	TmpInt16.TTbyte[1] = *(TerRecvBuf + 3);                    //��Ϣ�����ݳ���
	tmp = TmpInt16.IntII;                                      //����
	
	if(*(TerRecvBuf + tmp + 4) != 0x55 || *(TerRecvBuf + tmp + 5) != 0xAA)   //�жϰ�β
		return 0;
	
	TmpInt16.TTbyte[0] = *(TerRecvBuf + tmp + 2);
	TmpInt16.TTbyte[1] = *(TerRecvBuf + tmp + 3);
	
	if(TmpInt16.IntII != CheckSum16(TerRecvBuf + 4,tmp - 2))
		return 0;
	
	pSysCfg = GetSysCfgSpace();
	TmpInt16.IntII = DataLen;
	DataLen = tmp;
	
	TmpInt16.TTbyte[0] = *(TerRecvBuf +  4);
	TmpInt16.TTbyte[1] = *(TerRecvBuf +  5);
	tmp = TmpInt16.IntII;    										//��ϢID  
	
	switch(tmp)
	{
		case 0x0014:                                   	 			//����CANͨѶЭ�飬
			CanProNumSendBig = 1;
			break;
		case 0x0001:                                     			//�����豸״̬  �豸�ⲿ״̬��Ϣͨ��
			p = TerRecvBuf + 8;
			//SL_MEMBLOCK(p,DataLen,16);							//����ʹ��
			SL_Memcpy(&TerStateData,p,sizeof(TerStateData));        //��Ƭ����Ϣ
			p += sizeof(TerStateData);
			McuStatus = *p;   						//��Ƭ��״̬  
			p += 1;
			TerTime.TimeSec = TerStateData.McuTime;   //��Ƭ��ʱ��
			TerTime.VerifyBit = 1;         			 //�Ѿ���ȡϵͳʱ��
			WriteSleepStatus(*p);     				 //˯��״̬    
			//SL_Print("The Mcu Status::%d\r\n",McuStatus);
			//һ��ΪCAN����
			SL_Memcpy(&CanData,p,sizeof(CanData));   				//����Can����
			p += sizeof(CanData);
			//SL_Print("The Mcu Status:%d,%d,%d,%d,",CanData.EngineRotate,CanData.WaterTemp,CanData.Rotate1,CanData.Rotate2);
			//SL_Print("%d,%d,%d,%d,%d,%d,\r\n",CanData.EngineWorkTime,CanData.WarnValue1,CanData.TravlledSpeed,CanData.TotalTravlled,CanData.OilConsumption,CanData.TatalOilConsumption);
			//SL_Print("%d,%d\r\n",CanData.EngineOil,CanData.FuelPercent);
			//һ�¿�ʼ��ȡ����������
			TmpInt16.TTbyte[0]			= *(p + 0);
			TmpInt16.TTbyte[1]			= *(p + 1);

			if(TmpInt16.IntII > 160)
				TmpInt16.IntII = 0;
			p += 2;
			for(i = 0;i < TmpInt16.IntII;i++)
			{
				CurrentFaultCode.FaultCode[i] = *(p + i);
			}
			CurrentFaultCode.FaultCodeNum = TmpInt16.IntII / 4;          //��ǰ����������
			//SL_Print("The Mcu Status::%d,%d,%d\r\n",TerStateData.McuFirNum,TerStateData.McuTime,TerStateData.CanConnect);
			break;
		case 0x00FE:                //�й����õ���Ϣ
			p = TerRecvBuf + 6;

			TmpInt16.TTbyte[0] = *p;
			TmpInt16.TTbyte[1] = *(p + 1);
			
			p = TerRecvBuf + 8;
			if(LookForStr(p,"AT+Test",DataLen - 6) >= 0)         			//������������
			{
				pSysCfg->CfgFlag = 0x56;
				//SL_Print("AT+Test\r\n");
				break;
			}
	
			if(LookForStr(p,"HOMER3ETESTOVER!",DataLen - 6) >= 0)  		//�������ã��ص�����ģʽ
			{				  
				pSysCfg->CfgFlag = 0x55;
				SaveSysCfg();                                        			//��������    
				pTcpIp = GetTcpIpSocketSpce();
				pTcpIp->ResetLink = 1;                               			//��������
				break;
			}

			SL_Memset(TmpStr,'\0',12);
			SL_Memcpy(TmpStr,p,9);
			
			if(SL_Strcmp(TmpStr,"IIAP_ST87") == 0)
			{
				UpgradeST86Mode(p + 9,DataLen - 15);             //���øú���
				break;
			}
			
			if((i = LookForStr(p,"HOMER3E:",DataLen - 6)) >= 0)   		//����
			{
				p = p + i + 7;
				//SL_Print("Run BDWMODIF:\r\n");
				if(*(p) != ':')
					break;
				p++;
				for(i = 0;i < 16;i++)                            //�豸��
					pSysCfg->TerminalID[i] = *(p + i);           //��ȡ�豸��       

				i = GetDataStr(1,2,p,TmpStr,DataLen);
				if(i > 0)
				{
					TmpStr[i] = '\0';
					SL_Memcpy(pSysCfg->ServerAddr,TmpStr,i + 1);          	//��������ַ  	
				}

				i = GetDataStr(2,3,p,TmpStr,DataLen); 
				//SL_Print(">App:The ServerPort %d\r\n",i);
				if(i > 0)
				{	
					TmpStr[i] = '\0';
					pSysCfg->ServerPort = StrToDouble(TmpStr);         			//�������˿�
				}
			
				i = GetDataStr(3,4,p,TmpStr,DataLen);
				//SL_Print(">App:The Sleep Time %d\r\n",i);
				if(i > 0)
				{
					TmpStr[i] = '\0';
					pSysCfg->SleepTime = StrToDouble(TmpStr);					//����ʱ��
				}
							
				i = GetDataStr(4,5,p,TmpStr,DataLen);
				//SL_Print(">App:The DisTanceUpLoad %d\r\n",i);
				if(i > 0)
				{
					TmpStr[i] = '\0';
					pSysCfg->DistanceUpLoad = StrToDouble(TmpStr);				//��λ��λ
				}
			
				i = GetDataStr(5,6,p,TmpStr,DataLen);
				//SL_Print(">App:The UpLoadTime %d\r\n",i);
				if(i > 0)
				{
					TmpStr[i] = '\0';
					pSysCfg->TravelUpLoadTime = StrToDouble(TmpStr);			//��ʱ��λʱ��
				}

				i = GetDataStr(6,7,p,TmpStr,DataLen);							//CanЭ��汾��    
			
				if(i > 0)
				{
					TmpStr[i] = '\0';
					pSysCfg->HardWareNum= StrToDouble(TmpStr);					//Ӳ���汾��
				}

				i = GetDataStr(7,8,p,TmpStr,DataLen);                   		 	//CanЭ��汾��    
				if(i > 0)
				{
					TmpStr[i] = '\0';
					pSysCfg->CanProtocolNum = StrToDouble(TmpStr);   			//CanЭ��汾��  
					if(pSysCfg->CanProtocolNum == 0x03)                    		//3E�豸 ˮ���� 
					{
						pSysCfg->CarType = 0x09;                                //ˮ����������0x09 
					}
					else if(pSysCfg->CanProtocolNum == 0x04)                    //3E�豸 �¹���Mƽ̨������
					{
						pSysCfg->CarType = 0x03;                       
					}
					else if(pSysCfg->CanProtocolNum == 0x07)					//3E�豸 �Ϲ��� Mƽ̨������
					{
						pSysCfg->CarType = 0x0D;		                        //3E�豸��
					}
					else if(pSysCfg->CanProtocolNum == 0x08)					//3E�豸 �Ϲ��� ˮ����
					{
						pSysCfg->CarType = 0x0F;	                            //����	
					}
					else
					{
						pSysCfg->CanProtocolNum = 0x03;                        //Ĭ�� �Ϲ���ˮ����
						pSysCfg->CarType = 0x09; 
					}
				}

				i = GetDataStr(8,9,p,TmpStr,DataLen);
				if(i > 0)
				{
					SL_Memcpy(pSysCfg->TerApn,TmpStr,i);           			//Apn ����㷽ʽ
					pSysCfg->TerApn[i] = '\0';
				}
			}
			break;
		default:
			return 0;
					
	}
	return 1;
}





/**************************************************
**	��������
**	��������:
***************************************************/
void SendTerminalInfo(unsigned short int id)
{
	SysConfigStr				*pSys;
	unsigned short int 			len;
	unsigned int 				tmp;
	TcpIPSocketStr 				*pTcpIp;
	IntToChar					TmpInt16;              //��ʱ���� 
	unsigned char 				TmpStr[12];
	GNSSSTR 					*pGnss;                //��λ��Ϣ
	SysTimeStr					Time;                //ʱ��
	
	pSys = GetSysCfgSpace();
	len = 0;
	pTcpIp = GetTcpIpSocketSpce();
	
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0;//���ȣ��ܳ���-4
	TerSendBuf[len++] = 0;
	TmpInt16.IntII = id;
	TerSendBuf[len++] = TmpInt16.TTbyte[0];
	TerSendBuf[len++] = TmpInt16.TTbyte[1];
	TerSendBuf[len++] = 0;						//���ݳ��ȣ��ܳ���-8
	TerSendBuf[len++] = 0;
	TerSendBuf[len++] = pSys->CfgFlag;    //���ñ�־  
	TerSendBuf[len++] = ConfigFlag;      //Ĭ��Ϊ0
					
	TerSendBuf[len++] = '\r';
	TerSendBuf[len++] = '\n';
	SL_Memcpy(TerSendBuf + len,"����GPRS��������",sizeof("����GPRS��������"));   //���������Ҫ������ȷ��һ��
	len += sizeof("����GPRS��������");
	TerSendBuf[len++] = 'h';
	TerSendBuf[len++] = 'o';
	TerSendBuf[len++] = 'm';
	TerSendBuf[len++] = 'e';
	TerSendBuf[len++] = 'r';
	TerSendBuf[len++] = '3';
	TerSendBuf[len++] = 'e';
	TerSendBuf[len++] = ':';

	//�豸��
	for(tmp = 0;tmp < 16;tmp++)
		TerSendBuf[len++] = pSys->TerminalID[tmp];  
	TerSendBuf[len++] = ',';
			
	//SIM��ICCID����
	for(tmp = 0;tmp < 20;tmp++)
		TerSendBuf[len++] = pTcpIp->SimIccId[tmp]; 
	TerSendBuf[len++] = ',';
			
	//��������ַ:
	TmpInt16.IntII = SL_Strlen(pSys->ServerAddr);
	SL_Memcpy(TerSendBuf + len,pSys->ServerAddr,TmpInt16.IntII);
	len += TmpInt16.IntII;
	TerSendBuf[len++] = ',';
	
	//�������˿�
	SL_itoa(pSys->ServerPort,TmpStr,10);
	for(tmp = 0;tmp < SL_Strlen(TmpStr);tmp++)			  //�������˿�
		TerSendBuf[len++] = TmpStr[tmp];
	TerSendBuf[len++] = ',';

	//ANP�����
	tmp = SL_Strlen(pSys->TerApn);
	SL_Memcpy(TerSendBuf + len,pSys->TerApn,tmp);        // APNGprs�����  
	len += tmp;
	TerSendBuf[len++] = ',';                             //����3

	//��λ����  ��λ״̬
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
			
	//��λά��
	tmp = pGnss->latitude * 1000000;                   //ά��
	SL_Memset(TmpStr,0x30,12);
	UInt32ToStr(tmp,TmpStr,12);
	SL_Memcpy(TerSendBuf + len,TmpStr,9);              //���Ƶ����ͻ�����
	len += 9;
	//��λ����
	tmp = pGnss->longitude * 1000000;                  //����
	SL_Memset(TmpStr,0x30,12);
	UInt32ToStr(tmp,TmpStr,12);                        
	SL_Memcpy(TerSendBuf + len,TmpStr,9);              //���Ƶ����ͻ�����
	len += 9;

	//��λ�ٶ�
	tmp = pGnss->speed * 10;
	//SL_Print(">App:The :%d\r\n",i);
	SL_Memset(TmpStr,0x30,12);
	UInt32ToStr(tmp,TmpStr,12);
	for(tmp = 0;tmp < 4;tmp++)
		TerSendBuf[len++] = TmpStr[4 -tmp -1];

	//����Ƕ�
	tmp = pGnss->azimuth * 100;
	SL_Memset(TmpStr,0x30,12);
	UInt32ToStr(tmp,TmpStr,12);
	for(tmp = 0;tmp < 4;tmp++)
		TerSendBuf[len++] = TmpStr[4 -tmp -1];
			
	//ϵͳʱ��
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
					
	SL_Memset(TmpStr,0x30,12);
	CharToStr(pGnss->satellite_num,TmpStr,2);       //������������
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;
	TerSendBuf[len++] = ',';

	SL_Memset(TmpStr,0x30,12);
	CharToStr(pTcpIp->GsmSign,TmpStr,2);
	SL_Memcpy(TerSendBuf + len,TmpStr,2);           //�ź�ֵ
	len += 2;
	TerSendBuf[len++] = ',';

	//TerStateData.PowVol = 120;
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
	if(pTcpIp->LinkState > 0)
		TerSendBuf[len++] =  0x32;      //GSM����״̬  0:δע��;1:��ע��;2:�����ӡ�
	else
		TerSendBuf[len++] =  0x30;
	TerSendBuf[len++] = ',';

	tmp = GetVerNum();
	UInt32ToStr(tmp,TmpStr,12); 								 //�̼�Э��汾��
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;
	TerSendBuf[len++] = ',';
			
	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(pSys->SleepTime,TmpStr,12);	   		//˯��ʱ��
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
	TerSendBuf[len++] = ',';
					
	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(pSys->DistanceUpLoad,TmpStr,12);		//��λ�þ���
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
	TerSendBuf[len++] = ',';
					
	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(pSys->TravelUpLoadTime,TmpStr,12);	//��ʻ��λʱ��
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
	TerSendBuf[len++] = ',';
			
	//pSysCfg->HardWareNum = 12;                            // Ӳ���汾�ţ�
	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(pSys->HardWareNum,TmpStr,12);	  	//Ӳ���汾��
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
	TerSendBuf[len++] = ',';
					
	tmp = UInt32ToStr(pSys->CanProtocolNum,TmpStr,12);		//CanЭ��汾��
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
					
	TerSendBuf[len++] = ',';
	if(TerStateData.CanConnect> 0)                       //
		TerSendBuf[len++] = 0x30;
	else
		TerSendBuf[len++] = 0x31;
					
	TerSendBuf[len++] = ',';
	TerSendBuf[len++] = '0';   //SD��״̬  ,Ĭ�ϴ���0
	TerSendBuf[len++] = ',';

	TerSendBuf[len++] = TerStateData.McuFirNum / 10 + 0x30;   //��Ƭ���汾��--���汾��
	TerSendBuf[len++] = '.'; 
	TerSendBuf[len++] = TerStateData.McuFirNum % 10 + 0x30;   //
	TerSendBuf[len++] = ',';
					
	TerSendBuf[len++] = 0x0D;								  //�س�
	TerSendBuf[len++] = 0x0A;
									
	TerSendBuf[len] = 0x00;
	TerSendBuf[len + 1] = 0x00;
	TerSendBuf[len + 2] = 0x55;
	TerSendBuf[len + 3] = 0xAA;
	len += 4;
	TmpInt16.IntII = len - 6;								//�ܳ���
	TerSendBuf[2] = TmpInt16.TTbyte[0];
	TerSendBuf[3] = TmpInt16.TTbyte[1];
	TmpInt16.IntII	= len - 12; 							//���ݳ���
	TerSendBuf[6]	=TmpInt16.TTbyte[0];
	TerSendBuf[7]	=TmpInt16.TTbyte[1];
	SL_UartSendData(SL_UART_2,TerSendBuf,len);	
	//SL_Print(">App:Send Debug Data:  %d\r\n",pSysCfg->CfgFlag);
}



/**********************************************************
**	��������:
**	��������:������STM32��������  ��������
**	��������ģʽ:
**	1������ģʽ�������ӵ�Ƭ����ȡ���ݣ�����CAN���ݡ�״̬��Ϣ�ȡ�
**	2��������Ƭ������ͨ��FPT��ʽ���ص�Ƭ�������ļ����ְ����͵���Ƭ����
**	3������ST86/ST87Ӧ�ó����豸ͨ������3��ȡST87/86Ӧ�ó����ļ����ְ����͵�ST87/86ģ�顣
**	4���ൺ����ʹ�ô���2
**********************************************************/

void ProTerminalData(void)
{	
	static unsigned char 		step = 0,step1 = 0,step2 = 0;
	static struct stopwatch16 	sw,sw1;

	unsigned short int 			len;
	int 						tmp;
	SysConfigStr 				*pSys;                 //����
	IntToChar					TmpInt16;              //��ʱ���� 
	LongToChar					TmpInt32; 			   //��ʱ����
	unsigned char 				TmpStr[12];			   //
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
					if(read_stopwatch16_value(&sw) < 19)                       	     //��ʱ250
						break;
					SL_UartSendData(SL_UART_2,GetStatus,sizeof(GetStatus));    		//��ȡ�豸״̬��Ϣ ��Ҫ��ACC
					GetStatusBig = 0;
					init_stopwatch16(&sw); 
					step++;
					break;
				case 4:
					if(GetStatusBig == 0 && read_stopwatch16_value(&sw) < 10)   	//��ʱ 500ms  �ȴ�
						break;
					
					if(pTime->VerifyBit > 0)    	//�����Ҫ
					{
						step = 6;                 	//��ҪȥУʱ  
						init_stopwatch16(&sw);
						pTime->VerifyBit = 0;
						break;
					}
					if(McuStatus == 3)				//����ģʽ
					{
						step = 7;
						init_stopwatch16(&sw);
						break;
					}
					pSys = GetSysCfgSpace();
					//SL_Print("THe The Mcu Status:%d\r\n",McuStatus);
					if(McuStatus == 2 || pSys->CfgFlag == 0x56)				//
					{
						step++; 
						if(ConfigFlag == 0)
							ConfigFlag = 1;
						init_stopwatch16(&sw);		//
						break;
					}
					
					step = 2;                         //��������ģʽ
					init_stopwatch16(&sw);
					break;
				case 5:                               //����������Ϣ
					if(read_stopwatch16_value(&sw) < 10)
						break;
					//�����豸��Ϣ
					SendTerminalInfo(0x00FE);			//������Ϣ
					step = 2;   								//
					init_stopwatch16(&sw);
					break;
				case 6:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					step = 2;
					SendVerifyTimeCmd(pTime);              		//����У׼ʱ��  
					init_stopwatch16(&sw);
					break;
				case 7:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					SendTerminalInfo(0x00FD);   				 //���ͼ����Ϣ  ���Ͳ�����Ϣ
					step = 2;
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
					//SL_MEMBLOCK(UpgradeData->FileBuf[UpgradeData->FileTotalFram - 1],UpgradeData->FileLen[UpgradeData->FileTotalFram - 1],16);
					
					len = 0;
					SL_Memset(TerSendBuf,0,sizeof(TerSendBuf));          //
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
					SL_UartSendData(SL_UART_2,TerSendBuf,len);      //���ͳ�ȥ 
					init_stopwatch16(&sw1);
					step1++;
					break;
				case 1:                                                //��ʼ����  �����Ĺ�������Ҫ���  
					if(read_stopwatch16_value(&sw1) > 9600)        //1������Ҫ����������ȴ������
					{
						SL_FreeMemory(UpgradeData);
						RunMode = 0;         
						UpgradeStatus.UpgradeStatus = 1;     //����ʧ�� 
						//��������״̬ 
						return;
					}
					tmp = ReadComPkt(2,TerRecvBuf,sizeof(TerRecvBuf));    //
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

					TmpInt16.TTbyte[0] = *(TerRecvBuf + 4);
					TmpInt16.TTbyte[1] = *(TerRecvBuf + 5);
					if(TmpInt16.IntII != 0x001F)
						return;
					
					TmpInt16.TTbyte[0] = TerRecvBuf[10];      //����֡����
					TmpInt16.TTbyte[1] = TerRecvBuf[11];
				
					UpgradeData->FileCurrentFram = TmpInt16.IntII - 1;                  //��ǰ����֡����  
					SL_Print("The FileCurrentFram Num:%d\r\n",UpgradeData->FileCurrentFram);
					
					len = 0;
					SL_Memset(TerSendBuf,0,sizeof(TerSendBuf));    //�������
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
					//SL_Print("The FileCurrentFram Len:%d\r\n",UpgradeData->FileLen[UpgradeData->FileCurrentFram]);

					TmpInt16.IntII= CheckSum16(TerSendBuf + 12,len - 12);            //У�� 
					
					TerSendBuf[len++] = TmpInt16.TTbyte[0];
					TerSendBuf[len++] = TmpInt16.TTbyte[1];
					//SL_Print("CheckSum16: %d\r\n",TmpInt16.IntII);
					TerSendBuf[len++] = 0x55;
					TerSendBuf[len++] = 0xAA;
					SL_UartSendData(SL_UART_2,TerSendBuf,len);      //
					//SL_Print("Send Data Len %d\r\n",len);
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
					init_stopwatch16(&sw);
					GetBinFile(2,pUpgradeST86->CurrentFramNum);				//�������ݰ� 
					step2++;
					break;
				case 1:
					SL_Memset(TerRecvBuf,0,sizeof(TerRecvBuf));
					TmpInt16.IntII = ReadComPkt(2,TerRecvBuf,sizeof(TerRecvBuf));    //��ȡҪ���յ�������
					if(read_stopwatch16_value(&sw) < 15 && TmpInt16.IntII == 0)
						break;

					if(TerRecvBuf[0] != 0x5F || TerRecvBuf[1] != 0x5F)                //�жϰ�ͷ
					{
						SL_Memset(pUpgradeST86File,0,sizeof(SendQueueStr));    //������ൺ�����ݿռ�  
						WriteRunMode(0);      //���ص���������ģʽ
						break;
					}
					tmp = 0;
					TmpInt16.TTbyte[0] = *(TerRecvBuf + 2);                    //���ݳ��ȣ�����
					TmpInt16.TTbyte[1] = *(TerRecvBuf + 3);                    //��Ϣ�����ݳ���
					tmp = TmpInt16.IntII;                                      //����
			
					if(*(TerRecvBuf + tmp + 4) != 0x55 || *(TerRecvBuf + tmp + 5) != 0xAA)   //�жϰ�β
					{
						SL_Memset(pUpgradeST86File,0,sizeof(SendQueueStr));    //������ൺ�����ݿռ�  
						WriteRunMode(0);       //���ص���������ģʽ
						break;
					}
					
					TmpInt16.TTbyte[0] = *(TerRecvBuf + tmp + 2);
					TmpInt16.TTbyte[1] = *(TerRecvBuf + tmp + 3);
	
					if(TmpInt16.IntII != CheckSum16(TerRecvBuf + 4,tmp - 2))
					{
						SL_Memset(pUpgradeST86File,0,sizeof(SendQueueStr));    //������ൺ�����ݿռ�  
						WriteRunMode(0);     //���ص���������ģʽ
						break;
					}
					
					//��ʽ��������    TmpInt16.IntII��Ϊ���յ������ݵ����ݳ���ʹ��
					SL_Memset(TmpStr,'\0',12);
					SL_Memcpy(TmpStr,&TerRecvBuf[8],9);
					if(SL_Strcmp(TmpStr,"IIAP_ST87") == 0)
					{
						//SL_Print("Run The IIIIII\r\n");
						if(UpgradeST86Mode(&TerRecvBuf[8] + 9,tmp) == 2)        //���� 
						{	
							GetBinFile(5,0);//���������������
							init_stopwatch16(&sw);
							step2 = 3;
							break;
						}
					}
					init_stopwatch16(&sw);
					step2++;
					break;
				case 2:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					step2 = 0;
					break;
  				case 3:                   					//�������
  					if(read_stopwatch16_value(&sw) < 40)   //��ʱ1�������
						break;
					TmpInt32.LongLL = SL_AppUpdateInit(pUpgradeST86File, pUpgradeST86->FileLen);
					
                    SL_Print("---> SL APP update init ret:%d\n", TmpInt32.LongLL);
					
					
                    if (SL_APP_UPDATE_OK != TmpInt32.LongLL)
                    {
                        SL_Print("---> SL App update condition check fail, do not update!\n");
                    }
                    else
                    {
                        SL_Print("---> SL App update condition check ok, reset system and update ... \n");
						SL_Sleep(5000);				//�ȴ�10��
						SL_HstSendEvt(0x20161034);      //�������   �����豸
                        SL_PowerDown(0); 				//����
                    }
                    RunMode = 0;
					break;
				default:
					step2 = 0;
					break;
			}
			break;
	}
	
}












/**************************File End*************************/


