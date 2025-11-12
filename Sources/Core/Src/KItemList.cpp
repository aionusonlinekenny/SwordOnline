#include	"KCore.h"
#include	"MyAssert.H"
#include	"KItem.h"
#include	"KItemSet.h"
#include	"KNpc.h"
#include	"KMath.h"
#include	"KPlayer.h"
#include	"KItemList.h"
#include	"KItemChangeRes.h"
#include	<time.h>
#ifdef _SERVER
//#include	"../../Headers/IServer.h"
#include	"KObjSet.h"
#endif
#ifndef _SERVER
#include	"CoreShell.h"
#include	"../../Headers/IClient.h"
#endif

// PKËÀÍö³Í·£µô´©µÄ×°±¸£¬¸÷×°±¸µôÂäµÄÈ¨Öµ
#define		defEQUIP_POWER
#ifdef defEQUIP_POWER
	int		g_nEquipPower[itempart_num] =
	{2, 4, 2, 4, 1, 1, 1, 1, 1, 1, 0};
#endif

int KItemList::ms_ActiveEquip[itempart_num][MAX_ITEM_ACTIVE] = 
{
	{ itempart_foot, itempart_ring1	},	//	itempart_head = 0,
	{ itempart_weapon, itempart_head },	//	itempart_body,
	{ itempart_body, itempart_amulet },	//	itempart_belt,
	{ itempart_ring1, itempart_foot },	//	itempart_weapon,
	{ itempart_cuff, itempart_pendant },//	itempart_foot,
	{ itempart_belt, itempart_ring2 },	//	itempart_cuff,
	{ itempart_head, itempart_weapon },	//	itempart_amulet,
	{ itempart_cuff, itempart_pendant },//	itempart_ring1,
	{ itempart_amulet, itempart_body },	//	itempart_ring2,
	{ itempart_belt, itempart_ring2 },	//	itempart_pendant,
	{ itempart_horse, itempart_horse },	//	itempart_horse,
	{ itempart_mask, itempart_mask },	//	itempart_mask,
	{ itempart_fifong, itempart_fifong },//	itempart_fifong,
};

int KItemList::ms_ActivedEquip[itempart_num][MAX_ITEM_ACTIVE] =
{
	{ itempart_body, itempart_amulet },	//	itempart_head = 0,
	{ itempart_ring2, itempart_belt },	//	itempart_body,
	{ itempart_pendant, itempart_cuff },//	itempart_belt,
	{ itempart_amulet, itempart_body },	//	itempart_weapon,
	{ itempart_weapon, itempart_head },	//	itempart_foot,
	{ itempart_foot, itempart_ring1 },	//	itempart_cuff,
	{ itempart_belt, itempart_ring2 },	//	itempart_amulet,
	{ itempart_weapon, itempart_head },	//	itempart_ring1,
	{ itempart_cuff, itempart_pendant },//	itempart_ring2,
	{ itempart_foot, itempart_ring1 },	//	itempart_pendant,
	{ itempart_horse, itempart_horse },	//	itempart_horse,
	{ itempart_mask, itempart_mask },	//	itempart_mask,
	{ itempart_fifong, itempart_fifong },//	itempart_fifong,
};

KItemList::KItemList()
{
	m_PlayerIdx = 0;
	m_nListCurIdx = 0;
	m_KickAllMacatric = 0;
}

KItemList::~KItemList()
{

}


/*!*****************************************************************************
// Function		: KItemList::GetWeaponType
// Purpose		: È¡µÃÍæ¼Ò×°±¸µÄÎäÆ÷ÀàÐÍ
// Return		: int 
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemList::GetWeaponType()
{
	if (m_EquipItem[itempart_weapon] && Item[m_EquipItem[itempart_weapon]].GetDurability())
		return Item[m_EquipItem[itempart_weapon]].GetDetailType();
	else
		return -1;
}

void KItemList::GetWeaponDamage(int* nMin, int* nMax)
{
	int nWeaponIdx = m_EquipItem[itempart_weapon];
	if (nWeaponIdx && Item[nWeaponIdx].GetDurability())
	{
		_ASSERT(Item[nWeaponIdx].m_CommonAttrib.nItemGenre == item_equip 
			&& (Item[nWeaponIdx].m_CommonAttrib.nDetailType == equip_meleeweapon
			|| Item[nWeaponIdx].m_CommonAttrib.nDetailType == equip_rangeweapon));
		int nMinDamage, nMaxDamage, nEnhance;
		int nDamageMinBase = Item[nWeaponIdx].m_aryBaseAttrib[0].nValue[0];
		int	nDamageMaxBase = Item[nWeaponIdx].m_aryBaseAttrib[1].nValue[0];
		nMinDamage = 0;
		nMaxDamage = 0;
		nEnhance = 0;
		for (int i = 0; i < 6; i++)
		{
			switch(Item[nWeaponIdx].m_aryMagicAttrib[i].nAttribType)
			{
			case magic_weapondamagemin_v:
				nMinDamage += Item[nWeaponIdx].m_aryMagicAttrib[i].nValue[0];
				break;
			case magic_weapondamagemax_v:
				nMaxDamage += Item[nWeaponIdx].m_aryMagicAttrib[i].nValue[0];
				break;
			case magic_weapondamageenhance_p:
				nEnhance += Item[nWeaponIdx].m_aryMagicAttrib[i].nValue[0];
				break;
			default:
				break;
			}
		}
		*nMin = (nDamageMinBase + nMinDamage) * (100 + nEnhance) / 100;
		*nMax = (nDamageMaxBase + nMaxDamage) * (100 + nEnhance) / 100;
	}
	else	// ¿ÕÊÖ
	{
		/*
		int nDamageBase = Player[m_PlayerIdx].m_nCurStrength * Player[m_PlayerIdx].m_nCurDexterity;
		*nMin = nDamageBase >> 9;
		*nMax = nDamageBase >> 8;
		*/
		// ÊýÖµ¼ÆËã·½·¨ÐÞ¸Ä£¨ËïÓ¢ÒªÇó£© by Spe 03/06/11
		_ASSERT(STRENGTH_SET_DAMAGE_VALUE > 0);
		*nMin = Player[m_PlayerIdx].m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE + 1;
		*nMax = Player[m_PlayerIdx].m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE + 1;
	}
}
/*!*****************************************************************************
// Function		: KItemList::Add
// Purpose		: Íæ¼ÒµÃµ½Ò»¸ö×°±¸
// Return		: int 
// Argumant		: int nIdx		ItemÊý×éµÄidx
// Argumant		: int nPlace
// Argumant		: int nX
// Argumant		: int nY
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemList::Add(int nIdx, int nPlace, int nX, int nY)
{
	if (nIdx <= 0)
		return 0;

	if (Item[nIdx].GetID() == 0)
		return 0;

	int i = FindFree();
	if (!i)
		return 0;

	switch(nPlace)
	{
	case pos_hand:
		if (m_Hand)
			return 0;
		m_Items[i].nPlace = pos_hand;
		m_Items[i].nX = 0;
		m_Items[i].nY = 0;
		m_Hand = nIdx;
		break;
	case pos_equip:
		if (nX < 0 || nX >= itempart_num)
			return 0;
		if (m_EquipItem[nX] > 0)
			return 0;

		if (m_EquipItem[nX] < 0)
		printf("Xay ra loi gia tri equip !");

//	ÓÐ¿ÉÄÜÊÇÌõ¼þ²»¹»£¬±»ÆäËû×°±¸´ÕÉÏµÄ¡£ÊÇ·ñÄÜ×°±¸Ó¦¸Ã·Åµ½EquipÊ±¼ì²é
//		if (!CanEquip(nIdx, nX))
//			return 0;
		m_Items[i].nPlace = pos_equip;
		m_Items[i].nX = nX;
		m_Items[i].nY = 0;
		break;

	case pos_pgbox:
		if (nX < 0 || nX >= MAX_PGBOX_ITEM)
			return 0;
		if (m_PGBoxItem[nX] > 0)
			return 0;

		if (m_PGBoxItem[nX] < 0)
		printf("Xay ra loi gia tri pgbox !");

		m_Items[i].nPlace = pos_pgbox;
		m_Items[i].nX = nX;
		m_Items[i].nY = 0;
		break;

	case pos_equiproom:
		if (!m_Room[room_equipment].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_equiproom;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;
		break;
#ifndef _SERVER
	case pos_trade1:
		if ( !Player[CLIENT_PLAYER_INDEX].CheckTrading() )
			return 0;
		if (!m_Room[room_trade1].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_trade1;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;
		break;

	case pos_buyshop:
		if (Player[CLIENT_PLAYER_INDEX].CheckTrading() )
			return 0;
		if (!m_Room[room_buyshop].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_buyshop;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;
		break;
		

#endif

	case pos_repositoryroom:
		if (!m_Room[room_repository].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_repositoryroom;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;

	case pos_rpnew:
		if (!m_Room[room_rpnew].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_rpnew;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;

	case pos_immediacy:
		if (!m_Room[room_immediacy].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_immediacy;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;

	case pos_checkbox:
		if (!m_Room[room_checkbox].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_checkbox;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;

	default:
		return 0;
	}

	m_Items[i].nIdx = nIdx;
	m_FreeIdx.Remove(i);
	m_UseIdx.Insert(i);

	if (m_Items[i].nPlace == pos_equip)
	{
		Equip(m_Items[i].nIdx, nX);
	}

	if (m_Items[i].nPlace == pos_pgbox)
	{
		m_PGBoxItem[nX] = nIdx;
	}



#ifdef _SERVER
	ITEM_SYNC	sItem;
	sItem.ProtocolType = s2c_syncitem;
	sItem.m_ID = Item[nIdx].GetID();
	sItem.m_Genre = Item[nIdx].GetGenre();
	sItem.m_Detail = Item[nIdx].GetDetailType();
	sItem.m_Particur = Item[nIdx].GetParticular();
	sItem.m_Series = Item[nIdx].GetSeries();
	sItem.m_Level = Item[nIdx].GetLevel();
	sItem.m_nBindItem = Item[nIdx].GetBindItem();
	sItem.iShopCost = Item[nIdx].GetShopCost();
	sItem.iNgoaiTrang = Item[nIdx].GetNgoaiTrang();
	sItem.m_Luck = Item[nIdx].m_GeneratorParam.nLuck;
	sItem.m_btPlace = m_Items[i].nPlace;
	sItem.m_btX = m_Items[i].nX;
	sItem.m_btY = m_Items[i].nY;
	// Time Item
	sItem.m_Time.bYear = Item[nIdx].GetTime()->bYear;
	sItem.m_Time.bMonth = Item[nIdx].GetTime()->bMonth;
	sItem.m_Time.bDay = Item[nIdx].GetTime()->bDay;
	sItem.m_Time.bHour = Item[nIdx].GetTime()->bHour;
	// End
	sItem.m_nFifong = Item[nIdx].GetFiFongArmor();
	
	
	for (int j = 0; j < 6; j++)
		sItem.m_MagicLevel[j] = Item[nIdx].m_GeneratorParam.nGeneratorLevel[j];
	sItem.m_RandomSeed = Item[nIdx].m_GeneratorParam.uRandomSeed;
	sItem.m_Version = Item[nIdx].m_GeneratorParam.nVersion;
	sItem.m_Durability = Item[nIdx].GetDurability();
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sItem, sizeof(ITEM_SYNC));
#endif

#ifndef _SERVER
	KUiObjAtContRegion	pInfo;

	int PartConvert[itempart_num] = 
	{
		UIEP_HEAD,
		UIEP_BODY,
		UIEP_WAIST,
		UIEP_HAND,
		UIEP_FOOT,
		UIEP_FINESSE,
		UIEP_NECK,
		UIEP_FINGER1,
		UIEP_FINGER2,
		UIEP_WAIST_DECOR,
		UIEP_HORSE,
		UIEP_MASK, // MASK
		UIEP_PHIPHONG,
	};

	pInfo.Obj.uGenre = CGOG_ITEM;	//Ô´×°±¸
	pInfo.Obj.uId = nIdx;
	pInfo.Region.Width = Item[nIdx].GetWidth();
	pInfo.Region.Height = Item[nIdx].GetHeight();

	switch(nPlace)
	{
	case pos_immediacy:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_IMMEDIA_ITEM;
		break;
	case pos_hand:
		pInfo.eContainer = UOC_IN_HAND;
		break;
	case pos_equip:
		pInfo.Region.h = 0;
		pInfo.Region.v = PartConvert[nX];
		pInfo.eContainer = UOC_EQUIPTMENT;
		break;

	case pos_pgbox:
		pInfo.Region.h = 0;
		pInfo.Region.v = nX;
		pInfo.eContainer = UOC_PG_BOX;
		break;

	case pos_equiproom:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_ITEM_TAKE_WITH;
		break;
	case pos_repositoryroom:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_STORE_BOX;
		break;
	case pos_rpnew:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_RPNEW_BOX;
		break;

	case pos_trade1:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		break;

	case pos_buyshop:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		break;


	case pos_checkbox:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_CHECK_BOX;
		break;
	}
	if (nPlace != pos_trade1 && nPlace != pos_buyshop)
		CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo, 1);
	else if (nPlace == pos_trade1)
	{
		CoreDataChanged(GDCNI_TRADE_DESIRE_ITEM, (DWORD)&pInfo, 1);
	}
	else
	{
		CoreDataChanged(GDCNI_BUY_SHOP_DESIRE_ITEM, (DWORD)&pInfo, 1);
	}

#ifdef _DEBUG
	int nLoopIdx = 0;
	nLoopIdx = m_UseIdx.GetNext(nLoopIdx);
	g_DebugLog("[ITEM]Item Begin");
	while(nLoopIdx)
	{
		g_DebugLog("[ITEM]ItemListIdx:%d, Item:%d, ItemId:%d", nLoopIdx, m_Items[nLoopIdx].nIdx, Item[m_Items[nLoopIdx].nIdx].GetID());
		nLoopIdx = m_UseIdx.GetNext(nLoopIdx);
	}
#endif
#endif
	return i;
}

#ifdef _SERVER
void KItemList::Resend()
{
	int nLoopIdx = 0;
	nLoopIdx = m_UseIdx.GetNext(nLoopIdx);
	while(nLoopIdx)
	{
	int nIdx = m_Items[nLoopIdx].nIdx;

	ITEM_SYNC	sItem;
	sItem.ProtocolType = s2c_syncitem;
	sItem.m_ID = Item[nIdx].GetID();
	sItem.m_Genre = Item[nIdx].GetGenre();
	sItem.m_Detail = Item[nIdx].GetDetailType();
	sItem.m_Particur = Item[nIdx].GetParticular();
	sItem.m_Series = Item[nIdx].GetSeries();
	sItem.m_Level = Item[nIdx].GetLevel();
	sItem.m_nBindItem = Item[nIdx].GetBindItem();
	sItem.iShopCost = Item[nIdx].GetShopCost();
	sItem.iNgoaiTrang = Item[nIdx].GetNgoaiTrang();
	sItem.m_Luck = Item[nIdx].m_GeneratorParam.nLuck;

	sItem.m_btPlace = m_Items[nLoopIdx].nPlace;
	sItem.m_btX = m_Items[nLoopIdx].nX;
	sItem.m_btY = m_Items[nLoopIdx].nY;
	// Time Item
	sItem.m_Time.bYear = Item[nIdx].GetTime()->bYear;
	sItem.m_Time.bMonth = Item[nIdx].GetTime()->bMonth;
	sItem.m_Time.bDay = Item[nIdx].GetTime()->bDay;
	sItem.m_Time.bHour = Item[nIdx].GetTime()->bHour;
	// End
	sItem.m_nFifong = Item[nIdx].GetFiFongArmor();
	
	
	for (int j = 0; j < 6; j++)
		sItem.m_MagicLevel[j] = Item[nIdx].m_GeneratorParam.nGeneratorLevel[j];
	sItem.m_RandomSeed = Item[nIdx].m_GeneratorParam.uRandomSeed;
	sItem.m_Version = Item[nIdx].m_GeneratorParam.nVersion;
	sItem.m_Durability = Item[nIdx].GetDurability();
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sItem, sizeof(ITEM_SYNC));

		nLoopIdx = m_UseIdx.GetNext(nLoopIdx);
	}
}
#endif

/*!*****************************************************************************
// Function		: KItemList::Remove
// Purpose		: Íæ¼ÒÊ§È¥Ò»¸ö×°±¸
// Return		: int 
// Argumant		: int nGameIdxÎªÓÎÏ·ÊÀ½çÖÐµÀ¾ßÊý×éµÄ±àºÅ
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KItemList::Remove(int nGameIdx,BOOL bcheck)
{
	if (!nGameIdx)
		return FALSE;

	int nIdx = FindSame(nGameIdx);

	if (!nIdx)
		return FALSE;


	int i = 0;


#ifdef _SERVER
	
	if (!bcheck)
	{

	if (Item[nGameIdx].GetGenre() == item_equip && Item[nGameIdx].m_GeneratorParam.nVersion > 1)
	{
    printf("Del Item List Khoa Loi !\n");
	return FALSE;
	}

	}

#endif


#ifdef _SERVER
	ITEM_REMOVE_SYNC	sRemove;
	sRemove.ProtocolType = s2c_removeitem;
	sRemove.m_ID = Item[nGameIdx].m_dwID;
#endif

	switch(m_Items[nIdx].nPlace)
	{
	case pos_hand:
		m_Hand = 0;
		break;
	case pos_equip:
		UnEquip(m_Items[nIdx].nIdx);
		break;

	case pos_pgbox:

	for (i = 0; i < MAX_PGBOX_ITEM; i++)
		{
			if (m_PGBoxItem[i] == m_Items[nIdx].nIdx)
			{
				break;
			}
		}
	
	if (i == MAX_PGBOX_ITEM)
			break;


	m_PGBoxItem[i] = 0;

         break;

	case pos_immediacy:
		m_Room[room_immediacy].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_equiproom:
		m_Room[room_equipment].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_repositoryroom:
		m_Room[room_repository].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;









	case pos_rpnew:
		m_Room[room_rpnew].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;

	





	case pos_traderoom:
		m_Room[room_trade].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
#ifndef _SERVER
	case pos_trade1:
		if ( !Player[CLIENT_PLAYER_INDEX].CheckTrading() )
		{
			_ASSERT(0);
		}
		m_Room[room_trade1].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;



	case pos_buyshop:
		if ( Player[CLIENT_PLAYER_INDEX].CheckTrading() )
		{
			_ASSERT(0);
		}
		m_Room[room_buyshop].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;


#endif


	case pos_checkbox:
		m_Room[room_checkbox].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;




	default:
		return FALSE;
	}
#ifndef _SERVER
	// ¿Í»§¶Ë´ÓÍæ¼ÒÉíÉÏÈ¥³ý×°±¸¾ÍÓ¦¸Ã´Ó×°±¸±íÖÐÈ¥³ýµô¡£
	ItemSet.Remove(m_Items[nIdx].nIdx);

	// ½çÃæ´¦Àí
	KUiObjAtContRegion pInfo;

	int PartConvert[itempart_num] = 
	{
		UIEP_HEAD,
		UIEP_BODY,
		UIEP_WAIST,
		UIEP_HAND,
		UIEP_FOOT,
		UIEP_FINESSE,
		UIEP_NECK,
		UIEP_FINGER1,
		UIEP_FINGER2,
		UIEP_WAIST_DECOR,
		UIEP_HORSE,
		UIEP_MASK, // MASK
		UIEP_PHIPHONG,
	};

	pInfo.Obj.uGenre = CGOG_ITEM;
	pInfo.Obj.uId = m_Items[nIdx].nIdx;
	pInfo.Region.Width = Item[m_Items[nIdx].nIdx].GetWidth();
	pInfo.Region.Height = Item[m_Items[nIdx].nIdx].GetHeight();

	switch(m_Items[nIdx].nPlace)
	{
	case pos_hand:
		pInfo.Obj.uGenre = CGOG_NOTHING;
		pInfo.Region.h = 0;
		pInfo.Region.v = 0;
		pInfo.eContainer = UOC_IN_HAND;
		break;
	case pos_equiproom:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_ITEM_TAKE_WITH;
		break;
	case pos_repositoryroom:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_STORE_BOX;
		break;





	case pos_rpnew:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_RPNEW_BOX;
		break;
		




	case pos_immediacy:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_IMMEDIA_ITEM;
		break;
	case pos_equip:
		pInfo.Region.h = 0;
		pInfo.Region.v = PartConvert[m_Items[nIdx].nX];
		pInfo.eContainer = UOC_EQUIPTMENT;
		break;

    case pos_pgbox:
		pInfo.Region.h = 0;
		pInfo.Region.v = m_Items[nIdx].nX;
		pInfo.eContainer = UOC_PG_BOX;
		break;


	case pos_trade1:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		break;

	case pos_buyshop:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		break;


	case pos_checkbox:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_CHECK_BOX;
		break;
	}
	if (m_Items[nIdx].nPlace != pos_trade1 && m_Items[nIdx].nPlace != pos_buyshop)
	{
		CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo, 0);
	}
	else if (m_Items[nIdx].nPlace == pos_trade1)
	{
		CoreDataChanged(GDCNI_TRADE_DESIRE_ITEM, (DWORD)&pInfo, 0);
	}
	else
	{
		CoreDataChanged(GDCNI_BUY_SHOP_DESIRE_ITEM, (DWORD)&pInfo, 0);
	}


#endif
	m_Items[nIdx].nIdx = 0;
	m_Items[nIdx].nPlace = 0;
	m_Items[nIdx].nX = 0;
	m_Items[nIdx].nY = 0;
	m_FreeIdx.Insert(nIdx);
	m_UseIdx.Remove(nIdx);

#ifdef _SERVER
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sRemove, sizeof(ITEM_REMOVE_SYNC));
#endif
	return TRUE;
}

/*!*****************************************************************************
// Function		: KItemList::FindFree
// Purpose		: ²éÕÒ¿ÉÓÃ¿ÕË÷Òý
// Return		: int 
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemList::FindFree()
{
	return m_FreeIdx.GetNext(0);
}

/*!*****************************************************************************
// Function		: KItemList::FindSame
// Purpose		: 
// Return		: int 
// Argumant		: int nGameIdx
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemList::FindSame(int nGameIdx)
{
	int nIdx = 0;
	while(1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (!nIdx)
			break;

		if (m_Items[nIdx].nIdx == nGameIdx)
			return nIdx;
	}
	return 0;
}

BOOL KItemList::CheckHaveItem(int nGameIdx)
{

	int nIdx = 0;

	while(1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (!nIdx)
			break;

		if (m_Items[nIdx].nIdx == nGameIdx)
			return TRUE;
	}

	return FALSE;
}



/*!*****************************************************************************
// Function		: KItemList::Init
// Purpose		: ³õÊ¼»¯Íæ¼Ò×°±¸ÁÐ±í
// Return		: BOOL
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KItemList::Init(int nPlayerIdx)
{
	m_PlayerIdx = nPlayerIdx;
	m_Hand = 0;
	m_nBackHand = 0;
	m_nMaskLock = FALSE;
	// TODO: Maybe we can read size from ini file.
//	int nWidth = 6;
//	int nHeight = 10;
//	m_Room[room_equipment].Init(nWidth, nHeight);
//	m_Room[room_repository].Init(nWidth, nHeight);
//	m_Room[room_trade].Init(nWidth, nHeight);
//	m_Room[room_tradeback].Init(nWidth, nHeight);
	m_Room[room_equipment].Init(EQUIPMENT_ROOM_WIDTH, EQUIPMENT_ROOM_HEIGHT);
	m_Room[room_repository].Init(REPOSITORY_ROOM_WIDTH, REPOSITORY_ROOM_HEIGHT);



	m_Room[room_rpnew].Init(RPNEW_ROOM_WIDTH, RPNEW_ROOM_HEIGHT);


	m_Room[room_trade].Init(TRADE_ROOM_WIDTH, TRADE_ROOM_HEIGHT);
	m_Room[room_tradeback].Init(EQUIPMENT_ROOM_WIDTH, EQUIPMENT_ROOM_HEIGHT);
	m_Room[room_immediacy].Init(IMMEDIACY_ROOM_WIDTH, IMMEDIACY_ROOM_HEIGHT);

	m_Room[room_checkbox].Init(CHECKBOX_ROOM_WIDTH, CHECKBOX_ROOM_HEIGHT);

#ifndef _SERVER
	m_Room[room_trade1].Init(TRADE_ROOM_WIDTH, TRADE_ROOM_HEIGHT);		// Õâ¸öµÄ´óÐ¡±ØÐëÓë room_trade µÄ´óÐ¡Ò»Ñù
	m_Room[room_buyshop].Init(EQUIPMENT_ROOM_WIDTH, EQUIPMENT_ROOM_HEIGHT);	
#endif
	ZeroMemory(m_EquipItem, sizeof(m_EquipItem));				// Íæ¼Ò×°±¸µÄµÀ¾ß£¨¶ÔÓ¦ÓÎÏ·ÊÀ½çÖÐµÀ¾ßÊý×éµÄË÷Òý£©
	ZeroMemory(m_PGBoxItem, sizeof(m_PGBoxItem));
	ZeroMemory(m_Items, sizeof(m_Items));						// Íæ¼ÒÓµÓÐµÄËùÓÐµÀ¾ß£¨°üÀ¨×°±¸×ÅµÄºÍÏä×ÓÀï·ÅµÄ£¬¶ÔÓ¦ÓÎÏ·ÊÀ½çÖÐµÀ¾ßÊý×éµÄË÷Òý£©
	m_nListCurIdx = 0;											// ÓÃÓÚ GetFirstItem ºÍ GetNextItem
	
	m_FreeIdx.Init(MAX_PLAYER_ITEM);
	m_UseIdx.Init(MAX_PLAYER_ITEM);

	for (int i = MAX_PLAYER_ITEM - 1; i > 0 ; i--)
	{
		m_FreeIdx.Insert(i);
	}
	return TRUE;
}

/*!*****************************************************************************
// Function		: KItemList::CanEquip
// Purpose		: 
// Return		: BOOL 
// Argumant		: int nIdx
// Argumant		: int nPlace
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KItemList::CanEquip(int nIdx, int nPlace)
{
	if (m_PlayerIdx <= 0 || nIdx <= 0 || nIdx >= MAX_ITEM || Item[nIdx].GetGenre() != item_equip)
		return FALSE;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	KMagicAttrib* pData = NULL;

	if (nPlace != -1 && !Fit(nIdx, nPlace))
	{
		return FALSE;
	}

	int nCount = 0;
	do
	{
		pData = (KMagicAttrib*)Item[nIdx].GetRequirement(nCount);
		if (pData && !EnoughAttrib(pData))
			return FALSE;
		nCount++;
	} while (pData != NULL);
	return TRUE;
}

BOOL KItemList::CanEquip(KItem* pItem, int nPlace /* = -1 */)
{
	if (m_PlayerIdx <= 0 || !pItem)
		return FALSE;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	KMagicAttrib* pData = NULL;

	if (nPlace != -1 && !Fit(pItem, nPlace))
	{
		return FALSE;
	}

	int nCount = 0;
	do
	{
		pData = (KMagicAttrib*)pItem->GetRequirement(nCount);
		if (pData && !EnoughAttrib(pData))
			return FALSE;
		nCount++;
	} while (pData != NULL);
	return TRUE;
}

BOOL KItemList::EnoughAttrib(void* pAttrib)
{
	KMagicAttrib*	pData = (KMagicAttrib *)pAttrib;
	_ASSERT(pData);
	switch(pData->nAttribType)
	{
	case magic_requirestr:
		if (Player[m_PlayerIdx].m_nCurStrength < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requiredex:
		if (Player[m_PlayerIdx].m_nCurDexterity < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requirevit:
		if (Player[m_PlayerIdx].m_nCurVitality < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requireeng:
		if (Player[m_PlayerIdx].m_nCurEngergy < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requirelevel:
		if (Npc[Player[m_PlayerIdx].m_nIndex].m_Level < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requiremenpai:
		if (Player[m_PlayerIdx].m_cFaction.m_nCurFaction != pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requireseries:
		if (Npc[Player[m_PlayerIdx].m_nIndex].m_Series != pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requiresex:
		if (Npc[Player[m_PlayerIdx].m_nIndex].m_nSex != pData->nValue[0])
		{
			return FALSE;
		}
		break;
	default:
		break;
	}
	return TRUE;
}

int KItemList::HaveDamageItem(int nDur)
{
	int nIndex = 0;
	for (int i = 0; i < itempart_num; i ++)
	{
		nIndex = GetEquipment(i);
		if (Item[nIndex].GetDurability() >= 0 && 
			Item[nIndex].GetMaxDurability() > 0 && 
			Item[nIndex].GetDurability() < nDur)
		{
			return nIndex;
		}
	}
	return 0;
}

/*!*****************************************************************************
// Function		: KItemList::Equip
// Purpose		: 
// Return		: BOOL
// Argumant		: int nIdx£¬ÓÎÏ·ÊÀ½çÖÐµÄµÀ¾ßÊý×é±àºÅ£¬ÒªÇóÒ»¶¨ÊÇ×°±¸ÀàµÀ¾ß
// Argumant		: int nPlace£¬ÉíÉÏ×°±¸µÄÎ»ÖÃ£¬-1×Ô¶¯ÕÒÎ»ÖÃ
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KItemList::Equip(int nIdx, int nPlace /* = -1 */)
{
	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	if (m_PlayerIdx <= 0 || nIdx <= 0 || nNpcIdx <= 0 || item_equip != Item[nIdx].GetGenre())
		return FALSE;

	int nItemListIdx = FindSame(nIdx);
	if (!nItemListIdx)
	{
		_ASSERT(0);
		return FALSE;
	}

	int nEquipPlace = nPlace;
	if (-1 == nEquipPlace)
	{
		nEquipPlace = GetEquipPlace(Item[nIdx].GetDetailType());
	}
	else if (!Fit(nIdx, nEquipPlace))
	{
		return FALSE;
	}
	
	int nIdxItemFiFong = GetEquipment(itempart_fifong);
	int BodyIdx = GetEquipment(itempart_body);
	
	switch(nEquipPlace)
	{
	case itempart_head:
		Npc[nNpcIdx].m_HelmType = g_ItemChangeRes.GetHelmRes(Item[nIdx].GetParticular(), Item[nIdx].GetLevel());
		Npc[nNpcIdx].m_HelmTypeTemp = Npc[nNpcIdx].m_HelmType;
		break;
	case itempart_body:
	{	
		Npc[nNpcIdx].m_ArmorType = g_ItemChangeRes.GetArmorRes(Item[nIdx].GetParticular(), Item[nIdx].GetLevel());
		g_DebugLog("Test 1: %d",Item[nIdx].GetFiFongArmor());	
			if (nIdx != 0)
			{
				
				if (Npc[nNpcIdx].m_nSex == 0)
				{
					if (Item[nIdx].GetLevel() < 6)
						Npc[nNpcIdx].m_FifongType = 24;	 // Do
					else if (Item[nIdx].GetLevel() >= 6 && Item[nIdx].GetLevel() < 9)
						Npc[nNpcIdx].m_FifongType = 22;
					else
						Npc[nNpcIdx].m_FifongType = 23;	// Rach 24 Do 23 Den 22
					
					if (Item[nIdx].GetKind() == 1 && Item[nIdx].GetDetailType() == 2)
					{	if (Item[nIdx].GetParticular() == 30)
						{
							g_DebugLog("Check Ao: 1");
							Npc[nNpcIdx].m_FifongType = 23;
						}
						else if (Item[nIdx].GetParticular() == 29)
						{
							g_DebugLog("Check Ao: 2");
							Npc[nNpcIdx].m_FifongType = 24;
						}
						else if (Item[nIdx].GetParticular() == 31)
						
						{
							g_DebugLog("Check Ao: 2");
							Npc[nNpcIdx].m_FifongType = 22;
						}
					}
					g_DebugLog("Check Ao: %d %d %d",	Item[nIdx].GetDetailType(), Item[nIdx].GetParticular(), Npc[nNpcIdx].m_FifongType );
				}
				else
				{
					if (Item[nIdx].GetLevel() < 6)
						Npc[nNpcIdx].m_FifongType = 30;	 // Do
					else if (Item[nIdx].GetLevel() >= 6 && Item[nIdx].GetLevel() < 9)
						Npc[nNpcIdx].m_FifongType = 31;
					else
						Npc[nNpcIdx].m_FifongType = 29;	// Rach 24 Do 23 Den 22
						
					if (Item[nIdx].GetKind() == 1 && Item[nIdx].GetDetailType() == 2)
					{	if (Item[nIdx].GetParticular() == 30)
						Npc[nNpcIdx].m_FifongType = 29;
						else if (Item[nIdx].GetParticular() == 29)
						Npc[nNpcIdx].m_FifongType = 30;
						else if (Item[nIdx].GetParticular() == 31)
						Npc[nNpcIdx].m_FifongType = 31;
					}
				}
			}
		
			
			
			//g_DebugLog("Kiem tra %d %d %d", Npc[nNpcIdx].m_FifongType,nIdx,Item[nIdx].GetKind());
			if (Npc[nNpcIdx].m_FifongType > 0 && nIdx != 0 && m_EquipItem[itempart_fifong] != 0)
			{
				//g_DebugLog("Xac nhan thay doi FF!!! %d",Npc[nNpcIdx].m_FifongType);
				Npc[nNpcIdx].m_bFifongState = TRUE;
			}
			else
			{
				//g_DebugLog("Xac nhan xoa FF!!! %d",Npc[nNpcIdx].m_FifongType);
				Npc[nNpcIdx].m_FifongType = 0;
				Npc[nNpcIdx].m_bFifongState = FALSE;
			}
		if (nIdx == 32)
		{
			Npc[nNpcIdx].m_FifongType = 0;
			Npc[nNpcIdx].m_bFifongState = FALSE;
		}
		if (Item[nIdx].GetFiFongArmor() >0)
		{
			Npc[nNpcIdx].m_FifongType = Item[nIdx].GetFiFongArmor();
			Npc[nNpcIdx].m_bFifongState = TRUE;
		}	
	}	
		break;
	case itempart_weapon:			
		Npc[nNpcIdx].m_WeaponType = g_ItemChangeRes.GetWeaponRes(Item[nIdx].GetDetailType(), Item[nIdx].GetParticular(), Item[nIdx].GetLevel());
		Npc[nNpcIdx].m_WeaponTypeTemp = Npc[nNpcIdx].m_WeaponType;
		break;
	case itempart_mask: // MASK
	{
		//if (Item[nIdx].GetDurability() > 0)
		//Item[nIdx].SetDurability(Item[nIdx].GetDurability() - 1);
		if (!m_nMaskLock)
		{
			Npc[nNpcIdx].m_MaskType = Item[nIdx].GetBaseMagic();
		}
	}
		break;	
	case itempart_fifong:
			//Npc[nNpcIdx].m_FifongType = g_ItemChangeRes.GetFifongRes(Item[nIdx].GetParticular(), Item[nIdx].GetLevel());
			//g_DebugLog("Test Kind %d ",Item[m_EquipItem[itempart_body]].GetKind());
			if (m_EquipItem[itempart_body] != 0)
			{
				if (Npc[nNpcIdx].m_nSex == 0)
				{
					if (Item[m_EquipItem[itempart_body]].GetLevel() < 6)
						Npc[nNpcIdx].m_FifongType = 24;	 // Do
					else if (Item[m_EquipItem[itempart_body]].GetLevel() >= 6 && Item[m_EquipItem[itempart_body]].GetLevel() < 9)
						Npc[nNpcIdx].m_FifongType = 22;
					else
						Npc[nNpcIdx].m_FifongType = 23;	// Rach 24 Do 23 Den 22
					
					if (Item[m_EquipItem[itempart_body]].GetKind() == 1 && Item[m_EquipItem[itempart_body]].GetDetailType() == 2)
					{	if (Item[m_EquipItem[itempart_body]].GetParticular() == 30)
						Npc[nNpcIdx].m_FifongType = 23;
						else if (Item[m_EquipItem[itempart_body]].GetParticular() == 29)
						Npc[nNpcIdx].m_FifongType = 24;
						else if (Item[m_EquipItem[itempart_body]].GetParticular() == 31)
						Npc[nNpcIdx].m_FifongType = 22;
					}					
					//AAAAAAA
						
				}
				else
				{
					if (Item[m_EquipItem[itempart_body]].GetLevel() < 6)
						Npc[nNpcIdx].m_FifongType = 31;	 // Do
					else if (Item[m_EquipItem[itempart_body]].GetLevel() >= 6 && Item[m_EquipItem[itempart_body]].GetLevel() < 9)
						Npc[nNpcIdx].m_FifongType = 29;
					else
						Npc[nNpcIdx].m_FifongType = 30;	// Rach 24 Do 23 Den 22
						
					if (Item[m_EquipItem[itempart_body]].GetKind() == 1 && Item[m_EquipItem[itempart_body]].GetDetailType() == 2)
					{	if (Item[m_EquipItem[itempart_body]].GetParticular() == 30)
						Npc[nNpcIdx].m_FifongType = 30;
						else if (Item[m_EquipItem[itempart_body]].GetParticular() == 29)
						Npc[nNpcIdx].m_FifongType = 31;
						else if (Item[m_EquipItem[itempart_body]].GetParticular() == 31)
						Npc[nNpcIdx].m_FifongType = 29;
					}	
				}
			}
			
			if (Npc[nNpcIdx].m_FifongType > 0 && m_EquipItem[itempart_body] != 0 && nIdx != 0)
			{
				Npc[nNpcIdx].m_bFifongState = TRUE;
			}
			else
			{
				Npc[nNpcIdx].m_FifongType = 0;
				Npc[nNpcIdx].m_bFifongState = FALSE;
			}
			
			
			break;	
	case itempart_horse:
		Npc[nNpcIdx].m_HorseType = g_ItemChangeRes.GetHorseRes(Item[nIdx].GetParticular(), Item[nIdx].GetLevel());
		if (Npc[nNpcIdx].m_HorseType >= 0)
		{
			Npc[nNpcIdx].m_bRideHorse = TRUE;	
		}
		else
		{
			Npc[nNpcIdx].m_bRideHorse = FALSE;
		}
		break;
	default:
		break;
	}
	// ¸üÐÂ×°±¸×ÔÉí×ø±ê
#ifndef _SERVER	
   if (Item[nIdx].m_SpecialParam.uItemType == 1)
   {
	for (int k=0;k<Item[nIdx].m_SpecialParam.uItemGroupCount;k++)
	{
    Item[nIdx].m_GroupItemNameInfo[k][0] = 0;
	}

   }	
	Item[nIdx].m_Place = nEquipPlace; //noi tren nguoi cua item
#endif	
	m_EquipItem[nEquipPlace] = nIdx;
	m_Items[nItemListIdx].nPlace = pos_equip;
	m_Items[nItemListIdx].nX = nEquipPlace;
	m_Items[nItemListIdx].nY = 0;



	int nActive = GetEquipEnhance(nEquipPlace);

	if (nEquipPlace == itempart_horse)
	{
	if (Npc[nNpcIdx].m_bRideHorse)
	{
	Item[nIdx].ApplyMagicAttribToNPC(&Npc[nNpcIdx], nActive);
	InfectionNextEquip(nEquipPlace, TRUE);
	}
	}
	else if (nEquipPlace == itempart_head || nEquipPlace == itempart_body || nEquipPlace == itempart_belt || nEquipPlace == itempart_weapon || nEquipPlace == itempart_foot || nEquipPlace == itempart_cuff || nEquipPlace == itempart_mask)
	{

			if (Item[nIdx].GetDurability())
			{
			Item[nIdx].ApplyMagicAttribToNPC(&Npc[nNpcIdx], nActive);
			InfectionNextEquip(nEquipPlace, TRUE);
			
			}

	}
	else
	{
	Item[nIdx].ApplyMagicAttribToNPC(&Npc[nNpcIdx], nActive);
	InfectionNextEquip(nEquipPlace, TRUE);
	
	}




CheckDoGroupGoldItem();


	if (itempart_weapon == nEquipPlace)
	{		
#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].UpdateWeaponSkill();
#endif
		Player[m_PlayerIdx].SetNpcDamageAttrib();
	}



//#endif
	return TRUE;
}

/*!*****************************************************************************
// Function		: KItemList::InfectionNextEquip
// Purpose		: ×°±¸Ó°ÏìÆäËû×°±¸
// Return		: void 
// Argumant		: int nEquipPlace
// Argumant		: BOOL bEquipUp ÊÇ×°ÉÏ£¨TRUE£©»¹ÊÇÐ¶ÏÂ£¨FALSE£©
// Comments		:
// Author		: Spe
*****************************************************************************/





void KItemList::CheckDoGroupGoldItem()
{

int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

int	KickAllMacatricActive = 0;

for (int l = 0; l < itempart_num; l++)
{
int TypeItemActive = Item[m_EquipItem[l]].m_SpecialParam.uItemType;
if (!m_EquipItem[l] || TypeItemActive != 1)
	continue;





if (l == itempart_horse)
{
if (!Npc[nNpcIdx].m_bRideHorse)
	continue;
}

else if (l == itempart_head || l == itempart_body || l == itempart_belt || l == itempart_weapon || l == itempart_foot || l == itempart_cuff)
{

if (!Item[m_EquipItem[l]].GetDurability())
	continue;	 
}



	int GroupItemActive = Item[m_EquipItem[l]].m_SpecialParam.uItemGroup;
	int GroupItemActiveCount = Item[m_EquipItem[l]].m_SpecialParam.uItemGroupCount;
	int ItemDetailActive =  Item[m_EquipItem[l]].GetDetailType();
	int ItemParticularActive = Item[m_EquipItem[l]].GetParticular();
   	
	int ItemCountInGroupActive = 1;

if (GroupItemActive > 0 && GroupItemActiveCount > 0)
{

	
	for (int m = l+1; m < itempart_num; m++)
	{
	int TypeItemFind = Item[m_EquipItem[m]].m_SpecialParam.uItemType;
	if (!m_EquipItem[m] || TypeItemFind != 1)
	continue;




if (m == itempart_horse)
{
if (!Npc[nNpcIdx].m_bRideHorse)
	continue;
}

else if (m == itempart_head || m == itempart_body || m == itempart_belt || m == itempart_weapon || m == itempart_foot || m == itempart_cuff)
{

if (!Item[m_EquipItem[m]].GetDurability())
	continue;	 
}





	int GroupItemFind = Item[m_EquipItem[m]].m_SpecialParam.uItemGroup;
    int ItemDetailFind =  Item[m_EquipItem[m]].GetDetailType();
	int ItemParticularFind = Item[m_EquipItem[m]].GetParticular();

	if (GroupItemActive == GroupItemFind && (ItemParticularFind != ItemParticularActive || ItemDetailActive != ItemDetailFind))
	{



//////////////////////////


	BOOL bCheckC = FALSE;

	if (ItemDetailFind == 3)
	{
	for (int u = l+1; u < m; u++)
	{

    int TypeItemFindC = Item[m_EquipItem[u]].m_SpecialParam.uItemType;

	if (!m_EquipItem[u] || TypeItemFindC != 1)
	continue;



if (u == itempart_horse)
{
if (!Npc[nNpcIdx].m_bRideHorse)
	continue;
}

else if (u == itempart_head || u == itempart_body || u == itempart_belt || u == itempart_weapon || u == itempart_foot || u == itempart_cuff)
{

if (!Item[m_EquipItem[u]].GetDurability())
	continue;	 
}




	int GroupItemFindC = Item[m_EquipItem[u]].m_SpecialParam.uItemGroup;
	int ItemDetailFindC =  Item[m_EquipItem[u]].GetDetailType();
	int ItemParticularFindC = Item[m_EquipItem[u]].GetParticular();

	if (GroupItemFindC == GroupItemFind && ItemDetailFindC == ItemDetailFind && ItemParticularFindC == ItemParticularFind)
	{
	bCheckC = TRUE;
	break;
	}
	}
	}

	if (!bCheckC)
	{
    ItemCountInGroupActive++;
	}



////////////////////////////


	}
	}

	if (ItemCountInGroupActive >= GroupItemActiveCount)
	{
	KickAllMacatricActive = 1;
	break;
	}
}

}

if (m_KickAllMacatric != KickAllMacatricActive)
{

for (int i = 0; i < itempart_num; i++)
{

if (!m_EquipItem[i])
continue;




if (i == itempart_horse)
{
   if (Npc[nNpcIdx].m_bRideHorse)
	{
	Item[m_EquipItem[i]].RemoveMagicAttribFromNPC(&Npc[nNpcIdx],GetEquipEnhance(i));
	}
}

else if (i == itempart_head || i == itempart_body || i == itempart_belt || i == itempart_weapon || i == itempart_foot || i == itempart_cuff)
{

	if (Item[m_EquipItem[i]].GetDurability())
	{
	Item[m_EquipItem[i]].RemoveMagicAttribFromNPC(&Npc[nNpcIdx],GetEquipEnhance(i));
	}

}


else
{
	Item[m_EquipItem[i]].RemoveMagicAttribFromNPC(&Npc[nNpcIdx],GetEquipEnhance(i));
}





}

m_KickAllMacatric = KickAllMacatricActive;

for (int j = 0; j < itempart_num; j++)
{

if (!m_EquipItem[j])
continue;




if (j == itempart_horse)
{
   if (Npc[nNpcIdx].m_bRideHorse)
	{
	Item[m_EquipItem[j]].ApplyMagicAttribToNPC(&Npc[nNpcIdx],GetEquipEnhance(j));
	}
}

else if (j == itempart_head || j == itempart_body || j == itempart_belt || j == itempart_weapon || j == itempart_foot || j == itempart_cuff)
{

	if (Item[m_EquipItem[j]].GetDurability())
	{
	Item[m_EquipItem[j]].ApplyMagicAttribToNPC(&Npc[nNpcIdx],GetEquipEnhance(j));
	}

}


else
{
	Item[m_EquipItem[j]].ApplyMagicAttribToNPC(&Npc[nNpcIdx],GetEquipEnhance(j));
}




}



}





#ifndef _SERVER

for (int p = 0; p < itempart_num; p++)
{
if (!m_EquipItem[p] || Item[m_EquipItem[p]].m_SpecialParam.uItemType != 1)
continue;




if (p == itempart_horse)
{
if (!Npc[nNpcIdx].m_bRideHorse)
	continue;
}

else if (p == itempart_head || p == itempart_body || p == itempart_belt || p == itempart_weapon || p == itempart_foot || p == itempart_cuff)
{

if (!Item[m_EquipItem[p]].GetDurability())
	continue;	 
}








for (int q=0;q<Item[m_EquipItem[p]].m_SpecialParam.uItemGroupCount;q++)
{

Item[m_EquipItem[p]].m_GroupItemNameInfo[q][0]=0;

for (int k = 0; k < itempart_num; k++)
{
if (!m_EquipItem[k] || Item[m_EquipItem[k]].m_SpecialParam.uItemType != 1)
continue;


if (k == itempart_horse)
{
if (!Npc[nNpcIdx].m_bRideHorse)
	continue;
}

else if (k == itempart_head || k == itempart_body || k == itempart_belt || k == itempart_weapon || k == itempart_foot || k == itempart_cuff)
{

if (!Item[m_EquipItem[k]].GetDurability())
	continue;	 
}






if (Item[m_EquipItem[p]].m_GroupItemNameInfo[q][1] == Item[m_EquipItem[k]].GetDetailType() &&
	Item[m_EquipItem[p]].m_GroupItemNameInfo[q][2] == Item[m_EquipItem[k]].GetParticular())
{
Item[m_EquipItem[p]].m_GroupItemNameInfo[q][0]=1;
break;
}
}

}

}

#endif


}







void KItemList::InfectionNextEquip(int nEquipPlace, BOOL bEquip/* = FALSE */)
{
	if (m_PlayerIdx <= 0)
		return;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	for (int i = 0; i < MAX_ITEM_ACTIVE; i++)
	{
		int nInfectionEquip = ms_ActiveEquip[nEquipPlace][i];
		if (!m_EquipItem[nInfectionEquip])
			continue;


if (nInfectionEquip == itempart_horse)
{
if (!Npc[nNpcIdx].m_bRideHorse)
	continue;
}

else if (nInfectionEquip == itempart_head || nInfectionEquip == itempart_body || nInfectionEquip == itempart_belt || nInfectionEquip == itempart_weapon || nInfectionEquip == itempart_foot || nInfectionEquip == itempart_cuff)
{

if (!Item[m_EquipItem[nInfectionEquip]].GetDurability())
	continue;	 
}


		if (g_IsAccrue(Item[m_EquipItem[nEquipPlace]].GetSeries(),
			Item[m_EquipItem[nInfectionEquip]].GetSeries()) && m_KickAllMacatric == 0)
		{
			int nActive = GetEquipEnhance(nInfectionEquip);
			_ASSERT(nActive > 0);
			if (bEquip)
				Item[m_EquipItem[nInfectionEquip]].ApplyHiddenMagicAttribToNPC(&Npc[nNpcIdx], nActive);
			else
				Item[m_EquipItem[nInfectionEquip]].RemoveHiddenMagicAttribFromNPC(&Npc[nNpcIdx], nActive);
		}
	}
}
/*!*****************************************************************************
// Function		: KItemList::UnEquip
// Purpose		: ÒÆ³ý×°±¸
// Return		: BOOL 
// Argumant		: int nIdx ÓÎÏ·ÊÀ½çÖÐµÄµÀ¾ßÊý×éË÷Òý
// Comments		: 
// Author		: Spe
*****************************************************************************/
BOOL KItemList::UnEquip(int nIdx, int nPos/* = -1*/)
{
	int i = 0;
	if (m_PlayerIdx <= 0)
		return FALSE;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;
	if (nIdx <= 0)
		return FALSE;
	
	//if (nPos == 12) return FALSE;
	
	if (nPos <= 0)
	{
		for (i = 0; i < itempart_num; i++)
		{
			if (m_EquipItem[i] == nIdx)
			{
				break;
			}
		}
		// Ã»ÓÐ·¢ÏÖÉíÉÏÓÐÕâ¸ö×°±¸
		if (i == itempart_num)
			return FALSE;

	}
	else
	{
		if (m_EquipItem[nPos] != nIdx)	// ¶«Î÷²»¶Ô
			return FALSE;
		i = nPos;
	}
	// ÒÆ³ý¸Ã×°±¸¶ÔNPCµÄÊôÐÔµ÷Õû

	
    int nActive = GetEquipEnhance(i);
	
	if (i == itempart_head || i == itempart_body || i == itempart_belt || i == itempart_weapon || i == itempart_foot || i == itempart_cuff || i == itempart_mask)
	{

	if (Item[nIdx].GetDurability())
	{
		Item[nIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], nActive);
		InfectionNextEquip(i, FALSE);
	}
	

	}
	else if (i == itempart_horse)
	{
		if (Npc[nNpcIdx].m_bRideHorse) // ham tren ngua o day
		{
			Item[nIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], nActive);
			InfectionNextEquip(i, FALSE);
		}
	}
	else
	{
	g_DebugLog("remove khi thao item");
	Item[nIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], nActive);
	InfectionNextEquip(i, FALSE);
	}
	
	;
	int nIdxItemFiFong = GetEquipment(itempart_fifong);	

	
	
	// Õâ¾ä»°Ò»¶¨Òª·ÅÔÚÉÏÒ»¾äºó£¬±£Ö¤¼ÆËã¸Ã×°±¸¼¤»îµÄ×°±¸¼¤»îÊôÐÔ¸öÊý¼ÆËãµÄÕýÈ·ÐÔ
 #ifndef _SERVER 
	if (Item[nIdx].m_SpecialParam.uItemType == 1)
   {
	for (int k=0;k<Item[nIdx].m_SpecialParam.uItemGroupCount;k++)
	{
    Item[nIdx].m_GroupItemNameInfo[k][0] = 0;
	}

   }
	Item[nIdx].m_Place = -1; //noi tren nguoi cua item
#endif	
	m_EquipItem[i] = 0;

	switch(i)
	{
	case itempart_head:
		Npc[nNpcIdx].m_HelmType = g_ItemChangeRes.GetHelmRes(0, 0);
		Npc[nNpcIdx].m_HelmTypeTemp = Npc[nNpcIdx].m_HelmType;
		break;
	case itempart_body: //fifongnew
			Npc[nNpcIdx].m_ArmorType = g_ItemChangeRes.GetArmorRes(0, 0);
			Npc[nNpcIdx].m_FifongType = 0;
			Npc[nNpcIdx].m_bFifongState = FALSE;
			break;
	case itempart_weapon:
		Npc[nNpcIdx].m_WeaponType = g_ItemChangeRes.GetWeaponRes(0, 0, 0);
		Npc[nNpcIdx].m_WeaponTypeTemp = Npc[nNpcIdx].m_WeaponType;
		break;
	case itempart_mask: // MASK
		//if (Item[nIdx].GetDurability() > 0)
		//Item[nIdx].SetDurability(Item[nIdx].GetDurability() - 1);
		if (!m_nMaskLock)
		{
			Npc[nNpcIdx].m_MaskType = 0;
		}
	//	g_DebugLog("Animation: %d",Npc[nNpcIdx].m_MaskType);
		break;	
	case itempart_fifong: 
		{
			Npc[nNpcIdx].m_FifongType = 0;
			Npc[nNpcIdx].m_bFifongState = FALSE;
		}
		break;	
	case itempart_horse:	
		Npc[nNpcIdx].m_HorseType = -1;
		Npc[nNpcIdx].m_bRideHorse = FALSE;
		break;
	
	default:
		break;
	}


CheckDoGroupGoldItem();



	if (itempart_weapon == i)
	{
#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].UpdateWeaponSkill();
#endif
		Player[m_PlayerIdx].SetNpcDamageAttrib();
	}	
	return TRUE;
}

int KItemList::GetEquipPlace(int nType)
{
	int nRet = -1;
	switch(nType)
	{
	case equip_meleeweapon:
	case equip_rangeweapon:
		nRet = itempart_weapon;
		break;
	case equip_armor:
		nRet = itempart_body;
		break;
	case equip_helm:
		nRet = itempart_head;
		break;
	case equip_boots:
		nRet = itempart_foot;
		break;
	case equip_ring:
		nRet = itempart_ring1;
		break;
	case equip_amulet:
		nRet = itempart_amulet;
		break;
	case equip_belt:
		nRet = itempart_belt;
		break;
	case equip_cuff:
		nRet = itempart_cuff;
		break;
	case equip_pendant:
		nRet = itempart_pendant;
		break;
	case equip_horse:
		nRet = itempart_horse;
		break;
	case equip_mask: // MASK
		nRet = itempart_mask;
		break;
	case equip_fifong:
		nRet = itempart_fifong;
		break;	
	default:
		break;
	}
	return nRet;
}

/*!*****************************************************************************
// Function		: KItemList::Fit
// Purpose		: 
// Return		: BOOL 
// Argumant		: int nIdx
// Argumant		: int nPlace
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KItemList::Fit(int nIdx, int nPlace)
{
	BOOL	bRet = FALSE;
	_ASSERT(Item[nIdx].GetGenre() == item_equip);
	switch(Item[nIdx].GetDetailType())
	{
	case equip_meleeweapon:
	case equip_rangeweapon:
		if (nPlace == itempart_weapon)
			bRet = TRUE;
		break;
	case equip_armor:
		if (nPlace == itempart_body)
			bRet = TRUE;
		break;
	case equip_belt:
		if (nPlace == itempart_belt)
			bRet = TRUE;
		break;
	case equip_boots:
		if (nPlace == itempart_foot)
			bRet = TRUE;
		break;
	case equip_cuff:
		if (nPlace == itempart_cuff)
			bRet = TRUE;
		break;
	case equip_ring:
		if (nPlace == itempart_ring1 || nPlace == itempart_ring2)
			bRet = TRUE;
		break;
	case equip_amulet:
		if (nPlace == itempart_amulet)
			bRet = TRUE;
		break;
	case equip_pendant:
		if (nPlace == itempart_pendant)
			bRet = TRUE;
		break;
	case equip_helm:
		if (nPlace == itempart_head)
			bRet = TRUE;
		break;
	case equip_horse:
		if (nPlace == itempart_horse)
			bRet = TRUE;
		break;
	case equip_mask: // MASK
		if (nPlace == itempart_mask)
			bRet = TRUE;
		break;	
	case equip_fifong: // MASK
		if (nPlace == itempart_fifong)
			bRet = TRUE;
		break;	
	}
	return bRet;
}

BOOL KItemList::Fit(KItem* pItem, int nPlace)
{
	BOOL	bRet = FALSE;
	_ASSERT(pItem->GetGenre() == item_equip);
	switch(pItem->GetDetailType())
	{
	case equip_meleeweapon:
	case equip_rangeweapon:
		if (nPlace == itempart_weapon)
			bRet = TRUE;
		break;
	case equip_armor:
		if (nPlace == itempart_body)
			bRet = TRUE;
		break;
	case equip_belt:
		if (nPlace == itempart_belt)
			bRet = TRUE;
		break;
	case equip_boots:
		if (nPlace == itempart_foot)
			bRet = TRUE;
		break;
	case equip_cuff:
		if (nPlace == itempart_cuff)
			bRet = TRUE;
		break;
	case equip_ring:
		if (nPlace == itempart_ring1 || nPlace == itempart_ring2)
			bRet = TRUE;
		break;
	case equip_amulet:
		if (nPlace == itempart_amulet)
			bRet = TRUE;
		break;
	case equip_pendant:
		if (nPlace == itempart_pendant)
			bRet = TRUE;
		break;
	case equip_helm:
		if (nPlace == itempart_head)
			bRet = TRUE;
		break;
	case equip_horse:
		if (nPlace == itempart_horse)
			bRet = TRUE;
		break;
	case equip_mask: // MASK
		if (nPlace == itempart_mask)
			bRet = TRUE;
		break;
	case equip_fifong:
			if (nPlace == itempart_fifong)
				bRet = TRUE;
			break;	
	}
	return bRet;
}

int KItemList::GetEquipEnhance(int nPlace)
{
	if (m_PlayerIdx <= 0)
		return FALSE;
	
	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;
	
	if (nPlace < 0 || nPlace >= itempart_num)
	{
		_ASSERT(0);
		return 0;
	}
	
	if (m_KickAllMacatric == 1)
		return 3;
	
	int nRet = 0;

	int nSeries = Item[m_EquipItem[nPlace]].GetSeries();
	if (g_IsAccrue(Npc[nNpcIdx].m_Series, nSeries))
		nRet ++;
	for (int i = 0; i < MAX_ITEM_ACTIVE; i++)
	{
		// ¿´¿´¼¤»îÕâ¸ö×°±¸µÄ×°±¸ÖÐÊÇ·ñÏàÉúÕâ¸ö×°±¸
		int nActivedEquip = m_EquipItem[ms_ActivedEquip[nPlace][i]];
		if (nActivedEquip)
		{
			if (ms_ActivedEquip[nPlace][i] == itempart_horse)
			{
				if (!Npc[nNpcIdx].m_bRideHorse)
					continue;
			}
			
			else if (ms_ActivedEquip[nPlace][i] == itempart_head || ms_ActivedEquip[nPlace][i] == itempart_body || ms_ActivedEquip[nPlace][i] == itempart_belt || ms_ActivedEquip[nPlace][i] == itempart_weapon || ms_ActivedEquip[nPlace][i] == itempart_foot || ms_ActivedEquip[nPlace][i] == itempart_cuff)
			{
				
				if (!Item[nActivedEquip].GetDurability())
					continue;	 
			}
			
			
			if (g_IsAccrue(Item[nActivedEquip].GetSeries(), nSeries))
				nRet++;
		}
	}

	return nRet;
}

int KItemList::GetEquipEnhanceP(int nPlace)
{
	if (m_PlayerIdx <= 0)
		return FALSE;
	
	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;
	
	if (nPlace < 0 || nPlace >= itempart_num)
	{
		_ASSERT(0);
		return 0;
	}
	
	if (m_KickAllMacatric == 1)
		return 3;

	int nRet = 0;

	int nSeries = Item[m_EquipItem[nPlace]].GetSeries();
	if (g_IsAccrue(Npc[nNpcIdx].m_Series, nSeries))
		nRet ++;
	for (int i = 0; i < MAX_ITEM_ACTIVE; i++)
	{
		// ¿´¿´¼¤»îÕâ¸ö×°±¸µÄ×°±¸ÖÐÊÇ·ñÏàÉúÕâ¸ö×°±¸
		int nActivedEquip = m_EquipItem[ms_ActivedEquip[nPlace][i]];
		if (nActivedEquip)
		{
			if (g_IsAccrue(Item[nActivedEquip].GetSeries(), nSeries))
				nRet++;
		}
	}
	
	return nRet;
}

BOOL KItemList::EatMecidine(int nIdx)
{
	if (m_PlayerIdx <= 0)
		return FALSE;
	if (nIdx <= 0)
		return FALSE;
	
	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;
	int nGenre = Item[nIdx].GetGenre();
	int nLevel = Item[nIdx].GetLevel();
	
	//_ASSERT(nGenre == item_medicine || nGenre == item_townportal);
	
	if (nGenre == item_medicine)
	{
#ifdef _SERVER
		ItemSet.m_checkdel = 1;
		
		if (Item[nIdx].GetDetailType() == 8 || Item[nIdx].GetDetailType() == 9)
		{
			if (Npc[nNpcIdx].m_SubWorldIndex == g_SubWorldSet.SearchWorld(33) || Npc[nNpcIdx].m_SubWorldIndex == g_SubWorldSet.SearchWorld(37))
			{
				int nEfecSkId = (Item[nIdx].GetDetailType() - 8) * 5 + Item[nIdx].GetLevel() - 1 + 481;
				Npc[nNpcIdx].AddSkillEffect(nEfecSkId,1,5*60*18);
				Remove(nIdx);
				ItemSet.Remove(nIdx);
			}
			else
			{
				KPlayerChat::SendSystemInfo(1,m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Kh«ng thÓ sö dông vËt phÈm t¹i ®©y !",strlen("Kh«ng thÓ sö dông vËt phÈm t¹i ®©y !") );
			}
		}
		else
		{
			Item[nIdx].ApplyMagicAttribToNPC(&Npc[nNpcIdx], 3);
			Remove(nIdx);
			ItemSet.Remove(nIdx);
		}
#endif
		
#ifndef _SERVER
		if (Item[nIdx].GetDetailType() != 8 && Item[nIdx].GetDetailType() != 9)
		{
			if (Player[m_PlayerIdx].m_cTask.GetSaveVal(128) > 0 || nLevel != 4)
			{
				Item[nIdx].ApplyMagicAttribToNPC(&Npc[nNpcIdx], 3);
			}
			else
			{
				KSystemMessage Msg;	
				Msg.byConfirmType = SMCT_CLICK;
				Msg.byParamSize = 0;
				Msg.byPriority = 1;
				Msg.eType = SMT_PLAYER;
				strcpy(Msg.szMessage, "VËt phÈm kh«ng thÓ sö dông t¹i ®©y?");
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
			}
		}
#endif
	}
	
	if (nGenre == item_equip)
	{
#ifdef _SERVER
		//printf("[ %d %d %d %d %d ] [ %d %d %d %d %d %d %d %d %d ]\n",
			//Item[nIdx].GetGenre(),
			//Item[nIdx].GetDetailType(),
			//Item[nIdx].GetParticular(),
			//Item[nIdx].GetLevel(),
			//Item[nIdx].GetSeries(),
			//Item[nIdx].m_GeneratorParam.uRandomSeed,
			//Item[nIdx].m_GeneratorParam.nGeneratorLevel[0],
			//Item[nIdx].m_GeneratorParam.nGeneratorLevel[1],
			//Item[nIdx].m_GeneratorParam.nGeneratorLevel[2],
			//Item[nIdx].m_GeneratorParam.nGeneratorLevel[3],
			//Item[nIdx].m_GeneratorParam.nGeneratorLevel[4],
			//Item[nIdx].m_GeneratorParam.nGeneratorLevel[5],
			//Item[nIdx].m_GeneratorParam.nVersion,
			//Item[nIdx].m_GeneratorParam.nLuck
			//);

        #endif
	}




	if (nGenre == item_townportal)
	{
		if (!Npc[Player[m_PlayerIdx].m_nIndex].m_FightMode)
		{
#ifndef _SERVER	// ¿Í»§¶ËÏÈ¿´¿´ÊÇ²»ÊÇ·ÇÕ½¶·Çø£¬Èç¹ûÊÇµÄ»°¾Í±ð·¢¸øServerÁË¡£
			// ÏµÍ³ÏûÏ¢
			KSystemMessage Msg;
			
			Msg.byConfirmType = SMCT_CLICK;
			Msg.byParamSize = 0;
			Msg.byPriority = 1;
			Msg.eType = SMT_PLAYER;
			strcpy(Msg.szMessage, "Kh«ng thÓ sö dông khi ë trong thµnh ");
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
#endif
			return FALSE;
		}
		
#ifdef _SERVER
		ItemSet.m_checkdel = 2;
		Player[m_PlayerIdx].UseTownPortal();
		Remove(nIdx);
	    ItemSet.Remove(nIdx);
#endif
	}



   if (nGenre == item_task)
    {
#ifdef _SERVER


	   if (!Item[nIdx].IsStack() || (Item[nIdx].GetVersion() >= 1 && Item[nIdx].GetVersion() <= 200))
	   {
        char file_lua_chay[256];
        int id_queskey_dung = Item[nIdx].GetDetailType();
        sprintf(file_lua_chay,"%s%d%s","\\script\\item\\questkey\\",id_queskey_dung,".lua");
        Player[m_PlayerIdx].ExecuteScript(file_lua_chay,"main",nIdx);
	   }
	 
#endif
    }


	if (Npc[Player[m_PlayerIdx].m_nIndex].m_Doing == do_sit)
	{
		Npc[Player[m_PlayerIdx].m_nIndex].SendCommand(do_stand);
	}
	return TRUE;
}

#ifndef _SERVER
int KItemList::UseItem(int nIdx)
{
	if (m_PlayerIdx <= 0)
		return FALSE;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	if (0 == FindSame(nIdx))
	{
		return 0;
	}

	int		nRet = 0;
	switch(Item[nIdx].GetGenre())
	{
	case item_equip:
		break;
/*		if (Equip(nNpcIdx, nIdx))
			nRet = REQUEST_EQUIP_ITEM;*/
		break;
	case item_townportal:
	case item_task:
	case item_medicine:
		if (EatMecidine(nIdx))
			nRet = REQUEST_EAT_MEDICINE;
		break;
    case item_mine:
		if (EatMecidine(nIdx))
			nRet = REQUEST_EAT_MEDICINE;
		break;
	default:
		break;
	}
	return nRet;
}
#endif

#ifndef _SERVER
BOOL KItemList::SearchEquipment(int nWidth, int nHeight)
{
	if (nWidth < 0 || nHeight < 0)
	{
		return FALSE;
	}

	POINT	pPt;
	if (!m_Room[room_equipment].FindRoom(nWidth, nHeight, &pPt))
	{
		return FALSE;
	}
	return TRUE;
}

int KItemList::ChangeItemInPlayer(int nIdx)
{
	if (m_PlayerIdx <= 0)
		return FALSE;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	if (0 == FindSame(nIdx))
	{
		return 0;
	}
	int		nRet = Item[nIdx].GetDetailType();
	switch(nRet)
	{
		case itempart_head:
			break;
		case itempart_body:
			break;
		case itempart_belt:
			break;
		case itempart_weapon:
			break;
		case itempart_foot:
			break;
		case itempart_cuff:
			break;
		case itempart_amulet:
			break;
		case itempart_ring1:
			break;
		case itempart_ring2:
			break;
		case itempart_pendant:
			break;
		case itempart_mask:
			break;
		case itempart_num:
			break;
		case itempart_horse:
			break;
		default:
			break;
	}
	return nRet;
}
#endif

#ifdef _SERVER
BOOL KItemList::SearchPosition(int nWidth, int nHeight, ItemPos* pPos)
{
	if (nWidth <= 0 || nHeight <= 0 || NULL == pPos)
	{
		return FALSE;
	}

	POINT	pPt;
	if (!m_Room[room_equipment].FindRoom(nWidth, nHeight, &pPt))
	{

		if (0 != m_Hand)
		{
			return FALSE;
		}
		pPos->nPlace = pos_hand;
		pPos->nX = 0;
		pPos->nY = 0;
	}
	else
	{
		pPos->nPlace = pos_equiproom;
		pPos->nX = pPt.x;
		pPos->nY = pPt.y;
	}
	return TRUE;
}
#endif








#ifndef _SERVER
BOOL KItemList::SearchPosition(int nWidth, int nHeight, ItemPos* pPos)
{
	if (nWidth <= 0 || nHeight <= 0 || NULL == pPos)
	{
		return FALSE;
	}

	POINT	pPt;
	if (!m_Room[room_equipment].FindRoom(nWidth, nHeight, &pPt))
	{

		if (0 != m_Hand)
		{
			return FALSE;
		}
		pPos->nPlace = pos_hand;
		pPos->nX = 0;
		pPos->nY = 0;
	}
	else
	{
		pPos->nPlace = pos_equiproom;
		pPos->nX = pPt.x;
		pPos->nY = pPt.y;
	}
	return TRUE;
}
#endif

int	KItemList::SearchID(int nID)
{
	if (m_PlayerIdx <= 0)
		return 0;
	int nIdx = 0;
	while(1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (!nIdx)
			break;
		if (Item[m_Items[nIdx].nIdx].GetID() > 0 && Item[m_Items[nIdx].nIdx].GetID() == (DWORD)nID)
			return m_Items[nIdx].nIdx;
	}
	return 0;
}

void KItemList::ExchangeMoney(int pos1, int pos2, int nMoney)
{
	if (pos1 < 0 || pos2 < 0 || pos1 > room_trade || pos2 > room_trade)
		return;


   if (Player[m_PlayerIdx].m_bBlock && pos1 == room_repository && pos2 == room_equipment)
	   return;


//#ifdef _SERVER
//   if (Npc[Player[m_PlayerIdx].m_nIndex].m_SubWorldIndex == g_SubWorldSet.SearchWorld(43))
//		return;
//#endif


	if (m_Room[pos1].AddMoney(-nMoney))		// Ô´Î»ÖÃÄÜÄÃ³öÕâÃ´¶àÇ®À´
	{
		if (!m_Room[pos2].AddMoney(nMoney))	// Ä¿µÄµØÄÜ·Å²»ÏÂÈ¥
		{
			m_Room[pos1].AddMoney(nMoney);	// »¹Ô­Ô´Î»ÖÃµÄÇ®
		}
	}
	else
	{
		return;
	}

#ifndef _SERVER
	if (pos1 == room_equipment && pos2 == room_repository)
		SendClientCmdStoreMoney(0, nMoney);
	else if (pos1 == room_repository && pos2 == room_equipment)
		SendClientCmdStoreMoney(1, nMoney);
#endif
#ifdef _SERVER
	SendMoneySync();
#endif	
}

//----------------------------------------------------------------------
//	¹¦ÄÜ£ºµÃµ½ÎïÆ·À¸ºÍ´¢ÎïÏäµÄ×ÜÇ®Êý
//----------------------------------------------------------------------
int KItemList::GetMoneyAmount()
{
	return (m_Room[room_equipment].GetMoney() + m_Room[room_repository].GetMoney());
}

//----------------------------------------------------------------------
//	¹¦ÄÜ£ºµÃµ½ÎïÆ·À¸ºÍ´¢ÎïÏäµÄÇ®Êý
//----------------------------------------------------------------------
int KItemList::GetEquipmentMoney()
{
	return m_Room[room_equipment].GetMoney();
}

BOOL KItemList::AddMoney(int nRoom, int nMoney)
{
	if (nRoom < 0 || nRoom >= room_num)
		return FALSE;

	if ( !m_Room[nRoom].AddMoney(nMoney) )
		return FALSE;

#ifdef _SERVER
	SendMoneySync();
#endif

	return TRUE;
}

BOOL KItemList::CostMoney(int nMoney)
{
	if (nMoney > GetEquipmentMoney())
		return FALSE;

	if ( !m_Room[room_equipment].AddMoney(-nMoney) )
		return FALSE;

#ifdef _SERVER
	SendMoneySync();
#endif

	return TRUE;
}

BOOL KItemList::DecMoney(int nMoney)
{
	if (nMoney < 0)
		return FALSE;

	if (nMoney > m_Room[room_equipment].GetMoney())
	{
		nMoney -= m_Room[room_equipment].GetMoney();
		SetRoomMoney(room_equipment, 0);
		if (nMoney > m_Room[room_repository].GetMoney())
			SetRoomMoney(room_repository, 0);
		else
			AddMoney(room_repository, -nMoney);
	}
	else
	{
		AddMoney(room_equipment, -nMoney);
	}

#ifdef _SERVER
	SendMoneySync();
#endif

	return TRUE;
}

#ifdef _SERVER
//----------------------------------------------------------------------------------
//	¹¦ÄÜ£ºµ÷ÓÃ´Ë½Ó¿Ú±ØÐë±£Ö¤´«ÈëµÄnMoneyÊÇÒ»¸öÓÐÐ§Êý(ÕýÊýÇÒ²»³¬¹ýËùÓÐÇ®Êý)
//----------------------------------------------------------------------------------
void	KItemList::TradeMoveMoney(int nMoney)
{
	// ×Ô¼ºÇ®µÄ´¦Àí
	m_Room[room_trade].SetMoney(nMoney);
	SendMoneySync();

	// ¸ø¶Ô·½·¢ÏûÏ¢
	TRADE_MONEY_SYNC	sMoney;
	sMoney.ProtocolType = s2c_trademoneysync;
	sMoney.m_nMoney = nMoney;
	g_pServer->PackDataToClient(Player[Player[m_PlayerIdx].m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sMoney, sizeof(TRADE_MONEY_SYNC));
}
#endif

#ifdef _SERVER
//----------------------------------------------------------------------------------
//	¹¦ÄÜ£º·þÎñÆ÷·¢moneyÍ¬²½ÐÅÏ¢¸ø¿Í»§¶Ë
//----------------------------------------------------------------------------------
void	KItemList::SendMoneySync()
{
	PLAYER_MONEY_SYNC	sMoney;
	sMoney.ProtocolType = s2c_syncmoney;
	sMoney.m_nMoney1 = m_Room[room_equipment].GetMoney();
	sMoney.m_nMoney2 = m_Room[room_repository].GetMoney();
	sMoney.m_nMoney3 = m_Room[room_trade].GetMoney();
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMoney, sizeof(PLAYER_MONEY_SYNC));
}
#endif

void KItemList::SetMoney(int nMoney1, int nMoney2, int nMoney3)
{
	m_Room[room_equipment].SetMoney(nMoney1);
	m_Room[room_repository].SetMoney(nMoney2);
	m_Room[room_trade].SetMoney(nMoney3);
#ifndef _SERVER
	KUiObjAtContRegion	sMoney;
	sMoney.Obj.uGenre = CGOG_MONEY;
	sMoney.Obj.uId = nMoney2;
	sMoney.eContainer = UOC_STORE_BOX;
	CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&sMoney, 1);
#endif
}

void KItemList::SetRoomMoney(int nRoom, int nMoney)
{
	if (nRoom >= 0 && nRoom < room_num)
		m_Room[nRoom].SetMoney(nMoney);
}

void KItemList::ExchangeItem(ItemPos* SrcPos, ItemPos* DesPos)
{

	if (SrcPos->nPlace != DesPos->nPlace)
		return;


#ifdef _SERVER 


	if (Player[m_PlayerIdx].m_bBlock)
	{
		KPlayerChat::SendSystemInfo(1, m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Ch­a më khãa r­¬ng, kh«ng thÓ di chuyÓn vËt phÈm !", strlen("Ch­a më khãa r­¬ng, kh«ng thÓ di chuyÓn vËt phÈm !") );

		return;
	}
	
	if (Npc[Player[m_PlayerIdx].m_nIndex].m_SubWorldIndex == g_SubWorldSet.SearchWorld(43))
		return;
	

	if (m_Hand && !FindSame(m_Hand))
	{
		printf("Hack ExchangeItem m_Hand [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
		return;
	}
#endif

	int nTempHand = m_Hand;
	int	nEquipIdx1 = 0;

#ifdef _SERVER
	
	PLAYER_MOVE_ITEM_SYNC	sMove;
	sMove.ProtocolType = s2c_playermoveitem;
	sMove.m_btDownPos = SrcPos->nPlace;
	sMove.m_btDownX = SrcPos->nX;
	sMove.m_btDownY = SrcPos->nY;
	sMove.m_btUpPos = DesPos->nPlace;
	sMove.m_btUpX = DesPos->nX;
	sMove.m_btUpY = DesPos->nY;
#endif

	// Ô´×°±¸´ÓSrcPosµ½ÊÖÉÏ£¬ÊÖÉÏµÄ×°±¸µ½DesPos
	switch(SrcPos->nPlace)
	{
	case pos_hand:
		g_DebugLog("%s exchange item error", Npc[Player[m_PlayerIdx].m_nIndex].Name);
		return;
		break;

	case pos_equip:
		if (Player[this->m_PlayerIdx].CheckTrading())	// Èç¹ûÕýÔÚ½»Ò×
			return;
		if (SrcPos->nX < 0 || SrcPos->nX >= itempart_num || DesPos->nX < 0 || DesPos->nX >= itempart_num)
			return;

		#ifdef _SERVER
		if (SrcPos->nX != DesPos->nX)
		{
		printf("Hack ExchangeItem pos_equip 1 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
		return;
		}
		#endif


		nEquipIdx1 = m_EquipItem[SrcPos->nX];

	  #ifdef _SERVER
		if (m_Hand && nEquipIdx1 && m_Hand == nEquipIdx1)
		{
		    printf("Hack ExchangeItem pos_equip 2 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;
		}

		if (m_Hand && !CheckItemInAll(m_Hand))
		{
		printf("Hack ExchangeItem pos_equip 3 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (nEquipIdx1 && !CheckItemInAll(nEquipIdx1))
		{
		printf("Hack ExchangeItem pos_equip 4 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (m_Hand)
		{
		if (Item[m_Hand].m_nPriceShop)
		{
			Item[m_Hand].m_nPriceShop = 0;
	       PLAYER_UPDATE_SHOP_PRICE_COMMAND	IDC;
	       IDC.ProtocolType = s2c_playerupdateshopprice;
	       IDC.m_ID = Item[m_Hand].m_dwID;
	       IDC.m_Price = Item[m_Hand].m_nPriceShop;
	       if (g_pServer)
		   g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_UPDATE_SHOP_PRICE_COMMAND));
		}
		}

       #endif


		if (nEquipIdx1)
		{
	
			UnEquip(nEquipIdx1, SrcPos->nX);
		}
		if (m_Hand)
		{
			if (CanEquip(m_Hand, DesPos->nX))
			{
				Equip(m_Hand, DesPos->nX);
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			else if (nEquipIdx1)
			{
				Equip(nEquipIdx1, SrcPos->nX);
			}
		}
		else
		{
			m_Hand = nEquipIdx1;
			m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;






	case pos_pgbox:

		if (Player[this->m_PlayerIdx].CheckTrading())	// Èç¹ûÕýÔÚ½»Ò×
			return;
		if (SrcPos->nX < 0 || SrcPos->nX >= MAX_PGBOX_ITEM || DesPos->nX < 0 || DesPos->nX >= MAX_PGBOX_ITEM)
			return;

		#ifdef _SERVER
		if (SrcPos->nX != DesPos->nX)
		{
		printf("Hack ExchangeItem pos_pgbox 1 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
		return;
		}
		#endif


		nEquipIdx1 = m_PGBoxItem[SrcPos->nX];

	  #ifdef _SERVER
		if (m_Hand && nEquipIdx1 && m_Hand == nEquipIdx1)
		{
		    printf("Hack ExchangeItem pos_pgbox 2 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;
		}

		if (m_Hand && !CheckItemInAll(m_Hand))
		{
		printf("Hack ExchangeItem pos_pgbox 3 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (nEquipIdx1 && !CheckItemInAll(nEquipIdx1))
		{
		printf("Hack ExchangeItem pos_pgbox 4 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (m_Hand)
		{
		if (Item[m_Hand].m_nPriceShop)
		{
			Item[m_Hand].m_nPriceShop = 0;
	       PLAYER_UPDATE_SHOP_PRICE_COMMAND	IDC;
	       IDC.ProtocolType = s2c_playerupdateshopprice;
	       IDC.m_ID = Item[m_Hand].m_dwID;
	       IDC.m_Price = Item[m_Hand].m_nPriceShop;
	       if (g_pServer)
		   g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_UPDATE_SHOP_PRICE_COMMAND));
		}
		}
       #endif




		#ifdef _SERVER


		if (m_Hand)
		{






			if (m_Hand <= 0 || m_Hand >= MAX_ITEM)
			{
			printf("Hack ExchangeItem pos_pgbox 5 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
			}


			
		   if (SrcPos->nX >= 0 && SrcPos->nX <= 2)
			{
					
					if (Item[m_Hand].GetGenre() != item_task || Item[m_Hand].GetDetailType() < 44 || Item[m_Hand].GetDetailType() > 53)
					{

					KPlayerChat::SendSystemInfo(1, m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !", strlen("VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !") );
					return;	
					}


			}

		    else if (SrcPos->nX == 3 || SrcPos->nX == 16)
			{

			if (Item[m_Hand].GetGenre() != item_equip 
				//||  Item[m_Hand].GetVersion() > 1
				|| !Item[m_Hand].IsItemMagic()
				|| (Item[m_Hand].GetDetailType() != 2 && Item[m_Hand].GetDetailType() != 6 && Item[m_Hand].GetDetailType() != 5 && Item[m_Hand].GetDetailType() != 8 && Item[m_Hand].GetDetailType() != 7  && Item[m_Hand].GetDetailType() != 0 && Item[m_Hand].GetDetailType() != 1)
				||  Item[m_Hand].m_SpecialParam.uItemType != 0)
			{

            KPlayerChat::SendSystemInfo(1, m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !", strlen("VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !") );
			return;	
			}

			}


			else if (SrcPos->nX == 4)
			{

			//if (Item[m_Hand].GetGenre() != item_task || Item[m_Hand].GetDetailType() < 44 || Item[m_Hand].GetDetailType() > 53)
			if (Item[m_Hand].GetGenre() != item_task || Item[m_Hand].GetDetailType() != 414)
			{

            KPlayerChat::SendSystemInfo(1, m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !", strlen("VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !") );
			return;	
			}


			}

			else if (SrcPos->nX == 5)
			{

		//	if (Item[m_Hand].GetGenre() != item_task || Item[m_Hand].GetDetailType() < 167 || Item[m_Hand].GetDetailType() > 172 || Item[m_Hand].m_GeneratorParam.nGeneratorLevel[0])
			if (Item[m_Hand].GetGenre() != item_task || Item[m_Hand].GetDetailType() != 30)
			{

            KPlayerChat::SendSystemInfo(1, m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !", strlen("VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !") );
			return;	
			}


			}

			else if (SrcPos->nX >= 6 && SrcPos->nX <= 13)
			{

			if (Item[m_Hand].GetGenre() != item_task || Item[m_Hand].GetDetailType() < 28 || (Item[m_Hand].GetDetailType() > 30 && Item[m_Hand].GetDetailType() < 173) || Item[m_Hand].GetDetailType() > 175)
			{

            KPlayerChat::SendSystemInfo(1, m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !", strlen("VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !") );
			return;	
			}


			}



			else if (SrcPos->nX == 14)
			{

			if (Item[m_Hand].GetGenre() != item_equip 
				//||  Item[m_Hand].GetVersion() > 1
				|| !Item[m_Hand].IsItemMagic()
				|| (Item[m_Hand].GetDetailType() != 2 && Item[m_Hand].GetDetailType() != 6 && Item[m_Hand].GetDetailType() != 5 && Item[m_Hand].GetDetailType() != 8 && Item[m_Hand].GetDetailType() != 7  && Item[m_Hand].GetDetailType() != 0 && Item[m_Hand].GetDetailType() != 1)
				|| Item[m_Hand].m_SpecialParam.uItemType != 0
				)
			{

            KPlayerChat::SendSystemInfo(1, m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !", strlen("VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !") );
			return;	
			}

			}


			else if (SrcPos->nX == 15)
			{

			if (Item[m_Hand].GetGenre() != item_task || Item[m_Hand].GetDetailType() < 584 || (Item[m_Hand].GetDetailType() > 593 && Item[m_Hand].GetDetailType() < 623) || Item[m_Hand].GetDetailType() > 627)
			{

            KPlayerChat::SendSystemInfo(1, m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !", strlen("VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !") );
			return;	
			}


			}
/*
			else if (SrcPos->nX == 16)
			{

			if (Item[m_Hand].GetGenre() != item_equip 
				||  Item[m_Hand].GetVersion() > 1
				|| (Item[m_Hand].GetDetailType() != 2 && Item[m_Hand].GetDetailType() != 6 && Item[m_Hand].GetDetailType() != 5 && Item[m_Hand].GetDetailType() != 8 && Item[m_Hand].GetDetailType() != 7  && Item[m_Hand].GetDetailType() != 0 && Item[m_Hand].GetDetailType() != 1)
				|| Item[m_Hand].m_SpecialParam.uItemType != 2
				)
			{

            KPlayerChat::SendSystemInfo(1, m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !", strlen("VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !") );
			return;	
			}

			}
*/





			else if (SrcPos->nX == 17)
			{

		//	if (Item[m_Hand].GetGenre() != item_task || Item[m_Hand].GetDetailType() < 44 || Item[m_Hand].GetDetailType() > 53)
			if (Item[m_Hand].GetGenre() != item_task || Item[m_Hand].GetDetailType() != 415)
			{

            KPlayerChat::SendSystemInfo(1, m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !", strlen("VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !") );
			return;	
			}


			}

			else if (SrcPos->nX == 18)
			{

		//	if (Item[m_Hand].GetGenre() != item_task || Item[m_Hand].GetDetailType() < 167 || Item[m_Hand].GetDetailType() > 172 || !Item[m_Hand].m_GeneratorParam.nGeneratorLevel[0])
			if (Item[m_Hand].GetGenre() != item_task || Item[m_Hand].GetDetailType() != 175)
			{

            KPlayerChat::SendSystemInfo(1, m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !", strlen("VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !") );
			return;	
			}


			}



			else if (SrcPos->nX >= 19 && SrcPos->nX <= 26)
			{

			if (Item[m_Hand].GetGenre() != item_task || Item[m_Hand].GetDetailType() < 28 || (Item[m_Hand].GetDetailType() > 30 && Item[m_Hand].GetDetailType() < 173) || Item[m_Hand].GetDetailType() > 175)
			{

            KPlayerChat::SendSystemInfo(1, m_PlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !", strlen("VËt phÈm kh«ng thÝch hîp, kh«ng thÓ ®Æt !") );
			return;	
			}

			}






		}


        #endif








		if (nEquipIdx1)
		{
			m_PGBoxItem[SrcPos->nX] = 0;
		}

		if (m_Hand)
		{

			    m_PGBoxItem[DesPos->nX] = m_Hand;

				int nListIdx = FindSame(m_Hand);

				if (nListIdx)
				{
				m_Items[nListIdx].nPlace = pos_pgbox;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = 0;
				}
				else
				{

				#ifdef _SERVER
				printf("Loi ExchangeItem pos_pgbox hand khong thuoc nguoi choi [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
				#endif
				
				}

				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;

				

#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		
		}
		else
		{
			m_Hand = nEquipIdx1;
			m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;













	case pos_immediacy:
		if (Player[m_PlayerIdx].CheckTrading())	// Èç¹ûÕýÔÚ½»Ò×
			return;
		// ÅÐ¶ÏÊÇ·ñÓÐÍ¬ÀàÐÍµÄÎïÆ·£¬ÓÐµÄ»°²»ÈÃ·ÅÏÂÈ¥
		
	
	#ifdef _SERVER
		nEquipIdx1 = m_Room[room_immediacy].FindItem(SrcPos->nX, SrcPos->nY);
		if (m_Hand && nEquipIdx1 && m_Hand == nEquipIdx1)
		{
		    printf("Hack ExchangeItem pos_immediacy 1 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;
		}

		if (m_Hand && !CheckItemInAll(m_Hand))
		{
		printf("Hack ExchangeItem pos_immediacy 2 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (nEquipIdx1 && !CheckItemInAll(nEquipIdx1))
		{
		printf("Hack ExchangeItem pos_immediacy 3 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

	   	if (m_Hand)
		{
		if (Item[m_Hand].m_nPriceShop)
		{
			Item[m_Hand].m_nPriceShop = 0;
	       PLAYER_UPDATE_SHOP_PRICE_COMMAND	IDC;
	       IDC.ProtocolType = s2c_playerupdateshopprice;
	       IDC.m_ID = Item[m_Hand].m_dwID;
	       IDC.m_Price = Item[m_Hand].m_nPriceShop;
	       if (g_pServer)
		   g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_UPDATE_SHOP_PRICE_COMMAND));
		}
		}



     #endif
		
		
		
		if (m_Hand)
		{
			if (m_Room[room_immediacy].CheckSameDetailType(Item[m_Hand].GetGenre(), Item[m_Hand].GetDetailType()))
			{
#ifdef _SERVER
				BYTE	byFinished = s2c_itemexchangefinish;
				if (g_pServer)
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &byFinished, sizeof(BYTE));
#endif
#ifndef _SERVER
				KSystemMessage	sMsg;
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				sprintf(sMsg.szMessage, MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
#endif
				return;
			}
		}
		nEquipIdx1 = m_Room[room_immediacy].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;
		
		// ÏÈ°Ñ¶«Î÷¼ðÆðÀ´
		if (nEquipIdx1)
		{
			if (!m_Room[room_immediacy].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}

		// Èç¹ûÊó±êÉÏÓÐ¶«Î÷£¬ÏÈÊÔÊÔÄÜ²»ÄÜ°ÑËû·ÅÏÂÈ¥£¬Èç¹û²»ÐÐµÄ»°£¬»¹µÃ°ÑÔ­À´µÄ¶«Î÷·Å»ØÈ¥
		if (m_Hand)
		{
			if ((Item[m_Hand].GetGenre() == item_medicine || Item[m_Hand].GetGenre() == item_townportal || (Item[m_Hand].GetGenre() == item_task && !Item[m_Hand].IsStack()))
				&& m_Room[room_immediacy].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight())	
				)
			{
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_immediacy;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			// °ÑÔ­À´µÄ¶«Î÷·Å»ØÈ¥
			else
			{
				m_Room[room_immediacy].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// Èç¹ûÊó±êÉÏÃ»ÓÐ¶«Î÷£¬Ö»ÐèÒª°Ñ¼ðÆðÀ´µÄ¶«Î÷·Åµ½Êó±êÉÏ
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;
	case pos_repositoryroom:
		if (Player[m_PlayerIdx].CheckTrading())	// Èç¹ûÕýÔÚ½»Ò×
			return;
		nEquipIdx1 = m_Room[room_repository].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;


        #ifdef _SERVER
		if (m_Hand && nEquipIdx1 && m_Hand == nEquipIdx1)
		{
		    printf("Hack ExchangeItem pos_repositoryroom [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;
		}

		if (m_Hand && !CheckItemInAll(m_Hand))
		{
		printf("Hack ExchangeItem pos_repositoryroom 2 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (nEquipIdx1 && !CheckItemInAll(nEquipIdx1))
		{
		printf("Hack ExchangeItem pos_repositoryroom 3 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (m_Hand)
		{
		if (Item[m_Hand].m_nPriceShop)
		{
			Item[m_Hand].m_nPriceShop = 0;
	       PLAYER_UPDATE_SHOP_PRICE_COMMAND	IDC;
	       IDC.ProtocolType = s2c_playerupdateshopprice;
	       IDC.m_ID = Item[m_Hand].m_dwID;
	       IDC.m_Price = Item[m_Hand].m_nPriceShop;
	       if (g_pServer)
		   g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_UPDATE_SHOP_PRICE_COMMAND));
		}
		}



        #endif



	//#ifdef _SERVER
	//			if (m_Hand >0 &&
	//				nEquipIdx1 > 0 && 
	//				Item[m_Hand].GetGenre() == item_task &&
	//				Item[nEquipIdx1].GetGenre() == item_task &&
	//				Item[m_Hand].IsStack() &&
	//				Item[nEquipIdx1].IsStack() &&
	//				Item[m_Hand].GetDetailType() >= 0 &&
	//				Item[nEquipIdx1].GetDetailType() >= 0 &&
	//				Item[m_Hand].GetDetailType() == Item[nEquipIdx1].GetDetailType() &&
	//				Item[m_Hand].GetVersion() > 0 && 
	//				Item[nEquipIdx1].GetVersion() > 0&&
	//				Item[m_Hand].GetVersion() + Item[nEquipIdx1].GetVersion() <= MAX_ITEM_STACK
	//				)
	//			{
//
				
//				Item[nEquipIdx1].SetVersion(Item[m_Hand].GetVersion()+Item[nEquipIdx1].GetVersion());
//                Remove(m_Hand);
//	            ItemSet.Remove(m_Hand);
//				
//
//				PLAYER_ITEM_UPDATE_VERSION	IDC;
//	            IDC.ProtocolType = s2c_playerupdateitemvertion;
//	            IDC.m_ID = Item[nEquipIdx1].m_dwID;
//				IDC.m_Vesion = Item[nEquipIdx1].GetVersion();
//
//	            if (g_pServer)
//		        g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_ITEM_UPDATE_VERSION));

//				return;

//				}
//		#endif









		// ÏÈ°Ñ¶«Î÷¼ðÆðÀ´
		if (nEquipIdx1)
		{
			if (!m_Room[room_repository].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}

		// Èç¹ûÊó±êÉÏÓÐ¶«Î÷£¬ÏÈÊÔÊÔÄÜ²»ÄÜ°ÑËû·ÅÏÂÈ¥£¬Èç¹û²»ÐÐµÄ»°£¬»¹µÃ°ÑÔ­À´µÄ¶«Î÷·Å»ØÈ¥
		if (m_Hand)
		{
			if (m_Room[room_repository].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{


#ifdef _SERVER

	
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_repositoryroom;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));


#endif

#ifndef _SERVER


				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_repositoryroom;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#endif
			}
			// °ÑÔ­À´µÄ¶«Î÷·Å»ØÈ¥
			else
			{
				m_Room[room_repository].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// Èç¹ûÊó±êÉÏÃ»ÓÐ¶«Î÷£¬Ö»ÐèÒª°Ñ¼ðÆðÀ´µÄ¶«Î÷·Åµ½Êó±êÉÏ
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;


































case pos_rpnew:
		if (Player[m_PlayerIdx].CheckTrading())	// Èç¹ûÕýÔÚ½»Ò×
			return;
		nEquipIdx1 = m_Room[room_rpnew].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;


        #ifdef _SERVER
		if (m_Hand && nEquipIdx1 && m_Hand == nEquipIdx1)
		{
		    printf("Hack ExchangeItem pos_rpnew [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;
		}

		if (m_Hand && !CheckItemInAll(m_Hand))
		{
		printf("Hack ExchangeItem pos_rpnew 2 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (nEquipIdx1 && !CheckItemInAll(nEquipIdx1))
		{
		printf("Hack ExchangeItem pos_rpnew 3 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (m_Hand)
		{
		if (Item[m_Hand].m_nPriceShop)
		{
			Item[m_Hand].m_nPriceShop = 0;
	       PLAYER_UPDATE_SHOP_PRICE_COMMAND	IDC;
	       IDC.ProtocolType = s2c_playerupdateshopprice;
	       IDC.m_ID = Item[m_Hand].m_dwID;
	       IDC.m_Price = Item[m_Hand].m_nPriceShop;
	       if (g_pServer)
		   g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_UPDATE_SHOP_PRICE_COMMAND));
		}
		}



        #endif












		// ÏÈ°Ñ¶«Î÷¼ðÆðÀ´
		if (nEquipIdx1)
		{
			if (!m_Room[room_rpnew].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}

		// Èç¹ûÊó±êÉÏÓÐ¶«Î÷£¬ÏÈÊÔÊÔÄÜ²»ÄÜ°ÑËû·ÅÏÂÈ¥£¬Èç¹û²»ÐÐµÄ»°£¬»¹µÃ°ÑÔ­À´µÄ¶«Î÷·Å»ØÈ¥
		if (m_Hand)
		{
			if (m_Room[room_rpnew].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{


#ifdef _SERVER

	
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_rpnew;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));


#endif

#ifndef _SERVER


				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_rpnew;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#endif
			}
			// °ÑÔ­À´µÄ¶«Î÷·Å»ØÈ¥
			else
			{
				m_Room[room_rpnew].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// Èç¹ûÊó±êÉÏÃ»ÓÐ¶«Î÷£¬Ö»ÐèÒª°Ñ¼ðÆðÀ´µÄ¶«Î÷·Åµ½Êó±êÉÏ
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;






















case pos_checkbox:
		if (Player[m_PlayerIdx].CheckTrading())	// Èç¹ûÕýÔÚ½»Ò×
			return;
		nEquipIdx1 = m_Room[room_checkbox].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;


        #ifdef _SERVER
		if (m_Hand && nEquipIdx1 && m_Hand == nEquipIdx1)
		{
		    printf("Hack ExchangeItem pos_checkbox [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;
		}

		if (m_Hand && !CheckItemInAll(m_Hand))
		{
		printf("Hack ExchangeItem pos_checkbox 2 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (nEquipIdx1 && !CheckItemInAll(nEquipIdx1))
		{
		printf("Hack ExchangeItem pos_checkbox 3 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}


		if (m_Hand)
		{
		if (Item[m_Hand].m_nPriceShop)
		{
			Item[m_Hand].m_nPriceShop = 0;
	       PLAYER_UPDATE_SHOP_PRICE_COMMAND	IDC;
	       IDC.ProtocolType = s2c_playerupdateshopprice;
	       IDC.m_ID = Item[m_Hand].m_dwID;
	       IDC.m_Price = Item[m_Hand].m_nPriceShop;
	       if (g_pServer)
		   g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_UPDATE_SHOP_PRICE_COMMAND));
		}
		}


        #endif








		// ÏÈ°Ñ¶«Î÷¼ðÆðÀ´
		if (nEquipIdx1)
		{
			if (!m_Room[room_checkbox].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}

		// Èç¹ûÊó±êÉÏÓÐ¶«Î÷£¬ÏÈÊÔÊÔÄÜ²»ÄÜ°ÑËû·ÅÏÂÈ¥£¬Èç¹û²»ÐÐµÄ»°£¬»¹µÃ°ÑÔ­À´µÄ¶«Î÷·Å»ØÈ¥
		if (m_Hand)
		{
			if (m_Room[room_checkbox].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{


#ifdef _SERVER

	
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_checkbox;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));


#endif

#ifndef _SERVER


				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_checkbox;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#endif
			}
			// °ÑÔ­À´µÄ¶«Î÷·Å»ØÈ¥
			else
			{
				m_Room[room_checkbox].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// Èç¹ûÊó±êÉÏÃ»ÓÐ¶«Î÷£¬Ö»ÐèÒª°Ñ¼ðÆðÀ´µÄ¶«Î÷·Åµ½Êó±êÉÏ
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;















	case pos_equiproom:
		nEquipIdx1 = m_Room[room_equipment].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;



       #ifdef _SERVER
		if (m_Hand && nEquipIdx1 && m_Hand == nEquipIdx1)
		{
		    printf("Hack ExchangeItem pos_equiproom [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;
		}

		if (m_Hand && !CheckItemInAll(m_Hand))
		{
		printf("Hack ExchangeItem pos_equiproom 2 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (nEquipIdx1 && !CheckItemInAll(nEquipIdx1))
		{
		printf("Hack ExchangeItem pos_equiproom 3 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

       #endif



     // #ifdef _SERVER
	//			if (m_Hand >0 &&
	//				nEquipIdx1 > 0 && 
	//				Item[m_Hand].GetGenre() == item_task &&
	//				Item[nEquipIdx1].GetGenre() == item_task &&
	//				Item[m_Hand].IsStack() &&
	//				Item[nEquipIdx1].IsStack() &&
	//				Item[m_Hand].GetDetailType() >= 0 &&
	//				Item[nEquipIdx1].GetDetailType() >= 0 &&
	//				Item[m_Hand].GetDetailType() == Item[nEquipIdx1].GetDetailType() &&
	//				Item[m_Hand].GetVersion() > 0 && 
	//				Item[nEquipIdx1].GetVersion() > 0 &&
	//				Item[m_Hand].GetVersion() + Item[nEquipIdx1].GetVersion() <= MAX_ITEM_STACK
	//				)
	//			{

				
	//			Item[nEquipIdx1].SetVersion(Item[m_Hand].GetVersion()+Item[nEquipIdx1].GetVersion());
    //          Remove(m_Hand);
	//          ItemSet.Remove(m_Hand);
				

	//			PLAYER_ITEM_UPDATE_VERSION	IDC;
	//            IDC.ProtocolType = s2c_playerupdateitemvertion;
	//            IDC.m_ID = Item[nEquipIdx1].m_dwID;
	//			IDC.m_Vesion = Item[nEquipIdx1].GetVersion();
//
//	            if (g_pServer)
//		        g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_ITEM_UPDATE_VERSION));
//
//				return;
//
//				}
//		#endif









		// ÏÈ°Ñ¶«Î÷¼ðÆðÀ´
		if (nEquipIdx1)
		{
			if (!m_Room[room_equipment].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}
		// Èç¹ûÊó±êÉÏÓÐ¶«Î÷£¬ÏÈÊÔÊÔÄÜ²»ÄÜ°ÑËû·ÅÏÂÈ¥£¬Èç¹û²»ÐÐµÄ»°£¬»¹µÃ°ÑÔ­À´µÄ¶«Î÷·Å»ØÈ¥
		if (m_Hand)
		{
			if (m_Room[room_equipment].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{


#ifdef _SERVER


	int nListIdx = FindSame(m_Hand);
	m_Items[nListIdx].nPlace = pos_equiproom;
	m_Items[nListIdx].nX = DesPos->nX;
	m_Items[nListIdx].nY = DesPos->nY;
	m_Hand = nEquipIdx1;
	m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;

	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));


#endif



#ifndef _SERVER


				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_equiproom;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;

#endif


			}
			// °ÑÔ­À´µÄ¶«Î÷·Å»ØÈ¥
			else
			{
				m_Room[room_equipment].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// Èç¹ûÊó±êÉÏÃ»ÓÐ¶«Î÷£¬Ö»ÐèÒª°Ñ¼ðÆðÀ´µÄ¶«Î÷·Åµ½Êó±êÉÏ
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}

#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;
	case pos_traderoom:
		if ( !Player[m_PlayerIdx].CheckTrading() )	// ²»ÔÚ½»Ò×
			return;
#ifdef _SERVER


		if (m_Hand && (Item[m_Hand].GetGenre() == item_equip || Item[m_Hand].GetGenre() == item_medicine) && Item[m_Hand].m_GeneratorParam.nVersion > 1)
		{
         return; //Giao dich
		}
		if (m_Hand && Item[m_Hand].GetGenre() == item_equip && Item[m_Hand].GetDurability() == 0) // Trang bi do ben 0
		{
         return; //Giao dich
		}
		
#endif
		nEquipIdx1 = m_Room[room_trade].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;

#ifdef _SERVER
		if (m_Hand && nEquipIdx1 && m_Hand == nEquipIdx1)
		{
		    printf("Hack ExchangeItem pos_traderoom [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;
		}

		if (m_Hand && !CheckItemInAll(m_Hand))
		{
		printf("Hack ExchangeItem pos_traderoo 2 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (nEquipIdx1 && !CheckItemInAll(nEquipIdx1))
		{
		printf("Hack ExchangeItem pos_traderoo 3 [%s] [%s]\n",Player[m_PlayerIdx].m_AccoutName,Player[m_PlayerIdx].m_PlayerName);
			return;	
		}

		if (m_Hand)
		{
		if (Item[m_Hand].m_nPriceShop)
		{
			Item[m_Hand].m_nPriceShop = 0;
	       PLAYER_UPDATE_SHOP_PRICE_COMMAND	IDC;
	       IDC.ProtocolType = s2c_playerupdateshopprice;
	       IDC.m_ID = Item[m_Hand].m_dwID;
	       IDC.m_Price = Item[m_Hand].m_nPriceShop;
	       if (g_pServer)
		   g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_UPDATE_SHOP_PRICE_COMMAND));
		}
		}



#endif
		// ÏÈ°Ñ¶«Î÷¼ðÆðÀ´

		// Èç¹ûÊó±êÉÏÓÐ¶«Î÷£¬ÏÈÊÔÊÔÄÜ²»ÄÜ°ÑËû·ÅÏÂÈ¥£¬Èç¹û²»ÐÐµÄ»°£¬»¹µÃ°ÑÔ­À´µÄ¶«Î÷·Å»ØÈ¥
		if (m_Hand)
		{
			if (nEquipIdx1)	// ½»Ò×À¸ÓÐ¶«Î÷±»¼ñÆð
			{
			if (!m_Room[room_trade].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
			return;
			}

			if (m_Room[room_trade].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_traderoom;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
#ifdef _SERVER
				// ·¢ÏûÏ¢¸ø½»Ò×¶Ô·½
				if (nEquipIdx1)	// ½»Ò×À¸ÓÐ¶«Î÷±»¼ñÆð
				{
					ITEM_REMOVE_SYNC	sRemove;
					sRemove.ProtocolType = s2c_removeitem;
					sRemove.m_ID = Item[nEquipIdx1].m_dwID;
					g_pServer->PackDataToClient(Player[Player[m_PlayerIdx].m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sRemove, sizeof(ITEM_REMOVE_SYNC));
				}
				// ÊÖÉÏµÄ¶«Î÷·ÅÔÚÁË½»Ò×À¸
				ITEM_SYNC	sItem;
				sItem.ProtocolType = s2c_syncitem;
				sItem.m_ID = Item[m_Hand].GetID();
				sItem.m_Genre = Item[m_Hand].GetGenre();
				sItem.m_Detail = Item[m_Hand].GetDetailType();
				sItem.m_Particur = Item[m_Hand].GetParticular();
				sItem.m_Series = Item[m_Hand].GetSeries();
				sItem.m_Level = Item[m_Hand].GetLevel();
				sItem.m_Luck = Item[m_Hand].m_GeneratorParam.nLuck;
				sItem.m_btPlace = pos_trade1;
				sItem.m_btX = DesPos->nX;
				sItem.m_btY = DesPos->nY;
				// Time Item
				sItem.m_Time.bYear = Item[m_Hand].GetTime()->bYear;
				sItem.m_Time.bMonth = Item[m_Hand].GetTime()->bMonth;
				sItem.m_Time.bDay = Item[m_Hand].GetTime()->bDay;
				sItem.m_Time.bHour = Item[m_Hand].GetTime()->bHour;
				// End
				for (int j = 0; j < 6; j++)
					sItem.m_MagicLevel[j] = Item[m_Hand].m_GeneratorParam.nGeneratorLevel[j];
				sItem.m_RandomSeed = Item[m_Hand].m_GeneratorParam.uRandomSeed;
				sItem.m_Version = Item[m_Hand].m_GeneratorParam.nVersion;
				sItem.m_Durability = Item[m_Hand].GetDurability();
				sItem.iNgoaiTrang = Item[m_Hand].GetNgoaiTrang();

				g_pServer->PackDataToClient(Player[Player[m_PlayerIdx].m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sItem, sizeof(ITEM_SYNC));

				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
				m_Hand = nEquipIdx1;
				if (FindSame(nEquipIdx1))
					m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
			}
			// °ÑÔ­À´µÄ¶«Î÷·Å»ØÈ¥
			else
			{
				m_Room[room_trade].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// Èç¹ûÊó±êÉÏÃ»ÓÐ¶«Î÷£¬Ö»ÐèÒª°Ñ¼ðÆðÀ´µÄ¶«Î÷·Åµ½Êó±êÉÏ
		else
		{
			if (nEquipIdx1)	// ½»Ò×À¸ÓÐ¶«Î÷±»¼ñÆð
			{
			if (!m_Room[room_trade].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
			return;
			}

#ifdef _SERVER
			// ·¢ÏûÏ¢¸ø½»Ò×¶Ô·½
			if (nEquipIdx1)	// ½»Ò×À¸ÓÐ¶«Î÷±»¼ñÆð
			{
				ITEM_REMOVE_SYNC	sRemove;
				sRemove.ProtocolType = s2c_removeitem;
				sRemove.m_ID = Item[nEquipIdx1].m_dwID;
				g_pServer->PackDataToClient(Player[Player[m_PlayerIdx].m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sRemove, sizeof(ITEM_REMOVE_SYNC));
			}
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}
		}
		break;
	}


#ifndef _SERVER
	// Notify to Menu
	if (m_Hand != nTempHand)	// ½»»»³É¹¦ÁË
	{
	//uParam = (KUiObjAtRegion*)pInfo -> ÎïÆ·Êý¾Ý¼°Æä·ÅÖÃÇøÓòÎ»ÖÃÐÅÏ¢
	//nParam = bAdd -> 0Öµ±íÊ¾¼õÉÙÕâ¸öÎïÆ·£¬·Ç0Öµ±íÊ¾Ôö¼ÓÕâ¸öÎïÆ·
		KUiObjAtContRegion pInfo1, pInfo2;
		if (nTempHand == 0)
		{
			pInfo2.Obj.uGenre = CGOG_NOTHING;
			pInfo2.Obj.uId = 0;
			pInfo2.Region.Width = 0;
			pInfo2.Region.Height = 0;
		}
		else
		{
			pInfo2.Obj.uGenre = CGOG_ITEM;	//Ô´×°±¸
			pInfo2.Obj.uId = nTempHand;
			pInfo2.Region.Width = Item[nTempHand].GetWidth();
			pInfo2.Region.Height = Item[nTempHand].GetHeight();
		}
		
		if (m_Hand == 0)
		{
			pInfo1.Obj.uGenre = CGOG_NOTHING;
			pInfo1.Obj.uId = 0;
			pInfo1.Region.Width = 0;
			pInfo1.Region.Height = 0;
		}
		else
		{
			pInfo1.Obj.uGenre = CGOG_ITEM;
			pInfo1.Obj.uId = m_Hand;
			pInfo1.Region.Width = Item[m_Hand].GetWidth();
			pInfo1.Region.Height = Item[m_Hand].GetHeight();
		}

		int PartConvert[itempart_num] = 
		{
			UIEP_HEAD,
			UIEP_BODY,
			UIEP_WAIST,
			UIEP_HAND,
			UIEP_FOOT,
			UIEP_FINESSE,
			UIEP_NECK,
			UIEP_FINGER1,
			UIEP_FINGER2,
			UIEP_WAIST_DECOR,
			UIEP_HORSE,
			UIEP_MASK,
			UIEP_PHIPHONG,
		};

		switch(SrcPos->nPlace)
		{
		case pos_immediacy:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_IMMEDIA_ITEM;
			pInfo2.eContainer = UOC_IMMEDIA_ITEM;
			break;
		case pos_equiproom:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_ITEM_TAKE_WITH;
			pInfo2.eContainer = UOC_ITEM_TAKE_WITH;
			break;
		case pos_equip:
			pInfo1.Region.h = 0;
			pInfo1.Region.v = PartConvert[SrcPos->nX];
			pInfo2.Region.h = 0;
			pInfo2.Region.v = PartConvert[DesPos->nX];
			pInfo1.eContainer = UOC_EQUIPTMENT;
			pInfo2.eContainer = UOC_EQUIPTMENT;
			break;



		case pos_pgbox:
			pInfo1.Region.h = 0;
			pInfo1.Region.v = SrcPos->nX;
			pInfo2.Region.h = 0;
			pInfo2.Region.v = DesPos->nX;
			pInfo1.eContainer = UOC_PG_BOX;
			pInfo2.eContainer = UOC_PG_BOX;
			break;




		case pos_repositoryroom:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_STORE_BOX;
			pInfo2.eContainer = UOC_STORE_BOX;
			break;




		case pos_rpnew:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_RPNEW_BOX;
			pInfo2.eContainer = UOC_RPNEW_BOX;
			break;

			






		case pos_checkbox:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_CHECK_BOX;
			pInfo2.eContainer = UOC_CHECK_BOX;
			break;
		case pos_traderoom:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_TO_BE_TRADE;
			pInfo2.eContainer = UOC_TO_BE_TRADE;
			break;
		}
		CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo1, 0);
		CoreDataChanged(GDCNI_HOLD_OBJECT, (DWORD)&pInfo2, 0);
		CoreDataChanged(GDCNI_HOLD_OBJECT, (DWORD)&pInfo1, 1);
		CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo2, 1);
	}
#endif
	if (Npc[Player[m_PlayerIdx].m_nIndex].m_Doing == do_sit)
	{
		Npc[Player[m_PlayerIdx].m_nIndex].SendCommand(do_stand);
	}
}

#ifndef	_SERVER
//---------------------------------------------------------------------
//	¹¦ÄÜ£ºÎïÆ·´ÓÒ»¸öµØ·½Ö±½ÓÒÆ¶¯µ½ÁíÒ»¸öµØ·½£¬²»¾­¹ýÊó±êÕâ¸öÖÐ¼ä¹ý³Ì
//---------------------------------------------------------------------
BOOL	KItemList::AutoMoveItem(ItemPos SrcPos,ItemPos DesPos)
{
	if (Player[this->m_PlayerIdx].CheckTrading())	// Èç¹ûÕýÔÚ½»Ò×
		return FALSE;

	BOOL	bMove = FALSE;
	int		nIdx, nListIdx;

	// Ä¿Ç°Ö»Ö§³Ö´Óroom_equipmentµ½room_immediacy
	switch (SrcPos.nPlace)
	{
	case pos_equiproom:
		{
			switch (DesPos.nPlace)
			{
			case pos_immediacy:
				{
					nIdx = m_Room[room_equipment].FindItem(SrcPos.nX, SrcPos.nY);
					if (nIdx <= 0)
						return FALSE;
					if (Item[nIdx].GetGenre() != item_medicine)
					{
						_ASSERT(0);
						return FALSE;
					}
					if (!m_Room[room_equipment].PickUpItem(nIdx, SrcPos.nX, SrcPos.nY, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
						return FALSE;
					m_Room[room_immediacy].PlaceItem(DesPos.nX, DesPos.nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight());
					nListIdx = FindSame(nIdx);
					if (nListIdx <= 0)
						return FALSE;
					m_Items[nListIdx].nPlace = pos_immediacy;
					m_Items[nListIdx].nX = DesPos.nX;
					m_Items[nListIdx].nY = DesPos.nY;
					bMove = TRUE;
				}
				break;
			}
		}
		break;
	}

	if (!bMove)
		return bMove;

	// Í¨Öª½çÃæ
	KUiObjAtContRegion sSrcInfo, sDestInfo;

	sSrcInfo.Obj.uGenre		= CGOG_ITEM;
	sSrcInfo.Obj.uId		= nIdx;
	sSrcInfo.Region.Width	= Item[nIdx].GetWidth();
	sSrcInfo.Region.Height	= Item[nIdx].GetHeight();
	sSrcInfo.Region.h		= SrcPos.nX;
	sSrcInfo.Region.v		= SrcPos.nY;
	sSrcInfo.eContainer		= UOC_ITEM_TAKE_WITH;

	sDestInfo.Obj.uGenre	= CGOG_ITEM;
	sDestInfo.Obj.uId		= nIdx;
	sDestInfo.Region.Width	= Item[nIdx].GetWidth();
	sDestInfo.Region.Height	= Item[nIdx].GetHeight();
	sDestInfo.Region.h		= DesPos.nX;
	sDestInfo.Region.v		= DesPos.nY;
	sDestInfo.eContainer	= UOC_IMMEDIA_ITEM;

	CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&sSrcInfo, 0);
	CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&sDestInfo, 1);

	return bMove;
}
#endif

#ifndef	_SERVER
//---------------------------------------------------------------------
//	¹¦ÄÜ£ºÎïÆ·´ÓÒ»¸öµØ·½Ö±½ÓÒÆ¶¯µ½ÁíÒ»¸öµØ·½£¬²»¾­¹ýÊó±êÕâ¸öÖÐ¼ä¹ý³Ì
//---------------------------------------------------------------------
void	KItemList::MenuSetMouseItem()
{
	KUiObjAtContRegion	sInfo;
	if (!m_Hand)
	{
		CoreDataChanged(GDCNI_HOLD_OBJECT, 0, 0);
	}
	else
	{
		sInfo.Obj.uGenre = CGOG_ITEM;
		sInfo.Obj.uId = m_Hand;
		sInfo.Region.Width = Item[m_Hand].GetWidth();
		sInfo.Region.Height = Item[m_Hand].GetHeight();
		sInfo.Region.h = 0;
		sInfo.Region.v = 0;
		sInfo.eContainer = UOC_IN_HAND;
		CoreDataChanged(GDCNI_HOLD_OBJECT, (DWORD)&sInfo, 0);
	}
}
#endif

#ifdef _SERVER
BOOL KItemList::EatMecidine(int nPlace, int nX, int nY)
{
	int nItemIdx = 0;
	switch(nPlace)
	{
	case pos_equiproom:
		nItemIdx = m_Room[room_equipment].FindItem(nX, nY);
		if (nItemIdx > 0)
			return EatMecidine(nItemIdx);
		break;
	case pos_immediacy:
		nItemIdx = m_Room[room_immediacy].FindItem(nX, nY);
		if (nItemIdx > 0)
		{
			if (Item[nItemIdx].GetGenre() != item_medicine)
				return EatMecidine(nItemIdx);
			int		nGenre, nDetailType, nIdx, nXpos, nYpos;
			BOOL	bEat;

			nGenre = item_medicine;
			nDetailType = Item[nItemIdx].GetDetailType();

			bEat = EatMecidine(nItemIdx);
			if (bEat == FALSE)
				return bEat;

			if (FALSE == FindSameDetailTypeInEquipment(nGenre, nDetailType, &nIdx, &nXpos, &nYpos))
				return bEat;

			this->AutoMoveMedicine(nIdx, nXpos, nYpos, nX, nY);

			return bEat;
		}
		break;
	default:
		break;
	}

	return FALSE;
}
#endif

PlayerItem* KItemList::GetFirstItem()
{
	m_nListCurIdx = m_UseIdx.GetNext(0);
	return &m_Items[m_nListCurIdx];
}

PlayerItem* KItemList::GetNextItem()
{
	if ( !m_nListCurIdx )
		return NULL;
	m_nListCurIdx = m_UseIdx.GetNext(m_nListCurIdx);
	return &m_Items[m_nListCurIdx];
}

void	KItemList::ClearRoom(int nRoom)
{
	if (nRoom >= 0 && nRoom < room_num)
		this->m_Room[nRoom].Clear();
}


void	KItemList::ClearAll()
{


	ClearRoom(room_equipment);
	ClearRoom(room_repository);


	ClearRoom(room_rpnew);
	


	ClearRoom(room_trade);
	ClearRoom(room_tradeback);

#ifndef _SERVER
	ClearRoom(room_trade1);
	ClearRoom(room_buyshop);
#endif

	ClearRoom(room_immediacy);

	ClearRoom(room_checkbox);

    m_Hand = 0;
	m_nBackHand = 0;

	ZeroMemory(m_EquipItem, sizeof(m_EquipItem));
	ZeroMemory(m_PGBoxItem, sizeof(m_PGBoxItem));
	ZeroMemory(m_Items, sizeof(m_Items));

	m_nListCurIdx = 0;
	
	m_FreeIdx.Init(MAX_PLAYER_ITEM);
	m_UseIdx.Init(MAX_PLAYER_ITEM);

	for (int i = MAX_PLAYER_ITEM - 1; i > 0 ; i--)
	{
		m_FreeIdx.Insert(i);
	}

}

#ifdef _SERVER
int KItemList::FindNumberInAll(int nIdx)
{

if (nIdx <= 0 && nIdx >= MAX_ITEM)
return 0;

int nWidth = Item[nIdx].GetWidth();
int nHeight = Item[nIdx].GetHeight();

if (nWidth <= 0 || nHeight <= 0)
return 0;

int nNumberArray = 0;

nNumberArray += m_Room[room_equipment].FindNumberArrayItem(nIdx);
nNumberArray += m_Room[room_repository].FindNumberArrayItem(nIdx);


nNumberArray += m_Room[room_rpnew].FindNumberArrayItem(nIdx);


nNumberArray += m_Room[room_trade].FindNumberArrayItem(nIdx);

nNumberArray += m_Room[room_immediacy].FindNumberArrayItem(nIdx) * nWidth * nHeight;


nNumberArray += m_Room[room_checkbox].FindNumberArrayItem(nIdx);


if (m_Hand == nIdx)
nNumberArray += nWidth * nHeight;

for (int i=0;i<itempart_num;i++)
{

if (m_EquipItem[i] > 0 && m_EquipItem[i] == nIdx)
nNumberArray += nWidth * nHeight;

}

for (int j=0;j<MAX_PGBOX_ITEM;j++)
{

if (m_PGBoxItem[j] > 0 && m_PGBoxItem[j] == nIdx)
nNumberArray += nWidth * nHeight;

}




return nNumberArray;

}

#endif



#ifdef _SERVER
BOOL  KItemList::CheckItemInAll(int nIdx)
{

int nNumberArray = FindNumberInAll(nIdx);

if (!nNumberArray)
return FALSE;

int nWidth = Item[nIdx].GetWidth();
int nHeight = Item[nIdx].GetHeight();

if (nWidth <= 0 || nHeight <= 0)
return FALSE;

if (nNumberArray != (nWidth * nHeight))
return FALSE;

return TRUE;
}
#endif


void	KItemList::BackupTrade()
{
	if ( !m_Room[room_tradeback].m_pArray )
		m_Room[room_tradeback].Init(m_Room[room_equipment].m_nWidth, m_Room[room_equipment].m_nHeight);
	memcpy(m_Room[room_tradeback].m_pArray, m_Room[room_equipment].m_pArray, sizeof(int) * m_Room[room_tradeback].m_nWidth * m_Room[room_tradeback].m_nHeight);

	memcpy(this->m_sBackItems, this->m_Items, sizeof(PlayerItem) * MAX_PLAYER_ITEM);

	m_nBackHand = m_Hand;
}

void	KItemList::RecoverTrade()
{
	memcpy(m_Room[room_equipment].m_pArray, m_Room[room_tradeback].m_pArray, sizeof(int) * m_Room[room_tradeback].m_nWidth * m_Room[room_tradeback].m_nHeight);

#ifndef _SERVER
	int nIdx = 0;
	while((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (m_Items[nIdx].nPlace == pos_trade1)
			Remove(m_Items[nIdx].nIdx);
	}
/*	for (int i = 0; i < MAX_PLAYER_ITEM; i++)
	{
		if (m_Items[i].nIdx && m_Items[i].nPlace == pos_trade1)
			Remove(m_Items[i].nIdx);
	}*/
#endif
	memcpy(m_Items, m_sBackItems, sizeof(PlayerItem) * MAX_PLAYER_ITEM);
	m_Hand = m_nBackHand;
}

void	KItemList::StartTrade()
{
	BackupTrade();
	ClearRoom(room_trade);
	ClearRoom(room_trade1);
}

/*!*****************************************************************************
// Function		: KItemList::RemoveAll
// Purpose		: ÍË³öÊ±Çå³ýËùÓÐµÄ×°±¸
// Return		: void
// Comments		: »áÊµ¼ÊµØ´ÓÓÎÏ·ÊÀ½çÖÐµÄµÀ¾ßÊý×éÖÐÈ¥µô
// Author		: Spe
*****************************************************************************/
void KItemList::RemoveAll()
{
	int nIdx = m_UseIdx.GetNext(0);
	int nIdx1 = 0;
	while(nIdx)
	{
		nIdx1 = m_UseIdx.GetNext(nIdx);
		int nGameIdx = m_Items[nIdx].nIdx;
		Remove(m_Items[nIdx].nIdx,TRUE);
#ifdef _SERVER
		// ¿Í»§¶ËÔÚÉÏÃæKItemList::Remove()ÒÑ¾­×öÁËItemSet.Remove()
		ItemSet.m_checkdel = 3;
		ItemSet.Remove(nGameIdx,TRUE);
#endif
		nIdx = nIdx1;
	}

	ClearAll();

}

int KItemList::GetWeaponParticular()
{
	if (m_EquipItem[itempart_weapon])
		return Item[m_EquipItem[itempart_weapon]].GetParticular();
	return -1;
}

#ifdef _SERVER
BOOL KItemList::IsTaskItemExist(int nDetailType)
{
	int nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		int nGameIdx = m_Items[nIdx].nIdx;
		if (item_task != Item[nGameIdx].GetGenre())
			continue;
		if (nDetailType == Item[nGameIdx].GetDetailType())
		{
			return TRUE;
		}
	}
	return FALSE;
}
#endif

#ifdef _SERVER
int		KItemList::GetPlayerItemBlueNum()
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{	
		int nGameIdx = m_Items[nIdx].nIdx;
		if (Item[nGameIdx].GetGenre() != 0)
		{
			continue;
		}
		if (Item[nGameIdx].GetKind() == 1 || Item[nGameIdx].GetKind() == 2)
		{
			continue;
		}
		
		nNo++;
	}
	return nNo;
}
#endif
#ifdef _SERVER
int		KItemList::GetPlayerItemNum()
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{	
	nNo++;
	}
	return nNo;
}
#endif



#ifdef _SERVER
int		KItemList::GetPlayerItemIdxByNum(int nId)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{	
	nNo++;

	if (nNo == nId)
	{
	return m_Items[nIdx].nIdx;
	}


	}
	return -1;
}
#endif








#ifdef _SERVER
int		KItemList::GetPlayerItemPostIdx(int nIdxItem, int nValua)
{

	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{	


	if (m_Items[nIdx].nIdx == nIdxItem)
	{


		if (nValua == 1)
		{
		return m_Items[nIdx].nPlace;
		}
		else if (nValua == 2)
		{
		return m_Items[nIdx].nX;
		}
		else
		{
		return m_Items[nIdx].nY;
		}

	
	}


	}
	return -1;
}
#endif















#ifdef _SERVER
int		KItemList::GetItemIdxInCheckBoxByNum(int nGenre,int nDetailType,int nParticulType,int nLevel,int nNum)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{

		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;

        if (m_Items[nIdx].nPlace != pos_checkbox)
			continue;

		if (nGenre != Item[nGameIdx].GetGenre())
			continue;

		if (nDetailType != Item[nGameIdx].GetDetailType())
			continue;

		if (nParticulType != Item[nGameIdx].GetParticular())
			continue;

		if (nLevel != Item[nGameIdx].GetLevel())
			continue;

			nNo++;

		if (nNo == nNum)
			return nGameIdx;
		
	}
	return 0;
}
#endif



#ifdef _SERVER
int		KItemList::CheckDelItemPoison(int nGenre,int nDetailType,int nParticulType,int nLevel)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{

		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;


        if (m_Items[nIdx].nPlace != pos_equiproom)
			continue;

			if (nGenre != Item[nGameIdx].GetGenre())
			continue;

		if (nDetailType != Item[nGameIdx].GetDetailType())
			continue;

		if (nParticulType != Item[nGameIdx].GetParticular())
			continue;

		if (nLevel != Item[nGameIdx].GetLevel())
			continue;

		    Remove(nGameIdx);
			ItemSet.Remove(nGameIdx);
			nNo++;
			nIdx = 0;
		
	}
	return nNo;
}
#endif



#ifdef _SERVER
int		KItemList::DelPhiPhong()
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{

		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;
		
			
        if (m_Items[nIdx].nPlace != pos_equip)
			continue;
		
		
		if (Item[nGameIdx].GetDetailType() != 12)
			continue;
			
		    Remove(nGameIdx);
			ItemSet.Remove(nGameIdx);
			nNo++;
			nIdx = 0;
		
	}
	return nNo;
}
#endif



#ifdef _SERVER
int		KItemList::DeleteAllItemInCheckBox()
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{

		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;


        if (m_Items[nIdx].nPlace != pos_checkbox)
			continue;


		    Remove(nGameIdx);
			ItemSet.Remove(nGameIdx);
			nNo++;
			nIdx = 0;
		
	}
	return nNo;
}
#endif


#ifdef _SERVER
int		KItemList::DeleteAllItem()
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{

		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;


        if (m_Items[nIdx].nPlace != pos_equiproom)
			continue;


		    Remove(nGameIdx);
			ItemSet.Remove(nGameIdx);
			nNo++;
			nIdx = 0;
		
	}
	return nNo;
}
#endif

#ifdef _SERVER
int		KItemList::SetAutoRepair()
{
//	printf("CHEK \n");
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		
		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;


        if (m_Items[nIdx].nPlace != pos_equip)
			continue;

		//if (Item[nGameIdx].CanBeRepaired())
	//	{
		
			//Item[nIdx].SetDurability(Item[nIdx].GetMaxDurability());
			Item[nGameIdx].SetDurability(5);
		//	printf("Da sua item %d \n",nIdx);
			nNo++;
			nIdx = 0;
	//	}
	}
	return nNo;
}
#endif

#ifdef _SERVER
int		KItemList::CheckTrangSucInBox()
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{

		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;

        if (m_Items[nIdx].nPlace != pos_checkbox)
			continue;
		if (Item[m_Items[nIdx].nIdx].GetVersion() > 1)
		{
				nNo = -2;
				break;
		}
		if (Item[nGameIdx].GetGenre() == 0 && (Item[nGameIdx].GetDetailType() == 3 || Item[nGameIdx].GetDetailType() == 4 || Item[nGameIdx].GetDetailType() == 9))
		continue;

	
		nNo = -1;
		break;
	}
	return nNo;
}
#endif

#ifdef _SERVER
int		KItemList::GetAllItemInCheckBoxNum()
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{

		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;

        if (m_Items[nIdx].nPlace != pos_checkbox)
			continue;

			nNo++;
		
	}
	return nNo;
}
#endif



#ifdef _SERVER
int		KItemList::GetItemInCheckBoxNum(int nGenre,int nDetailType,int nParticulType,int nLevel)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{

		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;

        if (m_Items[nIdx].nPlace != pos_checkbox)
			continue;

		if (nGenre != Item[nGameIdx].GetGenre())
			continue;

		if (nDetailType != Item[nGameIdx].GetDetailType())
			continue;

		if (nParticulType != Item[nGameIdx].GetParticular())
			continue;

		if (nLevel != Item[nGameIdx].GetLevel())
			continue;

			nNo++;
		
	}
	return nNo;
}
#endif




#ifdef _SERVER
int		KItemList::CheckItemCountAll(int nGenre,int nDetailType,int nParticulType, int nLevel)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{

		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;

		if (nGenre != Item[nGameIdx].GetGenre())
			continue;

		if (nDetailType != Item[nGameIdx].GetDetailType())
			continue;

		if (nParticulType != Item[nGameIdx].GetParticular())
			continue;
		
		if (nLevel != Item[nGameIdx].GetLevel())
			continue;

		nNo++;
		
	}
	return nNo;
}
#endif


#ifdef _SERVER
int		KItemList::CheckTimeItemCount(int nTimeItem)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{

		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;

		

		if (Item[nGameIdx].m_GeneratorParam.nVersion != 550324)
			continue;


			nNo++;
		
	}
	return nNo;
}
#endif


#ifdef _SERVER
int		KItemList::CheckTimeItemCountAndGold(int nTimeItem)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{

		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;

		

		if (Item[nGameIdx].m_GeneratorParam.nVersion != 550324)
			continue;
		
		if ((Item[nGameIdx].GetDetailType() == 4 && Item[nGameIdx].GetParticular() < 6) ||
			(Item[nGameIdx].GetDetailType() == 2 && Item[nGameIdx].GetParticular() < 33) ||
			(Item[nGameIdx].GetDetailType() == 6 && Item[nGameIdx].GetParticular() < 3) ||
			(Item[nGameIdx].GetDetailType() == 5 && Item[nGameIdx].GetParticular() < 5) ||
			(Item[nGameIdx].GetDetailType() == 8 && Item[nGameIdx].GetParticular() < 3) ||
			(Item[nGameIdx].GetDetailType() == 7 && Item[nGameIdx].GetParticular() < 16) ||
			(Item[nGameIdx].GetDetailType() == 9 && Item[nGameIdx].GetParticular() < 6) ||
			(Item[nGameIdx].GetDetailType() == 3 && Item[nGameIdx].GetParticular() > 27) ||
			(Item[nGameIdx].GetDetailType() == 3 && Item[nGameIdx].GetParticular() < 7 ))
			continue;

			nNo++;
		
	}
	return nNo;
}
#endif




#ifdef _SERVER
int		KItemList::CheckItemBlockInCheckBox(int nGenre,int nDetailType,int nParticulType, int nLevel)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{

		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;

		if (nGenre != Item[nGameIdx].GetGenre())
			continue;

		if (nDetailType != Item[nGameIdx].GetDetailType())
			continue;

		if (nParticulType != Item[nGameIdx].GetParticular())
			continue;

		if (nLevel != Item[nGameIdx].GetLevel())
			continue;

		if (m_Items[nIdx].nPlace != pos_checkbox)
			continue;


			nNo = Item[nGameIdx].GetVersion();
		
	}
	return nNo;
}
#endif

















#ifdef _SERVER
int		KItemList::GetTaskItemBind(int nDetailType)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (item_task != Item[m_Items[nIdx].nIdx].GetGenre())
			continue;

        if (m_Items[nIdx].nPlace != pos_equiproom)
			continue;


		if (nDetailType == Item[m_Items[nIdx].nIdx].GetDetailType())
		{
		
			if (Item[m_Items[nIdx].nIdx].GetBindItem() == 1)
			{ 
				nNo++;
			}

		}
	}
	return nNo;
}
#endif









#ifdef _SERVER
int		KItemList::GetTaskItemNum(int nDetailType)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (item_task != Item[m_Items[nIdx].nIdx].GetGenre())
			continue;

        if (m_Items[nIdx].nPlace != pos_equiproom)
			continue;


		if (nDetailType == Item[m_Items[nIdx].nIdx].GetDetailType())
		{
		
		if (Item[m_Items[nIdx].nIdx].GetVersion() >= 1 && Item[m_Items[nIdx].nIdx].IsStack())
		{
			nNo += Item[m_Items[nIdx].nIdx].GetVersion();
		}
		else if (!Item[m_Items[nIdx].nIdx].IsStack())
		{
			nNo++;
		}

		}
	}
	return nNo;
}
#endif


#ifdef _SERVER
int		KItemList::GetTaskItemNumBox(int nDetailType)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (item_task != Item[m_Items[nIdx].nIdx].GetGenre())
			continue;

        if (m_Items[nIdx].nPlace != pos_checkbox)
			continue;


		if (nDetailType == Item[m_Items[nIdx].nIdx].GetDetailType())
		{
		
		if (Item[m_Items[nIdx].nIdx].GetVersion() >= 1 && Item[m_Items[nIdx].nIdx].IsStack())
		{
			nNo += Item[m_Items[nIdx].nIdx].GetVersion();
		}
		else if (!Item[m_Items[nIdx].nIdx].IsStack())
		{
			nNo++;
		}

		}
	}
	return nNo;
}
#endif


#ifdef _SERVER
int		KItemList::GetTaskItemNum2(int nDetailType)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (item_task != Item[m_Items[nIdx].nIdx].GetGenre())
			continue;

		if (nDetailType == Item[m_Items[nIdx].nIdx].GetDetailType())
		{
		
		if (Item[m_Items[nIdx].nIdx].GetVersion() >= 1 && Item[m_Items[nIdx].nIdx].IsStack())
		{
			nNo += Item[m_Items[nIdx].nIdx].GetVersion();
		}
		else if (!Item[m_Items[nIdx].nIdx].IsStack())
		{
			nNo++;
		}

		}
	}
	return nNo;
}
#endif



#ifdef _SERVER
int		KItemList::GetIdxTaskItemHaveFreeStask(int nDetailType)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (item_task != Item[m_Items[nIdx].nIdx].GetGenre())
			continue;

        if (m_Items[nIdx].nPlace != pos_equiproom)
			continue;


		if (nDetailType == Item[m_Items[nIdx].nIdx].GetDetailType())
		{
		
		if (Item[m_Items[nIdx].nIdx].GetVersion() >= 1 && Item[m_Items[nIdx].nIdx].GetVersion() < 200 && Item[m_Items[nIdx].nIdx].IsStack())
		{
			nNo = m_Items[nIdx].nIdx;
			break;
		}

		}
	}
	return nNo;
}
#endif



#ifdef _SERVER
BOOL KItemList::RemoveTaskItem(int nDetailType)
{
	int nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		int nGameIdx = m_Items[nIdx].nIdx;
		if (item_task != Item[nGameIdx].GetGenre())
			continue;

		if (m_Items[nIdx].nPlace != pos_equiproom)
			continue;



		if (nDetailType == Item[nGameIdx].GetDetailType())
		{

		if (Item[nGameIdx].GetVersion() > 1 && Item[nGameIdx].IsStack())
		{

		Item[nGameIdx].SetVersion(Item[nGameIdx].GetVersion()-1);

		
		PLAYER_ITEM_UPDATE_VERSION	IDC;
	    IDC.ProtocolType = s2c_playerupdateitemvertion;
	    IDC.m_ID = Item[nGameIdx].GetID();
		IDC.m_Vesion = Item[nGameIdx].GetVersion();

	    if (g_pServer)
		    g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_ITEM_UPDATE_VERSION));

		return TRUE;

		}
		else
		{
			ItemSet.m_checkdel = 4;
			Remove(nGameIdx);
			ItemSet.Remove(nGameIdx);
			return TRUE;
		}

		}
	}
	return FALSE;
}
#endif


#ifdef _SERVER
BOOL KItemList::RemoveTaskItemBox(int nDetailType)
{
	int nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		int nGameIdx = m_Items[nIdx].nIdx;
		if (item_task != Item[nGameIdx].GetGenre())
			continue;

		if (m_Items[nIdx].nPlace != pos_checkbox)
			continue;



		if (nDetailType == Item[nGameIdx].GetDetailType())
		{

		if (Item[nGameIdx].GetVersion() > 1 && Item[nGameIdx].IsStack())
		{

		Item[nGameIdx].SetVersion(Item[nGameIdx].GetVersion()-1);

		
		PLAYER_ITEM_UPDATE_VERSION	IDC;
	    IDC.ProtocolType = s2c_playerupdateitemvertion;
	    IDC.m_ID = Item[nGameIdx].GetID();
		IDC.m_Vesion = Item[nGameIdx].GetVersion();

	    if (g_pServer)
		    g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_ITEM_UPDATE_VERSION));

		return TRUE;

		}
		else
		{
			ItemSet.m_checkdel = 4;
			Remove(nGameIdx);
			ItemSet.Remove(nGameIdx);
			return TRUE;
		}

		}
	}
	return FALSE;
}
#endif


#ifdef _SERVER
BOOL		KItemList::SetItemTimeBox(int idxitem, int time)
{
	int		nNo = 0;
	int		nIdx = 0;
	int nGameIdx = idxitem;

	if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
	return FALSE;

	
	Item[nGameIdx].m_GeneratorParam.nVersion = time;
	nIdx = 0;
	return TRUE;
}

#endif


#ifdef _SERVER
BOOL		KItemList::SetHSD(int idxitem, int year, int month, int day, int hour)
{
	int		nNo = 0;
	int		nIdx = 0;
	int nGameIdx = idxitem;

	if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
	return FALSE;

			
	Item[nGameIdx].SetTime(year, month, day, hour);
	UpdateItem(nGameIdx);
	nIdx = 0;
	return TRUE;
}


int		KItemList::GetHSD(int idxitem, int inum)
{
	int		nNo = 0;
	int		nIdx = 0;
	int nGameIdx = idxitem;

	if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
	return FALSE;

	int resulf = Item[nGameIdx].GetTimeItem(inum);
	nIdx = 0;
	return resulf;
}

int		KItemList::GetDurationItem(int idxitem)
{
	int		nNo = 0;
	int		nIdx = 0;
	int nGameIdx = idxitem;

	if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
	return FALSE;

	int resulf = Item[nGameIdx].GetDurability();
	nIdx = 0;
	return resulf;
}
BOOL		KItemList::SetDurationItem(int idxitem)
{
	int		nNo = 0;
	int		nIdx = 0;
	int nGameIdx = idxitem;

	if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
	return FALSE;

			
	Item[nGameIdx].SetDurability(Item[nGameIdx].GetMaxDurability());
	
		ITEM_DURABILITY_CHANGE sIDC;
		sIDC.ProtocolType = s2c_itemdurabilitychange;
		sIDC.dwItemID = Item[nGameIdx].GetID();
		sIDC.nChange = Item[nGameIdx].GetMaxDurability();
		if (g_pServer)
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sIDC, sizeof(ITEM_DURABILITY_CHANGE));
	
	
	UpdateItem(nGameIdx);
	nIdx = 0;
	return TRUE;
}

void KItemList::UpdateItem(int nIdx)
{
	S2C_SYNC_ITEM_PARAM pParam;
	pParam.ProtocolType = s2c_sync_item_param;
	pParam.m_ID = Item[nIdx].GetID();
	pParam.m_Time.bYear = Item[nIdx].GetTime()->bYear;
	pParam.m_Time.bMonth = Item[nIdx].GetTime()->bMonth;
	pParam.m_Time.bDay = Item[nIdx].GetTime()->bDay;
	pParam.m_Time.bHour = Item[nIdx].GetTime()->bHour;

	
	if (g_pServer)
		g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&pParam, sizeof(S2C_SYNC_ITEM_PARAM));
}

#endif

#ifdef _SERVER
BOOL		KItemList::SetItemTimeOpenBlock(int time)
{
	int nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		int nGameIdx = m_Items[nIdx].nIdx;
		if (item_task == Item[nGameIdx].GetGenre())
			continue;

		if (m_Items[nIdx].nPlace != pos_checkbox)
			continue;


		Item[nGameIdx].m_GeneratorParam.nVersion = time;

	}
	return FALSE;
}
#endif



#ifdef _SERVER
BOOL KItemList::RemoveTaskItem2(int nDetailType)
{
	//printf("XOA ITEM %d \n",nDetailType);
	int nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		int nGameIdx = m_Items[nIdx].nIdx;
		if (item_task != Item[nGameIdx].GetGenre())
			continue;

		if (nDetailType == Item[nGameIdx].GetDetailType())
		{

		if (Item[nGameIdx].GetVersion() > 1 && Item[nGameIdx].IsStack())
		{

		Item[nGameIdx].SetVersion(Item[nGameIdx].GetVersion()-1);

		
		PLAYER_ITEM_UPDATE_VERSION	IDC;
	    IDC.ProtocolType = s2c_playerupdateitemvertion;
	    IDC.m_ID = Item[nGameIdx].GetID();
		IDC.m_Vesion = Item[nGameIdx].GetVersion();

	    if (g_pServer)
		    g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_ITEM_UPDATE_VERSION));

		return TRUE;

		}
		else
		{
			ItemSet.m_checkdel = 4;
			Remove(nGameIdx);
			ItemSet.Remove(nGameIdx);
			return TRUE;
		}

		}
	}
	return FALSE;
}
#endif

#ifdef _SERVER
void KItemList::SetBindAllItem()
{
	//printf("XOA ITEM %d \n",nDetailType);
	int nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		int nGameIdx = m_Items[nIdx].nIdx;
		if (item_task != Item[nGameIdx].GetGenre())
			continue;
		
	if (Item[nGameIdx].GetBindItem() == 0)
		continue;
		
		Item[nGameIdx].SetBindItem(0);	
		
		UpdateCurItem(nGameIdx);
		
	}
}
#endif



#ifdef _SERVER
//--------------------------------------------------------------------------
//	¹¦ÄÜ£º½»Ò×ÖÐ°Ñ trade room ÖÐµÄ item µÄ idx width height ÐÅÏ¢Ð´Èë itemset ÖÐµÄ m_psItemInfo ÖÐÈ¥
//--------------------------------------------------------------------------
void	KItemList::GetTradeRoomItemInfo()
{
	_ASSERT(ItemSet.m_psItemInfo);
//	if (!ItemSet.m_psItemInfo)
//	{
//		ItemSet.m_psItemInfo = new TRADE_ITEM_INFO[TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT];
//	}
	memset(ItemSet.m_psItemInfo, 0, sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);

	int		nItemIdx, nXpos, nYpos, nPos;

	nItemIdx = 0;
	nXpos = 0;
	nYpos = 0;
	nPos = 0;

	while (1)
	{
		nItemIdx = m_Room[room_trade].GetNextItem(nItemIdx, nXpos, nYpos, &nXpos, &nYpos);
		if (nItemIdx == 0)
			break;
		_ASSERT(nPos < TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);

		ItemSet.m_psItemInfo[nPos].m_nIdx = nItemIdx;
		ItemSet.m_psItemInfo[nPos].m_nWidth = Item[nItemIdx].GetWidth();
		ItemSet.m_psItemInfo[nPos].m_nHeight = Item[nItemIdx].GetHeight();
		nPos++;
	}

	// ´Ó´óµ½Ð¡ÅÅÐò
	TRADE_ITEM_INFO	sTemp;
	for (int i = nPos - 1; i >= 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (ItemSet.m_psItemInfo[j].m_nWidth * ItemSet.m_psItemInfo[j].m_nHeight < 
				ItemSet.m_psItemInfo[j + 1].m_nWidth * ItemSet.m_psItemInfo[j + 1].m_nHeight)
			{
				sTemp = ItemSet.m_psItemInfo[j];
				ItemSet.m_psItemInfo[j] = ItemSet.m_psItemInfo[j + 1];
				ItemSet.m_psItemInfo[j + 1] = sTemp;
			}
		}
	}
}
#endif

#ifdef _SERVER
//--------------------------------------------------------------------------
//	¹¦ÄÜ£º½»Ò×ÖÐÅÐ¶ÏÂò½øµÄÎïÆ·ÄÜ²»ÄÜÍêÈ«·Å½ø×Ô¼ºµÄÎïÆ·À¸
//--------------------------------------------------------------------------
BOOL	KItemList::TradeCheckCanPlace()
{
	LPINT	pnTempRoom;
	pnTempRoom = new int[EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT];
	memcpy(pnTempRoom, m_Room[room_equipment].m_pArray, sizeof(int) * EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT);

	int		nPos, i, j, a, b, nFind, nNext;
	for (nPos = 0; nPos < TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT; nPos++)
	{
		if (!ItemSet.m_psItemInfo[nPos].m_nIdx)
			break;
		nFind = 0;
		for (i = 0; i < EQUIPMENT_ROOM_HEIGHT - ItemSet.m_psItemInfo[nPos].m_nHeight + 1; i++)
		{
			for (j = 0; j < EQUIPMENT_ROOM_WIDTH - ItemSet.m_psItemInfo[nPos].m_nWidth + 1; j++)
			{
				nNext = 0;
				for (a = 0; a < ItemSet.m_psItemInfo[nPos].m_nHeight; a++)
				{
					for (b = 0; b < ItemSet.m_psItemInfo[nPos].m_nWidth; b++)
					{
						if (pnTempRoom[(i + a) * EQUIPMENT_ROOM_WIDTH + j + b])
						{
							nNext = 1;
							break;
						}
					}
					if (nNext)
						break;
				}
				// ÕÒµ½Ò»¸öÎ»ÖÃ
				if (!nNext)
				{
					// Êý¾Ý´¦Àí
					ItemSet.m_psItemInfo[nPos].m_nX = j;
					ItemSet.m_psItemInfo[nPos].m_nY = i;
					for (a = 0; a < ItemSet.m_psItemInfo[nPos].m_nHeight; a++)
					{
						for (b = 0; b < ItemSet.m_psItemInfo[nPos].m_nWidth; b++)
							pnTempRoom[(i + a) * EQUIPMENT_ROOM_WIDTH + j + b] = ItemSet.m_psItemInfo[nPos].m_nIdx;
					}

					nFind = 1;
					break;
				}
			}
			if (nFind)
				break;
		}
		if (!nFind)
		{
			delete []pnTempRoom;
			return FALSE;
		}
	}

	delete []pnTempRoom;
	return TRUE;
}
#endif

#ifdef _SERVER
//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÅÐ¶ÏÒ»¶¨³¤¿íµÄÎïÆ·ÄÜ·ñ·Å½øÎïÆ·À¸ (ÎªÁË·þÎñÆ÷Ð§ÂÊ£¬±¾º¯ÊýÀïÃæÃ»ÓÐµ÷ÓÃÆäËûº¯Êý)
//--------------------------------------------------------------------------



int	KItemList::CheckNguaPhienVu()
{
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int nIdx = m_Items[i].nIdx;
int nPlace = m_Items[i].nPlace;
int nItemGenre = Item[i].GetGenre();
int nDetailType = Item[i].GetDetailType();
int nParticular = Item[i].GetParticular();
int nLevel = Item[i].GetLevel();

if (nIdx > 0 && nIdx < MAX_ITEM && nItemGenre == 0 && nDetailType == 10 && nParticular == 7 && nLevel >= 2 && nLevel <= 6)
{
	return 1;
}
}
return 0;

}

BOOL	KItemList::CheckItemEquipCS()
{





for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int nIdx = m_Items[i].nIdx;
int nPlace = m_Items[i].nPlace;
int nItemGenre = Item[i].GetGenre();
int nDetailType = Item[i].GetDetailType();

if (nIdx > 0 && nIdx < MAX_ITEM && nPlace == pos_equip && nItemGenre == 0 && nDetailType != 10)
{

	return FALSE;
}
}


return TRUE;

}
BOOL	KItemList::CheckItemEquipMateBoy()
{



for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int nIdx = m_Items[i].nIdx;
int nPlace = m_Items[i].nPlace;
int nItemGenre = Item[i].GetGenre();
int nDetailType = Item[i].GetDetailType();
int nParticularType = Item[i].GetParticular();
int nLevel = Item[i].GetLevel();


if (nIdx > 0 && nIdx < MAX_ITEM && nPlace == pos_equip && nItemGenre == 0 && nDetailType == 2 && nParticularType == 28 && nLevel == 1)
	return TRUE;

}


return FALSE;

}
BOOL	KItemList::CheckItemEquipMateGirl()
{



for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int nIdx = m_Items[i].nIdx;
int nPlace = m_Items[i].nPlace;
int nItemGenre = Item[i].GetGenre();
int nDetailType = Item[i].GetDetailType();
int nParticularType = Item[i].GetParticular();
int nLevel = Item[i].GetLevel();


if (nIdx > 0 && nIdx < MAX_ITEM && nPlace == pos_equip && nItemGenre == 0 && nDetailType == 2 && nParticularType == 28 && nLevel == 4)
	return TRUE;

}


return FALSE;

}

BOOL	KItemList::CheckItemEquipMateGuest()
{



for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int nIdx = m_Items[i].nIdx;
int nPlace = m_Items[i].nPlace;
int nItemGenre = Item[i].GetGenre();
int nDetailType = Item[i].GetDetailType();
int nParticularType = Item[i].GetParticular();
int nLevel = Item[i].GetLevel();


if (nIdx > 0 && nIdx < MAX_ITEM && nPlace == pos_equip && nItemGenre == 0 && nDetailType == 2 && nParticularType == 28 && (nLevel == 7 || nLevel == 8))
	return TRUE;

}


return FALSE;

}

BOOL	KItemList::CheckCanPlaceInEquipment(int nWidth, int nHeight, int *pnX, int *pnY)
{
	if (nWidth <= 0 || nHeight <= 0 || !pnX || !pnY)
		return FALSE;

	_ASSERT(m_Room[room_equipment].m_pArray);

	LPINT	pnTempRoom;
	int		i, j, a, b, nNext;

	pnTempRoom = m_Room[room_equipment].m_pArray;

	for (i = 0; i < EQUIPMENT_ROOM_HEIGHT - nHeight + 1; i++)
	{
		for (j = 0; j < EQUIPMENT_ROOM_WIDTH - nWidth + 1; j++)
		{
			nNext = 0;
			for (a = 0; a < nHeight; a++)
			{
				for (b = 0; b < nWidth; b++)
				{
					if (pnTempRoom[(i + a) * EQUIPMENT_ROOM_WIDTH + j + b])
					{
						nNext = 1;
						break;
					}
				}
				if (nNext)
					break;
			}
			if (!nNext)
			{
				*pnX = j;
				*pnY = i;
				return TRUE;
			}
		}
	}

	return FALSE;
}
#endif

//------------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔÚroom_equipmentÖÐ²éÕÒÖ¸¶¨GenreºÍDetailTypeµÄÎïÆ·£¬µÃµ½ItemIdxºÍÎ»ÖÃ
//------------------------------------------------------------------------------
BOOL	KItemList::FindSameDetailTypeInEquipment(int nGenre, int nDetail, int *pnIdx, int *pnX, int *pnY)
{
	return m_Room[room_equipment].FindSameDetailType(nGenre, nDetail, pnIdx, pnX, pnY);
}

#ifdef _SERVER
//------------------------------------------------------------------------------
//	¹¦ÄÜ£º×Ô¶¯°ÑÒ»¸öÒ©Æ·´Óroom_equipmentÒÆ¶¯µ½room_immediacy
//------------------------------------------------------------------------------
BOOL	KItemList::AutoMoveMedicine(int nItemIdx, int nSrcX, int nSrcY, int nDestX, int nDestY)
{
	if (!m_Room[room_equipment].m_pArray || !m_Room[room_immediacy].m_pArray)
		return FALSE;
	if (nSrcX < 0 || nSrcX >= m_Room[room_equipment].m_nWidth || nSrcY < 0 || nSrcY >= m_Room[room_equipment].m_nHeight)
		return FALSE;
	if (nDestX < 0 || nDestX >= m_Room[room_immediacy].m_nWidth || nDestY < 0 || nDestY >= m_Room[room_immediacy].m_nHeight)
		return FALSE;
	if (nItemIdx != m_Room[room_equipment].m_pArray[nSrcY * m_Room[room_equipment].m_nWidth + nSrcX] ||
		0 != m_Room[room_immediacy].m_pArray[nDestY * m_Room[room_immediacy].m_nWidth + nDestX])
		return FALSE;

	_ASSERT(Item[nItemIdx].GetWidth() == 1 && Item[nItemIdx].GetHeight() == 1);
	if (!m_Room[room_equipment].PickUpItem(nItemIdx, nSrcX, nSrcY, Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight()))
		return FALSE;
	if (!m_Room[room_immediacy].PlaceItem(nDestX, nDestY, nItemIdx, Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight()))
	{
		m_Room[room_equipment].PlaceItem(nSrcX, nSrcY, nItemIdx, Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight());
		return FALSE;
	}

	int nListIdx = FindSame(nItemIdx);
	_ASSERT(nListIdx > 0);
	m_Items[nListIdx].nPlace = pos_immediacy;
	m_Items[nListIdx].nX = nDestX;
	m_Items[nListIdx].nY = nDestY;

	ITEM_AUTO_MOVE_SYNC	sMove;
	sMove.ProtocolType = s2c_ItemAutoMove;
	sMove.m_btSrcPos = pos_equiproom;
	sMove.m_btSrcX = nSrcX;
	sMove.m_btSrcY = nSrcY;
	sMove.m_btDestPos = pos_immediacy;
	sMove.m_btDestX = nDestX;
	sMove.m_btDestY = nDestY;
	if (g_pServer)
		g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(ITEM_AUTO_MOVE_SYNC));

	return TRUE;
}
#endif

#ifndef	_SERVER
void	KItemList::RemoveAllInOneRoom(int nRoom)
{
	if (nRoom < 0 || nRoom >= room_num)
		return;

	int		nItemIdx, nXpos, nYpos, nPos;

	nItemIdx = 0;
	nXpos = 0;
	nYpos = 0;
	nPos = 0;

	while (1)
	{
		nItemIdx = m_Room[nRoom].GetNextItem(nItemIdx, nXpos, nYpos, &nXpos, &nYpos);
		if (nItemIdx == 0)
			break;
		_ASSERT(nPos < m_Room[nRoom].m_nWidth * m_Room[nRoom].m_nHeight);

		Remove(nItemIdx);

		nPos++;
	}
}
#endif

#ifndef _SERVER
void KItemList::LockOperation()
{
	if (IsLockOperation())
	{
		_ASSERT(0);
		return;
	}
	m_bLockOperation = TRUE;
}
#endif

#ifndef _SERVER
void KItemList::UnlockOperation()
{
	if (!IsLockOperation())
	{
		return;
	}
	m_bLockOperation = FALSE;
}
#endif


#ifndef _SERVER
void KItemList::RemoveMagicDurability(int nIdx)
{


	if (nIdx <= 0)
		return;

	if (m_PlayerIdx > 0)
	
	{

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	if (nNpcIdx > 0)

	{
	

	if (nIdx == m_EquipItem[itempart_head])
	{

	Item[nIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], GetEquipEnhance(itempart_head));
	InfectionNextEquip(itempart_head, FALSE);
	
	}
	else if ( nIdx == m_EquipItem[itempart_body])
	{
	Item[nIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], GetEquipEnhance(itempart_body));
	InfectionNextEquip(itempart_body, FALSE);

	}
	else if (nIdx == m_EquipItem[itempart_belt])
	{
	Item[nIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], GetEquipEnhance(itempart_belt));
	InfectionNextEquip(itempart_belt, FALSE);
	
	}
	else if ( nIdx == m_EquipItem[itempart_weapon])
	{
	Item[nIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], GetEquipEnhance(itempart_weapon));
	InfectionNextEquip(itempart_weapon, FALSE);
	
	}
	else if (nIdx == m_EquipItem[itempart_foot])
	{
	Item[nIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], GetEquipEnhance(itempart_foot));
	InfectionNextEquip(itempart_foot, FALSE);
	
	}
	else if (nIdx == m_EquipItem[itempart_cuff])
	{
	Item[nIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], GetEquipEnhance(itempart_cuff));
	InfectionNextEquip(itempart_cuff, FALSE);
	
	}


	}

	}




}
#endif







int KItemList::GetActiveAttribNum(int nIdx)
{
	for (int i = 0; i < itempart_num; i++)
	{
		if (nIdx == m_EquipItem[i])
		{
			return GetEquipEnhance(i);
		}
	}
	return 0;
}

#ifdef _SERVER
void		KItemList::AutoDurationItem(int nRate)
{
	if (nRate <= 0 || nRate > 100)
	return;
	
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		
		int nGameIdx = m_Items[nIdx].nIdx;

		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;

        if (m_Items[nIdx].nPlace != pos_equip)
			continue;
	
		int  nDetail = Item[nGameIdx].GetDetailType();
		if (nDetail == 11 || nDetail == 10 || nDetail == 4 || nDetail == 9 || nDetail == 3)
			continue;
			
			int nOldDur = Item[nGameIdx].GetDurability();	
		if (nOldDur <= 0)
			continue;
			
			int Durability = nOldDur - (nRate * Item[nGameIdx].GetMaxDurability() / 100);
			if (Durability < 0)
			Durability = 1;

			Item[nGameIdx].SetDurability(Durability);
			
				ITEM_DURABILITY_CHANGE sIDC;
				sIDC.ProtocolType = s2c_itemdurabilitychange;
				sIDC.dwItemID = Item[nGameIdx].GetID();
				sIDC.nChange = Durability - nOldDur;
				if (g_pServer)
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sIDC, sizeof(ITEM_DURABILITY_CHANGE));
	}
}
#endif


#ifdef _SERVER
void		KItemList::SetItemNotRepair()
{

	
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		
		int nGameIdx = m_Items[nIdx].nIdx;
		
		if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
			continue;
		
        if (m_Items[nIdx].nPlace != pos_equip)
			continue;
		
		int  nDetail = Item[nGameIdx].GetDetailType();
	
		if (nDetail == 11 || nDetail == 10 || nDetail == 4 || nDetail == 9 || nDetail == 3)
			continue;
		
		

			int nOldDur = Item[nGameIdx].GetDurability();	
			int Durability = 0;

			
			Item[nGameIdx].SetDurability(Durability);
			
			ITEM_DURABILITY_CHANGE sIDC;
				sIDC.ProtocolType = s2c_itemdurabilitychange;
				sIDC.dwItemID = Item[nGameIdx].GetID();
				sIDC.nChange = Durability - nOldDur;
				if (g_pServer)
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sIDC, sizeof(ITEM_DURABILITY_CHANGE));
					
			UpdateCurItem(nGameIdx);		
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------------
//	¹¦ÄÜ£º¶ªÊ§ËæÉíÎïÆ·
//-------------------------------------------------------------------------------
void	KItemList::AutoLoseItemFromEquipmentRoom(int nRate)
{
	if (Player[m_PlayerIdx].CheckTrading())
		return;

	if (Player[m_PlayerIdx].m_bBlock)
	return;


	if (nRate <= 0 || nRate > 100)
		return;

	int		nItemIdx, nXpos, nYpos, nPos;

	nItemIdx = 0;
	nXpos = 0;
	nYpos = 0;
	nPos = 0;

	// ÏÈÍ³¼ÆËùÓÐÎïÆ·£¬ÅÅ³ýÈÎÎñÎïÆ·
	while (1)
	{
		nItemIdx = m_Room[room_equipment].GetNextItem(nItemIdx, nXpos, nYpos, &nXpos, &nYpos);
		if (nItemIdx == 0)
			break;




		if ((Item[nItemIdx].GetGenre() == item_equip || Item[nItemIdx].GetGenre() == item_medicine) && Item[nItemIdx].m_GeneratorParam.nVersion > 1)
			continue;

		ItemSet.m_sLoseItemFromEquipmentRoom[nPos].nIdx = nItemIdx;
		ItemSet.m_sLoseItemFromEquipmentRoom[nPos].nPlace = pos_equiproom;
		ItemSet.m_sLoseItemFromEquipmentRoom[nPos].nX = nXpos;
		ItemSet.m_sLoseItemFromEquipmentRoom[nPos].nY = nYpos;
		nPos++;
	}
	if (nPos == 0)
		return;

	KMapPos			sMapPos;
	int				nSelect;
	int				nObj;
	KObjItemInfo	sInfo;

	for (int i = 0; i < nPos; i++)
	{
		if (g_Random(100) >= nRate)
			continue;
		nItemIdx = ItemSet.m_sLoseItemFromEquipmentRoom[i].nIdx;
		Player[m_PlayerIdx].GetAboutPos(&sMapPos);


		if (Item[nItemIdx].m_nPriceShop)
		{
		   Item[nItemIdx].m_nPriceShop = 0;
	       PLAYER_UPDATE_SHOP_PRICE_COMMAND	IDC;
	       IDC.ProtocolType = s2c_playerupdateshopprice;
	       IDC.m_ID = Item[nItemIdx].m_dwID;
	       IDC.m_Price = Item[nItemIdx].m_nPriceShop;
	       if (g_pServer)
		   g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_UPDATE_SHOP_PRICE_COMMAND));
		}



		// ¶ªÆú
		if (Remove(nItemIdx))
		{
			sInfo.m_nItemID = nItemIdx;
			sInfo.m_nItemWidth = Item[nItemIdx].GetWidth();
			sInfo.m_nItemHeight = Item[nItemIdx].GetHeight();
			sInfo.m_nMoneyNum = 0;
			strcpy(sInfo.m_szName, Item[nItemIdx].m_CommonAttrib.szItemName);

	if (Item[nItemIdx].m_SpecialParam.uItemType == 1)
	sInfo.m_nColorID = 1;
	else if (Item[nItemIdx].m_SpecialParam.uItemType == 1)
	sInfo.m_nColorID = 3;
	else if (Item[nItemIdx].IsItemMagic())
	sInfo.m_nColorID = 2;
	else
    sInfo.m_nColorID = 0;

			
			sInfo.m_nMovieFlag = 1;
			sInfo.m_nSoundFlag = 1;
		    sInfo.m_dwNpcId = Npc[Player[m_PlayerIdx].m_nIndex].m_dwID;
			sInfo.m_bByPlayer = TRUE;

			nObj = ObjSet.Add(Item[nItemIdx].GetObjIdx(), sMapPos, sInfo);
			if (nObj >= 0)
			{
				Object[nObj].SetItemBelong(-1);
			}
			else
			{

			ItemSet.m_checkdel = 5;
			ItemSet.Remove(nItemIdx);

			}

			SHOW_MSG_SYNC	sMsg;
			sMsg.ProtocolType = s2c_msgshow;
			sMsg.m_wMsgID = enumMSG_ID_DEATH_LOSE_ITEM;
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID) + sizeof(sInfo.m_szName);
			sMsg.m_lpBuf = new BYTE[sMsg.m_wLength + 1];
			memcpy(sMsg.m_lpBuf, &sMsg, sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID));
			memcpy((char*)sMsg.m_lpBuf + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), sInfo.m_szName, sizeof(sInfo.m_szName));
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
		}
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------------
//	¹¦ÄÜ£º¶ªÊ§Ò»¸ö´©ÔÚÉíÉÏµÄ×°±¸(Âí²»»áµô)
//-------------------------------------------------------------------------------
void	KItemList::AutoLoseEquip()
{


if (Player[m_PlayerIdx].m_bBlock)
	return;



#ifndef defEQUIP_POWER
	int		i, nPos = 0;

	for (i = 0; i < itempart_num; i++)
	{
		if (m_EquipItem[i] <= 0)
			continue;


		if (((Item[m_EquipItem[i]].GetGenre() == item_equip || Item[m_EquipItem[i]].GetGenre() == item_medicine) && Item[m_EquipItem[i]].m_GeneratorParam.nVersion > 1) || (Item[m_EquipItem[i]].GetGenre() == item_task && Item[m_EquipItem[i]].GetBindItem() >= 1))
			continue;
		ItemSet.m_sLoseEquipItem[nPos].nIdx = m_EquipItem[i];
		ItemSet.m_sLoseEquipItem[nPos].nPlace = nPos;
		nPos++;
	}
	if (nPos == 0)
		return;

	int		nSelect = g_Random(nPos);
#endif

#ifdef defEQUIP_POWER
	int		i, nPos = 0, nTotalPower = 0;

	for (i = 0; i < itempart_num; i++)
	{
		if (m_EquipItem[i] <= 0)
			continue;
		if ((Item[m_EquipItem[i]].GetGenre() == item_equip || Item[m_EquipItem[i]].GetGenre() == item_medicine) && Item[m_EquipItem[i]].m_GeneratorParam.nVersion > 1)
			continue;
		nTotalPower += g_nEquipPower[i];
		ItemSet.m_sLoseEquipItem[nPos].nIdx = m_EquipItem[i];
		ItemSet.m_sLoseEquipItem[nPos].nPlace = nPos;
		ItemSet.m_sLoseEquipItem[nPos].nX = nTotalPower;	// nX ½èÓÃÒ»ÏÂ
		nPos++;
	}
	if (nTotalPower == 0)
		return;
	int		nSelect = g_Random(nTotalPower);
	for (i = 0; i < nPos; i++)
	{
		if (ItemSet.m_sLoseEquipItem[i].nX > nSelect)
		{
			nSelect = i;
			break;
		}
	}
	if (i >= nPos)
		return;
#endif

	int			 nItemIdx;
	KMapPos		sMapPos;

	nItemIdx = ItemSet.m_sLoseEquipItem[nSelect].nIdx;
	Player[m_PlayerIdx].GetAboutPos(&sMapPos);



	 if (Item[nItemIdx].m_nPriceShop)
		{
		   Item[nItemIdx].m_nPriceShop = 0;
	       PLAYER_UPDATE_SHOP_PRICE_COMMAND	IDC;
	       IDC.ProtocolType = s2c_playerupdateshopprice;
	       IDC.m_ID = Item[nItemIdx].m_dwID;
	       IDC.m_Price = Item[nItemIdx].m_nPriceShop;
	       if (g_pServer)
		   g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &IDC, sizeof(PLAYER_UPDATE_SHOP_PRICE_COMMAND));
		}


	if (Remove(nItemIdx))
	{
		int		nObj;
		KObjItemInfo	sInfo;
		sInfo.m_nItemID = nItemIdx;
		sInfo.m_nItemWidth = Item[nItemIdx].GetWidth();
		sInfo.m_nItemHeight = Item[nItemIdx].GetHeight();
		sInfo.m_nMoneyNum = 0;
		strcpy(sInfo.m_szName, Item[nItemIdx].m_CommonAttrib.szItemName);

		if (Item[nItemIdx].m_SpecialParam.uItemType == 1)
		sInfo.m_nColorID = 1;
		else if (Item[nItemIdx].m_SpecialParam.uItemType == 1)
		sInfo.m_nColorID = 3;
		else if (Item[nItemIdx].IsItemMagic())
		sInfo.m_nColorID = 2;
		else
		sInfo.m_nColorID = 0;


		sInfo.m_nMovieFlag = 1;
		sInfo.m_nSoundFlag = 1;
		sInfo.m_dwNpcId = Npc[Player[m_PlayerIdx].m_nIndex].m_dwID;
		sInfo.m_bByPlayer = TRUE;

		nObj = ObjSet.Add(Item[nItemIdx].GetObjIdx(), sMapPos, sInfo);
		if (nObj >= 0)
		{
			Object[nObj].SetItemBelong(-1);
		}
		else
		{
			ItemSet.m_checkdel = 6;
			ItemSet.Remove(nItemIdx);

		}

		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_DEATH_LOSE_ITEM;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID) + sizeof(sInfo.m_szName);
		sMsg.m_lpBuf = new BYTE[sMsg.m_wLength + 1];
		memcpy(sMsg.m_lpBuf, &sMsg, sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID));
		memcpy((char*)sMsg.m_lpBuf + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), sInfo.m_szName, sizeof(sInfo.m_szName));
		g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
	}
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------------
//	¹¦ÄÜ£º
//-------------------------------------------------------------------------------
int		KItemList::GetSameDetailItemNum(int nImmediatePos)
{
	if (nImmediatePos < 0 || nImmediatePos >= IMMEDIACY_ROOM_WIDTH * IMMEDIACY_ROOM_HEIGHT)
		return 0;
	int		nIdx = m_Room[room_immediacy].FindItem(nImmediatePos, 0);
	if (nIdx <= 0)
		return 0;
	return m_Room[room_equipment].CalcSameDetailType(Item[nIdx].GetGenre(), Item[nIdx].GetDetailType()) + 1;
}
#endif
#ifdef _SERVER
void KItemList::Abrade(int nType)
{
	int nItemIdx = 0;
	for (int i = 0; i < itempart_num; i++)
	{
		nItemIdx = m_EquipItem[i];
		if (nItemIdx)
		{
			int nOldDur = Item[nItemIdx].GetDurability();

			if (nOldDur == 0 || nOldDur == -1)
			    continue;

		





 int nRandRange = ItemSet.GetAbradeRange(nType, i);


 if (nRandRange == 0)
	 continue;

int nDur = nOldDur;


Item[nItemIdx].m_nPoitAbrate++;

if (Item[nItemIdx].m_nPoitAbrate < 0)
Item[nItemIdx].m_nPoitAbrate = 0;

if (Item[nItemIdx].m_nPoitAbrate >= nRandRange)
{
Item[nItemIdx].m_nPoitAbrate = 0;
nDur --;
}


			if (nDur == 0)
			{
				

	if (m_PlayerIdx > 0)
	
	{

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	if (nNpcIdx > 0)

	{
	



	if (i == itempart_head || i == itempart_body || i == itempart_belt || i == itempart_weapon || i == itempart_foot || i == itempart_cuff)
	{	
	Item[nItemIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], GetEquipEnhance(i));
	InfectionNextEquip(i, FALSE);
	}
	else if (i == itempart_mask)
	{
		Npc[nNpcIdx].m_MaskType = 0;
	}



	}

	}




	            Item[nItemIdx].SetDurability(nDur);

               	ITEM_DURABILITY_CHANGE sIDC;
				sIDC.ProtocolType = s2c_itemdurabilitychange;
				sIDC.dwItemID = Item[nItemIdx].GetID();
				sIDC.nChange = nDur - nOldDur;
				if (g_pServer)
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sIDC, sizeof(ITEM_DURABILITY_CHANGE));


				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_ITEM_DAMAGED;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
				sMsg.m_lpBuf = (void *)Item[nItemIdx].m_dwID;
				if (g_pServer)
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				sMsg.m_lpBuf = 0;
			}
			else if (nOldDur != nDur && nDur != -1)
			{
				Item[nItemIdx].SetDurability(nDur);

				ITEM_DURABILITY_CHANGE sIDC;
				sIDC.ProtocolType = s2c_itemdurabilitychange;
				sIDC.dwItemID = Item[nItemIdx].GetID();
				sIDC.nChange = nDur - nOldDur;
				if (g_pServer)
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sIDC, sizeof(ITEM_DURABILITY_CHANGE));
			}
		}
	}
}
#endif

void KItemList::SetMaskLock( BOOL bFlag )
{
	m_nMaskLock = bFlag;
#ifdef _SERVER
	MASK_SYNC Sync;
	Sync.ProtocolType = s2c_syncmasklock;
	Sync.ID = m_nMaskLock;
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&Sync, sizeof(MASK_SYNC));
#endif
}


// Time Item

#ifdef _SERVER
void KItemList::CheckItemTime()
{

	time_t rawtime;
	struct tm * timeinfo;
	
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	for (int i = 0;i < MAX_PLAYER_ITEM;i++)
	{

		if (Item[m_Items[i].nIdx].GetTime()->bYear)
		if (Item[m_Items[i].nIdx].GetTime()->bYear - 1900 < timeinfo->tm_year)
		{
			Remove(m_Items[i].nIdx);
			ItemSet.Remove(m_Items[i].nIdx);
		}
		else if (Item[m_Items[i].nIdx].GetTime()->bYear - 1900 == timeinfo->tm_year)
			if (Item[m_Items[i].nIdx].GetTime()->bMonth < timeinfo->tm_mon + 1)
			{
				Remove(m_Items[i].nIdx);
				ItemSet.Remove(m_Items[i].nIdx);
			}
			else if (Item[m_Items[i].nIdx].GetTime()->bMonth == timeinfo->tm_mon + 1)
				if (Item[m_Items[i].nIdx].GetTime()->bDay < timeinfo->tm_mday)
				{
					Remove(m_Items[i].nIdx);
					ItemSet.Remove(m_Items[i].nIdx);
				}
				else if (Item[m_Items[i].nIdx].GetTime()->bDay == timeinfo->tm_mday)
					if (Item[m_Items[i].nIdx].GetTime()->bHour <= timeinfo->tm_hour)
					{
						Remove(m_Items[i].nIdx);
						ItemSet.Remove(m_Items[i].nIdx);
					}
	}
}
#endif
//End

#ifdef _SERVER
	void KItemList::UpdateCurItem(int nGameIdx)
	{
		S2C_ITEM_REFRESH pItemSync;
		pItemSync.ProtocolType	= s2c_refestitem;
		pItemSync.m_nId 		= nGameIdx;
		pItemSync.m_dwID 		= Item[nGameIdx].GetID();
		pItemSync.m_Genre 		= Item[nGameIdx].GetGenre();
		pItemSync.m_Detail 		= Item[nGameIdx].GetDetailType();
		pItemSync.m_Particur 	= Item[nGameIdx].GetParticular();
		pItemSync.m_Series 		= Item[nGameIdx].GetSeries();
		pItemSync.m_Level 		= Item[nGameIdx].GetLevel();
		pItemSync.m_BindItem 		= Item[nGameIdx].GetBindItem();
		pItemSync.iShopCost 		= Item[nGameIdx].GetShopCost();
		pItemSync.m_nKind	= Item[nGameIdx].m_SpecialParam.uItemType;
		pItemSync.m_Time.bYear = Item[nGameIdx].GetTime()->bYear;
		pItemSync.m_Time.bMonth = Item[nGameIdx].GetTime()->bMonth;
		pItemSync.m_Time.bDay = Item[nGameIdx].GetTime()->bDay;
		pItemSync.m_Time.bHour = Item[nGameIdx].GetTime()->bHour;
		pItemSync.m_nFiFongArmor = Item[nGameIdx].GetFiFongArmor();
		pItemSync.iNgoaiTrang 		= Item[nGameIdx].GetNgoaiTrang();
		if (g_pServer)
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&pItemSync, sizeof(S2C_ITEM_REFRESH));
	}
	#endif

	int		KItemList::CountCommonItem(int nItemNature, int nItemGenre, int nDetailType, int nLevel, int nSeries, int P)
{
	int		nIdx = 0;
	int nResult = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		int nGameIdx = m_Items[nIdx].nIdx;

		//if (nItemNature != Item[nGameIdx].GetNature())
		//	continue;

		if (nItemGenre != Item[nGameIdx].GetGenre())
			continue;

		if (nDetailType > -1 && (nDetailType != Item[nGameIdx].GetDetailType()))
			continue;

		if (nLevel > -1 && (nLevel != Item[nGameIdx].GetLevel()))
			continue;

		if (nSeries > -1 && (nSeries != Item[nGameIdx].GetSeries()))
			continue;

		if (m_Items[nIdx].nPlace != P)
			continue;

		if (Item[nGameIdx].IsStack())
			nResult += Item[nGameIdx].IsStack();
		else
			nResult++;
	}
	return nResult;
}
