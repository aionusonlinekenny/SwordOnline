//-----------------------------------------------------------------------
//	Sword3 KNpc.cpp
//-----------------------------------------------------------------------
#include "KCore.h"
//#include <crtdbg.h>
#include "KNpcAI.h"
#include "KSkills.h"
#include "KObj.h"
#include "KObjSet.h"
#include "KMath.h"
#include "KPlayer.h"
#include "KNpc.h"
#include "GameDataDef.h"
#include "KSubWorldSet.h"
#include "KRegion.h"
#include "KNpcTemplate.h"
#include "KItemSet.h"
#ifdef _SERVER
//#include "KNetServer.h"
//#include "../MultiServer/Heaven/Interface/iServer.h"
#include "KPlayerSet.h"
#include "KSkillManager.h"
#else
#include "../../Headers/IClient.h"
#include "CoreShell.h"
#include "Scene/KScenePlaceC.h"
#include "KIme.h"
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "ImgRef.h"
#include "Text.h"
#endif
#include "KNpcAttribModify.h"
#include "CoreUseNameDef.h"
#include "KSubWorld.h"
#include "Scene/ObstacleDef.h"
#include "KThiefSkill.h"
#ifdef _STANDALONE
#include "KThiefSkill.cpp"
#endif

#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif

extern KLuaScript		*g_pNpcLevelScript;

#define	ATTACKACTION_EFFECT_PERCENT		60	// 发技能动作完成百分之多少才真正发出来
#define	MIN_JUMP_RANGE					64
#define	ACCELERATION_OF_GRAVITY			10

#define		SHOW_CHAT_WIDTH				24
#define		SHOW_CHAT_COLOR				0xffffffff
#define		SHOW_BLOOD_COLOR			0x00ff0000
#define		defMAX_SHOW_BLOOD_TIME		200
#define		defSHOW_BLOOD_MOVE_SPEED	1

#define		SHOW_LIFE_WIDTH				40
#define		SHOW_LIFE_HEIGHT			3

#define		SHOW_SPACE_HEIGHT			5

//-----------------------------------------------------------------------
#define	GAME_UPDATE_TIME		10
#define	GAME_SYNC_LOSS			100
#define	STAMINA_RECOVER_SCALE	4
// 区域的宽高（格子单位）
#define	REGIONWIDTH			defLOGIC_REGION_WIDTH
#define	REGIONHEIGHT		defLOGIC_REGION_HEIGHT
// 格子的宽高（像素单位，放大了1024倍）
#define	CELLWIDTH			(defLOGIC_CELL_WIDTH << 10)
#define	CELLHEIGHT			(defLOGIC_CELL_HEIGHT << 10)
// 当前区域
#define	CURREGION			SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex]
// 相邻区域的索引
#define	LEFTREGIONIDX		CURREGION.m_nConnectRegion[2]
#define	RIGHTREGIONIDX		CURREGION.m_nConnectRegion[6]
#define	UPREGIONIDX			CURREGION.m_nConnectRegion[4]
#define	DOWNREGIONIDX		CURREGION.m_nConnectRegion[0]
#define	LEFTUPREGIONIDX		CURREGION.m_nConnectRegion[3]
#define	LEFTDOWNREGIONIDX	CURREGION.m_nConnectRegion[1]
#define	RIGHTUPREGIONIDX	CURREGION.m_nConnectRegion[5]
#define	RIGHTDOWNREGIONIDX	CURREGION.m_nConnectRegion[7]

#define	LEFTREGION			SubWorld[m_SubWorldIndex].m_Region[LEFTREGIONIDX]
#define	RIGHTREGION			SubWorld[m_SubWorldIndex].m_Region[RIGHTREGIONIDX]
#define	UPREGION			SubWorld[m_SubWorldIndex].m_Region[UPREGIONIDX]
#define	DOWNREGION			SubWorld[m_SubWorldIndex].m_Region[DOWNREGIONIDX]
#define	LEFTUPREGION		SubWorld[m_SubWorldIndex].m_Region[LEFTUPREGIONIDX]
#define	LEFTDOWNREGION		SubWorld[m_SubWorldIndex].m_Region[LEFTDOWNREGIONIDX]
#define	RIGHTUPREGION		SubWorld[m_SubWorldIndex].m_Region[RIGHTUPREGIONIDX]
#define	RIGHTDOWNREGION		SubWorld[m_SubWorldIndex].m_Region[RIGHTDOWNREGIONIDX]

#define	CONREGION(x)		SubWorld[m_SubWorldIndex].m_Region[CURREGION.m_nConnectRegion[x]]
#define	CONREGIONIDX(x)		CURREGION.m_nConnectRegion[x]
// 当前技能


//-----------------------------------------------------------------------
// Npc[0]不在游戏世界中使用，做为一个NpcSet用于添加新的NPC。
KNpc	Npc[MAX_NPC];


KNpcTemplate	* g_pNpcTemplate[MAX_NPCSTYLE][MAX_NPC_LEVEL]; //0,0为起点

//-----------------------------------------------------------------------

KNpc::KNpc()
{
#ifdef _SERVER
	m_AiSkillRadiusLoadFlag = 0;	// 只需要在构造的时候初始化一次
#endif
	Init();
}

#ifndef _SERVER
void KNpc::DoMove(int nDestX,int nDestY,int status)
{
	if (status)
	{		
		SendCommand(do_run, nDestX, nDestY);
		SendClientCmdRun(nDestX, nDestY);
	}
	else
	{
		SendCommand(do_walk, nDestX, nDestY);
		SendClientCmdWalk(nDestX, nDestY);
	}
}
bool KNpc::AutoMove(int& nCurX,int& nCurY,int nDestX,int nDestY,int status)
{
	int x, y;
	SubWorld[0].Map2Mps(m_RegionIndex,m_MapX,m_MapY,m_OffX,m_OffY,&x,&y);// chuyen sang toa do game

	if (!m_PathFinder.CheckDistance(x, y, nDestX, nDestY, (status ? m_CurrentRunSpeed : m_CurrentWalkSpeed)*1.5f))// da den noi
	{
		return false;
	}
	if ((x == nCurX) && (y == nCurY))// dung tai cho
	{
		DoMove(nDestX,nDestY,status);
	}
	nCurX = x; nCurY = y;
	return true;
}
#endif
void KNpc::Init()
{

	m_dwID = 0;
	m_Index = 0;
	m_nPlayerIdx = 0;
	m_ProcessAI = 1;
	m_Kind = kind_normal;
	m_Series = series_metal;
	m_Camp = camp_free;
	m_CurrentCamp = camp_free;
	m_Doing = do_stand;
	m_Height = 0;
	m_Frames.nCurrentFrame = 0;
	m_Frames.nTotalFrame = 0;
	m_SubWorldIndex = 0;
	m_RegionIndex = -1;
	m_Experience = 0;
	m_ActiveSkillID = 0;
	m_SkillParam1 = 0;
	m_SkillParam2 = 0;
	m_TypeNpc = 0;

	m_NumObjT = 0;
	m_DirObjT = 0;


	m_IsBoss = 0;
	m_nShareDame = 0;
	m_nCurrentShareDame = 0;
    ZeroMemory(m_nNpcParam, sizeof(m_nNpcParam));
    m_bNpcFollowFindPath = FALSE;
    m_uFindPathTime = 0;
	m_uFindPathMaxTime = 0;
	m_uLastFindPathTime = 0;

	m_nPencentDameAdd = 0;
	m_nCurentPencentDameAdd = 0;

#ifndef _SERVER
	m_nChatContentLen = 0;
	m_nCurChatTime = 0;
	m_nChatNumLine = 0;
	m_nChatFontWidth = 0;
	m_nStature = 0;
	m_Task100 = 0;
	m_Task162 = 0;
    m_Task139 = 0;
    m_Task485 = 0;
	m_WorldStat = 0;
	m_PKValue = 0;
	m_nAutoNoActack = FALSE;
	m_nAutoOldX = 0;
	m_nAutoOldY = 0;
	m_nAutoOldLife = 0;
	m_bIsHidePlayer = TRUE;
	HanSuDung = 0;
	MangEffect[0][0] = 0;
	MangEffect[0][1] = 0;
	MangEffect[1][0] = 0;
	MangEffect[1][1] = 0;
	MangEffect[2][0] = 0;
	MangEffect[2][1] = 0;
	MangEffect[3][0] = 0;
	MangEffect[3][1] = 0;
	MangEffect[4][0] = 0;
	MangEffect[4][1] = 0;
	MangEffect[5][0] = 0;
	MangEffect[5][1] = 0;
	MangEffect[6][0] = 0;
	MangEffect[6][1] = 0;
	MangEffect[7][0] = 0;
	MangEffect[7][1] = 0;
	MangEffect[8][0] = 0;
	MangEffect[8][1] = 0;
	MangEffect[9][0] = 0;
	MangEffect[9][1] = 0;
	
#endif

	m_CurrentLife = 100;			// Npc的当前生命
	m_CurrentLifeMax = 100;		// Npc的当前生命最大值
	m_CurrentLifeReplenish = 0;	// Npc的当前生命回复速度
	m_CurrentMana = 100;			// Npc的当前内力
	m_CurrentManaMax = 100;		// Npc的当前最大内力
	m_CurrentManaReplenish = 0;	// Npc的当前内力回复速度
	m_CurrentStamina = 100;		// Npc的当前体力
	m_CurrentStaminaMax = 100;	// Npc的当前最大体力
	m_CurrentStaminaGain = 0;	// Npc的当前体力回复速度
	m_CurrentStaminaLoss = 0;	// Npc的当前体力下降速度
	m_CurrentAttackRating = 100;	// Npc的当前命中率
	m_CurrentDefend = 10;		// Npc的当前防御
	m_CurrentWalkSpeed = 5;		// Npc的当前走动速度
	m_CurrentRunSpeed = 10;		// Npc的当前跑动速度

	m_CurrentRWSpeed = 0;

	m_CurrentJumpSpeed = 12;	// Npc的当前跳跃速度
	m_CurrentJumpFrame = 40;	// Npc的当前跳跃时间
	m_CurrentAttackSpeed = 0;	// Npc的当前攻击速度
	m_CurrentCastSpeed = 0;		// Npc的当前施法速度
	m_CurrentVisionRadius = 40;	// Npc的当前视野范围
	m_CurrentAttackRadius = 30;	// Npc的当前攻击范围
	m_CurrentHitRecover = 0;	// Npc的当前受击回复速度
	m_CurrentAddPhysicsDamage = 0;	// Npc的当前物理伤害直接加的点数
	m_CurrentAddPhysicsMagic = 0;
	
	m_Dir = 0;					// Npc的方向
	m_JumpStep = 0;
	m_JumpDir = 0;			
	m_MapZ = 0;					// Npc的高度
	m_HelmType = 1;				// Npc的头盔类型
	m_ArmorType = 1;			// Npc的盔甲类型
	m_WeaponType = 1;			// Npc的武器类型
	m_HorseType = -1;			// Npc的骑马类型
	m_bRideHorse = FALSE;		// Npc是否骑马
	
	
	// mask
	m_MaskType = 0;
	m_MaskType_Temp = 0;
	m_MaskMark = 0;
	// end
	
	// fifong
	m_FifongType = 0;
	m_bFifongState = FALSE;
	// end
	
	ZeroMemory(Name, 32);		// Npc的名称
	m_NpcSettingIdx = 0;		// Npc的设定文件索引
	m_CorpseSettingIdx = 0;		// Body的设定文件索引
	ZeroMemory(ActionScript,32);
	m_ActionScriptID = 0;
	m_TrapScriptID = 0;
	m_DropScriptID = 0;

	m_btRankId					= 0;
	m_ImagePlayer  			= 0;
	m_LifeMax					= 100;		// Npc的最大生命
	m_LifeReplenish				= 0;		// Npc的生命回复速度
	m_ManaMax					= 100;		// Npc的最大内力
	m_ManaReplenish				= 0;		// Npc的内力回复速度
	m_StaminaMax				= 100;		// Npc的最大体力
	m_StaminaGain				= 0;		// Npc的体力回复速度
	m_StaminaLoss				= 0;		// Npc的体力下降速度
	m_AttackRating				= 100;		// Npc的命中率
	m_Defend					= 10;		// Npc的防御
	m_WalkSpeed					= 6;		// Npc的行走速度
	m_RunSpeed					= 10;		// Npc的跑动速度
	m_JumpSpeed					= 12;		// Npc的跳跃速度
	m_AttackSpeed				= 0;		// Npc的攻击速度
	m_CastSpeed					= 0;		// Npc的施法速度
	m_VisionRadius				= 40;		// Npc的视野范围
	m_DialogRadius				= 124;		// Npc的对话范围
	m_HitRecover				= 12;		// Npc的受击回复速度
	m_nPeopleIdx				= 0;

	m_LoopFrames				= 0;
	m_WalkFrame					= 12;
	m_RunFrame					= 15;
	m_StandFrame				= 15;
	m_DeathFrame				= 15;
	m_HurtFrame					= 10;
	m_AttackFrame				= 20;
	m_CastFrame					= 20;
	m_SitFrame					= 15;
	m_JumpFrame					= 40;
	m_AIMAXTime					= 25;
	m_NextAITime				= 0;
	m_ProcessState				= 1;
	m_ReviveFrame				= 100;
	m_bExchangeServer			= FALSE;
	m_bActivateFlag				= FALSE;
	m_FightMode					= 0;
	m_OldFightMode				= 0;
    m_ValueEx					= 0;  // van tieu

#ifdef _SERVER
	m_RateBoss = 0;
	m_nNextStatePos				= 0;
	m_pDropRate = NULL;

	m_nCurConutTimeSyn = 0;
#endif

#ifndef _SERVER

	ZeroMemory(m_TongName, 32);
	ZeroMemory(m_TongRank, 32);

	ZeroMemory(m_szNameOpenShop,32);

	m_dwTimeOpenShop = 0;


	m_nPeopleIdxCheckClient = 0;


	m_SyncSignal				= 0;
	m_sClientNpcID.m_dwRegionID	= 0;
	m_sClientNpcID.m_nNo		= -1;
	m_ResDir					= 0;
	m_nPKFlag					= 0;
	m_nSleepFlag				= 0;
	memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
	for (int i=0;i<MAX_FLOOD_SHOW;i++)
	{
	m_nBloodNo[i]					= 0;
	m_nBloodAlpha[i]			= 0;
	m_nBloodTime[i]				= 0;
	m_szBloodNo[i][0]				= 0;
	}

	m_nTongFlag					= 0;
#endif

	m_nLastPoisonDamageIdx = 0;
	m_nLastDamageIdx = 0;
	m_bHaveLoadedFromTemplate = FALSE;
	m_bClientOnly = FALSE;
}

ISkill* KNpc::GetActiveSkill()
{
	_ASSERT(m_ActiveSkillID < MAX_SKILL);
	int nCurLevel = m_SkillList.GetCurrentLevel(m_ActiveSkillID);
	if (nCurLevel > 0)
		return g_SkillManager.GetSkill(m_ActiveSkillID, nCurLevel);
	else 
		return NULL;
};


void KNpc::SetCurrentCamp(int nCamp)
{
	m_CurrentCamp = nCamp;

#ifdef _SERVER
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};

	if (m_RegionIndex < 0)
		return;

	NPC_CHGCURCAMP_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcchgcurcamp;
	NetCommand.ID = m_dwID;
	NetCommand.Camp = (BYTE)m_CurrentCamp;

	int	nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}

void KNpc::SetCamp(int nCamp)
{
	m_Camp = nCamp;
#ifdef _SERVER
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	if (m_RegionIndex < 0)
		return;

	NPC_CHGCAMP_SYNC	NetCommand;

	NetCommand.ProtocolType = (BYTE)s2c_npcchgcamp;
	NetCommand.ID = m_dwID;
	NetCommand.Camp = (BYTE)m_Camp;

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}

void KNpc::RestoreCurrentCamp()
{
	m_CurrentCamp = m_Camp;
#ifdef _SERVER
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	if (m_RegionIndex < 0)
		return;

	NPC_CHGCURCAMP_SYNC	NetCommand;
	
	NetCommand.ProtocolType = (BYTE)s2c_npcchgcurcamp;
	NetCommand.ID = m_dwID;
	NetCommand.Camp = (BYTE)m_CurrentCamp;
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}

#define		NPC_SHOW_CHAT_TIME		15000
int		IR_IsTimePassed(unsigned int uInterval, unsigned int& uLastTimer);

void KNpc::Activate()
{
	// 不存在这个NPC
	if (!m_Index)
	{
		//g_DebugLog("[DEATH] No Index: %d", m_Index);
		return;
	}

	// 切换地图中，不处理
	if (m_bExchangeServer)
	{
		//g_DebugLog("[DEATH] Change Server: %d", m_bExchangeServer);
		return;
	}

	// Check here
	if (m_bActivateFlag)
	{
		m_bActivateFlag = FALSE;	// restore flag
		return;
	}
//	m_bActivateFlag = TRUE;

	m_LoopFrames++;
	// Process npc special state, such as curse, etc.
	//g_DebugLog("[DEATH] m_ProcessState: %d", m_ProcessState);
	if (m_ProcessState)
	{
		if (ProcessState() && (m_Command.CmdKind != do_skill || m_Command.Param_X != 416))
			return;
	}

	if (m_ProcessAI)
	{
		NpcAI.Activate(m_Index);
	}
	else
	{
		NpcAI.NotActivate(m_Index);

	}

#ifdef _SERVER
	ProcCommandFix(m_ProcessAI);
	
	
	
	int Map = SubWorld[m_SubWorldIndex].m_SubWorldID;
		if (m_Kind == kind_player)
	{
		
		/*
		if (Map == 60)
		{
			if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(12) == 1)
			{
				if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(19) == 1)
					m_MaskType = 839;
				else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(19) == 2)
					m_MaskType = 849;
				else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(19) == 3)
					m_MaskType = 859;
				else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(19) == 4)
					m_MaskType = 869;
				else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(19) == 5)
					m_MaskType = 879;
				else	
					m_MaskType = 0;
			}
			else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(12) == 2)
			{
				if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(19) == 1)
					m_MaskType = 844;
				else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(19) == 2)
					m_MaskType = 854;
				else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(19) == 3)
					m_MaskType = 864;
				else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(19) == 4)
					m_MaskType = 874;
				else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(19) == 5)
					m_MaskType = 884;
				else	
					m_MaskType = 0;
			}
			else	
				m_MaskType = 0;
			}
			*/
			
	//	if (Map == 258)
	//		Player[m_nPlayerIdx].SetChatForbiddenFlag(1);

	}
	
#endif

	ProcCommand(m_ProcessAI);
	ProcStatus();

	
	
	// mask
	if (m_MaskType > 0 && m_MaskMark != 0 && m_MaskMark != m_MaskType)
	{
		ReSetRes(1);
		m_MaskMark = 0;
	}
	else if (m_MaskType > 0 && !m_MaskMark)
	{
		ReSetRes(0);
		m_MaskMark = m_MaskType;
		
	}
	else if (m_MaskType == 0 && m_MaskMark)
	{
		ReSetRes(1);
		m_MaskMark = 0;
	}
	// end
	// fifong
	if (m_FifongType > 0)
	{
		m_bFifongState = TRUE;
	}
	else
	{
		m_bFifongState = FALSE;
	}
	// end
	
#ifdef _SERVER
	this->m_cDeathCalcExp.Active();
#endif

#ifndef _SERVER

	if (m_RegionIndex == -1)
		return;

    HurtAutoMove();	
	
	

	int Map = SubWorld[m_SubWorldIndex].m_SubWorldID;
	
	if (Player[CLIENT_PLAYER_INDEX].m_HideGame1)
	{
		if (Player[CLIENT_PLAYER_INDEX].m_nIndex != m_Index && Npc[m_Index].m_Kind == kind_player)
		{
				m_MaskType = 1335;	
		}
	}
	else
	{
		if ((Map == 33 || Map == 258) && Player[CLIENT_PLAYER_INDEX].m_nIndex != m_Index && Npc[m_Index].m_Kind == kind_player)
				m_MaskType = 1335;
		else
			m_MaskType = 0;
	}
	

	
	if (m_MaskMark > 0)
		GetFrameCopyFromTemplate(m_MaskType, m_Level);
	// end
	
	AutoFixXY();
	int		nMpsX, nMpsY;

	m_DataRes.SetAction(m_ClientDoing);
	m_DataRes.SetRideHorse(m_bRideHorse);
	// m_DataRes.SetArmor(m_ArmorType,0,0);
	// fifong
	if (m_bFifongState == TRUE)
			m_DataRes.SetArmor(m_ArmorType, m_FifongType, TRUE);
		else
			m_DataRes.SetArmor(m_ArmorType, 0, FALSE);
	// end
	m_DataRes.SetHelm(m_HelmType);
	m_DataRes.SetHorse(m_HorseType);
	
	m_DataRes.SetWeapon(m_WeaponType);	
	
	// 处理技能产生的状态的特效
	m_DataRes.SetState(&m_StateSkillList, &g_NpcResList);

	if (Player[CLIENT_PLAYER_INDEX].m_nIndex == m_Index)
	{
		SubWorld[0].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
		m_DataRes.SetPos(m_Index, nMpsX, nMpsY, m_Height, TRUE);
	}
	else
	{
		SubWorld[0].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
		m_DataRes.SetPos(m_Index, nMpsX, nMpsY, m_Height, FALSE);
	}

	// client npc 时间计数处理：不往后跳
	if (m_Kind == kind_bird || m_Kind == kind_mouse)
		m_SyncSignal = SubWorld[0].m_dwCurrentTime;

	if (m_nChatContentLen > 0)
	{
		if (IR_GetCurrentTime() - m_nCurChatTime > NPC_SHOW_CHAT_TIME)
		{
			m_nChatContentLen = 0;
			m_nChatNumLine = 0;
			m_nChatFontWidth = 0;
			m_nCurChatTime = 0;
		}
	}
	
#endif
}

void KNpc::ProcStatus()
{
	//g_DebugLog("[DEATH] m_bExchangeServer: %d", m_bExchangeServer);
	if (m_bExchangeServer)
		return;

	switch(m_Doing)
	{
	case do_stand:
		OnStand();
		break;
	case do_run:
		OnRun();
		break;
	case do_walk:
		OnWalk();
		break;
	case do_attack:
	case do_magic:
		OnSkill();
		break;
	case do_sit:
		OnSit();
		break;
	case do_jump:
		OnJump();
		break;
	case do_hurt:
		OnHurt();
		break;
	case do_revive:
		OnRevive();
		break;
	case do_death:
		OnDeath();
		break;
	case do_defense:
		OnDefense();
		break;
	case do_special1:
		OnSpecial1();
		break;
	case do_special2:
		OnSpecial2();
		break;
	case do_special3:
		OnSpecial3();
		break;
	case do_special4:
		OnSpecial4();
		break;
	case do_manyattack:
		OnManyAttack();
		break;
	case do_runattack:
		OnRunAttack();
		break;

	case do_goattack:
		OnGoAttack();
		break;


	case do_jumpattack:
		OnJumpAttack();
		break;
	case do_idle:
		OnIdle();
	default:
		break;
	}
}

#ifdef _SERVER
void KNpc::ProcCommandFix(int nAI)
{

	if (m_NowCommand.CmdKind == do_none || m_bExchangeServer || m_Doing == do_death || m_Doing == do_revive)
		return;

	if (!nAI || m_RegionIndex < 0 || !IsPlayer())
		return;



if (m_Command.CmdKind == do_revive)
{
m_NowCommand.CmdKind = do_none;
}


if (m_NowCommand.CmdKind == do_skill)
{

int nX0,nY0,nX1,nY1,nDistance,nRadius;
GetMpsPos(&nX0,&nY0);


if (m_NowCommand.Param_Z <= 0) 
		return;

	if (m_NowCommand.Param_Y < 0)
	{
		if (m_NowCommand.Param_Y != -1) 
			return;

		if (m_NowCommand.Param_Z >= MAX_NPC || Npc[m_NowCommand.Param_Z].m_dwID == 0 || Npc[m_NowCommand.Param_Z].m_SubWorldIndex != m_SubWorldIndex)
			return;

		Npc[m_NowCommand.Param_Z].GetMpsPos(&nX1, &nY1);

    }
	else
	{
	nX1 = m_NowCommand.Param_Y;
	nY1 = m_NowCommand.Param_Z;
	}

nDistance = SubWorld[m_SubWorldIndex].GetDistance(nX0, nY0, nX1, nY1);
nRadius = m_nNpcRadius;



if (m_nNpcX > 0 && m_nNpcY > 0 && m_nNpcRadius > 0 && nRadius < nDistance && nRadius < 200)
{
	int nRet = CheckPath(m_nNpcX, m_nNpcY, m_CurrentRunSpeed);
	if (nRet == 1)
	{
			if (m_CurrentStamina > 0 || Player[m_nPlayerIdx].m_cPK.GetNormalPKState() != 2 && m_Doing != do_run)
			{
			SendCommand(do_run, m_nNpcX, m_nNpcY);
			}
			else if (m_Doing != do_walk)
			{
			SendCommand(do_walk, m_nNpcX, m_nNpcY);
		}
	}
	else 
	{
		SendCommand(m_NowCommand.CmdKind, m_NowCommand.Param_X, m_NowCommand.Param_Y, m_NowCommand.Param_Z);
        m_NowCommand.CmdKind = do_none;
	}
	}
}

}

#endif
#ifndef _SERVER
void	KNpc::RunWalkStopCmd()  // loi phu ve ko tu di chuyen
{
	int nMovePox_X, nMovePox_Y; //#dung lai
	GetMpsPos(&nMovePox_X,&nMovePox_Y);
	SendCommand(do_run, nMovePox_X,nMovePox_Y);
	SendClientCmdRun(nMovePox_X,nMovePox_Y);
}
#endif


void KNpc::ProcCommand(int nAI)
{
	// CmdKind < 0 表示没有指令	交换地图也不处理
	if (m_Command.CmdKind == do_none || m_bExchangeServer)
		return;

	if (nAI)
	{
		if (m_RegionIndex < 0)
			return;
		switch (m_Command.CmdKind)
		{
		case do_stand:
			DoStand();
			break;
		case do_walk:
			Goto(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_run:
			RunTo(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_ride:
			DoRide();
			break;
		case do_jump:
			JumpTo(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_skill:
			if (int nSkillIdx = m_SkillList.FindSame(m_Command.Param_X))
			{
				SetActiveSkill(nSkillIdx);
				DoSkill(m_Command.Param_Y, m_Command.Param_Z);
			}
			else
			{
				DoStand();
			}
			break;
		case do_sit:
			DoSit();
			break;
		case do_defense:
			DoDefense();
			break;
		case do_idle:
			DoIdle();
			break;
		case do_hurt:
			DoHurt(m_Command.Param_X, m_Command.Param_Y, m_Command.Param_Z);
			break;	
			// 因为跨地图能把ai设为1
		case do_revive:
			DoStand();
			m_ProcessAI = 1;
			m_ProcessState = 1;
#ifndef _SERVER
			this->SetInstantSpr(enumINSTANT_STATE_REVIVE);
#endif
			break;
		}
	}
	else
	{
		switch(m_Command.CmdKind)
		{

         case do_skill:
		 



#ifdef _SERVER


			 if (m_Command.Param_X == 416)
			 {

			if (int nSkillIdx = m_SkillList.FindSame(m_Command.Param_X))
			{
				SetActiveSkill(nSkillIdx);
				DoSkill(m_Command.Param_Y, m_Command.Param_Z);
			}

			 }

#endif



#ifndef _SERVER


			  if (m_Command.Param_X == 416 || Player[CLIENT_PLAYER_INDEX].m_nIndex != m_Index)
			 {

			if (int nSkillIdx = m_SkillList.FindSame(m_Command.Param_X))
			{
				SetActiveSkill(nSkillIdx);
				DoSkill(m_Command.Param_Y, m_Command.Param_Z);
			}

			 }



#endif





			break;


		case do_hurt:
			if (m_RegionIndex >= 0)
				DoHurt(m_Command.Param_X, m_Command.Param_Y, m_Command.Param_Z);
			break;
		case do_revive:
			DoStand();
			m_ProcessAI = 1;
			m_ProcessState = 1;
#ifndef _SERVER
			this->SetInstantSpr(enumINSTANT_STATE_REVIVE);
#endif
			break;
		default:
			break;
		}
	}
	m_Command.CmdKind = do_none;
}

BOOL KNpc::ProcessState()
{
	int nRet = FALSE;
	if (m_RegionIndex < 0)
		return FALSE;

	if (!(m_LoopFrames % GAME_UPDATE_TIME))
	{
// 生命、内力、体力变化只由服务器计算
#ifdef _SERVER
// BroadCastState();
		// 打坐中
		if (m_Doing == do_sit)
		{
			int nLifeAdd = m_CurrentLifeMax * 10 / 1000;

			if (m_Level < 80)
				nLifeAdd = m_CurrentLifeMax * 10 / 100;

			if (nLifeAdd <= 0)
				nLifeAdd = 1;
			m_CurrentLife += nLifeAdd;
			if (m_CurrentLife > m_CurrentLifeMax)
				m_CurrentLife = m_CurrentLifeMax;

			int nManaAdd = m_CurrentManaMax * 10 / 1000;
			if (m_Level < 80)
				nManaAdd = m_CurrentManaMax * 10 / 100;

			if (nManaAdd <= 0)
				nManaAdd = 1;
			m_CurrentMana += nManaAdd;
			if (m_CurrentMana > m_CurrentManaMax)
				m_CurrentMana = m_CurrentManaMax;

//			if (m_CurrentLife >= m_CurrentLifeMax && m_CurrentMana >= m_CurrentManaMax)
//				this->SendCommand(do_stand);
		}
		// 生命自然回复
	
	    if (m_StunState.nTime <= 0)
		m_CurrentLife += m_CurrentLifeReplenish * (100+m_CurrentLifeStolenAddPercent)/100;
		if (m_CurrentLife > m_CurrentLifeMax)
			m_CurrentLife = m_CurrentLifeMax;
		if (m_CurrentLife < 0)
            m_CurrentLife = 0;

		// 内力自然回复
		if (m_StunState.nTime <= 0)
		m_CurrentMana += m_CurrentManaReplenish;
		if (m_CurrentMana > m_CurrentManaMax)
			m_CurrentMana = m_CurrentManaMax;
		if (m_CurrentMana < 0)
            m_CurrentMana = 0;
		// 体力自然回复
		
		if (IsPlayer())
		{

		if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 2)
		{
            if (m_Doing == do_run)
			m_CurrentStamina -= PLAYER_STAMINA_LOSS;
		}
		else if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 0)
		{
		if (m_Doing == do_run)
			m_CurrentStamina += m_CurrentStaminaGain + PLAYER_STAMINA_LOSS / STAMINA_RECOVER_SCALE;
		else
			m_CurrentStamina += m_CurrentStaminaGain + PLAYER_STAMINA_LOSS ;
		}
		else
		{
		m_CurrentStamina += m_CurrentStaminaGain;
		}


		}
		else
		{
			if (m_Doing == do_run)
				m_CurrentStamina += m_CurrentStaminaGain / STAMINA_RECOVER_SCALE;
			else
				m_CurrentStamina += m_CurrentStaminaGain;
		}

		if (m_CurrentStamina > m_CurrentStaminaMax)
			m_CurrentStamina = m_CurrentStaminaMax;
		if (m_CurrentStamina < 0)
            m_CurrentStamina = 0;

#endif
		// 光环技能
	
		if (m_ActiveAuraID)
		{
			int nLevel = m_SkillList.GetCurrentLevel(m_ActiveAuraID);
			if (nLevel > 0)
			{
				int nMpsX, nMpsY;
				SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
				_ASSERT(m_ActiveAuraID < MAX_SKILL && nLevel < MAX_SKILLLEVEL);
#ifdef _SERVER
				NPC_SKILL_SYNC SkillCmd;
				SkillCmd.ID = this->m_dwID;
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_ActiveAuraID, nLevel);
				if (pOrdinSkill)
                {
			//		if (pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles)
					SkillCmd.nSkillID = pOrdinSkill->GetChildSkillId();
                }
				else
                {
					SkillCmd.nSkillID = 0;
                }

				SkillCmd.nSkillLevel = nLevel;
				SkillCmd.nMpsX = -1;
				SkillCmd.nMpsY = m_dwID;
				SkillCmd.ProtocolType = s2c_castskilldirectly;
				POINT	POff[8] = 
				{
					{0, 32},
					{-16, 32},
					{-16, 0},
					{-16, -32},
					{0, -32},
					{16, -32},
					{16, 0},
					{16, 32},
				};
				
				int nMaxCount = MAX_BROADCAST_COUNT;
				CURREGION.BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX, m_MapY);
				int i;
				for (i = 0; i < 8; i++)
				{
					if (CONREGIONIDX(i) == -1)
						continue;
					CONREGION(i).BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
				}
#endif				
				KSkill * pOrdinSkill1 = (KSkill *) g_SkillManager.GetSkill(m_ActiveAuraID, nLevel);
				int nChildSkillId = 0;
				if (pOrdinSkill1)
				{
					nChildSkillId = pOrdinSkill1->GetChildSkillId();
                    
					KSkill * pOrdinSkill2 = (KSkill *) g_SkillManager.GetSkill(nChildSkillId, nLevel);
					if (pOrdinSkill2)
                    {
						pOrdinSkill2->Cast(m_Index, nMpsX, nMpsY);
                    }
				}
			}
		}
	
	}

	
OnMadnessState();


#ifdef _SERVER

	if (m_NstatusState.nTime > 0)
	{
		m_NstatusState.nTime--;
	}

    if (m_NstatusState.nTime > 0)
	    m_PoisonState.nTime = 0;
	else if (m_PoisonState.nTime > 0)
	{
		m_PoisonState.nTime--;
		if (m_PoisonState.nValue[1] == 0)
		{
			m_PoisonState.nValue[1] = 1;
		}
		if (!(m_PoisonState.nTime % m_PoisonState.nValue[1]))
		{

			if (Npc[m_nLastPoisonDamageIdx].m_dwID)
			{

			if (relation_enemy == NpcSet.GetRelation(Npc[m_nLastPoisonDamageIdx].m_Index,m_Index))
			{
			CalcDamage(m_nLastPoisonDamageIdx, m_PoisonState.nValue[0], m_PoisonState.nValue[0], damage_poison, FALSE, FALSE, FALSE);
			}
			else
			{
			m_PoisonState.nTime = 0;
			}




			}
			else
			{
			m_PoisonState.nTime = 0;
			}

		}
	}
	// 冰冻状态
	if (m_NstatusState.nTime > 0 || m_Doing == do_goattack)
	    m_FreezeState.nTime = 0;
	else if (m_FreezeState.nTime > 0)
	{
		m_FreezeState.nTime--;
		if (m_FreezeState.nTime & 1)
		{
			nRet = TRUE;
		}
	}
	// 燃烧状态
	if (m_NstatusState.nTime > 0)
	    m_BurnState.nTime = 0;
	else if (m_BurnState.nTime > 0)
	{
		m_BurnState.nTime--;
		if (m_BurnState.nValue[1] == 0)
		{
			m_BurnState.nValue[1] = 1;
		}
		if (!(m_BurnState.nTime % m_BurnState.nValue[1]))
		{
			CalcDamage(m_Index, m_BurnState.nValue[0], m_BurnState.nValue[0], damage_fire, TRUE, FALSE);
		}
	}
	// 混乱状态
	if (m_NstatusState.nTime > 0)
	    m_ConfuseState.nTime = 0;
	else if (m_ConfuseState.nTime > 0)
	{
		m_ConfuseState.nTime--;
		if (m_ConfuseState.nTime <= 0)
		{
			m_CurrentCamp = m_Camp;
		}
	}
	// 眩晕状态
	if (m_NstatusState.nTime > 0 || m_Doing == do_goattack)
	    m_StunState.nTime = 0;
	else if (m_StunState.nTime > 0)
	{
		m_StunState.nTime--;
		nRet = TRUE;
	}

	if (m_InvisibilityState.nTime > 0)
	{
		m_InvisibilityState.nTime--;
	}


	if (m_NstatusState.nTime > 0)
	    m_MadnessState.nTime = 0;
	else if (m_MadnessState.nTime > 0)
	{
		m_MadnessState.nTime--;
	}



    if (m_CurrentTimeCastSkillEffec.nTime > 0)
	{
	m_CurrentTimeCastSkillEffec.nTime--;
	}

	if (m_CurrentTimeCastSkillEffec1.nTime > 0)
	{
	m_CurrentTimeCastSkillEffec1.nTime--;
	}

	if (m_CurrentTimeCastSkillEffec2.nTime > 0)
	{
	m_CurrentTimeCastSkillEffec2.nTime--;
	}

	// 补血状态
	if (m_LifeState.nTime > 0)
	{
		m_LifeState.nTime--;
		if (!(m_LifeState.nTime % GAME_UPDATE_TIME))
		{
			if (m_StunState.nTime <= 0)
			m_CurrentLife += m_LifeState.nValue[0] * (100+m_CurrentLifeStolenAddPercent)/100;
			if (m_CurrentLife > m_CurrentLifeMax)
			{
				m_CurrentLife = m_CurrentLifeMax;
			}
		}
	}
	// 补MANA状态
	if (m_ManaState.nTime > 0)
	{
		m_ManaState.nTime--;
		if (!(m_ManaState.nTime % GAME_UPDATE_TIME))
		{
			if (m_StunState.nTime <= 0)
			m_CurrentMana += m_ManaState.nValue[0];
			if (m_CurrentMana > m_CurrentManaMax)
			{
				m_CurrentMana = m_CurrentManaMax;
			}
		}
	}
	// 醉酒状态
	if (m_DrunkState.nTime > 0)
	{
		m_DrunkState.nTime--;
	}
#endif

int Map = SubWorld[m_SubWorldIndex].m_SubWorldID;
if (m_CurrentLife == 0 && !m_FightMode && (Map == 33 || Map == 37))
{
		OnDeath();
}	


#ifndef _SERVER
		
	bool bAdjustColorId = false;
	if (m_FreezeState.nTime > 0)
	{
		if (SubWorld[0].m_dwCurrentTime & 1)
			nRet = TRUE;
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_freeze);
		bAdjustColorId = true;
	}
	
    if (m_StunState.nTime > 0)
	{
		nRet = TRUE;
	}

	if (m_PoisonState.nTime > 0)
	{
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_poison);
		bAdjustColorId = true;
	}


	// 燃烧状态
	if (m_BurnState.nTime > 0)
	{
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_burn);
		bAdjustColorId = true;
	}
	// 混乱状态
	if (m_ConfuseState.nTime > 0)
	{
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_confuse);
		bAdjustColorId = true;
	}

	if (m_InvisibilityState.nTime > 0)
	{
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_invisibility);
		bAdjustColorId = true;
	}


	if (!bAdjustColorId)
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_physics);
#endif

	KStateNode* pNode;
	pNode = (KStateNode *)m_StateSkillList.GetHead();
	while(pNode)
	{
		KStateNode* pTempNode = pNode;
		pNode = (KStateNode *)pNode->GetNext();

		if (pTempNode->m_LeftTime == -1)	// 被动技能
			continue;

		if (pTempNode->m_LeftTime == 0)
		{
			int i;
			for (i = 0; i < MAX_SKILL_STATE; i++)
			{
				if (pTempNode->m_State[i].nAttribType)
					ModifyAttrib(m_Index, &pTempNode->m_State[i]);
			}
			_ASSERT(pTempNode != NULL);
			pTempNode->Remove();
	
			delete pTempNode;
#ifndef _SERVER			
			for (int t=0; t < 10; t++)
			{
				if (MangEffect[t][0] == pTempNode->m_SkillID && pTempNode->m_SkillID != 468 && pTempNode->m_SkillID != 494)
				{
					for (int u=t; u < 10; u++)
					{
						if (MangEffect[u][0] != NULL && MangEffect[u+1] != NULL)
						{
							MangEffect[u][0] = MangEffect[u+1][0];
							MangEffect[u][1] = MangEffect[u+1][1];
						}
					}
				}
			}
//xxxa			
#endif		
#ifdef _SERVER
			UpdateNpcStateInfo();
			BroadCastState();
#endif
			pTempNode = NULL;
			continue;
		}
		else
		{
		if (pTempNode->m_SkillID != 468)
			pTempNode->m_LeftTime --;

		if (pTempNode->m_SkillID == 329 || pTempNode->m_SkillID == 339)
		{
			if (!m_FightMode)
			{
				pTempNode->m_LeftTime = 0;	
			}
		}
		
		if (pTempNode->m_SkillID >= 481 && pTempNode->m_SkillID <= 490)
		{
			int Map = SubWorld[Npc[m_Index].m_SubWorldIndex].m_SubWorldID;	
			if (Map != 33 && Map != 37)
			{
				pTempNode->m_LeftTime = 0;	
			}
		}
#ifndef _SERVER		
	if (pTempNode->m_SkillID == 468)
			HanSuDung = pTempNode->m_LeftTime;
	int vt = -1;
			for (int i=0; i < 10; i++)
			{
				if (MangEffect[i][0] == pTempNode->m_SkillID && pTempNode->m_SkillID != 468 && pTempNode->m_SkillID != 494)
				{
					vt = i;
					break;
				}
			}
			
			if (vt == -1)
			{
				for (int j=0; j < 10; j++)
				{
					if (MangEffect[j][0] == 0 && pTempNode->m_SkillID != 468&& pTempNode->m_SkillID != 494)
					{
						MangEffect[j][0] = pTempNode->m_SkillID;
						MangEffect[j][1] = pTempNode->m_LeftTime;
						break;
					}
				}
			}
			else
			{
				MangEffect[vt][1] = pTempNode->m_LeftTime;
			}
#endif
		}
	}
	return nRet;

}


void KNpc::DoRide()
{

#ifdef _SERVER

if (IsPlayer())
{







int ItemActiveIdx = Player[m_nPlayerIdx].m_ItemList.GetEquipment(itempart_horse);
if (ItemActiveIdx != 0)
{
	int ActiveMagic = Player[m_nPlayerIdx].m_ItemList.GetEquipEnhanceP(itempart_horse);
		if (!m_bRideHorse)
		{


	if (g_SubWorldSet.GetGameTime() - Player[m_nPlayerIdx].m_dwTimeRideHorse >= (2*18))
	{
	Player[m_nPlayerIdx].m_dwTimeRideHorse = g_SubWorldSet.GetGameTime();
	}
	else
	{
	KPlayerChat::SendSystemInfo(1,m_nPlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Kh玭g th?l猲 ng鵤, 2 gi﹜ !",strlen("Kh玭g th?l猲 ng鵤, 2 gi﹜ !") );
	return;
	}



	        Item[ItemActiveIdx].ApplyMagicAttribToNPC(this,ActiveMagic);
			m_bRideHorse = TRUE;
		}
		else
		{


	if (g_SubWorldSet.GetGameTime() - Player[m_nPlayerIdx].m_dwTimeRideHorse >= (1*18))
	{
	Player[m_nPlayerIdx].m_dwTimeRideHorse = g_SubWorldSet.GetGameTime();
	}
	else
	{
	KPlayerChat::SendSystemInfo(1,m_nPlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Kh玭g th?xu鑞g ng鵤, 1 gi﹜ !",strlen("Kh玭g th?xu鑞g ng鵤, 1 gi﹜ !") );
	return;
	}

			



			Item[ItemActiveIdx].RemoveMagicAttribFromNPC(this,ActiveMagic);
			m_bRideHorse = FALSE;
		}
}
}

#endif
}

void KNpc::DoDeath(int nMode/* = 0*/,int nAttacker)
{
//	_ASSERT(m_Doing != do_death);
	// do_death == 10
	//g_DebugLog("[DEATH] m_Doing: %d", m_Doing);
	//_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

	if (m_Doing == do_death)
		return;

	if (IsPlayer() && !m_FightMode)	// 城镇内不会死亡
	{
		m_CurrentLife = 1;
		return;
	}
	// mask
	if (IsPlayer())
	{
		Player[m_nPlayerIdx].m_ItemList.SetMaskLock(FALSE);
		int nIdx = Player[m_nPlayerIdx].m_ItemList.GetEquipment(itempart_mask);
		m_MaskType = Item[nIdx].GetBaseMagic();
	}
// end

#ifdef _SERVER
	if (nAttacker > 0)
	{
		int nSkillLevel = m_SkillList.GetCurrentLevel(426);
		
		if (nSkillLevel > 0)
		{
			KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(479,nSkillLevel);
			if (pSkillActive)
			{
				
				KMagicAttrib* pMagicAttrib = pSkillActive->GetStateAttribs();
				int pMagicAttribNum = pSkillActive->GetStateAttribsNum();
				if (pMagicAttrib[0].nValue[1] > 0 && Npc[nAttacker].m_MadnessState.nTime <= 0)
				{
					Npc[nAttacker].m_MadnessState.nTime = pMagicAttrib[0].nValue[1];
					Npc[nAttacker].SetStateSkillEffect(m_Index, 479, 1, pMagicAttrib, pMagicAttribNum,pMagicAttrib[0].nValue[1]);
				}
			}
		}
	}
#endif

#ifndef _SERVER
	if (this->m_Kind == kind_normal)
		this->SetBlood(this->m_CurrentLife);
#endif

	m_Doing = do_death;
	m_ProcessAI	= 0;
	m_ProcessState = 0;

	m_Frames.nTotalFrame = m_DeathFrame;
	m_Frames.nCurrentFrame = 0;
	
	m_Height = 0;

#ifdef _SERVER
	int nPlayer = 0;
	// 杀死玩家不得经验
	if (this->m_Kind != kind_player && nAttacker > 0 && nAttacker < MAX_NPC)
	{
		nPlayer = m_cDeathCalcExp.CalcExp(nAttacker);
	}

	//丢物品
	DeathPunish(nMode, nPlayer);
	
	if (this->m_Kind == kind_normal)
	{
		if (m_DropScriptID && nPlayer)
			Player[nPlayer].ExecuteScript(m_DropScriptID, "DropRate", m_Index);

		if (m_ActionScriptID)
		{
			if (nPlayer)
				Player[nPlayer].ExecuteScript(m_ActionScriptID, "LastDamage", m_Index);
		}
	}

	if (this->m_Kind != kind_player && nPlayer > 0)
	{
		if (ActionScript[0])
		{
			char szNpcIndex[30];
			sprintf(szNpcIndex, "%d", nPlayer);
			Player[nPlayer].ExecuteScript(ActionScript, "OnDrop", m_Index);		
		}
	}

	if (IsPlayer())
	{
		if (m_nPlayerIdx > 0 && m_nPlayerIdx < MAX_PLAYER)
		{

			if (Player[m_nPlayerIdx].CanSave())
			{
				if (Player[m_nPlayerIdx].Save())
				{
					Player[m_nPlayerIdx].m_uMustSave = SAVE_REQUEST;
				}
			}
		}
	}
	
	if (this->m_Kind == kind_player)
	{
		if (Player[m_nPlayerIdx].m_dwDeathScriptId)
			Player[m_nPlayerIdx].ExecuteScript(Player[m_nPlayerIdx].m_dwDeathScriptId, "OnDeath", m_nLastDamageIdx);
	}

	NPC_DEATH_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcdeath;
	NetCommand.ID = m_dwID;

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
#ifndef _SERVER
	m_ClientDoing = cdo_death;
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx == m_Index)
	{
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdxCheckClient = 0;
	}
	for (int t=0; t < 10; t++)
	{
			MangEffect[t][0] = 0;
			MangEffect[t][1] = 0;		
	}
#endif
	if (IsPlayer())
	{
#ifdef _SERVER
		if (!Npc[nAttacker].IsPlayer())
		{
			if (Player[m_nPlayerIdx].m_cTeam.m_nFlag && Player[m_nPlayerIdx].m_cTeam.m_nID >= 0)
			{
				PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
				sLeaveTeam.ProtocolType = c2s_teamapplyleave;
				Player[m_nPlayerIdx].LeaveTeam((BYTE*)&sLeaveTeam);
			}
		}
#endif
	}
}

void KNpc::OnDeath()
{
	if (WaitForFrame())
	{
		//g_DebugLog("[DEATH] WaitForFrame TRUE");
		m_Frames.nCurrentFrame = m_Frames.nTotalFrame - 1;		// 保证不会有重回第一帧的情况
#ifndef _SERVER
		int		nTempX, nTempY;
		KObjItemInfo	sInfo;

		SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nTempX, &nTempY);
		sInfo.m_nItemID = 0;
		sInfo.m_nItemWidth = 0;
		sInfo.m_nItemHeight = 0;
		sInfo.m_nMoneyNum = 0;
		sInfo.m_nColorID = 0;
		sInfo.m_nMovieFlag = 0;
		sInfo.m_nSoundFlag = 0;
		sInfo.m_szName[0] = 0;
		ObjSet.ClientAdd(0, m_CorpseSettingIdx, 0, m_Dir, 0, nTempX, nTempY, sInfo);
		m_ProcessAI = 1;
#else
		//如果当间死者为主角，则看有没有死亡脚本，有则运行。

		if (IsPlayer() && Npc[m_nLastDamageIdx].IsPlayer())
		{
			char szMessageDeath[64];
			if (m_nPlayerIdx > 0)
			{
				sprintf(szMessageDeath,"B筺  b?%s nh tr鋘g thng",Npc[m_nLastDamageIdx].Name);
				KPlayerChat::SendSystemInfo(1, m_nPlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, szMessageDeath,strlen(szMessageDeath) );
				
				if (Player[m_nPlayerIdx].m_cTeam.m_nFlag && Player[m_nPlayerIdx].m_cTeam.m_nID >= 0)
				{
					PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
					sLeaveTeam.ProtocolType = c2s_teamapplyleave;
					Player[m_nPlayerIdx].LeaveTeam((BYTE*)&sLeaveTeam);
				}
			}
			
			if (Npc[m_nLastDamageIdx].m_nPlayerIdx > 0)
			{
				sprintf(szMessageDeath,"B筺  nh tr鋘g thng %s",Name);
				KPlayerChat::SendSystemInfo(1, Npc[m_nLastDamageIdx].m_nPlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, szMessageDeath, strlen(szMessageDeath) );
			}
		}

		if (IsPlayer())
		{
			
			if (Player[m_nPlayerIdx].m_dwDeathScriptId)
			{
				char szNpcIndex[30];
				sprintf(szNpcIndex, "%d", m_nLastDamageIdx);
				Player[m_nPlayerIdx].ExecuteScript(Player[m_nPlayerIdx].m_dwDeathScriptId, "DeathSelf", szNpcIndex);
			}
		}
		//如果死者是Npc，而打死他的是主角，并有死亡脚本则运行脚本
		else  if (Npc[m_nLastDamageIdx].IsPlayer())
		{
			int nIdx = Npc[m_nLastDamageIdx].m_nPlayerIdx;
			if (nIdx)
			{
				// 执行战斗npc死亡脚本
				if (ActionScript[0])
				{
					char szNpcIndex[30];
					sprintf(szNpcIndex, "%d", m_Index);
					Player[nIdx].ExecuteScript(ActionScript, "DeathSelf", szNpcIndex);		
				}
			}
		}
		else
		{
			if (ActionScript[0])
			{
				try
				{
					KLuaScript *pPlayScript =(KLuaScript*) g_GetScript(ActionScript);
					if (!pPlayScript)
					{
					}
					else
					{
						int nTopIndex = 0;
						
						pPlayScript->SafeCallBegin(&nTopIndex);	
					//	pPlayScript->CallFunction("OnDeath",1,"d",m_Index);
						pPlayScript->SafeCallEnd(nTopIndex);
					}
				}
				catch(...)
				{
					//printf("Xay ra loi chay Spcrit dieu khien \\script\\admin\\banghoi\\banghoi.lua !!!!!");
				}
			}
		}
#endif

		// 重生点
		if (m_Kind != kind_partner)//战斗Npc时
		{
			DoRevive();
#ifndef _SERVER
			// 客户端把NPC删除
			if (m_Kind != kind_player)
			{
				SubWorld[0].m_WorldMessage.Send(GWM_NPC_DEL, m_Index);
				return;
			}
#endif		
		}
		else	// 同伴类？以后再说吧
		{
			// 以后再说Not Finish
		}
	}
	else
	{
		//g_DebugLog("[DEATH] WaitForFrame FALSE");
	}
}

void KNpc::DoDefense()
{
	m_ProcessAI = 0;
}

void KNpc::OnDefense()
{
}

void KNpc::DoIdle()
{
	if (m_Doing == do_idle)
		return;
	m_Doing = do_idle;
}

void KNpc::OnIdle()
{
}

void KNpc::DoHurt(int nHurtFrames, int nX, int nY, int nHurtI)
{
	//_ASSERT(m_RegionIndex >= 0);
#ifndef _SERVER
	m_DataRes.SetBlur(FALSE);
#endif
	
	if (m_RegionIndex < 0)
		return;
	
	if ((m_Doing == do_hurt && nHurtI <= 100) || m_Doing == do_death || m_Doing == do_runattack || m_Doing == do_goattack)
		return;
	
	// 受击回复速度已经达到100%了，不做受伤动作
#ifdef _SERVER
	int giam_tho_thuong = 0;
	if (m_CurrentHitRecover <= 80)
	{
		giam_tho_thuong = (m_CurrentHitRecover/10) * 10;
	}
	else
	{
		giam_tho_thuong = 80;
	}
	
	if (!g_RandPercent(nHurtI/2+50-giam_tho_thuong/2))
	{
		return;
	}
	
#endif
	m_Doing = do_hurt;
	m_ProcessAI	= 0;

#ifdef _SERVER
	m_Frames.nTotalFrame = 18 * m_HurtFrame * nHurtI *(100 - giam_tho_thuong)/ 100000;
#else
	m_ClientDoing = cdo_hurt;
	m_Frames.nTotalFrame = nHurtFrames;
	m_nHurtDesX = nX;
	m_nHurtDesY = nY;
	if (m_Height > 0)
	{
		// 临时记录下来做为高度变化，在OnHurt中使用
		m_nHurtHeight = m_Height;
	}
	else
	{
		m_nHurtHeight = 0;
	}
#endif
	if (m_Frames.nTotalFrame == 0)
		m_Frames.nTotalFrame = 1;
	m_Frames.nCurrentFrame = 0;

#ifdef _SERVER	// 向周围9个Region广播发技能
	NPC_HURT_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npchurt;
	NetCommand.ID = m_dwID;
	NetCommand.nFrames = m_Frames.nTotalFrame;
	GetMpsPos(&NetCommand.nX, &NetCommand.nY);

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}

void KNpc::OnHurt()
{
	if (m_RegionIndex < 0)
	{
		g_DebugLog("[error]%s Region Index < 0 when hurt", Name);
		return;
	}
	int nX, nY;
	GetMpsPos(&nX, &nY);
#ifdef _SERVER
	m_Height = 0;
#endif
#ifndef _SERVER
if(m_Frames.nTotalFrame > 0)
	{
	m_Height = m_nHurtHeight * (m_Frames.nTotalFrame - m_Frames.nCurrentFrame - 1) / m_Frames.nTotalFrame;
	nX = nX + (m_nHurtDesX - nX) * m_Frames.nCurrentFrame / m_Frames.nTotalFrame;
	nY = nY + (m_nHurtDesY - nY) * m_Frames.nCurrentFrame / m_Frames.nTotalFrame;

	int nOldRegion = m_RegionIndex;
	//SetPos(nX, nY);
	SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
	int nRegion, nMapX, nMapY, nOffX, nOffY;
	nRegion = -1;
	nMapX = nMapY = nOffX = nOffY = 0;
	SubWorld[m_SubWorldIndex].Mps2Map(nX, nY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nRegion == -1)
	{
		SubWorld[0].m_Region[nOldRegion].RemoveNpc(m_Index);
		m_dwRegionID = 0;
	}
	else if (nOldRegion != nRegion)
	{
		m_RegionIndex = nRegion;
		m_MapX = nMapX;
		m_MapY = nMapY;
		m_OffX = nOffX;
		m_OffY = nOffY;
		SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index);
		m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
	}
	if (nRegion >= 0)
		CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
/*
	if ( !m_bClientOnly && m_RegionIndex >= 0 && m_Kind == kind_player && m_nPlayerIdx == CLIENT_PLAYER_INDEX)
		{
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			m_RegionIndex = nRegion;
			m_MapX = nMapX;
			m_MapY = nMapY;
			m_OffX = nOffX;
			m_OffY = nOffY;
			SubWorld[0].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);
		}
*/		
		
}	
#endif

	if (WaitForFrame())
	{
		g_DebugLog("[DEATH]On Hurt Finished");
		DoStand();
		m_ProcessAI = 1;
	}
}

void KNpc::DoSpecial1()
{
	DoBlurAttack();
}

void KNpc::OnSpecial1()
{
	if (WaitForFrame() &&m_Frames.nTotalFrame != 0)
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;	
	}
	else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
	{
		KSkill * pSkill = (KSkill*)GetActiveSkill();
		if (pSkill)
		{
			int nChildSkill = pSkill->GetChildSkillId();
			int nChildSkillLevel = pSkill->m_ulLevel;
			
			if (nChildSkill > 0)
			{
				KSkill * pChildSkill = (KSkill*)g_SkillManager.GetSkill(nChildSkill, nChildSkillLevel);
				if (pChildSkill)
				{
					pChildSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
				}
			}
		}

		if (m_Frames.nTotalFrame == 0)
		{
			m_ProcessAI = 1;
		}
	}
}

void KNpc::DoSpecial2()
{
}

void KNpc::OnSpecial2()
{

	if (WaitForFrame() &&m_Frames.nTotalFrame != 0)
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;	
	}
	else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
	{
		ISkill * pSkill = GetActiveSkill();
		eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();
		switch(eStyle)
		{
		case SKILL_SS_Thief:
			{
				( (KThiefSkill*)pSkill )->OnSkill(this);
			}
			break;
		} 
		
		if (m_Frames.nTotalFrame == 0)
		{
			m_ProcessAI = 1;
		}
	}
	
}

void KNpc::DoSpecial3()
{
}

void KNpc::OnSpecial3()
{
}

void KNpc::DoSpecial4()
{
}

void KNpc::OnSpecial4()
{
}

void KNpc::DoStand()
{
	m_Frames.nTotalFrame = m_StandFrame;
	if (m_Doing == do_stand)
	{
		return;
	}
	else
	{
		m_Doing = do_stand;
		m_Frames.nCurrentFrame = 0;
		GetMpsPos(&m_DesX, &m_DesY);
#ifndef _SERVER
		if (m_FightMode)
			m_ClientDoing = cdo_fightstand;
		else if (g_Random(6) != 1)
		{
			m_ClientDoing = cdo_stand;
		}
		else
		{
			m_ClientDoing = cdo_stand1;
		}
		m_DataRes.SetBlur(FALSE);
		m_DataRes.StopSound();
#endif
	}
}


void KNpc::OnStand()
{
	if (WaitForFrame())
	{
#ifndef _SERVER
		if (m_FightMode)
		{
			m_ClientDoing = cdo_fightstand;
		}
		else if (g_Random(6) != 1)
		{
			m_ClientDoing = cdo_stand;
		}
		else
		{
			m_ClientDoing = cdo_stand1;
		}
#endif
	}
}

void KNpc::DoRevive()
{
	if (m_RegionIndex < 0)
	{
		g_DebugLog("[error]%s Region Index < 0 when dorevive", Name);
		return;
	}
	if (m_Doing == do_revive)
	{
		return;
	}
	else
	{
		m_Doing = do_revive;
		m_ProcessAI = 0;
		m_ProcessState = 0;
	
		ClearStateSkillEffect();
		ClearNormalState();

#ifdef _SERVER
		if (IsPlayer())
			return;
		m_Frames.nTotalFrame = m_ReviveFrame;
		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
		SubWorld[m_SubWorldIndex].NpcChangeRegion(m_RegionIndex, VOID_REGION, m_Index);	// spe 03/06/28
		m_Frames.nCurrentFrame = 0;
#else
		// 客户端
		if (IsPlayer())
		{
			KSystemMessage Msg;

			Msg.byConfirmType = SMCT_UI_RENASCENCE;
			Msg.byParamSize = 0;
			Msg.byPriority = 255;
			Msg.eType = SMT_PLAYER;
			sprintf(Msg.szMessage, MSG_NPC_DEATH, Name);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
		}
		m_Frames.nTotalFrame = m_DeathFrame;
		m_ClientDoing = cdo_death;
#endif
	}
}

void KNpc::OnRevive()
{
#ifdef _SERVER
	if (!IsPlayer() && WaitForFrame())
	{
		Revive();
	}
#else	// 客户端
	m_Frames.nCurrentFrame = m_Frames.nTotalFrame - 1;
#endif
}

void KNpc::DoRun()
{
	//_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
	{
		return;
	}

	if (m_CurrentRunSpeed)
		m_Frames.nTotalFrame = (m_RunFrame * m_RunSpeed) / m_CurrentRunSpeed;
	else
		m_Frames.nTotalFrame = m_RunFrame;

#ifndef _SERVER
	if (m_FightMode)
	{
		m_ClientDoing = cdo_fightrun;
	}
	else
	{
		m_ClientDoing = cdo_run;
	}
#endif

	if (IsPlayer())
	{
/*		if (!Cost(attrib_stamina, m_CurrentStaminaLoss))
		{
			DoWalk();
			return;
		}*/
	}

#ifdef _SERVER

	NPC_RUN_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcrun;
	NetCommand.ID = m_dwID;
	NetCommand.nMpsX = m_DesX;
	NetCommand.nMpsY = m_DesY;

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif

	if (m_Doing == do_run)
	{
		return;
	}
	m_Doing = do_run;

	m_Frames.nCurrentFrame = 0;
}

void KNpc::OnRun(int nAddSpeed)
{
	WaitForFrame();
	if (nAddSpeed > 0)
	{
	ServeMove(nAddSpeed);
	}
	else
	{
	if (IsPlayer())
		{
		if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 2 && m_CurrentStamina <= 0)
		{
	    DoWalk();
		return;
		}
		}
	ServeMove(m_CurrentRunSpeed);
	}
}

void KNpc::DoSit()
{
	//_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
	{
		return;
	}

	if (m_Doing == do_sit)
	{
//		DoStand();
		return;
	}
	
	m_Doing = do_sit;
#ifdef _SERVER	// 向周围9个Region广播发技能
	NPC_SIT_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcsit;
	NetCommand.ID = m_dwID;
		
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif

#ifndef _SERVER
		m_ClientDoing = cdo_sit;

  

#endif

	m_Frames.nTotalFrame = m_SitFrame;
	m_Frames.nCurrentFrame = 0;
}

void KNpc::OnSit()
{
	// 体力换内力（没有设定）
	if (WaitForFrame())
	{
		m_Frames.nCurrentFrame = m_Frames.nTotalFrame - 1;
	}
}

void KNpc::DoSkill(int nX, int nY)
{
	//_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
	{
		return;
	}

	if (m_Doing == do_skill)
		return;
	
	// 非战斗状态不能发技能
	if (IsPlayer())
	{
		if (!m_FightMode)
			return;
#ifdef _SERVER
		if (m_nPlayerIdx > 0)
			Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeAttack);
#endif
	}






	ISkill * pSkill = GetActiveSkill();
	if(pSkill)
	{
		eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();

		if (m_SkillList.CanCast(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime)
			&& pSkill->CanCastSkill(m_Index, nX, nY) 
			&& ((pSkill->GetSkillReqLevel() != 80 && pSkill->GetSkillReqLevel() != 100) || this->m_Level >= 80)
			&& 
			( m_Kind != kind_player 
			|| Cost(pSkill->GetSkillCostType(), pSkill->GetSkillCost(this))
			)
			)
		{
	
			
		/*------------------------------------------------------------------------------------
		发技能时，当需指定目标对象时，传至Skill.Cast的两个参数第一个参数为-1,第二个为Npc index
		在S2C时，第二个参数必须由Server的NpcIndex转为NpcdwID参出去。
		在C收到该指令时，将NpcdwID转为本机的NpcIndex
			-------------------------------------------------------------------------------------*/
#ifdef _SERVER	// 向周围9个Region广播发技能
			NPC_SKILL_SYNC	NetCommand;
			
			NetCommand.ProtocolType = (BYTE)s2c_skillcast;
			NetCommand.ID = m_dwID;
			NetCommand.nSkillID = m_ActiveSkillID;
			NetCommand.nSkillLevel = m_SkillList.GetCurrentLevel(m_ActiveSkillID);
			
			if (nY <= 0 ) 
			{
				DoStand();
				return;
			}
			
			NetCommand.nMpsX = nX;
			if (nX == -1) //m_nDesX == -1 means attack someone whose id is DesY , and if m_nDesX == -2 means attack at somedir
			{
				NetCommand.nMpsY = Npc[nY].m_dwID;
				if (0 == NetCommand.nMpsY || Npc[nY].m_SubWorldIndex != m_SubWorldIndex)
					return;
			}
			else
			{
				NetCommand.nMpsY = nY;
			}

			m_SkillParam1 = nX;
			m_SkillParam2 = nY;
			m_DesX = nX;
			m_DesY = nY;
			
			POINT	POff[8] = 
			{
				{0, 32},
				{-16, 32},
				{-16, 0},
				{-16, -32},
				{0, -32},
				{16, -32},
				{16, 0},
				{16, 32},
			};
			int nMaxCount = MAX_BROADCAST_COUNT;
			CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
			int i;
			for (i = 0; i < 8; i++)
			{
				if (CONREGIONIDX(i) == -1)
					continue;
				CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
			}

if (m_InvisibilityState.nTime > 0)
m_InvisibilityState.nTime = 0;

#endif
			if (eStyle == SKILL_SS_Missles 
				|| eStyle == SKILL_SS_Melee 
				|| eStyle == SKILL_SS_InitiativeNpcState 
				|| eStyle == SKILL_SS_PassivityNpcState)
			{
				DoOrdinSkill((KSkill *) pSkill, nX, nY);
			}
			else
			{
				switch(eStyle)
				{
				case SKILL_SS_Thief:
					{
						((KThiefSkill*)pSkill)->DoSkill(this, nX, nY);

					}break;
				default:
					return;
				}
			}


		}	
		else
		{
			m_nPeopleIdx = 0;


#ifndef _SERVER	
			m_nPeopleIdxCheckClient = 0;
#endif



			m_nObjectIdx = 0;
			DoStand();
		}
	}
	else
	{
		_ASSERT(pSkill);
	}
} 

int KNpc::DoOrdinSkill(KSkill * pSkill, int nX, int nY)
{
	_ASSERT(pSkill);

#ifndef _SERVER		
	m_DataRes.StopSound();
	int x, y, tx, ty;
	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
	
	if (nY < 0)
		return 0;
	
	if (nX < 0)
	{
		if (nX != -1) 
			return 0;
		
		if (nY >= MAX_NPC || Npc[nY].m_dwID == 0 || Npc[nY].m_SubWorldIndex != m_SubWorldIndex)
			return 0;
		Npc[nY].GetMpsPos(&tx, &ty);
	}
	else
	{
		tx = nX;
		ty = nY;
	}
	
	m_SkillParam1 = nX;
	m_SkillParam2 = nY;
	m_DesX = nX;
	m_DesY = nY;
	
	m_Dir = g_GetDirIndex(x, y, tx, ty);
	if (pSkill->GetPreCastEffectFile()[0])
		m_DataRes.SetSpecialSpr((char *)pSkill->GetPreCastEffectFile());
	
	if (IsPlayer())
		pSkill->PlayPreCastSound(m_nSex,x, y);
	
	if (pSkill->IsNeedShadow())		
		m_DataRes.SetBlur(TRUE);
	else
		m_DataRes.SetBlur(FALSE);
#endif
	
	CLIENTACTION ClientDoing = pSkill->GetActionType();
	
#ifndef _SERVER
	if (ClientDoing >= cdo_count) 
		m_ClientDoing = cdo_magic;
	else if (ClientDoing != cdo_none)
		m_ClientDoing = ClientDoing;
#endif
	if (pSkill->GetSkillStyle() == SKILL_SS_Melee)
	{


		    KSkill * pSkill = NULL;	
            pSkill =(KSkill*) GetActiveSkill();
			if (m_bRideHorse)
			{
			m_SkillList.SetBeginCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime);
			m_SkillList.SetNextCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime + pSkill->GetDelayPerCastHorse());
			}
			else
			{
			m_SkillList.SetBeginCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime);
			m_SkillList.SetNextCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime + pSkill->GetDelayPerCast());
			}

		if (CastMeleeSkill(pSkill) == FALSE)
		{
			m_nPeopleIdx = 0;



			#ifndef _SERVER	
			m_nPeopleIdxCheckClient = 0;
            #endif


			m_nObjectIdx = 0;
			m_ProcessAI = 1;
			DoStand();
			
			return 1 ;
		}
	}
	//物理技能的技能释放时间与普通技能不同，一个是AttackFrame,一个是CastFrame
	else if (pSkill->IsSkillPhysical())
	{
		if (ClientDoing == cdo_none) 
			m_Frames.nTotalFrame = 0;
		else
			m_Frames.nTotalFrame = m_AttackFrame * 100 / (100 + m_CurrentAttackSpeed);
		
#ifndef _SERVER
		if (g_Random(3))
			m_ClientDoing = cdo_attack;
		else 
			m_ClientDoing = cdo_attack1;
#endif
		m_Doing = do_attack;
	}
	else
	{
		if (ClientDoing == cdo_none) 
			m_Frames.nTotalFrame = 0;
		else
			m_Frames.nTotalFrame = m_CastFrame * 100 / (m_CurrentCastSpeed + 100);
		m_Doing  = do_magic;
	}
	
	m_ProcessAI = 0;
	m_Frames.nCurrentFrame = 0;	
	return 1;
}



void KNpc::DoAttack()
{
	if (m_Doing == do_attack)
		return;

#ifndef _SERVER
	if (g_Random(2) == 1)
	{
		m_ClientDoing = cdo_attack;
	}
	else
	{
		m_ClientDoing = cdo_attack1;
	}
#endif

	m_ProcessAI = 0;
	m_Frames.nTotalFrame = m_AttackFrame * 100 / (100 + m_CurrentAttackSpeed);
	m_Frames.nCurrentFrame = 0;
	m_Doing = do_attack;
}


BOOL	KNpc::CastMeleeSkill(KSkill * pSkill)
{
	m_SpecialSkillStep = 0;
	BOOL bSuceess = FALSE;
	_ASSERT(pSkill);
	
	switch(pSkill->GetMeleeType())
	{
	case Melee_AttackWithBlur:
		{
			bSuceess = DoBlurAttack();
		}break;
	case Melee_Jump:
		{
			if (NewJump(m_DesX, m_DesY))
			{
				DoJump();
				bSuceess = TRUE;
			}
			
		}break;
	case Melee_JumpAndAttack:
		{
			if (m_DesX < 0 && m_DesY > 0) 
			{
				int x, y;
				SubWorld[m_SubWorldIndex].Map2Mps
					(
					Npc[m_DesY].m_RegionIndex,
					Npc[m_DesY].m_MapX, 
					Npc[m_DesY].m_MapY, 
					Npc[m_DesY].m_OffX, 
					Npc[m_DesY].m_OffY, 
					&x,
					&y
					);
				
				m_DesX = x + 1;
				m_DesY = y;
			}

			if (NewJump(m_DesX, m_DesY))
			{
				DoJumpAttack();
				bSuceess = TRUE;
			}

		}break;
	case Melee_RunAndAttack:
		{
			bSuceess = DoRunAttack();

		}break;
	case Melee_ManyAttack:
		{
			bSuceess = DoManyAttack();
		}break;
	case Melee_Go:
	{

	bSuceess = DoGoAttack();

	}break;
	default:
		m_ProcessAI = 1;
		break;
	}
	return bSuceess;

}


BOOL KNpc::DoGoAttack()
{

	m_ProcessAI = 0;

	switch(m_SpecialSkillStep)
	{
	case 0:
		m_Frames.nTotalFrame = 20;
		m_ProcessAI = 0;
		
#ifndef _SERVER
		m_DataRes.SetBlur(TRUE);

		if (m_FightMode)
		{
			m_ClientDoing = cdo_fightrun;
		}
		else
		{
			m_ClientDoing = cdo_run;
		}
#endif
		
		if (m_DesX < 0 && m_DesY > 0) 
		{
			int x, y;
			SubWorld[m_SubWorldIndex].Map2Mps
				(
				Npc[m_DesY].m_RegionIndex,
				Npc[m_DesY].m_MapX, 
				Npc[m_DesY].m_MapY, 
				Npc[m_DesY].m_OffX, 
				Npc[m_DesY].m_OffY, 
				&x,
				&y
				);

		m_DesX = x;
		m_DesY = y;
		}

		m_Frames.nCurrentFrame = 0;
		m_Doing = do_goattack;
		break;

	case 1:
#ifndef _SERVER
		
		if (m_FightMode)
			m_ClientDoing = cdo_fightstand;
		else if (g_Random(6) != 1)
		{
			m_ClientDoing = cdo_stand;
		}
		else
		{
			m_ClientDoing = cdo_stand1;
		}

		int x, y, tx, ty;
		SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
		if (m_SkillParam1 == -1)
		{
			Npc[m_SkillParam2].GetMpsPos(&tx, &ty);
		}
		else
		{
			tx = m_SkillParam1;
			ty = m_SkillParam2;
		}
		m_Dir = g_GetDirIndex(x, y, tx, ty);
#endif
		m_Frames.nTotalFrame = 0;
		m_Frames.nCurrentFrame = 0;
		m_Doing = do_goattack;
		break;

	case 2:
	case 3:
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
		break;
	}

	m_Frames.nCurrentFrame = 0;
		
	return TRUE;
}


BOOL KNpc::DoBlurAttack()// DoSpecail1
{
	if (m_Doing == do_special1)
		return FALSE;
	
	KSkill * pSkill = (KSkill*) GetActiveSkill();
	if (!pSkill) 
        return FALSE;
	
	_ASSERT(pSkill->GetSkillStyle() == SKILL_SS_Melee);

#ifndef _SERVER
		m_ClientDoing = pSkill->GetActionType();
		m_DataRes.SetBlur(TRUE);
#endif

	m_Frames.nTotalFrame = m_AttackFrame * 100 / (100 + m_CurrentAttackSpeed);
	m_Frames.nCurrentFrame = 0;
	m_Doing = do_special1;
	return TRUE;
}

void KNpc::DoMagic()
{
	if (m_Doing == do_magic)
		return;

	m_ProcessAI = 0;
#ifndef _SERVER
	m_ClientDoing = cdo_magic;
#endif
	m_Frames.nTotalFrame = m_CastFrame * 100 / (m_CurrentCastSpeed + 100);
	m_Frames.nCurrentFrame = 0;
	m_Doing = do_magic;
}

void KNpc::OnSkill()
{
	KSkill * pSkill = NULL;
	if (WaitForFrame() &&m_Frames.nTotalFrame != 0)
	{
		DoStand();
		m_ProcessAI = 1;	
	}
	else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif

		if (m_DesX == -1) 
		{
			if (m_DesY <= 0) 
				goto Label_ProcessAI;
			
			//此时该角色已经无效时
			if (Npc[m_DesY].m_RegionIndex < 0) 
				goto Label_ProcessAI;
		}
		
		
		pSkill =(KSkill*) GetActiveSkill();
		if (pSkill)
		{

			pSkill->Cast(m_Index, m_DesX, m_DesY);

			if (m_bRideHorse)
			{
			m_SkillList.SetBeginCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime);
			m_SkillList.SetNextCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime + pSkill->GetDelayPerCastHorse());
			}
			else
			{
			m_SkillList.SetBeginCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime);
			m_SkillList.SetNextCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime + pSkill->GetDelayPerCast());
			}
			
		}
		
Label_ProcessAI:
		if (m_Frames.nTotalFrame == 0)
		{
			m_ProcessAI = 1;
		}
	}
	
}

void KNpc::JumpTo(int nMpsX, int nMpsY)
{


	if (NewJump(nMpsX, nMpsY))
		DoJump();
	else
	{
		RunTo(nMpsX, nMpsY);
	}
}

void KNpc::RunTo(int nMpsX, int nMpsY)
{
	if (NewPath(nMpsX, nMpsY))
		DoRun();
}

void KNpc::Goto(int nMpsX, int nMpsY)
{
	if (NewPath(nMpsX, nMpsY))
		DoWalk();
}

void KNpc::Madnessto(int nMpsX, int nMpsY)
{
Goto(nMpsX,nMpsY);
}


void KNpc::DoWalk()
{
	//_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
	{
		return;
	}

	if (m_CurrentWalkSpeed)
		m_Frames.nTotalFrame = (m_WalkFrame * m_WalkSpeed) / m_CurrentWalkSpeed + 1;
	else
		m_Frames.nTotalFrame = m_WalkFrame;
	
#ifdef _SERVER		// Server端的代码
	NPC_WALK_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcwalk;
	NetCommand.ID = m_dwID;
	NetCommand.nMpsX = m_DesX;
	NetCommand.nMpsY = m_DesY;

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}	
#endif

	if (m_Doing == do_walk)
	{
		return;
	}
	m_Doing = do_walk;
	m_Frames.nCurrentFrame = 0;

#ifndef _SERVER
	if (m_FightMode)
	{
		m_ClientDoing = cdo_fightwalk;
	}
	else
	{
		m_ClientDoing = cdo_walk;
	}
#endif
	
}

void KNpc::DoPlayerTalk(char * szTalk)
{
#ifdef _SERVER
	//_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
	{
		return;
	}

	int nTalkLen = strlen(szTalk);
	if (!nTalkLen) return;
	BYTE * pNetCommand = new  BYTE[nTalkLen + 6 + 1];
	pNetCommand[0] = (BYTE)s2c_playertalk;
	*(DWORD *)(pNetCommand + 1) = m_dwID;
	pNetCommand[5] = nTalkLen;
	strcpy((char*)(pNetCommand + 6), szTalk);
	pNetCommand[nTalkLen + 6 ] = '\0';

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(pNetCommand, nTalkLen + 6 + 1, nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(pNetCommand, nTalkLen + 6 + 1, nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}

	if (pNetCommand)
	{
		delete [] pNetCommand;
	}
#endif
}

void KNpc::OnPlayerTalk()
{

}

void KNpc::OnWalk()
{
#ifndef	_SERVER
	// 处理客户端的动画换帧等……
#endif
	WaitForFrame();
	ServeMove(m_CurrentWalkSpeed);
}

int	KNpc::GetSkillLevel(int nSkillId)
{
	int nIndex = m_SkillList.FindSame(nSkillId);
	if (nIndex)
	{
		return m_SkillList.m_Skills[nIndex].SkillLevel;
	}
	else
	{
		return 0;
	}
}

void KNpc::ModifyAttrib(int nAttacker, void* pData)
{
	if (pData != NULL)
		g_NpcAttribModify.ModifyAttrib(this, pData);
}

#ifdef _SERVER
void KNpc::CalcDamage(int nAttacker, int nMin, int nMax, DAMAGE_TYPE nType, BOOL bIsMelee, BOOL bDoHurt /* TRUE */, BOOL bReturn /* = FALSE */,int nngu_hanh_tuong_khac,int nloai_ngu_hanh,int nhut_sinh_luc,int nhut_noi_luc)
{
	if (m_Doing == do_death || m_RegionIndex < 0)
		return;
	
	if (nMin + nMax <= 0)
		return;
	
	int	nRes = 0;
	int nDamageRange = nMax - nMin;
	int nDamage = 0;
	if (nDamageRange < 0) 
	{
		nDamage = nMax + g_Random(-nDamageRange);
	}
	else
	{
		nDamage = nMin + g_Random(nMax - nMin);
	}
	
	int khang_ngu_hanh_tuong_khac = 0;
	if ((nloai_ngu_hanh == 0 && m_Series == 1) || (nloai_ngu_hanh == 2 && m_Series == 3) || (nloai_ngu_hanh == 1 && m_Series == 4) || (nloai_ngu_hanh == 3 && m_Series == 0) || (nloai_ngu_hanh == 4 && m_Series == 2))
	{
		khang_ngu_hanh_tuong_khac -= nngu_hanh_tuong_khac;
	}
	else if ((nloai_ngu_hanh== 0 && m_Series == 3) || (nloai_ngu_hanh == 2 && m_Series == 4) || (nloai_ngu_hanh == 1 && m_Series == 0) || (nloai_ngu_hanh == 3 && m_Series == 2) || (nloai_ngu_hanh == 4 && m_Series == 1))
	{
		khang_ngu_hanh_tuong_khac += nngu_hanh_tuong_khac;
	}
	
	int AddMaxResistCs = 0;
	if (IsPlayer())
	{
		if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(139) == 1)
		{
			AddMaxResistCs += 1;
		}
		else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(139) == 2)
		{
			AddMaxResistCs += 2;
		}
		else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(139) == 3)
		{
			AddMaxResistCs += 4;
		}
		else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(139) == 4)
		{
			AddMaxResistCs += 6;
		}
		else if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(139) == 5)
		{
			AddMaxResistCs += 8;
		}
	}

	switch(nType)
	{
	case damage_physics:
		nRes = m_CurrentPhysicsResist+khang_ngu_hanh_tuong_khac;
		if (nRes > m_CurrentPhysicsResistMax+AddMaxResistCs)
		{
			nRes = m_CurrentPhysicsResistMax+AddMaxResistCs;
		}

		if (nRes > MAX_RESIST)
		{
			nRes = MAX_RESIST;
		}

		g_DebugLog("[数值]%s受到%d点原始物理伤害，物理抗性%d，物理盾%d", Name, nDamage, nRes, m_PhysicsArmor.nValue[0]);

		m_PhysicsArmor.nValue[0] -= nDamage;
		if (m_PhysicsArmor.nValue[0] < 0)
		{
			nDamage = -m_PhysicsArmor.nValue[0];
			m_PhysicsArmor.nValue[0] = 0;
			m_PhysicsArmor.nTime = 0;
		}
		else
		{
			nDamage = 0;
		}
		if (bIsMelee)
		{
			nMax = m_CurrentMeleeDmgRetPercent;
		}
		else
		{
			nMax = m_CurrentRangeDmgRetPercent;
		}
		break;
	case damage_cold:
		nRes = m_CurrentColdResist+khang_ngu_hanh_tuong_khac;
		if (nRes > m_CurrentColdResistMax+AddMaxResistCs)
		{
			nRes = m_CurrentColdResistMax+AddMaxResistCs;
		}
		if (nRes > MAX_RESIST)
		{
			nRes = MAX_RESIST;
		}

		g_DebugLog("[数值]%s受到%d点原始冰伤害，冰抗性%d，冰盾%d", Name, nDamage, nRes, m_ColdArmor.nValue[0]);

		m_ColdArmor.nValue[0] -= nDamage;

		if (m_ColdArmor.nValue[0] < 0)
		{
			nDamage = -m_ColdArmor.nValue[0];
			m_ColdArmor.nValue[0] = 0;
			m_ColdArmor.nTime = 0;
		}
		else
		{
			nDamage = 0;
		}
		nMax = m_CurrentRangeDmgRetPercent;
		break;
	case damage_fire:
		nRes = m_CurrentFireResist+khang_ngu_hanh_tuong_khac;
		if (nRes > m_CurrentFireResistMax+AddMaxResistCs)
		{
			nRes = m_CurrentFireResistMax+AddMaxResistCs;
		}
		if (nRes > MAX_RESIST)
		{
			nRes = MAX_RESIST;
		}

		g_DebugLog("[数值]%s受到%d点原始火伤害，火抗性%d，火盾%d", Name, nDamage, nRes, m_FireArmor.nValue[0]);

		m_FireArmor.nValue[0] -= nDamage;
		
		if (m_FireArmor.nValue[0] < 0)
		{
			nDamage = -m_FireArmor.nValue[0];
			m_FireArmor.nValue[0] = 0;
			m_FireArmor.nTime = 0;
		}
		else
		{
			nDamage = 0;
		}
		nMax = m_CurrentRangeDmgRetPercent;
		break;
	case damage_light:
		nRes = m_CurrentLightResist+khang_ngu_hanh_tuong_khac;
		if (nRes > m_CurrentLightResistMax+AddMaxResistCs)
		{
			nRes = m_CurrentLightResistMax+AddMaxResistCs;
		}
		if (nRes > MAX_RESIST)
		{
			nRes = MAX_RESIST;
		}
		g_DebugLog("[数值]%s受到%d点原始电伤害，电抗性%d，电盾%d", Name, nDamage, nRes, m_LightArmor.nValue[0]);

		m_LightArmor.nValue[0] -= nDamage;
		if (m_LightArmor.nValue[0] < 0)
		{
			nDamage = -m_LightArmor.nValue[0];
			m_LightArmor.nValue[0] = 0;
			m_LightArmor.nTime = 0;
		}
		else
		{
			nDamage = 0;
		}
		nMax = m_CurrentRangeDmgRetPercent;
		break;
	case damage_poison:
		g_DebugLog("[数值]%s受到%d点原始毒伤害", Name, nDamage);
		nRes = m_CurrentPoisonResist+khang_ngu_hanh_tuong_khac;
		if (nRes > m_CurrentPoisonResistMax+AddMaxResistCs)
		{
			nRes = m_CurrentPoisonResistMax+AddMaxResistCs;
		}
		if (nRes > MAX_RESIST)
		{
			nRes = MAX_RESIST;
		}
		
		g_DebugLog("[数值]%s受到%d点原始毒伤害，毒抗性%d，毒盾%d", Name, nDamage, nRes, m_PoisonArmor.nValue[0]);

		m_PoisonArmor.nValue[0] -= nDamage;
		if (m_PoisonArmor.nValue[0] < 0)
		{
			nDamage = -m_PoisonArmor.nValue[0];
			m_PoisonArmor.nValue[0] = 0;
			m_PoisonArmor.nTime = 0;
		}
		else
		{
			nDamage = 0;
		}
		nMax = m_CurrentRangeDmgRetPercent;
		m_nLastPoisonDamageIdx = nAttacker;
		break;
	case damage_magic:
		g_DebugLog("[数值]%s受到%d点原始魔法伤害", Name, nDamage);
		nRes = 0;
		break;
	default:
		nRes = 0;
		break;
	}

	if (!nDamage)
		return;

	// 第二步判断是否有魔法盾存在
	

    if (nType != damage_poison && nDamage > 0 && m_ManaShieldPoint.nValue[0] > 0)
    {
		nDamage -= m_ManaShieldPoint.nValue[0];
		if (nDamage < 1)
			nDamage = 1;
	}


	nDamage = nDamage * (100 - nRes) / 100;
	//g_DebugLog("[数值]%s实际伤害：%d", Name, nDamage);

	if (nDamage < 1)
		nDamage = 1;

	if (nAttacker > 0 && bReturn == FALSE && nType != damage_poison)
	{
		int nCurrentDmgRetPercentResist = Npc[nAttacker].m_CurrentDmgRetPercentResist;
		if (nCurrentDmgRetPercentResist < 0)
			nCurrentDmgRetPercentResist = 0;

		if (nCurrentDmgRetPercentResist > 95)
			nCurrentDmgRetPercentResist = 95;

		if (bIsMelee)
		{
			nMin = m_CurrentMeleeDmgRet;
			nMin += nDamage * nMax / 100;
			nMin = nMin * (100 - nCurrentDmgRetPercentResist)/100;

			if (nMin > 0 && (IsPlayer() || (m_SubWorldIndex != g_SubWorldSet.SearchWorld(33) && m_SubWorldIndex != g_SubWorldSet.SearchWorld(37))))
			{
			Npc[nAttacker].CalcDamage(m_Index, nMin, nMin, damage_magic, FALSE, FALSE, TRUE);
			}
		}
		else
		{
			nMin = m_CurrentRangeDmgRet;
			nMin += nDamage * nMax / 100;
			nMin = nMin * (100 - nCurrentDmgRetPercentResist)/100;

			if (nMin > 0 && (IsPlayer() || (m_SubWorldIndex != g_SubWorldSet.SearchWorld(33) && m_SubWorldIndex != g_SubWorldSet.SearchWorld(37))))
			{
				Npc[nAttacker].CalcDamage(m_Index, nMin, nMin, damage_magic, FALSE, FALSE, TRUE);
			}
		}

		//g_DebugLog("[数值]%s返回伤害%d", Name, nMin);
	}
	else if (nAttacker > 0 && bReturn == FALSE && nType == damage_poison)
	{	
	nMin = nDamage * m_CurrentPoisonDmgRetPercent / 100;
	Npc[nAttacker].CalcDamage(m_Index, nMin, nMin, damage_magic, FALSE, FALSE, TRUE);
	}

	if (this->m_Kind == kind_player && Npc[nAttacker].m_Kind == kind_player)
		nDamage = nDamage * NpcSet.m_nPKDamageRate / 100;
	m_nLastDamageIdx = nAttacker;
	if (m_Kind != kind_player && Npc[nAttacker].m_Kind == kind_player && Npc[nAttacker].m_nPlayerIdx > 0)
		m_cDeathCalcExp.AddDamage(Npc[nAttacker].m_nPlayerIdx, (m_CurrentLife - nDamage > 0 ? nDamage : m_CurrentLife));
	





if (m_CurrentManaShieldExPoint > 0)
{
if (m_CurrentManaShieldExPoint > nDamage)
{
nDamage = 0;
m_CurrentManaShieldExPoint -= nDamage;
}
else
{
nDamage -= m_CurrentManaShieldExPoint;
m_CurrentManaShieldExPoint = 0;
}
}






if (this->m_Kind == kind_player && Npc[nAttacker].m_Kind == kind_player)
	{

	if ((SubWorld[m_SubWorldIndex].m_SubWorldID == 43 && SubWorld[Npc[nAttacker].m_SubWorldIndex].m_SubWorldID == 43) || (SubWorld[m_SubWorldIndex].m_SubWorldID == 106 && SubWorld[Npc[nAttacker].m_SubWorldIndex].m_SubWorldID == 106) || (SubWorld[m_SubWorldIndex].m_SubWorldID == 258 && SubWorld[Npc[nAttacker].m_SubWorldIndex].m_SubWorldID == 258))
	{

		Player[Npc[nAttacker].m_nPlayerIdx].m_cTask.SetClearVal(50,Player[Npc[nAttacker].m_nPlayerIdx].m_cTask.GetClearVal(50) + nDamage);

	}

	}










if (nType != damage_poison)
{
	int nCurenManaShield = m_ManaShield.nValue[0];
	if (nCurenManaShield > 100)
		nCurenManaShield = 100;

	if (nCurenManaShield > 0)
	{
	
		int nManaDamage = nDamage * nCurenManaShield / 100;

		if (nManaDamage < m_CurrentMana)
		{
		m_CurrentMana -= nManaDamage;
		nDamage -= nManaDamage;
		}
		else
		{
		nDamage -= m_CurrentMana;
		m_CurrentMana = 0;       
		}

	}
}



















int OldPecentLife = 0;
if (m_CurrentLifeMax > 0)
    OldPecentLife = m_CurrentLife * 100/m_CurrentLifeMax;


if (!Npc[nAttacker].IsPlayer() && Npc[nAttacker].m_nCurentPencentDameAdd > 0)
{
nDamage = nDamage * (100 + Npc[nAttacker].m_nCurentPencentDameAdd)/100;
}


if (!IsPlayer() && m_nCurrentShareDame > 0)
{
nDamage = nDamage/(1+m_nCurrentShareDame);
}

if (nDamage < 1)
	nDamage = 1;








	







m_CurrentLife -= nDamage;


if (m_CurrentLife > m_CurrentLifeMax)
m_CurrentLife = m_CurrentLifeMax;

if (nType == damage_poison)
{
if (!Npc[nAttacker].m_CurrentTimeCastSkillEffec.nTime && g_RandPercent(Npc[nAttacker].m_CurrentDoSkillEffecPercent.nValue[0]))
{
KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(Npc[nAttacker].m_CurrentDoSkillEffecPercent.nValue[1],Npc[nAttacker].m_CurrentDoSkillEffecPercent.nValue[2]);

if (pSkillActive)
{
KMagicAttrib* pMagicAttrib = pSkillActive->GetStateAttribs();
int pMagicAttribNum = pSkillActive->GetStateAttribsNum();
SetStateSkillEffect(nAttacker, Npc[nAttacker].m_CurrentDoSkillEffecPercent.nValue[1], Npc[nAttacker].m_CurrentDoSkillEffecPercent.nValue[2], pMagicAttrib, pMagicAttribNum, pMagicAttrib[0].nValue[1]);
Npc[nAttacker].m_CurrentTimeCastSkillEffec.nTime = Npc[nAttacker].m_CurrentTimeCastSkillEffec.nValue[0];
}

}
}

if (nType == damage_poison && m_CurrentManaFromDamePercent > 0)
{
m_CurrentMana -= m_CurrentManaFromDamePercent * nDamage / 100;
if (m_CurrentMana < 0)
m_CurrentMana = 0;
}


if (m_CurrentDamage2Mana >= 30)
m_CurrentDamage2Mana = 30;

m_CurrentMana += m_CurrentDamage2Mana * nDamage / 100;
if (m_CurrentMana > m_CurrentManaMax)
{
m_CurrentMana = m_CurrentManaMax;
}







	if (m_CurrentLife >= 0)
{

	if ((Npc[nAttacker].m_CurrentLifeMax - Npc[nAttacker].m_CurrentLife) > (nDamage*nhut_sinh_luc/100))
	{
Npc[nAttacker].m_CurrentLife += nDamage*nhut_sinh_luc/100;
	}
	else
	{
Npc[nAttacker].m_CurrentLife = Npc[nAttacker].m_CurrentLifeMax;
	}

if ((Npc[nAttacker].m_CurrentManaMax - Npc[nAttacker].m_CurrentMana) > (nDamage*nhut_noi_luc/100))
	{
Npc[nAttacker].m_CurrentMana += nDamage*nhut_noi_luc/100;
	}
	else
	{
Npc[nAttacker].m_CurrentMana = Npc[nAttacker].m_CurrentManaMax;
	}


}

	else
{

	if ((Npc[nAttacker].m_CurrentLifeMax - Npc[nAttacker].m_CurrentLife) > ((nDamage+m_CurrentLife)*nhut_sinh_luc/100))
	{
Npc[nAttacker].m_CurrentLife += (nDamage+m_CurrentLife)*nhut_sinh_luc/100;
	}
	else
	{
Npc[nAttacker].m_CurrentLife = Npc[nAttacker].m_CurrentLifeMax;
	}


if ((Npc[nAttacker].m_CurrentManaMax - Npc[nAttacker].m_CurrentMana) > ((nDamage+m_CurrentLife)*nhut_noi_luc/100))
	{
Npc[nAttacker].m_CurrentMana += (nDamage+m_CurrentLife)*nhut_noi_luc/100;
	}
	else
	{
Npc[nAttacker].m_CurrentMana = Npc[nAttacker].m_CurrentManaMax;
	}

}



if (bDoHurt)
DoHurt(0,0,0,bDoHurt);


	if (m_CurrentLife <= 0)
	{
		int nMode = DeathCalcPKValue(nAttacker);
		DoDeath(nMode,nAttacker);

		// if 死的是玩家，关闭他的仇杀和切磋状态
		if (m_Kind == kind_player)
			Player[m_nPlayerIdx].m_cPK.CloseAll();
	}
	else if (m_CurrentLifeMax > 0)
	{

int PecentLife = m_CurrentLife * 100/m_CurrentLifeMax;

if (PecentLife <= 25 && OldPecentLife > 25)
{

if (!m_CurrentTimeCastSkillEffec2.nTime)
{
if (g_RandPercent(m_CurrentDoSkillEffecPercent2.nValue[0]))
{

KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(m_CurrentDoSkillEffecPercent2.nValue[1],m_CurrentDoSkillEffecPercent2.nValue[2]);
if (pSkillActive)
{
KMagicAttrib* pMagicAttrib = pSkillActive->GetStateAttribs();
int pMagicAttribNum = pSkillActive->GetStateAttribsNum();
SetStateSkillEffect(m_Index, m_CurrentDoSkillEffecPercent2.nValue[1], m_CurrentDoSkillEffecPercent2.nValue[2], pMagicAttrib, pMagicAttribNum, pMagicAttrib[0].nValue[1]);
}

}


if (m_bRideHorse)
m_CurrentTimeCastSkillEffec2.nTime = m_CurrentTimeCastSkillEffec2.nValue[1];
else
m_CurrentTimeCastSkillEffec2.nTime = m_CurrentTimeCastSkillEffec2.nValue[0];

}

}







	}
}
#endif

#ifdef _SERVER
BOOL KNpc::ReceiveDamage(int nLauncher, BOOL bIsMelee, void *pData, BOOL bUseAR, BOOL bDoHurt, int nDameXG)
{
	if (m_Doing == do_death || m_Doing == do_revive)
		return FALSE;

	// 发技能的NPC不存在了，不用算了。
	if (!Npc[nLauncher].m_Index)
		return FALSE;

	if (!pData)
		return FALSE;

	KMagicAttrib *pTemp = NULL;

	pTemp = (KMagicAttrib *)pData;

	int nAr = pTemp->nValue[0];
	pTemp++;
	int nIgnoreAr = pTemp->nValue[0];
	pTemp++;
		if (bUseAR)
	{
		if (!CheckHitTarget(nAr, m_CurrentDefend, nIgnoreAr))
		{
			g_DebugLog("[数值]%s闪过攻击", Name);
			return FALSE;
		}
	}

/*
	if (m_Doing != do_death)
		DoHurt(m_HurtFrame);//Question ?*/
	int nLife = m_CurrentLife;

int nDameXGTT = 0;
int nDohurdXGTT = 0;

if (nDameXG == 1)
{
nDameXGTT = 150;
nDohurdXGTT = 60;
}
else if (nDameXG == 2)
{
nDameXGTT = 50;
nDohurdXGTT = 50;
}
else if (nDameXG == 3)
{
nDameXGTT = 50;
nDohurdXGTT = 20;
}
else if (nDameXG == 4)
{
nDameXGTT = 100;
nDohurdXGTT = 30;
}


int *tong_sat_thuong_vat_ly = new int[3];
int *tong_bang_sat = new int[3];
int *tong_hoa_sat = new int[3];
int *tong_loi_sat = new int[3];
int *tong_doc_sat = new int[3];
int *tong_sat_thuong = new int[3];
int *tong_lam_choang = new int[3];
int *tong_chi_mang = new int[3];
int *tong_chi_tu = new int[3];
int *tong_hut_sinh_luc = new int[3];
int *tong_hut_noi_luc = new int[3];
int *tong_ngu_hanh_tuong_khac = new int[3];
int *tong_luyen_ky_nang = new int[3];

int *tong_mien_hieu_ung = new int[3];
int *tong_tang_hinh = new int[3];
int *tong_dien_loan = new int[3];


tong_sat_thuong_vat_ly[0] = pTemp->nValue[0];
tong_sat_thuong_vat_ly[2] = pTemp->nValue[2];
	pTemp++;
tong_bang_sat[0] = pTemp->nValue[0];
tong_bang_sat[1] = pTemp->nValue[1];
tong_bang_sat[2] = pTemp->nValue[2];
	pTemp++;
tong_hoa_sat[0] = pTemp->nValue[0];
tong_hoa_sat[2] = pTemp->nValue[2];
	pTemp++;
tong_loi_sat[0] = pTemp->nValue[0];
tong_loi_sat[2] = pTemp->nValue[2];
	pTemp++;
tong_doc_sat[0] = pTemp->nValue[0];
tong_doc_sat[1] = pTemp->nValue[1];
tong_doc_sat[2] = pTemp->nValue[2];
	pTemp++;
tong_sat_thuong[0] = pTemp->nValue[0];
tong_sat_thuong[2] = pTemp->nValue[2];
	pTemp++;
tong_lam_choang[0] = pTemp->nValue[0];
tong_lam_choang[1] = pTemp->nValue[1];
    pTemp++;
tong_chi_mang[0] = pTemp->nValue[0];
    pTemp++;
tong_chi_tu[0] = pTemp->nValue[0];
	pTemp++;
tong_hut_sinh_luc[0] = pTemp->nValue[0];
	pTemp++;
tong_hut_noi_luc[0] = pTemp->nValue[0];
	pTemp++;
tong_ngu_hanh_tuong_khac[0] = pTemp->nValue[0];
tong_ngu_hanh_tuong_khac[1] = pTemp->nValue[1];
	pTemp++;
tong_luyen_ky_nang[0] = pTemp->nValue[0];
tong_luyen_ky_nang[1] = pTemp->nValue[1];
tong_luyen_ky_nang[2] = pTemp->nValue[2];

	pTemp++;
tong_mien_hieu_ung[0] = pTemp->nValue[0];
tong_mien_hieu_ung[1] = pTemp->nValue[1];
tong_mien_hieu_ung[2] = pTemp->nValue[2];
	pTemp++;
tong_tang_hinh[0] = pTemp->nValue[0];
tong_tang_hinh[1] = pTemp->nValue[1];
tong_tang_hinh[2] = pTemp->nValue[2];
	pTemp++;
tong_dien_loan[0] = pTemp->nValue[0];
tong_dien_loan[1] = pTemp->nValue[1];
tong_dien_loan[2] = pTemp->nValue[2];

if (tong_mien_hieu_ung[0] > 0)
{
if (g_RandPercent(tong_mien_hieu_ung[0]))
{
if (tong_mien_hieu_ung[1] > 0 && tong_mien_hieu_ung[2] > 0 && m_NstatusState.nTime <= 0)
{
	KStateNode* pNode;
    pNode = (KStateNode *)m_StateSkillList.GetHead();
	while(pNode)
	{
	if (pNode->m_SkillID > 0 && pNode->m_Level > 0)
	{
	KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(pNode->m_SkillID,pNode->m_Level);
	if (pOrdinSkill)
	{
	if (pOrdinSkill->IsTargetEnemy())
	AddSkillEffect(pNode->m_SkillID, pNode->m_Level, 0);			
	}
	}
	pNode = (KStateNode *)pNode->GetNext();
	}

m_NstatusState.nTime = tong_mien_hieu_ung[1];

KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(tong_mien_hieu_ung[2],1);

if (pSkillActive)
{
KMagicAttrib* pMagicAttrib = pSkillActive->GetStateAttribs();
int pMagicAttribNum = pSkillActive->GetStateAttribsNum();
SetStateSkillEffect(m_Index, tong_mien_hieu_ung[2], 1, pMagicAttrib, pMagicAttribNum, tong_mien_hieu_ung[1]);
}


}
}
}

if (tong_tang_hinh[0] > 0 && m_InvisibilityState.nTime <= 0)
{
m_InvisibilityState.nTime = tong_tang_hinh[0];
}

if (tong_dien_loan[0] > 0 && m_NstatusState.nTime <= 0)
{
if (g_RandPercent(tong_dien_loan[0]))
{
if (tong_dien_loan[1] > 0 && tong_dien_loan[2] > 0 && m_MadnessState.nTime <= 0)
{
m_MadnessState.nTime = tong_dien_loan[1];


KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(tong_dien_loan[2],1);

if (pSkillActive)
{
KMagicAttrib* pMagicAttrib = pSkillActive->GetStateAttribs();
int pMagicAttribNum = pSkillActive->GetStateAttribsNum();
SetStateSkillEffect(m_Index, tong_dien_loan[2], 1, pMagicAttrib, pMagicAttribNum, tong_dien_loan[1]);
}


}
}
}


if (Npc[nLauncher].IsPlayer())
{

int Map = SubWorld[Npc[nLauncher].m_SubWorldIndex].m_SubWorldID;
if (!IsPlayer() || (IsPlayer() && m_FightMode && Map == 33)  || (IsPlayer() && m_FightMode && Map == 37))
//if (!IsPlayer())
{

int nMaxSkillLevel = MAX_LEVEL_SKILL_EXP;


	if (Player[Npc[nLauncher].m_nPlayerIdx].m_cTask.GetSaveVal(139) == 1)
	{
	nMaxSkillLevel += 1;
	}
	else if (Player[Npc[nLauncher].m_nPlayerIdx].m_cTask.GetSaveVal(139) == 2)
	{
	nMaxSkillLevel += 2;
	}
	else if (Player[Npc[nLauncher].m_nPlayerIdx].m_cTask.GetSaveVal(139) == 3)
	{
	nMaxSkillLevel += 4;
	}
	else if (Player[Npc[nLauncher].m_nPlayerIdx].m_cTask.GetSaveVal(139) == 4)
	{
	nMaxSkillLevel += 6;
	}
	else if (Player[Npc[nLauncher].m_nPlayerIdx].m_cTask.GetSaveVal(139) == 5)
	{
	nMaxSkillLevel += 8;
	}






if (Npc[nLauncher].m_SkillList.GetLevel(tong_luyen_ky_nang[1]) > 0 && Npc[nLauncher].m_SkillList.GetLevel(tong_luyen_ky_nang[1]) < nMaxSkillLevel)
{
	int NumKyNang;
	if (Map == 33 || Map == 37)
	{
		NumKyNang = 50;
	}
	else if (Map == 37)
	{
		NumKyNang = 35;
	}
	else
	{
		SYSTEMTIME TimeNow;
		GetLocalTime(&TimeNow);
		
			if (Player[Npc[nLauncher].m_nPlayerIdx].m_cTask.GetSaveVal(509) > 0)
			{
				if (Player[Npc[nLauncher].m_nPlayerIdx].m_cTask.GetSaveVal(173) > 0)
				{
					if (Map == 20 || Map == 109 ) // luyen skill 
						NumKyNang = 70;
					else
						NumKyNang = 50;
				}
				else
				{
					if (Map == 20 || Map == 109)
							NumKyNang = 70;
					else
						NumKyNang = 50;
				}
			}
			else
			{
				if (Player[Npc[nLauncher].m_nPlayerIdx].m_cTask.GetSaveVal(173) > 0)
				{
					if (Map == 20 || Map == 109)
							NumKyNang = 70;
					else
						NumKyNang = 50;
				}
				else
				{
					if (Map == 20 || Map == 109)
							NumKyNang = 70;
					else
						NumKyNang = 50;
				}
			}
		}
	

	Player[Npc[nLauncher].m_nPlayerIdx].m_cTask.SetSaveVal(tong_luyen_ky_nang[2],Player[Npc[nLauncher].m_nPlayerIdx].m_cTask.GetSaveVal(tong_luyen_ky_nang[2]) + NumKyNang);
	
	if (Player[Npc[nLauncher].m_nPlayerIdx].m_cTask.GetSaveVal(tong_luyen_ky_nang[2]) >= tong_luyen_ky_nang[0])
	{
		Player[Npc[nLauncher].m_nPlayerIdx].m_cTask.SetSaveVal(tong_luyen_ky_nang[2],0);
		Npc[nLauncher].m_SkillList.IncreaseLevel(Npc[nLauncher].m_SkillList.GetSkillIdx(tong_luyen_ky_nang[1]),1);
		PLAYER_SKILL_LEVEL_SYNC NewSkill;
		NewSkill.ProtocolType = s2c_playerskilllevel;
		NewSkill.m_nSkillID = tong_luyen_ky_nang[1];
		NewSkill.m_nSkillLevel = Npc[nLauncher].m_SkillList.GetLevel(tong_luyen_ky_nang[1]);
		NewSkill.m_nLeavePoint = Player[Npc[nLauncher].m_nPlayerIdx].m_nSkillPoint;
		g_pServer->PackDataToClient(Player[Npc[nLauncher].m_nPlayerIdx].m_nNetConnectIdx, (BYTE*)&NewSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));
	}
}
}
}










int dame_nhan_chi_mang = 1;
if (g_RandPercent(tong_chi_mang[0]))
{
dame_nhan_chi_mang = 2;
}

if (m_NstatusState.nTime > 0  || m_Doing == do_goattack)
bDoHurt = false;

CalcDamage(nLauncher, tong_sat_thuong_vat_ly[0]*dame_nhan_chi_mang * (100 + nDameXGTT)/100, tong_sat_thuong_vat_ly[2]*dame_nhan_chi_mang  * (100 + nDameXGTT)/100, damage_physics, bIsMelee, bDoHurt  * (100 + nDohurdXGTT) / 100,FALSE,tong_ngu_hanh_tuong_khac[0],tong_ngu_hanh_tuong_khac[1],tong_hut_sinh_luc[0],tong_hut_noi_luc[0]);

CalcDamage(nLauncher, tong_bang_sat[0]*dame_nhan_chi_mang, tong_bang_sat[2]*dame_nhan_chi_mang, damage_cold, bIsMelee, bDoHurt,FALSE,tong_ngu_hanh_tuong_khac[0],tong_ngu_hanh_tuong_khac[1]);
	
if (m_FreezeState.nTime <= 0 && m_NstatusState.nTime <= 0 && m_Doing != do_goattack)
{

int giam_lam_cham = 0;

if (m_CurrentFreezeTimeReducePercent < 200)
{
giam_lam_cham = (m_CurrentFreezeTimeReducePercent * 100) / 200 ;
}
else
{
giam_lam_cham = 95;
}
if (giam_lam_cham >= 95)
{
	giam_lam_cham = 95;
}

m_FreezeState.nTime = tong_bang_sat[1] * (100 - giam_lam_cham) / 100;

}





CalcDamage(nLauncher, tong_hoa_sat[0]*dame_nhan_chi_mang, tong_hoa_sat[2]*dame_nhan_chi_mang, damage_fire, bIsMelee, bDoHurt,FALSE,tong_ngu_hanh_tuong_khac[0],tong_ngu_hanh_tuong_khac[1]);

CalcDamage(nLauncher, tong_loi_sat[0]*dame_nhan_chi_mang, tong_loi_sat[2]*dame_nhan_chi_mang, damage_light, bIsMelee, bDoHurt,FALSE,tong_ngu_hanh_tuong_khac[0],tong_ngu_hanh_tuong_khac[1]);




if (m_StunState.nTime <= 0 && m_NstatusState.nTime <= 0 && m_Doing != do_goattack)
{

int giam_lam_choang = 0;

if (m_CurrentStunTimeReducePercent <= 200)
{
giam_lam_choang = (m_CurrentStunTimeReducePercent * 100) / 200;
}
else
{
giam_lam_choang = 95;
}

if (giam_lam_choang >= 95)
{
giam_lam_choang = 95;
}

if (tong_lam_choang[0] > 0)
{
if (g_RandPercent(tong_lam_choang[0]))
{
m_StunState.nTime = tong_lam_choang[1] * (100 - giam_lam_choang) / 100;
}
}
}





int giam_trung_doc = 0;
if (m_CurrentPoisonTimeReducePercent <= 200)
{
giam_trung_doc = (m_CurrentPoisonTimeReducePercent * 100) / 200;
}
else
{
giam_trung_doc = 95;
}
if (giam_trung_doc >= 95)
{
	giam_trung_doc = 95;
}

	if (m_PoisonState.nTime == 0)
	{
		m_PoisonState.nTime = tong_doc_sat[1] * (100 - giam_trung_doc) / 100;
		m_PoisonState.nValue[0] = tong_doc_sat[0] * (100 - giam_trung_doc/2)/100;
		m_PoisonState.nValue[1] = tong_doc_sat[2];
	}
	else
	{
		int d1, d2, t1, t2, c1, c2, d3,t3,c3,e;
		d1 = m_PoisonState.nValue[0];
		d2 = tong_doc_sat[0] * (100 - giam_trung_doc)/100;
		t1 = m_PoisonState.nTime;
		t2 = tong_doc_sat[1] * (100 - giam_trung_doc) / 100;
		c1 = m_PoisonState.nValue[1];
		c2 = tong_doc_sat[2];
		if (c1 > 0 && c2 > 0 && d1 > 0 && d2 > 0 && t1 > 0 && t2 > 0) 
		{
		
			c3 = (c1 + c2)/2;
			t3 = max(t1,t2);
			d3 = (d1 * t1 / c1 + d2 * t2 / c2) * c3 / t3 ;

			
			m_PoisonState.nValue[0] = d3;
			m_PoisonState.nTime = min(t3 + t2/3, t2 * 4);
			m_PoisonState.nValue[1] = c3;
		}
	}
	


CalcDamage(nLauncher, tong_doc_sat[0] * (100 - giam_trung_doc)/100 * dame_nhan_chi_mang, tong_doc_sat[0] * (100 - giam_trung_doc)/100 * dame_nhan_chi_mang,damage_poison, bIsMelee, bDoHurt,FALSE,tong_ngu_hanh_tuong_khac[0],tong_ngu_hanh_tuong_khac[1]);



if (m_NstatusState.nTime > 0)
{
m_PoisonState.nTime = 0;
}


if (tong_chi_tu[0] > 0 && m_TypeNpc != 3)
{


if (m_CurrentLife > 0 && m_CurrentLife <= m_CurrentLifeMax)
{

int nXPecentCT = 50;

if ((tong_ngu_hanh_tuong_khac[1] == 0 && m_Series == 1) || (tong_ngu_hanh_tuong_khac[1] == 2 && m_Series == 3) || (tong_ngu_hanh_tuong_khac[1] == 1 && m_Series == 4) || (tong_ngu_hanh_tuong_khac[1] == 3 && m_Series == 0) || (tong_ngu_hanh_tuong_khac[1] == 4 && m_Series == 2))
{
nXPecentCT = 100;
}
else if ((tong_ngu_hanh_tuong_khac[1] == 0 && m_Series == 3) || (tong_ngu_hanh_tuong_khac[1] == 2 && m_Series == 4) || (tong_ngu_hanh_tuong_khac[1] == 1 && m_Series == 0) || (tong_ngu_hanh_tuong_khac[1] == 3 && m_Series == 2) || (tong_ngu_hanh_tuong_khac[1] == 4 && m_Series == 1))
{
nXPecentCT = 25;
}





int nPecentCTActive = g_Random(100);
int nPecentCTLife = m_CurrentLife * 100 /m_CurrentLifeMax;

if (nPecentCTActive < (tong_chi_tu[0] * nXPecentCT * nPecentCTLife / 200000))
{
m_CurrentLife -= (tong_chi_tu[0] * m_CurrentLife / 200) * nXPecentCT /100 * nPecentCTLife / 100;
}
else if (nPecentCTActive < (tong_chi_tu[0] * nXPecentCT * nPecentCTLife / 100000))
{
m_CurrentLife -= (tong_chi_tu[0] * m_CurrentLife / 400) * nXPecentCT /100 * nPecentCTLife / 100;
}
else if (nPecentCTActive < (tong_chi_tu[0] * nXPecentCT * nPecentCTLife / 50000))
{
m_CurrentLife -= (tong_chi_tu[0] * m_CurrentLife / 800) * nXPecentCT /100 * nPecentCTLife / 100;
}
else if (nPecentCTActive < (tong_chi_tu[0] * nXPecentCT * nPecentCTLife / 25000))
{
m_CurrentLife -= (tong_chi_tu[0] * m_CurrentLife / 1600) * nXPecentCT /100 * nPecentCTLife / 100;
}
else if (nPecentCTActive < (tong_chi_tu[0] * nXPecentCT * nPecentCTLife / 12500))
{
m_CurrentLife -= (tong_chi_tu[0] * m_CurrentLife / 3200) * nXPecentCT /100 * nPecentCTLife / 100;
}


}

}





	
CalcDamage(nLauncher, tong_sat_thuong[0]*dame_nhan_chi_mang, tong_sat_thuong[2]*dame_nhan_chi_mang, damage_magic, bIsMelee, bDoHurt,FALSE,tong_ngu_hanh_tuong_khac[0],tong_ngu_hanh_tuong_khac[1]);


if (tong_sat_thuong_vat_ly[0] > 0 || tong_bang_sat[0] > 0 || tong_hoa_sat[0] > 0 || tong_loi_sat[0] > 0 || tong_doc_sat[0] >0)
{

if (!Npc[nLauncher].m_CurrentTimeCastSkillEffec1.nTime && g_RandPercent(Npc[nLauncher].m_CurrentDoSkillEffecPercent1.nValue[0]) && m_Kind == kind_player)
{

KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(Npc[nLauncher].m_CurrentDoSkillEffecPercent1.nValue[1],Npc[nLauncher].m_CurrentDoSkillEffecPercent1.nValue[2]);

if (pSkillActive)
{
KMagicAttrib* pMagicAttrib = pSkillActive->GetStateAttribs();
int pMagicAttribNum = pSkillActive->GetStateAttribsNum();
SetStateSkillEffect(nLauncher, Npc[nLauncher].m_CurrentDoSkillEffecPercent1.nValue[1], Npc[nLauncher].m_CurrentDoSkillEffecPercent1.nValue[2], pMagicAttrib, pMagicAttribNum, pMagicAttrib[0].nValue[1]);
Npc[nLauncher].m_CurrentTimeCastSkillEffec1.nTime = Npc[nLauncher].m_CurrentTimeCastSkillEffec1.nValue[0];
}

}


if (g_RandPercent(m_ReturnDoSkillEmty.nValue[0]))
{
			int nLevel = m_ReturnDoSkillEmty.nValue[2];
			
	      
			if (nLevel > 0)
			{
				int nMpsX, nMpsY;
				SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
				
				NPC_SKILL_SYNC SkillCmd;
				SkillCmd.ID = this->m_dwID;


				SkillCmd.nSkillID = m_ReturnDoSkillEmty.nValue[1];
           

				SkillCmd.nSkillLevel = nLevel;
				SkillCmd.nMpsX = -1;
				SkillCmd.nMpsY = m_dwID;
				SkillCmd.ProtocolType = s2c_castskilldirectly;

				POINT	POff[8] = 
				{
					{0, 32},
					{-16, 32},
					{-16, 0},
					{-16, -32},
					{0, -32},
					{16, -32},
					{16, 0},
					{16, 32},
				};
				
				int nMaxCount = MAX_BROADCAST_COUNT;
				CURREGION.BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX, m_MapY);
				int i;
				for (i = 0; i < 8; i++)
				{
					if (CONREGIONIDX(i) == -1)
						continue;
					CONREGION(i).BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
				}
				
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_ReturnDoSkillEmty.nValue[1], nLevel);
			
				if (pOrdinSkill)
				{
                 pOrdinSkill->Cast(m_Index, nMpsX, nMpsY);
				}
              
			}
		}
	}

	m_nPeopleIdx = nLauncher;

	if (IsPlayer() && (m_CurrentLife - nLife < 0))
	{
		if (m_nPlayerIdx > 0)
		{
           
			Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeDefend);
		}
	}



delete tong_sat_thuong_vat_ly;
delete tong_bang_sat;
delete tong_hoa_sat;
delete tong_loi_sat;
delete tong_doc_sat;
delete tong_sat_thuong;
delete tong_lam_choang;
delete tong_chi_mang;
delete tong_chi_tu;
delete tong_hut_sinh_luc;
delete tong_hut_noi_luc;
delete tong_ngu_hanh_tuong_khac;
delete tong_luyen_ky_nang;
delete tong_mien_hieu_ung;
delete tong_tang_hinh;
delete tong_dien_loan;



	return TRUE;
}
#endif

void KNpc::SetImmediatelySkillEffect(int nLauncher, void *pData, int nDataNum)
{
	if (!pData || !nDataNum)
		return;

	KMagicAttrib*	pTemp = (KMagicAttrib *)pData;
	_ASSERT(nDataNum <= MAX_SKILL_STATE);
	for (int i = 0; i < nDataNum; i++)
	{
		ModifyAttrib(nLauncher, pTemp);
		pTemp++;
	}
}

void KNpc::AppendSkillEffect(int nIdSkillActive,BOOL bIsSkillPhysical,BOOL bIsSkillMagic,BOOL bIsPhysical, BOOL bIsMelee, void *pSrcData, void *pDesData)
{

int CurrentSkillAddDameActive = 0;


if (nIdSkillActive > 0 && nIdSkillActive <= 500)
{
CurrentSkillAddDameActive = m_SkillList.m_CurrentListSkillAddDame[nIdSkillActive-1];
}




int DamePecentToLevel = 0;


if (IsPlayer())
{
if (m_Level > 100 && m_Level <= 200)
{
DamePecentToLevel = (m_Level - 100)/5;
}
}


	int nMinDamage = m_PhysicsDamage.nValue[0] + m_CurrentAddPhysicsDamage;
	int	nMaxDamage = m_PhysicsDamage.nValue[2] + m_CurrentAddPhysicsDamage;

	KMagicAttrib* pTemp = (KMagicAttrib *)pSrcData;
	KMagicAttrib* pDes = (KMagicAttrib *)pDesData;



	// Get AR_p
	if (pTemp->nAttribType == magic_attackrating_p)
	{
		pDes->nAttribType = magic_attackrating_v;
		pDes->nValue[0] = m_CurrentAttackRating + m_AttackRating * pTemp->nValue[0] / 100;
	}
	else
	{
		pDes->nAttribType = magic_attackrating_v;
		pDes->nValue[0] = m_CurrentAttackRating;
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_ignoredefense_p)
	{
		pDes->nAttribType = magic_ignoredefense_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_physicsenhance_p)
	{
		pDes->nAttribType = magic_physicsdamage_v;
		pDes->nValue[0] = nMinDamage * (100 + pTemp->nValue[0]*(100+CurrentSkillAddDameActive)/100) / 100;
		pDes->nValue[2] = nMaxDamage * (100 + pTemp->nValue[0]*(100+CurrentSkillAddDameActive)/100) / 100;
		if (IsPlayer())
		{



           if (m_nPlayerIdx <= 0 || m_nPlayerIdx >= MAX_PLAYER)
			{
			printf("Loi tinh dame pidx xuat chieu !\n");
	        return;
			}


			if (Player[m_nPlayerIdx].m_ItemList.GetWeaponType() == equip_meleeweapon)
			{


            if (Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular() >= MAX_MELEE_WEAPON || Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular() < 0)
			{
				printf("Loi tinh dame vu khi xuat chieu !\n");
				return;
			}


				pDes->nValue[0] += nMinDamage * m_CurrentMeleeEnhance[Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular()] / 100;
				pDes->nValue[2] += nMaxDamage * m_CurrentMeleeEnhance[Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular()] / 100;
			}
			else if (Player[m_nPlayerIdx].m_ItemList.GetWeaponType() == equip_rangeweapon)
			{
				pDes->nValue[0] += nMinDamage * m_CurrentRangeEnhance / 100;
				pDes->nValue[2] += nMaxDamage * m_CurrentRangeEnhance / 100;
			}
			else	// 空手
			{
				pDes->nValue[0] += nMinDamage * m_CurrentHandEnhance / 100;
				pDes->nValue[2] += nMaxDamage * m_CurrentHandEnhance / 100;
			}
		}
	}
	else if (pTemp->nAttribType == magic_physicsdamage_v)
	{
		pDes->nAttribType = magic_physicsdamage_v;
		pDes->nValue[0] = pTemp->nValue[0]*(100+CurrentSkillAddDameActive)/100;
		pDes->nValue[2] = pTemp->nValue[2]*(100+CurrentSkillAddDameActive)/100;
		
		
		
	if (!bIsPhysical)
	{
	    pDes->nValue[0] += m_PhysicsMagic.nValue[0] + m_CurrentAddPhysicsMagic;
	    pDes->nValue[2] += m_PhysicsMagic.nValue[2] + m_CurrentAddPhysicsMagic;

        pDes->nValue[0] += (pDes->nValue[0] * DamePecentToLevel)/MAX_PERCENT;
		pDes->nValue[2] += (pDes->nValue[2] * DamePecentToLevel)/MAX_PERCENT;
	}

      if (bIsSkillMagic)
	     {
		 pDes->nValue[0] += m_CurrentPhysicsDamage2.nValue[0];
		 pDes->nValue[2] += m_CurrentPhysicsDamage2.nValue[2];


		 pDes->nValue[0] += (pDes->nValue[0] * DamePecentToLevel)/100;
		 pDes->nValue[2] += (pDes->nValue[2] * DamePecentToLevel)/100;
		 


	     }


	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_colddamage_v)
	{
		pDes->nAttribType = magic_colddamage_v;
		pDes->nValue[0] = pTemp->nValue[0]*(100+CurrentSkillAddDameActive)/100;
		pDes->nValue[1] = pTemp->nValue[1] + m_CurrentColdEnhance;
		pDes->nValue[2] = pTemp->nValue[2]*(100+CurrentSkillAddDameActive)/100;
		
		
	if (!bIsPhysical)
	{
		pDes->nValue[0] += m_CurrentColdMagic.nValue[0];
		pDes->nValue[1] = max(pDes->nValue[1], m_CurrentColdMagic.nValue[1] + m_CurrentColdEnhance);
		pDes->nValue[2] += m_CurrentColdMagic.nValue[2];

		pDes->nValue[0] += (pDes->nValue[0] * DamePecentToLevel) / MAX_PERCENT;
		pDes->nValue[2] += (pDes->nValue[2] * DamePecentToLevel) / MAX_PERCENT;
	}

	if (bIsSkillMagic)
	 {
        pDes->nValue[0] += m_CurrentColdDamage2.nValue[0];
		pDes->nValue[1] = max(pDes->nValue[1], m_CurrentColdDamage2.nValue[1] + m_CurrentColdEnhance);
		pDes->nValue[2] += m_CurrentColdDamage2.nValue[2];

		 pDes->nValue[0] += (pDes->nValue[0] * DamePecentToLevel)/100;
		 pDes->nValue[2] += (pDes->nValue[2] * DamePecentToLevel)/100;
	 }

	}

	if (bIsSkillPhysical)
	{
		pDes->nValue[0] += m_CurrentColdDamage.nValue[0];
		pDes->nValue[1] = max(pDes->nValue[1], m_CurrentColdDamage.nValue[1] + m_CurrentColdEnhance);
		pDes->nValue[2] += m_CurrentColdDamage.nValue[2];
	}



	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_firedamage_v)
	{
		// How to use FireEnhance???
		pDes->nAttribType = magic_firedamage_v;
		pDes->nValue[0] = pTemp->nValue[0]*(100+CurrentSkillAddDameActive)/100 + pTemp->nValue[0]*(100+CurrentSkillAddDameActive)/100 * m_CurrentFireEnhance/100;
		pDes->nValue[2] = pTemp->nValue[2]*(100+CurrentSkillAddDameActive)/100 + pTemp->nValue[2]*(100+CurrentSkillAddDameActive)/100 * m_CurrentFireEnhance/100;

	if (bIsSkillMagic)
	{
       pDes->nValue[0] += m_CurrentFireDamage2.nValue[0] + m_CurrentFireDamage2.nValue[0] * m_CurrentFireEnhance /100;
       pDes->nValue[2] += m_CurrentFireDamage2.nValue[2] + m_CurrentFireDamage2.nValue[2] * m_CurrentFireEnhance /100;

	   pDes->nValue[0] += (pDes->nValue[0] * DamePecentToLevel)/100;
	   pDes->nValue[2] += (pDes->nValue[2] * DamePecentToLevel)/100;
	}

	}


	if (bIsSkillPhysical)
	{
		pDes->nValue[0] += m_CurrentFireDamage.nValue[0] + m_CurrentFireDamage.nValue[0] * m_CurrentFireEnhance /100;
		pDes->nValue[2] += m_CurrentFireDamage.nValue[2] + m_CurrentFireDamage.nValue[2] * m_CurrentFireEnhance /100;
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_lightingdamage_v)
	{
		pDes->nAttribType = magic_lightingdamage_v;
		pDes->nValue[0] = pTemp->nValue[0]*(100+CurrentSkillAddDameActive)/100 + (pTemp->nValue[2]*(100+CurrentSkillAddDameActive)/100 - pTemp->nValue[0]*(100+CurrentSkillAddDameActive)/100) * m_CurrentLightEnhance / 100;
		pDes->nValue[2] = pTemp->nValue[2]*(100+CurrentSkillAddDameActive)/100;

	if (bIsSkillMagic)
	{
       pDes->nValue[0] += m_CurrentLightDamage2.nValue[0];
       pDes->nValue[2] += m_CurrentLightDamage2.nValue[2];

	   pDes->nValue[0] += (pDes->nValue[0] * DamePecentToLevel)/100;
	   pDes->nValue[2] += (pDes->nValue[2] * DamePecentToLevel)/100;
	}

	}
	if (bIsSkillPhysical)
	{
		pDes->nValue[0] += m_CurrentLightDamage.nValue[0];
		pDes->nValue[2] += m_CurrentLightDamage.nValue[2];
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_poisondamage_v)
	{
		pDes->nAttribType = magic_poisondamage_v;
		pDes->nValue[0] = pTemp->nValue[0]*(100+CurrentSkillAddDameActive)/100;
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2] * (100 - m_CurrentPoisonEnhance) / 100;
		if (pDes->nValue[2] <= 0)
			pDes->nValue[2] = 1;
		
		if (!bIsPhysical) //!bIsPhysical phu dinh Damage noi ngoai cong
		{
			g_NpcAttribModify.MixPoisonDamage(pDes, &m_CurrentPoisonMagic);
			pDes->nValue[0] = m_PhysicsMagic.nValue[0] + pTemp->nValue[0] * (MAX_PERCENT - m_CurrentPoisonEnhance) / MAX_PERCENT;
			pDes->nValue[2] = m_PhysicsMagic.nValue[2] + pTemp->nValue[2] * (MAX_PERCENT - m_CurrentPoisonEnhance) / MAX_PERCENT;


			pDes->nValue[0] += pDes->nValue[0];
			pDes->nValue[2] += pDes->nValue[2];

		}

	if (bIsSkillMagic)
	{
		g_NpcAttribModify.MixPoisonDamage(pDes, &m_CurrentPoisonDamage2);
		pDes->nValue[0] += (pDes->nValue[0] * DamePecentToLevel)/100;	
		if (nIdSkillActive != 388 && nIdSkillActive !=359 && nIdSkillActive != 360 && nIdSkillActive != 203 && nIdSkillActive != 464 && nIdSkillActive != 465 && nIdSkillActive != 63 && nIdSkillActive != 68 && nIdSkillActive != 71)
		{
			pDes->nValue[0]  = 0;
		}
	//	if (nIdSkillActive == 203 && pDes->nValue[0] >= 200)
	//	{
	//		pDes->nValue[0] = 200;
	//	}

	}

	}
	if (bIsSkillPhysical)
	{
		// 合成毒伤害
		g_NpcAttribModify.MixPoisonDamage(pDes, &m_CurrentPoisonDamage);
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_magicdamage_v)
	{
		pDes->nAttribType = magic_magicdamage_v;
		pDes->nValue[0] = pTemp->nValue[0]*(100+CurrentSkillAddDameActive)/100;
		pDes->nValue[2] = pTemp->nValue[2]*(100+CurrentSkillAddDameActive)/100;
	}

	pTemp++;
	pDes++;

	if (pTemp->nAttribType == magic_stun_p)
	{
		pDes->nAttribType = magic_stun_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
	}

	pTemp++;
	pDes++;

	if (pTemp->nAttribType == magic_deadlystrike_p)
	{
		pDes->nAttribType = magic_deadlystrike_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}

    if (bIsSkillPhysical)
	{
		if (nIdSkillActive != 386)
		{
    pDes->nValue[0] += m_CurrentDeadlyStrike;
		}
		else
		{
    pDes->nValue[0] += m_CurrentDeadlyStrike*2/3;
		}
	}

	pTemp++;
	pDes++;

	if (pTemp->nAttribType == magic_fatallystrike_p)
	{
		pDes->nAttribType = magic_fatallystrike_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}


	pTemp++;
	pDes++;

	if (pTemp->nAttribType == magic_steallife_p)
	{
		pDes->nAttribType = magic_steallife_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}

    if (bIsSkillPhysical)
	{
    pDes->nValue[0] += m_CurrentLifeStolen;
	}


	pTemp++;
	pDes++;

	if (pTemp->nAttribType == magic_stealmana_p)
	{
		pDes->nAttribType = magic_stealmana_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}

    if (bIsSkillPhysical)
	{
    pDes->nValue[0] += m_CurrentManaStolen;
	}

	pTemp++;
	pDes++;

	if (pTemp->nAttribType == magic_seriesdame)
	{
		pDes->nAttribType = magic_seriesdame;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
	}

	pTemp++;
	pDes++;

	if (pTemp->nAttribType == magic_skilllevelexp)
	{
		pDes->nAttribType = magic_skilllevelexp;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}




	pTemp++;
	pDes++;

	if (pTemp->nAttribType == magic_nstatus_p)
	{
		pDes->nAttribType = magic_skilllevelexp;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}

	pTemp++;
	pDes++;

	if (pTemp->nAttribType == magic_invisibility_p)
	{
		pDes->nAttribType = magic_skilllevelexp;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}

	pTemp++;
	pDes++;

	if (pTemp->nAttribType == magic_madness_p)
	{
		pDes->nAttribType = magic_skilllevelexp;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}


}

int KNpc::CheckPath(int x0, int y0, int s)
{
	if (s >= defLOGIC_CELL_WIDTH)
	{
		s = defLOGIC_CELL_WIDTH - 1;
	}

	int x, y;

	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, 0, 0, &x, &y);
	x = (x << 10) + m_OffX;
	y = (y << 10) + m_OffY;

return m_PathFinder.GetDir(x, y, m_Dir, x0, y0, s, &m_Dir);

}

void KNpc::ServeMove(int MoveSpeed)
{
	if (m_Doing != do_walk && m_Doing != do_run && m_Doing != do_hurt && m_Doing != do_runattack && m_Doing != do_goattack)
		return;

	if (MoveSpeed <= 0)
		return;

	if (MoveSpeed >= defLOGIC_CELL_WIDTH)
	{
		MoveSpeed = defLOGIC_CELL_WIDTH - 1;
	}

#ifndef _SERVER
	if (m_RegionIndex < 0 || m_RegionIndex >= 9)
	{
		g_DebugLog("[zroc]Npc(%d)ServerMove RegionIdx = %d", m_Index, m_RegionIndex);
		_ASSERT(0);
		DoStand();
		return;
	}
#else
	_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;
#endif
	int x, y;

	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, 0, 0, &x, &y);
	x = (x << 10) + m_OffX;
	y = (y << 10) + m_OffY;

	int nRet = m_PathFinder.GetDir(x, y, m_Dir, m_DesX, m_DesY, MoveSpeed, &m_Dir);

#ifndef _SERVER
	if(nRet == 1)
	{
		x = g_DirCos(m_Dir, 64) * MoveSpeed;
		y = g_DirSin(m_Dir, 64) * MoveSpeed;
	}
	else if (nRet == 0)
	{
		g_DebugLog("DoStand: %d - %d - %d \n ",m_MapX,m_MapY,m_Index);
		DoStand();
		return;
	}
	else if (nRet == -1)
	{
		SubWorld[0].m_Region[m_RegionIndex].RemoveNpc(m_Index);
		SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
		g_DebugLog("Xoa lien tuc: %d - %d - %d \n ",m_MapX,m_MapY,m_Index);
		m_RegionIndex = -1;
		return;
	}
	else
	{
		return;
	}
#endif
#ifdef _SERVER
	if(nRet == 1)
	{
		x = g_DirCos(m_Dir, 64) * MoveSpeed;
		y = g_DirSin(m_Dir, 64) * MoveSpeed;
	}
	else
	{
		DoStand();
		return;
	}
#endif

	int nOldRegion = m_RegionIndex;
	int nOldMapX = m_MapX;
	int nOldMapY = m_MapY;
	int nOldOffX = m_OffX;
	int nOldOffY = m_OffY;

	m_OffX += x;
	m_OffY += y;
//	处理NPC的坐标变幻
//	CELLWIDTH、CELLHEIGHT、OffX、OffY均是放大了1024倍

	if (!m_bClientOnly)
		CURREGION.DecRef(m_MapX, m_MapY, obj_npc);
	
	if (m_OffX < 0)
	{
		m_MapX--;
		m_OffX += CELLWIDTH;
	}
	else if (m_OffX > CELLWIDTH)
	{
		m_MapX++;
		m_OffX -= CELLWIDTH;
	}

	if (m_OffY < 0)
	{
		m_MapY--;
		m_OffY += CELLHEIGHT;
	}
	else if (m_OffY > CELLHEIGHT)
	{
		m_MapY++;
		m_OffY -= CELLHEIGHT;
	}
	
	if (m_MapX < 0)
	{
		m_RegionIndex = LEFTREGIONIDX;
		m_MapX += REGIONWIDTH;
	}
	else if (m_MapX >= REGIONWIDTH)
	{
		m_RegionIndex = RIGHTREGIONIDX;
		m_MapX -= REGIONWIDTH;
	}

	if (m_RegionIndex >= 0)
	{
		if (m_MapY < 0)
		{
			m_RegionIndex = UPREGIONIDX;
			m_MapY += REGIONHEIGHT;
		}
		else if (m_MapY >= REGIONHEIGHT)
		{
			m_RegionIndex = DOWNREGIONIDX;
			m_MapY -= REGIONHEIGHT;
		}
		if (!m_bClientOnly && m_RegionIndex >= 0)
			CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
	}

	if (m_RegionIndex == -1)	// 不可能移动到-1 Region，如果出现这种情况，恢复原坐标
	{
		m_RegionIndex = nOldRegion;
		m_MapX = nOldMapX;
		m_MapY = nOldMapY;
		m_OffX = nOldOffX;
		m_OffY = nOldOffY;
		CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
		return;
	}

	if (nOldRegion != m_RegionIndex)
	{
#ifdef _SERVER
		SubWorld[m_SubWorldIndex].NpcChangeRegion(nOldRegion, m_RegionIndex, m_Index);
		if (IsPlayer())
		{
			SubWorld[m_SubWorldIndex].PlayerChangeRegion(nOldRegion, m_RegionIndex, m_nPlayerIdx);
			if (m_nPlayerIdx > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeMove);
			}
		}
#else
		SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index);
		m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
#endif
	}
}

void KNpc::ServeJump(int nSpeed)
{
	_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

	if (!(m_Doing == do_jump || m_Doing == do_jumpattack))
		return;
	
	if (nSpeed <= 0)
		return;

	if (nSpeed >= defLOGIC_CELL_WIDTH)
	{
		nSpeed = defLOGIC_CELL_WIDTH - 1;
	}

	m_OffX += g_DirCos(m_JumpDir, 64) * nSpeed;
	m_OffY += g_DirSin(m_JumpDir, 64) * nSpeed;

	// s = vt - a * t * t / 2
	m_Height = (m_JumpFirstSpeed * m_Frames.nCurrentFrame - ACCELERATION_OF_GRAVITY * m_Frames.nCurrentFrame * m_Frames.nCurrentFrame / 2) / 8;
	if (m_Height < 0)
		m_Height = 0;

	int nOldRegion = m_RegionIndex;
	int nOldMapX = m_MapX;
	int nOldMapY = m_MapY;
	int nOldOffX = m_OffX;
	int nOldOffY = m_OffY;
	CURREGION.DecRef(m_MapX, m_MapY, obj_npc);
	
	if (m_OffX < 0)
	{
		m_MapX--;
		m_OffX += CELLWIDTH;
	}
	else if (m_OffX > CELLWIDTH)
	{
		m_MapX++;
		m_OffX -= CELLWIDTH;
	}
	
	if (m_OffY < 0)
	{
		m_MapY--;
		m_OffY += CELLHEIGHT;
	}
	else if (m_OffY > CELLHEIGHT)
	{
		m_MapY++;
		m_OffY -= CELLHEIGHT;
	}
	
	if (m_MapX < 0)
	{
		m_RegionIndex = LEFTREGIONIDX;
		m_MapX += REGIONWIDTH;
	}
	else if (m_MapX >= REGIONWIDTH)
	{
		m_RegionIndex = RIGHTREGIONIDX;
		m_MapX -= REGIONWIDTH;
	}
	
	if (m_RegionIndex >= 0)
	{
		if (m_MapY < 0)
		{
			m_RegionIndex = UPREGIONIDX;
			m_MapY += REGIONHEIGHT;
		}
		else if (m_MapY >= REGIONHEIGHT)
		{
			m_RegionIndex = DOWNREGIONIDX;
			m_MapY -= REGIONHEIGHT;
		}
		if (m_RegionIndex >= 0)
			CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
	}

	if (m_RegionIndex == -1)	// 不可能移动到-1 Region，如果出现这种情况，恢复原坐标
	{
		m_RegionIndex = nOldRegion;
		m_MapX = nOldMapX;
		m_MapY = nOldMapY;
		m_OffX = nOldOffX;
		m_OffY = nOldOffY;
		CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
		return;
	}

	if (nOldRegion != m_RegionIndex)
	{
#ifdef _SERVER
		SubWorld[m_SubWorldIndex].NpcChangeRegion(nOldRegion, m_RegionIndex, m_Index);
		if (IsPlayer())
		{
			SubWorld[m_SubWorldIndex].PlayerChangeRegion(nOldRegion, m_RegionIndex, m_nPlayerIdx);
			if (m_nPlayerIdx > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeMove);
			}
		}
#else
		if (m_RegionIndex >= 0)
		{
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index);
			m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
		}
#endif
	}		
}

void KNpc::SendCommand(NPCCMD cmd,int x,int y, int z)
{
	if (this->IsPlayer())
	{
	if ((Player[this->m_nPlayerIdx].CheckShopOpen() || Player[this->m_nPlayerIdx].CheckShopBuyOpen()) && cmd != do_revive)
		return;

	}



	m_Command.CmdKind = cmd;
	m_Command.Param_X = x;
	m_Command.Param_Y = y;
	m_Command.Param_Z = z;
}

BOOL KNpc::NewPath(int nMpsX, int nMpsY)
{
	m_DesX = nMpsX;
	m_DesY = nMpsY;
	return TRUE;
}

BOOL KNpc::NewJump(int nMpsX, int nMpsY)
{
	//_ASSERT(m_CurrentJumpSpeed > 0);
	if (m_CurrentJumpSpeed <= 0)
		return FALSE;


	int nX, nY;
	GetMpsPos(&nX, &nY);

	if (nX == nMpsX && nY == nMpsY)
		return FALSE;


	int nRangeCheckL = (nX - nMpsX) * (nX - nMpsX) + (nY - nMpsY) * (nY - nMpsY);

	if (nRangeCheckL <= 32 * 32)
		return FALSE;


	int nDir = g_GetDirIndex(nX, nY, nMpsX, nMpsY);
	int	nMaxLength = m_CurrentJumpSpeed * m_CurrentJumpFrame;
	int	nWantLength = g_GetDistance(nX, nY, nMpsX, nMpsY);
	int	nSin = g_DirSin(nDir, 64);
	int	nCos = g_DirCos(nDir, 64);

	if (nWantLength > nMaxLength)
	{
		m_DesX = nX + ((nMaxLength * nCos) >> 10);
		m_DesY = nY + ((nMaxLength * nSin) >> 10);
		nWantLength = nMaxLength;
	}
	else if (nWantLength <= MIN_JUMP_RANGE)
	{
		m_DesX = nMpsX;
		m_DesY = nMpsY;
		return FALSE;
	}

	m_JumpStep = nWantLength / m_CurrentJumpSpeed;

	int nTestX = 0;
	int nTestY = 0;
	int nSuccessStep = 0;




	for (int i = 1; i < m_JumpStep + 1; i++)
	{
		nTestX = nX + ((m_CurrentJumpSpeed * nCos * i) >> 10);
		nTestY = nY + ((m_CurrentJumpSpeed * nSin * i) >> 10);





		int nBarrier = SubWorld[m_SubWorldIndex].GetBarrier(nTestX, nTestY);
	

		if (Obstacle_NULL == nBarrier)
		{
			nSuccessStep = i;
		}
		if (Obstacle_Normal == nBarrier || Obstacle_Fly == nBarrier)
		{
			
			if (nSuccessStep <= MIN_JUMP_RANGE / m_CurrentJumpSpeed)
			{
				return FALSE;
			}
			m_DesX = nX + ((m_CurrentJumpSpeed * nCos * (nSuccessStep-1)) >> 10);
			m_DesY = nY + ((m_CurrentJumpSpeed * nSin * (nSuccessStep-1)) >> 10);
			m_JumpStep = nSuccessStep-1;
			break;
		}
		int	nTrap = SubWorld[m_SubWorldIndex].GetTrap(nTestX, nTestY);
		if (nTrap)
		{
			if (i <= MIN_JUMP_RANGE / m_CurrentJumpSpeed)
			{
				return FALSE;
			}
			m_DesX = nX + ((m_CurrentJumpSpeed * nCos * i) >> 10);
			m_DesY = nY + ((m_CurrentJumpSpeed * nSin * i) >> 10);
			m_JumpStep = i;
			break;
		}
	}
	m_JumpDir = nDir;
	return TRUE;
}

void KNpc::SelfDamage(int nDamage)
{
	m_CurrentLife -= nDamage;
	if (m_CurrentLife <= 0)
	{
		m_CurrentLife = 1;
	}
}

BOOL KNpc::Cost(NPCATTRIB nType, int nCost, BOOL bOnlyCheckCanCast)
{
	if (!IsPlayer())
		return TRUE;

	int *pSource = NULL;

	switch(nType)
	{
	case attrib_mana:
		pSource = &m_CurrentMana;
		break;
	case attrib_life:
		pSource = &m_CurrentLife;
		break;
	case attrib_stamina:
		pSource = &m_CurrentStamina;
		break;
	default:
		break;
	}

	if (pSource)
	{
		if (*pSource < nCost)
		{
#ifndef _SERVER
			KSystemMessage Msg;

			Msg.byConfirmType = SMCT_NONE;
			Msg.byParamSize = 0;
			Msg.byPriority = 1;
			Msg.eType = SMT_NORMAL;
			switch(nType)
			{
			case attrib_mana:
				g_StrCpyLen(Msg.szMessage, MSG_NPC_NO_MANA, sizeof(Msg.szMessage));
				break;
			case attrib_life:
				g_StrCpyLen(Msg.szMessage, MSG_NPC_NO_LIFE, sizeof(Msg.szMessage));
				break;
			case attrib_stamina:
				g_StrCpyLen(Msg.szMessage, MSG_NPC_NO_STAMINA, sizeof(Msg.szMessage));
				break;
			default:
				break;
			}
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
#endif
			return FALSE;
		}
		else
		{
			if (!bOnlyCheckCanCast)
				*pSource -= nCost;
			return TRUE;
		}
	}
	return FALSE;
}

void KNpc::DoJump()
{

	_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

	if (m_Doing == do_jump)
		return;
	
	m_Doing = do_jump;
	m_Dir = m_JumpDir;
	m_ProcessAI	= 0;
	m_JumpFirstSpeed = ACCELERATION_OF_GRAVITY * (m_JumpStep - 1) / 2 ;
#ifdef _SERVER	// 向周围9个Region广播发技能
	NPC_JUMP_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcjump;
	NetCommand.ID = m_dwID;
	NetCommand.nMpsX = m_DesX;
	NetCommand.nMpsY = m_DesY;
		
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	for (int i= 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif

#ifndef _SERVER
		m_ClientDoing = cdo_jump;
#endif

	m_Frames.nTotalFrame = m_JumpStep;
	m_Frames.nCurrentFrame = 0;	
}

BOOL KNpc::OnJump()
{
	ServeJump(m_CurrentJumpSpeed);
	if (WaitForFrame())
	{
		DoStand();
		m_ProcessAI	= 1;
		return FALSE;
	}
	return TRUE;
}

BOOL KNpc::WaitForFrame()
{
	m_Frames.nCurrentFrame++;
	if (m_Frames.nCurrentFrame < m_Frames.nTotalFrame)
	{
		return FALSE;
	}
	m_Frames.nCurrentFrame = 0;
	return TRUE;
}

BOOL KNpc::IsReachFrame(int nPercent)
{
	if (m_Frames.nCurrentFrame == m_Frames.nTotalFrame * nPercent / 100)
	{
		return TRUE;
	}
	return FALSE;
}

//客户端从网络得到的NpcSettingIdx是包含高16位Npc的模板号与低16位为等级
void KNpc::Load(int nNpcSettingIdx, int nLevel)
{
	m_PathFinder.Init(m_Index);
	if (nLevel <= 0) nLevel = 1;

#ifndef _SERVER
	char	szNpcTypeName[32];
#endif
	if (nNpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID || nNpcSettingIdx == PLAYER_FEMALE_NPCTEMPLATEID)
	{
		m_NpcSettingIdx = nNpcSettingIdx;
		m_Level = nLevel;
#ifndef _SERVER
		if (nNpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID)
		{
			strcpy(szNpcTypeName, "男主角");
			m_StandFrame = NpcSet.GetPlayerStandFrame(TRUE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(TRUE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(TRUE);
		}
		else
		{
			strcpy(szNpcTypeName, "女主角");
			m_StandFrame = NpcSet.GetPlayerStandFrame(FALSE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(FALSE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(FALSE);
		}
#endif
		//		TODO: Load Player Data;
		m_WalkSpeed = NpcSet.GetPlayerWalkSpeed();
		m_RunSpeed = NpcSet.GetPlayerRunSpeed();
		m_AttackFrame = NpcSet.GetPlayerAttackFrame();
		m_HurtFrame	= NpcSet.GetPlayerHurtFrame();
	}
	else
	{
		GetNpcCopyFromTemplate(nNpcSettingIdx, nLevel);

#ifndef _SERVER	
		g_NpcSetting.GetString(nNpcSettingIdx + 2, "NpcResType", "", szNpcTypeName, sizeof(szNpcTypeName));
		if (!szNpcTypeName[0])
		{
			g_NpcKindFile.GetString(2, "人物名称", "", szNpcTypeName, sizeof(szNpcTypeName));//如果没找到，用第一个npc代替
		}
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIMode", 12, &m_AiMode);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam1", 12, &m_AiParam[0]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam2", 12, &m_AiParam[1]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam3", 12, &m_AiParam[2]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam4", 12, &m_AiParam[3]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam5", 12, &m_AiParam[4]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam6", 12, &m_AiParam[5]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam7", 12, &m_AiParam[6]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "ActiveRadius", 12, &m_ActiveRadius);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "ClientOnly", 0, &m_bClientOnly);
		// 飞行类，11，12，17，用AiParam[6]保存策划设定高度
		// add by flying
		if (m_AiMode == 11 || m_AiMode == 12 || m_AiMode == 17)
			m_AiParam[6] = m_AiMode;
#endif
	}
#ifndef _SERVER
	m_DataRes.Init(szNpcTypeName, &g_NpcResList);
	m_DataRes.SetAction(m_ClientDoing);
	m_DataRes.SetRideHorse(m_bRideHorse);
	// m_DataRes.SetArmor(m_ArmorType,0,0);
	//fifong
	if (m_bFifongState == TRUE)
			m_DataRes.SetArmor(m_ArmorType, m_FifongType, TRUE);
		else
			m_DataRes.SetArmor(m_ArmorType, 0, FALSE);
	// end
	m_DataRes.SetHelm(m_HelmType);
	m_DataRes.SetHorse(m_HorseType);
	m_DataRes.SetWeapon(m_WeaponType);
#endif
	m_CurrentCamp = m_Camp;
}

void KNpc::GetMpsPos(int *pPosX, int *pPosY)
{
#ifdef _SERVER
	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, pPosX, pPosY);
#else
	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, pPosX, pPosY);
//	KSubWorld::Map2Mps(C_REGION_X(m_RegionIndex), C_REGION_Y(m_RegionIndex), m_MapX, m_MapY, m_OffX, m_OffY, pPosX, pPosY);
#endif
}

BOOL	KNpc::SetActiveSkill(int nSkillIdx)
{
	if (nSkillIdx <= 0 || nSkillIdx >= MAX_NPCSKILL)
		return FALSE;

	if (!(m_SkillList.m_Skills[nSkillIdx].SkillId && m_SkillList.m_Skills[nSkillIdx].CurrentSkillLevel))
		return FALSE;

	m_ActiveSkillID = m_SkillList.m_Skills[nSkillIdx].SkillId;
	int nLevel = m_SkillList.m_Skills[nSkillIdx].SkillLevel;
	_ASSERT(m_ActiveSkillID < MAX_SKILL && nLevel < MAX_SKILLLEVEL && nLevel > 0);
	
	ISkill * pISkill =  g_SkillManager.GetSkill(m_ActiveSkillID, nLevel);
	if (pISkill)
    {
		m_CurrentAttackRadius = pISkill->GetAttackRadius();
	
    }
	
	//g_DebugLog("Test Su Dung: %d \n",m_ActiveSkillID);
	return TRUE;
}

void KNpc::SetAuraSkill(int nSkillID)
{
	int nCurLevel = 0;
	if (nSkillID <= 0 || nSkillID >= MAX_SKILL) 
    {
        nSkillID = 0;
    }
	else
	{
		nCurLevel = m_SkillList.GetCurrentLevel(nSkillID);
		if (nCurLevel <= 0) 
        {
            nSkillID = 0;
        }
		else
		{
			_ASSERT(nSkillID < MAX_SKILL && nCurLevel < MAX_SKILLLEVEL);
			
			KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nSkillID, nCurLevel);
            if (!pOrdinSkill || !pOrdinSkill->IsAura())
			{
				nSkillID  = 0;
			}
		}
	}
	m_ActiveAuraID = nSkillID;

#ifdef _SERVER
	if (m_ActiveAuraID)
	{
		int nStateSpecialId = 0;
		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillID, nCurLevel);
		
		if (pOrdinSkill)
        {
			nStateSpecialId = pOrdinSkill->GetStateSpecailId();
        }
		
		if (nStateSpecialId) m_btStateInfo[m_nNextStatePos] = nStateSpecialId;
		if ((++m_nNextStatePos) >= MAX_NPC_RECORDER_STATE) m_nNextStatePos = 0;
		BroadCastState();
	}
	else
	{
		UpdateNpcStateInfo();
		BroadCastState();
	}
#endif

#ifndef _SERVER
	SKILL_CHANGEAURASKILL_COMMAND ChangeAuraMsg;
	ChangeAuraMsg.ProtocolType = c2s_changeauraskill;
	ChangeAuraMsg.m_nAuraSkill = m_ActiveAuraID;
	if (g_pClient)
		g_pClient->SendPackToServer(&ChangeAuraMsg, sizeof(SKILL_CHANGEAURASKILL_COMMAND));
#endif

}

BOOL KNpc::SetPlayerIdx(int nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_PLAYER)
		return FALSE;

	if (m_Kind != kind_player)
		return FALSE;

	m_nPlayerIdx = nIdx;
	return TRUE;
}




void KNpc::OnMadnessState()
{

if (m_TypeNpc == 3)
return;


if (m_MadnessState.nTime > 0 && m_MadnessState.nValue[0] <= 0)
{
GetMpsPos(&m_PrevX, &m_PrevY);
m_MadnessState.nValue[0] = 1;
}


if (m_MadnessState.nTime > 0)
{

m_ProcessAI = 0;

#ifdef _SERVER

int	nOffX, nOffY;
	
		nOffX = g_Random(150+m_Index%150);
		nOffY = g_Random(150+m_Index%150);
		if (nOffX & 1)
		{
			nOffX = - nOffX;
		}
		if (nOffY & 1)
		{
			nOffY = - nOffY;
		}

if (m_Doing != do_hurt && m_Doing != do_death && m_Doing != do_revive)
Madnessto(m_PrevX + nOffX, m_PrevY + nOffY);

#endif


}
else if (m_MadnessState.nValue[0] > 0)
{
m_MadnessState.nValue[0] = 0;
if (m_Doing != do_hurt && m_Doing != do_death && m_Doing != do_revive)
{
m_ProcessAI = 1;
DoStand();
}
}



}





#ifdef _SERVER
BOOL KNpc::SendSyncData(int nClient)
{
	BOOL	bRet = FALSE;
	NPC_SYNC	NpcSync;

	NpcSync.ProtocolType		= (BYTE)s2c_syncnpc;
	NpcSync.m_btKind			= (BYTE)m_Kind;
	NpcSync.Camp				= (BYTE)m_Camp;
	NpcSync.CurrentCamp			= (BYTE)m_CurrentCamp;
	NpcSync.m_bySeries			= (BYTE)m_Series;
	NpcSync.m_Doing				= (BYTE)m_Doing;

	if (m_CurrentLife > 0 && m_CurrentLifeMax >= m_CurrentLife)
	{
		NpcSync.CurrentLife		= m_CurrentLife;
		NpcSync.CurrentLifeMax  = m_CurrentLifeMax;
	}
	else
	{
		NpcSync.CurrentLife		= 0;
		NpcSync.CurrentLifeMax  = 0;
	}

	if (this->IsPlayer())
		NpcSync.m_btMenuState	= (BYTE)Player[this->m_nPlayerIdx].m_cMenuState.m_nState;
	else
		NpcSync.m_btMenuState	= 0;
	GetMpsPos((int *)&NpcSync.MapX, (int *)&NpcSync.MapY);
	NpcSync.ID					= m_dwID;
	NpcSync.NpcSettingIdx		= MAKELONG(m_Level, m_NpcSettingIdx);

    NpcSync.m_IsBoss            = (BYTE)m_IsBoss;

	strcpy(NpcSync.m_szName, Name);
	NpcSync.m_wLength			= sizeof(NPC_SYNC) - 1 - sizeof(NpcSync.m_szName) + strlen(NpcSync.m_szName);

	if (SUCCEEDED(g_pServer->PackDataToClient(nClient, (BYTE*)&NpcSync, NpcSync.m_wLength + 1)))
	{
		//printf("Packing sync data ok...\n");
		bRet = TRUE;
	}
	else
	{
		printf("Packing sync data failed...\n");
		return FALSE;
	}
	g_DebugLog("[Sync]%d:%s<%d> request to %d. size:%d", SubWorld[m_SubWorldIndex].m_dwCurrentTime, Name, m_Kind, nClient, NpcSync.m_wLength + 1);



	BroadCastState();

	if (IsPlayer())
	{
		PLAYER_SYNC	PlayerSync;

		PlayerSync.ProtocolType		= (BYTE)s2c_syncplayer;
		PlayerSync.ID				= m_dwID;
		PlayerSync.ArmorType		= (BYTE)m_ArmorType;
		PlayerSync.AttackSpeed		= (BYTE)m_CurrentAttackSpeed;
		PlayerSync.CastSpeed		= (BYTE)m_CurrentCastSpeed;
		
		PlayerSync.HelmType			= (BYTE)m_HelmType;

		if(m_bRideHorse)
		{
		PlayerSync.HorseType		= (BYTE)m_HorseType;
		}
		else
		{
		PlayerSync.HorseType        = -1;
		}

		PlayerSync.RunSpeed			= (BYTE)m_CurrentRunSpeed;
		PlayerSync.WalkSpeed		= (BYTE)m_CurrentWalkSpeed;
		PlayerSync.WeaponType		= (BYTE)m_WeaponType;
		PlayerSync.RankID			= (BYTE)m_btRankId;
		PlayerSync.ImagePlayer		= (BYTE)Player[m_nPlayerIdx].m_ImagePlayer; // moi them
		PlayerSync.MaskType			= m_MaskType; // mask
		PlayerSync.m_btSomeFlag		= 0;
		
		// fifong
		if(m_bFifongState)
		{
			PlayerSync.FifongType		= m_FifongType;
		}
		else
		{
			PlayerSync.FifongType        = 0;
		}
		//end

		Player[m_nPlayerIdx].m_cTong.GetTongName(PlayerSync.m_TongName);

		Player[m_nPlayerIdx].m_cTong.GetTongRank(PlayerSync.m_TongRank);
		
		if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 1)
			PlayerSync.m_btSomeFlag |= 0x01;
		else if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 2)
			PlayerSync.m_btSomeFlag |= 0x10;

		if (Npc[Player[m_nPlayerIdx].m_nIndex].m_FightMode)
			PlayerSync.m_btSomeFlag |= 0x02;
		if (Player[m_nPlayerIdx].m_bSleepMode)
			PlayerSync.m_btSomeFlag |= 0x04;

		PlayerSync.m_Task139 = Player[m_nPlayerIdx].m_cTask.GetSaveVal(139);
		PlayerSync.m_Task485 = Player[m_nPlayerIdx].m_cTask.GetSaveVal(485);



		
	    PlayerSync.m_dwTime = Player[m_nPlayerIdx].m_dwTimeOpenShop;
	    memcpy(PlayerSync.szName, Player[m_nPlayerIdx].m_szNameOpenShop, sizeof(PlayerSync.szName));




		if (SUCCEEDED(g_pServer->PackDataToClient(nClient, (BYTE*)&PlayerSync, sizeof(PLAYER_SYNC))))
		{
			//printf("Packing player sync data ok...\n");
			bRet = TRUE;
		}
		else
		{
			printf("Packing player sync data failed...\n");
			return FALSE;
		}
	}
	return bRet;
}



void KNpc::BroadCastTong()
{
	if (!IsPlayer())
		return;

	if (m_RegionIndex < 0)
		return;

	NPC_UPDATE_TONG	NpcUpdateTongSync;
	NpcUpdateTongSync.ProtocolType = s2c_playerupdatetong;
	NpcUpdateTongSync.ID = m_dwID;
	Player[m_nPlayerIdx].m_cTong.GetTongName(NpcUpdateTongSync.m_TongName);
	Player[m_nPlayerIdx].m_cTong.GetTongRank(NpcUpdateTongSync.m_TongRank);

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast((BYTE*)&NpcUpdateTongSync, sizeof(NPC_UPDATE_TONG), nMaxCount, m_MapX, m_MapY);
	int j;
	for (j = 0; j < 8; j++)
	{
		int nConRegion = CURREGION.m_nConnectRegion[j];
		if (nConRegion == -1)
			continue;
		SubWorld[m_SubWorldIndex].m_Region[nConRegion].BroadCast((BYTE*)&NpcUpdateTongSync, sizeof(NPC_UPDATE_TONG), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
	}
}






void KNpc::BroadCastShop()
{
	if (!IsPlayer())
		return;

	if (m_RegionIndex < 0)
		return;



	NPC_UPDATE_SHOP	NpcUpdateShopSync;
	NpcUpdateShopSync.ProtocolType = s2c_playerupdateshop;
	NpcUpdateShopSync.ID = m_dwID;
	NpcUpdateShopSync.m_dwTime = Player[m_nPlayerIdx].m_dwTimeOpenShop;
	memcpy(NpcUpdateShopSync.szName, Player[m_nPlayerIdx].m_szNameOpenShop, sizeof(NpcUpdateShopSync.szName));




	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast((BYTE*)&NpcUpdateShopSync, sizeof(NPC_UPDATE_SHOP), nMaxCount, m_MapX, m_MapY);
	int j;
	for (j = 0; j < 8; j++)
	{
		int nConRegion = CURREGION.m_nConnectRegion[j];
		if (nConRegion == -1)
			continue;
		SubWorld[m_SubWorldIndex].m_Region[nConRegion].BroadCast((BYTE*)&NpcUpdateShopSync, sizeof(NPC_UPDATE_SHOP), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
	}
}





// 平时数据的同步
void KNpc::NormalSync()
{
	if (m_Doing == do_revive || m_Doing == do_death || !m_Index || m_RegionIndex < 0)
		return;

	NPC_NORMAL_SYNC NpcSync;
	int	nMpsX, nMpsY;

	GetMpsPos(&nMpsX, &nMpsY);

	NpcSync.ProtocolType = (BYTE)s2c_syncnpcmin;
	NpcSync.ID = m_dwID;
	if (m_CurrentLife > 0 && m_CurrentLifeMax >= m_CurrentLife)
	{
		NpcSync.CurrentLife = m_CurrentLife;
		NpcSync.CurrentLifeMax = m_CurrentLifeMax;
	}
	else
	{
		NpcSync.CurrentLife = 0;
		NpcSync.CurrentLifeMax = 0;
	}

	NpcSync.MapX = nMpsX;
	NpcSync.MapY = nMpsY;

	NpcSync.Camp = (BYTE)m_CurrentCamp;
	NpcSync.State = 0;
	if (m_FreezeState.nTime > 0)
		NpcSync.State |= STATE_FREEZE;
	if (m_PoisonState.nTime > 0)
		NpcSync.State |= STATE_POISON;
	if (m_ConfuseState.nTime > 0)
		NpcSync.State |= STATE_CONFUSE;
	if (m_StunState.nTime > 0)
		NpcSync.State |= STATE_STUN;
    if (m_InvisibilityState.nTime > 0)
		NpcSync.State |= STATE_INVISIBILITY;
	if (m_NstatusState.nTime > 0)
		NpcSync.State |= STATE_NSTATUS;
	if (m_MadnessState.nTime > 0)
		NpcSync.State |= STATE_MADNESS;

	NpcSync.Doing = (BYTE)m_Doing;

	// 由于同步数据每次同步的很少，所以上限就是MAX_PLAYER
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_PLAYER;//MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NpcSync, sizeof(NPC_NORMAL_SYNC), nMaxCount, m_MapX, m_MapY);
	int j;
	for (j = 0; j < 8; j++)
	{
		int nConRegion = CURREGION.m_nConnectRegion[j];
		if (nConRegion == -1)
			continue;
		_ASSERT(m_SubWorldIndex >= 0 && nConRegion >= 0);
		SubWorld[m_SubWorldIndex].m_Region[nConRegion].BroadCast((BYTE*)&NpcSync, sizeof(NPC_NORMAL_SYNC), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
	}

	if (IsPlayer())
	{
		PLAYER_NORMAL_SYNC PlayerSync;

		PlayerSync.ProtocolType	= (BYTE)s2c_syncplayermin;
		PlayerSync.ID			= m_dwID;
		PlayerSync.AttackSpeed	= (BYTE)m_CurrentAttackSpeed;
		PlayerSync.CastSpeed	= (BYTE)m_CurrentCastSpeed;
		if (PlayerSync.CastSpeed < 0)
		PlayerSync.CastSpeed = 0;
		
		PlayerSync.RunSpeed		= (BYTE)m_CurrentRunSpeed;
		PlayerSync.WalkSpeed	= (BYTE)m_CurrentWalkSpeed;
		PlayerSync.HelmType		= (BYTE)m_HelmType;
		PlayerSync.ArmorType	= (BYTE)m_ArmorType;
		PlayerSync.WeaponType	= (BYTE)m_WeaponType;

		PlayerSync.m_PKValue    = (BYTE)Player[m_nPlayerIdx].m_cPK.GetPKValue();

		if(m_bRideHorse)
		{
		PlayerSync.HorseType		= (BYTE)m_HorseType;
		}
		else
		{
		PlayerSync.HorseType        = -1;
		}

		PlayerSync.RankID		= (BYTE)m_btRankId;
		PlayerSync.ImagePlayer		= (BYTE)Player[m_nPlayerIdx].m_ImagePlayer; // moi them
		PlayerSync.MaskType			= m_MaskType; // mask
		
		// fifong
		if(m_bFifongState)
		{
			PlayerSync.FifongType		= m_FifongType;
		}
		else
		{
			PlayerSync.FifongType        = 0;
		}
		//end
		
		PlayerSync.m_Task139    = (BYTE)Player[m_nPlayerIdx].m_cTask.GetSaveVal(139);
		PlayerSync.m_Task485    = (BYTE)Player[m_nPlayerIdx].m_cTask.GetSaveVal(485);

		PlayerSync.m_btSomeFlag	= 0;
		if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 1)
			PlayerSync.m_btSomeFlag |= 0x01;
		else if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 2)
			PlayerSync.m_btSomeFlag |= 0x10;

		if (m_FightMode)
			PlayerSync.m_btSomeFlag |= 0x02;
		if (Player[m_nPlayerIdx].m_bSleepMode)
			PlayerSync.m_btSomeFlag |= 0x04;
		if (Player[m_nPlayerIdx].m_cTong.GetOpenFlag())
			PlayerSync.m_btSomeFlag |= 0x08;

		int nMaxCount = MAX_PLAYER;//MAX_BROADCAST_COUNT;
		CURREGION.BroadCast(&PlayerSync, sizeof(PLAYER_NORMAL_SYNC), nMaxCount, m_MapX, m_MapY);
		for (j = 0; j < 8; j++)
		{
			int nConRegion = CURREGION.m_nConnectRegion[j];
			if (nConRegion == -1)
				continue;
			SubWorld[m_SubWorldIndex].m_Region[nConRegion].BroadCast((BYTE*)&PlayerSync, sizeof(PLAYER_NORMAL_SYNC), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
		}

		NPC_PLAYER_TYPE_NORMAL_SYNC	sSync;
		sSync.ProtocolType = s2c_syncnpcminplayer;
		sSync.m_dwNpcID = m_dwID;
//		sSync.m_nRegionID = SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].m_RegionID;
		sSync.m_btCamp = (BYTE)m_CurrentCamp;
		sSync.m_dwMapX = nMpsX;
		sSync.m_dwMapY = nMpsY;
		sSync.m_wOffX = m_OffX;
		sSync.m_wOffY = m_OffY;
		sSync.m_byDoing = this->m_Doing;
		g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(sSync));
	}
}

void KNpc::BroadCastRevive(int nType)
{
	if (!IsPlayer())
		return;

	if (m_RegionIndex < 0)
		return;

	NPC_REVIVE_SYNC	NpcReviveSync;
	NpcReviveSync.ProtocolType = s2c_playerrevive;
	NpcReviveSync.ID = m_dwID;
	NpcReviveSync.Type = (BYTE)nType;

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast((BYTE*)&NpcReviveSync, sizeof(NPC_REVIVE_SYNC), nMaxCount, m_MapX, m_MapY);
	int j;
	for (j = 0; j < 8; j++)
	{
		int nConRegion = CURREGION.m_nConnectRegion[j];
		if (nConRegion == -1)
			continue;
		SubWorld[m_SubWorldIndex].m_Region[nConRegion].BroadCast((BYTE*)&NpcReviveSync, sizeof(NPC_REVIVE_SYNC), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
	}
}

int	KNpc::GetPlayerIdx()
{
	if (m_Kind != kind_player)
		return 0;
	return m_nPlayerIdx;
}

#endif

#ifndef _SERVER
#include "scene/KScenePlaceC.h"

int KNpc::PaintInfo(int nHeightOffset, bool bSelect, int nFontSize, DWORD dwBorderColor)
{
	int nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);
	DWORD	dwColor;
	int nHeightOff = nHeightOffset + nFontSize + 1;

	int Map = SubWorld[Npc[m_Index].m_SubWorldIndex].m_SubWorldID;	
	if  (Map == 209 && m_CurrentCamp == 0 && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	return nHeightOffset;

	if (m_Kind == kind_player)
	{


if (m_btRankId)
{
char szRankId[5];
char szRankName[32];
itoa(m_btRankId, szRankId, 10);
g_RankTabSetting.GetString(szRankId, "RANKSTR", "",szRankName, 32);


if (m_TongName[0] && m_TongRank[0])
{

	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)
	{

	g_pRepresent->OutputText(nFontSize, szRankName, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(szRankName) / 4, nMpsY, 0x00e400c6, 0, nHeightOff + nFontSize + 4 + nFontSize + 4, dwBorderColor);
	

	}
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	{
	g_pRepresent->OutputText(nFontSize, szRankName, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(szRankName) / 4, nMpsY, 0x00e400c6, 0, nHeightOff + nFontSize + 4 + nFontSize + 4, dwBorderColor);
	}
	}



}
else
{



	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
	g_pRepresent->OutputText(nFontSize, szRankName, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(szRankName) / 4, nMpsY, 0x00e400c6, 0, nHeightOff + nFontSize + 4, dwBorderColor);
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->OutputText(nFontSize, szRankName, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(szRankName) / 4, nMpsY, 0x00e400c6, 0, nHeightOff + nFontSize + 4, dwBorderColor);
	}

}


}










		switch(m_CurrentCamp)
		{
		case camp_begin:
			dwColor = 0xffffffff;
			break;
		case camp_justice:
			dwColor = 0xff000000 | (255 << 16) | (168 << 8) | 94;
			break;
		case camp_evil:
			dwColor = 0xff000000 | (255 << 16) | (146 << 8) | 255;
			break;
		case camp_balance:
			dwColor = 0xff000000 | (85 << 16) | (255 << 8) | 145;
			break;
		case camp_free:
			dwColor = 0xff000000 | (255 << 16);
			break;
		default:
			dwColor = 0xffff00ff;
			break;
/*		case camp_animal:
			dwColor = 
			break;
		case camp_event:
			break;*/
		}
		




if (m_TongName[0] && m_TongRank[0])
{
	char nTongRaw[64];

	
	if (SubWorld[Npc[CLIENT_PLAYER_INDEX].m_SubWorldIndex].m_SubWorldID == 258)
	{
		sprintf(nTongRaw,"");
	}
	else
	{
		sprintf(nTongRaw,"%s %s",m_TongName,m_TongRank);
	}
	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
	g_pRepresent->OutputText(nFontSize, nTongRaw, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(nTongRaw) / 4, nMpsY, dwColor, 0, nHeightOff + nFontSize + 4, dwBorderColor);
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->OutputText(nFontSize, nTongRaw, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(nTongRaw) / 4, nMpsY, dwColor, 0, nHeightOff + nFontSize + 4, dwBorderColor);
	}

}





		
		char	szString[128];
		if (SubWorld[Npc[CLIENT_PLAYER_INDEX].m_SubWorldIndex].m_SubWorldID == 258 && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
		{
			strcpy(szString, "Nh﹏ S?V?L﹎");
		}
		else
		{
			strcpy(szString, Name);
		}
		if (m_FreezeState.nTime || m_PoisonState.nTime || m_ConfuseState.nTime || m_StunState.nTime)
		{
			strcat(szString, "(");
			if (m_FreezeState.nTime)
				strcat(szString, "B╪g");
			if (m_PoisonState.nTime)
				strcat(szString, "чc");
			if (m_ConfuseState.nTime)
				strcat(szString, "xxxxxxxxxxxxx");
			if (m_StunState.nTime)
				strcat(szString, "Cho竛g");
			strcat(szString, ")");
		}
		// mask
		
		
		if (m_MaskType > 0)
		{
			KIniFile pIniMask;
			char Buff[128];
			
			if (pIniMask.Load("\\Ui\\npcbobo.ini") && m_InvisibilityState.nTime <= 0)
			{
				if (m_bRideHorse)
				{
					pIniMask.GetString("Actions", "0", "", Buff, sizeof(Buff));
					KRUImage RUIconImage;
					RUIconImage.nType = ISI_T_SPR;
					RUIconImage.Color.Color_b.a = 255;
					RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
					RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
					RUIconImage.uImage = 0;
					RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
					strcpy(RUIconImage.szImage, Buff);
					RUIconImage.oPosition.nX = nMpsX + (nFontSize * g_StrLen(Name) / 4) + 5;
					RUIconImage.oPosition.nY = nMpsY-nHeightOff*1.48;
					RUIconImage.oPosition.nZ = 0;
					RUIconImage.nFrame = 0;
					g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, FALSE);
				}
				else if (m_Doing == do_sit)
				{
					pIniMask.GetString("Actions", "1", "", Buff, sizeof(Buff));
					KRUImage RUIconImage;
					RUIconImage.nType = ISI_T_SPR;
					RUIconImage.Color.Color_b.a = 255;
					RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
					RUIconImage.uImage = 0;
					RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
					RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
					strcpy(RUIconImage.szImage, Buff);
					RUIconImage.oPosition.nX = nMpsX + (nFontSize * g_StrLen(Name) / 4) + 5;
					RUIconImage.oPosition.nY = nMpsY-nHeightOff*1.48;
					RUIconImage.oPosition.nZ = 0;
					RUIconImage.nFrame = 0;
					g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, FALSE);
				}
			}
		}
		// end
	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
	g_pRepresent->OutputText(nFontSize, szString, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(Name) / 4, nMpsY, dwColor, 0, nHeightOff, dwBorderColor);
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->OutputText(nFontSize, szString, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(Name) / 4, nMpsY, dwColor, 0, nHeightOff, dwBorderColor);
	}




   	nHeightOffset += nFontSize + 1;


if (m_Task139)
{
	KRUImage RUIconImage;
	RUIconImage.nType = ISI_T_SPR;
	RUIconImage.Color.Color_b.a = 255;
	RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	RUIconImage.uImage = 0;
	RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;

if (m_Task139 == 1)
strcpy(RUIconImage.szImage,SPRHIEGHTLEVEL1);
else if (m_Task139 == 2)
strcpy(RUIconImage.szImage,SPRHIEGHTLEVEL2);
else if (m_Task139 == 3)
strcpy(RUIconImage.szImage,SPRHIEGHTLEVEL3);
else if (m_Task139 == 4)
strcpy(RUIconImage.szImage,SPRHIEGHTLEVEL4);
else if (m_Task139 == 5)
strcpy(RUIconImage.szImage,SPRHIEGHTLEVEL5);
else
strcpy(RUIconImage.szImage,"");

	RUIconImage.oPosition.nX = nMpsX - nFontSize * g_StrLen(Name) / 4 - 15;
	RUIconImage.oPosition.nY = nMpsY -8;
	RUIconImage.oPosition.nZ = nHeightOff - 12;
	RUIconImage.nFrame = (SubWorld[0].m_dwCurrentTime*10/18)%10;

	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
    g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 0);
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 0);
	}

}

if (m_Task485 && SubWorld[Npc[CLIENT_PLAYER_INDEX].m_SubWorldIndex].m_SubWorldID != 59)
{
	KRUImage RUIconImage;
	RUIconImage.nType = ISI_T_SPR;
	RUIconImage.Color.Color_b.a = 255;
	RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	RUIconImage.uImage = 0;
	RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;

if (m_Task485 == 1)
strcpy(RUIconImage.szImage,SPRVIP1);
else if (m_Task485 == 2)
strcpy(RUIconImage.szImage,SPRVIP2);
else if (m_Task485 == 3)
strcpy(RUIconImage.szImage,SPRVIP3);
else if (m_Task485 == 4)
strcpy(RUIconImage.szImage,SPRVIP4);
else if (m_Task485 == 5)
strcpy(RUIconImage.szImage,SPRVIP5);
else if (m_Task485 == 6)
strcpy(RUIconImage.szImage,SPRVIP6);
else if (m_Task485 == 7)
strcpy(RUIconImage.szImage,SPRVIP7);
else if (m_Task485 == 8)
strcpy(RUIconImage.szImage,SPRVIP8);
else if (m_Task485 == 9)
strcpy(RUIconImage.szImage,SPRVIP9);
else if (m_Task485 == 10)
strcpy(RUIconImage.szImage,SPRVIP10);
else
strcpy(RUIconImage.szImage,"");

if (m_Task139 == 1)
{
	RUIconImage.oPosition.nX = nMpsX - nFontSize * g_StrLen(Name) / 4 - 18;
	RUIconImage.oPosition.nY = nMpsY + 24;
	RUIconImage.oPosition.nZ = nHeightOff - 12;
	RUIconImage.nFrame = (SubWorld[0].m_dwCurrentTime*10/18)%10;
}
else
{
	RUIconImage.oPosition.nX = nMpsX - nFontSize * g_StrLen(Name) / 4 - 25;
	RUIconImage.oPosition.nY = nMpsY -5;
	RUIconImage.oPosition.nZ = nHeightOff - 12;
	RUIconImage.nFrame = (SubWorld[0].m_dwCurrentTime*10/18)%10;
}
	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
    g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 0);
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 0);
	}
	
}


if (m_dwTimeOpenShop && m_szNameOpenShop[0])
{

int nOffSetT = 0;


if (m_btRankId)
{
nOffSetT += nFontSize + 4;
}

if (m_TongName[0] && m_TongRank[0])
{
nOffSetT += nFontSize + 4;
}


int nCX2 = nMpsX + nFontSize * g_StrLen(m_szNameOpenShop) / 4  + 16;

int nCX1 = nMpsX - nFontSize * g_StrLen(m_szNameOpenShop) / 4  - 16;

for (int i = nCX1; i <= nCX2;i+=2)
{
	KRUImage RUIconImageC;
	RUIconImageC.nType = ISI_T_SPR;
	RUIconImageC.Color.Color_b.a = 255;
	RUIconImageC.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	RUIconImageC.uImage = 0;
	RUIconImageC.nISPosition = IMAGE_IS_POSITION_INIT;
	RUIconImageC.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
    strcpy(RUIconImageC.szImage,SPRSHOPCENTER);
	RUIconImageC.oPosition.nX = i;
	RUIconImageC.oPosition.nY = nMpsY;
	RUIconImageC.oPosition.nZ = nHeightOff + nOffSetT + 35;
	RUIconImageC.nFrame = 0;

	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
    g_pRepresent->DrawPrimitives(1, &RUIconImageC, RU_T_IMAGE, 0);
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->DrawPrimitives(1, &RUIconImageC, RU_T_IMAGE, 0);
	}

}



KRUImage RUIconImageL;
	RUIconImageL.nType = ISI_T_SPR;
	RUIconImageL.Color.Color_b.a = 255;
	RUIconImageL.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	RUIconImageL.uImage = 0;
	RUIconImageL.nISPosition = IMAGE_IS_POSITION_INIT;
	RUIconImageL.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
    strcpy(RUIconImageL.szImage,SPRSHOPLEFT);
	RUIconImageL.oPosition.nX = nMpsX - nFontSize * g_StrLen(m_szNameOpenShop) / 4 - 10 - 16;
	RUIconImageL.oPosition.nY = nMpsY;
	RUIconImageL.oPosition.nZ = nHeightOff + nOffSetT + 35;
	RUIconImageL.nFrame = 0;



	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
    g_pRepresent->DrawPrimitives(1, &RUIconImageL, RU_T_IMAGE, 0);
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->DrawPrimitives(1, &RUIconImageL, RU_T_IMAGE, 0);
	}







	KRUImage RUIconImageR;
	RUIconImageR.nType = ISI_T_SPR;
	RUIconImageR.Color.Color_b.a = 255;
	RUIconImageR.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	RUIconImageR.uImage = 0;
	RUIconImageR.nISPosition = IMAGE_IS_POSITION_INIT;
	RUIconImageR.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
    strcpy(RUIconImageR.szImage,SPRSHOPRIGHT);
	RUIconImageR.oPosition.nX = nMpsX + nFontSize * g_StrLen(m_szNameOpenShop) / 4 - 10 + 16 ;
	RUIconImageR.oPosition.nY = nMpsY;
	RUIconImageR.oPosition.nZ = nHeightOff + nOffSetT + 35;
	RUIconImageR.nFrame = 0;




	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
    g_pRepresent->DrawPrimitives(1, &RUIconImageR, RU_T_IMAGE, 0);
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->DrawPrimitives(1, &RUIconImageR, RU_T_IMAGE, 0);
	}



g_pRepresent->OutputText(nFontSize, m_szNameOpenShop, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(m_szNameOpenShop) / 4, nMpsY, 0xfff6ff75, 0, nHeightOff+ nOffSetT + 35 - 4, dwBorderColor);
	




}








	}
	else if (m_Kind == kind_dialoger)
	{
		dwColor = 0xffffffff;


	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
    g_pRepresent->OutputText(nFontSize, Name, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(Name) / 4, nMpsY, dwColor, 0, nHeightOff, dwBorderColor);
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->OutputText(nFontSize, Name, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(Name) / 4, nMpsY, dwColor, 0, nHeightOff, dwBorderColor);	
	}

	nHeightOffset += nFontSize + 1;

	}
	else if (m_Kind == 0)
	{
		if (this->m_TypeNpc == 1 || this->m_TypeNpc == 5  || this->m_TypeNpc == 6)
			dwColor = 0xffebb200;
		else if (this->m_TypeNpc == 2)
			dwColor = 0x0000AE29;
		else if (this->m_IsBoss == 1)
			dwColor = 0x00000EEF;
		else if (this->m_TypeNpc == 4)
			dwColor = 0x00000EEF;
		else
            dwColor = 0xffffffff;


	if (m_TypeNpc != 3)
	{


	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
    g_pRepresent->OutputText(nFontSize, Name, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(Name) / 4, nMpsY, dwColor, 0, nHeightOff, dwBorderColor);
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->OutputText(nFontSize, Name, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(Name) / 4, nMpsY, dwColor, 0, nHeightOff, dwBorderColor);
	}


	nHeightOffset += nFontSize + 1;



	KRUImage RUIconImage;
	RUIconImage.nType = ISI_T_SPR;
	RUIconImage.Color.Color_b.a = 255;
	RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	RUIconImage.uImage = 0;
	RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	RUIconImage.bRenderFlag = 0;

if (m_Series == 0)
strcpy(RUIconImage.szImage,SPRSERIICON0);
else if (m_Series == 1)
strcpy(RUIconImage.szImage,SPRSERIICON1);
else if (m_Series == 2)
strcpy(RUIconImage.szImage,SPRSERIICON2);
else if (m_Series == 3)
strcpy(RUIconImage.szImage,SPRSERIICON3);
else if (m_Series == 4)
strcpy(RUIconImage.szImage,SPRSERIICON4);
else
strcpy(RUIconImage.szImage,"");

	RUIconImage.oPosition.nX = nMpsX + nFontSize * g_StrLen(Name) / 4 + 5;
	RUIconImage.oPosition.nY = nMpsY;
	RUIconImage.oPosition.nZ = nHeightOff;
	RUIconImage.nFrame = 0;



	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
    g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 0);
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 0);
	}




	}








	}








#ifdef SWORDONLINE_SHOW_DBUG_INFO
	if (Player[CLIENT_PLAYER_INDEX].m_DebugMode)
	{
		char szNameID[50];
		sprintf(szNameID,"[%d]", m_dwID);
		g_pRepresent->OutputText(12, szNameID, KRF_ZERO_END, nMpsX, nMpsY + 20, 0xfff0fff0, 0, m_Height);
	}
	
	if (Player[CLIENT_PLAYER_INDEX].m_nIndex == m_Index && Player[CLIENT_PLAYER_INDEX].m_DebugMode)
	{
		char	szMsg[256];
		int nCount[9];
		for (int i = 0; i < 9; i++)
			nCount[i] = 0;
		if (LEFTUPREGIONIDX >= 0)
			nCount[0] = LEFTUPREGION.m_NpcList.GetNodeCount();
		if (UPREGIONIDX >= 0)
			nCount[1] = UPREGION.m_NpcList.GetNodeCount();
		if (RIGHTUPREGIONIDX >= 0)
			nCount[2] = RIGHTUPREGION.m_NpcList.GetNodeCount();
		if (LEFTREGIONIDX >= 0)
			nCount[3] = LEFTREGION.m_NpcList.GetNodeCount();
		if (m_RegionIndex >= 0)
			nCount[4] = CURREGION.m_NpcList.GetNodeCount();
		if (RIGHTREGIONIDX >= 0)
			nCount[5] = RIGHTREGION.m_NpcList.GetNodeCount();
		if (LEFTDOWNREGIONIDX >= 0)
			nCount[6] = LEFTDOWNREGION.m_NpcList.GetNodeCount();
		if (DOWNREGIONIDX >= 0)
			nCount[7] = DOWNREGION.m_NpcList.GetNodeCount();
		if (RIGHTDOWNREGIONIDX >= 0)
			nCount[8] = RIGHTDOWNREGION.m_NpcList.GetNodeCount();
		
		int nPosX, nPosY;
		GetMpsPos(&nPosX, &nPosY);
		sprintf(szMsg,
			"NpcID:%d  Life:%d\nRegionIndex:%d Pos:%d,%d\nPlayerNumber:%d\n"
			"NpcNumber:\n%02d,%02d,%02d\n%02d,%02d,%02d\n%02d,%02d,%02d",
			m_dwID,
			m_CurrentLife,			
			m_RegionIndex,
			m_MapX,
			m_MapY,
			CURREGION.m_PlayerList.GetNodeCount(),
			nCount[0], nCount[1], nCount[2],
			nCount[3], nCount[4], nCount[5],
			nCount[6], nCount[7], nCount[8]			
			);
		
		g_pRepresent->OutputText(12, szMsg, -1, 320, 40, 0xffffffff);

	}
#endif

	return nHeightOffset;
}

int	KNpc::PaintChat(int nHeightOffset)
{
	if (m_Kind != kind_player)
		return nHeightOffset;
	if (m_nChatContentLen <= 0)
		return nHeightOffset;
	if (m_nChatNumLine <= 0)
		return nHeightOffset;

	int Map = SubWorld[Npc[m_Index].m_SubWorldIndex].m_SubWorldID;	
	if  (Map == 209 && m_CurrentCamp == 0)
	return nHeightOffset;	
		
	int nFontSize = 12;
	int					nWidth, nHeight;
	int					nMpsX, nMpsY;
	KRUShadow			sShadow;
	KOutputTextParam	sParam;
	sParam.BorderColor = 0;

	sParam.nNumLine = m_nChatNumLine;

	nWidth = m_nChatFontWidth * nFontSize / 2;
	nHeight = sParam.nNumLine * (nFontSize + 1);

	nWidth += 6;	//为了好看
	nHeight += 5;	//为了好看


	GetMpsPos(&nMpsX, &nMpsY);
	sParam.nX = nMpsX - nWidth / 2;
	sParam.nY = nMpsY;
	sParam.nZ = nHeightOffset + nHeight;
	sParam.Color = SHOW_CHAT_COLOR;
	sParam.nSkipLine = 0;
	sParam.nVertAlign = 0;

	sShadow.oPosition.nX = sParam.nX;
	sShadow.oPosition.nX -= 3;	//为了好看
	sShadow.oPosition.nY = sParam.nY;
	sShadow.oPosition.nZ = sParam.nZ;
	sShadow.oEndPos.nX = sParam.nX + nWidth;
	sShadow.oEndPos.nX += 2;	//为了好看
	sShadow.oEndPos.nY = sParam.nY;
	sShadow.oEndPos.nZ = sParam.nZ - nHeight;
	//sShadow.Color.Color_dw = 0x00FFFF00;
	//g_pRepresent->DrawPrimitives(1, &sShadow, RU_T_RECT, false);

	sShadow.Color.Color_dw = 0x14000000;
	//g_pRepresent->DrawPrimitives(1, &sShadow, RU_T_SHADOW, false);
	
	sParam.bPicPackInSingleLine = true;

	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
	g_pRepresent->OutputRichText(nFontSize, &sParam, m_szChatBuffer, m_nChatContentLen, nWidth);
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->OutputRichText(nFontSize, &sParam, m_szChatBuffer, m_nChatContentLen, nWidth);
	}





	return sParam.nZ;
}

#include "../../Engine/Src/Text.h"
int	KNpc::SetChatInfo(const char* Name, const char* pMsgBuff, unsigned short nMsgLength)
{
	int nFontSize = 12;

	char szChatBuffer[MAX_SENTENCE_LENGTH];

	memset(szChatBuffer, 0, sizeof(szChatBuffer));

	if (nMsgLength)
	{
		int nOffset = 0;
		if (pMsgBuff[0] != KTC_TAB)
		{
			szChatBuffer[nOffset] = (char)KTC_COLOR;
			nOffset++;
			szChatBuffer[nOffset] = (char)0xFF;
			nOffset++;
			szChatBuffer[nOffset] = (char)0xFF;
			nOffset++;
			szChatBuffer[nOffset] = (char)0x00;
			nOffset++;
			strncpy(szChatBuffer + nOffset, Name, 32);
			nOffset += strlen(Name);
			szChatBuffer[nOffset] = ':';
			nOffset++;
			szChatBuffer[nOffset] = (char)KTC_COLOR_RESTORE;
			nOffset++;
		}
		else
		{
			pMsgBuff ++;
			nMsgLength --;
		}

		if (nMsgLength)
		{
			// begin
			char *pszCheck1 = (char*)pMsgBuff;
			char *pszCheck2 = NULL;
			int nCountSymbol = 0;
			int i = 0;
			BOOL bOk = FALSE;
			
			while(i < nMsgLength)
			{
				if(*pszCheck1 == '[')
				{
					bOk = TRUE;
					break;
				}
				pszCheck1++;
				i++;
			}
			
			if (bOk)
			{
				while(i < nMsgLength)
				{
					if(*pszCheck1 == ']' && nCountSymbol == 29)
					{
						pMsgBuff = "";
						nMsgLength = 1;
						break;
					}
					if(*pszCheck1 == ',')
						nCountSymbol++;
					pszCheck1++; 
					i++;
				}
			}
			// end
			memcpy(szChatBuffer + nOffset, pMsgBuff, nMsgLength);
			nOffset += nMsgLength;

			memset(m_szChatBuffer, 0, sizeof(m_szChatBuffer));
			m_nChatContentLen = MAX_SENTENCE_LENGTH;
			TGetLimitLenEncodedString(szChatBuffer, nOffset, nFontSize, SHOW_CHAT_WIDTH,
				m_szChatBuffer, m_nChatContentLen, 2, true);

			m_nChatNumLine = TGetEncodedTextLineCount(m_szChatBuffer, m_nChatContentLen, SHOW_CHAT_WIDTH, m_nChatFontWidth, nFontSize, 0, 0, true);
			if (m_nChatNumLine >= 2)
				m_nChatNumLine = 2;
			m_nCurChatTime = IR_GetCurrentTime();
			return true;
		}
	}
	return false;
}


int KNpc::PaintTopTongKim(int pk1, int pk2, int pk3, int pk4, int xh)
{
	char HSD[50] = "";
	
	sprintf(HSD,"X誴 H筺g: %d",xh);
	g_pRepresent->OutputText(12, HSD, -1, 20,  116, 0xFFFFAE12);
	sprintf(HSD,"T輈h L騳: %d",pk1);
	g_pRepresent->OutputText(12, HSD, -1, 20,  130, 0xFFFFAE13);
	sprintf(HSD,"Ti猽 Di謙: %d",pk2);
	g_pRepresent->OutputText(12, HSD, -1, 20,  144, 0xFFFFAE14);
	sprintf(HSD,"T?Vong: %d",pk3);
	g_pRepresent->OutputText(12, HSD, -1, 20,  158, 0xFFFFAE15);
	sprintf(HSD,"Li猲 Tr秏: %d",pk4);
	g_pRepresent->OutputText(12, HSD, -1, 20,  172, 0xFFFFAE17);

return 0;
}

int KNpc::PaintEffect()
{
	/*	char digit[30]; 
		int iSecond = 0;
		int iHour = 0;
		int iMinute = 0;
		int iDay = 0;
		char HSD[100];
		int nam = 0;
		int thang =0;
		int ngay = 0;
		int gio = 0;
		
		if (HanSuDung != 0)
		{
			nam = HanSuDung / 1000000;
			thang = (HanSuDung - nam * 1000000) / 10000;
			ngay = (HanSuDung - nam * 1000000 - thang * 10000) / 100;
			gio = HanSuDung - nam * 1000000 - thang * 10000 - ngay * 100;

			if (nam == 22)
			{
				sprintf(HSD,"H誸 h筺: v躰h vi詎");
				g_pRepresent->OutputText(12, HSD, -1, 10,  85, 0xFFFFAE16);
			}
			else
			{
				sprintf(HSD,"H誸 h筺: %dh00 %d/%d/20%d",gio,ngay,thang,nam);
				g_pRepresent->OutputText(12, HSD, -1, 10,  85, 0xFFFFAE16);
			}
		}
		else
		{
			sprintf(HSD,"H誸 h筺: <產ng c藀 nh藅>");
			g_pRepresent->OutputText(12, HSD, -1, 10,  85, 0xFFFFAE16);
		}*/
							
	/*	for (int i =0; i < 10; i++)
		{
			if (MangEffect[i][0] != 0)
			{
					KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(MangEffect[i][0], 1);
					if (!pOrdinSkill) break;

					
					KRUImage RUIconImage;
					RUIconImage.nType = ISI_T_SPR;
					RUIconImage.Color.Color_b.a = 255;
					RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
					RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
					RUIconImage.uImage = 0;
					RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
					strcpy(RUIconImage.szImage, pOrdinSkill->m_szSkillIcon);
					if (MangEffect[i][0] == 460 || MangEffect[i][0] == 455 || MangEffect[i][0] == 469 || MangEffect[i][0] == 496|| MangEffect[i][0] == 505)
					{
							RUIconImage.oPosition.nX = 20 + 40 * i + 5;
							RUIconImage.oPosition.nY = 30;
					}
					else
					{
							RUIconImage.oPosition.nX = 20 + 40 * i;
							RUIconImage.oPosition.nY = 25;
					}
					
					RUIconImage.oPosition.nZ = 0;
					RUIconImage.nFrame = 0;
					g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, TRUE);
					
					
					
					
					iDay = MangEffect[i][1] / 24 / 60 / 60 / 18;
					iHour = (MangEffect[i][1] - iDay * 24 * 60 * 60 * 18) / 60 / 60 / 18;
					iMinute = (MangEffect[i][1] - iDay * 24 * 60 * 60 * 18 - iHour * 60 * 60 * 18)  / 60 / 18;
					iSecond = (MangEffect[i][1] - iDay * 24 * 60 * 60 * 18 - iHour * 60 * 60 * 18 - iMinute * 60 * 18) / 18;
	
						if (iDay > 0)
						{
						sprintf(digit,"%dd%dh",iDay,iHour);
							g_pRepresent->OutputText(12, digit, -1, RUIconImage.oPosition.nX + 1,  65, 0x00FFFF00);
						}
						else if (iHour > 0)
						{
						sprintf(digit,"%dh%dm",iHour,iMinute);
							g_pRepresent->OutputText(12, digit, -1, RUIconImage.oPosition.nX + 1,  65, 0x00FFFF00);
						}
						else if (iMinute > 0)
						{
							sprintf(digit,"%dm%ds",iMinute,iSecond);
							g_pRepresent->OutputText(12, digit, -1, RUIconImage.oPosition.nX + 1,  65, 0x00FFFF00);
						}
						else if (iSecond > 10)
						{
							sprintf(digit,"%ds",iSecond);
							g_pRepresent->OutputText(12, digit, -1, RUIconImage.oPosition.nX + 10,  65, 0x00FFFF00);
						}
						else
						{
							sprintf(digit,"N/A");
							g_pRepresent->OutputText(12, digit, -1, RUIconImage.oPosition.nX + 10,  65, 0x00FFFF00);
						}
			}
		
		}	*/
		return 0;		
}

int	KNpc::PaintLife(int nHeightOffset, bool bSelect)
{
	if (!bSelect &&
		(m_Kind != kind_player  && m_Kind != kind_normal)
		)
		return nHeightOffset;
	
	int Map = SubWorld[Npc[m_Index].m_SubWorldIndex].m_SubWorldID;	
	if  (Map == 209 && m_CurrentCamp == 0 && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	return nHeightOffset;
	
	
	if (m_CurrentLifeMax <= 0)
		return nHeightOffset;

	if (relation_enemy == NpcSet.GetRelation(Player[CLIENT_PLAYER_INDEX].m_nIndex,m_Index) &&
		(m_Kind == kind_player ||
		 m_Kind == kind_partner)
		)
		return nHeightOffset;		//有敌对关系的玩家不显示生命

	int	nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);
	int nWid = SHOW_LIFE_WIDTH;
	int nHei = SHOW_LIFE_HEIGHT;


	if (m_TypeNpc == 3)
	{
	nWid = SHOW_LIFE_WIDTH * 3 / 2;
	nHei = SHOW_LIFE_HEIGHT * 3 / 2;
	}


	KRUShadow	Blood;
	int nX = m_CurrentLife * 100 / m_CurrentLifeMax;

	if (m_Kind == kind_player && m_PKValue == MAX_DEATH_PUNISH_PK_VALUE)
	{
	Blood.Color.Color_b.r = 230;
	Blood.Color.Color_b.g = 0;
	Blood.Color.Color_b.b = 0;
	}
    else if (m_Kind == kind_player && m_nPKFlag == 2 )
      // (m_Kind == kind_player && relation_enemy == NpcSet.GetRelation(Player[CLIENT_PLAYER_INDEX].m_nIndex,m_Index)
	{
        Blood.Color.Color_b.r = 238;
		Blood.Color.Color_b.g = 18;
		Blood.Color.Color_b.b = 137;
	}
	else

	{

	if (nX >= 50)
	{
		Blood.Color.Color_b.r = 0;
		Blood.Color.Color_b.g = 255;
		Blood.Color.Color_b.b = 0;
	}
	else if (nX >= 25)
	{
		Blood.Color.Color_b.r = 255;
		Blood.Color.Color_b.g = 255;
		Blood.Color.Color_b.b = 0;
	}
	else
	{
		Blood.Color.Color_b.r = 255;
		Blood.Color.Color_b.g = 0;
		Blood.Color.Color_b.b = 0;
	}

	}

	Blood.Color.Color_b.a = 0;
	Blood.oPosition.nX = nMpsX - nWid / 2;
	Blood.oPosition.nY = nMpsY;
	Blood.oPosition.nZ = nHeightOffset + nHei;
	Blood.oEndPos.nX = Blood.oPosition.nX + nWid * nX / 100;
	Blood.oEndPos.nY = nMpsY;
	Blood.oEndPos.nZ = nHeightOffset;

    if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, FALSE);
    else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, FALSE);
	}
	

	Blood.Color.Color_b.r = 128;
	Blood.Color.Color_b.g = 128;
	Blood.Color.Color_b.b = 128;
	Blood.oPosition.nX = Blood.oEndPos.nX;
	Blood.oEndPos.nX = nMpsX + nWid / 2;

	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, FALSE);
    else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, FALSE);
	}

	
	KRUShadow	Blood1;


	Blood1.Color.Color_b.r = 0;
	Blood1.Color.Color_b.g = 0;
	Blood1.Color.Color_b.b = 255;


	
	
	return nHeightOffset + nHei;
}

int	KNpc::PaintMana(int nHeightOffset)
{
	if (m_Kind != kind_player &&
		m_Kind != kind_partner)
		return nHeightOffset;

	if (m_CurrentManaMax <= 0)
		return nHeightOffset;

	return nHeightOffset;

	int	nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);
	int nWid = 38;
	int nHei = 5;
	KRUShadow	Blood;
	int nX = m_CurrentMana * 100 / m_CurrentManaMax;
	if (nX >= 50)
	{
		Blood.Color.Color_b.r = 0;
		Blood.Color.Color_b.g = 0;
		Blood.Color.Color_b.b = 255;
	}
	else if (nX >= 25)
	{
		Blood.Color.Color_b.r = 255;
		Blood.Color.Color_b.g = 255;
		Blood.Color.Color_b.b = 0;
	}
	else
	{
		Blood.Color.Color_b.r = 255;
		Blood.Color.Color_b.g = 0;
		Blood.Color.Color_b.b = 0;
	}
	Blood.Color.Color_b.a = 0;
	Blood.oPosition.nX = nMpsX - nWid / 2;
	Blood.oPosition.nY = nMpsY;
	Blood.oPosition.nZ = nHeightOffset + nHei;
	Blood.oEndPos.nX = Blood.oPosition.nX + nWid * nX / 100;
	Blood.oEndPos.nY = nMpsY;
	Blood.oEndPos.nZ = nHeightOffset;
	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, FALSE);

	Blood.Color.Color_b.r = 255;
	Blood.Color.Color_b.g = 255;
	Blood.Color.Color_b.b = 255;
	Blood.oPosition.nX = Blood.oEndPos.nX;
	Blood.oEndPos.nX = nMpsX + nWid / 2;
	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, FALSE);
	
	return nHeightOffset + nHei;
}

void KNpc::Paint()
{
	if (m_ResDir != m_Dir)
	{
		int nDirOff = m_Dir - m_ResDir;
		if (nDirOff > 32)
			nDirOff -= 64;
		else if (nDirOff < - 32)
			nDirOff += 64;
		m_ResDir += nDirOff / 2;
		if (m_ResDir >= 64)
			m_ResDir -= 64;
		if (m_ResDir < 0)
			m_ResDir += 64;
	}



if (m_Index == Player[CLIENT_PLAYER_INDEX].m_nIndex)

{
	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)
	{
    m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame,FALSE, TRUE, Player[CLIENT_PLAYER_INDEX].m_HideMissle2);
	int nHeight = GetNpcPate() + GetNpcPatePeopleInfo();
	DrawMenuState(nHeight);
	}
    else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	{
	m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame,FALSE, TRUE, Player[CLIENT_PLAYER_INDEX].m_HideMissle2);
	int nHeight = GetNpcPate() + GetNpcPatePeopleInfo();
	DrawMenuState(nHeight);
	}
	}



}

else

{

//	int// nX, nY;
//	Npc[m_nIndex].GetMpsPos(&nX, &nY);
	
	int distance = NpcSet.GetDistance(Player[CLIENT_PLAYER_INDEX].m_nIndex, m_Index);		

	if (Player[CLIENT_PLAYER_INDEX].m_HideRes && m_Kind == kind_player && Player[CLIENT_PLAYER_INDEX].m_eHidePlayer3 < distance)
	return;
	
	if (Player[CLIENT_PLAYER_INDEX].m_bHidePlayer1 && m_CurrentCamp == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp && m_Kind == kind_player)
	return;
		
	if (Player[CLIENT_PLAYER_INDEX].m_bHidePlayer2 && m_CurrentCamp != Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp && m_Kind == kind_player)
	return;
	
	int Map = SubWorld[Npc[m_Index].m_SubWorldIndex].m_SubWorldID;	
	if  (Map == 209 && m_CurrentCamp == 0)
	return;

	if ((m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX))
	{
		m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame,FALSE, FALSE, Player[CLIENT_PLAYER_INDEX].m_HideMissle2);
		//g_DebugLog("HideRes: %d - %d - %d \n",m_ResDir,m_Frames.nTotalFrame,m_Frames.nCurrentFrame);
		
		int nHeight = GetNpcPate() + GetNpcPatePeopleInfo();
		DrawMenuState(nHeight);
	}
    else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	{
	m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame,FALSE, FALSE, Player[CLIENT_PLAYER_INDEX].m_HideMissle2);
	int nHeight = GetNpcPate() + GetNpcPatePeopleInfo();
	DrawMenuState(nHeight);
	}
	}

}







	if (m_StunState.nTime)
	{

	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
	m_DataRes.DrawStun();
	else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	m_DataRes.DrawStun();
	}
	
	}


}
#endif
//--------------------------------------------------------------------------
//	功能：增加基本最大生命点
//--------------------------------------------------------------------------
void	KNpc::AddBaseLifeMax(int nLife)
{
	m_LifeMax += nLife;
	m_CurrentLifeMax = m_LifeMax;
}

//--------------------------------------------------------------------------
//	功能：增加当前最大生命点
//--------------------------------------------------------------------------
void	KNpc::AddCurLifeMax(int nLife)
{
	m_CurrentLifeMax += nLife;
}


//--------------------------------------------------------------------------
//	功能：增加基本最大体力点
//--------------------------------------------------------------------------
void	KNpc::AddBaseStaminaMax(int nStamina)
{
	m_StaminaMax += nStamina;
	m_CurrentStaminaMax = m_StaminaMax;
}

//--------------------------------------------------------------------------
//	功能：增加当前最大体力点
//--------------------------------------------------------------------------
void	KNpc::AddCurStaminaMax(int nStamina)
{
	m_CurrentStaminaMax += nStamina;
}

//--------------------------------------------------------------------------
//	功能：增加基本最大内力点
//--------------------------------------------------------------------------
void	KNpc::AddBaseManaMax(int nMana)
{
	m_ManaMax += nMana;
	m_CurrentManaMax = m_ManaMax;
}

//--------------------------------------------------------------------------
//	功能：增加当前最大内力点
//--------------------------------------------------------------------------
void	KNpc::AddCurManaMax(int nMana)
{
	m_CurrentManaMax += nMana;
}

/*
//--------------------------------------------------------------------------
//	功能：重新计算生命回复速度
//--------------------------------------------------------------------------
void	KNpc::ResetLifeReplenish()
{
	m_LifeReplenish = (m_Level + 5) / 6;
	m_CurrentLifeReplenish = m_LifeReplenish;
}
*/

/*
//--------------------------------------------------------------------------
//	功能：计算当前最大生命点
//--------------------------------------------------------------------------
void	KNpc::CalcCurLifeMax()
{
}
*/

/*
//--------------------------------------------------------------------------
//	功能：计算当前最大体力点
//--------------------------------------------------------------------------
void	KNpc::CalcCurStaminaMax()
{
	m_CurrentStaminaMax = m_StaminaMax;		// 还需要加上 装备、技能、药物（临时）等的影响
}
*/

/*
//--------------------------------------------------------------------------
//	功能：计算当前最大内力点
//--------------------------------------------------------------------------
void	KNpc::CalcCurManaMax()
{
	m_CurrentManaMax = m_ManaMax;			// 还需要加上 装备、技能、药物（临时）等的影响
}
*/

//--------------------------------------------------------------------------
//	功能：计算当前生命回复速度
//--------------------------------------------------------------------------
void	KNpc::CalcCurLifeReplenish()
{
	m_CurrentLifeReplenish = m_LifeReplenish;	// 与角色系别、角色等级和是否使用药剂、技能和魔法装备有关
}

void	KNpc::Remove()
{
/*	m_LoopFrames = 0;
	m_Index = 0;
	m_PlayerIdx = -1;
	m_Kind = 0;
	m_dwID = 0;
	Name[0] = 0;*/
	Init();
#ifndef _SERVER
	m_DataRes.Remove(m_Index);
#endif
}

#ifndef _SERVER
void	KNpc::RemoveRes()
{
	m_DataRes.Remove(m_Index);
}
#endif
//--------------------------------------------------------------------------
//	功能：设定此 npc 的五行属性（内容还没完成）not end
//--------------------------------------------------------------------------
void	KNpc::SetSeries(int nSeries)
{
	if (nSeries >= series_metal && nSeries < series_num)
		m_Series = nSeries;
	else
		m_Series = series_metal;
}



void	KNpc::SetSex(int nSex)
{
 if (nSex != 0 && nSex != 1)
	 return;
    m_nSex = nSex;
}

/*!*****************************************************************************
// Function		: KNpc::SetStateSkill
// Purpose		: 
// Return		: void 
// Argumant		: int nSkillID
// Argumant		: int nLevel
// Argumant		: void *pData
// Argumant		: int nDataNum
// Argumant		: int nTime -1表示被动技能，时间无限
// Comments		:
// Author		: Spe
*****************************************************************************/
#ifdef _SERVER
void KNpc::AddSkillEffect(int nSkillID, int nLevel, int nTime)
{
	KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(nSkillID,nLevel);
   if (pSkillActive)
	{
	KMagicAttrib* pMagicAttrib = pSkillActive->GetStateAttribs();
	int pMagicAttribNum = pSkillActive->GetStateAttribsNum();
	SetStateSkillEffect(m_Index, nSkillID, nLevel, pMagicAttrib, pMagicAttribNum, nTime);
	}
				
}



int KNpc::GetLevelSkillEffectMan(int nSkillID)
{


	if (nSkillID <= 0) 
		return 0;

    KStateNode* pNode;

	pNode = (KStateNode *)m_StateSkillList.GetHead();
	while(pNode)
	{
		if (pNode->m_SkillID == nSkillID && pNode->m_Level > 0)
		{
			return pNode->m_Level;
		}
		pNode = (KStateNode *)pNode->GetNext();
	}


	return 0;
				
}








void KNpc::DoSkillMan(int nSkillID,int nSkillLevel)
{



	if (nSkillID <= 0 || nSkillID >= MAX_SKILL || nSkillLevel <= 0 || nSkillLevel >= MAX_SKILLLEVEL) 
		return;


			
				int nMpsX, nMpsY;
				SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
				
				NPC_SKILL_SYNC SkillCmd;
				SkillCmd.ID = this->m_dwID;


				SkillCmd.nSkillID = nSkillID;
           

				SkillCmd.nSkillLevel = nSkillLevel;
				SkillCmd.nMpsX = -1;
				SkillCmd.nMpsY = m_dwID;
				SkillCmd.ProtocolType = s2c_castskilldirectly;

				POINT	POff[8] = 
				{
					{0, 32},
					{-16, 32},
					{-16, 0},
					{-16, -32},
					{0, -32},
					{16, -32},
					{16, 0},
					{16, 32},
				};
				
				int nMaxCount = MAX_BROADCAST_COUNT;
				CURREGION.BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX, m_MapY);
				int i;
				for (i = 0; i < 8; i++)
				{
					if (CONREGIONIDX(i) == -1)
						continue;
					CONREGION(i).BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
				}
				
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillID, nSkillLevel);
			
				if (pOrdinSkill)
				{
                 pOrdinSkill->Cast(m_Index, nMpsX, nMpsY);
				}
				
}



#endif








void KNpc::SetStateSkillEffect(int nLauncher, int nSkillID, int nLevel, void *pData, int nDataNum, int nTime/* = -1*/)
{
	int Map = SubWorld[m_SubWorldIndex].m_SubWorldID;

	if ((Map == 258 || Map == 106) && nLauncher != m_Index)
	{
		if (nSkillID == 206 || nSkillID == 208 || nSkillID == 93 || nSkillID == 207 || nSkillID == 281 || nSkillID == 292 || nSkillID == 476
		|| nSkillID == 171 || nSkillID == 173 || nSkillID == 178  || nSkillID == 202)
		{
			return;
		}
	}
	if (nLevel <= 0|| nSkillID <= 0) return ;
	_ASSERT(nDataNum < MAX_SKILL_STATE);
	if (nDataNum >= MAX_SKILL_STATE)
		nDataNum = MAX_SKILL_STATE;
	
#ifdef _SERVER
	if (IsPlayer() && pData && nDataNum >= 0)
	{
		STATE_EFFECT_SYNC	Sync;
		Sync.ProtocolType = s2c_syncstateeffect;
		Sync.m_dwSkillID = nSkillID;
		Sync.m_nLevel = nLevel;
		Sync.m_nTime = nTime;
        
		memcpy(Sync.m_MagicAttrib, pData, sizeof(KMagicAttrib) * nDataNum);
		Sync.m_wLength = sizeof(Sync) - sizeof(KMagicAttrib) * (MAX_SKILL_STATE - nDataNum) - 1;
		g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &Sync, Sync.m_wLength + 1);
	}
#endif
	KStateNode* pNode;
	KMagicAttrib* pTemp = NULL;

	pNode = (KStateNode *)m_StateSkillList.GetHead();
	while(pNode)
	{
		if (pNode->m_SkillID == nSkillID)
		{
			if (pNode->m_Level == nLevel)
			{
				pNode->m_LeftTime = nTime;
                
			}
			else if (pNode->m_Level < nLevel)
			{
				pNode->m_Level = nLevel;

				pTemp = (KMagicAttrib *)pData;
				for (int i = 0; i < nDataNum; i++)
				{
					// 清除原技能的影响
					ModifyAttrib(nLauncher, &pNode->m_State[i]);
					// 把新等级下技能的影响计算到NPC身上
					ModifyAttrib(nLauncher, pTemp);
					pNode->m_State[i].nValue[0] = -pTemp->nValue[0];
					pNode->m_State[i].nValue[1] = -pTemp->nValue[1];
					pNode->m_State[i].nValue[2] = -pTemp->nValue[2];
					pTemp++;
				}
			}




			return;
		}
		pNode = (KStateNode *)pNode->GetNext();

	}

	if (m_NstatusState.nTime > 0)
	{
    KSkill * pSkill = (KSkill *)g_SkillManager.GetSkill(nSkillID, nLevel);
	if (pSkill)
	{
	if (pSkill->IsTargetEnemy())
       return;
	}
	}



	// 没有在循环中返回，说明是新技能
	pNode = new KStateNode;
	pNode->m_SkillID = nSkillID;
	pNode->m_Level = nLevel;
	pNode->m_LeftTime = nTime;
    
	_ASSERT(nSkillID < MAX_SKILL && nLevel < MAX_SKILLLEVEL);
	
	KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nSkillID, nLevel);
	if (pOrdinSkill)
        pNode->m_StateGraphics = pOrdinSkill->GetStateSpecailId();
	else
		pNode->m_StateGraphics = 0;
	
	#ifdef _SERVER	
	//如果为新的状态则需要更新角色的状态信息，并且广播给各个玩家
	if (pNode->m_StateGraphics) 
	{
		m_btStateInfo[m_nNextStatePos] = pNode->m_StateGraphics;
		if ((++m_nNextStatePos) >= MAX_NPC_RECORDER_STATE) m_nNextStatePos = 0;
		BroadCastState();	
	}
#else
	if (IsPlayer() && pNode->m_StateGraphics) 
	{
		m_btStateInfo[m_nNextStatePos] = pNode->m_StateGraphics;
		if ((++m_nNextStatePos) >= MAX_NPC_RECORDER_STATE)
			m_nNextStatePos = 0;
	}
#endif
	pTemp = (KMagicAttrib *)pData;
	for (int i = 0; i < nDataNum; i++)
	{
		// 调整NPC属性
		ModifyAttrib(nLauncher, pTemp);
		// 把相反值加入链表中以供移除时使用
		pNode->m_State[i].nAttribType = pTemp->nAttribType;
		pNode->m_State[i].nValue[0] = -pTemp->nValue[0];
		pNode->m_State[i].nValue[1] = -pTemp->nValue[1];
		pNode->m_State[i].nValue[2] = -pTemp->nValue[2];
		pTemp++;
	}
	m_StateSkillList.AddTail(pNode);
}

/*!*****************************************************************************
// Function		: KNpc::ModifyMissleCollsion
// Purpose		: 
// Return		: BOOL 
// Argumant		: BOOL bCollsion
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KNpc::ModifyMissleCollsion(BOOL bCollsion)
{
	if (bCollsion)
		return TRUE;

	if (g_RandPercent(m_CurrentPiercePercent))
		return TRUE;
	else
		return FALSE;
}

int KNpc::ModifyMissleLifeTime(int nLifeTime)
{
	if (IsPlayer())
	{
		//return Player[m_PlayerIdx].GetWeapon().GetRange();
		return nLifeTime;
	}
	else
	{
		return nLifeTime;
	}
}

int	KNpc::ModifyMissleSpeed(int nSpeed)
{
	if (m_CurrentSlowMissle)
	{
		return nSpeed / 2;
	}
	return nSpeed;
}

BOOL KNpc::DoManyAttack()
{
	m_ProcessAI = 0;
	
	KSkill * pSkill =(KSkill*) GetActiveSkill();
	if (!pSkill) 
        return FALSE;
	
	if (pSkill->GetChildSkillNum() <= m_SpecialSkillStep) 		
        goto ExitManyAttack;
#ifndef _SERVER
        m_DataRes.SetBlur(TRUE);
#endif
	
	m_Frames.nTotalFrame = pSkill->GetMissleGenerateTime(m_SpecialSkillStep);
	
	int x, y;
	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
//	m_DesX = x;
//	m_DesY = y;

	
#ifndef _SERVER
	if (m_nPlayerIdx > 0)
		pSkill->PlayPreCastSound(m_nSex, x ,y);
	if (g_Random(2))
		m_ClientDoing = cdo_attack;
	else 
		m_ClientDoing = cdo_attack1;
#endif

	
	m_Doing = do_manyattack;
	
	m_Frames.nCurrentFrame = 0;

	return TRUE;

ExitManyAttack:

#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
	DoStand();
	m_ProcessAI = 1;
	m_SpecialSkillStep = 0;

	return TRUE;
}

void KNpc::OnManyAttack()
{
	if (WaitForFrame())
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		KSkill * pSkill = (KSkill*)GetActiveSkill();
		if (!pSkill) 
            return ;

		int nPhySkillId =  pSkill->GetChildSkillId();//GetCurActiveWeaponSkill(); Changed 

		if (nPhySkillId > 0)
		{
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
			if (pOrdinSkill)
            {
				pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
            }
		}
		m_SpecialSkillStep ++;
		DoManyAttack();

	}	
}

BOOL	KNpc::DoRunAttack()
{
	m_ProcessAI = 0;

	switch(m_SpecialSkillStep)
	{
	case 0:
		m_Frames.nTotalFrame = 20;
		m_ProcessAI = 0;
		
#ifndef _SERVER
		m_DataRes.SetBlur(TRUE);

		if (m_FightMode)
		{
			m_ClientDoing = cdo_fightrun;
		}
		else
		{
			m_ClientDoing = cdo_run;
		}
#endif
		
		if (m_DesX < 0 && m_DesY > 0) 
		{
			int x, y;
			SubWorld[m_SubWorldIndex].Map2Mps
				(
				Npc[m_DesY].m_RegionIndex,
				Npc[m_DesY].m_MapX, 
				Npc[m_DesY].m_MapY, 
				Npc[m_DesY].m_OffX, 
				Npc[m_DesY].m_OffY, 
				&x,
				&y
				);

		m_DesX = x;
		m_DesY = y;
		}

		m_Frames.nCurrentFrame = 0;
		m_Doing = do_runattack;
		break;

	case 1:
#ifndef _SERVER
		
		if (m_FightMode)
			m_ClientDoing = cdo_fightstand;
		else if (g_Random(6) != 1)
		{
			m_ClientDoing = cdo_stand;
		}
		else
		{
			m_ClientDoing = cdo_stand1;
		}

		int x, y, tx, ty;
		SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
		if (m_SkillParam1 == -1)
		{
			Npc[m_SkillParam2].GetMpsPos(&tx, &ty);
		}
		else
		{
			tx = m_SkillParam1;
			ty = m_SkillParam2;
		}
		m_Dir = g_GetDirIndex(x, y, tx, ty);
#endif
		m_Frames.nTotalFrame = 0;
		m_Frames.nCurrentFrame = 0;
		m_Doing = do_runattack;
		break;

	case 2:
	case 3:
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
		break;
	}

	m_Frames.nCurrentFrame = 0;
		
	return TRUE;

}






void	KNpc::OnGoAttack()
{
				
	if (m_SpecialSkillStep == 0)
	{
		KSkill * pSkill = (KSkill*)GetActiveSkill();
		if (!pSkill) 
            return ;


	OnRun(52);
		
        if (m_Doing == do_stand || (DWORD)m_nCurrentMeleeTime > pSkill->GetMissleGenerateTime(0)) 
		{
			m_SpecialSkillStep ++;
			m_nCurrentMeleeTime = 0;

			DoGoAttack();
		
		}
		else
			m_nCurrentMeleeTime ++;

		m_ProcessAI = 0;
	}
	else if (m_SpecialSkillStep == 1)
	{
			m_ProcessAI = 0;	
		
			KSkill * pSkill = (KSkill*)GetActiveSkill();
			if (!pSkill) 
                return ;
			
            int nCurPhySkillId = pSkill->GetChildSkillId();//GetCurActiveWeaponSkill();
			if (nCurPhySkillId > 0)
			{
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nCurPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
				if (pOrdinSkill)
                {
				    pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
                }
			}
			DoStand();
			m_ProcessAI = 1;
			m_SpecialSkillStep = 0;
		
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
	}
	else
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
	}
}



void	KNpc::OnRunAttack()
{
				
	if (m_SpecialSkillStep == 0)
	{
		KSkill * pSkill = (KSkill*)GetActiveSkill();
		if (!pSkill) 
            return ;

      int AddSpeed = 0;
    KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(pSkill->GetSkillId(), pSkill->m_ulLevel);
	if (pOrdinSkill)
       {
	   AddSpeed = pOrdinSkill->m_addskillmisslesrun;
       }

	
	OnRun(AddSpeed);
        if (m_Doing == do_stand || (DWORD)m_nCurrentMeleeTime > pSkill->GetMissleGenerateTime(0)) 
		{
			m_SpecialSkillStep ++;
			m_nCurrentMeleeTime = 0;

			DoRunAttack();
		
		}
		else
			m_nCurrentMeleeTime ++;

		m_ProcessAI = 0;
	}
	else if (m_SpecialSkillStep == 1)
	{
			m_ProcessAI = 0;	
		
			KSkill * pSkill = (KSkill*)GetActiveSkill();
			if (!pSkill) 
                return ;
			
            int nCurPhySkillId = pSkill->GetChildSkillId();//GetCurActiveWeaponSkill();
			if (nCurPhySkillId > 0)
			{
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nCurPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
				if (pOrdinSkill)
                {
				    pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
                }
			}
			DoStand();
			m_ProcessAI = 1;
			m_SpecialSkillStep = 0;
		
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
	}
	else
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
	}
}

BOOL KNpc::DoJumpAttack()
{
	m_ProcessAI = 0;
	
	switch(m_SpecialSkillStep)
	{
	case 0:
		DoJump();

#ifndef _SERVER
		m_DataRes.SetBlur(TRUE);
		m_ClientDoing = cdo_jump;
#endif
		m_Doing = do_jumpattack;
		break;
		
	case 1:
#ifndef _SERVER
		if (g_Random(2))	
			m_ClientDoing = cdo_attack;
		else
			m_ClientDoing = cdo_attack1;
		int x, y, tx, ty;
		SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
		if (m_SkillParam1 == -1)
		{
			Npc[m_SkillParam2].GetMpsPos(&tx, &ty);
		}
		else
		{
			tx = m_SkillParam1;
			ty = m_SkillParam2;
		}
		m_Dir = g_GetDirIndex(x, y, tx, ty);
#endif
		m_Frames.nTotalFrame = m_AttackFrame * 100 / (100 + m_CurrentAttackSpeed);
		m_Frames.nCurrentFrame = 0;
		m_Doing = do_jumpattack;
		break;
		
	case 2:
	case 3:
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
		break;
	}
	
	m_Frames.nCurrentFrame = 0;
	
	return TRUE;
	
}

BOOL KNpc::OnJumpAttack()
{
	if (m_SpecialSkillStep == 0)
	{
		if (!OnJump())
		{
			m_SpecialSkillStep ++;
			m_nCurrentMeleeTime = 0;
			DoJumpAttack();
		}
		m_ProcessAI = 0;
	}
	else if (m_SpecialSkillStep == 1)
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		if (WaitForFrame() &&m_Frames.nTotalFrame != 0)
		{
			DoStand();
			m_ProcessAI = 1;	
		}
		else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
		{
			KSkill * pSkill =(KSkill*) GetActiveSkill();
			if (!pSkill) 
                return FALSE;
			
            int nCurPhySkillId = pSkill->GetChildSkillId();//GetCurActiveWeaponSkill();
			if (nCurPhySkillId > 0)
			{
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nCurPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
				if (pOrdinSkill)
                {
					pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
                }
			}
			DoStand();
			m_ProcessAI = 1;
			m_SpecialSkillStep = 0;
		}
	}
	else
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
	}
	return TRUE;
}

BOOL KNpc::CheckHitTarget(int nAR, int nDf, int nIngore/* = 0*/)
{
	int nDefense = nDf * (100 - nIngore) / 100;
	int nPercent = 0;
	//Question nAr+ nDefense  == 0 ,error!so.....,must modify
	if ((nAR + nDefense) == 0) 
		nPercent = 50;
	else
		nPercent = nAR * 100 / (nAR + nDefense);

	if (nPercent > MAX_HIT_PERCENT)
		nPercent = MAX_HIT_PERCENT;

	if (nPercent < MIN_HIT_PERCENT)
		nPercent = MIN_HIT_PERCENT;

	BOOL bRet = g_RandPercent(nPercent);
	//g_DebugLog("[数值]AttackRating %d : Defense %d: RandomPercent (%d, %d)", nAR, nDf, nPercent, bRet);
	return bRet;
}

void KNpc::GetNpcCopyFromTemplate(int nNpcTemplateId, int nLevel)
{
	if (nNpcTemplateId < 0 || nLevel < 1 ) 
		return;
	
	if (g_pNpcTemplate[nNpcTemplateId][nLevel]) //数据有效则拷贝，否则重新生成
		LoadDataFromTemplate(nNpcTemplateId, nLevel);
	else
	{
		if (!g_pNpcTemplate[nNpcTemplateId][0])
		{
			g_pNpcTemplate[nNpcTemplateId][0] = new KNpcTemplate;
			g_pNpcTemplate[nNpcTemplateId][0]->InitNpcBaseData(nNpcTemplateId);
			g_pNpcTemplate[nNpcTemplateId][0]->m_NpcSettingIdx = nNpcTemplateId;
			g_pNpcTemplate[nNpcTemplateId][0]->m_bHaveLoadedFromTemplate = TRUE;
		}
		KLuaScript * pLevelScript = NULL;		

#ifdef _SERVER
			pLevelScript = (KLuaScript*)g_GetScript(
			g_pNpcTemplate[nNpcTemplateId][0]->m_dwLevelSettingScript
			);
		
		if (pLevelScript == NULL)
			pLevelScript = g_pNpcLevelScript;
#else
		KLuaScript LevelScript;
		if (!g_pNpcTemplate[nNpcTemplateId][0]->m_szLevelSettingScript[0])
			pLevelScript = g_pNpcLevelScript;
		else
		{
			LevelScript.Init();
			if (!LevelScript.Load(g_pNpcTemplate[nNpcTemplateId][0]->m_szLevelSettingScript))
			{
				printf ("[Client][GetNpcCopyFromTemplate] nNpcTemplateId: %d. Can not read file %s", nNpcTemplateId, g_pNpcTemplate[nNpcTemplateId][0]->m_szLevelSettingScript);
				//_ASSERT(0);
				pLevelScript = g_pNpcLevelScript;
			}
			else
			{
				pLevelScript = &LevelScript;
			}
		}

#endif
		g_pNpcTemplate[nNpcTemplateId][nLevel] = new KNpcTemplate;
		*g_pNpcTemplate[nNpcTemplateId][nLevel] = *g_pNpcTemplate[nNpcTemplateId][0];
		g_pNpcTemplate[nNpcTemplateId][nLevel]->m_nLevel = nLevel;
		g_pNpcTemplate[nNpcTemplateId][nLevel]->InitNpcLevelData(&g_NpcKindFile, nNpcTemplateId, nLevel);
		g_pNpcTemplate[nNpcTemplateId][nLevel]->m_bHaveLoadedFromTemplate = TRUE;
		LoadDataFromTemplate(nNpcTemplateId,nLevel);
	}
}

void	KNpc::LoadDataFromTemplate(int nNpcTemplateId, int nLevel)
{
	if (nNpcTemplateId < 0)
	{
		g_DebugLog("[LoadDataFromTemplate] nNpcTemplateId < 0", nNpcTemplateId);
		return;
	}

	KNpcTemplate * pNpcTemp = g_pNpcTemplate[nNpcTemplateId][nLevel];

	strcpy(Name,pNpcTemp->Name);
	m_Kind = pNpcTemp->m_Kind;
	m_Camp = pNpcTemp->m_Camp;
	m_Series = pNpcTemp->m_Series;
//	m_HeadImage =	pNpcTemp->m_HeadImage;
	m_bClientOnly = pNpcTemp->m_bClientOnly;
	m_CorpseSettingIdx =	pNpcTemp->m_CorpseSettingIdx;
	m_DeathFrame =	pNpcTemp->m_DeathFrame;
	m_WalkFrame =	pNpcTemp->m_WalkFrame;
	m_RunFrame =	pNpcTemp->m_RunFrame;
	m_HurtFrame =	pNpcTemp->m_HurtFrame;
	m_WalkSpeed =	pNpcTemp->m_WalkSpeed;
	m_AttackFrame =	pNpcTemp->m_AttackFrame;
	m_CastFrame =	pNpcTemp->m_CastFrame;
	m_RunSpeed =	pNpcTemp->m_RunSpeed;
	m_StandFrame =  pNpcTemp->m_StandFrame;
	m_StandFrame1 = pNpcTemp->m_StandFrame1;
	m_NpcSettingIdx = pNpcTemp->m_NpcSettingIdx;
	m_nStature		= pNpcTemp->m_nStature;

	m_NumObjT      = pNpcTemp->m_NumObjT;
	m_DirObjT      = pNpcTemp->m_DirObjT;

	m_TypeNpc       = pNpcTemp->m_TypeNpc;
	m_nShareDame    = pNpcTemp->m_nShareDame;
	m_nCurrentShareDame    = m_nShareDame;
	m_nPencentDameAdd = pNpcTemp->m_nPencentDameAdd;
	m_nCurentPencentDameAdd = m_nPencentDameAdd;

#ifdef _SERVER
    m_RateBoss      = pNpcTemp->m_RateBoss;
	m_Treasure		= pNpcTemp->m_Treasure;
	m_SkillList		= pNpcTemp->m_SkillList;
	m_AiMode		= pNpcTemp->m_AiMode;
	m_AiAddLifeTime	= 0;
	m_pDropRate		= pNpcTemp->m_pItemDropRate;

	if (!m_AiSkillRadiusLoadFlag)
	{
		m_AiSkillRadiusLoadFlag = 1;
		int i;
		for (i = 0; i < MAX_AI_PARAM - 1; i ++)
			m_AiParam[i] =	pNpcTemp->m_AiParam[i];

		int		nMaxRadius = 0, nTempRadius;
		KSkill	*pSkill;
		for (i = 1; i < MAX_NPC_USE_SKILL + 1; i++)
		{
			pSkill = (KSkill*)g_SkillManager.GetSkill(m_SkillList.m_Skills[i].SkillId, m_SkillList.m_Skills[i].SkillLevel);
			if (!pSkill)
				continue;
			nTempRadius = pSkill->GetAttackRadius();
			if (nTempRadius > nMaxRadius)
				nMaxRadius = nTempRadius;
		}
		m_AiParam[MAX_AI_PARAM - 1] = nMaxRadius * nMaxRadius;
	}

	m_FireResistMax			= pNpcTemp->m_FireResistMax;
	m_ColdResistMax			= pNpcTemp->m_ColdResistMax;
	m_LightResistMax		= pNpcTemp->m_LightResistMax;
	m_PoisonResistMax		= pNpcTemp->m_PoisonResistMax;
	m_PhysicsResistMax		= pNpcTemp->m_PhysicsResistMax;
	m_ActiveRadius			= pNpcTemp->m_ActiveRadius;
	m_VisionRadius			= pNpcTemp->m_VisionRadius;
	m_AIMAXTime				= pNpcTemp->m_AIMAXTime;
	m_HitRecover			= pNpcTemp->m_HitRecover;
	m_ReviveFrame			= pNpcTemp->m_ReviveFrame;
	m_Experience			= pNpcTemp->m_Experience;
	m_CurrentExperience		= m_Experience;
	m_LifeMax				= pNpcTemp->m_LifeMax;
	m_LifeReplenish			= pNpcTemp->m_LifeReplenish;
	m_AttackRating			= pNpcTemp->m_AttackRating;
	m_Defend				= pNpcTemp->m_Defend;
	m_PhysicsDamage			= pNpcTemp->m_PhysicsDamage;
	m_RedLum				= pNpcTemp->m_RedLum;
	m_GreenLum				= pNpcTemp->m_GreenLum;
	m_BlueLum				= pNpcTemp->m_BlueLum;
	m_FireResist			= pNpcTemp->m_FireResist;
	m_ColdResist			= pNpcTemp->m_ColdResist;
	m_LightResist			= pNpcTemp->m_LightResist;
	m_PoisonResist			= pNpcTemp->m_PoisonResist;
	m_PhysicsResist			= pNpcTemp->m_PhysicsResist;
#else
	m_LifeMax				= pNpcTemp->m_LifeMax;
	m_ArmorType				= pNpcTemp->m_ArmorType;
	m_HelmType				= pNpcTemp->m_HelmType;
	m_WeaponType			= pNpcTemp->m_WeaponType;
	m_HorseType				= pNpcTemp->m_HorseType;
	m_bRideHorse			= pNpcTemp->m_bRideHorse;
	strcpy(ActionScript, pNpcTemp->ActionScript);
#endif
	
	/*for (int j  = 0; j < 4; j ++)
		m_SkillList.Add()
	int		m_nSkillID[4];
	int		m_nSkillLevel[4];*/

	//BOOL	m_bHaveLoadedFromTemplate;
	//int		m_nNpcTemplateId;
	m_Level = pNpcTemp->m_nLevel;
	RestoreNpcBaseInfo();

#ifdef _SERVER
if (m_Kind != kind_player && m_Kind != kind_dialoger)
{
	if (m_TypeNpc == 0)
	{
	int RamDom = g_Random(NPC_RANDOM_BOSS);
	if (RamDom < m_RateBoss)
	{
	m_CurrentFireResistMax	= m_FireResistMax;
	m_CurrentColdResistMax	= m_ColdResistMax;
	m_CurrentPoisonResistMax = m_PoisonResistMax;
	m_CurrentLightResistMax	= m_LightResistMax;
	m_CurrentPhysicsResistMax  = m_PhysicsResistMax;
	m_CurrentFireResist		= m_FireResist;
	m_CurrentColdResist		= m_ColdResist;
	m_CurrentPoisonResist	= m_PoisonResist;
	m_CurrentLightResist	= m_LightResist;
	m_CurrentPhysicsResist	= m_PhysicsResist;
	m_CurrentExperience = m_Experience * 20;
	m_CurrentTreasure = m_Treasure * 20 * 7;
	m_nCurrentShareDame = (m_nShareDame + 1) * 20 - 1;
	m_nCurentPencentDameAdd = (m_nPencentDameAdd + 100) * 2 - 100;

	m_IsBoss = 1;
	}
	else
	{
	m_CurrentFireResistMax	= m_FireResistMax;
	m_CurrentColdResistMax	= m_ColdResistMax;
	m_CurrentPoisonResistMax = m_PoisonResistMax;
	m_CurrentLightResistMax	= m_LightResistMax;
	m_CurrentPhysicsResistMax  = m_PhysicsResistMax;
	m_CurrentFireResist		= m_FireResist;
	m_CurrentColdResist		= m_ColdResist;
	m_CurrentPoisonResist	= m_PoisonResist;
	m_CurrentLightResist	= m_LightResist;
	m_CurrentPhysicsResist	= m_PhysicsResist;
	m_CurrentTreasure       = m_Treasure;
	m_CurrentExperience = m_Experience;
	m_nCurrentShareDame = m_nShareDame;
	m_nCurentPencentDameAdd = m_nPencentDameAdd;
    m_IsBoss = 0;
	}
	}
}
#endif
}

//-----------------------------------------------------------------------
//	功能：设定物理攻击的最大最小值 not end 需要考虑调用的地方
//-----------------------------------------------------------------------
void	KNpc::SetPhysicsDamage(int nMinDamage, int nMaxDamage)
{
	m_PhysicsDamage.nValue[0] = nMinDamage;
	m_PhysicsDamage.nValue[2] = nMaxDamage;
}

void	KNpc::SetReviveFrame(int nReviveFrame)
{
	m_ReviveFrame = nReviveFrame;
}

//-----------------------------------------------------------------------
//	功能：设定攻击命中率
//-----------------------------------------------------------------------
void	KNpc::SetBaseAttackRating(int nAttackRating)
{
	m_AttackRating = nAttackRating;
	// 此处还需要加上装备、技能的影响，计算出当前值
	m_CurrentAttackRating = m_AttackRating;
}

//-----------------------------------------------------------------------
//	功能：设定防御力
//-----------------------------------------------------------------------
void	KNpc::SetBaseDefence(int nDefence)
{
	m_Defend = nDefence;
	// 此处还需要加上装备、技能的影响，计算出当前值
	m_CurrentDefend = m_Defend;
}

/*
//-----------------------------------------------------------------------
//	功能：设定行走速度
//-----------------------------------------------------------------------
void	KNpc::SetBaseWalkSpeed(int nSpeed)
{
	m_WalkSpeed = nSpeed;
	// 此处还需要加上装备、技能的影响，计算出当前值 (not end)
	m_CurrentWalkSpeed = m_WalkSpeed;
}
*/

/*
//-----------------------------------------------------------------------
//	功能：设定跑步速度
//-----------------------------------------------------------------------
void	KNpc::SetBaseRunSpeed(int nSpeed)
{
	m_RunSpeed = nSpeed;
	// 此处还需要加上装备、技能的影响，计算出当前值 (not end)
	m_CurrentRunSpeed = m_RunSpeed;
}
*/

#ifdef _SERVER
void KNpc::DeathPunish(int nMode, int nBelongPlayer) //Exp PK Chet
{
#define	LOSE_EXP_SCALE		10
SYSTEMTIME TimeNow;
GetLocalTime(&TimeNow);
	if (IsPlayer())
	{
		// 被npc kill
		if (nMode == enumDEATH_MODE_NPC_KILL)
		{
			// 经验减少
			if (Player[m_nPlayerIdx].m_nExp > 0)
			{
				int nSubExp;
				if (m_Level <= 10)
					nSubExp = PlayerSet.m_cLevelAdd.GetLevelExp(m_Level)/5;
				else if (m_Level <= 50)
					nSubExp = PlayerSet.m_cLevelAdd.GetLevelExp(m_Level)/10;
				else if (m_Level < 80)
					nSubExp = PlayerSet.m_cLevelAdd.GetLevelExp(m_Level)/15;
				else if (m_Level <= 90)
					nSubExp =50000;
				else if (m_Level > 90)
					nSubExp = 100000;	
				if (TimeNow.wHour == 17)
					nSubExp = 0;		
				
				if (Player[m_nPlayerIdx].m_nExp >= nSubExp)
					Player[m_nPlayerIdx].DirectAddExp( -nSubExp );
				else
					Player[m_nPlayerIdx].DirectAddExp( -Player[m_nPlayerIdx].m_nExp );
			}
			// 钱减少
			int nMoney = Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney() / 2;
			nMoney = 0;
			if (nMoney > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.CostMoney(nMoney);
				// 损失金钱消息
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_DEC_MONEY;
				sMsg.m_lpBuf = (void *)(nMoney);
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
				g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				sMsg.m_lpBuf = 0;

				if (nMoney / 2 > 0)
					PlayerDeadCreateMoneyObj(nMoney / 2);
			}
		}
		// 切磋，没有惩罚
		else if (nMode == enumDEATH_MODE_PLAYER_NO_PUNISH)
		{
			return;
		}
		else if (nMode == enumDEATH_MODE_PKBATTLE_PUNISH)
		{
			return;
		}
		// PK致死，按PK值计算惩罚
		else //if (nMode == enumDEATH_MODE_PLAYER_PUNISH)
		{
			int		nPKValue;
			nPKValue = Player[this->m_nPlayerIdx].m_cPK.GetPKValue();
			if (nPKValue < 0)
				nPKValue = 0;
			if (nPKValue > MAX_DEATH_PUNISH_PK_VALUE)
				nPKValue = MAX_DEATH_PUNISH_PK_VALUE;

			// 经验减少
			int		nLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(m_Level);
			int 		nExpTru = nLevelExp / 100 * PlayerSet.m_sPKPunishParam[nPKValue].m_nExp;
			if (Player[m_nPlayerIdx].m_nExp	< -1600000000)
			{
				nExpTru = 0;
			}
//xxx			
			Player[m_nPlayerIdx].DirectAddExp( -nExpTru );
			// 钱减少
			int nMoney = Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney() / 100 * PlayerSet.m_sPKPunishParam[nPKValue].m_nMoney;
			if (nMoney > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.CostMoney(nMoney);
				// 损失金钱消息
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_DEC_MONEY;
				sMsg.m_lpBuf = (void *)(nMoney);
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
				g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				sMsg.m_lpBuf = 0;
				
				if (nMoney / 2 > 0)
					PlayerDeadCreateMoneyObj(nMoney / 2);
			}
			int Map = SubWorld[m_SubWorldIndex].m_SubWorldID;
        	if (Map == 20 && Player[m_nPlayerIdx].m_cTask.GetSaveVal(196) >= 1)
				{
				ChangeWorld(20, 202 * 8 * 32, 200 * 16 * 32);
				}
			// 丢失物品
			if (nPKValue > 0)
			{
				
				if (nPKValue == 10)
				{
					Player[m_nPlayerIdx].m_ItemList.SetItemNotRepair();
					char ThongBaoDoBen[100];
					sprintf(ThongBaoDoBen,"To祅 b?trang b?m芻 tr猲 ngi  h醤g, vui l遪g n Th?R蘮  ph鬰 h錳");
					KPlayerChat::SendSystemInfo(1,m_nPlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD,ThongBaoDoBen,strlen(ThongBaoDoBen) );
					
					if (Player[m_nPlayerIdx].m_nExp < (-Player[m_nPlayerIdx].m_nNextLevelExp/2))
					{
						if (Player[m_nPlayerIdx].m_cTask.GetSaveVal(534) == 100)
						ChangeWorld(5, 200 * 8 * 32, 200 * 16 * 32);	
						else
						ChangeWorld(5, 200 * 8 * 32, 200 * 16 * 32);	
						g_pServer->ShutdownClient(Player[m_nPlayerIdx].m_nNetConnectIdx);
						Player[m_nPlayerIdx].m_cTask.SetSaveVal(1,0);
						Player[m_nPlayerIdx].m_cTask.SetSaveVal(143,1);
					}
					
				}
				else
				{
					int dobengiam = nPKValue*5;
					Player[m_nPlayerIdx].m_ItemList.AutoDurationItem(dobengiam);
					char ThongBaoDoBen[100];
					sprintf(ThongBaoDoBen,"ч b襫 to祅 b?trang b?gi秏 %d ph莕 tr╩ do b筺 產ng c?%d 甶觤 PK trong ngi",dobengiam,nPKValue);
					KPlayerChat::SendSystemInfo(1,m_nPlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD,ThongBaoDoBen,strlen(ThongBaoDoBen) );
				}
				
			}

			// 丢失穿在身上的装备
			if (g_Random(100) < PlayerSet.m_sPKPunishParam[nPKValue].m_nEquip)
			{
				Player[m_nPlayerIdx].m_ItemList.AutoLoseEquip();
			}

            if (nMode == enumDEATH_MODE_PKPLAYER_PUNISH)
			{
				Player[m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nBeKilledAddPKValue);
			}

			if (m_nLastDamageIdx)
			{
				if (Npc[m_nLastDamageIdx].IsPlayer())
				{
					KPlayerChat::MakeEnemy(Name, Npc[m_nLastDamageIdx].Name);
				}
			}
		}
	}
//	else if (m_nLastDamageIdx && Npc[m_nLastDamageIdx].IsPlayer())
	else if (nBelongPlayer > 0 && m_pDropRate)
	{

		if (Player[nBelongPlayer].m_nIndex <= 0 || Player[nBelongPlayer].m_nIndex >= MAX_NPC)
			 return;

		if (m_TypeNpc == 2)
             return;

    int nLuck = Player[nBelongPlayer].m_nCurLucky;


	int		nSubLevel, nLevelPlayer, nRateMoney;
	
    nLevelPlayer = Npc[Player[nBelongPlayer].m_nIndex].m_Level;

    if (nLevelPlayer > 100)
    nLevelPlayer = 100;

	nSubLevel = nLevelPlayer - m_Level;
	int Map = SubWorld[m_SubWorldIndex].m_SubWorldID;

SYSTEMTIME TimeNow;
GetLocalTime(&TimeNow);

		nRateMoney = m_pDropRate->nMoneyRate;

		
	       if (m_IsBoss)
			{
				Player[nBelongPlayer].ExecuteScript("\\script\\pass\\addquai\\bossxanh.lua","OnDeath",0);
				if (g_RandPercent(m_pDropRate->nPropMagicRate))
				{
					LoseProp(nBelongPlayer);				
				}
			}

			else
			{

				if (g_RandPercent(m_pDropRate->nPropRate))
				{
					LoseProp(nBelongPlayer);
				}

			}

		int New_Treasure_LB = 0;
		if (TimeNow.wDay == 21)
			New_Treasure_LB = 40;
		else
			New_Treasure_LB = 20;
			
		int New_Treasure_THK = 0;
		if (TimeNow.wDay == 21)
			New_Treasure_THK = 30;
		else
			New_Treasure_THK = 20;	
			
		for (int u = 0; u < New_Treasure_LB; u++)
		{
			if ( m_TypeNpc == 5) // BOSS LENH BAI
			{
				if (g_RandPercent(100))
				{
					LoseSingleItem(nBelongPlayer);
				}
			}
		}
		for (int t = 0; t < New_Treasure_THK; t++)
		{
			if ( m_TypeNpc == 6) // BOSS TIEU HK
			{
				if (g_RandPercent(100))
				{
					LoseSingleItem(nBelongPlayer);
				}
			}
		}
		
		
		
		for (int i = 0; i < m_CurrentTreasure; i++)
		{
	
				if (m_IsBoss)
				{
					if (g_RandPercent(200))
					{
						LoseSingleItem(nBelongPlayer);
					}
				}
				else if (m_TypeNpc == 1) // BOSS HOANG KIm
				{
					if (g_RandPercent(100))
					{
						LoseSingleItem(nBelongPlayer);
					}
				}
				else
				{
					if (g_RandPercent(nRateMoney))  // Vong Lap Roi Tien Tu Quai
					{
						LoseMoney(nBelongPlayer);
					}
					if (g_RandPercent(50 + nLuck/2))
					{
						LoseSingleItem(nBelongPlayer);
					}
				}
			
		

			if (g_RandPercent(m_pDropRate->nPropAllRate))
			{
				LosePropAll(m_pDropRate->nPropAllByTeam);
			}



		}
	}
}


void	KNpc::DeathDropEvent(int nPlayer)
{

if (m_Index <= 0 || m_Index >= MAX_NPC)
return;

if (m_TypeNpc != 2)
return;

if (!m_pDropRate)
return;

if (nPlayer <= 0 || nPlayer >= MAX_PLAYER)
return;
		 
if (Player[nPlayer].m_nIndex <= 0 || Player[nPlayer].m_nIndex >= MAX_NPC)
return;


if (IsPlayer())
return;


		for (int i = 0; i < m_CurrentTreasure; i++)
		{
			int nLuck = Player[nPlayer].m_nCurLucky;

			if (g_RandPercent(m_pDropRate->nMoneyRate))
			{
				LoseMoney(nPlayer);
			}
			
			if (g_RandPercent(50 + nLuck/2))
			{
				LoseSingleItem(nPlayer);
			}
		}



}

// 玩家死的时候掉出来的钱生成一个object
void	KNpc::PlayerDeadCreateMoneyObj(int nMoneyNum)
{
	int		nX, nY;
	POINT	ptLocal;
	KMapPos	Pos;

	GetMpsPos(&nX, &nY);
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);

	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);
	
	int nObjIdx = ObjSet.AddMoneyObj(Pos, nMoneyNum);
	if (nObjIdx > 0 && nObjIdx < MAX_OBJECT)
	{
		Object[nObjIdx].SetItemBelong(-1);
		Object[nObjIdx].m_bByPlayer = TRUE;
	}
}

void KNpc::LoseMoney(int nBelongPlayer)
{
	int nX, nY;
	POINT	ptLocal;
	KMapPos	Pos;

	int nMoney = m_CurrentExperience * m_pDropRate->nMoneyScale / 100;
	if (nMoney <= 0)
		return;

	GetMpsPos(&nX, &nY);
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);
	
	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);
	
	int nObjIdx = ObjSet.AddMoneyObj(Pos, nMoney);
	if (nObjIdx > 0 && nObjIdx < MAX_OBJECT)
	{
		if (nBelongPlayer > 0)
			Object[nObjIdx].SetItemBelong(nBelongPlayer);
		else
			Object[nObjIdx].SetItemBelong(-1);

		Object[nObjIdx].m_bByPlayer = FALSE;
	}

}






void KNpc::LosePropAll(int nTeam)
{
	int nX, nY;
	POINT	ptLocal;
	KMapPos	Pos;

	int nDataID = 0;
	int nColorID = 0;

	
	nDataID = m_pDropRate->nPropAllScaleMin + g_Random(1 + m_pDropRate->nPropAllScaleMax - m_pDropRate->nPropAllScaleMin);
	
	

	if (nDataID <= 0)
		return;

	GetMpsPos(&nX, &nY);
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);
	
	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);
	
	int nObjIdx = ObjSet.AddPropObj(Pos, nDataID, nColorID);
	if (nObjIdx > 0 && nObjIdx < MAX_OBJECT)
	{
		
		Object[nObjIdx].SetItemBelong(-1);

		Object[nObjIdx].m_bByPlayer = FALSE;
	}

}







void KNpc::LoseProp(int nBelongPlayer)
{
	int nX, nY;
	POINT	ptLocal;
	KMapPos	Pos;

	int nDataID = 0;
	int nColorID = 0;

	if (m_IsBoss)
	{
		nDataID = m_pDropRate->nPropMagicScale;
		nColorID = 2;
	}
	else
	{
        nDataID = m_pDropRate->nPropScale;
		nColorID = 0;
	}

	if (nDataID <= 0)
		return;

	GetMpsPos(&nX, &nY);
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);
	
	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);
	
	int nObjIdx = ObjSet.AddPropObj(Pos, nDataID, nColorID);
	if (nObjIdx > 0 && nObjIdx < MAX_OBJECT)
	{
		if (nBelongPlayer > 0)
			Object[nObjIdx].SetItemBelong(nBelongPlayer);
		else
			Object[nObjIdx].SetItemBelong(-1);

		Object[nObjIdx].m_bByPlayer = FALSE;
	}

}




void KNpc::LoseSingleItem(int nBelongPlayer)
{
if (!m_pDropRate)
		return;

	if (m_pDropRate->nMaxItemLevelScale <= 0 || m_pDropRate->nMinItemLevelScale <= 0)
		return;

	int nRand = g_Random(m_pDropRate->nMaxRandRate);
	int nCheckRand = 0;	// 累加概率，确认是否落在区间内

	int i;
	for (i = 0; i < m_pDropRate->nCount; i++)
	{
		if (nRand >= nCheckRand && nRand < nCheckRand + m_pDropRate->pItemParam[i].nRate)
		{
			break;
		}
		nCheckRand += m_pDropRate->pItemParam[i].nRate;
	}

	if (i == m_pDropRate->nCount)
		return;

	int nGenre, nSeries, nLuck, nDetail, nParticular, nLevel, nLevelMagic, pnMagicLevel[6];
	nGenre = m_pDropRate->pItemParam[i].nGenre;
	nDetail = m_pDropRate->pItemParam[i].nDetailType;
	nParticular = m_pDropRate->pItemParam[i].nParticulType;
	nSeries = m_Series;

	nLuck = Player[nBelongPlayer].m_nCurLucky;

	int nMaxLevel = m_pDropRate->nMaxItemLevel;
	int nMinLevel = m_pDropRate->nMinItemLevel;

	int nMaxLevelMagic = m_pDropRate->nMaxItemLevelScale;
	int nMinLevelMagic = m_pDropRate->nMinItemLevelScale;

	if (nMaxLevel < nMinLevel)
	{
		int nTemp = nMinLevel;
		nMinLevel = nMaxLevel;
		nMaxLevel = nTemp;
	}

	if (nMaxLevelMagic < nMinLevelMagic)
	{
		int nTempM = nMinLevelMagic;
		nMinLevelMagic = nMaxLevelMagic;
		nMaxLevelMagic = nTempM;
	}

	int nLucky = Player[nBelongPlayer].m_nCurLucky;

	if (g_RandPercent(nLucky/10))
	{
	nLevel = nMaxLevel;
	}
	else if (g_RandPercent(nLucky/2 + 50))
	{
	nLevel = g_Random(nMaxLevel + 1 - nMinLevel) + nMinLevel;
	}
	else
	{
	nLevel = nMinLevel;
	}

	int nSlMagicTT = 0;
	int nRandomMgTT = g_Random(10);

	
	

	if (nRandomMgTT < (m_pDropRate->nMagicRate + nLucky/20 + 3))
	{
	nSlMagicTT = g_Random(4 + m_pDropRate->nMagicRate + nLucky/20 + 1);
	}
	else
	{
	nSlMagicTT = g_Random(2 + m_pDropRate->nMagicRate + nLucky/20 + 1);
	}



	BOOL	bSkip = FALSE;
	for (int j = 0; j < 6; j++)
	{
		if (!bSkip)
		{
			
			 if (j < nSlMagicTT)
			 {
				 
	             nLevelMagic = g_Random(nMaxLevelMagic + 1 - nMinLevelMagic) + nMinLevelMagic;
				
				 pnMagicLevel[j] = nLevelMagic;
			 }
			 else
			 {
				 pnMagicLevel[j] = 0;
				 bSkip = TRUE;
			 } 
		}
		else
		{
			pnMagicLevel[j] = 0;
		}
	}

	int nIdx = ItemSet.Add(nGenre, nSeries, nLevel, nLuck, nDetail, nParticular, pnMagicLevel, g_SubWorldSet.GetGameVersion());

	if (nIdx <= 0)
		return;

	int		nX, nY;
	POINT	ptLocal;
	KMapPos	Pos;

	GetMpsPos(&nX, &nY);
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);
	
	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);

	int nObj;
	KObjItemInfo sInfo;
	sInfo.m_nItemID = nIdx;
	sInfo.m_nItemWidth = Item[nIdx].GetWidth();
	sInfo.m_nItemHeight = Item[nIdx].GetHeight();
	sInfo.m_nMoneyNum = 0;
	strcpy(sInfo.m_szName, Item[nIdx].GetName());

	if (Item[nIdx].m_SpecialParam.uItemType == 1)
	sInfo.m_nColorID = 1;
	else if (Item[nIdx].m_SpecialParam.uItemType == 2)
	sInfo.m_nColorID = 3;
	else if (Item[nIdx].IsItemMagic())
	sInfo.m_nColorID = 2;
	else
    sInfo.m_nColorID = 0;


	sInfo.m_nMovieFlag = 1;
	sInfo.m_nSoundFlag = 1;
	nObj = ObjSet.Add(Item[nIdx].GetObjIdx(), Pos, sInfo);
	if (nObj < 0)
	{
		ItemSet.m_checkdel = 10;
		ItemSet.Remove(nIdx);
	}
	else
	{
		if (nBelongPlayer > 0)
			Object[nObj].SetItemBelong(nBelongPlayer);
		else
			Object[nObj].SetItemBelong(-1);

		Object[nObj].m_bByPlayer = FALSE;
	}
}

void KNpc::Revive()
{
	
	//RestoreLiveData();
	RestoreNpcBaseInfo();
	int nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[m_SubWorldIndex].Mps2Map(m_OriginX, m_OriginY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	m_RegionIndex = nRegion;
	m_MapX = nMapX;
	m_MapY = nMapY;
	m_MapZ = 0;
	m_OffX = nOffX;
	m_OffY = nOffY;

if (m_Kind != kind_player && m_Kind != kind_dialoger)
{
	if (m_TypeNpc == 0)
	{
	int RamDom = g_Random(NPC_RANDOM_BOSS);
	if (RamDom < m_RateBoss)
	{
	m_CurrentFireResistMax	= m_FireResistMax;
	m_CurrentColdResistMax	= m_ColdResistMax;
	m_CurrentPoisonResistMax = m_PoisonResistMax;
	m_CurrentLightResistMax	= m_LightResistMax;
	m_CurrentPhysicsResistMax  = m_PhysicsResistMax;
	m_CurrentFireResist		= m_FireResist;
	m_CurrentColdResist		= m_ColdResist;
	m_CurrentPoisonResist	= m_PoisonResist;
	m_CurrentLightResist	= m_LightResist;
	m_CurrentPhysicsResist	= m_PhysicsResist;
	m_CurrentTreasure = m_Treasure * 20 * 7;
	m_CurrentExperience = m_Experience * 20;
	m_nCurrentShareDame = (m_nShareDame + 1) * 20 - 1;
	m_nCurentPencentDameAdd = (m_nPencentDameAdd + 100) * 2 - 100;
	m_IsBoss = 1;
	}
	else
	{
	m_CurrentFireResistMax	= m_FireResistMax;
	m_CurrentColdResistMax	= m_ColdResistMax;
	m_CurrentPoisonResistMax = m_PoisonResistMax;
	m_CurrentLightResistMax	= m_LightResistMax;
	m_CurrentPhysicsResistMax  = m_PhysicsResistMax;
	m_CurrentFireResist		= m_FireResist;
	m_CurrentColdResist		= m_ColdResist;
	m_CurrentPoisonResist	= m_PoisonResist;
	m_CurrentLightResist	= m_LightResist;
	m_CurrentPhysicsResist	= m_PhysicsResist;
	m_CurrentTreasure       = m_Treasure;
	m_CurrentExperience = m_Experience;
	m_nCurrentShareDame = m_nShareDame;
	m_nCurentPencentDameAdd = m_nPencentDameAdd;
    m_IsBoss = 0;
	}
	}
}

	if (m_RegionIndex < 0)
		return;
	SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);
#ifdef _SERVER
	//SubWorld[m_SubWorldIndex].m_WorldMessage.Send(GWM_NPC_CHANGE_REGION, VOID_REGION, nRegion, m_Index);
	SubWorld[m_SubWorldIndex].NpcChangeRegion(VOID_REGION, nRegion, m_Index);	// spe 03/06/28
#else
	SubWorld[0].NpcChangeRegion(VOID_REGION, SubWorld[0].m_Region[nRegion].m_RegionID, m_Index);
	m_nAutoNoActack = FALSE;
#endif
	DoStand();
	m_ProcessAI = 1;
	m_ProcessState = 1;
	m_AiAddLifeTime = 0;

}

void KNpc::RestoreLiveData()
{

}
#endif

#ifdef	_SERVER
// 向周围九屏广播
void	KNpc::SendDataToNearRegion(void* pBuffer, DWORD dwSize)
{
	_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].BroadCast(pBuffer, dwSize, nMaxCount, m_MapX, m_MapY);
	for (int i= 0; i < 8; i++)
	{
		if (SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].m_nConnectRegion[i] < 0)
			continue;
		SubWorld[m_SubWorldIndex].m_Region[SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].m_nConnectRegion[i]].BroadCast(pBuffer, dwSize, nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
}
#endif

#ifdef	_SERVER
//-----------------------------------------------------------------------------
//	功能：死亡时候计算PK值
//-----------------------------------------------------------------------------
int		KNpc::DeathCalcPKValue(int nKiller)
{


	SYSTEMTIME TimeNow;
	GetLocalTime(&TimeNow);
	
	int Map = SubWorld[Npc[nKiller].m_SubWorldIndex].m_SubWorldID;

	
	if (nKiller <= 0 || nKiller >= MAX_NPC)
		return enumDEATH_MODE_NPC_KILL;
	
	if (!Npc[nKiller].m_dwID)
		return enumDEATH_MODE_PLAYER_NO_PUNISH;
	
	if (m_nCurPKPunishState == enumDEATH_MODE_PKBATTLE_PUNISH)
	{
		return enumDEATH_MODE_PKBATTLE_PUNISH;
	}
	// 玩家之间，城镇内
	if (this->m_Kind != kind_player || Npc[nKiller].m_Kind != kind_player || !m_FightMode)
	{
		return enumDEATH_MODE_NPC_KILL;
	}
	// 如果是切磋，不计算
	if (Player[m_nPlayerIdx].m_cPK.GetExercisePKAim() == Npc[nKiller].m_nPlayerIdx)
	{
		return enumDEATH_MODE_PLAYER_NO_PUNISH;
	}
	// 如果是仇杀
	if (Player[m_nPlayerIdx].m_cPK.GetEnmityPKState() == enumPK_ENMITY_STATE_PKING &&
		Player[m_nPlayerIdx].m_cPK.GetEnmityPKAim() == Npc[nKiller].m_nPlayerIdx)
	{
		printf("Test 5 \n");
		
		//if (Player[Npc[nKiller].m_nPlayerIdx].m_cPK.IsEnmityPKLauncher() && Player[m_nPlayerIdx].m_cPK.GetPKValue() < MAX_DEATH_PUNISH_PK_VALUE)
		if (Player[Npc[nKiller].m_nPlayerIdx].m_cPK.IsEnmityPKLauncher())
		{
			Player[Npc[nKiller].m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nEnmityAddPKValue);
			printf("Test 6 \n");
			if (Player[m_nPlayerIdx].m_nExp > (-Player[m_nPlayerIdx].m_nNextLevelExp/2))
			{
				return enumDEATH_MODE_PKPLAYER_PUNISH;
			}
		}
		return enumDEATH_MODE_PLAYER_PUNISH;
	}
	// 如果等级差过大，算PK新手(HLv:LLv >= 3:2)

		//if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 0 && Player[m_nPlayerIdx].m_cPK.GetPKValue() < MAX_DEATH_PUNISH_PK_VALUE)

			//if (Npc[nKiller].m_CurrentCamp == camp_free)
			//{
			//	Player[Npc[nKiller].m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nKillerPKFactionAddPKValue);
			//	printf("Den
			//}
			if (Player[Npc[nKiller].m_nPlayerIdx].m_cPK.GetNormalPKState() == 2 && Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 0)
			{
				Player[Npc[nKiller].m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nFactionPKFactionAddPKValue);
			}
		//if (Player[m_nPlayerIdx].m_cPK.GetPKValue() <= MAX_DEATH_PUNISH_PK_VALUE/2)
			if (Player[m_nPlayerIdx].m_nExp > (-Player[m_nPlayerIdx].m_nNextLevelExp/2) && Player[m_nPlayerIdx].m_cPK.GetPKValue() > 5) 
			{
					return enumDEATH_MODE_PKPLAYER_PUNISH;
			}

	
	return enumDEATH_MODE_PLAYER_PUNISH;
}
#endif

#ifdef	_SERVER
//-----------------------------------------------------------------------------
//	功能：查找周围9个Region中是否有指定的 player
//-----------------------------------------------------------------------------
BOOL	KNpc::CheckPlayerAround(int nPlayerIdx)
{
	if (nPlayerIdx <= 0 || m_RegionIndex < 0)
		return FALSE;
	if (SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].CheckPlayerIn(nPlayerIdx))
		return TRUE;
	int		nRegionNo;
	for (int i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		if (SubWorld[m_SubWorldIndex].m_Region[nRegionNo].CheckPlayerIn(nPlayerIdx))
			return TRUE;
	}
	return FALSE;
}
int	KNpc::FindAroundPlayer(const char* Name)
{
	int nNpc = 0;
	if (Name[0] <= 0 || m_RegionIndex < 0)
		return nNpc;
	nNpc = SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].SearchNpcName(Name);
	if (nNpc)
		return nNpc;
	int		nRegionNo;
	for (int i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nNpc = SubWorld[m_SubWorldIndex].m_Region[nRegionNo].SearchNpcName(Name);
		if (nNpc)
			return nNpc;
	}
	return nNpc;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	功能：设定头顶状态
//-------------------------------------------------------------------------
void	KNpc::SetMenuState(int nState, char *lpszSentence, int nLength)
{
	this->m_DataRes.SetMenuState(nState, lpszSentence, nLength);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	功能：获得头顶状态
//-------------------------------------------------------------------------
int		KNpc::GetMenuState()
{
	return this->m_DataRes.GetMenuState();
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	功能：查找周围9个Region中是否有指定 ID 的 npc
//-------------------------------------------------------------------------
DWORD	KNpc::SearchAroundID(DWORD dwID)
{
	int		nIdx, nRegionNo;
	nIdx = SubWorld[0].m_Region[m_RegionIndex].SearchNpc(dwID);
	if (nIdx)
		return nIdx;
	for (int i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[0].m_Region[m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nIdx = SubWorld[0].m_Region[nRegionNo].SearchNpc(dwID);
		if (nIdx)
			return nIdx;
	}
	return 0;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	功能：设定特殊的只播放一遍的随身spr文件
//-------------------------------------------------------------------------
void	KNpc::SetSpecialSpr(char *lpszSprName)
{
	m_DataRes.SetSpecialSpr(lpszSprName);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	功能：设定瞬间特效
//-------------------------------------------------------------------------
void	KNpc::SetInstantSpr(int nNo)
{
	char	szName[FILE_NAME_LENGTH];
	szName[0] = 0;
	NpcSet.m_cInstantSpecial.GetSprName(nNo, szName, sizeof(szName));
	if (szName[0])
		this->SetSpecialSpr(szName);
}
#endif

#ifndef _SERVER
int		KNpc::GetNormalNpcStandDir(int nFrame)
{
	return m_DataRes.GetNormalNpcStandDir(nFrame);
}
#endif

#ifdef _SERVER
//重新更新角色状态信息数据
void	KNpc::UpdateNpcStateInfo()
{
	int i = 0;
	memset(m_btStateInfo, 0 ,sizeof(BYTE) * MAX_NPC_RECORDER_STATE);
	KStateNode *pNode = (KStateNode*)m_StateSkillList.GetTail();

	if (m_ActiveAuraID)
	{
		int nLevel = m_SkillList.GetCurrentLevel(m_ActiveAuraID);
		if (nLevel > 0)
		{
			int nSpecialID = 0;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_ActiveAuraID, nLevel);
			if (pOrdinSkill)
            {
				if (nSpecialID = pOrdinSkill->GetStateSpecailId())
					m_btStateInfo[i++] = nSpecialID;
            }
		}
	}

	while ( pNode && i < MAX_NPC_RECORDER_STATE)
	{
		if (pNode->m_StateGraphics > 0)
			m_btStateInfo[i++] = pNode->m_StateGraphics;
		pNode = (KStateNode*)pNode->GetPrev();
	}

}

//广播状态数据
void	KNpc::BroadCastState()
{
	
	if (m_RegionIndex < 0)
		return;

	NPC_SYNC_STATEINFO StateInfo;
	StateInfo.ProtocolType	= (BYTE)s2c_syncnpcstate;
	StateInfo.m_ID			= m_dwID;
	int i;
	for (i = 0; i < MAX_NPC_RECORDER_STATE; i++)
	{
		StateInfo.m_btStateInfo[i] = m_btStateInfo[i];
	
	}
    

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&StateInfo, sizeof(NPC_SYNC_STATEINFO), nMaxCount, m_MapX, m_MapY);
	for (i= 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&StateInfo, sizeof(NPC_SYNC_STATEINFO), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
}
#endif

#ifndef _SERVER
void	KNpc::SetNpcState(BYTE* pNpcState)
{
	ClearNpcState();
	if (!pNpcState)
		return ;
	for (int i = 0; i < MAX_NPC_RECORDER_STATE; i++)
	{
		if (*(pNpcState + i) != 0)
		{
			KStateNode * pNewNode = new KStateNode;
			pNewNode->m_StateGraphics = *(pNpcState + i);
			pNewNode->m_LeftTime = -1;
			m_StateSkillList.AddTail(pNewNode);
		}
	}

}
#endif
void	KNpc::ClearNpcState()
{
	KStateNode * pNode = (KStateNode*)m_StateSkillList.GetHead();
	KStateNode * pTempNode = NULL;
	
	while(pNode)
	{
		pTempNode = pNode;
		pNode = (KStateNode*) pNode->GetNext();
		pTempNode->Remove();
		delete pTempNode;
	}
	return;
}


void	KNpc::RestoreNpcBaseInfo()
{

	m_CurrentCamp = m_Camp;
	m_ActiveSkillID = 0;
	m_ActiveAuraID = 0;

	m_nPeopleIdx = 0;


#ifndef _SERVER	
	m_nPeopleIdxCheckClient = 0;
#endif



	m_nLastDamageIdx = 0;
	m_nLastPoisonDamageIdx = 0;
	m_nObjectIdx = 0;

	m_CurrentLife			= m_LifeMax;
	m_CurrentLifeMax		= m_LifeMax;
	m_CurrentLifeReplenish	= m_LifeReplenish;
	m_CurrentMana			= m_ManaMax;
	m_CurrentManaMax		= m_ManaMax;
	m_CurrentManaReplenish	= m_ManaReplenish;
	m_CurrentStamina		= m_StaminaMax;
	m_CurrentStaminaMax		= m_StaminaMax;
	m_CurrentStaminaGain	= m_StaminaGain;
	m_CurrentStaminaLoss	= m_StaminaLoss;

	memset(&m_CurrentFireDamage, 0, sizeof(m_CurrentFireDamage));
	memset(&m_CurrentColdDamage, 0, sizeof(m_CurrentColdDamage));
	memset(&m_CurrentLightDamage, 0, sizeof(m_CurrentLightDamage));
	memset(&m_CurrentPoisonDamage, 0, sizeof(m_CurrentPoisonDamage));
	memset(&m_CurrentPoisonMagic, 0, sizeof(m_CurrentPoisonMagic));
	memset(&m_CurrentFireMagic, 0, sizeof(m_CurrentFireMagic));
	memset(&m_CurrentLightMagic, 0, sizeof(m_CurrentLightMagic));

	memset(&m_CurrentFireDamage2, 0, sizeof(m_CurrentFireDamage2));
	memset(&m_CurrentColdDamage2, 0, sizeof(m_CurrentColdDamage2));
	memset(&m_CurrentLightDamage2, 0, sizeof(m_CurrentLightDamage2));
	memset(&m_CurrentPoisonDamage2, 0, sizeof(m_CurrentPoisonDamage2));
	memset(&m_CurrentPhysicsDamage2, 0, sizeof(m_CurrentPhysicsDamage2));
	memset(&m_CurrentColdMagic, 0, sizeof(m_CurrentColdMagic));
	

	m_CurrentAttackRating	= m_AttackRating;
	m_CurrentDefend			= m_Defend;

	m_CurrentFireResist		= m_FireResist;
	m_CurrentColdResist		= m_ColdResist;
	m_CurrentPoisonResist	= m_PoisonResist;
	m_CurrentLightResist	= m_LightResist;
	m_CurrentPhysicsResist	= m_PhysicsResist;
	m_CurrentFireResistMax	= m_FireResistMax;
	m_CurrentColdResistMax	= m_ColdResistMax;
	m_CurrentPoisonResistMax = m_PoisonResistMax;
	m_CurrentLightResistMax	= m_LightResistMax;
	m_CurrentPhysicsResistMax  = m_PhysicsResistMax;

	m_CurrentWalkSpeed		= m_WalkSpeed;
	m_CurrentRunSpeed		= m_RunSpeed;

	m_CurrentRWSpeed        = 0;

	m_CurrentAttackSpeed	= m_AttackSpeed;
	m_CurrentCastSpeed		= m_CastSpeed;
	m_CurrentVisionRadius	= m_VisionRadius;
	m_CurrentActiveRadius	= m_ActiveRadius;
	m_CurrentHitRecover		= m_HitRecover;
	m_CurrentTreasure		= m_Treasure;

	m_CurrentDamage2Mana	= 0;
	m_CurrentManaPerEnemy	= 0;
	m_CurrentLifeStolen		= 0;
	m_CurrentManaStolen		= 0;
	m_CurrentStaminaStolen	= 0;
	m_CurrentKnockBack		= 0;	
	m_CurrentDeadlyStrike	= 0;
	m_CurrentBlindEnemy		= 0;
	m_CurrentPiercePercent	= 0;
	m_CurrentFreezeTimeReducePercent	= 0;
	m_CurrentPoisonTimeReducePercent	= 0;
	m_CurrentStunTimeReducePercent		= 0;
	m_CurrentFireEnhance	= 0;
	m_CurrentColdEnhance	= 0;
	m_CurrentPoisonEnhance	= 0;
	m_CurrentLightEnhance	= 0;
	m_CurrentRangeEnhance	= 0;
	m_CurrentHandEnhance	= 0;

	m_CurrentMeleeDmgRet	= 0;
	m_CurrentRangeDmgRet    = 0;
	m_CurrentMeleeDmgRetPercent = 0;
	m_CurrentRangeDmgRetPercent = 0;
	m_CurrentPoisonDmgRetPercent = 0;
	m_CurrentLifeStolenAddPercent = 0;
	m_CurrentManaFromDamePercent = 0;
	m_CurrentManaShieldExPoint = 0;
	m_CurrentDmgRetPercentResist = 0;

	Player[m_nPlayerIdx].m_nCurLucky = Player[m_nPlayerIdx].m_nLucky;

	memset(&m_ManaShield, 0, sizeof(m_ManaShield));
	memset(&m_ManaShieldPoint, 0, sizeof(m_ManaShieldPoint));
	memset(&m_LightArmor, 0, sizeof(m_LightArmor));
	memset(&m_PhysicsArmor, 0, sizeof(m_PhysicsArmor));
	memset(&m_ColdArmor, 0, sizeof(m_ColdArmor));
	memset(&m_FireArmor, 0, sizeof(m_FireArmor));
	memset(&m_PoisonArmor, 0, sizeof(m_PoisonArmor));
	memset(&m_ReturnDoSkillEmty, 0, sizeof(m_ReturnDoSkillEmty));
	memset(&m_CurrentBuffEmtyRetPercent, 0, sizeof(m_CurrentBuffEmtyRetPercent));
	memset(&m_CurrentBuffEmtyResistPercent, 0, sizeof(m_CurrentBuffEmtyResistPercent));
	memset(&m_CurrentDoSkillEffecPercent, 0, sizeof(m_CurrentDoSkillEffecPercent));
	memset(&m_CurrentTimeCastSkillEffec, 0, sizeof(m_CurrentTimeCastSkillEffec));
	memset(&m_CurrentDoSkillEffecPercent1, 0, sizeof(m_CurrentDoSkillEffecPercent1));
	memset(&m_CurrentTimeCastSkillEffec1, 0, sizeof(m_CurrentTimeCastSkillEffec1));
	memset(&m_CurrentDoSkillEffecPercent2, 0, sizeof(m_CurrentDoSkillEffecPercent2));
	memset(&m_CurrentTimeCastSkillEffec2, 0, sizeof(m_CurrentTimeCastSkillEffec2));
	
	for (int i=0; i<MAX_SKILL; i++)
	{
	m_SkillList.m_CurrentListSkillAddLevel[i] = 0;
	}	


	ZeroMemory(m_CurrentMeleeEnhance, sizeof(m_CurrentMeleeEnhance));
	ClearStateSkillEffect();
	ClearNormalState();

}

#ifndef _SERVER
void KNpc::DrawBorder()
{
	if (m_Index <= 0)
		return;

	m_DataRes.DrawBorder();
}

int KNpc::DrawMenuState(int n)
{
	if (m_Index <= 0)
		return n;
	
	return m_DataRes.DrawMenuState(n);
}

void KNpc::DrawBlood()
{
	if (m_Kind != kind_normal)
		return;

	int nFontSize = 12;
//	KRUShadow	Blood;
//
//	char	szString[128];
//	strcpy(szString, Name);
//	if (m_FreezeState.nTime || m_PoisonState.nTime || m_ConfuseState.nTime || m_StunState.nTime)
//	{
//		strcat(szString, "(");
//		if (m_FreezeState.nTime)
//			strcat(szString, "冰");
//		if (m_PoisonState.nTime)
//			strcat(szString, "毒");
//		if (m_ConfuseState.nTime)
//			strcat(szString, "乱");
//		if (m_StunState.nTime)
//			strcat(szString, "晕");
//		strcat(szString, ")");
//	}
//
//	int nLength = g_StrLen(szString) * nFontSize / 2;
//	int nX = 400 - nLength / 2;
//	int nY = 28;
//	nLength += 40;
//
//	Blood.Color.Color_b.r = 130;
//	Blood.Color.Color_b.g = 30;
//	Blood.Color.Color_b.b = 30;
//	Blood.Color.Color_b.a = 10;
//	Blood.oPosition.nX = nX - 20;
//	Blood.oPosition.nY = nY;
//	Blood.oPosition.nZ = 0;
//	Blood.oEndPos.nX = Blood.oPosition.nX + nLength * m_CurrentLife / m_CurrentLifeMax;
//	Blood.oEndPos.nY = nY + 16;
//
//	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, TRUE);
//
//	Blood.Color.Color_b.r = 30;
//	Blood.oPosition.nX = Blood.oEndPos.nX;
//	Blood.oEndPos.nX = nX - 20 + nLength;
//
//	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, TRUE);
//
//	g_pRepresent->OutputText(nFontSize, szString, KRF_ZERO_END, nX, nY + 2, 0xffffffff);

	
	int nHeightOff = GetNpcPate();
	//if (NpcSet.CheckShowLife())
	{
		nHeightOff = PaintLife(nHeightOff, true);
		nHeightOff += SHOW_SPACE_HEIGHT;
	}

}
#endif

#ifdef _SERVER
int KNpc::SetPos(int nX, int nY)
{
	if (m_SubWorldIndex < 0)
	{
		_ASSERT(0);
		return 0;
	}
	int nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[m_SubWorldIndex].Mps2Map(nX, nY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

	if (nRegion < 0)
	{
		g_DebugLog("[Script]SetPos error:SubWorld:%d, Pos(%d, %d)", SubWorld[m_SubWorldIndex].m_SubWorldID, nX, nY);
		return 0;
	}

	int nOldRegion = m_RegionIndex;
	if (m_RegionIndex >= 0)
	{
		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
		
		NPC_REMOVE_SYNC	RemoveSync;
		RemoveSync.ProtocolType = s2c_npcremove;
		RemoveSync.ID = m_dwID;
		SendDataToNearRegion(&RemoveSync, sizeof(NPC_REMOVE_SYNC));
	}
	m_RegionIndex = nRegion;
	m_MapX = nMapX;
	m_MapY = nMapY;
	m_MapZ = 0;
	m_OffX = nOffX;
	m_OffY = nOffY;

	if (nOldRegion != nRegion)
	{
		SubWorld[m_SubWorldIndex].NpcChangeRegion(nOldRegion, nRegion, m_Index);
		if (IsPlayer())
			SubWorld[m_SubWorldIndex].PlayerChangeRegion(nOldRegion, nRegion, m_nPlayerIdx);
	}
	SubWorld[m_SubWorldIndex].m_Region[nRegion].AddRef(m_MapX, m_MapY, obj_npc);

	DoStand();
	m_ProcessAI = 1;
	m_ProcessState = 1;
	return 1;
}
#endif

#ifdef _SERVER
int KNpc::SetPosU(int nX, int nY)
{
	if (m_Doing == do_revive || m_Doing == do_death || !m_Index || !IsPlayer())
		return 0;

	if (m_SubWorldIndex < 0)
	{
		_ASSERT(0);
		return 0;
	}
	int nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[m_SubWorldIndex].Mps2Map(nX, nY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

	if (nRegion < 0)
	{
		g_DebugLog("[Script]SetPos error U:SubWorld:%d, Pos(%d, %d)", SubWorld[m_SubWorldIndex].m_SubWorldID, nX, nY);
		return 0;
	}

	int nOldRegion = m_RegionIndex;
	if (m_RegionIndex >= 0)
	{
		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
	}
	m_RegionIndex = nRegion;
	m_MapX = nMapX;
	m_MapY = nMapY;
	m_MapZ = 0;
	m_OffX = nOffX;
	m_OffY = nOffY;

	if (nOldRegion != nRegion)
	{
		SubWorld[m_SubWorldIndex].NpcChangeRegion(nOldRegion, nRegion, m_Index);
		if (IsPlayer())
			SubWorld[m_SubWorldIndex].PlayerChangeRegion(nOldRegion, nRegion, m_nPlayerIdx);
	}
	SubWorld[m_SubWorldIndex].m_Region[nRegion].AddRef(m_MapX, m_MapY, obj_npc);





	NPC_POS_SYNC NpcSync;
	int	nMpsX, nMpsY;

	GetMpsPos(&nMpsX, &nMpsY);

	NpcSync.ProtocolType = (BYTE)s2c_syncposmin;
	NpcSync.ID = m_dwID;
	NpcSync.MapX = nMpsX;
	NpcSync.MapY = nMpsY;
	NpcSync.Doing = (BYTE)m_Doing;


	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_PLAYER;
	CURREGION.BroadCast(&NpcSync, sizeof(NPC_POS_SYNC), nMaxCount, m_MapX, m_MapY);
	int j;
	for (j = 0; j < 8; j++)
	{
		int nConRegion = CURREGION.m_nConnectRegion[j];
		if (nConRegion == -1)
			continue;
		_ASSERT(m_SubWorldIndex >= 0 && nConRegion >= 0);
		SubWorld[m_SubWorldIndex].m_Region[nConRegion].BroadCast((BYTE*)&NpcSync, sizeof(NPC_POS_SYNC), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
	}

	DoStand();
	m_ProcessAI = 1;
	m_ProcessState = 1;
	return 1;
}
#endif













#ifdef _SERVER
int KNpc::ChangeWorld(DWORD dwSubWorldID, int nX, int nY, bool relogin)
{
	int nTargetSubWorld = g_SubWorldSet.SearchWorld(dwSubWorldID);

	if (!IsPlayer())
		return 0;
	
	if (-1 == nTargetSubWorld)
	{
		if (m_SubWorldIndex >= 0)
		{
		SubWorld[m_SubWorldIndex].m_MissionArray.RemovePlayer(m_nPlayerIdx);
		}
		TobeExchangeServer(dwSubWorldID, nX, nY);
		g_DebugLog("[Map]World%d haven't been loaded!", dwSubWorldID);

		return 2;	// 需要加切换服务器的处理 -- spe
	}
	if (IsPlayer())
	Player[m_nPlayerIdx].m_nPrePayMoney = 0;// 不是跨服务器，不用还钱
	
	if (relogin == false)
	{
		if (nTargetSubWorld == m_SubWorldIndex)
		{
			return SetPos(nX, nY);
		}
	}

	int nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[nTargetSubWorld].Mps2Map(nX, nY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

	if (nRegion < 0)
	{
		g_DebugLog("[Map]Change Pos(%d,%d) Invalid!", nX, nY);
		return 0;
	}
	
	if (m_SubWorldIndex >= 0 && m_RegionIndex >= 0)
	{
		SubWorld[m_SubWorldIndex].RemovePlayer(m_RegionIndex, m_nPlayerIdx);
		SubWorld[m_SubWorldIndex].m_MissionArray.RemovePlayer(m_nPlayerIdx);
		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].RemoveNpc(m_Index);// m_WorldMessage.Send(GWM_NPC_REMOVE, m_RegionIndex, m_Index);
		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);

		NPC_REMOVE_SYNC	RemoveSync;
		RemoveSync.ProtocolType = s2c_npcremove;
		RemoveSync.ID = m_dwID;
		SendDataToNearRegion(&RemoveSync, sizeof(NPC_REMOVE_SYNC));
	}

	//int nSourceSubWorld = m_SubWorldIndex;
	//int nSourceRegion = m_RegionIndex;

	m_SubWorldIndex = nTargetSubWorld;
	m_RegionIndex = nRegion;
	m_MapX = nMapX;
	m_MapY = nMapY;
	m_MapZ = 0;
	m_OffX = nOffX;
	m_OffY = nOffY;
	SubWorld[nTargetSubWorld].m_Region[nRegion].AddNpc(m_Index);// m_WorldMessage.Send(GWM_NPC_ADD, nRegion, m_Index);	
	SubWorld[nTargetSubWorld].m_Region[nRegion].AddRef(m_MapX, m_MapY, obj_npc);
	
	DoStand();	
	m_ProcessAI = 1;

	if (IsPlayer())
	{
		SubWorld[nTargetSubWorld].SendSyncData(m_Index, Player[m_nPlayerIdx].m_nNetConnectIdx);
		SubWorld[nTargetSubWorld].AddPlayer(nRegion, m_nPlayerIdx);
	}
	return 1;
}
#endif

#ifdef _SERVER
void KNpc::TobeExchangeServer(DWORD dwMapID, int nX, int nY)
{
	if (!IsPlayer())
	{
		return;
	}

	m_OldFightMode = m_FightMode;
	m_bExchangeServer = TRUE;
	if (m_nPlayerIdx > 0 && m_nPlayerIdx <= MAX_PLAYER)
	{
		Player[m_nPlayerIdx].TobeExchangeServer(dwMapID, nX, nY);
	}
}
#endif

BOOL KNpc::IsPlayer()
{
#ifdef _SERVER
	return m_Kind == kind_player;
#else
	return m_Index == Player[CLIENT_PLAYER_INDEX].m_nIndex;
#endif
}

// 清除NPC身上的非被动类的技能状态
void KNpc::ClearStateSkillEffect()
{
	KStateNode* pNode;
	pNode = (KStateNode *)m_StateSkillList.GetHead();
	while(pNode)
	{
		KStateNode* pTempNode = pNode;
		pNode = (KStateNode *)pNode->GetNext();

		if (pTempNode->m_LeftTime == -1)	// 被动技能
			continue;

		if (pTempNode->m_LeftTime > 0)
		{
			for (int i = 0; i < MAX_SKILL_STATE; i++)
			{
				if (pTempNode->m_State[i].nAttribType)
					ModifyAttrib(m_Index, &pTempNode->m_State[i]);
			}
			_ASSERT(pTempNode != NULL);
			pTempNode->Remove();
			delete pTempNode;
#ifdef _SERVER
			UpdateNpcStateInfo();
			BroadCastState();
#endif
			pTempNode = NULL;
			continue;
		}
	}
}

void KNpc::ClearNormalState()
{
	ZeroMemory(&m_PoisonState, sizeof(m_PoisonState));
	ZeroMemory(&m_FreezeState, sizeof(m_FreezeState));
	ZeroMemory(&m_BurnState, sizeof(m_BurnState));
	ZeroMemory(&m_ConfuseState, sizeof(m_ConfuseState));
	ZeroMemory(&m_StunState, sizeof(m_StunState));
	ZeroMemory(&m_LifeState, sizeof(m_LifeState));
	ZeroMemory(&m_ManaState, sizeof(m_ManaState));
	ZeroMemory(&m_DrunkState, sizeof(m_DrunkState));
	ZeroMemory(&m_InvisibilityState, sizeof(m_InvisibilityState));
	ZeroMemory(&m_NstatusState, sizeof(m_NstatusState));
	ZeroMemory(&m_MadnessState, sizeof(m_MadnessState));
}

void KNpc::CheckTrap()
{
	if (m_Kind != kind_player)
		return;
	
	if (m_Index <= 0)
		return;

	if (m_SubWorldIndex < 0 || m_RegionIndex < 0)
		return;

	DWORD	dwTrap = SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].GetTrap(m_MapX, m_MapY);
	if (m_TrapScriptID == dwTrap)
	{
		return;
	}
	else
	{
		m_TrapScriptID = dwTrap;
	}

	if (!m_TrapScriptID)
	{
		return;
	}

	Player[m_nPlayerIdx].ExecuteScript(m_TrapScriptID, "main", 0);
}

void KNpc::SetFightMode(BOOL bFightMode)
{
	//g_DebugLog("[DEATH]SetMode:%d", bFightMode);
	m_FightMode = bFightMode;

	m_PoisonState.nTime = 0;

#ifdef _SERVER
	if (this->m_Kind == kind_player)
		Player[this->m_nPlayerIdx].m_cPK.CloseAll();
#endif
}

void KNpc::TurnTo(int nIdx)
{
	if (!Npc[nIdx].m_Index || !m_Index)
		return;

	int nX1, nY1, nX2, nY2;

	GetMpsPos(&nX1, &nY1);
	Npc[nIdx].GetMpsPos(&nX2, &nY2);

	m_Dir = g_GetDirIndex(nX1, nY1, nX2, nY2);
}

void KNpc::ReCalcStateEffect()
{
	KStateNode* pNode;
	pNode = (KStateNode *)m_StateSkillList.GetHead();
	while(pNode)
	{
		if (pNode->m_LeftTime != 0)	// 包括被动(-1)和主动(>0)
		{
			int i;
			for (i = 0; i < MAX_SKILL_STATE; i++)
			{
				if (pNode->m_State[i].nAttribType)
				{
					KMagicAttrib	MagicAttrib;
					MagicAttrib.nAttribType = pNode->m_State[i].nAttribType;
					MagicAttrib.nValue[0] = -pNode->m_State[i].nValue[0];
					MagicAttrib.nValue[1] = -pNode->m_State[i].nValue[1];
					MagicAttrib.nValue[2] = -pNode->m_State[i].nValue[2];
					ModifyAttrib(m_Index, &MagicAttrib);
				}
			}
		}
		pNode = (KStateNode *)pNode->GetNext();
	}
}

#ifndef _SERVER
extern KTabFile g_ClientWeaponSkillTabFile;
#endif

int		KNpc::GetCurActiveWeaponSkill()
{
	int nSkillId = 0;
	if (IsPlayer())
	{
		
		int nDetailType = Player[m_nPlayerIdx].m_ItemList.GetWeaponType();
		int nParticularType = Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular();
		
		//近身武器
		if (nDetailType == 0)
		{
			nSkillId = g_nMeleeWeaponSkill[nParticularType];
		}//远程武器
		else if (nDetailType == 1)
		{
			nSkillId = g_nRangeWeaponSkill[nParticularType];
		}//空手
		else if (nDetailType == -1)
		{
			nSkillId = g_nHandSkill;
		}
	}
	else
	{
#ifdef _SERVER
		//Real Npc
		return 0;
#else
		if (m_Kind == kind_player) // No Local Player
		{
			g_ClientWeaponSkillTabFile.GetInteger(m_WeaponType + 1, "SkillId", 0, &nSkillId);
		}
		else						//Real Npc
		{
			return 0;//
		}
#endif
	}
	return nSkillId;
}

#ifndef _SERVER



void	KNpc::AutoFixXY()
{

if (Player[CLIENT_PLAYER_INDEX].m_nIndex != m_Index)
{


if ((m_sSyncPos.m_nDoing == do_stand
	|| m_sSyncPos.m_nDoing == do_magic
	|| m_sSyncPos.m_nDoing == do_attack
	|| m_sSyncPos.m_nDoing == do_runattack
	|| m_sSyncPos.m_nDoing == do_manyattack
	|| m_sSyncPos.m_nDoing == do_jumpattack
	|| m_sSyncPos.m_nDoing == do_goattack
	) 
	&& (m_Doing == do_run || m_Doing == do_walk))
{	
	int	nRegionIdx;

		if ((DWORD)SubWorld[0].m_Region[m_RegionIndex].m_RegionID == m_sSyncPos.m_dwRegionID)
		{
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
			SubWorld[0].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);
		}
		else
		{
			nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
			if (nRegionIdx < 0)
				return;
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
			m_RegionIndex = nRegionIdx;
			m_dwRegionID = m_sSyncPos.m_dwRegionID;
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
		}
}

}

else if (m_Doing == do_sit)
{



int	nRegionIdx;

		if ((DWORD)SubWorld[0].m_Region[m_RegionIndex].m_RegionID == m_sSyncPos.m_dwRegionID)
		{
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
			SubWorld[0].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);
		}
		else
		{
			nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
			if (nRegionIdx < 0)
				return;
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
			m_RegionIndex = nRegionIdx;
			m_dwRegionID = m_sSyncPos.m_dwRegionID;
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
		}


}

}



void	KNpc::HurtAutoMove()
{
	

	if (this->m_Doing != do_hurt)
		return;
	
	if (m_sSyncPos.m_nDoing != do_hurt && m_sSyncPos.m_nDoing != do_stand)
		return;

	int	nFrames, nRegionIdx;

	nFrames = m_Frames.nTotalFrame - m_Frames.nCurrentFrame;
	if (nFrames <= 1)
	{
		if ((DWORD)SubWorld[0].m_Region[m_RegionIndex].m_RegionID == m_sSyncPos.m_dwRegionID)
		{
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
			SubWorld[0].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);
		}
		else
		{
			nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
			if (nRegionIdx < 0)
				return;
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
			m_RegionIndex = nRegionIdx;
			m_dwRegionID = m_sSyncPos.m_dwRegionID;
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
		}
	}
	else
	{
		nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
		if (nRegionIdx < 0)
			return;
		int		nNpcX, nNpcY, nSyncX, nSyncY;
		int		nNewX, nNewY, nMapX, nMapY, nOffX, nOffY;
		SubWorld[0].Map2Mps(m_RegionIndex, 
			m_MapX, m_MapY,
			m_OffX, m_OffY,
			&nNpcX, &nNpcY);
		SubWorld[0].Map2Mps(nRegionIdx, 
			m_sSyncPos.m_nMapX, m_sSyncPos.m_nMapY,
			m_sSyncPos.m_nOffX, m_sSyncPos.m_nOffY,
			&nSyncX, &nSyncY);
		nNewX = nNpcX + (nSyncX - nNpcX) / nFrames;
		nNewY = nNpcY + (nSyncY - nNpcY) / nFrames;
		SubWorld[0].Mps2Map(nNewX, nNewY, &nRegionIdx, &nMapX, &nMapY, &nOffX, &nOffY);
		_ASSERT(nRegionIdx >= 0);
		if (nRegionIdx < 0)
			return;
		if (nRegionIdx != m_RegionIndex)
		{
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
			m_RegionIndex = nRegionIdx;
			m_dwRegionID = m_sSyncPos.m_dwRegionID;
			m_MapX = nMapX;
			m_MapY = nMapY;
			m_OffX = nOffX;
			m_OffY = nOffY;
		}
		else
		{
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			m_MapX = nMapX;
			m_MapY = nMapY;
			m_OffX = nOffX;
			m_OffY = nOffY;
			SubWorld[0].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);
		}
	}
}

#endif

#ifndef _SERVER
void KNpc::ProcNetCommand(NPCCMD cmd, int x /* = 0 */, int y /* = 0 */, int z /* = 0 */)
{
	switch (cmd)
	{
	case do_death:
		DoDeath();
		break;
	case do_hurt:
		DoHurt(x, y, z);
		break;
	case do_revive:
		DoStand();
		m_ProcessAI = 1;
		m_ProcessState = 1;
		SetInstantSpr(enumINSTANT_STATE_REVIVE);
		break;
	case do_stand:
		DoStand();
		m_ProcessAI = 1;
		m_ProcessState = 1;
	default:
		break;
	}
}
#endif

#ifndef _SERVER
void	KNpc::ClearBlood(int nid)
{
	m_nBloodNo[nid]		= 0;
	m_nBloodAlpha[nid]	= 0;
	m_nBloodTime[nid]	= 0;
	m_szBloodNo[nid][0]	= 0;
}
#endif

#ifndef _SERVER
void	KNpc::SetBlood(int nNo)
{
	if (nNo <= 0)
		return;
    int id = 0;
	for (int i=0;i<MAX_FLOOD_SHOW;i++)
	{
	if (!m_szBloodNo[i][0])
	{
    id = i;
	break;
	}
    }

	int nLifeBlood;

	if (m_nCurrentShareDame > 0)
	{
	nLifeBlood = nNo * (m_nCurrentShareDame + 1) - g_Random(m_nCurrentShareDame + 1);
	}
	else
	{
	nLifeBlood = nNo;
	}

	m_nBloodNo[id]		= nLifeBlood;
	m_nBloodAlpha[id]	= 0;
	m_nBloodTime[id]	= defMAX_SHOW_BLOOD_TIME;
	sprintf(m_szBloodNo[id], "%d", nLifeBlood);
}
#endif

#ifndef _SERVER
BOOL KNpc::FindStateSkill(int nID)
{
	if (nID == 0)
		return FALSE;
	KStateNode* pNode;
	pNode = (KStateNode *)m_StateSkillList.GetTail();
	while(pNode)
	{
		if (pNode->m_SkillID == nID)
		{
			return TRUE;
		}
		pNode = (KStateNode *)pNode->GetPrev();
	}
	return FALSE;
}
int	KNpc::PaintBlood(int nHeightOffset)
{
	for (int i=0;i<MAX_FLOOD_SHOW;i++)
	{

	if (m_szBloodNo[i][0])
	{
	int	nHeightOff = nHeightOffset + (defMAX_SHOW_BLOOD_TIME - m_nBloodTime[i]) * defSHOW_BLOOD_MOVE_SPEED/3;
	int nFontSize = 16;
	DWORD	dwColor = SHOW_BLOOD_COLOR | (m_nBloodAlpha[i] << 24);
	int		nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);

	if (m_InvisibilityState.nTime <= 0 || m_nPlayerIdx == CLIENT_PLAYER_INDEX)	
	g_pRepresent->OutputText(nFontSize, m_szBloodNo[i], KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(m_szBloodNo[i]) / 4, nMpsY, dwColor, 0, nHeightOff);
    else if (m_InvisibilityState.nTime > 0 && m_Kind == kind_player && m_nPlayerIdx != CLIENT_PLAYER_INDEX)
	{
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag && ((DWORD)g_Team[0].m_nCaptain == m_dwID || g_Team[0].FindMemberID(m_dwID) >= 0))
	g_pRepresent->OutputText(nFontSize, m_szBloodNo[i], KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(m_szBloodNo[i]) / 4, nMpsY, dwColor, 0, nHeightOff);
	}

	m_nBloodTime[i]--;
	if (m_nBloodTime[i] <= 0)
	{
		ClearBlood(i);
	}

	if (m_nBloodTime[i]%3)
	{
	m_nBloodAlpha[i]++;
	}

	if (m_nBloodAlpha[i] > 31)
		m_nBloodAlpha[i] = 31;
	}

    }
	return nHeightOffset;
}
#endif

#ifndef _SERVER
int	KNpc::GetNpcPate()
{
	int nHeight = m_Height + m_nStature;
	if (m_Kind == kind_player)
	{
		if (m_nSex)
			nHeight += 84;	//女
		else
			nHeight += 84;	//男

		if (!m_MaskType) // mask
		{
			if (m_Doing == do_sit && MulDiv(10, m_Frames.nCurrentFrame, m_Frames.nTotalFrame) >= 8)
				nHeight -= MulDiv(30, m_Frames.nCurrentFrame, m_Frames.nTotalFrame);
			
			if (m_bRideHorse)
				nHeight += 38;	//骑马
		}
	}

	return nHeight;
}
#endif

#ifndef _SERVER
int	KNpc::GetNpcPatePeopleInfo()
{
	int nFontSize = 12;
	if (m_nChatContentLen > 0 && m_nChatNumLine > 0)
		return m_nChatNumLine * (nFontSize + 1);

	int nHeight = 0;
	if (NpcSet.CheckShowLife())
	{
		if (m_Kind == kind_player ||
			m_Kind == kind_partner)
		{
			if (m_CurrentLifeMax > 0 &&
				(relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex))
				)
				nHeight += SHOW_LIFE_HEIGHT;
		}
	}
	if (NpcSet.CheckShowName())
	{
		if (nHeight != 0)
			nHeight += SHOW_SPACE_HEIGHT;//好看

		if (m_Kind == kind_player || m_Kind == kind_dialoger)
			nHeight += nFontSize + 1;
	}
	return nHeight;
}
#endif

void KNpc::GetFrameCopyFromTemplate(int nNpcTemplateId, int nLevel)
{
	if (nNpcTemplateId < 0 || nLevel < 1 ) 
		return ;
	
	if (g_pNpcTemplate[nNpcTemplateId][nLevel]) //数据有效则拷贝，否则重新生成
		LoadFrameFromTemplate(nNpcTemplateId, nLevel);
	else
	{
		if (!g_pNpcTemplate[nNpcTemplateId][0])
		{
			g_pNpcTemplate[nNpcTemplateId][0] = new KNpcTemplate;
			g_pNpcTemplate[nNpcTemplateId][0]->InitNpcBaseData(nNpcTemplateId);
			g_pNpcTemplate[nNpcTemplateId][0]->m_NpcSettingIdx = nNpcTemplateId;
			g_pNpcTemplate[nNpcTemplateId][0]->m_bHaveLoadedFromTemplate = TRUE;
		}
		KLuaScript * pLevelScript = NULL;		
		
#ifdef _SERVER
		pLevelScript = (KLuaScript*)g_GetScript(
			g_pNpcTemplate[nNpcTemplateId][0]->m_dwLevelSettingScript
			);
		
		if (pLevelScript == NULL)
			pLevelScript = g_pNpcLevelScript;
#else
		KLuaScript LevelScript;
		if (!g_pNpcTemplate[nNpcTemplateId][0]->m_szLevelSettingScript[0])
		{
			pLevelScript = g_pNpcLevelScript;
		}
		else
		{
			LevelScript.Init();
			if (!LevelScript.Load(g_pNpcTemplate[nNpcTemplateId][0]->m_szLevelSettingScript))
			{
				//g_DebugLog ("[Client][GetFrameCopyFromTemplate] nNpcTemplateId: %d. Can not read file %s", nNpcTemplateId, g_pNpcTemplate[nNpcTemplateId][0]->m_szLevelSettingScript);
				//_ASSERT(0);
				pLevelScript = g_pNpcLevelScript;
			}
			else
			{
				pLevelScript = &LevelScript;
			}
		}
		
#endif
		g_pNpcTemplate[nNpcTemplateId][nLevel] = new KNpcTemplate;
		*g_pNpcTemplate[nNpcTemplateId][nLevel] = *g_pNpcTemplate[nNpcTemplateId][0];
		g_pNpcTemplate[nNpcTemplateId][nLevel]->m_nLevel = nLevel;
		g_pNpcTemplate[nNpcTemplateId][nLevel]->InitNpcLevelData(&g_NpcKindFile, nNpcTemplateId, nLevel);
		g_pNpcTemplate[nNpcTemplateId][nLevel]->m_bHaveLoadedFromTemplate = TRUE;
		LoadFrameFromTemplate(nNpcTemplateId,nLevel);
	}
}

void	KNpc::ReSetRes(int nMark)
{
	
#ifndef _SERVER
	char	szNpcTypeName[32];
	
#endif
	if (nMark == 1)
	{
#ifndef _SERVER
		if (m_NpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID)
		{
			strcpy(szNpcTypeName, "男主角");
			m_StandFrame = NpcSet.GetPlayerStandFrame(TRUE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(TRUE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(TRUE);
		}
		else
		{
			strcpy(szNpcTypeName, "女主角");
			m_StandFrame = NpcSet.GetPlayerStandFrame(FALSE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(FALSE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(FALSE);
		}
#endif
		m_WalkSpeed = NpcSet.GetPlayerWalkSpeed();
		m_RunSpeed = NpcSet.GetPlayerRunSpeed();
		m_AttackFrame = NpcSet.GetPlayerAttackFrame();
		m_HurtFrame	= NpcSet.GetPlayerHurtFrame();
	}
	else
	{
		GetFrameCopyFromTemplate(m_MaskType, m_Level);
#ifndef _SERVER	
		g_NpcSetting.GetString(m_MaskType + 2, "NpcResType", "", szNpcTypeName, sizeof(szNpcTypeName));
		if (!szNpcTypeName[0])
		{
			g_NpcKindFile.GetString(2, "人物名称", "", szNpcTypeName, sizeof(szNpcTypeName));//如果没找到，用第一个npc代替
		}
#endif
	}
#ifndef _SERVER
	this->RemoveRes();
	m_DataRes.Init(szNpcTypeName, &g_NpcResList);
	m_DataRes.SetAction(m_ClientDoing);
	m_DataRes.SetRideHorse(m_bRideHorse);
	// fifong
	if (m_bFifongState == TRUE)
		m_DataRes.SetArmor(m_ArmorType, m_FifongType, TRUE);
	else
		m_DataRes.SetArmor(m_ArmorType, 0, FALSE);
	// end
	m_DataRes.SetHelm(m_HelmType);
	m_DataRes.SetHorse(m_HorseType);
	m_DataRes.SetWeapon(m_WeaponType);
#endif
}


void	KNpc::LoadFrameFromTemplate(int nNpcTemplateId, int nLevel)
{
	if (nNpcTemplateId < 0 )
	{
		g_DebugLog("载入Npc%d模板请求非法！", nNpcTemplateId);
		return ;
	}
	
	KNpcTemplate * pNpcTemp = g_pNpcTemplate[nNpcTemplateId][nLevel];

//	m_HeadImage =	pNpcTemp->m_HeadImage;
	m_CorpseSettingIdx =	pNpcTemp->m_CorpseSettingIdx;
	m_DeathFrame =	pNpcTemp->m_DeathFrame;
	m_WalkFrame =	pNpcTemp->m_WalkFrame;
	m_RunFrame =	pNpcTemp->m_RunFrame;
	m_HurtFrame =	pNpcTemp->m_HurtFrame;
	m_StandFrame =  pNpcTemp->m_StandFrame;
	m_StandFrame1 = pNpcTemp->m_StandFrame1;
#ifndef _SERVER
	m_ArmorType				= pNpcTemp->m_ArmorType;
	m_HelmType				= pNpcTemp->m_HelmType;
	m_WeaponType			= pNpcTemp->m_WeaponType;
#endif
}