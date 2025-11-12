#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "UiFindPos.h"
#include "UiItem.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include <crtdbg.h>

extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI_ITEM	"UiFindPos.ini"

KUiFindPos* KUiFindPos::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	Kiem tra xem hop thoai co dang visible hay khong, neu visible thi tra ve con tro hop thoai
//--------------------------------------------------------------------------
KUiFindPos* KUiFindPos::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	Mo hop thoai, tra ve con tro hop thoai
//--------------------------------------------------------------------------
KUiFindPos* KUiFindPos::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiFindPos;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);

		m_pSelf->BringToTop();
		m_pSelf->Show();
		m_pSelf->UpdateData1();
		Wnd_GameSpaceHandleInput(false);

	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
// Dong hop thoai 
//--------------------------------------------------------------------------
void KUiFindPos::CloseWindow()
{
	if (m_pSelf)
	{
		Wnd_GameSpaceHandleInput(true);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

// -------------------------------------------------------------------------
// Khoi tao hop thoai
// -------------------------------------------------------------------------
void KUiFindPos::SetPositionOpen(int nX,int nY)
{
	if (m_pSelf)
	{
		m_pSelf->SetPosition(nX,nY);
		
	}
}

void KUiFindPos::Initialize()
{
	AddChild(&m_PosXInput);
	AddChild(&m_PosYInput);
	AddChild(&m_InfoText);
	AddChild(&m_Text);
	AddChild(&m_OKBtn);
	AddChild(&m_CancelBtn);
	
	char schemePath[256];
	g_UiBase.GetCurSchemePath(schemePath, 256);
	LoadScheme(schemePath);

	Wnd_AddWindow(this);
}

// -------------------------------------------------------------------------
// Tao layout hop thoai
// -------------------------------------------------------------------------
void KUiFindPos::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_ITEM);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_PosXInput.Init(&Ini, "PosXInput");
		m_pSelf->m_PosYInput.Init(&Ini, "PosYInput");
		m_pSelf->m_Text.Init(&Ini, "Text");
		m_pSelf->m_InfoText.Init(&Ini, "InfoText");
		m_pSelf->m_OKBtn.Init(&Ini, "OkBtn");
		m_pSelf->m_CancelBtn.Init(&Ini, "CancelBtn");
		m_pSelf->m_Text.SetText("/");
		m_pSelf->m_PosXInput.SetText("200");
		m_pSelf->m_PosYInput.SetText("200");
	}
}

// -------------------------------------------------------------------------------
// Gui goi tin yeu cau tim toa do
// -------------------------------------------------------------------------------
void KUiFindPos::OnOK()
{
		int PosX = m_PosXInput.GetIntNumber();
		int PosY = m_PosYInput.GetIntNumber();
		// g_pCoreShell->SceneMapOperation(GSMOI_FIND_POS, PosX, PosY);
		g_pCoreShell->SetPaintFindPos(TRUE);
		g_pCoreShell->SceneMapFindPosOperation(GSMOI_PAINT_SCENE_FIND_POS, PosX, PosY, false, true);
		CloseWindow();
}

// -------------------------------------------------------------------------
// Xu ly khi hop thoai dang mo
// -------------------------------------------------------------------------
int KUiFindPos::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
		{
			// Click len nut cancel ~> dong hop thoai
			CloseWindow();
			nRet = 1;
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OKBtn)
		{
			// Click len nut OK ~> Gui goi tin tim toa do
			OnOK();
			nRet = 1;
		}
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			OnOK();
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			CloseWindow();
			nRet = 1;
		}
		break;
	case WND_N_EDIT_SPECIAL_KEY_DOWN:
		if (nParam == VK_RETURN &&
			uParam == (unsigned int)(KWndWindow*)&m_PosXInput &&
			uParam == (unsigned int)(KWndWindow*)&m_PosYInput)
		{
			OnOK();
			nRet = 1;
		}
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	default:
		nRet =  KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}


// --------------------------------------------------------------------------------
// Xu ly khi nguoi dung nhap toa do
// --------------------------------------------------------------------------------
void KUiFindPos::OnCheckInput()
{
	int nPass = 0;
	char	szBuff1[16], szBuff2[16];
	// toa do x
	nPass = 0;
	nPass = m_PosXInput.GetIntNumber();
	if (nPass < 0)
		nPass = 0;
	while (nPass > CHEST_PW_MAX_VALUE)
		nPass = nPass / 10;	
	itoa(nPass, szBuff1, 10);
	m_PosXInput.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_PosXInput.SetIntText(nPass);
		
	// toa do y
	nPass = 0;
	nPass = m_PosYInput.GetIntNumber();
	if (nPass < 0)
		nPass = 0;
	while (nPass > CHEST_PW_MAX_VALUE)
		nPass = nPass / 10;	
	itoa(nPass, szBuff1, 10);
	m_PosYInput.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_PosYInput.SetIntText(nPass);
}

void KUiFindPos::UpdateData1()
{
	m_InfoText.SetText("NhËp täa ®é");
}

void KUiFindPos::Breathe()
{
	UpdateData();
}