/********************************************************************
	created:	2003/05/11
	file base:	GameServer
	file ext:	h
	author:		liupeng
	
	purpose:	
*********************************************************************/
#ifndef __INCLUDE_GAMESERVER_H__
#define __INCLUDE_GAMESERVER_H__

#include "Buffer.h"

#include "IGServer.h"

#include "KProtocol.h"

#include "IClient.h"
#include "RainbowInterface.h"
#include "IServer.h"
#include "HeavenInterface.h"

#include "CriticalSection.h"
#include "Event.h"

#include "KProtocolDef.h"

#include <map>
#include <list>

/*
 * class CGameServer
 */
class CGameServer : public IGServer
{
public:
	BYTE nServerID;

	typedef struct tagNetworkInterface
	{
		IServer *pServer;
		IClient	*pClient;
		
		HWND	hwndContainer;
		
	}NI, NEAR *PNI, FAR *LPNI;
	
	CGameServer( IServer *pGameSvrServer, 
		IClient	*pAccountClient,
		UINT nIdentityID = ( UINT )( -1 ) );

	virtual ~CGameServer();

	virtual bool Create();
	virtual bool Destroy();
	
	virtual bool Attach( const char *pAccountName );

	virtual void SendText( const char *pText, int nLength, UINT uOption );

	virtual bool AnalyzeRequire( const void *pData, size_t datalength );

	enum enumTask
	{
		enumTaskProtocol = 0x5,

		enumSyncRoleInfo,
		enumPlayerLogicLogout,
	};

	virtual bool DispatchTask( UINT nTask, const void *pData, size_t datalength, WORD nData);

	static bool SetupGlobalAllocator( size_t bufferSize, size_t maxFreeBuffers );

	static void __stdcall GameSvrEventNotify( LPVOID lpParam,
			const unsigned long &ulnID,
			const unsigned long &ulnEventType );

	static bool Begin( IServer *pGameSvrServer );
	static void End();
	
	static void SendToAll( const char *pText, int nLength, UINT uOption );
	
	static IGServer *QueryServer( UINT nMapID );
	static IGServer *GetServer( size_t nID );

	virtual size_t GetID() { return m_lnIdentityID; };
	virtual size_t GetContent();
	virtual size_t GetCapability() { return m_dwCapability; };
	virtual size_t GetIP( BYTE cType ) { if ( INTERNET_IP == cType ) { return m_nServerIP_Internet; } return m_nServerIP_Intraner; };
	virtual size_t GetIndentify() { return m_lnIdentityID; };
	
protected:

	bool _QueryMapID();
	bool _QueryGameSvrInfo();
	
	bool _RegisterAccount( const void *pData, size_t datalength );
	bool _UpdateMapID( const void *pData, size_t datalength );
	bool _UpdateGameSvrInfo( const void *pData, size_t datalength );
	bool _NotifyPlayerLogin( const void *pData, size_t datalength );
	bool _NotifyEnterGame( const void *pData, size_t datalength );
	bool _NotifyLeaveGame( const void *pData, size_t datalength );
	bool _RequestSvrIP( const void *pData, size_t datalength );

	bool _SyncRoleInfo( const void *pData, size_t datalength, WORD nData);
	bool _PlayerLogicLogout( const void *pData, size_t datalength );
	
	bool LargePackProcess( BYTE cProtocol, const void *pData, size_t datalength );
	bool SmallPackProcess( BYTE cProtocol, const void *pData, size_t datalength );

	bool PushAccount( const char *pAccountName );
	bool PopAccount( const char *pAccountName, bool bUnlockAccount, WORD nExtPoint);

	bool ConsumeMoney( const char *pAccountName );
	bool FreezeMoney( const char *pAccountName, WORD nExtPoint );

private:

	/*
	 * Map ID convert to a game server index
	 */
	static OnlineGameLib::Win32::CCriticalSection	m_csMapIDAction;

	typedef std::list< IGServer * >					stdServerList;
	typedef std::map< size_t, stdServerList >		stdMapIDConvert;

	static stdMapIDConvert							m_theMapIDConvert;

	static bool RegisterServer( UINT nID, IGServer *pGServer );
	
	/*
	 * Process protocol function
	 */
	typedef bool ( CGameServer::*ProcessProtocolFun[c2s_end] )( const void *pData, size_t datalength );

	ProcessProtocolFun	m_theProcessProtocolFun;

private:

	static OnlineGameLib::Win32::CBuffer::Allocator	m_theGlobalAllocator;

	static LONG		m_slnIdentityCounts;
	LONG			m_lnIdentityID;
	
	IServer			*m_pGameSvrServer;
//	IClient			*m_pAccountClient;
	
	OnlineGameLib::Win32::CPackager	m_thePackager;

	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Intraner;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Internet;


    OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward0;
    OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward1;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward2;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward3;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward4;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward5;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward6;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward7;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward8;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward9;
    OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward10;
    OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward11;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward12;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward13;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward14;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward15;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward16;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward17;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward18;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward19;
    OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward20;
    OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward21;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward22;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward23;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward24;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward25;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward26;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward27;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward28;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward29;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward30;
    OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward31;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward32;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward33;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward34;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward35;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward36;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward37;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward38;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward39;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward40;
    OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward41;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward42;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward43;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward44;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward45;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward46;
	OnlineGameLib::Win32::_tstring	m_sServerIPAddr_Foward47;


	unsigned long					m_nServerIP_Intraner;
	unsigned long					m_nServerIP_Internet;

	unsigned long					m_nServerIP_Foward0;
	unsigned long					m_nServerIP_Foward1;
	unsigned long					m_nServerIP_Foward2;
	unsigned long					m_nServerIP_Foward3;
	unsigned long					m_nServerIP_Foward4;
	unsigned long					m_nServerIP_Foward5;
	unsigned long					m_nServerIP_Foward6;
	unsigned long					m_nServerIP_Foward7;
	unsigned long					m_nServerIP_Foward8;
	unsigned long					m_nServerIP_Foward9;
	unsigned long					m_nServerIP_Foward10;
	unsigned long					m_nServerIP_Foward11;
	unsigned long					m_nServerIP_Foward12;
	unsigned long					m_nServerIP_Foward13;
	unsigned long					m_nServerIP_Foward14;
	unsigned long					m_nServerIP_Foward15;
	unsigned long					m_nServerIP_Foward16;
	unsigned long					m_nServerIP_Foward17;
	unsigned long					m_nServerIP_Foward18;
	unsigned long					m_nServerIP_Foward19;
	unsigned long					m_nServerIP_Foward20;
	unsigned long					m_nServerIP_Foward21;


	
	unsigned short					m_nServerPort;

	unsigned short					m_nServerPortFoward0;
	unsigned short					m_nServerPortFoward1;
	unsigned short					m_nServerPortFoward2;
	unsigned short					m_nServerPortFoward3;
	unsigned short					m_nServerPortFoward4;
	unsigned short					m_nServerPortFoward5;
	unsigned short					m_nServerPortFoward6;
	unsigned short					m_nServerPortFoward7;
	unsigned short					m_nServerPortFoward8;
	unsigned short					m_nServerPortFoward9;
	unsigned short					m_nServerPortFoward10;
	unsigned short					m_nServerPortFoward11;
	unsigned short					m_nServerPortFoward12;
	unsigned short					m_nServerPortFoward13;
	unsigned short					m_nServerPortFoward14;
	unsigned short					m_nServerPortFoward15;
	unsigned short					m_nServerPortFoward16;
	unsigned short					m_nServerPortFoward17;
	unsigned short					m_nServerPortFoward18;
	unsigned short					m_nServerPortFoward19;
	unsigned short					m_nServerPortFoward20;
	unsigned short					m_nServerPortFoward21;


	DWORD							m_dwCapability;




	int m_nSlectPort;





	/*
	 * Listen and accept data from network
	 */
	static OnlineGameLib::Win32::CEvent		m_shQuitEvent;
	static OnlineGameLib::Win32::CEvent		m_shStartupManagerThreadEvent;

	static HANDLE m_shManagerThread;

	static DWORD WINAPI	ManagerThreadFunction( void *pParam );

	/*
	 * Game server information
	 */	
	static OnlineGameLib::Win32::CCriticalSection	m_csGameSvrAction;
	
	typedef std::map< size_t, IGServer * >			stdGameSvr;

	static stdGameSvr								m_theGameServers;
	
	/*
	 * Somebody attach the gameserver be save in this table
	 */
	OnlineGameLib::Win32::CCriticalSection			m_csAITS;

	typedef std::map< std::string, void * >			stdAccountAttachIn;

	stdAccountAttachIn								m_theAccountInThisServer;

	bool AttatchAccountToGameServer( const char *pAccountName );
	bool HaveAccountInGameServer( const char *pAccountName );
	bool DetachAccountFromGameServer( const char *pAccountName );
};

#endif // __INCLUDE_GAMESERVER_H__