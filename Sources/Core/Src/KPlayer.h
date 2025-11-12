#ifndef KPlayerH
#define	KPlayerH
//#include <objbase.h>
#include "KWorldMsg.h"
#include "KMessage.h"
#include "KInventory.h"
#include "KPlayerFaction.h"
#include "KPlayerChat.h"
#include "KPlayerTask.h"
#include "KPlayerMenuState.h"
#include "KPlayerTrade.h"
#include "KPlayerTeam.h"
#include "KPlayerPK.h"
#include "KPlayerTong.h"
#include "KItemList.h"
#include "KNpc.h"
#include "KSkills.h"

#include "KPlayerDef.h"



#define		MAX_ANSWERNUM					50
#define		PLAYER_LIFE_REPLENISH			0
#define		PLAYER_MANA_REPLENISH			0
#define		PLAYER_STAMINA_GAIN				0
#define		PLAYER_STAMINA_LOSS				4

#define		STRENGTH_SET_DAMAGE_VALUE		5
#define		DEXTERITY_SET_DAMAGE_VALUE		5

#define		MAX_AVENGE_NUM					4
//-------------auto----------------------------------
#define		MAX_ARRAY_NPC_AUTO				50
#define		AUTO_TIME_LAG					50
#define		AUTO_COUNT_LAG					50
#define		AUTO_TIME_RESET_LAG				5000  /// thoi gian dung danh luc lag
#define		MAX_ARRAY_STATESKILL			3
#define		MAX_AUTO_SIZE				50
//--------------------------------------------------
#ifndef _SERVER
#define		MAX_AUTO_MANA_WAIL			 2 * 18
#define		MAX_AUTO_LIFE_WAIL			 2 * 18
#define		MAX_AUTO_OBJ_WAIL			 5 * 18
#define		MAX_AUTO_SKILL_BUFFE            3
#endif

enum	UIInfo //脚本通知显示的界面类型
{
	UI_SELECTDIALOG,
	UI_TRADEDIALOG,
	UI_TALKDIALOG,
	UI_NOTEINFO,
	UI_MSGINFO,//自右向左冒出来的信息
	UI_NEWSINFO,//新闻
	UI_NEWSINFO_1,
	UI_PLAYMUSIC,
	UI_OPENTONGUI,
};

enum
{
	Step_Training,
	Step_Repair,
	Step_InventoryMoney,
	Step_InventoryItem,
	Step_Return,
	Step_Completed,
};

// 重生点位置信息
typedef struct PLAYER_REVIVAL_POS_DATA
{
	int				m_nSubWorldID;		// 重生点地图
	int				m_ReviveID;			// 重生点索引
	int				m_nMpsX;			// 重生点地图位置 x
	int				m_nMpsY;			// 重生点地图位置 y
} PLAYER_REVIVAL_POS;

typedef struct
{
	int				m_nSubWorldId;		// 传送门世界ID
	int				m_nTime;			// 传送门保持时间
	int				m_nMpsX;
	int				m_nMpsY;
} PLAYER_TOWNPORTAL_POS;

typedef struct 
{
	DWORD			m_dwMapID;
	int				m_nX;
	int				m_nY;
} PLAYER_EXCHANGE_POS;

typedef struct
{
	int		m_nBuyIdx;
	DWORD	m_SubWorldID;
	int		m_nMpsX;
	int		m_nMpsY;
	void	Clear() {m_nBuyIdx = -1; m_SubWorldID = -1; m_nMpsX = 0; m_nMpsY = 0;}
} BuySellInfo;
class KIniFile;

class KEquipmentArray
{
public:
	KLinkArray	m_Link;
	int		FindSame(int nIdx);
};


#ifdef TOOLVERSION
class CORE_API KPlayer
#else
class KPlayer
#endif
{
private:

#ifndef _SERVER
	int				m_nLeftSkillID;
	int				m_nLeftSkillLevel;
	int				m_nRightSkillID;
	int				m_nRightSkillLevel;
	BOOL			m_MouseDown[2];
	BOOL 			m_bForbidTownPortal;
#endif

#ifdef _SERVER
	PLAYER_REVIVAL_POS		m_sLoginRevivalPos;	// 登入重生点位置（会存盘）
	PLAYER_REVIVAL_POS		m_sDeathRevivalPos;	// 死亡重生点（默认为登入重生点，不存盘）
	PLAYER_TOWNPORTAL_POS	m_sPortalPos;		// 传送门位置
	BOOL			m_bUseReviveIdWhenLogin;
	int				m_nExtPoint;				// 活动点数
	int				m_nChangeExtPoint;			// 变动的点数
#endif
	int				m_nPhysicsSkillID;		//当前玩家的物理攻击技能
	int				m_nPeapleIdx;
	int				m_nObjectIdx;
	int				m_nPickObjectIdx;
	int				m_nPlayerIndex;				// 本实例在 Player 数组中的位置
	KCacheNode *	m_pLastScriptCacheNode;

public:
#ifdef _SERVER
	PLAYER_EXCHANGE_POS		m_sExchangePos;
	KTimerTaskFun	m_TimerTask;
	BOOL			m_bIsQuiting;
	//BOOL			m_nLixian;
	BYTE			m_byLixian;
	
	UINT			m_uMustSave;
	DWORD			m_ulLastSaveTime;
	DWORD			m_dwLoginTime;
//	DWORD			m_uLastPingTime;
//	char			m_AccoutName[32];				
	void*			m_pStatusLoadPlayerInfo;	//加载玩家信息时用
	BYTE*			m_pCurStatusOffset;			//二进制时，记录读到指针位置了
	BOOL			m_bFinishLoading;			//完成加载
	BYTE			m_SaveBuffer[64 * 1024];	//保存缓冲
	int				m_nLastNetOperationTime;	//最后一次网络操作时间


	int				m_timetancong;
	int				m_nNumberNetPaketKT;
	BOOL			m_bForbidTownPortal;



	BOOL			m_bSleepMode;
	KList			m_PlayerWayPointList;		//
	KList			m_PlayerStationList;
	int				m_nViewEquipTime;			// 最后一次察看他人装备的时间
	int				m_nPrePayMoney;
	enum
	{
		FF_CHAT = 0x01,
	};
	int				m_nForbiddenFlag;			// 禁止标志
	//int				m_nLiXian;					// UYTHAC
	//int				m_nLiXian2;					// UYTHAC
	// Auto Play
	
	int				m_AutoLifePoison;
	int				m_AutoManaPoison;
	int				m_AutoAllPoison;
	int				m_AutoAllPoisonMin;
	BOOL			m_AutoSuaTrangBi;
	BOOL			m_AutoGuiTien;
	BOOL			m_AutoBanTrangBi;
	BOOL			m_AutoNhatDoChonLoc;

	//BOOL			m_AutoTTL;
	BOOL			m_AutoQuayLai;
	
	int				Auto_LuuMap;
	int				Auto_LuuThanhThi;
	int				Auto_LuuX;
	int				Auto_LuuY;
	int				Auto_DemTime;
	int				CheckViTriTrong;
	// Auto Loc Do
	int 			MangOption[39][2];
	int				MangDropItem[30];
	int				m_AutoNumSelect;
	BOOL			m_AutoRingSelect;
	BOOL			m_AutoWeaSelect;


	int 			CheckOption(int Type, int Value);
	// END

	char			m_szFunStringBox[32];
	DWORD			m_dwScriptStringBoxID;

	DWORD			m_dwScriptCheckItemBoxID;
	char			m_szFunCheckItemBox[32];

	DWORD			m_dwScriptSlectCheckBoxID;
	char			m_szFunSlectCheckBox[32];
	BYTE            m_btSlectCheckBox[3];

	DWORD			m_dwTaskAnswerScriptID;


	char			m_szCPUIPName[64];


	int             m_nPlayerBuyShop;

	DWORD           m_dwNpcRequestOld;
	int             m_nNpcRequestOldTime;

#endif

	DWORD			m_dwTimeThrowAwayItem;

	DWORD			m_dwTimeRideHorse;


	BOOL            m_bBlock;
	int             m_bTempBlock;


	char			m_szNameOpenShop[32];
	DWORD			m_dwTimeOpenShop;
	DWORD			m_dwTimeBuyShop;

	


	

#ifndef _SERVER
    int				m_nLastFightMode;
	int				m_RunStatus;				// 是跑还是走
	DWORD			m_dwNextLevelLeadExp;		// 统率力下一级经验值
	int				m_nSendMoveFrames;			// 用于控制客户端向服务器发送移动(走或跑)协议的频率，使之不能发送大量的移动协议，减小带宽压力
    int             m_nImageNpcID;
    int					m_SubWorldIndex;		// Npc所在的SubWorld ID
	// Update Them
	BOOL			m_HideRes;
	BOOL			m_HideMissle;
	BOOL			m_HideMissle2;
	BOOL			m_HideGame1;
	BOOL			m_HideGame2;
	BOOL			m_HideGame3;
	BOOL			m_AutoHorse;
	BOOL			m_QuanhDiem;
	BOOL			m_TargetPlayer;
	BOOL			m_TheoSau;
	BOOL			m_TuVe;
	BOOL			m_checkTargetPlayer;
	int				timeAutoInvite;
	BOOL			m_bUseTTL;
	BOOL			m_bUseTDP;

	BOOL			m_bNgaMy1;
	BOOL			m_bNgaMy2;
	BOOL			m_bNgaMy3;
	BOOL			m_bNgaMy4;
	BOOL			m_bNgaMy5;
	
	BOOL		m_bHidePlayer1;
	BOOL		m_bHidePlayer2;
	int			m_eHidePlayer3;
	int			m_eBoQuaPK;
	// Ket thuc update	
	int			TK_PK1;
	int			TK_PK2;
	int			TK_PK3;
	int			TK_PK4;
	
	int 			MangQuai[500][2];
	int             m_nBuyNumberUId;
	int             m_nBuyNumberIdx;
	int             m_nBuyNumberWidth;
	int             m_nBuyNumberHeigh;
	int             m_nBuyNumberPrice;
	int             m_nBuyNumberNum;
	BOOL            m_nAutoPlay;
    BOOL			m_bHideNpc;
	BOOL			m_bHidePlayer;
	BOOL			m_bHideSkill;
	int             m_nAutoTimeNextNpc;
	int             m_nAutoTimeNextObj;
	int             m_nAutoTimeNextMana;
	int             m_nAutoTimeNextLife;
	BOOL            m_nAutoPause;
	int             m_nAutoOldCurenLifeActive;
    int             m_nAutoStarNpcX;
    int             NgaMiSkill1;
    int             NgaMiSkill2;
	int             m_nAutoStarNpcY;
	int             m_nAutoStarSkillIDx;
	int             m_nAutoLife;
	int             m_nNgaMyBuff;
	int             m_nAutoMana;
	int             m_nAutoReturn;
	int             m_nAutoAtack;
	int             m_nTempAutoAtack;
	int             m_nAutoRange;
	int             m_nAutoNAtack;
	int             m_nPhamViTuVe;
	int             m_nGuiTienNew;
	BOOL			m_bMoneyObj;
	BOOL			m_bItemObj;
	BOOL			m_bItemSelect;
	BOOL			m_bPropObj;
	int 			checktoado;
	
	int             m_nAutoTimeNextLBMana;
	int             m_nAutoTimeNextLBLife;
	int             m_nAutoTimeNextLBMaLi;


#endif

	KIndexNode		m_Node;
	GUID			m_Guid;
	BOOL			m_bExchangeServer;
	int				m_DebugMode;
	DWORD			m_dwID;						// 玩家的32位ID
	int				m_nIndex;					// 玩家的Npc编号
	int				m_nNetConnectIdx;			// 第几个网络连接
	char			m_nPlayerHWID[64];			// Player HWID 64 len edit by phong kieu
	KItemList		m_ItemList;					// 玩家的装备列表
	BuySellInfo		m_BuyInfo;					// 进行的交易列表
	KPlayerMenuState	m_cMenuState;			// 是否处于交易或队伍开放状态
	KTrade			m_cTrade;					// 交易模块
    	//TamLTM check nhat do cua nguoi khac
	BOOL			m_bNotPickUpItem;
	BOOL			m_bNotPickUpMoney;
	//end code
	int				m_nAttributePoint;			// 未分配属性点
	int				m_nSkillPoint;				// 未分配技能点

	int				m_nStrength;				// 玩家的基本力量（决定基本伤害）
	int				m_nDexterity;				// 玩家的基本敏捷（决定命中、体力）
	int				m_nVitality;				// 玩家的基本活力（决定生命、体力）
	int				m_nEngergy;					// 玩家的基本精力（决定内力）
	int				m_nLucky;					// 玩家的基本运气（影响得到装备的好坏）
	BYTE			m_btChatSpecialChannel;		

	int				m_nCurStrength;				// 玩家的当前力量（决定基本伤害）
	int				m_nCurDexterity;			// 玩家的当前敏捷（决定命中、体力）
	int				m_nCurVitality;				// 玩家的当前活力（决定生命、体力）
	int				m_nCurEngergy;				// 玩家的当前精力（决定内力）
	int				m_nCurLucky;				// 玩家的当前运气（影响得到装备的好坏）
	void 			SetExp(int nExpAdd);
	int				m_nExp;						// 当前经验值(当前等级在npc身上)
	int				m_nNextLevelExp;			// 下一级经验值
	//BOOL 			Check_AcceptAutoName;
	DWORD			m_dwLeadExp;				// 统率力经验值
	DWORD			m_dwLeadLevel;				// 统率力等级
	char			m_PlayerName[32];
    char			m_AccoutName[32];			//edit by phong kieu khai bao bien m_AccoutName

	
	KPlayerTeam		m_cTeam;					// 玩家的组队信息
	KPlayerFaction	m_cFaction;					// 玩家的门派信息

	KPlayerChat		m_cChat;

	KPlayerTask		m_cTask;					// 玩家任务系统(变量)

	KPlayerPK		m_cPK;						// PK关系处理

	KPlayerTong		m_cTong;					// 自己的帮会信息

	DWORD			m_dwDeathScriptId;			// 
    DWORD			m_dwTimeBoxId;
    DWORD			m_dwGiveBoxId;
    DWORD			m_dwRewardId;
	DWORD			m_dwRewardExId;	

	char			m_szTaskAnswerFun[MAX_ANSWERNUM][32];
	int				m_nAvailableAnswerNum;//当前选择界面下，最大回答数。
	bool			m_bWaitingPlayerFeedBack;	//当前是否正等待玩家在客户端的反馈。该状态下，当前脚本不置空.类式对话选择情况
//	DWORD			m_dwOutOfDateFeedBackTime;	//当前等待玩家与服务器脚本交互的最大限定时刻，到了这个游戏时间如果玩家仍未反馈则视作废。
	BYTE			m_btTryExecuteScriptTimes;	//
	//char			m_CurScriptName[128];
	int				m_nWorldStat;
	int				m_nSectStat;
	int				m_ImagePlayer;	// chan dung
    int				m_ImageId;			// bien id npc image
    private:	

	//BOOL			m_bActiveAuto;
#ifndef _SEVER
	BOOL			m_bSortEquipment;
	int				m_nReturnPortalStep;
	int				m_nCurReturnPortalSec;
	KEquipmentArray	m_sListEquipment;
	BOOL			m_bPriorityUseMouse;
	int				m_nTimePriorityUseMouse;
	int				m_LifeAuto;
	int				m_ManaAuto;
	int				m_TownAuto;
	int				m_nTimeBuffSkillAuto;
	int				m_nPickAreaItem;
	int				m_nCheckTimeBuffSkillAuto;
	int				m_PosXAuto;
	int				m_PosYAuto;
	BOOL			m_bActacker;
	int				m_Actacker;
	//int				m_TimeUseItem;
	int				m_ArrayNpcNeast[MAX_ARRAY_NPC_AUTO];
	int				m_ArrayNpcLag[MAX_ARRAY_NPC_AUTO];
	int				m_Count_Acttack_Lag;
	int				m_ArrayTimeNpcLag[MAX_ARRAY_NPC_AUTO];
	int				m_ArrayTimeObjectLag[MAX_ARRAY_NPC_AUTO];
	int				m_nLifeLag;
	int				m_nTimeRunLag;
	BOOL			m_bObject;
	int				m_nObject;
	int				m_ArrayObjectNeast[MAX_ARRAY_NPC_AUTO];
	int				m_ArrayObjectLag[MAX_ARRAY_NPC_AUTO];
    int				m_nArrayNpcOverLook[MAX_ARRAY_NPC_AUTO];
	int				m_nArrayTimeNpcOverLook[MAX_ARRAY_NPC_AUTO];
	int				m_nArrayTimeObjectOverLook[MAX_ARRAY_NPC_AUTO];
    int				m_nArrayObjectOverLook[MAX_ARRAY_NPC_AUTO];
    int				m_nArrayTimeInvitePlayer[MAX_AUTO_SIZE];
    int				m_nArrayInvitePlayer[MAX_AUTO_SIZE];
public:
	BOOL			m_bBuffSkill[3];
	BOOL			m_AutoAttack;
	//BOOL			m_bFollowPeople;
	//int				m_FollowPeopleIdx;
	//char			m_FollowPeopleName[32];
    BOOL			m_bOverTarget;
	BOOL			m_bAutoParty;
	BOOL			m_bActiveAutoParty;
	BOOL			m_bAutoMove;
	BOOL			m_bPickItem;
	BYTE			m_btPickUpKind;
	BYTE			m_btDurabilityItem;
	BYTE			m_btDurabilityValue;
	BYTE			m_AutoUseTTL;
	int				m_AuraSkill[2];
	BOOL			m_bActiveSwitchAura;
	int 			m_AutoLifeReplenishP;
	BOOL 			m_AutoLifeReplenish;
	int				m_MoveMps[defMAX_AUTO_MOVEMPSL][3];
	int				m_MoveStep;
	BOOL			m_AutoMove;
	BOOL			m_bFilterEquipment;
	int				m_FilterMagic[defMAX_AUTO_FILTERL][2];
	unsigned int	m_SpaceActionTime;
	BOOL			m_SaveRAP;
	BOOL			m_EatLife;
	BOOL			m_EatMana;
	unsigned int	m_LifeTimeUse;
	unsigned int	m_ManaTimeUse;
	int				m_LifeAutoV;
	int				m_ManaAutoV;
	unsigned int	m_LifeCountDown;
	unsigned int	m_ManaCountDown;
	unsigned int	m_PortalCountDown;
	unsigned int	m_AntiPoisonCountDown;
	unsigned int	m_TTLCountDown;
	unsigned int	m_TuiDPhamCountDown;
	unsigned int	m_ReturnSetpCountDown;
	unsigned int	m_SortEQCountDown;
	BOOL			m_TPLife;
	int				m_TPLifeV;
	BOOL			m_TPMana;
	int				m_TPManaV;
	BOOL			m_TPNotMedicineBlood;
	BOOL			m_TPNotMedicineMana;
	BOOL			m_TPHightMoney;
	int				m_TPHightMoneyV;
	BOOL			m_AutoAntiPoison;
    BOOL			m_bTPNotEquipment;
	int				m_nTPNotEquipment;	
	BOOL			m_AutoTTL;
	BOOL			m_bAttackAround;
	int				m_RadiusAuto;
	int				m_DistanceAuto;
	BOOL			m_bFightDistance;
	int				m_ArrayStateSkill[MAX_ARRAY_NPC_AUTO];
	BOOL			m_bAttackPeople;
	BOOL			m_bAttackNpc;
	BOOL			m_bBuyEquip;
	BOOL			m_bInventoryMoney;
	BOOL			m_bInventoryItem;
	BOOL			m_bRepairEquip;
	BOOL			m_bReturnPortal;
	BOOL			m_Auto_TuiDuocPham;
	BOOL			m_Auto_BanItem;
	BOOL			m_bSaveJewelry;
	BOOL			m_bSortEquipment_Active;
	int					  m_dwEquipExpandTime;//thoi gian su dung hanh trang mo rong

    BOOL			m_bFollowPeople;
	char			m_szFollowName[32];
	int				m_nFollowRadius;

    BOOL			m_bMoveMps;
	int				m_nMoveMps[MAX_AUTO_LIST][3];
	int				m_nMoveStep;
	int				m_nMoveCount;	
#endif
// end

public:
	KPlayer();
	~KPlayer();
//	void			SetLockMove(LockMoveItem *LockMove);
//	LockMoveItem*	GetLockMove() {return &m_LockMove;};
	// auto playgame
    BOOL			m_bActiveAuto;
	void			SetAutoFlag(BOOL nbAuto);
	BOOL			GetAutoFlag() { return m_bActiveAuto; };
	BOOL			GetFightFlag() { return m_AutoAttack; };

	void			SetPlayerIndex(int nNo);					// 设定 m_nPlayerIndex
	void			GetAboutPos(KMapPos *pMapPos);			// 获得玩家附近一个空位置
	int				GetPlayerIndex();							// 获得本实例在 Player 数组中的位置
	DWORD			GetPlayerID(){return m_dwID;};
	void			GetFactionName(char *lpszName, int nSize);	// 获得当前门派名称 not end
	
	
	void			ChatFriendOnLine(DWORD dwID, int nFriendIdx);// 获得通知某好友上线了
	BOOL			ExecuteScript(char * ScriptFileName, char * szFunName, int nParam = 0);
	BOOL			ExecuteScript(char * ScriptFileName, char * szFunName, char * szParams);
	BOOL			ExecuteScript(DWORD dwScriptId, char * szFunName, char *  szParams);
	BOOL			ExecuteScript(DWORD dwScriptId,  char * szFunName, int nParam);



	BOOL			ExecuteScriptNew(char * ScriptFileName, char * szFunName, char * szParams, char * szParamsNew);
	BOOL			ExecuteScriptNew(DWORD dwScriptId, char * szFunName, char *  szParams, char * szParamsNew);



	BOOL			DoScript(char * ScriptCommand);				//执行某个脚本指令

	void			ChangeCurStrength(int nData);			// 改变当前力量(当 nData 小于 0 时，减少)
	void			ChangeCurDexterity(int nData);			// 改变当前敏捷(当 nData 小于 0 时，减少)
	void			ChangeCurVitality(int nData);			// 改变当前活力(当 nData 小于 0 时，减少)
	void			ChangeCurEngergy(int nData);			// 改变当前精力(当 nData 小于 0 时，减少)
	BOOL			ExecuteScript(char * ScriptFileName);
	void			Release();
	void			Active();								// 玩家每次游戏循环都需要处理的东西
	void			ProcessMsg(KWorldMsgNode *lpMsg);		// 处理世界消息，转为NPC命令
	
	LPSTR			GetPlayerName() { return m_PlayerName; };
    LPSTR			GetPlayerAccount() { return m_AccoutName; };//edit by phong kieu them ham getplayeraccount
//	LPSTR			GetNameComputer() { return m_szCPUIPName; };

	BOOL			NewPlayerGetBaseAttribute(int Series);	// 新玩家登陆时根据五行属性产生 力量 敏捷 活力 精力 四项数值
	void			AddBaseLucky(int nData);				// 增加基本运气

	void			AddExp(int nExp, int nTarLevel,BOOL bCheck = FALSE);		// 增加经验(原始数据，还未经过处理)
	void			AddSelfExp(int nExp, int nTarLevel);	// 增加经验(不需要再经过队伍分配的处理，但需要考虑被砍死npc的等级)
	void			DirectAddExp(int nExp);					// 直接增加经验值，不考虑其他因素
	void			LevelUp();								// 升一级
	void	        SetLevel(int nLevel);
	
	void			AddLeadExp(int nExp);					// 增加统率力经验
	void			UpdataCurData();
	void			ReCalcEquip();							// 重新计算身上的装备
	void			ReCalcState();							// 重新计算身上的状态

	void			ChangePlayerCamp(int nCamp);			// 改变玩家阵营
	void			Revive(int nType);						// 重生	

	BOOL			CheckTrading();
	BOOL			CheckShopOpen();
	BOOL			CheckShopBuyOpen();
	void			SetFirstDamage();
	void			SetBaseAttackRating();
	void			SetBaseDefence();
	void			SetBaseResistData();
	void			SetBaseSpeedAndRadius();
	int				GetPhysicsSkillId(){return m_nPhysicsSkillID;};
	void			SetPhysicsSkillId(int nPhysicsSkillID)
	{
		if (nPhysicsSkillID <= 0) return ;
		ISkill * pISkill =  g_SkillManager.GetSkill(nPhysicsSkillID, 1);
		if (!pISkill) 
            return ;
		
        if (nPhysicsSkillID > 0 && pISkill->IsPhysical())
			m_nPhysicsSkillID = nPhysicsSkillID;
	};
    void			SetImageNpcId(int nNumber); // set hinh anh npc cho say new
//	void			SetChangeFace(int nNumber);
#ifndef _SERVER
	void		   SortEquipment();
	void		   SetSortEquipment(BOOL bFlag);

	BOOL	     	CheckEquip(BYTE btDetail);
	BOOL		    CheckEquipMagic(int nIdx);
	BOOL			ReturnFromPortal();
	BOOL			InventoryItem();
	BOOL			AutoSellItem();
	int				GetTargetNpc() { return m_nPeapleIdx; };
	int				GetTargetObj() { return m_nObjectIdx; };
	void			SetTargetNpc(int n) { m_nPeapleIdx = n; };
	void			SetTargetObj(int n) { m_nObjectIdx = n; };
	void			FindSelectNpc(int x, int y, int nRelation);
	void			FindSelectObject(int x, int y);
	void			Walk(int nDir, int nSpeed);	
	void			TurnLeft();
	void			TurnRight();
	void			TurnBack();
	void			DrawSelectInfo();
	BOOL			ConformIdx(int nIdx);
	
	void            OpenBuyShop(DWORD dwId);
	void			GetAutoQDXY(char* szBufferXY)
	{
		if(m_PosXAuto == 0 || m_PosYAuto == 0)
		{
			int nDesX = 0;
			int nDesY = 0;
			Npc[m_nIndex].GetMpsPos(&nDesX, &nDesY);
			m_PosXAuto = nDesX;
			m_PosYAuto = nDesY;
		}
		sprintf(szBufferXY, "  [%d/%d]", m_PosXAuto/8/32, m_PosYAuto/16/32);
		//strcpy(szName, Player[i].m_AccoutName);
	};
	void			GetEchoDamage(int* nMin, int* nMax, int nType);// 获取界面需要显示的伤害值
	void			GetEchoAttack(int* nAttack, int nType);
	void			ProcessInputMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);// 处理键盘鼠标消息
	void			RecvSyncData();								// 接收同步数据

	void			ApplyTeamInfo(DWORD dwNpcID);				// 向服务器申请查询某个npc所在队伍的信息
	void			ApplySelfTeamInfo();						// 向服务器申请查询玩家自身的队伍情况
	BOOL			ApplyCreateTeam();//char *lpszTeamName);		// 玩家向服务器申请创建队伍
	BOOL			ApplyTeamOpenClose(BOOL bFlag);				// 队长向服务器申请开放、关闭队伍是否允许加入成员状态
	void			ApplyAddTeam(int nNpcIndex);				// 玩家向服务器申请加入某个队伍
	void			AcceptTeamMember(DWORD dwNpcID);			// 玩家通知服务器接受某个npc为队伍成员
	void			TeamDropApplyOne(DWORD dwNpcID);			// 队长删除加入队伍申请列表中的某个npc
	void			LeaveTeam();								// 通知服务器本玩家离开队伍
	void			TeamKickMember(DWORD dwNpcID);				// 队长通知服务器踢除某个队员
	void			ApplyTeamChangeCaptain(DWORD dwNpcID);		// 队长向服务器申请把自己的队长身份交给别的队员
	void			ApplyTeamDismiss();							// 队长向服务器申请解散队伍
	void			ApplySetPK(BOOL bPK);						// 玩家向服务器申请打开、关闭pk开关
	void			ApplyFactionData();							// 玩家向服务器申请门派数据
	void			SendChat(KUiMsgParam *pMsg, char *lpszSentence);// 客户端发送聊天语句给服务器
	void			ApplyAddBaseAttribute(int nAttribute, int nNo);// 队长向服务器申请增加四项属性中某一项的点数(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
	BOOL			ApplyAddSkillLevel(int nSkillID, int nAddPoint);// 向服务器申请某个技能升级
	BOOL			ApplyUseItem(int nItemID, ItemPos SrcPos);	// 向服务器申请使用某个物品（鼠标右键点击该物品）
	void			PickUpObj(int nObjIndex);					// 客户端鼠标点击obj检起某个物品，向服务器发消息
	void			ObjMouseClick(int nObjIndex);				// 客户端鼠标点击obj，向服务器发消息
	void			MoveItem(ItemPos DownPos, ItemPos UpPos);	// DownPos 不能是手，UpPos 必须是手
	int				ThrowAwayItem();
	void			ChatAddFriend(int nPlayerIdx);				// 客户端通过别人的添加聊天好友的申请
	void			ChatRefuseFriend(int nPlayerIdx);			// 客户端拒绝别人的添加聊天好友的申请
	void			TradeApplyOpen(char *lpszSentence, int nLength);
	void			TradeApplyClose();
	void			TradeApplyStart(int nNpcIdx);
	BOOL			TradeMoveMoney(int nMoney);					// 交易时输入自己的钱	
	void			TradeDecision(int nDecision);				// if nDecision == 0 推出交易  if nDecision == 1 确定交易  if nDecision == 2 取消交易确定
	void			TradeApplyLock(int nLockOrNot);				// 交易锁定或取消锁定

	void			SetChatCurChannel(int nChannelNo);			// 设定当前聊天频道
	void			TeamInviteAdd(DWORD dwNpcID);				// 邀请加入队伍

	void			SetLeftSkill(int nSkillID);
	void			SetRightSkill(int nSkillID);
	void			UpdateWeaponSkill();
	int				GetLeftSkill(){return m_nLeftSkillID;};
	int				GetRightSkill(){return m_nRightSkillID;};
	void			SetDefaultImmedSkill();
	void			s2cApplyAddTeam(BYTE* pProtocol);			// 收到服务器通知有人申请加入队伍
	void			s2cTradeChangeState(BYTE* pMsg);
	void			s2cTradeMoneySync(BYTE* pMsg);
	void			s2cTradeDecision(BYTE* pMsg);				// 收到服务器通知交易完成或取消
	void			SyncCurPlayer(BYTE* pMsg);
	BOOL			dacheck;
	void			s2cLevelUp(BYTE* pMsg);
	void			s2cGetCurAttribute(BYTE* pMsg);
	void			s2cSetExp(int nExp);
	void			s2cSyncMoney(BYTE* pMsg);
	void			s2cTradeApplyStart(BYTE* pMsg);
	void			CheckObject(int nIdx);
    void			PlayerStarBuyItemNumber(int nuId,int nIdx,int nWidth,int nHeight,int nPrice,int nNum = 1);
    void			PlayerSetAuto(int nLife,int nMana, int nReturn, int nAtack,int nRange, int nNAtack, BOOL bcheck, BOOL bMoneyObj, BOOL bItemObj, BOOL bPropObj, BOOL bItemSelect, int nNgaMyBuff, int nPhamViTuVe, int nGuiTienNew);
	void			PlayerAutoOn();
	BOOL 			PlayerAutoOnStolen();
	BOOL 			PlayerAutoOnSkill();
	BOOL 			PlayerAutoOnSkillTheoSau();
	BOOL			PlayerAutoPK();
	BOOL			CheckAutoTuVe();
	BOOL			AutoTheoSau();
	BOOL			QuayLaiDiaDiemCu();
	BOOL			QuayLaiDiaDiemCu2();
	BOOL 			PlayerAutoOnBuffE();
	BOOL 			PlayerAutoOnBuffL();
	BOOL 			PlayerAutoOnBuffH();
//	BOOL			PlayerAutoTeam();
	BOOL			PlayerAutoLocDo();
	void			CheckRideHouse(BOOL);
	void				SendShopCost();
	BOOL 			PlayerAutoOnObj();
#endif

#ifdef _SERVER
	void	        FixPlayer();
    void            PlayerAutoUnBlock();
    void            PlayerRestAP(int nsuc_manh_goc,int nthan_phap_goc,int nsinh_khi_goc,int nnoi_cong_goc);
	void			SetExtPoint(int nPoint, int nChangePoint);
	int				GetExtPoint();
	BOOL			PayExtPoint(int nPoint);
	int				GetExtPointChanged();
	void			RepairItem(DWORD dwItemID);
	BOOL			PrePay(int nMoney);
	void			SetLastNetOperationTime(int nTime);
	int				FindAroundPlayer(DWORD dwNpcID);		// 寻找玩家周围的某个指定npc id的player index
	int				FindAroundNpc(DWORD dwNpcID);			// 寻找玩家周围的某个指定npc id的npc index
	BOOL			CheckPlayerAround(int nPlayerIdx);		// 判断某玩家是否在周围
	BOOL			IsExchangingServer();
	void			TobeExchangeServer(DWORD dwMapID, int nX, int nY);
//	void			UpdateEnterGamePos(DWORD dwSubWorldID, int nX, int nY, int nFightMode);
	BOOL			IsWaitingRemove();
	BOOL			IsLoginTimeOut();
	void			WaitForRemove();
	void			LoginTimeOut();
	void			UseTownPortal(int bCheck = 0);
	void			BackToTownPortal();
	void			GetTownPortal(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY);
	void			GetRevID(int *lpnRevID);

	void			GetLoginRevivalPos(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY);		// 获取玩家登入重生点位置
	void			GetDeathRevivalPos(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY);		// 获取玩家死亡重生点位置
	void			SetRevivalPos(int nSubWorld, int nRevalId);								// 设定玩家重生点ID
	BOOL			Save();									// 保存玩家数据
	BOOL			CanSave();

	BOOL			CheckSaveRsul();

	void			ProcessUser();
	BOOL			SendSyncData(int &nStep, unsigned int &nParam);	// 发送同步数据
	BOOL			SendSyncData_Skill();					// 发送同步数据 - 技能
	void			SendCurNormalSyncData();				// 发送平时给自己的同步数据
	void			SendCurHeightSyncData();
	void			SetChatForbiddenFlag(int nFlag)
	{
		m_nForbiddenFlag = nFlag;
	};

	BOOL			AddFaction(char *lpszFactionName);		// 加入门派
	BOOL			AddFaction(int nFactionID);				// 加入门派
	BOOL			LeaveCurFaction();						// 离开门派
	BOOL			CurFactionOpenSkill(int nLevel);		// 开放当前门派某个等级的技能

	void			TaskClearTempVal();						// 清空任务临时变量
	int				TaskGetSaveVal(int nNo);				// 得到任务完成情况
	void			TaskSetSaveVal(int nNo, BOOL bFlag);	// 设定任务完成情况
	int				TaskGetClearVal(int nNo);				// 得到任务临时过程控制变量值
	void			TaskSetClearVal(int nNo, int nVal);		// 设定任务临时过程控制变量值

	//void			SetTimeTaskTime(DWORD time){m_dwTimeTaskTime = time;};
	void			BuyItem(BYTE* pProtocol);
	void			SellItem(BYTE* pProtocol);
	void			AutoSellItem(BYTE* pProtocol);
	void			StaskItem(BYTE* pProtocol);
	void			StringBox(BYTE* pProtocol);

	void			LoginPWStoreBox(BYTE* pProtocol);
	void			PWStoreBox(BYTE* pProtocol);
	void			BlockStoreBox(BYTE* pProtocol);
	void			UnBlockStoreBox(BYTE* pProtocol);

	void			NewPWStoreBox(BYTE* pProtocol);

	void			CheckItemBox(BYTE* pProtocol);
	void			SlectCheckBox(BYTE* pProtocol);
	void            ButtonPGBox(BYTE* pProtocol);

	void            ShopName(BYTE* pProtocol);
	void            ShopPrice(BYTE* pProtocol);

	void            ShopOpen(BYTE* pProtocol);

	void            OpenBuyShop(BYTE* pProtocol);
	void            ShopBuyItem(BYTE* pProtocol);
	void            CloseBuyShop(BYTE* pProtocol);


	void			BlockItem(BYTE* pProtocol);
	void			UnBlockItem(BYTE* pProtocol);
	void			QuitGame(int nQuitType);				// 退出游戏
	void			S2CSendTeamInfo(BYTE* pProtocol);		// 收到客户端查询某个npc组队信息的申请后，向这个客户端发送队伍信息
	void			SendSelfTeamInfo();						// 服务器向客户端发送队伍信息
	BOOL			CreateTeam(BYTE* pProtocol);			// 收到客户端请求创建一支队伍
	BOOL			SetTeamState(BYTE* pProtocol);			// 收到客户端请求开放、关闭本player队伍是否允许队员加入状态
	BOOL			S2CSendAddTeamInfo(BYTE* pProtocol);	// 收到客户端请求加入一支队伍
	BOOL			AddTeamMember(BYTE* pProtocol);			// 队长通知服务器接受某个npc为队伍成员
	void			LeaveTeam(BYTE* pProtocol);				// 收到客户端队员通知离开队伍
	void			TeamKickOne(BYTE* pProtocol);			// 收到客户端队长通知踢出某个队员
    void			SendTeamMessage(int nTeamID, const char* szMessage);
    void			SendSystemMessage(const char* szHead, const char* szMessage);
	BOOL			TeamChangeCaptain(BYTE* pProtocol);		// 收到客户端队长通知把队长身份交给某个队员
	void			TeamDismiss(BYTE* pProtocol);			// 收到客户端队长请求解散队伍
	void			SetPK(BYTE* pProtocol);					// 收到客户端请求设定PK状态
	void			SendFactionData(BYTE* pProtocol);		// 收到客户端请求获得门派数据
	void			ServerSendChat(BYTE* pProtocol);		// 收到客户端发来的聊天语句
	void			AddBaseAttribute(BYTE* pProtocol);		// 收到客户端要求增加基本属性点(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
	void			AddSkillPoint(BYTE* pProtocol);			// 收到客户端要求增加某个技能的点数
	BOOL			ServerPickUpItem(BYTE* pProtocol);		// 收到客户端消息鼠标点击某个obj拣起装备或金钱
	void			EatItem(BYTE* pProtocol);				// 收到客户端消息吃药
	void			ServerMoveItem(BYTE* pProtocol);		// 收到客户端消息移动物品
	void			ServerThrowAwayItem(BYTE* pProtocol);	// 收到客户端消息丢弃物品
	void			ChatSetTakeChannel(BYTE* pProtocol);	// 收到客户端消息设定聊天订阅频道
	void			ChatTransmitApplyAddFriend(BYTE* pProtocol);// 收到客户端请求转发聊天添加好友信息
	BOOL			ChatAddFriend(BYTE* pProtocol);			// 收到客户端消息添加聊天好友
	void			ChatRefuseFriend(BYTE* pProtocol);		// 收到客户端消息拒绝添加聊天好友
	void			ChatResendAllFriend(BYTE* pProtocol);
	void			ChatSendOneFriendData(BYTE* pProtocol);
	void			ChatDeleteFriend(BYTE* pProtocol);		// 收到客户端申请删除某个聊天好友
	void			ChatRedeleteFriend(BYTE* pProtocol);	// 收到客户端申请再次删除某个聊天好友
	void			TradeApplyOpen(BYTE* pProtocol);		// 收到客户端申请进入待交易状态
	void			TradeApplyClose(BYTE* pProtocol);		// 收到客户端申请取消待交易状态
	void			TradeApplyStart(BYTE* pProtocol);		// 收到客户端申请开始交易
	void			TradeMoveMoney(BYTE* pProtocol);		// 收到客户端申请交易中money的改变
	void			TradeDecision(BYTE* pProtocol);			// 收到客户端申请交易确定或取消
	void			c2sTradeReplyStart(BYTE* pProtocol);
	void			SyncTradeState();						// 给交易双方的客户端发送交易状态信息
	void			SendEquipItemInfo(int nTargetPlayer);	// 发送自己装备在身上的装备信息给别人看
	PLAYER_REVIVAL_POS* GetDeathRevivalPos() 
	{
		return &m_sDeathRevivalPos;
	};
	BOOL			IsUseReviveIdWhenLogin() { return m_bUseReviveIdWhenLogin; };
	void			SetLoginType(BOOL bUseReviveId) { m_bUseReviveIdWhenLogin = bUseReviveId; };

	BOOL			CreateTong(int nCamp, char *lpszTongName, char *lpszTitle);
	void			SetNumImg(int nNumber);
	void	        LoginTong(int nFigure,int nCamp,char* szName,char* szTitle,char* szTongName,char* szMaster);

    void            SendTongInfo(int nSlect,int nPlayerSect,int nNumberDirector,int nNumberManager,int nNumberMember,char *szTongName,int nCamp,int nLevel,int nMoney,char *szListMember);

    void		    MemberAddTong(int nCamp,char* szTitle,char* lpszTongName,char* lpszTongMaster);
	void		    DeleteTong();
	void		    UpdateTong(int dwTongId);

	int             GetIdxItemBox();
	int             GetIdxItemBox2();
	int             GetIdxItemBox3();
	int             GetIdxItemBoxUpdateItem();
	int             GetIdxItemBoxUpdateItem2();
	int             GetIdxItemBoxUpdateItem3();
	int             GetIdxItemBoxUpdateItem4();
	int				GetIdxItemBoxQuestKey(int GetIdxItemBoxQuestKey);
	int				DelEquipItemQuestKey(int IdQuestKey);


	int             CheckMagicItem(int nItemIdx,int nMagicId,int nMagicNumber1,int nMagicNumber2);
	int             GetLevelItem(int nItemIdx);
	int             GetDetailTypeItem(int nItemIdx);
	int             GetClassItem(int nItemIdx);
	int             GetParticularItem(int nItemIdx);
	int             GetSeriItem(int nItemIdx);
	int             GetGenreItem(int nItemIdx);
	int				GetTimeItem(int nItemIdx);

	int             IsBlockItem(int nItemIdx);

	int             GetIdxPGBoxItem(int nPage,int nLocation);

	int             GetIdMagicEventItem(int nItemIdx);
	int             GetLevelMagicEventItem(int nItemIdx);
	int             GetRequirementMagicEventItem(int nItemIdx);
	int             GetItemType(int nItemIdx);
	int             GetEquipMagicId(int nItemIdx,int nItemNum);
	int             GetEquipMagicLevel(int nItemIdx,int nItemNum);
	int             GetEquipMagicPoint(int nItemIdx,int nItemNum);

	int             GetEquipMagicLucky(int nItemIdx,int nItemNum);
	int             GetItemBoxLucky(int nItemIdx);

	int             GetGetEquipMagicRandomSeed(int nItemIdx);

	int             GetNumberNameInCpuTK();


	void            AddPropObj(int nId);
	void            AddPropObjPos(int nId, int nIdMap, int nPosX, int nPosY);

#endif

private:
//	void			CalcCurStrength();						// 计算当前力量
//	void			CalcCurDexterity();						// 计算当前敏捷
//	void			CalcCurVitality();						// 计算当前活力
//	void			CalcCurEngergy();						// 计算当前精力
	void			CalcCurLucky();							// 计算当前运气
	void			LevelAttriBaseLifeMax();					// 等级升一级后增加最大生命点
	void			LevelAttriBaseManaMax();					// 等级升一级后增加最大内力点
	void			LevelAttriBaseStaminaMax();				// 等级升一级后增加最大体力点

	void			SendFactionData();						// 向客户端发送门派数据

	void			AddBaseStrength(int nData);				// 增加基本力量
	void			AddBaseDexterity(int nData);			// 增加基本敏捷
	void			AddBaseVitality(int nData);				// 增加基本活力
	void			AddBaseEngergy(int nData);				// 增加基本精力
	void			SetNpcPhysicsDamage();					// 由当前力量计算对应npc的物理伤害(PhysicsDamage)
	void			SetNpcAttackRating();					// 由当前敏捷计算对应npc的攻击命中率(AttackRating)
	void			SetNpcDefence();						// 由当前敏捷计算对应npc的防御力
//	void			SetNpcWalkSpeed();						// 由当前敏捷计算对应npc的行走速度
//	void			SetNpcRunSpeed();						// 由当前敏捷计算对应npc的跑步速度

#ifndef _SERVER
	void			ProcessMouse(int x, int y, int Key, MOUSE_BUTTON nButton);
	void			OnButtonUp(int x,int y,MOUSE_BUTTON nButton);				// 处理鼠标键抬起
	void			OnButtonDown(int x,int y,int Key,MOUSE_BUTTON nButton);		// 处理鼠标键按下
	void			OnButtonMove(int x,int y,int Key,MOUSE_BUTTON nButton);		// 处理鼠标键按下后移动
	void			OnMouseMove(int x,int y);									// 处理鼠标移动
	int				NetCommandPlayerTalk(BYTE* pProtocol);

	//Question:为单机测试版使用
	friend			LuaInitStandAloneGame(Lua_State * L);
#endif


// 豆豆的东西
private:
	void			S2CExecuteScript(char * ScriptName, char * szParam);

#ifdef _SERVER
//数据库模块函数-----------------
private:
	int				LoadPlayerBaseInfo(BYTE * pRoleBuffer, BYTE * &pRoleBaseBuffer, unsigned int &nParam );
	int				LoadPlayerItemList(BYTE * pRoleBuffer, BYTE * &pItemBuffer, unsigned int &nParam );
	int				LoadPlayerFightSkillList(BYTE * pRoleBuffer, BYTE * &pSkillBuffer, unsigned int &nParam);
	int				LoadPlayerLifeSkilllList(BYTE * pRoleBuffer, BYTE * &pSkillBuffer, unsigned int &nParam);
	int				LoadPlayerFriendList(BYTE * pRoleBuffer, BYTE * &pFriendBuffer, unsigned int &nParam);
	int				LoadPlayerTaskList(BYTE * pRoleBuffer, BYTE * &pTaskBuffer, unsigned int &nParam);
	int				SavePlayerBaseInfo(BYTE * pRoleBuffer);
	int				SavePlayerItemList(BYTE * pRoleBuffer);
	int				SavePlayerFightSkillList(BYTE * pRoleBuffer);
	int				SavePlayerLifeSkilllList(BYTE * pRoleBuffer);
	int				SavePlayerFriendList(BYTE * pRoleBuffer);
	int				SavePlayerTaskList(BYTE * pRoleBuffer);
#endif

public:
	void			SetNpcDamageAttrib();
	void			DoScriptAction(PLAYER_SCRIPTACTION_SYNC * pUIInfo); //通知该客户端显示某个UI界面
	void			ProcessPlayerSelectFromUI(BYTE* pProtocol);			// 处理当玩家从选择菜单选择某项时的操作
#ifndef _SERVER
	void			DialogNpc(int nIndex);
	void			OnSelectFromUI(PLAYER_SELECTUI_COMMAND * pSelectUI, UIInfo eUIInfo);//当玩家从选择框中选择某项后，将向服务器发送			
	void			OnScriptAction(PLAYER_SCRIPTACTION_SYNC * );
#endif
#ifdef _SERVER
	void			RestoreLiveData();						//重生后恢复玩家的基本数据
	void			SetTimer(DWORD nTime, int nTimeTaskId);//时间任务脚本，开启计时器
	void			CloseTimer();							//关闭时间计时器

	int				AddDBPlayer(char * szPlayerName, int sex, DWORD * pdwID );
	int				LoadDBPlayerInfo(BYTE * pPlayerInfo,  int &nStep, unsigned int &nParam);
	BOOL			GetNewPlayerFromIni(KIniFile * pIniFile, BYTE * pRoleBuffer);
	int				UpdateDBPlayerInfo(BYTE * pPlayerInfo);
	int				DeletePlayer(char * szPlayerName = NULL);//注意：本函数是清除玩家帐号！！！，不能乱用
	void			LaunchPlayer();
	BOOL			Pay(int nMoney);
	BOOL			Earn(int nMoney);
	void			DialogNpc(BYTE * pProtocol);

	int				AddTempTaskValue(void* pData);
#endif
#ifndef _SERVER
	void			SendInfoAuto();
	void			PlayerAuto();
	void			MoveTo(int nX, int nY);
	void			ClearArrayNpcLag()
	{
		for (int i=0; i < MAX_ARRAY_NPC_AUTO; i++)
		{
			m_ArrayNpcLag[i] = 0;
		}
	};
	void			ClearArrayObjectLag()   // time nhat do 
	{
		for (int i=0; i < MAX_ARRAY_NPC_AUTO; i++)
		{
			m_ArrayObjectLag[i] = 0;
		}
	};
	void			ClearArrayTimeNpcLag()
	{
		for (int i=0; i < MAX_ARRAY_NPC_AUTO; i++)
		{
			m_ArrayTimeNpcLag[i] = 0;
		}
	};
	void			ClearArrayTimeObjectLag() // time nhat do 
	{
		for (int i=0; i < MAX_ARRAY_NPC_AUTO; i++)
		{
			m_ArrayTimeObjectLag[i] = 0;
		}
	};
	void			ClearArrayNpcNeast()
	{
		for (int i=0; i < MAX_ARRAY_NPC_AUTO; i++)
		{
			m_ArrayNpcNeast[i] = 0;
		}
	};
	void			ClearArrayObjectNeast()
	{
		for (int i=0; i < MAX_ARRAY_NPC_AUTO; i++)
		{
			m_ArrayObjectNeast[i] = 0;
		}
	};
	void			ClearArrayStateSkill()
	{
		for (int i=0; i < MAX_ARRAY_NPC_AUTO; i++)
		{
			m_ArrayStateSkill[i] = 0;
		}
	};
    int			FindFreeInvitePlayer()
	{
		for (int i=0; i < MAX_AUTO_SIZE; i++)
		{
			if (m_nArrayInvitePlayer[i] <= 0)
				return i;
		}		
		return -1;
	};	
	
	BOOL			FindSameInvitePlayer(int uID)
	{
		for (int i=0; i < MAX_AUTO_SIZE; i++)
		{
			if (m_nArrayInvitePlayer[i] == uID)
				return TRUE;
		}		
		return FALSE;
	};
    void			ClearArrayInvitePlayer(){memset(m_nArrayInvitePlayer, 0, sizeof(m_nArrayInvitePlayer));};
    void			ClearArrayTimeInvitePlayer(){memset(m_nArrayTimeInvitePlayer, 0, sizeof(m_nArrayTimeInvitePlayer));};
	void	        ClearArrayNpcOverLook(){memset(m_nArrayNpcOverLook, 0, sizeof(m_nArrayNpcOverLook));};
	void			ClearArrayObjectOverLook(){memset(m_nArrayObjectOverLook, 0, sizeof(m_nArrayObjectOverLook));};
	void			ClearArrayTimeNpcOverLook(){memset(m_nArrayTimeNpcOverLook, 0, sizeof(m_nArrayTimeNpcOverLook));};
	void			ClearArrayTimeObjectOverLook(){memset(m_nArrayTimeObjectOverLook, 0, sizeof(m_nArrayTimeObjectOverLook));};
	void			PlayerFollowActack(int i);
	void			AutoCheckUseItem();
	void			PlayerEatAItem();
	void			AutoReturn();
	BOOL			AutoAddNpc2Array(int nRelation);
	int				FindNearNpc2Array(int nRelation);
	BOOL			FindObjectNearPlayer();
	BOOL			AutoBuffSkillState();
	int				FindNearObject2Array();
	void			PlayerFollowObject(int nObject);
	void			PlayerUseItem(int type);
	void			AutoParty();
	void			PlayerFilterEquip();
	BOOL			IsRAPEquip(BYTE btDetail);
	BOOL			IsEquipSatisfyCondition(int nIdx);
	void 			SetRepairItem(BYTE btFlag) { m_btDurabilityItem = btFlag; };
	BYTE			GetRepairItem() { return m_btDurabilityItem; };
	void 			SetAutoUseTLL(BYTE btFlag) { m_AutoUseTTL = btFlag; };
	BYTE			GetAutoUseTLL() { return m_AutoUseTTL; };
	BOOL			IsNotValidNpc(int nIndex);
	BOOL			IsNotValidObject(int nObject);	
	void			PlayerSwitchAura();
	BOOL			PlayerMoveMps();
	int				GetMoveMpsCount();
#endif
};

#ifdef TOOLVERSION
extern CORE_API KPlayer	Player[MAX_PLAYER];
#else
extern KPlayer	Player[MAX_PLAYER];
#endif
#endif //KPlayerH

