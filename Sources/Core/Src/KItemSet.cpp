#include "KCore.h"
#include "KItem.h"
#include "KItemGenerator.h"
#include "KItemSet.h"
//#include "MyAssert.h"

KItemSet	ItemSet;
/*!*****************************************************************************
// Function		: KItemSet::KItemSet
// Purpose		: 
// Return		: 
// Comments		:
// Author		: Spe
*****************************************************************************/
KItemSet::KItemSet()
{
	m_dwIDCreator = 100;
	ZeroMemory(&m_sRepairParam, sizeof(REPAIR_ITEM_PARAM));
#ifdef _SERVER
	m_psItemInfo = NULL;
	m_psBackItemInfo = NULL;
#endif

#ifdef _SERVER
	int             m_checkdel;
#endif


}

KItemSet::~KItemSet()
{
#ifdef _SERVER
	if (m_psItemInfo)
		delete [] m_psItemInfo;
	m_psItemInfo = NULL;
	if (m_psBackItemInfo)
		delete [] m_psBackItemInfo;
	m_psBackItemInfo = NULL;
#endif
}

/*!*****************************************************************************
// Function		: KItemSet::Init
// Purpose		: 
// Return		: void 
// Comments		:
// Author		: Spe
*****************************************************************************/
void KItemSet::Init()
{
	
#ifdef _SERVER
	if (m_psItemInfo)
		delete [] m_psItemInfo;
	m_psItemInfo = NULL;
	m_psItemInfo = new TRADE_ITEM_INFO[TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT];
	memset(this->m_psItemInfo, 0, sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);
	if (m_psBackItemInfo)
		delete [] m_psBackItemInfo;
	m_psBackItemInfo = NULL;
	m_psBackItemInfo = new TRADE_ITEM_INFO[TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT];
	memset(this->m_psBackItemInfo, 0, sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);


	printf("MAX_ITEM : %d | MAX_PLAYER_ITEM : %d\n",MAX_ITEM,MAX_PLAYER_ITEM);



#endif
	KIniFile	IniFile;
	IniFile.Load(ITEM_ABRADE_FILE);
//	维修价格
	IniFile.GetInteger("Repair", "ItemPriceScale", 100, &m_sRepairParam.nPriceScale);
	IniFile.GetInteger("Repair", "MagicPriceScale", 10, &m_sRepairParam.nMagicScale);
//	攻击磨损
	IniFile.GetInteger("Attack", "Weapon", 256, &m_nItemAbradeRate[enumAbradeAttack][itempart_weapon]);
	IniFile.GetInteger("Attack", "Head", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_head]);
	IniFile.GetInteger("Attack", "Body", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_body]);
	IniFile.GetInteger("Attack", "Belt", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_belt]);
	IniFile.GetInteger("Attack", "Foot", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_foot]);
	IniFile.GetInteger("Attack", "Cuff", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_cuff]);
	IniFile.GetInteger("Attack", "Amulet", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_amulet]);
	IniFile.GetInteger("Attack", "Ring1", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_ring1]);
	IniFile.GetInteger("Attack", "Ring2", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_ring2]);
	IniFile.GetInteger("Attack", "Pendant", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_pendant]);
	IniFile.GetInteger("Attack", "Horse", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_horse]);
	IniFile.GetInteger("Attack", "Mask", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_mask]);

// 防御磨损
	IniFile.GetInteger("Defend", "Weapon", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_weapon]);
	IniFile.GetInteger("Defend", "Head", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_head]);
	IniFile.GetInteger("Defend", "Body", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_body]);
	IniFile.GetInteger("Defend", "Belt", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_belt]);
	IniFile.GetInteger("Defend", "Foot", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_foot]);
	IniFile.GetInteger("Defend", "Cuff", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_cuff]);
	IniFile.GetInteger("Defend", "Amulet", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_amulet]);
	IniFile.GetInteger("Defend", "Ring1", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_ring1]);
	IniFile.GetInteger("Defend", "Ring2", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_ring2]);
	IniFile.GetInteger("Defend", "Pendant", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_pendant]);
	IniFile.GetInteger("Defend", "Horse", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_horse]);
	IniFile.GetInteger("Defend", "Mask", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_mask]);
// 移动磨损
	IniFile.GetInteger("Move", "Weapon", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_weapon]);
	IniFile.GetInteger("Move", "Head", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_head]);
	IniFile.GetInteger("Move", "Body", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_body]);
	IniFile.GetInteger("Move", "Belt", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_belt]);
	IniFile.GetInteger("Move", "Foot", 64, &m_nItemAbradeRate[enumAbradeMove][itempart_foot]);
	IniFile.GetInteger("Move", "Cuff", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_cuff]);
	IniFile.GetInteger("Move", "Amulet", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_amulet]);
	IniFile.GetInteger("Move", "Ring1", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_ring1]);
	IniFile.GetInteger("Move", "Ring2", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_ring2]);
	IniFile.GetInteger("Move", "Pendant", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_pendant]);
	IniFile.GetInteger("Move", "Horse", 64, &m_nItemAbradeRate[enumAbradeMove][itempart_horse]);
	IniFile.GetInteger("Move", "Mask", 1, &m_nItemAbradeRate[enumAbradeMove][itempart_mask]);



m_nNextAddItem = 1;
m_nRemoteItem[0] = 0;


}

/*!*****************************************************************************
// Function		: KItemSet::SearchID
// Purpose		: 
// Return		: int 
// Argumant		: DWORD dwID
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemSet::SearchID(DWORD dwID)
{
	int nIdx = 0;


	for (int i=1;i<MAX_ITEM;i++)
	{

	if (!Item[i].GetShow() || Item[i].GetID() <= 0)
		continue;

	if (Item[i].GetID() == dwID)
	{
		nIdx = i;
		break;
	}


	}
	
	return nIdx;
	
}

int KItemSet::Add(KItem* pItem)
{
	KASSERT(NULL != pItem);

	int i = FindFree();

	if (!i)
		return 0;

	Item[i] = *pItem;

	Item[i].Add();

#ifdef _SERVER
	SetID(i);
#endif

	if (m_nRemoteItem[0] > 0 && m_nRemoteItem[0] < MAX_ITEM)
	{
	m_nRemoteItem[m_nRemoteItem[0]] = 0;
	m_nRemoteItem[0] --;
	}
	else if (m_nNextAddItem > 0 && m_nNextAddItem < MAX_ITEM)
	{
	m_nNextAddItem ++;
	}
	else
	{
	printf("Xay ra loi Add Item Set !\n");
	}

	return i;

}
/*!*****************************************************************************
// Function		: KItemSet::Add
// Purpose		: 
// Return		: int 数组编号
// Argumant		: int 道具类型（装备？药品？矿石？……）
// Argumant		: int 魔法等级（如对应于装备，就是一般装备，蓝色装备，亮金等……）
// Argumant		: int 五行属性
// Argumant		: int 等级
// Argumant		: int 幸运值
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemSet::Add(int nItemGenre, int nSeries, 
				  int nLevel, int nLuck, int nDetailType/*=-1*/, 
				  int nParticularType/*=-1*/, int* pnMagicLevel, int nVersion/*=0*/, UINT nRandomSeed, IN int nYear , IN int nMonth , IN int nDay , IN int nHour, IN int iNgoaiTrang ) // Time Item
{

	int i = FindFree();
	
	if (i == 0)
		return 0;

	KItem*	pItem = &Item[i];

	pItem->m_GeneratorParam.nVersion = nVersion;
	pItem->m_GeneratorParam.uRandomSeed = nRandomSeed;
	pItem->m_CommonAttrib.iNgoaiTrang = iNgoaiTrang;
	

	
	switch(nItemGenre)
	{
	case item_equip:			// 装备类
		ItemGen.Gen_Equipment(nDetailType, nParticularType, nSeries, nLevel, pnMagicLevel, nLuck, nVersion, pItem);
		break;
	case item_medicine:			// 药品类
		ItemGen.Gen_Medicine(nDetailType, nLevel, nVersion, pItem);
		break;
	case item_mine:				// 矿石类
		break;
	case item_materials:		// 药材类
		break;
	case item_task:				// 默认是任务类
		ItemGen.Gen_Quest(nDetailType, pItem, nVersion,pnMagicLevel,nSeries);
		break;
	case item_townportal:
		ItemGen.Gen_TownPortal(pItem); 
		break;
	default:
		break;
	}

	Item[i].Add();


#ifdef _SERVER
	SetID(i);
#endif
	// Time Item
	pItem->SetTime(nYear,nMonth,nDay,nHour);
	// End
	if (m_nRemoteItem[0] > 0 && m_nRemoteItem[0] < MAX_ITEM)
	{
	m_nRemoteItem[m_nRemoteItem[0]] = 0;
	m_nRemoteItem[0] --;
	}
	else if (m_nNextAddItem > 0 && m_nNextAddItem < MAX_ITEM)
	{
	m_nNextAddItem ++;
	}
	else
	{
	printf("Xay ra loi Add Item Set !\n");
	}

	return i;
}






int KItemSet::AddExit(int nItemGenre, int nSeries, 
				  int nLevel, int nLuck, int nDetailType/*=-1*/, 
				  int nParticularType/*=-1*/, int* pnMagicLevel, int nVersion/*=0*/, UINT nRandomSeed,IN int nYear , IN int nMonth, IN int nDay, IN int nHour)
{
	int i = FindFree();
	
	if (i == 0)
		return 0;


	KItem*	pItem = &Item[i];

	pItem->m_GeneratorParam.nVersion = nVersion;
	pItem->m_GeneratorParam.uRandomSeed = nRandomSeed;
	switch(nItemGenre)
	{
	case item_equip:			// 装备类
		ItemGen.Gen_ExistEquipment(nDetailType, nParticularType, nSeries, nLevel, pnMagicLevel, nLuck, nVersion, pItem);
		break;
	case item_medicine:			// 药品类
		ItemGen.Gen_Medicine(nDetailType, nLevel, nVersion, pItem);
		break;
	case item_mine:				// 矿石类
		break;
	case item_materials:		// 药材类
		break;
	case item_task:				// 默认是任务类
		ItemGen.Gen_Quest(nDetailType, pItem, nVersion,pnMagicLevel,nSeries);
		break;
	case item_townportal:
		ItemGen.Gen_TownPortal(pItem); 
		break;
	default:
		break;
	}

	Item[i].Add();


#ifdef _SERVER
	SetID(i);
#endif
	// Time Item
	pItem->SetTime(nYear,nMonth,nDay,nHour);
	// End
	
	if (m_nRemoteItem[0] > 0 && m_nRemoteItem[0] < MAX_ITEM)
	{
	m_nRemoteItem[m_nRemoteItem[0]] = 0;
	m_nRemoteItem[0] --;
	}
	else if (m_nNextAddItem > 0 && m_nNextAddItem < MAX_ITEM)
	{
	m_nNextAddItem ++;
	}
	else
	{
	printf("Xay ra loi AddExit Item Set !\n");
	}

	return i;
}








BOOL KItemSet::Get_SizeItem(int nItemGenre,
							int nDetailType,
							int nParticularType,
							int nLevel,
							int* nWidth,
							int* nHeight
								   )
{

return ItemGen.Get_SizeItem(nItemGenre,
							nDetailType,
							nParticularType,
							nLevel,
							nWidth,
							nHeight
								   );
}






/*!*****************************************************************************
// Function		: KItemSet::FindFree
// Purpose		: 
// Return		: int 
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemSet::FindFree()
{

	int nF = 0;

	if (m_nRemoteItem[0] > 0 && m_nRemoteItem[0] < MAX_ITEM)
	{
	nF = m_nRemoteItem[m_nRemoteItem[0]];

	if (nF <= 0 || nF >= MAX_ITEM)
	{

	printf("Loi m_nRemoteItem [%d] !\n",nF);

	}



	}
	else if (m_nNextAddItem > 0 && m_nNextAddItem < MAX_ITEM)
	{
	nF =  m_nNextAddItem;
	}
	
	if (nF <= 0 || nF >= MAX_ITEM || Item[nF].GetShow())
	{
		printf("Khong the Item Set FinFree [%d] [%d] !\n",m_nRemoteItem[0],m_nNextAddItem);
		nF = 0;
	}

	
	return nF;
}

void KItemSet::Remove(IN int nIdx,IN BOOL bcheck)
{


#ifdef _SERVER

if (nIdx <= 0 || nIdx >= MAX_ITEM)
{
printf("Loi Del Item Set Idx %d %d\n",nIdx,m_checkdel);
return;
}


if (!Item[nIdx].GetShow())
{
printf("Loi Del Item Set Show %d %d\n",nIdx,m_checkdel);
return;
}


	
	if (!bcheck)
	{

	if (Item[nIdx].GetGenre() == item_equip && Item[nIdx].m_GeneratorParam.nVersion > 1)
	{
    printf("Del Item Khoa Loi %d !\n",m_checkdel);
	return;
	}

	}



//	BOOL bCheck = FALSE;

//	int ItemIdx = 0;

//	while(1)
//	{
//		ItemIdx = m_UseIdx.GetNext(ItemIdx);
//		if (ItemIdx == nIdx)
//		{

	 Item[nIdx].Remove();

    if (m_nRemoteItem[0] >= 0 && m_nRemoteItem[0] < (MAX_ITEM-1))
	{
	m_nRemoteItem[m_nRemoteItem[0]+1] = nIdx;
	m_nRemoteItem[0] ++;
	}	
	else
	{
	printf("Xay ra loi Remove Item Set !\n");
	}
	      
//		  bCheck = TRUE;
//			break;
//		}
//	}

//	if (!bCheck)
//	printf("Xay ra loi del Item !");

#endif

#ifndef _SERVER

if (nIdx <= 0 || nIdx >= MAX_ITEM)
{
return;
}

if (!Item[nIdx].GetShow())
{
return;
}
	Item[nIdx].Remove();

	if (m_nRemoteItem[0] >= 0 && m_nRemoteItem[0] < (MAX_ITEM-1))
	{
	m_nRemoteItem[m_nRemoteItem[0]+1] = nIdx;
	m_nRemoteItem[0] ++;
	}	
	else
	{
	printf("Xay ra loi Remove Item Set !\n");
	}

#endif

}

void KItemSet::SetID(IN int nIdx)
{
	Item[nIdx].SetID(m_dwIDCreator);
	m_dwIDCreator++;

	if (m_dwIDCreator > 2000000000)
		printf("Max DWORD !!!\n");
}

#ifdef _SERVER
//---------------------------------------------------------------------------
//	功能：copy m_psItemInfo to m_psBackItemInfo
//---------------------------------------------------------------------------
void	KItemSet::BackItemInfo()
{
	_ASSERT(this->m_psItemInfo);
	_ASSERT(this->m_psBackItemInfo);
	if (!m_psItemInfo)
		return;
	if (!m_psBackItemInfo)
		m_psBackItemInfo = new TRADE_ITEM_INFO[TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT];
	memcpy(m_psBackItemInfo, this->m_psItemInfo, sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);
}
#endif

int KItemSet::GetAbradeRange(IN int nType, IN int nPart)
{
	if (nType < 0 || nType >= enumAbradeNum)
		return 0;
	if (nPart < 0 || nPart >= itempart_num)
		return 0;

	return m_nItemAbradeRate[nType][nPart];
}