#ifndef KItemSetH
#define	KItemSetH

#include "KLinkArray.h"
#include "KItem.h"

#define		IN
#define		OUT
class	KItem;

enum enumAbrade
{
	enumAbradeAttack = 0,
	enumAbradeDefend,
	enumAbradeMove,
	enumAbradeNum,
};
// 玩家之间交易进行时，用于判断玩家物品栏能否放下买进的物品
typedef struct
{
	int		m_nIdx;
	int		m_nX;
	int		m_nY;
	int		m_nWidth;
	int		m_nHeight;
} TRADE_ITEM_INFO;

typedef struct
{
	int		nPriceScale;
	int		nMagicScale;
} REPAIR_ITEM_PARAM;

class KItemSet
{
private:
	DWORD			m_dwIDCreator;		//	ID生成器，用于客户端与服务器端的交流
    int             m_nNextAddItem;
	int             m_nRemoteItem[MAX_ITEM];




public:
	int				m_nItemAbradeRate[enumAbradeNum][itempart_num];
	REPAIR_ITEM_PARAM	m_sRepairParam;


#ifdef _SERVER
	int             m_checkdel;
#endif


#ifdef _SERVER
	// 玩家之间交易进行时，用于判断玩家物品栏能否放下买进的物品
	TRADE_ITEM_INFO		*m_psItemInfo;
	// 玩家之间交易进行时，用于判断玩家物品栏能否放下买进的物品
	TRADE_ITEM_INFO		*m_psBackItemInfo;
	// copy m_psItemInfo to m_psBackItemInfo
	void			BackItemInfo();
	// 用于玩家被PK死亡后的惩罚计算，掉落随身物品
	PlayerItem		m_sLoseItemFromEquipmentRoom[EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT];
	// 用于玩家被PK死亡后的惩罚计算，掉落穿在身上的装备
	PlayerItem		m_sLoseEquipItem[itempart_num];
#endif

public:
	KItemSet();
	~KItemSet();
	void			Init();
	int				GetItemCount(IN int nItemGenre = -1);
	int				SearchID(IN DWORD dwID);
	int				Add(KItem* pItem);
	BOOL            Get_SizeItem(IN int nItemGenre,
								  IN int nDetailType,
								   IN int nParticularType,
								   IN int nLevel,
								   IN OUT int* nWidth,
								   IN OUT int* nHeight
								   );

	int				Add(IN int nItemGenre, IN int nSeries, IN int nLevel, IN int nLuck, IN int nDetail = -1, IN int nParticular = -1, IN int* pnMagicLevel = NULL, IN int nVersion = 0, IN UINT nRandomSeed = 0, IN int nYear = 0, IN int nMonth = 0, IN int nDay = 0, IN int nHour = 0, IN int iNgoaiTrang = 0);
	int				AddExit(IN int nItemGenre, IN int nSeries, IN int nLevel, IN int nLuck, IN int nDetail = -1, IN int nParticular = -1, IN int* pnMagicLevel = NULL, IN int nVersion = 0, IN UINT nRandomSeed = 0, IN int nYear = 0, IN int nMonth = 0, IN int nDay = 0, IN int nHour = 0);
	void			Remove(IN int nIdx,IN BOOL bcheck = FALSE);
	int				GetAbradeRange(IN int nType, IN int nPart);
private:
	void			SetID(IN int nIdx);
	int				FindFree();
};

extern KItemSet	ItemSet;
#endif
