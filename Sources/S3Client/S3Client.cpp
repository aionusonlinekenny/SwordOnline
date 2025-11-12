 // S3Client.cpp : Defines the entry point for the application.
//

#include "KWin32.h"
#include "KCore.h"
#include "S3Client.h"
#include "KWin32Wnd.h"
#include "../../Represent/iRepresent/iRepresentShell.h"
#include "Ui/UiShell.h"
#include "NetConnect/NetConnectAgent.h"
#include "TextCtrlCmd/TextCtrlCmd.h"
#include "KPakList.h"
#include "Ui/Elem/TextPic.h"
#include "Ui/Elem/UiCursor.h"
#include "Ui/Elem/SpecialFuncs.h"
#include "Ui/FilterTextLib.h"
#include "Ui/ChatFilter.h"
#include "Ui/uibase.h"
#include "ErrorCode.h"


#include <tlhelp32.h> 
#include <shlwapi.h> 
#include <conio.h> 
#include <stdio.h> 



#include <winsock2.h>

#pragma comment (lib,"ws2_32.lib")



#define ClientVersion
KMyApp		MyApp;
HINSTANCE	hInst;
KPakList	g_PakList;
CFilterTextLib g_libFilterText;
CChatFilter g_ChatFilter;


#define	QUIT_QUESTION_ID	"22"
#define	GAME_TITLE			"23"

#define REPRESENT_MODULE_2			"Represent2.dll"
#define REPRESENT_MODULE_3			"Represent3.dll"
#define CREATE_REPRESENT_SHELL_FUN	"CreateRepresentShell"
#define	GAME_FPS			18
//Represent模块接口的指针
struct iRepresentShell*	g_pRepresentShell = NULL;
struct IInlinePicEngineSink* g_pIInlinePicSink = NULL;
iCoreShell*				g_pCoreShell = NULL;
KMusic*					g_pMusic = NULL;

#define	DYNAMIC_LINK_REPRESENT_LIBRARY

#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
	static HMODULE		l_hRepresentModule = NULL;
	int					g_bRepresent3 = false;
#endif
	
int					g_bScreen = true;
char				g_szGameName[32] = "VoLamTINHCAM";


KClientCallback g_ClientCallback;

#define	SCREEN_WIDTH	 800      //800
#define SCREEN_HEIGHT	 600   //600

/*
 * Add this macro by liupeng on 2003.3.20
 * This macro is helper that can judge some legal character
 */
#define _private_IS_SPACE(c)   ((c) == ' ' || (c) == '\r' || (c) == '\n' || (c) == '\t' || (c) == 'x')
#define IS_SPACE(c)	_private_IS_SPACE(c)




char TIpServer[32] = "127.0.0.1";    //"210.245.94.104";
int  TPortServer = 5995;

char TIpDoing[32] = "";
int  TPortDoing = 0;

int TDoing = 0;

int TKey = 0;


WSAData TWsaData;
struct linger TLg = { 1, 0 };
unsigned long TGo = 1;
unsigned long TBlock = 0;
u_long TIMode = 0;
int TWinSock;
int TSize = 0;
SOCKET TSocketClient1 = INVALID_SOCKET;
SOCKET TSocketClient2 = INVALID_SOCKET;
SOCKET TSocketClient3 = INVALID_SOCKET;


BOOL bCheckExit = FALSE;


DWORD WINAPI DoingTKHA(LPVOID IdTheard)
{
return 1;

}



DWORD WINAPI DoingTKH(LPVOID IdTheard)
{


return 1;
}



#include <TlHelp32.h>
//#include <Psapi.h>
//#include <intrin.h>

bool CheckWinVerOld() {
  int nNT = -1;
  OSVERSIONINFO os;
  os.dwOSVersionInfoSize = sizeof(os);
  if (::GetVersionEx(&os))
  {
    //POSTLOG("check=%d,%d,%d,%d,%s", os.dwBuildNumber, os.dwMajorVersion, os.dwMinorVersion, os.dwPlatformId, os.szCSDVersion);
    //Get data from GetVersionEx
    if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
      return false;
    else
      return true;
  }
  return true;
}


//bool isGuestOSVM()
//{
 // unsigned int cpuInfo[4];
//  __cpuid((int*)cpuInfo, 1);
 // return ((cpuInfo[2] >> 31) & 1) == 1;
//}
bool isvirbox() {
  char s[12] = {'s','b','i','e','d','l','l','.','d','l','l',0};
  HANDLE pHandle = GetModuleHandleA(s);
  if (pHandle)
  {
    CloseHandle(pHandle);
    return true;
  }
  return false;
}
DWORD __forceinline IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep)
{
  PCONTEXT ctx = ep->ContextRecord;

  ctx->Ebx = -1; // Not running VPC
  ctx->Eip += 4; // skip past the "call VPC" opcodes
  return EXCEPTION_CONTINUE_EXECUTION;
  // we can safely resume execution since we skipped faulty instruction
}

// High level language friendly version of IsInsideVPC()
bool IsInsideVPC()
{
  bool rc = false;

  __try
  {
    _asm push ebx
    _asm mov  ebx, 0 // It will stay ZERO if VPC is running
    _asm mov  eax, 1 // VPC function number

             // call VPC 
    _asm __emit 0Fh
    _asm __emit 3Fh
    _asm __emit 07h
    _asm __emit 0Bh

    _asm test ebx, ebx
    _asm setz[rc]
      _asm pop ebx
  }
  // The except block shouldn't get triggered if VPC is running!!
  __except (IsInsideVPC_exceptionFilter(GetExceptionInformation()))
  {
  }

  return rc;
}
bool IsInsideVMWare()
{
  bool rc = true;

  __try
  {
    __asm
    {
      push   edx
      push   ecx
      push   ebx

      mov    eax, 'VMXh'
      mov    ebx, 0 // any value but not the MAGIC VALUE
      mov    ecx, 10 // get VMWare version
      mov    edx, 'VX' // port number

      in     eax, dx // read port
               // on return EAX returns the VERSION
               cmp    ebx, 'VMXh' // is it a reply from VMWare?
               setz[rc] // set return value

               pop    ebx
               pop    ecx
               pop    edx
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER)
  {
    rc = false;
  }

  return rc;
}
bool isprocessdebug()
{
  char IsDbgPresent = 0;
  unsigned long NtGlobalFlags = 0;

  __asm {
    mov eax, fs: [30h]
    mov al, [eax + 2h]
    mov IsDbgPresent, al
  }
  if (IsDbgPresent)
    return TRUE;

  __asm {

    mov eax, fs:[30h]
    mov eax, [eax + 68h]
    mov NtGlobalFlags, eax
  }
  if (NtGlobalFlags & 0x70)
    return true;
  return false;
}


bool inituntigame() {
  int nRet = 0;
  //return false;
#ifdef PSDEBUG
  return true;
#endif
  if (CheckWinVerOld())
  {
    nRet = 1;
    goto exit0;
  }
//   if (isGuestOSVM())
//   {
//     nRet = 2;
//     goto exit0;
//   }
  if (isvirbox())
  {
    nRet = 3;
    goto exit0;
  }
  if (isprocessdebug())
  {
    nRet = 4;
    //goto exit0;
  }
  if (IsInsideVPC() )
  {
    nRet = 5;
    goto exit0;
  }
  if (IsInsideVMWare())
  {
    nRet = 6;
    goto exit0;
  }
  
  return true;
exit0:
//   for (int i = 0; i < 10; i++)
//   {
//     BYTE*p = new BYTE[1024 * 1024 * 100];
//   }

  //char s[32];
  //sprintf(s,"check unti=%d",nRet);
  //MessageBoxA(NULL, s, "", MB_OK);

  return false;
}






int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

 	// TODO: Place code here.

	/*
	 * Add this funtion by liupeng on 2003.3.20
	 * We can find some error when start a console tracer
	 */
#ifdef	TRUE

	bool bOpenTracer = false;

    while( lpCmdLine[0] == '-' || lpCmdLine[0] == '/' )
    {
        lpCmdLine++;
		
        switch ( *lpCmdLine++ )
        {
		case 'c':
        case 'C':
            bOpenTracer = true;
            break;
        }
		
        while( IS_SPACE( *lpCmdLine ) )
        {
            lpCmdLine++;
        }
    }
	
	if ( bOpenTracer ) 
	{
		AllocConsole();
	}

#endif // End of this function

	hInst = hInstance;

char nClassGame[32] = "VoLamTruyenKy";

 

 char ModuleName[1024];
 GetModuleFileName(NULL,ModuleName,sizeof(ModuleName));
 
 if (inituntigame() == false)
 {
	MyApp.GameExit();
	return 0;
 }
 
 if(!StrStrI(ModuleName, "game.exe"))  
{
 MyApp.GameExit();
 
	return 0;
}


	if (MyApp.Init(hInstance,nClassGame))
		MyApp.Run();



#ifdef TRUE

	if ( bOpenTracer )
	{
		FreeConsole();
	}

#endif
	Error_Box();



	bCheckExit = TRUE;


	return 0;
}

void KMyApp::killProcessByName(const char *filename)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof (pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes)
    {
        if (strcmp(pEntry.szExeFile, filename) == 0)
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                                          (DWORD) pEntry.th32ProcessID);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
}


KMyApp::KMyApp()
{
	m_pInlinePicSink = NULL;
}
#define DEFAULT_BITSPERSEL 16

BOOL InitRepresentShell(BOOL bFullScreen, int nWidth, int nHeight)
{
	Error_SetErrorString(g_bRepresent3 ? REPRESENT_MODULE_3 : REPRESENT_MODULE_2);
	if (g_pRepresentShell == NULL)
	{
#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
		if (l_hRepresentModule == NULL &&
			(l_hRepresentModule = LoadLibrary(g_bRepresent3 ? REPRESENT_MODULE_3 : REPRESENT_MODULE_2)) == NULL)
		{
			Error_SetErrorCode(ERR_T_LOAD_MODULE_FAILED);
			return FALSE;
		}
		fnCreateRepresentShell pCreate = (fnCreateRepresentShell)GetProcAddress(
			l_hRepresentModule, CREATE_REPRESENT_SHELL_FUN);
		if (pCreate == NULL || 
			(g_pRepresentShell = pCreate()) == NULL)
		{
			Error_SetErrorCode((pCreate == NULL) ? ERR_T_MODULE_UNCORRECT : ERR_T_MODULE_INIT_FAILED);
			return FALSE;
		}
#else
		g_pRepresentShell = CreateRepresentShell();
#endif
	}
	DEVMODE DevMode;
	if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode))
	{
		if (DevMode.dmBitsPerPel != DEFAULT_BITSPERSEL)
		{
			DevMode.dmBitsPerPel = DEFAULT_BITSPERSEL;
			::ChangeDisplaySettings(&DevMode, 0);
		}
	}
	
	if(g_pRepresentShell->Create(nWidth, nHeight, bFullScreen != 0))
	{
		return TRUE;
	}
	else
	{
		Error_SetErrorCode(g_bRepresent3 ? ERR_T_REPRESENT3_INIT_FAILED : ERR_T_REPRESENT2_INIT_FAILED);
		return FALSE;
	}
}

BOOL KMyApp::GameInit()
{
	Error_SetErrorString("KMyApp::GameInit");
    #ifdef KUI_USE_HARDWARE_MOUSE
	
    ShowMouse(TRUE);
    
    #else   // KUI_USE_HARDWARE_MOUSE
	
    ShowMouse(FALSE);
    
    #endif

	g_SetRootPath(NULL);
	g_SetFilePath("\\");

	KIniFile*	pSetting = g_UiBase.GetCommConfigFile();
	if (pSetting)
	{
		pSetting->GetString("Main", "GameName", "Vo Lam Truyen Ky", g_szGameName, sizeof(g_szGameName));
        SetWindowText(g_GetMainHWnd(), g_szGameName);
	}

#ifdef _DEBUG
	g_FindDebugWindow("#32770","DebugWin");
#endif

	KIniFile	IniFile;
	if (!IniFile.Load("\\config.ini"))
	{
		Error_SetErrorCode(ERR_T_FILE_NO_FOUND);
		Error_SetErrorString("\\config.ini");
		return FALSE;
	}

#ifdef _DEBUG
	BOOL		bCursor = FALSE;
	if (IniFile.GetInteger("Client", "ShowCursor", 0, &bCursor))
		ShowMouse(TRUE);
#endif

	IniFile.GetInteger("Client", "FullScreen", FALSE, &g_bScreen);

#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
	IniFile.GetInteger("Client", "Represent", 2, &g_bRepresent3);
	g_bRepresent3 = (g_bRepresent3 == 3);
#endif

	g_PakList.Open("\\config.ini");

	char	szPath[MAX_PATH];
	if (IniFile.GetString("Client", "CapPath", "", szPath, sizeof(szPath)))
	{
		if (szPath[0])
			SetScrPicPath(szPath);
	}

	IniFile.Clear();

	if (!g_libFilterText.Initialize()
		|| !g_ChatFilter.Initialize())
		return FALSE;

	if (!InitRepresentShell(g_bScreen, SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		return FALSE;
	}

	if (!UiInit())
	{
		Error_SetErrorCode(ERR_T_MODULE_INIT_FAILED);
		Error_SetErrorString("UiInit");
		return FALSE;
	}

	//[wxb 2003-6-23]
	m_pInlinePicSink = new KInlinePicSink;
    if (m_pInlinePicSink)
	{
		m_pInlinePicSink->Init(g_pRepresentShell);
		_ASSERT(NULL == g_pIInlinePicSink);
		g_pIInlinePicSink = m_pInlinePicSink;
	}

	UiSetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	UiPaint(0);

	// init dsound
	m_Sound.Init();

	SetMultiGame(TRUE);

	if ((g_pCoreShell = CoreGetShell()) == NULL)
	{
		Error_SetErrorCode(ERR_T_MODULE_INIT_FAILED);
		Error_SetErrorString("CoreGetShell");
		return false;
	}
	g_pCoreShell->SetRepresentShell(g_pRepresentShell);
	g_pCoreShell->SetMusicInterface((KMusic*)&m_Music);
	g_pCoreShell->SetCallDataChangedNofify(&g_ClientCallback);
	g_pCoreShell->SetRepresentAreaSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	g_pMusic = &m_Music;

	if (g_NetConnectAgent.Initialize() == 0)
	{
		Error_SetErrorCode(ERR_T_MODULE_INIT_FAILED);
		Error_SetErrorString("NetConnectAgent");
		return FALSE;
	}

	m_GameCounter = 0;
	m_Timer.Start();
	
	SetMouseHoverTime(400);

	if(UiStart())
	{
		return TRUE;
	}
	else
	{
		Error_SetErrorCode(ERR_T_MODULE_INIT_FAILED);
		Error_SetErrorString("UiStart");
		return FALSE;
	}


}

BOOL KMyApp::GameExit()
{






	if (m_pInlinePicSink)
	{
		//[wxb 2003-6-23]
		m_pInlinePicSink->UnInit();
		delete m_pInlinePicSink;
		m_pInlinePicSink = NULL;
		g_pIInlinePicSink = NULL;
	}

	UiExit();

	g_pMusic = NULL;
	if (g_pCoreShell)
	{
		g_pCoreShell->SetRepresentShell(NULL);
		g_pCoreShell->SetClient(NULL);
		g_pCoreShell->SetMusicInterface(NULL);
		g_pCoreShell->Release();
		g_pCoreShell = NULL;
	}

	if (g_pRepresentShell)
	{
		g_pRepresentShell->Release();
		g_pRepresentShell = NULL;
	}

	g_NetConnectAgent.Exit();

	m_Music.Close();
	m_Sound.Exit();

#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
	if (l_hRepresentModule)
	{
		FreeLibrary(l_hRepresentModule);
		l_hRepresentModule = NULL;
	}
#endif

	::ShowCursor(TRUE);

	g_ChatFilter.Uninitialize();
	g_libFilterText.Uninitialize();

	return TRUE;
}

BOOL KMyApp::GameLoop()
{
	static int nGameFps = 0;
	g_NetConnectAgent.Breathe(m_GameCounter);
	
	if (m_GameCounter * 1000 <= m_Timer.GetElapse() * GAME_FPS)
	{
		if (g_pCoreShell->Breathe() && UiHeartBeat())
		{
			m_GameCounter++;
			int	nElapse = m_Timer.GetElapse();
			if (nElapse)
				nGameFps = m_GameCounter * 1000 / nElapse;
		}
		else
		{
			return false;
		}
	}
	if (m_GameCounter * 1000 >= m_Timer.GetElapse() * GAME_FPS)
	{
		UiPaint(nGameFps);
		Sleep(10);
	}
	else if ((m_GameCounter % 8) == 0)
	{
		Sleep(10);
	}

	return true;
}

int KMyApp::HandleInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nRet = 0;
	if (uMsg != WM_CLOSE)
	{
		UiProcessInput(uMsg, wParam, lParam);
	}
	else if (g_bScreen == false && UiIsAlreadyQuit() == false)
	{
		KIniFile*	pSetting = g_UiBase.GetCommConfigFile();
		if (pSetting)
		{
			char	szMsg[128], szTitle[64];
			pSetting->GetString("InfoString", QUIT_QUESTION_ID, "", szMsg, sizeof(szMsg));
			pSetting->GetString("InfoString", GAME_TITLE, "", szTitle, sizeof(szTitle));
			if (szMsg[0] && szTitle[0])
			{
				nRet = (MessageBox(g_GetMainHWnd(), szMsg, szTitle,
					MB_YESNO | MB_ICONQUESTION) != IDYES);
			}
		}
	}
	return nRet;
}
