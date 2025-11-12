#ifndef KPROTOCOL_H
#define KPROTOCOL_H

#ifndef __linux
#ifdef _STANDALONE
#include "GameDataDef.h"
#else
#include "../Sources/Core/src/GameDataDef.h"
#endif
#else
#include "GameDataDef.h"
#include <string.h>
#endif

#include "KProtocolDef.h"
#include "KRelayProtocol.h"

#pragma pack(push, enter_protocol)
#pragma	pack(1)

#define	PROTOCOL_MSG_TYPE	BYTE
#define PROTOCOL_MSG_SIZE	(sizeof(PROTOCOL_MSG_TYPE))
#define	MAX_PROTOCOL_NUM	250



typedef struct
{
	BYTE	ProtocolType;
	BYTE	WalkSpeed;
	BYTE	RunSpeed;
	BYTE	AttackSpeed;
	BYTE	CastSpeed;
	BYTE	HelmType;
	BYTE	ArmorType;
	BYTE	WeaponType;
	BYTE	HorseType;
	int		MaskType; // MASK
	BYTE	FifongType;
	BYTE	FifongState; //phiphong
	BYTE	RankID;
	DWORD	ID;
	BYTE	m_btSomeFlag;	// 0x01 PKFlag 0x02 FightModeFlag 0x04 SleepModeFlag
//	BYTE	m_btPKFlag;
//	BYTE	m_btFightMode;
//	BYTE	m_SleepMode;
//	char	Name[32];
	
	char	m_TongName[32];
	char	m_TongRank[32];
	BYTE    m_Task139;
	BYTE    m_Task485;

	DWORD	m_dwTime ;
	char    szName[32];
	BYTE 	ImagePlayer;	// chan dung
	BYTE	PKFlag;
} PLAYER_SYNC;



typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	BYTE	WalkSpeed;			// 是否只要传一份就够了（只改变一个或是同时改变）
	BYTE	RunSpeed;
	BYTE	AttackSpeed;
	BYTE	CastSpeed;			// 是否考虑不改变施法速度，或施法速度等于攻击速度
	BYTE	HelmType;
	BYTE	ArmorType;
	BYTE	WeaponType;
	BYTE	HorseType;
	BYTE	RankID;
	BYTE	m_btSomeFlag;	// 0x01 PKFlag 0x02 FightModeFlag 0x04 SleepModeFlag 0x08 TongOpenFlag
    BYTE	m_PKValue; 
	BYTE	m_Task139;
	BYTE	m_Task485;
	int		MaskType; // MASK
	BYTE	FifongType;
	BYTE	FifongState; //phiphong
//	BYTE	FightMode;
//	BYTE	m_btPKFlag;
//	BYTE	m_btSleepMode;
	BYTE 	ImagePlayer; // chan dung
	BYTE	PKFlag;
} PLAYER_NORMAL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	Camp;				// 阵营
	BYTE	CurrentCamp;		// 当前阵营
	BYTE	m_bySeries;			// 五行系
	int 	CurrentLife;		// 生命百分比
	int 	CurrentLifeMax;
//	int		Dir;
	BYTE	m_btMenuState;		// 组队、交易等状态
	BYTE	m_Doing;			// 行为
	BYTE	m_btKind;			// npc类型
	DWORD	MapX;				// 位置信息
	DWORD	MapY;				// 位置信息
	DWORD	ID;					// Npc的唯一ID
	int		NpcSettingIdx;		// 客户端用于加载玩家资源及基础数值设定
	BYTE    m_IsBoss; 
	char	m_szName[32];		// 名字
} NPC_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	int				m_nId;
	DWORD	m_dwID;
	short			m_Genre;
	short			m_Detail;
	short			m_Particur;
	BYTE			m_Series;
	BYTE			m_BindItem;
	short			m_Level;
	int				m_nKind;
	KTime			m_Time;
	BYTE			m_nFiFongArmor;
	int			iShopCost;
	int			iNgoaiTrang;
} S2C_ITEM_REFRESH;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	DWORD	MapX;
	DWORD	MapY;
	BYTE	Camp;
	int 	CurrentLife;
	int 	CurrentLifeMax;
//	int     RegionID;   // moi them thu 24.06
	BYTE	Doing;
	BYTE	State;
} NPC_NORMAL_SYNC;


typedef struct
{ 
	BYTE	ProtocolType;
	BYTE	nType;
	int		nValue;
} S2C_PLAYER_POINT_SYNC;

typedef struct
{ 
	BYTE ProtocolType;
	char	Value[32];
	int		Value1;
	char	Value2[64];
} S2C_TIME_BOX;



typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	DWORD	MapX;
	DWORD	MapY;
	BYTE	Doing;
} NPC_POS_SYNC;



typedef struct	
{
	BYTE	ProtocolType;
	int		nbutton;
} CP_TOADO;


typedef struct
{
	BYTE	ProtocolType;
	//int		m_nRegionID;
	DWORD	m_dwNpcID;
	DWORD	m_dwMapX;
	DWORD	m_dwMapY;
	WORD	m_wOffX;
	WORD	m_wOffY;
	BYTE	m_byDoing;
	BYTE	m_btCamp;	
} NPC_PLAYER_TYPE_NORMAL_SYNC;
                 
typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
} NPC_REMOVE_SYNC, NPC_SIT_SYNC, NPC_DEATH_SYNC, NPC_REQUEST_COMMAND, NPC_REQUEST_FAIL;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int		nMpsX;
	int		nMpsY;
} NPC_WALK_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	BYTE	Type;
} NPC_REVIVE_SYNC;




typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	char    m_TongName[32]; 
	char	m_TongRank[32];		
} NPC_UPDATE_TONG;


typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	DWORD   m_dwTime; 
	char	szName[32];		
} NPC_UPDATE_SHOP;






typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int		nMpsX;
	int		nMpsY;
} NPC_JUMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int		nMpsX;
	int		nMpsY;
} NPC_RUN_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int		nFrames;
	int		nX;
	int		nY;
} NPC_HURT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	BYTE	Camp;
} NPC_CHGCURCAMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	BYTE	Camp;
} NPC_CHGCAMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int		nSkillID;
	int		nSkillLevel;
	int		nMpsX;
	int		nMpsY;
} NPC_SKILL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	int		nSkillID;
	int		nMpsX;
	int		nMpsY;
	int		nNpcX;
	int		nNpcY;
	int     nNpcRadius;
} NPC_SKILL_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		nMpsX;
	int		nMpsY;
} NPC_WALK_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
//	BYTE	ReviveType;
} NPC_REVIVE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		nMpsX;
	int		nMpsY;
} NPC_RUN_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		nShopIndex;
	BYTE    nSlect;
} SALE_BOX_SYNC;

typedef struct 
{
	BYTE	ProtocolType;
	int		nNpcId;
} PLAYER_DIALOG_NPC_COMMAND; //主角与nNpcId对话的请求

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	int		m_nExp;				// 当前经验
} PLAYER_EXP_SYNC;				// 玩家同步经验

			

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
} PLAYER_APPLY_CREATE_TEAM;		// 客户端玩家创建队伍，向服务器发请求

struct PLAYER_SEND_CREATE_TEAM_SUCCESS
{
	BYTE	ProtocolType;		// 协议名称
	DWORD	nTeamServerID;		// 队伍在服务器上的唯一标识
	PLAYER_SEND_CREATE_TEAM_SUCCESS() {nTeamServerID = -1;}
};	// 服务器通知玩家队伍创建成功

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	BYTE	m_btErrorID;		// 队伍创建不成功原因：0 同名 1 玩家本身已经属于某一支队伍 3 当前处于不能组队状态
} PLAYER_SEND_CREATE_TEAM_FALSE;// 服务器通知客户端队伍创建不成功

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	DWORD	m_dwTarNpcID;		// 查询目标 npc id
} PLAYER_APPLY_TEAM_INFO;		// 客户端向服务器申请查询某个npc的组队情况

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
} PLAYER_APPLY_TEAM_INFO_FALSE;	// 服务器告知客户端申请查询某个npc的组队情况失败

typedef struct PLAYER_SEND_TEAM_INFO_DATA
{
	BYTE	ProtocolType;		// 协议名称
	int		m_nCaptain;			// 队长 npc id
	int		m_nMember[MAX_TEAM_MEMBER];	// 所有队员 npc id
	DWORD	nTeamServerID;		// 队伍在服务器上的唯一标识
	PLAYER_SEND_TEAM_INFO_DATA() {nTeamServerID = -1;};
} PLAYER_SEND_TEAM_INFO;		// 服务器向客户端发送某个队伍的信息数据

typedef struct PLAYER_SEND_SELF_TEAM_INFO_DATA
{
	BYTE	ProtocolType;							// 协议名称
	BYTE	m_btState;								// 队伍状态
	DWORD	m_dwNpcID[MAX_TEAM_MEMBER + 1];			// 每名成员的npc id （队长放在第一位）
	char	m_szNpcName[MAX_TEAM_MEMBER + 1][32];	// 每名成员的名字（队长放在第一位）
	DWORD	nTeamServerID;							// 队伍在服务器上的唯一标识
	DWORD	m_dwLeadExp;							// 玩家的统率力经验
	BYTE	m_btLevel[MAX_TEAM_MEMBER + 1];			// 每名成员的等级（队长放在第一位）
	PLAYER_SEND_SELF_TEAM_INFO_DATA() {memset(m_szNpcName, 0, 32 * (MAX_TEAM_MEMBER + 1)); nTeamServerID = -1;};
} PLAYER_SEND_SELF_TEAM_INFO;						// 服务器向客户端发送客户端自身队伍的信息数据

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	BYTE	m_btOpenClose;		// 打开或关闭
} PLAYER_TEAM_OPEN_CLOSE;		// 队伍队长向服务器申请开放、关闭队伍是否允许接收成员状态

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	DWORD	m_dwTarNpcID;		// 目标队伍队长npc id 或者 申请人 npc id
} PLAYER_APPLY_ADD_TEAM;		// 玩家向服务器申请加入某个队伍或者服务器向某个队长转发某个玩家的加入申请

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	DWORD	m_dwNpcID;			// 被接受入队伍的npc id
} PLAYER_ACCEPT_TEAM_MEMBER;	// 玩家通知服务器接受某个玩家入队伍

typedef struct PLAYER_TEAM_ADD_MEMBER_DATA
{
	BYTE	ProtocolType;		// 协议名称
	BYTE	m_btLevel;			// 加入者等级
	DWORD	m_dwNpcID;			// 加入者npc id
	char	m_szName[32];		// 加入者姓名
	PLAYER_TEAM_ADD_MEMBER_DATA() {memset(m_szName, 0, 32);};
} PLAYER_TEAM_ADD_MEMBER;		// 服务器通知队伍中的各个玩家有新成员加入

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	BYTE	m_btState;
	BYTE	m_btFlag;		// 打开或关闭
} PLAYER_TEAM_CHANGE_STATE;		// 队伍队长向服务器申请开放、关闭队伍是否允许接收成员状态


typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	BOOL	bMySelf;
} PLAYER_APPLY_LEAVE_TEAM;		// 客户端玩家申请离队

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	DWORD	m_dwNpcID;			// 离队npc id
} PLAYER_LEAVE_TEAM;			// 服务器通知各队员某人离队

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	DWORD	m_dwNpcID;			// 离队npc id
} PLAYER_TEAM_KICK_MEMBER;		// 队长踢除某个队员


typedef struct
{
	BYTE	ProtocolType;
	DWORD	m_dwNpcID;
} PLAYER_OPEN_BUY_SHOP;


typedef struct
{
	BYTE	ProtocolType;
} PLAYER_CLOSE_BUY_SHOP;


typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	DWORD	m_dwNpcID;			// 目标npc id
} PLAYER_APPLY_TEAM_CHANGE_CAPTAIN;// 队长向服务器申请把自己的队长身份交给别的队员

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	DWORD	m_dwCaptainID;		// 新队长npc id
	DWORD	m_dwMemberID;		// 新队员npc id
} PLAYER_TEAM_CHANGE_CAPTAIN;	// 服务器通知各队员更换队长

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
} PLAYER_APPLY_TEAM_DISMISS;	// 向服务器申请解散队伍

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	BYTE	m_btPKFlag;			// pk 开关
} PLAYER_SET_PK;				// 向服务器申请打开、关闭PK

typedef struct
{
	BYTE	ProtocolType;			// 协议名称
	BYTE	m_btCamp;				// 新阵营
	BYTE	m_btCurFaction;			// 当前门派
	BYTE	m_btFirstFaction;		// 首次加入门派
	int		m_nAddTimes;			// 加入门派次数
} PLAYER_FACTION_DATA;				// 服务器发给客户端门派信息
// MASK
typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
} MASK_SYNC;
// END

typedef struct
{
	BYTE	ProtocolType;			// 协议名称
} PLAYER_LEAVE_FACTION;				// 服务器通知玩家离开门派

typedef struct
{
	BYTE	ProtocolType;			// 协议名称
	BYTE	m_btCurFactionID;		// 当前门派id
	BYTE	m_btLevel;				// 技能开放等级
} PLAYER_FACTION_SKILL_LEVEL;		// 服务器通知玩家开放当前门派技能到某个等级

typedef struct
{
	BYTE	ProtocolType;			// 协议名称
} PLAYER_APPLY_FACTION_DATA;		// 客户端申请更新门派数据

typedef struct PLAYER_SEND_CHAT_DATA_COMMAND
{
	BYTE	ProtocolType;		// 协议名称
	WORD	m_wLength;
	BYTE	m_btCurChannel;		// 当前聊天频道
	BYTE	m_btType;			// MSG_G_CHAT 或 MSG_G_CMD 或……
	BYTE	m_btChatPrefixLen;	// 格式控制字符长度
	WORD	m_wSentenceLen;		// 聊天语句长度
	DWORD	m_dwTargetID;		// 聊天对象 id
	int		m_nTargetIdx;		// 聊天对象在服务器端的 idx
	char	m_szSentence[MAX_SENTENCE_LENGTH + CHAT_MSG_PREFIX_MAX_LEN];	// 聊天语句内容
	PLAYER_SEND_CHAT_DATA_COMMAND() {memset(m_szSentence, 0, sizeof(m_szSentence));};
} PLAYER_SEND_CHAT_COMMAND;		// 客户端聊天内容发送给服务器

typedef struct PLAYER_SEND_CHAT_DATA_SYNC
{
	BYTE	ProtocolType;		// 协议名称
	WORD	m_wLength;
	BYTE	m_btCurChannel;		// 当前聊天状态
	BYTE	m_btNameLen;		// 名字长度
	BYTE	m_btChatPrefixLen;	// 控制字符长度
	WORD	m_wSentenceLen;		// 聊天语句长度
	DWORD	m_dwSourceID;		// 
	char	m_szSentence[32 + CHAT_MSG_PREFIX_MAX_LEN + MAX_SENTENCE_LENGTH];	// 聊天语句内容
	PLAYER_SEND_CHAT_DATA_SYNC() { memset(m_szSentence, 0, sizeof(m_szSentence)); };
} PLAYER_SEND_CHAT_SYNC;		// 客户端聊天内容发送给服务器

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btState;
	int		m_nID;
	int		m_nDataID;
	int		m_nXpos;
	int		m_nYpos;
	int		m_nMoneyNum;
	int		m_nItemID;
	BYTE	m_btDir;
	BYTE	m_btItemWidth;
	WORD	m_wCurFrame;
	BYTE	m_btItemHeight;
	BYTE	m_btColorID;
	int		m_nGenre;
	int		m_nDetailType;
	BYTE	m_btFlag;
	int		m_dwNpcId;
	char	m_szName[OBJ_NAME_LENGTH];
} OBJ_ADD_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btState;
	int		m_nID;
} OBJ_SYNC_STATE;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btDir;
	int		m_nID;
} OBJ_SYNC_DIR;

typedef struct
{
	BYTE	ProtocolType;
	int		m_nID;
	BYTE	m_btSoundFlag;
} OBJ_SYNC_REMOVE;

typedef struct
{
	BYTE	ProtocolType;
	int		m_nID;
	int		m_nTarX;
	int		m_nTarY;
} OBJ_SYNC_TRAP_ACT;

typedef struct
{
	BYTE	ProtocolType;
	int		m_nID;
} OBJ_CLIENT_SYNC_ADD;

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	DWORD	m_dwLeadExp;		// 统率力经验值
} PLAYER_LEAD_EXP_SYNC;			// 同步统率力经验值

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	BYTE	m_btLevel;			// 当前等级
	int		m_nExp;				// 当前经验
	int		m_nAttributePoint;	// 剩余属性点
	int		m_nSkillPoint;		// 剩余技能点
	int		m_nBaseLifeMax;		// 当前最大生命值
	int		m_nBaseStaminaMax;	// 当前最大体力值
	int		m_nBaseManaMax;		// 当前最大内力值
} PLAYER_LEVEL_UP_SYNC;			// 玩家升级

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	BYTE	m_btLevel;			// 当前等级
	DWORD	m_dwTeammateID;		// 队友 npc id
} PLAYER_TEAMMATE_LEVEL_SYNC;	// 玩家升级的时候通知队友

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	BYTE	m_btAttribute;		// 属性(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
	int		m_nAddNo;			// 加的点数
} PLAYER_ADD_BASE_ATTRIBUTE_COMMAND;	// 玩家添加基本属性点

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	int		m_nSkillID;			// 技能id
	int		m_nAddPoint;		// 要加的点数
} PLAYER_ADD_SKILL_POINT_COMMAND;// 玩家申请增加某个技能的点数

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	BYTE	m_btAttribute;		// 属性(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
	int		m_nBasePoint;		// 基本点数
	int		m_nCurPoint;		// 当前点数
	int		m_nLeavePoint;		// 剩余未分配属性点
} PLAYER_ATTRIBUTE_SYNC;		// 玩家同步属性点

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	int		m_nSkillID;			// 技能id
	int		m_nSkillLevel;		// 技能等级
	int		m_nLeavePoint;		// 剩余未分配技能点
} PLAYER_SKILL_LEVEL_SYNC;		// 玩家同步技能点

//typedef struct
//{
//	BYTE	ProtocolType;		// 协议名称
//	int		m_nItemID;			// 物品id
//	int		m_nSourcePos;		// 来源位置
//	int		m_nTargetPos;		// 目的位置
//} PLAYER_EQUIP_ITEM_COMMAND;	// 玩家鼠标右键点击使用物品(装备)

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	BYTE	m_btPlace;			// 药品位置
	BYTE	m_btX;				// 药品位置
	BYTE	m_btY;				// 药品位置
	int		m_nItemID;			// 物品id
} PLAYER_EAT_ITEM_COMMAND;		// 玩家鼠标右键点击使用物品(吃药)

typedef struct
{
	BYTE	ProtocolType;		// 协议名称
	int		m_nObjID;			// 鼠标点击的obj的id
	BYTE	m_btPosType;		// 位置类型
	BYTE	m_btPosX;			// 坐标 x
	BYTE	m_btPosY;			// 坐标 y
} PLAYER_PICKUP_ITEM_COMMAND;		// 玩家获得物品（鼠标点击地图上的obj）

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_ID;				// 物品的ID
	BYTE			m_Genre;			// 物品的类型
	int				m_Detail;			// 物品的类别
	BYTE			m_Particur;			// 物品的详细类别
	BYTE			m_Series;			// 物品的五行
	BYTE			m_Level;			// 物品的等级
	BYTE			m_btPlace;			// 坐标
	BYTE			m_btX;				// 坐标
	BYTE			m_btY;				// 坐标
	BYTE			m_Luck;				// MF
	int 			m_MagicLevel[6];	// 生成参数
	int 			m_Version;			// 装备版本
	int 			m_nFifong;			// 装备版本
	int 			m_nBindItem;			// 装备版本
	WORD			m_Durability;		// 耐久度
	UINT			m_RandomSeed;		// 随机种子
	KTime			m_Time;
	int			iShopCost;
	int			iNgoaiTrang;
} ITEM_SYNC;







typedef struct
{
	BYTE			ProtocolType;
	int				m_ID;				
	BYTE			m_Genre;		
	int				m_Detail;			
	BYTE			m_Particur;		
	BYTE			m_Series;		
	BYTE			m_Level;		
	BYTE			m_btPlace;			
	BYTE			m_btX;		
	BYTE			m_btY;			
	BYTE			m_Luck;				
	int 			m_MagicLevel[6];	
	int 			m_Version;		
	WORD			m_Durability;		
	UINT			m_RandomSeed;
	int             m_nPriceShop;
	int			iNgoaiTrang;

} ITEM_BUY_SHOP_SYNC;


typedef struct
{
	BYTE			ProtocolType;	
	int	m_ID;
	KTime 	m_Time;
}S2C_SYNC_ITEM_PARAM;



typedef struct
{
	BYTE			ProtocolType;		
	char            m_szName[32];

	int             m_nTime;

} BOX_BUY_SHOP_SYNC;









typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_ID;				// 物品的ID
} ITEM_REMOVE_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_ID;				// 物品的ID
	int				m_Price;
} PLAYER_SHOP_PRICE_COMMAND;



typedef struct
{
	BYTE			ProtocolType;	
	int				m_ID;			
} PLAYER_SHOP_BUY_ITEM_COMMAND;




typedef struct
{
	BYTE			ProtocolType;
} PLAYER_SHOP_OPEN_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	nItemGenre;
	BYTE	nDetailType;
	BYTE	nLevel;
	BYTE	num;
} CLIENT_OPEN_SHOP;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_ID;				// 物品的ID
} PLAYER_BLOCK_ITEM_COMMAND;


typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	char			m_szString[32];				// 物品的ID
} PLAYER_STRING_BOX;







typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	char			m_szString[32];				// 物品的ID
} PLAYER_LOGIN_PW_STORE_BOX;



typedef struct
{
	BYTE			ProtocolType;	
	char			m_szString[32];	
	char			m_szStringNew[32];	
} PLAYER_PW_STORE_BOX;




typedef struct
{
	BYTE			ProtocolType;	
	char			m_szString[32];	
} PLAYER_NEW_PW_STORE_BOX;



typedef struct
{
	BYTE			ProtocolType;	
} PLAYER_BLOCK_STORE_BOX;
typedef struct
{
	BYTE			ProtocolType;	
} PLAYER_UNBLOCK_STORE_BOX;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	char			m_szString[32];				// 物品的ID
} PLAYER_SHOP_NAME;


typedef struct
{
	BYTE			ProtocolType;
} PLAYER_CHECK_ITEM_BOX;


typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_nSlect;
} PLAYER_SLECT_CHECK_BOX;



typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_nSlect;
} PLAYER_BUTTON_PG_BOX;



typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_ID;				// 物品的ID
    int             m_Number;
} PLAYER_STASK_ITEM_COMMAND;


typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_ID;				// 物品的ID
} PLAYER_UNBLOCK_ITEM_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_ID;				// 物品的ID
    int			m_Vesion;
} PLAYER_BLOCK_ITEM_RETURN_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_ID;				// 物品的ID
    int			m_Vesion;
} PLAYER_UNBLOCK_ITEM_RETURN_COMMAND;



typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_ID;				// 物品的ID
    int			    m_Price;
} PLAYER_SHOP_PRICE_RETURN_COMMAND;




typedef struct
{
	BYTE			ProtocolType;		
	DWORD				m_dwTime;			
    char			szName[32];
} PLAYER_SHOP_OPEN_RETURN_COMMAND;





typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_ID;				// 物品的ID
    int			    m_Price;
} PLAYER_UPDATE_SHOP_PRICE_COMMAND;




typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_ID;				// 物品的ID
} PLAYER_UNBLOCK_ITEM_NOW_COMMAND;


typedef struct
{
	BYTE			ProtocolType;	
	int				m_ID;	
	int             m_Vesion;
} PLAYER_ITEM_UPDATE_VERSION;








typedef struct
{
	BYTE			ProtocolType;	
	int				m_ID;	
	char            m_Title[32];
} PLAYER_OPEN_STRING_BOX;


typedef struct
{
	BYTE			ProtocolType;
	char            m_TitleBox[32];
	char            m_TitleTex[128];
} PLAYER_OPEN_CHECK_ITEM_BOX;



typedef struct
{
	BYTE			ProtocolType;
	char            m_szId[3];
} PLAYER_OPEN_SLECT_CHECK_BOX;









typedef struct
{
	BYTE			ProtocolType;
	BYTE            m_bBlock;
} PLAYER_BLOCK_BOX;








typedef struct
{
	BYTE			ProtocolType;
} PLAYER_OPEN_PG_BOX_ITEM;





typedef struct
{
	BYTE			ProtocolType;
} PLAYER_OPEN_NEW_PW_BOX_ITEM;












typedef struct
{
	BYTE			m_Phe;
	BYTE			m_Soluong;
	BYTE			m_ThoiGian;
	WORD			m_NguoiChoi;
	WORD			m_TNguoiChoi;
	WORD			m_NPC;
	WORD			m_TNPC;
	WORD			m_LienTramHT;
	WORD			m_TLienTramHT;
	WORD			m_BauVat;
	WORD			m_TBauVat;
	WORD			m_TichLuy;
	WORD			m_TuVong;
	WORD			m_LienTram;

	char			m_szTop1Name[32];
	BYTE			m_Top1Phe;
	WORD			m_Top1TichLuy;
	WORD			m_Top1NguoiChoi;
	WORD			m_Top1NPC;
	WORD			m_Top1TuVong;
	WORD			m_Top1LienTram;
	WORD			m_Top1BaoVat;

	char			m_szTop2Name[32];
	BYTE			m_Top2Phe;
	WORD			m_Top2TichLuy;
	WORD			m_Top2NguoiChoi;
	WORD			m_Top2NPC;
	WORD			m_Top2TuVong;
	WORD			m_Top2LienTram;
	WORD			m_Top2BaoVat;

	char			m_szTop3Name[32];
	BYTE			m_Top3Phe;
	WORD			m_Top3TichLuy;
	WORD			m_Top3NguoiChoi;
	WORD			m_Top3NPC;
	WORD			m_Top3TuVong;
	WORD			m_Top3LienTram;
	WORD			m_Top3BaoVat;

	char			m_szTop4Name[32];
	BYTE			m_Top4Phe;
	WORD			m_Top4TichLuy;
	WORD			m_Top4NguoiChoi;
	WORD			m_Top4NPC;
	WORD			m_Top4TuVong;
	WORD			m_Top4LienTram;
	WORD			m_Top4BaoVat;

	char			m_szTop5Name[32];
	BYTE			m_Top5Phe;
	WORD			m_Top5TichLuy;
	WORD			m_Top5NguoiChoi;
	WORD			m_Top5NPC;
	WORD			m_Top5TuVong;
	WORD			m_Top5LienTram;
	WORD			m_Top5BaoVat;

	char			m_szTop6Name[32];
	BYTE			m_Top6Phe;
	WORD			m_Top6TichLuy;
	WORD			m_Top6NguoiChoi;
	WORD			m_Top6NPC;
	WORD			m_Top6TuVong;
	WORD			m_Top6LienTram;
	WORD			m_Top6BaoVat;

	char			m_szTop7Name[32];
	BYTE			m_Top7Phe;
	WORD			m_Top7TichLuy;
	WORD			m_Top7NguoiChoi;
	WORD			m_Top7NPC;
	WORD			m_Top7TuVong;
	WORD			m_Top7LienTram;
	WORD			m_Top7BaoVat;

	char			m_szTop8Name[32];
	BYTE			m_Top8Phe;
	WORD			m_Top8TichLuy;
	WORD			m_Top8NguoiChoi;
	WORD			m_Top8NPC;
	WORD			m_Top8TuVong;
	WORD			m_Top8LienTram;
	WORD			m_Top8BaoVat;

	char			m_szTop9Name[32];
	BYTE			m_Top9Phe;
	WORD			m_Top9TichLuy;
	WORD			m_Top9NguoiChoi;
	WORD			m_Top9NPC;
	WORD			m_Top9TuVong;
	WORD			m_Top9LienTram;
	WORD			m_Top9BaoVat;

	char			m_szTop10Name[32];
	BYTE			m_Top10Phe;
	WORD			m_Top10TichLuy;
	WORD			m_Top10NguoiChoi;
	WORD			m_Top10NPC;
	WORD			m_Top10TuVong;
	WORD			m_Top10LienTram;
	WORD			m_Top10BaoVat;

} PLAYER_DATA_TOP_TK_NEW;





typedef struct
{
	BYTE			ProtocolType;
    PLAYER_DATA_TOP_TK_NEW			m_DataTop;

} PLAYER_SEND_TOP_TK_NEW;
















typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	BYTE			m_BuyIdx;			// 买第几个东西
	BYTE			m_Place;			// 放在身上哪个地方
	BYTE			m_X;				// 坐标X
	BYTE			m_Y;				// 坐标Y
} PLAYER_BUY_ITEM_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_nMoney1;			// 装备栏
	int				m_nMoney2;			// 贮物箱
	int				m_nMoney3;			// 交易栏
} PLAYER_MONEY_SYNC;					// 服务器通知客户端钱的数量

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	BYTE			m_btDownPos;
	BYTE			m_btDownX;
	BYTE			m_btDownY;
	BYTE			m_btUpPos;
	BYTE			m_btUpX;
	BYTE			m_btUpY;
} PLAYER_MOVE_ITEM_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	BYTE			m_btDownPos;
	BYTE			m_btDownX;
	BYTE			m_btDownY;
	BYTE			m_btUpPos;
	BYTE			m_btUpX;
	BYTE			m_btUpY;
} PLAYER_MOVE_ITEM_SYNC;

// s2c_ItemAutoMove
typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	BYTE			m_btSrcPos;
	BYTE			m_btSrcX;
	BYTE			m_btSrcY;
	BYTE			m_btDestPos;
	BYTE			m_btDestX;
	BYTE			m_btDestY;
} ITEM_AUTO_MOVE_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
} PLAYER_THROW_AWAY_ITEM_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_shLife;
	DWORD			m_shStamina;
	DWORD			m_shMana;
	DWORD			m_shAngry;

	int				m_shLifeMax;
	int				m_shStaminaMax;
	int				m_shManaMax;
	BYTE			m_btTeamData;
} CURPLAYER_NORMAL_SYNC;




typedef struct
{
	BYTE			ProtocolType;
	int			    m_Task100;
	int			    m_Task162;
	int			    m_Task191;
	int			    m_Task192;
	int			    m_Task193;
	int			    m_Task140;
	int			    m_Task139;
	int			    m_Task485;
    int			    m_Task128;
	int			    m_Task99;
	int			    m_Task15;
	int			    m_Task124;
} CURPLAYER_HEIGHT_SYNC;




typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	// npc部分
	DWORD			m_dwID;				// Npc的ID
	BYTE			m_btLevel;			// Npc的等级
	BYTE			m_btSex;			// 性别
	BYTE			m_btKind;			// Npc的类型
	BYTE			m_btSeries;			// Npc的五行系
	WORD			m_wLifeMax;			// Npc的最大生命
	WORD			m_wStaminaMax;		// Npc的最大体力
	WORD			m_wManaMax;			// Npc的最大内力
	int				m_HeadImage;
	// player 部分
	WORD			m_wAttributePoint;	// 未分配属性点
	WORD			m_wSkillPoint;		// 未分配技能点
	WORD			m_wStrength;		// 玩家的基本力量（决定基本伤害）
	WORD			m_wDexterity;		// 玩家的基本敏捷（决定命中、体力）
	WORD			m_wVitality;		// 玩家的基本活力（决定生命、体力）
	WORD			m_wEngergy;			// 玩家的基本精力（决定内力）
	WORD			m_wLucky;			// 玩家的基本幸运值
	int				m_nExp;				// 当前经验值(当前等级在npc身上)
	DWORD			m_dwLeadExp;		// 统率力经验值

	// 门派
	BYTE			m_btCurFaction;		// 当前门派
	BYTE			m_btFirstFaction;	// 第一次加入的是哪个门派
	int				m_nFactionAddTimes;	// 加入各种门派的总次数
	
	// 排名
	WORD			m_wWorldStat;		// 世界排名
	WORD			m_wSectStat;		// 门派排名

	// 钱
	int				m_nMoney1;
	int				m_nMoney2;
	int 			m_btImagePlayer;	// chan dung
} CURPLAYER_SYNC;  // chinh lai keu du lieu m_wLifeMax 

#define MAX_SCIRPTACTION_BUFFERNUM 1024

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wProtocolLong;
	BYTE	m_nOperateType;				//操作类型
	BYTE	m_bUIId, m_bOptionNum, m_bParam1, m_bParam2, m_Select;// m_bParam1,主信息是数字标识还是字符串标识, m_bParam2,是否是与服务器交互的选择界面
	int		m_nParam;
	int		m_nBufferLen;
	BOOL	m_bSelect;
	char	m_pContent[MAX_SCIRPTACTION_BUFFERNUM];				//带控制符
} PLAYER_SCRIPTACTION_SYNC;

typedef struct
{
	WORD	SkillId;
	BYTE	SkillLevel;
} SKILL_SEND_ALL_SYNC_DATA;

typedef struct
{
	BYTE						ProtocolType;
	WORD						m_wProtocolLong;
	SKILL_SEND_ALL_SYNC_DATA	m_sAllSkill[80];
} SKILL_SEND_ALL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	WeatherID;
} SYNC_WEATHER;

typedef struct defWORLD_SYNC
{
	BYTE	ProtocolType;
	int		SubWorld;
	int		Region;
	BYTE	Weather;
	DWORD	Frame;
	char    TongName[32];
	char    TongNameBC[32];
	int     TongT;
	int     TongVG;
	BYTE    CheckTong;
} WORLD_SYNC;




typedef struct 
{
	BYTE	ProtocolType;
	int		nSelectIndex;
}PLAYER_SELECTUI_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwTakeChannel;	// 订阅频道
} CHAT_SET_CHANNEL_COMMAND;				// 设定订阅频道

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	WORD			m_wLength;
	DWORD			m_dwTargetNpcID;	// 目标 npc id
	char			m_szInfo[MAX_SENTENCE_LENGTH];// 给对方的话
} CHAT_APPLY_ADD_FRIEND_COMMAND;		// 聊天添加好友

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_nSrcPlayerIdx;	// 来源 player idx
	char			m_szSourceName[32];	// 来源玩家名字
	char			m_szInfo[MAX_SENTENCE_LENGTH];// 对方给的话
} CHAT_APPLY_ADD_FRIEND_SYNC;			// 聊天添加好友

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_nTargetPlayerIdx;	// 被接受player idx
} CHAT_ADD_FRIEND_COMMAND;				// 添加某玩家为聊天好友

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_nTargetPlayerIdx;	// 被拒绝player idx
} CHAT_REFUSE_FRIEND_COMMAND;			// 拒绝添加某玩家为聊天好友

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 新添加好友的 id
	int				m_nIdx;				// 新添加好友在 player 数组中的位置
	char			m_szName[32];		// 新添加好友的名字
} CHAT_ADD_FRIEND_SYNC;					// 通知客户端成功添加一个聊天好友

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	WORD			m_wLength;
	char			m_szName[32];		// 拒绝者名字
} CHAT_REFUSE_FRIEND_SYNC;				// 通知客户端添加聊天好友的申请被拒绝

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_nTargetPlayerIdx;	// 出错 player idx (一般可能是此player下线或者换服务器了)
} CHAT_ADD_FRIEND_FAIL_SYNC;			// 通知客户端添加聊天好友失败

typedef struct
{
	BYTE			ProtocolType;	// c2s_viewequip
	DWORD			m_dwNpcID;
} VIEW_EQUIP_COMMAND;

//此结构已经被tagDBSelPlayer结构替换
/*typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_nSelect;			// 
} DB_PLAYERSELECT_COMMAND;*/

/*
 * { Add by liupeng 2003.05.10
 *
 * #pragma pack( push, 1 )
*/

/*
 * Nonstandard extension used : zero-sized array in struct/union
 */
#pragma warning(disable: 4200)

#define KSG_PASSWORD_MAX_SIZE   64

typedef struct tagKSG_PASSWORD
{
    char szPassword[KSG_PASSWORD_MAX_SIZE];    // 现在采用MD5的字符串，由于是32个字符，加上末尾'\0'，需要至少33个空间，因此使用64
} KSG_PASSWORD;

#define _NAME_LEN	32

struct tagProtoHeader
{
	BYTE	cProtocol;
};

struct tagResult : public tagProtoHeader
{
	BYTE	cResult;
};

struct tagDBSelPlayer : public tagProtoHeader
{
	char	szRoleName[_NAME_LEN];
	char	szAccountName[_NAME_LEN];
    KSG_PASSWORD    Password;
};

struct tagDBDelPlayer : public tagProtoHeader
{
	char	        szAccountName[_NAME_LEN];
    KSG_PASSWORD    Password;
	char	        szRoleName[_NAME_LEN];
};

//删除与新建角色的返回消息带的数据
struct tagNewDelRoleResponse : public tagDBSelPlayer
{
	bool	bSucceeded;		//是否成功
};

// 2003.05.11
struct tagDBSyncPlayerInfo : public tagProtoHeader
{
	size_t	dataLength;
	char	szData[0];
};

/*
 * 2003.06.27
 * s2c_gateway_broadcast
*/

#define	AP_WARNING_ALL_PLAYER_QUIT			1
#define	AP_NOTIFY_GAMESERVER_SAFECLOSE		2
#define	AP_NOTIFY_ALL_PLAYER				3
#define	MAX_GATEWAYBROADCAST_LEN	260
struct tagGatewayBroadCast : public tagProtoHeader
{
	UINT	uCmdType;
	char	szData[MAX_GATEWAYBROADCAST_LEN];
};

/*
 * 2003.05.22
 * s2c_syncgamesvr_roleinfo_cipher
*/
struct tagGuidableInfo : public tagProtoHeader
{
	GUID guid;
	WORD nExtPoint;			//可用的附送点
	WORD nChangePoint;		//变化的附送点
	size_t	datalength;
	char	szData[0];

};

/*
 * c2s_permitplayerlogin
 */
struct tagPermitPlayerLogin : public tagProtoHeader
{
	GUID guid;

	BYTE szRoleName[_NAME_LEN];
	
	/*
	 * Succeeded : true
	 * Failed	 : false
	 */
	bool bPermit;
};

struct tagPermitPlayerExchange
{
	BYTE cProtocol;
	GUID guid;
	DWORD dwIp;
	WORD wPort;
	bool bPermit;
	//DWORD dwIp1;
	//WORD wPort1;
	BYTE nServer;
};
/*
 * c2s_notifyplayerlogin
 */
struct tagNotifyPlayerLogin : public tagPermitPlayerLogin
{
	UINT			nIPAddr0;
	unsigned short	nPort0;
	UINT			nIPAddr1;
	unsigned short	nPort1;
	UINT			nIPAddr2;
	unsigned short	nPort2;
	UINT			nIPAddr3;
	unsigned short	nPort3;
	UINT			nIPAddr4;
	unsigned short	nPort4;
	UINT			nIPAddr5;
	unsigned short	nPort5;
	UINT			nIPAddr6;
	unsigned short	nPort6;
	UINT			nIPAddr7;
	unsigned short	nPort7;
	UINT			nIPAddr8;
	unsigned short	nPort8;
	UINT			nIPAddr9;
	unsigned short	nPort9;
	UINT			nIPAddr10;
	unsigned short	nPort10;
	UINT			nIPAddr11;
	unsigned short	nPort11;
	UINT			nIPAddr12;
	unsigned short	nPort12;
	UINT			nIPAddr13;
	unsigned short	nPort13;
	UINT			nIPAddr14;
	unsigned short	nPort14;
	UINT			nIPAddr15;
	unsigned short	nPort15;
	UINT			nIPAddr16;
	unsigned short	nPort16;
	UINT			nIPAddr17;
	unsigned short	nPort17;
	UINT			nIPAddr18;
	unsigned short	nPort18;
	UINT			nIPAddr19;
	unsigned short	nPort19;
	UINT			nIPAddr20;
	unsigned short	nPort20;
	UINT			nIPAddr21;
	unsigned short	nPort21;
	UINT			nIPAddr22;
	unsigned short	nPort22;
	UINT			nIPAddr23;
	unsigned short	nPort23;
	UINT			nIPAddr24;
	unsigned short	nPort24;
	UINT			nIPAddr25;
	unsigned short	nPort25;
	UINT			nIPAddr26;
	unsigned short	nPort26;
	UINT			nIPAddr27;
	unsigned short	nPort27;
	UINT			nIPAddr28;
	unsigned short	nPort28;
	UINT			nIPAddr29;
	unsigned short	nPort29;
	UINT			nIPAddr30;
	unsigned short	nPort30;
	UINT			nIPAddr31;
	unsigned short	nPort31;
	UINT			nIPAddr32;
	unsigned short	nPort32;
	UINT			nIPAddr33;
	unsigned short	nPort33;
	UINT			nIPAddr34;
	unsigned short	nPort34;
	UINT			nIPAddr35;
	unsigned short	nPort35;
	UINT			nIPAddr36;
	unsigned short	nPort36;
	UINT			nIPAddr37;
	unsigned short	nPort37;
	UINT			nIPAddr38;
	unsigned short	nPort38;
	UINT			nIPAddr39;
	unsigned short	nPort39;
	UINT			nIPAddr40;
	unsigned short	nPort40;
	UINT			nIPAddr41;
	unsigned short	nPort41;
	UINT			nIPAddr42;
	unsigned short	nPort42;
	UINT			nIPAddr43;
	unsigned short	nPort43;
	UINT			nIPAddr44;
	unsigned short	nPort44;
	UINT			nIPAddr45;
	unsigned short	nPort45;
	UINT			nIPAddr46;
	unsigned short	nPort46;
	UINT			nIPAddr47;
	unsigned short	nPort47;
};

/*
 * s2c_querymapinfo
 */
struct tagQueryMapInfo : public tagProtoHeader
{	
};

/*
 * s2c_querygameserverinfo
 */
struct tagQueryGameSvrInfo : public tagProtoHeader
{	
};

/*
 * s2c_notifysvrip
 */
struct tagNotifySvrIp : public tagProtoHeader
{
	WORD	pckgID;

	BYTE	cIPType;
	DWORD	dwMapID;

	DWORD	dwSvrIP;
};

/*
 * s2c_notifyplayerexchange
 */
struct tagNotifyPlayerExchange : public tagProtoHeader
{
	GUID			guid;
	UINT			nIPAddr;
	unsigned short	nPort;
	UINT			nIPAddr1;
	unsigned short	nPort1;

};

/*
 * c2s_requestsvrip
 */

/*
 * BYTE	cIPType
 */
#define INTRANER_IP	0
#define INTERNET_IP 1

struct tagRequestSvrIp : public tagProtoHeader
{
	WORD	pckgID;

	BYTE	cIPType;
	DWORD	dwMapID;
};

/*
 * c2c_notifyexchange
 */
struct tagSearchWay : public tagProtoHeader
{
	int		lnID;
	int		nIndex;
	DWORD	dwPlayerID;

	BYTE	nServerID;	
};

/*
 * c2s_updatemapinfo
 */
struct tagUpdateMapID : public tagProtoHeader
{
	/*
	 * For example : Are your clear older information when it 
	 *		update local informatin
	 */
	BYTE nServerID;

	BYTE cMapCount;

	short szMapID[0];	// C4200 warning
};

/*
 * c2s_updategameserverinfo
 */
struct tagGameSvrInfo : public tagProtoHeader
{
	UINT			nIPAddr_Intraner;
	UINT			nIPAddr_Internet;

	UINT			nIPAddr_Foward0;
	UINT			nIPAddr_Foward1;
	UINT			nIPAddr_Foward2;
	UINT			nIPAddr_Foward3;
	UINT			nIPAddr_Foward4;
	UINT			nIPAddr_Foward5;
	UINT			nIPAddr_Foward6;
	UINT			nIPAddr_Foward7;
	UINT			nIPAddr_Foward8;
	UINT			nIPAddr_Foward9;
	UINT			nIPAddr_Foward10;
	UINT			nIPAddr_Foward11;
	UINT			nIPAddr_Foward12;
	UINT			nIPAddr_Foward13;
	UINT			nIPAddr_Foward14;
	UINT			nIPAddr_Foward15;
	UINT			nIPAddr_Foward16;
	UINT			nIPAddr_Foward17;
	UINT			nIPAddr_Foward18;
	UINT			nIPAddr_Foward19;
	UINT			nIPAddr_Foward20;
	UINT			nIPAddr_Foward21;
	UINT			nIPAddr_Foward22;
	UINT			nIPAddr_Foward23;
	UINT			nIPAddr_Foward24;
	UINT			nIPAddr_Foward25;
	UINT			nIPAddr_Foward26;
	UINT			nIPAddr_Foward27;
	UINT			nIPAddr_Foward28;
	UINT			nIPAddr_Foward29;
	UINT			nIPAddr_Foward30;
	UINT			nIPAddr_Foward31;
	UINT			nIPAddr_Foward32;
	UINT			nIPAddr_Foward33;
	UINT			nIPAddr_Foward34;
	UINT			nIPAddr_Foward35;
	UINT			nIPAddr_Foward36;
	UINT			nIPAddr_Foward37;
	UINT			nIPAddr_Foward38;
	UINT			nIPAddr_Foward39;
	UINT			nIPAddr_Foward40;
	UINT			nIPAddr_Foward41;
	UINT			nIPAddr_Foward42;
	UINT			nIPAddr_Foward43;
	UINT			nIPAddr_Foward44;
	UINT			nIPAddr_Foward45;
	UINT			nIPAddr_Foward46;
	UINT			nIPAddr_Foward47;	

	unsigned short	nPort;
	unsigned short	nFowardPort0;
	unsigned short	nFowardPort1;
	unsigned short	nFowardPort2;
	unsigned short	nFowardPort3;
	unsigned short	nFowardPort4;
	unsigned short	nFowardPort5;
	unsigned short	nFowardPort6;
	unsigned short	nFowardPort7;
	unsigned short	nFowardPort8;
	unsigned short	nFowardPort9;
	unsigned short	nFowardPort10;
	unsigned short	nFowardPort11;
	unsigned short	nFowardPort12;
	unsigned short	nFowardPort13;
	unsigned short	nFowardPort14;
	unsigned short	nFowardPort15;
	unsigned short	nFowardPort16;
	unsigned short	nFowardPort17;
	unsigned short	nFowardPort18;
	unsigned short	nFowardPort19;
	unsigned short	nFowardPort20;
	unsigned short	nFowardPort21;
	unsigned short	nFowardPort22;
	unsigned short	nFowardPort23;
	unsigned short	nFowardPort24;
	unsigned short	nFowardPort25;
	unsigned short	nFowardPort26;
	unsigned short	nFowardPort27;
	unsigned short	nFowardPort28;
	unsigned short	nFowardPort29;
	unsigned short	nFowardPort30;
	unsigned short	nFowardPort31;
	unsigned short	nFowardPort32;
	unsigned short	nFowardPort33;
	unsigned short	nFowardPort34;
	unsigned short	nFowardPort35;
	unsigned short	nFowardPort36;
	unsigned short	nFowardPort37;
	unsigned short	nFowardPort38;
	unsigned short	nFowardPort39;
	unsigned short	nFowardPort40;
	unsigned short	nFowardPort41;
	unsigned short	nFowardPort42;
	unsigned short	nFowardPort43;
	unsigned short	nFowardPort44;
	unsigned short	nFowardPort45;
	unsigned short	nFowardPort46;
	unsigned short	nFowardPort47;


	WORD			wCapability;
};

/*
 * s2c_identitymapping
 */
struct tagIdentityMapping : public tagGameSvrInfo
{
	GUID guid;
};

/*
 * c2s_logiclogin
 * s2c_gmgateway2relaysvr
 * s2c_gmnotify
 */
struct tagLogicLogin : public tagProtoHeader
{
	char szName[64];
	GUID guid;
};

/*
 * s2c_logiclogout
 */
struct tagLogicLogout : public tagProtoHeader
{
	BYTE szRoleName[_NAME_LEN];
};

/*
 * c2s_registeraccount
 */
struct tagRegisterAccount : public tagProtoHeader
{
	BYTE szAccountName[_NAME_LEN];
};

/*
 * c2s_entergame
 */
struct tagEnterGame : public tagProtoHeader
{
	/*
	 * Succeeded : content is account name
	 * Failed	 : content is null
	 */

	BYTE szAccountName[_NAME_LEN];
};

struct tagEnterGame2 : public EXTEND_HEADER
{
	char szAccountName[_NAME_LEN];
	char szCharacterName[_NAME_LEN];
	DWORD	dwNameID;
	unsigned long	lnID;
};

/*
 * c2s_leavegame
 */

/*
 * BYTE cCmdType
 */
#define NORMAL_LEAVEGAME	0x0		// lock account
#define HOLDACC_LEAVEGAME	0x1A	// clear resource but don't to unlock account 

struct tagLeaveGame : public tagProtoHeader
{
	BYTE cCmdType;
	WORD nExtPoint;        //将要扣除的附送点
	/*
	 * Succeeded : content is account name
	 * Failed	 : content is null
	 */

	char szAccountName[_NAME_LEN];
};

struct tagLeaveGame2 : public EXTEND_HEADER
{
	BYTE cCmdType;
	char szAccountName[_NAME_LEN];
	BYTE nSelServer;
};
/*
*  c2s_registerfamily
*/
struct tagRegisterFamily : public tagProtoHeader
{
	BYTE bRegister;		//1 is Register, 0 is unRegister
	BYTE nFamily;
	BYTE RelayMethod;
};

/*
 * c2s_gmsvr2gateway_saverole
 */
struct tagGS2GWSaveRole : public tagProtoHeader
{
	size_t	datalength;
	BYTE	szData[0];
};

/*
 * #pragma pack( pop )
 *
 * } End of the struct define
 */

typedef struct
{
	char	szName[32];
	BYTE	Sex;
	BYTE	Series;
//	BYTE	HelmType;
//	BYTE	ArmorType;
//	BYTE	WeaponType;
	BYTE	Level;
} RoleBaseInfo/* client */, S3DBI_RoleBaseInfo /* server */;

typedef struct
{
	BYTE				ProtocolType;
	RoleBaseInfo		m_RoleList[MAX_PLAYER_IN_ACCOUNT];
} ROLE_LIST_SYNC;

//移自RoleDBManager/kroledbheader.h
//用来替换上面的ROLE_LIST_SYNC,ROLE_LIST_SYNC结构不再需要了
struct TProcessData
{
	unsigned char	nProtoId;
	size_t			nDataLen;//TRoleNetMsg时表示该Block的实际数据长度,TProcessData时表示Stream的实际数据长度
	unsigned long	ulIdentity;
	bool			bLeave;
	char            szAccountName[_NAME_LEN];
	char			pDataBuffer[1];//实际的数据
};

struct tagRoleEnterGame
{
	BYTE			ProtocolType;
	bool			bLock;
	char			Name[_NAME_LEN];
};

//新建角色的信息结构
//注释：新建决消息c2s_newplayer，传送的参数为TProcessData结构描述的数据，其中TProcessData::pDataBuffer要扩展为NEW_PLAYER_COMMAND
struct NEW_PLAYER_COMMAND
{
	BYTE			m_btRoleNo;			// 角色编号
	BYTE			m_btSeries;			// 五行系
	unsigned short	m_NativePlaceId;	//出生地ID
	char			m_szName[32];		// 姓名
};


typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 好友 id
	int				m_nPlayerIdx;		// 好友 player index
} CHAT_LOGIN_FRIEND_NONAME_SYNC;		// 玩家登录时发送玩家聊天好友数据（不带名字）

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	WORD			m_wLength;
	DWORD			m_dwID;				// 好友 id
	int				m_nPlayerIdx;		// 好友 player index
	char			m_szName[32];		// 好友名字
} CHAT_LOGIN_FRIEND_NAME_SYNC;			// 玩家登录时发送玩家聊天好友数据（带名字）

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
} CHAT_APPLY_RESEND_ALL_FRIEND_NAME_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 好友 id
} CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND;	// 申请得到玩家某个聊天好友的完整数据

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 好友 id
	int				m_nPlayerIdx;		// 好友 player index
	char			m_szName[32];		// 好友名字
} CHAT_ONE_FRIEND_DATA_SYNC;			// 发送玩家某一个聊天好友数据（带名字）

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 好友 id
	int				m_nPlayerIdx;		// 好友 player index
} CHAT_FRIEND_ONLINE_SYNC;				// 通知客户端有好友上线

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 被删除id
} CHAT_DELETE_FRIEND_COMMAND;			// 删除某个聊天好友

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 删除id
} CHAT_DELETE_FRIEND_SYNC;				// 被某个聊天好友删除

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 被删除id
} CHAT_REDELETE_FRIEND_COMMAND;			// 删除某个聊天好友

typedef struct 
{
	BYTE			ProtocolType;
	BYTE			m_LogoutType;
} LOGOUT_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			szAccName[32];
} LOGIN_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 好友 id
} CHAT_FRIEND_OFFLINE_SYNC;				// 通知客户端有好友下线

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	WORD			m_wLength;
	char			m_szSentence[MAX_SENTENCE_LENGTH];
} TRADE_APPLY_OPEN_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
} TRADE_APPLY_CLOSE_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	BYTE			m_btState;			// if == 0 close if == 1 open if == 2 trading
	DWORD			m_dwNpcID;			// 如果是开始交易，对方的 npc id
} TRADE_CHANGE_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	WORD			m_wLength;
	DWORD			m_dwID;
	BYTE			m_btState;
	char			m_szSentence[MAX_SENTENCE_LENGTH];
} NPC_SET_MENU_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;
} TRADE_APPLY_START_COMMAND;

// 服务器转发交易申请
typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_nDestIdx;			// 申请者在服务器端的player idx
	DWORD			m_dwNpcId;			// 申请者的 npc id
} TRADE_APPLY_START_SYNC;

// 接受或拒绝别人的交易申请
typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	BYTE			m_bDecision;		// 同意 1 不同意 0
	int				m_nDestIdx;			// 交易对方在服务器端的player idx
} TRADE_REPLY_START_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_nMoney;
} TRADE_MOVE_MONEY_COMMAND;		// c2s_trademovemoney

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_nMoney;
} TRADE_MONEY_SYNC;				// s2c_trademoneysync

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	BYTE			m_btDecision;		// 确定交易 1  退出交易 0  取消确定 4  锁定交易 2  取消锁定 3
} TRADE_DECISION_COMMAND;				// 交易执行或取消 c2s_tradedecision

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	BYTE			m_btDecision;		// 交易ok 1  交易取消 0  锁定 2  取消锁定 3
} TRADE_DECISION_SYNC;					// s2c_tradedecision

typedef struct
{
	BYTE			ProtocolType;		
	BYTE			m_byDir;			// 取钱的方向（0存，1取）
	DWORD			m_dwMoney;			// 钱数
} STORE_MONEY_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	WORD			m_wLength;			// 长度
	BYTE			m_btError;			// 错误类型	0 对方关闭了此频道，1 找不到对方
	char			m_szName[32];		// 对方名字
} CHAT_SCREENSINGLE_ERROR_SYNC;

typedef struct 
{
	BYTE			ProtocolType;		// 协议类型
	BYTE			m_btStateInfo[MAX_NPC_RECORDER_STATE];
	DWORD			m_ID;				// Npc的GID
}	NPC_SYNC_STATEINFO;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwNpcID;
} TEAM_INVITE_ADD_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	WORD			m_wLength;			// 长度
	int				m_nIdx;
	char			m_szName[32];
} TEAM_INVITE_ADD_SYNC;

typedef struct
{
	BYTE			ProtocolType;		//
	int				m_nAuraSkill;
} SKILL_CHANGEAURASKILL_COMMAND;		//更换光环技能

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btResult;
	int				m_nIndex;
} TEAM_REPLY_INVITE_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btSelfLock;
	BYTE			m_btDestLock;
	BYTE			m_btSelfOk;
	BYTE			m_btDestOk;
} TRADE_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	DWORD			m_dwSkillID;		// 技能
	int				m_nLevel;
	int				m_nTime;			// 时间
	KMagicAttrib	m_MagicAttrib[MAX_SKILL_STATE];
} STATE_EFFECT_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	DWORD			m_dwTime;
} PING_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	DWORD			m_dwReplyServerTime;
	DWORD			m_dwClientTime;
} PING_CLIENTREPLY_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btSitFlag;
} NPC_SIT_COMMAND;



typedef struct
{
	BYTE			ProtocolType;
} NPC_RIDE_COMMAND;


typedef struct
{
	BYTE			ProtocolType;
	int				nMpsX;
	int				nMpsY;
} NPC_JUMP_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	int				m_dwRegionID;
	int				m_nObjID;
} OBJ_MOUSE_CLICK_SYNC;

typedef struct tagSHOW_MSG_SYNC
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	WORD			m_wMsgID;
	LPVOID			m_lpBuf;
	tagSHOW_MSG_SYNC() {m_lpBuf = NULL;};
	~tagSHOW_MSG_SYNC() {Release();}
	void	Release() {if (m_lpBuf) delete []m_lpBuf; m_lpBuf = NULL;}
} SHOW_MSG_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btFlag;
} PK_APPLY_NORMAL_FLAG_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btFlag;
} PK_NORMAL_FLAG_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	DWORD			m_dwNpcID;
} PK_APPLY_ENMITY_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	BYTE			m_btState;
	DWORD			m_dwNpcID;
	char			m_szName[32];
} PK_ENMITY_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	BYTE			m_btState;
	DWORD			m_dwNpcID;
	char			m_szName[32];
} PK_EXERCISE_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	int				m_nPKValue;
} PK_VALUE_SYNC;

typedef struct
{
	int		m_nID;				// 物品的ID
	BYTE	m_btGenre;			// 物品的类型
	BYTE	m_btDetail;			// 物品的类别
	BYTE	m_btParticur;		// 物品的详细类别
	BYTE	m_btSeries;			// 物品的五行
	BYTE	m_btLevel;			// 物品的等级
	BYTE	m_btLuck;			// MF
	int 	m_btMagicLevel[6];	// 生成参数
	WORD  	m_wVersion;			// 装备版本
	DWORD	m_dwRandomSeed;		// 随机种子
	int		iNgoaiTrang;
} SViewItemInfo;

typedef struct
{
	BYTE			ProtocolType;
	DWORD			m_dwNpcID;
	SViewItemInfo	m_sInfo[itempart_num];
	int             m_Task100;
	int             m_Task162;
	int             m_Task139;
	int             m_WorldStat;
	int             m_PKValue;

} VIEW_EQUIP_SYNC;				// s2c_viewequip

typedef struct//该结构是所统计的玩家的基本数据
{
	char	Name[20];
	int		nValue;
	BYTE	bySort;
	int     nHeightValue;
}TRoleList;

// 游戏统计结构
typedef struct
{
	TRoleList MoneyStat[10];			//金钱最多排名列表（十个玩家，最多可达到100个）
	TRoleList LevelStat[10];			//级别最多排名列表（十个玩家，最多可达到100个）
	TRoleList KillerStat[10];			//杀人最多排名列表
	
	//[门派号][玩家数]，其中[0]是没有加入门派的玩家
	TRoleList MoneyStatBySect[11][10];	//各门派金钱最多排名列表
	TRoleList LevelStatBySect[11][10];	//各门派级别最多排名列表

	//[门派号]，其中[0]是没有加入门派的玩家
	int SectPlayerNum[11];				//各个门派的玩家数
	int SectMoneyMost[11];				//财富排名前一百玩家中各门派所占比例数
	int SectLevelMost[11];				//级别排名前一百玩家中各门派所占比例数
}  TGAME_STAT_DATA;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	bSleep;
	DWORD	NpcID;
} NPC_SLEEP_SYNC;

//////////////
//排名相关
typedef struct
{
	BYTE		ProtocolType;
	DWORD		dwLadderID;
	TRoleList	StatData[10];
} LADDER_DATA;

typedef struct
{
	BYTE		ProtocolType;
	WORD		wSize;
	int			nCount;
	DWORD		dwLadderID[0];
} LADDER_LIST;

typedef struct
{
	BYTE		ProtocolType;
	DWORD		dwLadderID;
} LADDER_QUERY;


///////////////////
//chat 相关

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	packageID;
	char	someone[_NAME_LEN];
	BYTE	sentlen;
} CHAT_SOMEONECHAT_CMD, CHAT_SOMEONECHAT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	packageID;
	//BYTE	filter;
	DWORD	channelid;
	BYTE	cost;	//0: 无限制，1: 10元/句，2: <10Lv ? 不能说 : MaxMana/2/句, 3: MaxMana/10/句,4: <20Lv ? 不能说 : MaxMana*4/5/句
	BYTE	sentlen;
	BYTE		figure;

} CHAT_CHANNELCHAT_CMD;

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	packageID;
	char	someone[_NAME_LEN];
	DWORD	channelid;
	BYTE	sentlen;
	BYTE	figure;
} CHAT_CHANNELCHAT_SYNC;

enum {codeSucc, codeFail, codeStore};
typedef struct
{
	BYTE	ProtocolType;
	DWORD	packageID;
	BYTE	code;
} CHAT_FEEDBACK;

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	WORD	wChatLength;
} CHAT_EVERYONE;


typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	WORD	wChatLength;
	BYTE	byHasIdentify;
	WORD	wPlayerCount;
} CHAT_GROUPMAN;


typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	nameid;
	unsigned long lnID;
	WORD	wChatLength;
} CHAT_SPECMAN;


enum { tgtcls_team, tgtcls_kim, tgtcls_fac, tgtcls_tong, tgtcls_scrn, tgtcls_bc};
typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	nFromIP;
	DWORD	nFromRelayID;
	DWORD	channelid;
	BYTE	TargetCls;
	DWORD	TargetID;
	WORD	routeDateLength;
} CHAT_RELEGATE;


///////////////////////////////////////
// tong 相关

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
} S2C_TONG_HEAD;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
} STONG_PROTOCOL_HEAD;

typedef struct
{
	int		m_nPlayerIdx;
	int		m_nCamp;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_APPLY_CREATE;

typedef struct
{
	int		m_nPlayerIdx;
	int		m_nCamp;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_APPLY_CHANGE;


typedef struct
{
	int		m_nPlayerIdx;
	DWORD	m_dwNpcID;
} STONG_SERVER_TO_CORE_APPLY_ADD;

typedef struct
{
	int		m_nSelfIdx;
	int		m_nTargetIdx;
	DWORD	m_dwNameID;
} STONG_SERVER_TO_CORE_CHECK_ADD_CONDITION;

typedef struct
{
	int		m_nCamp;
	int		m_nPlayerIdx;
	DWORD	m_dwPlayerNameID;
	char	m_szTongName[64];
} STONG_SERVER_TO_CORE_CREATE_SUCCESS;

typedef struct
{
	int		m_nSelfIdx;
	int		m_nTargetIdx;
	DWORD	m_dwNameID;
} STONG_SERVER_TO_CORE_REFUSE_ADD;

typedef struct
{
	int		m_nSelfIdx;
	int		m_nInfoID;
	int		m_nParam1;
	int		m_nParam2;
	int		m_nParam3;
	char	m_szName[32];
} STONG_SERVER_TO_CORE_GET_INFO;

typedef struct
{
	int		m_nCamp;
	int		m_nPlayerIdx;
	DWORD	m_dwPlayerNameID;
	char	m_szTongName[32];
	char	m_szMasterName[32];
	char	m_szTitleName[32];
} STONG_SERVER_TO_CORE_ADD_SUCCESS;

typedef struct
{
	int		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTitle[32];
	char	m_szName[32];
} STONG_SERVER_TO_CORE_BE_INSTATED;

typedef struct
{
	int		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
} STONG_SERVER_TO_CORE_BE_KICKED;

typedef struct
{
	int		m_nPlayerIdx;
	BOOL	m_bSuccessFlag;
	char	m_szName[32];
} STONG_SERVER_TO_CORE_LEAVE;

typedef struct
{
	int		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	DWORD	m_dwTongNameID;
	char	m_szName[32];
} STONG_SERVER_TO_CORE_CHECK_GET_MASTER_POWER;

typedef struct
{
	int		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	DWORD	m_dwTongNameID;
	char	m_szTitle[32];
	char	m_szName[32];
} STONG_SERVER_TO_CORE_CHANGE_AS;

typedef struct
{
	DWORD	m_dwTongNameID;
	char	m_szName[32];
} STONG_SERVER_TO_CORE_CHANGE_MASTER;

typedef struct
{
	DWORD	m_dwParam;
	int		m_nFlag;
	int		m_nCamp;
	int		m_nFigure;
	int		m_nPos;
	char	m_szTongName[32];
	char	m_szTitle[32];
	char	m_szMaster[32];
	char	m_szName[32];
} STONG_SERVER_TO_CORE_LOGIN;

// 玩家申请建立帮会 用扩展协议
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btCamp;
	char	m_szName[defTONG_NAME_MAX_LENGTH + 1];
} TONG_APPLY_CREATE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btCamp;
	char	m_szName[defTONG_NAME_MAX_LENGTH + 1];
} TONG_APPLY_CHANGE_COMMAND;


// 玩家申请加入帮会 用扩展协议
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwNpcID;
} TONG_APPLY_ADD_COMMAND;

// 玩家申请加入帮会 用扩展协议
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btCurFigure;
	BYTE	m_btCurPos;
	BYTE	m_btNewFigure;
	BYTE	m_btNewPos;
	char	m_szName[32];
} TONG_APPLY_INSTATE_COMMAND;

// 帮会建立失败 扩展协议
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btFailId;
} TONG_CREATE_FAIL_SYNC;

// 转发加入帮会申请 扩展协议
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int		m_nPlayerIdx;
	char	m_szName[32];
} TONG_APPLY_ADD_SYNC;

// 通知玩家建立帮会成功 用普通协议
typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btCamp;
	char	m_szName[defTONG_NAME_MAX_LENGTH + 1];
} TONG_CREATE_SYNC;

// 通知玩家加入帮会 用扩展协议
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btCamp;
	char	m_szTongName[32];
	char	m_szTitle[32];
	char	m_szMaster[32];
} TONG_Add_SYNC;

// 玩家申请解散帮会 用扩展协议
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
} TONG_APPLY_DISMISS_COMMAND;

// 玩家帮会被解散 用普通协议
typedef struct
{
	BYTE	ProtocolType;
} TONG_DISMISS_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	int	m_bLifePoison;
	int m_bManaPoison;
	int m_bAllPoison;
	int m_bAllPoisonMin;

	BOOL m_bSuaTrangBi;
	BOOL m_bUseTTL;
	BOOL m_bNhatDoChonLoc;
	BOOL m_bGuiTien;
	BOOL m_bQuayLai;
	int m_NumSelect;
	BOOL m_RingSelect;
	BOOL m_WeaSelect;
} AUTO_QUAYLAI;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_bAuto;
	BYTE	m_bActive;
} PLAYER_REQUEST_AUTO;


typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_ID;				// 物品的ID
	BYTE			m_Number;	
} PLAYER_SELL_ITEM_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
} S2CEXIT_GAME;

typedef struct tagPLAYER_COMMAND
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	WORD			m_wMsgID;
	LPVOID			m_lpBuf;
	tagPLAYER_COMMAND() {m_lpBuf = NULL;};
	~tagPLAYER_COMMAND() {Release();}
	void	Release() {if (m_lpBuf) delete []m_lpBuf; m_lpBuf = NULL;}
} PLAYER_COMMAND;


typedef struct
{
	BYTE	ProtocolType;
	int	m_Option1;
	int m_Option2;
	int m_Option3;
	int m_Option4;
	int m_Option5;
	int m_Option6;
	int m_Option7;
	int m_Option8;
	int m_Option9;
	int m_Option10;
	int	m_Option11;
	int m_Option12;
	int m_Option13;
	int m_Option14;
	int m_Option15;
	int m_Option16;
	int m_Option17;
	int m_Option18;
	int m_Option19;
	int m_Option20;
	int	m_Option21;
	int m_Option22;
	int m_Option23;
	int m_Option24;
	int m_Option25;
	int m_Option26;
	int m_Option27;
	int m_Option28;
	int m_Option29;
	int m_Option30;
	int	m_Option31;
	int m_Option32;
	int m_Option33;
	int m_Option34;
	int m_Option35;
	int m_Option36;
	int m_Option37;
	int m_Option38;
	int m_Option39;
} AUTO_LOCDO;


typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int		m_nPlayerIdx;
	DWORD	m_dwNameID;
	BYTE	m_btFlag;			// 是否接受 TRUE 接受 FALSE 不接受
} TONG_ACCEPT_MEMBER_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btInfoID;
	int		m_nParam1;
	int		m_nParam2;
	int		m_nParam3;
	char	m_szBuf[64];
} TONG_APPLY_INFO_COMMAND;

typedef struct
{
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTitle[32];
	char	m_szName[32];
	BYTE	m_nStatus;
	BYTE	m_PlayerLevel;
	BYTE	m_PlayerPoint;
	int	m_PlayerTime;
	int	m_PlayerTimeEnd;

} TONG_ONE_LEADER_INFO;

typedef struct
{
	char	m_szName[32];
	char	m_szTitle[32];
	int		m_nStatus;
	int		m_PlayerLevel;
	int		m_PlayerPoint;
	int		m_PlayerTime;
	int		m_PlayerTimeEnd;
} TONG_ONE_MEMBER_INFO;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwNpcID;
	DWORD	m_dwMoney;
	int		m_nCredit;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	char	m_szTongName[32];
	TONG_ONE_LEADER_INFO	m_sMember[1 + defTONG_MAX_DIRECTOR];
} TONG_HEAD_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwMoney;
	int		m_nCredit;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	BYTE	m_btStateNo;
	BYTE	m_btCurNum;
	char	m_szTongName[32];
	TONG_ONE_LEADER_INFO	m_sMember[defTONG_ONE_PAGE_MAX_NUM];
} TONG_MANAGER_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwMoney;
	int		m_nCredit;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	BYTE	m_btStateNo;
	BYTE	m_btCurNum;
	char	m_szTitle[32];
	char	m_szTongName[32];
	TONG_ONE_MEMBER_INFO	m_sMember[defTONG_ONE_PAGE_MAX_NUM];
} TONG_MEMBER_INFO_SYNC;

// 玩家自身在帮会中的信息 用扩展协议
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btJoinFlag;
	BYTE	m_btFigure;
	BYTE	m_btCamp;
	char	m_szTongName[32];
	char	m_szTitle[32];
	char	m_szMaster[32];
} TONG_SELF_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btSuccessFlag;
	BYTE	m_btOldFigure;
	BYTE	m_btOldPos;
	BYTE	m_btNewFigure;
	BYTE	m_btNewPos;
	char	m_szTitle[32];
	char	m_szName[32];
} TONG_INSTATE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
} TONG_APPLY_KICK_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btSuccessFlag;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
} TONG_KICK_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
} TONG_APPLY_LEAVE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
} TONG_APPLY_CHANGE_MASTER_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	char	m_szTitle[32];
} TONG_APPLY_CHANGE_TITLE_FEMALE;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	char	m_szTitle[32];

} TONG_APPLY_CHANGE_TITLE_MALE;





typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	int 	m_nMoney;
} TONG_APPLY_DEPOSIT_MONEY;



typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	int 	m_nMoney;
} TONG_APPLY_TAKE_MONEY;




typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	char	m_szName[32];
	char	m_szTitle[32];
} TONG_APPLY_CHANGE_TITLE;



typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFailID;
	char	m_szName[32];
} TONG_CHANGE_MASTER_FAIL_SYNC;

// tong 相关 end
typedef struct
{
	BYTE	ProtocolType;
	DWORD	m_dwNpcID;
	WORD	m_wGoldFlag;
} NPC_GOLD_CHANGE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwItemID;
	int		nChange;
} ITEM_DURABILITY_CHANGE;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwItemID;
} ITEM_REPAIR;



typedef struct
{
	BYTE	ProtocolType;
} OPEN_SHOP_GOLD;

typedef struct // protocol image npc
{
	BYTE			ProtocolType;
	BYTE			m_nType;
	int				m_nValue;
} IMAGENPC_VALUE_SYNC;	

typedef struct
{
	BYTE	ProtocolType;
} OPEN_TOP_TK_NEW;

typedef struct	
{
	BYTE	ProtocolType;
	int	nNum;
} SET_IMAGE;

typedef struct	
{
	BYTE	ProtocolType;
	int 	nValueIndex;
	int 	nValue;
	BOOL 	bCheck;
} PLAYER_TONGKIM_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	int				nType;
	char			szFunc[32];
} PLAYER_UI_CMD_SCRIPT; // protocol load script
// 在调用这支函数之前必须判断是否处于交易状态，如果正在交易，不能调用这支函数
void SendClientCmdSell(int nID);

void SendClientCmdBlockItem(int nID);

void SendClientCmdStaskItem(int nID,int nNumber);
void SendClientCmdStringBox(char* szString);



void SendClientCmdLoginPWStoreBox(char* szString);
void SendClientCmdPWStoreBox(char* szString,char* szStringNew);
void SendClientCmdBlockStoreBox();


void SendClientCmdNewPWStoreBox(char* szString);



void SendClientCmdCheckItemBox();
void SendClientCmdSlectCheckBox(int nSlect);

void SendClientCmdButtonPGBox(int nSlect);

void SendClientCmdShopName(char* szString);

void SendClientCmdShopPrice(int nID, int nPrice);

void SendClientCmdOpenShop();

void SendClientCmdCloseBuyShop();


void SendClientCmdShopBuyItem(int nID);

void SendClientCmdUnBlockItem(int nID);

// 在调用这支函数之前必须判断是否处于交易状态，如果正在交易，不能调用这支函数
BOOL SendClientCmdBuy(int nBuyIdx, int nPlace, int nX, int nY);
// 在调用这支函数之前必须判断是否处于交易状态，如果正在交易，不能调用这支函数
void SendClientCmdRun(int nX, int nY);
// 在调用这支函数之前必须判断是否处于交易状态，如果正在交易，不能调用这支函数
void SendClientCmdWalk(int nX, int nY);
// 在调用这支函数之前必须判断是否处于交易状态，如果正在交易，不能调用这支函数
void SendClientCmdSkill(int nSkillID, int nX, int nY,int nX0 = -1, int nY0 = -1,int nRadius = -1);
//void SendClientCmdPing();
void SendClientCmdSit(int nSitFlag);
void SendClientCmdRide();
void SendClientCmdMoveItem(void* pDownPos, void* pUpPos);
void SendClientCmdQueryLadder(DWORD	dwLadderID);
void SendClientCmdRequestNpc(int nID);
void SendClientCmdJump(int nX, int nY);
void SendClientCmdStoreMoney(int nDir, int nMoney);
//void SendClientCmdRevive(int nReviveType);
void SendClientCmdRevive();
void SendObjMouseClick(int nObjID, DWORD dwRegionID);
void SendClientCmdRepair(DWORD dwID);
void SendClientCmdLiXian();

void SendClientCmdOpenShopGold();
void SendClientCmdOpenTopTKNew();
void SendClientToaDo(int nstt);
void SendClientCPSetImageCmd(int nNumber);
void SendUiCmdScript(int nName,char*szFunc);
void SendClientCmdAutoPlay(BOOL nbAuto, BOOL nbActive);
void SendClientCmdAutoSell(int nId);
void SendClientCmdOpenShop(BYTE nItemGenre, BYTE nDetailType, BYTE	nLevel, BYTE num);

extern	int	g_nProtocolSize[MAX_PROTOCOL_NUM];
#pragma pack(pop, enter_protocol)
#endif
