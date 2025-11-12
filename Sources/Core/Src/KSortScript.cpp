#include "KCore.h"
#include "KSortScript.h"
#include "LuaFuns.h"
#include "KFilePath.h"
#include "KDebug.h"
#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#else 
#include <io.h>
#include <direct.h>
#endif
//#include "Shlwapi.h"

KLuaScript g_ScriptSet[MAX_SCRIPT_IN_SET];
KScriptBinTree g_ScriptBinTree;
unsigned int   nCurrentScriptNum;
char g_szCurScriptDir[MAX_PATH];

void	LoadScriptInDirectory(LPSTR lpszRootDir, LPSTR lpszSubDir);

int	operator<(KSortScriptNode ScriptLeft, KSortScriptNode ScriptRight)
	{
		return ScriptLeft.GetScriptID() < ScriptRight.GetScriptID();
	};
	
	int operator==(KSortScriptNode ScriptLeft, KSortScriptNode ScriptRight)
	{
		return ScriptLeft.GetScriptID() == ScriptRight.GetScriptID();
	};

//将szFilePath目录下的所有脚本文件加载进ScriptBinTree二叉树中
static unsigned long LoadAllScript(char * szFilePath)
{
	g_SetFilePath("\\");
	char szRootPath[100];
	char szOldRootPath[MAX_PATH];

//	GetCurrentDirectory(MAX_PATH, szOldRootPath);
	getcwd(szOldRootPath, MAXPATH);
	g_GetFullPath(szRootPath,szFilePath);
	
	LoadScriptInDirectory(szRootPath, "");
	chdir(szOldRootPath);
//	SetCurrentDirectory(szOldRootPath);

	return nCurrentScriptNum;
}

unsigned long g_IniScriptEngine()
{
	g_szCurScriptDir[0] = 0;
	nCurrentScriptNum = 0;
	g_ScriptBinTree.ClearList();
	return LoadAllScript("\\script");
}



const KScript * g_GetScript(DWORD dwScriptId)
{
	KSortScriptNode ScriptNode;
	ScriptNode.SetScriptID(dwScriptId);
	if (g_ScriptBinTree.Find(ScriptNode))
	{
		return ScriptNode.GetScript();
	}
	return NULL;
}
const KScript * g_GetScript(const char * szRelativeScriptFile)
{
	DWORD dwScriptId = g_FileName2Id((LPSTR)szRelativeScriptFile);
	return g_GetScript(dwScriptId);
}

extern int LuaIncludeFile(Lua_State * L);

//加载脚本，该文件名参数为相对目录
static BOOL LoadScriptToSortListA(char * szRelativeFile)
{
	if (!szRelativeFile || !szRelativeFile[0]) return FALSE;
	KSortScriptNode ScriptNode ;
	ScriptNode.SetScriptID(g_FileName2Id(szRelativeFile));
	int t  =strlen(szRelativeFile);
	if (t >= 90)
		t ++;

#ifdef _DEBUG
	//strcpy(ScriptNode.m_szScriptName, szRelativeFile);
#endif
	if (nCurrentScriptNum < MAX_SCRIPT_IN_SET)
	{
		g_ScriptSet[nCurrentScriptNum].Init();
		g_ScriptSet[nCurrentScriptNum].RegisterFunctions(GameScriptFuns, g_GetGameScriptFunNum());
		g_StrCpyLen(g_ScriptSet[nCurrentScriptNum].m_szScriptName, szRelativeFile, 100);
		if (g_ScriptSet[nCurrentScriptNum].Load(szRelativeFile))
		{
		
		}
		else
		{
			g_DebugLog("[脚本]加载脚本%s，出错，该脚本无法加载！！请检查！！", szRelativeFile);
			return FALSE;
		}
	}
	else
	{
		g_DebugLog("[脚本]严重错误!脚本数量超限制%d！请立即解决！！", MAX_SCRIPT_IN_SET);
		return FALSE;
	}
	
	ScriptNode.SetScriptIndex(nCurrentScriptNum++);
	g_ScriptBinTree.Insert(ScriptNode);
	return TRUE;
}

//加载脚本，该文件名参数为实际目录
static BOOL LoadScriptToSortList(char * szFileName)
{
	if (!szFileName || !szFileName[0]) return FALSE;
	if (nCurrentScriptNum>= MAX_SCRIPT_IN_SET)
	{
		g_DebugLog("[Script]脚本总容量超过%d,严重错误请检查!",MAX_SCRIPT_IN_SET);
		return FALSE;
	}

	int nFileNameLen = strlen(szFileName);
	
	char szRootPath[MAX_PATH];	
	g_GetRootPath(szRootPath);
//	char szRelativePath[MAX_PATH];
	char *szRelativePath;
	char szCurrentDirectory[MAX_PATH];


//	GetCurrentDirectory(MAX_PATH, szCurrentDirectory);
	getcwd(szCurrentDirectory, MAX_PATH);
	szRelativePath = szCurrentDirectory + strlen(szRootPath);
//	PathRelativePathTo(szRelativePath,szRootPath, FILE_ATTRIBUTE_DIRECTORY,szCurrentDirectory , FILE_ATTRIBUTE_NORMAL );
	char szRelativeFile[MAX_PATH];
	if (szRelativePath[0] == '.' && szRelativePath[1] == '\\')
		sprintf(szRelativeFile, "%s\\%s", szRelativePath + 1, szFileName);
	else
		sprintf(szRelativeFile, "%s\\%s", szRelativePath, szFileName);
	g_StrLower(szRelativeFile);
	g_DebugLog("[Script]Loading Script %s %d", szRelativeFile, g_FileName2Id(szRelativeFile));
//
	return LoadScriptToSortListA(szRelativeFile);
//	return FALSE;
}


void	LoadScriptInDirectory(LPSTR lpszRootDir, LPSTR lpszSubDir)
{
	int				nFlag;
	char			szRealDir[MAX_PATH];

	sprintf(szRealDir, "%s\\%s", lpszRootDir, lpszSubDir);
	if(chdir(szRealDir)) 
		return;

	WIN32_FIND_DATA FindData;
	HANDLE dir = FindFirstFile("*.*", &FindData);
	while(dir != INVALID_HANDLE_VALUE) {
		if(strcmp(FindData.cFileName, ".") == 0 || strcmp(FindData.cFileName, "..") == 0)	{
			if(!FindNextFile(dir, &FindData)) break;
			continue;
		}
		if(FindData.dwFileAttributes == _A_SUBDIR)
		{
			LoadScriptInDirectory(szRealDir, FindData.cFileName);
		}
		else
		{	
			nFlag = 0;
			for (int i = 0; i < (int)strlen(FindData.cFileName);  i++)
			{
				if (FindData.cFileName[i] == '.')
					break;
				if (FindData.cFileName[i] == '\\')
				{
					nFlag = 1;
					break;
				}
			}
			if (nFlag == 1)
			{
				LoadScriptInDirectory(szRealDir, FindData.cFileName);
			}
			else
			{
				char* r = strrchr(FindData.cFileName,'.');
				if (r && !stricmp(r, ".LUA"))
				{
					if (!LoadScriptToSortList(FindData.cFileName))
						printf("-------Error loading %s file------\n", FindData.cFileName);
				}
			}
		}
		if(!FindNextFile(dir, &FindData)) 
			break;
	} 
	FindClose (dir);
	chdir(lpszRootDir);
}

void UnLoadScript(DWORD dwScriptID)
{
	KSortScriptNode ScriptNode;
	ScriptNode.SetScriptID(dwScriptID);
	g_ScriptBinTree.Delete(ScriptNode);
}

BOOL ReLoadScript(const char * szRelativePathScript)
{
	if (!szRelativePathScript || !szRelativePathScript[0])
		return FALSE;
	char script[MAX_PATH];
	strcpy(script, szRelativePathScript);
//	_strlwr(script);
        g_StrLower(script);
	UnLoadScript(g_FileName2Id(script));
	return LoadScriptToSortListA(script);
}

unsigned long  ReLoadAllScript()
{
	g_ScriptBinTree.ClearList();
	return g_IniScriptEngine();
}

