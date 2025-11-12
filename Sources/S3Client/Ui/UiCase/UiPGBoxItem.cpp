/*****************************************************************************************
//	界面--聊天窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-27
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiPGBoxItem.h"
#include "UiSysMsgCentre.h"
#include "../../../core/src/coreshell.h"
#include "../UiBase.h"
#include "crtdbg.h"
#include "../UiSoundSetting.h"

#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

extern iCoreShell*		g_pCoreShell;

#define 	SCHEME_INI			"UiPGBoxItem.ini"
#define 	SCHEME_INI_PAGE1	"UiPGBoxItemPage1.ini"
#define 	SCHEME_INI_PAGE2	"UiPGBoxItemPage2.ini"
#define 	SCHEME_INI_PAGE3	"UiPGBoxItemPage3.ini"
#define 	SCHEME_INI_PAGE4	"UiPGBoxItemPage4.ini"
#define 	SCHEME_INI_PAGE5	"UiPGBoxItemPage5.ini"

KUiPGBoxItem* KUiPGBoxItem::m_pSelf = NULL;

void KUiPGBoxItemPage1::Initialize(/*int nSubPageIndex*/)
{

	AddChild(&m_Tex1);
	AddChild(&m_Tex2);
	AddChild(&m_Tex3);
	AddChild(&m_MainTex);
	AddChild(&m_bButton);

   for (int i = 0; i < 3; i ++)
	{
		m_ItemBox[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_ItemBox[i]);
		m_ItemBox[i].SetContainerId((int)UOC_PG_BOX);
	}
}

//载入界面方案
void KUiPGBoxItemPage1::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI_PAGE1, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");

	m_Tex1.Init(&Ini, "Tex1");
	m_Tex2.Init(&Ini, "Tex2");
	m_Tex3.Init(&Ini, "Tex3");
	m_MainTex.Init(&Ini, "MainTex");
	m_bButton.Init(&Ini, "Button");

	for (int i = 0; i < 3; i ++)
	{
		char		BuffSection[128];
	    sprintf(BuffSection, "ItemBox%d", i);
		m_ItemBox[i].Init(&Ini, BuffSection);
	}



	}
}



int	KUiPGBoxItemPage1::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{

		int nRet = 0;
	switch(uMsg)
	{
	case WND_N_ITEM_PICKDROP:
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM) || 
		g_UiBase.IsOperationEnable(UIS_O_TRADE_ITEM) || 
		g_UiBase.IsOperationEnable(UIS_O_BLOCK_UNBLOCK_ITEM) ||
		g_UiBase.GetStatus() == UIS_S_TRADE_REPAIR ||
		g_UiBase.IsOperationEnable(UIS_O_S_PRICE_ITEM)
		)
			OnEquiptChanged((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;

	case WND_N_BUTTON_CLICK:

		if (uParam == (unsigned int)(KWndWindow*)&m_bButton)
		{
		
		if (g_pCoreShell)
		 {
		    g_pCoreShell->OperationRequest(GOI_BUTTON_PG_BOX,1, 0);
		 }

		}


		break;


	default:
		nRet = KWndPage::WndProc(uMsg, uParam, nParam);
	}

	return nRet;

}






void KUiPGBoxItemPage1::OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion	Drop, Pick;
	KUiDraggedObject	Obj;
	KWndWindow*			pWnd = NULL;

	UISYS_STATUS	eStatus = g_UiBase.GetStatus();
	if (pPickPos)
	{
		//_ASSERT(pPickPos->pWnd);
		((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = 0;
		Pick.eContainer = UOC_PG_BOX;
		pWnd = pPickPos->pWnd;

	  if (eStatus == UIS_S_TRADE_SALE)
		{
			return;
		}
		else if (eStatus == UIS_S_BLOCK_ITEM)
		{
	
			return;
		}
		else if (eStatus == UIS_S_UNBLOCK_ITEM)
		{
			return;
		}

		
		else if (eStatus == UIS_S_TRADE_NPC)
		{		
			return;
		}
		else if (eStatus == UIS_S_TRADE_BUY)
		{
			return;
		}

		else if (eStatus == UIS_S_PRICE_ITEM)
		{
			return;
		}

		

	}
	else if (pDropPos)
	{
		pWnd = pDropPos->pWnd;
	}
	else
		return;



	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = 0;
		Drop.eContainer = UOC_PG_BOX;
	}

	for (int i = 0; i < 3; i++)
	{
		if (pWnd == (KWndWindow*)&m_ItemBox[i])
		{
			Drop.Region.v = Pick.Region.v = i;
			break;
		}
	}

	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}



void KUiPGBoxItemPage1::UpdateDataP(KUiObjAtRegion* Equips)
{


	int	i;
	for (i = 0; i < 3; i++)
		m_ItemBox[i].Celar();

	for (i = 0; i < 3; i++)
	{
		if (Equips[i].Obj.uGenre != CGOG_NOTHING)
			UpdateEquip(&Equips[i], true);
	}


}


void KUiPGBoxItemPage1::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (pEquip)
	{
		for (int i = 0; i < 3; i++)
		{
			if (i == pEquip->Region.v)
			{
				if (bAdd)
					m_ItemBox[i].HoldObject(pEquip->Obj.uGenre, pEquip->Obj.uId,
						pEquip->Region.Width, pEquip->Region.Height);
				else
					m_ItemBox[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
				break;
			}
		}
	}
}




void KUiPGBoxItemPage2::Initialize(/*int nSubPageIndex*/)
{


	AddChild(&m_Tex1);
	AddChild(&m_Tex2);
	AddChild(&m_Tex3);
	AddChild(&m_Tex4);
	AddChild(&m_MainTex);
	AddChild(&m_bButton);


   for (int i = 0; i < 11; i ++)
	{
		m_ItemBox[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_ItemBox[i]);
		m_ItemBox[i].SetContainerId((int)UOC_PG_BOX);
	}


}

//载入界面方案
void KUiPGBoxItemPage2::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI_PAGE2, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");


	m_Tex1.Init(&Ini, "Tex1");
	m_Tex2.Init(&Ini, "Tex2");
	m_Tex3.Init(&Ini, "Tex3");
	m_Tex4.Init(&Ini, "Tex4");
	m_MainTex.Init(&Ini, "MainTex");
	m_bButton.Init(&Ini, "Button");

	for (int i = 0; i < 11; i ++)
	{
		char		BuffSection[128];
	    sprintf(BuffSection, "ItemBox%d", i);
		m_ItemBox[i].Init(&Ini, BuffSection);
	}




	}
}



int	KUiPGBoxItemPage2::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{

		int nRet = 0;
	switch(uMsg)
	{
	case WND_N_ITEM_PICKDROP:
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM) || 
		g_UiBase.IsOperationEnable(UIS_O_TRADE_ITEM) || 
		g_UiBase.IsOperationEnable(UIS_O_BLOCK_UNBLOCK_ITEM) ||
		g_UiBase.GetStatus() == UIS_S_TRADE_REPAIR ||
		g_UiBase.IsOperationEnable(UIS_O_S_PRICE_ITEM)
		)
			OnEquiptChanged((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;


	case WND_N_BUTTON_CLICK:

		if (uParam == (unsigned int)(KWndWindow*)&m_bButton)
		{
		
		if (g_pCoreShell)
		 {
		    g_pCoreShell->OperationRequest(GOI_BUTTON_PG_BOX,2, 0);
		 }

		}


		break;


	default:
		nRet = KWndPage::WndProc(uMsg, uParam, nParam);
	}

	return nRet;

}






void KUiPGBoxItemPage2::OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion	Drop, Pick;
	KUiDraggedObject	Obj;
	KWndWindow*			pWnd = NULL;

	UISYS_STATUS	eStatus = g_UiBase.GetStatus();
	if (pPickPos)
	{
		//_ASSERT(pPickPos->pWnd);
		((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = 0;
		Pick.eContainer = UOC_PG_BOX;
		pWnd = pPickPos->pWnd;

	  if (eStatus == UIS_S_TRADE_SALE)
		{
			return;
		}
		else if (eStatus == UIS_S_BLOCK_ITEM)
		{
	
			return;
		}
		else if (eStatus == UIS_S_UNBLOCK_ITEM)
		{
			return;
		}

		
		else if (eStatus == UIS_S_TRADE_NPC)
		{		
			return;
		}
		else if (eStatus == UIS_S_TRADE_BUY)
		{
			return;
		}
		else if (eStatus == UIS_S_PRICE_ITEM)
		{
			return;
		}

	}
	else if (pDropPos)
	{
		pWnd = pDropPos->pWnd;
	}
	else
		return;



	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = 0;
		Drop.eContainer = UOC_PG_BOX;
	}

	for (int i = 0; i < 11; i++)
	{
		if (pWnd == (KWndWindow*)&m_ItemBox[i])
		{
			Drop.Region.v = Pick.Region.v = i+3;
			break;
		}
	}

	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}



void KUiPGBoxItemPage2::UpdateDataP(KUiObjAtRegion* Equips)
{


	int	i;
	for (i = 0; i < 11; i++)
		m_ItemBox[i].Celar();

	for (i = 0; i < 11; i++)
	{
		if (Equips[i+3].Obj.uGenre != CGOG_NOTHING)
			UpdateEquip(&Equips[i+3], true);
	}


}


void KUiPGBoxItemPage2::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (pEquip)
	{
		for (int i = 0; i < 11; i++)
		{
			if ((i+3) == pEquip->Region.v)
			{
				if (bAdd)
					m_ItemBox[i].HoldObject(pEquip->Obj.uGenre, pEquip->Obj.uId,
						pEquip->Region.Width, pEquip->Region.Height);
				else
					m_ItemBox[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
				break;
			}
		}
	}
}









void KUiPGBoxItemPage3::Initialize(/*int nSubPageIndex*/)
{



	AddChild(&m_Tex1);
	AddChild(&m_Tex2);

	AddChild(&m_MainTex);
	AddChild(&m_bButton);


   for (int i = 0; i < 2; i ++)
	{
		m_ItemBox[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_ItemBox[i]);
		m_ItemBox[i].SetContainerId((int)UOC_PG_BOX);
	}



}

//载入界面方案
void KUiPGBoxItemPage3::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI_PAGE3, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");



	m_Tex1.Init(&Ini, "Tex1");
	m_Tex2.Init(&Ini, "Tex2");
	m_MainTex.Init(&Ini, "MainTex");
	m_bButton.Init(&Ini, "Button");

	for (int i = 0; i < 2; i ++)
	{
		char		BuffSection[128];
	    sprintf(BuffSection, "ItemBox%d", i);
		m_ItemBox[i].Init(&Ini, BuffSection);
	}



	}
}



int	KUiPGBoxItemPage3::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{

		int nRet = 0;
	switch(uMsg)
	{
	case WND_N_ITEM_PICKDROP:
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM) || 
		g_UiBase.IsOperationEnable(UIS_O_TRADE_ITEM) || 
		g_UiBase.IsOperationEnable(UIS_O_BLOCK_UNBLOCK_ITEM) ||
		g_UiBase.GetStatus() == UIS_S_TRADE_REPAIR ||
		g_UiBase.IsOperationEnable(UIS_O_S_PRICE_ITEM)
		)
			OnEquiptChanged((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;

	case WND_N_BUTTON_CLICK:

		if (uParam == (unsigned int)(KWndWindow*)&m_bButton)
		{
		
		if (g_pCoreShell)
		 {
		    g_pCoreShell->OperationRequest(GOI_BUTTON_PG_BOX,3, 0);
		 }

		}


		break;



	default:
		nRet = KWndPage::WndProc(uMsg, uParam, nParam);
	}

	return nRet;

}






void KUiPGBoxItemPage3::OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion	Drop, Pick;
	KUiDraggedObject	Obj;
	KWndWindow*			pWnd = NULL;

	UISYS_STATUS	eStatus = g_UiBase.GetStatus();
	if (pPickPos)
	{
		//_ASSERT(pPickPos->pWnd);
		((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = 0;
		Pick.eContainer = UOC_PG_BOX;
		pWnd = pPickPos->pWnd;

	  if (eStatus == UIS_S_TRADE_SALE)
		{
			return;
		}
		else if (eStatus == UIS_S_BLOCK_ITEM)
		{
	
			return;
		}
		else if (eStatus == UIS_S_UNBLOCK_ITEM)
		{
			return;
		}

		
		else if (eStatus == UIS_S_TRADE_NPC)
		{		
			return;
		}
		else if (eStatus == UIS_S_TRADE_BUY)
		{
			return;
		}
		else if (eStatus == UIS_S_PRICE_ITEM)
		{
			return;
		}

	}
	else if (pDropPos)
	{
		pWnd = pDropPos->pWnd;
	}
	else
		return;



	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = 0;
		Drop.eContainer = UOC_PG_BOX;
	}

	for (int i = 0; i < 2; i++)
	{
		if (pWnd == (KWndWindow*)&m_ItemBox[i])
		{
			Drop.Region.v = Pick.Region.v = i+3+11;
			break;
		}
	}

	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}



void KUiPGBoxItemPage3::UpdateDataP(KUiObjAtRegion* Equips)
{


	int	i;
	for (i = 0; i < 2; i++)
		m_ItemBox[i].Celar();

	for (i = 0; i < 2; i++)
	{
		if (Equips[i+3+11].Obj.uGenre != CGOG_NOTHING)
			UpdateEquip(&Equips[i+3+11], true);
	}


}


void KUiPGBoxItemPage3::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (pEquip)
	{
		for (int i = 0; i < 2; i++)
		{
			if ((i+3+11) == pEquip->Region.v)
			{
				if (bAdd)
					m_ItemBox[i].HoldObject(pEquip->Obj.uGenre, pEquip->Obj.uId,
						pEquip->Region.Width, pEquip->Region.Height);
				else
					m_ItemBox[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
				break;
			}
		}
	}
}





void KUiPGBoxItemPage4::Initialize(/*int nSubPageIndex*/)
{



	AddChild(&m_Tex1);
	AddChild(&m_Tex2);
	AddChild(&m_Tex3);
	AddChild(&m_Tex4);

	AddChild(&m_MainTex);
	AddChild(&m_bButton);


   for (int i = 0; i < 11; i ++)
	{
		m_ItemBox[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_ItemBox[i]);
		m_ItemBox[i].SetContainerId((int)UOC_PG_BOX);
	}



}

//载入界面方案
void KUiPGBoxItemPage4::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI_PAGE4, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");


	m_Tex1.Init(&Ini, "Tex1");
	m_Tex2.Init(&Ini, "Tex2");
	m_Tex3.Init(&Ini, "Tex3");
	m_Tex4.Init(&Ini, "Tex4");
	m_MainTex.Init(&Ini, "MainTex");
	m_bButton.Init(&Ini, "Button");

	for (int i = 0; i < 11; i ++)
	{
		char		BuffSection[128];
	    sprintf(BuffSection, "ItemBox%d", i);
		m_ItemBox[i].Init(&Ini, BuffSection);
	}


	}
}



int	KUiPGBoxItemPage4::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{

		int nRet = 0;
	switch(uMsg)
	{
	case WND_N_ITEM_PICKDROP:
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM) || 
		g_UiBase.IsOperationEnable(UIS_O_TRADE_ITEM) || 
		g_UiBase.IsOperationEnable(UIS_O_BLOCK_UNBLOCK_ITEM) ||
		g_UiBase.GetStatus() == UIS_S_TRADE_REPAIR ||
		g_UiBase.IsOperationEnable(UIS_O_S_PRICE_ITEM)
		)
			OnEquiptChanged((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;


	case WND_N_BUTTON_CLICK:

		if (uParam == (unsigned int)(KWndWindow*)&m_bButton)
		{
		
		if (g_pCoreShell)
		 {
		    g_pCoreShell->OperationRequest(GOI_BUTTON_PG_BOX,4, 0);
		 }

		}


		break;



	default:
		nRet = KWndPage::WndProc(uMsg, uParam, nParam);
	}

	return nRet;

}






void KUiPGBoxItemPage4::OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion	Drop, Pick;
	KUiDraggedObject	Obj;
	KWndWindow*			pWnd = NULL;

	UISYS_STATUS	eStatus = g_UiBase.GetStatus();
	if (pPickPos)
	{
		//_ASSERT(pPickPos->pWnd);
		((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = 0;
		Pick.eContainer = UOC_PG_BOX;
		pWnd = pPickPos->pWnd;

	  if (eStatus == UIS_S_TRADE_SALE)
		{
			return;
		}
		else if (eStatus == UIS_S_BLOCK_ITEM)
		{
	
			return;
		}
		else if (eStatus == UIS_S_UNBLOCK_ITEM)
		{
			return;
		}

		
		else if (eStatus == UIS_S_TRADE_NPC)
		{		
			return;
		}
		else if (eStatus == UIS_S_TRADE_BUY)
		{
			return;
		}
		else if (eStatus == UIS_S_PRICE_ITEM)
		{
			return;
		}

	}
	else if (pDropPos)
	{
		pWnd = pDropPos->pWnd;
	}
	else
		return;



	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = 0;
		Drop.eContainer = UOC_PG_BOX;
	}

	for (int i = 0; i < 11; i++)
	{
		if (pWnd == (KWndWindow*)&m_ItemBox[i])
		{
			Drop.Region.v = Pick.Region.v = i+3+11+2;
			break;
		}
	}

	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}



void KUiPGBoxItemPage4::UpdateDataP(KUiObjAtRegion* Equips)
{


	int	i;
	for (i = 0; i < 11; i++)
		m_ItemBox[i].Celar();

	for (i = 0; i < 11; i++)
	{
		if (Equips[i+3+11+2].Obj.uGenre != CGOG_NOTHING)
			UpdateEquip(&Equips[i+3+11+2], true);
	}


}


void KUiPGBoxItemPage4::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (pEquip)
	{
		for (int i = 0; i < 11; i++)
		{
			if ((i+3+11+2) == pEquip->Region.v)
			{
				if (bAdd)
					m_ItemBox[i].HoldObject(pEquip->Obj.uGenre, pEquip->Obj.uId,
						pEquip->Region.Width, pEquip->Region.Height);
				else
					m_ItemBox[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
				break;
			}
		}
	}
}




void KUiPGBoxItemPage5::Initialize(/*int nSubPageIndex*/)
{

}

//载入界面方案
void KUiPGBoxItemPage5::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI_PAGE5, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
	}
}



int	KUiPGBoxItemPage5::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{

	return KWndPage::WndProc(uMsg, uParam, nParam);
}


KUiPGBoxItem* KUiPGBoxItem::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiPGBoxItem;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
		Wnd_GameSpaceHandleInput(false);
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	功能：如果窗口正被显示，则返回实例指针
//--------------------------------------------------------------------------
KUiPGBoxItem* KUiPGBoxItem::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	功能：关闭窗口，同时可以选则是否删除对象实例
//--------------------------------------------------------------------------
void KUiPGBoxItem::CloseWindow(bool bDestroy)
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
		Wnd_GameSpaceHandleInput(true);
	}
}




void KUiPGBoxItem::Initialize()
{
	//m_pPage1.Initialize();
	//AddPage(&m_pPage1, &m_bPage1);

	m_pPage2.Initialize();
	AddPage(&m_pPage2, &m_bPage2);

	m_pPage3.Initialize();
	AddPage(&m_pPage3, &m_bPage3);

	m_pPage4.Initialize();
	AddPage(&m_pPage4, &m_bPage4);

	//m_pPage5.Initialize();
	//AddPage(&m_pPage5, &m_bPage5);

	AddChild(&m_bClose);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}



void KUiPGBoxItem::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		//m_pSelf->m_bPage1.Init(&Ini, "ButtonPage1");
		//m_pSelf->m_pPage1.LoadScheme(pScheme);
		m_pSelf->m_bPage2.Init(&Ini, "ButtonPage2");
		m_pSelf->m_pPage2.LoadScheme(pScheme);
		m_pSelf->m_bPage3.Init(&Ini, "ButtonPage3");
		m_pSelf->m_pPage3.LoadScheme(pScheme);
		m_pSelf->m_bPage4.Init(&Ini, "ButtonPage4");
		m_pSelf->m_pPage4.LoadScheme(pScheme);
		m_pSelf->m_bPage5.Init(&Ini, "ButtonPage5");
		//m_pSelf->m_pPage5.LoadScheme(pScheme);
		//m_pSelf->m_bClose.Init(&Ini, "ButtonClose");
	}
}






int KUiPGBoxItem::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{

	if (uMsg == WND_N_BUTTON_CLICK && (KWndWindow*)uParam == (KWndWindow*)&m_bClose)
	{
		CloseWindow(false);
		return 0;
	}
	return KWndPageSet::WndProc(uMsg, uParam, nParam);
}



void KUiPGBoxItem::UpdateData()
{
	if (m_pSelf)
	{

	KUiObjAtRegion	Equips[27];

	g_pCoreShell->GetGameData(GDI_ITEM_IN_PG_BOX, (unsigned int)&Equips, 0);

	m_pSelf->m_pPage1.UpdateDataP(Equips);
	m_pSelf->m_pPage2.UpdateDataP(Equips);
	m_pSelf->m_pPage3.UpdateDataP(Equips);
	m_pSelf->m_pPage4.UpdateDataP(Equips);

	}
}


void KUiPGBoxItem::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	m_pSelf->m_pPage1.UpdateEquip(pEquip,bAdd);
	m_pSelf->m_pPage2.UpdateEquip(pEquip,bAdd);
	m_pSelf->m_pPage3.UpdateEquip(pEquip,bAdd);
	m_pSelf->m_pPage4.UpdateEquip(pEquip,bAdd);
}
