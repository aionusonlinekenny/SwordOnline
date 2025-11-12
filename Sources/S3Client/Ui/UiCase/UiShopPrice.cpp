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
#include "UiShopPrice.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiShopPrice.ini"

KUiShopPrice* KUiShopPrice::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiShopPrice* KUiShopPrice::OpenWindow(KUiObjAtContRegion* pObj,
			KUiItemBuySelInfo* pPriceInfo)
{
	if (pObj == NULL || pPriceInfo == NULL)
		return NULL;
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiShopPrice;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->m_ItemInfo = *pObj;
		m_pSelf->m_PriceInfo = *pPriceInfo;
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

KUiShopPrice::KUiShopPrice()
{
	m_PriceInfo.szItemName[0] = 0;
	m_PriceInfo.nPrice = 0;
	m_ItemInfo.Obj.uGenre = CGOG_NOTHING;
}

//如果窗口正被显示，则返回实例指针
KUiShopPrice* KUiShopPrice::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	功能：关闭销毁窗口
//--------------------------------------------------------------------------
void KUiShopPrice::CloseWindow(bool bDestroy)
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
void KUiShopPrice::Show()
{
	m_ItemName.SetText(m_PriceInfo.szItemName);
	m_ItemPrice.SetText("0");
	m_ItemPrice.SetIntText(m_PriceInfo.nPrice);

	m_PriceType.CheckButton(true);

	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetExclusive((KWndWindow*)this);
}

//隐藏窗口
void KUiShopPrice::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
int KUiShopPrice::Initialize()
{
	AddChild(&m_ItemName);
	AddChild(&m_ItemPrice);
	
	AddChild(&m_PriceType);

	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);

	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	return true;
}

//--------------------------------------------------------------------------
//	功能：载入窗口的界面方案
//--------------------------------------------------------------------------
void KUiShopPrice::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		m_ItemName.Init(&Ini, "ItemName");
		m_ItemPrice.Init(&Ini, "ItemPrice");

		m_PriceType.Init(&Ini, "Remember");

		m_OkBtn.Init(&Ini, "OkBtn");
		m_CancelBtn.Init(&Ini, "CancelBtn");
	}
}

//--------------------------------------------------------------------------
//	功能：窗口消息函数
//--------------------------------------------------------------------------
int KUiShopPrice::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
		{

		int nPrice = m_ItemPrice.GetIntNumber();
		if (nPrice < 0)
			nPrice = 0;
		if (nPrice > 999999999)
			nPrice = 999999999;

		if (!m_PriceType.IsButtonChecked())// tien xu
		{
			if (nPrice > 0) nPrice += 1000000000;
		}

		g_pCoreShell->OperationRequest(GOI_SHOP_PRICE,
			(unsigned int)(&m_ItemInfo),nPrice);

		OnCancel();
			
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
		{

		int nPrice = m_ItemPrice.GetIntNumber();

			if (nPrice)
			{
			g_pCoreShell->OperationRequest(GOI_SHOP_PRICE,
			(unsigned int)(&m_ItemInfo),0);
			}


			OnCancel();
		}
		

		break;

	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
	
		int nPrice = m_ItemPrice.GetIntNumber();
		if (nPrice < 0)
			nPrice = 0;
		if (nPrice > 999999999)
			nPrice = 999999999;

		if (!m_PriceType.IsButtonChecked())// tien xu
		{
			if (nPrice > 0) nPrice += 1000000000;
		}

		g_pCoreShell->OperationRequest(GOI_SHOP_PRICE,
			(unsigned int)(&m_ItemInfo),nPrice);

		OnCancel();
			
		}
		else if (uParam == VK_ESCAPE)
		{

		int nPrice = m_ItemPrice.GetIntNumber();

			if (nPrice)
			{
			g_pCoreShell->OperationRequest(GOI_SHOP_PRICE,
			(unsigned int)(&m_ItemInfo),0);
			}


			OnCancel();
		}
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

void KUiShopPrice::OnCancel()
{
	CloseWindow(false);
}
