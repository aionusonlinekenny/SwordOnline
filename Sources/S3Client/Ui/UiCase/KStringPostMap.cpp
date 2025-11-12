// -------------------------------------------------------------------------
//	文件名		：	交易确认框
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-5-28
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "KStringPostMap.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	STRING_POS_MAP_SCHEME 	"UiStringPostMap.ini"

KUiStringPostMap* KUiStringPostMap::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiStringPostMap* KUiStringPostMap::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiStringPostMap;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

KUiStringPostMap::KUiStringPostMap()
{

}

//如果窗口正被显示，则返回实例指针
KUiStringPostMap* KUiStringPostMap::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	功能：关闭销毁窗口
//--------------------------------------------------------------------------
void KUiStringPostMap::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}



void KUiStringPostMap::SetPositionOpen(int nX,int nY)
{
	if (m_pSelf)
	{
		m_pSelf->SetPosition(nX,nY);
		
	}
}

//显示窗口
void KUiStringPostMap::Show()
{
	m_StringEdit.SetText("");
	KWndImage::Show();
	Wnd_SetFocusWnd((KWndWindow*)&m_StringEdit);
	Wnd_SetExclusive((KWndWindow*)this);
}

//隐藏窗口
void KUiStringPostMap::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
void KUiStringPostMap::Initialize()
{

	AddChild(&m_StringBoxName);
	AddChild(&m_IMGEdit);
	AddChild(&m_StringEdit);

	AddChild(&m_BtnDone);
	AddChild(&m_BtnCancel);

	

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);


	Wnd_AddWindow(this);
}


/*********************************************************************
* 功能：载入界面方案
**********************************************************************/
void KUiStringPostMap::LoadScheme(const char* pScheme)
{
	if(m_pSelf)
	{
    	char		Buff[128];
    	KIniFile	Ini;
    	sprintf(Buff, "%s\\%s", pScheme, STRING_POS_MAP_SCHEME);
    	if (Ini.Load(Buff))
    	{
		    m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_StringBoxName.Init(&Ini, "BoxName");
			m_pSelf->m_IMGEdit.Init(&Ini, "IMGInput");
			m_pSelf->m_StringEdit.Init(&Ini, "Input");
			m_pSelf->m_BtnDone.Init(&Ini, "BtnDone");
			m_pSelf->m_BtnCancel.Init(&Ini, "BtnCancel");
		}
		/*--------------------*/
	}
	/*--------------------*/
}


//--------------------------------------------------------------------------
//	功能：窗口消息函数
//--------------------------------------------------------------------------
int KUiStringPostMap::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_BtnDone)
		{	
			OnOk();		
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_BtnCancel)
		{
			OnCancel();
		}
	
		break;
	case WND_N_EDIT_CHANGE:
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			
			OnOk();
		
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	功能：响应点击买按钮
//--------------------------------------------------------------------------
void KUiStringPostMap::OnOk()
{ 

	char szBuff[32];

	m_StringEdit.GetText(szBuff, sizeof(szBuff), TRUE);

	if (szBuff[0] && g_pCoreShell)
	{

	int nPostX = 0,nPostY = 0;
	nPostX = atoi(szBuff);

	int bCheck = FALSE;
	for (int i=0;i<(strlen(szBuff)-1);i++)
	{

	if (szBuff[i] < 48 || szBuff[i] > 57)
	{
	bCheck = TRUE;
	continue;
	}
	

	if (bCheck && szBuff[i] >= 48 && szBuff[i] <= 57)
	{
	nPostY = atoi(&szBuff[i]);
	break;
	}

	}
	


	g_pCoreShell->SceneMapOperation(GSMOI_SCENE_POST_SEARCH_MAP, nPostX * 8,nPostY * 8);

	}





	CloseWindow(false);
}




void KUiStringPostMap::OnCancel()
{
	CloseWindow(false);
}
