// KeyGen-editplus.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <windows.h>
#include <time.h>
char ReChar(char ch);
char DeCodeChar(char ch);
void GetDReg05Code(char* pUserName, int ulen);
unsigned short* CreateTable();
void RandonRegCode(char* pRegCode, int rLen, char* pAsc, int aLen);
char* FomartStr(char* str, WORD sNum);
WORD  SearchTable(int nNum, char* pCUserName, int len);
int  SprintfChars(char* pUserName, int len, char* pCUserName, int nNum);


char ch5 = {};
unsigned short* pTable = {};
int _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned)time(NULL));
	char AscArry[36] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A,
		0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54,
		0x55, 0x56, 0x57, 0x58, 0x59, 0x5A };
	char RegCode[29] = {};
	char DRegCode[29] = {};
	char UserName[10] = {};

	RandonRegCode(RegCode, 29, AscArry, 36);
	printf("input Name:");
	scanf_s("%s", UserName, 10);
	int ulen = strlen(UserName);
	int rlen = _countof(RegCode);
	pTable = CreateTable();
	char* pAscii = (char*)malloc(sizeof(char) * 3000);

	int slen = SprintfChars(UserName, ulen, pAscii, 3000);
	WORD sNum = SearchTable(0, pAscii, slen);
	char* pCNum = (char*)malloc(sizeof(char) * 10);
	FomartStr(pCNum, sNum);
	GetDReg05Code(UserName, ulen);
	RegCode[2] = ReChar(pCNum[0]);
	RegCode[3] = ReChar(pCNum[1]);
	RegCode[4] = ReChar(ch5);
	for (int i = 0; i < 29; i++)
	{
		if ((i + 1) % 6)
		{
			char ret = DeCodeChar(RegCode[i]);
			ret >>= 1;
			if (ret >= 10)
				DRegCode[i] = ret + 55;
			else
				DRegCode[i] = ret + 48;
		}
		else
			DRegCode[i] = 0x2D;
	}
	slen = SprintfChars(DRegCode + 2, rlen - 2, pAscii, 3000);
	sNum = SearchTable(0, pAscii, slen);
	FomartStr(pCNum, sNum);
	RegCode[0] = ReChar(pCNum[0]);
	RegCode[1] = ReChar(pCNum[1]);
	printf("RegCode:");
	for (int i = 0; i < 29; i++)
	{
		printf("%C", RegCode[i]);
	}
	system("pause");
	return 0;
}
int SprintfChars(char* pUserName, int len, char* pCUserName, int nNum)
{
	int ret;
	int i = 0;
	int j = 0;
	if (len <= 0)
	{
		pCUserName = NULL;
		ret = 0;
	}
	else
	{
		while (j < len)
		{
			sprintf_s(&pCUserName[i], nNum - i, "%04X", pUserName[j++]);
			i += 4;
		}
		pCUserName[i] = 0;
		ret = i;
	}
	return ret;
}
WORD SearchTable(int nNum, char* pCUserName, int len)
{
	WORD ret;
	int index;
	char* p = pCUserName;
	if (len <= 0)
	{
		ret = nNum;
	}
	else
	{
		ret = nNum;
		while (p < pCUserName + len)
		{
			index = *p++^LOBYTE(ret);
			ret = pTable[index] ^ (ret >> 8);
		}
	}
	return ret;
}
char* FomartStr(char* str, WORD sNum)
{
	sprintf_s(str, 10, "%04X", sNum);
	return str;
}
unsigned short* CreateTable()
{
	unsigned short * pTable = (unsigned short *)malloc(0x200);
	memset(pTable, 0, 0x200);
	for (int i = 0; i < 0x100; i++)
	{
		int j = 1;
		unsigned short eax = 0xC0C1;
		while (j < 0x100)
		{
			//test edx,ecx;
			if (i&j)
				pTable[i] ^= eax;
			eax += eax;
			eax ^= 0x4003;
			j += j;
		}
	}
	return pTable;
}

char ReChar(char ch)
{
	char arr[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
		0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A };
	int len = _countof(arr);
	for (int i = 0; i < len; i++)
	{
		char tmp;
		char ret = DeCodeChar(arr[i]);
		ret >>= 1;
		if (ret >= 10)
			tmp = ret + 55;
		else
			tmp = ret + 48;
		if (tmp == ch)
		{
			return arr[i];
		}
	}
}
char DeCodeChar(char ch)
{
	//except：I/O
	char chArry[] =
	{
		50, 51, 52, 53,//	2,3,4,5,
		54, 55, 56, 57,//	6,7,8,9,
		65, 66, 67, 68,//	A,B,C,D,
		69, 70, 71, 72,//	E,F,G,H,
		74, 75, 76, 77,//	J,K,L,M,
		78, 80, 81, 82,//	N,P,Q,R,
		83, 84, 85, 86,//	S,T,U,V,
		87, 88, 89, 90 //	W,X,Y,Z		
	};

	char ret = 0;
	int v1 = 2;
	char* p = &chArry[1];
	while (v1 - 2 < 32)
	{
		if (ch == *(p - 1))
			ret = v1 - 2;
		if (ch == *(p))
			ret = v1 - 1;
		if (ch == *(p + 1))
			ret = v1;
		if (ch == *(p + 2))
			ret = v1 + 1;
		p += 4;
		v1 += 4;
	}
	return ret;
}


void GetDReg05Code(char* pUserName, int ulen)
{
	int nNum01 = 0;
	int nNum02 = 0;
	int nNum03 = 1;
	int i = 0;
	while (i < ulen - 1)
	{
		nNum01 += pUserName[i];
		nNum02 += pUserName[i + 1];
		i += 2;
	}
	if (i < ulen)
		nNum03 = pUserName[i] + 1;
	sprintf_s(&ch5, 2, "%01X", ((9 * (nNum01 + nNum02 + nNum03) + 0xA) / 3 + 36) % 16);
}
void RandonRegCode(char* pRegCode, int rLen, char* pAsc, int aLen)
{
	for (int i = 0; i < rLen; i++)
	{
		if ((i + 1) % 6 == 0)
			pRegCode[i] = 0x2D;
		else
			pRegCode[i] = pAsc[rand() % aLen];
	}
}
