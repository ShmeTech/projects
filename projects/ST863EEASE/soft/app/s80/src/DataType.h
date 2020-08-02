
/***********************************************
**	FileName:DataType.h
**	Time:2017.01.26
**	Fun:��ʱϵͳʹ�õ��Զ������ݽṹ
************************************************/

#ifndef DATATYPE_H
#define DATATYPE_H


#define BLIND_NUM  250                                           //ä������



/***************************************************************
**	��λ���ݽṹ
**	
****************************************************************/

typedef struct 
{
	double 					latitude;								//ά��
	double 					longitude;	  							//����
	unsigned short int 		altitude;       						//���θ߶�
	unsigned short int 		azimuth;	  							//����Ƕ�
	float 					speed;		            				//�ٶ�
	float  					ghdop_v;      							//��λ����
	unsigned char 			satellite_num;         	 				//ʹ����������
	unsigned char 			satellite_view_num; 					//������������
	unsigned char 			status;                 				//��λ״̬  
	unsigned char			TYear;                                  //GNSS-ʱ��-��
	unsigned char 			TMon;									//GNSS-ʱ��-��
	unsigned char 			TDay;									//GNSS-ʱ��-��
	unsigned char 			THour;									//GNSS-ʱ��-ʱ
	unsigned char			TMin;									//GNSS-ʱ��-��
	unsigned char 			TSec;                                   //GNSS-ʱ��-��                           
	unsigned int            gnss_time;              				//��λʱ��,��ʱ����2000��1��1�ţ�0ʱ0��0��Ϊ��׼��
	unsigned char 			latitude_ns;            				//ά�ȱ�ʶ
	unsigned char 			longitude_ew;           				//���ȱ�ʶ
	unsigned char 			GnssModeStatus;                         //��λ״̬
}GNSSSTR; 


/***************************************************
**	��λ����ʹ��
***************************************************/
typedef struct
{
	double					StartLat;         						//��ʼ����
	double 					StartLong;          					//��ʼά��
	double 					EndLat;									//��������
	double 					EndLong;            					//����ά��
	float					Speed; 									//��ʻ�ٶ�
	unsigned short int 		StartAzimuth;     						//����Ƕ�
	unsigned short int 		EndAzimuth;     						//����Ƕ�
	double 					Distance;                               //��ʻ����
}DistanceProStr;

/****************************************************/
//TCPIP������Ϣ
typedef struct 
{

	unsigned char 			GprsApn[20];           					//���뷽ʽ
	unsigned char 			GprsNetActive;		   					//GPRS����״̬
	BOOL          			SimState;         	   					//�Ƿ���Sim��
	unsigned char 			SimIccId[25];     	   					//SIM��ICC_ID
	unsigned char			GsmSign;       	 						//GSM�ź�ֵ			
	unsigned char 			ServerAddr[50];        					//ƽ̨��ַ  ��������ַ
	unsigned short int 		ServerPort;       						//�˿�    �������˿�  
	unsigned char 			UlrToIPFlag;	                        //��ַ��ʶ��0:IP��ַ��1:URL	
	unsigned char 			LinkState;             					//����״̬
	unsigned char 			FtpLinkState;							//FTP����״̬
	unsigned char  			LinkNum;              					//���Ӵ��� ��Ҫ��ס���Ӷ��ٴ�֮������Ĭ�����ص�ַ
	unsigned char 			LinkCount;								//GPRS���Ŵ�����
	unsigned char 			GprsLinkState;             				//Gprs����״̬
	unsigned char 			SendState;              				//����״̬ 
	int			 			SimStatus;              				//Sim��״̬
	int						NetWorkState;           				//�ƶ�����״̬
	unsigned char			NetGprsState;           				//GPRS����״̬   GPRS�����Ƿ��� 
	unsigned char 			GprsAttachState;						//GPRS���總��״̬
	unsigned char 			ErrorCode;              				//������ ����ʱû��ʹ�� 
	unsigned char 			ResetLink;              				//��������
	unsigned char			LinkType;								//(1)FPT ����(0)TCP/IP����  
	unsigned char 			FtpLinkNum;								//FTP���Ӵ���
	unsigned char			FtpAddr[50];           					//Ftp��������ַ
	unsigned char 			FtpUserName[20];       					//�û���
	unsigned char 			FtpUserPassd[20];      					//����
	unsigned char 			FtpFileName[20];       					//�ļ�����  
	unsigned short int 		FtpPort;              					//�˿�
	
	unsigned short int 		FtpFileLen;            					//�ļ�����
	unsigned char 			*FtpFileSapce;          				//FTP���ص��ļ�ָ��
	unsigned char 			FtpLoadFig;          					//���ر�ʶ
	
}TcpIPSocketStr;


/*********************************************
**	
*********************************************/

typedef struct
{
	unsigned char 			IpAddr[50];        					//ƽ̨��ַ  ��������ַ
	unsigned short int      Port;
}IpAddrBackStr;


/************************************************
**	ϵͳ����
************************************************/
typedef struct
{

	unsigned char 			TerminalID[18];      					//�豸ID
	unsigned char           TerminalType;		 					//�ն�����
	unsigned char 			FirVerNum;			 					//Ӳ���汾��
	unsigned char 			NumNum;              					//���۱���
	unsigned char 			ServerAddr[50];      					//��������ַ(��������ַ������IP)
	unsigned int short 		ServerPort;     	 					//�������˿� 
	unsigned char 			UpdateAddr[50];      					//�豸���µ�ַ
	unsigned int short 		UpdatePort;     	 					//�豸�̼����µ�ַ   
	unsigned int 			RunTime;            	 				//����ǰ����ʱ��
	unsigned char 			SleepSign;           					//���߱�ʶ 
	unsigned int 			SleepTime;           					//����ʱ��
	unsigned int 			TravelUpLoadTime;    					//������ʻʱ�����ϴ�ʱ����  ��λ��	
	unsigned int        	WorkUpLoadTime;      					//����ʱ�ϴ����ݼ��ʱ�� 
	unsigned short int  	DistanceUpLoad;     	 				//���������ϱ�
	unsigned short int  	AzimuthUpLoad;       					//����������ϱ� 
	unsigned short int      CanProtocolNum;		 					//CANЭ��� 
	unsigned char 			CarType;             					//��������
	unsigned char 			UserCode;            					//�û�����  
	unsigned char 			CfgFlag;             					//���ñ�־λ
	unsigned char           TerApn[10];								//�����
	unsigned char			TerUser[10];							//�û���
	unsigned char			TerPassd[10];							//����
	unsigned char 			HardWareNum;							//Ӳ���汾��
	unsigned char 			SaveBit;             					//��д��ʶ
}SysConfigStr;


//ϵͳʱ��
typedef struct
{
	unsigned char 			Year;              								//ϵͳʱ��-��
	unsigned char 			Mon;               								//ϵͳʱ��-��
	unsigned char 			MDay;              								//ϵͳʱ��-��
	unsigned char 			WDay;              								//ϵͳʱ��-��
	unsigned char 			Hour;              								//ϵͳʱ��-ʱ
	unsigned char 			Min;               								//ϵͳʱ��-��
	unsigned char 			Sec;               								//ϵͳʱ��-��
	unsigned int 			TimeSec;           								//ϵͳʱ��-���룬��2000��1��1��0ʱ0��0�����
	unsigned char   		VerifyBit;         								//ʱ��У׼  ��ʶʱ���Ƿ��Ѿ�У׼
}SysTimeStr;


/***********************************************************
**	�����ϴ��ṹ
***********************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 			DataBuf[500];     							//���ͻ�����
	unsigned short int 		DataLen;          							//���ݳ���
	unsigned char 			DataCrc;          							//У��
	unsigned char       	DataBig;          							//���ݰ���ʶ(0:�������ݣ�1:ä���洢����)
	
}FiFoStr;
#pragma pack()

/*************************************************************
**	���Ͷ���
*************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 		QNum;             							//���Ͷ���
	unsigned short int 		QWrite;           							//���Ͷ��ж�ͷ
	unsigned short int 		QRead;            							//���Ͷ��ж�β
	FiFoStr            		QData[BLIND_NUM];       				    //���Ͷ������ݻ�����
}SendQueueStr;
#pragma pack()


/**************************************************************
**	CAN���ݽṹ��(С���)
***************************************************************/

#pragma pack(1)

typedef struct
{
	unsigned short int   EngineRotate;      						//������ת��
	unsigned char        WaterTemp;    								//��ȴˮ�¶�
	unsigned char        EngineOil;    								//����ѹ��
	unsigned char        FuelPercent;  								//ȼ�ϰٷֱ�
	unsigned int         EngineWorkTime;    						//����������ʱ��  
	unsigned short       SysVal;            						//ϵͳ��ѹ
	unsigned int         TotalTravlled;     						//����ʻ���
	unsigned short int   TravlledSpeed;     						//��ʻ�ٶ� 
	unsigned short int   Rotate1;        							//��Ͳת�� 
	unsigned short int   Rotate2;        							//������ת��
	unsigned short int   Rotate3;        							//����������
	unsigned short int   Rotate4;        							//��ѡ����ת��
	unsigned short int   Rotate5;        							//ι�����ת��
	unsigned short int   Rotate6;        							//����ת��
	unsigned short int   LossRatio;         						//��ѡ��ʧ��
	unsigned char        CabTemp;           						//��ʻ���¶�
	unsigned char  		 ReapHigh;          						//���߶�
	unsigned char        Aperture1;         						//��ɸ����
	unsigned char        Aperture2;         						//��ɸ����
	unsigned char        Aperture3;         						//βɸ����
	unsigned char        WarnValue;         						//��������  
	unsigned char 		 WorkFlag;									//�����״̬
}CanWheatStr;	
#pragma pack()



/************************************************************
**	Can���ݽṹ��(ˮ����)
************************************************************/

#pragma pack(1)

typedef struct
{
	unsigned short int   EngineRotate;      						//������ת��
	unsigned char        WaterTemp;    								//��ȴˮ�¶�
	unsigned short int   Rotate1;        							//��Ͳת��
	unsigned short int   Rotate2;        							//������ת��
	unsigned short       SysVol;            						//ϵͳ��ѹ
	unsigned int 		 EngineWorkTime;    						//����������ʱ�� 
	unsigned char        WarnValue;         						//��������
	unsigned short int   TravlledSpeed;     						//��ʻ�ٶ�
	unsigned int         TotalTravlled;     						//����ʻ���
}CanPaddyStr;

#pragma pack()

/**************************************************************
**	Can���ݽṹ(���׻�)
***************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int   EngineRotate;      //������ת��
	unsigned char        WaterTemp;    		//��ȴˮ�¶�
	unsigned char        EngineOil;    		//����ѹ��
	unsigned char        FuelPercent;  		//ȼ�ϰٷֱ�
	unsigned int 		 EngineWorkTime;    //����������ʱ�� 
	unsigned short       SysVol;            //ϵͳ��ѹ
	unsigned int         TotalTravlled;     //����ʻ���
	unsigned short int   Rotate1;        	//
	unsigned short int   Rotate2;        	//������ת��
	unsigned short int   TravlledSpeed;     //��ʻ�ٶ�
	unsigned short int   ReapHigh;          //���߶�
	unsigned char        WarnValue;         //��������

	
}CanMaizeStr;
#pragma pack()


/**************************************************
**	Can���ݽṹ(������)  (Mƽ̨������)
***************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int	 EngineRotate;		//������ת��
	unsigned char		 WaterTemp; 		//��ȴˮ�¶�
	unsigned char		 EngineOil; 		//����ѹ��
	unsigned char		 FuelPercent;		//ȼ�ϰٷֱ�
	unsigned int		 EngineWorkTime;	//����������ʱ�� 
	unsigned short		 SysVol;			//ϵͳ��ѹ
	unsigned short int	 TravlledSpeed; 	//��ʻ�ٶ�
	unsigned int		 TotalTravlled; 	//����ʻ���
	unsigned char		 WarnValue; 		//��������
}CanTractorStr;

#pragma pack()



/**************************************************
**	Can���ݽṹ(������)  (Pƽ̨������)
***************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 		 PTOStatus;					//PTO״̬
	unsigned short int	 EngineRotate;				//������ת��    
	unsigned char		 WaterTemp; 				//������ˮ�£�
	unsigned char 		 SpeedLock;					//������״̬����/�أ�
	unsigned char        RangeStatus;        		//����״̬
	unsigned char        BrakeFluidLight;    		//�ƶ�Һ��
	unsigned char		 DistanceLight;      		//Զ���״̬
	unsigned char    	 GeneralLight;		 		//��ͨ�ƹ�״̬
	unsigned char 		 StopLight;                 //פ����
	unsigned char		 Light1;			 		//ǰ������
	unsigned char		 Light2;             		//ǰ���
	unsigned char		 Light3;			 		//������
	unsigned char		 Light4;			 		//�󵥵㹤����
	unsigned char		 Light5;             		//��ת���
	unsigned char		 Light6;             		//��ת���
	unsigned char		 Light7;            	 	//����
	unsigned char 		 PowerShiftLow;      		//����ת������
	unsigned char 		 PowerShiftMid;		 		//����ת������
	unsigned char 	     PowerShiftHight;    		//����ת������
	unsigned char 		 AirBreakLow;		 		//��ѹ�ƶ���״̬
	unsigned char        FuelPositionLow;   		//ȼ��λ�õͱ���
	unsigned char 		 ShuttleEmpty;       		//��ʽ�����յ�
	unsigned char 		 ShuttleRetreat;     		//��ʽ�������˵�
	unsigned char 		 ShuttleAdvance;     		//��ʽ����ǰ����
	unsigned char 		 CrawlStatus;        		//���е�ײ��
	unsigned int		 TravlledDistance; 	 		//����ʻ���
	unsigned int		 TravlledTotalDistance; 	//����ʻ��� 
	unsigned char	     Light9;					//Ԥ������
	unsigned int		 EngineWorkTime;			//����������ʱ�� 
	unsigned int 	     CarWorkTime;       		//��������ʱ��
	unsigned int 	     PTOWorkTime;       		//PTO����ʱ��
	unsigned char    	 EngineFuelTemp;    		//������ȼ���¶�    
	unsigned short int	 EngineOilTemp;     		//�����������¶�   
	unsigned char   	 EngineOilPressure; 		//����������ѹ��
	unsigned char 	     Light10;           		//������ˮ�¸�Ԥ����
	unsigned char        Light11;					//����������ѹ����Ԥ����
	unsigned short int   PTOSped;           		//PTOת��
	unsigned short int   PTOSpeedSet;               //PTOת���趨
	unsigned char		 HandBrake;         		//���ƶ�״̬
	unsigned char        ClutchStatus;      		//�����̤��״̬
	unsigned short int	 TravlledSpeed;     		//��ʻ�ٶ�
	unsigned char        EngineAirfiltrate; 		//���������״̬
	unsigned short int   BatteryVol;        		//��ص�ѹ
	unsigned char        GearBoxOilLow;     		//��������ѹ�ͱ���
	unsigned char        GearBoxOilHight;   		//��������ѹ�߱���
	unsigned char 		 FuelPosition;      		//ȼ��λ��
	unsigned char		 DynamoStatus;      		//��������״̬  
}CanPTractorStr;

#pragma pack()




/*****************************************
**	���л��͵�CAN���ݶ����ڸ����ݽṹ����
*******************************************/
#pragma pack(1)
typedef struct
{
	unsigned short int    	EngineRotate;      			 //������ת��
	unsigned char        	WaterTemp;    				 //��ȴˮ�¶�
	unsigned short int   	Rotate1;        			 //����ת��
	unsigned short int   	Rotate2;        			 //������ת��
	unsigned short int   	SysVol;            			 //ϵͳ��ѹ
	unsigned int 		 	EngineWorkTime;    			 //����������ʱ�� 
	unsigned char        	WarnValue1;         		 //��������
	unsigned short int   	TravlledSpeed;     			 //��ʻ�ٶ�
	unsigned int         	TotalTravlled;     			 //����ʻ���  
	unsigned int 		 	OilConsumption;	             //�����ͺ��ͺ�
	unsigned int 		 	TatalOilConsumption;         //���ͺ�  																							
	unsigned char        	EngineOil;    				 //����ѹ�� 
	unsigned char        	FuelPercent;  				 //ȼ�ϰٷ�	
}CanDataStr;
#pragma pack()



//Pƽ̨��ǰ�����������
typedef struct
{
	unsigned char FaultCodeNum;         	//����������
	unsigned char FaultCode[160];			//
}CurrentFaultCodeStr;             	 	//��ǰ������


//�����û�Э���
#pragma pack(1)
typedef struct
{
	unsigned short int   CanProtocolSerial;  //����Э����
	unsigned char        LockCardState;      //����״̬
}TerminalCanProStr;
#pragma pack()


//ͨѶЭ����Ϣͷ
#pragma pack(1)
typedef struct 
{
    unsigned char 			frame_start[3];			// ��Ϣ��ͷ
    unsigned char			msg_id;          		// ��ϢID
    unsigned char 			device_id[16];   		// �豸��
	unsigned char 			blind_flag;             // ä����ʶ
    unsigned char 			msg_line;  				// �����Ϣ�к�
    unsigned char 			msg_body_num;     		// ��Ϣ������
    unsigned short int 		msg_len;        		// ��Ϣ�ܳ���
}MsgHeadStr;

#pragma pack()

/*************************************************************
**	�豸״̬��Ϣ��
************************************************************/

#pragma pack(1)
typedef struct 
{
	unsigned short int 	 	msg_device_type;                    //��Ϣ��������
	unsigned short int 		msg_device_len;                     //��Ϣ�峤��(�������򳤶�)
	
	unsigned char      		msg_device_manu_num;                 //
	unsigned char 	   		msg_device_manu_type;                //
	unsigned char      		msg_device_user_num;                 //
	unsigned char      		msg_device_car_num;                  //��������
	unsigned char      		msg_device_protocol_ver;             //ͨѶЭ��汾
	unsigned char      		msg_device_firware_ver;              //�̼��汾          
	unsigned char      		msg_device_hardware_ver;             //�Ѿ��汾��
}MsgDeviceStr;
#pragma pack()


/*************************************************
**	
*************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 		msg_input_type;						//
	unsigned short int 		msg_input_len;						//
	unsigned char      		msg_input_io;						//
	unsigned char      		msgAccState;						//
	unsigned char	   		msgMotoState;						//
	unsigned short int 		msgFrq1;							//
	unsigned short int 		msgFrq2;							//
	unsigned short int 		msgPWM1;							//
	unsigned short int 		msgPWM2;							//
	unsigned short int 		msgInputVol1;						//
	unsigned short int 		msgInputVol2;						//
	unsigned short int 		msgPowVol;							//
	unsigned short int 		msgBatteryVol;						//
	unsigned int       		msgWarningState;					//
	unsigned char      		msgLine;							//
} MsgInputStr;

#pragma pack()


//
#pragma pack(1)

typedef struct 
{
	unsigned short int 		msg_gps_type;                  	//
	unsigned short int 		msg_gps_len;                   	//
	unsigned int       		msg_gps_latitude;        		//
	unsigned int       		msg_gps_longitude;             	//
	unsigned short int 		msg_gps_speed;           		//
	unsigned short int 		msg_gps_azimuth;         		//
	unsigned short int 		msg_gps_altitude;        		//
	unsigned char      		msg_gps_year;                  	//
	unsigned char      		msg_gps_mon;                   	//
	unsigned char      		msg_gps_day;					//
	unsigned char      		msg_gps_hour;                 	//
	unsigned char      		msg_gps_min;                   	//
	unsigned char      		msg_gps_sec;                   	//
	unsigned char      		msg_gps_satellite_num;         	//
	unsigned char      		msg_gps_view_num;              	//
	unsigned short int 		msg_gps_ghdop_v;             	//
	unsigned char      		msg_gps_status;              	//
	unsigned char      		msg_gps_line;
}MsgGnssStr;

#pragma pack()



//

#pragma pack(1)
typedef struct
{
	unsigned short int 		MsgFarmType;                   	//
	unsigned short int 		MsgFarmLen;                     // 

	unsigned short int 		MsgFarmBig;						//
	unsigned char 	   		MsgTimeYear;					//
	unsigned char 	   		MsgTimeMon;						//
	unsigned char 	   		MsgTimeDay;						//
	unsigned char 	   		MsgTimeHour;					//
	unsigned char 	   		MsgTimeMin;						//
	unsigned char 	   		MsgTimeSec;						//
	unsigned char      		MsgLine;						//
}MsgFarmStr;

#pragma pack()


#pragma pack(1)

/**************************************************************
**	CAN���ݽṹ��(С���)
***************************************************************/
typedef struct
{
	unsigned short int 	 	MsgCanWheatType;                  //��Ϣ��������
	unsigned short int 	 	MsgCanWheatLen;                   //��Ϣ�����ݳ���

	unsigned short int   	MsgEngineRotate;      			  //������ת��
	unsigned char        	MsgWaterTemp;    				  //��ȴˮ�¶�
	unsigned char        	MsgEngineOil;    		          //����ѹ��
	unsigned char        	MsgFuelPercent;  		          //ȼ�ϰٷֱ�
	unsigned int         	MsgEngineWorkTime;               //����������ʱ��  
	unsigned short       	MsgSysVal;                       //ϵͳ��ѹ
	unsigned int         	MsgTotalTravlled;                //����ʻ���
	unsigned short int   	MsgTravlledSpeed;                //��ʻ�ٶ� 
	unsigned short int   	MsgRotate1;        				 //��Ͳת�� 
	unsigned short int   	MsgRotate2;        				 //������ת��
	unsigned short int   	MsgRotate3;        				 //����������
	unsigned short int   	MsgRotate4;        				 //��ѡ����ת��
	unsigned short int   	MsgRotate5;        				 //ι�����ת��
	unsigned short int   	MsgRotate6;        				 //����ת��
	unsigned short int   	MsgLossRatio;         			 //��ѡ��ʧ��
	unsigned char        	MsgCabTemp;           			 //��ʻ���¶�
	unsigned char		 	MsgReapHigh;          			 //���߶�
	unsigned char        	MsgAperture1;         			 //��ɸ����
	unsigned char        	MsgAperture2;         			 //��ɸ����
	unsigned char        	MsgAperture3;         			 //βɸ����
	unsigned char        	MsgWarnValue;         			 //�������� 
	unsigned char 		 	MsgLine;
}MsgCanWheatStr;	
#pragma pack()


#pragma pack(1)

/**************************************************************
**	CAN���ݽṹ��(С���)
***************************************************************/
typedef struct
{
	unsigned short int 	 	MsgCanWheatType;                  //
	unsigned short int 	 	MsgCanWheatLen;                   //

	unsigned short int   	MsgEngineRotate;      			  //������ת��
	unsigned char        	MsgWaterTemp;    				  //��ȴˮ�¶�
	unsigned char        	MsgEngineOil;    		          //����ѹ��
	unsigned char        	MsgFuelPercent;  		          //ȼ�ϰٷֱ�  
	unsigned char 			MsgLine;                          //
}MsgCanWheatOldStr;
#pragma pack()


#pragma pack(1)
typedef struct
{
	unsigned short int 		MsgIntelligentType;
	unsigned short int 		MsgIntelligentLen;

	unsigned char			MsgVECUMan;							//VECU����
	unsigned char			MsgVECUType;						//VECU�ͺ�
	unsigned short int		MsgCANProVer;						//����汾
	unsigned char			MsgEngineMan;						//����������
	unsigned char			MsgEngineType;						//����������
	unsigned char			MsgEECUMan;							//EECU����
	unsigned char			MsgEECUType;						//EECU����
	unsigned char			MsgDeviceType;						//�豸����
	unsigned char			MsgResourceCfg;						//������Դ����
	unsigned int			MsgEngineWorkTime;					//����������Сʱ 
	unsigned int			MsgTotalTravlled;					//�����
	unsigned short int		MsgFuelNum;							//ȼ���� 
	unsigned short int		MsgSysVol;							//ϵͳ��ѹ 
	unsigned short int		MsgTravlledSpeed;					//��ʻ�ٶ� 
	unsigned char			MsgFuelPercent;						//ȼ�Ͱٷֱ�
	unsigned char			MsgWarnValue1;						//��������
	unsigned char			MsgClutchStatus;					//�����״̬
	unsigned char			MsgCutTableState;					//��̨״̬
	unsigned short int		MsgStripRotate;						//��Ƥ��ת�� 
	unsigned short int		MsgLiftRotate;						//������ת�� 
	unsigned short int		MsgRotate1;							//����(����)��Ͳת�� 
	unsigned short int		MsgRotate3;							//������ת�� 
	unsigned char			MsgRotateWarn1;						//������Ͳת�ٱ���״̬λ
	unsigned char			MsgRotateWarn2;						//������ת�ٱ���״̬λ
	unsigned char			MsgRotateWarn3;						//������ת�ٱ���״̬λ
	unsigned char			MsgRotateWarn4;						//ι�����ת�ٱ���״̬λ
	unsigned char			MsgRotateWarn5;						//����ת�ٱ���״̬λ
	unsigned char			MsgRotateWarn6;						//��ѡ����ת�ٱ���״̬λ
	unsigned char			MsgRotateWarn7;						//������ת�ٱ���״̬λ
	unsigned char			MsgRotateWarn8;						//��Ƥ��ת�ٱ���״̬λ
	unsigned short int		MsgRotate4;							//��ѡ����ת�� 
	unsigned short int		MsgTorque1;							//����(����)��Ͳת�� 
	unsigned short int		MsgRotate5;							//ι�����ת�� 
	unsigned short int		MsgRotate6;							//����ת�� 
	unsigned short int		MsgRotate7;							//������ת�� 
	unsigned short int		MsgDisplaceQuantity;				//������ǰ��λ����
	unsigned short int		MsgReelHigh;						//�����ָ߶�
	unsigned short int		MsgRotate8;							//������ת�� 
	unsigned short int		MsgLossRatio;						//��ѡ��ʧ�ʣ�����
	unsigned short int		MsgCutTableHigh;					//��̨�߶�
	unsigned short int		MsgReapHigh;						//���߶�
	unsigned char			MsgAperture1;						//��ɸ����
	unsigned char			MsgAperture2;						//��ɸ����
	unsigned char			MsgAperture3;						//βɸ����
	unsigned short int		MsgMoistureRate;					//��ˮ��
	unsigned char			MsgBrokenRate;						//������
	unsigned char			MsgCutterKnifeFre;					//�Ƶ��
	unsigned char			MsgSunkGap;							//�����϶(���ֵ)
	unsigned char			MsgDynamoStatus;					//���ָʾ
	unsigned char			MsgPlaceLight;						//λ�õ�
	unsigned char			MsgLight6;							//��ת��
	unsigned char			MsgLight5;							//��ת��
	unsigned char			MsgDistanceLight;					//Զ��
	unsigned char			MsgLight8;							//����
	unsigned char			MsgAirFilterWarn;					//���˶�������
	unsigned char			MsgLight9;							//Ԥ��
	unsigned char			MsgGearN;							//�յ�N
	unsigned char			MsgHandBrake;						//��ɲ
	unsigned char			MsgGearR;							//����R
	unsigned char			MsgGearF;							//ǰ����F
	unsigned char			MsgStartupState;					//��״̬
	unsigned char			MsgOilWater;						//��ˮ����
	unsigned char			MsgEngineFault;						//����������
	unsigned char			MsgUnloadSafeSwitch;				//ж����ȫ����
	unsigned char			MsgUnfoldLimitSwitch;				//ж��Ͳչ����λ����
	unsigned char			MsgRegainLimitSwitch;				//ж��Ͳ�ջ���λ����
	unsigned char			MsgButton1;							//������ٰ�ť
	unsigned char			MsgButton2;							//������ٰ�ť
	unsigned char			MsgButton3;							//������������ť
	unsigned char			MsgButton4;							//�������½���ť
	unsigned char			MsgButton5;							//������Ͳ���ٰ�ť
	unsigned char			MsgButton6;							//������Ͳ���ٰ�ť
	unsigned char			MsgButton7;							//��������ϰ�ť
	unsigned char			MsgButton8;							//����Ϸ��밴ť
	unsigned char			MsgButton9;							//����������ϰ�ť
	unsigned char			MsgButton10;						//������Ϸ��밴ť
	unsigned char			MsgButton11;						//ж��Ͳչ����ť
	unsigned char			MsgButton12;						//ж��Ͳ�ջذ�ť
	unsigned char			MsgButton13;						//��̨������ť
	unsigned char			MsgButton14;						//��̨�½���ť
	unsigned char			MsgECUComState;						//ECUͨ��״̬
	unsigned char			MsgRunMode1;						//��̨�߶��Զ�����ģʽ����״̬
	unsigned char			MsgRunMode2;						//�����Զ�����ģʽ����״̬
	unsigned char			MsgDrivingLamp;						//�ռ��г�������
	unsigned char			MsgRadiotube1;						//��̨������ŷ�
	unsigned char			MsgRadiotube2;						//��̨�½���ŷ�
	unsigned char			MsgRadiotube3;						//������������ŷ�
	unsigned char			MsgRadiotube4;						//�������½���ŷ�
	unsigned char			MsgRadiotube5;						//������Ͳ�ٶ�������ŷ�
	unsigned char			MsgRadiotube6;						//������Ͳ�ٶ��½���ŷ�
	unsigned char			MsgRadiotube7;						//��������ϵ�ŷ�
	unsigned char			MsgRadiotube8;						//����Ϸ����ŷ�
	unsigned char			MsgRadiotube9;						//����������ϵ�ŷ�
	unsigned char			MsgRadiotube10;						//������Ϸ����ŷ�
	unsigned char			MsgRadiotube11;						//ж��Ͳչ����ŷ�
	unsigned char			MsgRadiotube12;						//ж��Ͳ�ջص�ŷ�
	unsigned char			MsgRetrogressRelay;					//�����̵���
	unsigned char			MsgBackupRelay;						//�����������
	unsigned char			MsgRadiotube13;						//������ٵ�ŷ�
	unsigned short int		MsgHeaderLeftDistance;				//��̨����״���
	unsigned short int		MsgHeaderRightDistance;				//��̨�Ҳ��״���
	unsigned char			MsgFeedQuantity;					//ι���������ֵ��
	unsigned char			MsgLeftLossRatio;					//��ѡ�󴫸�����ʧ��
	unsigned char			MsgMidLossRatio;					//��ѡ�д�������ʧ��
	unsigned char			MsgRightLossRatio;					//��ѡ�Ҵ�������ʧ��
	unsigned int			MsgOdometerMeter;					//�׼����
	unsigned short int		MsgOtherRatio;						//��ѡ������
	unsigned char			MsgHeaderLossRatio;					//��̨��ʧ�ʣ����ֵ��
	unsigned char			MsgEntrainmentLossRatio;			//�д���ʧ�ʣ����ֵ��
	unsigned short int		MsgFoodStirTorque;					//ι�����Ť��
	unsigned short int		MsgDraughtFanAngle;					//����ַ��Ƕ�
	unsigned short int		MsgFrontMotorRatio;					//ǰ���ת��
	unsigned short int		MsgBackMotorRatio;					//�����ת��
	unsigned short int		MsgDripAngle;						//�������Ƕ�
	unsigned char			MsgCabTemp;							//��ʻ���¶�
	unsigned char			MsgHydraulicOil;					//Һѹ���¶�
	unsigned short int		MsgFoodWeightDry;					//��ʳ���� 
	unsigned short int		MsgFoodWeightWet;					//��ʳʪ�� 
	unsigned short int		MsgAreaYield;						//Ķ��
	unsigned short int		MsgCerealFlowCheck;					//���������������
	unsigned short int		MsgAreaExpend;						//Ķ��������
	unsigned char			MsgStateBit1;						//��ѡ��ʧ��״̬λ
	unsigned char			MsgStateBit2;						//��̨���ο���״̬λ
	unsigned char			MsgIdlingFlag;						//���ٱ�־λ
	unsigned char			MsgLoadPowerSwitch;					//���ɹ��ʿ���
	unsigned short int		MsgFeedbackValue1;					//ǧ�������÷���ֵ
	unsigned short int		MsgFeedbackValue2;					//���θ߶����÷���ֵ
	unsigned short int		MsgFeedbackValue3;					//������Ͳ����ת�ٵͱ�������ֵ
	unsigned short int		MsgFeedbackValue4;					//��̨��λ���������޸߶�
	unsigned short int		MsgFeedbackValue5;					//��̨��λ���������޸߶�
	unsigned char			MsgFeedbackValue6;					//��ǰ�����������÷���ֵ
	unsigned char			MsgFeedbackValue7;					//����������ģʽ���÷���ֵ
	unsigned char			MsgFeedbackValue8;					//��̨��ʧ�����÷���ֵ
	unsigned char			MsgFeedbackValue9;					//�����������÷���ֵ
	unsigned char			MsgResetKey1;						//������ʻ������㣨�Ǳ�����
	unsigned char			MsgResetKey2;						//�׼����㣨�Ǳ�����
	unsigned char			MsgResetKey3;						//��̨�߶�һ����λ���Ǳ�����
	unsigned char			MsgResetKey4;						//ж��Ͳ���ƣ��Ǳ�����
	unsigned char			MsgResetKey5;						//�����Զ����ڿ��ƣ��Ǳ����ƣ�
	unsigned char			MsgResetKey6;						//��ʳ�������㣨�Ǳ�����
	unsigned short int		MsgSetValue1;						//ǧ��������ֵ
	unsigned short int		MsgSetValue2;						//���θ߶�����ֵ
	unsigned short int		MsgSetValue3;						//������Ͳ����ת�ٵͱ�������ֵ
	unsigned short int		MsgSetValue4;						//��̨��λ���ã����޸߶�
	unsigned short int		MsgSetValue5;						//��̨��λ���ã����޸߶�
	unsigned char			MsgSetValue6;						//��ǰ������������
	unsigned char			MsgSetValue7;						//����������ģʽ
	unsigned char			MsgSetValue8;						//��̨��ʧ������ֵ
	unsigned char			MsgSetValue9;						//������������ֵ
	unsigned short int		MsgEngineRotate;					//������ת��
	unsigned char			MsgWaterTemp;						//��ȴˮ��
	unsigned char			MsgEngineOil;						//����ѹ��
	unsigned char 			MsgLine;							//��Ϣ�����к�

}MsgIntelligent;          //����С�����Ϣ��
#pragma pack()

/************************************************************
**	Can���ݽṹ��(ˮ����)
************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 	 	MsgCanPaddyType;                  	//
	unsigned short int 	 	MsgCanPaddyLen;                   	//

	unsigned short int   	MsgEngineRotate;      				//������ת��
	unsigned char        	MsgWaterTemp;    					//��ȴˮ�¶�
	unsigned short int   	MsgRotate1;        					//
	unsigned short int   	MsgRotate2;        					//������ת��
	unsigned short int   	MsgSysVol;            				//ϵͳ��ѹ
	unsigned int 		 	MsgEngineWorkTime;    				//����������ʱ�� 
	unsigned char        	MsgWarnValue;         				//��������
	unsigned short int   	MsgTravlledSpeed;     				//��ʻ�ٶ�
	unsigned int         	MsgTotalTravlled;     				//����ʻ���  
	
	unsigned char        	MsgEngineOil;    					//����ѹ��  (�¼�)
	unsigned char        	MsgFuelPercent;  					//ȼ�ϰٷֱ�
	unsigned char 			MsgLine1;

}MsgCanPaddyStr;
#pragma pack()


/**************************************************************
**	Can���ݽṹ(���׻�)
***************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 	 MsgCanMaizeType;                  //
	unsigned short int 	 MsgCanMaizeLen;                   //

	unsigned short int   MsgEngineRotate;      				//������ת��
	unsigned char        MsgWaterTemp;    					//��ȴˮ�¶�
	unsigned char        MsgEngineOil;    					//����ѹ��
	unsigned char        MsgFuelPercent;  					//ȼ�ϰٷֱ�
	unsigned int 		 MsgEngineWorkTime;    				//����������ʱ�� 
	unsigned short       MsgSysVol;            				//ϵͳ��ѹ
	unsigned int         MsgTotalTravlled;     				//����ʻ���
	unsigned short int   MsgRotate1;        				//
	unsigned short int   MsgRotate2;        				//������ת��
	unsigned short int   MsgTravlledSpeed;     				//��ʻ�ٶ�
	unsigned short int   MsgReapHigh;          				//���߶�
	unsigned char        MsgWarnValue;         				//��������
	unsigned char 		 MsgLine;							//
	
}MsgCanMaizeStr;
#pragma pack()


/**************************************************
**	Can���ݽṹ(������)
***************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 	 MsgCanTractorType;                 //��Ϣ��������  
	unsigned short int 	 MsgCanTractorLen;                  //��Ϣ�峤��

	unsigned short int	 MsgEngineRotate;					//������ת��
	unsigned char		 MsgWaterTemp; 						//��ȴˮ�¶�
	unsigned char		 MsgEngineOil; 						//����ѹ��
	unsigned char		 MsgFuelPercent;					//ȼ�ϰٷֱ�
	unsigned int		 MsgEngineWorkTime;					//����������ʱ�� 
	unsigned short		 MsgSysVol;							//ϵͳ��ѹ
	unsigned short int	 MsgTravlledSpeed; 					//��ʻ�ٶ�
	unsigned int		 MsgTotalTravlled; 					//����ʻ���
	unsigned char		 MsgWarnValue; 						//�������� 
	unsigned char 		 MsgLine;
}MsgCanTractorStr;

#pragma pack()



/**************************************************
**	Can���ݽṹ(Pƽ̨������)
***************************************************/

#pragma pack(1)

typedef struct
{
	unsigned short int 	 MsgCanPtractorType;			//
	unsigned short int   MsgCanPtractorLen;				//
	
	unsigned char 		 MsgPTOStatus;					//PTO״̬
	unsigned short int	 MsgEngineRotate;				//������ת��    
	unsigned char		 MsgWaterTemp; 					//������ˮ�£�
	unsigned char 		 MsgSpeedLock;					//������״̬����/�أ�
	unsigned char        MsgRangeStatus;        		//����״̬
	unsigned char        MsgBrakeFluidLight;    		//�ƶ�Һ��
	unsigned char		 MsgDistanceLight;      		//Զ���״̬
	unsigned char    	 MsgGeneralLight;		 		//��ͨ�ƹ�״̬
	unsigned char 		 MsgStopLight;                  //פ����
	unsigned char		 MsgLight1;			 			//ǰ������
	unsigned char		 MsgLight2;             		//ǰ���
	unsigned char		 MsgLight3;			 			//������
	unsigned char		 MsgLight4;			 			//�󵥵㹤����
	unsigned char		 MsgLight5;             		//��ת���
	unsigned char		 MsgLight6;             		//��ת���
	unsigned char		 MsgLight7;            	 		//����
	unsigned char 		 MsgPowerShiftLow;      		//����ת������
	unsigned char 		 MsgPowerShiftMid;		 		//����ת������
	unsigned char 	     MsgPowerShiftHight;    		//����ת������
	unsigned char 		 MsgAirBreakLow;		 		//��ѹ�ƶ���״̬
	unsigned char        MsgFuelPositionLow;   			//ȼ��λ�õͱ���
	unsigned char 		 MsgShuttleEmpty;       		//��ʽ�����յ�
	unsigned char 		 MsgShuttleRetreat;     		//��ʽ�������˵�
	unsigned char 		 MsgShuttleAdvance;     		//��ʽ����ǰ����
	unsigned char 		 MsgCrawlStatus;        		//���е�ײ��
	unsigned int		 MsgTravlledDistance; 	 		//����ʻ���
	unsigned int		 MsgTravlledTotalDistance; 		//����ʻ��� 
	unsigned char	     MsgLight9;						//Ԥ������
	unsigned int		 MsgEngineWorkTime;				//����������ʱ�� 
	unsigned int 	     MsgCarWorkTime;       			//��������ʱ��
	unsigned int 	     MsgPTOWorkTime;       			//PTO����ʱ��
	unsigned char    	 MsgEngineFuelTemp;    			//������ȼ���¶�    
	unsigned short int	 MsgEngineOilTemp;     			//�����������¶�   
	unsigned char   	 MsgEngineOilPressure; 			//����������ѹ��
	unsigned char 	     MsgLight10;           			//������ˮ�¸�Ԥ����
	unsigned char        MsgLight11;					//����������ѹ����Ԥ����
	unsigned short int   MsgPTOSped;           			//PTOת��
	unsigned short int   MsgPTOSpeedSet;                //PTOת���趨
	unsigned char		 MsgHandBrake;         			//���ƶ�״̬
	unsigned char        MsgClutchStatus;      			//�����̤��״̬
	unsigned short int	 MsgTravlledSpeed;     			//��ʻ�ٶ�
	unsigned char        MsgEngineAirfiltrate; 			//���������״̬
	unsigned short int   MsgBatteryVol;        			//��ص�ѹ
	unsigned char        MsgGearBoxOilLow;     			//��������ѹ�ͱ���
	unsigned char        MsgGearBoxOilHight;   			//��������ѹ�߱���
	unsigned char 		 MsgFuelPosition;      			//ȼ��λ��
	unsigned char		 MsgDynamoStatus;      			//��������״̬   
	
	unsigned char 	 	 MsgLine;						//��Ϣ
	
}MsgCanPTractorStr;         //

#pragma pack()



/******************************************
**
**
*******************************************/
#pragma pack(1)
typedef struct
{
	unsigned short int 	MsgOilType;
	unsigned short int 	MsgOilLen;

	unsigned int 		MsgOil;      				//�����ͺ�
	unsigned int 	   	MsgOilTotal;  				//�ۼ��ͺ�
	unsigned char 		MsgLine;    				//
}MsgOilStr;
#pragma pack()


//Pƽ̨��ǰ�����������
#pragma pack(1)
typedef struct
{
	unsigned short int MsgFaultCodeType;         	//���������� 
	unsigned short int MsgFaultCodeLen;				//
	unsigned char MsgFaultCode[10][8];				//
}MsgCurrentFaultCodeStr;             	 		//��ǰ������
#pragma pack()


//Pƽ̨��ʷ������
#pragma pack(1)
typedef struct
{
	unsigned short int MsgFaultCodeType;
	unsigned short int MsgFaultCodeLen;
	
	unsigned char MsgFaultCode[10][8];
}MsgHistoryFaultCodeStr;                   //��ʷ������
#pragma pack()



/***************************************************
**	STM32���ݿ�
****************************************************/
typedef struct
{
	unsigned short int TypeCode;					//
	unsigned short int DataLen;						//
	void *table;
}TerDataBlockStr;

/****************************************************
**	��STM32���ƿ�
*****************************************************/
typedef struct
{	
	unsigned char 	flag;						  //
	void 			*block;						  //
}TerDataTcbStr;



/***************************************************
**	��Ƭ�����ɼ���״̬���ݽṹ
**	
***************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 			AccState;		//ACC״̬
	unsigned char 			CanConnect;		//CAN����״̬
	unsigned short int 		PowVol;       //�ⲿ�����ѹ�¶�
	unsigned short int 		Battery;     //��ص�ѹ�¶�
	unsigned short int 		McuTemp;        //��Ƭ���¶�
	unsigned int 			McuTime;		//��Ƭ��ʱ��
	unsigned short int 		McuFirNum;     //��Ƭ���汾��
}TerStateStr;
#pragma pack()

/**************************************************
**	SD�������Ϣ
**	
**************************************************/
typedef struct
{
	unsigned char 			SDCardCheck;                //SD���Ƿ����
	unsigned char 			SDCardType;           		//SD������
	unsigned short int 		SDCardTotalSpace;    		//SD�������� 
	unsigned short int 		SDCardSurplusSpace;         //SD��ʣ������
	unsigned char 			SDCardState;                //SD��״̬
	unsigned char 			NC;                         //Ԥ��һ���ֽ�
}TerSDCardStr;


/**************************************************
**	ϵͳ���У������Ϣ
**	
***************************************************/
#pragma pack(1)
typedef struct
{

	unsigned int 			ACCTotal;                    			//ACC��ʱ��  
	double		 			ACCTotalHour;                			//Сʱ
	double 					TotalDistance;               			//��ʻ�����  ������λ��ЧС��
	unsigned int 			TimeSec;                     			//ϵͳʱ�� 
	double 					latitude;								//ά��
	double 					longitude;	  							//����
	unsigned short int 		altitude;       						//���θ߶�
	unsigned short int 		azimuth;	  							//����Ƕ�
	float 					speed;		            				//�ٶ�
	float  					ghdop_v;      							//��λ����
	unsigned char 			satellite_num;         	 				//ʹ����������
	unsigned char 			satellite_view_num; 					//������������
	unsigned char 			status;                 				//��λ״̬  
	unsigned char			TYear;                                  //GNSS-ʱ��-��
	unsigned char 			TMon;									//GNSS-ʱ��-��
	unsigned char 			TDay;									//GNSS-ʱ��-��
	unsigned char 			THour;									//GNSS-ʱ��-ʱ
	unsigned char			TMin;									//GNSS-ʱ��-��
	unsigned char 			TSec;                                   //GNSS-ʱ��-��                           
	unsigned int            gnss_time;              				//��λʱ��
	unsigned char 			latitude_ns;            				//ά�ȱ�ʶ
	unsigned char 			longitude_ew;           				//���ȱ�ʶ
}SysRunDataStr;
#pragma pack()



//���ֽ���������
typedef union  
{
	unsigned short int		IntII;					//
	unsigned char			TTbyte[2];				//
}IntToChar;


//���ֽ���������
typedef union  
{
	unsigned int			LongLL;					//
	unsigned char			TTbyte[4];				//
}LongToChar;


/**************************************
**	�̼���������
***************************************/
#pragma pack(1)
typedef struct 
{
	unsigned char			UpgradeStatus;		   //0:
	unsigned char 			UpgradeFlag;           //������ʶ
 }UpgradeStatusStr;
#pragma pack()

/**************************************
**	�̼������ļ�
**************************************/
#pragma pack(1)
typedef struct
{
	unsigned int 			FileTotalLen;           //�ļ��ܳ��ȣ����ֽ�Ϊ��λ��
	unsigned short int 		FileVerfy;				//�ļ�У��
	unsigned short int 		FileCurrentFram;	    //
	unsigned short int 		FileTotalFram;   		//
	unsigned short int 		FileLen[100];           //
	unsigned char 			FileBuf[100][1024];   	//�ļ�������
}UpgradeFileStr;
#pragma pack()




/**************************************
**	˵��:
**  ͨ����������ST86����
**	���յ������ݰ�
**************************************/
#pragma pack(1)
typedef struct
{
	unsigned int 			FileLen;           			//�����ļ��ܳ���
	unsigned short int 		FileVerfy;					//�ļ�У��ֵ������У���
	unsigned int 			CurrentLen;					//��ǰ����
	unsigned int 			CurrentFramNum;   			//��ǰ���ݰ�   
}UpgradeST86FileStr;
#pragma pack()



#endif




/*******************************File End******************************/





