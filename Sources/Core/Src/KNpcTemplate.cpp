#include "KCore.h"

#include "KNpcTemplate.h"
#define MAX_VALUE_LEN 300

#ifdef _SERVER
KItemDropRateTree g_ItemDropRateBinTree;
static KItemDropRate * g_GenItemDropRate(char * szDropIniFile)
{
	if ((!szDropIniFile)|| (!szDropIniFile[0]))
		return NULL;
	
	KIniFile IniFile;
	if (IniFile.Load(szDropIniFile))
	{
		KItemDropRate * pnewDrop = new KItemDropRate;
		IniFile.GetInteger("Main", "Count", 0, &pnewDrop->nCount);
		IniFile.GetInteger("Main", "RandRange", 0, &pnewDrop->nMaxRandRate);
		IniFile.GetInteger("Main", "MagicRate", 0, &pnewDrop->nMagicRate);
		IniFile.GetInteger("Main", "MoneyRate", 20, &pnewDrop->nMoneyRate);
		IniFile.GetInteger("Main", "MoneyScale", 50, &pnewDrop->nMoneyScale);
		IniFile.GetInteger("Main", "PropRate", 0, &pnewDrop->nPropRate);
		IniFile.GetInteger("Main", "PropScale", 0, &pnewDrop->nPropScale);
		IniFile.GetInteger("Main", "PropMagicRate", 0, &pnewDrop->nPropMagicRate);
		IniFile.GetInteger("Main", "PropMagicScale", 0, &pnewDrop->nPropMagicScale);



		IniFile.GetInteger("Main", "PropAllRate", 0, &pnewDrop->nPropAllRate);
		IniFile.GetInteger("Main", "PropAllScaleMin", 0, &pnewDrop->nPropAllScaleMin);
		IniFile.GetInteger("Main", "PropAllScaleMax", 0, &pnewDrop->nPropAllScaleMax);
		IniFile.GetInteger("Main", "PropAllByTeam", 0, &pnewDrop->nPropAllByTeam);



		IniFile.GetInteger("Main", "MinItemLevelScale", 20, &pnewDrop->nMinItemLevelScale);
		IniFile.GetInteger("Main", "MaxItemLevelScale", 10, &pnewDrop->nMaxItemLevelScale);
		IniFile.GetInteger("Main", "MaxItemLevel", 10, &pnewDrop->nMaxItemLevel);
		IniFile.GetInteger("Main", "MinItemLevel", 1, &pnewDrop->nMinItemLevel);

		if(pnewDrop->nCount <= 0)
		{
			delete pnewDrop;
			return NULL;
		}

		pnewDrop->pItemParam = new KItemDropRate::KItemParam[pnewDrop->nCount];
		char szSection[10];
		
		KItemDropRate::KItemParam * pItemParam = pnewDrop->pItemParam;
		for(int i = 0; i < pnewDrop->nCount; i ++, pItemParam ++)
		{
			sprintf(szSection, "%d", i + 1);
			IniFile.GetInteger(szSection, "Genre", 0, &(pItemParam->nGenre));
			IniFile.GetInteger(szSection, "Detail", 0, &(pItemParam->nDetailType));
			IniFile.GetInteger(szSection, "Particular", 0, &(pItemParam->nParticulType));
			IniFile.GetInteger(szSection, "RandRate", 0, &(pItemParam->nRate));
		}
		return pnewDrop;
	}
	else
	{
		printf("not load %s\n", szDropIniFile);
		return NULL;
	}

	return NULL;
}


int	operator<(KItemDropRateNode Left, KItemDropRateNode Right)
{
	return strcmp(Left.m_szFileName, Right.m_szFileName);
};

int operator==(KItemDropRateNode Left, KItemDropRateNode Right)
{
	return (strcmp(Left.m_szFileName, Right.m_szFileName) == 0);
};


#endif


void	KNpcTemplate::InitNpcBaseData(int nNpcTemplateId)
{
	if (nNpcTemplateId < 0 ) return;
	int nNpcTempRow = nNpcTemplateId + 2;

	g_NpcSetting.GetString(nNpcTempRow, "Name", "", Name, sizeof(Name));
	g_NpcSetting.GetInteger(nNpcTempRow, "Kind", 0, (int *)&m_Kind);
	g_NpcSetting.GetInteger(nNpcTempRow, "Camp", 0, &m_Camp);
	g_NpcSetting.GetInteger(nNpcTempRow, "Series", 0, &m_Series);
	
	g_NpcSetting.GetInteger(nNpcTempRow, "HeadImage",	0, &m_HeadImage);
	g_NpcSetting.GetInteger(nNpcTempRow, "ClientOnly",	0, &m_bClientOnly);
	g_NpcSetting.GetInteger(nNpcTempRow, "CorpseIdx",	0, &m_CorpseSettingIdx);
	
	g_NpcSetting.GetInteger(nNpcTempRow, "DeathFrame",	12, &m_DeathFrame);
	g_NpcSetting.GetInteger(nNpcTempRow, "WalkFrame",	15, &m_WalkFrame);
	g_NpcSetting.GetInteger(nNpcTempRow, "RunFrame",	15, &m_RunFrame);
	g_NpcSetting.GetInteger(nNpcTempRow, "HurtFrame",	10, &m_HurtFrame);
	g_NpcSetting.GetInteger(nNpcTempRow, "WalkSpeed",	5, &m_WalkSpeed);
	g_NpcSetting.GetInteger(nNpcTempRow, "AttackSpeed",	20, &m_AttackFrame);
	g_NpcSetting.GetInteger(nNpcTempRow, "CastSpeed",	20, &m_CastFrame);
	g_NpcSetting.GetInteger(nNpcTempRow, "RunSpeed",	10, &m_RunSpeed);
	g_NpcSetting.GetInteger(nNpcTempRow, "StandFrame",	15, &m_StandFrame);
	g_NpcSetting.GetInteger(nNpcTempRow, "StandFrame1", 15, &m_StandFrame1);
	g_NpcSetting.GetInteger(nNpcTempRow, "Stature",		0,  &m_nStature);
	g_NpcSetting.GetInteger(nNpcTempRow, "TypeNpc",		0,  &m_TypeNpc);
	g_NpcSetting.GetInteger(nNpcTempRow, "ShareDame",	0,  &m_nShareDame);
	g_NpcSetting.GetInteger(nNpcTempRow, "PencentDameAdd",	0,  &m_nPencentDameAdd);

	g_NpcSetting.GetInteger(nNpcTempRow, "NumObjT",	0,  &m_NumObjT);
	g_NpcSetting.GetInteger(nNpcTempRow, "DirObjT",	0,  &m_DirObjT);




#ifdef _SERVER
	g_NpcSetting.GetInteger(nNpcTempRow, "RateBoss",	0,  &m_RateBoss);
	g_NpcSetting.GetInteger(nNpcTempRow, "Treasure",		0, &m_Treasure);
	g_NpcSetting.GetInteger(nNpcTempRow, "AIMode",	0, &m_AiMode);
	g_NpcSetting.GetInteger(nNpcTempRow, "AIParam1",	0, &m_AiParam[0]);
	g_NpcSetting.GetInteger(nNpcTempRow, "AIParam2",	0, &m_AiParam[1]);
	g_NpcSetting.GetInteger(nNpcTempRow, "AIParam3",	0, &m_AiParam[2]);
	g_NpcSetting.GetInteger(nNpcTempRow, "AIParam4",	0, &m_AiParam[3]);
	g_NpcSetting.GetInteger(nNpcTempRow, "AIParam5",	0, &m_AiParam[4]);
	g_NpcSetting.GetInteger(nNpcTempRow, "AIParam6",	0, &m_AiParam[5]);
	g_NpcSetting.GetInteger(nNpcTempRow, "AIParam7",	0, &m_AiParam[6]);
	g_NpcSetting.GetInteger(nNpcTempRow, "AIParam8",	0, &m_AiParam[7]);
	g_NpcSetting.GetInteger(nNpcTempRow, "AIParam9",	0, &m_AiParam[8]);
	g_NpcSetting.GetInteger(nNpcTempRow, "AIParam10",	5, &m_AiParam[9]);

	g_NpcSetting.GetInteger(nNpcTempRow, "FireResistMax",	0, &m_FireResistMax);
	g_NpcSetting.GetInteger(nNpcTempRow, "ColdResistMax",	0, &m_ColdResistMax);
	g_NpcSetting.GetInteger(nNpcTempRow, "LightResistMax",	0, &m_LightResistMax);
	g_NpcSetting.GetInteger(nNpcTempRow, "PoisonResistMax",	0, &m_PoisonResistMax);
	
	g_NpcSetting.GetInteger(nNpcTempRow, "PhysicsResistMax",	0, &m_PhysicsResistMax);
	g_NpcSetting.GetInteger(nNpcTempRow, "ActiveRadius", 30, &m_ActiveRadius);
	g_NpcSetting.GetInteger(nNpcTempRow, "VisionRadius", 40, &m_VisionRadius);
	
	char szDropFile[200];
	g_NpcSetting.GetString(nNpcTempRow, "DropRateFile", "", szDropFile, sizeof(szDropFile));
	strlwr(szDropFile);
	strcpy(m_szDropRateFile, szDropFile);
	KItemDropRateNode DropNode;
	strcpy(DropNode.m_szFileName, szDropFile);
	
	if (g_ItemDropRateBinTree.Find(DropNode))
	{
		m_pItemDropRate = DropNode.m_pItemDropRate;
	}
	else
	{
		KItemDropRateNode newDropNode;
		strcpy(newDropNode.m_szFileName,szDropFile);
		newDropNode.m_pItemDropRate = g_GenItemDropRate(szDropFile);
		g_ItemDropRateBinTree.Insert(newDropNode);
		m_pItemDropRate = newDropNode.m_pItemDropRate;
	}
	
	int nAIMaxTime = 0;
	g_NpcSetting.GetInteger(nNpcTempRow, "AIMaxTime", 25, (int*)&nAIMaxTime);
	m_AIMAXTime = (BYTE)nAIMaxTime;
	
	g_NpcSetting.GetInteger(nNpcTempRow, "HitRecover", 0, &m_HitRecover);
	g_NpcSetting.GetInteger(nNpcTempRow, "ReviveFrame", 2400, &m_ReviveFrame);
	

#else
	g_NpcSetting.GetInteger(nNpcTempRow, "ArmorType", 0, &m_ArmorType);
	g_NpcSetting.GetInteger(nNpcTempRow, "HelmType", 0, &m_HelmType);
	g_NpcSetting.GetInteger(nNpcTempRow, "WeaponType", 0, &m_WeaponType);
	g_NpcSetting.GetInteger(nNpcTempRow, "HorseType", -1, &m_HorseType);
	g_NpcSetting.GetInteger(nNpcTempRow, "RideHorse",0, &m_bRideHorse);	
	g_NpcSetting.GetString(nNpcTempRow, "ActionScript", "", ActionScript,sizeof(ActionScript));
#endif
	
}

void KNpcTemplate::InitNpcLevelData(KTabFile * pKindFile, int nNpcTemplateId, int nLevel)
{
	if (nNpcTemplateId < 0 || nLevel <= 0) return;
	int nNpcTempRow = nNpcTemplateId + 2;
	
	m_nLevel = nLevel;

		m_NpcSettingIdx = nNpcTemplateId;
#ifdef _SERVER				
		//技能
		int szValue1;
		int szValue2;

		g_NpcSetting.GetInteger(nNpcTempRow, "Skill1",0,&szValue1);
		g_NpcSetting.GetInteger(nNpcTempRow, "Level1",0, &szValue2);
		if (szValue1 && szValue2)
			m_SkillList.SetNpcSkill(1,szValue1,szValue2);
		
		g_NpcSetting.GetInteger(nNpcTempRow, "Skill2",0,&szValue1);
		g_NpcSetting.GetInteger(nNpcTempRow, "Level2",0,&szValue2);
		if (szValue1 && szValue2)
			m_SkillList.SetNpcSkill(2,szValue1,szValue2);
		
		g_NpcSetting.GetInteger(nNpcTempRow, "Skill3",0,&szValue1);
		g_NpcSetting.GetInteger(nNpcTempRow, "Level3",0, &szValue2);
		if (szValue1 && szValue2)
			m_SkillList.SetNpcSkill(3,szValue1,szValue2);

		g_NpcSetting.GetInteger(nNpcTempRow, "Skill4",0,&szValue1);
		g_NpcSetting.GetInteger(nNpcTempRow, "Level4",0, &szValue2);
		if (szValue1 && szValue2)
			m_SkillList.SetNpcSkill(4,szValue1,szValue2);

		g_NpcSetting.GetInteger(nNpcTempRow, "AruraSkill",0,&szValue1);
		g_NpcSetting.GetInteger(nNpcTempRow, "AruraSkillLevel",0, &szValue2);
		if (szValue1 && szValue2)
			m_SkillList.SetNpcSkill(5,szValue1,szValue2);

		g_NpcSetting.GetInteger(nNpcTempRow, "PassSkill",0,&szValue1);
		g_NpcSetting.GetInteger(nNpcTempRow, "PassSkillLevel",0, &szValue2);
		if (szValue1 && szValue2)
			m_SkillList.SetNpcSkill(6,szValue1,szValue2);


		float nParam1 = 0;
		float nParam2 = 0;
		float nParam3 = 0;
		float nParam = 1;
		
		g_NpcSetting.GetFloat(nNpcTempRow, "ExpNumber", 1, &nParam);
		g_NpcSetting.GetFloat(nNpcTempRow, "ExpParam", 0, &nParam1);
		m_Experience = nParam * nParam1;
		
		g_NpcSetting.GetFloat(nNpcTempRow, "LifeParam", 1, &nParam);
		m_LifeMax = nParam;
		if (m_LifeMax == 0) m_LifeMax = 100;
		

		g_NpcSetting.GetFloat(nNpcTempRow, "LifeReplenish",1, &nParam);
		m_LifeReplenish = nParam;


		g_NpcSetting.GetFloat(nNpcTempRow, "ARParam", 1, &nParam);
		m_AttackRating = nParam;
		if (m_AttackRating == 0) m_AttackRating = 100;


		g_NpcSetting.GetFloat(nNpcTempRow, "DefenseParam", 1, &nParam);
		m_Defend = nParam;


		g_NpcSetting.GetFloat(nNpcTempRow, "MinDamageParam", 1, &nParam);
		m_PhysicsDamage.nValue[0] = nParam;

		g_NpcSetting.GetFloat(nNpcTempRow, "MaxDamageParam", 1, &nParam);
		m_PhysicsDamage.nValue[2] = nParam;

		g_NpcSetting.GetInteger(nNpcTempRow, "RedLum",0,&szValue1);
		m_RedLum = szValue1;

		g_NpcSetting.GetInteger(nNpcTempRow, "GreenLum",0,&szValue1);
		m_GreenLum= szValue1;

		g_NpcSetting.GetInteger(nNpcTempRow, "BlueLum",0,&szValue1);
		m_BlueLum = szValue1;
		
		g_NpcSetting.GetInteger(nNpcTempRow, "FireResist",0,&szValue1);
		m_FireResist = szValue1;

		g_NpcSetting.GetInteger(nNpcTempRow, "ColdResist",0,&szValue1);
		m_ColdResist = szValue1;

		g_NpcSetting.GetInteger(nNpcTempRow, "LightResist",0,&szValue1);
		m_LightResist = szValue1;

		g_NpcSetting.GetInteger(nNpcTempRow, "PoisonResist",0,&szValue1);
		m_PoisonResist = szValue1;

		g_NpcSetting.GetInteger(nNpcTempRow, "PhysicsResist",0,&szValue1);
		m_PhysicsResist = szValue1;

#endif
#ifndef _SERVER
		int nParam1 = 0;
		int nParam2 = 0;
		int nParam3 = 0;
		int nParam = 1;
		g_NpcSetting.GetInteger(nNpcTempRow, "LifeParam", 1, &nParam);
		m_LifeMax = nParam;
		if (m_LifeMax == 0) m_LifeMax = 100;
#endif

}

int KNpcTemplate::GetNpcLevelDataFromScript(KLuaScript * pScript, char * szDataName, int nLevel, char * szParam)
{
	int nTopIndex = 0;
	int nReturn = 0;
	if (szParam == NULL|| szParam[0] == 0 || strlen(szParam) < 3)
	{
		return 0;
	}

	
	pScript->SafeCallBegin(&nTopIndex);
	pScript->CallFunction("GetNpcLevelData", 1, "dss", nLevel, szDataName, szParam);
	nTopIndex = Lua_GetTopIndex(pScript->m_LuaState);
	nReturn = (int) Lua_ValueToNumber(pScript->m_LuaState, nTopIndex);
	pScript->SafeCallEnd(nTopIndex);
	

	return nReturn;
}

int KNpcTemplate::GetNpcLevelDataFromScript(KLuaScript * pScript, char * szDataName, int nLevel, double nParam1, double nParam2, double nParam3)
{
	int nTopIndex = 0;
	int nReturn = 0;

	
	pScript->SafeCallBegin(&nTopIndex);
	pScript->CallFunction("GetNpcKeyData", 1, "dsnnn", nLevel, szDataName, nParam1, nParam2, nParam3);
	nTopIndex = Lua_GetTopIndex(pScript->m_LuaState);
	nReturn = (int) Lua_ValueToNumber(pScript->m_LuaState, nTopIndex);
	pScript->SafeCallEnd(nTopIndex);
	

	return nReturn;
}


int KNpcTemplate::SkillString2Id(char * szSkillString)
{
	if (!szSkillString[0]) return 0;
	int nSkillNum = g_OrdinSkillsSetting.GetHeight() - 1;
	char szSkillName[100];
	for (int i = 0 ;  i < nSkillNum; i ++)
	{
		g_OrdinSkillsSetting.GetString(i + 2, "SkillName", "", szSkillName, sizeof(szSkillName));
		if (g_StrCmp(szSkillString, szSkillName))
		{
			int nSkillId = 0;
			g_OrdinSkillsSetting.GetInteger(i + 2, "SkillId", 0, &nSkillId);
			return nSkillId;
		}
	}
	//g_DebugLog("无找到该[%s]魔法的Id,请检查！", szSkillString);
	return 0;
}


