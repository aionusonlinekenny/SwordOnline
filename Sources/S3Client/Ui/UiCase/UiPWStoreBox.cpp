// -------------------------------------------------------------------------
//	ÎÄ¼þÃû		£º	½»Ò×È·ÈÏ¿ò
//	´´½¨Õß		£º	Wooy(Wu yue)
//	´´½¨Ê±¼ä	£º	2003-5-28
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiPWStoreBox.h"
#include "UiInformation.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	PW_STOREBOX_SCHEME 	"UiPasswordStoreBox.ini"

KUiPWStoreBox* KUiPWStoreBox::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´ò¿ª´°¿Ú£¬·µ»ØÎ¨Ò»µÄÒ»¸öÀà¶ÔÏóÊµÀý
//--------------------------------------------------------------------------
KUiPWStoreBox* KUiPWStoreBox::OpenWindow()
{


	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiPWStoreBox;
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

KUiPWStoreBox::KUiPWStoreBox()
{

}

//Èç¹û´°¿ÚÕý±»ÏÔÊ¾£¬Ôò·µ»ØÊµÀýÖ¸Õë
KUiPWStoreBox* KUiPWStoreBox::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹Ø±ÕÏú»Ù´°¿Ú
//--------------------------------------------------------------------------
void KUiPWStoreBox::CloseWindow(bool bDestroy)
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


//ÏÔÊ¾´°¿Ú
void KUiPWStoreBox::Show()
{
	m_NumberPW.SetText("");
	m_NumberEdit.SetText("");
	m_NumberEditTrue.SetText("");
	


	KWndImage::Show();


	Wnd_SetExclusive((KWndWindow*)this);
}

//Òþ²Ø´°¿Ú
void KUiPWStoreBox::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º³õÊ¼»¯
//--------------------------------------------------------------------------
void KUiPWStoreBox::Initialize()
{

	AddChild(&m_NumberPW);
	AddChild(&m_NumberEdit);
	AddChild(&m_NumberEditTrue);
	
	AddChild(&m_BtnDone);
	AddChild(&m_BtnCancel);

	

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);


	Wnd_AddWindow(this);
}


/*********************************************************************
* ¹¦ÄÜ£ºÔØÈë½çÃæ·½°¸
**********************************************************************/
void KUiPWStoreBox::LoadScheme(const char* pScheme)
{
	if(m_pSelf)
	{
    	char		Buff[128];
    	KIniFile	Ini;
    	sprintf(Buff, "%s\\%s", pScheme, PW_STOREBOX_SCHEME);
    	if (Ini.Load(Buff))
    	{
		    m_pSelf->Init(&Ini, "Main");
		
			m_pSelf->m_NumberPW.Init(&Ini, "InputPW");
			m_pSelf->m_NumberEdit.Init(&Ini, "InputNumber");
			m_pSelf->m_NumberEditTrue.Init(&Ini, "InputNumberTrue");

			m_pSelf->m_BtnDone.Init(&Ini, "BtnDone");
			m_pSelf->m_BtnCancel.Init(&Ini, "BtnCancel");
		}
		/*--------------------*/
	}
	/*--------------------*/
}


//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´°¿ÚÏûÏ¢º¯Êý
//--------------------------------------------------------------------------
int KUiPWStoreBox::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
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
//	¹¦ÄÜ£ºÏìÓ¦µã»÷Âò°´Å¥
//--------------------------------------------------------------------------
void KUiPWStoreBox::OnOk()
{ 


	char Buff[32];
	char BuffNew[32];
	char BuffNewTrue[32];

	m_NumberPW.GetText(Buff, sizeof(Buff), TRUE);
	m_NumberEdit.GetText(BuffNew, sizeof(BuffNew), TRUE);
	m_NumberEditTrue.GetText(BuffNewTrue, sizeof(BuffNewTrue), TRUE);

	if (g_pCoreShell)
	{

	   if (!Buff[0] || !BuffNew[0] || !BuffNewTrue[0] || strlen(Buff) != 6 || strlen(BuffNew) != 6 || strlen(BuffNewTrue) != 6)
	   {
			UIMessageBox("M· r­¬ng yªu cÇu 6 ch÷ sè !",this,"Tho¸t");
			return;
		}

	   else if (strcmp(BuffNew,BuffNewTrue) != 0)
	   {

		   UIMessageBox("M· r­¬ng nhËp l¹i kh«ng khíp !",this,"Tho¸t");
		   return;

	   }

	   else
	   {
			if (!g_pCoreShell->GetGameData(GDI_PLAYER_BLOCK_BOX, 0, 0))
			{
		    g_pCoreShell->OperationRequest(GOI_PW_STORE_BOX,(unsigned int)(&Buff), (int)(&BuffNew));
			}
	   }
	}

	


	CloseWindow(false);
}




void KUiPWStoreBox::OnCancel()
{
	CloseWindow(false);
}
