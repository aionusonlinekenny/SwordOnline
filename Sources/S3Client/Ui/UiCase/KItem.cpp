#include "KCore.h"
#include "MyAssert.H"
#include "KTabFile.h"
#include "KNpc.h"
#include "KItem.h"
#include "KItemSet.h"
#include	<time.h>

#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#else
#include "S3DBInterface.h"
#endif

#ifndef _SERVER
#include "ImgRef.h"
#include "KPlayer.h"
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "KMagicDesc.h"
#endif

KItem	Item[MAX_ITEM];
int GetRandomNumber(int nMin, int nMax);

KItem::KItem()
{
	::memset(&m_CommonAttrib,    0, sizeof(m_CommonAttrib));
	::memset(m_aryBaseAttrib,    0, sizeof(m_aryBaseAttrib));
	::memset(m_aryRequireAttrib, 0, sizeof(m_aryRequireAttrib));
	::memset(m_aryMagicAttrib,   0, sizeof(m_aryMagicAttrib));
	::memset(&m_GeneratorParam,	 0, sizeof(m_GeneratorParam));
	::memset(&m_SpecialParam,	 0, sizeof(m_SpecialParam));
	m_nCurrentDur = -1;
#ifndef _SERVER
	::memset(&m_Image,   0, sizeof(KRUImage));
#endif
	m_bShow = FALSE;


	m_nPriceShop = 0;
	m_nPriceGold = 0;

	m_nPriceTL = 0;
    m_nPriceVD = 0;

	m_nPoitAbrate = 0;

#ifndef _SERVER
for (int i=0;i<11;i++)
{
strcpy(m_GroupItemName[i],"");
}
m_ItemLevel = 0;
m_Place = -1;
#endif

for (int e=0;e<11;e++)
{
for (int f=0;f<3;f++)
{
m_GroupItemNameInfo[e][f] = 0;
}
}

}

KItem::~KItem()
{
}

void* KItem::GetRequirement(IN int nReq)
{
	int i = sizeof(m_aryRequireAttrib)/sizeof(m_aryRequireAttrib[0]);
	if (nReq >= i)
		return NULL;

	return &m_aryRequireAttrib[nReq];
}
/******************************************************************************
功能:	将item上的魔法应用到NPC身上
入口：	pNPC: 指向NPC的指针，nMagicAcive：打开的隐藏属性数目
出口:	魔法被应用。
		具体工作由KNpc的成员函数完成。
		KItem 对象本身没有成员变量被修改
******************************************************************************/
void KItem::ApplyMagicAttribToNPC(IN KNpc* pNPC, IN int nMagicActive /* = 0 */) const
{
	_ASSERT(this != NULL);
	_ASSERT(nMagicActive >= 0);

	int nCount = nMagicActive;
	int i;

	// 基础属性调整NPC
	for (i = 0; i < sizeof(m_aryBaseAttrib)/sizeof(m_aryBaseAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryBaseAttrib[i]);
		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
		}
	}
	// 魔法属性调整NPC
	for (i = 0; i < sizeof(m_aryMagicAttrib)/sizeof(m_aryMagicAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryMagicAttrib[i]);

		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			if (i & 1)						// 为奇数，是后缀（i从零开始）
			{
				if (nCount > 0)
				{
					pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
					nCount--;
				}
			}
			else
			{
				pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
			}
		}
	}
}


/******************************************************************************
功能:	将item上的魔法从NPC身上移除
入口：	pNPC: 指向NPC的指针，nMagicAcive：打开的隐藏属性数目
出口:	魔法被应用。
		具体工作由KNpc的成员函数完成。
		KItem 对象本身没有成员变量被修改
******************************************************************************/
void KItem::RemoveMagicAttribFromNPC(IN KNpc* pNPC, IN int nMagicActive /* = 0 */) const
{
	_ASSERT(this != NULL);
	_ASSERT(nMagicActive >= 0);

	int nCount = nMagicActive;
	int	i;
	
	// 基础属性调整NPC
	for (i = 0; i < sizeof(m_aryBaseAttrib)/sizeof(m_aryBaseAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryBaseAttrib[i]);
		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			KItemNormalAttrib RemoveAttrib;
			RemoveAttrib.nAttribType = pAttrib->nAttribType;
			RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
			RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
			RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
			pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
		}
	}

	for (i = 0; i < sizeof(m_aryMagicAttrib)/sizeof(m_aryMagicAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryMagicAttrib[i]);

		if (INVALID_ATTRIB != pAttrib->nAttribType)		// TODO: 为 -1 定义一个常量?
		{
			KItemNormalAttrib RemoveAttrib;
			if (i & 1)						// 为奇数，是后缀（i从零开始）
			{
				if (nCount > 0)
				{
					RemoveAttrib.nAttribType = pAttrib->nAttribType;
					RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
					RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
					RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
					pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
					nCount--;
				}
			}
			else
			{
				RemoveAttrib.nAttribType = pAttrib->nAttribType;
				RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
				RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
				RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
				pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
			}
		}
	}
}

/******************************************************************************
功能:	将item上的第N项隐藏魔法属性应用到NPC身上
入口：	pNPC: 指向NPC的指针
出口:	魔法被应用。
		具体工作由KNpc的成员函数完成。
		KItem 对象本身没有成员变量被修改
******************************************************************************/

/******************************************************************************
功能:	将item上的第N项隐藏魔法属性从NPC身上移除
入口：	pNPC: 指向NPC的指针，nMagicActive：第n项魔法属性
出口:	魔法被移除。
		具体工作由KNpc的成员函数完成。
		KItem 对象本身没有成员变量被修改
******************************************************************************/
void KItem::ApplyHiddenMagicAttribToNPC(IN KNpc* pNPC, IN int nMagicActive) const
{
	_ASSERT(this != NULL);
	if (nMagicActive <= 0)
		return;

	const KItemNormalAttrib* pAttrib;
	pAttrib = &(m_aryMagicAttrib[(nMagicActive << 1) - 1]);	// 后缀为隐藏属性所以乘2减一
	if (-1 != pAttrib->nAttribType)
	{
		pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
	}
}
void KItem::RemoveHiddenMagicAttribFromNPC(IN KNpc* pNPC, IN int nMagicActive) const
{
	_ASSERT(this != NULL);
	if (nMagicActive <= 0)
		return;

	const KItemNormalAttrib* pAttrib;
	pAttrib = &(m_aryMagicAttrib[(nMagicActive << 1) - 1]);	// 后缀为隐藏属性所以乘2减一
	if (-1 != pAttrib->nAttribType)
	{
		KItemNormalAttrib RemoveAttrib;
		RemoveAttrib.nAttribType = pAttrib->nAttribType;
		RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
		RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
		RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
		pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
	}
}

/******************************************************************************
功能:	根据配置文件中的数据,为item的各项赋初值
入口：	pData: 给出来自配置文件的数据
出口:	成功时返回非零, 以下成员变量被值:
			m_CommonAttrib,m_aryBaseAttrib,m_aryRequireAttrib
		失败时返回零
说明:	CBR: Common,Base,Require
******************************************************************************/
BOOL KItem::SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT* pData)
{
	_ASSERT(pData != NULL);
	
	BOOL bEC = FALSE;
	if (pData)
	{
		//SetAttrib_Common(pData);
		*this = *pData;		// 运算符重载
		SetAttrib_Base(pData->m_aryPropBasic);
		SetAttrib_Req(pData->m_aryPropReq);
		m_SpecialParam.uItemType = pData->m_ItemType;
		m_SpecialParam.uItemGroup = pData->m_ItemGroup;
		m_SpecialParam.uItemGroupCount = 0;
		bEC = TRUE;
	}
	else
	{
	m_SpecialParam.uItemType = 0;
	m_SpecialParam.uItemGroup = 0;
	m_SpecialParam.uItemGroupCount = 0;
	}

#ifndef _SERVER
for (int i=0;i<11;i++)
{
strcpy(m_GroupItemName[i],"");
}
m_ItemLevel = 0;
#endif
for (int e=0;e<11;e++)
{
for (int f=0;f<3;f++)
{
m_GroupItemNameInfo[e][f] = 0;
}
}


	return bEC;
}

BOOL KItem::SetAttrib_Base(const KEQCP_BASIC* pBasic)
{
	for (int i = 0;
		 i < sizeof(m_aryBaseAttrib)/sizeof(m_aryBaseAttrib[0]); i++)
	{
		KItemNormalAttrib* pDst;
		const KEQCP_BASIC* pSrc;
		pDst = &(m_aryBaseAttrib[i]);
		pSrc = &(pBasic[i]);
		pDst->nAttribType = pSrc->nType;
		pDst->nValue[0] = ::GetRandomNumber(pSrc->sRange.nMin, pSrc->sRange.nMax);
		pDst->nValue[1] = 0;	// RESERVED
		pDst->nValue[2] = 0;	// RESERVED
		if (pDst->nAttribType == magic_durability_v)
			SetDurability(pDst->nValue[0]);
	}
	
	return TRUE;
}

BOOL KItem::SetAttrib_Req(const KEQCP_REQ* pReq)
{
	for (int i = 0;
		 i < sizeof(m_aryRequireAttrib)/sizeof(m_aryRequireAttrib[0]); i++)
	{
		KItemNormalAttrib* pDst;
		pDst = &(m_aryRequireAttrib[i]);
		pDst->nAttribType = pReq[i].nType;
		pDst->nValue[0] = pReq[i].nPara;
		pDst->nValue[1] = 0;	// RESERVED
		pDst->nValue[2] = 0;	// RESERVED
	}
	return TRUE;
}

/******************************************************************************
功能:	根据传入的数据, 为item的魔法属性赋初值
入口：	pMA: 给出数据
出口:	成功时返回非零, 以下成员变量被值:
			m_aryMagicAttrib
		失败时返回零
******************************************************************************/
BOOL KItem::SetAttrib_MA(IN const KItemNormalAttrib* pMA)
{
	if (NULL == pMA)
		{ _ASSERT(FALSE); return FALSE; }

	for (int i = 0; i < sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]); i++)
	{
		m_aryMagicAttrib[i] = pMA[i];
		// 永不磨损
		if (m_aryMagicAttrib[i].nAttribType == magic_indestructible_b)
		{
			SetDurability(-1);
		}
	}
	return TRUE;
}



/******************************************************************************
功能:	根据传入的数据, 为item的魔法属性赋初值
入口：	pMA: 给出数据
出口:	成功时返回非零, 以下成员变量被值:
			m_aryMagicAttrib
		失败时返回零
******************************************************************************/
BOOL KItem::SetAttrib_MA(IN const KMACP* pMA)
{
	if (NULL == pMA)
		{ _ASSERT(FALSE); return FALSE; }

	for (int i = 0; i < sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]); i++)
	{
		const KMACP* pSrc;
		KItemNormalAttrib* pDst;
		pSrc = &(pMA[i]);
		pDst = &(m_aryRequireAttrib[i]);

		pDst->nAttribType = pSrc->nPropKind;
		pDst->nValue[0] =  ::GetRandomNumber(pSrc->aryRange[0].nMin, pSrc->aryRange[0].nMax);
		pDst->nValue[1] =  ::GetRandomNumber(pSrc->aryRange[1].nMin, pSrc->aryRange[1].nMax);
		pDst->nValue[2] =  ::GetRandomNumber(pSrc->aryRange[2].nMin, pSrc->aryRange[2].nMax);
	}
	return TRUE;
}

void KItem::operator = (const KBASICPROP_EQUIPMENT& sData)
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	
	::strcpy(pCA->szItemName,  sData.m_szName);
	
	//Time Item
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	// End
	pCA->nFiFongArmor = 0;
	pCA->nBind = 0;

	m_nPriceShop = 0;

    m_nPriceGold = 0;

	m_nPriceTL = 0;
    m_nPriceVD = 0;

	m_nPoitAbrate = 0;
	m_nPoitAbrate = 0;


#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);

	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

void KItem::operator = (const KBASICPROP_MEDMATERIAL& sData)
{
	// 赋值: 共同属性部分
	KItemCommonAttrib* pCA = &(m_CommonAttrib);
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	::strcpy(pCA->szItemName,  sData.m_szName);
	
	//Time Item
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	// End
	pCA->nFiFongArmor = 0;
	pCA->nBind = 0;
	
	m_nPriceShop = 0;

    m_nPriceGold = 0;

	m_nPriceTL = 0;
    m_nPriceVD = 0;

	m_nPoitAbrate = 0;

#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);
#endif
	// 赋值: 基本属性部分
	KItemNormalAttrib* pBA = m_aryBaseAttrib;
	pBA[0].nAttribType = sData.m_nAttrib1_Type;
	pBA[0].nValue[0]   = sData.m_nAttrib1_Para;
	pBA[1].nAttribType = sData.m_nAttrib2_Type;
	pBA[1].nValue[0]   = sData.m_nAttrib2_Para;
	pBA[2].nAttribType = sData.m_nAttrib3_Type;
	pBA[2].nValue[0]   = sData.m_nAttrib3_Para;
	
	// 赋值: 需求属性部分: 无
	// 赋值: 魔法属性部分: 无
#ifndef _SERVER
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

void KItem::operator = (const KBASICPROP_MINE& sData)
{
	// 赋值: 共同属性部分
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	::strcpy(pCA->szItemName,  sData.m_szName);
	// Time Item
	pCA->LimitTime.bYear = 0;
		pCA->LimitTime.bMonth = 0;
		pCA->LimitTime.bDay = 0;
		pCA->LimitTime.bHour = 0;
	// End
	pCA->nFiFongArmor = 0;
	pCA->nBind = 0;
	
	m_nPriceShop = 0;

    m_nPriceGold = 0;

	m_nPriceTL = 0;
    m_nPriceVD = 0;

	m_nPoitAbrate = 0;


#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);
#endif
	// 赋值: 需求属性部分: 无
	// 赋值: 魔法属性部分: 无
#ifndef _SERVER
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

void KItem::operator = (const KBASICPROP_QUEST& sData)
{
	// 赋值: 共同属性部分
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = 0;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = -1;
	pCA->nIsThrow		 = sData.m_nIsThrowItem;
	
	::strcpy(pCA->szItemName,  sData.m_szName);
	// Time Item
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	//End
	pCA->nFiFongArmor = 0;
	pCA->nBind = 0;
	
	m_nPriceShop = 0;

    m_nPriceGold = 0;

	m_nPriceTL = 0;
    m_nPriceVD = 0;

	m_nPoitAbrate = 0;



#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);
#endif
	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	// 赋值: 需求属性部分: 无
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	// 赋值: 魔法属性部分: 无
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));
#ifndef _SERVER
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

void KItem::operator = (const KBASICPROP_TOWNPORTAL& sData)
{
	// 赋值: 共同属性部分
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = 0;
	pCA->nParticularType = 0;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = 0;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = 1;
	pCA->nSeries		 = -1;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//Time Item
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	// End
	pCA->nFiFongArmor = 0;
	pCA->nBind = 0;
	
	
#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);
#endif
	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	// 赋值: 需求属性部分: 无
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	// 赋值: 魔法属性部分: 无
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));
#ifndef _SERVER
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

void KItem::operator = (const KBASICPROP_MEDICINE& sData)
{
	// 赋值: 共同属性部分
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//Time Item
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	// End
	pCA->nFiFongArmor = 0;
	pCA->nBind = 0;

	m_nPriceShop = 0;

    m_nPriceGold = 0;

	m_nPriceTL = 0;
    m_nPriceVD = 0;

	m_nPoitAbrate = 0;

#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);
#endif
	// 赋值: 基本属性部分
	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	KItemNormalAttrib* pBA = m_aryBaseAttrib;
	pBA[0].nAttribType = sData.m_aryAttrib[0].nAttrib;
	pBA[0].nValue[0]   = sData.m_aryAttrib[0].nValue;
	pBA[0].nValue[1]   = sData.m_aryAttrib[0].nTime;
	pBA[1].nAttribType = sData.m_aryAttrib[1].nAttrib;
	pBA[1].nValue[0]   = sData.m_aryAttrib[1].nValue;
	pBA[1].nValue[1]   = sData.m_aryAttrib[1].nTime;
	
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));

#ifndef _SERVER
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif

}

void KItem::operator = (const KBASICPROP_EQUIPMENT_UNIQUE& sData)
{
	// 赋值: 共同属性部分
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//Time Item
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	// End	
	pCA->nFiFongArmor = 0;
	pCA->nBind = 0;
	

	m_nPriceShop = 0;

    m_nPriceGold = 0;

	m_nPriceTL = 0;
    m_nPriceVD = 0;

	m_nPoitAbrate = 0;

#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);

	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

//flying add this overloaded operator to generate a gold item.
void KItem::operator = (const KBASICPROP_EQUIPMENT_GOLD& sData)
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;	
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//Time Item
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	// End
	pCA->nFiFongArmor = 0;
	pCA->nBind = 0;
	

	m_nPriceShop = 0;

    m_nPriceGold = 0;


	m_nPriceTL = 0;
    m_nPriceVD = 0;

	m_nPoitAbrate = 0;

#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

BOOL KItem::Gen_Equipment_Unique(const KBASICPROP_EQUIPMENT* pEqu,
								 const KBASICPROP_EQUIPMENT_UNIQUE* pUni)
{
	_ASSERT(this != NULL);
	_ASSERT(pEqu != NULL);
	_ASSERT(pUni != NULL);

	if (NULL == pEqu || NULL == pUni)
		{ _ASSERT(FALSE); return FALSE; }

	// 赋值: 共同属性部分
	*this = *pUni;		// 运算符重载
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->bStack  = pEqu->m_bStack;
	pCA->nWidth  = pEqu->m_nWidth;
	pCA->nHeight = pEqu->m_nHeight;

	SetAttrib_Base(pEqu->m_aryPropBasic);		// 赋值: 基本属性部分
	SetAttrib_Req(pUni->m_aryPropReq);			// 赋值: 需求属性部分
	SetAttrib_MA(pUni->m_aryMagicAttribs);		// 赋值: 魔法属性部分

	return TRUE;
}

void KItem::Remove()
{
#ifndef _SERVER
m_Place = -1;
#endif
	m_bShow = FALSE;
}

void KItem::Add()
{
	m_bShow = TRUE;
}




int  KItem::CheckMagicItem(int nMagicId,int nMagicNumber1,int nMagicNumber2)
{

	if (!m_aryMagicAttrib)
		return 0;

	for (int i = 0; i < sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]); i++)
	{
		if (m_aryMagicAttrib[i].nAttribType == nMagicId && m_aryMagicAttrib[i].nValue[0] >= nMagicNumber1 && m_aryMagicAttrib[i].nValue[0] <= nMagicNumber2)
			return 1;
	}

	return 0;

}



BOOL KItem::SetBaseAttrib(IN const KItemNormalAttrib* pAttrib)
{
	if (!pAttrib)
		return FALSE;

	for (int i = 0; i < sizeof(m_aryBaseAttrib) / sizeof(m_aryBaseAttrib[0]); i++)
	{
		m_aryBaseAttrib[i] = pAttrib[i];
	}
	return TRUE;
}

BOOL KItem::SetRequireAttrib(IN const KItemNormalAttrib* pAttrib)
{
	if (!pAttrib)
		return FALSE;

	for (int i = 0; i < sizeof(m_aryRequireAttrib) / sizeof(m_aryRequireAttrib[0]); i++)
	{
		m_aryRequireAttrib[i] = pAttrib[i];
	}
	return TRUE;
}

BOOL KItem::SetMagicAttrib(IN const KItemNormalAttrib* pAttrib)
{
	return SetAttrib_MA(pAttrib);
}

//------------------------------------------------------------------
//	磨损，返回值表示剩余耐久度
//------------------------------------------------------------------
int KItem::Abrade(IN const int nRandRange)
{
	if (m_nCurrentDur == -1 || nRandRange == 0)	// 永不磨损
		return -1;

	if (g_Random(nRandRange) == 0)	// nRandRange分之一的概率
	{
		m_nCurrentDur--;
		if (m_nCurrentDur == 0)
		{
			return 0;
		}
	}
	return m_nCurrentDur;
}

#ifndef _SERVER
void KItem::Paint(int nX, int nY)
{
	m_Image.oPosition.nX = nX;
	m_Image.oPosition.nY = nY;
	m_Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	g_pRepresent->DrawPrimitives(1, &m_Image, RU_T_IMAGE, TRUE);
}

void KItem::GetDesc(char* pszMsg, bool bShowPrice, int nPriceScale, int nActiveAttrib, bool bActiveDes)
{
	char	szColor[item_number][32] = 
	{
		"",
		"<color=White>",
		"",
		"",
		"<color=Yellow>",
	};
	int nDetailType = GetDetailType();
	
	if (m_CommonAttrib.nItemGenre == item_equip && nDetailType != equip_mask)
	{
		if (m_nCurrentDur == 0 && nDetailType != equip_horse && nDetailType != equip_pendant && nDetailType != equip_ring && nDetailType != equip_amulet)
		{
			strcpy(szColor[item_equip], "<color=Red>");
		}
		else if (m_SpecialParam.uItemType == 1)	// 黄金装备
		{
			strcpy(szColor[item_equip], "<color=Yellow>");
		}
		else if (m_SpecialParam.uItemType == 2)	// 黄金装备
		{
			strcpy(szColor[item_equip], "<color=Blue>");
		}
		else if (IsItemMagic())	// 魔法装备
		{
			strcpy(szColor[item_equip], "<color=Blue>");
		}
		else
		{
			strcpy(szColor[item_equip], "<color=White>");
		}
	}
	if (m_CommonAttrib.nItemGenre == item_equip && m_nCurrentDur == 0 && nDetailType != equip_horse && nDetailType != equip_pendant && nDetailType != equip_ring && nDetailType != equip_amulet)
	{
		char item_name[120];
		sprintf(item_name," <Trang b� t�n h�i> %s",m_CommonAttrib.szItemName);
		strcpy(pszMsg, szColor[m_CommonAttrib.nItemGenre]);
		strcat(pszMsg, " ");
		strcat(pszMsg, item_name);
	}
	else
	{
		strcpy(pszMsg, szColor[m_CommonAttrib.nItemGenre]);
		strcat(pszMsg, " ");
		strcat(pszMsg, m_CommonAttrib.szItemName);
	}

	if (m_CommonAttrib.nItemGenre == item_equip && GetDetailType() != equip_mask)
	{
	char item_level[32];
	
    if (m_SpecialParam.uItemType == 1)
	{
	sprintf(item_level," [C�p %d]",m_ItemLevel);
	}
	else if (m_SpecialParam.uItemType == 2)
	{
	sprintf(item_level," [C�p %d]",(m_CommonAttrib.nLevel - 1)%10 + 1);
	}
	else
	{
    sprintf(item_level," [C�p %d]",m_CommonAttrib.nLevel);
	}
	strcat(pszMsg, item_level);
	}

	strcat(pszMsg, " ");

	strcat(pszMsg, "\n");





if (m_CommonAttrib.nItemGenre == item_equip && GetDetailType() != equip_mask)
{

if (m_GeneratorParam.nVersion == 2)
{
strcat(pszMsg," <color=Green>Trang b� n�y �� ���c kh�a b�o hi�m <color=White>");
strcat(pszMsg, "\n");
}
else if (m_GeneratorParam.nVersion == 550324)
{
strcat(pszMsg," <color=Green>Trang b� trong tr�ng th�i kh�a v�nh vi�n <color=White>");
strcat(pszMsg, "\n");
}
else if (m_GeneratorParam.nVersion > 2)
{


    int DayMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    int TimeNowMinute = m_GeneratorParam.nVersion - 3;

	int Month,Day,Hour,Minute;

	Month = 1;

	for (int t = 0; t < 12; t++)
	{
		if (TimeNowMinute >= DayMonth[t] * 24 * 60)
		{
		TimeNowMinute -= DayMonth[t] * 24 * 60;
		Month++;
		}
		else
		{
		break;
		}
	}

	Day = TimeNowMinute/(24 * 60);
	TimeNowMinute -= Day * (24 * 60);
	Hour = TimeNowMinute/60;
	TimeNowMinute -= Hour * 60;
	Minute = TimeNowMinute;
	
sprintf(pszMsg,"%s <color=Green>�ang m� kh�a: %d:%d %d-%d <color=White>",pszMsg,Hour,Minute,Day,Month);
strcat(pszMsg, "\n");
	


}

}
// nay test
if (m_CommonAttrib.nItemGenre == item_task)
{
	if (GetBindItem() == 1)
	{
		strcat(pszMsg," <color=Green>V�t ph�m �ang trong tr�ng th�i kh�a.<color=White>");
		strcat(pszMsg, "\n");
	}
}




	if (bShowPrice)
	{

		if (this->m_nPriceGold > 0)
		{



		char szPrice1[32];

		
		sprintf(szPrice1, "<color=Yellow>Gi� tr�: %d Xu", m_nPriceGold);


		strcat(pszMsg, " ");
		strcat(pszMsg, szPrice1);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");



		}





		else if (this->m_nPriceTL > 0)
		{



		char szPrice1[32];

		
		sprintf(szPrice1, "<color=Green>Gi� tr�: %d t�ch l�y", m_nPriceTL);


		strcat(pszMsg, " ");
		strcat(pszMsg, szPrice1);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");



		}


		else if (this->m_nPriceVD > 0)
		{



		char szPrice1[32];

		
		sprintf(szPrice1, "<color=Green>Gi� tr�: %d vinh d�", m_nPriceVD);


		strcat(pszMsg, " ");
		strcat(pszMsg, szPrice1);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");



		}







		else if (nPriceScale > 0)
		{
		int nvan1 = (m_CommonAttrib.nPrice / nPriceScale) /10000;
		int nluong1 = (m_CommonAttrib.nPrice / nPriceScale) %10000;

		char szPrice1[32];

		if (nvan1 > 0)
		sprintf(szPrice1, "Gi� tr�: %d v�n %d l��ng", nvan1,nluong1);
		else
		sprintf(szPrice1, "Gi� tr�: %d l��ng", nluong1);


		strcat(pszMsg, " ");
		strcat(pszMsg, szPrice1);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");

		}
	}


	if (m_nPriceShop > 0)
	{
        char szPrice2[32];
		if (m_nPriceShop < 1000000000)
		{
			int nvan2 = m_nPriceShop/10000;
			int nluong2 = m_nPriceShop%10000;

			if (nvan2 > 0)
				sprintf(szPrice2, "<color=Yellow>Gi� b�n �� xu�t: %d v�n %d l��ng", nvan2,nluong2);
			else
				sprintf(szPrice2, "<color=Yellow>Gi� b�n �� xu�t: %d l��ng",nluong2);
		}
		else
		{
			sprintf(szPrice2, "<color=Yellow>Gi� b�n �� xu�t: %d xu",m_nPriceShop-1000000000);
		}


		strcat(pszMsg, " ");
		strcat(pszMsg, szPrice2);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
	}


	if (m_CommonAttrib.nItemGenre == item_equip && GetDetailType() != equip_mask)
	{
		switch(m_CommonAttrib.nSeries)
		{
		case series_metal:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=White>Thu�c t�nh ng� h�nh: <color=Metal>Kim ");
			strcat(pszMsg, " ");
			break;
		case series_wood:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=White>Thu�c t�nh ng� h�nh: <color=Wood>M�c ");
			strcat(pszMsg, " ");
			break;
		case series_water:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=White>Thu�c t�nh ng� h�nh: <color=Water>Thu� ");
			strcat(pszMsg, " ");
			break;
		case series_fire:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=White>Thu�c t�nh ng� h�nh: <color=Fire>Ho� ");
			strcat(pszMsg, " ");
			break;
		case series_earth:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=White>Thu�c t�nh ng� h�nh: <color=Earth>Th� ");
			strcat(pszMsg, " ");
			break;
		}
	strcat(pszMsg, "\n");
	}
	strcat(pszMsg, "<color=White>");
	strcat(pszMsg, m_CommonAttrib.szIntro);
	strcat(pszMsg, "\n");







	if (m_CommonAttrib.nItemGenre == item_task && m_CommonAttrib.nLevel == 2)
	{
		switch(m_CommonAttrib.nSeries)
		{
		case series_metal:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=White>Thu�c t�nh ng� h�nh: <color=Metal>Kim ");
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
			break;
		case series_wood:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=White>Thu�c t�nh ng� h�nh: <color=Wood>M�c ");
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
			break;
		case series_water:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=White>Thu�c t�nh ng� h�nh: <color=Water>Thu� ");
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
			break;
		case series_fire:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=White>Thu�c t�nh ng� h�nh: <color=Fire>Ho� ");
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
			break;
		case series_earth:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=White>Thu�c t�nh ng� h�nh: <color=Earth>Th� ");
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
			break;
		}

		
		


		if (m_GeneratorParam.nGeneratorLevel[0])
		{

		char *pszMagicId;
		pszMagicId = (char *)g_MagicDesc.GetDesc2(m_GeneratorParam.nGeneratorLevel[0]);

		if (pszMagicId )
		{
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=HBlue>Thu�c t�nh: <color=Metal>");
			strcat(pszMsg, pszMagicId);
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
		}

		
		if (m_GeneratorParam.nGeneratorLevel[1] > 0 && m_GeneratorParam.nGeneratorLevel[1] <= 10)
		{	
	
			char pszMagicLevel[64];
			sprintf(pszMagicLevel, "<color=HBlue>C�p �� c��ng h�a:<color=Metal>%d",m_GeneratorParam.nGeneratorLevel[1]);
			strcat(pszMsg, " ");
			strcat(pszMsg, pszMagicLevel);
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
		}



		switch(m_GeneratorParam.nGeneratorLevel[2])
		{
		case 1:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=Green>Lo�i Trang b� c� th� kh�m n�m:V� Kh�");
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
			break;
		case 2:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=Green>Lo�i Trang b� c� th� kh�m n�m:Y Ph�c");
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
			break;
		case 3:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=Green>Lo�i Trang b� c� th� kh�m n�m:Gi�y");
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
			break;
		case 4:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=Green>Lo�i Trang b� c� th� kh�m n�m:�ai L�ng");
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
			break;
		case 5:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=Green>Lo�i Trang b� c� th� kh�m n�m:M�");
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
			break;

		case 6:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=Green>Lo�i Trang b� c� th� kh�m n�m:Bao Tay");
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
			break;
		case 7:
			strcat(pszMsg, " ");
			strcat(pszMsg, "<color=Green>Lo�i Trang b� c� th� kh�m n�m:T�t c� c�c Trang b�");
			strcat(pszMsg, " ");
			strcat(pszMsg, "\n");
			break;
		
		}










		}

		

	}




















	for (int i = 0; i < 7; i++)
	{
		if (!m_aryBaseAttrib[i].nAttribType)
		{
			continue;
		}
		if (m_aryBaseAttrib[i].nAttribType == magic_durability_v)
		{
			char	szDurInfo[120];
			if (m_nCurrentDur == -1)
				sprintf(szDurInfo, "<color=Yellow>Kh�ng th� b� ph� hu� <color=White>");
			else if (m_nCurrentDur == 0)
				sprintf(szDurInfo, "<color=Red>Trang b� �� h�ng kh�ng c� t�c d�ng\nMang ��n <color=Yellow>Th� R�n<color=Red> �� ph�c h�i <color=White>");
			else
			{
			//	if (GetDetailType() == equip_mask)
			//		sprintf(szDurInfo, "<color=Yellow>V�t ph�m ch� s� d�ng duy nh�t 1 l�n");
		//		else
					sprintf(szDurInfo, "�� b�n s� d�ng: %3d /%3d", GetDurability(), GetMaxDurability());
			}		
			strcat(pszMsg, " ");
			strcat(pszMsg, szDurInfo);
			strcat(pszMsg, " ");
		}
		else
		{
			char* pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryBaseAttrib[i]);
			if (!pszInfo || !pszInfo[0])
				continue;
			strcat(pszMsg, " ");
			strcat(pszMsg, pszInfo);
			strcat(pszMsg, " ");
		}
		strcat(pszMsg, "\n");
	}
	
	
	for (i = 0; i < 6; i++)
	{
		if (!m_aryRequireAttrib[i].nAttribType)
		{
			continue;
		}
		char* pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryRequireAttrib[i]);
		if (!pszInfo || !pszInfo[0])
			continue;
		if (Player[CLIENT_PLAYER_INDEX].m_ItemList.EnoughAttrib(&m_aryRequireAttrib[i]))
		{
			strcat(pszMsg, "<color=White>");
		}
		else
		{
			strcat(pszMsg, "<color=Red>");
		}
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, "\n");
	}







	for (i = 0; i < 6; i++)
	{
		if (!m_aryMagicAttrib[i].nAttribType)
		{
			continue;
		}

		char  pszInfoT[64]="";

		char* pszInfo;

		if (m_aryBaseAttrib[i].nAttribType == magic_indestructible_b)
		{
			
			if (m_nCurrentDur == -1)
				sprintf(pszInfo, "Kh�ng th� b� ph� hu� ");
		}
		else if (m_aryMagicAttrib[i].nAttribType == magic_addallskillslevel_v)
		{
		sprintf(pszInfoT,"K� n�ng v�n c�: +%d C�p",m_aryMagicAttrib[i].nValue[0]);
		pszInfo = pszInfoT;
		}

		else if (m_aryMagicAttrib[i].nAttribType == magic_itempinknomagic)
		{
		sprintf(pszInfoT,"<color=Yellow>Ch�a kh�m n�m");
		pszInfo = pszInfoT;
		}

		else if (m_aryMagicAttrib[i].nAttribType == magic_addskilllevel_v)
		{

		KSkill * pSkillAdd = (KSkill *) g_SkillManager.GetSkill(m_aryMagicAttrib[i].nValue[2],1);
		if (pSkillAdd)
		{
		sprintf(pszInfoT,"K� n�ng [ %s ]: +%d C�p",pSkillAdd->GetSkillName(),m_aryMagicAttrib[i].nValue[0]);
		pszInfo = pszInfoT;
		}

		}
		else
		{
		
			pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryMagicAttrib[i]);
			
		}

		if (!pszInfo || !pszInfo[0])
			continue;

		if (m_SpecialParam.uItemType == 1)
		{


	   if ((i & 1) == 0)
		{
			strcat(pszMsg, "<color=HYellow>");
		}
		else
		{
			if ((i>>1) < nActiveAttrib)
			{
				strcat(pszMsg, "<color=HYellow>");
			}
			else
			{
				strcat(pszMsg, "<color=DYellow>");
			}
		}



		}


		else 	if (m_SpecialParam.uItemType == 2)
		{


	   if ((i & 1) == 0)
		{
			strcat(pszMsg, "<color=HBlue>"); 
		//strcat(pszMsg, "<color=HPink>"); 
		}
		else
		{
			if ((i>>1) < nActiveAttrib)
			{
				strcat(pszMsg, "<color=HBlue>");
			//strcat(pszMsg, "<color=HPink>");
			}
			else
			{
				strcat(pszMsg, "<color=DBlue>");
				//strcat(pszMsg, "<color=DPink>");
			}
		}



		}


		else
		{

		if ((i & 1) == 0)
		{
			strcat(pszMsg, "<color=HBlue>");
		}
		else
		{
			if ((i>>1) < nActiveAttrib)
			{
				strcat(pszMsg, "<color=HBlue>");
			}
			else
			{
				strcat(pszMsg, "<color=DBlue>");
			}
		}

		}

strcat(pszMsg, " ");
		strcat(pszMsg, pszInfo);
strcat(pszMsg, " ");
		strcat(pszMsg, "\n");



	}


	
// Time Item	
if (m_CommonAttrib.LimitTime.bYear && CheckItemTime())
	{
		time_t rawtime;
		struct tm * timeinfo;
		
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		
		char sTmp[64];
		strcat(pszMsg, "\n");

		if (m_CommonAttrib.LimitTime.bDay < 10 && m_CommonAttrib.LimitTime.bMonth < 10)
		{
			sprintf(sTmp,"<color=Green>H�n S� D�ng: %d:00 0%d/0%d/%d",m_CommonAttrib.LimitTime.bHour,m_CommonAttrib.LimitTime.bDay,m_CommonAttrib.LimitTime.bMonth,m_CommonAttrib.LimitTime.bYear);
		}
		else if (m_CommonAttrib.LimitTime.bDay < 10)
		{
			sprintf(sTmp,"<color=Green>H�n S� D�ng: %d:00 0%d/%d/%d",m_CommonAttrib.LimitTime.bHour,m_CommonAttrib.LimitTime.bDay,m_CommonAttrib.LimitTime.bMonth,m_CommonAttrib.LimitTime.bYear);
		}
		else if (m_CommonAttrib.LimitTime.bMonth < 10)
		{
			sprintf(sTmp,"<color=Green>H�n S� D�ng: %d:00 %d/0%d/%d",m_CommonAttrib.LimitTime.bHour,m_CommonAttrib.LimitTime.bDay,m_CommonAttrib.LimitTime.bMonth,m_CommonAttrib.LimitTime.bYear);
		
		}
		else
		{
		sprintf(sTmp,"<color=Green>H�n S� D�ng: %d:00 %d/%d/%d",m_CommonAttrib.LimitTime.bHour,m_CommonAttrib.LimitTime.bDay,m_CommonAttrib.LimitTime.bMonth,m_CommonAttrib.LimitTime.bYear);
		}
		
		
		strcat(pszMsg,sTmp);
		strcat(pszMsg, "\n");
	}
	else if (!CheckItemTime())
	{
		time_t rawtime;
		struct tm * timeinfo;
		
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		
		strcat(pszMsg, "\n");
		strcat(pszMsg, "<color=red>V�t ph�m �� h�t h�n s� d�ng");
		strcat(pszMsg, "\n");
	}
// End

if (m_CommonAttrib.nItemGenre == item_equip && GetDetailType() != equip_mask)
{
if (m_SpecialParam.uItemType == 1)
{
for (int i=0; i<m_SpecialParam.uItemGroupCount;i++)
{
strcat(pszMsg, "\n");
break;
}
}
}






if (m_CommonAttrib.nItemGenre == item_equip && GetDetailType() != equip_mask)
{
if (m_SpecialParam.uItemType == 1)
{
for (int i=0; i<m_SpecialParam.uItemGroupCount;i++)
{
if (m_GroupItemNameInfo[i][0] == 1)
{
strcat(pszMsg, "<color=HGreen>");
strcat(pszMsg," ");
strcat(pszMsg,m_GroupItemName[i]);
strcat(pszMsg," ");
strcat(pszMsg, "\n");
}
else
{
strcat(pszMsg, "<color=DGreen>");
strcat(pszMsg," ");
strcat(pszMsg,m_GroupItemName[i]);
strcat(pszMsg," ");
strcat(pszMsg, "\n");
}
}
}
}

strcat(pszMsg, "\n");
 
 if (m_CommonAttrib.nItemGenre == item_equip && m_aryMagicAttrib[0].nAttribType && bActiveDes )
    {
        strcat(pszMsg, "<color=Yellow>");
	char szReturn[32];
	char Guide[100];
	if (m_Place == itempart_weapon || m_Place == itempart_head)
	{
	ReturnSupportSeries(m_CommonAttrib.nSeries,szReturn);
	sprintf(Guide,"\nC�n h� %s c�a D�y chuy�n v� Trang ph�c �� k�ch ho�t thu�c t�nh �m", szReturn);
	strcat(pszMsg, Guide);
	}
	else if(m_Place == itempart_foot || m_Place == itempart_ring1)
	{
	ReturnSupportSeries(m_CommonAttrib.nSeries,szReturn);
	sprintf(Guide,"\nC�n h� %s c�a V� kh� v� N�n �� k�ch ho�t thu�c t�nh �m", szReturn);
	strcat(pszMsg, Guide);
	}
	else if(m_Place == itempart_pendant || m_Place == itempart_cuff)
	{
	ReturnSupportSeries(m_CommonAttrib.nSeries,szReturn);
	sprintf(Guide,"\nC�n h� %s c�a Gi�y v� Nh�n tr�n �� k�ch ho�t thu�c t�nh �m", szReturn);
	strcat(pszMsg, Guide);
	}
	else if(m_Place == itempart_belt || m_Place == itempart_ring2)
	{
	ReturnSupportSeries(m_CommonAttrib.nSeries,szReturn);
	sprintf(Guide,"\nC�n h� %s c�a Ng�c b�i v� Bao tay �� k�ch ho�t thu�c t�nh �m", szReturn);
	strcat(pszMsg, Guide);
	}
	else if(m_Place == itempart_body || m_Place == itempart_amulet)
	{
	ReturnSupportSeries(m_CommonAttrib.nSeries,szReturn);
	sprintf(Guide,"\nC�n h� %s c�a Th�t l�ng v� Nh�n d��i �� k�ch ho�t thu�c t�nh �m", szReturn);
	strcat(pszMsg, Guide);
	}
    }

}
// Tra lai ngu hanh tuong sinh
void KItem::ReturnSupportSeries(int nSeries, LPSTR lpRString)
{
	ZeroMemory(lpRString,sizeof(lpRString));
	if(nSeries == 0)
	strcpy(lpRString,"<color=Earth>Th� <color=Yellow>");
	else if (nSeries == 1)
	strcpy(lpRString,"<color=Water>Th�y<color=Yellow>");
	else if (nSeries == 2)
	strcpy(lpRString,"<color=Metal>Kim<color=Yellow>");
	else if (nSeries == 3)
	strcpy(lpRString,"<color=Wood>M�c<color=Yellow>");
	else if (nSeries == 4)
	strcpy(lpRString,"<color=Fire>H�a<color=Yellow>");
}
#endif

int KItem::GetMaxDurability()
{
	for (int i = 0; i < 7; i++)
	{
		if (m_aryBaseAttrib[i].nAttribType == magic_durability_v)
		{
			return m_aryBaseAttrib[i].nValue[0];
		}
	}
	return -1;
}

int KItem::GetTotalMagicLevel()
{
	int nRet = 0;
	for (int i = 0; i < 6; i++)
	{
		if (m_SpecialParam.uItemType == 2)
		{
		nRet += (m_GeneratorParam.nGeneratorLevel[i]%100)/10 + 1;
		}
		else
		{
		nRet += m_GeneratorParam.nGeneratorLevel[i];
		}
	}
	return nRet;


}

int KItem::GetRepairPrice()
{
	if (ItemSet.m_sRepairParam.nMagicScale == 0)
		return 0;

	if (GetGenre() != item_equip)
		return 0;

	if (m_nCurrentDur == -1)
		return 0;

	int nMaxDur = GetMaxDurability();
	int nSumMagic = GetTotalMagicLevel();

	if (nMaxDur <= 0)
		return 0;

	int nHsXPrice = 1;
	if (m_SpecialParam.uItemType == 2)
	nHsXPrice = 5;

	return nHsXPrice * m_CommonAttrib.nPrice * ItemSet.m_sRepairParam.nPriceScale / 100 * (nMaxDur - m_nCurrentDur) / nMaxDur * (ItemSet.m_sRepairParam.nMagicScale + nSumMagic) / ItemSet.m_sRepairParam.nMagicScale;
}

BOOL KItem::CanBeRepaired()
{
	if (GetGenre() != item_equip)
		return FALSE;

	if (m_nCurrentDur == -1 || m_nCurrentDur == 0)
		return FALSE;

	int nMaxDur = GetMaxDurability();
	if (m_nCurrentDur == nMaxDur)
		return FALSE;

	return TRUE;
}

// Time Item
void KItem::SetTime( int bYear,BYTE bMonth,BYTE bDay,BYTE bHour )
{
	if (bYear)
	{
		if (bHour > 24)
		{
			bDay++;
			bHour -= 24;
		}
		if (bMonth == 2 && bDay > 28)
		{
			bMonth++;
			bDay = 1;
		}
		if ((bMonth == 1 || bMonth == 3 || bMonth == 5 || bMonth == 7 || bMonth == 8 || bMonth == 10 || bMonth == 12) && (bDay > 31)) 
		{
			bMonth++;
			bDay = 1;
		}
		if ((bMonth == 4 || bMonth == 6 || bMonth == 9 || bMonth == 11) && (bDay > 30))
		{
			bMonth++;
			bDay = 1;
		}
		if (bMonth > 12)
		{
			bYear++;
			bMonth -= 12;
		}
		m_CommonAttrib.LimitTime.bYear = bYear;
		m_CommonAttrib.LimitTime.bMonth = bMonth;
		m_CommonAttrib.LimitTime.bDay = bDay;
		m_CommonAttrib.LimitTime.bHour = bHour;
	}
}
int KItem::GetTimeItem(int iNum)
{
	if (iNum == 0)
		{return m_CommonAttrib.LimitTime.bYear;}
	else if (iNum == 1)
		{return m_CommonAttrib.LimitTime.bMonth;}
	else if (iNum == 2)
		{return m_CommonAttrib.LimitTime.bDay;}
	else if (iNum == 3)
		{return m_CommonAttrib.LimitTime.bHour;}
return 0;
}


BOOL KItem::CheckItemTime()
{
	BOOL bRet = TRUE;
	time_t rawtime;
	struct tm * timeinfo;
	
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	
	if (GetTime()->bYear)
	{
		if (GetTime()->bYear - 1900 < timeinfo->tm_year)
		{
			bRet = FALSE;
		}
		else if (GetTime()->bYear - 1900 == timeinfo->tm_year)
		{
			if (GetTime()->bMonth < timeinfo->tm_mon + 1)
			{
				bRet = FALSE;
			}
			else if (GetTime()->bMonth == timeinfo->tm_mon + 1)
			{
				if (GetTime()->bDay < timeinfo->tm_mday)
				{
					bRet = FALSE;
				}
				else if (GetTime()->bDay == timeinfo->tm_mday)
				{
					if (GetTime()->bHour <= timeinfo->tm_hour)
					{
						bRet = FALSE;
					}
				}
			}
		}
	}
	return bRet;
}
//End