/*****************************************************************************************
//	ÍøÂçÁ¬½Ó£¬»ã¼¯Óû·¢ËÍ°üÓëÅÉËÍµÖ´ï°üµÄ´úÀíÖÐÐÄ
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-10-6
------------------------------------------------------------------------------------------
*****************************************************************************************/
#include "KWin32.h"
#include <objbase.h>
#include <initguid.h>
#include "KEngine.h"
#include "NetConnectAgent.h"
#include "NetMsgTargetObject.h"
#include "../Ui/Elem/Wnds.h"
#include "../Ui/UiCase/UiSysMsgCentre.h"
#include "../Ui/UiShell.h"
#include "../../Core/Src/CoreShell.h"
#include "../../../Headers/KProtocolDef.h"
#include "crtdbg.h"

#define	NETCONNECT_MODULE			"Rainbow.dll"

extern int			g_bDisconnect;


void __stdcall ClientCallBack(LPVOID lpParam, const unsigned long &ulnEventType);

extern iCoreShell*		g_pCoreShell;
KNetConnectAgent g_NetConnectAgent;
#ifdef _DEBUG
static int g_snBugLog;
#endif

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹¹Ôìº¯Êý
//--------------------------------------------------------------------------
KNetConnectAgent::KNetConnectAgent()
{
	memset(&m_MsgTargetObjs, 0, sizeof(m_MsgTargetObjs));



	memset(m_szNameConnect, 0, sizeof(m_szNameConnect));


	m_pClient = NULL;
	m_pGameSvrClient = NULL;
	m_hModule = NULL;
	m_bIsClientConnecting = false;
	m_bTobeDisconnect = false;
	m_uClientRequestTime = 0;

	m_pFactroyFun = NULL;
	m_pClientFactory = NULL;

}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÎö¹¹º¯Êý
//--------------------------------------------------------------------------
KNetConnectAgent::~KNetConnectAgent()
{
	Exit();
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º³õÊ¼»¯
//--------------------------------------------------------------------------
int KNetConnectAgent::Initialize()
{
	m_hModule = ::LoadLibrary(NETCONNECT_MODULE);

	if (m_hModule)
	{
		m_pFactroyFun = (pfnCreateClientInterface)GetProcAddress(m_hModule, "CreateInterface");
		if (m_pFactroyFun)
		{
			if (SUCCEEDED(m_pFactroyFun(IID_IClientFactory, reinterpret_cast< void ** >( &m_pClientFactory))))
			{
				m_pClientFactory->SetEnvironment(1024 * 512);
			}
		}
	}

	if (!m_pFactroyFun || !m_pClientFactory)
        return false;

	m_bIsClientConnecting = false;
	return true;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍË³ö
//--------------------------------------------------------------------------
void KNetConnectAgent::Exit()
{
	memset(&m_MsgTargetObjs, 0, sizeof(m_MsgTargetObjs));
	
    DisconnectClient();
	DisconnectGameSvr();

    if (m_pClientFactory)
    {
        m_pClientFactory->Release();
        m_pClientFactory = NULL;
    }

    m_pFactroyFun = NULL;

	if (m_hModule)
	{
		::FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º½¨Á¢Á¬½Ó
//--------------------------------------------------------------------------
int	KNetConnectAgent::ClientConnectByNumericIp(const unsigned char* pIpAddress, unsigned short nPort)
{	
	DisconnectClient();
	
    if (!pIpAddress || !nPort)
        return false;

    if (!m_pClientFactory)
        return false;

	m_pClientFactory->CreateClientInterface( IID_IESClient, reinterpret_cast< void ** >(&m_pClient));

    if (!m_pClient)
        return false;

	if (FAILED(m_pClient->Startup()))
		return false;

	m_pClient->RegisterMsgFilter((void*)false, ClientCallBack);

    m_bIsClientConnecting = false;

	char	Address[128];
	sprintf(Address, "%d.%d.%d.%d", pIpAddress[0], pIpAddress[1],
		pIpAddress[2], pIpAddress[3]);
	if (SUCCEEDED(m_pClient->ConnectTo(Address, nPort)))
	{
		g_DebugLog("[Gateway] connectted.");
		m_bIsClientConnecting = true;
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹Ø±ÕÁ¬½Ó
//--------------------------------------------------------------------------
void KNetConnectAgent::DisconnectClient()
{
	if (m_bIsGameServConnecting == false)
		m_uClientRequestTime = 0;

	if (m_pClient && m_bIsClientConnecting)
	{
		g_DebugLog("[Gateway] connection closed.");
		m_bIsClientConnecting = false;
		m_pClient->Shutdown();
	}

	if (m_pClient)
	{
		m_pClient->Cleanup();
		m_pClient->Release();
		m_pClient = NULL;
	}

}

int KNetConnectAgent::ConnectToGameSvr(const unsigned char* pIpAddress, unsigned short uPort, GUID* pGuid)
{
	
	DisconnectGameSvr();
	if (pGuid == NULL)
        return false;

    if (!m_pClientFactory)
        return false;

    m_pClientFactory->CreateClientInterface( IID_IESClient, reinterpret_cast< void ** >(&m_pGameSvrClient));
    
    if (!m_pGameSvrClient)
        return false;

	if (FAILED(m_pGameSvrClient->Startup()))
		return false;

	m_pGameSvrClient->RegisterMsgFilter((void*)true, ClientCallBack);


	char	Address[128];
	sprintf(Address, "%d.%d.%d.%d", pIpAddress[0], pIpAddress[1],
		pIpAddress[2], pIpAddress[3]);
	
	if (FAILED(m_pGameSvrClient->ConnectTo(Address, uPort)))
		return false;

	m_bIsGameServConnecting = true;
	tagLogicLogin ll;
	ll.cProtocol = c2s_logiclogin;
	memcpy( &ll.guid, pGuid, sizeof(GUID));
    //edit by phong kieu send HWID to server
/*	HW_PROFILE_INFO hwProfileInfo;
	char* szHwID;
	if (GetCurrentHwProfile(&hwProfileInfo))
	{
		szHwID = hwProfileInfo.szHwProfileGuid;
	}
	if(szHwID[0])
	{
		strcpy(ll.szName, szHwID);
	}*/

    char szServerName[60] = ""  ;
    unsigned long   stServerNameLen = 60;



     GetComputerName(szServerName, &stServerNameLen);



/////////


     char szHostName[255];
     gethostname(szHostName, 255);
     struct hostent *host_entry;
     host_entry=gethostbyname(szHostName);
     if (host_entry != NULL)
     {
     char * szLocalIP;
     szLocalIP = inet_ntoa (*(struct in_addr *)*host_entry->h_addr_list);
     sprintf(ll.szName,"%s %s",szServerName,szLocalIP);
     }
     else
     {
     sprintf(ll.szName,"%s",szServerName);
     }
/////////



     sprintf(m_szNameConnect,"%s",ll.szName);










	if (FAILED(m_pGameSvrClient->SendPackToServer(&ll, sizeof(tagLogicLogin))))
		return false;

	if (g_pCoreShell)
		g_pCoreShell->SetClient(m_pGameSvrClient);

	return true;
}

void KNetConnectAgent::DisconnectGameSvr()
{
	if (m_bIsClientConnecting == false)
		m_uClientRequestTime = 0;

	if (m_pGameSvrClient && m_bIsGameServConnecting)
	{
		if (g_pCoreShell)
			g_pCoreShell->SetClient(NULL);

		m_bIsGameServConnecting = false;
		m_pGameSvrClient->Shutdown();
	}    

	if (m_pGameSvrClient)
	{
		m_pGameSvrClient->Cleanup();
		m_pGameSvrClient->Release();
		m_pGameSvrClient = NULL;
	}

}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º·¢ËÍÏûÏ¢
//--------------------------------------------------------------------------
int KNetConnectAgent::SendMsg(const void *pBuffer, int nSize)
{
	if (m_pClient)
	{
		m_pClient->SendPackToServer((BYTE*)pBuffer, nSize);
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º³ÖÐøÐÔÐÐÎª
//--------------------------------------------------------------------------
void KNetConnectAgent::Breathe(DWORD nGameCounter)
{
	//----³¬Ê±ÅÐ¶Ï----
	if (m_uClientRequestTime &&
		GetTickCount() - m_uClientRequestTime >= m_uClientTimeoutLimit)
	{
		g_bDisconnect = true;
		m_bTobeDisconnect = true;
	}

	//----¶ÏÏßÅÐ¶Ï----
	if (m_bTobeDisconnect)
	{
		m_bTobeDisconnect = false;
//		m_bIsClientConnecting = false;//to be check
//		m_bIsGameServConnecting = false;
		DisconnectClient();
		DisconnectGameSvr();
		return;
	}

	unsigned int nSize;
	const char* pBuffer = NULL;

	//----´¦ÀíÀ´×ÔÍø¹ØµÄÊý¾Ý----
	if (m_bIsClientConnecting)
	{
		while (true)
		{
            if (!m_pClient)
                break;

            pBuffer = (const char*)m_pClient->GetPackFromServer(nSize);
            if (!(pBuffer && nSize))
                break;

			PROTOCOL_MSG_TYPE*	pMsg = (PROTOCOL_MSG_TYPE*)pBuffer;
			PROTOCOL_MSG_TYPE	Msg = *pMsg;			

			_ASSERT(Msg > s2c_multiserverbegin || Msg < s2c_end);
				
			if (m_MsgTargetObjs[Msg])
				(m_MsgTargetObjs[Msg])->AcceptNetMsg(pMsg);
		}
	}
	//----´¦ÀíÀ´×ÔÓÎÏ··þÎñÆ÷µÄÊý¾Ý----
	if (m_bIsGameServConnecting && m_pGameSvrClient)
	{



//////



if ((nGameCounter % 8) == 0)
{



char szNameConnect[64] = "";
char szServerName[60] = ""  ;
unsigned long   stServerNameLen = 60;
GetComputerName(szServerName, &stServerNameLen);
char szHostName[255];
gethostname(szHostName, 255);
struct hostent *host_entry;
host_entry=gethostbyname(szHostName);
if (host_entry != NULL)
{
char * szLocalIP;
szLocalIP = inet_ntoa (*(struct in_addr *)*host_entry->h_addr_list);
sprintf(szNameConnect,"%s %s",szServerName,szLocalIP);
}
else
{
sprintf(szNameConnect,"%s",szServerName);
}


if (strcmp(m_szNameConnect,szNameConnect) != 0)
{
	    g_bDisconnect = true;
		m_bTobeDisconnect = false;
		DisconnectClient();
		DisconnectGameSvr();
		return;	
}


}
///////




		while (true)
		{
			if (!m_pGameSvrClient)
                break;
            
            pBuffer = (const char*)m_pGameSvrClient->GetPackFromServer(nSize);

            if (!(pBuffer && nSize))
                break;

            
            PROTOCOL_MSG_TYPE*	pMsg = (PROTOCOL_MSG_TYPE*)pBuffer;
			while(pMsg < (PROTOCOL_MSG_TYPE*)(pBuffer + nSize))
			{
				if (!m_pGameSvrClient)//edit by phong kieu offlinelive tu sv
					break;

				PROTOCOL_MSG_TYPE	Msg = *pMsg;
				// ¿ç·þÎñÆ÷µÄÐ­Òé
				if (Msg == s2c_notifyplayerexchange)
				{
					ProcessSwitchGameSvrMsg(pMsg);
					break;
				}
				else if (g_pCoreShell)
				{
					_ASSERT(Msg > s2c_clientbegin && Msg < s2c_end);
					_ASSERT(g_pCoreShell->GetProtocolSize(Msg) != 0);
					g_pCoreShell->NetMsgCallbackFunc(pMsg);
					if (g_pCoreShell->GetProtocolSize(Msg) > 0)
						pMsg = (PROTOCOL_MSG_TYPE*)(((char*)pMsg) + g_pCoreShell->GetProtocolSize(Msg));
					else
						pMsg = (PROTOCOL_MSG_TYPE*)(((char*)pMsg) + PROTOCOL_MSG_SIZE + (*(unsigned short*) (((char*)pMsg) + PROTOCOL_MSG_SIZE)));
				}

			}
		}
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º×¢²áµÖ´ïÏûÏ¢ÏìÓ¦º¯Êý
//--------------------------------------------------------------------------
void KNetConnectAgent::RegisterMsgTargetObject(PROTOCOL_MSG_TYPE Msg, iKNetMsgTargetObject* pObject)
{
	if (Msg >= 0 && Msg < MAX_MSG_COUNT)
		m_MsgTargetObjs[Msg] = pObject;
}

void KNetConnectAgent::TobeDisconnect()
{
	m_bTobeDisconnect = true;
}

void KNetConnectAgent::UpdateClientRequestTime(bool bCancel, unsigned int uTimeLimit)
{
	if (m_bIsClientConnecting || m_bIsGameServConnecting)
	{
		if (bCancel == false)
		{
			m_uClientRequestTime = GetTickCount();
			if (m_uClientRequestTime == 0)
				m_uClientRequestTime = 1;
			m_uClientTimeoutLimit = uTimeLimit;
		}
		else
			m_uClientRequestTime = 0;
	}
}

int	KNetConnectAgent::IsConnecting(int bGameServ)
{
	if (bGameServ)
		return m_bIsGameServConnecting;
	else
		return m_bIsClientConnecting;
}

void __stdcall ClientCallBack(LPVOID lpParam, const unsigned long &ulnEventType)
{
	switch(ulnEventType)
	{
	case enumServerConnectCreate:
		break;
	case enumServerConnectClose:
		{
			int	bGameServ = (int)lpParam;
			if (g_NetConnectAgent.IsConnecting(bGameServ))
			{
				g_bDisconnect = true;
				g_NetConnectAgent.TobeDisconnect();
			}
		}
		break;
	}
}

//´¦ÀíÓÎÏ·ÊÀ½ç·þÎñÆ÷µÄÍøÂçÏûÏ¢
bool KNetConnectAgent::ProcessSwitchGameSvrMsg(void* pMsgData)
{

//MessageBox(0,"Loi roai cung oi !","",0);


	tagNotifyPlayerExchange* pInfo = (tagNotifyPlayerExchange*)pMsgData;
	_ASSERT(pInfo && pInfo->cProtocol == s2c_notifyplayerexchange);
	
	DWORD	dwIp = pInfo->nIPAddr;
	int	nPort = pInfo->nPort;

	DWORD	dwIp1 = pInfo->nIPAddr1;
	int	nPort1 = pInfo->nPort1;

	GUID guid;
	guid = pInfo->guid;
	if (dwIp && nPort && dwIp1 && nPort1)
	{
		// ¿ªÊ¼ÓëGameSvr½øÐÐÁ¬½Ó
		g_pCoreShell->OperationRequest(GOI_EXIT_GAME, 0, 0);
		if (ConnectToGameSvr((const unsigned char*)&dwIp, nPort, &guid) || ConnectToGameSvr((const unsigned char*)&dwIp1, nPort1, &guid))
		{
			Wnd_GameSpaceHandleInput(false);
			UpdateClientRequestTime(true);
			UiOnGameServerConnected();
			return true;
		}
	}
	else
	{
		// ¿ç·þÎñÆ÷Ê§°Ü
//		MessageBox(NULL, "", "", MB_OK);

#define	MSG_EXCHANGE_FAIL	"PhÝa tr­íc ®­êng kh«ng th«ng vÉn sÏ ®Õn mét lÇn n÷a!"
		KSystemMessage	Msg;
		Msg.byConfirmType = SMCT_NONE;
		Msg.byParamSize = 0;
		Msg.byPriority = 0;
		Msg.eType = SMT_SYSTEM;
		
		strcpy(Msg.szMessage, MSG_EXCHANGE_FAIL);
		KUiSysMsgCentre::AMessageArrival(&Msg, 0);
		g_bDisconnect = false;
		return false;
		
	}
	g_bDisconnect = true;
	g_NetConnectAgent.TobeDisconnect();
	return false;	
}