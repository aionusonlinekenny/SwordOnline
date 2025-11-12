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
#include "UiStaskItem.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiStackItem.ini"

KUiStaskItem* KUiStaskItem::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiStaskItem* KUiStaskItem::OpenWindow(KUiObjAtContRegion* pObj,
			KUiItemBuySelInfo* pPriceInfo)
{
	if (pObj == NULL || pPriceInfo == NULL)
		return NULL;
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiStaskItem;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		
		m_pSelf->m_TextImg.Hide();
		
			m_pSelf->m_TextImg.Show();
			m_pSelf->m_OkBtn.Enable(true);
		
		
		m_pSelf->m_ItemInfo = *pObj;
		m_pSelf->m_PriceInfo = *pPriceInfo;
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

KUiStaskItem::KUiStaskItem()
{
	m_PriceInfo.szItemName[0] = 0;
	m_PriceInfo.nPrice = 0;
	m_ItemInfo.Obj.uGenre = CGOG_NOTHING;
}

//如果窗口正被显示，则返回实例指针
KUiStaskItem* KUiStaskItem::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	功能：关闭销毁窗口
//--------------------------------------------------------------------------
void KUiStaskItem::CloseWindow(bool bDestroy)
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


//显示窗口
void KUiStaskItem::Show()
{
	m_BoxName.SetText("T竎h");
	m_ItemName.SetText(m_PriceInfo.szItemName);
	int van = m_PriceInfo.nPrice/10000;
	int luong = m_PriceInfo.nPrice%10000;
	m_nBuyNumber = 1;
	m_BuyNumber.SetIntText(m_nBuyNumber);
	
	

	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetExclusive((KWndWindow*)this);
}

//隐藏窗口
void KUiStaskItem::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
int KUiStaskItem::Initialize()
{
	AddChild(&m_BoxName);
	AddChild(&m_ItemName);
	AddChild(&m_TextImg);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	AddChild(&m_BuyNumber);
	AddChild(&m_NextNumber);
	AddChild(&m_PrevNumber);
	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	return true;
}

//--------------------------------------------------------------------------
//	功能：载入窗口的界面方案
//--------------------------------------------------------------------------
void KUiStaskItem::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		m_BoxName.Init(&Ini, "BoxName");
		m_ItemName.Init(&Ini, "ItemName");
		m_BuyNumber.Init(&Ini, "BuyNumber");

					
		Ini.GetString("Price", "Color", "", Buff, sizeof(Buff));
		m_uNormalPriceColor = GetColor(Buff);
		Ini.GetString("Price", "CantBuyColor", "", Buff, sizeof(Buff));
		m_uNotEnoughMoneyPriceColor = GetColor(Buff);

		m_TextImg.Init(&Ini, "TextImg");
		m_OkBtn.Init(&Ini, "OkBtn");
		m_CancelBtn.Init(&Ini, "CancelBtn");
		m_NextNumber.Init(&Ini, "Increase");
		m_PrevNumber.Init(&Ini, "Decrease");


	
	}
}

//--------------------------------------------------------------------------
//	功能：窗口消息函数
//--------------------------------------------------------------------------
int KUiStaskItem::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
		{	
			OnOk();		
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnCancel();
		else if (uParam == (unsigned int)(KWndWindow*)&m_NextNumber)
		{

		m_nBuyNumber = m_BuyNumber.GetIntNumber();
		if (m_nBuyNumber < 20)
		{
		m_nBuyNumber++;
		m_BuyNumber.SetIntText(m_nBuyNumber);
		}
		
		}

		else if (uParam == (unsigned int)(KWndWindow*)&m_PrevNumber)
		{

		m_nBuyNumber = m_BuyNumber.GetIntNumber();
		if (m_nBuyNumber > 1)
		{
		m_nBuyNumber--;
		m_BuyNumber.SetIntText(m_nBuyNumber);
		}
		
		}

		break;
	case WND_N_EDIT_CHANGE:
		UpdateNumber();
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
void KUiStaskItem::OnOk()
{ 
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_STASK_ITEM,
			(unsigned int)(&m_ItemInfo), m_nBuyNumber);
	}
	CloseWindow(false);
}


void KUiStaskItem::UpdateNumber()
{
    m_nBuyNumber = m_BuyNumber.GetIntNumber();

	if (m_nBuyNumber < 1)
	{
	m_nBuyNumber = 1;
	m_BuyNumber.SetIntText(m_nBuyNumber);
	}
	else if (m_nBuyNumber > 20)
	{
	m_nBuyNumber = 20;
	m_BuyNumber.SetIntText(m_nBuyNumber);
	}

	;

}

//--------------------------------------------------------------------------
//	功能：响应点击卖按钮
//--------------------------------------------------------------------------



void KUiStaskItem::OnCancel()
{
	CloseWindow(false);
}
