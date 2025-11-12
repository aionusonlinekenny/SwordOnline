            // KSkills.cpp: implementation of the KSkills class.
//
//////////////////////////////////////////////////////////////////////
#include "KCore.h"
#ifdef _STANDALONE
#include "KSG_StringProcess.h"
#else
#include "../../Engine/Src/KSG_StringProcess.h"
#endif
#include "KSkills.h"
#include "KMissle.h"
#include "KMissleSet.h"
#include "KNpc.h"
#include "math.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "KMath.h"
#include "KEngine.h"
#include "KTabFile.h"
#include "KTabFileCtrl.h"
#include "KMissleMagicAttribsData.h"
#include "KPlayer.h"
#ifndef _SERVER
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "scene/KScenePlaceC.h"
#include "../../Represent/iRepresent/KRepresentUnit.h"
#include "imgref.h"
#include "KMagicDesc.h"
#include "KOption.h"
#endif

//#define SHOW_SKILL_MORE_INFO
#define	 NPCINDEXOFOBJECT 0 //物件发魔法时所对应的Npc编号
const char * g_MagicID2String(int nAttrib);
extern  const KScript * g_GetScript(DWORD dwScriptId);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*!*****************************************************************************
// Function		: KSkill::KSkill
// Purpose		: 
// Return		: 
// Comments		:
// Author		: RomanDou
*****************************************************************************/
KSkill::KSkill()
{
	m_nFlySkillId =  m_nCollideSkillId = m_nVanishedSkillId = 0;
	
    // add by FreewayChen in 2003.6.6
    m_nImmediateAttribsNum = m_nStateAttribsNum = m_nMissleAttribsNum = m_nDamageAttribsNum = 0;
	m_nSkillCostType = attrib_mana;
    m_nWaitTime = 0;
	m_nEquiptLimited = 0;
	m_bDoHurt = 0;

	for (int i=0;i<6;i++)
	{
	for (int j=0;j<2;j++)
	{
	m_addskillsdame[i][j] = 0;
	}
	}
    m_addskillmisslesrun = 0;

#ifndef _SERVER
	m_szSkillDesc[0] = 0;
	m_szManPreCastSoundFile[0] = 0;
	m_szFMPreCastSoundFile[0] = 0;
#else
	m_dwSkillLevelUpScriptID = 0;
	m_dwSkillLevelDataScriptId = 0;
#endif
	
}
/*!*****************************************************************************
// Function		: KSkill::~KSkill
// Purpose		: 
// Return		: 
// Comments		:
// Author		: RomanDou
*****************************************************************************/
KSkill::~KSkill()
{
	
}

/*!*****************************************************************************
// Function		: KSkill::Param2PCoordinate
// Purpose		: 
// Return		: 
// Argumant		: int nLauncher
// Argumant		: int nParam1
// Argumant		: int nParam2
// Argumant		: int nParam3
// Argumant		: int *npPX
// Argumant		: int *npPY
// Comments		:
// Author		: RomanDou
*****************************************************************************/
inline int	KSkill::Param2PCoordinate(int nLauncher, int nParam1, int nParam2 , int *npPX, int *npPY, eSkillLauncherType eLauncherType)  const 
{
	
	int nRegionId, nDesMapX, nDesMapY ;
	int nTargetId = -1;
	if (eLauncherType == SKILL_SLT_Obj) return 0;
	
	switch(nParam1)
	{
	case -1://nParam2 参数指向某个Npc，或Obj的Index
		nTargetId		= nParam2;
		nRegionId		= Npc[nParam2].m_RegionIndex;
		nDesMapX		= Npc[nParam2].m_MapX;
		nDesMapY		= Npc[nParam2].m_MapY;
		
		if (eLauncherType == SKILL_SLT_Npc)
			SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(nRegionId, nDesMapX , nDesMapY, Npc[nParam2].m_OffX , Npc[nParam2].m_OffY, npPX, npPY);
		else if(eLauncherType == SKILL_SLT_Obj)
			SubWorld[Object[nLauncher].m_nSubWorldID].Map2Mps(nRegionId, nDesMapX, nDesMapY, Object[nParam2].m_nOffX , Object[nParam2].m_nOffY, npPX, npPY);
		else;
		break;
		
	case -2://nParam 参数指向某个方向
		
		break;
	default://默认时, nParam1 与nParam2 为实际点坐标
		*npPX = nParam1;
		*npPY = nParam2;
		break;
	}
	
	/*if (*npPX < 0 || *npPY < 0)	
		g_DebugLog("Param2PCoordinate函数获得参数违法！nParam1 ,nParam2 [%d,%d], nPX,nPY", nParam1, nParam2, *npPX, * npPY)*/;
	
	return nTargetId;
}

BOOL KSkill::CanCastSkill(int nLauncher, int &nParam1, int &nParam2)  const 
{
	//对自已的主动辅助技能
	if (m_bTargetSelf && nParam1 != -1) 
	{
		nParam1 = -1;
		nParam2 = nLauncher;
		return TRUE;
	}
	else
	{
		if (m_bTargetOnly && nParam1 != -1) return FALSE;
		
		if (nParam1 == -1)
		{
			if ( nParam2 <= 0 || nParam2 >= MAX_NPC) return FALSE;
			NPC_RELATION  Relation = NpcSet.GetRelation(nLauncher, nParam2);
			
			if (m_bTargetEnemy)
			{
				if (Relation & relation_enemy) goto relationisvalid;
			}
			
			if (m_bTargetAlly)
			{
				if (Relation & relation_ally) goto relationisvalid;
			}
			
			if (m_bTargetSelf)
			{
				if (Relation & relation_self) goto relationisvalid;
			}
			return FALSE;
		}
		
	}
	
relationisvalid:

	if (Npc[nLauncher].IsPlayer())
	{
		if (IsPhysical())
		{
			int nWeapoinSkill = Npc[nLauncher].GetCurActiveWeaponSkill();
			if ((DWORD)nWeapoinSkill != m_nId)
			{
				return FALSE;
			}
		}

		//-2表示技能不受当前装备的限制,
		//-1表示空手限制
		//0-99受某种近身攻击类的装备限制 取值为该装备的具体类别号
		//100-199受某种远程攻击类的装备限制 取值为该装备的具体类别号 加100

		if (-2 != m_nEquiptLimited)
		{

#ifdef _SERVER
			int nPlayerIdx		= Npc[nLauncher].GetPlayerIdx();
#else
			int nPlayerIdx		= CLIENT_PLAYER_INDEX;
#endif
			int nDetailType		= Player[nPlayerIdx].m_ItemList.GetWeaponType();
			int nParticularType = Player[nPlayerIdx].m_ItemList.GetWeaponParticular();
			
			//近身武器
			if (nDetailType == 0)
			{
				
			}//远程武器
			else if (nDetailType == 1)
			{
				nParticularType += MAX_MELEEWEAPON_PARTICULARTYPE_NUM;
			}//空手
			else if (nDetailType == -1)
			{
				nParticularType = -1;
			}
			
			if (nParticularType != m_nEquiptLimited)
				return FALSE;
		}
		
		//0表示不限制
		//1表示不可以骑马发该技能
		//2表示必须骑马发该技能
		if (m_nHorseLimited)
		{
			switch(m_nHorseLimited)
			{
			case 1:
				{
					if (Npc[nLauncher].m_bRideHorse)
						return FALSE;
				}
				break;
			case 2:
				{
					if (!Npc[nLauncher].m_bRideHorse)
						return FALSE;
				}
				break;
			default:
				return FALSE;
			}
		}
		
		if ((m_bTargetOnly) && nParam1 == -1)
		{
#ifndef _SERVER
			int distance = NpcSet.GetDistance(nLauncher, nParam2);
			if (distance > GetAttackRadius() && GetAttackRadius() >= 200) return FALSE;
#endif


#ifdef _SERVER
			int distance = NpcSet.GetDistance(nLauncher, nParam2);
			if (distance > (GetAttackRadius() + 256) && GetAttackRadius() >= 200) return FALSE;
#endif
		}

		else if (nParam1 == -1)
		{

#ifdef _SERVER
			int distance = NpcSet.GetDistance(nLauncher, nParam2);
			if (distance > (GetAttackRadius() + 256) && GetAttackRadius() >= 200) return FALSE;
#endif
		}
		/*else
		{
		if (nParam1 < 0 || nParam2 < 0) return FALSE;
		
		  #ifndef _SERVER
		  int nLauncherPX = 0, nLauncherPY = 0;
		  Npc[nLauncher].GetMpsPos(&nLauncherPX, &nLauncherPY);
		  int ndistance = g_GetDistance(nLauncherPX, nLauncherPY, nParam1, nParam2);
		  if (ndistance > GetAttackRadius()) return FALSE;
		  #endif
		  }
		*/
	}
	return TRUE;
}

//		当玩家调用某个技能时发生 [5/28/2002]
//		客户端和服务器端在技能的调用方面有一些不同
//		服务器端一般收到的经过客户端处理完的参数
//		游戏世界以消息命令机制执行每个变化，因此对于发技能应该也是统一数据接口
//		客户端时，如果该消息是来自本机玩家的输入，则必须将其转换为实际的消息
//		来执行。同时还应将转换好的消息传给服务器端
/*
有关传的参数是MapX，还是PointX根据具体的魔法技能而定
比如一般飞行魔法为Map坐标，而定点魔法为Point坐标
*/
/*
注意当调用Cast时，必须已确保当前的nLauncherIndex与Socket相对应的dwId一致，即IsMatch()通过。
*/

/*!*****************************************************************************
// Function		: KSkill::Cast
// Purpose		: 发技能的统一接口
// Return		: 
// Argumant		: int nLauncher 发送者Id
// Argumant		: int nParam1   
// Argumant		: int nParam2
// Argumant		: int nWaitTime 发送的延迟时间
// Argumant		: eSkillLauncherType eLauncherType 发送者类型
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL	KSkill::Cast(int nLauncher, int nParam1, int nParam2, int nWaitTime, eSkillLauncherType eLauncherType)  const 
{
	//-----------------接口函数入口点，检测参数合法性-------------------------------
	
	if (nLauncher < 0 )
	{
		g_DebugLog("Skill::Cast(), nLauncher < 0 , Return False;"); 
		return FALSE; 
	}
	//检查发送者是否符合要求

	switch(eLauncherType)
	{
	case SKILL_SLT_Npc:
		{
			if (MAX_NPC <= nLauncher) return FALSE;
			if (Npc[nLauncher].m_dwID < 0) return FALSE;
			if (nParam1 == -1)
			{
				if (nParam2 >= MAX_NPC) 
					return FALSE;
				
				if (
					(Npc[nParam2].m_Index <= 0)
					|| Npc[nLauncher].m_SubWorldIndex != Npc[nParam2].m_SubWorldIndex
					)
					return FALSE;
			}
		}
		break;
		
	case SKILL_SLT_Obj:
		{
			return FALSE;
			if (MAX_OBJECT <= nLauncher) return FALSE;
			if (Object[nLauncher].m_nDataID < 0) return FALSE;
		}
		break;
	case SKILL_SLT_Missle:
		{
			if (MAX_MISSLE <= nLauncher) 
				return FALSE;
			
			if (Missle[nLauncher].m_nMissleId < 0) 
				return FALSE;
			
			if (nParam1 == -1)
			{
				if (nParam2 >= MAX_NPC) 
					return FALSE;
				
				if ((Npc[nParam2].m_Index <= 0) ||  Missle[nLauncher].m_nSubWorldId != Npc[nParam2].m_SubWorldIndex)
					return FALSE;
			}

		}
		break;
	default:
		{
			return FALSE;
		}
	}

	
	
	if (nParam1 < 0 && nParam2 < 0 ) 
		return FALSE;
	
	if (nWaitTime < 0 ) 
	{
		g_DebugLog("Call Skill::Cast(), nWaitTime < 0 "); 
		nWaitTime = 0;
	}
	
	//------------------------------------------------------------------------------
	
	switch(m_eSkillStyle)
	{
		
	case	SKILL_SS_Missles:				//发子弹
		{
			CastMissles(nLauncher, nParam1, nParam2, nWaitTime, eLauncherType);
		}
		break;
		
	case	SKILL_SS_Melee:
		{
			
			
		}break;
		
	case	SKILL_SS_InitiativeNpcState:	//改变角色的主动状态
		{
			CastInitiativeSkill(nLauncher, nParam1, nParam2, nWaitTime);
		}
		break;
		
	case	SKILL_SS_PassivityNpcState:	//改变角色的被动状态	
		{
			CastPassivitySkill(nLauncher, nParam1, nParam2, nWaitTime);
		}
		break;
		
	case	SKILL_SS_CreateNpc:			//产生新的Npc、怪物 
		{
			
		}
		break;
		
	case	SKILL_SS_BuildPoison:			//炼毒术
		{
			
		}
		break;
		
	case	SKILL_SS_AddPoison:			//加毒术
		{
			
		}
		break;
		
	case	SKILL_SS_GetObjDirectly:		//隔空取物	
		{
			
		}
		break;
		
	case	SKILL_SS_StrideObstacle:		//跨越障碍
		{
			
		}
		break;
		
	case	SKILL_SS_BodyToObject:		//尸变
		{
			
		}
		break;
		
	case	SKILL_SS_Mining:				//采矿
		{
			
		}
		break;
		
	case	SKILL_SS_RepairWeapon:		//修复术
		{
			
		}
		break;
		
	case	SKILL_SS_Capture:				//捕捉术 
		{
			
		}
		break;
	}
	
	if (m_bStartEvent && m_nStartSkillId > 0 && m_nEventSkillLevel > 0)
	{
		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nStartSkillId, m_nEventSkillLevel);
		if (!pOrdinSkill) 
            return FALSE;
		
        pOrdinSkill->Cast(nLauncher, nParam1, nParam2, nWaitTime, eLauncherType);
	}
	
	return TRUE;	  
}

/*!*****************************************************************************
// Function		: KSkill::Vanish
// Purpose		: 子弹生命结束时回调
// Return		: 
// Argumant		: KMissle* Missle
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void	KSkill::Vanish(KMissle * pMissle)  const 
{
	OnMissleEvent(Missle_VanishEvent, pMissle);
}

BOOL KSkill::OnMissleEvent(unsigned short usEvent, KMissle * pMissle)  const 
{
	if (!pMissle) 
        return FALSE;

	int nLauncherIdx = pMissle->m_nLauncher;
	
    if (
		pMissle->m_nMissleId <= 0 
		|| pMissle->m_nMissleId >= MAX_MISSLE 
		|| nLauncherIdx <= 0
		|| nLauncherIdx >= MAX_NPC
		|| Npc[nLauncherIdx].m_Index <= 0
		)
        return FALSE;

	
	if (
		(!Npc[nLauncherIdx].IsMatch(pMissle->m_dwLauncherId)) 
		|| Npc[nLauncherIdx].m_SubWorldIndex != pMissle->m_nSubWorldId
		|| Npc[nLauncherIdx].m_RegionIndex < 0
		)
	{
		return FALSE;
	}
	
	int nEventSkillId = 0;
	int nEventSkillLevel = 0;
	switch(usEvent)
	{
	case Missle_FlyEvent:
		if (!m_bFlyingEvent || m_nFlySkillId <= 0 || m_nEventSkillLevel <= 0)
			return FALSE;
		nEventSkillId = m_nFlySkillId ;
		nEventSkillLevel = m_nEventSkillLevel;
		break;
		
	case Missle_StartEvent:
		if (!m_bStartEvent || m_nStartSkillId <= 0 || m_nEventSkillLevel <= 0)
			return FALSE;
		nEventSkillId = m_nStartSkillId ;
		nEventSkillLevel = m_nEventSkillLevel;
		break;
		
	case Missle_VanishEvent:
		if (!m_bVanishedEvent || m_nVanishedSkillId <= 0 || m_nEventSkillLevel <= 0)
			return FALSE;
		nEventSkillId = m_nVanishedSkillId ;
		nEventSkillLevel = m_nEventSkillLevel;
		break;
		
	case Missle_CollideEvent:
		if (!m_bCollideEvent || m_nCollideSkillId <= 0 || m_nEventSkillLevel <= 0)
			return FALSE;
		nEventSkillId = m_nCollideSkillId;
		nEventSkillLevel = m_nEventSkillLevel;
		break;
	default:
		return FALSE;
	}
		
	int nDesPX = 0, nDesPY = 0;
	
	if (m_bByMissle)
	{
		pMissle->GetMpsPos(&nDesPX, &nDesPY);
	}
	else
	{
		Npc[nLauncherIdx].GetMpsPos(&nDesPX, &nDesPY);
	}
	
	KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nEventSkillId, nEventSkillLevel);
	if (!pOrdinSkill) 
        return FALSE;
	
	BOOL bRetCode = FALSE;
	
    if (m_bByMissle)    //When Event
	{
		if (pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles)
		{
			bRetCode = pOrdinSkill->CastMissles(pMissle->m_nMissleId, nDesPX, nDesPY, 0, SKILL_SLT_Missle);
		}
	}
	else
	{
		if (pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles)
		{   
            bRetCode = pOrdinSkill->CastMissles(nLauncherIdx, nDesPX, nDesPY, 0, SKILL_SLT_Npc);
		}
	}
	
	return bRetCode;
}

/*!*****************************************************************************
// Function		: KSkill::FlyEvent
// Purpose		: 
// Return		: void 
// Argumant		: int nMissleId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void KSkill::FlyEvent(KMissle * pMissle)  const 
{
	OnMissleEvent(Missle_FlyEvent, pMissle);
}

/*!*****************************************************************************
// Function		: KSkill::Collidsion
// Purpose		: 子弹被撞时回调
// Return		: 
// Argumant		: KMissle* Missle
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void	KSkill::Collidsion(KMissle * pMissle)  const 
{
	OnMissleEvent(Missle_CollideEvent, pMissle);
}

/*!*****************************************************************************
// Function		: KSkill::CastMissles
// Purpose		: 发送子弹技能
// Return		: 
// Argumant		: int nLauncher  发送者id
// Argumant		: int nParam1
// Argumant		: int nParam2
// Argumant		: int nWaitTime  延长时间
// Argumant		: eSkillLauncherType eLauncherType 发送者类型
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL	KSkill::CastMissles(int nLauncher, int nParam1, int nParam2, int nWaitTime  , eSkillLauncherType eLauncherType )  const 
{


	int nRegionId		=	0;
	int	nDesMapX		=	0;//地图坐标
	int nDesMapY		=	0;
	int nDesOffX		=	0;
	int nDesOffY		=	0;
	int nSrcOffX		=	0;
	int nSrcOffY		=	0;
	int nSrcPX			=	0;//点坐标
	int nSrcPY			=	0;
	int nDesPX			=	0;
	int nDesPY			=	0;
	int nDistance		=	0;
	int nDir			=	0;
	int nDirIndex		=	0;
	int nTargetId		=	-1;
	int nRefPX			=	0;
	int nRefPY			=	0;
	TOrdinSkillParam	SkillParam ;
	SkillParam.eLauncherType = SKILL_SLT_Npc;
	SkillParam.nParent = 0;
	SkillParam.eParentType = (eSkillLauncherType)0;
	SkillParam.nWaitTime = nWaitTime;
	SkillParam.nTargetId = 0;




	if (nLauncher <= 0) return FALSE;
	
	switch(m_eMisslesForm)
	{
	/*
	火墙时，第一数字参数表示子弹之间的长度间隔
	X2  = X1 + N * SinA
	Y2  = Y2 - N * CosA
	*/
		
	case	SKILL_MF_Wall:			//墙形	多个子弹呈垂直方向排列，类式火墙状
		{
			//墙形魔法不可以只传方向
			if (nParam1 == SKILL_SPT_Direction) return FALSE;
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{	
					nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2, &nDesPX, &nDesPY,  SKILL_SLT_Npc);

					if (Npc[nLauncher].m_SubWorldIndex < 0) 
					{
						return FALSE;
					}
					
					SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
					
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					nDir = nDir + MaxMissleDir / 4;
					if (nDir >= MaxMissleDir) nDir -= MaxMissleDir;
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTagetX = nDesPX;
					SkillParam.nTagetY = nDesPY;

					
					CastWall(&SkillParam , nDir, nDesPX, nDesPY);
				}	break;
			case SKILL_SLT_Obj:
				{
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					
					SubWorld[Missle[nLauncher].m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					int nDir = pMissle->m_nDir + MaxMissleDir / 4;
					if (nDir >= MaxMissleDir) nDir -= MaxMissleDir;
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.nParent = SKILL_SLT_Missle;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					SkillParam.nTagetX = pMissle->m_nTagetX;
					SkillParam.nTagetY = pMissle->m_nTagetY;
					CastWall(&SkillParam,  nDir, nRefPX, nRefPY);
				}break;
			}
		}break;
		
		
	case	SKILL_MF_Line:				//线形	多个子弹呈平行于玩家方向排列
		{
			
			if (nParam1 == SKILL_SPT_Direction)
			{
				switch(eLauncherType)
				{
				case SKILL_SLT_Npc:
					{
						SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
						if (nParam2 > MaxMissleDir || nParam2 < 0) return FALSE;
						nDir = nParam2;
						SkillParam.nLauncher = nLauncher;
						SkillParam.eLauncherType = eLauncherType;
						SkillParam.nTargetId = nTargetId;
						CastLine(&SkillParam, nDir, nSrcPX,nSrcPY);
						
					}break;
				case SKILL_SLT_Obj:
					{
						
					}break;
				case SKILL_SLT_Missle:
					{
						KMissle * pMissle = &Missle[nLauncher];
						if (nParam2 > MaxMissleDir || nParam2 < 0) return FALSE;
						if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
						nDir = nParam2;
						SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
						SkillParam.nLauncher = pMissle->m_nLauncher;
						SkillParam.nParent = nLauncher;
						SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
						SkillParam.nTagetX = pMissle->m_nTagetX;
					    SkillParam.nTagetY = pMissle->m_nTagetY;
						CastWall(&SkillParam, nDir,  nRefPX, nRefPY);
					}break;
				}
				
			}
			else
			{
				switch(eLauncherType)
				{
				case SKILL_SLT_Npc:
					{
						nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2, &nDesPX, &nDesPY,  SKILL_SLT_Npc);
						SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
						nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
						nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
						SkillParam.nLauncher = nLauncher;
						SkillParam.eLauncherType = eLauncherType;
						SkillParam.nTargetId = nTargetId;
						SkillParam.nTagetX = nDesPX;
					    SkillParam.nTagetY = nDesPY;
						if (m_nChildSkillNum == 1 && (g_MisslesLib[m_nChildSkillId].m_eMoveKind == MISSLE_MMK_Line || g_MisslesLib[m_nChildSkillId].m_eMoveKind == MISSLE_MMK_Parabola) ) 
						{
							if (nSrcPX == nDesPX && nSrcPY == nDesPY)		return FALSE ;
							nDistance = g_GetDistance(nSrcPX, nSrcPY, nDesPX, nDesPY);
							
							if (nDistance == 0 ) return FALSE;
							int		nYLength = nDesPY - nSrcPY;
							int		nXLength = nDesPX - nSrcPX;
							int		nSin = (nYLength << 10) / nDistance;	// 放大1024倍
							int		nCos = (nXLength << 10) / nDistance;
							
							if (abs(nSin) > 1024) 
								return FALSE;

							if (abs(nCos) > 1024) 
								return FALSE;
							
							
							CastExtractiveLineMissle(&SkillParam, nDir, nSrcPX, nSrcPY, nCos, nSin, nDesPX, nDesPY);
						}
						else
							CastLine(&SkillParam, nDir, nSrcPX,nSrcPY);
					}break;
				case SKILL_SLT_Obj:
					{
					}break;
				case SKILL_SLT_Missle:
					{
						KMissle * pMissle = &Missle[nLauncher];
						if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
						SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
						SkillParam.nLauncher = pMissle->m_nLauncher;
						SkillParam.nParent = nLauncher;
						SkillParam.eParentType = eLauncherType;
						SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
						SkillParam.nTagetX = pMissle->m_nTagetX;
					    SkillParam.nTagetY = pMissle->m_nTagetY;
						CastLine(&SkillParam,  pMissle->m_nDir,  nRefPX, nRefPY);
					}break;
				}
			}
		}
		break;
		
		//  数字参数一表示子弹之间的角度差，以64方向为准
		//  传来的X/Y参数为格子坐标
		
	case	SKILL_MF_Spread:				//散形	多个子弹呈一定的角度的发散状	
		{
			
			if (nParam1 == SKILL_SPT_Direction)
			{
				switch(eLauncherType)
				{
				case SKILL_SLT_Npc:
					{
						SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
						if (nParam2 > MaxMissleDir || nParam2 < 0) return FALSE;
						nDir = nParam2;
						SkillParam.nLauncher = nLauncher;
						SkillParam.eLauncherType = eLauncherType;
						CastSpread(&SkillParam, nDir, nSrcPX,nSrcPY);
					}break;
				case SKILL_SLT_Obj:
					{
						
					}break;
				case SKILL_SLT_Missle:
					{
						KMissle * pMissle = &Missle[nLauncher];
						if (nParam2 > MaxMissleDir || nParam2 < 0) return FALSE;
						if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
						nDir = nParam2;
						SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
						SkillParam.nLauncher = pMissle->m_nLauncher;
						SkillParam.nParent = nLauncher;
						SkillParam.eParentType = eLauncherType;
						SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
						SkillParam.nTagetX = pMissle->m_nTagetX;
					    SkillParam.nTagetY = pMissle->m_nTagetY;
						CastSpread(&SkillParam, nDir,  nRefPX, nRefPY);
					}break;
				}
			}
			else
			{
				switch(eLauncherType)
				{
				case SKILL_SLT_Npc:
					{
						nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2, &nDesPX, &nDesPY, SKILL_SLT_Npc);		
						SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
						nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
						nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
						SkillParam.nLauncher = nLauncher;
						SkillParam.eLauncherType = eLauncherType;
						SkillParam.nTargetId = nTargetId;
						SkillParam.nTagetX = nDesPX;
					    SkillParam.nTagetY = nDesPY;

						
						if (m_nChildSkillNum == 1 && (g_MisslesLib[m_nChildSkillId].m_eMoveKind == MISSLE_MMK_Line) ) 
						{
							if (nSrcPX == nDesPX && nSrcPY == nDesPY)		return FALSE ;
							nDistance = g_GetDistance(nSrcPX, nSrcPY, nDesPX, nDesPY);
							
							if (nDistance == 0 ) return FALSE;
							int		nYLength = nDesPY - nSrcPY;
							int		nXLength = nDesPX - nSrcPX;
							int		nSin = (nYLength << 10) / nDistance;	// 放大1024倍
							int		nCos = (nXLength << 10) / nDistance;

							if (abs(nSin) > 1024) 
								return FALSE;
							
							if (abs(nCos) > 1024) 
								return FALSE;

							CastExtractiveLineMissle(&SkillParam, nDir, nSrcPX, nSrcPY, nCos, nSin, nDesPX, nDesPY);
						}
						else
							CastSpread(&SkillParam, nDir, nSrcPX, nSrcPY);
					}break;
				case SKILL_SLT_Obj:
					{
						
					}break;
				case SKILL_SLT_Missle:
					{
						KMissle * pMissle = &Missle[nLauncher];
						if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
						SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
						SkillParam.nLauncher = pMissle->m_nLauncher;
						SkillParam.nParent = nLauncher;
						SkillParam.eParentType = eLauncherType;
						SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
						SkillParam.nTagetX = pMissle->m_nTagetX;
					    SkillParam.nTagetY = pMissle->m_nTagetY;
						CastSpread(&SkillParam ,pMissle->m_nDir,  nRefPX, nRefPY);
					}break;
				}
			}
			
		}break;
		
		
		//以当前点为圆点产生多个围扰的子弹
		//分成两种情况，一种为以原地为原心发出，另一种为以目标点为原心发出
		// 数字参数一表示 是否为原地发出
		
	case	SKILL_MF_Circle:				//圆形	多个子弹围成一个圈
		{
			if (nParam1 == SKILL_SPT_Direction) return FALSE;
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2,  &nDesPX, &nDesPY, eLauncherType);
					SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;
					SkillParam.nTagetX = nDesPX;
					SkillParam.nTagetY = nDesPY;
					
					if (m_nValue1 == 0)
						CastCircle(&SkillParam, nDir, nSrcPX, nSrcPY);
					else
						CastCircle(&SkillParam, nDir, nDesPX, nDesPY);
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.eParentType = eLauncherType;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					SkillParam.nTagetX = pMissle->m_nTagetX;
					SkillParam.nTagetY = pMissle->m_nTagetY;
					CastCircle(&SkillParam, pMissle->m_nDir,  nRefPX, nRefPY);

				}break;
			}
			
		}break;
		
	case	SKILL_MF_Random:				//随机	多个子弹随机排放
		{
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					
				}break;
			}
		}
		break;
		
	case	SKILL_MF_AtTarget:				//定点	多个子弹根据
		{
			if (nParam1 == SKILL_SPT_Direction) return FALSE;	
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2, &nDesPX, &nDesPY);
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;
					SkillParam.nTagetX = nDesPX;
					SkillParam.nTagetY = nDesPY;
					CastZoneZ(&SkillParam, nDir, nDesPX, nDesPY);
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.eParentType = eLauncherType;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					SkillParam.nTagetX = pMissle->m_nTagetX;
					SkillParam.nTagetY = pMissle->m_nTagetY;
					CastZoneZ(&SkillParam, pMissle->m_nDir, nRefPX, nRefPY);
				}break;
			}
		}break;
		
	case	SKILL_MF_AtFirer:				//本身	多个子弹停在玩家当前位置
		{
			if (nParam1 == SKILL_SPT_Direction) return FALSE;
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;
					SkillParam.nTagetX = nDesPX;
					SkillParam.nTagetY = nDesPY;
					CastZoneZ(&SkillParam,  nDir, nSrcPX, nSrcPY);
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.eParentType = eLauncherType;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					SkillParam.nTagetX = pMissle->m_nTagetX;
					SkillParam.nTagetY = pMissle->m_nTagetY;
					CastZoneZ(&SkillParam , pMissle->m_nDir, nRefPX, nRefPY);
				}break;
			}
			
		}break;
		
	case	SKILL_MF_Zone:
		{
			if (nParam1 == SKILL_SPT_Direction) return FALSE;
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2,  &nDesPX, &nDesPY);
					SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;
					SkillParam.nTagetX = nDesPX;
					SkillParam.nTagetY = nDesPY;
					CastZone(&SkillParam, nDir, nSrcPX, nSrcPY);
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.eParentType = eLauncherType;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					SkillParam.nTagetX = pMissle->m_nTagetX;
					SkillParam.nTagetY = pMissle->m_nTagetY;
					CastZone(&SkillParam, pMissle->m_nDir, nRefPX, nRefPY);
				}break;
			}
		}break;
	}
	return TRUE;
}

/*!*****************************************************************************
// Function		: KSkill::CastZone
// Purpose		: 
// Return		: int 
// Argumant		: int nLauncher
// Argumant		: eSkillLauncherType eLauncherType
// Argumant		: int nDir
// Argumant		: int nRefPX
// Argumant		: int nRefPY
// Argumant		: int nWaitTime
// Argumant		: int nTargetId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
//nValue1 = 0 表示矩形区域  nValue1 = 1 表示圆形区域
//nValue2 = 0 







int KSkill::CastZoneZ(TOrdinSkillParam * pSkillParam , int nDir, int nRefPX, int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	
	if (eLauncherType != SKILL_SLT_Npc) return 0;
	int nCastMissleNum	= 0;
	int nBeginPX ;
	int nBeginPY ;
	if (m_nChildSkillNum == 1)
	{
		nBeginPX = nRefPX;
		nBeginPY = nRefPY;
	}
	else 
	{
		nBeginPX		= nRefPX - m_nChildSkillNum * defLOGIC_CELL_WIDTH / 2;
		nBeginPY		= nRefPY - m_nChildSkillNum * defLOGIC_CELL_HEIGHT / 2;
	}
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	for (int i = 0; i < m_nChildSkillNum; i ++)
		for (int j = 0; j < m_nChildSkillNum; j ++)
		{
			if (m_bBaseSkill)
			{
				int nMissleIndex ;
				int nSubWorldId ; 
				
				nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
				
				if (m_nValue1 == 1)
					if ( ((i - m_nChildSkillNum / 2) * (i - m_nChildSkillNum / 2) + (j - m_nChildSkillNum / 2) * (j - m_nChildSkillNum / 2)) > (m_nChildSkillNum * m_nChildSkillNum / 4))			continue;
					
					
					if (nSubWorldId < 0)	goto exit;
					int nDesSubX = nBeginPX + j * defLOGIC_CELL_WIDTH;
					int nDesSubY = nBeginPY +  i * defLOGIC_CELL_HEIGHT;
					nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX , nDesSubY);
					
					if (nMissleIndex < 0)	continue;
					
					Missle[nMissleIndex].m_nDir				= nDir;
					Missle[nMissleIndex].m_nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
					CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
					Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
					Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
					Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
					Missle[nMissleIndex].m_nLauncher		= nLauncher;
					Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
					#ifdef _SERVER	
					Missle[nMissleIndex].m_nPKFlag			= Player[Npc[nLauncher].GetPlayerIdx()].m_cPK.GetNormalPKState();	
					#endif
					
					
					
					if (pSkillParam->nParent)
						Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
					else 
						Missle[nMissleIndex].m_nParentMissleIndex = 0;
					
					Missle[nMissleIndex].m_nSkillId			= m_nId;
					Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i * m_nChildSkillNum + j);
					Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
					Missle[nMissleIndex].m_nRefPX			= nDesSubX;
					Missle[nMissleIndex].m_nRefPY			= nDesSubY;
					
					if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line|| Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack)
					{
						Missle[nMissleIndex].m_nXFactor = g_DirCos(nDir, MaxMissleDir);
						Missle[nMissleIndex].m_nYFactor = g_DirSin(nDir, MaxMissleDir);
					}
#ifdef _SERVER
					Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
					nCastMissleNum ++;
			}
			else
			{
				_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
				if (pOrdinSkill) 
				{
					if (!pSkillParam->nParent)
						nCastMissleNum += pOrdinSkill->Cast(nLauncher, nBeginPX + j * defLOGIC_CELL_WIDTH , nBeginPY +  i * defLOGIC_CELL_HEIGHT, pSkillParam->nWaitTime + GetMissleGenerateTime(i * m_nChildSkillNum + j ), eLauncherType);
					else 
						nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nLauncher, nBeginPX + j * defLOGIC_CELL_WIDTH , nBeginPY +  i * defLOGIC_CELL_HEIGHT, pSkillParam->nWaitTime + GetMissleGenerateTime(i * m_nChildSkillNum + j ), pSkillParam->eLauncherType);
				}
			}
			
		}
exit:	
#ifdef _SERVER
		if (pNewMagicAttribsData)
			if (pNewMagicAttribsData->GetRef() == 0)
				delete pNewMagicAttribsData;
#endif
			return nCastMissleNum;
}

















int KSkill::CastZone(TOrdinSkillParam * pSkillParam , int nDir, int nRefPX, int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	
	
	if (eLauncherType != SKILL_SLT_Npc) return 0;
	int nCastMissleNum	= 0;
	int nBeginPX ;
	int nBeginPY ;
	int nOffSetX ;
	int nOffSetY ;
	if (m_nChildSkillNum == 1)
	{
		nBeginPX = nRefPX;
		nBeginPY = nRefPY;
		nOffSetX = 0;
		nOffSetY = 0;
	}
	else 
	{
		nOffSetX = g_DirSin(nDir, MaxMissleDir);
		nOffSetY = g_DirCos(nDir, MaxMissleDir);
		nBeginPX		= nRefPX - (((m_nChildSkillNum-1)/2 * nOffSetX * m_nValue1) >> 10);
		nBeginPY		= nRefPY + (((m_nChildSkillNum-1)/2 * nOffSetY * m_nValue1) >> 10);
	}
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	for (int i = 0; i < m_nChildSkillNum; i ++)
		
		{
			if (m_bBaseSkill)
			{
				int nMissleIndex ;
				int nSubWorldId ; 
				
				nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
				
		
					
					if (nSubWorldId < 0)	goto exit;
					int nDesSubX = nBeginPX + ((i * nOffSetX * m_nValue1) >> 10);
					int nDesSubY = nBeginPY - ((i * nOffSetY * m_nValue1) >> 10);
					nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX , nDesSubY);
					
					if (nMissleIndex < 0)	continue;
					
					Missle[nMissleIndex].m_nDir				= nDir;
					Missle[nMissleIndex].m_nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
					CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
					Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
					Missle[nMissleIndex].m_dwFollowNpcID    = Npc[pSkillParam->nTargetId].m_dwID;


					Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
					Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
					Missle[nMissleIndex].m_nLauncher		= nLauncher;
					Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
					#ifdef _SERVER	
					Missle[nMissleIndex].m_nPKFlag			= Player[Npc[nLauncher].GetPlayerIdx()].m_cPK.GetNormalPKState();	
					#endif
					
					if (pSkillParam->nParent)
						Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
					else 
						Missle[nMissleIndex].m_nParentMissleIndex = 0;
					
					Missle[nMissleIndex].m_nSkillId			= m_nId;
					Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
					Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
					Missle[nMissleIndex].m_nRefPX			= nDesSubX;
					Missle[nMissleIndex].m_nRefPY			= nDesSubY;
					
					if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line|| Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack|| Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Follow)
					{
						Missle[nMissleIndex].m_nXFactor = g_DirCos(nDir, MaxMissleDir);
						Missle[nMissleIndex].m_nYFactor = g_DirSin(nDir, MaxMissleDir);
						Missle[nMissleIndex].m_nTagetX = pSkillParam->nTagetX - (((m_nChildSkillNum-1)/2 * nOffSetX * m_nValue1) >> 10) + ((i * nOffSetX * m_nValue1) >> 10);
				        Missle[nMissleIndex].m_nTagetY = pSkillParam->nTagetY + (((m_nChildSkillNum-1)/2 * nOffSetY * m_nValue1) >> 10) - ((i * nOffSetY * m_nValue1) >> 10);
					}
#ifdef _SERVER
					Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
					nCastMissleNum ++;
			}
			else
			{
				_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
				if (pOrdinSkill) 
				{
					if (!pSkillParam->nParent)
						nCastMissleNum += pOrdinSkill->Cast(nLauncher, nBeginPX + ((i * nOffSetX * m_nValue1) >> 10),nBeginPY - ((i * nOffSetY * m_nValue1) >> 10), pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
					else 
						nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nLauncher, nBeginPX + ((i * nOffSetX * m_nValue1) >> 10) , nBeginPY -  ((i * nOffSetY * m_nValue1) >> 10), pSkillParam->nWaitTime + GetMissleGenerateTime(i), pSkillParam->eLauncherType);
				}
			}
			
		}
exit:	
#ifdef _SERVER
		if (pNewMagicAttribsData)
			if (pNewMagicAttribsData->GetRef() == 0)
				delete pNewMagicAttribsData;
#endif
			return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KSkill::CastLine
// Purpose		: 
// Return		: 
// Argumant		: int nLauncher
// Argumant		: eSkillLauncherType eLauncherType
// Argumant		: int nDir
// Argumant		: int nRefPX
// Argumant		: int nRefPY
// Argumant		: int nWaitTime
// Argumant		: int nTargetId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
// Value1 子弹之间的间距
// Value2 
int		KSkill::CastLine(TOrdinSkillParam *pSkillParam, int nDir, int nRefPX, int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	if (eLauncherType != SKILL_SLT_Npc) return 0;
	int	nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
	int nDesSubX		= 0;
	int nDesSubY		= 0;
	int nCastMissleNum	= 0;

	
	//子弹之间的间距
	int nMSDistanceEach = m_nValue1;
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	//分别生成多少子弹
	for(int i = 0; i < m_nChildSkillNum; i++)
	{
		nDesSubX	= nRefPX + ((nMSDistanceEach * (i + 1) * g_DirCos(nDirIndex, MaxMissleDir) )>>10);
		nDesSubY	= nRefPY + ((nMSDistanceEach * (i + 1) * g_DirSin(nDirIndex, MaxMissleDir) )>>10);
		
		if (nDesSubX < 0 || nDesSubY < 0) 	continue;
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId ; 
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	goto exit;
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX, nDesSubY);
			
			if (nMissleIndex < 0)	continue;
			
			Missle[nMissleIndex].m_nDir				= nDir;
			Missle[nMissleIndex].m_nDirIndex		= nDirIndex;
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			#ifdef _SERVER	
			Missle[nMissleIndex].m_nPKFlag			= Player[Npc[nLauncher].GetPlayerIdx()].m_cPK.GetNormalPKState();	
			#endif
			
			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;	
			Missle[nMissleIndex].m_nRefPX			= nDesSubX;
			Missle[nMissleIndex].m_nRefPY			= nDesSubY;
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack)
			{
				Missle[nMissleIndex].m_nXFactor = g_DirCos(nDir, MaxMissleDir);
				Missle[nMissleIndex].m_nYFactor = g_DirSin(nDir, MaxMissleDir);
				Missle[nMissleIndex].m_nTagetX = pSkillParam->nTagetX;
				Missle[nMissleIndex].m_nTagetY = pSkillParam->nTagetY;
			}
			
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			nCastMissleNum ++;
		}
		else
		{
			_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum += pOrdinSkill->Cast(nLauncher, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
				else
					nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nParent, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), pSkillParam->eParentType);
				
			}
		}
		
	}
	
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
			delete pNewMagicAttribsData;
#endif
		return nCastMissleNum;
}

int		KSkill::CastExtractiveLineMissle(TOrdinSkillParam* pSkillParam,  int nDir,int nSrcX, int nSrcY, int nXOffset, int nYOffset, int nDesX, int nDesY)  const 
{
	
	_ASSERT(pSkillParam);
	
	int nLauncher = pSkillParam->nLauncher;
	if (pSkillParam->eLauncherType != SKILL_SLT_Npc) return 0;	
	int	nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
	int nDesSubX		= 0;
	int nDesSubY		= 0;
	int nCastMissleNum	= 0;
	
	//子弹之间的间距
	
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	//分别生成多少子弹
	{
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId ; 
			
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	goto exit;
			nMissleIndex = MissleSet.Add(nSubWorldId, nSrcX, nSrcY);
			
			if (nMissleIndex < 0)	goto exit;
			
			Missle[nMissleIndex].m_nDir				= nDir;
			Missle[nMissleIndex].m_nDirIndex		= nDirIndex;
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Parabola)
			{
				int nLength = g_GetDistance(nSrcX, nSrcY, nDesX, nDesY);
				int nTime = nLength / Missle[nMissleIndex].m_nSpeed;
				Missle[nMissleIndex].m_nHeightSpeed	= Missle[nMissleIndex].m_nZAcceleration * (nTime - 1) / 2;
				
			}
			
			Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			#ifdef _SERVER	
			Missle[nMissleIndex].m_nPKFlag			= Player[Npc[nLauncher].GetPlayerIdx()].m_cPK.GetNormalPKState();	
			#endif


			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(0);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;	
			Missle[nMissleIndex].m_nRefPX			= nSrcX;
			Missle[nMissleIndex].m_nRefPY			= nSrcY;

			int nTempR = 0;
			int nTempMapX = 0;
			int nTempMapY = 0;
			int nTempOffsetX = 0;
			int nTempOffsetY = 0;

			Missle[nMissleIndex].m_bNeedReclaim = TRUE;
			int nLength = g_GetDistance(nSrcX, nSrcY, nDesX, nDesY);
			Missle[nMissleIndex].m_nFirstReclaimTime = nLength / Missle[nMissleIndex].m_nSpeed + Missle[nMissleIndex].m_nStartLifeTime;
			Missle[nMissleIndex].m_nEndReclaimTime = Missle[nMissleIndex].m_nFirstReclaimTime + defLOGIC_CELL_WIDTH / Missle[nMissleIndex].m_nSpeed + 2;

			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Parabola)
			{	
				Missle[nMissleIndex].m_nXFactor = nXOffset;
				Missle[nMissleIndex].m_nYFactor = nYOffset;
				Missle[nMissleIndex].m_nTagetX = pSkillParam->nTagetX;
				Missle[nMissleIndex].m_nTagetY = pSkillParam->nTagetY;
			}
			
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
			nCastMissleNum ++;
		}
		else
		{
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum += pOrdinSkill->Cast(nLauncher, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(0), pSkillParam->eLauncherType);
				else
					nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nParent, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(0), pSkillParam->eParentType);
				
			}
		}
		
	}
	
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
			delete pNewMagicAttribsData;
#endif
		
		return nCastMissleNum;
		
}

/*!*****************************************************************************
// Function		: KSkill::CastWall
// Purpose		: Wall Magic 
// Return		: int 
// Argumant		: int nLauncher
// Argumant		: eSkillLauncherType eLauncherType
// Argumant		: int nDir
// Argumant		: int nRefPX
// Argumant		: int nRefPY
// Argumant		: int nWaitTime
// Argumant		: int nTargetId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
/*
m_nValue1 表示子弹之间的距离，单位像素点
*/
int KSkill::CastWall(TOrdinSkillParam * pSkillParam,  int nDir , int nRefPX , int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	
	if (eLauncherType != SKILL_SLT_Npc) return 0;
	int	nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
	int nDesSubX		= 0;
	int nDesSubY		= 0;
	int nCastMissleNum	= 0;
	
	
	//子弹之间的间距
	int nMSDistanceEach = m_nValue1;
	int nCurMSDistance	= -1 * nMSDistanceEach * m_nChildSkillNum / 2;
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	//分别生成多少子弹
	for(int i = 0; i < m_nChildSkillNum; i++)
	{
		nDesSubX	= nRefPX + ((nCurMSDistance * g_DirCos(nDirIndex, MaxMissleDir)) >>10);
		nDesSubY	= nRefPY + ((nCurMSDistance * g_DirSin(nDirIndex, MaxMissleDir)) >>10);
		
		if (nDesSubX < 0 || nDesSubY < 0) 	continue;
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId ; 
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	
			{
				goto exit;
			}
			
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX, nDesSubY);
			if (nMissleIndex < 0)	
			{
				continue;
			}

			if (m_nValue2)
			{
				int nDirTemp = nDir - MaxMissleDir / 4;
				if (nDirTemp < 0) nDirTemp += MaxMissleDir;
				Missle[nMissleIndex].m_nDir				= nDirTemp;
				Missle[nMissleIndex].m_nDirIndex = g_Dir2DirIndex(nDirTemp, 64);

			}
			else
			{
				Missle[nMissleIndex].m_nDir				= nDir;
				Missle[nMissleIndex].m_nDirIndex		= nDirIndex;
			}
			
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
			
			if (pSkillParam->nTargetId)
				Missle[nMissleIndex].m_dwFollowNpcID	= Npc[pSkillParam->nTargetId].m_dwID;
			
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			#ifdef _SERVER	
			Missle[nMissleIndex].m_nPKFlag			= Player[Npc[nLauncher].GetPlayerIdx()].m_cPK.GetNormalPKState();	
			#endif
			
			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
			Missle[nMissleIndex].m_nRefPX			= nDesSubX;
			Missle[nMissleIndex].m_nRefPY			= nDesSubY;
			
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line|| Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack)
			{
				Missle[nMissleIndex].m_nXFactor = g_DirCos(Missle[nMissleIndex].m_nDir, MaxMissleDir);
				Missle[nMissleIndex].m_nYFactor = g_DirSin(Missle[nMissleIndex].m_nDir, MaxMissleDir);
			}
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
			nCastMissleNum ++;
		}
		else
		{
			_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum += pOrdinSkill->Cast(nLauncher, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
				else
					nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nParent, nDesSubX, nDesSubY, pSkillParam->nWaitTime +  GetMissleGenerateTime(i), pSkillParam->eParentType);
			}
		}
		
		nCurMSDistance += nMSDistanceEach;
	}
	
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
			delete pNewMagicAttribsData;
#endif
		return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KSkill::CastCircle
// Purpose		: 
// Return		: 
// Argumant		: int nLauncher
// Argumant		: eSkillLauncherType  eLauncherType
// Argumant		: int nDir
// Argumant		: int nRefPX
// Argumant		: int nRefPY
// Argumant		: int nWaitTime
// Argumant		: int nTargetId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
// Value1  == 0 表示发送者为圆心产生圆，否则以目标点为圆心产生圆
int		KSkill::CastCircle(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType  eLauncherType = pSkillParam->eLauncherType;
	if (eLauncherType != SKILL_SLT_Npc) return 0;	
	int nDesSubPX	= 0;
	int nDesSubPY	= 0;
	int nFirstStep	= m_nValue2;			//第一步的长度，子弹在刚发出去时离玩家的距离
	int nCurSubDir	= 0;
	int nDirPerNum  = 	MaxMissleDir / m_nChildSkillNum  ;
	int nCastMissleNum = 0;
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	//分别生成多个子弹
	for(int i = 0; i < m_nChildSkillNum; i++)
	{
		int nCurSubDir	= nDir + nDirPerNum * i ;
		
		if (nCurSubDir < 0)
			nCurSubDir = MaxMissleDir + nCurSubDir;
		
		if (nCurSubDir >= MaxMissleDir)
			nCurSubDir -= MaxMissleDir;
		
		int nSinAB	= g_DirSin(nCurSubDir, MaxMissleDir);
		int nCosAB	= g_DirCos(nCurSubDir, MaxMissleDir);
		
		nDesSubPX	= nRefPX + ((nCosAB * nFirstStep) >> 10);
		nDesSubPY	= nRefPY + ((nSinAB * nFirstStep) >> 10);
		
		
		
		if (nDesSubPX < 0 || nDesSubPY < 0) 	continue;
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId ; 
			
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	goto exit;
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubPX, nDesSubPY);
			
			if (nMissleIndex < 0)	
			{
				continue;
			}
			
			Missle[nMissleIndex].m_nDir			= nCurSubDir;
			Missle[nMissleIndex].m_nDirIndex	= g_Dir2DirIndex(nCurSubDir, MaxMissleDir);
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			
			Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
			if (pSkillParam->nTargetId)
				Missle[nMissleIndex].m_dwFollowNpcID	= Npc[pSkillParam->nTargetId].m_dwID;

			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			#ifdef _SERVER	
			Missle[nMissleIndex].m_nPKFlag			= Player[Npc[nLauncher].GetPlayerIdx()].m_cPK.GetNormalPKState();	
			#endif
			
			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
			Missle[nMissleIndex].m_nRefPX			= nDesSubPX;
			Missle[nMissleIndex].m_nRefPY			= nDesSubPY;
			
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Follow)
			{
				Missle[nMissleIndex].m_nXFactor = g_DirCos(nCurSubDir, MaxMissleDir);
				Missle[nMissleIndex].m_nYFactor = g_DirSin(nCurSubDir, MaxMissleDir);
			}
			
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
			nCastMissleNum ++;
			
		}
		else
		{
			_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum += pOrdinSkill->Cast(nLauncher, nDesSubPX, nDesSubPY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
				else
					nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nParent, nDesSubPX, nDesSubPY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), pSkillParam->eParentType);
			}
		}
		
	}
	
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
			delete pNewMagicAttribsData;
#endif
		
		return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KSkill::CastSpread
// Purpose		: 
// Return		: 
// Argumant		: int nLauncher
// Argumant		: eSkillLauncherType eLauncherType
// Argumant		: int nDir
// Argumant		: int nRefPX
// Argumant		: int nRefPY
// Argumant		: int nWaitTime
// Argumant		: int nTargetId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
/*
Value1 每个子弹相差的角度单位
Value2 每一步的长度，第一步的长度，子弹在刚发出去时离玩家的距离
*/
int		KSkill::CastSpread(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	if (eLauncherType != SKILL_SLT_Npc) return 0;
	int nDesSubMapX		= 0;
	int nDesSubMapY		= 0;
	int nFirstStep		= m_nValue2;			//第一步的长度，子弹在刚发出去时离玩家的距离
	int nCurMSRadius	= m_nChildSkillNum / 2 ; 
	int nCurSubDir		= 0;
	int	nCastMissleNum  = 0;			//实际发送的Missle的数量
	
	// Sin A+B = SinA*CosB + CosA*SinB
	// Cos A+B = CosA*CosB - SinA*SinB
	// Sin A = nYFactor
	// Cos A = nXFactor
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif
	
	int nDesSubX = 0;
	int nDesSubY = 0;
	int nXFactor = 0;
	int nYFactor = 0;
	
	if (pSkillParam->nTargetId > 0)
	{
		int nTargetId = pSkillParam->nTargetId;
		int nDistance = 0;
		int nDesX, nDesY;
		if (Npc[nTargetId].m_Index > 0 && Npc[nTargetId].m_SubWorldIndex >= 0) 
			SubWorld[Npc[nTargetId].m_SubWorldIndex].Map2Mps(Npc[nTargetId].m_RegionIndex, Npc[nTargetId].m_MapX, Npc[nTargetId].m_MapY, Npc[nTargetId].m_OffX, Npc[nTargetId].m_OffY, &nDesX, &nDesY);
		
		nDistance = (int)sqrt((nDesX - nRefPX)*(nDesX - nRefPX) +	(nDesY - nRefPY)*(nDesY - nRefPY));
		
		if (nDistance == 0)
		{
			printf("Xay ra loi skills, tu dong xua loi !\n");
			return 0;
		}

		nXFactor = ((nDesX - nRefPX)<<10) / nDistance;
		nYFactor = ((nDesY - nRefPY)<<10) / nDistance;
		
		nDesSubX = nRefPX + ((nXFactor * nFirstStep)>>10);
		nDesSubY = nRefPY + ((nYFactor * nFirstStep)>>10);
		
		if (nDesSubX < 0  || nDesSubY < 0 ) return 0;
	}
	
	int nTargetId = pSkillParam->nTargetId;


   int nCurBaseSubDir = nDir;

	
	   
		if (nCurBaseSubDir < 0)
			nCurBaseSubDir = MaxMissleDir + nCurBaseSubDir;
		
		if (nCurBaseSubDir >= MaxMissleDir)
			nCurBaseSubDir -= MaxMissleDir;
		
		int nSinBaseAB	;
		int nCosBaseAB	;
		
		
			nSinBaseAB = g_DirSin(nCurBaseSubDir, MaxMissleDir);
			nCosBaseAB = g_DirCos(nCurBaseSubDir, MaxMissleDir);
	

    int nBasenDistance = (int)sqrt((pSkillParam->nTagetX - nRefPX)*(pSkillParam->nTagetX - nRefPX) + (pSkillParam->nTagetY - nRefPY)*(pSkillParam->nTagetY - nRefPY));
	
	
	int nIdxMissleTVC[3];
	nIdxMissleTVC[0] = -1;
	nIdxMissleTVC[1] = -1;
	nIdxMissleTVC[2] = -1;


	


	//分别生成多个子弹
	for(int i = 0; i < m_nChildSkillNum; i++)
	{

		nCurSubDir		= nDir - m_nValue1 * nCurMSRadius;
		
		
		if (nCurSubDir < 0)
			nCurSubDir = MaxMissleDir + nCurSubDir;
		
		if (nCurSubDir >= MaxMissleDir)
			nCurSubDir -= MaxMissleDir;
		
		int nSinAB	;
		int nCosAB	;
		
	
			nSinAB = g_DirSin(nCurSubDir, MaxMissleDir);
			nCosAB = g_DirCos(nCurSubDir, MaxMissleDir);
		
		
		nDesSubX	= nRefPX + ((nCosAB * nFirstStep) >> 10);
		nDesSubY	= nRefPY + ((nSinAB * nFirstStep) >> 10);
		
		if (nDesSubX < 0 || nDesSubY < 0) 	continue;
		
		if (m_bBaseSkill)
		{
			
			int nMissleIndex ;
			int nSubWorldId ; 
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	goto exit;
			
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX, nDesSubY);
			
			if (nMissleIndex < 0)	continue;
			
			Missle[nMissleIndex].m_nDir				= nCurSubDir;
			Missle[nMissleIndex].m_nDirIndex		= g_Dir2DirIndex(nCurSubDir, MaxMissleDir);
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			Missle[nMissleIndex].m_nFollowNpcIdx	= nTargetId;
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			Missle[nMissleIndex].m_dwFollowNpcID	= Npc[nTargetId].m_dwID;
			#ifdef _SERVER	
			Missle[nMissleIndex].m_nPKFlag			= Player[Npc[nLauncher].GetPlayerIdx()].m_cPK.GetNormalPKState();	
			#endif

	
			

			if (m_nId == 346)
			{

			if (nIdxMissleTVC[0] == -1)
			{
			nIdxMissleTVC[0] = nMissleIndex;
			}
			else if (nIdxMissleTVC[1] == -1)
			{
			nIdxMissleTVC[1] = nMissleIndex;
			}
			else if (nIdxMissleTVC[2] == -1)
			{
			nIdxMissleTVC[2] = nMissleIndex;
			}


			}




			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
			Missle[nMissleIndex].m_nXFactor			= nCosAB;
			Missle[nMissleIndex].m_nYFactor			= nSinAB;
			Missle[nMissleIndex].m_nRefPX			= nDesSubX;
			Missle[nMissleIndex].m_nRefPY			= nDesSubY;

			Missle[nMissleIndex].m_nTagetX          = pSkillParam->nTagetX + (((nCosAB - nCosBaseAB) * nBasenDistance) >> 10);
			Missle[nMissleIndex].m_nTagetY          = pSkillParam->nTagetY + (((nSinAB - nSinBaseAB) * nBasenDistance) >> 10);
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			nCastMissleNum ++;
		}
		else
		{
			_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum +=  pOrdinSkill->Cast(nLauncher,  nRefPX, nRefPY , pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
				else
					nCastMissleNum +=  pOrdinSkill->Cast(pSkillParam->nParent,  nRefPX, nRefPY , pSkillParam->nWaitTime + GetMissleGenerateTime(i), pSkillParam->eParentType); 
			}
		}
		
		nCurMSRadius -- ;
	}




			if (m_nId == 346)
			{

			if (nIdxMissleTVC[0] > 0 && nIdxMissleTVC[0] < MAX_MISSLE && nIdxMissleTVC[1] > 0 && nIdxMissleTVC[1] < MAX_MISSLE && nIdxMissleTVC[2] > 0 && nIdxMissleTVC[2] < MAX_MISSLE)
			{

			if (Missle[nIdxMissleTVC[0]].m_nMissleId > 0 && Missle[nIdxMissleTVC[1]].m_nMissleId > 0 && Missle[nIdxMissleTVC[2]].m_nMissleId > 0)
			{

			Missle[nIdxMissleTVC[0]].m_nIdxMissTVC[0] = nIdxMissleTVC[1];
			Missle[nIdxMissleTVC[0]].m_nIdxMissTVC[1] = nIdxMissleTVC[2];
			Missle[nIdxMissleTVC[0]].m_nDirMissTVC = Missle[nIdxMissleTVC[1]].m_nDir;

			Missle[nIdxMissleTVC[1]].m_nIdxMissTVC[0] = nIdxMissleTVC[0];
			Missle[nIdxMissleTVC[1]].m_nIdxMissTVC[1] = nIdxMissleTVC[2];
			Missle[nIdxMissleTVC[1]].m_nDirMissTVC = Missle[nIdxMissleTVC[1]].m_nDir;
			
			Missle[nIdxMissleTVC[2]].m_nIdxMissTVC[0] = nIdxMissleTVC[0];
			Missle[nIdxMissleTVC[2]].m_nIdxMissTVC[1] = nIdxMissleTVC[1];
			Missle[nIdxMissleTVC[2]].m_nDirMissTVC = Missle[nIdxMissleTVC[1]].m_nDir;

			}

			}


			}







exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
			delete pNewMagicAttribsData;
#endif
		
		return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KSkill::GetChildSkillNum
// Purpose		: 由于可能某些技能中，随着技能等级的升级，子弹的数目也会因此增加，所以通过该函数获得实际的子技能数目
// Return		: 
// Argumant		: int nLevel
// Comments		:
// Author		: RomanDou
*****************************************************************************/
int 	KSkill::GetChildSkillNum(int nLevel)  const 
{
	return m_nChildSkillNum;
};
/*!*****************************************************************************
// Function		: KSkill::CreateMissle
// Purpose		: 设置子弹的基本数据，以及该技能该等级下的对子弹信息的变动数据
//					设置用于数值计算的指针
// Return		: 
// Argumant		: int nChildSkillId
// Argumant		: int nMissleIndex
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void	KSkill::CreateMissle(int nLauncher, int nChildSkillId, int nMissleIndex)  const 
{
	_ASSERT(nChildSkillId > 0 && nChildSkillId < MAX_MISSLESTYLE && nMissleIndex > 0);
	
	if (nLauncher <= 0) 
	{
		return ;
	}
	
	KMissle * pMissle = &Missle[nMissleIndex];
	
	g_MisslesLib[nChildSkillId] = *pMissle;//复制拷贝对象
	
	pMissle->m_nLevel			= m_ulLevel;
	pMissle->m_bCollideEvent	= m_bCollideEvent;
	pMissle->m_bVanishedEvent   = m_bVanishedEvent;
	pMissle->m_bStartEvent		= m_bStartEvent;
	pMissle->m_bFlyEvent		= m_bFlyingEvent;
	pMissle->m_nFlyEventTime	= m_nFlyEventTime;
	pMissle->m_nMissleId		= nMissleIndex;
	pMissle->m_bClientSend      = m_bClientSend;
	pMissle->m_bMustBeHit		= m_bMustBeHit;
	pMissle->m_bIsMelee			= m_bIsMelee;
	pMissle->m_bByMissle		= m_bByMissle;
	pMissle->m_bTargetSelf		= (m_bTargetSelf == 1);
	pMissle->m_nInteruptTypeWhenMove = m_nInteruptTypeWhenMove;
	pMissle->m_bHeelAtParent	= m_bHeelAtParent;
	pMissle->m_bUseAttackRating	= m_bUseAttackRate;
	pMissle->m_bDoHurt			= m_bDoHurt;
	
	if (pMissle->m_nInteruptTypeWhenMove)
	{
		Npc[nLauncher].GetMpsPos(&pMissle->m_nLauncherSrcPX, &pMissle->m_nLauncherSrcPY);
	}
	pMissle->m_eRelation = m_eRelation;

	
#ifndef _SERVER
	pMissle->m_MissleRes.m_bNeedShadow   = m_bNeedShadow;
	pMissle->m_MissleRes.m_nMaxShadowNum = m_nMaxShadowNum;
	pMissle->m_MissleRes.m_nMissleId	 = nMissleIndex;
	if (!pMissle->m_MissleRes.Init()) g_DebugLog("创建子弹贴图失败！！！%s", __FILE__) ;
#endif
	
	pMissle->DoWait();
	
	for (int i = 0  ; i < m_nMissleAttribsNum; i ++)
	{
		switch (m_MissleAttribs[i].nAttribType)
		{
		case magic_missle_movekind_v:
			{
				pMissle->m_eMoveKind	= (eMissleMoveKind) m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_speed_v:	
			{
				pMissle->m_nSpeed		= m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_lifetime_v:
			{
				pMissle->m_nLifeTime	= m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_height_v:	
			{
				pMissle->m_nHeight		= m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_damagerange_v:
			{
				pMissle->m_nDamageRange = m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_radius_v:	
			{
			}break;
		}
	}
	
	if (m_bIsMelee)
		pMissle->m_nLifeTime = Npc[nLauncher].ModifyMissleLifeTime(pMissle->m_nLifeTime);
	else
	{
		pMissle->m_nSpeed = Npc[nLauncher].ModifyMissleSpeed(pMissle->m_nSpeed);
		pMissle->m_bCollideVanish = Npc[nLauncher].ModifyMissleCollsion(pMissle->m_bCollideVanish);
	}
	//printf("%KSkill: %d %d %d %d \n",nLauncher, nChildSkillId, nMissleIndex ,pMissle->m_nLifeTime);
}


/*!*****************************************************************************
// Function		: KSkill::GetInfoFromTabFile
// Purpose		: 从TabFile中获得该技能的常规属性
// Return		: 
// Argumant		: int nCol
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL	KSkill::GetInfoFromTabFile(int nRow)
{
	KITabFile * pITabFile = &g_OrdinSkillsSetting;
	return GetInfoFromTabFile(&g_OrdinSkillsSetting, nRow);
}


BOOL	KSkill::GetInfoFromTabFile(KITabFile *pSkillsSettingFile, int nRow)
{
	if (!pSkillsSettingFile || nRow < 0) return FALSE;
	//	
	pSkillsSettingFile->GetString(nRow, "SkillName",		"", m_szName, sizeof(m_szName) ,TRUE);
	pSkillsSettingFile->GetInteger(nRow, "SkillId",			0, (int *)&m_nId,TRUE);
	
	int nReqLevel = 0;
	pSkillsSettingFile->GetInteger(nRow, "ReqLevel",		0, (int *)&nReqLevel, TRUE);
	m_usReqLevel = (unsigned short)nReqLevel;


	pSkillsSettingFile->GetInteger(nRow, "EqtLimit",		-2, (int *)&m_nEquiptLimited, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "HorseLimit",		0, (int *)&m_nHorseLimited, TRUE);

	pSkillsSettingFile->GetInteger(nRow, "DoHurt",			0, (int *)&m_bDoHurt);
	pSkillsSettingFile->GetInteger(nRow, "ChildSkillNum",	0, &m_nChildSkillNum,TRUE);
	pSkillsSettingFile->GetInteger(nRow, "MisslesForm",		0, (int *)&m_eMisslesForm, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CharClass",		0, &m_nCharClass, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "SkillStyle",		0, (int *)&m_eSkillStyle, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CharAnimId",		0, (int *)&m_nCharActionId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsPhysical",		0, &m_bIsPhysical, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsSkillPhysical",		0, &m_bIsSkillPhysical, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsSkillMagic",		0, &m_bIsSkillMagic, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsAura",			0, &m_bIsAura, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsUseAR",			0, &m_bUseAttackRate, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetOnly",		0, &m_bTargetOnly, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetEnemy",		0, &m_bTargetEnemy, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetAlly",		0, &m_bTargetAlly, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetObj",		0, &m_bTargetObj, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "BaseSkill",		0, &m_bBaseSkill, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ByMissle",		0, &m_bByMissle, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ChildSkillId",	0, &m_nChildSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "FlyEvent",		0, &m_bFlyingEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "StartEvent",		0, &m_bStartEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CollideEvent",	0, &m_bCollideEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "VanishedEvent",	0, &m_bVanishedEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "FlySkillId",		0, &m_nFlySkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "StartSkillId",	0, &m_nStartSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "VanishedSkillId",	0, &m_nVanishedSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CollidSkillId",	0, &m_nCollideSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "SkillCostType",	0, (int *)&m_nSkillCostType, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CostValue",		0, &m_nCost, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TimePerCast",		0, &m_nMinTimePerCast, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TimePerCastHorse",0, &m_nMinTimePerCastHorse, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "Param1",			0, &m_nValue1, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "Param2",			0, &m_nValue2, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ChildSkillLevel", 0, &m_nChildSkillLevel, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "EventSkillLevel", 0, &m_nEventSkillLevel, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsMelee",			0, &m_bIsMelee, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "FlyEventTime",	0, &m_nFlyEventTime, TRUE);
	
	pSkillsSettingFile->GetInteger(nRow, "MslsGenerate",	0, (int *)&m_eMisslesGenerateStyle, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "MslsGenerateData",0, &m_nMisslesGenerateData, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "MaxShadowNum",	0, &m_nMaxShadowNum, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "AttackRadius",	50, &m_nAttackRadius, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "WaitTime",		0, &m_nWaitTime, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ClientSend",		0, &m_bClientSend, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetSelf",		0, &m_bTargetSelf, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "StopWhenMove",	0, &m_nInteruptTypeWhenMove, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "HeelAtParent",  0, (int *)&m_bHeelAtParent, TRUE );
	
	//服务器端需要获得并通知客户端
	pSkillsSettingFile->GetInteger(nRow, "StateSpecialId",  0, &m_nStateSpecialId, TRUE);

	m_eRelation = 0;
	if (m_bTargetEnemy)
		m_eRelation |= relation_enemy;
	
	if (m_bTargetAlly)
		m_eRelation |= relation_ally;
	
	if (m_bTargetSelf)
		m_eRelation |= relation_self;
	
#ifndef _SERVER
	pSkillsSettingFile->GetString(nRow, "SkillDesc", "", m_szSkillDesc, 100);
	pSkillsSettingFile->GetInteger(nRow, "NeedShadow",		0, &m_bNeedShadow, TRUE);
	pSkillsSettingFile->GetString(nRow, "SkillIcon","\\spr\\skill\\图标\\通用.spr",	m_szSkillIcon, 80);
	if (!m_szSkillIcon[0])	strcpy(m_szSkillIcon, "\\spr\\skill\\图标\\通用.spr");
	pSkillsSettingFile->GetInteger(nRow, "LRSkill",		0, (int*)&m_eLRSkillInfo);
	pSkillsSettingFile->GetString(nRow, "PreCastSpr", "", m_szPreCastEffectFile, 100);
	pSkillsSettingFile->GetString(nRow, "ManCastSnd","", m_szManPreCastSoundFile, 100);
	pSkillsSettingFile->GetString(nRow, "FMCastSnd", "", m_szFMPreCastSoundFile, 100);
#else
	char szLevelScript[MAX_PATH];
	
	//读取设定脚本1，记录每一级技能的属性变化，数值设定的
	pSkillsSettingFile->GetString(nRow, "LvlSetScript", "", szLevelScript, MAX_PATH);
	if (szLevelScript[0])
	{
		strlwr(szLevelScript);
		m_dwSkillLevelDataScriptId = g_FileName2Id(szLevelScript);
	}
	
	//读取设定脚本2，记录技能限制信息
	pSkillsSettingFile->GetString(nRow, "LevelUpScript","",szLevelScript, MAX_PATH);
	if (szLevelScript[0])
	{
		strlwr(szLevelScript);
		m_dwSkillLevelUpScriptID = g_FileName2Id(szLevelScript);
	}
#endif
	return TRUE;
}

/*!*****************************************************************************
// Function		: KSkill::LoadSkillLevelData
// Purpose		: 读表获得当前等级下当前技能的技能、子弹、碰撞数值影响
// Return		: 
// Argumant		: int nLevel
// Comments		:
// Author		: Romandou
****************************************************************************/
void		KSkill::LoadSkillLevelData(unsigned long  nLevel /* =0*/, int nParam)
{
	m_nMissleAttribsNum = 0;
	m_nDamageAttribsNum = 0;
	m_nImmediateAttribsNum = 0;
	m_nStateAttribsNum	= 0;		//个数，最大10

    for (int l=0;l<6;l++)
	{
	for (int m=0;m<2;m++)
	{
	m_addskillsdame[l][m] = 0;
	}
	}

    m_addskillmisslesrun = 0;
	
	char szSettingScriptName[MAX_PATH];
	char szSettingNameValue[100];
	char szSettingDataValue[100];
	char szResult[300];
	int nRowId = nParam;
	if (nRowId < 2) return ;
	//Question 必须顺序一至才行
	KLuaScript * pScript = NULL;
#ifndef _SERVER
	g_OrdinSkillsSetting.GetString(nRowId,  "LvlSetScript", "", szSettingScriptName, MAX_PATH );
	if (!szSettingScriptName[0]) return;
	g_SetFilePath("\\");
	//加载Lua脚本
	KLuaScript Script;
	Script.Init();
	if (!Script.Load(szSettingScriptName)) 
	{
		g_DebugLog("[KSkill::LoadSkillLevelData] Can not load script %s�", szSettingScriptName);
		return;
	}
	pScript  = &Script;
#else
	if (!m_dwSkillLevelDataScriptId) 
	{
		printf("Khong the lay duoc script thiet lap cua ky nang [%s]\n", GetSkillName());
		return ;
	}
	
	pScript = (KLuaScript*)g_GetScript(m_dwSkillLevelDataScriptId);
	
	if (!pScript)
	{
		printf("Khong the lay duoc script thiet lap cua ky nang [%s]\n", GetSkillName());
		return ;
	}
#endif
	


	int nSafeIndex = 1;
	pScript->SafeCallBegin(&nSafeIndex);
	
	for(int i = 0 ;  i  < MAXSKILLLEVELSETTINGNUM ; i ++)
	{
		char szSettingName[40];
		char szSettingData[40];
		sprintf(szSettingName, "LvlSetting%d", i + 1);
		sprintf(szSettingData, "LvlData%d", i + 1);
		
		g_OrdinSkillsSetting.GetString(nRowId, szSettingName, "", szSettingNameValue, 100);
		g_OrdinSkillsSetting.GetString(nRowId, szSettingData, "", szSettingDataValue, 100);
		
		if (szSettingNameValue[0] == 0 	|| szSettingDataValue[0] == '0'	)
		{
			continue;
		}
		
		pScript->CallFunction("GetSkillLevelData", 1, "ssd", szSettingNameValue, szSettingDataValue, nLevel);
		const char * szType = lua_typename(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
		if (Lua_IsNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)
		{
			int nResult = (int)Lua_ValueToNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			sprintf(szResult, "%d", nResult);
		}
		else if (Lua_IsString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)
		{
			strcpy(szResult , (char *)Lua_ValueToString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)));
		}
		else
		{
			char szMsg[300];
			sprintf(szMsg, "当获得该技能等级为%d（%s,%s）时，出错，请检查脚本!,",nLevel, szSettingNameValue, szSettingDataValue);
			g_DebugLog(szMsg);
			break;
		}
		ParseString2MagicAttrib(nLevel, szSettingNameValue, szResult);
		
	}
	pScript->SafeCallEnd(nSafeIndex);

	
}

#ifdef _SERVER
//When nLauncher == 0 , means neednt  AppendSkillEffect;
KMissleMagicAttribsData* KSkill::CreateMissleMagicAttribsData(int nLauncher)  const 
{
	if (nLauncher < 0 || m_bClientSend) return NULL; 
	
	KMissleMagicAttribsData* pMissleMagicAttribsData = new KMissleMagicAttribsData;
	
	pMissleMagicAttribsData->m_pStateMagicAttribs = (KMagicAttrib *)m_StateAttribs;
	pMissleMagicAttribsData->m_nStateMagicAttribsNum = m_nStateAttribsNum;

	pMissleMagicAttribsData->m_pImmediateAttribs = (KMagicAttrib *)m_ImmediateAttribs;
	pMissleMagicAttribsData->m_nImmediateMagicAttribsNum = m_nImmediateAttribsNum;
	
	KMagicAttrib * pDamageAttribs =  new KMagicAttrib[MAX_MISSLE_DAMAGEATTRIB];
	pMissleMagicAttribsData->m_nDamageMagicAttribsNum = m_nDamageAttribsNum;
	
	//根据玩家的基本属性，确定子弹的伤害
	if (nLauncher)
	{
		Npc[nLauncher].AppendSkillEffect(m_nId,m_bIsSkillPhysical,m_bIsSkillMagic,m_bIsPhysical, m_bIsMelee, (KMagicAttrib *)m_DamageAttribs, pDamageAttribs);
	}
	else
	{
		memcpy(pDamageAttribs, (KMagicAttrib *)m_DamageAttribs, sizeof(m_DamageAttribs));
	}
	
	pMissleMagicAttribsData->m_pDamageMagicAttribs = pDamageAttribs;
	return pMissleMagicAttribsData;
}
#endif
/*!*****************************************************************************
// Function		: KSkill::SetMissleGenerateTime
// Purpose		: 获得当前的子弹的实际产生时间
// Return		: void 
// Argumant		: Missle * pMissle
// Argumant		: int nNo
// Comments		:
// Author		: RomanDou
*****************************************************************************/
unsigned int KSkill::GetMissleGenerateTime(int nNo) const 
{
	
	switch(m_eMisslesGenerateStyle)
	{
	case SKILL_MGS_NULL:
		{
			return m_nWaitTime;
		}break;
		
	case SKILL_MGS_SAMETIME:
		{
			return  m_nWaitTime + m_nMisslesGenerateData;
		}break;
		
	case SKILL_MGS_ORDER:		
		{
			return  m_nWaitTime + nNo * m_nMisslesGenerateData;
		}break;
		
	case SKILL_MGS_RANDONORDER:	
		{
			if (g_Random(2) == 1) 
				return m_nWaitTime + nNo * m_nMisslesGenerateData + g_Random(m_nMisslesGenerateData);
			else 
				return m_nWaitTime + nNo * m_nMisslesGenerateData  - g_Random(m_nMisslesGenerateData / 2);
		}break;
		
	case SKILL_MGS_RANDONSAME:	
		{
			return  m_nWaitTime + g_Random(m_nMisslesGenerateData);
		}break;
		
	case SKILL_MGS_CENTEREXTENDLINE:
		{
			if (m_nChildSkillNum <= 1) return m_nWaitTime;
			int nCenter = m_nChildSkillNum / 2	;
			return m_nWaitTime + abs(nNo - nCenter) * m_nMisslesGenerateData ;
		}
	}
	return m_nWaitTime;
}

int KSkill::GetSkillIdFromName(char * szSkillName)  
{
	//	
	if (!szSkillName || !szSkillName[0]) 
        return -1;
	
	for (int i = 0; i < MAX_SKILL; i ++)
	{
		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(i, 1);
		if (pOrdinSkill) 
		{
			if (!strcmp(pOrdinSkill->m_szName, szSkillName))
            {
                return i;
            }
		}
	}
	return -1;
	
}


/*!*****************************************************************************
// Function		: KSkill::CastInitiativeSkill
// Purpose		: 主动辅助技能
// Return		: BOOL 
// Argumant		: int nLauncher
// Argumant		: int nParam1
// Argumant		: int nParam2
// Argumant		: int nWaitTime
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL KSkill::CastInitiativeSkill(int nLauncher, int nParam1, int nParam2, int nWaitTime)  const 
{
	
#ifdef _SERVER
	//对自已的主动辅助技能
	if (nParam1 != -1 && m_bTargetSelf) 
	{
		nParam1 = -1;
		nParam2 = nLauncher;
	}
	else
	{
		if (nParam1 != -1 || nParam2 <= 0 || nParam2 >= MAX_NPC) return FALSE;
		
		NPC_RELATION  Relation = NpcSet.GetRelation(nLauncher, nParam2);
		
		if (m_bTargetEnemy)
		{
			if (Relation & relation_enemy) 
				goto lab_processdamage;
		}
		
		if (m_bTargetAlly)
		{
			if (Relation & relation_ally) 
				goto lab_processdamage;
		}
		
		if (m_bTargetSelf)
		{
			if (Relation & relation_self) 
				goto lab_processdamage;
		}
		return FALSE;
		
	}
	
lab_processdamage:			
	
	KMissleMagicAttribsData * pAttribsData = CreateMissleMagicAttribsData(nLauncher);
	if (pAttribsData) 
	{
		if (Npc[nParam2].ReceiveDamage(nLauncher, m_bIsMelee, pAttribsData->m_pDamageMagicAttribs, m_bUseAttackRate, m_bDoHurt))
		{
			if (m_eRelation & relation_enemy && g_RandPercent(Npc[nParam2].m_CurrentBuffEmtyRetPercent.nValue[0]))
			{

			if (pAttribsData->m_nStateMagicAttribsNum > 0)
			{

				KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(Npc[nParam2].m_CurrentBuffEmtyRetPercent.nValue[1],Npc[nParam2].m_CurrentBuffEmtyRetPercent.nValue[2]);
                if (pSkillActive)
				{
				KMagicAttrib* pMagicAttrib = pSkillActive->GetStateAttribs();
				int pMagicAttribNum = pSkillActive->GetStateAttribsNum();
				Npc[nParam2].SetStateSkillEffect(nParam2, Npc[nParam2].m_CurrentBuffEmtyRetPercent.nValue[1], Npc[nParam2].m_CurrentBuffEmtyRetPercent.nValue[2], pMagicAttrib, pMagicAttribNum, pMagicAttrib[0].nValue[1]);
				}


				Npc[nLauncher].SetStateSkillEffect(nParam2, m_nId, m_ulLevel, pAttribsData->m_pStateMagicAttribs, pAttribsData->m_nStateMagicAttribsNum, pAttribsData->m_pStateMagicAttribs[0].nValue[1]);
			
			}

			}

			else if (m_eRelation & relation_enemy && g_RandPercent(Npc[nParam2].m_CurrentBuffEmtyResistPercent.nValue[0]))
			{

			if (pAttribsData->m_nStateMagicAttribsNum > 0)
			{

				KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(Npc[nParam2].m_CurrentBuffEmtyResistPercent.nValue[1],Npc[nParam2].m_CurrentBuffEmtyResistPercent.nValue[2]);
                if (pSkillActive)
				{
				KMagicAttrib* pMagicAttrib = pSkillActive->GetStateAttribs();
				int pMagicAttribNum = pSkillActive->GetStateAttribsNum();
				Npc[nParam2].SetStateSkillEffect(nParam2, Npc[nParam2].m_CurrentBuffEmtyResistPercent.nValue[1], Npc[nParam2].m_CurrentBuffEmtyResistPercent.nValue[2], pMagicAttrib, pMagicAttribNum, pMagicAttrib[0].nValue[1]);
				}
			
			}

			}


			else
			{

			if (pAttribsData->m_nStateMagicAttribsNum > 0)
			{

				Npc[nParam2].SetStateSkillEffect(nLauncher, m_nId, m_ulLevel, pAttribsData->m_pStateMagicAttribs, pAttribsData->m_nStateMagicAttribsNum, pAttribsData->m_pStateMagicAttribs[0].nValue[1]);

				for (int i=0;i<pAttribsData->m_nStateMagicAttribsNum;i++)
				{
				if (pAttribsData->m_pStateMagicAttribs[i].nAttribType == magic_doskillbuff1 || pAttribsData->m_pStateMagicAttribs[i].nAttribType == magic_doskillbuff2 || pAttribsData->m_pStateMagicAttribs[i].nAttribType == magic_doskillbuff3 || pAttribsData->m_pStateMagicAttribs[i].nAttribType == magic_doskillbuff4 || pAttribsData->m_pStateMagicAttribs[i].nAttribType == magic_doskillbuff5)
				{

				int nLevel = Npc[nLauncher].m_SkillList.GetCurrentLevel(pAttribsData->m_pStateMagicAttribs[i].nValue[0]);
			    if (nLevel > pAttribsData->m_pStateMagicAttribs[i].nValue[1])
                nLevel = pAttribsData->m_pStateMagicAttribs[i].nValue[1];
				if (nLevel >0)
				{
                KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(pAttribsData->m_pStateMagicAttribs[i].nValue[0],nLevel);
                if (pSkillActive)
				{
				KSkill * pSkillActive1 = (KSkill *) g_SkillManager.GetSkill(pSkillActive->GetChildSkillId(),nLevel);
				if (pSkillActive1)
				{
				KMagicAttrib* pMagicAttrib1 = pSkillActive1->GetStateAttribs();
				int pMagicAttribNum1 = pSkillActive1->GetStateAttribsNum();
				Npc[nParam2].SetStateSkillEffect(nLauncher, pSkillActive->GetChildSkillId(), nLevel, pMagicAttrib1, pMagicAttribNum1, pMagicAttrib1[0].nValue[1]);
				}
				}
				}

				}
				}


			}
			}
		
			if (pAttribsData->m_nImmediateMagicAttribsNum > 0)
				Npc[nParam2].SetImmediatelySkillEffect(nLauncher, pAttribsData->m_pImmediateAttribs, pAttribsData->m_nImmediateMagicAttribsNum);
			
		}
		return TRUE;
	}
	if (pAttribsData->DelRef() == 0)
		delete pAttribsData;
#endif //_SERVER
	return TRUE;
}

/*!*****************************************************************************
// Function		: KSkill::CastPassivitySkill
// Purpose		: 被动
// Return		: BOOL 
// Argumant		: int nLauncher
// Argumant		: int nParam1
// Argumant		: int nParam2
// Argumant		: int nWaitTime
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL KSkill::CastPassivitySkill(int nLauncher, int nParam1, int nParam2, int nWaitTime)  const 
{
#ifdef _SERVER
	//是被动技能时，是否还需要生成MissleMagicAttribs?
	KMissleMagicAttribsData * pAttribsData = (KMissleMagicAttribsData*)m_StateAttribs;//CreateMissleMagicAttribsData(nLauncher);
	if (m_nStateAttribsNum > 0)
	{
		Npc[nLauncher].SetStateSkillEffect(nLauncher, m_nId, m_ulLevel, (KMagicAttrib *)m_StateAttribs, m_nStateAttribsNum, -1);
	}
#endif
	return TRUE;
}

/*!*****************************************************************************
// Function		: KSkill::ParseString2MagicAttrib
// Purpose		: 解析通过脚本运算获得的技能数据
// Return		: 
// Argumant		: char * szMagicAttribName
// Argumant		: char * szValue
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL	KSkill::ParseString2MagicAttrib(unsigned long ulLevel, char * szMagicAttribName, char * szValue)  
{
	int nValue1 = 0;
	int nValue2 = 0;
	int nValue3 = 0;
    const char *pcszTemp = NULL;
	if ((!szMagicAttribName) || (!szMagicAttribName[0])) return FALSE;
	//nValue2 当值为-1时为永久性状态，0为非状态，其它值为有时效性状态魔法效果
	//需要将状态数据与非状态数据分离出来，放入相应的数组内，并记录总数量
	
	for (int i  = 0 ; i <= magic_pass_new_end; i ++)
	{
		if (!strcmp(szMagicAttribName, MagicAttrib2String(i)))
		{
            pcszTemp = szValue;
            nValue1 = KSG_StringGetInt(&pcszTemp, 0);
            KSG_StringSkipSymbol(&pcszTemp, ',');
            nValue2 = KSG_StringGetInt(&pcszTemp, 0);
            KSG_StringSkipSymbol(&pcszTemp, ',');
            nValue3 = KSG_StringGetInt(&pcszTemp, 0);
			//sscanf(szValue, "%d,%d,%d", &nValue1, &nValue2, &nValue3);


			if (i > magic_missle_begin && i < magic_missle_end)
			{
				m_MissleAttribs[m_nMissleAttribsNum].nAttribType = i;
				m_MissleAttribs[m_nMissleAttribsNum].nValue[0] = nValue1;
				m_MissleAttribs[m_nMissleAttribsNum].nValue[1] = nValue2;
				m_MissleAttribs[m_nMissleAttribsNum].nValue[2] = nValue3;
				m_nMissleAttribsNum ++;
				return TRUE;
			}
			if (i > magic_skill_begin && i < magic_skill_end)
			{
				switch(i)
				{
				case magic_skill_cost_v:				// 消耗MANA
					{
						m_nCost = nValue1;
					}
					break;
					
				case magic_skill_costtype_v:
					{
						m_nSkillCostType = (NPCATTRIB)nValue1;
					}break;
					
				case magic_skill_mintimepercast_v: 		// 每次发魔法的间隔时间
					{
						m_nMinTimePerCast = nValue1;
						m_nMinTimePerCastHorse =  nValue2;
					}break;
					
				case magic_skill_misslenum_v:
					{
						m_nChildSkillNum = nValue1;
					}break;
					
				case magic_skill_misslesform_v:
					{
						m_eMisslesForm = (eMisslesForm) nValue1;
					}break;
				case magic_skill_param1_v:
					{
						m_nValue1 = nValue1;
					}
					break;
				case magic_skill_param2_v:	
					{
						m_nValue2 = nValue2;
					}break;
				case magic_skill_eventskilllevel:
					{
						m_nEventSkillLevel = nValue1;
					}
				}
				return TRUE;
			}
			
			if ((i > magic_damage_begin && i < magic_damage_end) || (i > magic_dame_new_begin && i < magic_dame_new_end))
			{
				switch(i)
				{
				case magic_attackrating_v:
				case magic_attackrating_p:
					m_DamageAttribs[0].nAttribType = i;
					m_DamageAttribs[0].nValue[0] = nValue1;
					m_DamageAttribs[0].nValue[1] = nValue2;
					m_DamageAttribs[0].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_ignoredefense_p:
					m_DamageAttribs[1].nAttribType = i;
					m_DamageAttribs[1].nValue[0] = nValue1;
					m_DamageAttribs[1].nValue[1] = nValue2;
					m_DamageAttribs[1].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_physicsdamage_v:
				case magic_physicsenhance_p:
					m_DamageAttribs[2].nAttribType = i;
					m_DamageAttribs[2].nValue[0] = nValue1;
					m_DamageAttribs[2].nValue[1] = nValue2;
					m_DamageAttribs[2].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_colddamage_v:
				case magic_coldenhance_p:
					m_DamageAttribs[3].nAttribType = i;
					m_DamageAttribs[3].nValue[0] = nValue1;
					m_DamageAttribs[3].nValue[1] = nValue2;
					m_DamageAttribs[3].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_firedamage_v:
				case magic_fireenhance_p:
					m_DamageAttribs[4].nAttribType = i;
					m_DamageAttribs[4].nValue[0] = nValue1;
					m_DamageAttribs[4].nValue[1] = nValue2;
					m_DamageAttribs[4].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_lightingdamage_v:
				case magic_lightingenhance_p:
					m_DamageAttribs[5].nAttribType = i;
					m_DamageAttribs[5].nValue[0] = nValue1;
					m_DamageAttribs[5].nValue[1] = nValue2;
					m_DamageAttribs[5].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_poisondamage_v:
				case magic_poisonenhance_p:
					m_DamageAttribs[6].nAttribType = i;
					m_DamageAttribs[6].nValue[0] = nValue1;
					m_DamageAttribs[6].nValue[1] = nValue2;
					m_DamageAttribs[6].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_magicdamage_v:
				case magic_magicenhance_p:
					m_DamageAttribs[7].nAttribType = i;
					m_DamageAttribs[7].nValue[0] = nValue1;
					m_DamageAttribs[7].nValue[1] = nValue2;
					m_DamageAttribs[7].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_stun_p:
					m_DamageAttribs[8].nAttribType = i;
					m_DamageAttribs[8].nValue[0] = nValue1;
					m_DamageAttribs[8].nValue[1] = nValue2;
					m_DamageAttribs[8].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_deadlystrike_p:
					m_DamageAttribs[9].nAttribType = i;
					m_DamageAttribs[9].nValue[0] = nValue1;
					m_DamageAttribs[9].nValue[1] = nValue2;
					m_DamageAttribs[9].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_fatallystrike_p:
					m_DamageAttribs[10].nAttribType = i;
					m_DamageAttribs[10].nValue[0] = nValue1;
					m_DamageAttribs[10].nValue[1] = nValue2;
					m_DamageAttribs[10].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_steallife_p:
					m_DamageAttribs[11].nAttribType = i;
					m_DamageAttribs[11].nValue[0] = nValue1;
					m_DamageAttribs[11].nValue[1] = nValue2;
					m_DamageAttribs[11].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_stealmana_p:
					m_DamageAttribs[12].nAttribType = i;
					m_DamageAttribs[12].nValue[0] = nValue1;
					m_DamageAttribs[12].nValue[1] = nValue2;
					m_DamageAttribs[12].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_seriesdame:
					m_DamageAttribs[13].nAttribType = i;
					m_DamageAttribs[13].nValue[0] = nValue1;
					m_DamageAttribs[13].nValue[1] = nValue2;
					m_DamageAttribs[13].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
				    break;
				case magic_addskilldame1:
					m_addskillsdame[0][0] = nValue1;
					m_addskillsdame[0][1] = nValue2;
					break;
				case magic_addskilldame2:
	                m_addskillsdame[1][0] = nValue1;
					m_addskillsdame[1][1] = nValue2;
					break;
				case magic_addskilldame3:
	                m_addskillsdame[2][0] = nValue1;
					m_addskillsdame[2][1] = nValue2;
					break;

				case magic_addskilldame4:
	                m_addskillsdame[3][0] = nValue1;
					m_addskillsdame[3][1] = nValue2;
					break;
				case magic_addskilldame5:
	                m_addskillsdame[4][0] = nValue1;
					m_addskillsdame[4][1] = nValue2;
					break;
				case magic_addskilldame6:
	                m_addskillsdame[5][0] = nValue1;
					m_addskillsdame[5][1] = nValue2;
					break;
				case magic_skilllevelexp:
					m_DamageAttribs[14].nAttribType = i;
					m_DamageAttribs[14].nValue[0] = nValue1;
					m_DamageAttribs[14].nValue[1] = nValue2;
					m_DamageAttribs[14].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_skilladdmissles:
					if (nValue1 == 1)
					{
					m_addskillmisslesrun = nValue2;
					}
					break;

				case magic_nstatus_p:
					m_DamageAttribs[15].nAttribType = i;
					m_DamageAttribs[15].nValue[0] = nValue1;
					m_DamageAttribs[15].nValue[1] = nValue2;
					m_DamageAttribs[15].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_invisibility_p:
					m_DamageAttribs[16].nAttribType = i;
					m_DamageAttribs[16].nValue[0] = nValue1;
					m_DamageAttribs[16].nValue[1] = nValue2;
					m_DamageAttribs[16].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_madness_p:
					m_DamageAttribs[17].nAttribType = i;
					m_DamageAttribs[17].nValue[0] = nValue1;
					m_DamageAttribs[17].nValue[1] = nValue2;
					m_DamageAttribs[17].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				}
				return TRUE;
			}
			
			//剩下的为数据计算时的数据参数
			//根据nValue2值决定状态参数还是非状态参数
			if (nValue2 == 0) 
			{
				m_ImmediateAttribs[m_nImmediateAttribsNum].nAttribType = i;
				m_ImmediateAttribs[m_nImmediateAttribsNum].nValue[0] = nValue1;
				m_ImmediateAttribs[m_nImmediateAttribsNum].nValue[1] = nValue2;
				m_ImmediateAttribs[m_nImmediateAttribsNum].nValue[2] = nValue3;
				m_nImmediateAttribsNum ++;
				return TRUE;
				
			}
			else
			{
				m_StateAttribs[m_nStateAttribsNum].nAttribType = i;
				m_StateAttribs[m_nStateAttribsNum].nValue[0] = nValue1;
				m_StateAttribs[m_nStateAttribsNum].nValue[1] = nValue2;
				m_StateAttribs[m_nStateAttribsNum].nValue[2] = nValue3;
				m_nStateAttribsNum ++;
				return TRUE;
			}
			
		}
	}
	return FALSE;
}

const char * KSkill::MagicAttrib2String(int MagicAttrib)  const 
{
	return 	g_MagicID2String(MagicAttrib);
}

#ifndef _SERVER
void	KSkill::DrawSkillIcon(int x, int y, int Width, int Height)  
{
	
	if (!m_szSkillIcon[0]) return ;
	
	m_RUIconImage.nType = ISI_T_SPR;
	m_RUIconImage.Color.Color_b.a = 255;
	m_RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	m_RUIconImage.uImage = 0;
	m_RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	m_RUIconImage.bRenderFlag = 0;
	strcpy(m_RUIconImage.szImage, m_szSkillIcon);
	m_RUIconImage.oPosition.nX = x;
	m_RUIconImage.oPosition.nY = y;
	m_RUIconImage.oPosition.nZ = 0;
	m_RUIconImage.nFrame = 0;
	g_pRepresent->DrawPrimitives(1, &m_RUIconImage, RU_T_IMAGE, 1);

    DWORD BeginCast = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetBeginCastTime(m_nId);
	DWORD FinishCast = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetNextCastTime(m_nId);
    DWORD HCast = FinishCast - BeginCast;
	if (BeginCast >= 0 && FinishCast >= 0 && (HCast/18) > 0)
	{
    
	if (SubWorld[0].m_dwCurrentTime >= BeginCast && SubWorld[0].m_dwCurrentTime < FinishCast)
	{




	KRUShadow	Blood;

	Blood.Color.Color_dw = 0x0f999999;
	Blood.oPosition.nX = x;
	Blood.oPosition.nY = y;
	Blood.oPosition.nZ = 0;
	Blood.oEndPos.nX = x + 36;
	Blood.oEndPos.nY = y + (FinishCast - SubWorld[0].m_dwCurrentTime) * 36 / HCast;
	Blood.oEndPos.nZ = 0;

    	
	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, TRUE);

	

	}

	}


}

void	KSkill::GetDesc(unsigned long ulSkillId, unsigned long ulLevel,unsigned long ulCurLevel, char * pszMsg, int nOwnerIndex,  bool bGetNextLevelDesc)
{
	
	if (!pszMsg) return;
	if (nOwnerIndex <= 0 )	return ;
	strcpy(pszMsg,"");
	char szTemp[300];
	
	KSkill * pTempSkill = NULL;
	KSkill * pCurSkill = NULL;
	KSkill * pNextSkill = NULL;
	
	if(ulCurLevel == 0)
	{
		pNextSkill = (KSkill *)g_SkillManager.GetSkill(ulSkillId, 1);
		pTempSkill = pNextSkill;
	}
	else
	{
		pCurSkill = (KSkill *) g_SkillManager.GetSkill(ulSkillId, ulCurLevel);
		pNextSkill = (KSkill *) g_SkillManager.GetSkill(ulSkillId, ulCurLevel + 1);
		pTempSkill = pCurSkill;
	}
	
	if (pTempSkill == NULL)
	{
		return;
	}
	
	strcat(pszMsg, "<color=Yellow>");
	strcat(pszMsg, " ");
	strcat(pszMsg, pTempSkill->m_szName);
	strcat(pszMsg, " ");
	strcat(pszMsg, "\n<bclr=Black><color>");
	
	
#ifdef SHOW_SKILL_MORE_INFO
	switch(pTempSkill->m_eSkillStyle)
	{
	case SKILL_SS_Missles:
		{
			strcat(pszMsg, "子弹技\n");
			szTemp[0] = 0;
			switch(pTempSkill->m_eMisslesForm)
			{
			case SKILL_MF_Wall:
				{
					sprintf(szTemp, "墙形 数量%d\n", pTempSkill->m_nChildSkillNum);
				}break;			//墙形	多个子弹呈垂直方向排列，类式火墙状
				
			case SKILL_MF_Line:
				{
					sprintf(szTemp, "线形 数量%d\n", pTempSkill->m_nChildSkillNum);
				}break;					//线形	多个子弹呈平行于玩家方向排列
			case SKILL_MF_Spread:
				{
					sprintf(szTemp, "散形 数量%d\n", pTempSkill->m_nChildSkillNum);
				}break;				//散形	多个子弹呈一定的角度的发散状	
			case SKILL_MF_Circle:
				{
					sprintf(szTemp, "圆形发散 数量%d\n", pTempSkill->m_nChildSkillNum);
				}break;				//圆形	多个子弹围成一个圈
			case SKILL_MF_Random:{}break;				//随机	多个子弹随机排放
			case SKILL_MF_Zone:
				{
					if (pTempSkill->m_nValue1)
						sprintf(szTemp, "圆形区域 数量%d\n", pTempSkill->m_nChildSkillNum);
					else 
						sprintf(szTemp, "方形区域 数量%d\n", pTempSkill->m_nChildSkillNum);
					
				}break;					//区域	多个子弹放至在某个范围内
			case SKILL_MF_AtTarget:
				{
					if (pTempSkill->m_nValue1)
						sprintf(szTemp, "定点圆形区域 数量%d\n", pTempSkill->m_nChildSkillNum);
					else 
						sprintf(szTemp, "定点方形区域 数量%d\n", pTempSkill->m_nChildSkillNum);
				}break;				//定点	多个子弹根据
			case SKILL_MF_AtFirer:
				{
					if (pTempSkill->m_nValue1)
						sprintf(szTemp, "定点圆形区域 数量%d\n", pTempSkill->m_nChildSkillNum);
					else 
						sprintf(szTemp, "定点方形区域 数量%d\n", pTempSkill->m_nChildSkillNum);
					
				}break;				//本身	多个子弹停在玩家当前位置
			}
			strcat(pszMsg, szTemp);
			
		}break;			//	子弹类		本技能用于发送子弹类
		
	case SKILL_SS_Melee:
		{
			strcat(pszMsg, "格斗技\n");
		}break;
	case SKILL_SS_InitiativeNpcState:
		{
			strcat(pszMsg, "主动辅助武功\n");			
			if (pTempSkill->m_StateAttribs[0].nValue[1] > 0)
			{
				sprintf (szTemp, "状态持续时间:%d\n" ,pTempSkill->m_StateAttribs[0].nValue[1]);
				strcat(pszMsg,szTemp);
			}
		}break;	//	主动类		本技能用于改变当前Npc的主动状态
	case SKILL_SS_PassivityNpcState:
		{
			strcat(pszMsg, "被动辅助武功\n");
		}break;		//	被动类		本技能用于改变Npc的被动状态
		
		
	case SKILL_SS_CreateNpc:{}break;				//	产生Npc类	本技能用于生成一个新的Npc
	case SKILL_SS_BuildPoison:{}break;			//	炼毒类		本技能用于炼毒
	case SKILL_SS_AddPoison:{}break;				//	加毒类		本技能用于给武器加毒性
	case SKILL_SS_GetObjDirectly:{}break;		//	取物类		本技能用于隔空取物
	case SKILL_SS_StrideObstacle :{}break;		//	跨越类		本技能用于跨越障碍
	case SKILL_SS_BodyToObject:{}break;			//	变物类		本技能用于将尸体变成宝箱
	case SKILL_SS_Mining:{}break;				//	采矿类		本技能用于采矿随机生成矿石
	case SKILL_SS_RepairWeapon:{}break;			//	修复类		本技能用于修复装备
	case SKILL_SS_Capture:{}break;				//	捕捉类		本技能用于捕捉动物Npc
	}
	if (g_MisslesLib[pTempSkill->m_nChildSkillId].m_bRangeDamage) strcat(pszMsg, "区域伤害 ");
	
	switch(g_MisslesLib[pTempSkill->m_nChildSkillId].m_eMoveKind)
	{
	case MISSLE_MMK_Stand:
		{
			strcat(pszMsg, "原地 ");
		}break;							//	原地
	case MISSLE_MMK_Line:
		{
			strcat(pszMsg, "直线飞行 ");
		}break;							//	直线飞行
	case MISSLE_MMK_Random:{}break;							//	随机飞行（暗黑二女巫的Charged Bolt）
	case MISSLE_MMK_Circle:
		{
			strcat(pszMsg, "环形飞行 ");
		}break;							//	环行飞行（围绕在身边，暗黑二刺客的集气）
	case MISSLE_MMK_Helix:
		{
			strcat(pszMsg, "环形飞行 ");
		}break;							//	阿基米德螺旋线（暗黑二游侠的Bless Hammer）
	case MISSLE_MMK_Follow:{}break;							//	跟踪目标飞行
	case MISSLE_MMK_Motion:{}break;							//	玩家动作类
	case MISSLE_MMK_Parabola:
		{
			strcat(pszMsg, "抛物飞行 ");
		}break;						//	抛物线
	case MISSLE_MMK_SingleLine:{}break;						//	必中的单一直线飞行魔法
	}
	
	if (!g_MisslesLib[pTempSkill->m_nChildSkillId].m_bCollideVanish)
		strcat(pszMsg, "穿透 ");
	if (g_MisslesLib[pTempSkill->m_nChildSkillId].m_nDamageRange > 1) 
	{
		sprintf(szTemp, "伤害范围:%d ", g_MisslesLib[pTempSkill->m_nChildSkillId].m_nDamageRange);
		strcat(pszMsg, szTemp);
	}
	if (g_MisslesLib[pTempSkill->m_nChildSkillId].m_nKnockBack)
	{
		sprintf(szTemp, "震退距离:%d ", g_MisslesLib[pTempSkill->m_nChildSkillId].m_nKnockBack);
		strcat(pszMsg, szTemp);
	}
	if (g_MisslesLib[pTempSkill->m_nChildSkillId].m_bAutoExplode)
	{
		strcat(pszMsg, "消亡自爆 ");
	}
	
	if (pTempSkill->m_bIsAura) strcat(pszMsg, "光环 ");
	if (pTempSkill->m_bIsPhysical) strcat(pszMsg, "物理 ");
	if (pTempSkill->m_bIsMelee) strcat(pszMsg, "近身 ");
	if (pTempSkill->m_bTargetOnly) strcat(pszMsg, "必中 ");
	if (pTempSkill->m_bTargetAlly) strcat(pszMsg, "对友 ");
	if (pTempSkill->m_bTargetEnemy) strcat(pszMsg, "对敌 ");
	if (pTempSkill->m_bTargetObj)	  strcat(pszMsg, "对物 ");
	if (pTempSkill->m_bTargetSelf) strcat(pszMsg, "对已 ");
	if (pTempSkill->m_bUseAttackRate) strcat(pszMsg, "考虑命中率 ");
#endif
	
	strcat (pszMsg, "\n");
	strcat(pszMsg, " ");
	strcat(pszMsg, pTempSkill->m_szSkillDesc);
	strcat(pszMsg, " ");
	strcat(pszMsg, "\n\n");
	
	if (!pTempSkill->IsPhysical())
	{
		
		if (ulCurLevel != ulLevel)
		{
		strcat(pszMsg, "<color=blue>");
		sprintf(szTemp, " ��ng c�p hi�n th�i: %d (%d+%d)", ulCurLevel,ulLevel,ulCurLevel-ulLevel);
		strcat(pszMsg, szTemp);
		strcat(pszMsg, "<color>");
		strcat(pszMsg, "\n");
		strcat(pszMsg, "\n");
		}
		else
		{
		sprintf(szTemp, " ��ng c�p hi�n th�i: %d ", ulCurLevel);
		strcat(pszMsg, szTemp);
		strcat(pszMsg, "\n");
		strcat(pszMsg, "\n");
		}
	}
	
	int i = 0;

	if (pCurSkill)
	{
		pCurSkill->GetDescAboutLevel(pszMsg,ulCurLevel,nOwnerIndex);
	}


				
if (pTempSkill->m_nEquiptLimited == 0)
{
strcat(pszMsg, "\n");
strcat(pszMsg, " H�n ch� v� kh� : ");
strcat(pszMsg, "<color=wood>");
strcat(pszMsg, "Tr��ng Ki�m \n");
strcat(pszMsg, "<color>");

}
else if (pTempSkill->m_nEquiptLimited == 1)
{
strcat(pszMsg, "\n");
strcat(pszMsg, " H�n ch� v� kh� : ");
strcat(pszMsg, "<color=wood>");
strcat(pszMsg, "��n �ao \n");
strcat(pszMsg, "<color>");

}
else if (pTempSkill->m_nEquiptLimited == 2)
{
strcat(pszMsg, "\n");
strcat(pszMsg, " H�n ch� v� kh� : ");
strcat(pszMsg, "<color=wood>");
strcat(pszMsg, "C�n B�ng \n");
strcat(pszMsg, "<color>");

}
else if (pTempSkill->m_nEquiptLimited == 3)
{
strcat(pszMsg, "\n");
strcat(pszMsg, " H�n ch� v� kh� : ");
strcat(pszMsg, "<color=wood>");
strcat(pszMsg, "Th��ng k�ch \n");
strcat(pszMsg, "<color>");

}
else if (pTempSkill->m_nEquiptLimited == 4)
{
strcat(pszMsg, "\n");
strcat(pszMsg, " H�n ch� v� kh� : ");
strcat(pszMsg, "<color=wood>");
strcat(pszMsg, "Song Chu� \n");
strcat(pszMsg, "<color>");

}
else if (pTempSkill->m_nEquiptLimited == 5)
{
strcat(pszMsg, "\n");
strcat(pszMsg, " H�n ch� v� kh� : ");
strcat(pszMsg, "<color=wood>");
strcat(pszMsg, "Song �ao \n");
strcat(pszMsg, "<color>");

}
else if (pTempSkill->m_nEquiptLimited == 9)
{
strcat(pszMsg, "\n");
strcat(pszMsg, " H�n ch� v� kh� : ");
strcat(pszMsg, "<color=wood>");
strcat(pszMsg,"Tay Kh�ng \n");
strcat(pszMsg, "<color>");

}
	if (pTempSkill->m_nHorseLimited)
	{
		switch(pTempSkill->m_nHorseLimited)
		{
		case 1:
			{
				strcat(pszMsg, "\n");
				strcat(pszMsg, " Kh�ng th� s� d�ng khi tr�n ng�a \n");
			}
			break;
		case 2:
			{
				strcat(pszMsg, "\n");
				strcat(pszMsg, "\n");
			}
			break;
		default:
			return ;
		}
	}

	if (!pTempSkill->IsPhysical())
	{
		if (bGetNextLevelDesc)
		{
			if (pNextSkill)
			{
				strcat(pszMsg, "\n <color=Red>��ng c�p k� ti�p \n");
				pNextSkill->GetDescAboutLevel(pszMsg,ulCurLevel,nOwnerIndex);
			}
			else
			{
				
			}
		}
	}
}

void KSkill::GetDescAboutLevel(char * pszMsg,int pszlevel,int nOwnerIndex)
{
	
	char szTemp[100];
	int nGetCost = GetSkillCost(NULL);

	if (nGetCost)
	{
		switch(m_nSkillCostType)
		{
		case attrib_mana:	
			sprintf(szTemp, " Ti�u hao n�i l�c: %d \n", nGetCost);
			strcat(pszMsg,szTemp);
			break;
		case attrib_stamina:
			sprintf(szTemp, " Ti�u hao th� l�c: %d \n", nGetCost);
			strcat(pszMsg,szTemp);
			break;
		case attrib_life:
			sprintf(szTemp, " Ti�u hao sinh l�c: %d \n", nGetCost);
			strcat(pszMsg,szTemp);
			break;
		}
	}
	
	int nGetAttackRadius = GetAttackRadius();
	if (nGetAttackRadius)
	{
		sprintf(szTemp," Ph�m vi hi�u qu�: %d \n", nGetAttackRadius);
		strcat(pszMsg,szTemp);
	}


   	int	DelaySkillCast = GetDelayPerCast()/18;
	int	DelaySkillCastHorse = GetDelayPerCastHorse()/18;

	if (DelaySkillCast > 0)
	{
		strcat(pszMsg,"");
		sprintf(szTemp,"Th�i gian ph�c h�i chi�u th�c: %d", DelaySkillCast);
		strcat(pszMsg,szTemp);
		strcat(pszMsg," gi�y\n");
		strcat(pszMsg,"");
	}

	if (DelaySkillCastHorse > 0)
	{
		strcat(pszMsg,"");
		sprintf(szTemp,"Th�i gian ph�c h�i chi�u th�c tr�n ng�a: %d", DelaySkillCastHorse);
		strcat(pszMsg,szTemp);
		strcat(pszMsg," gi�y\n");
		strcat(pszMsg,"");
	}

	for (int i  = 0; i < MAX_MISSLE_DAMAGEATTRIB; i ++)
	{
		if (m_DamageAttribs[i].nAttribType == magic_skilllevelexp)
		{
		if (m_DamageAttribs[i].nValue[0] > 0 && m_DamageAttribs[i].nValue[2] > 0)
		{
		int ExpPecentUpLevel = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(m_DamageAttribs[i].nValue[2])*100/m_DamageAttribs[i].nValue[0];
        strcat(pszMsg, " ");
        strcat(pszMsg, "Luy�n t�p k� n�ng: ");
        sprintf(pszMsg,"%s%d",pszMsg,ExpPecentUpLevel);
        strcat(pszMsg, "% ");
        strcat(pszMsg, "\n");
		}
		}
		else if (m_DamageAttribs[i].nAttribType == magic_nstatus_p)
		{
		if (m_DamageAttribs[i].nValue[0] > 0)
		{
		int PecentEffec = m_DamageAttribs[i].nValue[0];
		int TimeEffec = m_DamageAttribs[i].nValue[1]/18;
        strcat(pszMsg, " ");
        strcat(pszMsg, "X�c su�t ");
        sprintf(pszMsg,"%s%d",pszMsg,PecentEffec);
        strcat(pszMsg, "% lo�i b� tr�ng th�i kh�ng t�t cho b�n th�n v� ��ng ��i. Duy tr� k�o d�i ");
		sprintf(pszMsg,"%s%d",pszMsg,TimeEffec);
		strcat(pszMsg, " gi�y ");
        strcat(pszMsg, "\n");
		}
		}
		else if (m_DamageAttribs[i].nAttribType == magic_invisibility_p)
		{
		if (m_DamageAttribs[i].nValue[0] > 0)
		{
		int TimeEffec = m_DamageAttribs[i].nValue[0]/18;
        strcat(pszMsg, " ");
        strcat(pszMsg, "Tr�ng th�i �n th�n trong v�ng ");
        sprintf(pszMsg,"%s%d",pszMsg,TimeEffec);
		strcat(pszMsg, " gi�y. Khi thi tri�n v� c�ng kh�ng th� duy tr� ");
        strcat(pszMsg, "\n");
		}
		}
		else if (m_DamageAttribs[i].nAttribType == magic_madness_p)
		{
		if (m_DamageAttribs[i].nValue[0] > 0)
		{
		int PecentEffec = m_DamageAttribs[i].nValue[0];
		int TimeEffec = m_DamageAttribs[i].nValue[1]/18;
        strcat(pszMsg, " ");
        strcat(pszMsg, "X�c su�t ");
        sprintf(pszMsg,"%s%d",pszMsg,PecentEffec);
        strcat(pszMsg, "% l�m cho m�c ti�u r�i v�o tr�ng th�i ho�ng lo�n trong ");
		sprintf(pszMsg,"%s%d",pszMsg,TimeEffec);
		strcat(pszMsg, " gi�y\n");
		strcat(pszMsg, " ");
		strcat(pszMsg, "Sau khi ch�t, l�m cho ��i ph�ng ho�ng lo�n trong ");
		sprintf(pszMsg,"%s%d",pszMsg,TimeEffec);
		strcat(pszMsg, " gi�y ");
        strcat(pszMsg, "\n");
		}
		}
		
	}






int CurrentSkillAddDameActive = 0;

if (m_nId > 0 && m_nId <= 500)
{
CurrentSkillAddDameActive = Npc[nOwnerIndex].m_SkillList.m_CurrentListSkillAddDame[m_nId-1];
if (CurrentSkillAddDameActive > 0)
{
strcat(pszMsg, " ");
strcat(pszMsg, "H� tr� t� k� n�ng kh�c: ");
sprintf(pszMsg,"%s%d",pszMsg,CurrentSkillAddDameActive);
strcat(pszMsg, "%");
strcat(pszMsg, " ");
strcat(pszMsg, "\n");
}
}

	//不随等级变化的立即伤害
	for (i  = 0; i < m_nImmediateAttribsNum; i ++)
	{
		if (!m_ImmediateAttribs[i].nAttribType) continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc(&m_ImmediateAttribs[i]);
		if (!pszInfo) continue;
		strcat(pszMsg, " ");
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
	}
	//子弹随玩家属性计算而成的伤害
	//KMagicAttrib *DamageAttribs[MAX_MISSLE_DAMAGEATTRIB];
	KMagicAttrib *DamageAttribs = m_DamageAttribs;
	//根据玩家的基本属性，确定子弹的伤害
	//Npc[nOwnerIndex].AppendSkillEffect(m_DamageAttribs, DamageAttribs);
	for (i  = 0; i < MAX_MISSLE_DAMAGEATTRIB; i ++)
	{
		if (!(DamageAttribs + i)->nAttribType) continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc((DamageAttribs + i));
		if (!pszInfo) continue;
		strcat(pszMsg, " ");
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
	}
	//状态技能效果
	for (i  = 0; i < m_nStateAttribsNum; i ++)
	{
		if (!m_StateAttribs[i].nAttribType) continue;


	if (m_StateAttribs[i].nAttribType == magic_returndoskillemty_p)
		{
        int nLevelRtSkill = m_StateAttribs[i].nValue[2];

        KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(m_StateAttribs[i].nValue[1],1);
		strcat(pszMsg, " ");
		strcat(pszMsg, "Khi b� c�ng k�ch x�c su�t ");
		sprintf(pszMsg,"%s%d",pszMsg,m_StateAttribs[i].nValue[0]);
		strcat(pszMsg, "% thi tri�n k� n�ng ");
		strcat(pszMsg, "<color=blue>[ ");
		strcat(pszMsg, pSkillActive->m_szName);
		strcat(pszMsg, " ]<color>");
        strcat(pszMsg, " ��ng c�p ");
		sprintf(pszMsg,"%s%d",pszMsg,nLevelRtSkill);
		strcat(pszMsg, " b� ��ng");
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
		continue;
		}
	else if(m_StateAttribs[i].nAttribType == magic_doskilleffec_p)
	{
        KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(m_StateAttribs[i].nValue[1],m_StateAttribs[i].nValue[2]);
	    KMagicAttrib * pStateAttribs = pSkillActive->GetStateAttribs(); 
		int pStateAttribsNum = pSkillActive->GetStateAttribsNum();
    int nPecentMana = 0;
	for (int k  = 0; k < pStateAttribsNum; k ++)
	{
	if (!pStateAttribs[k].nAttribType) continue;
	if (pStateAttribs[k].nAttribType == magic_manafromdame_p)
	{
	nPecentMana = pStateAttribs[k].nValue[0];
	}
	}

	int nTimePecentMana = pStateAttribs[0].nValue[1]/18;
			
		strcat(pszMsg, " ");
		strcat(pszMsg, "X�c su�t ");
		sprintf(pszMsg,"%s%d",pszMsg,m_StateAttribs[i].nValue[0]);
		strcat(pszMsg, " khi ��i ph��ng b� ��c s�t r�t ");
		sprintf(pszMsg,"%s%d",pszMsg,nPecentMana);
		strcat(pszMsg, " % n�i l�c.");
		strcat(pszMsg, " K�o d�i trong th�i gian ");
		sprintf(pszMsg,"%s%d",pszMsg,nTimePecentMana);
        strcat(pszMsg, " gi�y");
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
    continue;
	}
	else if(m_StateAttribs[i].nAttribType == magic_doskilleffec1_p)
	{
        KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(m_StateAttribs[i].nValue[1],m_StateAttribs[i].nValue[2]);
	    KMagicAttrib * pStateAttribs = pSkillActive->GetStateAttribs(); 
		int pStateAttribsNum = pSkillActive->GetStateAttribsNum();
    
		int nFireRes = 0;
		int nPhysicRes = 0;
		int nReturDameRange = 0;

	for (int k  = 0; k < pStateAttribsNum; k ++)
	{
	if (!pStateAttribs[k].nAttribType) continue;
	if (pStateAttribs[k].nAttribType == magic_fireres_p)
	{
	nFireRes = -pStateAttribs[k].nValue[0];
	}
	else if (pStateAttribs[k].nAttribType == magic_physicsres_p)
	{
	nPhysicRes = -pStateAttribs[k].nValue[0];
	}
	else if (pStateAttribs[k].nAttribType == magic_rangedamagereturn_p)
	{
	nReturDameRange = -pStateAttribs[k].nValue[0];
	}
	}

	int nTimeEfeec = pStateAttribs[0].nValue[1]/18;
			
		strcat(pszMsg, " ");
		strcat(pszMsg, "X�c su�t ");
		sprintf(pszMsg,"%s%d",pszMsg,m_StateAttribs[i].nValue[0]);
		strcat(pszMsg, "% khi t�n c�ng ch�nh x�c khi�n m�c ti�u gi�m ph�n ��n t�m xa ");
		sprintf(pszMsg,"%s%d",pszMsg,nReturDameRange);
		strcat(pszMsg, "%");
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
		strcat(pszMsg, " ");
		strcat(pszMsg, "Gi�m kh�ng ho� ");
		sprintf(pszMsg,"%s%d",pszMsg,nFireRes);
		strcat(pszMsg, "%, gi�m kh� n�ng ph�ng th� ");
		sprintf(pszMsg,"%s%d",pszMsg,nPhysicRes);
		strcat(pszMsg, "%.");
		strcat(pszMsg, " K�o d�i trong th�i gian ");
		sprintf(pszMsg,"%s%d",pszMsg,nTimeEfeec);
        strcat(pszMsg, " gi�y");
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
    continue;
	}
	else if(m_StateAttribs[i].nAttribType == magic_doskilleffec2_p)
	{
        KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(m_StateAttribs[i].nValue[1],m_StateAttribs[i].nValue[2]);
	    KMagicAttrib * pStateAttribs = pSkillActive->GetStateAttribs(); 
		int pStateAttribsNum = pSkillActive->GetStateAttribsNum();
    
		int nMamaX = 0;
		int nRunSpeed = 0;


	for (int k  = 0; k < pStateAttribsNum; k ++)
	{
	if (!pStateAttribs[k].nAttribType) continue;
	if (pStateAttribs[k].nAttribType == magic_manashieldex_v)
	{
	nMamaX = pStateAttribs[k].nValue[0];
	}
	else if (pStateAttribs[k].nAttribType == magic_fastwalkrun_p)
	{
	nRunSpeed = pStateAttribs[k].nValue[0];
	}
	}

	int nTimeEfeec = pStateAttribs[0].nValue[1]/18;
			
		strcat(pszMsg, " ");
		strcat(pszMsg, "Khi sinh l�c xu�ng ��n 25% c� ");		
		sprintf(pszMsg,"%s%d",pszMsg,m_StateAttribs[i].nValue[0]);
		strcat(pszMsg, "% x�c su�t ph�t ra L��ng Nghi Ch�n Kh�");
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
		strcat(pszMsg, " ");
		strcat(pszMsg, "N�i l�c b�n nh�n l�n ");
		sprintf(pszMsg,"%s%d",pszMsg,nMamaX);
		strcat(pszMsg, " l�n, h� th�n. T�c �� di chuy�n t�ng ");
		sprintf(pszMsg,"%s%d",pszMsg,nRunSpeed);
		strcat(pszMsg, "%.");
		strcat(pszMsg, " Duy tr� trong th�i gian ");
		sprintf(pszMsg,"%s%d",pszMsg,nTimeEfeec);
        strcat(pszMsg, " gi�y");
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
    continue;
	}
	else if(m_StateAttribs[i].nAttribType == magic_timecastskilleffec_v || m_StateAttribs[i].nAttribType == magic_timecastskilleffec1_v || m_StateAttribs[i].nAttribType == magic_timecastskilleffec2_v)
	{
    int nTimeCast1 = m_StateAttribs[i].nValue[0]/18;
	int nTimeCast2 = m_StateAttribs[i].nValue[1]/18;
	if (nTimeCast1 > 0)
	{
	strcat(pszMsg, " ");
	strcat(pszMsg, "Th�i gian ph�c h�i chi�u th�c: ");
	sprintf(pszMsg,"%s%d",pszMsg,nTimeCast1);
	strcat(pszMsg, " gi�y");
	strcat(pszMsg, " ");
	strcat(pszMsg, "\n");
	}

    if (nTimeCast2 > 0)
	{
	strcat(pszMsg, " ");
	strcat(pszMsg, "Th�i gian ph�c h�i chi�u th�c tr�n ng�a: ");
	sprintf(pszMsg,"%s%d",pszMsg,nTimeCast2);
	strcat(pszMsg, " gi�y");
	strcat(pszMsg, " ");
	strcat(pszMsg, "\n");
	}

	}
	else if (m_StateAttribs[i].nAttribType == magic_doskillbuff1 || m_StateAttribs[i].nAttribType == magic_doskillbuff2 || m_StateAttribs[i].nAttribType == magic_doskillbuff3 || m_StateAttribs[i].nAttribType == magic_doskillbuff4 || m_StateAttribs[i].nAttribType == magic_doskillbuff5)
	{

    int nLevelRtSkill = Npc[nOwnerIndex].m_SkillList.GetCurrentLevel(m_StateAttribs[i].nValue[0]);
	if (nLevelRtSkill > m_StateAttribs[i].nValue[1])
        nLevelRtSkill = m_StateAttribs[i].nValue[1];
    KSkill * pSkillActive = (KSkill *) g_SkillManager.GetSkill(m_StateAttribs[i].nValue[0],nLevelRtSkill);


	if (pSkillActive)
	{

	strcat(pszMsg, " ");
    strcat(pszMsg, "<color=blue>");
	strcat(pszMsg, pSkillActive->m_szName);
	strcat(pszMsg, " [");
	sprintf(pszMsg,"%s%d",pszMsg,nLevelRtSkill);
	strcat(pszMsg, " C�p]");
	strcat(pszMsg, "<color>");
    strcat(pszMsg, " ");
	strcat(pszMsg, "\n");
	


	KMagicAttrib * pStateAttribs = pSkillActive->GetStateAttribs(); 
	int pStateAttribsNum = pSkillActive->GetStateAttribsNum();
   
	for (int k  = 0; k < pStateAttribsNum; k ++)
	{
	if (!pStateAttribs[k].nAttribType) continue;
    char * pszInfo = (char *)g_MagicDesc.GetDesc(&pStateAttribs[k]);
		if (!pszInfo) continue;
		strcat(pszMsg, " ");
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
	}
    }
    continue;
	}

		char * pszInfo = (char *)g_MagicDesc.GetDesc(&m_StateAttribs[i]);
		if (!pszInfo) continue;
		strcat(pszMsg, " ");
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
	}








if (pszlevel > 0 && pszlevel < MAX_SKILLLEVEL)
{

KSkill * pSkillActive;
KSkill * pSkillEvent;

int EventCount = 1;
pSkillActive = (KSkill *) g_SkillManager.GetSkill(m_nId,pszlevel);
if (pSkillActive->m_nEventSkillLevel > 0 && pSkillActive->m_nEventSkillLevel < MAX_SKILLLEVEL)
{

if (pSkillActive->m_nStartSkillId > 0 && pSkillActive->m_nStartSkillId < MAX_SKILL)
{
	EventCount++;
	pSkillEvent = (KSkill *) g_SkillManager.GetSkill(pSkillActive->m_nStartSkillId,pSkillActive->m_nEventSkillLevel);

strcat(pszMsg, "\n");
strcat(pszMsg, " ");
strcat(pszMsg, "Th�c th� ");
sprintf(pszMsg,"%s%d",pszMsg,EventCount);
strcat(pszMsg, ": ");
strcat(pszMsg, "<color=blue>");
strcat(pszMsg,pSkillEvent->m_szName);
strcat(pszMsg, " ");
strcat(pszMsg, "<color>");
strcat(pszMsg, "\n");


	KMagicAttrib *DamageAttribs = pSkillEvent->m_DamageAttribs;
	for (i  = 0; i < MAX_MISSLE_DAMAGEATTRIB; i ++)
	{
		if (!(DamageAttribs + i)->nAttribType || (DamageAttribs + i)->nAttribType == magic_seriesdame) continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc((DamageAttribs + i));
		if (!pszInfo) continue;
		strcat(pszMsg, " ");
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
	}
}






if (pSkillActive->m_nFlySkillId > 0 && pSkillActive->m_nFlySkillId < MAX_SKILL)
{
	EventCount++;
	pSkillEvent = (KSkill *) g_SkillManager.GetSkill(pSkillActive->m_nFlySkillId,pSkillActive->m_nEventSkillLevel);

strcat(pszMsg, "\n");
strcat(pszMsg, " ");
strcat(pszMsg, "Th�c th� ");
sprintf(pszMsg,"%s%d",pszMsg,EventCount);
strcat(pszMsg, ": ");
strcat(pszMsg, "<color=blue>");
strcat(pszMsg,pSkillEvent->m_szName);
strcat(pszMsg, " ");
strcat(pszMsg, "<color>");
strcat(pszMsg, "\n");


	KMagicAttrib *DamageAttribs = pSkillEvent->m_DamageAttribs;
	for (i  = 0; i < MAX_MISSLE_DAMAGEATTRIB; i ++)
	{
		if (!(DamageAttribs + i)->nAttribType || (DamageAttribs + i)->nAttribType == magic_seriesdame) continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc((DamageAttribs + i));
		if (!pszInfo) continue;
		strcat(pszMsg, " ");
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
	}
}


if (pSkillActive->m_nCollideSkillId > 0 && pSkillActive->m_nCollideSkillId < MAX_SKILL)
{
	EventCount++;
	pSkillEvent = (KSkill *) g_SkillManager.GetSkill(pSkillActive->m_nCollideSkillId,pSkillActive->m_nEventSkillLevel);

strcat(pszMsg, "\n");
strcat(pszMsg, " ");
strcat(pszMsg, "Th�c th� ");
sprintf(pszMsg,"%s%d",pszMsg,EventCount);
strcat(pszMsg, ": ");
strcat(pszMsg, "<color=blue>");
strcat(pszMsg,pSkillEvent->m_szName);
strcat(pszMsg, " ");
strcat(pszMsg, "<color>");
strcat(pszMsg, "\n");


	KMagicAttrib *DamageAttribs = pSkillEvent->m_DamageAttribs;
	for (i  = 0; i < MAX_MISSLE_DAMAGEATTRIB; i ++)
	{
		if (!(DamageAttribs + i)->nAttribType || (DamageAttribs + i)->nAttribType == magic_seriesdame) continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc((DamageAttribs + i));
		if (!pszInfo) continue;
		strcat(pszMsg, " ");
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
	}
}





if (pSkillActive->m_nVanishedSkillId > 0 && pSkillActive->m_nVanishedSkillId < MAX_SKILL)
{
	EventCount++;
	pSkillEvent = (KSkill *) g_SkillManager.GetSkill(pSkillActive->m_nVanishedSkillId,pSkillActive->m_nEventSkillLevel);

strcat(pszMsg, "\n");
strcat(pszMsg, " ");
strcat(pszMsg, "Th�c th� ");
sprintf(pszMsg,"%s%d",pszMsg,EventCount);
strcat(pszMsg, ": ");
strcat(pszMsg, "<color=blue>");
strcat(pszMsg,pSkillEvent->m_szName);
strcat(pszMsg, " ");
strcat(pszMsg, "<color>");
strcat(pszMsg, "\n");


	KMagicAttrib *DamageAttribs = pSkillEvent->m_DamageAttribs;
	for (i  = 0; i < MAX_MISSLE_DAMAGEATTRIB; i ++)
	{
		if (!(DamageAttribs + i)->nAttribType || (DamageAttribs + i)->nAttribType == magic_seriesdame) continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc((DamageAttribs + i));
		if (!pszInfo) continue;
		strcat(pszMsg, " ");
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
	}
}












}

}

	for (i  = 0; i < 6; i ++)
	{
	if (m_addskillsdame[i][0] > 0)
	{
        strcat(pszMsg, "\n");
		break;
	}
	}

    KSkill * pSkillAddDame;

	for (i  = 0; i < 6; i ++)
	{
	if (m_addskillsdame[i][0] <= 0) continue;

    pSkillAddDame = (KSkill *)g_SkillManager.GetSkill(m_addskillsdame[i][0], 1);
	if (pSkillAddDame)
	{
      if (pSkillAddDame->GetSkillStyle() != 1)
	  {
		strcat(pszMsg, " ");
		strcat(pszMsg,"K� N�ng ");
		strcat(pszMsg,pSkillAddDame->GetSkillName());
		strcat(pszMsg," : +");
		sprintf(pszMsg,"%s%d",pszMsg,m_addskillsdame[i][1]);
		strcat(pszMsg,"%");
		strcat(pszMsg, " ");
		strcat(pszMsg, "\n");
	  }
	}

	}

}

void KSkill::PlayPreCastSound(BOOL bIsFeMale, int nX, int nY)  const 
{
	char * pSoundFile = NULL;
	
	if (!bIsFeMale)
		pSoundFile = (char *)m_szManPreCastSoundFile;
	else 
		pSoundFile = (char *)m_szFMPreCastSoundFile;
	
	int		nCenterX = 0, nCenterY = 0, nCenterZ = 0;
	
	// 获得屏幕中心点的地图坐标 not end
	g_ScenePlace.GetFocusPosition(nCenterX, nCenterY, nCenterZ);
	KCacheNode * pSoundNode = NULL;
	pSoundNode = (KCacheNode*) g_SoundCache.GetNode(pSoundFile, (KCacheNode*)pSoundNode);
	KWavSound * pWave = (KWavSound*)pSoundNode->m_lpData;
	if (pWave)
	{
		pWave->Play((nX - nCenterX) * 5, (10000 - (abs(nX - nCenterX) + abs(nY - nCenterY))) * Option.GetSndVolume() / 100 - 10000, 0);
	}
}
#endif
