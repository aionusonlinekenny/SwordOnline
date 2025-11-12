/********************************************************************
File        : UiTongCreateSheet.cpp
Creator     : Fyt(Fan Zhanpeng)
create data : 08-29-2003(mm-dd-yyyy)
Description : 创建帮会的表单
*********************************************************************/

#include "KWin32.h"
#include "KIniFile.h"
#include "CoreShell.h"
#include "GameDataDef.h"

#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"

#include "../UiBase.h"
#include "../UiSoundSetting.h"

#include "UiTongChangeCamp.h"


#define TONG_CREATE_SHEET_SCHEME "UiTongChangeCamp.ini"

KUiTongChangeCamp* KUiTongChangeCamp::ms_pSelf = NULL;


extern iCoreShell *g_pCoreShell;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KUiTongChangeCamp::KUiTongChangeCamp()
{
	m_nSelectSide = -1;
}

KUiTongChangeCamp::~KUiTongChangeCamp()
{

}


/*********************************************************************
* 功能：打开窗口
**********************************************************************/
KUiTongChangeCamp* KUiTongChangeCamp::OpenWindow(char* String)
{
	if (ms_pSelf == NULL)
	{
		ms_pSelf = new KUiTongChangeCamp;
		if (ms_pSelf)
			ms_pSelf->Initialize();
	}
	if (ms_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		ms_pSelf->BringToTop();
		ms_pSelf->Show();
		ms_pSelf->m_EditTongName.SetText(String);
		Wnd_SetExclusive((KWndWindow*)ms_pSelf);
	}
	return ms_pSelf;
}


/*********************************************************************
* 功能：如果窗口正被显示，则返回实例指针
**********************************************************************/
KUiTongChangeCamp* KUiTongChangeCamp::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
		return ms_pSelf;
	return NULL;
}


/*********************************************************************
* 功能：关闭窗口，同时可以选则是否删除对象实例
**********************************************************************/
void KUiTongChangeCamp::CloseWindow(bool bDestory)
{
	if (ms_pSelf)
	{
		Wnd_ReleaseExclusive((KWndWindow*)ms_pSelf);
		ms_pSelf->Hide();
		if (bDestory)
		{
			ms_pSelf->Destroy();
			ms_pSelf = NULL;
		}
	}
}


/*********************************************************************
* 功能：初始化
**********************************************************************/
void KUiTongChangeCamp::Initialize()
{
	AddChild(&m_EditTongName);
	AddChild(&m_BtnOrder);
	AddChild(&m_BtnNatural);
	AddChild(&m_BtnChaos);
	AddChild(&m_BtnDone);
	AddChild(&m_BtnCancel);
	AddChild(&m_TextError);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}


/*********************************************************************
* 功能：载入界面方案
**********************************************************************/
void KUiTongChangeCamp::LoadScheme(const char* pScheme)
{
	if(ms_pSelf)
	{
    	char		Buff[128];
    	KIniFile	Ini;
    	sprintf(Buff, "%s\\%s", pScheme, TONG_CREATE_SHEET_SCHEME);
    	if (Ini.Load(Buff))
    	{
		    ms_pSelf->Init(&Ini, "Main");

		    ms_pSelf->m_BtnOrder.Init(&Ini, "BtnOrder");
			ms_pSelf->m_BtnChaos.Init(&Ini, "BtnChaos");
			ms_pSelf->m_BtnNatural.Init(&Ini, "BtnNatural");

			ms_pSelf->m_TextError.Init(&Ini, "ErrorBox");
			ms_pSelf->m_EditTongName.Init(&Ini, "Input");

			ms_pSelf->m_BtnDone.Init(&Ini, "BtnDone");
			ms_pSelf->m_BtnCancel.Init(&Ini, "BtnCancel");

			ms_pSelf->m_TextError.SetText("Phi i m祏: 100 v筺");
		}
		/*--------------------*/
	}
	/*--------------------*/
}


/*********************************************************************
* 功能：窗口函数
**********************************************************************/
int KUiTongChangeCamp::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_BtnOrder )
		{
			AlignmentButtonCheck(camp_justice);
		}
		else if(uParam == (unsigned int)&m_BtnChaos)
		{
			AlignmentButtonCheck( camp_evil  );
		}
		else if(uParam == (unsigned int)&m_BtnNatural  )
		{
			AlignmentButtonCheck(camp_balance);
		}
		/*----------------------------------------------------------*/
		else if(uParam == (unsigned int)&m_BtnCancel)
		{
			CloseWindow();
		}
		else if(uParam == (unsigned int)&m_BtnDone)
		{
			OnDone();
		}
		break;

	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
		break;
	}
    return 1;
}


/*********************************************************************
* 功能：正中邪CheckBox的管理函数
**********************************************************************/
void KUiTongChangeCamp::AlignmentButtonCheck(NPCCAMP eSide)
{
	m_BtnOrder.CheckButton(FALSE);
	m_BtnChaos.CheckButton(FALSE);
	m_BtnNatural.CheckButton(FALSE);

	switch(eSide)
	{
	case camp_justice:
		m_BtnOrder.CheckButton(TRUE);
		break;

	case camp_evil:
		m_BtnChaos.CheckButton(TRUE);
		break;

	case camp_balance:
		m_BtnNatural.CheckButton(TRUE);
		break;
	}
	m_nSelectSide = eSide;
}


/*********************************************************************
* 功能：响应确认按钮被按下
**********************************************************************/
void KUiTongChangeCamp::OnDone()
{
	char Buff[16];

	m_EditTongName.GetText(Buff, sizeof(Buff));
	if (Buff[0] && g_pCoreShell)
	{
		if(m_nSelectSide != -1)
		{
			g_pCoreShell->TongOperation(GTOI_TONG_CHANGE_CAMP, (unsigned int)Buff, (int)m_nSelectSide);
			CloseWindow();
		}
		else
		{
			m_TextError.SetText("Vui l遪g ch鋘 m祏 bang h閕 !");
		}
	}
}
