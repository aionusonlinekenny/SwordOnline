#include "KEngine.h"
#include "KCore.h"
#ifndef _STANDALONE
#include "../../Headers/IClient.h"
#else
#include "IClient.h"
#endif
#include "KObjSet.h"
#include "KNpcSet.h"
#include "KPlayer.h"
#include "KPlayerSet.h"
#include "KPlayerTeam.h"
#include "KPlayerFaction.h"
#include "KNpc.h"
#include "KSubWorld.h"
#include "LuaFuns.h"
#include "KNewProtocolProcess.h"
#include "KSkills.h"
#include "KItemSet.h"
#include "KBuySell.h"
#include "KSubWorldSet.h"
//#include "MyAssert.h"
#include "Scene/ObstacleDef.h"
#include "KMath.h"
#ifndef _STANDALONE
#include "CoreShell.h"
#include "..\..\..\Headers\KGmProtocol.h"
#else
#include "KGmProtocol.h"
#endif
BOOL TextMsgProcessGMCmd(int nPlayerIdx, const char * pGMCmd, int nLen);
void KNewProtocolProcess::P_ProcessGMExecute(IClient* pTransfer,
											 DWORD dwFromIP, DWORD dwFromRelayID,
											 int nPlayerIndex, BYTE* pData, int nDataSize)
{
}

void KNewProtocolProcess::P_ProcessGMFindPlayer(IClient* pTransfer,
												DWORD dwFromIP, DWORD dwFromRelayID,
												int nPlayerIndex, BYTE* pData, int nDataSize)
{
}

void KNewProtocolProcess::ExecuteLocalServer(const void * const pData, const size_t datalength)
{
}
