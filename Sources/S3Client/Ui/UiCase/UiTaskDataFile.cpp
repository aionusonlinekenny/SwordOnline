/* 
 * File:     UiTaskDataFile.h
 * Desc:     任务记事文件操作
 * Author:   flying
 * Creation: 2003/7/19
 */
//-----------------------------------------------------------------------------

// system header file section. standard C-Runtime library.
#include "KWin32.h"
#include "KFile.h"
#include "KFilePath.h"
#include "UiTaskDataFile.h"
#include "../UiBase.h"

#define		FILE_NAME	"MissionMemory.dat"

KTaskDataFile::KPersonalRecord*			KTaskDataFile::ms_pPersonalRecord = NULL;
KTaskDataFile::KTaskSystemRecordNode*	KTaskDataFile::ms_pSystemRecordList = NULL;
int										KTaskDataFile::ms_nSystemRecordCount = 0;
KTaskDataFile::KTaskBindRecordNode*	KTaskDataFile::ms_pBindRecordList = NULL;
int										KTaskDataFile::ms_nBindRecordCount = 0;

void KTaskDataFile::LoadData()
{
	ClearAll();

	KFile	File;
	char	szFileName[128];
	int 	nPreSize;
	int		i;
		
	GetFileName(szFileName, sizeof(szFileName));
	bool	bOk = false;
	while(File.Open(szFileName))
	{
		//==读文件头==
		TASK_FILE_HEADER	Header;
		if (File.Read(&Header, sizeof(Header)) != sizeof(Header))
			break;
		if (*(int*)(&(Header.cFlag[0])) != TASK_FILE_FLAG)
			break;

		//==读个人纪录==
		if (Header.nPersonalRecordBytes)
		{
			ms_pPersonalRecord = (KPersonalRecord*)malloc(Header.nPersonalRecordBytes);
			if (ms_pPersonalRecord)
			{
				if (!File.Read(ms_pPersonalRecord, Header.nPersonalRecordBytes))
					break;
			}
			else
			{
				break;
			}
		}

		TASK_SYSTEM_RECORD	srecord;
		nPreSize = sizeof(TASK_SYSTEM_RECORD) - sizeof(srecord.cBuffer);
		for (i = 0; i < Header.nSystemRecordCount; i++)
		{
			if (File.Read(&srecord, nPreSize) != nPreSize)
				break;
			KTaskSystemRecordNode* pNode = (KTaskSystemRecordNode*)malloc(
				sizeof(KTaskSystemRecordNode) + srecord.nContentLen);
			if (pNode)
			{
				if (File.Read(pNode->Record.cBuffer, srecord.nContentLen) != srecord.nContentLen)
				{
					free (pNode);
					break;
				}
				pNode->pNext = NULL;
				pNode->Record.nContentLen = srecord.nContentLen;
				pNode->Record.tTime = srecord.tTime;
				pNode->Record.uReserved = srecord.uReserved;
				AppendSystemRecord(pNode);
			}
		}	

		TASK_BIND_RECORD	brecord;
		nPreSize = sizeof(TASK_BIND_RECORD) - sizeof(brecord.cBuffer);
		for (i = 0; i < Header.nBindRecordCount; i++)
		{
			if (File.Read(&brecord, nPreSize) != nPreSize)
				break;
			KTaskBindRecordNode* pNode = (KTaskBindRecordNode*)malloc(
				sizeof(KTaskBindRecordNode) + brecord.nContentLen);
			if (pNode)
			{
				if (File.Read(pNode->Record.cBuffer, brecord.nContentLen) != brecord.nContentLen)
				{
					free (pNode);
					break;
				}
				pNode->pNext = NULL;
				pNode->Record.nContentLen = brecord.nContentLen;
				pNode->Record.tTime = brecord.tTime;
				pNode->Record.uReserved = brecord.uReserved;
				AppendBindRecord(pNode);
			}
		}		

		bOk = true;
		break;
	};
	File.Close();
	if (bOk == false)
		ClearAll();
}

bool KTaskDataFile::SaveData()
{
	KFile	File;
	char	szFileName[128];
	int		i,j;
	
	GetFileName(szFileName, sizeof(szFileName));

	bool	bOk = false;
	while (ms_pPersonalRecord || ms_nSystemRecordCount > 0 || ms_nBindRecordCount > 0)
	{
		if (!File.Create(szFileName))
			break;
		//==写文件头==
		TASK_FILE_HEADER	Header = { 0 };
		*(int*)(&(Header.cFlag[0])) = TASK_FILE_FLAG;
		if (ms_pPersonalRecord && ms_pPersonalRecord->nLen > 0)
		{
			Header.nPersonalRecordBytes = sizeof(KPersonalRecord) +
				ms_pPersonalRecord->nLen - sizeof(ms_pPersonalRecord->cBuffer);
		}
		Header.nSystemRecordCount = ms_nSystemRecordCount;
		Header.nBindRecordCount = ms_nBindRecordCount;
		if (File.Write(&Header, sizeof(Header)) != sizeof(Header))
			break;		

		if (ms_pPersonalRecord)
		{
			if (File.Write(ms_pPersonalRecord, Header.nPersonalRecordBytes) != Header.nPersonalRecordBytes)
				break;
		}
		

		KTaskSystemRecordNode* psCurrent = ms_pSystemRecordList;
		for (i = 0; i < ms_nSystemRecordCount; i++)
		{
			int nSize = sizeof(TASK_SYSTEM_RECORD) + psCurrent->Record.nContentLen -
				sizeof(psCurrent->Record.cBuffer);
			if (File.Write(&psCurrent->Record, nSize) != nSize)
				break;
			psCurrent = psCurrent->pNext;
		}
	
	
		KTaskBindRecordNode* pbCurrent = ms_pBindRecordList;
		for (j = 0; j < ms_nBindRecordCount; j++)
		{
			int nSize = sizeof(TASK_BIND_RECORD) + pbCurrent->Record.nContentLen -
				sizeof(pbCurrent->Record.cBuffer);
			if (File.Write(&pbCurrent->Record, nSize) != nSize)
				break;
			pbCurrent = pbCurrent->pNext;
		}		
		
		if (i >= ms_nSystemRecordCount || j >= ms_nBindRecordCount)
		{
			bOk = true;
		}
		break;
	};

	File.Close();
	if (bOk == false)
	{
		char	szFullName[MAX_PATH];
		g_GetFullPath(szFullName, szFileName);
		DeleteFile(szFullName);
	}
	return bOk;
}

void KTaskDataFile::GetFileName(char* pszFileName, int nLen)
{
	if (pszFileName)
	{
		g_UiBase.GetUserPrivateDataFolder(pszFileName, nLen);
		strcat(pszFileName, FILE_NAME);
	}
}
 
void KTaskDataFile::ClearAll()
{
	if (ms_pPersonalRecord)
	{
		free (ms_pPersonalRecord);
		ms_pPersonalRecord = NULL;
	}
	if (ms_pSystemRecordList)
	{
		free (ms_pSystemRecordList);
		ms_pSystemRecordList = NULL;
	}
	ms_nSystemRecordCount = 0;
	
	if (ms_pBindRecordList)
	{
		free (ms_pBindRecordList);
		ms_pBindRecordList = NULL;
	}
	ms_nBindRecordCount = 0;	
}

int	KTaskDataFile::GetPersonalRecord(char* pszBuffer, int nBufferSize)
{
	int nRet = 0;
	if (ms_pPersonalRecord && pszBuffer)
	{
		if (nBufferSize > ms_pPersonalRecord->nLen)
		{
			nRet = ms_pPersonalRecord->nLen;
			memcpy(pszBuffer, ms_pPersonalRecord->cBuffer, nRet);
			pszBuffer[nRet] = 0;
		}
	}
	return nRet;
}

bool KTaskDataFile::SetPersonalRecord(const char* pstrRecord, int nLen)
{
	bool	bOk = false;
	if (ms_pPersonalRecord)
	{
		free(ms_pPersonalRecord);
		ms_pPersonalRecord = NULL;
	}
	if (pstrRecord && nLen > 0)
	{
		ms_pPersonalRecord = (KPersonalRecord*)malloc(sizeof(KPersonalRecord)
			+ nLen - sizeof(((KPersonalRecord*)0)->cBuffer));
		if (ms_pPersonalRecord)
		{
			ms_pPersonalRecord->nLen = nLen;
			memcpy(ms_pPersonalRecord->cBuffer, pstrRecord, nLen);
			bOk = true;
		}
	}
	return bOk;
}

bool KTaskDataFile::RemoveSystemRecord(int nIndex)
{
	bool	bOk = false;
	if (nIndex >= 0 && nIndex < ms_nSystemRecordCount)
	{
		KTaskSystemRecordNode* pToRemove = NULL;
		if (nIndex == 0)
		{
			pToRemove = ms_pSystemRecordList;
			ms_pSystemRecordList = pToRemove->pNext;
		}
		else
		{
			KTaskSystemRecordNode* pPre = ms_pSystemRecordList;
			while(nIndex > 1)
			{
				pPre = pPre->pNext;
				nIndex --;
			};
			pToRemove = pPre->pNext;
			pPre->pNext = pToRemove->pNext;
		}
		free (pToRemove);
		ms_nSystemRecordCount --;
		bOk = true;
	}
	return bOk;
}

int KTaskDataFile::GetSystemRecordCount()
{
	return ms_nSystemRecordCount;
}

const TASK_SYSTEM_RECORD* KTaskDataFile::GetSystemRecord(int nIndex)
{
	TASK_SYSTEM_RECORD* pRet = NULL;
	if (nIndex >= 0 && nIndex < ms_nSystemRecordCount)
	{
		KTaskSystemRecordNode* pNode = ms_pSystemRecordList;
		while(nIndex > 0)
		{
			pNode = pNode->pNext;
			nIndex --;
		}
		pRet = &pNode->Record;
	}
	return pRet;
}

bool KTaskDataFile::InsertSystemRecord(TASK_SYSTEM_RECORD* pRecord)
{
	bool bOk = false;
	if (pRecord)
	{
		if (pRecord->nContentLen > 0 && pRecord->pBuffer)
		{
			KTaskSystemRecordNode* pNode = (KTaskSystemRecordNode*)malloc(
				sizeof(KTaskSystemRecordNode) + pRecord->nContentLen);
			if (pNode)
			{
				pNode->pNext = NULL;
				memcpy(pNode->Record.cBuffer, pRecord->pBuffer, pRecord->nContentLen);
				pNode->Record.nContentLen = pRecord->nContentLen;
				pNode->Record.tTime = pRecord->tTime;
				pNode->Record.uReserved = pRecord->uReserved;
				InsertSystemRecord(pNode);
				bOk = true;
			}
		}
	}
	return bOk;
}

void KTaskDataFile::AppendSystemRecord(KTaskSystemRecordNode* pNode)
{
	if (pNode)
	{
		if (ms_pSystemRecordList == NULL)
		{
			ms_pSystemRecordList = pNode;
		}
		else
		{
			KTaskSystemRecordNode* pPre = ms_pSystemRecordList;
			while(pPre->pNext)
				pPre = pPre->pNext;
			pPre->pNext = pNode;
		}
		ms_nSystemRecordCount ++;
	}
}

void KTaskDataFile::InsertSystemRecord(KTaskSystemRecordNode* pNode)
{
	if (pNode)
	{
		pNode->pNext = ms_pSystemRecordList;
		ms_pSystemRecordList = pNode;
		ms_nSystemRecordCount ++;
	}
}

// BindRecord
bool KTaskDataFile::RemoveBindRecord(int nIndex)
{
	bool	bOk = false;
	if (nIndex >= 0 && nIndex < ms_nBindRecordCount)
	{
		KTaskBindRecordNode* pToRemove = NULL;
		if (nIndex == 0)
		{
			pToRemove = ms_pBindRecordList;
			ms_pBindRecordList = pToRemove->pNext;
		}
		else
		{
			KTaskBindRecordNode* pPre = ms_pBindRecordList;
			while(nIndex > 1)
			{
				pPre = pPre->pNext;
				nIndex --;
			};
			pToRemove = pPre->pNext;
			pPre->pNext = pToRemove->pNext;
		}
		free (pToRemove);
		ms_nBindRecordCount --;
		bOk = true;
	}
	return bOk;
}

int KTaskDataFile::GetBindRecordCount()
{
	return ms_nBindRecordCount;
}

const TASK_BIND_RECORD* KTaskDataFile::GetBindRecord(int nIndex)
{
	TASK_BIND_RECORD* pRet = NULL;
	if (nIndex >= 0 && nIndex < ms_nBindRecordCount)
	{
		KTaskBindRecordNode* pNode = ms_pBindRecordList;
		while(nIndex > 0)
		{
			pNode = pNode->pNext;
			nIndex --;
		}
		pRet = &pNode->Record;
	}
	return pRet;
}

bool KTaskDataFile::InsertBindRecord(TASK_BIND_RECORD* pRecord)
{
	bool bOk = false;
	if (pRecord)
	{
		if (pRecord->nContentLen > 0 && pRecord->pBuffer)
		{
			KTaskBindRecordNode* pNode = (KTaskBindRecordNode*)malloc(
				sizeof(KTaskBindRecordNode) + pRecord->nContentLen);
			if (pNode)
			{
				pNode->pNext = NULL;
				memcpy(pNode->Record.cBuffer, pRecord->pBuffer, pRecord->nContentLen);
				pNode->Record.nContentLen = pRecord->nContentLen;
				pNode->Record.tTime = pRecord->tTime;
				pNode->Record.uReserved = pRecord->uReserved;
				InsertBindRecord(pNode);
				bOk = true;
			}
		}
	}
	return bOk;
}

void KTaskDataFile::AppendBindRecord(KTaskBindRecordNode* pNode)
{
	if (pNode)
	{
		if (ms_pBindRecordList == NULL)
		{
			ms_pBindRecordList = pNode;
		}
		else
		{
			KTaskBindRecordNode* pPre = ms_pBindRecordList;
			while(pPre->pNext)
				pPre = pPre->pNext;
			pPre->pNext = pNode;
		}
		ms_nBindRecordCount ++;
	}
}

void KTaskDataFile::InsertBindRecord(KTaskBindRecordNode* pNode)
{
	if (pNode)
	{
		pNode->pNext = ms_pBindRecordList;
		ms_pBindRecordList = pNode;
		ms_nBindRecordCount ++;
	}
}