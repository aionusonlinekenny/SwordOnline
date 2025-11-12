#include "KCore.h"
#include "KPlayer.h"
#include "LuaFuns.h"
#include "KGMCommand.h"
#include "KSortScript.h"
#define GMCMD_GENRE_PREFIX_LEN 3
#define GM_CTRL_CMD_PREFIX '?'

static TGameMaster_Command GM_Command[]=
{
	{"DoSct",			GMDoScriptAction},				//DoSct Say("abc");
	{"ds",				GMDoScriptAction},				//DS Say("abc");

	{"dw",				GMDoWorldScriptAction},			//dw AddNews("abc");

	{"RunSctFile",		GMRunScriptFile},				//RunSctFile /Scripts/Abc.lua main 10
	{"RSF",				GMRunScriptFile},				//RSF /Scripts/Abc.lua main 10

	{"ReLoadSct",		GMReloadScriptFile},			//ReloadSct  /Scripts/Abc.lua
	{"RLS",				GMReloadScriptFile},
	
	{"ReLoadAllSct",	GMReloadAllScriptFile},			//ReloadAllSct
	{"RLAS",			GMReloadAllScriptFile},
	
};
#ifdef _DEBUG
CORE_API BOOL TextGMFilter(int nPlayerIdx, const char* pText, int nLen)
#else
BOOL TextGMFilter(int nPlayerIdx, const char* pText, int nLen)
#endif
{

return FALSE;
	
}

BOOL TextMsgProcessGMCmd(int nPlayerIdx, const char * pGMCmd, int nLen)
{
		
	return FALSE;
}

BOOL  ProcessGMCommand(int nPlayerIdx, EGameMasterCommand eCommand, const char * pParam, int nLen)
{


	return FALSE;
}

int GetNextUnit(const char * szString , const char cDiv, int nLen, char * szResult)
{

	return 0;	
}
