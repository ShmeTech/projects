
/***********************************************
** 	FileName:
**	Time:
************************************************/

#ifndef PRODATA_H
#define PRODATA_H



unsigned char DecToHex(unsigned char dec);
unsigned char HexToDec(unsigned char bcd);
int LookForStr(unsigned char *buf,unsigned char *str,int datasize);
unsigned char Int16ToStr(unsigned short int number,char *str);
void AsciiToData(char *des, char *src, int size);
unsigned char CalcCrc8(unsigned char *start, int size);
unsigned char BccVerify(unsigned char *buf,unsigned int len);
unsigned char VerfySum(unsigned char *dest,unsigned short int len);
unsigned char AsciiToHex(unsigned char a,unsigned char b);
double ComputeDistance(double lat1, double lng1, double lat2, double lng2);
unsigned short int CheckSum16(unsigned char *buf,unsigned int len);
unsigned short int CheckSum16B(unsigned char *buf,unsigned int len)	;

void CharToStr(unsigned char n,unsigned char *buf,unsigned char len);
int UInt32ToStr(unsigned int n,unsigned char *buf,unsigned char len);
unsigned char CheckServerAddr(unsigned char *Data,unsigned int Len);



#endif




/***************************File End**************************/


