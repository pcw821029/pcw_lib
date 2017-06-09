/*****************************************
 *	파일 이름 : strsplit.c
 *	작성자 : 박철우
 *	작성일 : 2017.06.07
 *	e-mail : pcw4844@naver.com
 *	설 명 : README 파일 참고
 * **************************************/
#include<stdio.h>
#include<string.h>

#define MAX_SPLIT_CNT 10

int strSplit(char* chpStringData, char chSeparate, char** chppStorage, int iMaxSplitCount){
	int iStringCount=0;
	char* chpSplitString = chpStringData;
	char* chpEndOfSplitString;

	chppStorage[iStringCount++] = chpSplitString;
	while(1){
		chpEndOfSplitString=strchr(chpSplitString,chSeparate);
		if(chpEndOfSplitString == NULL)
			break;
		if(iStringCount < iMaxSplitCount){
			*chpEndOfSplitString=0;
			chpSplitString=chpEndOfSplitString+1;
			chppStorage[iStringCount++]=chpSplitString;
		}else
			break;
	}   
	return iStringCount; 
}

int main()
{
	char strBuff[]="12,34,56,78,90";
	char* parts[MAX_SPLIT_CNT];
	int retSplitCnt;
	int i;

	retSplitCnt = strSplit(strBuff, ',', parts, MAX_SPLIT_CNT);
	for(i=0; i<retSplitCnt; i++)
		fprintf(stderr,"%02d. %s\n",i+1, parts[i]);
	return 0;
}
