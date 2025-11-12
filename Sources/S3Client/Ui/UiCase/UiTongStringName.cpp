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

#include "UiTongStringName.h"


#define TONG_STRING_NAME_SCHEME "UiTongStringName.ini"

KUiTongStringName* KUiTongStringName::ms_pSelf = NULL;


extern iCoreShell *g_pCoreShell;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KUiTongStringName::KUiTongStringName()
{
	m_pMain = NULL;
	m_nSelectTong = 0;
	m_nStringTong[0] = 0;
}

KUiTongStringName::~KUiTongStringName()
{

}


/*********************************************************************
* 功能：打开窗口
**********************************************************************/
KUiTongStringName* KUiTongStringName::OpenWindow(int nSelect,char *nString)
{
	if (ms_pSelf == NULL)
	{
		ms_pSelf = new KUiTongStringName;
		if (ms_pSelf)
			ms_pSelf->Initialize();
	}
	if (ms_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		ms_pSelf->BringToTop();
		ms_pSelf->Show();
		ms_pSelf->m_nSelectTong = nSelect;
		strcpy(ms_pSelf->m_nStringTong,nString);

		if (ms_pSelf->m_nSelectTong == 4 || ms_pSelf->m_nSelectTong == 5)
		{
		ms_pSelf->m_StringBoxName.SetText(nString);
		ms_pSelf->m_EditTongName.Hide();
		ms_pSelf->m_EditTongNumber.Show();
		}
		else
		{
		ms_pSelf->m_EditTongName.Show();
		ms_pSelf->m_EditTongNumber.Hide();
		}


	}

	return ms_pSelf;
}


/*********************************************************************
* 功能：如果窗口正被显示，则返回实例指针
**********************************************************************/
KUiTongStringName* KUiTongStringName::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
		return ms_pSelf;
	return NULL;
}


/*********************************************************************
* 功能：关闭窗口，同时可以选则是否删除对象实例
**********************************************************************/
void KUiTongStringName::CloseWindow(bool bDestory)
{
	if (ms_pSelf)
	{
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
void KUiTongStringName::Initialize()
{
	AddChild(&m_EditTongName);

	AddChild(&m_StringBoxName);
	AddChild(&m_EditTongNumber);
	
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
void KUiTongStringName::LoadScheme(const char* pScheme)
{
	if(ms_pSelf)
	{
    	char		Buff[128];
    	KIniFile	Ini;
    	sprintf(Buff, "%s\\%s", pScheme, TONG_STRING_NAME_SCHEME);
    	if (Ini.Load(Buff))
    	{
		    ms_pSelf->Init(&Ini, "Main");
			ms_pSelf->m_EditTongName.Init(&Ini, "Input");


			ms_pSelf->m_StringBoxName.Init(&Ini, "BoxName");
	        ms_pSelf->m_EditTongNumber.Init(&Ini, "InputNumber");



			ms_pSelf->m_BtnDone.Init(&Ini, "BtnDone");
			ms_pSelf->m_BtnCancel.Init(&Ini, "BtnCancel");
		}
		/*--------------------*/
	}
	/*--------------------*/
}


/*********************************************************************
* 功能：窗口函数
**********************************************************************/
int KUiTongStringName::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
	
		if(uParam == (unsigned int)&m_BtnCancel)
		{
			if(m_pMain)
			{
    			m_pMain->Show();
	    		m_pMain = NULL;
			}
			CloseWindow();
		}
		else if(uParam == (unsigned int)&m_BtnDone)
		{
			if(m_pMain)
			{
    			m_pMain->Show();
	    		m_pMain = NULL;
			}
			OnDone();
		}
		break;

	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
		break;
	}
    return 1;
}



void KUiTongStringName::OnDone()
{
	char Buff[16];

	

	if (m_nSelectTong == 4 || m_nSelectTong == 5)
	{
	m_EditTongNumber.GetText(Buff, sizeof(Buff), TRUE);
	}
	else
	{
	m_EditTongName.GetText(Buff, sizeof(Buff), TRUE);
	}


	if(Buff[0] && g_pCoreShell)
	{
		if(m_nSelectTong == 1)
		{
				KTongOperationParam Param;
    			KTongMemberItem Member;
				memset(&Param, 0, sizeof(KTongOperationParam));
			    Param.eOper = TONG_ACTION_NAME_FEMALE;
				strcpy(Param.szPassword,Buff);
			    memset(&Member, 0, sizeof(KTongMemberItem));  
 			    g_pCoreShell->TongOperation(GTOI_TONG_ACTION, (unsigned int)&Param, (int)&Member);

		}
		else if(m_nSelectTong == 2)
		{
				KTongOperationParam Param;
    			KTongMemberItem Member;
				memset(&Param, 0, sizeof(KTongOperationParam));
			    Param.eOper = TONG_ACTION_NAME_MALE;
				strcpy(Param.szPassword,Buff);
			    memset(&Member, 0, sizeof(KTongMemberItem));  
 			    g_pCoreShell->TongOperation(GTOI_TONG_ACTION, (unsigned int)&Param, (int)&Member);


		}
		else if(m_nSelectTong == 3)
		{
			if (m_nStringTong[0])
			{
				KTongOperationParam Param;
    			KTongMemberItem Member;
				memset(&Param, 0, sizeof(KTongOperationParam));
			    Param.eOper = TONG_ACTION_NAME;
				strcpy(Param.szPassword,Buff);
			    memset(&Member, 0, sizeof(KTongMemberItem));
				strcpy(Member.Name,m_nStringTong);

 			    g_pCoreShell->TongOperation(GTOI_TONG_ACTION, (unsigned int)&Param, (int)&Member);
			}
		}

		else if(m_nSelectTong == 4)
		{
			if (m_nStringTong[0])
			{
				KTongOperationParam Param;
    			KTongMemberItem Member;
				memset(&Param, 0, sizeof(KTongOperationParam));
			    Param.eOper = TONG_ACTION_DEPOSIT_MONEY;
				strcpy(Param.szPassword,Buff);
			    memset(&Member, 0, sizeof(KTongMemberItem));
				strcpy(Member.Name,m_nStringTong);
 			    g_pCoreShell->TongOperation(GTOI_TONG_ACTION, (unsigned int)&Param, (int)&Member);

			}
		}

		else if(m_nSelectTong == 5)
		{
			if (m_nStringTong[0])
			{
				KTongOperationParam Param;
    			KTongMemberItem Member;
				memset(&Param, 0, sizeof(KTongOperationParam));
			    Param.eOper = TONG_ACTION_TAKE_MONEY;
				strcpy(Param.szPassword,Buff);
			    memset(&Member, 0, sizeof(KTongMemberItem));
				strcpy(Member.Name,m_nStringTong);
 			    g_pCoreShell->TongOperation(GTOI_TONG_ACTION, (unsigned int)&Param, (int)&Member);
			}
		}







		 
	}

	CloseWindow();
	
}


void KUiTongStringName::LinkToMainWindow(KWndWindow *pMain)
{
	if(pMain && ms_pSelf)
	{
		ms_pSelf->m_pMain = pMain;
		ms_pSelf->m_pMain->Hide();
	}
}