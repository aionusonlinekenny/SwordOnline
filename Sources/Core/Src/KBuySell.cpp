#include "KEngine.h"
#include "KCore.h"
#include "KTabFile.h"
#include "KPlayer.h"
#include "KNpc.h"
#include "KItemGenerator.h"
#include "KSubWorldSet.h"
#include "KItemSet.h"
#include "KBuySell.h"
#ifndef _STANDALONE
#include "CoreShell.h"
#include "crtdbg.h"
#endif
#include "CoreUseNameDef.h"
#ifdef _SERVER
//#include "../MultiServer/Heaven/Interface/iServer.h"
#endif

#define	SHOP_BOX_WIDTH		6
#define	SHOP_BOX_HEIGHT		10

KBuySell	BuySell;

KBuySell::KBuySell()
{
#ifndef _SERVER
	m_pShopRoom = NULL;
#endif
	m_Item = NULL;
	m_SellItem = NULL;
	m_Width = 0;
	m_Height = 0;
	m_MaxItem = 0;
}

KBuySell::~KBuySell()
{
#ifndef _SERVER
	if (m_pShopRoom)
	{
		delete m_pShopRoom;
		m_pShopRoom = NULL;
	}
#endif
	if (m_Item)
	{
		delete [] m_Item;
		m_Item = NULL;
	}
	if (m_SellItem)
	{
		for (int i = 0; i < m_Height; i++)
		{
			if (m_SellItem[i])
			{
				delete m_SellItem[i];
				m_SellItem[i] = NULL;
			}
		}
		delete m_SellItem;
		m_SellItem = NULL;
	}
	m_Width = 0;
	m_Height = 0;
	m_MaxItem = 0;
}

BOOL KBuySell::Init()
{
	KTabFile		GoodsFile;
	KTabFile		BuySellFile;

//	g_SetFilePath("\\");
	if (!BuySellFile.Load(BUYSELL_FILE) || !GoodsFile.Load(GOODS_FILE))
		return FALSE;

	int nHeight = GoodsFile.GetHeight() - 1;
	int nWidth = GoodsFile.GetWidth() - 1;
	if (nWidth == 0 || nHeight == 0)
		return FALSE;

	m_Item = (KItem *)new KItem[nHeight];
	if (!m_Item)
		return FALSE;

	ItemGenerate ItemGenerator;
	for (int k = 0; k < nHeight; k++)
	{
		GoodsFile.GetInteger(k + 2, 1, -1, &ItemGenerator.nGenre);
		GoodsFile.GetInteger(k + 2, 2, -1, &ItemGenerator.nDetailType);
		GoodsFile.GetInteger(k + 2, 3, -1, &ItemGenerator.nParticularType);
		GoodsFile.GetInteger(k + 2, 4, -1, &ItemGenerator.nSeriesReq);
		GoodsFile.GetInteger(k + 2, 5, -1, &ItemGenerator.nLevel);


		int nPriceGold = -1;


		int nPriceTL = -1;
		int nPriceVD = -1;

		GoodsFile.GetInteger(k + 2, 6, -1, &nPriceGold);

		GoodsFile.GetInteger(k + 2, 7, -1, &nPriceTL);
		GoodsFile.GetInteger(k + 2, 8, -1, &nPriceVD);


		switch(ItemGenerator.nGenre)
		{
		case item_equip:
			ItemGen.Gen_Equipment(ItemGenerator.nDetailType,
				ItemGenerator.nParticularType,
				ItemGenerator.nSeriesReq,
				ItemGenerator.nLevel,
				NULL,
				0,
				g_SubWorldSet.GetGameVersion(),
				&m_Item[k]);
			break;
		case item_medicine:
			ItemGen.Gen_Medicine(ItemGenerator.nDetailType, 
				ItemGenerator.nLevel, 
				g_SubWorldSet.GetGameVersion(),
				&m_Item[k]);
			break;
		case item_townportal:
			ItemGen.Gen_TownPortal(&m_Item[k]);
			break;

		case item_task:
		    ItemGen.Gen_Quest(ItemGenerator.nDetailType, &m_Item[k],g_SubWorldSet.GetGameVersion(),NULL,ItemGenerator.nSeriesReq);
		break;

		default:
			break;
		}

		m_Item[k].m_nPriceGold = nPriceGold;

		m_Item[k].m_nPriceTL = nPriceTL;
        m_Item[k].m_nPriceVD = nPriceVD;

		m_MaxItem++;
	}

	m_Height = BuySellFile.GetHeight() - 1;
	m_Width  = BuySellFile.GetWidth();
	
	if (m_Width == 0 || m_Height == 0)
		return FALSE;

	m_SellItem = (int **)new int*[m_Height];
	if (!m_SellItem)
		return FALSE;

	for (int i = 0; i < m_Height; i++)
	{
		m_SellItem[i] = NULL;
		m_SellItem[i] = (int *)new int[m_Width];
		if (!m_SellItem[i])
			return FALSE;
		for (int j = 0; j < m_Width; j++)
		{
			BuySellFile.GetInteger(i + 2, j + 1, -1, &m_SellItem[i][j]);
			if (m_SellItem[i][j] == -1)
			{
				continue;
			}
			_ASSERT(m_SellItem[i][j] > 0);		// ²ß»®ÊÇ´Ó1¿ªÊ¼µÄ
			if (m_SellItem[i][j] > 0)
				m_SellItem[i][j] -= 1;			// ÎªÁË²ß»®´Ó1¿ªÊ¼Ìî±í
		}
	}

#ifndef _SERVER
	if (!m_pShopRoom)
	{
		m_pShopRoom = new KInventory;
		m_pShopRoom->Init(SHOP_BOX_WIDTH, SHOP_BOX_HEIGHT);
	}
#endif
	return TRUE;
}

KItem* KBuySell::GetItem(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_MaxItem || !m_Item)
		return NULL;

	return &m_Item[nIndex];
}

int KBuySell::GetItemIndex(int nShop, int nIndex)
{
	if (!m_SellItem || nShop < 0 || nShop >= m_Height || nIndex < 0 || nIndex >= m_Width)
		return -1;

	if (!m_SellItem[nShop])
		return -1;

	return m_SellItem[nShop][nIndex];
}

#ifdef _SERVER
BOOL KBuySell::AutoSell(int nPlayerIdx, int nBuy, int nIdx, int nBuyNumber)
{
	KASSERT(nPlayerIdx >= 0 && nPlayerIdx < MAX_PLAYER);
	KASSERT(nIdx >= 0 && nIdx < MAX_ITEM);
	
	if (Npc[Player[nPlayerIdx].m_nIndex].m_FightMode)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_FIGHT_MODE_ERROR1;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		if(g_pServer && Player[nPlayerIdx].m_nNetConnectIdx != -1)
			g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}	

	if (Item[nIdx].GetGenre() == item_task)
		return FALSE;

    if ((Item[nIdx].GetGenre() == item_equip || Item[nIdx].GetGenre() == item_medicine) && Item[nIdx].m_GeneratorParam.nVersion > 1)
		return FALSE;

	if (Item[nIdx].GetGenre() == item_equip && Item[nIdx].GetDurability() == 0)
		return FALSE;
		
	if (Npc[Player[nPlayerIdx].m_nIndex].m_FightMode)
        return FALSE;

    if (nBuy != Player[nPlayerIdx].m_BuyInfo.m_nBuyIdx)
	{
		g_DebugLog("BuySell: %s buy idx error!", Npc[Player[nPlayerIdx].m_nIndex].Name);
		return FALSE;
	}
	int nMoney = Item[nIdx].GetPrice() ;//* Item[nIdx].GetStackNum();
    nMoney /= BUY_SELL_SCALE;
	if (nMoney <= 0)
		nMoney = 0;
	if (nMoney)
		Player[nPlayerIdx].Earn(nMoney);
	Player[nPlayerIdx].m_ItemList.Remove(nIdx);
	ItemSet.Remove(nIdx);	

	return TRUE;
}

BOOL	KBuySell::AutoBuyItem(int nPlayerIdx, BYTE nItemGenre, BYTE nDetailType, BYTE nLevel, BYTE nBuyNumber)
{
	BOOL bShowMsg = TRUE;
	SHOW_MSG_SYNC	sMsg;
	sMsg.ProtocolType = s2c_msgshow;	
	for(int i = 0; i < nBuyNumber; i++)
	{
		int nIndex = ItemSet.Add(nItemGenre, 0, nLevel, 0, nDetailType, 0,  0, g_SubWorldSet.GetGameVersion());
		if (nIndex <= 0)
			return FALSE;

		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);		
		if (Player[nPlayerIdx].m_ItemList.GetEquipmentMoney() < Item[nIndex].GetPrice())
		{
			//bShowMsg = FALSE;		
			//sMsg.m_wMsgID = enumMSG_ID_SHOP_NO_MONEY;
			break;
		}

		int nWidth = Item[nIndex].GetWidth();
		int nHeight = Item[nIndex].GetHeight();

		ItemPos	Pos;
		if (Player[nPlayerIdx].m_ItemList.SearchPosition(nWidth, nHeight, &Pos))
		{
			if(Pos.nPlace == pos_hand)
			{
					break;
			}
			if(Player[nPlayerIdx].m_ItemList.Add(nIndex, Pos.nPlace, Pos.nX, Pos.nY))
			{
				int nPrice = Item[nIndex].GetPrice();
				Player[nPlayerIdx].Pay(nPrice);
			}
		}
		else
		{
				break;
		}
	}
	if (!bShowMsg)
		g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);	

	return bShowMsg;
}

BOOL KBuySell::Buy(int nPlayerIdx, int nBuy, int nBuyIdx, int nPlace, int nX, int nY)
{
	KASSERT(nPlayerIdx >= 0 && nPlayerIdx < MAX_PLAYER);

	if (nPlayerIdx < 0 && nPlayerIdx >= MAX_PLAYER)
	{
	printf("Xay ra loi giao dich 1, tu dong xua loi !\n");
	return FALSE;
	}





	if (Player[nPlayerIdx].m_bBlock)
	{
	BYTE	byFinished = s2c_itemexchangefinish;
	if (g_pServer)
		g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &byFinished, sizeof(BYTE));
	return FALSE;
	}




	if (Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex == g_SubWorldSet.SearchWorld(43))
	{
	BYTE	byFinished = s2c_itemexchangefinish;
	if (g_pServer)
		g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &byFinished, sizeof(BYTE));
	return FALSE;
	}



	if (nBuy != Player[nPlayerIdx].m_BuyInfo.m_nBuyIdx)
	{
		return FALSE;
	}

	if (nBuy >= m_Height)
	{
	printf("Xay ra loi giao dich 10 [%s] [%s], tu dong xua loi !\n",Player[nPlayerIdx].m_PlayerName,Player[nPlayerIdx].m_AccoutName);
	return FALSE;
	}




	if (nBuyIdx >= m_Width)
	{
	printf("Xay ra loi giao dich 2 [%s] [%s], tu dong xua loi !\n",Player[nPlayerIdx].m_PlayerName,Player[nPlayerIdx].m_AccoutName);
	return FALSE;
	}

	if (nBuy < 0)
	{
	printf("Xay ra loi giao dich 3 [%s] [%s], tu dong xua loi !\n",Player[nPlayerIdx].m_PlayerName,Player[nPlayerIdx].m_AccoutName);
	return FALSE;
	}

	if (nBuyIdx < 0)
	{
	printf("Xay ra loi giao dich 4 [%s] [%s], tu dong xua loi !\n",Player[nPlayerIdx].m_PlayerName,Player[nPlayerIdx].m_AccoutName);
	return FALSE;
	}

	if (!m_SellItem)
	{
	printf("Xay ra loi giao dich 5 [%s] [%s], tu dong xua loi !\n",Player[nPlayerIdx].m_PlayerName,Player[nPlayerIdx].m_AccoutName);
	return FALSE;
	}

	if (!m_SellItem[nBuy])
	{
	printf("Xay ra loi giao dich 6 [%s] [%s], tu dong xua loi !\n",Player[nPlayerIdx].m_PlayerName,Player[nPlayerIdx].m_AccoutName);
	return FALSE;
	}

    if (!m_SellItem[nBuy][nBuyIdx])
	{
	printf("Xay ra loi giao dich 7 [%s] [%s], tu dong xua loi !\n",Player[nPlayerIdx].m_PlayerName,Player[nPlayerIdx].m_AccoutName);
	return FALSE;
	}

	if (m_SellItem[nBuy][nBuyIdx] < 0 || m_SellItem[nBuy][nBuyIdx] >= m_MaxItem)
	{
	printf("Xay ra loi giao dich 8 [%s] [%s], tu dong xua loi !\n",Player[nPlayerIdx].m_PlayerName,Player[nPlayerIdx].m_AccoutName);
	return FALSE;
	}

	int nIdx = m_SellItem[nBuy][nBuyIdx];
	if (nIdx < 0)
	{
	printf("Xay ra loi giao dich 9 [%s] [%s], tu dong xua loi !\n",Player[nPlayerIdx].m_PlayerName,Player[nPlayerIdx].m_AccoutName);
	return FALSE;
	}


if (nPlace != pos_hand && nPlace != pos_equiproom)
{
printf("Mua Item nham ruong [%s] [%s], loi !\n",Player[nPlayerIdx].GetPlayerName(),Player[nPlayerIdx].m_AccoutName);
return FALSE;
}


ItemPos	Pos;

if (!Player[nPlayerIdx].m_ItemList.SearchPosition(m_Item[nIdx].GetWidth(), m_Item[nIdx].GetHeight(), &Pos))
{
printf("Het cho trong [%s] [%s], loi !\n",Player[nPlayerIdx].GetPlayerName(),Player[nPlayerIdx].m_AccoutName);
return FALSE;
}

if (Pos.nPlace != nPlace || Pos.nX != nX || Pos.nY != nY)
{
return FALSE;
}


if (Npc[Player[nPlayerIdx].m_nIndex].m_FightMode)
return FALSE;


int nPriceGold = m_Item[nIdx].m_nPriceGold;


int nPriceTL = m_Item[nIdx].m_nPriceTL;
int nPriceVD = m_Item[nIdx].m_nPriceVD;

if (nPriceGold > 0)
{

if (Player[nPlayerIdx].m_cTask.GetSaveVal(99) != Player[nPlayerIdx].m_cTask.GetSaveVal(101))
{
	printf("Vang mua Item khong trung Task [%s] [%s], loi !\n",Player[nPlayerIdx].GetPlayerName(),Player[nPlayerIdx].m_AccoutName);
	return FALSE;
}


if (Player[nPlayerIdx].m_cTask.GetSaveVal(99) <= 0 || Player[nPlayerIdx].m_cTask.GetSaveVal(101) <= 0)
	return FALSE;



if (Player[nPlayerIdx].m_cTask.GetSaveVal(99) < nPriceGold)
	return FALSE;




/*

if (
	
	m_Item[nIdx].GetGenre() == 4 && 
	(
	m_Item[nIdx].GetDetailType() == 2 || 
	m_Item[nIdx].GetDetailType() == 161
//	m_Item[nIdx].GetDetailType() == 22 ||
//	m_Item[nIdx].GetDetailType() == 35 ||
//	m_Item[nIdx].GetDetailType() == 36 ||
//	m_Item[nIdx].GetDetailType() == 37 ||
//	m_Item[nIdx].GetDetailType() == 38
	)
	

	)

{

KPlayerChat::SendSystemInfo(1,nPlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm ch­a b¸n, kh«ng thÓ mua !", strlen("VËt phÈm ch­a b¸n, kh«ng thÓ mua !") );

BYTE	byFinished = s2c_itemexchangefinish;
	if (g_pServer)
		g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &byFinished, sizeof(BYTE));

		return FALSE;
}


*/




int nItemIdx = ItemSet.Add(&m_Item[nIdx]);

	if (!nItemIdx)
		return FALSE;

	Player[nPlayerIdx].m_cTask.SetSaveVal(99,Player[nPlayerIdx].m_cTask.GetSaveVal(99) - nPriceGold);

	Player[nPlayerIdx].m_cTask.SetSaveVal(101,Player[nPlayerIdx].m_cTask.GetSaveVal(101) - nPriceGold);

	Player[nPlayerIdx].m_ItemList.Add(nItemIdx, nPlace, nX, nY);





		 if (Player[nPlayerIdx].CanSave())
			{
				if (Player[nPlayerIdx].Save())
				{
					Player[nPlayerIdx].m_uMustSave = SAVE_REQUEST;
				}
	
			}



}


















else if (nPriceTL > 0)
{




if (Player[nPlayerIdx].m_cTask.GetSaveVal(15) <= 0)
	return FALSE;



if (Player[nPlayerIdx].m_cTask.GetSaveVal(15) < nPriceTL)
	return FALSE;




int nItemIdx = ItemSet.Add(&m_Item[nIdx]);

	if (!nItemIdx)
		return FALSE;

	Player[nPlayerIdx].m_cTask.SetSaveVal(15,Player[nPlayerIdx].m_cTask.GetSaveVal(15) - nPriceTL);


	Player[nPlayerIdx].m_ItemList.Add(nItemIdx, nPlace, nX, nY);





		 if (Player[nPlayerIdx].CanSave())
			{
				if (Player[nPlayerIdx].Save())
				{
					Player[nPlayerIdx].m_uMustSave = SAVE_REQUEST;
				}
	
			}



}



else if (nPriceVD > 0)
{




if (Player[nPlayerIdx].m_cTask.GetSaveVal(124) <= 0)
	return FALSE;



if (Player[nPlayerIdx].m_cTask.GetSaveVal(124) < nPriceVD)
	return FALSE;




int nItemIdx = ItemSet.Add(&m_Item[nIdx]);

	if (!nItemIdx)
		return FALSE;

	Player[nPlayerIdx].m_cTask.SetSaveVal(124,Player[nPlayerIdx].m_cTask.GetSaveVal(124) - nPriceVD);


	Player[nPlayerIdx].m_ItemList.Add(nItemIdx, nPlace, nX, nY);





		 if (Player[nPlayerIdx].CanSave())
			{
				if (Player[nPlayerIdx].Save())
				{
					Player[nPlayerIdx].m_uMustSave = SAVE_REQUEST;
				}
	
			}



}

















else

{

	if (Player[nPlayerIdx].m_nIndex <= 0 || Player[nPlayerIdx].m_nIndex >= MAX_NPC)
		return FALSE;

	if (Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex < 0)
		return FALSE;


	if (m_Item[nIdx].GetPrice() <= 0)
		return FALSE;

int nMoneyMapVG = 0;

if (SubWorld[Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex].m_bCheckTong)
{
int nTongVG = SubWorld[Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex].m_nTongVG;
if (nTongVG >= 0 && nTongVG <= 50)
{


if (SubWorld[Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex].m_dwTongName == Player[nPlayerIdx].m_cTong.GetTongNameID())
{
nMoneyMapVG = m_Item[nIdx].GetPrice() * nTongVG /200;
}
else
{
nMoneyMapVG = m_Item[nIdx].GetPrice() * nTongVG /100;
}



if (m_Item[nIdx].GetPrice() <= nMoneyMapVG || nMoneyMapVG < 0)
{
nMoneyMapVG = 0;
}
}
}


	if (Player[nPlayerIdx].m_ItemList.GetEquipmentMoney() < (m_Item[nIdx].GetPrice()+nMoneyMapVG))
		return FALSE;

	int nItemIdx = ItemSet.Add(&m_Item[nIdx]);
	if (!nItemIdx)
		return FALSE;

	Player[nPlayerIdx].Pay(m_Item[nIdx].GetPrice()+nMoneyMapVG);
	Player[nPlayerIdx].m_ItemList.Add(nItemIdx, nPlace, nX, nY);


if (SubWorld[Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex].m_bCheckTong)
{



try
	{
	
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript("\\script\\admin\\banghoi\\banghoi.lua");
		if (pScript)
		{

			int nTopIndex = 0;
			
			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction("AddMoneyMain",0, "dsd",SubWorld[Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex].m_dwTongName,SubWorld[Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex].m_szTongNameBC,nMoneyMapVG));
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);

			
		}
		
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[\\script\\admin\\banghoi\\banghoi.lua]!!!!!");
	}





}





}





	return TRUE;
}

/*******************************************************************************
²ÎÊý nIdx Ö¸ÓÎÏ·ÀïItemÊý×éµÄ±àºÅ
*******************************************************************************/
BOOL KBuySell::Sell(int nPlayerIdx, int nBuy, int nIdx)
{
	KASSERT(nPlayerIdx >= 0 && nPlayerIdx < MAX_PLAYER);
	KASSERT(nIdx >= 0 && nIdx < MAX_ITEM);

	if (Player[nPlayerIdx].m_bBlock)
	{
	BYTE	byFinished = s2c_itemexchangefinish;
	if (g_pServer)
		g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &byFinished, sizeof(BYTE));
	return FALSE;
	}




	if (Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex == g_SubWorldSet.SearchWorld(43))
	{
	BYTE	byFinished = s2c_itemexchangefinish;
	if (g_pServer)
		g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &byFinished, sizeof(BYTE));
	return FALSE;
	}






	if (nIdx <= 0 || nIdx >= MAX_ITEM)
	{
	return FALSE;
	}

	if (Item[nIdx].GetGenre() == item_task)
		return FALSE;

    if ((Item[nIdx].GetGenre() == item_equip || Item[nIdx].GetGenre() == item_medicine) && Item[nIdx].m_GeneratorParam.nVersion > 1)
		return FALSE;

	if (Item[nIdx].GetGenre() == item_equip && Item[nIdx].GetDurability() == 0)
		return FALSE;
		
	if (Npc[Player[nPlayerIdx].m_nIndex].m_FightMode)
        return FALSE;

	if (nBuy != Player[nPlayerIdx].m_BuyInfo.m_nBuyIdx)
	{
		g_DebugLog("BuySell: %s buy idx error!", Npc[Player[nPlayerIdx].m_nIndex].Name);
		return FALSE;
	}
	int nMoney = Item[nIdx].GetPrice();
	nMoney /= BUY_SELL_SCALE;
	if (nMoney <= 0)
		nMoney = 0;
	ItemSet.m_checkdel = 11;
	Player[nPlayerIdx].Earn(nMoney);
	Player[nPlayerIdx].m_ItemList.Remove(nIdx);
	ItemSet.Remove(nIdx);
	return TRUE;
}
#endif

#ifndef _SERVER
void KBuySell::PaintItem(int nIdx, int nX, int nY)
{
	int nShop = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
	if (nShop < 0 || nShop >= m_Height)
		return;
	int nItemIdx = GetItemIndex(nShop, nIdx);

	int x = nX;
	int y = nY;

	KItem* pItem = GetItem(nItemIdx);

	if (pItem)
	{
		pItem->Paint(x, y);
	}
}

void KBuySell::OpenSale(int nShop,int nSlect)
{
	if (nShop < 0 || nShop >= m_Height)
		return;
	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx = nShop;
	CoreDataChanged(GDCNI_NPC_TRADE, nSlect, TRUE);
}
#endif

#ifdef _SERVER
void KBuySell::OpenSale(int nPlayerIdx, int nShop,int nSlect)
{
	if (nPlayerIdx <= 0 || nPlayerIdx > MAX_PLAYER)
	{
		return;
	}

	Player[nPlayerIdx].m_BuyInfo.m_nBuyIdx = nShop;
	Player[nPlayerIdx].m_BuyInfo.m_SubWorldID = Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex;
	Npc[Player[nPlayerIdx].m_nIndex].GetMpsPos(
		&Player[nPlayerIdx].m_BuyInfo.m_nMpsX,
		&Player[nPlayerIdx].m_BuyInfo.m_nMpsY);

	SALE_BOX_SYNC saleSync;
	saleSync.ProtocolType = s2c_opensalebox;
	saleSync.nShopIndex = nShop;
	saleSync.nSlect = (BYTE) nSlect;
	g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &saleSync, sizeof(SALE_BOX_SYNC));
}
#endif
