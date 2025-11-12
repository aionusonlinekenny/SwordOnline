#ifndef	KItemListH
#define	KItemListH

#include "KLinkArray.h"
#include "KItem.h"
#define	MAX_ITEM_ACTIVE	2


//#define		EQUIPMENT_ROOM_WIDTH		6
//#define		EQUIPMENT_ROOM_HEIGHT		10
//#define		REPOSITORY_ROOM_WIDTH		6
//#define		REPOSITORY_ROOM_HEIGHT		10
//#define		TRADE_ROOM_WIDTH			6
//#define		TRADE_ROOM_HEIGHT			10


#define	REQUEST_EQUIP_ITEM		1
#define	REQUEST_EAT_MEDICINE	2

typedef struct
{
	int		nPlace;
	int		nX;
	int		nY;
} ItemPos;


class CORE_API KItemList
{
private:
	int			m_PlayerIdx;
	int			m_Hand;
	int			m_nBackHand;
	int			m_EquipItem[itempart_num];						// 玩家装备的道具（对应游戏世界中道具数组的索引）

	int			m_PGBoxItem[MAX_PGBOX_ITEM];



	PlayerItem	m_Items[MAX_PLAYER_ITEM];						// 玩家拥有的所有道具（包括装备着的和箱子里放的，对应游戏世界中道具数组的索引）
	PlayerItem	m_sBackItems[MAX_PLAYER_ITEM];					// 交易过程中 m_Items 的备份
	static int	ms_ActiveEquip[itempart_num][MAX_ITEM_ACTIVE];	// 某个部分的装备所激活的装备
	static int	ms_ActivedEquip[itempart_num][MAX_ITEM_ACTIVE];	// 某个部分的装备被什么装备激活
	KLinkArray	m_FreeIdx;
	KLinkArray	m_UseIdx;
	int			m_nListCurIdx;									// 用于 GetFirstItem 和 GetNextItem
	BOOL		m_nMaskLock; // MASK
#ifndef _SERVER
	BOOL		m_bLockOperation;
#endif
public:
	KInventory	m_Room[room_num];
	int         m_KickAllMacatric;

	BOOL		CheckHaveItem(int nGameIdx);

private:
	int			FindFree();
	int			FindSame(int nGameIdx);							// nGameIdx指游戏世界中道具数组的编号
	int			GetEquipPlace(int nType);						// 取得某类型装备应该放的位置
	BOOL		Fit(int nIdx, int nPlace);						// 检查是否可以装备上
	BOOL		Fit(KItem* pItem, int nPlace);
	int			GetEquipEnhance(int nPlace);					// 取得装备的激活属性数量
	int			GetActiveEquipPlace(int nPlace, int nCount);	// 取得nPlace的装备激活的第nCount个装备位置
	void		InfectionNextEquip(int nPlace, BOOL bEquip = FALSE);
	void        CheckDoGroupGoldItem();
	// 在room_equipment中查找指定Genre和DetailType的物品，得到ItemIdx和位置
	BOOL		FindSameDetailTypeInEquipment(int nGenre, int nDetail, int *pnIdx, int *pnX, int *pnY);
	friend	class KPlayer;

public:
	KItemList();
	~KItemList();
	int			Init(int nIdx);
	int			GetEquipEnhanceP(int nPlace);
	int			GetEquipment(int nIdx) { return m_EquipItem[nIdx]; }
	int			GetItemPGBox(int nIdx) { return m_PGBoxItem[nIdx]; }
	int			GetActiveAttribNum(int nIdx);					// nIdx指游戏世界中道具数组的编号
	int			GetWeaponType();								// 取得武器的类型，近程武器(equip_meleeweapon)还是远程武器(equip_rangeweapon)
	int			GetWeaponParticular();							// 取得武器的具体类型，用于计算不同的伤害加强
	void		GetWeaponDamage(int* nMin, int* nMax);			// 取得武器的伤害
	int			Add(int nIdx, int nPlace, int nX, int nY);		// nIdx指游戏世界中道具数组的编号
	BOOL		Remove(int nIdx,BOOL bcheck = FALSE);								// nIdx指游戏世界中道具数组的编号
	void		RemoveAll();
	BOOL		CanEquip(int nIdx, int nPlace = -1);			// nIdx指游戏世界中道具数组的编号
	BOOL		CanEquip(KItem* pItem, int nPlace = -1);
	BOOL		EnoughAttrib(void* pData);
	
	int			HaveDamageItem(int nDur);
	
	BOOL		Equip(int nIdx, int nPlace = -1);				// nIdx指游戏世界中道具数组的编号
	BOOL		UnEquip(int nIdx, int nPlace = -1);				// nIdx指游戏世界中道具数组的编号
	BOOL		EatMecidine(int nIdx);							// nIdx指游戏世界中道具数组的编号
	PlayerItem*	GetFirstItem();
	PlayerItem*	GetNextItem();
	int			SearchID(int nID);
	void		ExchangeMoney(int nSrcRoom, int DesRoom, int nMoney);
	void		ExchangeItem(ItemPos* SrcPos,ItemPos* DesPos);
	int			GetMoneyAmount();					// 得到物品栏和储物箱的总钱数
	int			GetEquipmentMoney();				// 得到物品栏和储物箱的钱数
	BOOL		AddMoney(int nRoom, int nMoney);
	BOOL		CostMoney(int nMoney);
	BOOL		DecMoney(int nMoney);
	void		SetMoney(int nMoney1, int nMoney2, int nMoney3);
	void		SetRoomMoney(int nRoom, int nMoney);
	int			GetMoney(int nRoom) { return m_Room[nRoom].GetMoney(); }	// 取得空间的钱
	void		SetPlayerIdx(int nIdx);				// 设定玩家索引
	int			Hand() { return m_Hand; };
	void		ClearRoom(int nRoom);
	void	    ClearAll();


	#ifdef _SERVER
	void Resend();
	int 	    FindNumberInAll(int nIdx);
	BOOL        CheckItemInAll(int nIdx);
	
	void		UpdateCurItem(int nGameIdx);
	#endif


	void		BackupTrade();
	void		RecoverTrade();
	void		StartTrade();
	// MASK
	BOOL		GetMaskLock() {return m_nMaskLock;};
	void		SetMaskLock(BOOL bFlag);	
	int			CountCommonItem(int nItemNature, int nItemGenre, int nDetailType = -1, int nLevel = -1, int nSeries = -1,  int P = pos_equiproom);
	// END
#ifdef	_SERVER
	void		Abrade(int nType);
	void		TradeMoveMoney(int nMoney);			// 调用此接口必须保证传入的nMoney是一个有效数(正数且不超过所有钱数)
	void		SendMoneySync();					// 服务器发money同步信息给客户端
	BOOL		IsTaskItemExist(int nDetailType);
	int			GetTaskItemNum(int nDetailType);
	int			GetTaskItemBind(int nDetailType);
	int			GetTaskItemNum2(int nDetailType);
	int			GetTaskItemNumBox(int nDetailType);

	int			GetAllItemInCheckBoxNum();
	int			GetItemInCheckBoxNum(int nGenre,int nDetailType,int nParticulType,int nLevel);
	int			CheckItemCountAll(int nGenre,int nDetailType,int nParticulType, int nLevel);
	int			CheckItemBlockInCheckBox(int nGenre,int nDetailType,int nParticulType,int nLevel);
	int			CheckTimeItemCount(int nTimeItem);
	int			CheckTimeItemCountAndGold(int nTimeItem);
	int			CheckTrangSucInBox();
	int			DeleteAllItem();
	int			DeleteAllItemInCheckBox();
	int			DelPhiPhong();
	int			CheckDelItemPoison(int nGenre,int nDetailType,int nParticulType,int nLevel);

	int			GetItemIdxInCheckBoxByNum(int nGenre,int nDetailType,int nParticulType,int nLevel,int nNum);
	int			SetAutoRepair();



	int			GetPlayerItemNum();
	int			GetPlayerItemBlueNum();
	int			GetPlayerItemIdxByNum(int nId);
	int			GetPlayerItemPostIdx(int nIdxItem, int nValua);


	int		    GetIdxTaskItemHaveFreeStask(int nDetailType);
	BOOL		RemoveTaskItem(int nDetailType);
	BOOL		RemoveTaskItem2(int nDetailType);
	void		SetBindAllItem();
	BOOL		RemoveTaskItemBox(int nDetailType);
	BOOL		SetItemTimeBox(int idxitem, int time);
	BOOL		SetHSD(int idxitem, int year, int month, int day, int hour);
	int		GetHSD(int idxitem, int inum);
	int 			GetDurationItem(int idxitem);
	BOOL		SetDurationItem(int idxitem);
	BOOL		SetItemTimeOpenBlock(int time);
	void		UpdateItem(int nIdx);
	
	void		GetTradeRoomItemInfo();				// 交易中把 trade room 中的 item 的 idx width height 信息写入 itemset 中的 m_psItemInfo 中去
	BOOL		TradeCheckCanPlace();				// 交易中判断买进的物品能不能完全放进自己的物品栏
	BOOL		CheckCanPlaceInEquipment(int nWidth, int nHeight, int *pnX, int *pnY);// 判断一定长宽的物品能否放进物品栏
	BOOL		EatMecidine(int nPlace, int nX, int nY);		// 吃什么地方的药
	// 自动把一个药品从room_equipment移动到room_immediacy
	BOOL		AutoMoveMedicine(int nItemIdx, int nSrcX, int nSrcY, int nDestX, int nDestY);
	// 丢失随身物品
	void		AutoLoseItemFromEquipmentRoom(int nRate);
	void		AutoDurationItem(int nRate);
	void		SetItemNotRepair();
	// 丢失一个穿在身上的装备(马不会掉)
	void		AutoLoseEquip();
	// Time Item
	void			CheckItemTime();
	//End
	BOOL		SearchPosition(int nWidth, int nHeight, ItemPos* pPos);


	BOOL		CheckItemEquipCS();
	int	CheckNguaPhienVu();
	BOOL		CheckItemEquipMateBoy();
	BOOL		CheckItemEquipMateGirl();
	BOOL		CheckItemEquipMateGuest();


#endif

#ifndef	_SERVER
	void        RemoveMagicDurability(int nIdx);
	int			UseItem(int nIdx);	
    BOOL		SearchEquipment(int nWidth, int nHeight);
    int			ChangeItemInPlayer(int nIdx);//edit by phong kieu mac trang bi vao nguoi					
	BOOL		SearchPosition(int nWidth, int nHeight, ItemPos* pPos);
	BOOL		AutoMoveItem(ItemPos SrcPos,ItemPos DesPos);
	//void		DropItemIdx(int nIdx);
	void		MenuSetMouseItem();
	void		RemoveAllInOneRoom(int nRoom);
	void		LockOperation();										// 锁定客户端对装备的操作
	void		UnlockOperation();
	BOOL		IsLockOperation() { return m_bLockOperation; };
	int			GetSameDetailItemNum(int nImmediatePos);
#endif
};
#endif
