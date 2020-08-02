
/*****************************************************************
**	FileName:Rtc.c
**	Time:
******************************************************************/

#include "include.h"


/************************ȫ�ֱ���**************************/

static SysTimeStr 			SysTime;             //�豸ʱ��

static SysTimeStr			SetTime;

static unsigned char 		SleepStatus; 				   //ϵͳ����״̬
     
/*****************************************************************
**	��������:void CalculateWeek(SysTimeStr  *t)
**	��������:��������ʱ��������
**	�������:
**	�������:��
*****************************************************************/

void CalculateWeek(SysTimeStr  *t)
{
	short int c,y,m,w,d;
	unsigned short int tm_year;

	tm_year = 2000 + t->Year;
   	c = tm_year / 100;
	y = tm_year % 100;
	m = t->Mon;
	if(m == 1)
	{
		c = (tm_year - 1) / 100;
		y = (tm_year - 1) % 100;
		m = 13;
	}
	if(m == 2)
	{
		c = (tm_year - 1) / 100;
		y = (tm_year - 1) % 100;
		m = 14;
	}
	d = t->MDay;
	w = y + (y / 4) + (c / 4) - 2 * c + (26 * (m + 1) / 10) + d - 1;
	if(w < 0)
		w =((w % 7) + 7);
	t->WDay = w % 7;

}



/*******************************************************************
**	��������:
**	��������:ʱ��
********************************************************************/

SysTimeStr UnixToCalendar(unsigned int t)
{
	SysTimeStr t_tm;
    unsigned int i;
    unsigned int tmp;
    unsigned int tmp1;  
	
    tmp = t % 86400;
    t_tm.Hour = tmp / 3600;                    //ʱ
    t_tm.Min = tmp % 3600 / 60;                //��
    t_tm.Sec = tmp % 3600 % 60;                //��

    tmp = t / 86400;                           //������

    for(i = 2000;i < 2255;i++)       //ȷ������
    {
        if(i % 400==0 || (i % 4==0 && i % 100 != 0))
        {
            if(tmp < 366)
                break;        //�˳�forѭ��
            tmp -= 366;
        }
        else
        {
            if(tmp < 365)
                break;      //�˳�forѭ��
            tmp -= 365;
        }
    }

    t_tm.Year = i - 2000;   //��

    for(i = 1;i <= 12;i++)       //ȷ����
    {
        switch(i)
        {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                if(tmp < 31)
                {
                    t_tm.Mon = i;
                    i = 13;
                }
                else
                {
                    tmp -= 31;
                }
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                if(tmp < 30)
                {
                    t_tm.Mon = i;
                    i = 13;
                }
                else
                {
                    tmp -= 30;
                }
                break;
            case 2:
                tmp1 = t_tm.Year + 2000;
                if(tmp1 % 400==0 || (tmp1 % 4==0 && tmp1 % 100 != 0))
                {
                    if(tmp1 < 29)
                    {
                        t_tm.Mon = i;
                        i = 13;
                    }
                    else
                    {
                        tmp -= 29;
                    }
                }
                break;
        }
    }
	
    t_tm.MDay = tmp + 1;   //  ����
	CalculateWeek(&t_tm);
	t_tm.TimeSec = t;
	return t_tm;
}



/**********************************************************************
**	��������:
**	��������:ʱ��תʱ���
**	�������:
**	�������:
**********************************************************************/

unsigned int CalendarToUnix(SysTimeStr t)
{
    unsigned int rv;
    unsigned int tmp;
    unsigned short int i;

    rv = 0;
    for(i = 0; i < t.Year;i++)
    {
        tmp = i + 2000;
        if(tmp % 400==0 || (tmp % 4==0 && tmp % 100 != 0))
        {
            rv += 366;         //����
        }
        else
        {
            rv += 365;         //������
        }
    }

    tmp = i + 2000;

    for(i = 1;i < t.Mon;i++)
    {
        switch(i)
        {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                rv += 31;
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                rv += 30;
                break;
            case 2:
                if(tmp % 400==0 || (tmp % 4==0 && tmp % 100 != 0))
                    rv += 29;        //����
                else
                    rv += 28;        //������
                break;
        }
    }

    for(i = 1;i < t.MDay;i++)
    {
        rv++;
    }

    rv *= 86400;

    rv = rv + t.Hour * 3600 + t.Min * 60 + t.Sec;

	return rv;               //
}



/******************************************************
**	��������:
** 	��������:
******************************************************/

void ProcessSysSleep(void)
{
	static unsigned char step;
	static unsigned int counter = 0;
	unsigned char tmp;
	SysConfigStr *p;
	
	p = GetSysCfgSpace();

	tmp = ReadAccState();
	
	switch(step)
	{
		case 0:
			if(tmp > 0)
				break;
			SleepStatus = 1;  //�������� 
			counter = 0;
			step++;
			break;
		case 1:
			counter++;
			if(tmp > 0)
			{
				SleepStatus = 0;   //�������� 
				step = 0;
				break;
			}
			if(counter >= p->RunTime)
			{
				SleepStatus = 2;
				SL_Print("Enter Sleep Mode");
				SaveSendQueue();							
				SL_GpioWrite(SL_GPIO_4, SL_PIN_LOW);       //�ر�GNSSģ���Դ  
				step++;
				counter = 0;
				break;
			}
			break;
		case 2:
			if(counter++ < 10)                 //�ȴ�10���ӣ���ä�����ݴ洢��� λ��          
				break;
			step++;
			SendTerSleepCmd();     					//ָ�������   
			SL_PowerDown(1);     					//�ر�     ����ط�����һ��
			//�ػ�
			break;
		case 3:
			break;
		default:
			step = 0;
			break;
	}
}


/********************************************************
**	��������:
**	��������:
*********************************************************/

void ProcessRtc(void)
{
	static unsigned char 	step = 0;
	GNSSSTR 				*p_gnss;   //������ָ������GNSS���ݿռ��ָ��
	int tmp;
	SysTimeStr 				*pTime;
	SysRunDataStr			*pRun;
	
	pTime = GetTerTimeSpace();
	pRun = GetSysRunDataSpace();
	
	switch(step)
	{
		case 0:
			if(pTime->VerifyBit == 0)    //��û�л�ȡ��ϵͳʱ��
				return;
			step++;
			SysTime.VerifyBit = 1;
			SysTime.TimeSec =  (pTime->TimeSec > pRun->TimeSec) ? pTime->TimeSec : pRun->TimeSec;    //�������ʼ��ʱ��  
			SysTime = UnixToCalendar(SysTime.TimeSec);       	//ˢ��ʱ�� 
			SendVerifyTimeCmd(&SysTime);            			//У׼һ��ʱ��
			SL_Print(">App Init Sys Time :%d,%d,%d,%d\r\n",pTime->VerifyBit,pRun->TimeSec,pTime->TimeSec,SysTime.TimeSec);  //������Ϣ
			break;
		case 1:
			SysTime.TimeSec = pTime->TimeSec;            		//ʱ������ 
			SysTime = UnixToCalendar(SysTime.TimeSec);       	//ˢ��ʱ�� 
			
			break;
		default:
			step = 0;
			break;
	}
	
	p_gnss = GetGnssDataSpace();                    //�ȶ�GNSSʱ��
	if(p_gnss->status != 'A')               		//�������λ��������ʱ��  �����Ѿ���λ
		return;
	
	tmp = (p_gnss->gnss_time > SysTime.TimeSec) ? (p_gnss->gnss_time - SysTime.TimeSec) : (SysTime.TimeSec - p_gnss->gnss_time);

	if(tmp > 3)                          //У׼ʱ��
	{
		SysTime.TimeSec = p_gnss->gnss_time;
		
		SysTime = UnixToCalendar(SysTime.TimeSec);
	}

	
	tmp = (pTime->TimeSec > SysTime.TimeSec) ? (pTime->TimeSec - SysTime.TimeSec) : (SysTime.TimeSec - pTime->TimeSec);
	if(tmp > 3)
	{
		pTime->TimeSec = SysTime.TimeSec;
		SetTime.Year = SysTime.Year;                       //�� 
		SetTime.Mon = SysTime.Mon;
		SetTime.MDay = SysTime.MDay;
		SetTime.Hour = SysTime.Hour;
		SetTime.Min = SysTime.Min;
		SetTime.Sec = SysTime.Sec;
		SetTime.VerifyBit = 1;      //У׼ʱ�� 
	}
}



/***************************************************
** ��������:
** ��������:
***************************************************/

SysTimeStr *GetSysTimeSpace(void)
{
	return &SysTime;
}




/***************************************************
** ��������:
** ��������:
***************************************************/

SysTimeStr *GetSetTimeSpace(void)
{
	return &SetTime;
}



/*************************************************
**	��������:
**	��������:
**	�������:
**	�������:��������״̬ 0:�����ߣ�1:�������ߣ�
*************************************************/

unsigned char ReadSleepStatus(void)
{	
	return SleepStatus;
}


/**********************************************
**	��������:
**	��������:
***********************************************/
void SetSysTime(unsigned int Time)
{
	SysTime = UnixToCalendar(Time);
}



/****************************File End*************************/

