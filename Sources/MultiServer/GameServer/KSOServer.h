#ifndef KSOServerH
#define	KSOServerH

#include <map>
#include "KEngine.h"
#include "IClient.h"
#ifndef _STANDALONE
#include "RainbowInterface.h"
#include "HeavenInterface.h"
#include "..\..\Core\src\CoreServerShell.h"
#include "UsesWinsock.h"
#include "Buffer.h"
using OnlineGameLib::Win32::CPackager;

#else
#include "CoreServerShell.h"
#endif

#include "IServer.h"
#include "KTimer.h"
#include "KPakList.h"
#include "KTransferUnit.h"


enum NetStatus
{
	enumNetUnconnect = 0,
	enumNetConnected,
};

struct GameStatus 
{
	GameStatus() 
	{
		nGameStatus = 0; 
		nNetStatus = 0;
		nExchangeStatus = 0;
		nPlayerIndex = 0;
		nSendPingTime = 0;
		nReplyPingTime = 0;

		nTimeNextCreadConnect = 0;
		bTimeCheckCreadConnect = FALSE;



		memset(szName, 0, sizeof(szName));
	};
	int					nPlayerIndex;
	int					nGameStatus;
	int					nNetStatus;
	int					nExchangeStatus;
	int					nSendPingTime;
	int					nReplyPingTime;
	int					nTimeNextCreadConnect;
	BOOL				bTimeCheckCreadConnect;


	char                szName[64];
} ;

class KSwordOnLineSever
{
private:
#ifdef _STANDALONE
	ZBuffer *net_buffer;
#endif
	int					m_nMaxPlayerCount;
	int					m_nPrecision;
	static const int	m_snMaxBuffer;
	static const int	m_snBufferSize;
	int					m_nMaxPlayer;
	int					m_nGameLoop;
	int					m_nServerPort;


	int					m_nServerPort1;


	int					m_nGatewayPort;
	int					m_nDatabasePort;
	int					m_nTransferPort;
	int					m_nChatPort;
	int					m_nTongPort;
	DWORD				m_dwIntranetIp;
	DWORD				m_dwInternetIp;
	char				m_szGatewayIP[16];
	char				m_szDatabaseIP[16];
	char				m_szTransferIP[16];
	char				m_szChatIP[16];
	char				m_szTongIP[16];
	BOOL				m_bIsRunning;

	BOOL				m_bRunningDataBase;

	BOOL				m_bRunningGateway;


	int					m_nFowardServerPort0;
	DWORD				m_dwFowardServerIp0;
	int					m_nFowardServerPort1;
	DWORD				m_dwFowardServerIp1;
	int					m_nFowardServerPort2;
	DWORD				m_dwFowardServerIp2;
	int					m_nFowardServerPort3;
	DWORD				m_dwFowardServerIp3;
	int					m_nFowardServerPort4;
	DWORD				m_dwFowardServerIp4;
	int					m_nFowardServerPort5;
	DWORD				m_dwFowardServerIp5;
	int					m_nFowardServerPort6;
	DWORD				m_dwFowardServerIp6;
	int					m_nFowardServerPort7;
	DWORD				m_dwFowardServerIp7;
	int					m_nFowardServerPort8;
	DWORD				m_dwFowardServerIp8;
	int					m_nFowardServerPort9;
	DWORD				m_dwFowardServerIp9;

	int					m_nFowardServerPort10;
	DWORD				m_dwFowardServerIp10;

	int					m_nFowardServerPort11;
	DWORD				m_dwFowardServerIp11;

	int					m_nFowardServerPort12;
	DWORD				m_dwFowardServerIp12;

	int					m_nFowardServerPort13;
	DWORD				m_dwFowardServerIp13;

	int					m_nFowardServerPort14;
	DWORD				m_dwFowardServerIp14;

	int					m_nFowardServerPort15;
	DWORD				m_dwFowardServerIp15;

	int					m_nFowardServerPort16;
	DWORD				m_dwFowardServerIp16;

	int					m_nFowardServerPort17;
	DWORD				m_dwFowardServerIp17;

	int					m_nFowardServerPort18;
	DWORD				m_dwFowardServerIp18;
	int					m_nFowardServerPort19;
	DWORD				m_dwFowardServerIp19;

	int					m_nFowardServerPort20;
	DWORD				m_dwFowardServerIp20;
	int					m_nFowardServerPort21;
	DWORD				m_dwFowardServerIp21;


	int					m_nServerPortMin;
	int					m_nServerPortMax;


	int					m_nServerPortRange;

	int					m_nServerPortFowardRange1;
	int					m_nServerPortFowardRange2;
	int					m_nServerPortFowardRange3;


	int					m_nTimeServerPortChange;
	int					m_nServerPortFoward1;
	int					m_nServerPortFoward2;
	int					m_nServerPortFoward3;





//	BOOL				m_bSaveFlag;
//	int					m_nSaveCount;
    KPakList	g_PakList;
	IServer*			m_pServer;
	IClient*			m_pGatewayClient;
	IClient*			m_pDatabaseClient;
	IClient*			m_pTransferClient;
	IClient*			m_pChatClient;
	IClient*			m_pTongClient;
	GameStatus*			m_pGameStatus;
	struct iCoreServerShell*	m_pCoreServerShell;
	KTimer				m_Timer;
	typedef std::map<DWORD, KTransferUnit*>	IP2CONNECTUNIT;
	IP2CONNECTUNIT		m_mapIp2TransferUnit;
public:
	KSwordOnLineSever();
	~KSwordOnLineSever();
	BOOL				Init();
	BOOL				Breathe();


	int					GetNetStatus(const unsigned long lnID);
	void				SetNetStatus(const unsigned long lnID, NetStatus nStatus);
	void				SetRunningStatus(BOOL bStatus);
	void				SetRunningDataBase(BOOL bStatus);
	BOOL				GetRunningDataBase();


	void                SetRunningGateway(BOOL bStatus);
    BOOL                GetRunningGateway();








	void				Release();
private:
	void				MessageLoop();
	void				GatewayMessageProcess(const char* pChar, size_t nSize);
	void				DatabaseMessageProcess(const char* pChar, size_t nSize);
	void				DatabaseLargePackProcess(const char* pChar, size_t nSize);
	void				TransferMessageProcess(const char* pChar, size_t nSize);
	void				PlayerMessageProcess(const unsigned long lnID, const char* pChar, size_t nSize);
	void				GatewayLargePackProcess(const void *pData, size_t dataLength);
	void				GatewaySmallPackProcess(const void *pData, size_t dataLength);
	void				GatewayBoardCastProcess(const char* pData, size_t dataLength);
	void				TransferLargePackProcess(const void *pData, size_t dataLength, KTransferUnit* pUnit);
	void				TransferSmallPackProcess(const void *pData, size_t dataLength, KTransferUnit* pUnit);
	void				TransferAskWayMessageProcess(const char *pData, size_t dataLength);
	void				TransferLoseWayMessageProcess(const char *pData, size_t dataLength);
	void				ChatMessageProcess(const char *pChar, size_t nSize);
	void				ChatGroupMan(const void *pData, size_t dataLength);
	void				ChatSpecMan(const void *pData, size_t dataLength);

	BOOL				ConformAskWay(const void* pData, int nSize, DWORD *pdwID);
	void				MainLoop();
	int					ProcessLoginProtocol(const unsigned long lnID, const char* pData, size_t dataLength);
	BOOL				ProcessSyncReplyProtocol(const unsigned long lnID, const char* pData, size_t dataLength);
	void				PingClient(const unsigned long lnID);
	void				ProcessPingReply(const unsigned long lnID, const char* pData, size_t dataLength);

	BOOL				SendGameDataToClient(const unsigned long lnID, const int nPlayerIndex);

	void				ExSpcritStarPlayer(const int nPlayerIndex);

	void				ExSpcritStopPlayer(const int nPlayerIndex);

	void				KickAcountAndBlock(const int nPlayerIndex,const char* szNameCpuIp);

	void				SavePlayerData();
	BOOL				SavePlayerData(int nIndex, bool bUnLock);
	void				ExitAllPlayer();
	void				PlayerLogoutGateway();
	void				PlayerExchangeServer();
	BOOL				GetLocalIpAddress(DWORD *pIntranet, DWORD *pInternet);

	// 帮会协议处理( relay 发过来的协议)
	void				TongMessageProcess(const char *pChar, size_t nSize);
	// 帮会协议处理(客户端通过帮会扩展协议发过来的协议)
	void				ProcessPlayerTongMsg(const unsigned long nPlayerIdx, const char* pData, size_t dataLength);

	BOOL				CheckPlayerID(unsigned long netidx, DWORD nameid);



	BOOL				ChangePortServer(unsigned short uPort, unsigned short uPort1);


};

#endif
