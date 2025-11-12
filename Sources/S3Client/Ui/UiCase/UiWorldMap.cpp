#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"
#include <crtdbg.h>
#include "UiWorldMap.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../core/src/CoreShell.h"
#include "../../../core/src/GameDataDef.h"

extern iRepresentShell*	g_pRepresentShell;
extern iCoreShell*		g_pCoreShell;

KUiWorldmap* KUiWorldmap::m_pSelf = NULL;

#define		SCHEME_INI_WORLD		"UiWorldmap.ini"
#define		WORLD_MAP_INFO_FILE		"\\Settings\\MapList.ini"

void MapToggleStatus();

KUiWorldmap* KUiWorldmap::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiWorldmap;
		if (m_pSelf)
			m_pSelf->Initialize();
	}

	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->UpdateData();
		m_pSelf->Show();
		m_pSelf->BringToTop();
		Wnd_SetExclusive(m_pSelf);
	}
	return m_pSelf;
}

void KUiWorldmap::CloseWindow()
{
	if (m_pSelf)
	{
		if (m_pSelf->m_Country.IsVisible())
			m_pSelf->m_Country.Hide();
		else if (m_pSelf->m_City.IsVisible())
			m_pSelf->m_City.Hide();
		else if (m_pSelf->m_Capital.IsVisible())
			m_pSelf->m_Capital.Hide();
		else if (m_pSelf->m_Tong.IsVisible())
			m_pSelf->m_Tong.Hide();
		else if (m_pSelf->m_Battlefield.IsVisible())
			m_pSelf->m_Battlefield.Hide();
		else if (m_pSelf->m_Field.IsVisible())
			m_pSelf->m_Field.Hide();
		else if (m_pSelf->m_Cave.IsVisible())
			m_pSelf->m_Cave.Hide();
		else if (m_pSelf->m_Others.IsVisible())
			m_pSelf->m_Others.Hide();	
	
		Wnd_ReleaseExclusive(m_pSelf);
		m_pSelf->Destroy();
		m_pSelf = NULL;
		MapToggleStatus();
	}
}

KUiWorldmap* KUiWorldmap::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	else
		return NULL;
}

//³õÊ¼»¯
void KUiWorldmap::Initialize()
{	
	// new add by lzlsky301
	AddChild(&m_Country);
	AddChild(&m_City);
	AddChild(&m_Capital);
	AddChild(&m_Tong);
	AddChild(&m_Battlefield);
	AddChild(&m_Field);
	AddChild(&m_Cave);
	AddChild(&m_Others);
	
	AddChild(&m_fengxiang);	
	AddChild(&m_bianjing);	
	AddChild(&m_yangzhou);	
	AddChild(&m_linan);	
	AddChild(&m_xiangyang);	
	AddChild(&m_chengdu);	
	AddChild(&m_dali);		

	char szBuffer[128];
	g_UiBase.GetCurSchemePath(szBuffer, sizeof(szBuffer));
	strcat(szBuffer, "\\"SCHEME_INI_WORLD);
	KIniFile	Ini;
	if (Ini.Load(szBuffer))
	{
		Init(&Ini, "WorldMap");
		m_Country.Init(&Ini, "Country");
		m_City.Init(&Ini, "City");
		m_Capital.Init(&Ini, "Capital");
		m_Tong.Init(&Ini, "Tong");
		m_Battlefield.Init(&Ini, "Battlefield");
		m_Field.Init(&Ini, "Field");
		m_Cave.Init(&Ini, "Cave");
		m_Others.Init(&Ini, "Others");
		
		int nBtnPosX = 0, nBtnPosY = 0;
		m_fengxiang.Init(&Ini, "PureTextBtn");
		Ini.GetInteger2("fengxiang","BtnStartPos", &nBtnPosX, &nBtnPosY);
		m_fengxiang.SetPosition(nBtnPosX, nBtnPosY);
		
		m_bianjing.Init(&Ini, "PureTextBtn");
		Ini.GetInteger2("bianjing","BtnStartPos", &nBtnPosX, &nBtnPosY);
		m_bianjing.SetPosition(nBtnPosX, nBtnPosY);	
		
		m_yangzhou.Init(&Ini, "PureTextBtn");
		Ini.GetInteger2("yangzhou","BtnStartPos", &nBtnPosX, &nBtnPosY);
		m_yangzhou.SetPosition(nBtnPosX, nBtnPosY);	
		
		m_linan.Init(&Ini, "PureTextBtn");
		Ini.GetInteger2("linan","BtnStartPos", &nBtnPosX, &nBtnPosY);
		m_linan.SetPosition(nBtnPosX, nBtnPosY);	
		
		m_xiangyang.Init(&Ini, "PureTextBtn");
		Ini.GetInteger2("xiangyang","BtnStartPos", &nBtnPosX, &nBtnPosY);
		m_xiangyang.SetPosition(nBtnPosX, nBtnPosY);	
		
		m_chengdu.Init(&Ini, "PureTextBtn");
		Ini.GetInteger2("chengdu","BtnStartPos", &nBtnPosX, &nBtnPosY);
		m_chengdu.SetPosition(nBtnPosX, nBtnPosY);
		
		m_dali.Init(&Ini, "PureTextBtn");
		Ini.GetInteger2("dali","BtnStartPos", &nBtnPosX, &nBtnPosY);
		m_dali.SetPosition(nBtnPosX, nBtnPosY);		
		
		m_Country.Hide();
		m_City.Hide();
		m_Capital.Hide();
		m_Tong.Hide();
		m_Battlefield.Hide();
		m_Field.Hide();
		m_Cave.Hide();
		m_Others.Hide();		
	}
	
	Wnd_AddWindow(this, WL_TOPMOST);
	return;
}

int KUiWorldmap::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nResult = false;

	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_KEYDOWN:
		CloseWindow();
		nResult = true;
		break;
	default:
		nResult = KWndWindow::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nResult;
}

void KUiWorldmap::UpdateData()
{
	if (g_pCoreShell)
	{
		KIniFile	Ini;
		if (Ini.Load(WORLD_MAP_INFO_FILE))
		{
			char	szBuffer[128];
			if (Ini.GetString("List", "WorldMapImage", "", szBuffer, sizeof(szBuffer)))	
			{
				SetImage(ISI_T_SPR, szBuffer, true);

				int nAreaX = -1, nAreaY = 0;
				char szMapType[32];
				char szTongInfo[32] = "Bang héi chiÕm lÜnh: %s";
				KUiSceneTimeInfo Info;
				g_pCoreShell->SceneMapOperation(GSMOI_SCENE_TIME_INFO, (unsigned int)&Info, 0);
				sprintf(szBuffer, "%d_MapPos", Info.nSceneId);
				Ini.GetInteger2("List", szBuffer, &nAreaX, &nAreaY);
				sprintf(szBuffer, "%d_MapType", Info.nSceneId);
				Ini.GetString("List", szBuffer, "", szMapType, sizeof(szMapType));
				if (nAreaX != -1)
				{	
					if (strcmp(szMapType, "Country") == 0)
					{
						int nWidth, nHeight;
						m_Country.GetSize(&nWidth,  &nHeight);
						m_Country.SetPosition((nAreaX - nWidth / 2) + 30,
						(nAreaY - nHeight / 2) + 17);
						m_Country.Show();
					}
					else if (strcmp(szMapType, "City") == 0)
					{
						int nWidth, nHeight;
						m_City.GetSize(&nWidth,  &nHeight);
						m_City.SetPosition((nAreaX - nWidth / 2) + 30,
						(nAreaY - nHeight / 2) + 17);
						m_City.Show();
					}
					else if (strcmp(szMapType, "Capital") == 0)
					{
						int nWidth, nHeight;
						m_Capital.GetSize(&nWidth,  &nHeight);
						m_Capital.SetPosition((nAreaX - nWidth / 2) + 30,
						(nAreaY - nHeight / 2) + 17);
						m_Capital.Show();
					}
					else if (strcmp(szMapType, "Tong") == 0)
					{
						int nWidth, nHeight;
						m_Tong.GetSize(&nWidth,  &nHeight);
						m_Tong.SetPosition((nAreaX - nWidth / 2) + 30,
						(nAreaY - nHeight / 2) + 17);
						m_Tong.Show();
					}
					else if (strcmp(szMapType, "Battlefield") == 0)
					{
						int nWidth, nHeight;
						m_Battlefield.GetSize(&nWidth,  &nHeight);
						m_Battlefield.SetPosition((nAreaX - nWidth / 2) + 30,
						(nAreaY - nHeight / 2) + 17);
						m_Battlefield.Show();
					}
					else if (strcmp(szMapType, "Field") == 0)
					{
						int nWidth, nHeight;
						m_Field.GetSize(&nWidth,  &nHeight);
						m_Field.SetPosition((nAreaX - nWidth / 2) + 30,
						(nAreaY - nHeight / 2) + 17);
						m_Field.Show();
					}
					else if (strcmp(szMapType, "Cave") == 0)
					{
						int nWidth, nHeight;
						m_Cave.GetSize(&nWidth,  &nHeight);
						m_Cave.SetPosition((nAreaX - nWidth / 2) + 30,
						(nAreaY - nHeight / 2) + 17);
						m_Cave.Show();
					}
					else if (strcmp(szMapType, "Others") == 0)
					{
						int nWidth, nHeight;
						m_Others.GetSize(&nWidth,  &nHeight);
						m_Others.SetPosition((nAreaX - nWidth / 2) + 30,
						(nAreaY - nHeight / 2) + 17);
						m_Others.Show();
					}
				}
				
				sprintf(szBuffer, szTongInfo, "V« hÖ");
				m_fengxiang.SetText(szBuffer);
				
				sprintf(szBuffer, szTongInfo, "V« hÖ");
				m_bianjing.SetText(szBuffer);
				
				sprintf(szBuffer, szTongInfo, "V« hÖ");
				m_yangzhou.SetText(szBuffer);
				
				sprintf(szBuffer, szTongInfo, "V« hÖ");
				m_linan.SetText(szBuffer);
				
				sprintf(szBuffer, szTongInfo, "V« hÖ");
				m_xiangyang.SetText(szBuffer);
				
				sprintf(szBuffer, szTongInfo, "V« hÖ");
				m_chengdu.SetText(szBuffer);
				
				sprintf(szBuffer, szTongInfo, "V« hÖ");
				m_dali.SetText(szBuffer);
				
			}
		}
		Ini.Clear();
	}
}

//»î¶¯º¯Êý
void KUiWorldmap::Breathe()
{
	if (m_Country.IsVisible())
		m_Country.NextFrame();
	else if (m_City.IsVisible())
		m_City.NextFrame();
	else if (m_Capital.IsVisible())
		m_Capital.NextFrame();
	else if (m_Tong.IsVisible())
		m_Tong.NextFrame();
	else if (m_Battlefield.IsVisible())
		m_Battlefield.NextFrame();
	else if (m_Field.IsVisible())
		m_Field.NextFrame();
	else if (m_Cave.IsVisible())
		m_Cave.NextFrame();
	else if (m_Others.IsVisible())
		m_Others.NextFrame();
}