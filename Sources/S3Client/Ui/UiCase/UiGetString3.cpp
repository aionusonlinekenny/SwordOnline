#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiGetString3.h"
#include "UiInformation.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiGetString3.ini"


KUiGetString3* KUiGetString3::m_pSelf = NULL;
KUiGetString3::KUiGetString3()
{
	m_pRequester = NULL;
	m_nMaxPoint = 0;	
}
//--------------------------------------------------------------------------
// chuc nang: mo cua so
//--------------------------------------------------------------------------
KUiGetString3* KUiGetString3::OpenWindow(int nPoint, int nMaxPoint, const char* pszTitle,	KWndWindow* pRequester, unsigned int uParam)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiGetString3;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_pRequester = pRequester;		
		m_pSelf->m_uRequesterParam = uParam;
		if (nMaxPoint > 0)
			m_pSelf->m_nMaxPoint = nMaxPoint;
		else
			m_pSelf->m_nMaxPoint = 0;		
		strcpy(m_pSelf->szTitle, pszTitle);
		m_pSelf->m_Title.SetText(m_pSelf->szTitle);	
		m_pSelf->m_StringEdit.SetIntText(nPoint);		
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	chuc nang: dong cua so
//--------------------------------------------------------------------------
void KUiGetString3::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->m_pRequester = NULL;
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();
	}
}

// kiem tra 
KUiGetString3*	KUiGetString3::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

// hien thi
void KUiGetString3::Show()
{
	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetFocusWnd(&m_StringEdit);
	Wnd_SetExclusive((KWndWindow*)this);
}

// an
void KUiGetString3::Hide()
{
	Wnd_SetFocusWnd(NULL);
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	chuc nang: khoi tao
//--------------------------------------------------------------------------
void KUiGetString3::Initialize()
{
	AddChild(&m_StringEdit);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	AddChild(&m_Increase);
	AddChild(&m_Decrease);
	AddChild(&m_Title);
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	chuc nang: load
//--------------------------------------------------------------------------
void KUiGetString3::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_StringEdit.Init(&Ini, "StringInput");
			m_pSelf->m_OkBtn.Init(&Ini, "OkBtn");
			m_pSelf->m_CancelBtn.Init(&Ini, "CancelBtn");
			m_pSelf->m_Increase.Init(&Ini, "Increase");
			m_pSelf->m_Decrease.Init(&Ini, "Decrease");	
			m_pSelf->m_Title.Init(&Ini, "Title");			
		}
	}
}

//--------------------------------------------------------------------------
// chuc nang: su kien
//--------------------------------------------------------------------------
int KUiGetString3::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
			OnOk();
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnCancel();
		else if (uParam == (unsigned int)(KWndWindow*)&m_Increase)
			OnIncrease();
		else if (uParam == (unsigned int)(KWndWindow*)&m_Decrease)
			OnDecrease();			
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
			OnOk();
		else if (uParam == VK_ESCAPE)
			OnCancel();
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

//--------------------------------------------------------------------------
//	chuc nang: goi ham di
//--------------------------------------------------------------------------
void KUiGetString3::OnOk()
{
	OnCheckInput();
	int nNumber = m_StringEdit.GetIntNumber();	
	if (m_pRequester)
		m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, WND_OPER_RESULT(nNumber));
	CloseWindow(true);
}

// tang
void KUiGetString3::OnIncrease()
{	
	int nNumber = m_StringEdit.GetIntNumber();
	nNumber++;
	if (nNumber >= m_nMaxPoint)
		nNumber = m_nMaxPoint;	
	m_StringEdit.SetIntText(nNumber);
}

// giam
void KUiGetString3::OnDecrease()
{	
	int nNumber = m_StringEdit.GetIntNumber();
	nNumber--;
	if (nNumber <= 0)
		nNumber = 0;
	m_StringEdit.SetIntText(nNumber);
}

// huy bo
void KUiGetString3::OnCancel()
{
	if (m_pRequester)
		m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, WND_OPER_RESULT(-1));
	CloseWindow(true);
}

// kiem tra
void KUiGetString3::OnCheckInput()
{
	int nPoint = m_StringEdit.GetIntNumber();
	if (nPoint <= 0)
		nPoint = 0;
	else if (nPoint >= m_nMaxPoint)
		nPoint = m_nMaxPoint;
	char	szBuff1[16], szBuff2[16];
	itoa(nPoint, szBuff1, 10);
	m_StringEdit.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_StringEdit.SetIntText(nPoint);
}