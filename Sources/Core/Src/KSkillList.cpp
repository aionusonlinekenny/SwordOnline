 #include "KEngine.h"
#include "KCore.h"
#include "SkillDef.h"
#include "KSkillList.h"
#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#include "crtdbg.h"
#else
#include "S3DBInterface.h"
#endif
#include "KSkills.h"
#include "KPlayer.h"

#ifndef _SERVER
#include "cOREsHELL.H"
#endif

KSkillList::KSkillList()
{
	ZeroMemory(m_Skills, sizeof(NPCSKILL) * MAX_NPCSKILL);
	m_nNpcIndex = 0;
	for (int i=0; i<MAX_SKILL; i++)
	{
	m_CurrentListSkillAddDame[i] = 0;
	m_CurrentListSkillAddLevel[i] = 0;
	}
}

KSkillList::~KSkillList()
{
}


static int g_CallCount = 0;

int	KSkillList::FindSame(int nSkillID)
{
    g_CallCount++;

	if (!nSkillID)
		return 0;
	
	for (int i=1; i<MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId == nSkillID)
		{
			return i;
		}
	}
	return 0;	
}

int KSkillList::FindFree()
{
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		if (!m_Skills[i].SkillId)
		{
			return i;
		}
	}
	return 0;	
}

#ifdef _SERVER
void KSkillList::SetNpcSkill(int nSkillNo, int nSkillId, int nSkillLevel)
{
	if (nSkillNo <= 0 || nSkillLevel <= 0 || nSkillId <= 0)
		return ;
	if (nSkillNo >= MAX_NPCSKILL || nSkillId >= MAX_SKILL || nSkillLevel >= MAX_SKILLLEVEL) 
		return ;
	if (m_CurrentListSkillAddLevel[nSkillId-1] < 0)
		m_CurrentListSkillAddLevel[nSkillId-1] = 0;

	if ((nSkillLevel + m_CurrentListSkillAddLevel[nSkillId-1]) >= MAX_SKILLLEVEL) 
		return ;
	
	m_Skills[nSkillNo].SkillId = nSkillId;
	m_Skills[nSkillNo].SkillLevel = nSkillLevel;
	m_Skills[nSkillNo].CurrentSkillLevel = nSkillLevel + m_CurrentListSkillAddLevel[nSkillId-1];
	m_Skills[nSkillNo].MaxTimes = 0;
	m_Skills[nSkillNo].RemainTimes = 0;

UpdateAddDameSkillLevel();

	_ASSERT(nSkillId > 0 && nSkillLevel > 0);
	KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillId, nSkillLevel + m_CurrentListSkillAddLevel[nSkillId-1]);
	if (!pOrdinSkill) 
        return ;
	
    if (pOrdinSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
	{
		pOrdinSkill->Cast(m_nNpcIndex, -1, m_nNpcIndex);
	}
}

void KSkillList::RemoveNpcSkill(int nSkillNo)
{
	if (nSkillNo <= 0 )
		return ;
	if (nSkillNo >= MAX_NPCSKILL) 
		return ;
	
	m_Skills[nSkillNo].SkillId = 0;
	m_Skills[nSkillNo].CurrentSkillLevel = 0;
	m_Skills[nSkillNo].SkillLevel = 0;
	m_Skills[nSkillNo].MaxTimes = 0;
	m_Skills[nSkillNo].RemainTimes = 0;
}

#endif

int KSkillList::GetCount()
{
	int nCount = 0;
	
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId)
		{
			nCount++;
		}
	}
	return nCount;
}

#ifndef _SERVER
void KSkillList::SetSkillLevel(int nId/*技能的ID号*/, int nLevel)
{
	int i = FindSame(nId);
	
	if (!i)
	{
		Add(nId, nLevel);
		return;		
	}
	else
		//m_Skills[i].SkillLevel = nLevel;
		SetLevel(i, nLevel);
}
#endif

#ifndef _SERVER
BOOL KSkillList::SetLevel(int nIndex/*技能列表的索引号*/, int nLevel)
{
	if (nIndex <= 0 || nIndex >= MAX_NPCSKILL)
		return FALSE;
	if (nLevel < 0 || nLevel >= MAX_SKILLLEVEL)
		return FALSE;
	
	int nInc = nLevel - m_Skills[nIndex].SkillLevel;
	IncreaseLevel(nIndex, nInc);
	return TRUE;
}
#endif




int	KSkillList::GetSkillIdx(int nSkillId)
{
	int i;
	if (nSkillId <= 0)
		return 0;
	i = FindSame(nSkillId);
	if (i)
	return i;

return 0;
}


void KSkillList::UpdateAddDameSkillLevel()
{
    KSkill * pSkillAddDame; 

	for (int k = 0; k < MAX_SKILL; k++)
	{
	m_CurrentListSkillAddDame[k] = 0;
    }

	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
	if (m_Skills[i].SkillId > 0 && m_Skills[i].SkillId < MAX_SKILL  && m_Skills[i].CurrentSkillLevel > 0 && m_Skills[i].CurrentSkillLevel< MAX_SKILLLEVEL)
	{
	pSkillAddDame = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].CurrentSkillLevel);	
	if (pSkillAddDame) 
	{
	for (int j = 0; j < 6; j++)
	{	
	if (pSkillAddDame->m_addskillsdame[j][0] > 0 && pSkillAddDame->m_addskillsdame[j][0] < MAX_SKILL)
	{
    m_CurrentListSkillAddDame[pSkillAddDame->m_addskillsdame[j][0]-1] += pSkillAddDame->m_addskillsdame[j][1];
	}
	}
	}
	}
	}

}







void KSkillList::UpdateMagicAddSkillLevel()
{
//    KSkill * pSkillAddDame; 

	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
	if (m_Skills[i].SkillId > 0 && m_Skills[i].SkillId < MAX_SKILL)
	{
	m_Skills[i].CurrentSkillLevel = m_Skills[i].SkillLevel + m_CurrentListSkillAddLevel[m_Skills[i].SkillId-1];
	}
	}

    UpdateAddDameSkillLevel();

	for (i = 1; i < MAX_NPCSKILL; i++)
	{
	if (m_Skills[i].SkillId > 0 && m_Skills[i].SkillId < MAX_SKILL  && m_Skills[i].CurrentSkillLevel > 0 && m_Skills[i].CurrentSkillLevel< MAX_SKILLLEVEL)
	{

			ISkill * pSkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].CurrentSkillLevel);
			if (pSkill) 
			{
			if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
			{
				((KSkill *)pSkill)->Cast(m_nNpcIndex, -1, m_nNpcIndex);
			}
			}
	
	}
	}

}










BOOL KSkillList::IncreaseLevel(int nIdx, int nLvl)
{
	if (nIdx <= 0 || nIdx >= MAX_NPCSKILL)
		return FALSE;
	if (nLvl <= 0)
		return FALSE;
	
	if (m_CurrentListSkillAddLevel[m_Skills[nIdx].SkillId-1] < 0)
		m_CurrentListSkillAddLevel[m_Skills[nIdx].SkillId-1] = 0;

    if ((m_Skills[nIdx].SkillLevel + nLvl) >= MAX_SKILLLEVEL || (m_Skills[nIdx].SkillLevel + nLvl + m_CurrentListSkillAddLevel[m_Skills[nIdx].SkillId-1]) >= MAX_SKILLLEVEL)
	return FALSE;


	m_Skills[nIdx].SkillLevel += nLvl;
	m_Skills[nIdx].CurrentSkillLevel = m_Skills[nIdx].SkillLevel;
    m_Skills[nIdx].CurrentSkillLevel += m_CurrentListSkillAddLevel[m_Skills[nIdx].SkillId-1];


	
UpdateAddDameSkillLevel();

	KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[nIdx].SkillId, m_Skills[nIdx].CurrentSkillLevel);
	if (!pOrdinSkill) 
        return FALSE;


	if (pOrdinSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
	{
		pOrdinSkill->Cast(m_nNpcIndex, -1, m_nNpcIndex);
	}
	return TRUE;
}

int KSkillList::Add(int nSkillID, int nLevel, int nMaxTimes, int RemainTimes)
{

	int i;
	
	if (nSkillID <= 0 || nSkillID >= MAX_SKILL || nLevel < 0)
		return 0;
	if (m_CurrentListSkillAddLevel[nSkillID-1] < 0)
		m_CurrentListSkillAddLevel[nSkillID-1] = 0;

	if (nLevel >= MAX_SKILLLEVEL || (nLevel + m_CurrentListSkillAddLevel[nSkillID-1]) >= MAX_SKILLLEVEL)
	return 0;

	i = FindSame(nSkillID);
	if (i)
	{
		if (nLevel > m_Skills[i].SkillLevel)
		{
			int nInc = nLevel - m_Skills[i].SkillLevel;
			IncreaseLevel(i, nInc);
		}
		m_Skills[i].MaxTimes = 0;
		m_Skills[i].RemainTimes = 0;
		
		if (m_Skills[i].CurrentSkillLevel  > 0)
		{
			ISkill * pSkill = g_SkillManager.GetSkill(nSkillID, m_Skills[i].CurrentSkillLevel);
			if (!pSkill) 
				return 0;
			
			//如果改技能属于被动辅助技能时，则设置Npc状态
			if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
			{
				((KSkill *)pSkill)->Cast(m_nNpcIndex, -1, m_nNpcIndex);
			}
		}

		return i;
	}

	i = FindFree();
	if (i)
	{
		m_Skills[i].SkillId = nSkillID;
		m_Skills[i].SkillLevel = nLevel;
		m_Skills[i].CurrentSkillLevel = nLevel + m_CurrentListSkillAddLevel[nSkillID-1];
		m_Skills[i].MaxTimes = 0;
		m_Skills[i].RemainTimes = 0;
		UpdateAddDameSkillLevel();
        

		if (m_Skills[i].CurrentSkillLevel > 0)
		{
			ISkill * pSkill =  g_SkillManager.GetSkill(nSkillID, m_Skills[i].CurrentSkillLevel);
			if (!pSkill) 
				return 0 ;
			
			if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
			{
				((KSkill*)pSkill)->Cast(m_nNpcIndex, -1, m_nNpcIndex);
			}
		}

		return i;
	}
	
	return 0;
}

int KSkillList::GetLevel(int nSkillID)
{
	int i;
	
	if (!nSkillID)
		return 0;
	
	i = FindSame(nSkillID);
	if (i)
	{
		_ASSERT(m_Skills[i].SkillLevel < MAX_SKILLLEVEL);
		return m_Skills[i].SkillLevel;
	}

	return 0;
}

int KSkillList::GetCurrentLevel(int nSkillID)
{
	int i;
	
	if (!nSkillID)
		return 0;
	
	i = FindSame(nSkillID);

	if (i)
	{
		return m_Skills[i].CurrentSkillLevel;
	}

	return 0;
}

BOOL KSkillList::CanCast(int nSkillID, DWORD dwTime)
{
	
	if (!nSkillID)
		return FALSE;
	
	int i = FindSame(nSkillID);
	if (!i)
	{
		return FALSE;
	}

	if (m_Skills[i].CurrentSkillLevel <= 0 || m_Skills[i].NextCastTime > dwTime)
		return FALSE;
	
	return TRUE;
}

void KSkillList::SetNextCastTime(int nSkillID, DWORD dwTime)
{
//	m_Skills[nSkillID].NextCastTime = dwTime;
//	return;
	if (!nSkillID)
		return;;
	
	int i = FindSame(nSkillID);
	if (!i)
		return;
	
	m_Skills[i].NextCastTime = dwTime;
}

void KSkillList::SetBeginCastTime(int nSkillID, DWORD dwTime)
{
//	m_Skills[nSkillID].NextCastTime = dwTime;
//	return;
	if (!nSkillID)
		return;;
	
	int i = FindSame(nSkillID);
	if (!i)
		return;
	
	m_Skills[i].BeginCastTime = dwTime;
}



DWORD KSkillList::GetNextCastTime(int nSkillID)
{
//	m_Skills[nSkillID].NextCastTime = dwTime;
//	return;
	if (!nSkillID)
		return -1;;
	
	int i = FindSame(nSkillID);
	if (!i)
		return -1;
	
	return m_Skills[i].NextCastTime;
}

DWORD KSkillList::GetBeginCastTime(int nSkillID)
{
//	m_Skills[nSkillID].NextCastTime = dwTime;
//	return;
	if (!nSkillID)
		return - 1;;
	
	int i = FindSame(nSkillID);
	if (!i)
		return -1;
	
	return m_Skills[i].BeginCastTime;
}



	
#ifndef _SERVER
int	KSkillList::GetSkillSortList(KUiSkillData * pSkillList)
{
	//Ques
	if (!pSkillList) return 0;
	memset(pSkillList, 0, sizeof(KUiSkillData) * 50);
	int nCount = 0;
	ISkill* pSkill = NULL;
	KSkill * pOrdinSkill = NULL;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId)
		{
			_ASSERT(m_Skills[i].SkillLevel >= 0);
			
			if (!m_Skills[i].CurrentSkillLevel)
			{
				pSkill =  g_SkillManager.GetSkill(m_Skills[i].SkillId, 1);
			}
			else 
			{
				pSkill =  g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].CurrentSkillLevel);
			}
			
			if (!pSkill)
				continue;
			eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();
			
			switch(eStyle)
			{
			case SKILL_SS_Missles:			//	子弹类		本技能用于发送子弹类
			case SKILL_SS_Melee:
			case SKILL_SS_InitiativeNpcState:	//	主动类		本技能用于改变当前Npc的主动状态
			case SKILL_SS_PassivityNpcState:		//	被动类		本技能用于改变Npc的被动状态
				{
					pOrdinSkill = (KSkill * ) pSkill;
					if (pOrdinSkill->IsPhysical())
						continue;

				}
				break;
			case SKILL_SS_Thief:					//	偷窃类
				{
					
				}
				break;
			default:
				continue;
			}
			
            KUiSkillData * pSkill = pSkillList + nCount;
			pSkill->uGenre = CGOG_SKILL_FIGHT;
			pSkill->uId = m_Skills[i].SkillId;
			pSkill->nLevel = m_Skills[i].SkillLevel;
			pSkill->nCurrentLevel = m_Skills[i].CurrentSkillLevel;
			if ((++nCount) == 50)
			{
				g_DebugLog("Npc的技能数量超过了限制50！！！，请检查!!!");			
				break;
			}
		}
	}
	return nCount;
}

int KSkillList::GetLeftSkillSortList(KUiSkillData* pSkillList)
{
	if (!pSkillList) return 0;
	memset(pSkillList, 0, sizeof(KUiSkillData) * 65);
	int nCount = 1;
	
	pSkillList->uGenre = CGOG_SKILL_SHORTCUT;
	pSkillList->uId = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetCurActiveWeaponSkill();//物理技能置前!
	pSkillList->nData = 0;
	
	ISkill * pISkill = NULL;
	KSkill * pOrdinSkill = NULL;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId && m_Skills[i].CurrentSkillLevel > 0)
		{
			_ASSERT(m_Skills[i].SkillId > 0);
			pISkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].CurrentSkillLevel);
			if (!pISkill)
				continue;
			
			eSkillStyle eStyle = (eSkillStyle)pISkill->GetSkillStyle();
			switch(eStyle)
			{
			case SKILL_SS_Missles:			//	子弹类		本技能用于发送子弹类
			case SKILL_SS_Melee:
			case SKILL_SS_InitiativeNpcState:	//	主动类		本技能用于改变当前Npc的主动状态
			case SKILL_SS_PassivityNpcState:		//	被动类		本技能用于改变Npc的被动状态
				{
					pOrdinSkill = (KSkill * ) pISkill;
					if ((!pOrdinSkill->IsPhysical()) &&	
						(pOrdinSkill->GetSkillLRInfo() == BothSkill) || 
						(pOrdinSkill->GetSkillLRInfo() == leftOnlySkill)
						)
					{
						
					}
					else 
					if (m_Skills[i].SkillId != 53)
						continue;					
				}
				break;
				
			case SKILL_SS_Thief:
				{
					
					continue;
					
				}break;
			}
			
			KUiSkillData * pSkill = pSkillList + nCount;
			pSkill->uGenre = CGOG_SKILL_SHORTCUT;
			pSkill->uId = m_Skills[i].SkillId;
			pSkill->nData = nCount / 8;
			nCount ++;
			if (nCount >= 65)
				break;
			
		}
	}
	return nCount;
}

int KSkillList::GetRightSkillSortList(KUiSkillData* pSkillList)
{
	if (!pSkillList) return 0;
	memset(pSkillList, 0, sizeof(KUiSkillData) * 65);
	
	int nCount = 1;
	pSkillList->uGenre = CGOG_SKILL_SHORTCUT;
	//pSkillList->uId = (unsigned int)-1;//物理技能置前!
	pSkillList->uId = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetCurActiveWeaponSkill();//物理技能置前!;	 
	pSkillList->nData = 0;
	ISkill * pISkill = NULL;
	KSkill * pOrdinSkill = NULL;
	
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId && m_Skills[i].CurrentSkillLevel > 0)
		{
			_ASSERT(m_Skills[i].SkillId > 0);
			pISkill  =  g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].CurrentSkillLevel);
			if(!pISkill)
				continue;

			eSkillStyle eStyle = ( eSkillStyle ) pISkill->GetSkillStyle();
			
			switch(eStyle)
			{
			case SKILL_SS_Missles:			//	子弹类		本技能用于发送子弹类
			case SKILL_SS_Melee:
			case SKILL_SS_InitiativeNpcState:	//	主动类		本技能用于改变当前Npc的主动状态
			case SKILL_SS_PassivityNpcState:		//	被动类		本技能用于改变Npc的被动状态
				{
					pOrdinSkill = (KSkill * ) pISkill;
					if (
						(!pOrdinSkill->IsPhysical()) && 
						(pOrdinSkill->GetSkillLRInfo() == BothSkill) || 
						(pOrdinSkill->GetSkillLRInfo() == RightOnlySkill)
						)
					{
						
					}
					else 
					if (m_Skills[i].SkillId != 53)
						continue;
				}

				break;
				
			case SKILL_SS_Thief:
				{
					
				}break;
			}
			
			
			
			KUiSkillData * pSkill = pSkillList + nCount;
			pSkill->uGenre = CGOG_SKILL_SHORTCUT;
			pSkill->uId = m_Skills[i].SkillId;
			pSkill->nData = nCount / 8;
			nCount ++;
			if (nCount >= 65)
				break;
			
		}
	}
	return nCount;
}

// auto game

int KSkillList::GetSkillState(int nIndex)
{
	if (nIndex > 0)
	{
		int i = 0, j = 0;
		for (i = 1; i < MAX_NPCSKILL; i++)
		{
			int nCurLevel = 0;
			if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
			{	
				nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
				if(nCurLevel > 0)
				{
					KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
					if (!pOrdinSkill) 
						continue;
		
					if(pOrdinSkill->IsAura() || pOrdinSkill->IsTargetEnemy())
						continue;

					if ((pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles || 
						pOrdinSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState) && pOrdinSkill->IsTargetSelf())
					{
						j++;
						if (j == nIndex)
							return m_Skills[i].SkillId;
					}
				}
				else
					continue;
			}
		}
		return 0;
	}
	int nCount = 0;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		int nCurLevel = 0;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
		{	
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
			if(nCurLevel > 0)
			{
				KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
				if (!pOrdinSkill) 
					continue;
	
				if(pOrdinSkill->IsAura())
					continue;

				if ((pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles || 
					pOrdinSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState) && pOrdinSkill->IsTargetSelf())
					nCount++;
			}
			else
				continue;
		}
	}
	return nCount;
}

int KSkillList::GetSkillFight(int nIndex)
{
	if (nIndex > 0)
	{
		int i = 0, j = 0;
		for (i = 1; i < MAX_NPCSKILL; i++)
		{
			int nCurLevel = 0;
			if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
			{	
				nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
				if(nCurLevel > 0)
				{
					KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
					if (!pOrdinSkill) 
						continue;

					if (pOrdinSkill->IsAbs())
						continue;

					if(pOrdinSkill->IsAura())
						continue;

					if ((pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles || 
						pOrdinSkill->GetSkillStyle() == SKILL_SS_Melee) && pOrdinSkill->IsTargetEnemy())
					{
						j++;
						if (j == nIndex)
							return m_Skills[i].SkillId;
					}
				}
				else
					continue;
			}
		}
		return 0;
	}
	int nCount = 0;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		int nCurLevel = 0;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
		{	
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
			if(nCurLevel > 0)
			{
				KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
				if (!pOrdinSkill) 
					continue;
	
				if (pOrdinSkill->IsAbs())
					continue;

				if(pOrdinSkill->IsAura())
					continue;

				if ((pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles || 
					pOrdinSkill->GetSkillStyle() == SKILL_SS_Melee) && pOrdinSkill->IsTargetEnemy())
					nCount++;
			}
			else
				continue;
		}
	}
	return nCount;
}
int KSkillList::FindSkillAura(int nIndex)
{
	if (nIndex > 0)
	{
		int i = 0, j = 0;
		for (i = 1; i < MAX_NPCSKILL; i++)
		{
			int nCurLevel = 0;
			if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
			{	
				nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
				if(nCurLevel > 0)
				{
					KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
					if (!pOrdinSkill) 
						continue;
		
					if(pOrdinSkill->IsAura())
					{
						j++;
						if (j == nIndex)
							return m_Skills[i].SkillId;
					}
				}
				else
					continue;
			}
		}
		return 0;
	}
	int _nCount = 0;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		int nCurLevel = 0;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
		{	
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
			if(nCurLevel > 0)
			{
				KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
				if (!pOrdinSkill) 
					continue;
	
				if(pOrdinSkill->IsAura())
					_nCount++;
			}
			else
				continue;
		}
	}
	return _nCount;
}




int KSkillList::GetSkillPosition(int nSkillId)//获得技能在技能界面的位置
{
	if (nSkillId <= 0) return -1;
	KSkill * pOrdinSkill = NULL;
	
	ISkill * pISkill = NULL;
	int nCount = 0;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		_ASSERT(m_Skills[i].SkillLevel >= 0);
		if (m_Skills[i].SkillId<= 0) continue;
		
		if (m_Skills[i].CurrentSkillLevel == 0)
		{
			pISkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, 1);
		}
		else
		{
			pISkill =  g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].CurrentSkillLevel);
		}

		if (!pISkill) 
        	continue ;


		eSkillStyle eStyle = ( eSkillStyle ) pISkill->GetSkillStyle();
		switch(eStyle)
		{
		case SKILL_SS_Missles:			//	子弹类		本技能用于发送子弹类
		case SKILL_SS_Melee:
		case SKILL_SS_InitiativeNpcState:	//	主动类		本技能用于改变当前Npc的主动状态
		case SKILL_SS_PassivityNpcState:		//	被动类		本技能用于改变Npc的被动状态
			{
				pOrdinSkill = (KSkill * ) pISkill;
				if (
					(!m_Skills[i].SkillId) || 
					(pOrdinSkill->IsPhysical())
					)
				{
					continue;
				}
			}
			break;
			
		case SKILL_SS_Thief:
			{
				
				
			}break;
		}
		
		
		if (m_Skills[i].SkillId == nSkillId)
		{
			return nCount;
		}
		nCount ++;
		
	}
	return nCount;
}
#endif

#ifdef _SERVER
int	KSkillList::UpdateDBSkillList(BYTE * pSkillBuffer)
{
	if (!pSkillBuffer)
		return -1;
	
	int nCount = 0;

	TDBSkillData * pSkillData = (TDBSkillData *) pSkillBuffer;
	for (int i = 0 ; i < MAX_NPCSKILL; i ++)
	{
		if (m_Skills[i].SkillId > 0)
		{
			pSkillData->m_nSkillId = m_Skills[i].SkillId;
			pSkillData->m_nSkillLevel = m_Skills[i].SkillLevel;
			pSkillData++;
			nCount ++;
		}
	}
	return nCount;
}

int		KSkillList::ClearSkillLevel(bool bClearPhysicalLevel)
{
	int nTotalSkillPoint = 0;

	for (int i = 0 ; i < MAX_NPCSKILL; i ++)
	{
		if (m_Skills[i].SkillId > 0)
		{
			ISkill * pSkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, 1);
			if (pSkill)
			{

				if (m_nNpcIndex > 0 && m_nNpcIndex < MAX_NPC)
				{

				if (Npc[m_nNpcIndex].IsPlayer())
				{

				int nPlayerIdx = Npc[m_nNpcIndex].GetPlayerIdx();


int nMaxSkillLevel = 0;

if (Player[nPlayerIdx].m_cTask.GetSaveVal(139) == 1)
{
nMaxSkillLevel += 1;
}
else if (Player[nPlayerIdx].m_cTask.GetSaveVal(139) == 2)
{
nMaxSkillLevel += 2;
}
else if (Player[nPlayerIdx].m_cTask.GetSaveVal(139) == 3)
{
nMaxSkillLevel += 4;
}
else if (Player[nPlayerIdx].m_cTask.GetSaveVal(139) == 4)
{
nMaxSkillLevel += 6;
}
else if (Player[nPlayerIdx].m_cTask.GetSaveVal(139) == 5)
{
nMaxSkillLevel += 8;
}









				if ((((KSkill*)pSkill)->GetSkillReqLevel() == 80 || ((KSkill*)pSkill)->GetSkillReqLevel() == 120) && m_Skills[i].SkillLevel > (20 + nMaxSkillLevel))
				{
				m_Skills[i].CurrentSkillLevel = m_Skills[i].CurrentSkillLevel - (m_Skills[i].SkillLevel - (20+nMaxSkillLevel));
				m_Skills[i].SkillLevel = 20+nMaxSkillLevel;
				}

				}

				}


				if (pSkill->IsPhysical() || ((KSkill*)pSkill)->GetSkillReqLevel() == 80 || ((KSkill*)pSkill)->GetSkillReqLevel() == 120)
					continue;

				


				nTotalSkillPoint += m_Skills[i].SkillLevel;
				m_Skills[i].SkillLevel = 0;
				m_Skills[i].CurrentSkillLevel = 0;
			}
		}
	}
	return nTotalSkillPoint;
}
#endif
