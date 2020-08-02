
/***********************************************
**	FileName:
**	Time:
************************************************/

#ifndef DATATYPE_H
#define DATATYPE_H


#define BLIND_NUM  250



/***************************************************************
***��λ���ݽṹ
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
	unsigned int            gnss_time;              				//��λʱ��
	unsigned char 			latitude_ns;            				//ά�ȱ�ʶ
	unsigned char 			longitude_ew;           				//���ȱ�ʶ
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
	double 					Distance;
}DistanceProStr;

/****************************************************/
//TCPIP������Ϣ
typedef struct 
{
	unsigned char 			GprsApn[20];           					// ���뷽ʽ
	unsigned char 			GprsUser[20];          					//�����û���
	unsigned char 			GprsPassd[20];  	   					//��������
	unsigned char 			GprsNetActive;		   					//GPRS����״̬
	BOOL          			SimState;         	   					//�Ƿ���Sim��
	unsigned char 			SimIccId[25];     	   					//SIM��ICC_ID
	unsigned char			GsmSign;       	 						//GSM�ź�ֵ			
	unsigned char 			SocketId; 	   							//Socket��� 
	unsigned char 			SocketType;     	 					//Socket
	unsigned char 			ServerAddr[50];        					//ƽ̨��ַ  ��������ַ
	unsigned short int 		ServerPort;       						//�˿�    �������˿�  
	unsigned char 			UlrToIPFlag;	                        //��ַ��ʶ��0:IP��ַ��1:URL	
	unsigned char 			LocalAddr[20];         					//����IP��ַ
	unsigned short 			LocalPort;            					//���ض˿�
	unsigned char 			LinkState;             					//����״̬
	unsigned char 			FtpLinkState;							//FTP����״̬
	unsigned int  			LinkNum;              					//���Ӵ���
	unsigned char 			LinkEnable;             				//�Ƿ���������
	unsigned char 			SendState;              				//����״̬ 
	int			 			SimStatus;              				//Sim��״̬
	int						NetWorkState;           				//�ƶ�����״̬
	unsigned char			NetGprsState;           				//GPRS����״̬  
	unsigned char 			ErrorCode;              				//������ ����ʱû��ʹ�� 
	unsigned char 			ResetLink;              				//��������
	unsigned char			LinkType;								//(1)FPT ����(0)TCP/IP����  
	unsigned char			FtpAddr[50];           					//Ftp��������ַ
	unsigned char 			FtpUserName[20];       					//�û���
	unsigned char 			FtpUserPassd[20];      					//����
	unsigned char 			FtpFileName[20];       					//�ļ�����  
	unsigned short int 		FtpPort;              						//�˿�
	unsigned short int 		FtpFileLen;            					//�ļ�����
	unsigned char 			*FtpFileSapce;          				//FTP���ص��ļ�ָ��
	unsigned char 			FtpLoadFig;          					//���ر�ʶ
	
}TcpIPSocketStr;


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
	unsigned char 	Year;              								//ϵͳʱ��-��
	unsigned char 	Mon;               								//ϵͳʱ��-��
	unsigned char 	MDay;              								//ϵͳʱ��-��
	unsigned char 	WDay;              								//ϵͳʱ��-��
	unsigned char 	Hour;              								//ϵͳʱ��-ʱ
	unsigned char 	Min;               								//ϵͳʱ��-��
	unsigned char 	Sec;               								//ϵͳʱ��-��
	unsigned int 	TimeSec;           								//ϵͳʱ��-���룬��2000��1��1��0ʱ0��0�����
	unsigned char   VerifyBit;         								//ʱ��У׼  ��ʶʱ���Ƿ��Ѿ�У׼
}SysTimeStr;


/***********************************************************
**	�����ϴ��ṹ
***********************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 		DataBuf[500];     							//���ͻ�����
	unsigned short int 	DataLen;          							//���ݳ���
	unsigned char 		DataCrc;          							//У��
	unsigned char       DataBig;          							//���ݰ���ʶ(0:�������ݣ�1:ä���洢����)
	
}FiFoStr;
#pragma pack()

/*************************************************************
**	���Ͷ���
*************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int QNum;             							//���Ͷ���
	unsigned short int QWrite;           							//���Ͷ��ж�ͷ
	unsigned short int QRead;            							//���Ͷ��ж�β
	FiFoStr            QData[BLIND_NUM];       						//���Ͷ������ݻ�����
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
	unsigned char 		 WorkFlag;
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
	unsigned short       SysVol;            //ϵͳ��ѹ
	unsigned int 		 EngineWorkTime;    //����������ʱ�� 
	unsigned char        WarnValue;         //��������
	unsigned short int   TravlledSpeed;     //��ʻ�ٶ�
	unsigned int         TotalTravlled;     //����ʻ���
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
**	Can���ݽṹ(������)
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



//�����û�Э���
#pragma pack(1)
typedef struct
{
	unsigned short int   CanProtocolSerial;  //����Э����
	unsigned char        LockCardState;      //����״̬
}TerminalCanProStr;
#pragma pack()



#pragma pack(1)


//ͨѶЭ����Ϣͷ
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
	unsigned short int msg_input_type;
	unsigned short int msg_input_len;
	unsigned char      msg_input_io;
	unsigned char      msgAccState;
	unsigned char	   msgMotoState;
	unsigned short int msgFrq1;
	unsigned short int msgFrq2;
	unsigned short int msgPWM1;
	unsigned short int msgPWM2;
	unsigned short int msgInputVol1;
	unsigned short int msgInputVol2;
	unsigned short int msgPowVol;
	unsigned short int msgBatteryVol;
	unsigned int       msgACCTotalTime;
	unsigned char      msgLine;
} MsgInputStr;

#pragma pack()


//
#pragma pack(1)

typedef struct 
{
	unsigned short int msg_gps_type;                  	//
	unsigned short int msg_gps_len;                   	//
	unsigned int       msg_gps_latitude;        		//
	unsigned int       msg_gps_longitude;             	//???��
	unsigned short int msg_gps_speed;           		//?��?��
	unsigned short int msg_gps_azimuth;         		//??DD��??��
	unsigned short int msg_gps_altitude;        		//o??��???��
	unsigned char      msg_gps_year;                  	//GPS����??-?��
	unsigned char      msg_gps_mon;                   	//GPS����???a?a??
	unsigned char      msg_gps_day;									  //
	unsigned char      msg_gps_hour;                 	//
	unsigned char      msg_gps_min;                   	//
	unsigned char      msg_gps_sec;                   	//
	unsigned char      msg_gps_satellite_num;         	//
	unsigned char      msg_gps_view_num;              	//
	unsigned short int msg_gps_ghdop_v;             	//?????��??��������
	unsigned char      msg_gps_status;              	//GPS?��??���䨬?
	unsigned char      msg_gps_line;
}MsgGnssStr;

#pragma pack()



//

#pragma pack(1)


typedef struct
{
	unsigned short int MsgFarmType;                   //
	unsigned short int MsgFarmLen;                      // 

	unsigned short int MsgFarmBig;
	unsigned char 	   MsgTimeYear;
	unsigned char 	   MsgTimeMon;
	unsigned char 	   MsgTimeDay;
	unsigned char 	   MsgTimeHour;
	unsigned char 	   MsgTimeMin;
	unsigned char 	   MsgTimeSec;
	unsigned char      MsgLine;
}MsgFarmStr;

#pragma pack()


#pragma pack(1)

/**************************************************************
**	CAN���ݽṹ��(С���)
***************************************************************/
typedef struct
{
	unsigned short int 	 MsgCanWheatType;                  //
	unsigned short int 	 MsgCanWheatLen;                   //

	unsigned short int   MsgEngineRotate;      			  //������ת��
	unsigned char        MsgWaterTemp;    				  //��ȴˮ�¶�
	unsigned char        MsgEngineOil;    		          //����ѹ��
	unsigned char        MsgFuelPercent;  		          //ȼ�ϰٷֱ�
	unsigned int         MsgEngineWorkTime;               //����������ʱ��  
	unsigned short       MsgSysVal;                       //ϵͳ��ѹ
	unsigned int         MsgTotalTravlled;                //����ʻ���
	unsigned short int   MsgTravlledSpeed;                //��ʻ�ٶ� 
	unsigned short int   MsgRotate1;        	//��Ͳת�� 
	unsigned short int   MsgRotate2;        	//������ת��
	unsigned short int   MsgRotate3;        	//����������
	unsigned short int   MsgRotate4;        	//��ѡ����ת��
	unsigned short int   MsgRotate5;        	//ι�����ת��
	unsigned short int   MsgRotate6;        	//����ת��
	unsigned short int   MsgLossRatio;         //��ѡ��ʧ��
	unsigned char        MsgCabTemp;           //��ʻ���¶�
	unsigned char		 MsgReapHigh;          //���߶�
	unsigned char        MsgAperture1;         //��ɸ����
	unsigned char        MsgAperture2;         //��ɸ����
	unsigned char        MsgAperture3;         //βɸ����
	unsigned char        MsgWarnValue;         //�������� 
	unsigned char 		 MsgLine;
}MsgCanWheatStr;	
#pragma pack()

/************************************************************
**	Can���ݽṹ��(ˮ����)
************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 	 MsgCanPaddyType;                  //
	unsigned short int 	 MsgCanPaddyLen;                   //

	unsigned short int   MsgEngineRotate;      //������ת��
	unsigned char        MsgWaterTemp;    		//��ȴˮ�¶�
//	unsigned char        MsgEngineOil;    		//����ѹ��
	unsigned short int   MsgRotate1;        	//
	unsigned short int   MsgRotate2;        	//������ת��
	unsigned short int   MsgSysVol;            //ϵͳ��ѹ
	unsigned int 		 MsgEngineWorkTime;    //����������ʱ�� 
	unsigned char        MsgWarnValue;         //��������
	unsigned short int   MsgTravlledSpeed;     //��ʻ�ٶ�
	unsigned int         MsgTotalTravlled;     //����ʻ���
	unsigned char 		 MsgLine;
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

	unsigned short int   MsgEngineRotate;      //������ת��
	unsigned char        MsgWaterTemp;    		//��ȴˮ�¶�
	unsigned char        MsgEngineOil;    		//����ѹ��
	unsigned char        MsgFuelPercent;  		//ȼ�ϰٷֱ�
	unsigned int 		 MsgEngineWorkTime;    //����������ʱ�� 
	unsigned short       MsgSysVol;            //ϵͳ��ѹ
	unsigned int         MsgTotalTravlled;     //����ʻ���
	unsigned short int   MsgRotate1;        	//
	unsigned short int   MsgRotate2;        	//������ת��
	unsigned short int   MsgTravlledSpeed;     //��ʻ�ٶ�
	unsigned short int   MsgReapHigh;          //���߶�
	unsigned char        MsgWarnValue;         //��������
	unsigned char 		 MsgLine;
	
}MsgCanMaizeStr;
#pragma pack()


/**************************************************
**	Can���ݽṹ(������)
***************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 	 MsgCanTractorType;                  //��Ϣ��������  
	unsigned short int 	 MsgCanTractorLen;                   //��Ϣ�峤��

	unsigned short int	 MsgEngineRotate;		//������ת��
	unsigned char		 MsgWaterTemp; 		//��ȴˮ�¶�
	unsigned char		 MsgEngineOil; 		//����ѹ��
	unsigned char		 MsgFuelPercent;		//ȼ�ϰٷֱ�
	unsigned int		 MsgEngineWorkTime;	//����������ʱ�� 
	unsigned short		 MsgSysVol;			//ϵͳ��ѹ
	unsigned short int	 MsgTravlledSpeed; 	//��ʻ�ٶ�
	unsigned int		 MsgTotalTravlled; 	//����ʻ���
	unsigned char		 MsgWarnValue; 		//�������� 
	unsigned char 		 MsgLine;
}MsgCanTractorStr;

#pragma pack()





/***************************************************
**	STM32���ݿ�
****************************************************/
typedef struct
{
	unsigned short int TypeCode;
	unsigned short int DataLen;
	void *table;
}TerDataBlockStr;

/****************************************************
**	��STM32���ƿ�
*****************************************************/
typedef struct
{	
	unsigned char 	flag;
	void 			*block;
}TerDataTcbStr;



/***************************************************
**	��Ƭ�����ɼ���״̬���ݽṹ
***************************************************/

typedef struct
{
	unsigned char AccState;    			//ACC״̬
	unsigned char MotoState;   			//Moto״̬
	unsigned char SdCardState; 			//SD��״̬ 
	unsigned char E2PROMState; 			//EEPROM״̬
	unsigned char AntConState;    			//��������״̬
	unsigned char ShellState;  			//���״̬
	unsigned char AntShortState;   		//���߶�·״̬ 
	unsigned char CanStatus;			//Can ����״̬
	//unsigned char NC1[11];               //Ԥ��11���ֽ�
	unsigned short int PowVol;          //��Դ��ѹ
	unsigned short int Battery;         //��ص�ѹ
	unsigned short int TerTemp;         //��Ƭ��Ƭ���¶�
	unsigned short int NC2;             //Ԥ�������ֽ�
	
}TerStateStr;

/**************************************************
**	SD�������Ϣ
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
***************************************************/
#pragma pack(1)
typedef struct
{
	unsigned int 	ACCTotal;                    //ACC��ʱ��  
	double		 	ACCTotalHour;                //Сʱ
	double 			TotalDistance;               //��ʻ�����  ������λ��ЧС��
	unsigned int 	TimeSec;                     //ϵͳʱ��
}SysRunDataStr;
#pragma pack()



//���ֽ���������
typedef union  
{
	unsigned short int	IntII;

	unsigned char	TTbyte[2];
}IntToChar;


//���ֽ���������
typedef union  
{
	unsigned int	LongLL;
	unsigned char	TTbyte[4];
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
	unsigned short int 		FileCurrentFram;	
	unsigned short int 		FileTotalFram;   		//
	unsigned short int 		FileLen[100];           
	unsigned char 			FileBuf[100][1024];   	//�ļ�������
}UpgradeFileStr;
#pragma pack()

#endif

/*******************************File End******************************/


