
/******************************************************************
**	FileName:
**	Time:
******************************************************************/

#include "include.h"



/*********************����ȫ�ֱ���******************************/

static SysConfigStr 					SysCfg;           //�豸������Ϣ



/****************************************************************
**	��������:
**	��������:
****************************************************************/

SysConfigStr *GetSysCfgSpace(void)
{
	return &SysCfg;
}



/*****************************************************************
**	��������:
**	��������:�����ļ�����
******************************************************************/


unsigned int FileCheckOrGreate(unsigned char *FileName, unsigned char *Pucbuff, unsigned int ulLenth,unsigned char WRSign)
{

	int ls1=0,fl_bt;
	unsigned int  WROKSign;

	fl_bt= SL_FileOpen(FileName, SL_FS_RDWR);	//��д��ʽ
	
	if (fl_bt < 0)	//ԭ��û��
	{	
		fl_bt = SL_FileCreate(FileName);
		if(fl_bt == 0)
			SL_Print("Creat File  %d\r\n",fl_bt);
		SL_Sleep(100);
	}


    if(WRSign == 1)
    {
		ls1=SL_FileWrite(fl_bt, Pucbuff, ulLenth);   //д�ļ�
		if (ls1==ulLenth)
		{
			WROKSign = 1;
		}
	   else
		{
			WROKSign = 0;
		}
    }
   else
    {
		ls1 = SL_FileRead(fl_bt, Pucbuff, ulLenth);
		SL_Print("The Read File.....%d\r\n",ls1);
		if (ls1 > 0)
		{
			WROKSign = ls1;
		}
	   else
		{
			WROKSign = 0;
		}
    }
    SL_Sleep(100);
	ls1=SL_FileClose(fl_bt);	
	SL_Sleep(100);
	
	
	return WROKSign;

}



/*****************************************************************
**	��������:
**	��������:
*****************************************************************/

void ReadSysCfg(void)
{
	unsigned char Flag;
	
	Flag = FileCheckOrGreate((unsigned char *)"ConfigFile",(unsigned char *)&SysCfg,sizeof(SysCfg),0);
	if(Flag > 0)
		SL_Print("Read CfgFile OK.......\r\n");
}



/****************************************************************
**	��������:
**	��������:
*****************************************************************/

void SaveSysCfg(void)
{
	unsigned char Flag;

	Flag = FileCheckOrGreate((unsigned char *)"ConfigFile",(unsigned char *)&SysCfg,sizeof(SysCfg),1);
	if(Flag > 0)
		SL_Print("Write CfgFile OK.......\r\n");
}

/********************************************************
**	��������:
**	��������:
********************************************************/

void InitSysTimer(void)
{
	SL_TASK 	   stSltask;

	stSltask.element[0] = SL_GetAppTaskHandle();

	SL_StartTimer(stSltask, 3, SL_TIMER_MODE_PERIODIC, SL_SecondToTicks(1)); 
	SL_StartTimer(stSltask, 4, SL_TIMER_MODE_PERIODIC, SL_MilliSecondToTicks(25));	//������ʱ�� 

}


/***************************************************************
**	��������:
**	��������:
****************************************************************/

void InitSysConfig(void)
{
	SL_Sleep(1000);
	ReadSysCfg();   
	SL_Sleep(1000);	               														    //��ʱһ��ʱ��
	SysCfg.CfgFlag = 0x56;                                                               	//
	//SL_Print("SysCfg.CfgFlag:%d\r\n",SysCfg.CfgFlag);
	if(SysCfg.CfgFlag != 0x55)              												//���û�����ù�  �ظ���������
	{
		//SL_Memset(SysCfg.TerminalID,'0',18);   //
		SL_Memcpy(SysCfg.TerminalID,"LOV1231703600519",sizeof("LOV1231703600519"));  		//
		
		SL_Memcpy(SysCfg.ServerAddr,"njgps-bc.lovol.com",sizeof("njgps-bc.lovol.com"));    //��������ַ
		SysCfg.ServerPort = 15000;
		SysCfg.RunTime =0;             														//����ǰ����ʱ��
		SysCfg.SleepSign = 1;          														//���߱�ʶ 
		SysCfg.SleepTime = 43200;          													//����ʱ��
		SysCfg.TravelUpLoadTime = 20;    													//������ʻʱ�����ϴ�ʱ����  ��λ��	
		SysCfg.WorkUpLoadTime = 1;      													//����ʱ�ϴ����ݼ��ʱ�� 
		SysCfg.DistanceUpLoad = 200;      													//���������ϱ�
		SysCfg.AzimuthUpLoad = 360;       													//����������ϱ� 
		SysCfg.CanProtocolNum = 0x09;       												//Э���  
		SysCfg.UserCode = 22;                                                               //�û����룬����   0x22
		SysCfg.CarType = 0x01;                                                              //������:С���ھ��:0x01															//
		SL_Memcpy(SysCfg.TerApn,"CMNET",sizeof("CMNET"));		                            //			
		//SysCfg.ConLockCar = 0;
		//SysCfg.SaveBit = 0;  	//��д��ʶ
		//SysCfg.CfgFlag = 0x55;
		//SaveSysCfg();                                                        
	}
	//SysCfg.CfgFlag = 0x55; 
	SysCfg.WorkUpLoadTime = 1;
	//SysCfg.TravelUpLoadTime = 120;  
}



/***************************File End*******************************/









