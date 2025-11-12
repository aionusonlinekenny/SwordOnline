//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KFaction.cpp
// Date:	2002.09.26
// Code:	边城浪子
// Desc:	Faction Class
//---------------------------------------------------------------------------

#include	"KCore.h"
//#include	"MyAssert.h"
#include	"KIniFile.h"
#include	"KSkills.h"
#include	"KFaction.h"
#include	"CoreUseNameDef.h"


KFaction	g_Faction;


//---------------------------------------------------------------------------
//	功能：初始化，载入门派说明文件
//---------------------------------------------------------------------------
BOOL	KFaction::Init()
{
	int			i;

	for (i = 0; i < MAX_FACTION; i++)
	{
		m_sAttribute[i].m_nIndex = i;
		m_sAttribute[i].m_nSeries = series_metal;
		m_sAttribute[i].m_nCamp = camp_justice;
		m_sAttribute[i].m_szName[0] = 0;
	}

	KIniFile	Ini;
	if ( !Ini.Load(FACTION_FILE) )
		return FALSE;

	char		szSection[32];
	for (i = 0; i < MAX_FACTION; ++i)
	{
		sprintf(szSection, "%d", i);

		Ini.GetString(szSection, "Name", "none", m_sAttribute[i].m_szName, sizeof(m_sAttribute[i].m_szName));
		Ini.GetInteger(szSection, "Series", 0, &m_sAttribute[i].m_nSeries);
		Ini.GetInteger(szSection, "Camp", 0, &m_sAttribute[i].m_nCamp);
	}

	Ini.Clear();

	return TRUE;
}

//---------------------------------------------------------------------------
//	功能：根据五行属性和本属性第几个门派得到门派编号
//---------------------------------------------------------------------------
int		KFaction::GetID(int nSeries, int nNo)
{
	if (nSeries < series_metal || nSeries >= series_num || nNo < 0 || nNo >= FACTIONS_PRR_SERIES)
		return -1;
	return nSeries * FACTIONS_PRR_SERIES + nNo;
}

//---------------------------------------------------------------------------
//	功能：根据五行属性和门派名得到门派编号
//---------------------------------------------------------------------------
int		KFaction::GetID(int nSeries, char *lpszName)
{
	if (nSeries < series_metal || nSeries >= series_num)
		return -1;
	if ( !lpszName || !lpszName[0])
		return -1;
    for (int i = 0; i < MAX_FACTION; i++)
	{
		if (strcmp(lpszName, m_sAttribute[i].m_szName) == 0)
			return i;
	}
	return -1;
}

//---------------------------------------------------------------------------
//	功能：获得某个门派的阵营
//---------------------------------------------------------------------------
int		KFaction::GetCamp(int nFactionID)
{
	if (nFactionID < 0 || nFactionID >= MAX_FACTION)
		return -1;
	return m_sAttribute[nFactionID].m_nCamp;
}
