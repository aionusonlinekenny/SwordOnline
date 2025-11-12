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
#include "UiLoginPWStoreBox.h"
#include "UiInformation.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	LOGIN_PW_STOREBOX_SCHEME 	"UiLoginPasswordStoreBox.ini"

KUiLoginPWStoreBox* KUiLoginPWStoreBox::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´ò¿ª´°¿Ú£¬·µ»ØÎ¨Ò»µÄÒ»¸öÀà¶ÔÏóÊµÀý
//--------------------------------------------------------------------------
KUiLoginPWStoreBox* KUiLoginPWStoreBox::OpenWindow()
{


	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiLoginPWStoreBox;
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

KUiLoginPWStoreBox::KUiLoginPWStoreBox()
{

}

//Èç¹û´°¿ÚÕý±»ÏÔÊ¾£¬Ôò·µ»ØÊµÀýÖ¸Õë
KUiLoginPWStoreBox* KUiLoginPWStoreBox::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹Ø±ÕÏú»Ù´°¿Ú
//--------------------------------------------------------------------------
void KUiLoginPWStoreBox::CloseWindow(bool bDestroy)
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
void KUiLoginPWStoreBox::Show()
{
	m_NumberEdit.SetText("");
	KWndImage::Show();

	Wnd_SetExclusive((KWndWindow*)this);
}

//Òþ²Ø´°¿Ú
void KUiLoginPWStoreBox::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º³õÊ¼»¯
//--------------------------------------------------------------------------
void KUiLoginPWStoreBox::Initialize()
{


	AddChild(&m_NumberEdit);

	AddChild(&m_Btn1);
	AddChild(&m_Btn2);
	AddChild(&m_Btn3);
	AddChild(&m_Btn4);
	AddChild(&m_Btn5);
	AddChild(&m_Btn6);
	AddChild(&m_Btn7);
	AddChild(&m_Btn8);
	AddChild(&m_Btn9);
	AddChild(&m_Btn0);
	AddChild(&m_Btndc);
	
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
void KUiLoginPWStoreBox::LoadScheme(const char* pScheme)
{
	if(m_pSelf)
	{
    	char		Buff[128];
    	KIniFile	Ini;
    	sprintf(Buff, "%s\\%s", pScheme, LOGIN_PW_STOREBOX_SCHEME);
    	if (Ini.Load(Buff))
    	{
		    m_pSelf->Init(&Ini, "Main");
			
			m_pSelf->m_NumberEdit.Init(&Ini, "InputNumber");

			m_pSelf->m_Btn1.Init(&Ini, "Btn1");
			m_pSelf->m_Btn2.Init(&Ini, "Btn2");
			m_pSelf->m_Btn3.Init(&Ini, "Btn3");
			m_pSelf->m_Btn4.Init(&Ini, "Btn4");
			m_pSelf->m_Btn5.Init(&Ini, "Btn5");
			m_pSelf->m_Btn6.Init(&Ini, "Btn6");
			m_pSelf->m_Btn7.Init(&Ini, "Btn7");
			m_pSelf->m_Btn8.Init(&Ini, "Btn8");
			m_pSelf->m_Btn9.Init(&Ini, "Btn9");
			m_pSelf->m_Btn0.Init(&Ini, "Btn0");

			m_pSelf->m_Btndc.Init(&Ini, "Btndc");

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
int KUiLoginPWStoreBox::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
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



		else if (uParam == (unsigned int)(KWndWindow*)&m_Btn1)
		{
	        m_NumberEdit.InsertString("1", 1);
		}
			else if (uParam == (unsigned int)(KWndWindow*)&m_Btn2)
		{
	        m_NumberEdit.InsertString("2", 1);
		}
			else if (uParam == (unsigned int)(KWndWindow*)&m_Btn3)
		{
	        m_NumberEdit.InsertString("3", 1);
		}
			else if (uParam == (unsigned int)(KWndWindow*)&m_Btn4)
		{
	        m_NumberEdit.InsertString("4", 1);
		}
			else if (uParam == (unsigned int)(KWndWindow*)&m_Btn5)
		{
	        m_NumberEdit.InsertString("5", 1);
		}
			else if (uParam == (unsigned int)(KWndWindow*)&m_Btn6)
		{
	        m_NumberEdit.InsertString("6", 1);
		}
			else if (uParam == (unsigned int)(KWndWindow*)&m_Btn7)
		{
	        m_NumberEdit.InsertString("7", 1);
		}
			else if (uParam == (unsigned int)(KWndWindow*)&m_Btn8)
		{
	        m_NumberEdit.InsertString("8", 1);
		}
			else if (uParam == (unsigned int)(KWndWindow*)&m_Btn9)
		{
	        m_NumberEdit.InsertString("9", 1);
		}
			else if (uParam == (unsigned int)(KWndWindow*)&m_Btn0)
		{
	        m_NumberEdit.InsertString("0", 1);
		}
			else if (uParam == (unsigned int)(KWndWindow*)&m_Btndc)
		{
	        m_NumberEdit.SetText("");
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
void KUiLoginPWStoreBox::OnOk()
{ 
	

	char Buff[32];

	m_NumberEdit.GetText(Buff, sizeof(Buff), TRUE);

	if (g_pCoreShell)
	{

		 if (!Buff[0] || strlen(Buff) != 6)
		 {
			UIMessageBox("M· r­¬ng yªu cÇu 6 ch÷ sè !",this,"Tho¸t");
			return;
		 }
		 else
		 {



			if (g_pCoreShell->GetGameData(GDI_PLAYER_BLOCK_BOX, 0, 0))
			{
		g_pCoreShell->OperationRequest(GOI_LOGIN_PW_STORE_BOX,(unsigned int)(&Buff), 0);
			}



		 }
	}

	CloseWindow(false);
}




void KUiLoginPWStoreBox::OnCancel()
{
	CloseWindow(false);
}
