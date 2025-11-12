//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItemGenerator.CPP
// Date:	2002.08.26
// Code:	DongBo
// Desc:    CPP file. 本文件实现的类用于生成道具
//---------------------------------------------------------------------------
#include "KEngine.h"
#include "KCore.h"
#include "MyAssert.H"
#include "KItem.h"
#include "KSubWorldSet.h"
#include "KItemGenerator.h"

KItemGenerator	ItemGen;			//	装备生成器

KItemGenerator::KItemGenerator()
{
}

KItemGenerator::~KItemGenerator()
{
}

/******************************************************************************
	功能：	数据初始化. 从tab file中读取数据
******************************************************************************/
BOOL KItemGenerator::Init()
{
	if (!m_BPTLib.Init())		// 此调用从若干的tab file中载入所有的初始属性
		return FALSE;

	for (int i = equip_meleeweapon; i < equip_detailnum - equip_meleeweapon; i++)
	{
		switch(i)
		{
		case equip_meleeweapon:
			m_EquipNumOfEntries[i] = m_BPTLib.GetMeleeWeaponRecordNumber();
			break;
		case equip_rangeweapon:
			m_EquipNumOfEntries[i] = m_BPTLib.GetRangeWeaponRecordNumber();
			break;
		case equip_armor:
			m_EquipNumOfEntries[i] = m_BPTLib.GetArmorRecordNumber();
			break;
		case equip_helm:
			m_EquipNumOfEntries[i] = m_BPTLib.GetHelmRecordNumber();
			break;
		case equip_boots:
			m_EquipNumOfEntries[i] = m_BPTLib.GetBootRecordNumber();
			break;
		case equip_belt:
			m_EquipNumOfEntries[i] = m_BPTLib.GetBeltRecordNumber();
			break;
		case equip_cuff:
			m_EquipNumOfEntries[i] = m_BPTLib.GetCuffRecordNumber();
			break;
		case equip_amulet:
			m_EquipNumOfEntries[i] = m_BPTLib.GetAmuletRecordNumber();
			break;
		case equip_ring:
			m_EquipNumOfEntries[i] = m_BPTLib.GetRingRecordNumber();
			break;
		case equip_pendant:
			m_EquipNumOfEntries[i] = m_BPTLib.GetPendantRecordNumber();
			break;
		case equip_horse:
			m_EquipNumOfEntries[i] = m_BPTLib.GetHorseRecordNumber();
			break;
		case equip_mask: // MASK
			m_EquipNumOfEntries[i] = m_BPTLib.GetMaskRecordNumber();
			break;
		case equip_fifong:
			m_EquipNumOfEntries[i] = m_BPTLib.GetFifongRecordNumber();
			break;	
		}
	
	}
	m_MedNumOfEntries = m_BPTLib.GetMedicineRecordNumber();
	return TRUE;
/*
	m_BPTLib.Init();
	int naryMALevel[] = {1,2,3,4,5,6};
	KItem theItem;
	Gen_Equipment(1, 1, 0, 5, naryMALevel, 45, &theItem);
	return TRUE;
*/
}

/******************************************************************************
	功能：	生成药品
	入口:	nDetailType: 药品类型
			nLevel: 等级
			nVersion: 版本号
	出口:	成功时返回非零, 相关数据在 pItem 所指对象中给出
			失败时返回零
******************************************************************************/
BOOL KItemGenerator::Gen_Medicine(IN int nDetailType,
								  IN int nLevel,
								  IN int nVersion,
								  IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

pItem->m_SpecialParam.uItemType = 0;
pItem->m_SpecialParam.uItemGroup = 0;
pItem->m_SpecialParam.uItemGroupCount = 0;
pItem->m_nPriceShop = 0;
pItem->m_nPriceGold = 0;


pItem->m_nPriceTL = 0;
pItem->m_nPriceVD = 0;

pItem->m_nPoitAbrate = 0;

#ifndef _SERVER
for (int p=0;p<11;p++)
{
strcpy(pItem->m_GroupItemName[p],"");
}
pItem->m_ItemLevel = 0;
#endif

for (int e=0;e<11;e++)
{
for (int f=0;f<3;f++)
{
pItem->m_GroupItemNameInfo[e][f] = 0;
}
}



	BOOL bEC = FALSE;
	
#ifdef _SERVER	// 服务器版本
	pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed();
	pItem->m_GeneratorParam.nVersion = nVersion;
	pItem->m_GeneratorParam.nLuck = 0;
#else
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	int nGameVersion = pItem->m_GeneratorParam.nVersion;
#endif

	const int i = nDetailType * 5 + nLevel - 1;	// 本公式由策划定义
												// 等级数据从1开始，所以要减回来
	const KBASICPROP_MEDICINE* pMed = NULL;
	pMed = m_BPTLib.GetMedicineRecord(i);
	if (NULL == pMed)
	{
		_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pMed;
	return TRUE;
}
/******************************************************************************
	功能：	生成随机装备
	入口:	nSeriesReq: 五行属性
			nLevel: 等级
			nLucky: 运气值
			nVersion: 版本号
	出口:	成功时返回非零, 相关数据在 pItem 所指对象中给出
			失败时返回零
******************************************************************************/
BOOL KItemGenerator::Gen_Item(IN int nItemGenre,
							  IN int nSeriesReq,
							  IN int nLevel,
							  IN int nLucky,
							  IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	switch (nItemGenre)
	{
	case item_equip:
		{
			int nDetailType, nParticularType, i;
			nDetailType = g_Random(equip_detailnum);
			nParticularType = g_Random(m_EquipNumOfEntries[nDetailType] + 1);
			int naryMALevel[6];
			ZeroMemory(naryMALevel, 6 * sizeof(int));
			for (i = 0; i < 6; i++)
			{
				naryMALevel[i] = g_Random(nLevel / 10 + 1);
				if (!naryMALevel[i])
					break;
			}
			Gen_Equipment(nDetailType, nParticularType, nSeriesReq, nLevel, naryMALevel, nLucky, g_SubWorldSet.GetGameVersion(), pItem);
		}
		break;
	case item_medicine:
		{
			int nDetailType;
			nDetailType = g_Random(medicine_detailnum);
			Gen_Medicine(nDetailType, nLevel, g_SubWorldSet.GetGameVersion(), pItem);
		}
		break;
	case item_task:
		{

			int naryMALevel[6];
			ZeroMemory(naryMALevel, 6 * sizeof(int));
			Gen_Quest(nSeriesReq, pItem,g_SubWorldSet.GetGameVersion(),naryMALevel,nSeriesReq);			
		}
		break;
	case item_townportal:
		{
			Gen_TownPortal(pItem);
		}
		break;
	default:
		break;
	}
	
	return TRUE;
}
/******************************************************************************
	功能：	生成随机装备
	入口:	nDetailType: 具体类别, 如近程武器、暗器、......
			nParticularType: 详细类别
			nSeriesReq: 五行属性
			nLevel: 等级
			pnaryMALevel: 魔法属性等级数组[6]
			nLucky: 运气值
	出口:	成功时返回非零, 相关数据在 pItem 所指对象中给出
			失败时返回零
******************************************************************************/


BOOL KItemGenerator::Get_SizeItem(IN int nItemGenre,
								  IN int nDetailType,
								   IN int nParticularType,
								   IN int nLevel,
								   IN OUT int* nWidth,
								   IN OUT int* nHeight
								   )
{

	
BOOL bCheck = FALSE;
*nWidth = 0;
*nHeight = 0;

	switch(nItemGenre)
	{
	case item_equip:		
		{

   const KBASICPROP_EQUIPMENT* pEqu = NULL;
   const int i = nParticularType * 10 + nLevel - 1;
												
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask: // MASK
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;		
	case equip_fifong: // MASK
		pEqu = m_BPTLib.GetFifongRecord(i);
		break;		
		
	default:
		break;
	}

	if (pEqu != NULL)
	{

	*nWidth = pEqu->m_nWidth;
	*nHeight = pEqu->m_nHeight;

    bCheck = TRUE;
    }

		}
		break;
	case item_medicine:		
		{



	const int i = nDetailType * 5 + nLevel - 1;	
	const KBASICPROP_MEDICINE* pMed = NULL;
	pMed = m_BPTLib.GetMedicineRecord(i);

	if (pMed != NULL)
	{

	*nWidth = pMed->m_nWidth;
	*nHeight = pMed->m_nHeight;

    bCheck = TRUE;
    }

		}
		break;
	case item_mine:			
		break;
	case item_materials:		
		break;
	case item_task:				
		{

		const KBASICPROP_QUEST* pQuest = NULL;
	    pQuest = m_BPTLib.GetQuestRecord(nDetailType);

	if (pQuest != NULL)
	{
	*nWidth = pQuest->m_nWidth;
	*nHeight = pQuest->m_nHeight;
    bCheck = TRUE;
    }

		}
		break;
	case item_townportal:
		{
		const KBASICPROP_TOWNPORTAL* pPortal = NULL;
	    pPortal = m_BPTLib.GetTownPortalRecord(0);

	if (pPortal != NULL)
	{
	*nWidth = pPortal->m_nWidth;
	*nHeight = pPortal->m_nHeight;
    bCheck = TRUE;
    }

		}
		break;
	default:
		break;
	}

return bCheck;

}

BOOL KItemGenerator::Gen_Equipment(IN int nDetailType,
								   IN int nParticularType,
								   IN int nSeriesReq,
								   IN int nLevel,
								   IN const int* pnaryMALevel,
								   IN int nLucky,
								   IN int nVersion,
								   IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);
pItem->m_SpecialParam.uItemType = 0;
pItem->m_SpecialParam.uItemGroup = 0;
pItem->m_SpecialParam.uItemGroupCount = 0;
pItem->m_nPriceShop = 0;
pItem->m_nPriceGold = 0;


pItem->m_nPriceTL = 0;
pItem->m_nPriceVD = 0;

pItem->m_nPoitAbrate = 0;

#ifndef _SERVER
for (int p=0;p<11;p++)
{
strcpy(pItem->m_GroupItemName[p],"");
}
pItem->m_ItemLevel = 0;
#endif

for (int e=0;e<11;e++)
{
for (int f=0;f<3;f++)
{
pItem->m_GroupItemNameInfo[e][f] = 0;
}
}


	BOOL bEC = FALSE;
#ifdef _SERVER	// 服务器版本
	pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed();
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6);
	else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);
	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = nLucky;
#else
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6);
#endif
	int nGameVersion = pItem->m_GeneratorParam.nVersion;

	// 根据入口参数, 确定装备的基本数据
	const int i = nParticularType * 10 + nLevel - 1;// 本公式由策划定义
												// 详见SPE 2002年8月31日7:40
												// email: 基础道具数值表
												// 等级数据从1开始，所以要减回来
	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask: // MASK
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;	
	case equip_fifong:
		pEqu = m_BPTLib.GetFifongRecord(i);
		break;	
	default:
		break;
	}
	if (NULL == pEqu)
		{ _ASSERT(FALSE); return bEC; }
		// 运行至此的直接原因: 只有n种装备, 而上面 i 的值在[0,n-1]之外
		// 检查3点: nParticularType 有误?
		//			nLevel 有误?
		//			原始的tab file有误导致 m_BPTLib.m_BPTEquipment 所
		//			管理的数据有问题?
	pItem->SetAttrib_CBR(pEqu);
	pItem->SetSeries(nSeriesReq);

	if (NULL == pnaryMALevel)
		return TRUE;
	// 确定装备的魔法属性

if (pItem->m_SpecialParam.uItemType == 1)
{
KItemNormalAttrib	sMA_Gold[6];	// 道具的魔法属性	
bEC = Gen_MagicAttribGold(pEqu, pnaryMALevel, nLucky, sMA_Gold, nGameVersion);


pItem->SetSeries(pEqu->m_ItemSeries);

#ifndef _SERVER
pItem->m_ItemLevel = pEqu->m_ItemLevel;
#endif

if (bEC)
{



int NumberFind = 0;

for (int j=0;j<11;j++)
{
	switch(j)
	{
	case equip_meleeweapon:
		NumberFind = m_BPTLib.GetMeleeWeaponRecordNumber();
		break;
	case equip_rangeweapon:
		NumberFind = m_BPTLib.GetRangeWeaponRecordNumber();
		break;
	case equip_armor:
		NumberFind = m_BPTLib.GetArmorRecordNumber();
		break;
	case equip_helm:
		NumberFind = m_BPTLib.GetHelmRecordNumber();
		break;
	case equip_boots:
		NumberFind = m_BPTLib.GetBootRecordNumber();
		break;
	case equip_belt:
		NumberFind = m_BPTLib.GetBeltRecordNumber();
		break;
	case equip_amulet:
		NumberFind = m_BPTLib.GetAmuletRecordNumber();
		break;
	case equip_ring:
		NumberFind = m_BPTLib.GetRingRecordNumber();
		break;
	case equip_cuff:
		NumberFind = m_BPTLib.GetCuffRecordNumber();
		break;
	case equip_pendant:
		NumberFind = m_BPTLib.GetPendantRecordNumber();
		break;
	case equip_horse:
		NumberFind = m_BPTLib.GetHorseRecordNumber();
		break;
	default:
		break;
	}

for (int m=0;m<NumberFind;m++)
{
switch(j)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(m);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(m);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(m);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(m);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(m);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(m);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(m);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(m);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(m);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(m);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(m);
		break;
	default:
		break;
	}


if (pItem->m_SpecialParam.uItemType == 1 && pEqu->m_ItemGroup == pItem->m_SpecialParam.uItemGroup)
{
if (pItem->m_SpecialParam.uItemGroupCount < 11)
{

int CheckItemInGroup = 0;

for (int u=0;u<pItem->m_SpecialParam.uItemGroupCount;u++)
{
if (pItem->m_GroupItemNameInfo[u][1] == pEqu->m_nDetailType && pItem->m_GroupItemNameInfo[u][2] == pEqu->m_nParticularType)
{
CheckItemInGroup = 1;
break;
}
}

if (CheckItemInGroup == 0)
{
#ifndef _SERVER
strcpy(pItem->m_GroupItemName[pItem->m_SpecialParam.uItemGroupCount],pEqu->m_szName);
#endif
pItem->m_GroupItemNameInfo[pItem->m_SpecialParam.uItemGroupCount][0] = 0;
pItem->m_GroupItemNameInfo[pItem->m_SpecialParam.uItemGroupCount][1] = pEqu->m_nDetailType;
pItem->m_GroupItemNameInfo[pItem->m_SpecialParam.uItemGroupCount][2] = pEqu->m_nParticularType;
pItem->m_SpecialParam.uItemGroupCount++;
}


}
}

}

}





pItem->SetAttrib_MA(sMA_Gold);
}
else
{
pItem->m_SpecialParam.uItemType = 0;
KItemNormalAttrib	sMA[6];	
bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);
if (bEC)
pItem->SetAttrib_MA(sMA);
}

}


else if (pItem->m_SpecialParam.uItemType == 2)
{

KItemNormalAttrib	sMA[6];	
bEC = Gen_MagicAttribPink(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);


#ifndef _SERVER
pItem->m_ItemLevel = pEqu->m_ItemLevel;
#endif

if (bEC)
pItem->SetAttrib_MA(sMA);


}


else
{
KItemNormalAttrib	sMA[6];	
bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);
if (bEC)
pItem->SetAttrib_MA(sMA);
}

return bEC;

}

/******************************************************************************
	功能：	生成魔法属性
	入口:	nType: 装备类型
			pnaryMALevel: 数组指针, 给出魔法的等级要求, 共6个
			nSeriesReq: 五行属性
			nLucky: 运气值
	出口:	在 pnaryMA 所指数组中返回魔法的属性核心数据, 共6个
******************************************************************************/
// Add by Freeway Chen in 2003.5.30
BOOL KItemGenerator::Gen_MagicAttrib(
    int nType, const int* pnaryMALevel, int nSeriesReq, int nLucky,
	KItemNormalAttrib* pnaryMA, int nGameVersion
)
{
    int nResult = false;

    KBPT_ClassMAIT  SelectedMagicTable;	
	// 记录每一个记录选中的魔法属性，主要是为了最后将使用标志清除
    KMAGICATTRIB_TABFILE *pMagicAttrTable[6] = { NULL, NULL, NULL, NULL, NULL, NULL };

    int i = 0;

    _ASSERT(pnaryMALevel);
    _ASSERT(pnaryMA);

    for (i = 0; i < 6; i++) 
    {
        // 如果发现级别为零，表示不需要继续处理了，退出
        if (pnaryMALevel[i] == 0)	
        	break;

		if (pnaryMALevel[i] < 0 || pnaryMALevel[i] > 10)
		{

			printf("Da xac dinh duoc loi RollBack 1\n");
			break;
		}


        SelectedMagicTable.Clear();

        // 根据前缀后缀、物品类型、五行需求、需求级别将所有符合条件的魔法属性项取出来
        const KBPT_ClassMAIT *pCMITItem = m_BPTLib.GetCMIT(
            1 - (i & 1),        // 前缀后缀
            nType,              // 物品类型
            nSeriesReq,         // 五行需求 
            pnaryMALevel[i]     // 需求级别
        );

        _ASSERT(pCMITItem);
        if (!pCMITItem)
        {
            g_DebugLog(
                "[GenMagicAttrib] GetCMIT Error, nPrefixPostfix = %d, nType = %d, nSeries = %d, nLevel = %d", 
                1 - (i & 1), nType, nSeriesReq, pnaryMALevel[i]
            );
            break;
        }

		if (nLucky < 0)
			{

			printf("Da xac dinh duoc loi RollBack 2\n");
			break;
		}

        // 遍历符合条件的魔法属性，根据
        // 1.是否已经使用，
        // 2.运气值
        // 3.是否有相同的“属性调整类别”
        // 取出相应的项放在SelectedMagicTable中
        int nCMITItemCount = pCMITItem->GetCount();
        int j = 0;
        int nDecide = GetRandomNumber(0, 100 - 1) / (1 + nLucky * 20 / 100);
		
        for (j = 0; j < nCMITItemCount; j++)
        {
            // 根据索引值，取出相应的项
            int nMAIndex = pCMITItem->Get(j);
            KMAGICATTRIB_TABFILE *pMAItem = (KMAGICATTRIB_TABFILE *)m_BPTLib.GetMARecord(nMAIndex);
            if (!pMAItem)
            {
                _ASSERT(pMAItem);
                continue;
            }
			
			
            // 如果已经使用，跳过取下一个
            if (pMAItem->m_nUseFlag)
                continue;

            // 如果运气不足，取下一个
            if ((pMAItem->m_DropRate[nType]) <= nDecide)
                continue;
				
			if (pMAItem->m_MagicAttrib.nPropKind == 135)  // may man
			{
				if (GetRandomNumber(0,80) != 0)
				continue;
			}
			
			if (pMAItem->m_MagicAttrib.nPropKind == 104)  // phong thu vat ly 
			{
				if (GetRandomNumber(0,4) != 0)
				continue;
			}

			if (pMAItem->m_MagicAttrib.nPropKind == 114)  // khang tat ca 
			{
				if (GetRandomNumber(0,50) != 0)
				continue;
			}
			
			if (pMAItem->m_MagicAttrib.nPropKind == 115)  // toc do danh 
			{
				if (GetRandomNumber(0,5) != 0)
				continue;
			}
			if (pMAItem->m_MagicAttrib.nPropKind == 136) // hut noi luc
			{
				if (GetRandomNumber(0,29) != 0)
				continue;
			}
			if (pMAItem->m_MagicAttrib.nPropKind == 137)  // hut noi luc
			{
				if (GetRandomNumber(0,24) != 0)
				continue;
			}
				
	

			//if (GetRandomNumber(0,130) != 0)
			//continue;

			
			
			
			
            // 检查是否已经有相同的“属性调整类别”
            int k = 0;
            for (k = 0; k < i; k++) // Quet lai xem trong 6 dong da co dong nay chua
            {

				if (!pMagicAttrTable[k])
				{
				printf("Da xac dinh duoc loi RollBack 3\n");
			    continue;
				}
				

                _ASSERT(pMagicAttrTable[k]);

                if ((pMagicAttrTable[k]->m_MagicAttrib.nPropKind) == (pMAItem->m_MagicAttrib.nPropKind))
                    break;
            }

            // 如果发现 “属性调整类别”相同，说明已经有一个相同的值被选中，那么取下一个
            if (k < i)
                continue;
		
            SelectedMagicTable.Insert(nMAIndex);
        }

        int nSelectedCount = SelectedMagicTable.GetCount();

        if (nSelectedCount == 0)
        {   
            // 如果没有满足条件的魔法属性，就退出
            break;
        }
        
        // 如果发现满足条件的魔法属性，就设置
        int nLuckyItemIndex = GetRandomNumber(0, nSelectedCount - 1);
        KMAGICATTRIB_TABFILE *pMAItem = (KMAGICATTRIB_TABFILE *)m_BPTLib.GetMARecord(
             SelectedMagicTable.Get(nLuckyItemIndex)
        );


			if (!pMAItem)
				{
				printf("Da xac dinh duoc loi RollBack 4\n");
			    break;
				}




        // 保存满足条件的魔法属性，以便在最后的时候将使用标志清除
        pMagicAttrTable[i] = pMAItem;

        _ASSERT(pMAItem);
        _ASSERT(!(pMAItem->m_nUseFlag));     // 确定还没有使用

        // 设置使用标志
        pMAItem->m_nUseFlag = true;
		
        pnaryMA[i].nAttribType = pMAItem->m_MagicAttrib.nPropKind;
        pnaryMA[i].nValue[0] = GetRandomNumber(
            pMAItem->m_MagicAttrib.aryRange[0].nMin, 
            pMAItem->m_MagicAttrib.aryRange[0].nMax
        );
        pnaryMA[i].nValue[1] = GetRandomNumber(
            pMAItem->m_MagicAttrib.aryRange[1].nMin, 
            pMAItem->m_MagicAttrib.aryRange[1].nMax
        );
        pnaryMA[i].nValue[2] = GetRandomNumber(
            pMAItem->m_MagicAttrib.aryRange[2].nMin, 
            pMAItem->m_MagicAttrib.aryRange[2].nMax
        );
		g_DebugLog(
			"[GenMagicAttrib] pnaryMA[%d]: (%d:%s), nAttribType = %d, nValue[0] = %d, nValue[1] = %d, nValue[2] = %d,", 
			i,
			SelectedMagicTable.Get(nLuckyItemIndex),
			pMAItem->m_szName,
			pnaryMA[i].nAttribType,
			pnaryMA[i].nValue[0],
			pnaryMA[i].nValue[1],
			pnaryMA[i].nValue[2]
		);

    } // for 遍历需要取回的魔法属性表

    for (NULL; i < 6; i++)
    {
        // 清空剩下的项
        pnaryMA[i].nAttribType = 0;
        pnaryMA[i].nValue[0] = 0;
        pnaryMA[i].nValue[1] = 0;
        pnaryMA[i].nValue[2] = 0;
    }

    for (i = 0; i < 6; i++)
    {
        if (!pMagicAttrTable[i])
            break;

        _ASSERT(pMagicAttrTable[i]->m_nUseFlag);   // 确定已经使用

        pMagicAttrTable[i]->m_nUseFlag = false;    // 设置为没用使用
    }

    nResult = true;
//Exit0:
	return nResult;
}









BOOL KItemGenerator::Gen_MagicAttribPink(
    int nType, const int* pnaryMALevel, int nSeriesReq, int nLucky,
	KItemNormalAttrib* pnaryMA, int nGameVersion
)
{
    int nResult = false;

    KBPT_ClassMAIT  SelectedMagicTable;	
	
    KMAGICATTRIB_TABFILE *pMagicAttrTable[6] = { NULL, NULL, NULL, NULL, NULL, NULL };

    int i = 0;

    BOOL bCheckPink = FALSE;

	

    for (i = 0; i < 6; i++) 
    {
        
        if (pnaryMALevel[i] <= 0)	
        	break;

		

		int pnaryMALevelPink = (pnaryMALevel[i] % 100)/10 + 1;
		int pnaryMALuckyPink = (pnaryMALevel[i] % 10000) / 100 + 1;
		int pnaryMAIdPink = pnaryMALevel[i] / 10000;


		if (pnaryMAIdPink <= 0)
			break;

		if (pnaryMAIdPink == magic_itempinknomagic)
		{
		pnaryMA[i].nAttribType = magic_itempinknomagic;
        pnaryMA[i].nValue[0] = 0;
        pnaryMA[i].nValue[1] = 0;
        pnaryMA[i].nValue[2] = 0;
		bCheckPink = TRUE;
		continue;
		}


		if (bCheckPink)
		{
			break;
		}


		
        
        const KBPT_ClassMAIT *pCMITItem = m_BPTLib.GetCMIT(
            1 - (i & 1),        
            nType,              
            nSeriesReq,         
            pnaryMALevelPink    
        );

    
        if (!pCMITItem)
        {
            
            break;
        }

        
        int nCMITItemCount = pCMITItem->GetCount();
        int j = 0;
      
		int pnaryjS = -1;

        for (j = 0; j < nCMITItemCount; j++)
        {
            
            int nMAIndex = pCMITItem->Get(j);
            KMAGICATTRIB_TABFILE *pMAItem = (KMAGICATTRIB_TABFILE *)m_BPTLib.GetMARecord(nMAIndex);
            if (!pMAItem)
            {
                _ASSERT(pMAItem);
                continue;
            }

            
            if (pMAItem->m_nUseFlag)
                continue;
            
           

            if (pnaryMAIdPink == (pMAItem->m_MagicAttrib.nPropKind))
			{

				if (pMAItem->m_nLevel == pnaryMALevelPink)
				{
				   pnaryjS = j;
                   break;
				}
				else if (pMAItem->m_nLevel < pnaryMALevelPink)
				{
				    pnaryjS = j;
				}


			}
           
        }

        

		if (pnaryjS < 0 || pnaryjS >= nCMITItemCount)
                break;
      
        
        
       
        KMAGICATTRIB_TABFILE *pMAItem = (KMAGICATTRIB_TABFILE *)m_BPTLib.GetMARecord(pCMITItem->Get(pnaryjS));

       
        pMagicAttrTable[i] = pMAItem;

       
        pMAItem->m_nUseFlag = true;


		int nTTH,nTTMin,nTTMax;

		if (pnaryMALuckyPink > 100)
			pnaryMALuckyPink = 100;

		if (pnaryMALuckyPink < 1)
			pnaryMALuckyPink = 1;


	
		nTTH = pMAItem->m_MagicAttrib.aryRange[0].nMax - pMAItem->m_MagicAttrib.aryRange[0].nMin;

		if (GetRandomNumber(1,100) <= pnaryMALuckyPink)
		{
		nTTMin = pMAItem->m_MagicAttrib.aryRange[0].nMin + nTTH * (pnaryMALuckyPink+10)/120;
		nTTMax = pMAItem->m_MagicAttrib.aryRange[0].nMax;
		}
		else
		{
		nTTMin = pMAItem->m_MagicAttrib.aryRange[0].nMin;
		nTTMax = pMAItem->m_MagicAttrib.aryRange[0].nMin + nTTH * (pnaryMALuckyPink+10)/120;	
		}
	


		
        pnaryMA[i].nAttribType = pMAItem->m_MagicAttrib.nPropKind;
        pnaryMA[i].nValue[0] = GetRandomNumber(
            nTTMin, 
            nTTMax
        );




		nTTH = pMAItem->m_MagicAttrib.aryRange[1].nMax - pMAItem->m_MagicAttrib.aryRange[1].nMin;

		if (GetRandomNumber(1,100) <= pnaryMALuckyPink)
		{
		nTTMin = pMAItem->m_MagicAttrib.aryRange[1].nMin + nTTH * (pnaryMALuckyPink+10)/120;
		nTTMax = pMAItem->m_MagicAttrib.aryRange[1].nMax;
		}
		else
		{
		nTTMin = pMAItem->m_MagicAttrib.aryRange[1].nMin;
		nTTMax = pMAItem->m_MagicAttrib.aryRange[1].nMin + nTTH * (pnaryMALuckyPink+10)/120;	
		}



        pnaryMA[i].nValue[1] = GetRandomNumber(
            nTTMin, 
            nTTMax
        );




		nTTH = pMAItem->m_MagicAttrib.aryRange[2].nMax - pMAItem->m_MagicAttrib.aryRange[2].nMin;

		if (GetRandomNumber(1,100) <= pnaryMALuckyPink)
		{
		nTTMin = pMAItem->m_MagicAttrib.aryRange[2].nMin + nTTH * (pnaryMALuckyPink+10)/120;
		nTTMax = pMAItem->m_MagicAttrib.aryRange[2].nMax;
		}
		else
		{
		nTTMin = pMAItem->m_MagicAttrib.aryRange[2].nMin;
		nTTMax = pMAItem->m_MagicAttrib.aryRange[2].nMin + nTTH * (pnaryMALuckyPink+10)/120;	
		}


        pnaryMA[i].nValue[2] = GetRandomNumber(
            nTTMin, 
            nTTMax
        );
		
		
    } 

    for (NULL; i < 6; i++)
    {
     
        pnaryMA[i].nAttribType = 0;
        pnaryMA[i].nValue[0] = 0;
        pnaryMA[i].nValue[1] = 0;
        pnaryMA[i].nValue[2] = 0;
    }

    for (i = 0; i < 6; i++)
    {
        if (!pMagicAttrTable[i])
            break;



        pMagicAttrTable[i]->m_nUseFlag = false;    
    }

    nResult = true;

	return nResult;
}












BOOL KItemGenerator::Gen_MagicAttribGold(const KBASICPROP_EQUIPMENT* pEqu, const int* pnaryMALevel,int nLucky,
	KItemNormalAttrib* pnaryMA, int nGameVersion
)
{

 int nResult = false;
 int nItemGroup =  pEqu->m_ItemGroup;

int i=0;

if (nItemGroup > 0)
{

for (i=0; i < 6; i++)
 {

if (pEqu->m_aryPropGold[i].nType <= 0)
break;

 if (pEqu->m_aryPropGold[i].nType/1000 == 1)//tui hieu roi no co toi 4 so
 {
 pnaryMA[i].nAttribType = 204;
 pnaryMA[i].nValue[0] = GetRandomNumber(pEqu->m_aryPropGold[i].sRange.nMin,pEqu->m_aryPropGold[i].sRange.nMax);
 pnaryMA[i].nValue[1] = -1;
 pnaryMA[i].nValue[2] = pEqu->m_aryPropGold[i].nType%1000;
 }
 else
 {

 pnaryMA[i].nAttribType = pEqu->m_aryPropGold[i].nType;
 pnaryMA[i].nValue[0] = GetRandomNumber(pEqu->m_aryPropGold[i].sRange.nMin,pEqu->m_aryPropGold[i].sRange.nMax);

 if (pnaryMA[i].nAttribType == 126)
 {
 pnaryMA[i].nValue[2] = 6;
 }
 else
 {
 pnaryMA[i].nValue[2] = 0;
 }

 pnaryMA[i].nValue[1] = -1;

 }

 }

    
}


 for (NULL; i < 6; i++)
    {
        
        pnaryMA[i].nAttribType = 0;
        pnaryMA[i].nValue[0] = 0;
        pnaryMA[i].nValue[1] = 0;
        pnaryMA[i].nValue[2] = 0;
    }




nResult = true;

return nResult;
}











// Following Code is implement by DongBo
//BOOL KItemGenerator::Gen_MagicAttrib(int nType, const int* pnaryMALevel, int nSeriesReq, int nLucky,
//									 KItemNormalAttrib* pnaryMA)
//{
//	
//    // 确定适用于该装备的全部魔法属性及其数量
//	if (FALSE == GMA_GetAvaliableMA(nType))
//		{ _ASSERT(FALSE); return FALSE; }
//
//	// 确定上述各魔法属性的等级,出现概率
//	if (FALSE == GMA_GetLevelAndDropRate(nType))
//		{ _ASSERT(FALSE); return FALSE; }
//
//	// 从上述魔法属性中选出符合条件的. 策划要求选出6个
//	for (int i = 0; i < 6; i++)				// 奇数: 前缀, 偶数: 后缀
//	{
//		GMA_ChooseMA((i&1), pnaryMALevel[i], nLucky, &(pnaryMA[i]));
//		if (pnaryMA[i].nAttribType == 0)
//			break;
//	}
//	return TRUE;
//}

/******************************************************************************
	功能:	确定适用于指定装备的全部魔法属性及其数量
	入口:	nType: 指定装备
	出口:	成功时返回非零. m_GMA_naryMA[0]给出适用于该装备的全部魔法属性前缀
							m_GMA_nCount[0]给出上述魔法属性的总数
							m_GMA_naryMA[1]给出适用于该装备的全部魔法属性后缀
							m_GMA_nCount[1]给出上述魔法属性的总数
			失败时返回零
******************************************************************************/
BOOL KItemGenerator::GMA_GetAvaliableMA(int nType)
{
	//TODO: 可能需要对nType进行转换, 从装备类型值变成 m_BPTLib.m_CMAT 数组的索引
	for (int i = 0; i < 2; i++)		// 0: 前缀, 1: 后缀
	{
		const KBPT_ClassifiedMAT* pCMAT;
		pCMAT = m_BPTLib.GetCMAT(i, nType);
		if (pCMAT == NULL)
			{ _ASSERT(FALSE); return FALSE; }
		m_GMA_nCount[i] = NUMOFCMA;
		pCMAT->GetAll((int*)m_GMA_naryMA[i], &(m_GMA_nCount[i]));
	}
	return TRUE;
}

/******************************************************************************
	功能:	确定各给定魔法属性的等级要求和出现概率
	入口:	nType: 指定装备
			m_GMA_naryMA数组: 给出全部魔法属性的索引值
	出口:	成功时返回非零. m_GMA_naryLevel数组给出上述魔法属性的等级要求
							m_GMA_naryDropRate给出上述魔法属性的出现概率
			失败时返回零
******************************************************************************/
BOOL KItemGenerator::GMA_GetLevelAndDropRate(int nType)
{
	//TODO: 可能需要对nType进行转换, 从装备类型值变成 m_BPTLib.m_CMAT 数组的索引

	for (int i = 0; i < 2; i++)		// 0: 前缀, 1: 后缀
	{
		for (int n = 0; n < m_GMA_nCount[i]; n++)
		{
			const KMAGICATTRIB_TABFILE* pMATF;
			pMATF = GetMARecord(m_GMA_naryMA[i][n]);
			if (pMATF == NULL)
				{ _ASSERT(FALSE); return FALSE; }
			m_GMA_naryLevel[i][n] = pMATF->m_nLevel;
			m_GMA_naryDropRate[i][n] = pMATF->m_DropRate[nType];		// 可优化
		}
	}
	return TRUE;
}

/******************************************************************************
	功能:	从全部可用的魔法属性中选出符合条件的魔法
	入口:	nPos: 0: 选前缀, 1: 选后缀
			nLevel: 对魔法属性的等级要求
			nLucky: 运气值
			m_GMA_naryMA数组: 给出全部可用的魔法属性的索引值
	出口:	*pINA 给出所选魔法的核心参数
******************************************************************************/
void KItemGenerator::GMA_ChooseMA(int nPos, int nLevel, int nLucky, KItemNormalAttrib* pINA)
{
	_ASSERT(pINA != NULL);

	// 赋初值
	pINA->nAttribType = 0;
	pINA->nValue[0] = 0;
	pINA->nValue[1] = 0;
	pINA->nValue[2] = 0;

	// 筛选出全部可用的魔法属性
	int nDropRate = ::GetRandomNumber(0, 100) - nLucky;
	int nCount = GMA_GetCandidateMA(nPos, nLevel, nDropRate);
	if (nCount > 0)
	{
		int nFinal = ::GetRandomNumber(0, nCount);
		int nMAi = m_GMA_naryCandidateMA[nFinal];
		int nMA = m_GMA_naryMA[nPos][nMAi];
		if (nMA == -1)	// 随机出重复的属性了
		{
			pINA->nAttribType = 0;
			return;
		}
		m_GMA_naryMA[nPos][nMAi] = -1;		// 置为-1,表示该MA已用
		const KMAGICATTRIB_TABFILE* pMATF = GetMARecord(nMA);
		if (NULL != pMATF)
		{
			const KMACP* pMACP = &(pMATF->m_MagicAttrib);
			pINA->nAttribType = pMACP->nPropKind;
			pINA->nValue[0] = ::GetRandomNumber(pMACP->aryRange[0].nMin, pMACP->aryRange[0].nMax);
			pINA->nValue[1] = ::GetRandomNumber(pMACP->aryRange[1].nMin, pMACP->aryRange[1].nMax);
			pINA->nValue[2] = ::GetRandomNumber(pMACP->aryRange[2].nMin, pMACP->aryRange[2].nMax);
		}
	}
}

/******************************************************************************
	功能:	获取指定的魔法属性记录
	入口:	i: 指定记录
	出口:	成功时返回指向该记录的指针
			失败时返回NULL
******************************************************************************/
const KMAGICATTRIB_TABFILE* KItemGenerator::GetMARecord(int i) const
{
	return m_BPTLib.GetMARecord(i);
}

/******************************************************************************
	功能:	从全部可用的魔法属性中选出符合条件的魔法
	入口:	nPos: 0: 选前缀, 1: 选后缀
			nLevel: 对魔法属性的等级要求
			nDropRate: 选出出现概率大于此值的魔法属性
			m_GMA_naryMA数组: 给出全部可用的魔法属性的索引值
			m_GMA_naryLevel数组: 给出上述魔法属性的等级要求
			m_GMA_naryDropRate数组: 给出上述魔法属性的出现概率
	出口:	m_GMA_naryCandidateMA数组给出符合条件的魔法
	说明:	若 n = m_GMA_naryMA[nPos][m_GMA_naryCandidateMA[i]]
			则 GetMARecord(n) 返回指向魔法属性结构的指针
******************************************************************************/
int KItemGenerator::GMA_GetCandidateMA(int nPos, int nLevel, int nDropRate)
{
	int nCount = 0;
	for (int i = 0; i < m_GMA_nCount[nPos]; i++)
	{
		if (m_GMA_naryMA[nPos][i] != -1 &&		// -1表示该MA已用过
			m_GMA_naryLevel[nPos][i] == nLevel &&
			m_GMA_naryDropRate[nPos][i] >= nDropRate)
		{
			m_GMA_naryCandidateMA[nCount++] = i;
		}
	}
	return nCount;
}

BOOL KItemGenerator::GetMedicineCommonAttrib(IN int nDetailType, IN int nLevel, IN OUT KItem* pItem)
{
	if (NULL == pItem)
	{
		KASSERT(FALSE);
		return FALSE;
	}

	const int i = nDetailType * 5 + nLevel - 1;

	const KBASICPROP_MEDICINE* pMed = NULL;
	pMed = m_BPTLib.GetMedicineRecord(i);
	if (NULL == pMed)
	{
		_ASSERT(FALSE);
		return FALSE;
	}
	*pItem = *pMed;
	return TRUE;
}

/******************************************************************************
	功能：	随机生成黄金装备装备
	入口:	nCondition: 前提条件，(Emf+Pmf)*Level的运算结果
			pItem: 装备项目指针。
	出口:	成功时返回真, 相关数据在 pItem 所指对象中给出
			失败时返回假
******************************************************************************/
// Add by flying on May.30.2003
// Try to get a "Gold Item" if possible.
BOOL KItemGenerator::GetGoldItemByRandom(IN int nCondition, OUT KItem * pItem)
{
	BOOL bResult = FALSE;
	const KBASICPROP_EQUIPMENT_GOLD* pArray[MAX_ITEM];
	int i = 0;
	int j = 0;
	int nIndex = 0;

	::memset(pArray, 0, sizeof(KBASICPROP_EQUIPMENT_GOLD *) * MAX_ITEM);

	for (i = 0; i < MAX_ITEM; i++)
	{
		const KBASICPROP_EQUIPMENT_GOLD* pTemp = m_BPTLib.GetGoldItemRecord(i);
		if (pTemp == 0)
			break;
		if (pTemp->m_nRarity < nCondition)
			pArray[j] = pTemp;
	}

	if (j == 0)
		goto Exit0;			 
	nIndex = GetRandomNumber(0, j);	
	*pItem = *pArray[nIndex];
Exit0:
	return FALSE;
}

/******************************************************************************
	功能：	生成指定黄金装备装备
	入口:	nCondition: 前提条件，(Emf+Pmf)*Level的运算结果
			pItem: 装备项目指针。
	出口:	成功时返回真, 相关数据在 pItem 所指对象中给出
			失败时返回假
******************************************************************************/
// Add by flying on May.30.2003
// Try to get a "Gold Item" if possible.
BOOL KItemGenerator::GetGoldItemByIndex(IN int nIndex, OUT KItem * pItem)
{
	BOOL bResult = FALSE;
	int i = 0;
	int j = 0;
	int nCount = 0;

	nCount = m_BPTLib.GetGoldItemNumber();
	const KBASICPROP_EQUIPMENT_GOLD* pTemp = m_BPTLib.GetGoldItemRecord(i);
	if (nIndex >= nCount)
		goto Exit0;
	if (pTemp == NULL)
		goto Exit0;
	*pItem = *pTemp;
	bResult = TRUE;
Exit0:
	return bResult;
}

BOOL KItemGenerator::GetEquipmentCommonAttrib(IN int nDetailType, IN int nParticularType, IN int nLevel, IN int nSeries, IN OUT KItem* pItem)
{
	if (NULL == pItem)
	{
		KASSERT(FALSE);
		return FALSE;
	}


pItem->m_SpecialParam.uItemType = 0;
pItem->m_SpecialParam.uItemGroup = 0;
pItem->m_SpecialParam.uItemGroupCount = 0;
pItem->m_nPriceShop = 0;
pItem->m_nPriceGold = 0;

pItem->m_nPriceTL = 0;
pItem->m_nPriceVD = 0;

pItem->m_nPoitAbrate = 0;


#ifndef _SERVER
for (int p=0;p<11;p++)
{
strcpy(pItem->m_GroupItemName[p],"");
}
pItem->m_ItemLevel = 0;
#endif

for (int e=0;e<11;e++)
{
for (int f=0;f<3;f++)
{
pItem->m_GroupItemNameInfo[e][f] = 0;
}
}

	const int i = nParticularType * 10 + nLevel - 1;// 本公式由策划定义
												// 详见SPE 2002年8月31日7:40
												// email: 基础道具数值表
	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask: // MASK
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;	
	case equip_fifong:
		pEqu = m_BPTLib.GetFifongRecord(i);
		break;		
	default:
		break;
	}
	if (NULL == pEqu)
		{ _ASSERT(FALSE); return FALSE; }
		// 运行至此的直接原因: 只有n种装备, 而上面 i 的值在[0,n-1]之外
		// 检查3点: nParticularType 有误?
		//			nLevel 有误?
		//			原始的tab file有误导致 m_BPTLib.m_BPTEquipment 所
		//			管理的数据有问题?
	pItem->SetAttrib_CBR(pEqu);
	pItem->SetSeries(nSeries);
	return TRUE;
}

/******************************************************************************
	功能：	生成随机装备
	入口:	nDetailType: 具体类别, 如近程武器、暗器、......
			nParticularType: 详细类别
			nSeriesReq: 五行属性
			nLevel: 等级
			pnaryMALevel: 魔法属性等级数组[6]
			nLucky: 运气值
	出口:	成功时返回非零, 相关数据在 pItem 所指对象中给出
			失败时返回零
******************************************************************************/
BOOL KItemGenerator::Gen_ExistEquipment(IN int nDetailType,
								   IN int nParticularType,
								   IN int nSeriesReq,
								   IN int nLevel,
								   IN const int* pnaryMALevel,
								   IN int nLucky,
								   IN int nVersion,
								   IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);


pItem->m_SpecialParam.uItemType = 0;
pItem->m_SpecialParam.uItemGroup = 0;
pItem->m_SpecialParam.uItemGroupCount = 0;
pItem->m_nPriceShop = 0;
pItem->m_nPriceGold = 0;

pItem->m_nPriceTL = 0;
pItem->m_nPriceVD = 0;

pItem->m_nPoitAbrate = 0;

#ifndef _SERVER
for (int p=0;p<11;p++)
{
strcpy(pItem->m_GroupItemName[p],"");
}
pItem->m_ItemLevel = 0;
#endif
for (int e=0;e<11;e++)
{
for (int f=0;f<3;f++)
{
pItem->m_GroupItemNameInfo[e][f] = 0;
}
}

	BOOL bEC = FALSE;

	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6);
	int nGameVersion = pItem->m_GeneratorParam.nVersion;

	// 根据入口参数, 确定装备的基本数据
	const int i = nParticularType * 10 + nLevel - 1;// 本公式由策划定义
												// 详见SPE 2002年8月31日7:40
												// email: 基础道具数值表
												// 等级数据从1开始，所以要减回来
	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask: // MASK
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;		
	case equip_fifong:
			pEqu = m_BPTLib.GetFifongRecord(i);
			break;	
	default:
		break;
	}
	if (NULL == pEqu)
		{ _ASSERT(FALSE); return bEC; }
		// 运行至此的直接原因: 只有n种装备, 而上面 i 的值在[0,n-1]之外
		// 检查3点: nParticularType 有误?
		//			nLevel 有误?
		//			原始的tab file有误导致 m_BPTLib.m_BPTEquipment 所
		//			管理的数据有问题?
	

	
	
	pItem->SetAttrib_CBR(pEqu);
	pItem->SetSeries(nSeriesReq);

	if (NULL == pnaryMALevel)
		return TRUE;



if (pItem->m_SpecialParam.uItemType == 1)
{
KItemNormalAttrib	sMA_Gold[6];	// 道具的魔法属性	
bEC = Gen_MagicAttribGold(pEqu, pnaryMALevel, nLucky, sMA_Gold, nGameVersion);


pItem->SetSeries(pEqu->m_ItemSeries);

#ifndef _SERVER
pItem->m_ItemLevel = pEqu->m_ItemLevel;
#endif


if (bEC)
{

int NumberFind = 0;

for (int j=0;j<11;j++)
{
	switch(j)
	{
	case equip_meleeweapon:
		NumberFind = m_BPTLib.GetMeleeWeaponRecordNumber();
		break;
	case equip_rangeweapon:
		NumberFind = m_BPTLib.GetRangeWeaponRecordNumber();
		break;
	case equip_armor:
		NumberFind = m_BPTLib.GetArmorRecordNumber();
		break;
	case equip_helm:
		NumberFind = m_BPTLib.GetHelmRecordNumber();
		break;
	case equip_boots:
		NumberFind = m_BPTLib.GetBootRecordNumber();
		break;
	case equip_belt:
		NumberFind = m_BPTLib.GetBeltRecordNumber();
		break;
	case equip_amulet:
		NumberFind = m_BPTLib.GetAmuletRecordNumber();
		break;
	case equip_ring:
		NumberFind = m_BPTLib.GetRingRecordNumber();
		break;
	case equip_cuff:
		NumberFind = m_BPTLib.GetCuffRecordNumber();
		break;
	case equip_pendant:
		NumberFind = m_BPTLib.GetPendantRecordNumber();
		break;
	case equip_horse:
		NumberFind = m_BPTLib.GetHorseRecordNumber();
		break;
	default:
		break;
	}

for (int m=0;m<NumberFind;m++)
{
switch(j)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(m);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(m);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(m);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(m);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(m);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(m);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(m);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(m);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(m);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(m);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(m);
		break;
	default:
		break;
	}



if (pItem->m_SpecialParam.uItemType == 1 && pEqu->m_ItemGroup == pItem->m_SpecialParam.uItemGroup)
{
if (pItem->m_SpecialParam.uItemGroupCount < 11)
{

int CheckItemInGroup = 0;

for (int u=0;u<pItem->m_SpecialParam.uItemGroupCount;u++)
{
if (pItem->m_GroupItemNameInfo[u][1] == pEqu->m_nDetailType && pItem->m_GroupItemNameInfo[u][2] == pEqu->m_nParticularType)
{
CheckItemInGroup = 1;
break;
}
}

if (CheckItemInGroup == 0)
{
#ifndef _SERVER
strcpy(pItem->m_GroupItemName[pItem->m_SpecialParam.uItemGroupCount],pEqu->m_szName);
#endif
pItem->m_GroupItemNameInfo[pItem->m_SpecialParam.uItemGroupCount][0] = 0;
pItem->m_GroupItemNameInfo[pItem->m_SpecialParam.uItemGroupCount][1] = pEqu->m_nDetailType;
pItem->m_GroupItemNameInfo[pItem->m_SpecialParam.uItemGroupCount][2] = pEqu->m_nParticularType;
pItem->m_SpecialParam.uItemGroupCount++;
}


}
}

}

}

pItem->SetAttrib_MA(sMA_Gold);
}
else
{
pItem->m_SpecialParam.uItemType = 0;
KItemNormalAttrib	sMA[6];	
bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);
if (bEC)
pItem->SetAttrib_MA(sMA);
}

}


else if (pItem->m_SpecialParam.uItemType == 2)
{

KItemNormalAttrib	sMA[6];	
bEC = Gen_MagicAttribPink(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);

#ifndef _SERVER
pItem->m_ItemLevel = pEqu->m_ItemLevel;
#endif

if (bEC)
pItem->SetAttrib_MA(sMA);
}



else
{
KItemNormalAttrib	sMA[6];	
bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);
if (bEC)
pItem->SetAttrib_MA(sMA);
}


	return bEC;
}

BOOL KItemGenerator::Gen_Quest(IN int nDetailType, IN OUT KItem* pItem,IN int nVersion,IN const int* pnaryMALevel,IN int nSeriesReq)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);




pItem->m_SpecialParam.uItemType = 0;
pItem->m_SpecialParam.uItemGroup = 0;
pItem->m_SpecialParam.uItemGroupCount = 0;
pItem->m_nPriceShop = 0;
pItem->m_nPriceGold = 0;

pItem->m_nPriceTL = 0;
pItem->m_nPriceVD = 0;


pItem->m_nPoitAbrate = 0;

#ifndef _SERVER
for (int p=0;p<11;p++)
{
strcpy(pItem->m_GroupItemName[p],"");
}
pItem->m_ItemLevel = 0;
#endif
for (int e=0;e<11;e++)
{
for (int f=0;f<3;f++)
{
pItem->m_GroupItemNameInfo[e][f] = 0;
}
}

int nSeriesReqM = nSeriesReq;
int pnaryMALevelM[6];

ZeroMemory(pnaryMALevelM, sizeof(int) * 6);

if (pnaryMALevel)
{
memcpy(pnaryMALevelM, pnaryMALevel, sizeof(int) * 6);
}


	BOOL bEC = FALSE;
	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));


pItem->m_GeneratorParam.nVersion = nVersion;
	
	const KBASICPROP_QUEST* pQuest = NULL;
	pQuest = m_BPTLib.GetQuestRecord(nDetailType);

	if (NULL == pQuest)
	{
		_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pQuest;


if (pItem->GetLevel() == 2)
{


pItem->SetSeries(nSeriesReqM);

if (pnaryMALevelM)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevelM, sizeof(int) * 6);
	else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);

}


	return TRUE;
}

BOOL KItemGenerator::Gen_TownPortal(IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);


pItem->m_SpecialParam.uItemType = 0;
pItem->m_SpecialParam.uItemGroup = 0;
pItem->m_SpecialParam.uItemGroupCount = 0;
pItem->m_nPriceShop = 0;
pItem->m_nPriceGold = 0;

pItem->m_nPriceTL = 0;
pItem->m_nPriceVD = 0;

pItem->m_nPoitAbrate = 0;

#ifndef _SERVER
for (int p=0;p<11;p++)
{
strcpy(pItem->m_GroupItemName[p],"");
}
pItem->m_ItemLevel = 0;
#endif	
for (int e=0;e<11;e++)
{
for (int f=0;f<3;f++)
{
pItem->m_GroupItemNameInfo[e][f] = 0;
}
}
	BOOL bEC = FALSE;
	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));
	
	const KBASICPROP_TOWNPORTAL* pPortal = NULL;
	pPortal = m_BPTLib.GetTownPortalRecord(0);
	if (NULL == pPortal)
	{
		_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pPortal;
	return TRUE;
}
