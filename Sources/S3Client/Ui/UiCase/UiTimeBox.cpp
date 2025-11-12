#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiTimeBox.h"
#include "UiInformation.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
//#include "../../../Engine/src/KDebug.h"

extern iRepresentShell*	g_pRepresentShell;
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiTimeBox.ini"


KUiTimeBox* KUiTimeBox::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	chuc nang: mo hop thoai chinh
//--------------------------------------------------------------------------
KUiTimeBox* KUiTimeBox::OpenWindow(const char* pszTitle, int nTime, char* pszAction)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTimeBox;
		if (m_pSelf)
		{
			m_pSelf->Initialize();
			m_pSelf->nRunTime = 0;				
		}	
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_nTime = nTime;		
		m_pSelf->m_Title.SetText(pszTitle);
		strcpy(m_pSelf->szAction, pszAction);
		m_pSelf->nRunTime = 0;			
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

void KUiTimeBox::Show()
{
	KWndImage::Show();
	Wnd_SetExclusive((KWndWindow*)this);
}

//--------------------------------------------------------------------------
//	chuc nang: dong hop thoai
//--------------------------------------------------------------------------
void KUiTimeBox::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;		
		}
		else
		{
			m_pSelf->Hide();
		}		
	}
}

// chuc nang: kiem tra hop thoai
KUiTimeBox*	KUiTimeBox::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	else
		return NULL;
}
//--------------------------------------------------------------------------
//	khoi tao
//--------------------------------------------------------------------------
void KUiTimeBox::Initialize()
{
	AddChild(&m_Title);
	AddChild(&m_Image);	
	AddChild(&m_Time);
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	load ngoai ini
//--------------------------------------------------------------------------
void KUiTimeBox::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_Title.Init(&Ini, "Text");
			m_pSelf->m_Time.Init(&Ini, "Time");
			m_pSelf->m_Image.Init(&Ini, "Image");			
		}
	}
}

//--------------------------------------------------------------------------
//	chuc nang: thuc thi
//--------------------------------------------------------------------------
void KUiTimeBox::OnOk()
{
	g_pCoreShell->OperationRequest(GOI_ADD_UI_CMD_SCRIPT, 2, (unsigned int)m_pSelf->szAction);
	CloseWindow(false);
}

//--------------------------------------------------------------------------
// chuc nang: su kien
//--------------------------------------------------------------------------
int KUiTimeBox::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nResult = false;
	switch(uMsg)
	{
		case WM_KEYDOWN:
		{	
			CloseWindow(false);
			nResult = true;
		}		
		break;
		default:
		nResult = KWndWindow::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nResult;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void KUiTimeBox::Breathe()
{
	if (IsVisible())
		nRunTime++;
	if (nRunTime)
	{	
		char Buff[32];
		int nPercent = (nRunTime*100)/(m_nTime*18);
		sprintf(Buff, "%d%s", nPercent, "%");
		m_Image.Set2IntValue(nRunTime, m_nTime*18);	
		m_Time.SetText(Buff);
	}
	if (nRunTime == m_nTime*18)	
		OnOk();
}