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
#include "UiTradeConfirmWnd.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiTradeConfirm.ini"

KUiTradeConfirm* KUiTradeConfirm::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´ò¿ª´°¿Ú£¬·µ»ØÎ¨Ò»µÄÒ»¸öÀà¶ÔÏóÊµÀý
//--------------------------------------------------------------------------
KUiTradeConfirm* KUiTradeConfirm::OpenWindow(KUiObjAtContRegion* pObj,
			KUiItemBuySelInfo* pPriceInfo, TRADE_CONFIRM_ACTION eAction, int nslect)
{
	if (pObj == NULL || pPriceInfo == NULL)
		return NULL;
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTradeConfirm;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->m_BuyImg.Hide();
		m_pSelf->m_SaleImg.Hide();
		m_pSelf->m_RepairImg.Hide();
		if (eAction == TCA_SALE)
		{
			m_pSelf->m_SaleImg.Show();
			m_pSelf->m_OkBtn.Enable(true);
			m_pSelf->m_Money.SetTextColor(m_pSelf->m_uNormalPriceColor);
		}
		else
		{
			if (eAction == TCA_BUY)
				m_pSelf->m_BuyImg.Show();
			else
				m_pSelf->m_RepairImg.Show();


			int nHoldMoney = 0;
			if (nslect == 1)
			{

			nHoldMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_GOLD, 0, 0);

			m_pSelf->m_OkBtn.Enable(nHoldMoney >= pPriceInfo->nPrice);
			m_pSelf->m_Money.SetTextColor((nHoldMoney >= pPriceInfo->nPrice) ?
				m_pSelf->m_uNormalPriceColor : m_pSelf->m_uNotEnoughMoneyPriceColor);
			}

			else if (nslect == 2)
			{

			nHoldMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_TL, 0, 0);

			m_pSelf->m_OkBtn.Enable(nHoldMoney >= pPriceInfo->nPrice);
			m_pSelf->m_Money.SetTextColor((nHoldMoney >= pPriceInfo->nPrice) ?
				m_pSelf->m_uNormalPriceColor : m_pSelf->m_uNotEnoughMoneyPriceColor);
			}



			else if (nslect == 3)
			{

			nHoldMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_VD, 0, 0);

			m_pSelf->m_OkBtn.Enable(nHoldMoney >= pPriceInfo->nPrice);
			m_pSelf->m_Money.SetTextColor((nHoldMoney >= pPriceInfo->nPrice) ?
				m_pSelf->m_uNormalPriceColor : m_pSelf->m_uNotEnoughMoneyPriceColor);
			}

			else

			{

			nHoldMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);

			m_pSelf->m_OkBtn.Enable(nHoldMoney >= pPriceInfo->nPrice);
			m_pSelf->m_Money.SetTextColor((nHoldMoney >= pPriceInfo->nPrice) ?
				m_pSelf->m_uNormalPriceColor : m_pSelf->m_uNotEnoughMoneyPriceColor);

			}

			
		}

		m_pSelf->m_nslect = nslect;

		m_pSelf->m_ItemInfo = *pObj;
		m_pSelf->m_PriceInfo = *pPriceInfo;
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

KUiTradeConfirm::KUiTradeConfirm()
{
	m_PriceInfo.szItemName[0] = 0;
	m_PriceInfo.nPrice = 0;
	m_ItemInfo.Obj.uGenre = CGOG_NOTHING;
	m_nslect = 0;
}

//Èç¹û´°¿ÚÕý±»ÏÔÊ¾£¬Ôò·µ»ØÊµÀýÖ¸Õë
KUiTradeConfirm* KUiTradeConfirm::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹Ø±ÕÏú»Ù´°¿Ú
//--------------------------------------------------------------------------
void KUiTradeConfirm::CloseWindow(bool bDestroy)
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
void KUiTradeConfirm::Show()
{
	m_ItemName.SetText(m_PriceInfo.szItemName);
	int van = m_PriceInfo.nPrice/10000;
	int luong = m_PriceInfo.nPrice%10000;
	m_nBuyNumber = 1;
	m_BuyNumber.SetIntText(m_nBuyNumber);
	char ngan_luong[32] = "";

	if (!m_nslect)
	{

	if (van > 0)
	sprintf(ngan_luong,"%d %s ",van,"v¹n");

	sprintf(ngan_luong,"%s%d %s",ngan_luong,luong,"l­îng");

	}
	else if (m_nslect == 1)
	{
	sprintf(ngan_luong,"%d Xu",m_PriceInfo.nPrice);
	}
	else if (m_nslect == 2)
	{
	sprintf(ngan_luong,"%d tÝch lòy",m_PriceInfo.nPrice);
	}
	else if (m_nslect == 3)
	{
	sprintf(ngan_luong,"%d vinh dù",m_PriceInfo.nPrice);
	}


	m_Money.SetText(ngan_luong);

	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetExclusive((KWndWindow*)this);
}

//Òþ²Ø´°¿Ú
void KUiTradeConfirm::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º³õÊ¼»¯
//--------------------------------------------------------------------------
int KUiTradeConfirm::Initialize()
{
	AddChild(&m_ItemName);
	AddChild(&m_Money);
	AddChild(&m_BuyImg);
	AddChild(&m_SaleImg);
	AddChild(&m_RepairImg);
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
//	¹¦ÄÜ£ºÔØÈë´°¿ÚµÄ½çÃæ·½°¸
//--------------------------------------------------------------------------
void KUiTradeConfirm::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		m_ItemName.Init(&Ini, "ItemName");
		m_BuyNumber.Init(&Ini, "BuyNumber");

		m_Money.Init(&Ini, "Price");			
		Ini.GetString("Price", "Color", "", Buff, sizeof(Buff));
		m_uNormalPriceColor = GetColor(Buff);
		Ini.GetString("Price", "CantBuyColor", "", Buff, sizeof(Buff));
		m_uNotEnoughMoneyPriceColor = GetColor(Buff);

		m_BuyImg.Init(&Ini, "BuyImg");
		m_SaleImg.Init(&Ini, "SaleImg");
		m_RepairImg.Init(&Ini, "RepairImg");
		m_OkBtn.Init(&Ini, "OkBtn");
		m_CancelBtn.Init(&Ini, "CancelBtn");
		m_NextNumber.Init(&Ini, "Increase");
		m_PrevNumber.Init(&Ini, "Decrease");

	int van = m_PriceInfo.nPrice/10000;
	int luong = m_PriceInfo.nPrice%10000;
	char ngan_luong[32] = "";


	if (!m_nslect)
	{

	if (van > 0)
	sprintf(ngan_luong,"%d %s ",van,"v¹n");

	sprintf(ngan_luong,"%s%d %s",ngan_luong,luong,"l­îng");

	}
	else if (m_nslect == 1)
	{
	sprintf(ngan_luong,"%d Xu",m_PriceInfo.nPrice);
	}
	else if (m_nslect == 2)
	{
	sprintf(ngan_luong,"%d tÝch lòy",m_PriceInfo.nPrice);
	}
	else if (m_nslect == 3)
	{
	sprintf(ngan_luong,"%d vinh dù",m_PriceInfo.nPrice);
	}



	m_Money.SetText(ngan_luong);
	
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´°¿ÚÏûÏ¢º¯Êý
//--------------------------------------------------------------------------
int KUiTradeConfirm::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
		{
			if (m_BuyImg.IsVisible())
				OnBuy();
			else if (m_SaleImg.IsVisible())
				OnSale();
			else
				OnRepair();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnCancel();
		else if (uParam == (unsigned int)(KWndWindow*)&m_NextNumber)
		{

		m_nBuyNumber = m_BuyNumber.GetIntNumber();
		if (m_nBuyNumber < 60)
		{
		m_nBuyNumber++;
		m_BuyNumber.SetIntText(m_nBuyNumber);
		}
		
		}

		else if (uParam == (unsigned int)(KWndWindow*)&m_PrevNumber)
		{

		m_nBuyNumber = m_BuyNumber.GetIntNumber();
		if (m_nBuyNumber > 0)
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
			if (m_SaleImg.IsVisible())
			{
				OnSale();
			}
			else if (m_BuyImg.IsVisible())
			{
				if (m_BuyImg.IsDisable())
					OnCancel();
				else
					OnBuy();
			}
			else if (m_RepairImg.IsDisable())
			{
				OnCancel();
			}
			else
			{
				OnRepair();
			}
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
void KUiTradeConfirm::OnBuy()
{ 
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_BUY,
			(unsigned int)(&m_ItemInfo), m_nBuyNumber);
	}
	CloseWindow(false);
}


void KUiTradeConfirm::UpdateNumber()
{
    m_nBuyNumber = m_BuyNumber.GetIntNumber();

	if (m_nBuyNumber < 0)
	{
	m_nBuyNumber = 1;
	m_BuyNumber.SetIntText(m_nBuyNumber);
	}
	else if (m_nBuyNumber > 60)
	{
	m_nBuyNumber = 60;
	m_BuyNumber.SetIntText(m_nBuyNumber);
	}

	int van = m_nBuyNumber* m_PriceInfo.nPrice/10000;
	int luong = m_nBuyNumber* m_PriceInfo.nPrice%10000;
	char ngan_luong[32] = "";
	if (!m_nslect)
	{

	if (van > 0)
	sprintf(ngan_luong,"%d %s ",van,"v¹n");

	sprintf(ngan_luong,"%s%d %s",ngan_luong,luong,"l­îng");
	}
	else if (m_nslect == 1)
	{
	sprintf(ngan_luong,"%d Xu",m_nBuyNumber * m_PriceInfo.nPrice);
	}
	else if (m_nslect == 2)
	{
	sprintf(ngan_luong,"%d tÝch lòy",m_nBuyNumber * m_PriceInfo.nPrice);
	}
	else if (m_nslect == 3)
	{
	sprintf(ngan_luong,"%d vinh dù",m_nBuyNumber * m_PriceInfo.nPrice);
	}

	m_Money.SetText(ngan_luong);

}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÏìÓ¦µã»÷Âô°´Å¥
//--------------------------------------------------------------------------
void KUiTradeConfirm::OnSale()
{
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_SELL,
			(unsigned int)(&m_ItemInfo), 0);
	}
	CloseWindow(false);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÏìÓ¦µãÐÞÀí°´Å¥
//--------------------------------------------------------------------------
void KUiTradeConfirm::OnRepair()
{
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_REPAIR,
			(unsigned int)(&m_ItemInfo), 0);
	}
	CloseWindow(false);
}


void KUiTradeConfirm::OnCancel()
{
	CloseWindow(false);
}
