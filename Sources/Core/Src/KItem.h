//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItem.h
// Date:	2002.08
// Code:	LiXueWu, DongBo
// Desc:	Header File
//---------------------------------------------------------------------------

#ifndef	KItemH
#define	KItemH

#include	"KBasPropTbl.h"
#include	"KMagicAttrib.h"
#include	"GameDataDef.h"
#ifndef _SERVER
#include	 "../../Represent/iRepresent/KRepresentUnit.h"
#endif
#include	"KTabFile.h"
//#include	"KPlayer.h"
class KPlayer;
class KIniFile;

#define		IN
#define		OUT

#define		MAX_ITEM_STACK	50


#ifdef _SERVER


#define		MAX_ITEM	200000
// #define		MAX_ITEM	70000


#else
#define		MAX_ITEM	512
#endif
#define		KItemNormalAttrib KMagicAttrib
/*
//装备属性作用于Player的何种属性
#define	ID_LIFE		0
#define	ID_MAXLIFE	1
#define	ID_LIEFREPLENISH		2

#define	ID_MANA		3
#define	ID_MAXMANA	4
#define	ID_MANAREPLENISH		5
*/

enum ITEMGENRE
{
	item_equip = 0,			// 装备
	item_medicine,			// 药品
	item_mine,				// 矿石
	item_materials,			// 药材
	item_task,				// 任务
	item_townportal,		// 传送门
	item_number,			// 类型数目
};

enum EQUIPLEVEL
{
	equip_normal = 0,		// 普通装备
	equip_magic,			// 魔法装备（1 ~ 2个魔法前后缀）
	equip_rare,				// 稀有装备（3 ~ 6个魔法前后缀）
	equip_unique,			// 唯一装备
	equip_set,				// 套装
	equip_number,			// 装备等级数目
	// flying add this item
	equip_gold,				//黄金装备
};

enum EQUIPDETAILTYPE
{
	equip_meleeweapon = 0,
	equip_rangeweapon,
	equip_armor,
	equip_ring,
	equip_amulet,
	equip_boots,
	equip_belt,
	equip_helm,
	equip_cuff,
	equip_pendant,
	equip_horse,
	equip_mask, // Mak
	equip_fifong,
	equip_detailnum,
};

enum MEDICINEDETAILTYPE
{
	medicine_blood = 0,
	medicine_mana,
	medicine_both,
	medicine_stamina,
	medicine_antipoison,
	medicine_detailnum,
};

typedef struct
{
	int		nItemGenre;				// 道具种类 (武器? 药品? 矿石?)
	int		nDetailType;			// 在该种类中的具体类别
	int		nParticularType;		// 详细类别
	int		nObjIdx;				// 地图上摆放时对应的物件数据编号
	BOOL	bStack;					// 是否可叠放
	int		nWidth;					// 道具栏中所占宽度
	int		nHeight;				// 道具栏中所占高度
	int		nPrice;					// 价格
	int		nLevel;					// 等级
	int		nSeries;				// 五行属性
	int		nBind;				// 五行属性

	char	szItemName[80];			// 名称
#ifndef _SERVER
	char	szImageName[80];		// 界面中的动画文件名
	char	szIntro[256];			// 说明文字
	
#endif
	// Time Item
	KTime	LimitTime;
	// End
	int		nNpcImage;				// NPC图象影响
	int		nIsThrow;
	int		nFiFongArmor;
	int		iShopCost;				
	int		iNgoaiTrang;			
public:

} KItemCommonAttrib;

typedef struct
{
	UINT	uRandomSeed;
	int		nGeneratorLevel[6];
	int		nVersion;
	int		nLuck;
} KItemGeneratorParam;


typedef struct
{
    int         uItemType;
	int         uItemGroup;
	int         uItemGroupCount;
} KItemSpecialParam;




typedef struct
{
int m_ItemIdx;
KItemGeneratorParam m_GeneratorParam;
KItemCommonAttrib m_CommonAttrib;
} KItemCheck;


class KNpc;

class KItem
{
public:
	KItem();
	~KItem();

// 以下是核心成员变量
public:
	KItemCommonAttrib	m_CommonAttrib;			// 各种道具的共同属性
	KItemNormalAttrib	m_aryBaseAttrib[7];		// 道具的基础属性
	KItemNormalAttrib	m_aryRequireAttrib[6];	// 道具的需求属性
    KItemNormalAttrib	m_aryMagicAttrib[6];	// 道具的魔法属性

// 以下是辅助性的成员变量
public:
	KItemGeneratorParam	m_GeneratorParam;		// 道具的生成参数
    KItemSpecialParam m_SpecialParam;

#ifndef _SERVER
	char        m_GroupItemName[11][64];
	int         m_ItemLevel;
	int			m_Place;//noi tren nguoi
#endif

int         m_GroupItemNameInfo[11][3];

int         m_nPriceShop;
int         m_nPriceGold;


int         m_nPriceTL;
int         m_nPriceVD;


int 	    m_nPoitAbrate;



private:
	BOOL	m_bShow;							// 自身在道具数组中的索引
	DWORD	m_dwID;								// 独立的ID，用于客户端与服务器端的交流
	int		m_nCurrentDur;						// 当前耐久度
#ifndef _SERVER
	KRUImage	m_Image;
#endif
// 以下是对外接口
public:
	void	ApplyMagicAttribToNPC(IN KNpc*, IN int = 0) const;
	void	RemoveMagicAttribFromNPC(IN KNpc*, IN int = 0) const;
	void	ApplyHiddenMagicAttribToNPC(IN KNpc*, IN int) const;
	void	RemoveHiddenMagicAttribFromNPC(IN KNpc*, IN int) const;
	KItemGeneratorParam * GetItemParam(){return &m_GeneratorParam;};
	void	SetID(DWORD dwID) { m_dwID = dwID; };
	DWORD	GetID() const { return m_dwID; };
	void 	SetShow(BOOL bShow) {m_bShow = bShow; };
	BOOL 	GetShow() const { return m_bShow; };
	int		GetDetailType() const { return m_CommonAttrib.nDetailType; };
	void		SetGenre(int nItemGenre) { m_CommonAttrib.nItemGenre = nItemGenre; };
	void		SetParticular(int nParticularType) { m_CommonAttrib.nParticularType = nParticularType; };
	void		SetDetailType(int nDetailType) { m_CommonAttrib.nDetailType = nDetailType; };
	void		SetLevel(int nLevel) { m_CommonAttrib.nLevel = nLevel; };
	int		GetGenre() const { return m_CommonAttrib.nItemGenre; };
	int		GetParticular() { return m_CommonAttrib.nParticularType; };
	int		GetSeries() const { return m_CommonAttrib.nSeries; };
	int		GetBindItem() const { return m_CommonAttrib.nBind; };
	void		SetBindItem(int nBind) { m_CommonAttrib.nBind = nBind; };
	int		GetShopCost() const { return m_CommonAttrib.iShopCost; };
	void		SetShopCost(int iShopCost) { m_CommonAttrib.iShopCost = iShopCost; };
	int		GetNgoaiTrang() const { return m_CommonAttrib.iNgoaiTrang; };
	void		SetNgoaiTrang(int iNgoaiTrang) { m_CommonAttrib.iNgoaiTrang = iNgoaiTrang; };
	
	
	
	void	SetSeries(int nSeries) { m_CommonAttrib.nSeries = nSeries; };
	int		GetLevel() { return m_CommonAttrib.nLevel; };
	int		GetWidth() const { return m_CommonAttrib.nWidth; };
	int		GetHeight() const { return m_CommonAttrib.nHeight; };
	int		GetPrice() const { return m_CommonAttrib.nPrice; };
	int		GetIsThrow() const { return m_CommonAttrib.nIsThrow; };
	char*	GetName() const { return (char *)m_CommonAttrib.szItemName; };
	int		GetObjIdx() { return m_CommonAttrib.nObjIdx;};
	void*	GetRequirement(IN int);
	int		GetMaxDurability();
	int		GetTotalMagicLevel();
	int		GetRepairPrice();
	int		GetVersion() {return m_GeneratorParam.nVersion;};
	void	SetVersion(int nVersion) { m_GeneratorParam.nVersion = nVersion; };
	BOOL	IsStack() const { return m_CommonAttrib.bStack; };
	void	Remove();
	void	Add();
	

	int		GetFiFongArmor(){ return m_CommonAttrib.nFiFongArmor; };
	void	SetFiFongArmor(int nID){m_CommonAttrib.nFiFongArmor = nID;};	
	
	BOOL	SetBaseAttrib(IN const KItemNormalAttrib*);
	BOOL	IsItemMagic()
	{
	if (m_aryMagicAttrib[0].nAttribType)
	return true;

	return false;
	}

	int     CheckMagicItem(int nMagicId,int nMagicNumber1,int nMagicNumber2);
	
	BOOL	SetRequireAttrib(IN const KItemNormalAttrib*);
	BOOL	SetMagicAttrib(IN const KItemNormalAttrib*);
	void	SetDurability(IN const int nDur) { m_nCurrentDur = nDur; };
	int		GetDurability() { return m_nCurrentDur; };
	KItemGeneratorParam*	GetGeneratorParam() { return &m_GeneratorParam; }
	int		Abrade(IN const int nRange);
	BOOL	CanBeRepaired();
// Time Item
	void	SetTime(int bYear,BYTE bMonth,BYTE bDay,BYTE bHour);
	int GetTimeItem(int iNum);
	KTime*	GetTime() { return & m_CommonAttrib.LimitTime; };	
	BOOL	CheckItemTime();
// End	

	int GetBaseMagic()  const { return m_aryBaseAttrib[0].nValue[0]; }; // MASK
	int		GetKind() { return m_SpecialParam.uItemType; };
	int		GetItemGroup() { return m_SpecialParam.uItemGroup; };
	int		GetBaseMagicAttribType(int i = 0)  const { return m_aryBaseAttrib[i].nAttribType; };
#ifndef _SERVER
	void	Paint(int nX, int nY);
	void	GetDesc(char* pszMsg, bool bShowPrice = false, int nPriceScale = 1, int nActiveAttrib = 0, bool bActiveDes = false);
	void	ReturnSupportSeries(int nSeries, LPSTR lpRString);
#endif

// 以下是辅助接口
friend class	KItemGenerator;
friend class	KPlayer;
friend class	KItemList;
private:
	BOOL SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT*);
	BOOL SetAttrib_MA(IN const KItemNormalAttrib*);
	BOOL SetAttrib_MA(IN const KMACP*);
	void operator = (const KBASICPROP_EQUIPMENT&);
	void operator = (const KBASICPROP_MEDMATERIAL&);
	void operator = (const KBASICPROP_MINE&);
	void operator = (const KBASICPROP_MEDICINE&);
	void operator = (const KBASICPROP_QUEST&);
	void operator = (const KBASICPROP_TOWNPORTAL&);
	void operator = (const KBASICPROP_EQUIPMENT_UNIQUE&);
	// flying add this overload operator.
	void operator = (const KBASICPROP_EQUIPMENT_GOLD&);
	BOOL Gen_Equipment_Unique(const KBASICPROP_EQUIPMENT*, const KBASICPROP_EQUIPMENT_UNIQUE*);

// 以下是辅助函数
private:
	BOOL SetAttrib_Base(const KEQCP_BASIC*);
	BOOL SetAttrib_Req(const KEQCP_REQ*);
};

extern KItem Item[MAX_ITEM];

#endif
