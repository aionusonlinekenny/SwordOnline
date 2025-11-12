/*****************************************************************************************
//	界面--储物箱界面
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-21
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "UiBuyShop.h"
#include "UiItem.h"
#include "BuySlectItemShop.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include <crtdbg.h>


extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI_ITEM	"UiBuyShop.ini"

KUiBuyShop* KUiBuyShop::m_pSelf = NULL;

KUiBuyShop* KUiBuyShop::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

KUiBuyShop* KUiBuyShop::OpenWindow(char* szTitleBox)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiBuyShop;
		if (m_pSelf)
			m_pSelf->Initialize();
	}

	if (m_pSelf)
	{
		if (KUiItem::GetIfVisible() == NULL)
			KUiItem::OpenWindow();
		else
			UiSoundPlay(UI_SI_WND_OPENCLOSE);

		m_pSelf->UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
		m_pSelf->m_TitleBox.SetText(szTitleBox);
		Wnd_GameSpaceHandleInput(false);

	}

	return m_pSelf;
}

//--------------------------------------------------------------------------
//	功能：关闭窗口
//--------------------------------------------------------------------------
void KUiBuyShop::CloseWindow()
{
	if (m_pSelf)
	{
		if (g_UiBase.GetStatus() == UIS_S_IDLE)
		{
			if (g_pCoreShell)
				g_pCoreShell->OperationRequest(GOI_BUY_SHOP_CLOSE,0,0);
		}

		Wnd_GameSpaceHandleInput(true);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

// -------------------------------------------------------------------------
// 功能	: 初始化
// -------------------------------------------------------------------------
void KUiBuyShop::Initialize()
{
	AddChild(&m_TitleBox);
	AddChild(&m_CloseBtn);
	AddChild(&m_ItemBox);

	m_ItemBox.SetContainerId((int)UOC_BUY_SHOP);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

void KUiBuyShop::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
{
	if (pItem)
	{
		UiSoundPlay(UI_SI_PICKPUT_ITEM);

			KUiDraggedObject Obj;
			Obj.uGenre = pItem->Obj.uGenre;
			Obj.uId = pItem->Obj.uId;
			Obj.DataX = pItem->Region.h;
			Obj.DataY = pItem->Region.v;
			Obj.DataW = pItem->Region.Width;
			Obj.DataH = pItem->Region.Height;
			if (bAdd)
				m_ItemBox.AddObject(&Obj, 1);
			else
				m_ItemBox.RemoveObject(&Obj);

	
	}
}

void KUiBuyShop::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_ITEM);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_TitleBox.Init(&Ini, "TitleBox");
		m_pSelf->m_CloseBtn.Init(&Ini, "CloseBtn");
		m_pSelf->m_ItemBox.Init(&Ini, "ItemBox");
		m_pSelf->m_ItemBox.EnableTracePutPos(true);
	}
}

// -------------------------------------------------------------------------
// 功能	: 窗口函数
// -------------------------------------------------------------------------
int KUiBuyShop::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_ITEM_PICKDROP:
		OnItemPickDrop((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;
	case WND_N_LEFT_CLICK_SHIFT_ITEM:
		break;
	case WND_N_BUTTON_CLICK:

		if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
		{

			CloseWindow();
		}
		break;
	case WND_M_OTHER_WORK_RESULT:
		break;
	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiBuyShop::OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	if (g_UiBase.GetStatus() != UIS_S_IDLE)
		return;
	KUiObjAtContRegion	Pick;
	KUiDraggedObject	Obj;

	if (pPickPos)
	{
		_ASSERT(pPickPos->pWnd);		
		((KWndObjectMatrix*)(pPickPos->pWnd))->GetObject(
			Obj, pPickPos->h, pPickPos->v);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = Obj.DataX;
		Pick.Region.v = Obj.DataY;
		Pick.eContainer = UOC_CHECK_BOX;
	

	           KUiItemBuySelInfo	Price = { 0 };
		if (g_pCoreShell->GetGameData(GDI_SHOP_ITEM_PRICE,
			(unsigned int)(&Pick), (int)(&Price)) != 0)
		{
			KBuySelectItemShop::OpenWindow(&Pick,
			&Price);

		}
	}
}