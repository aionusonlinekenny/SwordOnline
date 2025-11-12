/*******************************************************************************
// FileName			:	ScriptFuns.cpp
// FileAuthor		:	RomanDou
// FileCreateDate	:	2002-11-19 15:58:20
// FileDescription	:	脚本指令集
// Revision Count	:	
*******************************************************************************/
#ifndef WIN32
#include <string>
#endif

#include "KWin32.h"
#include "KEngine.h"
#include "KDebug.h"
#include "KStepLuaScript.h"
#include "LuaLib.h"
#include "KScriptList.h"
#include <string.h>
#include "LuaFuns.h"
#include "KCore.h"
#include "KNpc.h"
#include "KSubWorld.h"
#include "KObjSet.h"
#include "KItemSet.h"
//#include "KNetClient.h"
#include "KScriptValueSet.h"
#include "KNpcSet.h"
#include "KPlayerSet.h"
#include "KPlayer.h"
#include "KSubWorldSet.h"
#include "KProtocolProcess.h"
#include "KBuySell.h"
#include "KTaskFuns.h"
#include "KPlayerDef.h"
#include "TaskDef.h"
#include "GameDataDef.h"
#ifdef _SERVER
//#include "KNetServer.h"
//#include "../MultiServer/Heaven/interface/iServer.h"
#include "KNewProtocolProcess.h"
#endif
#include "KSortScript.h"
#ifndef __linux
#include "Shlwapi.h"
#include "windows.h"
#include "winbase.h"
#include <direct.h>
#include "time.h"
#else
#include "unistd.h"
#endif

#ifdef _STANDALONE
#include "KSG_StringProcess.h"
#else
#include "../../Engine/Src/KSG_StringProcess.h"
#endif

#ifndef WIN32
typedef struct  _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
}	SYSTEMTIME;
typedef struct  _FILETIME
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
}	FILETIME;
#endif

inline const char* _ip2a(DWORD ip) { in_addr ia; ia.s_addr = ip; return inet_ntoa(ia); }
inline DWORD _a2ip(const char* cp) { return inet_addr(cp); }

KScriptList		g_StoryScriptList;
KStepLuaScript * LuaGetScript(Lua_State * L);
int	GetPlayerIndex(Lua_State * L);
extern int g_GetPriceToStation(int,int);
extern int g_GetPriceToWayPoint(int,int);
extern int g_GetPriceToDock(int ,int );

//BitValue = GetBit(Value, BitNo)
int LuaGetBit(Lua_State * L)
{
	int nBitValue = 0;
	int nIntValue = (int)Lua_ValueToNumber(L, 1);
	int nBitNumber = (int)Lua_ValueToNumber(L, 2);
	
	if (nBitNumber >= 32 || nBitNumber <= 0) 
		goto lab_getbit;
	nBitValue = (nIntValue & (1 << (nBitNumber - 1))) != 0;
lab_getbit:
	Lua_PushNumber(L, nBitValue);
	return 1;
}

//NewBit = SetBit(Value, BitNo, BitValue)
int LuaSetBit(Lua_State * L)
{
	int nIntValue = (int)Lua_ValueToNumber(L, 1);
	int nBitNumber = (int)Lua_ValueToNumber(L, 2);
	int nBitValue = (int)Lua_ValueToNumber(L,3);
	nBitValue = (nBitValue == 1);
	
	if (nBitNumber > 32 || nBitNumber <= 0) 
		goto lab_setbit;
	
	nIntValue = (nIntValue | (1 << (nBitNumber - 1)));
lab_setbit:
	Lua_PushNumber(L, nIntValue);
	return 1;
}

//ByteValue = GetByte(Value, ByteNo)
int LuaGetByte(Lua_State * L)
{
	int nByteValue = 0;
	int nIntValue = (int)Lua_ValueToNumber(L, 1);
	int nByteNumber = (int)Lua_ValueToNumber(L, 2);
	
	if (nByteNumber > 4 || nByteNumber <= 0) 
		goto lab_getByte;
	nByteValue = (nIntValue & (0xff << ((nByteNumber - 1) * 8) )) >> ((nByteNumber - 1) * 8);
	
lab_getByte:
	Lua_PushNumber(L, nByteValue);
	return 1;
}

//NewByte = SetByte(Value, ByteNo, ByteValue)
int LuaSetByte(Lua_State * L)
{
	BYTE * pByte =	NULL;
	int nIntValue = (int)Lua_ValueToNumber(L, 1);
	int nByteNumber = (int)Lua_ValueToNumber(L, 2);
	int nByteValue = (int)Lua_ValueToNumber(L,3);
	nByteValue = (nByteValue & 0xff);
	
	if (nByteNumber > 4 || nByteNumber <= 0) 
		goto lab_setByte;
	
	pByte = (BYTE*)&nIntValue;
	*(pByte + (nByteNumber -1)) = (BYTE)nByteValue;
	//nIntValue = (nIntValue | (0xff << ((nByteNumber - 1) * 8) )) ;
lab_setByte:
	Lua_PushNumber(L, nIntValue);
	return 1;
}
#ifdef _SERVER
int LuaModifyRepute(Lua_State *L)
{
	if (Lua_GetTopIndex(L) <1) 
		return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;
	int nValue = (int)Lua_ValueToNumber(L,1);
	
	Player[nPlayerIndex].m_cTask.SetSaveVal(TASKVALUE_REPUTE, Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_REPUTE) + nValue);
	if (nValue < 0)
	{
		char szMsg[100];
		sprintf(szMsg, "您的声望减少了%d点!", -nValue);		
//		KPlayerChat::SendSystemInfo(1, nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *) szMsg, strlen(szMsg) );
	}
	else
	{
		char szMsg[100];
		sprintf(szMsg, "您的声望增加了%d点!", nValue);		
//		KPlayerChat::SendSystemInfo(1, nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *) szMsg, strlen(szMsg) );
	}
	
	
	return 0;	
}

int LuaGetRepute(Lua_State *L)
{
	int nValue = 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		goto lab_getrepute;
	nValue = Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_REPUTE);
lab_getrepute:
	Lua_PushNumber(L, nValue);
	return 1;	
}
#endif

int GetSubWorldIndex(Lua_State * L)
{
	Lua_GetGlobal(L, SCRIPT_SUBWORLDINDEX);
	if (lua_isnil(L,Lua_GetTopIndex(L)))
        return -1;
	int nIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
    if (nIndex >= MAX_SUBWORLD || nIndex <= 0) 
	{
		_ASSERT(0);
		return -1;
	}
	if (SubWorld[nIndex].m_nIndex >= MAX_SUBWORLD || SubWorld[nIndex].m_nIndex < 0)
	{
		_ASSERT(0);
		return -1;
	}
    return nIndex;
}

//Idx = SubWorldID2Idx(dwID)
int LuaSubWorldIDToIndex(Lua_State * L)
{
	int nTargetSubWorld = -1;
	int nSubWorldID = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_subworldid2idx;
	
	nSubWorldID = (int)Lua_ValueToNumber(L, 1);
	nTargetSubWorld = g_SubWorldSet.SearchWorld(nSubWorldID);	
	
lab_subworldid2idx:
	Lua_PushNumber(L, nTargetSubWorld);
	return 1;
}


/*
Say(sMainInfo, nSelCount, sSel1, sSel2, sSel3, .....,sSeln) 
Say(nMainInfo, nSelCount, sSel1, sSel2, sSel3, .....,sSeln) 
Say(nMainInfo, nSelCount, SelTab)
如果是客户端的则不会向服务器端发送任何操作

  Say(100, 3, 10, 23,43)
  Say("选择什么？", 2, "是/yes", "否/no");
  Say("选什么呀", 2, SelTab);
*/
//**************************************************************************************************************************************************************
//												界面脚本
//**************************************************************************************************************************************************************
int LuaSelectUI(Lua_State * L)
{
	char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	char * pContent = NULL;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;
	
	if (Lua_IsNumber(L,2))
	{
		nOptionNum = (int)Lua_ValueToNumber(L,2);
	}
	else 
	{
		_ASSERT(0);
		return 0;
	}
	
	if  (Lua_IsNumber(L,1))
	{
		nMainInfo = (int)Lua_ValueToNumber(L,1);
		nDataType = 1 ;
	}
	else if (Lua_IsString(L, 1)) 	//检查主信息是字符串还是字符串标识号
	{
		strMain = (char *)Lua_ValueToString(L, 1);
		nDataType = 0 ;
	}
	else
		return 0;
	
	BOOL bStringTab = FALSE;//标识传进来的选项数据存放在一个数组中，还是许多字符串里
	
	if (Lua_IsString(L,3))
		bStringTab = FALSE;
	else if (Lua_IsTable(L, 3))
	{
		bStringTab = TRUE;
	}
	else 
	{if (nOptionNum > 0)  return 0;
	}
	
	if (bStringTab == FALSE)
	{
		//获得实际传入的选项的个数
		if (nOptionNum > nParamNum - 2) nOptionNum = nParamNum - 2;
	}
	
	if (nOptionNum > MAX_ANSWERNUM) nOptionNum = MAX_ANSWERNUM;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_SELECTDIALOG;
	UiInfo.m_bParam1 = nDataType;//主信息的类型，字符串(0)或数字(1)
	UiInfo.m_bOptionNum = nOptionNum;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	//主信息为字符串
	if (nDataType == 0)
	{
		if (strMain)
			sprintf(UiInfo.m_pContent, "%s", strMain);
		pContent = UiInfo.m_pContent;
	}
	else if (nDataType == 1) //主信息为数字标识
	{
		*(int *)UiInfo.m_pContent = nMainInfo;
		pContent = UiInfo.m_pContent + sizeof(int);
		*pContent = 0;
	}
	
	if (nOptionNum > MAX_ANSWERNUM)
		nOptionNum = MAX_ANSWERNUM;
	
	Player[nPlayerIndex].m_nAvailableAnswerNum = nOptionNum;		
	
	for (int i  = 0; i < nOptionNum; i ++)
	{	
		char  pAnswer[100];
		pAnswer[0] = 0;
		
		if (bStringTab)
		{
			Lua_PushNumber(L, i + 1);
			Lua_RawGet(L, 3);
			char * pszString = (char *)Lua_ValueToString(L, Lua_GetTopIndex(L));
			if (pszString)
			{
				g_StrCpyLen(pAnswer, pszString, 100);
			}
		}
		else 
		{
			char * pszString = (char *)Lua_ValueToString(L, i + 3);
			if (pszString)
				g_StrCpyLen(pAnswer, pszString, 100);
		}
		
		char * pFunName = strstr(pAnswer, "/");
		
		if (pFunName)
		{
			g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i], pFunName + 1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
			*pFunName = 0;
			sprintf(pContent, "%s|%s", pContent, pAnswer);
		}
		else 
		{
			strcpy(Player[nPlayerIndex].m_szTaskAnswerFun[i], "main");
			sprintf(pContent, "%s|%s", pContent, pAnswer);
		}
	}
	
	if (nDataType == 0)
		UiInfo.m_nBufferLen  = strlen(pContent);
	else 
		UiInfo.m_nBufferLen = strlen(pContent) + sizeof(int);
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	
	if (nOptionNum == 0)
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	}
	
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}


int LuaSayNew(Lua_State * L)
{
	char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	char * pContent = NULL;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;
	
	if (Lua_IsNumber(L,2))
	{
		nOptionNum = (int)Lua_ValueToNumber(L,2);
	}
	else 
	{
		_ASSERT(0);
		return 0;
	}
	
	if  (Lua_IsNumber(L,1))
	{
		nMainInfo = (int)Lua_ValueToNumber(L,1);
		nDataType = 1 ;
	}
	else if (Lua_IsString(L, 1)) 	//检查主信息是字符串还是字符串标识号
	{
		strMain = (char *)Lua_ValueToString(L, 1);
		nDataType = 0 ;
	}
	else
		return 0;
	
	BOOL bStringTab = FALSE;//标识传进来的选项数据存放在一个数组中，还是许多字符串里
	
	if (Lua_IsString(L,3))
		bStringTab = FALSE;
	else if (Lua_IsTable(L, 3))
	{
		bStringTab = TRUE;
	}
	else 
	{if (nOptionNum > 0)  return 0;
	}
	
	if (bStringTab == FALSE)
	{
		//获得实际传入的选项的个数
		if (nOptionNum > nParamNum - 2) nOptionNum = nParamNum - 2;
	}
	
	if (nOptionNum > MAX_ANSWERNUM) nOptionNum = MAX_ANSWERNUM;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_SELECTDIALOG;
	UiInfo.m_bParam1 = nDataType;//主信息的类型，字符串(0)或数字(1)
	UiInfo.m_bOptionNum = nOptionNum;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 1;
	
	//主信息为字符串
	if (nDataType == 0)
	{
		if (strMain)
			sprintf(UiInfo.m_pContent, "%s", strMain);
		pContent = UiInfo.m_pContent;
	}
	else if (nDataType == 1) //主信息为数字标识
	{
		*(int *)UiInfo.m_pContent = nMainInfo;
		pContent = UiInfo.m_pContent + sizeof(int);
		*pContent = 0;
	}
	
	if (nOptionNum > MAX_ANSWERNUM)
		nOptionNum = MAX_ANSWERNUM;
	
	Player[nPlayerIndex].m_nAvailableAnswerNum = nOptionNum;		
	
	for (int i  = 0; i < nOptionNum; i ++)
	{	
		char  pAnswer[1024];
		pAnswer[0] = 0;
		
		if (bStringTab)
		{
			Lua_PushNumber(L, i + 1);
			Lua_RawGet(L, 3);
			char * pszString = (char *)Lua_ValueToString(L, Lua_GetTopIndex(L));
			if (pszString)
			{
				g_StrCpyLen(pAnswer, pszString, 100);
			}
		}
		else 
		{
			char * pszString = (char *)Lua_ValueToString(L, i + 3);
			if (pszString)
				g_StrCpyLen(pAnswer, pszString, 100);
		}
		
		char * pFunName = strstr(pAnswer, "/");
		
		if (pFunName)
		{
			g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i], pFunName + 1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
			*pFunName = 0;
			sprintf(pContent, "%s|%s", pContent, pAnswer);
		}
		else 
		{
			strcpy(Player[nPlayerIndex].m_szTaskAnswerFun[i], "main");
			sprintf(pContent, "%s|%s", pContent, pAnswer);
		}
	}
	
	if (nDataType == 0)
		UiInfo.m_nBufferLen  = strlen(pContent);
	else 
		UiInfo.m_nBufferLen = strlen(pContent) + sizeof(int);
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	
	if (nOptionNum == 0)
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	}
	
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

int LuaSelectImage(Lua_State * L)
{
	char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	char * pContent = NULL;
	int nIdImage = 0;
	BOOL bStringTab = FALSE;	
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	
	int nParamNum = Lua_GetTopIndex(L);
	
	if (nParamNum < 2) return 0;

	if (nParamNum >= 2)
	{
		if (Lua_IsNumber(L,2))
		{
			nOptionNum = (int)Lua_ValueToNumber(L,2);
		}
		else 
		{
			_ASSERT(0);
			return 0;
		}	
		
		if  (Lua_IsNumber(L,1))
		{
			nMainInfo = (int)Lua_ValueToNumber(L,1);
			nDataType = 1 ;
		}
		else if (Lua_IsString(L,1)) 	//检查主信息是字符串还是字符串标识号
		{
			strMain = (char *)Lua_ValueToString(L,1);
			nDataType = 0 ;
		}
		else
			return 0;
			
		if (nParamNum >= 3)
		{
			nIdImage = (int)Lua_ValueToNumber(L,3);
			if (Lua_IsString(L,4))
				bStringTab = FALSE;
			else if (Lua_IsTable(L, 4))
			{
				bStringTab = TRUE;
			}
			else 
			{
			if (nOptionNum > 0)  
				return 0;
			}
		}
		else if(nParamNum >= 2)
		{
			nIdImage = 0;
			if (Lua_IsString(L,3))
				bStringTab = FALSE;
			else if (Lua_IsTable(L, 3))
			{
				bStringTab = TRUE;
			}
			else 
			{
			if (nOptionNum > 0)  
				return 0;
			}
		}
	}
	
	Player[nPlayerIndex].SetImageNpcId(nIdImage);
	
	if (bStringTab == FALSE)
	{
		if (nOptionNum > nParamNum - 2) 
		nOptionNum = nParamNum - 2;
	}
	
	if (nOptionNum > MAX_ANSWERNUM) 
		nOptionNum = MAX_ANSWERNUM;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_SELECTDIALOG;
	UiInfo.m_bParam1 = nDataType;
	UiInfo.m_bOptionNum = nOptionNum;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 2;	
	
	if (nDataType == 0)
	{
		if (strMain)
			sprintf(UiInfo.m_pContent, "%s", strMain);
		pContent = UiInfo.m_pContent;
	}
	else if (nDataType == 1) 
	{
		*(int *)UiInfo.m_pContent = nMainInfo;
		pContent = UiInfo.m_pContent + sizeof(int);
		*pContent = 0;
	}
	
	if (nOptionNum > MAX_ANSWERNUM)
		nOptionNum = MAX_ANSWERNUM;
	
	Player[nPlayerIndex].m_nAvailableAnswerNum = nOptionNum;	
	
	if (nParamNum >= 2)
	{
		for (int i  = 0; i < nOptionNum; i ++)
		{	
			char  pAnswer[1024];
			pAnswer[0] = 0;
			
			if (bStringTab)
			{

				if (nParamNum >= 3)
				{
					Lua_PushNumber(L, i + 1);
					Lua_RawGet(L, 4);
				}
				else if (nParamNum >= 2)
				{
					Lua_PushNumber(L, i + 1);
					Lua_RawGet(L, 3);
				}
				char * pszString = (char *)Lua_ValueToString(L, Lua_GetTopIndex(L));
				if (pszString)
				{
					g_StrCpyLen(pAnswer, pszString, 100);
				}
			}
			else 
			{			
				int a;			
				if (nParamNum >= 3)
				{
					a = i + 4;
				char * pszString = (char *)Lua_ValueToString(L, a);
				if (pszString)
					g_StrCpyLen(pAnswer, pszString, 100);
				}
				else if (nParamNum >= 2)
				{
					a = i + 3;
				char * pszString = (char *)Lua_ValueToString(L, a);
				if (pszString)
					g_StrCpyLen(pAnswer, pszString, 100);
				}
			}
			
			char * pFunName = strstr(pAnswer, "/");
			
			if (pFunName)
			{
				g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i], pFunName + 1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
				*pFunName = 0;
				sprintf(pContent, "%s|%s", pContent, pAnswer);
			}
			else 
			{
				strcpy(Player[nPlayerIndex].m_szTaskAnswerFun[i], "Main");
				sprintf(pContent, "%s|%s", pContent, pAnswer);
			}
		}
	}
	if (nDataType == 0)
		UiInfo.m_nBufferLen  = strlen(pContent);
	else 
		UiInfo.m_nBufferLen = strlen(pContent) + sizeof(int);
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	
	if (nOptionNum == 0)
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	}
	
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}


//PutMsg(szMsg/MsgId)
int LuaSendMessageInfo(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_MSGINFO;
	UiInfo.m_bOptionNum = 1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else 
	{
		
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

//AddGlobalNews(Newsstr)
int LuaAddGlobalNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_NORMAL;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddLocalNews(Newsstr)
int LuaAddLocalNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_NORMAL;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastLocalServer(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddGlobalCountNews(strNew/newid, time)
int LuaAddGlobalCountNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_COUNTING;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	int nTime = (int)Lua_ValueToNumber(L,2);
	
	if (nTime <= 0)
		nTime = 1;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(int *)((char *)UiInfo.m_pContent + sizeof(int)) = nTime;
		UiInfo.m_nBufferLen = sizeof(int) * 2;
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		*(int *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTime;
		UiInfo.m_nBufferLen += sizeof(int);
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddLocalCountNews(strNew/newid, time)
int LuaAddLocalCountNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_COUNTING;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	int nTime = (int)Lua_ValueToNumber(L,2);
	
	if (nTime <= 0)
		nTime = 1;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(int *)((char *)UiInfo.m_pContent + sizeof(int)) = nTime;
		UiInfo.m_nBufferLen = sizeof(int) * 2;
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		*(int *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTime;
		UiInfo.m_nBufferLen += sizeof(int);
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastLocalServer(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddGlobalTimeNews(strNew/newid, year,month,day,hour,mins)
int LuaAddGlobalTimeNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 6) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_TIMEEND;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int) + sizeof(SYSTEMTIME);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent)) + sizeof(SYSTEMTIME);
		UiInfo.m_bParam1 = 0;
	}
	
	SYSTEMTIME *pSystemTime = 	(SYSTEMTIME *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen - sizeof(SYSTEMTIME));
	memset(pSystemTime, 0, sizeof(SYSTEMTIME));
	
	SYSTEMTIME LocalTime ;
	memset(&LocalTime, 0, sizeof(SYSTEMTIME));
	
	LocalTime.wYear = (WORD)Lua_ValueToNumber(L,2);
	LocalTime.wMonth =(WORD)Lua_ValueToNumber(L,3);
	LocalTime.wDay = (WORD)Lua_ValueToNumber(L, 4);
	LocalTime.wHour = (WORD)Lua_ValueToNumber(L,5);
	LocalTime.wMinute = (WORD)Lua_ValueToNumber(L,6);
	FILETIME ft;
	FILETIME sysft;
#ifdef WIN32
	SystemTimeToFileTime(&LocalTime, &ft);
	LocalFileTimeToFileTime(&ft, &sysft);
	FileTimeToSystemTime(&sysft, pSystemTime);
#else
	memcpy(pSystemTime, &LocalTime, sizeof(LocalTime));
#endif
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddLocalTimeNews(strNew/newid, year,month,day,hour,mins)
int LuaAddLocalTimeNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 6) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_TIMEEND;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int) + sizeof(SYSTEMTIME);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent)) + sizeof(SYSTEMTIME);
		UiInfo.m_bParam1 = 0;
	}
	
	SYSTEMTIME *pSystemTime = 	(SYSTEMTIME *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen - sizeof(SYSTEMTIME));
	memset(pSystemTime, 0, sizeof(SYSTEMTIME));
	
	SYSTEMTIME LocalTime ;
	memset(&LocalTime, 0, sizeof(SYSTEMTIME));
	
	LocalTime.wYear = (WORD)Lua_ValueToNumber(L,2);
	LocalTime.wMonth =(WORD)Lua_ValueToNumber(L,3);
	LocalTime.wDay = (WORD)Lua_ValueToNumber(L, 4);
	LocalTime.wHour = (WORD)Lua_ValueToNumber(L,5);
	LocalTime.wMinute = (WORD)Lua_ValueToNumber(L,6);
	FILETIME ft;
	FILETIME sysft;
#ifdef WIN32
	SystemTimeToFileTime(&LocalTime, &ft);
	LocalFileTimeToFileTime(&ft, &sysft);
	FileTimeToSystemTime(&sysft, pSystemTime);
#else
	memcpy(pSystemTime, &LocalTime, sizeof(LocalTime));
#endif
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastLocalServer(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}
int LuaSetNpcValue(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2 ) return 0 ;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	int nValue = (int)Lua_ValueToNumber(L, 2);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC) return 0;
	Npc[nNpcIndex].m_ValueEx = nValue;
	return 0;
}
//AddGlobalNewsEx(Newsstr)
int LuaAddGlobalNewsEx(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO_1;
	UiInfo.m_bOptionNum = NEWSMESSAGE_NORMAL_1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 128);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddLocalNewsEx(Newsstr)
int LuaAddLocalNewsEx(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO_1;
	UiInfo.m_bOptionNum = NEWSMESSAGE_NORMAL_1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 128);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastLocalServer(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddGlobalCountNewsEx(strNew/newid, time)
int LuaAddGlobalCountNewsEx(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO_1;
	UiInfo.m_bOptionNum = NEWSMESSAGE_COUNTING_1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	int nTime = (int)Lua_ValueToNumber(L,2);
	
	if (nTime <= 0)
		nTime = 1;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(int *)((char *)UiInfo.m_pContent + sizeof(int)) = nTime;
		UiInfo.m_nBufferLen = sizeof(int) * 2;
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 128);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		*(int *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTime;
		UiInfo.m_nBufferLen += sizeof(int);
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddLocalCountNewsEx(strNew/newid, time)
int LuaAddLocalCountNewsEx(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO_1;
	UiInfo.m_bOptionNum = NEWSMESSAGE_COUNTING_1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	int nTime = (int)Lua_ValueToNumber(L,2);
	
	if (nTime <= 0)
		nTime = 1;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(int *)((char *)UiInfo.m_pContent + sizeof(int)) = nTime;
		UiInfo.m_nBufferLen = sizeof(int) * 2;
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 128);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		*(int *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTime;
		UiInfo.m_nBufferLen += sizeof(int);
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastLocalServer(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddGlobalTimeNewsEx(strNew/newid, year,month,day,hour,mins)
int LuaAddGlobalTimeNewsEx(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 6) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO_1;
	UiInfo.m_bOptionNum = NEWSMESSAGE_TIMEEND_1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int) + sizeof(SYSTEMTIME);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 128);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent)) + sizeof(SYSTEMTIME);
		UiInfo.m_bParam1 = 0;
	}
	
	SYSTEMTIME *pSystemTime = 	(SYSTEMTIME *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen - sizeof(SYSTEMTIME));
	memset(pSystemTime, 0, sizeof(SYSTEMTIME));
	
	SYSTEMTIME LocalTime ;
	memset(&LocalTime, 0, sizeof(SYSTEMTIME));
	
	LocalTime.wYear = (WORD)Lua_ValueToNumber(L,2);
	LocalTime.wMonth =(WORD)Lua_ValueToNumber(L,3);
	LocalTime.wDay = (WORD)Lua_ValueToNumber(L, 4);
	LocalTime.wHour = (WORD)Lua_ValueToNumber(L,5);
	LocalTime.wMinute = (WORD)Lua_ValueToNumber(L,6);
	FILETIME ft;
	FILETIME sysft;
#ifdef WIN32
	SystemTimeToFileTime(&LocalTime, &ft);
	LocalFileTimeToFileTime(&ft, &sysft);
	FileTimeToSystemTime(&sysft, pSystemTime);
#else
	memcpy(pSystemTime, &LocalTime, sizeof(LocalTime));
#endif
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddLocalTimeNewsEx(strNew/newid, year,month,day,hour,mins)
int LuaAddLocalTimeNewsEx(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 6) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO_1;
	UiInfo.m_bOptionNum = NEWSMESSAGE_TIMEEND_1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int) + sizeof(SYSTEMTIME);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 128);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent)) + sizeof(SYSTEMTIME);
		UiInfo.m_bParam1 = 0;
	}
	
	SYSTEMTIME *pSystemTime = 	(SYSTEMTIME *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen - sizeof(SYSTEMTIME));
	memset(pSystemTime, 0, sizeof(SYSTEMTIME));
	
	SYSTEMTIME LocalTime ;
	memset(&LocalTime, 0, sizeof(SYSTEMTIME));
	
	LocalTime.wYear = (WORD)Lua_ValueToNumber(L,2);
	LocalTime.wMonth =(WORD)Lua_ValueToNumber(L,3);
	LocalTime.wDay = (WORD)Lua_ValueToNumber(L, 4);
	LocalTime.wHour = (WORD)Lua_ValueToNumber(L,5);
	LocalTime.wMinute = (WORD)Lua_ValueToNumber(L,6);
	FILETIME ft;
	FILETIME sysft;
#ifdef WIN32
	SystemTimeToFileTime(&LocalTime, &ft);
	LocalFileTimeToFileTime(&ft, &sysft);
	FileTimeToSystemTime(&sysft, pSystemTime);
#else
	memcpy(pSystemTime, &LocalTime, sizeof(LocalTime));
#endif
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastLocalServer(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}
//AddNote(str/strid)
int LuaAddNote(Lua_State * L)
{
	char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1) 
		return 0;
	
	int nParam2 = 0;
	
	BYTE nType = (BYTE)Lua_ValueToNumber(L,1);
	
	if  (Lua_IsNumber(L,2))
	{
		nMainInfo = (int)Lua_ValueToNumber(L,2);
		nDataType = 1 ;
	}
	else if (Lua_IsString(L, 2)) 	//检查主信息是字符串还是字符串标识号
	{
		strMain = (char *)Lua_ValueToString(L, 2);
		nDataType = 0 ;
	}
	else
		return 0;
	
	if (nParamNum > 2)
	{
		nParam2 = (int)Lua_ValueToNumber(L, 3);
	}
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NOTEINFO;
	UiInfo.m_bParam1 = nDataType;//主信息的类型，字符串(0)或数字(1)
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	
	UiInfo.m_bOptionNum = 0;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	switch (nType)
	{
	case 1:
		UiInfo.m_Select = 1;		
		break;
	case 2:
		UiInfo.m_Select = 2;			
		break;
	}	
	//主信息为字符串
	if (nDataType == 0)
	{
		if (strMain)
			sprintf(UiInfo.m_pContent, "%s", strMain);
		int nLen = strlen(strMain);
		*(int*)(UiInfo.m_pContent + nLen) = nParam2;
		UiInfo.m_nBufferLen = nLen + sizeof(int);
	}
	else if (nDataType == 1) //主信息为数字标识
	{
		*(int *)UiInfo.m_pContent = nMainInfo;
		*(int *)(UiInfo.m_pContent + sizeof(int)) = nParam2;
		UiInfo.m_nBufferLen = sizeof(int) + sizeof(int);
	}
	
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

/*
**
**格式1:Talk(SentenceNum, CallBack-Fun(结束后的回调函数), sTalk1, sTalk2, sTalk3, sTalk4,...sTalkN);  
Talk(SentenceNum, CallBack-Fun(结束后的回调函数), nTalk1, nTalk2,nTalk3,nTalk4,...nTalkN);  
**格式2:Talk(SentenceNum, CallBack-Fun, SentenceTab);
**例子:Talk(3,"EndTalk", "玩家：请问现在几点钟了？", "雇员：现在5点钟了","太谢谢你了！");
**
*/

int LuaTalkUI(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	char * pContent = NULL;
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3) 
		return 0;
	
	if (Lua_IsNumber(L,1))
	{
		nOptionNum = (int)Lua_ValueToNumber(L,1);
	}
	else 
	{
		_ASSERT(0);
		return 0;
	}
	
	const char * pCallBackFun = Lua_ValueToString(L,2);
	
	//检查主信息是字符串还是字符串标识号
	
	if  (Lua_IsNumber(L,3))
	{
		nDataType = 1 ;
	}
	else if (Lua_IsString(L, 3)) 
	{
		nDataType = 0 ;
	}
	else
		return 0;
	
	
	//获得实际传入的选项的个数
	if (nOptionNum > nParamNum - 2) 
		nOptionNum = nParamNum - 2;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_TALKDIALOG;
	UiInfo.m_bParam1 = nDataType;//主信息的类型，字符串(0)或数字(1)
	UiInfo.m_bOptionNum = nOptionNum;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select = 0;
	pContent = UiInfo.m_pContent;
	pContent[0] = 0;
	size_t nContentLen = 0;
	for (int i  = 0; i < nOptionNum; i ++)
	{	
		const char * pString = NULL;
		if (!nDataType)//StringInfo
		{
			pString = Lua_ValueToString(L, i + 3);
			if (nContentLen  + strlen(pString) >= MAX_SCIRPTACTION_BUFFERNUM)
			{
				nOptionNum = i;
				UiInfo.m_bOptionNum = nOptionNum;
				break;
			}
			nContentLen += strlen(pString);
			sprintf(pContent, "%s%s|", pContent, pString);
		}
		else
		{
			int j = (int)Lua_ValueToNumber(L, i + 3);
			sprintf(pContent, "%s%d|", pContent, j);
		}
	}
	UiInfo.m_nBufferLen  = strlen(pContent);
	
	if (!pCallBackFun || strlen(pCallBackFun) <= 0)
	{
		UiInfo.m_nParam = 0;
		Player[nPlayerIndex].m_nAvailableAnswerNum = 0;
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else
	{
		UiInfo.m_nParam = 1;
		Player[nPlayerIndex].m_nAvailableAnswerNum = 1;
		g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[0], pCallBackFun, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	}
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
	
}


int LuaIncludeFile(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;
	
	if (Lua_IsString(L,1))
	{
		const char * pFileName = lua_tostring(L,1);	
		char lszCurrentDirectory[MAX_PATH];
		int nLen = 0;
		if (pFileName[0] != '\\' && pFileName[0] != '/')
		{
			getcwd(lszCurrentDirectory, MAX_PATH);
			nLen = strlen(lszCurrentDirectory);
			if (lszCurrentDirectory[nLen - 1] == '\\' || lszCurrentDirectory[nLen - 1] == '/')
				lszCurrentDirectory[nLen - 1] = 0;
#ifdef WIN32
			g_StrCat(lszCurrentDirectory, "\\");
			g_StrCat(lszCurrentDirectory, (char*)pFileName);
#else
			g_StrCat(lszCurrentDirectory, "/");
			g_StrCat(lszCurrentDirectory, (char*)pFileName);
			for (int i = 0; lszCurrentDirectory[i]; i++)
			{
				if (lszCurrentDirectory[i] == '\\')
					lszCurrentDirectory[i] = '/';
			}
#endif
		}
		else
		{
			g_GetRootPath(lszCurrentDirectory);
			nLen = strlen(lszCurrentDirectory);
			if (lszCurrentDirectory[nLen - 1] == '\\' || lszCurrentDirectory[nLen - 1] == '/')
				lszCurrentDirectory[nLen - 1] = 0;
#ifdef WIN32
			g_StrCat(lszCurrentDirectory, "\\");
			g_StrCat(lszCurrentDirectory, (char*)pFileName + 1);
#else
			g_StrCat(lszCurrentDirectory, "/");
			g_StrCat(lszCurrentDirectory, (char*)pFileName + 1);
			for (int i = 0; lszCurrentDirectory[i]; i++)
			{
				if (lszCurrentDirectory[i] == '\\')
					lszCurrentDirectory[i] = '/';
			}
#endif
		}
		strlwr(lszCurrentDirectory + nLen);
		lua_dofile(L, lszCurrentDirectory);
		return 0;
	}
	else
		return 0;
}

//**************************************************************************************************************************************************************
//												任务脚本
//**************************************************************************************************************************************************************

int LuaGetTaskValue(Lua_State * L)
{
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0) 
	{
		int nValue = (int)Player[nPlayerIndex].m_cTask.GetSaveVal((int)Lua_ValueToNumber(L,1));		
		Lua_PushNumber(L, nValue);
	}
	else
		Lua_PushNil(L);
	
	return 1;
}

int LuaSetTaskValue(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	int nValueIndex = (int)Lua_ValueToNumber(L, 1);
	int nValue = (int)Lua_ValueToNumber(L, 2);
	if (nValueIndex == 600 || nValueIndex == 700) return 0;
	if (nPlayerIndex <= 0) return 0;
	Player[nPlayerIndex].m_cTask.SetSaveVal(nValueIndex, nValue);
	if (nValueIndex == TASK_CHAT_TONGKIM)
	{
		PLAYER_TONGKIM_SYNC     IDC;

		IDC.ProtocolType = s2c_playertongkimsync;
		IDC.nValueIndex = nValueIndex; //nTaskID ma
		IDC.nValue = nValue;
		//IDC.bCheck = 1;
		#ifdef _SERVER
		if (g_pServer)
			 g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(PLAYER_TONGKIM_SYNC));
		#endif
	}
	return 0;
}


int LuaRandomNew(Lua_State * L)
{

	int nMin = (int)Lua_ValueToNumber(L, 1);
	int nMax = (int)Lua_ValueToNumber(L, 2);
	if (nMin > nMax) 
		return 0;
	

	int grandommax = g_Random(nMax - nMin + 1 );
	int nValue =  grandommax + nMin; 
	//printf("Test: %d - %d - %d - %d\n",nMin,nMax,grandommax,nValue);
	Lua_PushNumber(L, nValue);
	
	return 1;
}
int LuaRANDOMC(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);

	if (nParamNum < 2)
		return 0;
	
	srand( (unsigned)time( NULL ) );
	if (Lua_IsTable(L,1))
	{
		Lua_PushNumber(L, (int)Lua_ValueToNumber(L,2));
		Lua_RawGet(L, 1);
		Lua_PushNumber(L, (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L)));
	}
	else if (Lua_IsTable(L,2))
	{
		int nResult = ::GetRandomNumber(1, (int)Lua_ValueToNumber(L,1));
		Lua_PushNumber(L, nResult);
		Lua_RawGet(L, 2);
		Lua_PushNumber(L, (int)Lua_ValueToNumber(L, (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L))));
	}
	else
	{
		int nResult = ::GetRandomNumber(1, nParamNum);
		Lua_PushNumber(L, (int)Lua_ValueToNumber(L, nResult));
	}
	return 1;
}

int LuaRANDOM(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);

	if (nParamNum < 1)
		return 0;
	int nResult = 0;
	if (nParamNum > 1)
		nResult = GetRandomNumber((int)Lua_ValueToNumber(L, 1), (int)Lua_ValueToNumber(L, 2));
	else
		nResult = GetRandomNumber(0, (int)Lua_ValueToNumber(L, 1));
	Lua_PushNumber(L, nResult);
	return 1;
}

#ifndef _SERVER
#define MAX_TEMPVALUENUM_INCLIENT 500
int g_TempValue[MAX_TEMPVALUENUM_INCLIENT];
#endif




int LuaGetTempTaskValue(Lua_State * L)
{
	int nTempIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
	
#ifdef _SERVER
	if(nTempIndex >= MAX_TEMP_TASK)
	{
		Lua_PushNil(L) ;
		return 1;
	}
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0) 
	{
		Lua_PushNil(L);
		return 1;
	}
	
	int nValue = Player[nPlayerIndex].m_cTask.GetClearVal(nTempIndex);
	Lua_PushNumber(L, nValue);
#else
	
	if (nTempIndex >= 0 && nTempIndex < MAX_TEMPVALUENUM_INCLIENT)
		Lua_PushNumber(L, g_TempValue[nTempIndex]);
	else 
		Lua_PushNil(L);
#endif
	return 1;
}

int LuaSetTempTaskValue(Lua_State * L)
{
	int nTempIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L) - 1);
	int nValue = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
#ifdef _SERVER	
	Lua_GetGlobal(L, SCRIPT_PLAYERINDEX);
	int nPlayerIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
	if (nPlayerIndex <= 0) return 0;
	Player[nPlayerIndex].m_cTask.SetClearVal(nTempIndex, nValue);
#else
	g_TempValue[nTempIndex] = nValue;
#endif
	return 0;
}

#ifdef _SERVER
//---------------------------------交易、买卖、打开储藏箱-----------------------
//Sale(id)
//------------------------------------------------------------------------------
int LuaSale(Lua_State * L)
{
	int nTop = Lua_GetTopIndex(L);
	if (nTop <= 0) return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nShopId = (int)Lua_ValueToNumber(L,1);

		int nSlect = 0;

		if (nTop >= 2)
		{
		nSlect = (int)Lua_ValueToNumber(L,2);
		}

		BuySell.OpenSale(nPlayerIndex, nShopId - 1,nSlect);
	}
	return 0;
}

int LuaTrade(Lua_State * L)
{
	//Question 未写
	return 0;
}

int LuaOpenBox(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	BYTE	NetCommand = (BYTE)s2c_openstorebox;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &NetCommand, sizeof(BYTE));
	return 0;
}

//---------------------------------时间任务-------------------------------------
//SetTimer(Time, TimerTaskId)
int LuaSetTimer(Lua_State  * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2 ) return 0;
	int nPlayerIndex  = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	Player[nPlayerIndex].SetTimer((DWORD) (int)Lua_ValueToNumber(L, 1), (int)Lua_ValueToNumber(L,2));
	return 0;
}

int LuaStopTimer(Lua_State * L)
{
	int nPlayerIndex  = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	Player[nPlayerIndex].CloseTimer();
	return 0;
}

int LuaGetCurTimerId(Lua_State * L)
{
	int nPlayerIndex  = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	int nTimerId = Player[nPlayerIndex].m_TimerTask.GetTaskId();
	Lua_PushNumber(L, nTimerId);
	return 1;
}

int LuaGetRestTime(Lua_State * L)
{
	int nPlayerIndex  = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
	{
		Lua_PushNil(L);
		return 1;
	}
	int nRestTime = Player[nPlayerIndex].m_TimerTask.GetRestTime();//m_dwTimeTaskTime - g_SubWorldSet.GetGameTime();
	
	if (nRestTime > 0)
		Lua_PushNumber(L, nRestTime);
	else
		Lua_PushNumber(L, 0);
	
	return 1;
}

int LuaGetMissionRestTime(Lua_State * L)
{
	int RestTime = 0;
	if (Lua_GetTopIndex(L) >= 2)
	{
		int nSubWorldIndex = GetSubWorldIndex(L);
		if (nSubWorldIndex >= 0) 
		{
			int nMissionId = (int)Lua_ValueToNumber(L, 1);
			int nTimerId = (int)Lua_ValueToNumber(L, 2);
			
			if (nMissionId < 0 || nTimerId < 0 )
				goto lab_getmissionresttime;
			
			KMission Mission;
			Mission.SetMissionId(nMissionId);
			KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
			if (pMission)
			{
				RestTime = (int)pMission->GetTimerRestTimer(nTimerId);
			}
		}
	}
	
lab_getmissionresttime:
	Lua_PushNumber(L, RestTime);
	return 1;
}


//**************************************************************************************************************************************************************
//												组队脚本
//**************************************************************************************************************************************************************
int LuaIsLeader(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0 ) 
	{
		if (Player[nPlayerIndex].m_cTeam.m_nFlag && Player[nPlayerIndex].m_cTeam.m_nFigure == TEAM_CAPTAIN)
			Lua_PushNumber(L,1);
		else 
			Lua_PushNumber(L,0);
		
	}
	else 
		Lua_PushNumber(L, 0);
	return 1;
}

int LuaGetTeamId(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (Player[nPlayerIndex].m_cTeam.m_nFlag)
			Lua_PushNumber(L, Player[nPlayerIndex].m_cTeam.m_nID);
		else Lua_PushNil(L);
	}
	else 
		Lua_PushNil(L);
	return 1;
}

int LuaGetTeamSize(Lua_State * L)
{
	int nTeamSize = 0;
	int nTeamId = -1;
	if (Lua_GetTopIndex(L) >= 1)
	{
		nTeamId = Lua_ValueToNumber(L, 1);
	}
	else
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			if (Player[nPlayerIndex].m_cTeam.m_nFlag)
				nTeamId = Player[nPlayerIndex].m_cTeam.m_nID;
			else 
				nTeamId = -1;
		}
	}
	
	if (nTeamId < 0)
		nTeamSize = 0;
	else
		nTeamSize = g_Team[nTeamId].m_nMemNum + 1;
	Lua_PushNumber(L, nTeamSize);
	return 1;
}




int LuaLeaveTeam(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (Player[nPlayerIndex].m_cTeam.m_nFlag)
		{
			PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
			sLeaveTeam.ProtocolType = c2s_teamapplyleave;
			Player[nPlayerIndex].LeaveTeam((BYTE*)&sLeaveTeam);
		}
	}
	return 0;
}

int LuaSetCreateTeamOption(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Player[nPlayerIndex].m_cTeam.SetCreatTeamFlag(nPlayerIndex, (int)Lua_ValueToNumber(L, 1) > 0);
	}
	return 0;
}
//**************************************************************************************************************************************************************
//												聊天消息脚本
//**************************************************************************************************************************************************************

int	LuaMsgToPlayer(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0) 
	{
		const char *  szMsg = Lua_ValueToString(L,1);
		if (szMsg)
			KPlayerChat::SendSystemInfo(1, nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *) szMsg, strlen(szMsg) );
	}
	
	return 0;
}

int LuaMsgToTeam(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0	) return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (Player[nPlayerIndex].m_cTeam.m_nID >= 0)
		{
			const char * szMsg = Lua_ValueToString(L,1);
			Player[nPlayerIndex].SendTeamMessage(Player[nPlayerIndex].m_cTeam.m_nID, szMsg);
		}
	}
	return 0;
}

int LuaMsgToSubWorld(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;
	
	const char * szMsg = Lua_ValueToString(L,1);
	if (szMsg)
		KPlayerChat::SendSystemInfo(0, 0, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *)szMsg, strlen(szMsg) );
	return 0;
}

int LuaMsgToWorld(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;

	int nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	 return 0;
	
	const char * szMsg = Lua_ValueToString(L,1);
	if (szMsg)
		KPlayerChat::SendSystemInfo(99, nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *)szMsg, strlen(szMsg) );
	return 0;
}

int LuaMsgToTong(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;

	int nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	 return 0;
	
	const char * szMsg = Lua_ValueToString(L,1);
	if (szMsg)
		KPlayerChat::SendSystemInfo(101, nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *)szMsg, strlen(szMsg) );
	return 0;
}








int LuaMsgToWorldById(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 1 ) return 0;


	
	const char * szMsg = Lua_ValueToString(L,1);
	int nIdMap = (int)Lua_ValueToNumber(L, 2);

	int nIdxMap = g_SubWorldSet.SearchWorld(nIdMap);

	if (nIdxMap != -1)
	{

	if (szMsg)
		KPlayerChat::SendSystemInfo(100, nIdxMap, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *)szMsg, strlen(szMsg) );

	}

	return 0;
}



int LuaMsgToAroundRegion(Lua_State * L)
{
	
	return 0;
}

//**************************************************************************************************************************************************************
//												主角脚本
//**************************************************************************************************************************************************************

//**************************************************************************************************************************************************************
//												主角脚本
//**************************************************************************************************************************************************************

/*功能：让玩家进入新的一个游戏世界
nPlayerIndex:主角的Index
nSubWorldIndex:游戏世界id
nPosX:
nPosY:
*/
//NewWorld(WorldId, X,Y)
int  LuaEnterNewWorld(Lua_State * L) 
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) 
		return 0;

	int nResult = 0;
	if (Lua_GetTopIndex(L) >= 3)
	{
		DWORD dwWorldId = (DWORD)Lua_ValueToNumber(L, 1);

		nResult = Npc[Player[nPlayerIndex].m_nIndex].ChangeWorld(dwWorldId, (int)Lua_ValueToNumber(L,2) * 32, (int)Lua_ValueToNumber(L,3) * 32);
	}
	Lua_PushNumber(L, nResult);
	return 1;
}

//SetPos(X,Y)
int LuaSetPos(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount != 2) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	
	int nX = (int) Lua_ValueToNumber(L,1);
	int nY = (int) Lua_ValueToNumber(L,2);
	
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].SetPos(nX * 32, nY * 32);
	}
	return 0;
}

int LuaGetPos(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex > 0)
	{
		int nPosX = 0;
		int nPosY = 0;
		Npc[Player[nPlayerIndex].m_nIndex].GetMpsPos(&nPosX, &nPosY);
		Lua_PushNumber(L, nPosX);
		Lua_PushNumber(L, nPosY);
		Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex); 
	}
	else
		return 0;
	return 3;
}
int LuaDropMoney(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;

	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	int nIndex = 0;
	int nSubWorldIndex = 0;
	int nX, nY;
	int nMoney = 0;

	if (nParamNum > 1)
	{
		nIndex = Player[nPlayerIndex].m_nIndex;
		nMoney = (int)Lua_ValueToNumber(L, 1);
	}
	if (nParamNum > 2)
	{
		nIndex = (int)Lua_ValueToNumber(L, 1);
		nMoney = (int)Lua_ValueToNumber(L, 2);
	}

	if (nIndex <= 0)
	{
		nIndex = Player[nPlayerIndex].m_nIndex;
	}
	if (nMoney <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Npc[nIndex].GetMpsPos(&nX, &nY);
	nSubWorldIndex = Npc[nIndex].GetSubWorldIndex();

	if (nSubWorldIndex < 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	POINT	ptLocal;
	KMapPos	Pos;
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[nSubWorldIndex].GetFreeObjPos(ptLocal);

	Pos.nSubWorld = nSubWorldIndex;
	SubWorld[nSubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y,
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY,
		&Pos.nOffX, &Pos.nOffY);
	int nObj = ObjSet.AddMoneyObj(Pos, nMoney * 1);
	if (nObj > 0)
	{
		Object[nObj].SetItemBelong(nPlayerIndex);
		Lua_PushNumber(L,nObj);
		return 1;
	}
	return 0;
}

//W,X,Y = GetWorldPos()


int LuaGetPlayerDef(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{	
		int idx = Player[nPlayerIndex].m_nIndex;
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentPhysicsResist); 
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentPoisonResist);
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentColdResist);
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentFireResist);
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentLightResist);
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentLifeMax);
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentManaMax);
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentStaminaMax);
		
	}
	else
	{
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		return 8;
	}
	return 8;
}

int LuaGetPlayerSpeed(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{	
		int idx = Player[nPlayerIndex].m_nIndex;
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentAttackSpeed);
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentCastSpeed); 
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentRunSpeed);
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentWalkSpeed);
		
	}
	else
	{
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		return 4;
	}
	return 4;
}

int LuaGetPlayerReduce(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{	
		int idx = Player[nPlayerIndex].m_nIndex;
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentPiercePercent);
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentPoisonTimeReducePercent); 
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentFreezeTimeReducePercent);
		Lua_PushNumber(L, (int)Npc[idx].m_CurrentStunTimeReducePercent);
		
	}
	else
	{
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		return 4;
	}
	return 4;
}


int LuaGetNewWorldPos(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex > 0)
	{
		int nPosX = 0;
		int nPosY = 0;
		Npc[Player[nPlayerIndex].m_nIndex].GetMpsPos(&nPosX, &nPosY);

		int nSubWorldIndex = Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex;
		int nSubWorldID = 0;
		if (nSubWorldIndex >= 0 && nSubWorldIndex < MAX_SUBWORLD)
		{
			nSubWorldID = SubWorld[nSubWorldIndex].m_SubWorldID;
		}

		Lua_PushNumber(L, nSubWorldID); 
		Lua_PushNumber(L, ((int)(nPosX / 32)));
		Lua_PushNumber(L, ((int)(nPosY / 32)));
	}
	else
	{
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		return 3;
	}
	return 3;
}


int LuaGetNewWorldPosIdx(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	
	int nPlayerIndex = (int)Lua_ValueToNumber(L,1);
	if (nPlayerIndex > 0)
	{
		int nPosX = 0;
		int nPosY = 0;
		Npc[nPlayerIndex].GetMpsPos(&nPosX, &nPosY);

		int nSubWorldIndex = Npc[nPlayerIndex].m_SubWorldIndex;
		int nSubWorldID = 0;
		if (nSubWorldIndex >= 0 && nSubWorldIndex < MAX_SUBWORLD)
		{
			nSubWorldID = SubWorld[nSubWorldIndex].m_SubWorldID;
		}

		Lua_PushNumber(L, nSubWorldID); 
		Lua_PushNumber(L, ((int)(nPosX / 32)));
		Lua_PushNumber(L, ((int)(nPosY / 32)));
	}
	else
	{
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		return 3;
	}
	return 3;
}
// TimeBox
int LuaOpenTimeBox(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	int nParamNum = Lua_GetTopIndex(L);
	int nTime = 0;	
	const char * szAction;
	const char * Title;

	if (nParamNum < 1)
		return 0;
	if (nParamNum > 4)
	{
		Title = Lua_ValueToString(L, 1);
		nTime = (int)Lua_ValueToNumber(L, 2);
		szAction = Lua_ValueToString(L, 3);
		char * szScript = (char *)Lua_ValueToString(L, 4);
		Player[nPlayerIndex].m_dwTimeBoxId = g_FileName2Id(szScript);
	}
	else
	{
		Title = Lua_ValueToString(L, 1);
		nTime = (int)Lua_ValueToNumber(L, 2);
		szAction = Lua_ValueToString(L, 3);
		Player[nPlayerIndex].m_dwTimeBoxId = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;
	}


	S2C_TIME_BOX NetCommand;
	NetCommand.ProtocolType = s2c_timebox;
	strcpy(NetCommand.Value, Title);
	NetCommand.Value1 = nTime;
	strcpy(NetCommand.Value2, szAction);
	if(g_pServer && Player[nPlayerIndex].m_nNetConnectIdx != -1)
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx,&NetCommand,sizeof(S2C_TIME_BOX));
	
	return 0;
}

int LuaGetFromToward(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	

	int nSubWorldId = 0;


	Player[nPlayerIndex].GetRevID(&nSubWorldId);

	Lua_PushNumber(L,nSubWorldId);

	return 1;
}



int LuaDropItem(Lua_State *L)
{
int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	int nIndex = 0;
	int nSubWorldIndex = 0;
	int nX, nY;
	int nIdx = 0;

	if (nParamNum > 1)
	{
		nIdx = (int)Lua_ValueToNumber(L, 1);
	}
	if (nParamNum > 2)
	{
		nIndex = (int)Lua_ValueToNumber(L, 1);
		nIdx = (int)Lua_ValueToNumber(L, 2);
	}

	if (nIndex <= 0)
		nIndex = Player[nPlayerIndex].m_nIndex;

	if (nIdx <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Npc[nIndex].GetMpsPos(&nX, &nY);
	nSubWorldIndex = Npc[nIndex].GetSubWorldIndex();

	if (nSubWorldIndex < 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	if (nIdx)
	{
		POINT	ptLocal;
		KMapPos	Pos;
		KObjItemInfo	sInfo;
		ptLocal.x = nX;
		ptLocal.y = nY;
		SubWorld[nSubWorldIndex].GetFreeObjPos(ptLocal);

		Pos.nSubWorld = nSubWorldIndex;
		SubWorld[nSubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y,
			&Pos.nRegion, &Pos.nMapX, &Pos.nMapY,
			&Pos.nOffX, &Pos.nOffY);
		sInfo.m_nItemID = nIdx;
		sInfo.m_nItemWidth = Item[nIdx].GetWidth();
		sInfo.m_nItemHeight = Item[nIdx].GetHeight();
		sInfo.m_nMoneyNum = 0;
		strcpy(sInfo.m_szName, Item[nIdx].GetName());
		sInfo.m_nColorID = Item[nIdx].GetKind();
		sInfo.m_nMovieFlag = 1;
		sInfo.m_nSoundFlag = 1;

		int nObj = ObjSet.Add(Item[nIdx].GetObjIdx(), Pos, sInfo);
		if (nObj > 0)
		{
		//	if (Item[nIndex].LockPick())
		//	{
		//		Object[nObj].SetEntireBelong(nPlayerIndex);
		//	}
		//	else
			{
				Object[nObj].SetItemBelong(nPlayerIndex);
			}
			Lua_PushNumber(L, nObj);
			return 1;
		}
	}
	return 0;
}

/*AddEventItem(id)*/
KTabFile g_EventItemTab;
int LuaAddEventItem(Lua_State *L)
{
	// modify by spe 03/06/13
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	int nEventId = 0;
	int nYear = 0,nMonth = 0,nDay = 0,nHour = 0;
	if (Lua_IsNumber(L, 1))
	{
		nEventId = (int)Lua_ValueToNumber(L,1);
	}
	else
	{
		char * szEventItm = (char *)Lua_ValueToString(L,1);
		
		if (!g_EventItemTab.GetInteger(szEventItm, "具体类别", 0, &nEventId))
		{
			Lua_PushNumber(L,0);
			return 1;
		}
	}

	


	int nLevel = 0;

	if (!g_EventItemTab.GetInteger(nEventId+2, "Level", 0, &nLevel))
	{
			Lua_PushNumber(L,0);
			return 1;
	}


	if (nLevel == 2)
	{
		Lua_PushNumber(L,0);
		return 1;
	}


	int nWidth,nHeight;

	if (!ItemSet.Get_SizeItem(item_task,nEventId,0,0,&nWidth,&nHeight))
	{
		Lua_PushNumber(L,0);
        return 1;
	}



   int nIdxStask = Player[nPlayerIndex].m_ItemList.GetIdxTaskItemHaveFreeStask(nEventId);

   if (nIdxStask > 0 && nIdxStask < MAX_ITEM && Item[nIdxStask].GetVersion() >= 1 && Item[nIdxStask].GetVersion() < 200)
   {

   Item[nIdxStask].SetVersion(Item[nIdxStask].GetVersion()+1);

   PLAYER_ITEM_UPDATE_VERSION	IDC;
	    IDC.ProtocolType = s2c_playerupdateitemvertion;
	    IDC.m_ID = Item[nIdxStask].GetID();
		IDC.m_Vesion = Item[nIdxStask].GetVersion();

	    if (g_pServer)
		    g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(PLAYER_ITEM_UPDATE_VERSION));


   }

   else

   {
   

	int	nx, ny;
	if (!Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(nWidth, nHeight, &nx, &ny))
	{
		return 1;
	    Lua_PushNumber(L,0);
	}

	
	int nIndex = ItemSet.Add(item_task, 0, 0, 0, nEventId, 0, 0, 1, 0);
	
		
	if (nIndex <= 0)
	{
		printf("Add [%s] [%s]\n",Player[nPlayerIndex].m_AccoutName,Player[nPlayerIndex].m_PlayerName);
		Lua_PushNumber(L, 0);
		return 1;

	}

	if (nWidth != Item[nIndex].GetWidth() || nHeight != Item[nIndex].GetHeight())
	{
		printf("Loi Add KT [%d %d] [%d %d]\n",nWidth,nHeight,Item[nIndex].GetWidth(),Item[nIndex].GetHeight());
		Lua_PushNumber(L,0);
		return 1;
	}

	
	int		x, y;
	if (Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &x, &y))
	{
		Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_equiproom, x, y);
		Lua_PushNumber(L,nIndex);
		return 1;
	}


   }

   Lua_PushNumber(L,0);
	
	return 1;
}



int LuaAddEventItemMagic(Lua_State *L)
{
	// modify by spe 03/06/13
	if (Lua_GetTopIndex(L) < 5)
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	int nEventId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nEventId = (int)Lua_ValueToNumber(L,1);
	}
	else
	{
		char * szEventItm = (char *)Lua_ValueToString(L,1);
		
		if (!g_EventItemTab.GetInteger(szEventItm, "具体类别", 0, &nEventId))
		{
			Lua_PushNumber(L,0);
			return 1;
		}
	}



	int nLevel = 0;

	if (!g_EventItemTab.GetInteger(nEventId+2, "Level", 0, &nLevel))
	{
			Lua_PushNumber(L,0);
			return 1;
	}


	if (nLevel != 2)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	

	int nWidth,nHeight;

	if (!ItemSet.Get_SizeItem(item_task,nEventId,0,0,&nWidth,&nHeight))
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	int	nx, ny;
	if (!Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(nWidth, nHeight, &nx, &ny))
	{
		Lua_PushNumber(L,0);
		return 1;
	}


	int nSeri = (int)Lua_ValueToNumber(L,2);

	int nMagicId = (int)Lua_ValueToNumber(L,3);

	int nMagicLevel = (int)Lua_ValueToNumber(L,4);

	int nMagicItem = (int)Lua_ValueToNumber(L,5);

	int nItemLevel[6];
	
	ZeroMemory(nItemLevel, sizeof(nItemLevel));

	nItemLevel[0] = nMagicId;
	nItemLevel[1] = nMagicLevel;
	nItemLevel[2] = nMagicItem;


	int nIndex = ItemSet.Add(item_task, nSeri, 0, 0, nEventId, 0, nItemLevel, 1, 0);
	
		
	if (nIndex <= 0)
	{
		printf("Add 2 [%s] [%s]\n",Player[nPlayerIndex].m_AccoutName,Player[nPlayerIndex].m_PlayerName);
		Lua_PushNumber(L, 0);
		return 1;
	}

	if (nWidth != Item[nIndex].GetWidth() || nHeight != Item[nIndex].GetHeight())
	{
		printf("Loi Add KT 2 [%d %d] [%d %d]\n",nWidth,nHeight,Item[nIndex].GetWidth(),Item[nIndex].GetHeight());
		Lua_PushNumber(L, 0);
		return 1;
	}

	
	int		x, y;
	if (Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &x, &y))
	{
		Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_equiproom, x, y);
		Lua_PushNumber(L, 1);
		return 1;
	}


	Lua_PushNumber(L,0);
	
	return 1;
}







int LuaAddItemPink(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 26)
	{
		g_DebugLog("[Script]使用AddItem参数数量不符!");
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nItemClass		= (int)Lua_ValueToNumber(L, 1);
	int nDetailType		= (int)Lua_ValueToNumber(L, 2);
	int nParticularType	= (int)Lua_ValueToNumber(L, 3);
	int nLevel			= (int)Lua_ValueToNumber(L, 4);
	int nSeries			= (int)Lua_ValueToNumber(L, 5);
	int nLuck			= (int)Lua_ValueToNumber(L, 6);


	int nLuck1		= (int)Lua_ValueToNumber(L, 7);
	int nLuck2		= (int)Lua_ValueToNumber(L, 8);
	int nLuck3	= (int)Lua_ValueToNumber(L, 9);
	int nLuck4			= (int)Lua_ValueToNumber(L, 10);
	int nLuck5			= (int)Lua_ValueToNumber(L, 11);
	int nLuck6			= (int)Lua_ValueToNumber(L, 12);

	int nItemLevel[6];

	
	int LevelMagic1 = Lua_ValueToNumber(L, 13);
	int IdMagic1 = Lua_ValueToNumber(L, 14);
	int LevelMagic2 = Lua_ValueToNumber(L, 15);
	int IdMagic2 = Lua_ValueToNumber(L, 16);
	int LevelMagic3 = Lua_ValueToNumber(L, 17);
	int IdMagic3 = Lua_ValueToNumber(L, 18);
	int LevelMagic4 = Lua_ValueToNumber(L, 19);
	int IdMagic4 = Lua_ValueToNumber(L, 20);
	int LevelMagic5 = Lua_ValueToNumber(L, 21);
	int IdMagic5 = Lua_ValueToNumber(L, 22);
	int LevelMagic6 = Lua_ValueToNumber(L, 23);
	int IdMagic6 = Lua_ValueToNumber(L, 24);

	int IsExit = Lua_ValueToNumber(L, 25);
	int RandomSeed = Lua_ValueToNumber(L, 26);


	if (nLuck1 < 1)
		nLuck1 = 1;

	if (nLuck1 > 100)
		nLuck1 = 100;



	if (nLuck2 < 1)
		nLuck2 = 1;

	if (nLuck2 > 100)
		nLuck2 = 100;



	if (nLuck3 < 1)
		nLuck3 = 1;

	if (nLuck3 > 100)
		nLuck3 = 100;



	if (nLuck4 < 1)
		nLuck4 = 1;

	if (nLuck4 > 100)
		nLuck4 = 100;



	if (nLuck5 < 1)
		nLuck5 = 1;

	if (nLuck5 > 100)
		nLuck5 = 100;



	if (nLuck6 < 1)
		nLuck6 = 1;

	if (nLuck6 > 100)
		nLuck6 = 100;




	ZeroMemory(nItemLevel, sizeof(nItemLevel));

	
	if (LevelMagic1 <= 0)
	{
	nItemLevel[0] = 0;
	}
	else
	{
	nItemLevel[0] = (LevelMagic1-1) * 10 + (nLuck1-1) * 100 + IdMagic1 * 10000;		
	}





	if (LevelMagic2 <= 0)
	{
	nItemLevel[1] = 0;
	}
	else
	{
	nItemLevel[1] = (LevelMagic2-1)  * 10 + (nLuck2-1) * 100 + IdMagic2 * 10000;		
	}



	if (LevelMagic3 <= 0)
	{
	nItemLevel[2] = 0;
	}
	else
	{
	nItemLevel[2] = (LevelMagic3-1)  * 10 + (nLuck3-1) * 100 + IdMagic3 * 10000;		
	}



	if (LevelMagic4 <= 0)
	{
	nItemLevel[3] = 0;
	}
	else
	{
	nItemLevel[3] = (LevelMagic4-1)  * 10 + (nLuck4-1) * 100 + IdMagic4 * 10000;		
	}





	if (LevelMagic5 <= 0)
	{
	nItemLevel[4] = 0;
	}
	else
	{
	nItemLevel[4] = (LevelMagic5-1)  * 10 + (nLuck5 - 1) * 100 + IdMagic5 * 10000;		
	}




	if (LevelMagic6 <= 0)
	{
	nItemLevel[5] = 0;
	}
	else
	{
	nItemLevel[5] = (LevelMagic6-1)  * 10 + (nLuck6-1) * 100 + IdMagic6 * 10000;		
	}






	if (nItemClass != item_equip)
	{
		Lua_PushNumber(L,0);
		return 1;
	}




	int nWidth,nHeight;

	if (!ItemSet.Get_SizeItem(nItemClass,nDetailType,nParticularType,nLevel,&nWidth,&nHeight))
	{
		Lua_PushNumber(L,0);
        return 1;
	}


	int	nx, ny;
	if (!Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(nWidth, nHeight, &nx, &ny))
	{
		Lua_PushNumber(L,0);
		return 1;
	}


	int nIndex = 0;
	
	if (IsExit)
	{
		nIndex = ItemSet.AddExit(nItemClass, nSeries, nLevel, nLuck, nDetailType, nParticularType,  nItemLevel, g_SubWorldSet.GetGameVersion(),RandomSeed);
	}
	else
	{
        nIndex = ItemSet.Add(nItemClass, nSeries, nLevel, nLuck, nDetailType, nParticularType,  nItemLevel, g_SubWorldSet.GetGameVersion());
	}
	
	
	
	
	
	if (nIndex <= 0)
	{
		printf("Add [%s] [%s]\n",Player[nPlayerIndex].m_AccoutName,Player[nPlayerIndex].m_PlayerName);
		Lua_PushNumber(L,0);
		return 1;
	}

	if (nWidth != Item[nIndex].GetWidth() || nHeight != Item[nIndex].GetHeight())
	{
		printf("Loi Add KT [%d %d] [%d %d]\n",nWidth,nHeight,Item[nIndex].GetWidth(),Item[nIndex].GetHeight());
		Lua_PushNumber(L,0);
		return 1;
	}

    
	
	int		x, y;
	if (Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &x, &y))
	{
		Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_equiproom, x, y);
		Lua_PushNumber(L,nIndex);
		return 1;
	}


    Lua_PushNumber(L,0);	
	return 1;
	
}













///////////




int LuaAddItemEx(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 14)
	{
		g_DebugLog("[Script]使用AddItem参数数量不符!");
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nItemClass		= (int)Lua_ValueToNumber(L, 1);
	int nDetailType		= (int)Lua_ValueToNumber(L, 2);
	int nParticularType	= (int)Lua_ValueToNumber(L, 3);
	int nLevel			= (int)Lua_ValueToNumber(L, 4);
	int nSeries			= (int)Lua_ValueToNumber(L, 5);
	int nLuck			= (int)Lua_ValueToNumber(L, 6);
	int nItemLevel[6];
	
	ZeroMemory(nItemLevel, sizeof(nItemLevel));

	nItemLevel[0] = (int)Lua_ValueToNumber(L, 7);
	nItemLevel[1] = (int)Lua_ValueToNumber(L, 8);
	nItemLevel[2] = (int)Lua_ValueToNumber(L, 9);
	nItemLevel[3] = (int)Lua_ValueToNumber(L, 10);
	nItemLevel[4] = (int)Lua_ValueToNumber(L, 11);
	nItemLevel[5] = (int)Lua_ValueToNumber(L, 12);

	int nVersion = (int)Lua_ValueToNumber(L, 13);
	int nRandomSeed = (int)Lua_ValueToNumber(L, 14);
	



	if (nItemClass == item_task)
	{
		Lua_PushNumber(L,0);
		return 1;
	}




	int nWidth,nHeight;

	if (!ItemSet.Get_SizeItem(nItemClass,nDetailType,nParticularType,nLevel,&nWidth,&nHeight))
	{
		Lua_PushNumber(L,0);
        return 1;
	}


	int	nx, ny;
	if (!Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(nWidth, nHeight, &nx, &ny))
	{
		Lua_PushNumber(L,0);
		return 1;
	}


	
	int nIndex = ItemSet.AddExit(nItemClass, nSeries, nLevel, nLuck, nDetailType, nParticularType,  nItemLevel, g_SubWorldSet.GetGameVersion(), nRandomSeed);
	if (nIndex <= 0)
	{
		printf("Add EX [%s] [%s]\n",Player[nPlayerIndex].m_AccoutName,Player[nPlayerIndex].m_PlayerName);
		Lua_PushNumber(L,0);
		return 1;
	}

	if (nWidth != Item[nIndex].GetWidth() || nHeight != Item[nIndex].GetHeight())
	{
		printf("Loi Add EX KT [%d %d] [%d %d]\n",nWidth,nHeight,Item[nIndex].GetWidth(),Item[nIndex].GetHeight());
		Lua_PushNumber(L,0);
		return 1;
	}

    
	
	int		x, y;
	if (Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &x, &y))
	{
		Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_equiproom, x, y);
		Lua_PushNumber(L,nIndex);
		return 1;
	}


    Lua_PushNumber(L,0);	
	return 1;
	
}


//////////

// int LuaSetMateName(Lua_State * L)
// {
	// int nPlayerIndex = GetPlayerIndex(L);
	// if (nPlayerIndex <= 0) return 0;
	// const char* szName = Lua_ValueToString(L,1);
	// SET_MATE_NAME	SyncMate;
	// SyncMate.ProtocolType = s2c_setmatename;
	// strcpy(SyncMate.szName,szName);
	// g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &SyncMate, sizeof(SET_MATE_NAME));
	// return 0;
// }


/*AddItem(nItemClass, nDetailType, nParticualrType, nLevel, nSeries, nLuck, nItemLevel..6)*/
int LuaAddItem(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 7)
	{
		g_DebugLog("[Script]使用AddItem参数数量不符!");
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nItemClass		= (int)Lua_ValueToNumber(L, 1);
	int nDetailType		= (int)Lua_ValueToNumber(L, 2);
	int nParticularType	= (int)Lua_ValueToNumber(L, 3);
	int nLevel			= (int)Lua_ValueToNumber(L, 4);
	int nSeries			= (int)Lua_ValueToNumber(L, 5);
	int nLuck			= (int)Lua_ValueToNumber(L, 6);
	
	int nItemLevel[6];
	// Time Item
	int nYear = 0;
	int nMonth = 0;
	int nDay = 0;
	int nHour = 0;
	// End
	ZeroMemory(nItemLevel, sizeof(nItemLevel));
	nItemLevel[0] = (int)Lua_ValueToNumber(L, 7);
	
	if (nParamNum >= 12)
	{
		nItemLevel[1] = (int)Lua_ValueToNumber(L, 8);
		nItemLevel[2] = (int)Lua_ValueToNumber(L, 9);
		nItemLevel[3] = (int)Lua_ValueToNumber(L, 10);
		nItemLevel[4] = (int)Lua_ValueToNumber(L, 11);
		nItemLevel[5] = (int)Lua_ValueToNumber(L, 12);
	}
	else
	{
		for (int i = 0; i < 5; i ++)
			nItemLevel[i + 1] = nItemLevel[0];
	}



	if (nItemClass == item_task)
	{
		Lua_PushNumber(L,0);
		return 1;
	}




	int nWidth,nHeight;

	if (!ItemSet.Get_SizeItem(nItemClass,nDetailType,nParticularType,nLevel,&nWidth,&nHeight))
	{
		//printf("Dung o day sao!!\n");
		Lua_PushNumber(L,0);
        return 1;
	}


	int	nx, ny;
	if (!Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(nWidth, nHeight, &nx, &ny))
	{
		//printf("Dung o day ak!! \n");
		Lua_PushNumber(L,0);
		return 1;
	}


	
	int nIndex = ItemSet.Add(nItemClass, nSeries, nLevel, nLuck, nDetailType, nParticularType,  nItemLevel, g_SubWorldSet.GetGameVersion());
	if (nIndex <= 0)
	{
		printf("Add [%s] [%s]\n",Player[nPlayerIndex].m_AccoutName,Player[nPlayerIndex].m_PlayerName);
		Lua_PushNumber(L,0);
		return 1;
	}

	if (nWidth != Item[nIndex].GetWidth() || nHeight != Item[nIndex].GetHeight())
	{
		printf("Loi Add KT [%d %d] [%d %d]\n",nWidth,nHeight,Item[nIndex].GetWidth(),Item[nIndex].GetHeight());
		Lua_PushNumber(L,0);
		return 1;
	}

    
//	printf("Test: %d - %d \n", Item[nIndex].GetDurability(), Item[nIndex].GetMaxDurability());
	int		x, y;
	if (Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &x, &y))
	{
		Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_equiproom, x, y);
		Lua_PushNumber(L,nIndex);
		return 1;
	}


    Lua_PushNumber(L,0);	
	return 1;
	
}



int LuaCheckFreeBoxItem(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 4)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nItemClass		= (int)Lua_ValueToNumber(L, 1);
	int nDetailType		= (int)Lua_ValueToNumber(L, 2);
	int nParticularType	= (int)Lua_ValueToNumber(L, 3);
	int nLevel			= (int)Lua_ValueToNumber(L, 4);
	

	int nWidth,nHeight;

	if (!ItemSet.Get_SizeItem(nItemClass,nDetailType,nParticularType,nLevel,&nWidth,&nHeight))
    {
		Lua_PushNumber(L,0);
		return 1;
	}


	int	nx, ny;

	if (!Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(nWidth, nHeight, &nx, &ny))
	{
		Lua_PushNumber(L,0);
		return 1;
	}


	Lua_PushNumber(L,1);
	return 1;
	
}





int LuaGetPlayerItemCount(Lua_State * L)
{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
		
			int nCount = Player[nPlayerIndex].m_ItemList.GetPlayerItemNum();
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,0);
		}

	
	return 1;
}

int LuaGetPlayerItemBlueCount(Lua_State * L)
{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
		
			int nCount = Player[nPlayerIndex].m_ItemList.GetPlayerItemBlueNum();
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,0);
		}

	
	return 1;
}






















int LuaGetItemIdxInCheckBoxByNum(Lua_State * L)
{

	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 5)
	{
		Lua_PushNumber(L,-1);
		return 1;
	}

		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
		
			int nGenre = (int)Lua_ValueToNumber(L,1);
			int nDetailType = (int)Lua_ValueToNumber(L,2);
			int nParticulType = (int)Lua_ValueToNumber(L,3);
			int nLevel = (int)Lua_ValueToNumber(L,4);
			int nNum = (int)Lua_ValueToNumber(L,5);
			int nIdx = Player[nPlayerIndex].m_ItemList.GetItemIdxInCheckBoxByNum(nGenre,nDetailType,nParticulType,nLevel,nNum);
			Lua_PushNumber(L,nIdx);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}

	
	return 1;
}





int LuaGetPlayerItemIdxByNum(Lua_State * L)
{

	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
		
			int nNum = (int)Lua_ValueToNumber(L,1);
			int nIdx = Player[nPlayerIndex].m_ItemList.GetPlayerItemIdxByNum(nNum);
			Lua_PushNumber(L,nIdx);
		}
		else 
		{
			Lua_PushNumber(L,0);
		}

	
	return 1;
}




int LuaGetBindItemIdx(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nIdx = (int)Lua_ValueToNumber(L,1);
			if (nIdx > 0 && nIdx < MAX_ITEM)
			{
				Lua_PushNumber(L,Item[nIdx].GetBindItem());
			}
			else
			{
				Lua_PushNumber(L,0);
			}
		}
		else 
		{
		Lua_PushNumber(L,0);
		}	
	return 1;
}





int LuaGetCuongHoa(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nIdx = (int)Lua_ValueToNumber(L,1);
			if (nIdx > 0 && nIdx < MAX_ITEM)
			{
				Lua_PushNumber(L,Item[nIdx].GetNgoaiTrang());
			}
			else
			{
				Lua_PushNumber(L,0);
			}
		}
		else 
		{
		Lua_PushNumber(L,0);
		}	
	return 1;
}



int LuaGetGetPramaItemIdx(Lua_State * L)
{

	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
	{
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		return 23;
	}

		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
		
			int nIdx = (int)Lua_ValueToNumber(L,1);


			if (nIdx > 0 && nIdx < MAX_ITEM)
			{
				Lua_PushNumber(L,Item[nIdx].GetGenre());
				Lua_PushNumber(L,Item[nIdx].GetDetailType());
				Lua_PushNumber(L,Item[nIdx].GetParticular());
				Lua_PushNumber(L,Item[nIdx].GetLevel());
				Lua_PushNumber(L,Item[nIdx].GetSeries());
				Lua_PushNumber(L,Item[nIdx].m_GeneratorParam.uRandomSeed);
				Lua_PushNumber(L,Item[nIdx].m_GeneratorParam.nGeneratorLevel[0]);
				Lua_PushNumber(L,Item[nIdx].m_GeneratorParam.nGeneratorLevel[1]);
				Lua_PushNumber(L,Item[nIdx].m_GeneratorParam.nGeneratorLevel[2]);
				Lua_PushNumber(L,Item[nIdx].m_GeneratorParam.nGeneratorLevel[3]);
				Lua_PushNumber(L,Item[nIdx].m_GeneratorParam.nGeneratorLevel[4]);
				Lua_PushNumber(L,Item[nIdx].m_GeneratorParam.nGeneratorLevel[5]);
				Lua_PushNumber(L,Item[nIdx].m_GeneratorParam.nVersion);
				Lua_PushNumber(L,Item[nIdx].m_GeneratorParam.nLuck);
				Lua_PushNumber(L,Player[nPlayerIndex].m_ItemList.GetPlayerItemPostIdx(nIdx,1));
				Lua_PushNumber(L,Player[nPlayerIndex].m_ItemList.GetPlayerItemPostIdx(nIdx,2));
				Lua_PushNumber(L,Player[nPlayerIndex].m_ItemList.GetPlayerItemPostIdx(nIdx,3));
				Lua_PushNumber(L,Player[nPlayerIndex].m_ItemList.GetHSD(nIdx,0));
				Lua_PushNumber(L,Player[nPlayerIndex].m_ItemList.GetHSD(nIdx,1));
				Lua_PushNumber(L,Player[nPlayerIndex].m_ItemList.GetHSD(nIdx,2));
				Lua_PushNumber(L,Player[nPlayerIndex].m_ItemList.GetHSD(nIdx,3));
				Lua_PushNumber(L,Item[nIdx].GetBindItem());
				Lua_PushNumber(L,Item[nIdx].GetNgoaiTrang());
			}

			else
			{

		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
			}
		}
		else 
		{
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		}	
	return 23;
}

int LuaAddItemPrama(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 17)
	{
		g_DebugLog("[Script]使用AddItem参数数量不符!");
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nItemClass		= (int)Lua_ValueToNumber(L, 1);
	int nDetailType		= (int)Lua_ValueToNumber(L, 2);
	int nParticularType	= (int)Lua_ValueToNumber(L, 3);
	int nLevel			= (int)Lua_ValueToNumber(L, 4);
	int nSeries			= (int)Lua_ValueToNumber(L, 5);
	int nVesion			= (int)Lua_ValueToNumber(L, 13);
	int nLuck			= (int)Lua_ValueToNumber(L, 14);

	int uRandomSeed     = (int)Lua_ValueToNumber(L, 6);

	int nItemLevel[6];
	
	
	ZeroMemory(nItemLevel, sizeof(nItemLevel));
	nItemLevel[0] = (int)Lua_ValueToNumber(L, 7);
	
	
		nItemLevel[1] = (int)Lua_ValueToNumber(L, 8);
		nItemLevel[2] = (int)Lua_ValueToNumber(L, 9);
		nItemLevel[3] = (int)Lua_ValueToNumber(L, 10);
		nItemLevel[4] = (int)Lua_ValueToNumber(L, 11);
		nItemLevel[5] = (int)Lua_ValueToNumber(L, 12);

	int nAddPlace = (int)Lua_ValueToNumber(L, 15);
	int nAddX = (int)Lua_ValueToNumber(L, 16);
	int nAddY = (int)Lua_ValueToNumber(L, 17);

	

	int nWidth,nHeight;

	if (!ItemSet.Get_SizeItem(nItemClass,nDetailType,nParticularType,nLevel,&nWidth,&nHeight))
	{
		printf("Khong co kich thuoc Item  [%s] [%s]\n",Player[nPlayerIndex].m_AccoutName,Player[nPlayerIndex].m_PlayerName);
		Lua_PushNumber(L,0);
        return 1;
	}



	
	int nIndex = ItemSet.Add(nItemClass, nSeries, nLevel, nLuck, nDetailType, nParticularType,  nItemLevel, g_SubWorldSet.GetGameVersion());
	if (nIndex <= 0)
	{
		printf("Add Item Erro [%s] [%s]\n",Player[nPlayerIndex].m_AccoutName,Player[nPlayerIndex].m_PlayerName);
		Lua_PushNumber(L,0);
		return 1;
	}

	if (nWidth != Item[nIndex].GetWidth() || nHeight != Item[nIndex].GetHeight())
	{
		printf("Loi Add KT [%d %d] [%d %d]\n",nWidth,nHeight,Item[nIndex].GetWidth(),Item[nIndex].GetHeight());
		Lua_PushNumber(L,0);
		return 1;
	}



	Item[nIndex].m_GeneratorParam.uRandomSeed = uRandomSeed;
	Item[nIndex].m_GeneratorParam.nGeneratorLevel[0] = nItemLevel[0];
	Item[nIndex].m_GeneratorParam.nGeneratorLevel[1] = nItemLevel[1];
	Item[nIndex].m_GeneratorParam.nGeneratorLevel[2] = nItemLevel[2];
	Item[nIndex].m_GeneratorParam.nGeneratorLevel[3] = nItemLevel[3];
	Item[nIndex].m_GeneratorParam.nGeneratorLevel[4] = nItemLevel[4];
	Item[nIndex].m_GeneratorParam.nGeneratorLevel[5] = nItemLevel[5];
	Item[nIndex].m_GeneratorParam.nVersion = nVesion;
	Item[nIndex].m_GeneratorParam.nLuck = nLuck;

    
	
	
	if (!Player[nPlayerIndex].m_ItemList.Add(nIndex, nAddPlace, nAddX, nAddY))
	{
		printf("Loi Add Item List KT [%d %d %d]\n",nAddPlace,nAddX,nAddY);
		Lua_PushNumber(L,0);
		return 1;
	}


    Lua_PushNumber(L,nIndex);
	return 1;
	
}























int LuaGetItemCountInCheckBox(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) >= 4)
	{
	
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{

			int nGenre = (int)Lua_ValueToNumber(L,1);
			int nDetailType = (int)Lua_ValueToNumber(L,2);
			int nParticulType = (int)Lua_ValueToNumber(L,3);
			int nLevel = (int)Lua_ValueToNumber(L,4);

			int nCount = Player[nPlayerIndex].m_ItemList.GetItemInCheckBoxNum(nGenre,nDetailType,nParticulType,nLevel);
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}
	}
	else
	{
		Lua_PushNumber(L , -1);
	}
	
	return 1;
}

int LuaGetItemCountAll(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) >= 4)
	{
	
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{

			int nGenre = (int)Lua_ValueToNumber(L,1);
			int nDetailType = (int)Lua_ValueToNumber(L,2);
			int nParticulType = (int)Lua_ValueToNumber(L,3);
			int nLevel = (int)Lua_ValueToNumber(L,4);

			int nCount = Player[nPlayerIndex].m_ItemList.CheckItemCountAll(nGenre,nDetailType,nParticulType,nLevel);
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}
	}
	else
	{
		Lua_PushNumber(L , -1);
	}
	
	return 1;
}

int LuaGetTimeItem(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) >= 1)
	{
	
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{

			int nTimeItem = (int)Lua_ValueToNumber(L,1);
			int nCount = Player[nPlayerIndex].m_ItemList.CheckTimeItemCount(nTimeItem);
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}
	}
	else
	{
		Lua_PushNumber(L , -1);
	}
	
	return 1;
}

int LuaGetTimeItemAndGold(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) >= 1)
	{
	
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{

			int nTimeItem = (int)Lua_ValueToNumber(L,1);
			int nCount = Player[nPlayerIndex].m_ItemList.CheckTimeItemCountAndGold(nTimeItem);
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}
	}
	else
	{
		Lua_PushNumber(L , -1);
	}
	
	return 1;
}

int LuaCheckItemBlockInCheckBox(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) >= 4)
	{
	
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{

			int nGenre = (int)Lua_ValueToNumber(L,1);
			int nDetailType = (int)Lua_ValueToNumber(L,2);
			int nParticulType = (int)Lua_ValueToNumber(L,3);
			int nLevel = (int)Lua_ValueToNumber(L,4);
			int nCount = Player[nPlayerIndex].m_ItemList.CheckItemBlockInCheckBox(nGenre,nDetailType,nParticulType, nLevel);
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}
	}
	else
	{
		Lua_PushNumber(L , -1);
	}
	
	return 1;
}






int LuaDelPhiPhong(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) >= 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nCount = Player[nPlayerIndex].m_ItemList.DelPhiPhong();
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}

	}
	else
	{
		Lua_PushNumber(L , -1);
	}


	return 1;
}


int LuaDeleteAllItem(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) >= 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nCount = Player[nPlayerIndex].m_ItemList.DeleteAllItem();
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}

	}
	else
	{
		Lua_PushNumber(L , -1);
	}


	return 1;
}


int LuaDeleteAllItemInCheckBox(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) >= 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nCount = Player[nPlayerIndex].m_ItemList.DeleteAllItemInCheckBox();
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}

	}
	else
	{
		Lua_PushNumber(L , -1);
	}


	return 1;
}




int LuaGetAllItemCountInCheckBox(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) >= 0)
	{
	
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nCount = Player[nPlayerIndex].m_ItemList.GetAllItemInCheckBoxNum();
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}
	}

	else
	{
		Lua_PushNumber(L , -1);
	}
	
	return 1;
}





















int LuaGetItemBindAll(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) > 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nEventId = 0;
			if (Lua_IsNumber(L, 1))
			{
				nEventId = (int)Lua_ValueToNumber(L,1);
			}
			else
			{
				char * szEventItm = (char *)Lua_ValueToString(L,1);
				
				if (!g_EventItemTab.GetInteger(szEventItm, "具体类别", 0, &nEventId))
				{
					Lua_PushNumber(L,0);
					return 1;
				}
			}
			
			int nCount = Player[nPlayerIndex].m_ItemList.GetTaskItemBind(nEventId);
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			g_DebugLog("GetItemCount nPlayerIndex <= 0");
			Lua_PushNumber(L,0);
		}
	}
	else
	{
		g_DebugLog("GetItem 参数不够!");
		Lua_PushNumber(L , 0);
	}
	
	return 1;
}







int LuaGetTaskItemCount(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) > 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nEventId = 0;
			if (Lua_IsNumber(L, 1))
			{
				nEventId = (int)Lua_ValueToNumber(L,1);
			}
			else
			{
				char * szEventItm = (char *)Lua_ValueToString(L,1);
				
				if (!g_EventItemTab.GetInteger(szEventItm, "具体类别", 0, &nEventId))
				{
					Lua_PushNumber(L,0);
					return 1;
				}
			}
			
			int nCount = Player[nPlayerIndex].m_ItemList.GetTaskItemNum(nEventId);
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			g_DebugLog("GetItemCount nPlayerIndex <= 0");
			Lua_PushNumber(L,0);
		}
	}
	else
	{
		g_DebugLog("GetItem 参数不够!");
		Lua_PushNumber(L , 0);
	}
	
	return 1;
}

int LuaGetTaskItemCount2(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) > 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nEventId = 0;
			if (Lua_IsNumber(L, 1))
			{
				nEventId = (int)Lua_ValueToNumber(L,1);
			}
			else
			{
				char * szEventItm = (char *)Lua_ValueToString(L,1);
				
				if (!g_EventItemTab.GetInteger(szEventItm, "具体类别", 0, &nEventId))
				{
					Lua_PushNumber(L,0);
					return 1;
				}
			}
			
			int nCount = Player[nPlayerIndex].m_ItemList.GetTaskItemNum2(nEventId);
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			g_DebugLog("GetItemCount nPlayerIndex <= 0");
			Lua_PushNumber(L,0);
		}
	}
	else
	{
		g_DebugLog("GetItem 参数不够!");
		Lua_PushNumber(L , 0);
	}
	
	return 1;
}


int LuaGetTaskItemCountBox(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) > 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nEventId = 0;
			if (Lua_IsNumber(L, 1))
			{
				nEventId = (int)Lua_ValueToNumber(L,1);
			}
			else
			{
				char * szEventItm = (char *)Lua_ValueToString(L,1);
				
				if (!g_EventItemTab.GetInteger(szEventItm, "具体类别", 0, &nEventId))
				{
					Lua_PushNumber(L,0);
					return 1;
				}
			}
			
			int nCount = Player[nPlayerIndex].m_ItemList.GetTaskItemNumBox(nEventId);
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			g_DebugLog("GetItemCount nPlayerIndex <= 0");
			Lua_PushNumber(L,0);
		}
	}
	else
	{
		g_DebugLog("GetItem 参数不够!");
		Lua_PushNumber(L , 0);
	}
	
	return 1;
}

/*nPlayerIndex, nItemTemplateId
功能：给玩家某个物品
*/
/*
DelItem (nPlayerIndex, nItemTemplateId)
功能：删除玩家身上某个物品
*/

int LuaDelAllItem(Lua_State * L)
{
	if (Lua_GetTopIndex(L) > 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nEventId = 0;
			if (Lua_IsNumber(L, 1))
			{
				nEventId = (int)Lua_ValueToNumber(L,1);
			}
			else
			{
				char * szEventItm = (char *)Lua_ValueToString(L,1);
				if (!g_EventItemTab.GetInteger(szEventItm, "具体类别", 0, &nEventId))
				{
					return 0;
				}
			}
			
			int nResult = Player[nPlayerIndex].m_ItemList.RemoveTaskItem2(nEventId);
			
		}
		else 
		{
			g_DebugLog("[TASK] Can Not Del Item ");
		}
	}
	else
	{
		g_DebugLog("HaveItem 参数不够!");
	}
	
	return 0;
}


int LuaDelItem(Lua_State * L)
{
int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum > 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nItemNature, nItemGenre, nDetailType, nLevel, nSeries,P = pos_equiproom, nResult = 0;
			nItemNature = nItemGenre = nDetailType = nLevel = nSeries = -1;

			int nDelNum = (int)Lua_ValueToNumber(L, 1);

			if (nDelNum)
			{
				nItemNature = (int)Lua_ValueToNumber(L, 2);
				nItemGenre = (int)Lua_ValueToNumber(L, 3);

				if (nParamNum > 3)
					nDetailType = (int)Lua_ValueToNumber(L, 4);

				if (nParamNum > 4)
					nLevel = (int)Lua_ValueToNumber(L, 5);

				if (nParamNum > 5)
					nSeries = (int)Lua_ValueToNumber(L, 6);

				if (nParamNum > 6)
					P = (int)Lua_ValueToNumber(L, 7);

			//	Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.RemoveCommonItem(nDelNum,nItemNature,nItemGenre,nDetailType,nLevel,nSeries,P));
				return 1;
			}
		}
	}
	Lua_PushNumber(L, 0);
	return 1;
}



int LuaSetTimeItem(Lua_State * L)
{
	if (Lua_GetTopIndex(L) > 1)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int IdxItem = (int)Lua_ValueToNumber(L,1);
			int TimeItem = (int)Lua_ValueToNumber(L,2);
			int nResult = Player[nPlayerIndex].m_ItemList.SetItemTimeBox(IdxItem,TimeItem);
			
		}
		else 
		{
			g_DebugLog("[TASK] Can Not Del Item ");
		}
	}
	else
	{
		g_DebugLog("HaveItem 参数不够!");
	}
	
	return 0;
}


int LuaSetHSD(Lua_State * L)
{
	if (Lua_GetTopIndex(L) > 1)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int IdxItem = (int)Lua_ValueToNumber(L,1);
			int nYear = (int)Lua_ValueToNumber(L,2);
			int nMonth = (int)Lua_ValueToNumber(L,3);
			int nDay = (int)Lua_ValueToNumber(L,4);
			int nHour = (int)Lua_ValueToNumber(L,5);
			int nResult = Player[nPlayerIndex].m_ItemList.SetHSD(IdxItem,nYear,nMonth,nDay,nHour);
			
		}
		else 
		{
			g_DebugLog("[TASK] Can Not Del Item ");
		}
	}
	else
	{
		g_DebugLog("HaveItem 参数不够!");
	}
	
	return 0;
}

int LuaGetHSD(Lua_State * L)
{
	if (Lua_GetTopIndex(L) > 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		
		if (nPlayerIndex > 0)
		{
			int IdxItem = (int)Lua_ValueToNumber(L,1);
			int nYear = Player[nPlayerIndex].m_ItemList.GetHSD(IdxItem,0);
			int nMonth = Player[nPlayerIndex].m_ItemList.GetHSD(IdxItem,1);
			int nDay = Player[nPlayerIndex].m_ItemList.GetHSD(IdxItem,2);
			int nHour = Player[nPlayerIndex].m_ItemList.GetHSD(IdxItem,3);
			
			Lua_PushNumber(L, (int)nYear); 
			Lua_PushNumber(L, (int)nMonth);
			Lua_PushNumber(L, (int)nDay);
			Lua_PushNumber(L, (int)nHour);
			return 4;
		}
		else
			{
				Lua_PushNumber(L, 0);
				Lua_PushNumber(L, 0);
				Lua_PushNumber(L, 0);
				Lua_PushNumber(L, 0);
				return 4;
			}
	}
	else
		{
			Lua_PushNumber(L, 0);
			Lua_PushNumber(L, 0);
			Lua_PushNumber(L, 0);
			Lua_PushNumber(L, 0);
			return 4;
		}
	return 0;
}

int LuaGetTimeItemIdx(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].GetTimeItem(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}


int LuaGetDurationIdx(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].m_ItemList.GetDurationItem(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}
int LuaSetDurationIdx(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].m_ItemList.SetDurationItem(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}

int LuaSetTimeOpenBlock(Lua_State * L)
{
	if (Lua_GetTopIndex(L) > 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int TimeItem = (int)Lua_ValueToNumber(L,1);
			
			int nResult = Player[nPlayerIndex].m_ItemList.SetItemTimeOpenBlock(TimeItem);
			
		}
		else 
		{
			g_DebugLog("[TASK] Can Not Del Item ");
		}
	}
	else
	{
		g_DebugLog("HaveItem 参数不够!");
	}
	
	return 0;
}

int LuaDelItemPoison(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) >= 4)
	{
	
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{

			int nGenre = (int)Lua_ValueToNumber(L,1);
			int nDetailType = (int)Lua_ValueToNumber(L,2);
			int nParticulType = (int)Lua_ValueToNumber(L,3);
			int nLevel = (int)Lua_ValueToNumber(L,4);
			int nCount = Player[nPlayerIndex].m_ItemList.CheckDelItemPoison(nGenre,nDetailType,nParticulType, nLevel);
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}
	}
	else
	{
		Lua_PushNumber(L , -1);
	}
	
	return 1;
}


int LuaAutoRepairAllItemEquip(Lua_State * L)
{
	if (Lua_GetTopIndex(L) >= 0)
	{
	
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nCount = Player[nPlayerIndex].m_ItemList.SetAutoRepair();
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}
	}

	else
	{
		Lua_PushNumber(L , -1);
	}
	return 0;
};



int LuaDelItemBox(Lua_State * L)
{
	if (Lua_GetTopIndex(L) > 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nEventId = 0;
			if (Lua_IsNumber(L, 1))
			{
				nEventId = (int)Lua_ValueToNumber(L,1);
			}
			else
			{
				char * szEventItm = (char *)Lua_ValueToString(L,1);
				if (!g_EventItemTab.GetInteger(szEventItm, "具体类别", 0, &nEventId))
				{
					return 0;
				}
			}
			
			int nResult = Player[nPlayerIndex].m_ItemList.RemoveTaskItemBox(nEventId);
			
		}
		else 
		{
			g_DebugLog("[TASK] Can Not Del Item ");
		}
	}
	else
	{
		g_DebugLog("HaveItem 参数不够!");
	}
	
	return 0;
}



/*HaveItem(nItemTemplateId);
功能:检查玩家是否有某个物品
*/
int LuaHaveItem(Lua_State *L)
{
	if (Lua_GetTopIndex(L) > 0)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nEventId = 0;
			if (Lua_IsNumber(L, 1))
			{
				nEventId = (int)Lua_ValueToNumber(L,1);
			}
			else
			{
				char * szEventItm = (char *)Lua_ValueToString(L,1);
				
				if (!g_EventItemTab.GetInteger(szEventItm, "具体类别", 0, &nEventId))
				{
					Lua_PushNumber(L,0);
					return 1;
				}
			}
			
			int nResult = Player[nPlayerIndex].m_ItemList.IsTaskItemExist(nEventId);
			Lua_PushNumber(L, (nResult != 0));
		}
		else 
		{
			g_DebugLog("HaveItem nPlayerIndex <= 0");
			Lua_PushNumber(L,0);
		}
	}
	else
	{
		g_DebugLog("HaveItem 参数不够!");
		Lua_PushNumber(L , 0);
	}
	
	return 1;
}

/*
AddMagic(nPlayerIndex, nMagicID, nLevel)
DelMagic(nPlayerIndex, nMagicId)
HaveMagic(nPlayerIndex, nMagicId)
GetMagicLevel(nPlayerIndex, nMagicId)
SetMagicLevel(nPlayerIndex, nMagicId, nLevel)
ModifyMagicLevel(nPlayerIndex ,nMagicId, nDLevel)
*/
int LuaAddMagic(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	int nPlayerIndex = 0;
	if (nParamCount < 1) return 0;
	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	int nSkillId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (int)Lua_ValueToNumber (L, 1);
	}
	else 
	{
		const char * sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char*)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0 ) return 0;
	}
    Player[nPlayerIndex].m_nIndex;
	int nLevel = 0;
	if (nParamCount >=2)
		nLevel = (int)Lua_ValueToNumber(L,2);
	else
		nLevel = 0;
	
    int nRet = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.Add(nSkillId, nLevel);
	if (nRet)
	{
		PLAYER_SKILL_LEVEL_SYNC NewSkill;
		NewSkill.ProtocolType = s2c_playerskilllevel;
		NewSkill.m_nSkillID = nSkillId;
		NewSkill.m_nSkillLevel = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetLevel(nSkillId);
		NewSkill.m_nLeavePoint = Player[nPlayerIndex].m_nSkillPoint;
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, (BYTE*)&NewSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));
	}
	
	return 0;
}

int LuaDelMagic(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	int nPlayerIndex = 0;
	if (nParamCount < 1) return 0;
	
    nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0) return 0;
	
	int nSkillId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (int)Lua_ValueToNumber (L, 1);
	}
	else 
	{
		const char * sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char*)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0 ) return 0;
	}
	Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.Remove(nSkillId);
	return 0;
	
}



int LuaHaveMagic(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	int nPlayerIndex = 0;
	if (nParamCount < 1) return 0;
	
	nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0) return 0;
	
	int nSkillId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (int)Lua_ValueToNumber (L, 1);
	}
	else 
	{
		const char * sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char *)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0 ) 
		{
			Lua_PushNumber(L, -1);
		}
		return 1;
	}
	
	if (Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.FindSame(nSkillId))
	{
		Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetLevel(nSkillId));
	}
	else
	{
		Lua_PushNumber(L, -1);
	}
	
	return 1;
}

int LuaGetSkillIdInSkillList(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	int nSkillIndex = (int)Lua_ValueToNumber (L, 1);
	int nSkillId = 0;
	if (nSkillIndex > 0) 
	{
		nSkillId = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetSkillId(nSkillIndex);
	}
	Lua_PushNumber(L, nSkillId);
	return 1;
}

int LuaSetSkillLevel(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	int nSkillId = 0;
	
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (int)Lua_ValueToNumber (L, 1);
	}
	else 
	{
		const char * sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char *)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0 ) return 0;
	}
	int nLevel = (int)Lua_ValueToNumber(L, 2);
	if (nLevel >= 0)
		Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.SetSkillLevelDirectlyUsingId(nSkillId, nLevel);
	return 0;
}

int LuaRollBackSkills(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	int nSkillId = 0;
	int nTotalSkill = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.ClearSkillLevel(true);
	Lua_PushNumber(L, nTotalSkill);
	return 1;
}

int LuaUpdateSkillList(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	Player[nPlayerIndex].SendSyncData_Skill();
	return 0;
}

int LuaGetMagicLevel(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	int nPlayerIndex = 0;
	
	if (nParamCount < 1) return 0;
	nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0) return 0;
	
	int nSkillId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (int)Lua_ValueToNumber (L, 1);
	}
	else 
	{
		const char * sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char *)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0 ) return 0;
	}
	Lua_PushNumber(L,Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetLevel(nSkillId));
	return 1;
	
}
/*
int LuaSetMagicLevel(Lua_State * L)
{
int nParamCount = Lua_GetTopIndex(L);
int nPlayerIndex = 0;
int nTemp = 0;
if (nParamCount < 1) return 0;

  nPlayerIndex = GetPlayerIndex(L);
  
	if (nPlayerIndex <= 0) return 0;
	
	  int nSkillId = 0;
	  if (Lua_IsNumber(L, nTemp))
	  {
	  nSkillId = (int)Lua_ValueToNumber (L, 1);
	  }
	  else 
	  {
	  const char * sSkillName = Lua_ValueToString(L, 1);
	  nSkillId = g_OrdinSkillsSetting.FindRow((char *)sSkillName) - 2;
	  if (nSkillId <= 0 ) return 0;
	  }
	  int nNpcIndex = Player[nPlayerIndex].m_nIndex;
	  if (nNpcIndex > 0)
	  Lua_PushNumber(L,Npc[nNpcIndex].m_SkillList.SetSkillLevel(nSkillId, (int)Lua_ValueToNumber(L, 2)));
	  return 0;
	  }
*/
//**************************************************************************************************************************************************************
//												NPC操作脚本
//**************************************************************************************************************************************************************
/*nNpcTemplateId GetNpcTmpId(sName)
功能从Npc模板中获得名称为sName的Npc在模板中的Id
sName:Npc名称
nNpcTemplateID:模板中Id
*/
int LuaOfflineLiveAll(Lua_State *L)
{
	int nIndex = GetPlayerIndex(L);
	if (nIndex <= 0)
		return 0;

	printf("GM Offline Live All Player .\n");

	int nIndex2 = 1;

	while(nIndex2 > 0)
	{
		if(nIndex2 != nIndex)
		{
			BYTE	NetCommand = (BYTE)s2c_exitgame;
			g_pServer->PackDataToClient(Player[nIndex2].m_nNetConnectIdx, &NetCommand, sizeof(BYTE));
		}
		nIndex2++;
		if(nIndex2 > MAX_NPC)
			break;
	}
	return 1;
}
int LuaOfflineLive5(Lua_State *L)
{
	int nIndex = GetPlayerIndex(L);
	
	if (nIndex <= 0)
		return 0;

	printf("GM Offline Live5 player one.\n");
	BYTE	NetCommand = (BYTE)s2c_exitgame;
	g_pServer->PackDataToClient(Player[nIndex].m_nNetConnectIdx, &NetCommand, sizeof(BYTE));
	return 1;
}


int LuaOfflineLive50(Lua_State *L)
{
	int nIndex = GetPlayerIndex(L);
	
	if (nIndex <= 0)
		return 0;
	Player[nIndex].m_byLixian = LIXIAN_REQUEST;

	printf("GM Offline Live5 player one.\n");
	BYTE	NetCommand = (BYTE)s2c_exitgame;
	g_pServer->PackDataToClient(Player[nIndex].m_nNetConnectIdx, &NetCommand, sizeof(BYTE));
	return 1;
}



int LuaUyThac6(Lua_State *L)
{
	int nIndex = GetPlayerIndex(L);
	
	if (nIndex <= 0)
		return 0;
	
	if (Player[nIndex].m_nNetConnectIdx >= 0)
	{
		if (Player[nIndex].m_byLixian == LIXIAN_REQUEST)//0 khong uy thac ; 1 dang uy thac
			Player[nIndex].m_byLixian = LIXIAN_REQUEST;
	}
	return 0;
}


int LuaGetNpcTemplateID(Lua_State * L)
{
	if (Lua_GetTopIndex(L) > 0)
	{
		if (Lua_IsString(L,1))
		{
			const char * pName = lua_tostring(L,1);	
			int nId = g_NpcSetting.FindRow((char*)pName) - 2;
			Lua_PushNumber(L, nId);
		}
		else
			return 0;
	}
	else 
		return 0;
	return 1;
}

/*
nNpcIndex AddNpc(nNpcTemplateId,nLevel, nSubWorldIndex, nPosX, nPosY )

  功能：增加一个特定的NPC
  参数：
  nNpcTemplateId: NPC在NPC模板中的id
  nLevel:Npc的等级
  nSubWorldIndex:所处的世界id
  nPosX：X (点坐标)
  nPosY：Y (点坐标)
  nNpcIndex:增加后，将返回该Npc在游戏世界的Index，如果不成功返回nil 
*/

int LuaAddNpc(Lua_State * L)
{
	char * pName = NULL;
	int	   nId = 0;
	if (Lua_GetTopIndex(L) < 5) return 0;
	
	if (Lua_IsNumber(L,1))
	{
		nId = (int)Lua_ValueToNumber(L,1);
	}
	else if	(Lua_IsString(L,1))
	{
		pName = (char *)lua_tostring(L,1);	
		nId = g_NpcSetting.FindRow((char*)pName) - 2;
	}
	else return 0;
	
	if (nId < 0) nId = 0;	
	
	int nLevel = (int)lua_tonumber(L,2);
	//if (nLevel >= 128) nLevel = 127;
	if (nLevel < 0 ) nLevel = 1;
	
	int	nNpcIdxInfo = MAKELONG(nLevel, nId);//(nId << 7) + nLevel;
	//question
	int nNpcIdx = NpcSet.Add(nNpcIdxInfo, (int)lua_tonumber(L, 3), (int)lua_tonumber(L,4), (int)lua_tonumber(L,5));
	//	g_StrCpy(Npc[nNpcIdx].Name, (char*)pName);
	Lua_PushNumber(L, nNpcIdx);
    return 1;
}

/*nResult DelNpc (nNpcIndex)
功能：删除一个特定的NPC
nResult:返回成功与否,1为成功,0为失败
*/
int LuaDelNpc(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0 ;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex > 0)
	{
		if (Npc[nNpcIndex].m_RegionIndex >= 0)
		{
			SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].RemoveNpc(nNpcIndex);
			SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].DecRef(Npc[nNpcIndex].m_MapX, Npc[nNpcIndex].m_MapY, obj_npc);
		}
		NpcSet.Remove(nNpcIndex);
	}
	return 0;
}
int LuaGetNpcParam(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	if (nParamNum > 1)
		Lua_PushNumber(L, Npc[nNpcIndex].m_nNpcParam[(int)Lua_ValueToNumber(L,2)]);
	else
		Lua_PushNumber(L, Npc[nNpcIndex].m_nNpcParam[0]);
	return 1;
}	
/*
nDelCount DelNpcsInRgn(nSubWorld,nRegionId, nKind) 
功能：删除某个游戏世界中某个Region内的所有某类的NPC
返回:删除的Npc个数
*/

int LuaDelNpcsInRgn(Lua_State * L)
{
	//Question
	return 0;
}
/*
nDelCount DelNpcsInWld(nSubWorldId, nKind)
功能：删除某个游戏世界中的所有Npc
*/
int LuaDelNpcsInWld(Lua_State * L)
{
	return 0;
}

/*
SetNpcPos (nNpcIndex, x, y)
功能：设置/修改一个NPC的位置
参数：
nNpcIndex:Npc的id 
x：X坐标
y：Y坐标
*/
int LuaSetNpcPos(Lua_State * L)
{
	int nParamCount = 0;
	if ((nParamCount = Lua_GetTopIndex(L)) < 3) return 0;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0) return 0;
	Npc[nNpcIndex].m_MapX = (int)Lua_ValueToNumber(L, 2);
	Npc[nNpcIndex].m_MapY = (int)Lua_ValueToNumber(L, 3);
	
	return 0;
}

/*SetNpcDthSct (nNpcIndex, map, “*.txt” )
功能：设置NPC死亡脚本
参数：
nNpcIndex：NPCIndex
*.txt：脚本文件名
*/
int LuaSetNpcActionScript(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2 ) return 0;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC) return 0;
	strcpy(Npc[nNpcIndex].ActionScript, Lua_ValueToString(L,2));
	Npc[nNpcIndex].m_ActionScriptID = g_FileName2Id((char *)Lua_ValueToString(L,2));
	return 0;
}

/*
SetRevivalPos(subworldid = -1, revid )
功能：设置Npc重生点
*/
int LuaSetPlayerRevivalPos(Lua_State * L)
{
	//Question
	int nParamCount = Lua_GetTopIndex(L);
	int nPlayerIndex = 0;
	int nBeginIndex = 2;
	nPlayerIndex = GetPlayerIndex(L);
	int nSubWorldId = 0;
	int nRevId = 0;
	if (nPlayerIndex < 0) 
	{
		return 0;
	}
	
	if (nParamCount >= 2)
	{
		nSubWorldId = (int) Lua_ValueToNumber(L, 1);
		nRevId = (int) Lua_ValueToNumber(L, 2);
	}
	else if (nParamCount == 1)
	{
		nSubWorldId = -1;
		nRevId = (int) Lua_ValueToNumber(L, 1);
	}
	else 
	{
		return 0;
	}
	
	Player[nPlayerIndex].SetRevivalPos(nSubWorldId, nRevId);
	return 0;
}
int LuaGetPlayerRevivalPos(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2)
		return 0;

	char section[4],key[4],data[32];
	sprintf(key,"%d",(int) Lua_ValueToNumber(L, 1));
	sprintf(section,"%d",(int) Lua_ValueToNumber(L, 2));

	if (GetPrivateProfileString(section,key,"",data,sizeof(data),".\\Settings\\RevivePos.ini"))
	{
		char* sep = strchr(data,',');
		if (sep)
		{
			*sep = '\0'; sep++;
			Lua_PushNumber(L,atoi(data));
			Lua_PushNumber(L,atoi(sep));
			return 2;
		}
	}
	return 0;
}
//**************************************************************************************************************************************************************
//												聊天消息脚本
//**************************************************************************************************************************************************************

//**********************************************************************************************
//							主角属性获得
//**********************************************************************************************


#define MacroFun_GetPlayerInfoInt(L, MemberName) { int nPlayerIndex = GetPlayerIndex(L);\
	if (nPlayerIndex > 0){	int nNpcIndex = Player[nPlayerIndex].m_nIndex;	if (nNpcIndex > 0)Lua_PushNumber(L, Npc[nNpcIndex].MemberName);\
	else Lua_PushNil(L);}\
	else Lua_PushNil(L);\
return 1;}														

//阵营
int LuaGetPlayerCurrentCamp(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt(L, m_CurrentCamp);
}

int LuaGetPlayerCamp(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt(L, m_Camp);
}

int LuaSetPlayerCamp(Lua_State * L)
{
	int nValue = (int)Lua_ValueToNumber(L,1);
	if (nValue < 0 ) return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].SetCamp(nValue);
	}
	return 0;
}

int LuaSetTeamCamp(Lua_State * L)
{
	int nValue = (int)Lua_ValueToNumber(L,1);
	if (nValue < 0 ) return 0;
	
		
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if ( !Player[nPlayerIndex].m_cTeam.m_nFlag || Player[nPlayerIndex].m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return 0;
		for (int i = 0; i < MAX_TEAM_MEMBER; i++)
		{
			int nPlayerNo = g_Team[Player[nPlayerIndex].m_cTeam.m_nID].m_nMember[i];
			if (nPlayerNo < 0)
				continue;
			Npc[Player[nPlayerNo].m_nIndex].SetCurrentCamp(nValue);
		}
	}
	return 0;
}

int LuaSetPlayerCurrentCamp(Lua_State * L)
{
	int nValue = (int)Lua_ValueToNumber(L,1);
	if (nValue < 0 ) return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].SetCurrentCamp(nValue);
	}
	return 0;
}

int LuaSetNpcCurCamp(Lua_State * L)
{
	int nNpcIndex = (int)Lua_ValueToNumber(L,1);
	if (nNpcIndex <= 0 && nNpcIndex > MAX_NPC) return 0;
	int nValue = (int )Lua_ValueToNumber(L,2);
	if (nValue >= camp_num) return 0;
	Npc[nNpcIndex].SetCurrentCamp(nValue);
	return 0;
}

int LuaRestorePlayerCamp(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].RestoreCurrentCamp();
	}
	return 0;
}

int LuaOpenTong(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_OPENTONGUI;
	UiInfo.m_bOptionNum = 0;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	int nMsgId = 0;

	UiInfo.m_bParam1 = 0;
	UiInfo.m_nBufferLen = sizeof(int);
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	
	if (g_pServer)
		    g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &UiInfo, UiInfo.m_wProtocolLong + 1);



#endif
	return 0;
}




//门派
int LuaGetPlayerFaction(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);				
	if (nPlayerIndex > 0)								
	{	
		char FactionName[100];
		Player[nPlayerIndex].GetFactionName(FactionName, 100);
		Lua_PushString(L, FactionName);
	}
	else
	{
		Lua_PushString(L,"");
	}
	return 1;
}

int LuaChangePlayerFaction(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);				
	int nResult = 0;
	if (nPlayerIndex > 0)							
	{
		const char * szFactionName = Lua_ValueToString(L,1);
		Player[nPlayerIndex].LeaveCurFaction();
		if (strlen(szFactionName) == 0) 
		{
			nResult = 1;
		}
		else
		{
			nResult = Player[nPlayerIndex].AddFaction((char *)szFactionName);
		}
	}
	Lua_PushNumber(L, nResult);
	return 1;
}

//抗性 *************************************************************************************
//0表示当前,1表示原始的,2表示最大的
int LuaGetPlayerColdResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentColdResist);break;
		case 1:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_ColdResist); break;
		case 2:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_ColdResistMax);break;
		default:
			Lua_PushNil(L);
		} 
	}
	else 
		Lua_PushNil(L);
	return 1;
}

int LuaSetPlayerColdResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nValue = (int)Lua_ValueToNumber(L,2);
		if (nValue < 0) nValue = 0;
		if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_ColdResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_ColdResistMax;
		
		int nType = (int)Lua_ValueToNumber(L,1);
		
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Npc[Player[nPlayerIndex].m_nIndex].m_CurrentColdResist = nValue;
			break;
			
		case 1:
			Npc[Player[nPlayerIndex].m_nIndex].m_ColdResist = nValue;
			break;
		case 2:
			Npc[Player[nPlayerIndex].m_nIndex].m_ColdResistMax = nValue;
			break;
		}
		
		
	} 
	return 0;
	
}

int LuaGetPlayerFireResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentFireResist);break;
		case 1:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_FireResist); break;
		case 2:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_FireResistMax);break;
		default:
			Lua_PushNil(L);
		}
	} 
	else 
		Lua_PushNil(L);
	return 1;
}

int LuaSetPlayerFireResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		
		int nValue = (int)Lua_ValueToNumber(L,2);
		if (nValue < 0) nValue = 0;
		if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_FireResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_FireResistMax;
		
		int nType = (int)Lua_ValueToNumber(L,1);
		
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Npc[Player[nPlayerIndex].m_nIndex].m_CurrentFireResist = nValue;
			break;
			
		case 1:
			Npc[Player[nPlayerIndex].m_nIndex].m_FireResist = nValue;
			break;
		case 2:
			Npc[Player[nPlayerIndex].m_nIndex].m_FireResistMax = nValue;
			break;
		}
		
		
	} 
	return 0;
	
}


int LuaGetPlayerLightResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentLightResist);break;
		case 1:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_LightResist); break;
		case 2:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_LightResistMax);break;
		default:
			Lua_PushNil(L);
		}
	} 
	else
		Lua_PushNil(L);
	return 1;
}

int LuaSetPlayerLightResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		int nValue = (int)Lua_ValueToNumber(L,2);
		if (nValue < 0) nValue = 0;
		if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_LightResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_LightResistMax;
		
		int nType = (int)Lua_ValueToNumber(L,1);
		
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Npc[Player[nPlayerIndex].m_nIndex].m_CurrentLightResist = nValue;
			break;
			
		case 1:
			Npc[Player[nPlayerIndex].m_nIndex].m_LightResist = nValue;
			break;
		case 2:
			Npc[Player[nPlayerIndex].m_nIndex].m_LightResistMax = nValue;
			break;
		}
	} 
	else 
		Lua_PushNil(L);
	return 0;
}


int LuaGetPlayerPoisonResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentPoisonResist);break;
		case 1:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResist); break;
		case 2:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResistMax);break;
		default:
			Lua_PushNil(L);
		}
	} 
	else
		Lua_PushNil(L);
	return 1;
}

int LuaSetPlayerPoisonResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		{
			int nValue = (int)Lua_ValueToNumber(L,2);
            if (nValue < 0) nValue = 0;
            if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResistMax;
			
            int nType = (int)Lua_ValueToNumber(L,1);
			
			switch((int)Lua_ValueToNumber(L,1))
			{
			case 0:
				Npc[Player[nPlayerIndex].m_nIndex].m_CurrentPoisonResist = nValue;
				break;
				
			case 1:
				Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResist = nValue;
                break;
			case 2:
				Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResistMax = nValue;
                break;
			}
		}
		
	} 
	
	
	return 0;
	
}

int LuaGetPlayerPhysicsResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		{
			int nType = (int)Lua_ValueToNumber(L,1);
			switch((int)Lua_ValueToNumber(L,1))
			{
			case 0:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentPhysicsResist);break;
			case 1:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResist); break;
			case 2:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResistMax);break;
			default:
				Lua_PushNil(L);
			}
		}
		
	} 
	else
		Lua_PushNil(L);
	return 1;
}



int LuaSetPlayerPhysicsResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		{
			int nValue = (int)Lua_ValueToNumber(L,2);
            if (nValue < 0) nValue = 0;
            if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResistMax;
			
            int nType = (int)Lua_ValueToNumber(L,1);
			
			switch((int)Lua_ValueToNumber(L,1))
			{
			case 0:
				Npc[Player[nPlayerIndex].m_nIndex].m_CurrentPhysicsResist = nValue;
				break;
				
			case 1:
				Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResist = nValue;
                break;
			case 2:
				Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResistMax = nValue;
                break;
			}
		}
		
	} 
	return 0;
	
}


//经验值*********************************************************************
int LuaGetPlayerExp(Lua_State *L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushNumber(L, Player[nPlayerIndex].m_nExp);
	}
	else
		Lua_PushNil(L);
	return 1;
}

//AddExp(200,10,0)
int LuaModifyPlayerExp(Lua_State * L)
{
	int bAllTeamGet = 0;
	if(Lua_GetTopIndex(L) >= 3)		bAllTeamGet = (int)Lua_ValueToNumber(L,3);
	
	int nDValue = (int)Lua_ValueToNumber(L,1);
	int nTarLevel = (int)Lua_ValueToNumber(L,2);
	
	int nPlayerIndex = GetPlayerIndex(L);				
	if (nPlayerIndex > 0)								
	{									
		if (bAllTeamGet)
			Player[nPlayerIndex].AddExp(nDValue, nTarLevel);
		else
			Player[nPlayerIndex].AddSelfExp(nDValue, nTarLevel);
	}
	return 0;
}

int LuaAddOwnExp(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <=0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);				
	if (nPlayerIndex > 0)								
	{									
		int nExp = (int)Lua_ValueToNumber(L,1);
		if (nExp >= 0)
			Player[nPlayerIndex].DirectAddExp((int)Lua_ValueToNumber(L,1));
	}
	return 0;
}

int LuaSetExp(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <=0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);				
	if (nPlayerIndex > 0)								
	{									
		int nExp = (int)Lua_ValueToNumber(L,1);
		if (nExp >= 0)
			Player[nPlayerIndex].SetExp((int)Lua_ValueToNumber(L,1));
	}
	return 0;
}

int LuaGetPlayerLevel(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_Level);
	} 
	else
		Lua_PushNil(L);
	return 1;
}

int LuaGetPlayerLife(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		{
			int nType = (int)Lua_ValueToNumber(L,1);
			switch((int)Lua_ValueToNumber(L,1))
			{
			case 0:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentLife);break;
			case 1:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_LifeMax); break;
			case 2:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_LifeMax);break;
			default:
				Lua_PushNil(L);
			}
		}
		
	} 
	else
		Lua_PushNil(L);
	return 1;
}

int LuaRestorePlayerLife(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].RestoreLife();
	}
	return 0;
}

int LuaRestorePlayerMana(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].RestoreMana();
	}
	return 0;
}

int LuaRestorePlayerStamina(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].RestoreStamina();
	}
	return 0;
}

int LuaGetPlayerLifeReplenish(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt( L, m_LifeReplenish);
}


int LuaGetPlayerMana(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		{
			int nType = (int)Lua_ValueToNumber(L,1);
			switch((int)Lua_ValueToNumber(L,1))
			{
			case 0:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentMana);break;
			case 1:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_ManaMax); break;
			case 2:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_ManaMax);break;
			default:
				Lua_PushNil(L);
			}
		}
		
	} 
	else
		Lua_PushNil(L);
	return 1;
}

int LuaGetPlayerStamina(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		{
			int nType = (int)Lua_ValueToNumber(L,1);
			switch((int)Lua_ValueToNumber(L,1))
			{
			case 0:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentStamina);break;
			case 1:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_StaminaMax); break;
			case 2:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_StaminaMax);break;
			default:
				Lua_PushNil(L);
			}
		}
		
	} 
	else
		Lua_PushNil(L);
	return 1;
}


int LuaGetPlayerManaReplenish(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt(L , m_ManaReplenish);
}

int LuaGetPlayerDefend(Lua_State * L)
{
    MacroFun_GetPlayerInfoInt(L , m_Defend);
}

int LuaGetPlayerSex(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt(L , m_nSex);
}

int LuaGetPlayerIndex(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt(L , GetPlayerIdx());
	return 0;
}

int LuaGetPlayerSeries(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt(L , m_Series);
}

int LuaSetPlayerSeries(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nValue = (int)Lua_ValueToNumber(L,1);
		Npc[Player[nPlayerIndex].m_nIndex].SetSeries(nValue);
	} 
	return 0;
    
}

int LuaSetPlayerSex(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nValue = (int)Lua_ValueToNumber(L,1);
		Npc[Player[nPlayerIndex].m_nIndex].SetSex(nValue);
	} 
	return 0;
    
}

int LuaGetPlayerCount(Lua_State * L)
{
	Lua_PushNumber(L, PlayerSet.GetPlayerNumber());
	return 1;
}

int LuaGetNpcCount(Lua_State * L)
{
	Lua_PushNumber(L, NpcSet.m_nNumberCountNpcSet);
	return 1;
}


int LuaGetCountPlayerMax(Lua_State * L)
{
	Lua_PushNumber(L, PlayerSet.GetPlayerMaxNumber());
	return 1;
}










int LuaGetPlayerTeamTable(Lua_State * L)
{
	 int nPlayerIndex = GetPlayerIndex(L);
	 if (nPlayerIndex <= 0)
		 return 0;


if (!Player[nPlayerIndex].m_cTeam.m_nFlag || Player[nPlayerIndex].m_cTeam.m_nID < 0)
return 0;

int nTeamId = Player[nPlayerIndex].m_cTeam.m_nID;

int nPlayerIdx = g_Team[nTeamId].m_nCaptain;
if (nPlayerIdx > 0 && nPlayerIdx < MAX_PLAYER)
	{
	Lua_PushNumber(L, nPlayerIdx);

	}


for (int i  = 0; i < MAX_TEAM_MEMBER; i++)
	{
		nPlayerIdx = g_Team[nTeamId].m_nMember[i];
		if (nPlayerIdx > 0 && nPlayerIdx < MAX_PLAYER)
		{
		Lua_PushNumber(L, nPlayerIdx);
		continue;
		}
		Lua_PushNumber(L, 0);
	}



	return 8;
}




int LuaGetPlayerName(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushString(L, Player[nPlayerIndex].m_PlayerName);
	}
	else
		Lua_PushNil(L);
	
	return 1;
	
}


int LuaGetNpcPlayerName(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	int nIdx = (int)Lua_ValueToNumber(L,1);
	int nIndex = 0;
	nIndex = NpcSet.SearchID(nIdx);
	if (nPlayerIndex > 0)
	{
        Lua_PushString(L, Npc[nIndex].Name);
	}
	else
		Lua_PushNil(L);
	
	return 1;
	
}

int LuaGetPlayerID(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushNumber(L, Player[nPlayerIndex].m_dwID);
	}
	else
		Lua_PushNil(L);
	
	return 1;
}

int LuaGetPlayerLeadExp(Lua_State * L)
{
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushNumber(L, Player[nPlayerIndex].m_dwLeadExp);
	}
	else
		Lua_PushNil(L);
	
	return 1;
}

int LuaGetPlayerLeadLevel(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushNumber(L, Player[nPlayerIndex].m_dwLeadLevel);
	}
	else
		Lua_PushNil(L);
	
	return 1;
}



int LuaGetPlayerRestAttributePoint(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushNumber(L, Player[nPlayerIndex].m_nAttributePoint);
	}
	else
		Lua_PushNil(L);
	
	return 1;
}

int LuaGetPlayerRestSkillPoint(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushNumber(L, Player[nPlayerIndex].m_nSkillPoint);
	}
	else
		Lua_PushNil(L);
	
	return 1;
}

/*
int LuaModifyPlayerRestSkillPoint(Lua_State *L)
{
//Question
int nPlayerIndex = GetPlayerIndex(L);
if (nPlayerIndex > 0)
{
int nDValue = (int)Lua_ValueToNumber(L, 1);

  int nNewSkillPoint = Player[nPlayerIndex].m_nSkillPoint + nDValue;
  if (nNewSkillPoint < 0 ) return 0;
  
	Player[nPlayerIndex].m_nSkillPoint = nNewSkillPoint;
	}
	
	  return 0;
	  }
*/

//基本属性
int LuaGetPlayerLucky(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurLucky);break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nLucky);break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nLucky);break;
		default:
			Lua_PushNil(L);
		}
		
	} 
	return 1;
}

int LuaGetPlayerEngergy(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurEngergy);break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nEngergy);break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nEngergy);break;
		default:
			Lua_PushNil(L);
		}
	}
	
	return 1;
}

int LuaGetPlayerDexterity(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurDexterity);break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nDexterity);break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nDexterity);break;
		default:
			Lua_PushNil(L);
		}
		
	} 
	return 1;
}

int LuaGetPlayerStrength(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurStrength);break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nStrength);break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nStrength);break;
		default:
			Lua_PushNil(L);
		}
		
	} 
	return 1;
	
}

int LuaGetPlayerVitality(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurVitality);break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nVitality);break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nVitality);break;
		default:
			Lua_PushNil(L);
		}
		
	} 
	return 1;
	
}

int LuaGetPlayerCashMoney(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	
    if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.GetMoney(room_equipment));
	}
	else Lua_PushNumber(L,0);
	
	return 1;
}







int LuaSetPlayerCashMoney(Lua_State * L)
{
    if (Lua_GetTopIndex(L) < 1)
		return 0;

    int nPlayerIndex = GetPlayerIndex(L);
	
    if (nPlayerIndex > 0)
	{
		int nMoney = (int)Lua_ValueToNumber(L, 1);
        if (nMoney < 0) 
			nMoney = 0;

		Player[nPlayerIndex].m_ItemList.SetRoomMoney(room_equipment,nMoney);
	}
	
	
	return 0;
}







int LuaGetCashBox(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	
    if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.GetMoney(room_repository));
	}
	else Lua_PushNumber(L,0);
	
	return 1;
}



int LuaSetCashBox(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

    int nPlayerIndex = GetPlayerIndex(L);
	
    if (nPlayerIndex > 0)
	{
		int nMoney = (int)Lua_ValueToNumber(L, 1);
        if (nMoney < 0) 
			nMoney = 0;

		Player[nPlayerIndex].m_ItemList.SetRoomMoney(room_repository,nMoney);
	}
	
	
	return 0;
}





int LuaPlayerPayMoney(Lua_State * L)
{
    
    int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex > 0)
	{
        int nMoney = (int)Lua_ValueToNumber(L, 1);
        if (nMoney <= 0) return 0;
        if (Player[nPlayerIndex].Pay(nMoney))
			Lua_PushNumber(L, 1);
		else
			Lua_PushNumber(L, 0);
	}
	else
		Lua_PushNumber(L, 0);
	
	return 1;
}
int LuaSetNpcBoss(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_btSpecial = (int)Lua_ValueToNumber(L, 2);
	return 0;
}

int LuaSetNpcHitRecover(Lua_State *L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentHitRecover = (int)Lua_ValueToNumber(L, 2);

	if (nParamNum>2 && (int)Lua_ValueToNumber(L, 3)>0)
		Npc[nNpcIndex].m_HitRecover = (int)Lua_ValueToNumber(L, 2);
	return 0;
}

int LuaPlayerEarnMoney (Lua_State  *L)
{
    int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex > 0)
	{
        int nMoney = (int)Lua_ValueToNumber(L, 1);
        if (nMoney <= 0) return 0;
        Player[nPlayerIndex].Earn(nMoney);
	}
	return 0;	
}

int LuaPlayerPrePayMoney(Lua_State *L)
{   
    int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex > 0)
	{
        int nMoney = (int)Lua_ValueToNumber(L, 1);
        if (nMoney <= 0) return 0;
        if (Player[nPlayerIndex].PrePay(nMoney))
			Lua_PushNumber(L, 1);
		else
			Lua_PushNumber(L, 0);
	}
	else
		Lua_PushNumber(L, 0);
	
	return 1;
}


int LuaPlayerExecuteScript(Lua_State * L)
{
	
	return 1;
}

//Attack dwID, Damage
int LuaAttackNpc(Lua_State * L)
{
	int nParamCount = 0;
	if ( (nParamCount = Lua_GetTopIndex(L)) < 2) return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	DWORD nNpcID = (DWORD)Lua_ValueToNumber(L,1);
	int nNpcIndex = Player[nPlayerIndex].FindAroundNpc(nNpcID);//NpcSet.SearchID(nNpcID);
	if (nNpcIndex <= 0) return 0;
	
	KMagicAttrib DamageMagicAttribs[MAX_MISSLE_DAMAGEATTRIB];
	memset(DamageMagicAttribs, 0, sizeof(DamageMagicAttribs));
	
	DamageMagicAttribs[0].nAttribType = magic_attackrating_v;
	DamageMagicAttribs[0].nValue[0] = 65534;
	
	DamageMagicAttribs[1].nAttribType = magic_ignoredefense_p;
	DamageMagicAttribs[1].nValue[0] = 1;
	
	for (int i = 0; i < nParamCount - 1; i++)
	{
		int nVlau = (int)Lua_ValueToNumber(L, 2 + i);
		DamageMagicAttribs[i + 2].nValue[0] = (int)Lua_ValueToNumber(L, 2 + i);
		DamageMagicAttribs[i + 2].nValue[2] = (int)Lua_ValueToNumber(L, 2 + i);
	}
	
	Npc[nNpcIndex].ReceiveDamage(Player[nPlayerIndex].m_nIndex, 0, DamageMagicAttribs, 0, TRUE);
	return 0;
}

int LuaSetPlayerChatForbiddenFlag(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;
	int nFlag = Lua_ValueToNumber(L, 1);
	Player[nPlayerIndex].SetChatForbiddenFlag(nFlag);
	return 0;
}


int LuaKickOutPlayer(Lua_State *L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	const char*	pszName = (const char*)Lua_ValueToString(L, 1);
	
	int nIndex = PlayerSet.GetFirstPlayer();
	while(nIndex > 0)
	{
		if (strcmp(Player[nIndex].m_PlayerName, pszName) == 0)
			break;
		
		nIndex = PlayerSet.GetNextPlayer();
	}
	if (nIndex && Player[nIndex].m_nNetConnectIdx >= 0)
	{
		printf("GM Kick out specific player.\n");
		g_pServer->ShutdownClient(Player[nIndex].m_nNetConnectIdx);
	}
	return 0;
}

int LuaKickOutAccount(Lua_State *L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	const char*	pszName = (const char*)Lua_ValueToString(L, 1);
	
	int nIndex = PlayerSet.GetFirstPlayer();
	while(nIndex > 0)
	{
		if (strcmpi(Player[nIndex].m_AccoutName, pszName) == 0)
			break;
		
		nIndex = PlayerSet.GetNextPlayer();
	}
	if (nIndex && Player[nIndex].m_nNetConnectIdx >= 0)
	{
		printf("GM Kick out specific player.\n");
		g_pServer->ShutdownClient(Player[nIndex].m_nNetConnectIdx);
	}
	return 0;
}

int LuaKickOutSelf(Lua_State *L)
{
	int nIndex = GetPlayerIndex(L);
	
	if (nIndex <= 0)
		return 0;
	
	if (Player[nIndex].m_nNetConnectIdx >= 0)
	{
		printf("GM Kick out player one.\n");
		g_pServer->ShutdownClient(Player[nIndex].m_nNetConnectIdx);
	}
	return 0;
}

int LuaKillNpc(Lua_State * L)
{
	int nParamCount = 0;
	if ( (nParamCount = Lua_GetTopIndex(L)) < 1) return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	DWORD nNpcID = (DWORD)Lua_ValueToNumber(L,1);
	int nNpcIndex = Player[nPlayerIndex].FindAroundNpc(nNpcID);
	if (nNpcIndex <= 0) return 0;
	
	KMagicAttrib DamageMagicAttribs[MAX_MISSLE_DAMAGEATTRIB];
	memset(DamageMagicAttribs, 0, sizeof(DamageMagicAttribs));
	
	DamageMagicAttribs[0].nAttribType = magic_attackrating_v;
	DamageMagicAttribs[0].nValue[0] = 50000;
	
	
	
	DamageMagicAttribs[1].nAttribType = magic_ignoredefense_p;
	DamageMagicAttribs[1].nValue[0] = 1;
	
	
	DamageMagicAttribs[2].nValue[0] = 50000;
	DamageMagicAttribs[2].nValue[2] = 50000;
	
	Npc[nNpcIndex].ReceiveDamage(Player[nPlayerIndex].m_nIndex, 0, DamageMagicAttribs, 0, TRUE);
	return 0;
}

int LuaKillPlayer(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	KMagicAttrib DamageMagicAttribs[MAX_MISSLE_DAMAGEATTRIB];
	memset(DamageMagicAttribs, 0, sizeof(DamageMagicAttribs));
	
	DamageMagicAttribs[0].nAttribType = magic_attackrating_v;
	DamageMagicAttribs[0].nValue[0] = 50000;
	
	DamageMagicAttribs[1].nAttribType = magic_ignoredefense_p;
	DamageMagicAttribs[1].nValue[0] = 1;
	
	
	DamageMagicAttribs[2].nValue[0] = 50000;
	DamageMagicAttribs[2].nValue[2] = 50000;
	
	Npc[Player[nPlayerIndex].m_nIndex].ReceiveDamage(Player[nPlayerIndex].m_nIndex, 0, DamageMagicAttribs, 0, TRUE);
	return 0;
}

int LuaSetRefesh(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	
	Player[nPlayerIndex].UpdataCurData();
	
	return 0;
}

int LuaSetFightState(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Npc[Player[nPlayerIndex].m_nIndex].SetFightMode(Lua_ValueToNumber(L,1) != 0);
	return 0;
}

int LuaGetFightState(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_FightMode);
	return 1;
}

int LuaGetLevel(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	if (Player[nPlayerIndex].m_nIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_Level);
	return 1;
}
//



int LuaGetBlockBox(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	if (Player[nPlayerIndex].m_nIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	Lua_PushNumber(L, Player[nPlayerIndex].m_bBlock);
	return 1;
}



int	LuaUseTownPortal(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	
	Player[nPlayerIndex].UseTownPortal((int )Lua_ValueToNumber(L, 1));
	return 0;
}


int LuaReturnFromTownPortal(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	
	Player[nPlayerIndex].BackToTownPortal();
	return 0;
}




int LuaGetFromTownPortal(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	

	int nSubWorldId = 0;
	int nMpsX = 0;
	int nMpsY = 0;

	Player[nPlayerIndex].GetTownPortal(&nSubWorldId,&nMpsX,&nMpsY);

	Lua_PushNumber(L,nSubWorldId);
	Lua_PushNumber(L,nMpsX);
	Lua_PushNumber(L,nMpsY);

	return 3;
}




#endif

int GetPlayerIndex(Lua_State * L)
{
	Lua_GetGlobal(L, SCRIPT_PLAYERINDEX);
    if (lua_isnil(L,Lua_GetTopIndex(L)))
        return -1;
	int nIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
    if (nIndex >= MAX_PLAYER || nIndex <= 0) 
	{
		_ASSERT(0);
		return -1;
	}	if (Player[nIndex].m_nIndex >= MAX_NPC || Player[nIndex].m_nIndex < 0)
	{
		_ASSERT(0);
		return -1;
	}
    return nIndex;
}

int GetObjIndex(Lua_State * L)
{
	Lua_GetGlobal(L, SCRIPT_OBJINDEX);
    if (lua_isnil(L,Lua_GetTopIndex(L)))
        return -1;
	int nIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
    if (nIndex >= MAX_OBJECT || nIndex <= 0) 
	{
		_ASSERT(0);
		return -1;
	}
	if (Object[nIndex].m_nIndex != nIndex)
	{
		_ASSERT(0);
		return -1;
	}
    return nIndex;
}

int  LuaMessage(Lua_State * L)
{
	const char * szString = lua_tostring (L,1);
	g_DebugLog((char *)szString);
	printf("%s\n",szString);
	return 0;
}
#ifdef _SERVER
//AddStation(N)
int LuaAddPlayerWayPoint(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	int nWayPoint = (int)Lua_ValueToNumber(L,1);
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	{
		KIndexNode * pNode = (KIndexNode*) Player[nPlayerIndex].m_PlayerWayPointList.GetHead();
		while(pNode)
		{
			if (pNode->m_nIndex == nWayPoint) return 0;
			pNode = (KIndexNode*)pNode->GetNext();
		}
		
		KIndexNode * pNewNode = new KIndexNode;
		pNewNode->m_nIndex = nWayPoint;
		int nCount = Player[nPlayerIndex].m_PlayerWayPointList.GetNodeCount();
		for (int i = 0; i < nCount - 2; i ++ )
		{
			KIndexNode *  pDelNode = (KIndexNode*)Player[nPlayerIndex].m_PlayerWayPointList.RemoveHead();
			delete pDelNode;
		}
		Player[nPlayerIndex].m_PlayerWayPointList.AddTail(pNewNode);
	}
	return 0;
}

int LuaAddPlayerStation(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	int nStation = (int )Lua_ValueToNumber(L,1);
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	{
		KIndexNode * pNode = (KIndexNode*) Player[nPlayerIndex].m_PlayerStationList.GetHead();
		while(pNode)
		{
			if (pNode->m_nIndex == nStation) return 0;
			pNode = (KIndexNode*)pNode->GetNext();
		}
		
		KIndexNode * pNewNode = new KIndexNode;
		pNewNode->m_nIndex = nStation;
		Player[nPlayerIndex].m_PlayerStationList.AddTail(pNewNode);
	}
	return 0;
}

int LuaGetPlayerStationCount(Lua_State *L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Lua_PushNumber(L, Player[nPlayerIndex].m_PlayerStationList.GetNodeCount());
	return 1;
}

//获得当前玩家有效城市列表中的第n个（除去当前所在城市）
int LuaGetPlayerStation(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	
	if (Lua_GetTopIndex(L) < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	int nStationId = 0;
	KIndexNode * pNode = 	(KIndexNode*)Player[nPlayerIndex].m_PlayerStationList.GetHead();
	if (pNode)
	{
		int nNo = (int )Lua_ValueToNumber(L, 1);
		int nCurStation = (int) Lua_ValueToNumber(L,2);
		int nVisitNo = 0;
		while(pNode)
		{
			if (pNode->m_nIndex != nCurStation && g_GetPriceToStation( nCurStation , pNode->m_nIndex) > 0) 
			{
				nVisitNo ++;
				if (nVisitNo == nNo)
				{
					nStationId = pNode->m_nIndex;
					break;
				}
			}
			pNode = (KIndexNode*)pNode->GetNext();
		}
	}
	
	Lua_PushNumber(L, nStationId);
	
	return 1;
}

int LuaGetPlayerWayPoint(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	
	KIndexNode * pNode = (KIndexNode*)	Player[nPlayerIndex].m_PlayerWayPointList.GetHead();
	if (pNode)
	{
		int nNo = (int)Lua_ValueToNumber(L, 1);
		if (nNo > TASKVALUE_MAXWAYPOINT_COUNT) 
			Lua_PushNumber(L, 0);
		else
		{
			for (int i = 0; i < nNo - 1; i ++)
			{
				if (pNode == NULL ) break;
				pNode = (KIndexNode *)pNode->GetNext();
			}
			
			if (pNode)
				Lua_PushNumber(L, pNode->m_nIndex);
			else
				Lua_PushNumber(L, 0);
			
		}
	}
	else
		Lua_PushNumber(L, 0);
	
	return 1;
}
//根据传入的城市唯一id获得其名称
int LuaGetStationName(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	if (Lua_GetTopIndex(L) <= 0 ) 
	{
		Lua_PushString(L, "");
		return 1;
	}
	int nStationId = (int)Lua_ValueToNumber(L,1);
	char szName[50];
	g_StationTabFile.GetString(nStationId + 1, "DESC", "无名城",  szName, 50 );
	Lua_PushString(L, szName);
	return 1;
}

int LuaGetWayPointName(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	if (Lua_GetTopIndex(L) <= 0 ) 
	{
		Lua_PushString(L, "");
		return 1;
	}
	int nWayPointId = (int)Lua_ValueToNumber(L,1);
	char szName[50];
	g_WayPointTabFile.GetString(nWayPointId + 1, "DESC", "未记录",  szName, 50 );
	Lua_PushString(L, szName);
	return 1;
}

//GetCityCount 
int LuaGetAllStationCount(Lua_State * L)
{
	int nCityCount = g_StationTabFile.GetHeight() - 1;
	if (nCityCount < 0) nCityCount = 0;
	Lua_PushNumber(L,nCityCount);
	return 1;
}

//cityid, price = GetCity(citynum, curcity)
int LuaGetCity(Lua_State * L)
{
	return 0;
}


int LuaGetPriceToWayPoint(Lua_State *L)
{
	
	int nCurStation = (int)Lua_ValueToNumber(L,1);
	int nDesWayPoint = (int)Lua_ValueToNumber(L,2);
	Lua_PushNumber(L, g_GetPriceToWayPoint(nCurStation, nDesWayPoint));
	return 1;
}

int LuaGetPriceToStation(Lua_State *L)
{
	int nCurStation	 = (int)Lua_ValueToNumber(L,1);
	int nNextStation = (int)Lua_ValueToNumber(L,2);
	Lua_PushNumber(L, g_GetPriceToStation(nCurStation, nNextStation));
	return 1;
}

int LuaGetStationPos(Lua_State * L)
{
	int nStationId = (int)Lua_ValueToNumber(L,1);
	char szPos[100] ;
	int nCount = 0;
	int nRow = g_StationTabFile.FindColumn("COUNT");
	g_StationTabFile.GetInteger(nStationId + 1, nRow,  0, &nCount);
	if (nCount <= 0) return 0;
	int nRandSect = g_Random(100) % nCount + 1;
	char szSectName[32];
	sprintf(szSectName, "SECT%d", nRandSect);
	char szValue[100];
	nRow = g_StationTabFile.FindColumn(szSectName);
	g_StationTabFile.GetString(nStationId + 1, nRow, "0,0,0", szValue, 100);
	
    int nX, nY, nWorld;
    const char *pcszTemp = szValue;
    
    nWorld = KSG_StringGetInt(&pcszTemp, 0);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    nX = KSG_StringGetInt(&pcszTemp, 0);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    nY = KSG_StringGetInt(&pcszTemp, 0);
	//sscanf(szValue, "%d,%d,%d", &nWorld, &nX, &nY);
	
    Lua_PushNumber(L,nWorld);
	Lua_PushNumber(L,nX);
	Lua_PushNumber(L,nY);
	return 3;
}

int LuaGetWayPointPos(Lua_State * L)
{
	int nWayPointId = (int)Lua_ValueToNumber(L,1);
	char szPos[100] ;
	int nCount = 0;
	char szValue[30];
	int nRow;
	nRow = g_WayPointTabFile.FindColumn("SECT");
	g_WayPointTabFile.GetString(nWayPointId + 1, nRow, "0,0,0", szValue, 30);
	int nX, nY, nWorld;
    const char *pcszTemp = szValue;
    
    nWorld = KSG_StringGetInt(&pcszTemp, 0);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    nX = KSG_StringGetInt(&pcszTemp, 0);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    nY = KSG_StringGetInt(&pcszTemp, 0);
	//sscanf(szValue, "%d,%d,%d", &nWorld, &nX, &nY);
	
	Lua_PushNumber(L,nWorld);
	Lua_PushNumber(L,nX);
	Lua_PushNumber(L,nY);
	return 3;
}

int LuaGetRank(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Lua_PushNumber(L,Npc[Player[nPlayerIndex].m_nIndex].m_btRankId);
	return 1;
}

int LuaSetRank(Lua_State * L)
{
	BYTE btRank = (BYTE)Lua_ValueToNumber(L,1);
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Npc[Player[nPlayerIndex].m_nIndex].m_btRankId = btRank;
	return 0;
}

int LuaSetObjPropState(Lua_State *L)
{
	int  nParamNum = ( int ) Lua_GetTopIndex(L);
	int nState  = 1;
	
	if (nParamNum >= 1)
	{
		nState = (int)Lua_ValueToNumber(L,1);
		nState = (nState == 0)?0 : 1;
	}
	
	int nIndex = 0;
	if ((nIndex = GetObjIndex(L)) < 0) 
		return 0;
	
	Object[nIndex].SetState(nState);
	return 0;
}


int	LuaGetServerName(Lua_State * L)
{
	char szServerName[100]  ;
	unsigned long   stServerNameLen = 100;

#ifndef __linux
	if (GetComputerName(szServerName, &stServerNameLen))
	{
		Lua_PushString(L, szServerName);
	}
	else
#else
	if (SOCKET_ERROR != gethostname(szServerName, sizeof(szServerName)))
	{
		Lua_PushString(L, szServerName);
	}
	else
#endif
	Lua_PushString(L, "");

	return 1;
}

//GetWharfCount(nDock)
int LuaGetDockCount(Lua_State * L)
{
	int nCount = 0;
	int nCurStation = 0;
	int nTotalCount = 0;
	int i  = 0;
	if (Lua_GetTopIndex(L) < 1 )  
	{
		goto DockCount;
	}
	
	nCurStation = (int)Lua_ValueToNumber(L,1);
	nTotalCount = g_DockPriceTabFile.GetHeight() - 1;
	
	for (i = 0; i < nTotalCount; i ++)
	{
		int nPrice = g_GetPriceToDock(nCurStation, i + 1);
		if (nPrice > 0) nCount ++;
	}
	
DockCount:
	Lua_PushNumber(L, nCount);
	return 1;
}

int LuaGetDockPrice(Lua_State * L)
{
	int nCurDock = (int)Lua_ValueToNumber(L,1);
	int nDesDock = (int)Lua_ValueToNumber(L,2);
	Lua_PushNumber(L, g_GetPriceToDock(nCurDock, nDesDock));
	return 1;
}

int LuaGetDock(Lua_State * L)
{
	int nCurDock = (int)Lua_ValueToNumber(L, 1);
	int nDock = (int)Lua_ValueToNumber(L, 2);
	int nCount = 0;
	int nTotalCount = g_DockPriceTabFile.GetHeight() - 1;
	int nGetDock = 0;
	
	for (int i = 0; i < nTotalCount; i ++)
	{
		int nPrice = g_GetPriceToDock(nCurDock, i + 1);
		if (nPrice > 0) 
		{
			nCount ++ ;
			if (nCount == nDock)
			{
				nGetDock = i + 1;
				break;
			}
		}
	}
	Lua_PushNumber(L, nGetDock);
	return 1;
}

int LuaGetDockName(Lua_State * L)
{
	int nDock  = (int)Lua_ValueToNumber(L, 1);
	char szName[100] ;
	
	if (nDock > g_DockPriceTabFile.GetHeight() - 1)
	{
		strcpy(szName, "未知码头");
		goto DockName;
	}
	
	g_DockTabFile.GetString(nDock + 1, "DESC", "未知码头", szName, 100);
	
DockName:
	Lua_PushString (L, szName);
	return 1;
}

int LuaGetDockPos(Lua_State * L)
{
	int nDock  = (int)Lua_ValueToNumber(L, 1);
	if (nDock > g_DockTabFile.GetHeight() - 1) 
	{
		printf("GetWharfPos Script Is Error!");
		return 0;
	}
	
	char szPos[100] ;
	int nCount = 0;
	int nRow = g_DockTabFile.FindColumn("COUNT");
	g_DockTabFile.GetInteger(nDock + 1, nRow,  0, &nCount);
	if (nCount <= 0) return 0;
	int nRandSect = g_Random(100) % nCount + 1;
	char szSectName[32];
	sprintf(szSectName, "SECT%d", nRandSect);
	char szValue[100];
	nRow = g_DockTabFile.FindColumn(szSectName);
	g_DockTabFile.GetString(nDock + 1, nRow, "0,0,0", szValue, 100);
	int nX, nY, nWorld;
    const char *pcszTemp = szValue;
    
    nWorld = KSG_StringGetInt(&pcszTemp, 0);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    nX = KSG_StringGetInt(&pcszTemp, 0);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    nY = KSG_StringGetInt(&pcszTemp, 0);
	//sscanf(szValue, "%d,%d,%d", &nWorld, &nX, &nY);
	
	Lua_PushNumber(L,nWorld);
	Lua_PushNumber(L,nX);
	Lua_PushNumber(L,nY);
	return 3;
}

int LuaGetWayPointFightState(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	int nFightState = 0;
	if (Lua_GetTopIndex(L) > 0 ) 
	{
		int nWayPointId = (int)Lua_ValueToNumber(L,1);
		g_WayPointTabFile.GetInteger(nWayPointId + 1, "FightState", 0,  &nFightState);
	}
	Lua_PushNumber(L, nFightState);
	return 1;
}

// SetMissionValue(valueid, value)
int LuaSetMissionValue(Lua_State * L)
{
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0) 
		return 0;
	
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) 
		return 0;
	
	int nValueId = (int)Lua_ValueToNumber(L, 1);
	int nValue = (int)Lua_ValueToNumber(L, 2);
	
	if (nValueId  < 0)
		return 0;
	SubWorld[nSubWorldIndex].m_MissionArray.SetMissionValue(nValueId, nValue);
	return 0;
}

int LuaGetMissionValue(Lua_State * L)
{
	int nResultValue = 0;
	int nSubWorldIndex = -1;
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1) 
		goto lab_getmissionvalue;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	
	if (nSubWorldIndex >= 0) 
	{
		int  nValueId = (int)Lua_ValueToNumber(L, 1);
		if (nValueId > 0)
			nResultValue = SubWorld[nSubWorldIndex].m_MissionArray.GetMissionValue(nValueId);
	}
	
lab_getmissionvalue:
	Lua_PushNumber(L, nResultValue);
	return 1;
}


// SetMissionValue(mapid/mapname, valueid, value)
int LuaSetGlobalMissionValue(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) 
		return 0;
	
	int nValueId = (int)Lua_ValueToNumber(L, 1);
	int nValue = (int)Lua_ValueToNumber(L, 2);
	
	if (nValueId  < 0)
		return 0;
	g_GlobalMissionArray.SetMissionValue(nValueId, nValue);
	return 0;
}

int LuaGetGlobalMissionValue(Lua_State * L)
{
	int nResultValue = 0;
	int nValueId = 0;
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1) 
		goto lab_getglobalmissionvalue;
	nValueId = (int)Lua_ValueToNumber(L, 1);
	if (nValueId < 0)
		goto lab_getglobalmissionvalue;
	
	nResultValue = g_GlobalMissionArray.GetMissionValue(nValueId);
	
lab_getglobalmissionvalue:
	Lua_PushNumber(L, nResultValue);
	return 1;
}

//StartMission(missionid)
int LuaInitMission(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nMissionId = (int)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0 )
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0) 
		return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0) 
		return 0;
	
	KMission Mission;
	Mission.SetMissionId(nMissionId);
	KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
	if (pMission)
	{
		_ASSERT(0);
		return 0;
	}
	
	pMission = SubWorld[nSubWorldIndex].m_MissionArray.Add();
	if (pMission)
	{
		pMission->m_MissionPlayer.Clear();
		pMission->SetMissionId(nMissionId);
		char szScript[MAX_PATH];
		g_MissionTabFile.GetString(nMissionId + 1, 2, "", szScript, MAX_PATH);
		if (szScript[0])
			Player[nPlayerIndex].ExecuteScript(szScript, "InitMission", "");
	}
	
	return 0;
}

int LuaRunMission(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nMissionId = (int)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0 )
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0) 
		return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0) 
		return 0;
	KMission Mission;
	Mission.SetMissionId(nMissionId);
	KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
	if (pMission)
	{
		char szScript[MAX_PATH];
		g_MissionTabFile.GetString(nMissionId + 1, 2, "", szScript, MAX_PATH);
		if (szScript[0])
			Player[nPlayerIndex].ExecuteScript(szScript, "RunMission", "");
	}
	
	return 0;
}
//CloseMission(missionId)
int LuaCloseMission(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nMissionId = (int)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0 )
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0) 
		return 0;
	KMission StopMission;
	StopMission.SetMissionId(nMissionId);
	KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&StopMission);
	if (pMission)
	{
		char szScript[MAX_PATH];
		g_MissionTabFile.GetString(nMissionId + 1, 2, "", szScript, MAX_PATH);
		if (szScript[0])
			pMission->ExecuteScript(szScript, "EndMission",0);
		pMission->StopMission();
		SubWorld[nSubWorldIndex].m_MissionArray.Remove(pMission);
		
	}
	return 0;
}
//StopMissionTimer(missionid, timerid)
int LuaStopMissionTimer(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2) 
		return 0;
	int nMissionId = (int)Lua_ValueToNumber(L, 1);
	int nTimerId = (int)Lua_ValueToNumber(L, 2);
	int nSubWorldIndex = GetSubWorldIndex(L);
	
	if (nMissionId < 0 || nTimerId < 0 ) 
		return 0;
	
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			KTimerTaskFun StopTimer;
			StopTimer.SetTimer(1, nTimerId);
			KTimerTaskFun * pTimer = pMission->m_cTimerTaskSet.GetData(&StopTimer);
			if (pTimer)
			{
				pTimer->CloseTimer();
				pMission->m_cTimerTaskSet.Remove(pTimer);
			}
		}
		
	}
	
	return 0;
}

//StartMissionTimer(missionid, timerid, time)
int LuaStartMissionTimer(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 3) 
		return 0;
	int nMissionId = (int)Lua_ValueToNumber(L, 1);
	int nTimerId = (int)Lua_ValueToNumber(L, 2);
	int nTimeInterval = (int)Lua_ValueToNumber(L, 3);
	int nSubWorldIndex = GetSubWorldIndex(L);
	
	if (nMissionId < 0 || nTimerId < 0 || nTimeInterval < 0) 
		return 0;
	
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			KTimerTaskFun * pTimer = pMission->m_cTimerTaskSet.Add();
			if (pTimer)
			{
				pTimer->SetTimer(nTimeInterval, nTimerId);
			}
		}
		
	}
	return 0;
}
//SetTempRev(worldid, x, y)
int LuaSetDeathRevivalPos(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0) 
		return 0;
	int nParamCount = Lua_GetTopIndex(L);
	
	PLAYER_REVIVAL_POS * pTempRev = Player[nPlayerIndex].GetDeathRevivalPos();
	
	if (nParamCount > 2)
	{
		pTempRev->m_nSubWorldID  = (int) Lua_ValueToNumber(L, 1);
		pTempRev->m_nMpsX = (int) Lua_ValueToNumber(L, 2);
		pTempRev->m_nMpsY = (int) Lua_ValueToNumber(L, 3); 
	}
	else if (nParamCount == 1)
	{
		pTempRev->m_nSubWorldID = SubWorld[Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex].m_SubWorldID;
		POINT Pos;
		int nRevId = (int) Lua_ValueToNumber(L, 1);
		g_SubWorldSet.GetRevivalPosFromId(pTempRev->m_nSubWorldID, nRevId, &Pos);
		pTempRev->m_ReviveID = nRevId;
		pTempRev->m_nMpsX = Pos.x;
		pTempRev->m_nMpsY = Pos.y;
	}
	else 
	{
		return 0;
	}
	
	return 0;
}

int LuaPlayerExecuteRevive(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
		return 0;
	int nParamCount = Lua_GetTopIndex(L);

	int nReviveType = REMOTE_REVIVE_TYPE;

	if (nParamCount > 1)
		nReviveType  = (int) Lua_ValueToNumber(L, 1);

	Player[nPlayerIndex].Revive(nReviveType);
	return 0;
}


//AddMSPlayer(MissionId, PlayerIndex, groupid); / AddMSPlayer(MissionId, groupid)
int LuaAddMissionPlayer(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) 
		return 0;
	int nMissionId = 0;
	int nPlayerIndex = 0;
	int nGroupId = 0;
	if (nParamCount >=3)
	{
		nMissionId = (int)Lua_ValueToNumber(L,1);
		nPlayerIndex = (int )Lua_ValueToNumber(L,2);
		nGroupId = (int) Lua_ValueToNumber(L,3);
	}
	else
	{
		nMissionId = (int)Lua_ValueToNumber(L,1);
		nGroupId = (int) Lua_ValueToNumber(L,2);
		nPlayerIndex = GetPlayerIndex(L);
	}
	
	if (nMissionId < 0 || nPlayerIndex <= 0 || nGroupId <0)
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			pMission->AddPlayer(nPlayerIndex, Player[nPlayerIndex].m_dwID, nGroupId);
		}
	}
	return 0;
}

int LuaRevivalAllNpc(Lua_State * L)
{
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		SubWorld[nSubWorldIndex].RevivalAllNpc();
	}
	return 0;
}

//RemoveMSPlayer(MissionId, PlayerIndex, groupid)
int LuaRemoveMissionPlayer(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) 
		return 0;
	int nMissionId = 0;
	int nPlayerIndex = 0;
	int nGroupId = 0;
	if (nParamCount >=3)
	{
		nMissionId = (int)Lua_ValueToNumber(L,1);
		nPlayerIndex = (int )Lua_ValueToNumber(L,2);
		nGroupId = (int) Lua_ValueToNumber(L,3);
	}
	else
	{
		nMissionId = (int)Lua_ValueToNumber(L,1);
		nGroupId = (int) Lua_ValueToNumber(L,2);
		nPlayerIndex = GetPlayerIndex(L);
	}
	
	if (nMissionId < 0 || nPlayerIndex <= 0 || nGroupId <0)
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			pMission->RemovePlayer(nPlayerIndex);
			//pMission->AddPlayer(nPlayerIndex, Player[nPlayerIndex].m_dwID, nGroupId);
		}
	}
	return 0;
}
//GetNextPlayer(mission, idx,group)
int LuaGetNextPlayer(Lua_State * L)
{
	unsigned long nPlayerIndex = 0;
	
	if (Lua_GetTopIndex(L) < 2)
	{
		Lua_PushNumber(L ,0);
		Lua_PushNumber(L, 0);
		return 2;
	}
	
	int nMissionId	= (int)Lua_ValueToNumber(L, 1);
	int nIdx		= (int)Lua_ValueToNumber(L, 2);
	int nGroup		= (int)Lua_ValueToNumber(L, 3);
	int nSubWorldIndex = GetSubWorldIndex(L);
	int nResultIdx = 0;
	
	if (nMissionId < 0 || nIdx < 0 || nGroup < 0) 
		goto lab_getnextplayer;
	
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			nResultIdx = pMission->GetNextPlayer(nIdx, nGroup, nPlayerIndex);
		}
	}
	
lab_getnextplayer:	
	Lua_PushNumber(L, nResultIdx);
	Lua_PushNumber(L, nPlayerIndex);
	return 2;
}

//MSMsg2Group(missionid, string , group)
int LuaMissionMsg2Group(Lua_State * L)
{
	int nMissionId = (int)Lua_ValueToNumber(L,1);
	char * strMsg = (char *)Lua_ValueToString(L, 2);
	int	nGroupId = (int) Lua_ValueToNumber(L, 3);
	
	if (nMissionId < 0 || !strMsg || nGroupId <0)
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			pMission->Msg2Group(strMsg, nGroupId);
		}
	}
	
	return 0;
}

//MSMsg2Group(missionid, string)
int LuaMissionMsg2All(Lua_State * L)
{
	int nMissionId = (int)Lua_ValueToNumber(L,1);
	char * strMsg = (char *)Lua_ValueToString(L, 2);
	
	if (nMissionId < 0 || !strMsg)
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			pMission->Msg2All(strMsg);
		}
	}
	
	return 0;
}

//MSMsg2Group(missionid, string , group)
int LuaMissionMsg2Player(Lua_State * L)
{
	int nMissionId = (int)Lua_ValueToNumber(L,1);
	char * strMsg = (char *)Lua_ValueToString(L, 2);
	int	nPlayerIndex = (int) Lua_ValueToNumber(L, 3);
	
	if (nMissionId < 0 || !strMsg || nPlayerIndex <0)
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			pMission->Msg2Group(strMsg, nPlayerIndex);
		}
	}
	
	return 0;
}

int LuaMissionPlayerCount(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	unsigned long ulCount = 0;
	int nMissionId = 0;
	int nGroupId = 0;
	int nSubWorldIndex = 0;
	if (nParamCount < 1) 
		goto lab_getmissionplayercount;
	
	if (nParamCount >= 2)
	{
		nMissionId = (int)Lua_ValueToNumber(L,1);
		nGroupId = (int) Lua_ValueToNumber(L,2);
	}
	else
	{
		nMissionId = (int)Lua_ValueToNumber(L,1);
	}
	
	if (nMissionId < 0 || nGroupId <0)
		goto lab_getmissionplayercount;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			ulCount = pMission->GetGroupPlayerCount(nGroupId);
		}
	}
	
lab_getmissionplayercount:
	Lua_PushNumber(L, ulCount);
	return 1;
}

int LuaSetPlayerDeathScript(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) 
		return 0;
	char * szScript = (char *)Lua_ValueToString(L, 1);
	Player[nPlayerIndex].m_dwDeathScriptId = g_FileName2Id(szScript);
	return 0;
}

int LuaNpcIndexToPlayerIndex(Lua_State * L)
{
	int nResult = 0;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <=  0 || nNpcIndex >= MAX_NPC)
		goto lab_npcindextoplayerindex;
	
	if (Npc[nNpcIndex].m_Index > 0 && Npc[nNpcIndex].IsPlayer())
	{
		if (Npc[nNpcIndex].GetPlayerIdx() > 0)
			nResult = Npc[nNpcIndex].GetPlayerIdx();
	}
	
lab_npcindextoplayerindex:
	Lua_PushNumber(L, nResult);
	return 1;
	
}
//  
int LuaGetMissionPlayer_PlayerIndex(Lua_State * L)
{
	unsigned long nResult = 0;
	int nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 2) 
		goto lab_getmissionplayer_npcindex;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		int nMissionId = (int)Lua_ValueToNumber(L, 1);
		int nDataIndex = (int)Lua_ValueToNumber(L, 2);
		if (nMissionId < 0 || nDataIndex < 0)
			goto lab_getmissionplayer_npcindex;
		
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			nResult = pMission->GetMissionPlayer_PlayerIndex(nDataIndex);
		}
	}
	
lab_getmissionplayer_npcindex:
	Lua_PushNumber(L,nResult);
	return 1;
}

int LuaGetMissionPlayer_DataIndex(Lua_State * L)
{
	unsigned long nResult = 0;
	int nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 2) 
		goto lab_getmissionplayer_dataindex;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		int nMissionId = (int)Lua_ValueToNumber(L, 1);
		int nPlayerIndex = (int)Lua_ValueToNumber(L, 2);
		if (nMissionId < 0 || nPlayerIndex < 0)
			goto lab_getmissionplayer_dataindex;
		
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			nResult = pMission->GetMissionPlayer_DataIndex(nPlayerIndex);
		}
	}
	
lab_getmissionplayer_dataindex:
	Lua_PushNumber(L,nResult);
	return 1;
}

//SetMPParam(missionid, nDidx, vid, v)
int LuaSetMissionPlayerParam(Lua_State * L)
{
	int nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 4) 
		return 0;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		int nMissionId = (int)Lua_ValueToNumber(L, 1);
		int nDataIndex = (int)Lua_ValueToNumber(L, 2);
		int nParamId =	 (int)Lua_ValueToNumber(L ,3);
		int nValue =	 (int )Lua_ValueToNumber(L, 4);
		
		if (nMissionId < 0 || nDataIndex < 0 || nParamId > 2)
			return 0;
		
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			if (nParamId == 1)
				pMission->m_MissionPlayer.SetParam1(nDataIndex, nValue);
			else
				pMission->m_MissionPlayer.SetParam2(nDataIndex, nValue);
		}
	}
	return 0;
}

int LuaGetMissionPlayerParam(Lua_State * L)
{
	int nResult = 0;
	int nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 3) 
		goto lab_getmissionplayerparam;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		int nMissionId = (int)Lua_ValueToNumber(L, 1);
		int nDataIndex = (int)Lua_ValueToNumber(L, 2);
		int nParamId =	 (int)Lua_ValueToNumber(L ,3);
		
		if (nMissionId < 0 || nDataIndex < 0 || nParamId > 2)
			goto lab_getmissionplayerparam;
		
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			if (nParamId == 1)
				nResult = pMission->m_MissionPlayer.GetParam1(nDataIndex);
			else
				nResult = pMission->m_MissionPlayer.GetParam2(nDataIndex);
		}
	}
lab_getmissionplayerparam:
	Lua_PushNumber(L, nResult);
	return 1;
}

int LuaGetPlayerMissionGroup(Lua_State * L)
{
	int nResult = 0;
	int nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 2) 
		goto lab_getmissionplayergroup;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		int nMissionId = (int)Lua_ValueToNumber(L, 1);
		int nNpcIndex = (int)Lua_ValueToNumber(L, 2);
		
		if (nMissionId < 0 || nNpcIndex < 0)
			goto lab_getmissionplayergroup;
		
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			nResult = pMission->GetMissionPlayer_GroupId(nNpcIndex);
		}			
	}
lab_getmissionplayergroup:
	Lua_PushNumber(L ,nResult);
	return 1;
	
}

int LuaSetPlayerRevivalOptionWhenLogout(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) 
		return 0;
	int nType = (int)Lua_ValueToNumber(L, 1);
	
	if (nType)
		Player[nPlayerIndex].SetLoginType(1);
	else
		Player[nPlayerIndex].SetLoginType(0);
	
	return 0;
}



int LuaGetPlayerRevivalOptionWhenLogout(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) 
		return 0;
	
	if (Player[nPlayerIndex].IsUseReviveIdWhenLogin())
		Lua_PushNumber(L ,1);
	else
		Lua_PushNumber(L ,0);
	
	return 1;
}




int LuaReLoadScript(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;

	char * szScript = (char *)Lua_ValueToString(L, 1);
	ReLoadScript(szScript);
	return 0;
}











int LuaSetPlayerPKValue(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;	
	int nPKValue = (int)Lua_ValueToNumber(L,1);
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) 
		return 0;
	Player[nPlayerIndex].m_cPK.SetPKValue(nPKValue);
	return 0;
}

int LuaGetPlayerPKValue(Lua_State * L)
{
	int nPKValue = 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getplayerpkvalue;
	
	if (Player[nPlayerIndex].m_nIndex <= 0) 
		goto lab_getplayerpkvalue;
	nPKValue = Player[nPlayerIndex].m_cPK.GetPKValue();
	
lab_getplayerpkvalue:
	Lua_PushNumber(L, nPKValue);
	return 1;
}


int	LuaGetCurNpcIndex(Lua_State * L)
{
	int nNpcIndex = 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getcurnpcindex;
	
lab_getcurnpcindex:
	Lua_PushNumber(L, Player[nPlayerIndex].m_nIndex);
	return 1;
}




//showladder(count, ladderid1, ladderid2.....)
int LuaShowLadder(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) 
		return 0;
	int nLadderCount = (DWORD) Lua_ValueToNumber(L, 1);
	if (nLadderCount <= 0)
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	
	if (nLadderCount > nParamCount - 1 )
		nLadderCount = nParamCount - 1;
	BYTE Buffer[sizeof(LADDER_LIST) + 50 * sizeof(DWORD)];
	LADDER_LIST * pLadderList = (LADDER_LIST*)&Buffer;
	pLadderList->ProtocolType = s2c_ladderlist;
	pLadderList->nCount = nLadderCount;
	pLadderList->wSize = sizeof(LADDER_LIST) + nLadderCount * sizeof(DWORD) - 1;
	for (int i = 0; i < nLadderCount; i ++)
	{
		pLadderList->dwLadderID[i] = (DWORD)Lua_ValueToNumber(L, i + 2);
	}
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &Buffer, pLadderList->wSize + 1);
	return 0;
	
}

int LuaSwearBrother(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	int nTeamId = Lua_ValueToNumber(L, 1);
	
	if (nTeamId >= MAX_TEAM || nTeamId < 0)
		return 0;
	
	KPlayerChat::STRINGLIST BrotherList;
	_ASSERT(g_Team[nTeamId].m_nCaptain > 0);

	std::string strCapName;
	strCapName = Npc[Player[g_Team[nTeamId].m_nCaptain].m_nIndex].Name;
	BrotherList.push_back(strCapName);

	for (int i  = 0; i < g_Team[nTeamId].m_nMemNum; i++)
	{
		int nPlayerIndex = g_Team[nTeamId].m_nMember[i];
		if ( nPlayerIndex > 0 && nPlayerIndex < MAX_PLAYER)
		{
			std::string strName;
			strName = 	Npc[Player[nPlayerIndex].m_nIndex].Name;
			BrotherList.push_back(strName);
		}
	}
	
	KPlayerChat::MakeBrother(BrotherList);
	return 0;
}

int LuaMakeEnemy(Lua_State * L)
{
	
	return 0;
}


int LuaAddLeadExp(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	
	int nLeadExp = (int)Lua_ValueToNumber(L, 1);
	Player[nPlayerIndex].AddLeadExp(nLeadExp);
	return 0;
}

int LuaGetLeadLevel(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	int nLeadLevel = 0;
	if (nPlayerIndex <= 0)
		goto lab_getleadlevel;
	nLeadLevel = (int)Player[nPlayerIndex].m_dwLeadLevel;
	
lab_getleadlevel:
	Lua_PushNumber(L, nLeadLevel);
	return 1;
}

int LuaAddMagicPoint(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	int nSkillPoint = (int)Lua_ValueToNumber(L, 1);
	
	Player[nPlayerIndex].m_nSkillPoint += nSkillPoint;
	if (Player[nPlayerIndex].m_nSkillPoint < 0)
		Player[nPlayerIndex].m_nSkillPoint = 0;
	
	S2C_PLAYER_POINT_SYNC NetCommand;
	NetCommand.ProtocolType = s2c_playerpointsync;
	NetCommand.nType = 1;
	NetCommand.nValue = nSkillPoint;
	if(g_pServer && Player[nPlayerIndex].m_nNetConnectIdx != -1)
			g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx,&NetCommand,sizeof(S2C_PLAYER_POINT_SYNC));
	return 0;
}

int LuaGetMagicPoint(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	int nSkillPoint = 0;
	if (nPlayerIndex <= 0)
		goto lab_getmagicpoint;
	
lab_getmagicpoint:
	Lua_PushNumber(L, Player[nPlayerIndex].m_nSkillPoint);
	return 1;
}

int LuaAddPropPoint(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	int nPropPoint = 0;
	if (nPlayerIndex <= 0)
		return 0;
	nPropPoint = (int)Lua_ValueToNumber(L, 1);
	Player[nPlayerIndex].m_nAttributePoint += nPropPoint;

	S2C_PLAYER_POINT_SYNC NetCommand;
	NetCommand.ProtocolType = s2c_playerpointsync;
	NetCommand.nType = 0;
	NetCommand.nValue = nPropPoint;
	if(g_pServer && Player[nPlayerIndex].m_nNetConnectIdx != -1)
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx,&NetCommand,sizeof(S2C_PLAYER_POINT_SYNC));
	return 0;
}


//PayExtPoint
int LuaPayExtPoint(Lua_State * L)
{
	int nResult = 0;
	int nPay = 0;
	int nPlayerIndex = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_payextpoint;
	
	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_payextpoint;
	nPay = Lua_ValueToNumber(L, 1);
	if (nPay < 0)
		goto lab_payextpoint;
	nResult = Player[nPlayerIndex].PayExtPoint(nPay);

lab_payextpoint:
	Lua_PushNumber(L, nResult);
	return 1;
}

//SetExtPoint
int LuaSetExtPoint(Lua_State * L)
{
	int nResult = 0;
	int nPoint = 0;
	int nChange = 0;
	int nPlayerIndex = 0;
	if (Lua_GetTopIndex(L) < 2)
	return 0;
	
	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	return 0;
	nPoint = Lua_ValueToNumber(L, 1);
	nChange = Lua_ValueToNumber(L, 2);

	Player[nPlayerIndex].SetExtPoint(nPoint,nChange);

	return 1;
}
//PayExtPoint
int LuaGetExtPoint(Lua_State * L)
{
	int nResult = 0;
	int nPlayerIndex = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_getextpoint;
	
	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getextpoint;
	
	nResult = Player[nPlayerIndex].GetExtPoint();
	
lab_getextpoint:
	Lua_PushNumber(L, nResult);
	return 1;
}

int LuaGetRestPropPoint(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	int nPropPoint = 0;
	if (nPlayerIndex <= 0)
		goto lab_getrestproppoint;
	nPropPoint = Player[nPlayerIndex].m_nAttributePoint;
	
lab_getrestproppoint:
	Lua_PushNumber(L, nPropPoint);
	return 1;
}

//Msg2GM(str, id)
int LuaMsgToGameMaster(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if ( nParamCount < 1 ) 
		return 0;
	int nParamID = 0;
	const char * szMsg = Lua_ValueToString(L, 1) ;
	if (!szMsg) 
		return 0;

	if (nParamCount < 2)
	{
		nParamID = 0;
	}
	else
	{
		nParamID = (int) Lua_ValueToNumber(L, 2);
	}

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0) 
	{
		char szID[32];

		sprintf(szID, "%d", nParamID);
		//KPlayerChat::SendInfoToGM(Player[nPlayerIndex].m_AccoutName, Npc[Player[nPlayerIndex].m_nIndex].Name, (char *) szMsg, strlen(szMsg) );
		KPlayerChat::SendInfoToGM("GM", szID, (char *) szMsg, strlen(szMsg) );
	}
	return 0;
}

//Msg2IP(IP, ID, str)
int LuaMsgToIP(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if ( nParamCount < 3 ) 
		return 0;
	int nIP = 0;
	const char * szIP = Lua_ValueToString(L, 1) ;
	nIP = _a2ip(szIP);
	if (nIP == 0)
		return 0;

	int nID = (int) Lua_ValueToNumber(L, 2);
	const char * szMsg = Lua_ValueToString(L, 3) ;
	if (!szMsg) 
		return 0;

	int nParamID = 0;
	if (nParamCount < 4)
	{
		nParamID = 0;
	}
	else
	{
		nParamID = (int) Lua_ValueToNumber(L, 4);
	}
	char szID[32];
	sprintf(szID, "%d", nParamID);
	KPlayerChat::SendInfoToIP(nIP, nID, "GM", szID, (char *) szMsg, strlen(szMsg) );
	return 0;
}

int LuaGetPlayerInfo(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	char szDesMsg[250];
	char szMsg[800];
	int nNpcIdx = 0;
	KNpc * pNpc = NULL;
	KPlayer* pPlayer = NULL;
	szDesMsg[0] = 0;
	if (nPlayerIndex <= 0)
		goto lab_getplayerinfo;
	
	pPlayer = &Player[nPlayerIndex];
	nNpcIdx = pPlayer->m_nIndex;
	pNpc = &Npc[nNpcIdx];
	sprintf(szMsg, "Name:%s,Lvl:%d,IP:%s,Lf:%d|%d,Mn:%d|%d,Mny:%d,sp:%d,ap:%d", pNpc->Name, pNpc->m_Level, g_pServer->GetClientInfo(pPlayer->m_nNetConnectIdx), pNpc->m_CurrentLife, pNpc->m_CurrentLifeMax, pNpc->m_CurrentMana, pNpc->m_CurrentManaMax, pPlayer->m_ItemList.GetMoney(room_equipment), pPlayer->m_nSkillPoint, pPlayer->m_nAttributePoint );
	g_StrCpyLen(szDesMsg, szMsg, 250);
	
lab_getplayerinfo:
	Lua_PushString(L, szDesMsg);
	return 1;
}

int LuaGetIP(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	char szDesMsg[200];
	szDesMsg[0] = 0;

	KPlayer *pPlayer = NULL;

	if (nPlayerIndex <= 0)
		goto lab_getplayerip;

	pPlayer = &Player[nPlayerIndex];
	if (pPlayer->m_byLixian != LIXIAN_DOING)
		strcpy(szDesMsg, g_pServer->GetClientInfo(pPlayer->m_nNetConnectIdx));
	
lab_getplayerip:
	Lua_PushString(L, szDesMsg);
	return 1;
}



// UYTHAC
/*int LuaIsLiXian(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	printf("Debug Set Uy Thac\n");
	if (!Npc[Player[nPlayerIndex].m_nIndex].m_FightMode)
		Player[nPlayerIndex].m_nLiXian = 1;
	return 1;
}

int LuaNotLiXian(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	Player[nPlayerIndex].m_nLiXian = 0;
	return 1;
}
*/
//END
// MASK
int LuaSetMask(Lua_State * L)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex <= 0)
			return 0;
		
		if (Lua_IsNumber(L,1))
		{
			Player[nPlayerIndex].m_ItemList.SetMaskLock(TRUE);
			Npc[Player[nPlayerIndex].m_nIndex].m_MaskType = (int)Lua_ValueToNumber(L,1);
		}
		return 0;
	}

	int LuaReSetMask(Lua_State * L)
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex <= 0)
			return 0;
		
		Player[nPlayerIndex].m_ItemList.SetMaskLock(FALSE);
		int nIdx = Player[nPlayerIndex].m_ItemList.GetEquipment(itempart_mask);
		
		Npc[Player[nPlayerIndex].m_nIndex].m_MaskType = Item[nIdx].GetBaseMagic();
		return 0;
	}
	//END
int LuaSetNpcKind(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	int nKind = (int)Lua_ValueToNumber(L, 2);
	if (nKind < kind_normal || nKind >= kind_num)
		return 0;

	Npc[nNpcIndex].m_Kind = nKind;
	return 0;
}


int LuaSetFiFong(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount > 2)
	{
		if (Lua_IsNumber(L, 1) && Lua_IsNumber(L, 2))
		{
			int nIdx = Lua_ValueToNumber(L, 1);
			int nValue = Lua_ValueToNumber(L, 2) - 2;
			int nGenre = Item[nIdx].GetGenre();
			int nDetailType = Item[nIdx].GetDetailType();
			int nGold = Item[nIdx].m_SpecialParam.uItemType;
			if (nIdx && nValue && nGenre == item_equip && nDetailType == equip_armor && nGold <=0)
			{
				Item[nIdx].SetFiFongArmor(nValue);
				Player[nPlayerIndex].m_ItemList.UpdateCurItem(nIdx);
			}
		}
	}
	else if (nParamCount > 1)
	{
		int nValue = (int)Lua_ValueToNumber(L, 1) - 2;
		if (Lua_IsNumber(L, 1) && nValue > 0)
		{
			Npc[Player[nPlayerIndex].m_nIndex].m_FifongType = nValue;
		}
	}
	return 0;
}

int LuaSetFiFongArmor(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	// if (Lua_GetTopIndex(L) < 3)
		// return 0;
		int nValue = Lua_ValueToNumber(L, 1) - 2;
		int nIdQuestKey = Lua_ValueToNumber(L, 2);
		int nIdx = Player[nPlayerIndex].m_ItemList.GetEquipment(itempart_body);
		int nGenre = Item[nIdx].GetGenre();
		int nDetailType = Item[nIdx].GetDetailType();
		int nLevelItem = Item[nIdx].GetLevel();
		int nGold = Item[nIdx].m_SpecialParam.uItemType;
		if (nIdx)
		{
			if (nGenre == item_equip && nDetailType == equip_armor)
			{
				if (nGold<=0)
				{
					int nFiFongArmor = Item[nIdx].GetFiFongArmor();
					if (nFiFongArmor <= 0)
					{
						int gioitinh = Npc[Player[nPlayerIndex].m_nIndex].m_nSex;
						if (gioitinh == 0)
						{
							if (nLevelItem < 5)
							{
								nValue = 27;
							}
							else if (nLevelItem >= 5 && nLevelItem < 8)
							{
								nValue = 28;
							
							}
							else
							{
								nValue = 29;
							}
						}
						else
						{
							if (nLevelItem < 5)
							{
								nValue = 27;
							}
							else if (nLevelItem >= 5 && nLevelItem < 8)
							{
								nValue = 28;
							
							}
							else
							{
								nValue = 29;
							}
						}	
							Npc[Player[nPlayerIndex].m_nIndex].m_FifongType = nValue;
							Item[nIdx].SetFiFongArmor(nValue);
							Player[nPlayerIndex].m_ItemList.RemoveTaskItem(251);
							Player[nPlayerIndex].m_ItemList.UpdateCurItem(nIdx);
							return 1;
					}
					else
					{
							nValue = 0;
							Npc[Player[nPlayerIndex].m_nIndex].m_FifongType = nValue;
							Item[nIdx].SetFiFongArmor(nValue);
							Player[nPlayerIndex].m_ItemList.RemoveTaskItem(251);
							Player[nPlayerIndex].m_ItemList.UpdateCurItem(nIdx);
							return 1;
					}
				}
				else if (nGold == 1)
				{
					int nFiFongArmor = Item[nIdx].GetFiFongArmor();
					if (nFiFongArmor <= 0)
					{
						nValue = 27 + g_Random(3); //xxx 
						Npc[Player[nPlayerIndex].m_nIndex].m_FifongType = nValue;
						Item[nIdx].SetFiFongArmor(nValue);
						Player[nPlayerIndex].m_ItemList.RemoveTaskItem(251);
						Player[nPlayerIndex].m_ItemList.UpdateCurItem(nIdx);
						return 1;
					}
					else
					{
						nValue = 0;
							Npc[Player[nPlayerIndex].m_nIndex].m_FifongType = nValue;
							Item[nIdx].SetFiFongArmor(nValue);
							Player[nPlayerIndex].m_ItemList.RemoveTaskItem(251);
							Player[nPlayerIndex].m_ItemList.UpdateCurItem(nIdx);
							return 1;
					}
					
				}
				else
				{
					char szMsg[100];
					sprintf(szMsg, "Y ph鬰 b筺 產ng m芻 kh玭g th?kh秏 phi phong!!!");		
					KPlayerChat::SendSystemInfo(1, nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *) szMsg, strlen(szMsg) );
				}
			}
		}
	return 0;
}

int LuaGetFiFongArmor(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0) 
	{
		int nIdx = Player[nPlayerIndex].m_ItemList.GetEquipment(itempart_body);
		int nValue = (int)Item[nIdx].GetFiFongArmor();		
		Lua_PushNumber(L, nValue);
	}
	else
		Lua_PushNil(L);
	
	return 1;
}
	
int LuaSetDeathPunish(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex < 0) 
		return 0;

		int nState = Lua_ValueToNumber(L, 1);
		if (nState == 0)
			Npc[Player[nPlayerIndex].m_nIndex].m_nCurPKPunishState = 0;
		else
			Npc[Player[nPlayerIndex].m_nIndex].m_nCurPKPunishState = enumDEATH_MODE_PKBATTLE_PUNISH;

	return 0;
}



int LuaHideNpc(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;
	
	int nNpcIndex  = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	}
	else 
	{
		const char * szName = Lua_ValueToString(L, 1);
		int nSubWorldIndex = GetSubWorldIndex(L);
		if (nSubWorldIndex < 0)
			return 0;
		
		nNpcIndex = SubWorld[nSubWorldIndex].FindNpcFromName(szName);
	}
	
	if (nNpcIndex > 0 || nNpcIndex < MAX_NPC)
	{
		int nFrame = Lua_ValueToNumber(L, 2);
		if (nFrame <= 0)
			nFrame = 1;
		
		Npc[nNpcIndex].ExecuteRevive();
		Npc[nNpcIndex].m_Frames.nTotalFrame = nFrame;
		Npc[nNpcIndex].m_Frames.nCurrentFrame = 0;
	}
	
	return 0;
}





int LuaDelItemIdx(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        int nIdx = (int)Lua_ValueToNumber(L, 1);
		if (nIdx > 0 && Player[nPlayerIndex].m_ItemList.CheckHaveItem(nIdx))
		{
		if (Item[nIdx].GetGenre() == item_task)
		{
		if (Item[nIdx].IsStack() && Item[nIdx].GetVersion() > 1)
		{
	

		Item[nIdx].SetVersion(Item[nIdx].GetVersion()-1);

		
		PLAYER_ITEM_UPDATE_VERSION	IDC;
	    IDC.ProtocolType = s2c_playerupdateitemvertion;
	    IDC.m_ID = Item[nIdx].GetID();
		IDC.m_Vesion = Item[nIdx].GetVersion();

	    if (g_pServer)
		    g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(PLAYER_ITEM_UPDATE_VERSION));


		}
		else
		{
		ItemSet.m_checkdel = 12;
		Player[nPlayerIndex].m_ItemList.Remove(nIdx);
		ItemSet.Remove(nIdx);
		}		
		}
		}

	
	}
	
	return 0;	
}





int LuaDelEquipIdx(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        int nIdx = (int)Lua_ValueToNumber(L, 1);
		if (nIdx > 0 && Player[nPlayerIndex].m_ItemList.CheckHaveItem(nIdx))
		{
		if (Item[nIdx].GetGenre() == item_equip)
		{	
			ItemSet.m_checkdel = 13;
			Player[nPlayerIndex].m_ItemList.Remove(nIdx);
			ItemSet.Remove(nIdx);			
		}
		}

	
	}
	
	return 0;	
}




int LuaGetNpcSeries(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Lua_PushNumber(L,Npc[nNpcIndex].m_Series);
	return 1;
}



int LuaSetNpcSeries(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0 ;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex > 0)
	{
    int iSeries = Lua_ValueToNumber(L, 2);
    Npc[nNpcIndex].m_Series = iSeries;
	}

	return 0;
}


int LuaSetNpcExp(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	int nExp = (int)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentExperience = nExp;
	if (nParamNum>2 && (int)Lua_ValueToNumber(L, 3)>0)
		Npc[nNpcIndex].m_Experience = nExp;
	return 0;
}

int LuaSetNpcLife(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	int nLife = (int)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentLife = nLife;
	Npc[nNpcIndex].m_CurrentLifeMax = nLife;

	if (nParamNum>2 && (int)Lua_ValueToNumber(L, 3)>0)
		Npc[nNpcIndex].m_LifeMax = nLife;
	return 0;
}


int LuaSetNpcName(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	if (Lua_IsNumber(L,2))
	{
		KTabFile Replace;
		Replace.Load(NPC_NAME_FILE);
		Replace.GetString((int)Lua_ValueToNumber(L,2)+2, "targetname","", Npc[nNpcIndex].Name, sizeof(Npc[nNpcIndex].Name));
	}
	else if (Lua_IsString(L,2))
		strcpy(Npc[nNpcIndex].Name, (char*)Lua_ValueToString(L,2));

	return 0;
}

int LuaGetNpcName(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Lua_PushString(L, Npc[nNpcIndex].Name);
	return 1;
}


int LuaRestAP(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0 && nPlayerIndex < MAX_PLAYER)
	{
int suc_manh = (int)Lua_ValueToNumber(L, 1);
int than_phap = (int)Lua_ValueToNumber(L, 2);
int sinh_khi = (int)Lua_ValueToNumber(L, 3);
int noi_cong = (int)Lua_ValueToNumber(L, 4);
Player[nPlayerIndex].PlayerRestAP(suc_manh,than_phap,sinh_khi,noi_cong);
	}
	
	return 0;	
}

int LuaGetLevelExp(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
    Lua_PushNumber(L,Player[nPlayerIndex].m_nNextLevelExp);
	else
	Lua_PushNil(L);
	
	return 1;	
}



int LuaSetx2Exp(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	int nTimex2 = (int) Lua_ValueToNumber(L,1);
	if (nPlayerIndex <= 0) return 0;

Player[nPlayerIndex].m_cTask.SetSaveVal(172,nTimex2);
	
	return 0;
}

int LuaSetx2Skill(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	int nTimex2 = (int) Lua_ValueToNumber(L,1);
	if (nPlayerIndex <= 0) return 0;

Player[nPlayerIndex].m_cTask.SetSaveVal(173,nTimex2);

	return 0;
}


int LuaGetComputerName(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushString(L, Player[nPlayerIndex].m_szCPUIPName);
	}
	else
		Lua_PushNil(L);
	
	return 1;	
}

//edit by phong kieu ham lua viet them GetAccount
int LuaGetPlayerAccount(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushString(L, Player[nPlayerIndex].m_AccoutName);
	}
	else
	{
		Lua_PushString(L, "");
	}
	return 1;
}


int LuaSetLevel(Lua_State * L)
{	
	int nPlayerIndex = GetPlayerIndex(L);
	int nNumber = Lua_GetTopIndex(L);
	if (nPlayerIndex <= 0 || nNumber < 1)	
	{
		Lua_PushNumber(L,0);
		return 0;
	}
	
	if (Lua_IsNumber(L,1))
	{
		int nValue = (int)Lua_ValueToNumber(L, 1);
		Player[nPlayerIndex].SetLevel(nValue);
		Lua_PushNumber(L,1);
		
	}
	else
		Lua_PushNumber(L,0);

	return 1;
}


int LuaSetItemMagicEx(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	int nItemIdex = (int) Lua_ValueToNumber(L,1);

	if (nPlayerIndex <= 0) return 0;

    Item[nItemIdex].m_GeneratorParam.uRandomSeed = Lua_ValueToNumber(L,2);
	Item[nItemIdex].m_GeneratorParam.nGeneratorLevel[0] = Lua_ValueToNumber(L,3);
	Item[nItemIdex].m_GeneratorParam.nGeneratorLevel[1] = Lua_ValueToNumber(L,4);
	Item[nItemIdex].m_GeneratorParam.nGeneratorLevel[2] = Lua_ValueToNumber(L,5);
	Item[nItemIdex].m_GeneratorParam.nGeneratorLevel[3] = Lua_ValueToNumber(L,6);
	Item[nItemIdex].m_GeneratorParam.nGeneratorLevel[4] = Lua_ValueToNumber(L,7);
	Item[nItemIdex].m_GeneratorParam.nGeneratorLevel[5] = Lua_ValueToNumber(L,8);
	Item[nItemIdex].m_GeneratorParam.nVersion = Lua_ValueToNumber(L,9);
	Item[nItemIdex].m_GeneratorParam.nLuck = Lua_ValueToNumber(L,10);

	return 0;
}

int LuaAddSkillEffect(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	int nSkillId = (int) Lua_ValueToNumber(L,1);
	int nSkillLevel = (int) Lua_ValueToNumber(L,2);
	int nSkillTime = (int) Lua_ValueToNumber(L,3);

	if (nPlayerIndex <= 0) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC)
		return 0;

    Npc[Player[nPlayerIndex].m_nIndex].AddSkillEffect(nSkillId,nSkillLevel,nSkillTime);
	return 0;
}






int LuaGetLevelSkillEffectMan(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	int nSkillId = (int) Lua_ValueToNumber(L,1);

	if (nPlayerIndex <= 0) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC)
		return 0;

	Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].GetLevelSkillEffectMan(nSkillId));
    
	return 1;
}


int LuaDoSkillMan(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 1 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	int nSkillId = (int) Lua_ValueToNumber(L,1);
	int nSkillLevel = (int) Lua_ValueToNumber(L,2);

	if (nPlayerIndex <= 0) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC)
		return 0;

	Npc[Player[nPlayerIndex].m_nIndex].DoSkillMan(nSkillId,nSkillLevel);
    
	return 0;
}




int LuaGetNumberNameInCpuTK(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC)
		return 0;

	Lua_PushNumber(L, Player[nPlayerIndex].GetNumberNameInCpuTK());

    
	return 1;
}






int LuaBlockPKState(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	int nBlock = (int) Lua_ValueToNumber(L,1);

	if (nPlayerIndex <= 0) return 0;

    Player[nPlayerIndex].m_cPK.BlockPKState(nBlock);
	return 0;
}

int LuaSetPKState(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	int nValue = (int) Lua_ValueToNumber(L,1);

	if (nPlayerIndex <= 0 || nValue < 0 || nValue > 2) return 0;
 
    Player[nPlayerIndex].m_cPK.SetNormalPKStateDB(nValue);

	return 0;
}






int LuaGetPKState(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0) return 0;
 
	Lua_PushNumber(L, Player[nPlayerIndex].m_cPK.GetNormalPKState());

	return 1;
}







int LuaSetItemLock(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	int nItemIdex = (int) Lua_ValueToNumber(L,1);

	if (nPlayerIndex <= 0 || nItemIdex < 0) return 0;
 
    Item[nItemIdex].m_GeneratorParam.nVersion = 2;

	return 0;
}

int LuaSetBindItem(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	int nItemIdex = (int) Lua_ValueToNumber(L,1);
	int nValue = (int) Lua_ValueToNumber(L,2);
	if (nValue <= 0) return 0;
	if (nPlayerIndex <= 0 || nItemIdex < 0) return 0;
 
    Item[nItemIdex].SetBindItem(nValue);
	Player[nPlayerIndex].m_ItemList.UpdateCurItem(nItemIdex);
	return 0;
}
int LuaSetNpcDefense(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	int nDefense = (int)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentDefend = nDefense;
	if (nParamNum>2 && (int)Lua_ValueToNumber(L, 3)>0)
		Npc[nNpcIndex].m_Defend = nDefense;
	return 0;
}

int LuaSetCuongHoa(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	int nItemIdex = (int) Lua_ValueToNumber(L,1);
	int nValue = (int) Lua_ValueToNumber(L,2);
	if (nValue <= 0) return 0;
	if (nPlayerIndex <= 0 || nItemIdex < 0) return 0;
 
    Item[nItemIdex].SetNgoaiTrang(nValue);
	Player[nPlayerIndex].m_ItemList.UpdateCurItem(nItemIdex);
	return 0;
}

int LuaSetBindAllItem(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <=  0) return 0;

	Player[nPlayerIndex].m_ItemList.SetBindAllItem();
	
	return 0;
}

int LuaFixPlayer(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;
    Player[nPlayerIndex].FixPlayer();

	return 0;
}



int LuaCreadTong(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 3 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	int nCamp = (int) Lua_ValueToNumber(L,1);
	char szName[32];
	char szTitle[32];
	g_StrCpyLen(szName, Lua_ValueToString(L,2), 32);
	g_StrCpyLen(szTitle, Lua_ValueToString(L,3), 32);
    Player[nPlayerIndex].CreateTong(nCamp,szName,szTitle);

	return 0;
}

int LuaLoginTong(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 6 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	int nFigure = (int) Lua_ValueToNumber(L,1);
	int nCamp = (int) Lua_ValueToNumber(L,2);

	char szName[32];
	char szTitle[32];
	char szTongName[32];
	char szMaster[32];
	

	g_StrCpyLen(szName, Lua_ValueToString(L,3), 32);	
	g_StrCpyLen(szTitle, Lua_ValueToString(L,4), 32);
	g_StrCpyLen(szTongName, Lua_ValueToString(L,5), 32);
	g_StrCpyLen(szMaster, Lua_ValueToString(L,6), 32);

    Player[nPlayerIndex].LoginTong(nFigure,nCamp,szName,szTitle,szTongName,szMaster);

	return 0;
}


int LuaSendTongInfo(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 10 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	int nSlect = (int) Lua_ValueToNumber(L,1);
	int nPlayerSect = (int) Lua_ValueToNumber(L,2);


	char szTongName[32];
	char szListMember[4086];

	int nNumberDirector = Lua_ValueToNumber(L,3);
	int nNumberManager = Lua_ValueToNumber(L,4);
	int nNumberMember = Lua_ValueToNumber(L,5);
	g_StrCpyLen(szTongName, Lua_ValueToString(L,6), 32);
	int nCamp = (int) Lua_ValueToNumber(L,7);
	int nLevel = (int) Lua_ValueToNumber(L,8);
	int nMoney = (int) Lua_ValueToNumber(L,9);
	g_StrCpyLen(szListMember, Lua_ValueToString(L,10), 4086);
	
	Player[nPlayerIndex].SendTongInfo(nSlect,nPlayerSect,nNumberDirector,nNumberManager,nNumberMember,szTongName,nCamp,nLevel,nMoney,szListMember);
 

	return 0;
}

int LuaMemberAddTong(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 4 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	int nCamp = (int) Lua_ValueToNumber(L,1);
	


	char szTongName[32];
	char szTitle[32];
	char szMaster[32];

	g_StrCpyLen(szTitle, Lua_ValueToString(L,2), 32);
	g_StrCpyLen(szTongName, Lua_ValueToString(L,3), 32);
	g_StrCpyLen(szMaster, Lua_ValueToString(L,4), 32);


	Player[nPlayerIndex].MemberAddTong(nCamp,szTitle,szTongName,szMaster);
 
   
	return 0;
}



int LuaDeleteTong(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;


	if (nNumberPrama < 1) 
	{
	Player[nPlayerIndex].DeleteTong();
	}
	else
	{

	char szName[32];

	g_StrCpyLen(szName, Lua_ValueToString(L,1), 32);
	

	for (int i=0;i<MAX_PLAYER;i++)
	{

	if (Player[i].m_nIndex > 0 && Player[i].m_nIndex < MAX_NPC && !Player[i].IsExchangingServer())
	{

	if (!strcmp(Npc[Player[i].m_nIndex].Name, szName))
	{
	Player[i].DeleteTong();
	return 1;
	}

	}

	}


	}
   
	return 0;
}


int LuaUpdateTong(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;


	if (nNumberPrama < 1)
	{
     return 0;
	}
	else if (nNumberPrama < 2) 
	{

	int dwTongId = (int) Lua_ValueToNumber(L,1);

	Player[nPlayerIndex].UpdateTong(dwTongId);

	}
	else
	{

	int dwTongId = (int) Lua_ValueToNumber(L,1);

	char szName[32];

	g_StrCpyLen(szName, Lua_ValueToString(L,2), 32);
	

	for (int i=0;i<MAX_PLAYER;i++)
	{

	if (Player[i].m_nIndex > 0 && Player[i].m_nIndex < MAX_NPC && !Player[i].IsExchangingServer())
	{

	if (!strcmp(Npc[Player[i].m_nIndex].Name, szName))
	{
	Player[i].UpdateTong(dwTongId);
	return 1;
	}

	}

	}


	}
   
	return 0;
}




int LuaChangeTitleFeMaleTong(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;


	if (nNumberPrama < 1)
     return 0;


	int dwTongId = (int) Lua_ValueToNumber(L,1);
	

	for (int i=0;i<MAX_PLAYER;i++)
	{

	if (Player[i].m_nIndex > 0 && Player[i].m_nIndex < MAX_NPC && !Player[i].IsExchangingServer() && Player[i].m_cTong.GetTongNameID() == dwTongId && Player[i].m_cTong.GetFigure() == enumTONG_FIGURE_MEMBER)
	{

	if (Npc[Player[i].m_nIndex].m_nSex == 1)
	{
	Player[i].UpdateTong(dwTongId);
	}

	}

	}

   
	return 0;
}






int LuaChangeTitleMaleTong(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;


	if (nNumberPrama < 1)
     return 0;


	int dwTongId = (int) Lua_ValueToNumber(L,1);
	

	for (int i=0;i<MAX_PLAYER;i++)
	{

	if (Player[i].m_nIndex > 0 && Player[i].m_nIndex < MAX_NPC && !Player[i].IsExchangingServer() && Player[i].m_cTong.GetTongNameID() == dwTongId && Player[i].m_cTong.GetFigure() == enumTONG_FIGURE_MEMBER)
	{

	if (Npc[Player[i].m_nIndex].m_nSex == 0)
	{
	Player[i].UpdateTong(dwTongId);
	}

	}

	}

   
	return 0;
}






int LuaChangeTitleTong(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;


	if (nNumberPrama < 2)
     return 0;
	


	int dwTongId = (int) Lua_ValueToNumber(L,1);

	char szName[32];

	g_StrCpyLen(szName, Lua_ValueToString(L,2), 32);
	

	for (int i=0;i<MAX_PLAYER;i++)
	{

	if (Player[i].m_cTong.GetTongNameID() == dwTongId && Player[i].m_cTong.GetFigure() != enumTONG_FIGURE_MEMBER && Player[i].m_nIndex > 0 && Player[i].m_nIndex < MAX_NPC && !Player[i].IsExchangingServer())
	{

	if (!strcmp(Npc[Player[i].m_nIndex].Name, szName))
	{
	Player[i].UpdateTong(dwTongId);
	return 1;
	}

	}

	}


	
   
	return 0;
}



int LuaGetTongNameID(Lua_State * L)
{


	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;


    Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.GetTongNameID());
   
	return 1;
}


int LuaGetTongFigure(Lua_State * L)
{


	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;


    Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.GetFigure());
   
	return 1;
}




int LuaGetTongName(Lua_State * L)
{


	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	char szTongName[32];

    Player[nPlayerIndex].m_cTong.GetTongName(szTongName);

    Lua_PushString(L, szTongName);
   
	return 1;
}










int LuaLoadTongMap(Lua_State * L)
{

	int nNumberPrama = Lua_GetTopIndex(L);

	if (nNumberPrama < 7)
     return 0;
	

	int nIdMap = (int) Lua_ValueToNumber(L,1);
	int nMapTongId = (int) Lua_ValueToNumber(L,3);
	int nMapTongT  =  (int) Lua_ValueToNumber(L,4);
	int nMapTongVG  =  (int) Lua_ValueToNumber(L,5);
	int nMapTongBCId = (int) Lua_ValueToNumber(L,7);
	int nCheckMap = (int) Lua_ValueToNumber(L,8);
	
	char szMapTongName[32];
	char szMapTongNameBC[32];

	g_StrCpyLen(szMapTongName, (char*)Lua_ValueToString(L,2), 32);
	g_StrCpyLen(szMapTongNameBC, (char*)Lua_ValueToString(L,6), 32);

	int nIdxMap = g_SubWorldSet.SearchWorld(nIdMap);

	if (nIdxMap != -1)
	{

    SubWorld[nIdxMap].LoadTong(szMapTongName,nMapTongId,nMapTongT,nMapTongVG,szMapTongNameBC,nMapTongBCId,nCheckMap);
	}

	
   
	return 0;
}




int LuaGetTongMap(Lua_State * L)
{

	int nNumberPrama = Lua_GetTopIndex(L);

	if (nNumberPrama < 1)
     return 0;
	

	int nIdMap = (int) Lua_ValueToNumber(L,1);


	int nIdxMap = g_SubWorldSet.SearchWorld(nIdMap);

	if (nIdxMap != -1)
	{

	Lua_PushNumber(L, SubWorld[nIdxMap].m_bCheckTong);
	Lua_PushString(L, SubWorld[nIdxMap].m_szTongName);
	Lua_PushNumber(L, SubWorld[nIdxMap].m_dwTongName);
	Lua_PushString(L, SubWorld[nIdxMap].m_szTongNameBC);
	Lua_PushNumber(L, SubWorld[nIdxMap].m_dwTongNameBC);
	Lua_PushNumber(L, SubWorld[nIdxMap].m_nTongT);
	Lua_PushNumber(L, SubWorld[nIdxMap].m_nTongVG);

	return 7;
	}
   
	return 0;
}





int LuaSetTongMap(Lua_State * L)
{

	int nNumberPrama = Lua_GetTopIndex(L);

	if (nNumberPrama < 7)
     return 0;
	

	int nIdMap = (int) Lua_ValueToNumber(L,1);


	int nIdxMap = g_SubWorldSet.SearchWorld(nIdMap);

	if (nIdxMap != -1)
	{





	int nMapTongId = (int) Lua_ValueToNumber(L,3);

	int nMapTongT  =  (int) Lua_ValueToNumber(L,6);
	int nMapTongVG  =  (int) Lua_ValueToNumber(L,7);
	int nMapTongBCId = (int) Lua_ValueToNumber(L,5);
	
	char szMapTongName[32];
	char szMapTongNameBC[32];

	g_StrCpyLen(szMapTongName, Lua_ValueToString(L,2), 32);
	g_StrCpyLen(szMapTongNameBC, Lua_ValueToString(L,4), 32);







try
	{
	
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript("\\script\\admin\\banghoi\\banghoi.lua");;
		if (pScript)
		{

			

			int nTopIndex = 0;
			
			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction("SetTongMapMain",0, "dsdsddd",nIdMap,szMapTongName,nMapTongId,szMapTongNameBC,nMapTongBCId,nMapTongT,nMapTongVG));
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);

			
		}
		
		

	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", g_FileName2Id("\\script\\admin\\banghoi\\banghoi.lua"));
	}






	return 0;
	}
   
	return 0;
}






int LuaDropEvent(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (nNumberPrama < 1)
     return 0;
	
	int nNpcIndex = (int) Lua_ValueToNumber(L,1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC) return 0;

	Npc[nNpcIndex].DeathDropEvent(nPlayerIndex);
	
   
	return 0;
}



int LuaOpenStringBox(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID == 0) return 0;

	if (nNumberPrama < 3)
     return 0;

	g_StrCpyLen(Player[nPlayerIndex].m_szFunStringBox, Lua_ValueToString(L,3), sizeof(Player[nPlayerIndex].m_szFunStringBox));

	Player[nPlayerIndex].m_dwScriptStringBoxID = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;

	PLAYER_OPEN_STRING_BOX     IDC;

	IDC.ProtocolType = s2c_playeropenstringbox;
	IDC.m_ID = (int) Lua_ValueToNumber(L,1);
	g_StrCpyLen(IDC.m_Title, Lua_ValueToString(L,2), sizeof(IDC.m_Title));

    if (g_pServer)
		 g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(PLAYER_OPEN_STRING_BOX));

   
	return 0;
}




int LuaOpenCheckItemBox(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID == 0) return 0;

	if (nNumberPrama < 3)
     return 0;

	g_StrCpyLen(Player[nPlayerIndex].m_szFunCheckItemBox, Lua_ValueToString(L,3), sizeof(Player[nPlayerIndex].m_szFunCheckItemBox));

	Player[nPlayerIndex].m_dwScriptCheckItemBoxID = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;

	PLAYER_OPEN_CHECK_ITEM_BOX     IDC;

	IDC.ProtocolType = s2c_playeropencheckitembox;
	g_StrCpyLen(IDC.m_TitleBox, Lua_ValueToString(L,1), sizeof(IDC.m_TitleBox));
	g_StrCpyLen(IDC.m_TitleTex, Lua_ValueToString(L,2), sizeof(IDC.m_TitleTex));

    if (g_pServer)
		 g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(PLAYER_OPEN_CHECK_ITEM_BOX));

   
	return 0;
}




int LuaOpenSlectBoxCheck(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID == 0) return 0;

	if (nNumberPrama < 4)
     return 0;

	g_StrCpyLen(Player[nPlayerIndex].m_szFunSlectCheckBox, Lua_ValueToString(L,4), sizeof(Player[nPlayerIndex].m_szFunSlectCheckBox));

	Player[nPlayerIndex].m_dwScriptSlectCheckBoxID = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;


	
	Player[nPlayerIndex].m_btSlectCheckBox[0] = (BYTE) Lua_ValueToNumber(L,1);
	Player[nPlayerIndex].m_btSlectCheckBox[1] = (BYTE) Lua_ValueToNumber(L,2);
	Player[nPlayerIndex].m_btSlectCheckBox[2] = (BYTE) Lua_ValueToNumber(L,3);



	PLAYER_OPEN_SLECT_CHECK_BOX     IDC;

	IDC.ProtocolType = s2c_playeropenslectcheckbox;

    IDC.m_szId[0] = (char) Lua_ValueToNumber(L,1);
	IDC.m_szId[1] = (char) Lua_ValueToNumber(L,2);
	IDC.m_szId[2] = (char) Lua_ValueToNumber(L,3);

    if (g_pServer)
		 g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(PLAYER_OPEN_SLECT_CHECK_BOX));

   
	return 0;
}




int LuaOpenPGBoxItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;


	if (nNumberPrama < 0)
     return 0;




	PLAYER_OPEN_PG_BOX_ITEM     IDC;

	IDC.ProtocolType = s2c_playeropenpgboxitem;

    if (g_pServer)
		 g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(PLAYER_OPEN_PG_BOX_ITEM));

   
	return 0;
}



int LuaGetTimeNew(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 0) 
		return 0;

	int nValue3 = (((g_SubWorldSet.m_nLoopRate%(18*60*60))%(18*60))/18 + g_SubWorldSet.m_TimeNew_Giay)%60;
	int nValue2 = ((g_SubWorldSet.m_nLoopRate%(18*60*60))/(18*60) + g_SubWorldSet.m_TimeNew_Phut + (((g_SubWorldSet.m_nLoopRate%(18*60*60))%(18*60))/18 + g_SubWorldSet.m_TimeNew_Giay)/60)%60;
	int nValue1 = (g_SubWorldSet.m_nLoopRate/(18*60*60) + g_SubWorldSet.m_TimeNew_Gio + ((g_SubWorldSet.m_nLoopRate%(18*60*60))/(18*60) + g_SubWorldSet.m_TimeNew_Phut + (((g_SubWorldSet.m_nLoopRate%(18*60*60))%(18*60))/18 + g_SubWorldSet.m_TimeNew_Giay)/60)/60)%24;


	Lua_PushNumber(L, nValue1);
	Lua_PushNumber(L, nValue2);
	Lua_PushNumber(L, nValue3);

	

	return 3;
}






int LuaSetTimeNew(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 3) 
		return 0;
	
	g_SubWorldSet.m_TimeNew_Gio = (int)Lua_ValueToNumber(L, 1);
	g_SubWorldSet.m_TimeNew_Phut = (int)Lua_ValueToNumber(L, 2);
	g_SubWorldSet.m_TimeNew_Giay = (int)Lua_ValueToNumber(L, 3);
	

	return 0;
}





int LuaSleepGameSV(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1) 
		return 0;
	
	int nTimeSleep = (int)Lua_ValueToNumber(L, 1);
	::Sleep(nTimeSleep);

	

	return 0;
}









int LuaUpdateTopTKNew(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;


	if (nNumberPrama < 0)
     return 0;





	try
	{
	
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript("\\script\\hoatdong\\tongkim\\dieukhientk.lua");;
		if (pScript)
		{

			

			int nTopIndex = 0;
			
			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction("UpdateTop",0, "d",nPlayerIndex));
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);

			
		}
		
		

	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", g_FileName2Id("\\script\\hoatdong\\tongkim\\dieukhientk.lua"));
	}



   
	return 0;
}




int LuaDeleteTopTKNew(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;


	if (nNumberPrama < 0)
     return 0;



	try
	{
	
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript("\\script\\hoatdong\\tongkim\\dieukhientk.lua");;
		if (pScript)
		{

			

			int nTopIndex = 0;
			
			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction("DeleteTop",0, "d",nPlayerIndex));
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);

			
		}
		
		

	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", g_FileName2Id("\\script\\hoatdong\\tongkim\\dieukhientk.lua"));
	}

   
	return 0;
}






int LuaSendTopTKNew(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 94)
     return 0;


	

   	PLAYER_SEND_TOP_TK_NEW     IDC;

	IDC.ProtocolType = s2c_playersendtoptknew;




	IDC.m_DataTop.m_Phe = (BYTE)Lua_ValueToNumber(L, 1);
	IDC.m_DataTop.m_ThoiGian = (BYTE)Lua_ValueToNumber(L, 2);
	IDC.m_DataTop.m_NguoiChoi = (WORD)Lua_ValueToNumber(L, 3);
	IDC.m_DataTop.m_TNguoiChoi = (WORD)Lua_ValueToNumber(L, 4);
	IDC.m_DataTop.m_NPC = (WORD)Lua_ValueToNumber(L, 5);
	IDC.m_DataTop.m_TNPC = (WORD)Lua_ValueToNumber(L, 6);
	IDC.m_DataTop.m_LienTramHT = (WORD)Lua_ValueToNumber(L, 7);
	IDC.m_DataTop.m_TLienTramHT = (WORD)Lua_ValueToNumber(L, 8);
	IDC.m_DataTop.m_BauVat = (WORD)Lua_ValueToNumber(L, 9);
	IDC.m_DataTop.m_TBauVat = (WORD)Lua_ValueToNumber(L, 10);
	IDC.m_DataTop.m_TichLuy = (WORD)Lua_ValueToNumber(L, 11);
	IDC.m_DataTop.m_TuVong = (WORD)Lua_ValueToNumber(L, 12);
	IDC.m_DataTop.m_LienTram = (WORD)Lua_ValueToNumber(L, 13);


	g_StrCpyLen(IDC.m_DataTop.m_szTop1Name, Lua_ValueToString(L,14), sizeof(IDC.m_DataTop.m_szTop1Name));
	IDC.m_DataTop.m_Top1Phe = (BYTE)Lua_ValueToNumber(L, 15);
	IDC.m_DataTop.m_Top1TichLuy = (WORD)Lua_ValueToNumber(L, 16);
	IDC.m_DataTop.m_Top1NguoiChoi = (WORD)Lua_ValueToNumber(L, 17);
	IDC.m_DataTop.m_Top1NPC = (WORD)Lua_ValueToNumber(L, 18);
	IDC.m_DataTop.m_Top1TuVong = (WORD)Lua_ValueToNumber(L, 19);
	IDC.m_DataTop.m_Top1LienTram = (WORD)Lua_ValueToNumber(L, 20);
	IDC.m_DataTop.m_Top1BaoVat = (WORD)Lua_ValueToNumber(L, 21);




	g_StrCpyLen(IDC.m_DataTop.m_szTop2Name, Lua_ValueToString(L,22), sizeof(IDC.m_DataTop.m_szTop2Name));
	IDC.m_DataTop.m_Top2Phe = (BYTE)Lua_ValueToNumber(L, 23);
	IDC.m_DataTop.m_Top2TichLuy = (WORD)Lua_ValueToNumber(L, 24);
	IDC.m_DataTop.m_Top2NguoiChoi = (WORD)Lua_ValueToNumber(L, 25);
	IDC.m_DataTop.m_Top2NPC = (WORD)Lua_ValueToNumber(L, 26);
	IDC.m_DataTop.m_Top2TuVong = (WORD)Lua_ValueToNumber(L, 27);
	IDC.m_DataTop.m_Top2LienTram = (WORD)Lua_ValueToNumber(L, 28);
	IDC.m_DataTop.m_Top2BaoVat = (WORD)Lua_ValueToNumber(L, 29);


	g_StrCpyLen(IDC.m_DataTop.m_szTop3Name, Lua_ValueToString(L,30), sizeof(IDC.m_DataTop.m_szTop3Name));
	IDC.m_DataTop.m_Top3Phe = (BYTE)Lua_ValueToNumber(L, 31);
	IDC.m_DataTop.m_Top3TichLuy = (WORD)Lua_ValueToNumber(L, 32);
	IDC.m_DataTop.m_Top3NguoiChoi = (WORD)Lua_ValueToNumber(L, 33);
	IDC.m_DataTop.m_Top3NPC = (WORD)Lua_ValueToNumber(L, 34);
	IDC.m_DataTop.m_Top3TuVong = (WORD)Lua_ValueToNumber(L, 35);
	IDC.m_DataTop.m_Top3LienTram = (WORD)Lua_ValueToNumber(L, 36);
	IDC.m_DataTop.m_Top3BaoVat = (WORD)Lua_ValueToNumber(L, 37);


	g_StrCpyLen(IDC.m_DataTop.m_szTop4Name, Lua_ValueToString(L,38), sizeof(IDC.m_DataTop.m_szTop4Name));
	IDC.m_DataTop.m_Top4Phe = (BYTE)Lua_ValueToNumber(L, 39);
	IDC.m_DataTop.m_Top4TichLuy = (WORD)Lua_ValueToNumber(L, 40);
	IDC.m_DataTop.m_Top4NguoiChoi = (WORD)Lua_ValueToNumber(L, 41);
	IDC.m_DataTop.m_Top4NPC = (WORD)Lua_ValueToNumber(L, 42);
	IDC.m_DataTop.m_Top4TuVong = (WORD)Lua_ValueToNumber(L, 43);
	IDC.m_DataTop.m_Top4LienTram = (WORD)Lua_ValueToNumber(L, 44);
	IDC.m_DataTop.m_Top4BaoVat = (WORD)Lua_ValueToNumber(L, 45);


	g_StrCpyLen(IDC.m_DataTop.m_szTop5Name, Lua_ValueToString(L,46), sizeof(IDC.m_DataTop.m_szTop5Name));
	IDC.m_DataTop.m_Top5Phe = (BYTE)Lua_ValueToNumber(L, 47);
	IDC.m_DataTop.m_Top5TichLuy = (WORD)Lua_ValueToNumber(L, 48);
	IDC.m_DataTop.m_Top5NguoiChoi = (WORD)Lua_ValueToNumber(L, 49);
	IDC.m_DataTop.m_Top5NPC = (WORD)Lua_ValueToNumber(L, 50);
	IDC.m_DataTop.m_Top5TuVong = (WORD)Lua_ValueToNumber(L, 51);
	IDC.m_DataTop.m_Top5LienTram = (WORD)Lua_ValueToNumber(L, 52);
	IDC.m_DataTop.m_Top5BaoVat = (WORD)Lua_ValueToNumber(L, 53);


	g_StrCpyLen(IDC.m_DataTop.m_szTop6Name, Lua_ValueToString(L,54), sizeof(IDC.m_DataTop.m_szTop6Name));
	IDC.m_DataTop.m_Top6Phe = (BYTE)Lua_ValueToNumber(L, 55);
	IDC.m_DataTop.m_Top6TichLuy = (WORD)Lua_ValueToNumber(L, 56);
	IDC.m_DataTop.m_Top6NguoiChoi = (WORD)Lua_ValueToNumber(L, 57);
	IDC.m_DataTop.m_Top6NPC = (WORD)Lua_ValueToNumber(L, 58);
	IDC.m_DataTop.m_Top6TuVong = (WORD)Lua_ValueToNumber(L, 59);
	IDC.m_DataTop.m_Top6LienTram = (WORD)Lua_ValueToNumber(L, 60);
	IDC.m_DataTop.m_Top6BaoVat = (WORD)Lua_ValueToNumber(L, 61);


	g_StrCpyLen(IDC.m_DataTop.m_szTop7Name, Lua_ValueToString(L,62), sizeof(IDC.m_DataTop.m_szTop7Name));
	IDC.m_DataTop.m_Top7Phe = (BYTE)Lua_ValueToNumber(L, 63);
	IDC.m_DataTop.m_Top7TichLuy = (WORD)Lua_ValueToNumber(L, 64);
	IDC.m_DataTop.m_Top7NguoiChoi = (WORD)Lua_ValueToNumber(L, 65);
	IDC.m_DataTop.m_Top7NPC = (WORD)Lua_ValueToNumber(L, 66);
	IDC.m_DataTop.m_Top7TuVong = (WORD)Lua_ValueToNumber(L, 67);
	IDC.m_DataTop.m_Top7LienTram = (WORD)Lua_ValueToNumber(L, 68);
	IDC.m_DataTop.m_Top7BaoVat = (WORD)Lua_ValueToNumber(L, 69);


	g_StrCpyLen(IDC.m_DataTop.m_szTop8Name, Lua_ValueToString(L,70), sizeof(IDC.m_DataTop.m_szTop8Name));
	IDC.m_DataTop.m_Top8Phe = (BYTE)Lua_ValueToNumber(L, 71);
	IDC.m_DataTop.m_Top8TichLuy = (WORD)Lua_ValueToNumber(L, 72);
	IDC.m_DataTop.m_Top8NguoiChoi = (WORD)Lua_ValueToNumber(L, 73);
	IDC.m_DataTop.m_Top8NPC = (WORD)Lua_ValueToNumber(L, 74);
	IDC.m_DataTop.m_Top8TuVong = (WORD)Lua_ValueToNumber(L, 75);
	IDC.m_DataTop.m_Top8LienTram = (WORD)Lua_ValueToNumber(L, 76);
	IDC.m_DataTop.m_Top8BaoVat = (WORD)Lua_ValueToNumber(L, 77);


	g_StrCpyLen(IDC.m_DataTop.m_szTop9Name, Lua_ValueToString(L,78), sizeof(IDC.m_DataTop.m_szTop9Name));
	IDC.m_DataTop.m_Top9Phe = (BYTE)Lua_ValueToNumber(L, 79);
	IDC.m_DataTop.m_Top9TichLuy = (WORD)Lua_ValueToNumber(L, 80);
	IDC.m_DataTop.m_Top9NguoiChoi = (WORD)Lua_ValueToNumber(L, 81);
	IDC.m_DataTop.m_Top9NPC = (WORD)Lua_ValueToNumber(L, 82);
	IDC.m_DataTop.m_Top9TuVong = (WORD)Lua_ValueToNumber(L, 83);
	IDC.m_DataTop.m_Top9LienTram = (WORD)Lua_ValueToNumber(L, 84);
	IDC.m_DataTop.m_Top9BaoVat = (WORD)Lua_ValueToNumber(L, 85);


	g_StrCpyLen(IDC.m_DataTop.m_szTop10Name, Lua_ValueToString(L,86), sizeof(IDC.m_DataTop.m_szTop10Name));
	IDC.m_DataTop.m_Top10Phe = (BYTE)Lua_ValueToNumber(L, 87);
	IDC.m_DataTop.m_Top10TichLuy = (WORD)Lua_ValueToNumber(L, 88);
	IDC.m_DataTop.m_Top10NguoiChoi = (WORD)Lua_ValueToNumber(L, 89);
	IDC.m_DataTop.m_Top10NPC = (WORD)Lua_ValueToNumber(L, 90);
	IDC.m_DataTop.m_Top10TuVong = (WORD)Lua_ValueToNumber(L, 91);
	IDC.m_DataTop.m_Top10LienTram = (WORD)Lua_ValueToNumber(L, 92);
	IDC.m_DataTop.m_Top10BaoVat = (WORD)Lua_ValueToNumber(L, 93);

	IDC.m_DataTop.m_Soluong = (BYTE)Lua_ValueToNumber(L, 94);








    if (g_pServer)
		 g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(PLAYER_SEND_TOP_TK_NEW));

	return 0;
}







int LuaOpenNewPWBox(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;


	if (nNumberPrama < 0)
     return 0;




	PLAYER_OPEN_NEW_PW_BOX_ITEM     IDC;

	IDC.ProtocolType = s2c_playeropennewpwboxitem;

    if (g_pServer)
		 g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(PLAYER_OPEN_NEW_PW_BOX_ITEM));

   
	return 0;
}


int LuaCheckTrangSucInBox(Lua_State * L)
{
	
	if (Lua_GetTopIndex(L) >= 0)
	{
	
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			int nCount = Player[nPlayerIndex].m_ItemList.CheckTrangSucInBox();
			Lua_PushNumber(L, nCount);
		}
		else 
		{
			Lua_PushNumber(L,-1);
		}
	}

	else
	{
		Lua_PushNumber(L , -1);
	}
	
	return 1;
}

int LuaGetIdxItemBoxQuestKey(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	
	int nValueIndex = (int)Lua_ValueToNumber(L, 1);
	
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;


	int nItemIdx = Player[nPlayerIndex].GetIdxItemBoxQuestKey(nValueIndex);

	Lua_PushNumber(L, nItemIdx);
   
	return 1;
}


int LuaGetIdxItemBox(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 0)
     return 0;


	int nItemIdx = Player[nPlayerIndex].GetIdxItemBox();

	Lua_PushNumber(L, nItemIdx);
   
	return 1;
}


int LuaGetIdxItemBox2(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 0)
     return 0;


	int nItemIdx = Player[nPlayerIndex].GetIdxItemBox2();

	Lua_PushNumber(L, nItemIdx);
   
	return 1;
}


int LuaGetIdxItemBox3(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 0)
     return 0;


	int nItemIdx = Player[nPlayerIndex].GetIdxItemBox3();

	Lua_PushNumber(L, nItemIdx);
   
	return 1;
}
int LuaDelEquipItemQuestKey(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);
	int IdQuestKey = (int) Lua_ValueToNumber(L,1);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;


	int nItemIdx = Player[nPlayerIndex].DelEquipItemQuestKey(IdQuestKey);

	Lua_PushNumber(L, nItemIdx);
   
	return 1;
}


int LuaGetIdxItemBoxUpdateItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 0)
     return 0;


	int nItemIdx = Player[nPlayerIndex].GetIdxItemBoxUpdateItem();

	Lua_PushNumber(L, nItemIdx);
   
	return 1;
}

int LuaGetIdxItemBoxUpdateItem3(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 0)
     return 0;


	int nItemIdx = Player[nPlayerIndex].GetIdxItemBoxUpdateItem3();

	Lua_PushNumber(L, nItemIdx);
   
	return 1;
}
int LuaGetIdxItemBoxUpdateItem4(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 0)
     return 0;


	int nItemIdx = Player[nPlayerIndex].GetIdxItemBoxUpdateItem4();

	Lua_PushNumber(L, nItemIdx);
   
	return 1;
}

int LuaLayIdxHkmpTrongHomDo(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 0)
     return 0;


	int nItemIdx = Player[nPlayerIndex].GetIdxItemBoxUpdateItem2();

	Lua_PushNumber(L, nItemIdx);
   
	return 1;
}


int LuaGetIdxPGItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 2)
     return 0;

	int nPage = (int) Lua_ValueToNumber(L,1);

	int nLocation = (int) Lua_ValueToNumber(L,2);


	int nItemIdx = Player[nPlayerIndex].GetIdxPGBoxItem(nPage,nLocation);

	Lua_PushNumber(L, nItemIdx);
   
	return 1;
}









int LuaGetIdMagicEventItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].GetIdMagicEventItem(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}





int LuaGetLevelMagicEventItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].GetLevelMagicEventItem(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}




int LuaGetRequirementMagicEventItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].GetRequirementMagicEventItem(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}


int LuaGetItemType(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].GetItemType(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}



int LuaGetEquipMagicId(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 2)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nItemNum = (int) Lua_ValueToNumber(L,2);

	int nLevel = Player[nPlayerIndex].GetEquipMagicId(nItemIdx,nItemNum);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}




int LuaGetEquipMagicLevel(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 2)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nItemNum = (int) Lua_ValueToNumber(L,2);

	int nLevel = Player[nPlayerIndex].GetEquipMagicLevel(nItemIdx,nItemNum);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}

int LuaSetNpcDropScript(Lua_State *L)
{
	if (Lua_GetTopIndex(L) < 2 ) return 0;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC) return 0;
	Npc[nNpcIndex].m_DropScriptID = g_FileName2Id((char *)Lua_ValueToString(L,2));
	return 0;
}

int LuaSetNpcSkill(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 4)
		return 0;

	int nNpcIdx = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIdx <= 0 || nNpcIdx >= MAX_NPC)
		return 0;
	int nSkillID = (int)Lua_ValueToNumber(L, 2);
	if (nSkillID <= 0)
		return 0;
	int nSkillLevel = (int)Lua_ValueToNumber(L, 3);
	if (nSkillLevel < 1)
		nSkillLevel = 1;
	int nSkillPos = (int)Lua_ValueToNumber(L, 4);
	if (nSkillPos < 0 || nSkillPos > MAX_NPCSKILL)
		return 0;
	Npc[nNpcIdx].m_SkillList.SetNpcSkill(nSkillPos, nSkillID, nSkillLevel);
	return 1;
}



int LuaGetEquipMagicPoint(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 2)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nItemNum = (int) Lua_ValueToNumber(L,2);

	int nLevel = Player[nPlayerIndex].GetEquipMagicPoint(nItemIdx,nItemNum);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}






int LuaGetEquipMagicLucky(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 2)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nItemNum = (int) Lua_ValueToNumber(L,2);

	int nLevel = Player[nPlayerIndex].GetEquipMagicLucky(nItemIdx,nItemNum);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}

int LuaGetItemBoxLucky(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].GetItemBoxLucky(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}

int LuaCheckItemNguaPV(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 0)
     return 0;

	

	int nCheck = Player[nPlayerIndex].m_ItemList.CheckNguaPhienVu();

	Lua_PushNumber(L, nCheck);
   
	return 1;
}

int LuaCheckItemEquipCS(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 0)
     return 0;

	

	int nCheck = Player[nPlayerIndex].m_ItemList.CheckItemEquipCS();

	Lua_PushNumber(L, nCheck);
   
	return 1;
}



// int LuaDoMateSit(Lua_State * L)
// {

	// int nNumberPrama = Lua_GetTopIndex(L);

	// int nPlayerIndex = GetPlayerIndex(L);
	// if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	// if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	// if (nNumberPrama < 0)
     // return 0;
	// BOOL bSit = (BOOL) Lua_ValueToNumber(L,1);

	// PLAYER_SIT     IDC;
	// IDC.ProtocolType = s2c_playersit;
    // IDC.bSit = bSit;
    // if (g_pServer)
		 // g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(PLAYER_SIT));




   
	// return 1;
// }

int LuaAddPropObj(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nId = (int) Lua_ValueToNumber(L,1);

	Player[nPlayerIndex].AddPropObj(nId);

   
	return 0;
}



int LuaAddPropObjPos(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	if (nNumberPrama < 4)
     return 0;

	int nId = (int) Lua_ValueToNumber(L,1);
	int nMapId = (int) Lua_ValueToNumber(L,2);
	int nPosX = (int) Lua_ValueToNumber(L,3);
	int nPosY = (int) Lua_ValueToNumber(L,4);
	
	POINT	ptLocal;
	KMapPos	Pos;
	int nDataID = 0;
	int nColorID = 0;
    nDataID = nId;


	if (nDataID <= 0)
		return 0;

	ptLocal.x = nPosX;
	ptLocal.y = nPosY;


	int nSubWorldIndex = nMapId;

	if (nSubWorldIndex < 0 || nSubWorldIndex >= MAX_SUBWORLD)
		return 0;


	SubWorld[nSubWorldIndex].GetFreeObjPos(ptLocal);
	
	Pos.nSubWorld = nSubWorldIndex;
	SubWorld[nSubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);
	
	int nObjIdx = ObjSet.AddPropObj(Pos, nDataID, nColorID);
	if (nObjIdx > 0 && nObjIdx < MAX_OBJECT)
	{
			Object[nObjIdx].SetItemBelong(-1);
	}

   
	return 0;
}



int LuaAddTrap(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	if (nNumberPrama < 4)
     return 0;


	int nIdMap = (int) Lua_ValueToNumber(L,1);

	int nIdxMap = g_SubWorldSet.SearchWorld(nIdMap);

	if (nIdxMap != -1)
	{
    SubWorld[nIdxMap].AddTrap((int)Lua_ValueToNumber(L,2),(int)Lua_ValueToNumber(L,3),(char *)Lua_ValueToString(L,4));
	}

	return 0;
}




int LuaSetPosU(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	
	int nX = (int) Lua_ValueToNumber(L,1);
	int nY = (int) Lua_ValueToNumber(L,2);
	
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].SetPosU(nX * 32, nY * 32);
	}
	return 0;
}



int LuaGetEquipMagicRandomSeed(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].GetGetEquipMagicRandomSeed(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}



int LuaCheckMagicItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 4)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nMagicId = (int) Lua_ValueToNumber(L,2);

	int nMagicNumber1 = (int) Lua_ValueToNumber(L,3);

	int nMagicNumber2 = (int) Lua_ValueToNumber(L,4);

	int nReturn = Player[nPlayerIndex].CheckMagicItem(nItemIdx,nMagicId,nMagicNumber1,nMagicNumber2);

	Lua_PushNumber(L, nReturn);
   
	return 1;
}



int LuaGetLevelItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].GetLevelItem(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}



int LuaGetParticularItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].GetParticularItem(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}


int LuaGetGenreItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].GetGenreItem(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}




int LuaIsBlockItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].IsBlockItem(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}



int LuaGetDetailTypeItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].GetDetailTypeItem(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}

int LuaGetNameItemBox(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);
	
	Lua_PushString(L, Item[nItemIdx].GetName());
	
	return 1;
	
}

int LuaGetClassItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nLevel = Player[nPlayerIndex].GetClassItem(nItemIdx);

	Lua_PushNumber(L, nLevel);
   
	return 1;
}



int LuaGetSeriItem(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	int nItemIdx = (int) Lua_ValueToNumber(L,1);

	int nSeri = Player[nPlayerIndex].GetSeriItem(nItemIdx);

	Lua_PushNumber(L, nSeri);
   
	return 1;
}
int LuaSetNpcRevTime(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].SetReviveFrame((int)Lua_ValueToNumber(L, 2));
	return 0;
}
int LuaSetNpcSpeed(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentWalkSpeed = (int)Lua_ValueToNumber(L, 2);
	if (nParamNum>2 && (int)Lua_ValueToNumber(L, 3)>0)
		Npc[nNpcIndex].m_WalkSpeed = (int)Lua_ValueToNumber(L, 2);
	return 0;
}

int LuaSetNpcLifeReplenish(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	int nLifeRep = (int)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentLifeReplenish = nLifeRep;
	if (nParamNum>2 && (int)Lua_ValueToNumber(L, 3)>0)
		Npc[nNpcIndex].m_LifeReplenish = nLifeRep;
	return 0;
}


int LuaSaveData(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	     if (Player[nPlayerIndex].CanSave())
			{
				if (Player[nPlayerIndex].Save())
				{
					Player[nPlayerIndex].m_uMustSave = SAVE_REQUEST;
				}
	
			}

   
	return 0;
}
int LuaSetNpcAR(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	int nAttackR = (int)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentAttackRating = nAttackR;
	if (nParamNum>2 && (int)Lua_ValueToNumber(L, 3)>0)
		Npc[nNpcIndex].m_AttackRating = nAttackR;
	return 0;
}

int LuaSetNpcDamage(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].SetPhysicsDamage((int)Lua_ValueToNumber(L, 2), (int)Lua_ValueToNumber(L, 3));
	return 0;
}
int LuaSetNpcResist(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 6)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentPhysicsResistMax = (int)Lua_ValueToNumber(L, 2);
	Npc[nNpcIndex].m_CurrentPoisonResistMax = (int)Lua_ValueToNumber(L, 3);
	Npc[nNpcIndex].m_CurrentLightResistMax = (int)Lua_ValueToNumber(L, 4);
	Npc[nNpcIndex].m_CurrentFireResistMax = (int)Lua_ValueToNumber(L, 5);
	Npc[nNpcIndex].m_CurrentColdResistMax = (int)Lua_ValueToNumber(L, 6);

	Npc[nNpcIndex].m_CurrentPhysicsResist = (int)Lua_ValueToNumber(L, 2);
	Npc[nNpcIndex].m_CurrentPoisonResist = (int)Lua_ValueToNumber(L, 3);
	Npc[nNpcIndex].m_CurrentLightResist = (int)Lua_ValueToNumber(L, 4);
	Npc[nNpcIndex].m_CurrentFireResist = (int)Lua_ValueToNumber(L, 5);
	Npc[nNpcIndex].m_CurrentColdResist = (int)Lua_ValueToNumber(L, 6);

	if ((nParamNum > 6) && (int)Lua_ValueToNumber(L, 7))
	{
		Npc[nNpcIndex].m_PhysicsResist = (int)Lua_ValueToNumber(L, 2);
		Npc[nNpcIndex].m_PoisonResist = (int)Lua_ValueToNumber(L, 3);
		Npc[nNpcIndex].m_LightResist = (int)Lua_ValueToNumber(L, 4);
		Npc[nNpcIndex].m_FireResist = (int)Lua_ValueToNumber(L, 5);
		Npc[nNpcIndex].m_ColdResist = (int)Lua_ValueToNumber(L, 6);

		Npc[nNpcIndex].m_PhysicsResistMax = (int)Lua_ValueToNumber(L, 2);
		Npc[nNpcIndex].m_PoisonResistMax = (int)Lua_ValueToNumber(L, 3);
		Npc[nNpcIndex].m_LightResistMax = (int)Lua_ValueToNumber(L, 4);
		Npc[nNpcIndex].m_FireResistMax = (int)Lua_ValueToNumber(L, 5);
		Npc[nNpcIndex].m_ColdResistMax = (int)Lua_ValueToNumber(L, 6);
	}
	return 0;
}
int LuaSetNpcDmgEx(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 6)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	if ((nParamNum > 6) && (int)Lua_ValueToNumber(L, 7))
	{
		Npc[nNpcIndex].m_CurrentAddPhysicsDamage = (int)Lua_ValueToNumber(L, 2);
		Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[0] = (int)Lua_ValueToNumber(L, 3);
		if (Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[0] > 0)
		{
			Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[1] = POISON_DAMAGE_TIME;//pMagic->nValue[1];
			Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[2] = POISON_DAMAGE_INTERVAL;//pMagic->nValue[2];
		}
		else
		{
			Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[0] = 0;
			Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[1] = 0;
			Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[2] = 0;
		}
		Npc[nNpcIndex].m_CurrentColdDamage.nValue[0] = (int)Lua_ValueToNumber(L, 4);
		Npc[nNpcIndex].m_CurrentColdDamage.nValue[2] = (int)Lua_ValueToNumber(L, 4);
		if (Npc[nNpcIndex].m_CurrentColdDamage.nValue[0] > 0 && Npc[nNpcIndex].m_CurrentColdMagic.nValue[2] > 0)
			Npc[nNpcIndex].m_CurrentColdDamage.nValue[1] = COLD_DAMAGE_TIME;
		else
		{
			Npc[nNpcIndex].m_CurrentColdDamage.nValue[0] = 0;
			Npc[nNpcIndex].m_CurrentColdDamage.nValue[2] = 0;
			Npc[nNpcIndex].m_CurrentColdDamage.nValue[1] = 0;
		}
		Npc[nNpcIndex].m_CurrentFireDamage.nValue[0] = (int)Lua_ValueToNumber(L, 5);
		Npc[nNpcIndex].m_CurrentFireDamage.nValue[2] = (int)Lua_ValueToNumber(L, 5);
		Npc[nNpcIndex].m_CurrentLightDamage.nValue[0] = (int)Lua_ValueToNumber(L, 6);
		Npc[nNpcIndex].m_CurrentLightDamage.nValue[2] = (int)Lua_ValueToNumber(L, 6);
	}
	else
	{
		Npc[nNpcIndex].m_CurrentAddPhysicsMagic = (int)Lua_ValueToNumber(L, 2);
		Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[0] = (int)Lua_ValueToNumber(L, 3);
		if (Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[0] > 0)
		{
			Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[1] = POISON_DAMAGE_TIME;//pMagic->nValue[1];
			Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[2] = POISON_DAMAGE_INTERVAL;//pMagic->nValue[2];
		}
		else
		{
			Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[0] = 0;
			Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[1] = 0;
			Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[2] = 0;
		}
		Npc[nNpcIndex].m_CurrentColdMagic.nValue[0] = (int)Lua_ValueToNumber(L, 4);
		Npc[nNpcIndex].m_CurrentColdMagic.nValue[2] = (int)Lua_ValueToNumber(L, 4);
		if (Npc[nNpcIndex].m_CurrentColdMagic.nValue[0] > 0 && Npc[nNpcIndex].m_CurrentColdMagic.nValue[2] > 0)
			Npc[nNpcIndex].m_CurrentColdMagic.nValue[1] = COLD_DAMAGE_TIME;
		else
		{
			Npc[nNpcIndex].m_CurrentColdMagic.nValue[0] = 0;
			Npc[nNpcIndex].m_CurrentColdMagic.nValue[2] = 0;
			Npc[nNpcIndex].m_CurrentColdMagic.nValue[1] = 0;
		}
		Npc[nNpcIndex].m_CurrentFireMagic.nValue[0] = (int)Lua_ValueToNumber(L, 5);
		Npc[nNpcIndex].m_CurrentFireMagic.nValue[2] = (int)Lua_ValueToNumber(L, 5);
		Npc[nNpcIndex].m_CurrentLightMagic.nValue[0] = (int)Lua_ValueToNumber(L, 6);
		Npc[nNpcIndex].m_CurrentLightMagic.nValue[2] = (int)Lua_ValueToNumber(L, 6);
	}
	return 0;
}

int LuaBlockBox(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 1)
     return 0;

	Player[nPlayerIndex].m_bBlock = (int) Lua_ValueToNumber(L,1);


	PLAYER_BLOCK_BOX     IDC;

	IDC.ProtocolType = s2c_playerblockbox;

    IDC.m_bBlock = Player[nPlayerIndex].m_bBlock;


    if (g_pServer)
		 g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(PLAYER_BLOCK_BOX));

   
	return 0;
}





#endif

int LuaPlayMusic(Lua_State * L)//PlayMusic(musicname,loop=1, vol );
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_PLAYMUSIC;
	UiInfo.m_bOptionNum = 1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	int nMsgId = 0;
	
	g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), sizeof(UiInfo.m_pContent));
	UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
	UiInfo.m_bParam1 = 0;
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

int LuaFadeInMusic(Lua_State * L)
{
	return 0;
}

int LuaFadeOutMusic(Lua_State * L)
{
	return 0;
}


#ifndef _SERVER




int LuaPlaySound(Lua_State * L)
{
	
	return 0;
}

int LuaPlaySprMovie(Lua_State * L)
{
	return 0;
}
#endif
TLua_Funcs GameScriptFuns[] = 
{
	//通用指令
	
	{"Say", LuaSelectUI},
    {"SayNew",			LuaSayNew},
	{"SayImg",			LuaSelectImage},
	{"Talk", LuaTalkUI},
	{"GetTaskTemp", LuaGetTempTaskValue},
	{"SetTaskTemp", LuaSetTempTaskValue},
	{"Message", LuaMessage},
	{"Print", LuaMessage},
	
	{"GetBit",	LuaGetBit},
	{"GetByte",	LuaGetByte},
	{"SetBit",	LuaSetBit},
	{"SetByte",	LuaSetByte},
	{"Include",LuaIncludeFile},
	{"PutMessage", LuaSendMessageInfo},
	{"AddGlobalNews",LuaAddGlobalNews},
	{"AddGlobalTimeNews",LuaAddGlobalTimeNews},
	{"AddGlobalCountNews",LuaAddGlobalCountNews	},
	{"AddLocalNews",LuaAddLocalNews},
	{"AddLocalTimeNews",LuaAddLocalTimeNews},
	{"AddLocalCountNews",LuaAddLocalCountNews	},

	{"AddGlobalNewsEx",	LuaAddGlobalNewsEx},
	{"AddGlobalTimeNewsEx",LuaAddGlobalTimeNews},
	{"AddGlobalCountNewsEx",LuaAddGlobalCountNewsEx	},
	{"AddLocalNewsEx",	LuaAddLocalNewsEx},
	{"AddLocalTimeNewsEx",LuaAddLocalTimeNewsEx},
	{"AddLocalCountNewsEx",LuaAddLocalCountNewsEx	},


	
	//服务器端脚本指令
#ifdef _SERVER
	//-----------------基本功能
	{"AddRepute", LuaModifyRepute},	//修改声望值
	{"GetRepute",LuaGetRepute},		
	{"GetNpcIdx", LuaGetCurNpcIndex},
	{"SetNpcKind",		LuaSetNpcKind},
	{"SetNpcSkill",		LuaSetNpcSkill},
	
	{"SetTimer",		LuaSetTimer},		//SetTimer(时间量, 时间TaskId):给玩家打开计时器,时间到时将自动调用OnTimer函数
	{"StopTimer",		LuaStopTimer},		//StopTimer()：关闭当前玩家的计时器
	{"GetRestTime",		LuaGetRestTime},	//GetRestTime:获得计时器将触发的剩于时间	
	{"GetTimerId",		LuaGetCurTimerId},	//CurTimerId = GetTimerId():获得当前执行的计时器的id,如果没有则返回0
	{"GetTask",			LuaGetTaskValue},	//GetTask(任务号):获得当前玩家该任务号的值
	{"SetTask",			LuaSetTaskValue},	//SetTask(任务号,值):设置任务值
	{"RANDOM",				LuaRANDOM},
	{"RandomNew",			LuaRandomNew},	//SetTask(任务号,值):设置任务值
    {"RANDOMC",				LuaRANDOMC},
	{"IsCaptain",		LuaIsLeader},		//IsCaptain()是否为队长
	{"GetTeam",			LuaGetTeamId},		//GetTeam()返回玩家队伍ID
	{"GetTeamSize",		LuaGetTeamSize},	//GetTeamSize()
//	{"GetTeamName",		LuaGetTeamName},	//GetTeamSize()
	{"LeaveTeam",		LuaLeaveTeam},		//LeaveTeam()让玩家离开自身队伍
	{"Msg2Player",		LuaMsgToPlayer	},	//Msg2Player(消息)
	{"Msg2Team",		LuaMsgToTeam},		//Msg2Team(消息)通知玩家的组
	{"Msg2SubWorld",	LuaMsgToSubWorld},	//Msg2SubWorld(消息)通知世界
	{"Msg2Region",		LuaMsgToAroundRegion},//Msg2Region(消息)通知周围Region
	{"Msg2GM",			LuaMsgToGameMaster}, //Msg2GM(StrInfo)
	{"Msg2IP",			LuaMsgToIP}, //Msg2IP(IP, ID, StrInfo)
	{"GetInfo",			LuaGetPlayerInfo}, //str = GetInfo()
	{"GetIP",			LuaGetIP},
	{"SetPos",			LuaSetPos},			//SetPos(x,y)进入某点	
	{"GetPos",			LuaGetPos},			//GetPos() return x,y,subworldindex
	{"GetWorldPos",		LuaGetNewWorldPos},	//W,X,Y = GetWorldPos()返回于NewWorld配陪的坐标
	{"GetWorldPosIdx",		LuaGetNewWorldPosIdx},	//W,X,Y = GetWorldPos()返回于NewWorld配陪的坐标
	{"GetFromToward",		LuaGetFromToward},	//W,X,Y = GetWorldPos()返回于NewWorld配陪的坐标
	{"NewWorld",		LuaEnterNewWorld},
	{"DropItem",		LuaDropItem},		//DropItem(NpcId, 物品id或物品名)
	{"AddItem",			LuaAddItem},		//AddItem(nItemClass, nDetailType, nParticualrType, nLevel, nSeries, nLuck, nItemLevel..6)
	{"AddEventItem",	LuaAddEventItem	},	//AddEventItem(事件物品ID)
	{"DelItem",			LuaDelItem},
	{"DelItemPoison",			LuaDelItemPoison},
	{"DelItemBox",			LuaDelItemBox},
	{"SetNpcReplenish",	LuaSetNpcLifeReplenish},
	{"SetNpcAR",		LuaSetNpcAR},
	{"SetNpcDamage",	LuaSetNpcDamage},
	{"SetNpcResist",	LuaSetNpcResist},
	{"SetNpcDmgEx",		LuaSetNpcDmgEx},
	

	{"AutoRepairAllItemEquip",			LuaAutoRepairAllItemEquip},		//AddItem(nItemClass, nDetailType, nParticualrType, nLevel, nSeries, nLuck, nItemLevel..6)
	

	//
	{"HaveItem",		LuaHaveItem},
	{"GetItemCount",	LuaGetTaskItemCount}, //GetItemCount(TaskItemName Or TaskItemId)
	{"GetItemBindAll",	LuaGetItemBindAll}, //GetItemCount(TaskItemName Or TaskItemId)
	{"GetItemCount2",	LuaGetTaskItemCount2}, //GetItemCount(TaskItemName Or TaskItemId)
	{"GetItemCountBox",	LuaGetTaskItemCountBox}, //GetItemCount(TaskItemName Or TaskItemId)
	
	{"GetTimeItem",	LuaGetTimeItem}, //GetItemCount(TaskItemName Or TaskItemId)
	{"GetTimeItemAndGold",	LuaGetTimeItemAndGold}, //GetItemCount(TaskItemName Or TaskItemId)
	

	{"AddMagic",		LuaAddMagic},		//AddMagic(魔法id或魔法名)给玩家加某个魔法
	{"DelMagic",		LuaDelMagic},		//DelMagic(魔法id或魔法名)
	{"HaveMagic",		LuaHaveMagic},		//HaveMagic(魔法id或魔法名)返回0或1
	{"GetMagicLevel",	LuaGetMagicLevel},	//GetMagicLevel(魔法id或魔法名)返回等级
	{"AddMagicPoint",	LuaAddMagicPoint},
	{"GetMagicPoint",	LuaGetMagicPoint},
	
	{"SubWorldID2Idx",	LuaSubWorldIDToIndex}, //SubWorldID2Idx
	
	
	{"AddLeadExp",		LuaAddLeadExp},
	{"GetLeadLevel",	LuaGetLeadLevel},
	
	{"SetFightState",	LuaSetFightState}, 
	{"GetFightState",	LuaGetFightState},

	{"SetNpcDropScript",LuaSetNpcDropScript},
	
	{"AddNpc",			LuaAddNpc},			//AddNpc(人物模板id或人物模板名,所处世界id，点坐标x,点坐标y),返回npcid值
	{"DelNpc",			LuaDelNpc},			//DelNpc(Npcid)
	{"GetNpcValue",		LuaGetNpcParam},
	{"SetNpcScript",	LuaSetNpcActionScript},	//SetNpcScript(npcid, 脚本文件名)设置npc当前的脚本
	{"SetRevPos",		LuaSetPlayerRevivalPos},//SetRevPos(点位置X，点位置Y)设置玩家的当前世界的等入点位置
	{"GetRevPos",		LuaGetPlayerRevivalPos},
	{"SetTempRevPos",	LuaSetDeathRevivalPos}, //SetTempRevPos(subworldid, x, y ) or SetTempRevPos(id);
	{"GetCurCamp",		LuaGetPlayerCurrentCamp},//GetCurCamp()获得玩家的当前阵营
	{"GetCamp",			LuaGetPlayerCamp	},//GetCamp()获得玩家阵营
	{"SetCurCamp",		LuaSetPlayerCurrentCamp},//SetCurCamp(阵营号):设置玩家当前阵营
	{"SetTeamCamp",		LuaSetTeamCamp},//SetCurCamp(阵营号):设置玩家当前阵营
	{"SetCamp",			LuaSetPlayerCamp},		  //SetCamp(阵营号):设置阵营	
	{"RestoreCamp",		LuaRestorePlayerCamp	},//RestoreCamp()恢复阵营
	{"GetFaction",		LuaGetPlayerFaction,	},//GetFaction()获得玩家的门派名 
	{"SetFaction",		LuaChangePlayerFaction},  //SetFaction(门派名):设置玩家门派名
	{"GetColdR",		LuaGetPlayerColdResist},	
	{"SetNpcDefense",	LuaSetNpcDefense},
	{"SetColdR",		LuaGetPlayerColdResist},
	{"GetFireR",		LuaGetPlayerFireResist	},
	{"SetFireR",		LuaSetPlayerFireResist	},
	{"GetLightR",		LuaGetPlayerLightResist	},
	{"SetLightR",		LuaSetPlayerLightResist},
	{"GetPoisonR",		LuaGetPlayerPoisonResist},
	{"SetNpcRevTime",	LuaSetNpcRevTime},
	{"SetNpcSpeed",		LuaSetNpcSpeed},
	{"Revive",			LuaPlayerExecuteRevive}, //SetTempRevPos(subworldid, x, y ) or SetTempRevPos(id);




	{"SetPoisonR",		LuaSetPlayerPoisonResist},
	{"GetPhyR",			LuaGetPlayerPhysicsResist	},
	{"SetPhyR",			LuaSetPlayerPhysicsResist	},
	{"GetExp",			LuaGetPlayerExp	},			//GetExp():获得玩家的当前经验值
	{"AddExp",			LuaModifyPlayerExp},		//AddExp(经验值，对方等级，是否组队共享经验值)
	{"AddOwnExp",		LuaAddOwnExp	},			//AddOwnExp(Exp)，给玩家直接加经验
	{"SetExp",			LuaSetExp	},			//AddOwnExp(Exp)，给玩家直接加经验
	{"GetLife",			LuaGetPlayerLife},			//GetLife()获得玩家的生命值
	{"RestoreLife",		LuaRestorePlayerLife},		//RestoreLife()恢复玩家的生命
	{"GetMana",			LuaGetPlayerMana},			//GetMana()获得玩家的Mana
	{"RestoreMana",		LuaRestorePlayerMana},		//RestoreMana()恢复玩家的Mana
	{"GetStamina",		LuaGetPlayerStamina},		//GetStamina()获得玩家Stamina
	{"RestoreStamina",	LuaRestorePlayerStamina},	//RestoreMana()恢复玩家的Stamina
	{"GetDefend",		LuaGetPlayerDefend},		//GetDefend()获得玩家的防御力
	{"GetSex",			LuaGetPlayerSex},			//GetSex()获得玩家的性别
	{"GetSeries",		LuaGetPlayerSeries},		//GetSeries()获得玩家的系0man/1woman
	{"SetSeries",		LuaSetPlayerSeries},		//SetSeries(性别号)
	{"SetSex",          LuaSetPlayerSex},
	
	{"GetName",			LuaGetPlayerName},			//GetName()获得玩家的姓名
	{"GetNamePlayer",			LuaGetNpcPlayerName},			//GetName()获得玩家的姓名
	
	{"GetUUID",			LuaGetPlayerID},			//GetUUID()获得玩家的唯一ID
	{"GetLeadExp",		LuaGetPlayerLeadExp},		//GetLeadExp()获得玩家的统率经验值
	{"GetLeadLevel",	LuaGetPlayerLeadLevel},		//GetLeadLevel()获得玩家的统率等级
	{"GetLevel",		LuaGetLevel},				//GetLevel()GetPlayers Level
	{"GetRestAP",		LuaGetPlayerRestAttributePoint},//GetRestAP()获得玩家的剩于属性点数
	{"GetRestSP",		LuaGetPlayerRestSkillPoint},	//GetRestSP()获得玩家的剩于技能点数	
	{"GetLucky",		LuaGetPlayerLucky},			//GetLucky()获得玩家的幸运值
	{"GetEng",			LuaGetPlayerEngergy},		//GetEng()获得玩家的力量值Eng
	{"GetDex",			LuaGetPlayerDexterity},		//GetDex()获得玩家的Dex
	{"GetStrg",			LuaGetPlayerStrength},		//GetStrg()
	{"GetVit",			LuaGetPlayerVitality},		//GetVit()
//	{"GetDame",			LuaGetPlayerDame},		//GetVit()
	{"GetDef",			LuaGetPlayerDef},		//GetVit()
	{"GetReduce",			LuaGetPlayerReduce},		//GetVit()
	{"GetSpeed",			LuaGetPlayerSpeed},		//GetVit()
	{"SetRefesh",			LuaSetRefesh},		//GetVit()
	
	
	{"GetCash",			LuaGetPlayerCashMoney},		//GetCash()获得玩家的现金
	{"Pay",				LuaPlayerPayMoney},			//Pay(金额数)扣除玩家金钱成功返回1，失败返回0
	{"Earn",			LuaPlayerEarnMoney},		//Earn(金额数)增加玩家金钱
	{"PrePay",			LuaPlayerPrePayMoney},		//付定金，成功返回1，失败返回0
	{"ExeScript",		LuaPlayerExecuteScript},	//ExeScript(脚本文件名,参数),执行时会调用main函数	
	{"AttackNpc",		LuaAttackNpc},				//AttackNpc(NpcDwid,物理伤害值，冰，火，电，毒）
	{"KillNpc",			LuaKillNpc},				//KillNpc(NpcDWID)
	{"KillPlayer",		LuaKillPlayer},				//KillPlayer();
	{"Sale",			LuaSale},					//Sale(SaleId)买卖，SaleId为便卖的物品信息列表id
	{"UseTownPortal",	LuaUseTownPortal	},
	{"ReturnFromPortal",LuaReturnFromTownPortal	},
	{"SetNpcCurCamp",	LuaSetNpcCurCamp},
	{"OpenBox",			LuaOpenBox},
	{"AddStation",	LuaAddPlayerStation},
	{"AddTermini",		LuaAddPlayerWayPoint},
	{"GetStation",		LuaGetPlayerStation	},
	{"GetStationCount", LuaGetPlayerStationCount},
	{"SetNpcBoss",		LuaSetNpcBoss},
	
	{"GetCityCount", LuaGetAllStationCount},
	{"GetCity", LuaGetCity},
	{"SetNpcHitRecover",LuaSetNpcHitRecover},
	
	{"GetWayPoint",		LuaGetPlayerWayPoint},
	{"GetStationName",	LuaGetStationName},
	{"GetWayPointName", LuaGetWayPointName},
	{"GetPrice2Station", LuaGetPriceToStation},
	{"GetPrice2WayPoint", LuaGetPriceToWayPoint	},
	{"GetStationPos",	LuaGetStationPos},
	{"GetWayPointPos",	LuaGetWayPointPos},
	{"GetPlayerCount",	LuaGetPlayerCount},
	{"GetNpcCount",	LuaGetNpcCount},
	{"GetRank",			LuaGetRank},//GetRank()
	{"SetRank",			LuaSetRank},//SetRank(id)
	{"SetPropState",	LuaSetObjPropState},//SetPropState( hide = 1) hide obj
	{"GetServerName",	LuaGetServerName},
	
	
	//------------------Station Script ---------------
	{"GetWharfName",	LuaGetDockName},
	{"GetWharfCount",	LuaGetDockCount},
	{"GetWharfPrice",	LuaGetDockPrice},
	{"GetWharf",		LuaGetDock},
	{"GetWharfPos",		LuaGetDockPos},
	{"GetTerminiFState", LuaGetWayPointFightState},
	//------------------------------------------------
	{"KickOutPlayer",	LuaKickOutPlayer},
	{"KickOutSelf",		LuaKickOutSelf},
	{"GetSkillId",		LuaGetSkillIdInSkillList},
	{"SetSkillLevel",	LuaSetSkillLevel},
	{"SetChatFlag",		LuaSetPlayerChatForbiddenFlag},
    {"SetNpcValue",		LuaSetNpcValue},
    {"UyThac6",			LuaUyThac6},
	{"OfflineLive5",	LuaOfflineLive5},
	{"UythacOff",	LuaOfflineLive50},
    {"OfflineLiveAll",	LuaOfflineLiveAll},
	//------------------------------------------------
	
	{"AddNote", LuaAddNote},
	//-----------------Mission Script-----------------
	{"GetMissionV", LuaGetMissionValue},//GetMissionV(Vid)
	{"SetMissionV", LuaSetMissionValue},//SetMissionV(Vid, Value)
	{"GetGlbMissionV", LuaGetGlobalMissionValue	},
	{"SetGlbMissionV", LuaSetGlobalMissionValue	},
	{"OpenMission", LuaInitMission},//OpenMission(missionid)
	{"RunMission", LuaRunMission},
	{"CloseMission", LuaCloseMission},//CloseMission(missionid)
	{"StartMissionTimer", LuaStartMissionTimer},////StartMissionTimer(missionid, timerid, time)
	{"StopMissionTimer", LuaStopMissionTimer},
	{"GetMSRestTime", LuaGetMissionRestTime}, //GetMSRestTime(missionid, timerid)
	{"GetMSIdxGroup",LuaGetPlayerMissionGroup},//GetPlayerGroup(missionid, playerid);
	
	{"AddMSPlayer", LuaAddMissionPlayer},
	{"DelMSPlayer", LuaRemoveMissionPlayer},
	{"GetNextPlayer", LuaGetNextPlayer},
	{"PIdx2MSDIdx", LuaGetMissionPlayer_DataIndex},//(missionid, pidx)
	{"MSDIdx2PIdx", LuaGetMissionPlayer_PlayerIndex},//(missionid, dataidx)
	{"NpcIdx2PIdx", LuaNpcIndexToPlayerIndex},
	{"GetMSPlayerCount", LuaMissionPlayerCount},//GetMSPlayerCount(missionid, group = 0)

	{"RevivalAllNpc",	LuaRevivalAllNpc},
	
	{"SetPMParam", LuaSetMissionPlayerParam },
	{"GetPMParam", LuaGetMissionPlayerParam},
	{"Msg2MSGroup", LuaMissionMsg2Group},
	{"Msg2MSAll", LuaMissionMsg2All},
	{"Msg2MSPlayer", LuaMissionMsg2Player},
	{"SetDeathScript", LuaSetPlayerDeathScript},
	{"Death", LuaKillNpc},

	{"HideNpc", LuaHideNpc}	,//HideNpc(npcindex/npcname, hidetime)
	{"SetLogoutRV", LuaSetPlayerRevivalOptionWhenLogout},
	{"SetCreateTeam",LuaSetCreateTeamOption},
	{"GetPK", LuaGetPlayerPKValue},  //pkValue = GetPK() 
	{"SetPK", LuaSetPlayerPKValue}, //SetPK(pkValue)
	//------------------------------------------------
	//排名相关函数
	{"ShowLadder", LuaShowLadder}, //ShowLadder(LadderCount, LadderId1,LadderId2,...);
	//------------------------------------------------

	{"OpenTong",	LuaOpenTong},	//OpenTong()通知玩家打开帮会界面	
	{"SetPunish",	LuaSetDeathPunish},// SetPunish(0/1) 0表示不受任何惩罚
	//-------------------------------------------------
	//结拜
	{ "SwearBrother", LuaSwearBrother}, // ret = SwearBrother(TeamId);
	{"MakeEnemy",	LuaMakeEnemy}, //结仇 MakeEnemy(enemyname)
	{"RollbackSkill", LuaRollBackSkills},
	{"UpdateSkill", LuaUpdateSkillList},
	//-------------------------------------------------

	{"AddProp",		LuaAddPropPoint},//加玩家属性点
	{"GetProp",		LuaGetRestPropPoint },
	
	{"GetExtPoint",	LuaGetExtPoint},
	{"PayExtPoint",	LuaPayExtPoint},
	{"SetExtPoint",	LuaSetExtPoint},
	//UYTHAC
//	{"IsLiXian",LuaIsLiXian},
//	{"NotLiXian",LuaNotLiXian},

	//END
	// MASK
	{"SetMask",				LuaSetMask},
	{"ReSetMask",			LuaReSetMask},	
	// END
	
	{"SetFiFong",			LuaSetFiFong},	//SetFiFongGold(nIdx, ID Res)
		
		{"SetFiFongArmor",		LuaSetFiFongArmor},	//SetFiFongArmor(ID Res)
		
		{"GetFiFongArmor",		LuaGetFiFongArmor},
	
	
	{"DelItemIdx",LuaDelItemIdx},
	

	{"GetNpcName",LuaGetNpcName},
	{"SetNpcName",LuaSetNpcName},
	{"GetNpcSeries",LuaGetNpcSeries},
	{"SetNpcSeries",LuaSetNpcSeries},
	{"SetNpcExp",		LuaSetNpcExp},
	{"SetNpcLife",		LuaSetNpcLife},


	{"RestAP",LuaRestAP},
	{"GetLevelExp",LuaGetLevelExp},
	{"Setx2Exp",LuaSetx2Exp},
	{"Setx2Skill",LuaSetx2Skill},
	{"GetAccount",LuaGetPlayerAccount},
	{"GetComputerName",LuaGetComputerName},
	{"SetLevel",LuaSetLevel},
	{"SetItemMagicEx",LuaSetItemMagicEx},
	{"AddSkillEffect",LuaAddSkillEffect},
	{"BlockPKState",LuaBlockPKState},
	{"SetPKState",LuaSetPKState},
	{"SetItemLock",LuaSetItemLock},
	{"SetBindItem",LuaSetBindItem},
	{"SetBindAllItem",LuaSetBindAllItem},
	{"SetCuongHoa",LuaSetCuongHoa},
	
	{"FixPlayer",LuaFixPlayer},
	{"CreadTong",LuaCreadTong},
	{"LoginTong",LuaLoginTong},
	{"SendTongInfo",LuaSendTongInfo},
	{"MemberAddTong",LuaMemberAddTong},
	{"DeleteTong",LuaDeleteTong},
	{"UpdateTong",LuaUpdateTong},
	{"ChangeTitleFeMaleTong",LuaChangeTitleFeMaleTong},
	{"ChangeTitleMaleTong",LuaChangeTitleMaleTong},
	{"ChangeTitleTong",LuaChangeTitleTong},
	{"GetTongNameID",LuaGetTongNameID},
	{"DropEvent",LuaDropEvent},
	{"CheckFreeBoxItem",LuaCheckFreeBoxItem},
	{"OpenStringBox",LuaOpenStringBox},
	{"SaveData",LuaSaveData},
	{"BlockBox",LuaBlockBox},
	{"GetBlockBox",LuaGetBlockBox},
	{"GetTongName",LuaGetTongName},
	{"OpenCheckItemBox",LuaOpenCheckItemBox},
	
	{"GetIdxItemBox",LuaGetIdxItemBox},
	{"GetIdxItemBox2",LuaGetIdxItemBox2},
	{"GetIdxItemBox3",LuaGetIdxItemBox3},
	{"GetIdxItemBoxUpdateItem",LuaGetIdxItemBoxUpdateItem}, // Lay Idx Cua Tat Ca Item Theo Thu Tu
	{"GetIdxItemBoxUpdateItem3",LuaGetIdxItemBoxUpdateItem3}, // Lay Idx Cua Item trong Box (Bo qua Khoa Vinh Vien, Chi Check 1 vat pham)
	{"GetIdxItemBoxUpdateItem4",LuaGetIdxItemBoxUpdateItem4}, // Lay Idx Cua trang bi ton hai
	{"LayIdxHkmpTrongHomDo",LuaLayIdxHkmpTrongHomDo}, // Lay Idx Cua Tat Ca Item HKMP Theo Thu Tu trong Hom Update
	{"GetIdxItemBoxQuestKey",LuaGetIdxItemBoxQuestKey}, // Lay Idx Tat ca QuestKey trong Box
	{"DelEquipItemQuestKey",LuaDelEquipItemQuestKey}, // Xoa tat ca thuy tinh
	{"CheckTrangSucInBox",LuaCheckTrangSucInBox}, // Lay Idx Tat ca QuestKey trong Box
    {"TimeBox",			LuaOpenTimeBox},
	
	

	{"CheckMagicItem",LuaCheckMagicItem},
	{"GetSeriItem",LuaGetSeriItem},
	{"GetLevelItem",LuaGetLevelItem},
	{"GetParticularItem",LuaGetParticularItem},
	{"GetTimeItemIdx",LuaGetTimeItemIdx},
	{"GetDetailTypeItem",LuaGetDetailTypeItem},
	{"GetNameItemBox",LuaGetNameItemBox},
	{"GetDurationIdx",LuaGetDurationIdx},
	{"SetDurationIdx",LuaSetDurationIdx},
	
	{"GetGenreItem",LuaGetGenreItem},
	{"GetClassItem",LuaGetClassItem},
	{"IsBlockItem",LuaIsBlockItem},
	{"DelEquipIdx",LuaDelEquipIdx},

	{"OpenSlectBoxCheck",LuaOpenSlectBoxCheck},
	{"OpenPGBoxItem",LuaOpenPGBoxItem},
	{"AddEventItemMagic",LuaAddEventItemMagic},
	{"AddItemPink",LuaAddItemPink},
	{"GetIdxPGItem",LuaGetIdxPGItem},
	{"GetIdMagicEventItem",LuaGetIdMagicEventItem},
	{"GetLevelMagicEventItem",LuaGetLevelMagicEventItem},
	{"GetRequirementMagicEventItem",LuaGetRequirementMagicEventItem},
	{"GetItemType",LuaGetItemType},
	{"GetEquipMagicId",LuaGetEquipMagicId},
	{"GetEquipMagicLevel",LuaGetEquipMagicLevel},
	{"GetEquipMagicPoint",LuaGetEquipMagicPoint},
	{"GetEquipMagicLucky",LuaGetEquipMagicLucky},
	{"GetItemBoxLucky",LuaGetItemBoxLucky},
	{"GetEquipMagicRandomSeed",LuaGetEquipMagicRandomSeed},
	{"CheckItemEquipCS",LuaCheckItemEquipCS},
	{"CheckItemNguaPV",LuaCheckItemNguaPV},
	// {"DoMateSit",LuaDoMateSit},


	{"AddPropObj",LuaAddPropObj},
	{"AddPropObjPos",LuaAddPropObjPos},

	{"AddTrap",LuaAddTrap},
	{"SetPosU",	LuaSetPosU},	
	{"LoadTongMap",LuaLoadTongMap},
	{"GetTongFigure",LuaGetTongFigure},

	{"Msg2World",	LuaMsgToWorld},
	{"Msg2Tong",	LuaMsgToTong},

	{"GetTongMap",LuaGetTongMap},
	{"SetTongMap",LuaSetTongMap},
	{"DropMoney",		LuaDropMoney},


	{"SetTongMap",LuaSetTongMap},
	{"GetFromTownPortal",LuaGetFromTownPortal},
	{"GetCountPlayerMax",	LuaGetCountPlayerMax},
	{"GetPlayerTeamTable",	LuaGetPlayerTeamTable},
	{"GetLogoutRV", LuaGetPlayerRevivalOptionWhenLogout},
	{"ReLoadScript", LuaReLoadScript},

	{"GetPlayerItemCount", LuaGetPlayerItemCount},
	{"GetPlayerItemBlueCount", LuaGetPlayerItemBlueCount},

	{"GetPlayerItemIdxByNum",LuaGetPlayerItemIdxByNum},

	{"GetPramaItemIdx",LuaGetGetPramaItemIdx},
	{"GetBindItem",LuaGetBindItemIdx},
	{"GetCuongHoa",LuaGetCuongHoa},

	{"AddItemPrama",LuaAddItemPrama},
	{"SetTimeItem",LuaSetTimeItem},
	
	{"SetHSD",LuaSetHSD}, // Time Item
	{"GetHSD",LuaGetHSD}, // Time Item
	

	{"SetTimeOpenBlock",LuaSetTimeOpenBlock},
	


	{"GetCashBox",LuaGetCashBox},
	{"SetCashBox",LuaSetCashBox},

	{"SetCash",LuaSetPlayerCashMoney},


	{"OpenNewPWBox",LuaOpenNewPWBox},

	{"SetTimeNew",LuaSetTimeNew},
	{"GetTimeNew",LuaGetTimeNew},

	{"UpdateTopTKNew",LuaUpdateTopTKNew},
	{"DeleteTopTKNew",LuaDeleteTopTKNew},

	{"SendTopTKNew",LuaSendTopTKNew},

	{"Msg2WorldById",	LuaMsgToWorldById},

	{"GetLevelSkillEffectMan",LuaGetLevelSkillEffectMan},

	{"DoSkillMan",LuaDoSkillMan},

	{"GetNumberNameInCpuTK",LuaGetNumberNameInCpuTK},

	{"GetPKState",LuaGetPKState},

	{"SleepGameSV",LuaSleepGameSV},


	{"GetAllItemCountInCheckBox",LuaGetAllItemCountInCheckBox},
	{"DelAllItem",LuaDelAllItem},
	{"DelPhiPhong",LuaDelPhiPhong},
	

	{"GetItemCountInCheckBox",LuaGetItemCountInCheckBox},
	{"GetItemCountAll",LuaGetItemCountAll},
	{"CheckItemBlockInCheckBox",LuaCheckItemBlockInCheckBox},
	


	{"DeleteAllItem",LuaDeleteAllItem},
	{"DeleteAllItemInCheckBox",LuaDeleteAllItemInCheckBox},
	{"GetItemIdxInCheckBoxByNum",LuaGetItemIdxInCheckBoxByNum},
	{"AddItemEx",LuaAddItemEx},

	// {"SetMateName",LuaSetMateName},

	
	

	
	//{"Trade",			LuaTrade	},				//Trade("maininfo", "IniFileName.ini的路径名")
	//Trade("MainInfo", n, "item1|price1|function1", "item2|price2|function2", ......, "itemn|pricen|functionn")
	
	//客户端脚本指令
#else 
	{"PlaySound", LuaPlaySound}, //PlaySound(Sound);
	{"PlaySprMovie",LuaPlaySprMovie},//PlaySprMovie(npcindex, Movie, times)
#endif
	{"PlayMusic", LuaPlayMusic}, //PlayMusic(Music,Loop)
	{"FadeInMusic",LuaFadeInMusic},
	{"FadeOutMusic",LuaFadeOutMusic},
};


TLua_Funcs WorldScriptFuns[] =// 非指对玩家的脚本指令集
{
	//通用指令

	{"AddLocalNews",LuaAddLocalNews},
	{"AddLoaclTimeNews",LuaAddLocalTimeNews},
	{"AddLocalCountNews",LuaAddLocalCountNews	},
	//服务器端脚本指令
#ifdef _SERVER
	{"Msg2SubWorld",	LuaMsgToSubWorld},	//Msg2SubWorld(消息)通知世界
	{"Msg2IP",			LuaMsgToIP}, //Msg2IP(IP, ID, StrInfo)	
	{"SubWorldID2Idx",	LuaSubWorldIDToIndex}, //SubWorldID2Idx
	{"GetServerName",	LuaGetServerName},
	{"KickOutPlayer",	LuaKickOutPlayer},
	{"KickOutAccount",	LuaKickOutAccount},
#endif

//	{"GetMissionV", LuaGetMissionValue},//GetMissionV(Vid)
//	{"SetMissionV", LuaSetMissionValue},//SetMissionV(Vid, Value)
//	{"GetGlbMissionV", LuaGetGlobalMissionValue	},
//	{"SetGlbMissionV", LuaSetGlobalMissionValue	},
//	{"OpenMission", LuaInitMission},//OpenMission(missionid)
//	{"RunMission", LuaRunMission},
//	{"CloseMission", LuaCloseMission},//CloseMission(missionid)
//	{"StartMissionTimer", LuaStartMissionTimer},////StartMissionTimer(missionid, timerid, time)
//	{"StopMissionTimer", LuaStopMissionTimer},
//	{"GetMSRestTime", LuaGetMissionRestTime}, //GetMSRestTime(missionid, timerid)
//	{"GetMSIdxGroup",LuaGetPlayerMissionGroup},//GetPlayerGroup(missionid, playerid);
//	
//	{"AddMSPlayer", LuaAddMissionPlayer},
//	{"DelMSPlayer", LuaRemoveMissionPlayer},
//	{"GetNextPlayer", LuaGetNextPlayer},
//	{"PIdx2MSDIdx", LuaGetMissionPlayer_DataIndex},//(missionid, pidx)
//	{"MSDIdx2PIdx", LuaGetMissionPlayer_PlayerIndex},//(missionid, dataidx)
//	{"NpcIdx2PIdx", LuaNpcIndexToPlayerIndex},

	//{"GetMSPlayerCount", LuaMissionPlayerCount},//GetMSPlayerCount(missionid, group = 0)

//	{"RevivalAllNpc",	LuaRevivalAllNpc},
	
	//{"SetPMParam", LuaSetMissionPlayerParam },
	//{"GetPMParam", LuaGetMissionPlayerParam},
	//{"Msg2MSGroup", LuaMissionMsg2Group},
	//{"Msg2MSAll", LuaMissionMsg2All},
	//{"Msg2MSPlayer", LuaMissionMsg2Player},


}; 

int g_GetGameScriptFunNum()
{
	return sizeof(GameScriptFuns)  / sizeof(GameScriptFuns[0]);
}

int g_GetWorldScriptFunNum()
{
	return sizeof(WorldScriptFuns)  / sizeof(WorldScriptFuns[0]);
}