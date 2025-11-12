#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/Wnds.h"
#include "../Elem/WndMessage.h"
#include "../Elem/MouseHover.h"
#include "../elem/PopupMenu.h"
#include "UiAuto.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../Engine/src/Text.h"
#include "../../../Engine/src/KDebug.h"
#include "../../../core/src/coreshell.h"
#include <crtdbg.h>
#include "UiSysMsgCentre.h"


extern iRepresentShell*	g_pRepresentShell;
extern iCoreShell*		g_pCoreShell;

KUiPlayerItem*	m_pNearbyPlayersList;

KUiAuto* KUiAuto::m_pSelf = NULL;

#define		SCHEME_INI_AUTO			"\\Ui\\Ui3\\UiAutoPlayNew.ini"
#define		MSG_NON_SETTINGS		"Kh«ng thiÕt lËp"
#define		MOVEMPS_KEYNAME				"%d:%d/%d"
#define		MENU_ROOM		0x00
#define		MENU_FOLLOW		0x13
#define		MENU_PLAYER		0x12
#define	SELECT_TYPE_PICK_ITEM		0x001
#define	MENU_SELECT_FOLLOW_PEOPLE	0x002
#define	MENU_SELECT_AURA_SKILL1		0x003
#define	MENU_SELECT_AURA_SKILL2		0x004
#define	MENU_SELECT_PICK_TYPE		0x005
#define	MENU_SELECT_SORT_MAGIC		0x006
#define	MENU_SELECT_CHOOSE_TEAM		0x007
#define	MENU_SELECT_SUPPORT_SKILL1	0x008
#define	MENU_SELECT_SUPPORT_SKILL2	0x009
#define	MENU_SELECT_SUPPORT_SKILL3	0x010
#define	MENU_SELECT_AUTO_ATTACK		0x011


#define		MSG_FORMAT_MAGIC				"%s >= %d"

/*******************************************************************
Tab chinh
*******************************************************************/
KUiAuto* KUiAuto::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiAuto;
		if (m_pSelf)
		{
			m_pSelf->Initialize();
			m_pSelf->m_RecoverPad.Show();
		}
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
		
	}
	return m_pSelf;
}


void KUiAuto::CloseWindow()
{
	if (m_pSelf)
	{
		m_pSelf->Destroy();
		m_pSelf->m_RecoverPad.SaveRecoverSetting();
		m_pSelf->m_FightPad.SaveFightSetting();
		m_pSelf->m_PickPad.SavePickSetting();
		m_pSelf = NULL;
	}
}

KUiAuto* KUiAuto::GetIfVisible()
{
if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

void KUiAuto::Initialize()
{
	m_RecoverPad.Initialize();
	AddPage(&m_RecoverPad, &m_RecoverBtn);	
	m_FightPad.Initialize();
	AddPage(&m_FightPad, &m_FightBtn);
	m_PickPad.Initialize();
	AddPage(&m_PickPad, &m_PickBtn);
	m_OtherPad.Initialize();
	
	AddChild(&m_StartBtn);
	AddChild(&m_CloseBtn);
	
	IsAuto = FALSE;
	LoadScheme();
	Wnd_AddWindow(this);
}

void KUiAuto::LoadScheme()
{
	KIniFile	Ini;
	if (m_pSelf && Ini.Load(SCHEME_INI_AUTO))
	{
		m_pSelf->Init(&Ini, "Main");			
		m_pSelf->m_FightBtn.Init(&Ini, "FightBtn");
		m_pSelf->m_RecoverBtn.Init(&Ini, "RecoverBtn");
		m_pSelf->m_PickBtn.Init(&Ini, "PickBtn");//nhat do
		m_pSelf->m_OtherBtn.Init(&Ini, "OtherBtn");
		
		m_pSelf->m_CloseBtn.Init(&Ini, "CloseBtn");
		m_pSelf->m_StartBtn.Init(&Ini, "StartBtn");			

		m_pSelf->m_bIsCheck = FALSE;
		m_pSelf->m_FightPad.LoadScheme();
		m_pSelf->m_RecoverPad.LoadScheme();
		m_pSelf->m_PickPad.LoadScheme();
		m_pSelf->m_OtherPad.LoadScheme();
		if (g_pCoreShell->GetAutoFlag()) {
			m_pSelf->IsAuto = TRUE;
			m_pSelf->UpdateButton(m_pSelf->IsAuto);
		}
		else
		{
			m_pSelf->IsAuto = FALSE;
			m_pSelf->UpdateButton(m_pSelf->IsAuto);
		}
	}
		
}

int KUiAuto::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_StartBtn)
		{
			if (IsAuto == FALSE)
			{
				IsAuto = TRUE;
				UpdateButton(IsAuto);
				OnStart();
			}
			else
			{
				IsAuto = FALSE;
				UpdateButton(IsAuto);
				OnStop();
			}
		}		
		else if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
			CloseWindow();		
		break;
	}
	return KWndPageSet::WndProc(uMsg, uParam, nParam);
}

void KUiAuto::UpdateButton(BOOL nIsAuto)
{
	if 	(nIsAuto)
		m_StartBtn.CheckButton(true);
	else
		m_StartBtn.CheckButton(false);
}

void KUiAuto::OnStart()
{	
	m_bIsCheck = TRUE;
	g_pCoreShell->SetActiveAutoPlay(m_bIsCheck);
}

void KUiAuto::OnStop()
{
	m_bIsCheck = FALSE;
	g_pCoreShell->SetActiveAutoPlay(m_bIsCheck);
}

void KUiAuto::OnSave()
{	

}

void KUiAuto::UpdateSceneTimeInfo(KUiSceneTimeInfo* pInfo)
{
	if (m_pSelf && pInfo)
		m_pSelf->m_FightPad.m_CurrentMps = *pInfo;
}
/*******************************************************************
	chien dau
*******************************************************************/
char Array_AutoAttack[][50] = 
{
	"Qu¸i VËt",
	"Ng­êi Ch¬i",
	"Mäi §èi T­îng",
};

void KUiFight::Initialize()
{
	AddChild(&m_Fight);
	AddChild(&m_Range);
	AddChild(&m_FightCBtn);	
	
	AddChild(&m_AutoAttackBtn);	
	AddChild(&m_AutoAttackList);	
	AddChild(&m_AutoAttackEdit);	
	AddChild(&m_QDiemXY);
	AddChild(&m_QDiemBtn);	
	//AddChild(&m_FollowBtn);	
	//AddChild(&m_FollowListBtn);
	//AddChild(&m_NameFollowPeople);

    AddChild(&m_FollowChecker);// theo sau
	AddChild(&m_FollowText);// theo sau
	AddChild(&m_FollowShadow);// theo sau
	AddChild(&m_FollowEdit); // theo sau
	AddChild(&m_FollowPopupBtn);// theo sau
	AddChild(&m_FollowRadiusEdit);  // theo sau

	AddChild(&m_OverTargetCK);
	AddChild(&m_OverTargetTxt);

    
/*	AddChild(&m_MoveMpsCK);
	AddChild(&m_MoveMpsText);
	AddChild(&m_MoveMpsShadow);
	AddChild(&m_MoveMpsL);
	AddChild(&m_MoveMpsL_Scroll);
	m_MoveMpsL.SetScrollbar(&m_MoveMpsL_Scroll);
	AddChild(&m_MoveMpsGetBtn);
	AddChild(&m_MoveMpsDelAllBtn);	*/

	AddChild(&m_DiChuyenToaDoBtn);
	
	AddChild(&m_CoordGetBtn);
	AddChild(&m_CoordDelBtn);
	AddChild(&m_CoordDelAllBtn);
	AddChild(&m_CoordL);	
	AddChild(&m_CoordL_Scroll);
	m_CoordL.SetScrollbar(&m_CoordL_Scroll);	
	
	AddChild(&m_Support1Btn);	
	AddChild(&m_Support2Btn);	
	AddChild(&m_Support3Btn);		
	AddChild(&m_Support1Edit);	
	AddChild(&m_Support2Edit);	
	AddChild(&m_Support3Edit);		
	AddChild(&m_Support1List);	
	AddChild(&m_Support2List);	
	AddChild(&m_Support3List);		
	
   memset(m_MoveMpsList, 0, sizeof(m_MoveMpsList));	
    //memset(m_nMpsList, 0, sizeof(m_nMpsList));
	nRangeNum = 512;
	nFightNum = 64;	

    memset(m_szFollowName, 0, sizeof(m_szFollowName)); // theo sau
	memset(m_CurrentSelMagic, 0, sizeof(m_CurrentSelMagic));
//	memset(m_cFollowName, 0, sizeof(m_cFollowName));	
}
void KUiFight::LoadScheme()
{
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO))
	{
		KWndPage::Init(&Ini, "MainFight");
		m_Range.Init(&Ini, "EditRange");
		m_Fight.Init(&Ini, "EditFight");
		m_FightCBtn.Init(&Ini, "FightCBtn");

		m_AutoAttackBtn.Init(&Ini, "AutoAttackBtn");
		m_AutoAttackList.Init(&Ini, "AutoAttackList");
		m_AutoAttackEdit.Init(&Ini, "AutoAttackEdit");		
		m_QDiemXY.Init(&Ini, "AutoQDiemXY");
		m_QDiemBtn.Init(&Ini, "QDiemBtn");
		//m_FollowBtn.Init(&Ini, "FollowBtn");		
		//m_NameFollowPeople.Init(&Ini, "NameFollowPeople");	
		//m_FollowListBtn.Init(&Ini, "FollowListBtn");
     	m_FollowChecker.Init(&Ini, "FollowChecker");// theo sau
	    m_FollowText.Init(&Ini, "FollowText");// theo sau
	    m_FollowShadow.Init(&Ini, "FollowShadow");// theo sau
	    m_FollowEdit.Init(&Ini, "FollowEdit"); // theo sau
    	m_FollowPopupBtn.Init(&Ini, "FollowPopupBtn");// theo sau
	    m_FollowRadiusEdit.Init(&Ini, "FollowRadiusEdit");// theo sau	
        
        m_OverTargetCK.Init(&Ini, "OverTargetCK");
     	m_OverTargetTxt.Init(&Ini, "OverTargetTxt");	

      /*  m_MoveMpsText.Init(&Ini, "MoveMpsText");
	    m_MoveMpsCK.Init(&Ini, "MoveMpsCK");
	    m_MoveMpsShadow.Init(&Ini, "MoveMpsShadow");
	    m_MoveMpsL.Init(&Ini, "MoveMpsL");
	    m_MoveMpsL_Scroll.Init(&Ini, "MoveMpsL_Scroll");
	    m_MoveMpsGetBtn.Init(&Ini, "MoveMpsGetBtn");
	    m_MoveMpsDelAllBtn.Init(&Ini, "MoveMpsDelAllBtn");	*/

		m_DiChuyenToaDoBtn.Init(&Ini, "DiChuyenToaDoBtn");
		m_CoordGetBtn.Init(&Ini, "CoordGetBtn");
		m_CoordDelBtn.Init(&Ini, "CoordDelBtn");
		m_CoordDelAllBtn.Init(&Ini, "CoordDelAllBtn");
		m_CoordL.Init(&Ini, "CoordL");			
		m_CoordL_Scroll.Init(&Ini, "CoordL_Scroll");

		m_Support1Btn.Init(&Ini, "Support1Btn");
		m_Support2Btn.Init(&Ini, "Support2Btn");
		m_Support3Btn.Init(&Ini, "Support3Btn");
		m_Support1Edit.Init(&Ini, "Support1Edit");
		m_Support2Edit.Init(&Ini, "Support2Edit");
		m_Support3Edit.Init(&Ini, "Support3Edit");
		m_Support1List.Init(&Ini, "Support1List");
		m_Support2List.Init(&Ini, "Support2List");
		m_Support3List.Init(&Ini, "Support3List");		
		
		m_Range.SetIntText(nRangeNum);
		m_Fight.SetIntText(nFightNum);
		m_Support1Edit.SetText(MSG_NON_SETTINGS);
		m_Support2Edit.SetText(MSG_NON_SETTINGS);
		m_Support3Edit.SetText(MSG_NON_SETTINGS);
	//	m_NameFollowPeople.SetText(MSG_NON_SETTINGS);		
		LoadFightSetting();
	}
	
}
void KUiFight::OnCheckInput()
{
	g_pCoreShell->PAIOperation(GPI_FOLLOW_PEOPLE_RADIUS, m_FollowRadiusEdit.GetIntNumber(), NULL, NULL); // theo sau
}

int KUiFight::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_EDIT_CHANGE:
        OnCheckInput();
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,7,m_Range.GetIntNumber());
		break;
	case WND_N_BUTTON_CLICK:

        if (uParam == (unsigned int)(KWndWindow*)&m_FollowChecker)
		{
			if (m_FollowChecker.IsButtonChecked())
			{
				g_pCoreShell->PAIOperation(GPI_FOLLOW_PEOPLE, TRUE, NULL, NULL);
				m_FollowChecker.CheckButton(true);
	/*	if (m_DiChuyenToaDoBtn.IsButtonChecked())
		{
			m_DiChuyenToaDoBtn.CheckButton(false);
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,16,!IsCheck);			
		}*/
				if (m_DiChuyenToaDoBtn.IsButtonChecked())
				{
					m_DiChuyenToaDoBtn.CheckButton(false);
					g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 16, 0);
				}
			}
			else
			{
				g_pCoreShell->PAIOperation(GPI_FOLLOW_PEOPLE, FALSE, NULL, NULL);
				m_FollowChecker.CheckButton(false);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_FollowPopupBtn)
			PlayerList(MENU_FOLLOW); // loi day 
		else if (uParam == (unsigned int)(KWndWindow*)&m_DiChuyenToaDoBtn)
		{
			if (m_DiChuyenToaDoBtn.IsButtonChecked())
			{
				m_DiChuyenToaDoBtn.CheckButton(true);
			    g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 16, 1);

				if (m_FollowChecker.IsButtonChecked())
				{
					g_pCoreShell->PAIOperation(GPI_FOLLOW_PEOPLE, FALSE, NULL, NULL);
					m_FollowChecker.CheckButton(false);
				}
			}
			else
			{
				m_DiChuyenToaDoBtn.CheckButton(false);
				g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 16, 0);
			}
		}
     	else if (uParam == (unsigned int)(KWndWindow*)&m_OverTargetCK)
		{
			if (m_OverTargetCK.IsButtonChecked())
			{
				m_OverTargetCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_OVER_TARGET, TRUE, NULL, NULL);
			}
			else
			{
				m_OverTargetCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_OVER_TARGET, FALSE, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_AutoAttackBtn)
			OnAutoAttack();
		else if (uParam == (unsigned int)(KWndWindow*)&m_AutoAttackList)
			PopUpAutoAttack();			
	//	else if (uParam == (unsigned int)(KWndWindow*)&m_FollowBtn)
	//		OnFollowPeople();
		else if (uParam == (unsigned int)(KWndWindow*)&m_Support1Btn)
			OnBuffSkill1();
		else if (uParam == (unsigned int)(KWndWindow*)&m_Support2Btn)
			OnBuffSkill2();
		else if (uParam == (unsigned int)(KWndWindow*)&m_Support3Btn)
			OnBuffSkill3();	
		else if (uParam == (unsigned int)(KWndWindow*)&m_Support1List)
			SupportSkillPopup(enumMagic_ID1);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Support2List)
			SupportSkillPopup(enumMagic_ID2);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Support3List)
			SupportSkillPopup(enumMagic_ID3);			
	//	else if (uParam == (unsigned int)(KWndWindow*)&m_FollowListBtn)
	//		PopUpFollow();
		else if (uParam == (unsigned int)(KWndWindow*)&m_DiChuyenToaDoBtn)
			OnActiveMove();  // do chuyen toa do
		else if (uParam == (unsigned int)(KWndWindow*)&m_FightCBtn)
			OnFightRadius();
		else if (uParam == (unsigned int)(KWndWindow*)&m_QDiemBtn)
			OnArrayRadius();						
		else if (uParam == (unsigned int)(KWndWindow*)&m_CoordGetBtn)
		{
			InsertMoveMpsList(m_CurrentMps.nSceneId, m_CurrentMps.nScenePos0*32, m_CurrentMps.nScenePos1*64);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CoordDelBtn)
		{
			int nSel = m_CoordL.GetCurSel();
			int nCount = 0;
			int i;
			memset(m_MoveMpsList[nSel], 0, sizeof(m_MoveMpsList[nSel]));	

			for (i = 0; i < defMAX_AUTO_MOVEMPSL; i++)
			{
				if (m_MoveMpsList[i][0])
				{
					nCount++;
				}
			}
			
			for (i = nSel; i < nCount; i++)			
			{
				for (int j = 0; j < 3; j++)
				{
					m_MoveMpsList[i][j] = m_MoveMpsList[i+1][j];
					m_MoveMpsList[i+1][j] = 0;				
				}		
			}			
			nCount--;			
							
			SetMoveMpsList();			
		}		
		else if (uParam == (unsigned int)(KWndWindow*)&m_CoordDelAllBtn)
		{
			memset(m_MoveMpsList, 0, sizeof(m_MoveMpsList));
			SetMoveMpsList();
		}
        /*  else if (uParam == (unsigned int)(KWndWindow*)&m_MoveMpsGetBtn)
			InsertMoveMpsList(m_SceneInfo.nSceneId, m_SceneInfo.nScenePos0, m_SceneInfo.nScenePos1);
		else if (uParam == (unsigned int)(KWndWindow*)&m_MoveMpsDelAllBtn)
		{
			memset(m_nMpsList, 0, sizeof(m_nMpsList));
			SetMoveMpsList();
		}*/
		SaveFightSetting();
		break;
	
	case WND_M_MENUITEM_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			/*if (HIWORD(nParam) == MENU_SELECT_FOLLOW_PEOPLE && (short)(LOWORD(nParam)) >= 0)
				ProcessPopUpFollow(LOWORD(nParam));*/
            if (HIWORD(nParam) == MENU_FOLLOW && (short)(LOWORD(nParam)) >= 0)
			{
				strcpy(m_szFollowName, m_pNearbyPlayersList[(short)(LOWORD(nParam))].Name); // theo sau
				m_FollowEdit.SetText(m_szFollowName);
				g_pCoreShell->PAIOperation(GPI_FOLLOW_PEOPLE_NAME, (unsigned int)&m_szFollowName, NULL, NULL);
			}
			else if (HIWORD(nParam) == MENU_SELECT_SUPPORT_SKILL1 && (short)(LOWORD(nParam)) >= 0)
				SetSupportSkill(enumMagic_ID1, LOWORD(nParam));
			else if (HIWORD(nParam) == MENU_SELECT_SUPPORT_SKILL2 && (short)(LOWORD(nParam)) >= 0)
				SetSupportSkill(enumMagic_ID2, LOWORD(nParam));	
			else if (HIWORD(nParam) == MENU_SELECT_SUPPORT_SKILL3 && (short)(LOWORD(nParam)) >= 0)
				SetSupportSkill(enumMagic_ID3, LOWORD(nParam));	
			else if (HIWORD(nParam) == MENU_SELECT_AUTO_ATTACK && (short)(LOWORD(nParam)) >= 0)
				ProcessPopUpAttack(LOWORD(nParam));				
		}
		break;
     /* case WND_N_SCORLLBAR_POS_CHANGED:
		if (uParam == (unsigned int)&m_MoveMpsL_Scroll)
			m_MoveMpsL.SetTopItemIndex(nParam);
		break;*/
	case WND_N_SCORLLBAR_POS_CHANGED:
		if (uParam == (unsigned int)&m_CoordL_Scroll)
			m_CoordL.SetTopItemIndex(nParam);
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

BOOL KUiFight::InsertMoveMpsList(int nSubWorldId, int nMpsX, int nMpsY)
{
	for (int i = 0; i < defMAX_AUTO_MOVEMPSL; i ++)
	{
		if (m_MoveMpsList[i][0] == nSubWorldId && 
			m_MoveMpsList[i][1] == nMpsX && 
			m_MoveMpsList[i][2] == nMpsY)
		{
			i = defMAX_AUTO_MOVEMPSL;
			break;
		}
		else if (m_MoveMpsList[i][0] == 0 && 
				m_MoveMpsList[i][1] == 0 && 
				m_MoveMpsList[i][2] == 0)
		{
			break;
		}
	}
	if (i < defMAX_AUTO_MOVEMPSL)
	{
		m_MoveMpsList[i][0] = nSubWorldId;
		m_MoveMpsList[i][1] = nMpsX;
		m_MoveMpsList[i][2] = nMpsY;
		SetMoveMpsList();
	}
	return TRUE;
}

void KUiFight::SetMoveMpsList()
{
	char szList[32];
	char szListView[32];
	m_CoordL.ResetContent();
	for (int i = 0; i < defMAX_AUTO_MOVEMPSL; i ++)
	{
		g_pCoreShell->SetMoveMap(GAUTO_AUTO_MOVEMPSID, i, m_MoveMpsList[i][0]);
		g_pCoreShell->SetMoveMap(GAUTO_AUTO_MOVEMPSX, i, m_MoveMpsList[i][1]);
		g_pCoreShell->SetMoveMap(GAUTO_AUTO_MOVEMPSY, i, m_MoveMpsList[i][2]);

		if(m_MoveMpsList[i][0])
		{
			sprintf(szList, "%d:%d/%d", m_MoveMpsList[i][0], m_MoveMpsList[i][1], m_MoveMpsList[i][2]);
			sprintf(szListView, "%d:%d/%d", m_MoveMpsList[i][0], m_MoveMpsList[i][1]/8/32, m_MoveMpsList[i][2]/16/32);
			m_CoordL.AddString(i, szListView);
		}
	}
}

void KUiFight::PlayerList(int uParam)
{
	if (m_pNearbyPlayersList)
	{
		free (m_pNearbyPlayersList);
		m_pNearbyPlayersList = NULL;
	}
	int nActionDataCount = g_pCoreShell->GetGameData(GDI_NEARBY_PLAYER_LIST, 0, 0);
	if (nActionDataCount > 0) 
	{
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	m_pNearbyPlayersList = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * nActionDataCount);
	if (m_pNearbyPlayersList)
	{
		g_pCoreShell->GetGameData(GDI_NEARBY_PLAYER_LIST, (unsigned int)m_pNearbyPlayersList, nActionDataCount);
			for (int i = 0; i < nActionDataCount; i++)
			{
				strncpy(pSelUnitMenu->Items[i].szData, m_pNearbyPlayersList[i].Name, sizeof(m_pNearbyPlayersList[i].Name));
				pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
				pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
				pSelUnitMenu->nNumItem++;
			}
		}
		int x, y;
		GetAbsolutePos(&x, &y);
		if (uParam == MENU_FOLLOW)
			m_FollowShadow.GetAbsolutePos(&x, &y);
		pSelUnitMenu->nX = x;
		pSelUnitMenu->nY = y;
		KPopupMenu::Popup(pSelUnitMenu, this, uParam);
	}
}
/*
BOOL KUiFight::InsertMoveMpsList(int nSubWorldId, int nMpsX, int nMpsY)
{
	for (int i = 0; i < MAX_AUTO_LIST; i ++)
	{
		if (m_nMpsList[i][0] == nSubWorldId && 
			m_nMpsList[i][1] == nMpsX && 
			m_nMpsList[i][2] == nMpsY)
		{
			i = MAX_AUTO_LIST;
			break;
		}
		else if (m_nMpsList[i][0] == 0 && 
				m_nMpsList[i][1] == 0 && 
				m_nMpsList[i][2] == 0)
		{
			break;
		}
	}
	if (i < MAX_AUTO_LIST)
	{
		m_nMpsList[i][0] = nSubWorldId;
		m_nMpsList[i][1] = nMpsX;
		m_nMpsList[i][2] = nMpsY;
		SetMoveMpsList();
	}
	return TRUE;
}

void KUiFight::SetMoveMpsList()
{
	char szList[32];
    char szListView[32];
	m_MoveMpsL.ResetContent();
	for (int i = 0; i < MAX_AUTO_LIST; i ++)
	{
		g_pCoreShell->PAIOperation(GPI_AUTO_MOVEMPSID, i, m_nMpsList[i][0], NULL);
		g_pCoreShell->PAIOperation(GPI_AUTO_MOVEMPSX, i, m_nMpsList[i][1], NULL);
		g_pCoreShell->PAIOperation(GPI_AUTO_MOVEMPSY, i, m_nMpsList[i][2], NULL);

		if(m_nMpsList[i][0])
		{
			sprintf(szList, MOVEMPS_KEYNAME, m_nMpsList[i][0], m_nMpsList[i][1]/8, m_nMpsList[i][2]/8);
            sprintf(szListView, MOVEMPS_KEYNAME, m_nMpsList[i][0], m_nMpsList[i][1]/8/32, m_nMpsList[i][2]/16/32);
			m_MoveMpsL.AddString(i, szList);
		}
	}
}*/
void KUiFight::OnAutoAttack()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_AutoAttackBtn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_AutoAttackBtn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,5,IsCheck);	
}

void KUiFight::OnBuffSkill1()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_Support1Btn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_Support1Btn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,0,IsCheck);	
}

void KUiFight::OnBuffSkill2()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_Support2Btn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_Support2Btn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,1,IsCheck);	
}

void KUiFight::OnBuffSkill3()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_Support3Btn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_Support3Btn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,2,IsCheck);	
}


void KUiFight::OnFightRadius()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,40,m_Fight.GetIntNumber());
		m_FightCBtn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,40,m_Fight.GetIntNumber());
		m_FightCBtn.CheckButton(false);
	}	
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,41,IsCheck);	
}

void KUiFight::OnArrayRadius()  // di chuyen quanh diem
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,7,m_Range.GetIntNumber());
		char	cQDiemXY[32];
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,47, (unsigned int)cQDiemXY);
		m_QDiemBtn.CheckButton(true);
		m_QDiemXY.SetText(cQDiemXY);
		
		if (m_DiChuyenToaDoBtn.IsButtonChecked())
		{
			m_DiChuyenToaDoBtn.CheckButton(false);
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,16,!IsCheck);			
		}
	}
	else
	{
		IsCheck = FALSE;
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,7,m_Range.GetIntNumber());
		m_QDiemBtn.CheckButton(false);
		m_QDiemXY.SetText("");
	}	
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,6,IsCheck);	
}

void KUiFight::PopUpFollow()
{
	if (m_pNearbyPlayersList)
	{
		free (m_pNearbyPlayersList);
		m_pNearbyPlayersList = NULL;
	}
	int nActionDataCount = g_pCoreShell->GetGameData(GDI_NEARBY_PLAYER_LIST, 0, 0);
	if (nActionDataCount > 0) 
	{
		struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
		if (pSelUnitMenu == NULL)
			return;
		KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
		pSelUnitMenu->nNumItem = 0;
		pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;

		m_pNearbyPlayersList = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * nActionDataCount);
		if (m_pNearbyPlayersList)
		{
			g_pCoreShell->GetGameData(GDI_NEARBY_PLAYER_LIST, (unsigned int)m_pNearbyPlayersList, nActionDataCount);
			for (int i = 0; i< nActionDataCount;i++)
			{
				strncpy(pSelUnitMenu->Items[i].szData, m_pNearbyPlayersList[i].Name, 63);
				pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
				pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
				pSelUnitMenu->nNumItem++;
			}
		}
		int Left, Top, nWidth;
		GetSize(&nWidth, NULL);
		GetAbsolutePos(&Left, &Top);
		int nX = 0, nY = 0;
		KIniFile	Ini;
		if (Ini.Load(SCHEME_INI_AUTO))
			Ini.GetInteger2("PopupPosition","PopUpFollow",&nX, &nY);
			
		pSelUnitMenu->nX = Left + nX;
		pSelUnitMenu->nY = Top + nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_FOLLOW_PEOPLE);
      
	} 
	else 
	{
		KSystemMessage	Msg;
		Msg.byConfirmType = SMCT_NONE;
		Msg.byParamSize = 0;
		Msg.byPriority = 0;
		Msg.eType = SMT_NORMAL;
		Msg.uReservedForUi = 0;
		strcpy(Msg.szMessage, "Xung quanh kh«ng cã ai c¶.");
		KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
	}
}
/*
void KUiFight::ProcessPopUpFollow(int nAction)
{
	strcpy(m_cFollowName, m_pNearbyPlayersList[nAction].Name);
	m_NameFollowPeople.SetText(m_cFollowName);
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,4,(unsigned int)&m_cFollowName);
}
*/
void KUiFight::PopUpAutoAttack()
{
	int nActionDataCount = sizeof(Array_AutoAttack) / sizeof(Array_AutoAttack[0]);
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;

	for (int i = 0; i < nActionDataCount; i++)
	{
		strncpy(pSelUnitMenu->Items[i].szData, Array_AutoAttack[i], 63);
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int Left, Top, nWidth;
	GetSize(&nWidth, NULL);
	GetAbsolutePos(&Left, &Top);
	int nX = 0, nY = 0;
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO))
		Ini.GetInteger2("PopupPosition","PopUpAutoAttack",&nX, &nY);
	
	pSelUnitMenu->nX = Left + nX;
	pSelUnitMenu->nY = Top + nY;
	KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_AUTO_ATTACK);
}

void KUiFight::ProcessPopUpAttack(int nAction)
{
	m_nAction = nAction;
	if (nAction == 0)
	{
		m_AutoAttackEdit.SetText(Array_AutoAttack[0]);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,44,TRUE);			
	}
	else if (nAction == 1)
	{
		m_AutoAttackEdit.SetText(Array_AutoAttack[1]);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,45,TRUE);			
	}
	else
	{
		m_AutoAttackEdit.SetText(Array_AutoAttack[2]);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,46,TRUE);
	}
}

/*
void KUiFight::OnFollowPeople()
{
	if (IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_FollowBtn.CheckButton(true);
        g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,3,IsCheck);
	}
	else
	{
		IsCheck = FALSE;
		m_FollowBtn.CheckButton(false);
        g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,3,IsCheck);
	}
}
*/
void KUiFight::OnActiveMove()
{
	if (IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_DiChuyenToaDoBtn.CheckButton(true);
		
		if (m_QDiemBtn.IsButtonChecked())
		{
			m_QDiemBtn.CheckButton(false);
			m_QDiemXY.SetText("");
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,6,!IsCheck);	
		}		
	}
	else
	{
		IsCheck = FALSE;
		m_DiChuyenToaDoBtn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,16,IsCheck);	
}

void KUiFight::SupportSkillPopup(int nIndex)
{
	int nActionDataCount = g_pCoreShell->FindSkillInfo(0,0) + 1;
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		if (i == 0)
		{
			strncpy(pSelUnitMenu->Items[i].szData, MSG_NON_SETTINGS, sizeof(MSG_NON_SETTINGS));
			pSelUnitMenu->Items[i].uID = 0;
		}
		else
		{
			int _nID = g_pCoreShell->FindSkillInfo(0,i);
			g_pCoreShell->GetSkillName(_nID, pSelUnitMenu->Items[i].szData);
			pSelUnitMenu->Items[i].uID = _nID;
		}
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int nX = 0, nY = 0;
	int Left, Top, nWidth;
	GetSize(&nWidth, NULL);
	GetAbsolutePos(&Left, &Top);
	KIniFile	Ini;
	switch (nIndex)
	{
	case enumMagic_ID1:
		if (Ini.Load(SCHEME_INI_AUTO))
			Ini.GetInteger2("PopupPosition","SPPosition1",&nX, &nY);
		pSelUnitMenu->nX = Left + nX;
		pSelUnitMenu->nY = Top + nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_SUPPORT_SKILL1);
		break;
	case enumMagic_ID2:
		if (Ini.Load(SCHEME_INI_AUTO))
			Ini.GetInteger2("PopupPosition","SPPosition2",&nX, &nY);
		pSelUnitMenu->nX = Left + nX;
		pSelUnitMenu->nY = Top + nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_SUPPORT_SKILL2);
		break;
	case enumMagic_ID3:
		if (Ini.Load(SCHEME_INI_AUTO))
			Ini.GetInteger2("PopupPosition","SPPosition3",&nX, &nY);
		pSelUnitMenu->nX = Left + nX;
		pSelUnitMenu->nY = Top + nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_SUPPORT_SKILL3);
		break;		
	}
}

void KUiFight::SetSupportSkill(int nIndex, int nID)
{
	switch (nIndex)
	{
	case enumMagic_ID1:
		if (nID > 0)
		{
			m_CurrentSelMagic[enumSel_Support_Magic1] = nID;
			char Name[128];
			g_pCoreShell->GetSkillName(nID, Name);
			m_Support1Edit.SetText(Name);
		}
		else
		{
			m_CurrentSelMagic[enumSel_Support_Magic1] = 0;
			m_Support1Edit.SetText(MSG_NON_SETTINGS);
		}	
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,13,nID);
		break;
	case enumMagic_ID2:
		if (nID > 0)
		{
			m_CurrentSelMagic[enumSel_Support_Magic2] = nID;
			char Name[128];
			g_pCoreShell->GetSkillName(nID, Name);
			m_Support2Edit.SetText(Name);
		}
		else
		{
			m_CurrentSelMagic[enumSel_Support_Magic2] = 0;
			m_Support2Edit.SetText(MSG_NON_SETTINGS);
		}	
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,42,nID);
		break;
	case enumMagic_ID3:
		if (nID > 0)
		{
			m_CurrentSelMagic[enumSel_Support_Magic3] = nID;
			char Name[128];
			g_pCoreShell->GetSkillName(nID, Name);
			m_Support3Edit.SetText(Name);
		}
		else
		{
			m_CurrentSelMagic[enumSel_Support_Magic3] = 0;
			m_Support3Edit.SetText(MSG_NON_SETTINGS);
		}	
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,43,nID);
		break;		
	}
}

void KUiFight::LoadFightSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
    int nValue;
	int 	m_nRadius, m_nDistance, m_bAutoUseSkill1Btn, m_bAutoUseSkill2Btn, m_bAutoUseSkill3Btn;
	BOOL	m_bAutoAttack, m_bQDiemBtn, m_bFightCBtn,m_bDiChuyenToaDoBtn;
	if (pConfigFile)
	{
		pConfigFile->GetInteger("Fighting", "Range", 512, (int*)(&m_nRadius));
		m_Range.SetIntText(m_nRadius);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,7, m_nRadius);	
		
		pConfigFile->GetInteger("Fighting", "QDiemBtn", 0, (int*)(&m_bQDiemBtn));
		if (m_bQDiemBtn)
		{
			char	cQDiemXY[32];
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,47, (unsigned int)cQDiemXY);
			m_QDiemBtn.CheckButton(true);
			m_QDiemXY.SetText(cQDiemXY);
		}
		else
			m_QDiemBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,6, m_bQDiemBtn);	
		
		pConfigFile->GetInteger("Fighting", "Fight", 64, (int*)(&m_nDistance));
		m_Fight.SetIntText(m_nDistance);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,40, m_nDistance);		
		
		pConfigFile->GetInteger("Fighting", "FightCBtn", 0, (int*)(&m_bFightCBtn));
		if (m_bFightCBtn)
			m_FightCBtn.CheckButton(true);
		else
			m_FightCBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,41, m_bFightCBtn);		
		
		pConfigFile->GetInteger("Fighting", "DiChuyenToaDoBtn", 0, (int*)(&m_bDiChuyenToaDoBtn));
		if (m_bDiChuyenToaDoBtn)
			m_DiChuyenToaDoBtn.CheckButton(true);
		else
			m_DiChuyenToaDoBtn.CheckButton(false);			
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,16,m_bDiChuyenToaDoBtn);	

     	pConfigFile->GetInteger("Fighting", "OverTargetCK", 0, &nValue);
		g_pCoreShell->PAIOperation(GPI_OVER_TARGET, nValue > 0, NULL, NULL);
		m_OverTargetCK.CheckButton(nValue > 0);		
		
		pConfigFile->GetInteger("Fighting", "AutoUseSkill1Btn", 0, (int*)(&m_bAutoUseSkill1Btn));
		if (m_bAutoUseSkill1Btn)
			m_Support1Btn.CheckButton(true);			
		else
			m_Support1Btn.CheckButton(false);			
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,0,m_bAutoUseSkill1Btn);	

		pConfigFile->GetInteger("Fighting", "AutoUseSkill2Btn", 0, (int*)(&m_bAutoUseSkill2Btn));
		if (m_bAutoUseSkill2Btn)
			m_Support2Btn.CheckButton(true);			
		else
			m_Support2Btn.CheckButton(false);			
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,1,m_bAutoUseSkill2Btn);	

		pConfigFile->GetInteger("Fighting", "AutoUseSkill3Btn", 0, (int*)(&m_bAutoUseSkill3Btn));
		if (m_bAutoUseSkill3Btn)
			m_Support3Btn.CheckButton(true);			
		else
			m_Support3Btn.CheckButton(false);			
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,2,m_bAutoUseSkill3Btn);			

		char szKeyName[10];
		for (int i = 0; i < enumSel_Support_Magic_Num; i ++)
		{
			sprintf(szKeyName, "SelMagic_%d", i);
			pConfigFile->GetInteger("Fighting", szKeyName, 0, &m_CurrentSelMagic[i]);
			SetSupportSkill(i, m_CurrentSelMagic[i]);
		}					
		pConfigFile->GetInteger("Fighting", "Follow", 0, &nValue);
		m_FollowChecker.CheckButton(nValue > 0);
		g_pCoreShell->PAIOperation(GPI_FOLLOW_PEOPLE, nValue > 0, NULL, NULL);

		pConfigFile->GetString("Fighting", "FollowName", "", m_szFollowName, sizeof(m_szFollowName));  // theo sau 
		if (m_szFollowName[0])
			m_FollowEdit.SetText(m_szFollowName);
		else
			m_FollowEdit.SetText("Ch­a râ");
		g_pCoreShell->PAIOperation(GPI_FOLLOW_PEOPLE_NAME, (unsigned int)&m_szFollowName, NULL, NULL);
		pConfigFile->GetInteger("Fighting", "FollowRadius", 0, &nValue);
		if (nValue <= 0)
			nValue = 300;
		m_FollowRadiusEdit.SetIntText(nValue);// theo sau

		pConfigFile->GetInteger("Fighting", "AutoAttack", 0, (int*)(&m_bAutoAttack));
		if (m_bAutoAttack)
			m_AutoAttackBtn.CheckButton(true);
		else
			m_AutoAttackBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,5,m_bAutoAttack);		
			
		for (i = 0; i < defMAX_AUTO_MOVEMPSL; i++)
		{
			sprintf(szKeyName, "MapID%d", i);
			pConfigFile->GetInteger("Fighting", szKeyName, 0, (int*)(&m_MoveMpsList[i][0]));
			sprintf(szKeyName, "MapX%d", i);
			pConfigFile->GetInteger("Fighting", szKeyName, 0, (int*)(&m_MoveMpsList[i][1]));
			sprintf(szKeyName, "MapY%d", i);
			pConfigFile->GetInteger("Fighting", szKeyName, 0, (int*)(&m_MoveMpsList[i][2]));			
		}
		SetMoveMpsList();	
		
		pConfigFile->GetInteger("Fighting", "FightKind", 0, &m_nAction);
		ProcessPopUpAttack(m_nAction);		
	}
	g_UiBase.CloseAutoSettingFile(true);
}

void KUiFight::SaveFightSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	if (pConfigFile)
	{
        char szKeyName[10];
		int nCount = 0;

		if (m_FightCBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Fighting", "FightCBtn", 1);
		else
			pConfigFile->WriteInteger("Fighting", "FightCBtn", 0);
			
		if (m_QDiemBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Fighting", "QDiemBtn", 1);
		else
			pConfigFile->WriteInteger("Fighting", "QDiemBtn", 0);	
			
		if (m_AutoAttackBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Fighting", "AutoAttack", 1);
		else
			pConfigFile->WriteInteger("Fighting", "AutoAttack", 0);	
			
		if (m_Support1Btn.IsButtonChecked())
			pConfigFile->WriteInteger("Fighting", "AutoUseSkill1Btn", 1);
		else
			pConfigFile->WriteInteger("Fighting", "AutoUseSkill1Btn", 0);
			
		if (m_Support2Btn.IsButtonChecked())
			pConfigFile->WriteInteger("Fighting", "AutoUseSkill2Btn", 1);
		else
			pConfigFile->WriteInteger("Fighting", "AutoUseSkill2Btn", 0);	

		if (m_Support3Btn.IsButtonChecked())
			pConfigFile->WriteInteger("Fighting", "AutoUseSkill3Btn", 1);
		else
			pConfigFile->WriteInteger("Fighting", "AutoUseSkill3Btn", 0);	
       // char szKeyName[10];
	//	int nCount = 0;
	//	for (int i = 0; i < MAX_AUTO_LIST; i++)
	//	{
	//		if (m_nMpsList[i][0])
	//		{
	//			sprintf(szKeyName, "%d", nCount);
	//			pConfigFile->WriteInteger3("MoveMpsList", szKeyName, m_nMpsList[i][0], m_nMpsList[i][1], m_nMpsList[i][2]);
	//			nCount++;
	//		}
	//	}
				
			
	//	char szKeyName[10];
		for (int i = 0; i < enumSel_Support_Magic_Num; i ++)
		{
			sprintf(szKeyName, "SelMagic_%d", i);
			pConfigFile->WriteInteger("Fighting", szKeyName, m_CurrentSelMagic[i]);	
		}		
			
/*		if (m_FollowBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Fighting", "FollowBtn", 1);
		else
			pConfigFile->WriteInteger("Fighting", "FollowBtn", 0);	
			
		pConfigFile->WriteString("Fighting", "FollowName", m_cFollowName);	*/
			
		if (m_DiChuyenToaDoBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Fighting", "DiChuyenToaDoBtn", 1);		
		else
			pConfigFile->WriteInteger("Fighting", "DiChuyenToaDoBtn", 0);
		pConfigFile->WriteInteger("Fighting", "OverTargetCK", 		m_OverTargetCK.IsButtonChecked());			
		pConfigFile->WriteInteger("Fighting", "Range", m_Range.GetIntNumber());
		pConfigFile->WriteInteger("Fighting", "Fight", m_Fight.GetIntNumber());
        pConfigFile->WriteInteger("Fighting", "Follow", 		m_FollowChecker.IsButtonChecked());// theo sau
		pConfigFile->WriteString("Fighting",  "FollowName", 	m_szFollowName);  // theo sau
		pConfigFile->WriteInteger("Fighting", "FollowRadius", 	m_FollowRadiusEdit.GetIntNumber());// theo sau
	//	pConfigFile->WriteInteger("Fighting", "MoveCK", 		m_MoveMpsCK.IsButtonChecked());  
		
		for (i = 0; i < defMAX_AUTO_MOVEMPSL; i++)
		{
			sprintf(szKeyName, "MapID%d", i);
			pConfigFile->WriteInteger("Fighting", szKeyName, m_MoveMpsList[i][0]);
			sprintf(szKeyName, "MapX%d", i);
			pConfigFile->WriteInteger("Fighting", szKeyName, m_MoveMpsList[i][1]);
			sprintf(szKeyName, "MapY%d", i);
			pConfigFile->WriteInteger("Fighting", szKeyName, m_MoveMpsList[i][2]);				
		}	
		
		pConfigFile->WriteInteger("Fighting", "FightKind", m_nAction);		
	}
	g_UiBase.CloseAutoSettingFile(true);
}
/*******************************************************************
	phuc hoi
*******************************************************************/
char Array_PTTeam[][50] = 
{
	"Tõ chèi lêi mêi",
	"Tù nhËn lêi mêi",
	"Tù tæ ®éi",
};
void KUiRecover::Initialize()
{
	AddChild(&m_Life);
	AddChild(&m_Mana);
	AddChild(&m_LifeTxt);
	AddChild(&m_ManaTxt);
	AddChild(&m_MoreMoney);
	AddChild(&m_DurabilityTxt);
	
	AddChild(&m_LifeBtn);	
	AddChild(&m_ManaBtn);	
	AddChild(&m_NoLifeBtn);	
	AddChild(&m_NoManaBtn);
	AddChild(&m_DurabilityBtn);
	AddChild(&m_MoreMoneyBtn);
	AddChild(&m_TeamBtn);
	AddChild(&m_TeamListBtn);
	AddChild(&m_GDBtn);
	AddChild(&m_TTLBtn);				
	AddChild(&m_BuffBtn);
	AddChild(&m_BuffNMTxt);
    
	

	AddChild(&m_BuyItemBtn);
	AddChild(&m_InventoryMIBtn);
	AddChild(&m_RepairBtn);
	AddChild(&m_ReturnPortalBtn);
	
	
	
	AddChild(&m_LifeTxt2);
	AddChild(&m_ManaTxt2);
	AddChild(&m_LifeBtn2);
	AddChild(&m_ManaBtn2);
	
	AddChild(&m_NameChooseTeam);
	AddChild(&m_ChuyenVongSangBtn);	
	AddChild(&m_Aura1Btn);	
	AddChild(&m_Aura2Btn);		
	AddChild(&m_Aura1Edit);	
	AddChild(&m_Aura2Edit);	
    
    
	IsBuyItem = FALSE;
	IsInventoryMI = FALSE;
	IsRepairItem = FALSE;
	IsReturnPortal = FALSE;

		

	nLifeNum = 500;
	nManaNum = 200;
	nTownNum = 200;
	nTown1Num = 50;

	nMoneyNum = 10;
	nDurabilityNum = 0;
	nBuffTxtNum = 0;
	nLifeTxtNum = 1000;
	nManaTxtNum = 1000;
	IsCheck = FALSE;
}
void KUiRecover::LoadScheme()
{
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO))
	{
		KWndPage::Init(&Ini, "MainRecover");
		m_Life		.Init(&Ini, "EditLife");
		m_Mana		.Init(&Ini, "EditMana");		
		m_LifeTxt	.Init(&Ini, "LifeTxt");
		m_ManaTxt	.Init(&Ini, "ManaTxt");		
		m_MoreMoney	.Init(&Ini, "MoreMoney");
		m_DurabilityTxt	.Init(&Ini, "DurabilityTxt");
		m_BuffNMTxt	.Init(&Ini, "BuffNMTxt");
		
		m_LifeTxt2	.Init(&Ini, "LifeTxt2");
		m_ManaTxt2	.Init(&Ini, "ManaTxt2");		
		m_LifeBtn2	.Init(&Ini, "LifeBtn2");		
		m_ManaBtn2	.Init(&Ini, "ManaBtn2");
		
		m_LifeBtn	.Init(&Ini, "LifeBtn");
		m_ManaBtn	.Init(&Ini, "ManaBtn");
		m_NoLifeBtn	.Init(&Ini, "NoLifeBtn");		
		m_NoManaBtn	.Init(&Ini, "NoManaBtn");	
		m_DurabilityBtn	.Init(&Ini, "DurabilityBtn");	
		m_MoreMoneyBtn	.Init(&Ini, "MoreMoneyBtn");	
		m_TeamBtn	.Init(&Ini, "TeamBtn");	
		m_TeamListBtn	.Init(&Ini, "TeamList");		
		m_GDBtn	.Init(&Ini, "GDBtn");	
		m_TTLBtn	.Init(&Ini, "TTLBtn");				
		m_BuffBtn	.Init(&Ini, "BuffBtn");
		m_NameChooseTeam	.Init(&Ini, "NameChooseTeam");	

		m_ChuyenVongSangBtn.Init(&Ini, "ChuyenVongSangBtn");	
		m_Aura1Btn.Init(&Ini, "Aura1Btn");
		m_Aura2Btn.Init(&Ini, "Aura2Btn");
		m_Aura1Edit.Init(&Ini, "Aura1Edit");
		m_Aura2Edit.Init(&Ini, "Aura2Edit");	
       
		m_BuyItemBtn.Init(&Ini, "BuyItemBtn");
		m_InventoryMIBtn.Init(&Ini, "InventoryMIBtn");
		m_RepairBtn.Init(&Ini, "RepairItemBtn");
		m_ReturnPortalBtn.Init(&Ini, "ReturnPortalBtn");
		
			
		
		m_Life.SetIntText(nLifeNum);
		m_Mana.SetIntText(nManaNum);
		m_LifeTxt.SetIntText(nLifeTxtNum);
		m_ManaTxt.SetIntText(nManaTxtNum);
		m_MoreMoney.SetIntText(nMoneyNum);	
		m_DurabilityTxt.SetIntText(nDurabilityNum);	
		m_BuffNMTxt.SetIntText(nBuffTxtNum);
		m_LifeTxt2.SetIntText(nTownNum);
		m_ManaTxt2.SetIntText(nTown1Num);
		ProcessPopUpChooseTeam(0);
		m_Aura1Edit.SetText(MSG_NON_SETTINGS);
		m_Aura2Edit.SetText(MSG_NON_SETTINGS);		
		LoadRecoverSetting();
	}	
}

int KUiRecover::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_BuffBtn)
			OnBuff();		
		else if (uParam == (unsigned int)(KWndWindow*)&m_DurabilityBtn)
			OnDurability();
		else if (uParam == (unsigned int)(KWndWindow*)&m_LifeBtn)
			OnCheckLife();
		else if (uParam == (unsigned int)(KWndWindow*)&m_ManaBtn)
			OnCheckMana();
		else if (uParam == (unsigned int)(KWndWindow*)&m_LifeBtn2)
			OnCheckLife2();
		else if (uParam == (unsigned int)(KWndWindow*)&m_ManaBtn2)
			OnCheckMana2();
		else if (uParam == (unsigned int)(KWndWindow*)&m_Aura1Btn)
			AuraSkillPopup(enumMagic_ID1);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Aura2Btn)
			AuraSkillPopup(enumMagic_ID2);
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChuyenVongSangBtn)
			OnActiveChangeAura();			
		else if (uParam == (unsigned int)(KWndWindow*)&m_GDBtn)
			OnUseGD();
		else if (uParam == (unsigned int)(KWndWindow*)&m_TTLBtn)
			OnUseTTL();
		else if (uParam == (unsigned int)(KWndWindow*)&m_MoreMoneyBtn)
			OnMoreMoney();
		else if (uParam == (unsigned int)(KWndWindow*)&m_NoLifeBtn)
			OnNoLife();
		else if (uParam == (unsigned int)(KWndWindow*)&m_NoManaBtn)
			OnNoMana();
		else if (uParam == (unsigned int)(KWndWindow*)&m_TeamBtn)
			OnChooseTeam();
		else if (uParam == (unsigned int)(KWndWindow*)&m_TeamListBtn)
			PopUpChooseTeam();
       
		else if (uParam == (unsigned int)(KWndWindow*)&m_BuyItemBtn)
		{
			if(IsBuyItem == FALSE)
			{
				IsBuyItem = TRUE;
				m_BuyItemBtn.CheckButton(true);
			}
			else
			{
				IsBuyItem = FALSE;
				m_BuyItemBtn.CheckButton(false);
			}	
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, eBuyItem, IsBuyItem);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_InventoryMIBtn)
		{
			if(IsInventoryMI == FALSE)
			{
				IsInventoryMI = TRUE;
				m_InventoryMIBtn.CheckButton(true);
			}
			else
			{
				IsInventoryMI = FALSE;
				m_InventoryMIBtn.CheckButton(false);
			}	
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, eInventoryIM, IsInventoryMI);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_RepairBtn)
		{
			if(IsRepairItem == FALSE)
			{
				IsRepairItem = TRUE;
				m_RepairBtn.CheckButton(true);
			}
			else
			{
				IsRepairItem = FALSE;
				m_RepairBtn.CheckButton(false);
			}	
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, eRepairEquip, IsRepairItem);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ReturnPortalBtn)
		{
			if(IsReturnPortal == FALSE)
			{
				IsReturnPortal = TRUE;
				m_ReturnPortalBtn.CheckButton(true);
			}
			else
			{
				IsReturnPortal = FALSE;
				m_ReturnPortalBtn.CheckButton(false);
			}	
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, eReturnPortal, IsReturnPortal);
		}
	
	
		SaveRecoverSetting();
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	case WND_M_MENUITEM_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			if (HIWORD(nParam) == MENU_SELECT_CHOOSE_TEAM && (short)(LOWORD(nParam)) >= 0)
				ProcessPopUpChooseTeam(LOWORD(nParam));
			else if (HIWORD(nParam) == MENU_SELECT_AURA_SKILL1 && (short)(LOWORD(nParam)) >= 0)
				SetAuraSkill(enumMagic_ID1, LOWORD(nParam));
			else if (HIWORD(nParam) == MENU_SELECT_AURA_SKILL2 && (short)(LOWORD(nParam)) >= 0)
				SetAuraSkill(enumMagic_ID2, LOWORD(nParam));				
		}
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiRecover::OnCheckInput()
{
	int nLife = m_Life.GetIntNumber();
	int nMana = m_Mana.GetIntNumber();
	int nLifeTxt = m_LifeTxt.GetIntNumber();
	int nManaTxt = m_ManaTxt.GetIntNumber();
	int nDurabilityTxt = m_DurabilityTxt.GetIntNumber();
	int nMoreMoney = m_MoreMoney.GetIntNumber();
	int nLife2 = m_LifeTxt2.GetIntNumber();
	int nMana2 = m_ManaTxt2.GetIntNumber();
	
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 23, nLife);
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 24, nLifeTxt);
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 25, nMana);
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 26, nManaTxt);
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,  9, nDurabilityTxt);
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 34, nMoreMoney);
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 29, nLife2);
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 30, nMana2);
}

void KUiRecover::OnBuff()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_BuffBtn.CheckButton(true);
		int nBuff = m_BuffNMTxt.GetIntNumber();
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,15,nBuff);
	}
	else
	{
		IsCheck = FALSE;
		m_BuffBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,15,0);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,14,IsCheck);	
}

void KUiRecover::OnDurability()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_DurabilityBtn.CheckButton(true);
		int nRepair = m_DurabilityTxt.GetIntNumber();
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,9,nRepair);
	}
	else
	{
		IsCheck = FALSE;
		m_DurabilityBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,9,0);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,8,IsCheck);	
}

void KUiRecover::OnCheckLife()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_LifeBtn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_LifeBtn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,21,IsCheck);	
}

void KUiRecover::OnCheckMana()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_ManaBtn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_ManaBtn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,22,IsCheck);	
}

void KUiRecover::OnCheckLife2()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_LifeBtn2.CheckButton(true);
		int nLifeTxt2 = m_LifeTxt2.GetIntNumber();
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 29, nLifeTxt2);
	}
	else
	{
		IsCheck = FALSE;
		m_LifeBtn2.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 29, 0);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,27,IsCheck);	
}

void KUiRecover::OnCheckMana2()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_ManaBtn2.CheckButton(true);
		int nManaTxt2 = m_ManaTxt2.GetIntNumber();
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 30, nManaTxt2);
	}
	else
	{
		IsCheck = FALSE;
		m_ManaBtn2.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, 30, 0);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,28,IsCheck);	
}


void KUiRecover::OnUseTTL()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_TTLBtn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_TTLBtn.CheckButton(false);
	}	
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,36,IsCheck);	
}

void KUiRecover::OnMoreMoney()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_MoreMoneyBtn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_MoreMoneyBtn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,33,IsCheck);	
}

void KUiRecover::OnNoLife()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_NoLifeBtn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_NoLifeBtn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,31,IsCheck);	
}

void KUiRecover::OnNoMana()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_NoManaBtn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_NoManaBtn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,32,IsCheck);	
}

void KUiRecover::OnUseGD()
{
	if(IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_GDBtn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_GDBtn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,35,IsCheck);	
}

void KUiRecover::PopUpChooseTeam()
{
	int nActionDataCount = sizeof(Array_PTTeam) / sizeof(Array_PTTeam[0]);
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		strncpy(pSelUnitMenu->Items[i].szData, Array_PTTeam[i], sizeof(Array_PTTeam[i]));
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int Left, Top, nWidth;
	GetSize(&nWidth, NULL);
	GetAbsolutePos(&Left, &Top);
	int nX = 0, nY = 0;
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO))
		Ini.GetInteger2("PopupPosition","PopUpPT",&nX, &nY);
	
	pSelUnitMenu->nX = Left + nX;
	pSelUnitMenu->nY = Top + nY;
	KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_CHOOSE_TEAM);
}

void KUiRecover::ProcessPopUpChooseTeam(int nAction)
{
	m_nAction = nAction;
	if (nAction == 0)
	{
		m_NameChooseTeam.SetText(Array_PTTeam[0]);
		g_pCoreShell->TeamOperation(TEAM_OI_REFUSE_INVITE, 0, TRUE);   /// tu choi pt
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,39,FALSE);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,38,FALSE);			
	}
	else if (nAction == 1)
	{
		m_NameChooseTeam.SetText(Array_PTTeam[1]);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,39,TRUE);  // che do vao pt 
		g_pCoreShell->TeamOperation(TEAM_OI_REFUSE_INVITE, 0, FALSE);  
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,38,FALSE);				
	}
	else
	{
		m_NameChooseTeam.SetText(Array_PTTeam[2]);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,38,TRUE);  // che do tu pt
		g_pCoreShell->TeamOperation(TEAM_OI_REFUSE_INVITE, 0, FALSE);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,39,FALSE);	
	}
}

void KUiRecover::OnActiveChangeAura()
{
	if (IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_ChuyenVongSangBtn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_ChuyenVongSangBtn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,12,IsCheck);	
}

void KUiRecover::AuraSkillPopup(int nIndex)
{
	int nActionDataCount = g_pCoreShell->FindSkillInfo(2,0) + 1;
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		if (i == 0)
		{
			strncpy(pSelUnitMenu->Items[i].szData, MSG_NON_SETTINGS, sizeof(MSG_NON_SETTINGS));
			pSelUnitMenu->Items[i].uID = 0;
		}
		else
		{
			int _nID = g_pCoreShell->FindSkillInfo(2,i);
			g_pCoreShell->GetSkillName(_nID, pSelUnitMenu->Items[i].szData);
			pSelUnitMenu->Items[i].uID = _nID;
		}
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int nX = 0, nY = 0;
	int Left, Top, nWidth;
	GetSize(&nWidth, NULL);
	GetAbsolutePos(&Left, &Top);
	KIniFile	Ini;
	switch (nIndex)
	{
	case enumMagic_ID1:
		if (Ini.Load(SCHEME_INI_AUTO))
			Ini.GetInteger2("PopupPosition","AuraPosition1",&nX, &nY);
		pSelUnitMenu->nX = Left + nX;
		pSelUnitMenu->nY = Top + nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_AURA_SKILL1);
		break;
	case enumMagic_ID2:
		if (Ini.Load(SCHEME_INI_AUTO))
			Ini.GetInteger2("PopupPosition","AuraPosition2",&nX, &nY);
		pSelUnitMenu->nX = Left + nX;
		pSelUnitMenu->nY = Top + nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_AURA_SKILL2);
		break;
	}
}

void KUiRecover::SetAuraSkill(int nIndex, int nID)
{
	switch (nIndex)
	{
	case enumMagic_ID1:
		if (nID > 0)
		{
			m_CurrentSelMagic[enumSel_Magic_Aura1] = nID;
			char Name[128];
			g_pCoreShell->GetSkillName(nID, Name);
			m_Aura1Edit.SetText(Name);
		}
		else
		{
			m_Aura1Edit.SetText(MSG_NON_SETTINGS);
		}	
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,10,nID);
		break;
	case enumMagic_ID2:
		if (nID > 0)
		{
			m_CurrentSelMagic[enumSel_Magic_Aura2] = nID;
			char Name[128];
			g_pCoreShell->GetSkillName(nID, Name);
			m_Aura2Edit.SetText(Name);
		}
		else
		{
			m_Aura2Edit.SetText(MSG_NON_SETTINGS);
		}	
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,11,nID);
		break;
	}
}

void KUiRecover::OnChooseTeam()
{
	if (IsCheck == FALSE)
	{
		IsCheck = TRUE;
		m_TeamBtn.CheckButton(true);
	}
	else
	{
		IsCheck = FALSE;
		m_TeamBtn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,37,IsCheck);

}


void KUiRecover::LoadRecoverSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	int 	m_nDurabilityTxt, m_nBuffNMTxt,m_nEditLife,m_nEditMana,m_nEditLife2,m_nEditMana2,m_nMoreMoney;
    BOOL	m_bBuyBtn, m_bInventoryBtn;
	BOOL	m_bRepairBtn, m_bReturnPT;
	BOOL	m_BuffSkill,m_Durability,m_NoLife,m_NoMana;
	BOOL	m_nLifeBtn,m_nManaBtn,m_nLifeBtn2,m_nManaBtn2,m_nGDBtn,m_nTTLBtn,m_nTeamBtn, m_nMoreMoneyBtn, m_bChuyenVongSangBtn;
	if (pConfigFile)
	{
		pConfigFile->GetInteger("Recover", "DurabilityTxt", 0, (int*)(&m_nDurabilityTxt));
		m_DurabilityTxt.SetIntText(m_nDurabilityTxt);
		pConfigFile->GetInteger("Recover", "BuffNMTxt", 0, (int*)(&m_nBuffNMTxt));
		m_BuffNMTxt.SetIntText(m_nBuffNMTxt);
		pConfigFile->GetInteger("Recover", "EditLife", 0, (int*)(&m_nEditLife));
		m_Life.SetIntText(m_nEditLife);
		pConfigFile->GetInteger("Recover", "EditMana", 0, (int*)(&m_nEditMana));
		m_Mana.SetIntText(m_nEditMana);
		pConfigFile->GetInteger("Recover", "EditLife2", 0, (int*)(&m_nEditLife2));
		m_LifeTxt2.SetIntText(m_nEditLife2);
		pConfigFile->GetInteger("Recover", "EditMana2", 0, (int*)(&m_nEditMana2));
		m_ManaTxt2.SetIntText(m_nEditMana2);
		pConfigFile->GetInteger("Recover", "MoreMoney", 0, (int*)(&m_nMoreMoney));
		m_MoreMoney.SetIntText(m_nMoreMoney);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,34,m_nMoreMoney);
		
       
	

		pConfigFile->GetInteger("Pick", "BuyBtn", 0, (int*)(&m_bBuyBtn));
		if (m_bBuyBtn)
			m_BuyItemBtn.CheckButton(true);
		else
			m_BuyItemBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,eBuyItem,m_bBuyBtn);

		pConfigFile->GetInteger("Pick", "InventoryBtn", 0, (int*)(&m_bInventoryBtn));
		if (m_bInventoryBtn)
			m_InventoryMIBtn.CheckButton(true);
		else
			m_InventoryMIBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,eInventoryIM,m_bInventoryBtn);

		pConfigFile->GetInteger("Pick", "InventoryBtn2", 0, (int*)(&m_bRepairBtn));
		if (m_bRepairBtn)
			m_RepairBtn.CheckButton(true);
		else
			m_RepairBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,eRepairEquip,m_bRepairBtn);

		pConfigFile->GetInteger("Pick", "ReturnPtBtn", 0, (int*)(&m_bReturnPT));
		if (m_bReturnPT)
			m_ReturnPortalBtn.CheckButton(true);
		else
			m_ReturnPortalBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,eReturnPortal,m_bReturnPT);
        	
		pConfigFile->GetInteger("Recover", "ChuyenVongSangBtn", 0, (int*)(&m_bChuyenVongSangBtn));		
        if (m_bChuyenVongSangBtn)
			m_ChuyenVongSangBtn.CheckButton(true);
		else
			m_ChuyenVongSangBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,12,m_bChuyenVongSangBtn);		
		
		char szKeyName[10];
		int nValue;
		for (int i = 0; i < enumSel_Magic_Num; i ++)
		{
			sprintf(szKeyName, "SelMagic%d", i);
			pConfigFile->GetInteger("Recover", szKeyName, 0, &m_CurrentSelMagic[i]);
			if (i >= enumSel_Magic_Aura1 && i <= enumSel_Magic_Aura2)			
			{
				if (i == enumSel_Magic_Aura1) nValue = 0;
				SetAuraSkill(nValue, m_CurrentSelMagic[i]);
				nValue++;
			}
		}			
			
		pConfigFile->GetInteger("Recover", "GDBtn", 0, (int*)(&m_nGDBtn));
		if (m_nGDBtn)
			m_GDBtn.CheckButton(true);
		else
			m_GDBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,35,m_nGDBtn);		
		
		pConfigFile->GetInteger("Recover", "TTLBtn", 0, (int*)(&m_nTTLBtn));
		if (m_nTTLBtn)
			m_TTLBtn.CheckButton(true);
		else
			m_TTLBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,36,m_nTTLBtn);		
		
		pConfigFile->GetInteger("Recover", "BuffSkill", 0, (int*)(&m_BuffSkill));
		if (m_BuffSkill)
			m_BuffBtn.CheckButton(true);
		else
			m_BuffBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,15,m_BuffSkill);		
		
		pConfigFile->GetInteger("Recover", "DurabilityBtn", 0, (int*)(&m_Durability));
		if (m_Durability)
		{
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,9,m_nDurabilityTxt);
			m_DurabilityBtn.CheckButton(true);
		}
		else
		{
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,9,0);
			m_DurabilityBtn.CheckButton(false);
		}
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,8,m_Durability);		
		
		pConfigFile->GetInteger("Recover", "NoLife", 0, (int*)(&m_NoLife));
		if (m_NoLife)
			m_NoLifeBtn.CheckButton(true);
		else
			m_NoLifeBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,31,m_NoLife);		
		
		pConfigFile->GetInteger("Recover", "NoMana", 0, (int*)(&m_NoMana));
		if (m_NoMana)
			m_NoManaBtn.CheckButton(true);
		else
			m_NoManaBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,32,m_NoMana);		
		
		pConfigFile->GetInteger("Recover", "LifeBtn", 0, (int*)(&m_nLifeBtn));
		if (m_nLifeBtn)
			m_LifeBtn.CheckButton(true);
		else
			m_LifeBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,21,m_nLifeBtn);		
		
		pConfigFile->GetInteger("Recover", "ManaBtn", 0, (int*)(&m_nManaBtn));
		if (m_nManaBtn)
			m_ManaBtn.CheckButton(true);
		else
			m_ManaBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,22,m_nManaBtn);		
		
		pConfigFile->GetInteger("Recover", "LifeBtn2", 0, (int*)(&m_nLifeBtn2));
		if (m_nLifeBtn2)
		{
			m_LifeBtn2.CheckButton(true);
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,29,m_nEditLife2);
		}
		else
		{
			m_LifeBtn2.CheckButton(false);
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,29,0);
		}
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,27,m_nLifeBtn2);		
		
		pConfigFile->GetInteger("Recover", "ManaBtn2", 0, (int*)(&m_nManaBtn2));
		if (m_nManaBtn2)
		{
			m_ManaBtn2.CheckButton(true);
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,30,m_nEditMana2);
		}
		else
		{
			m_ManaBtn2.CheckButton(false);
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,30,0);
		}
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,28,m_nManaBtn2);		
		
		pConfigFile->GetInteger("Recover", "MoreMoneyBtn", 0, (int*)(&m_nMoreMoneyBtn));
		if (m_nMoreMoneyBtn)
			m_MoreMoneyBtn.CheckButton(true);
		else
			m_MoreMoneyBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,33,m_nMoreMoneyBtn);		
		
	  		pConfigFile->GetInteger("Recover", "TeamBtn", 0, (int*)(&m_nTeamBtn));
		if (m_nTeamBtn)
			m_TeamBtn.CheckButton(true);
		else
			m_TeamBtn.CheckButton(false);
		   g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,37,m_nTeamBtn);	
		
		pConfigFile->GetInteger("Recover", "TeamText", 0, &m_nAction);
		ProcessPopUpChooseTeam(m_nAction);
	
	}
	g_UiBase.CloseAutoSettingFile(true);
}

void KUiRecover::SaveRecoverSetting() // phuc hoi 
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	if (pConfigFile)
	{
		if (m_ChuyenVongSangBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "ChuyenVongSangBtn", 1);
		else
			pConfigFile->WriteInteger("Recover", "ChuyenVongSangBtn", 0);	
			
		char szKeyName[10];
		for (int i = 0; i < enumSel_Magic_Num; i ++)
		{
			sprintf(szKeyName, "SelMagic%d", i);
			pConfigFile->WriteInteger("Recover", szKeyName, m_CurrentSelMagic[i]);		
		}			

		if (m_BuffBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "BuffSkill", 1);
		else
			pConfigFile->WriteInteger("Recover", "BuffSkill", 0);
			
		if (m_DurabilityBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "DurabilityBtn", 1);
		else
			pConfigFile->WriteInteger("Recover", "DurabilityBtn", 0);
			
		if (m_LifeBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "LifeBtn", 1);
		else
			pConfigFile->WriteInteger("Recover", "LifeBtn", 0);
			
		if (m_ManaBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "ManaBtn", 1);
		else
			pConfigFile->WriteInteger("Recover", "ManaBtn", 0);
			
		if (m_LifeBtn2.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "LifeBtn2", 1);
		else
			pConfigFile->WriteInteger("Recover", "LifeBtn2", 0);
			
		if (m_ManaBtn2.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "ManaBtn2", 1);
		else
			pConfigFile->WriteInteger("Recover", "ManaBtn2", 0);
			
		if (m_NoLifeBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "NoLife", 1);
		else
			pConfigFile->WriteInteger("Recover", "NoLife", 0);
			
		if (m_NoManaBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "NoMana", 1);
		else
			pConfigFile->WriteInteger("Recover", "NoMana", 0);
			
		if (m_TTLBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "TTLBtn", 1);
		else
			pConfigFile->WriteInteger("Recover", "TTLBtn", 0);
			
		if (m_GDBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "GDBtn", 1);
		else
			pConfigFile->WriteInteger("Recover", "GDBtn", 0);
			
		if (m_MoreMoneyBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "MoreMoneyBtn", 1);
		else
			pConfigFile->WriteInteger("Recover", "MoreMoneyBtn", 0);
			
		if (m_TeamBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Recover", "TeamBtn", 1);
		else
			pConfigFile->WriteInteger("Recover", "TeamBtn", 0);

		if (m_BuyItemBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Pick", "BuyBtn", 1);
		else
			pConfigFile->WriteInteger("Pick", "BuyBtn", 0);

		if (m_InventoryMIBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Pick", "InventoryBtn", 1);
		else
			pConfigFile->WriteInteger("Pick", "InventoryBtn", 0);

		if (m_RepairBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Pick", "InventoryBtn2", 1);
		else
			pConfigFile->WriteInteger("Pick", "InventoryBtn2", 0);

		if (m_ReturnPortalBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Pick", "ReturnPtBtn", 1);
		else
			pConfigFile->WriteInteger("Pick", "ReturnPtBtn", 0);
		
	

		
			
		pConfigFile->WriteInteger("Recover", "DurabilityTxt", m_DurabilityTxt.GetIntNumber());
		pConfigFile->WriteInteger("Recover", "BuffNMTxt", m_BuffNMTxt.GetIntNumber());
		pConfigFile->WriteInteger("Recover", "EditLife", m_Life.GetIntNumber());
		pConfigFile->WriteInteger("Recover", "EditMana", m_Mana.GetIntNumber());
		pConfigFile->WriteInteger("Recover", "EditLife2", m_LifeTxt2.GetIntNumber());
		pConfigFile->WriteInteger("Recover", "EditMana2", m_ManaTxt2.GetIntNumber());
		pConfigFile->WriteInteger("Recover", "MoreMoney", m_MoreMoney.GetIntNumber());
		pConfigFile->WriteInteger("Recover", "TeamText", m_nAction);
	}
	g_UiBase.CloseAutoSettingFile(true);
}
/*******************************************************************
	Nhat do
*******************************************************************/

char Data_Pick[][64] = 
{
	"NhÆt TiÒn",
	"NhÆt VËt PhÈm",	
	"NhÆt §å Mµu",	
	"NhÆt TÊt C¶",
};

char szArray_SelRoom[][64] = 
{
	"R­¬ng cßn 1 « tù phï",
	"R­¬ng cßn 2 « tù phï",
	"R­¬ng cßn 4 « tù phï",
	"R­¬ng cßn 6 « tù phï",
};

char Data_Magic[defMAX_AUTO_FILTERL][2][64] = 
{
	"Sinh lùc(®)", 		"lifemax_v",
	"Néi lùc(®)", 		"manamax_v",
	"ThÓ lùc(®)", 		"staminamax_v",
	"Phôc håi sinh lùc(®)", 	"lifereplenish_v",
	"Phôc håi néi lùc(®)", 	"manareplenish_v",
	"Phôc håi thÓ lùc(®)", 	"staminareplenish_v",
	"Hót sinh lùc(%)", 	"steallifeenhance_p",
	"Hót néi lùc(%)", 	"stealmanaenhance_p",
	"Hót thÓ lùc(%)", 	"stealstaminaenhance_p",
	"Søc m¹nh(®)", 		"strength_v",
	"Th©n ph¸p(®)", 	"dexterity_v",
	"Sinh khÝ(®)", 		"vitality_v",
	"Néi c«ng(®)", 		"energy_v",
	"Phßng thñ(%)", 	"physicsres_p",
	"Kh¸ng ®éc(%)", 	"poisonres_p",
	"Kh¸ng b¨ng(%)", 	"coldres_p",
	"Kh¸ng háa(%)", 	"fireres_p",
	"Kh¸ng l«i(%)", 	"lightingres_p",
	"Kh¸ng tÊt c¶(%)", 	"allres_p",
	"Tèc ®éc ch¹y(%)", 	"fastwalkrun_p",
	"Tèc ®é ®¸nh(%)", 	"attackspeed_v",
	"Ph¶n ®ßn cËn(®)", 	"meleedamagereturn_v",
	"STVL ngo¹i(®)", 	"addphysicsdamage_v",
	"B¨ng s¸t ngo¹i(®)", "addcolddamage_v",
	"§éc s¸t ngo¹i(®)", "addpoisondamage_v",
	"STVL ngo¹i(%)", 	"addphysicsdamage_p",
	"CHSTTNL(%)", 		"damagetomana_p",
	"May m¾n(%)", 		"lucky_v",
	"Bá qua nÐ tr¸nh(%)", "ignoredefense_p",
	"§é chÝnh x¸c(®)", 	"attackrating_v",
	"STLV néi(®)", 		"metalskill_v",
	"B¨ng s¸t néi(®)", 	"waterskill_v",
	"Háa s¸t néi(®)", 	"fireskill_v",
	"L«i s¸t néi(®)", 	"earthskill_v",
	"§éc s¸t néi(®)", 	"woodskill_v",
	"Gi¶m chËm(%)", 	"freezetimereduce_p",
	"Gi¶m tróng ®éc(%)", "poisontimereduce_p",
	"Gi¶m cho¸ng(%)", 	"stuntimereduce_p",
	"Gi¶m th­¬ng(%)", 	"fasthitrecover_v",
	"Kü n¨ng vèn cã(®)", "allskill_v",
};

void KUiPick::Initialize()
{
	AddChild(&m_FilterDelBtn);
	AddChild(&m_FilterDelAll);
	AddChild(&m_FilterAddBtn);
	AddChild(&m_FilterL);
	AddChild(&m_FilterL_Scroll);
	m_FilterL.SetScrollbar(&m_FilterL_Scroll);
	AddChild(&m_PutBtn);	
	AddChild(&m_PutListBtn);
	
	AddChild(&m_PickBtn);		
	AddChild(&m_PickListBtn);	

	AddChild(&m_PickTypeTxt);
    AddChild(&m_BanItemBtn);
    AddChild(&m_SortITemBtn);
    AddChild(&m_OpenTuiDPBtn);

	AddChild(&m_ReturnBtn);
	AddChild(&m_FillterEdit);
	AddChild(&m_FilterEdit);

	AddChild(&m_TPNotEquipmentChecker);   
	AddChild(&m_TPNotEquipmentShadow);
	AddChild(&m_TPNotEquipmentText);
	AddChild(&m_TPNotEquipmentPopup);
	
	IsPickItem = FALSE;
    IsSortItem = FALSE;
    IsBanItem = FALSE;	
    IPNotEquipmentChecker = FALSE;
   	IsOpenTuiDP = FALSE;
	m_CurrentPickKind = 0;
	m_CurrentSelMagic = 0;
	m_nSelectIndex = 0;
    m_nSelRoom = 0;
	memset(m_FilterMagicIndex, 0, sizeof(m_FilterMagicIndex));
	memset(m_cFilterMagic, 0, sizeof(m_cFilterMagic));
	memset(m_FilterMagicV, 0, sizeof(m_FilterMagicV));
}
void KUiPick::LoadScheme()
{
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO))
	{
		KWndPage::Init(&Ini, "MainPick");

		m_FilterDelBtn.Init(&Ini, "FilterDelBtn");
		m_FilterDelAll.Init(&Ini, "FilterDelAll");
		m_FilterAddBtn.Init(&Ini, "FilterAddBtn");
		m_FilterL.Init(&Ini, "FilterL");
		m_FilterL_Scroll.Init(&Ini, "FilterL_Scroll");
		m_FillterEdit.Init(&Ini, "FillterEdit");
		m_PickBtn.Init(&Ini, "PickItemBtn");
		m_PickListBtn	.Init(&Ini, "PickList");

		m_PutBtn.Init(&Ini, "PutBtn");
		m_PutListBtn	.Init(&Ini, "PutList");	

		m_ReturnBtn.Init(&Ini, "ReturnItem");
		m_PickTypeTxt.Init(&Ini, "PickTypeTxt");
		m_FilterEdit.Init(&Ini, "FilterTxt");
        m_BanItemBtn.Init(&Ini, "BanItemBtn");
        m_SortITemBtn.Init(&Ini, "SortItemBtn");
        m_OpenTuiDPBtn.Init(&Ini, "OpenTuiDPBtn");

        m_TPNotEquipmentChecker.Init(&Ini, "TPNotEquipmentChecker");
	    m_TPNotEquipmentText.Init(&Ini, "TPNotEquipmentText");
	    m_TPNotEquipmentShadow.Init(&Ini, "TPNotEquipmentShadow");
	    m_TPNotEquipmentPopup.Init(&Ini, "TPNotEquipmentPopup");

		LoadPickSetting();
	}	
}

int KUiPick::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_PickBtn)
			OnPick();
		else if (uParam == (unsigned int)(KWndWindow*)&m_BanItemBtn)
		{
			if(IsBanItem == FALSE)
			{
				IsBanItem = TRUE;
				m_BanItemBtn.CheckButton(true);
			}
			else
			{
				IsBanItem = FALSE;
				m_BanItemBtn.CheckButton(false);
			}	
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, eABanItem, IsBanItem);
		}
         else if (uParam == (unsigned int)(KWndWindow*)&m_SortITemBtn)
		{
			if(IsSortItem == FALSE)
			{
				IsSortItem = TRUE;
				m_SortITemBtn.CheckButton(true);
			}
			else
			{
				IsSortItem = FALSE;
				m_SortITemBtn.CheckButton(false);
			}	
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, eSortItem, IsSortItem);
		}
        else if (uParam == (unsigned int)(KWndWindow*)&m_TPNotEquipmentChecker)
		{
			if (m_TPNotEquipmentChecker.IsButtonChecked())
			{
				m_TPNotEquipmentChecker.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_TP_NOT_EQUIPMENT, TRUE, NULL, NULL);
			}
			else
			{
				m_TPNotEquipmentChecker.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_TP_NOT_EQUIPMENT, FALSE, NULL, NULL);				
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_TPNotEquipmentPopup)
			    PopupSelRoomNE();

        else if (uParam == (unsigned int)(KWndWindow*)&m_OpenTuiDPBtn)
		{
			if(IsOpenTuiDP == FALSE)
			{
				IsOpenTuiDP = TRUE;
				m_OpenTuiDPBtn.CheckButton(true);
			}
			else
			{
				IsOpenTuiDP = FALSE;
				m_OpenTuiDPBtn.CheckButton(false);
			}	
			g_pCoreShell->OperationRequest(GOI_AUTO_PLAY, eAutoTuiDuocPham, IsOpenTuiDP);
		}
		
		else if (uParam == (unsigned int)(KWndWindow*)&m_ReturnBtn)
			OnGiveItem();
		else if (uParam == (unsigned int)(KWndWindow*)&m_PutBtn)
			OnFillterItem();
		else if (uParam == (unsigned int)(KWndWindow*)&m_PickListBtn)
			OnSelectPickType(MENU_SELECT_PICK_TYPE);
		else if (uParam == (unsigned int)(KWndWindow*)&m_PutListBtn)
			OnSelectPickType(MENU_SELECT_SORT_MAGIC);
		else if (uParam == (unsigned int)(KWndWindow*)&m_FilterAddBtn)
		{
			int i = FilterSameMagic(Data_Magic[m_CurrentSelMagic][1]);

			if (i >= 0 && i < defMAX_AUTO_FILTERL)
			{
				strcpy(m_cFilterMagic[i], Data_Magic[m_CurrentSelMagic][1]);
				m_FilterMagicV[i] = m_FillterEdit.GetIntNumber();
			}
			else
			{
				for (i = 0; i < defMAX_AUTO_FILTERL; i++)
				{
					if (!m_cFilterMagic[i][0])
					{
						strcpy(m_cFilterMagic[i], Data_Magic[m_CurrentSelMagic][1]);
						m_FilterMagicV[i] = m_FillterEdit.GetIntNumber();
						break;
					}
				}
			}
			SetFilterMagicList();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_FilterDelBtn)
		{
			int nSel = m_FilterL.GetCurSel();
			memset(m_cFilterMagic[m_FilterMagicIndex[nSel]], 0, sizeof(m_cFilterMagic[nSel]));
			m_FilterMagicV[m_FilterMagicIndex[nSel]] = 0;
			SetFilterMagicList();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_FilterDelAll)
		{
			memset(m_cFilterMagic, 0, sizeof(m_cFilterMagic));
			memset(m_FilterMagicV, 0, sizeof(m_FilterMagicV));
			SetFilterMagicList();
		}
		SavePickSetting();
		break;
	case WND_M_MENUITEM_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
            if (HIWORD(nParam) == MENU_ROOM && (short)(LOWORD(nParam)) >= 0)
				SetPortalRoomNE(LOWORD(nParam));
			else if ((short)(LOWORD(nParam)) >= 0)
				SetPick(HIWORD(nParam), (short)(LOWORD(nParam)));
		}
        break;
	case WND_N_LIST_ITEM_SEL:
		if(uParam == (unsigned int)&m_FilterL)
		{
			m_nSelectIndex = nParam;
		}
		break;
	case WND_N_SCORLLBAR_POS_CHANGED:
		if(uParam == (unsigned int)&m_FilterL_Scroll)
		{
    		m_FilterL.SetTopItemIndex(nParam);
		}
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiPick::PopupSelRoomNE()
{
	int nActionDataCount = sizeof(szArray_SelRoom) / sizeof(szArray_SelRoom[0]);
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
			if ((i == enumRoomNotEnough1) || 
			(i == enumRoomNotEnough2) || 
			(i == enumRoomNotEnough4) || 
			(i == enumRoomNotEnough6)
			)
		{
			strncpy(pSelUnitMenu->Items[i].szData, szArray_SelRoom[i], sizeof(szArray_SelRoom[i]));
			pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
			pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
			pSelUnitMenu->nNumItem++;
		}
	}
   
	int x, y;
	m_TPNotEquipmentShadow.GetAbsolutePos(&x, &y);
	pSelUnitMenu->nX = x;
	pSelUnitMenu->nY = y;
	KPopupMenu::Popup(pSelUnitMenu, this, MENU_ROOM);
}

void KUiPick::SetPortalRoomNE(int nSel)
{
	m_nSelRoom = nSel;
	m_TPNotEquipmentText.SetText(szArray_SelRoom[m_nSelRoom]);
	g_pCoreShell->PAIOperation(GPI_TP_NOT_EQUIPMENTV, m_nSelRoom, NULL, NULL);
}

void KUiPick::OnPick()
{	
	if(IsPickItem == FALSE)
	{
		IsPickItem = TRUE;
		m_PickBtn.CheckButton(true);
	}
	else
	{
		IsPickItem = FALSE;
		m_PickBtn.CheckButton(false);
	}	
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,17,IsPickItem);	
}

void KUiPick::OnFillterItem()
{	
	if(IsPickItem == FALSE)
	{
		IsPickItem = TRUE;
		m_PutBtn.CheckButton(true);
	}
	else
	{
		IsPickItem = FALSE;
		m_PutBtn.CheckButton(false);
	}	
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,19,IsPickItem);	
}

void KUiPick::OnGiveItem() 
{	
	if(IsPickItem == FALSE)
	{
		IsPickItem = TRUE;
		m_ReturnBtn.CheckButton(true);
	}
	else
	{
		IsPickItem = FALSE;
		m_ReturnBtn.CheckButton(false);
	}
	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,20,IsPickItem);	
}

void KUiPick::SetFilterMagicList()
{
	memset(m_FilterMagicIndex, 0, sizeof(m_FilterMagicIndex));
	m_FilterL.ResetContent();

	char Buff[64];
	int Count = 0;
	for (int i = 0; i < defMAX_AUTO_FILTERL; i++)
	{
		g_pCoreShell->SetSortItem(NULL, 0, i);
		if (m_cFilterMagic[i][0])
		{
			g_pCoreShell->SetSortItem((unsigned int)m_cFilterMagic[i], m_FilterMagicV[i], Count);

			sprintf(Buff, MSG_FORMAT_MAGIC, Data_Magic[FindSameMagic(m_cFilterMagic[i])][0], m_FilterMagicV[i]);
			m_FilterL.AddString(Count, Buff);
			m_FilterMagicIndex[Count] = i;
			Count++;
		}
	}
}

int KUiPick::FilterSameMagic(const char* szMagic)
{
	for (int i = 0; i < defMAX_AUTO_FILTERL; i ++)
	{
		if (strcmp(szMagic, m_cFilterMagic[i]) == 0)
			return i;
	}
	return -1;
}

int KUiPick::FindSameMagic(const char* szMagic)
{
	for (int i = 0; i < defMAX_AUTO_FILTERL; i ++)
	{
		if (strcmp(szMagic, Data_Magic[i][1]) == 0)
			return i;
	}
	return -1;
}
void KUiPick::SetPick(int nIndex, int nSel)
{
	switch (nIndex)
	{
		case MENU_SELECT_PICK_TYPE:
			if (nSel >= enumPickUpEarn && nSel < enumPickNum)
			{
				m_CurrentPickKind = nSel;
				m_PickTypeTxt.SetText(Data_Pick[m_CurrentPickKind]);
				g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,18,m_CurrentPickKind);
			}
			break;
		case MENU_SELECT_SORT_MAGIC:
			m_CurrentSelMagic = nSel;
			m_FilterEdit.SetText(Data_Magic[m_CurrentSelMagic][0]);
			break;
	}
	SavePickSetting();
}

void KUiPick::OnSelectPickType(int nIndex)
{
	int nActionDataCount = 0;
	if (nIndex == MENU_SELECT_PICK_TYPE)
		nActionDataCount = sizeof(Data_Pick) / sizeof(Data_Pick[0]);
	else if (nIndex = MENU_SELECT_SORT_MAGIC)
		nActionDataCount = sizeof(Data_Magic) / sizeof(Data_Magic[0]);
	else return;
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		if (nIndex == MENU_SELECT_PICK_TYPE)
			strncpy(pSelUnitMenu->Items[i].szData, Data_Pick[i], sizeof(Data_Pick[i]));
		else if (nIndex = MENU_SELECT_SORT_MAGIC)
			strncpy(pSelUnitMenu->Items[i].szData, Data_Magic[i][0], sizeof(Data_Magic[i][0]));
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int nX = 0, nY = 0;
	int Left, Top, nWidth;
	GetSize(&nWidth, NULL);
	GetAbsolutePos(&Left, &Top);
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO))
	{
		if (nIndex == MENU_SELECT_PICK_TYPE)
			Ini.GetInteger2("PopupPosition","PickItem",&nX, &nY);
		else if (nIndex = MENU_SELECT_SORT_MAGIC) 
			Ini.GetInteger2("PopupPosition","PickMagic",&nX, &nY);
	}	
	pSelUnitMenu->nX = Left + nX;
	pSelUnitMenu->nY = Top + nY;
	KPopupMenu::Popup(pSelUnitMenu, this, nIndex);
}

void KUiPick::LoadPickSetting()
{
    int nValue;
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	BOOL	m_bReturnBtn,m_bPickBtn,m_bPutBtn;
	BOOL	m_bBanItemBtn, m_bSortBtn;
	int 	m_nFillterEdit, m_bOTuiDPBtn;
	if (pConfigFile)
	{
		pConfigFile->GetInteger("Pick", "SelMagic", 0, &m_CurrentSelMagic);
		m_FilterEdit.SetText(Data_Magic[m_CurrentSelMagic][0]);
		pConfigFile->GetInteger("Pick", "FillterEdit", 0, (int*)(&m_nFillterEdit));
		m_FillterEdit.SetIntText(m_nFillterEdit);
		
		pConfigFile->GetInteger("Pick", "ReturnBtn", 0, (int*)(&m_bReturnBtn));
		if (m_bReturnBtn)
			m_ReturnBtn.CheckButton(true);
		else
			m_ReturnBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,20,m_bReturnBtn);		
		
		pConfigFile->GetInteger("Pick", "PickBtn", 0, (int*)(&m_bPickBtn));
		if (m_bPickBtn)
			m_PickBtn.CheckButton(true);
		else
			m_PickBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,17,m_bPickBtn);		

	    pConfigFile->GetInteger("Pick", "cBanItemBtn", 0, (int*)(&m_bBanItemBtn));
		if (m_bBanItemBtn)
			m_BanItemBtn.CheckButton(true);
		else
			m_BanItemBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,eABanItem,m_bBanItemBtn);

		pConfigFile->GetInteger("Pick", "TPRoomNECK", 0, &nValue);
		m_TPNotEquipmentChecker.CheckButton(nValue > 0);
		g_pCoreShell->PAIOperation(GPI_TP_NOT_EQUIPMENT, nValue > 0, NULL, NULL);
	//	m_TPNotEquipmentChecker.CheckButton(nValue > 0);
	//	g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,55,  nValue > 0); // them tinh nang full ruong
		
		pConfigFile->GetInteger("Pick", "TPRoomNEV", 0, &nValue);
		SetPortalRoomNE(nValue);	

		pConfigFile->GetInteger("Pick", "SortBtn", 0, (int*)(&m_bSortBtn));
		if (m_bSortBtn)
			m_SortITemBtn.CheckButton(true);
		else
			m_SortITemBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,eSortItem,m_bSortBtn);

        pConfigFile->GetInteger("Pick", "OTuiDPBtn", 0, (int*)(&m_bOTuiDPBtn));
		if (m_bOTuiDPBtn)
			m_OpenTuiDPBtn.CheckButton(true);
		else
			m_OpenTuiDPBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,eAutoTuiDuocPham,m_bOTuiDPBtn);



		
		pConfigFile->GetInteger("Pick", "PutBtn", 0, (int*)(&m_bPutBtn));
		if (m_bPutBtn)
			m_PutBtn.CheckButton(true);
		else
			m_PutBtn.CheckButton(false);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,19,m_bPutBtn);		
		
		pConfigFile->GetInteger("Pick", "PickKind", 0, &m_CurrentPickKind);
		m_PickTypeTxt.SetText(Data_Pick[m_CurrentPickKind]);
		g_pCoreShell->OperationRequest(GOI_AUTO_PLAY,18,m_CurrentPickKind);
		memset(m_cFilterMagic, 0, sizeof(m_cFilterMagic));
		memset(m_FilterMagicV, 0, sizeof(m_FilterMagicV));
		if (pConfigFile->IsSectionExist("FilterMagic"))
		{
			char szKeyName[10];
			for (int i = 0; i < defMAX_AUTO_FILTERL; i++)
			{
				sprintf(szKeyName, "Name%d", i);
				pConfigFile->GetString("FilterMagic", szKeyName, "", m_cFilterMagic[i], sizeof(m_cFilterMagic[i]));
				sprintf(szKeyName, "Value%d", i);
				pConfigFile->GetInteger("FilterMagic", szKeyName, 0, &m_FilterMagicV[i]);
			}
		}	
		SetFilterMagicList();
	}
	g_UiBase.CloseAutoSettingFile(true);
}

void KUiPick::SavePickSetting()  // nhat do 
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	if (pConfigFile)
	{
		pConfigFile->WriteInteger("Pick", "PickKind", m_CurrentPickKind);
		if (m_ReturnBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Pick", "ReturnBtn", 1);
		else
			pConfigFile->WriteInteger("Pick", "ReturnBtn", 0);
		if (m_PickBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Pick", "PickBtn", 1);
		else
			pConfigFile->WriteInteger("Pick", "PickBtn", 0);

		if (m_BanItemBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Pick", "cBanItemBtn", 1);
		else
			pConfigFile->WriteInteger("Pick", "cBanItemBtn", 0);

        if (m_SortITemBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Pick", "SortBtn", 1);
		else
			pConfigFile->WriteInteger("Pick", "SortBtn", 0);

	    if (m_OpenTuiDPBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Pick", "OTuiDPBtn", 1);
		else
			pConfigFile->WriteInteger("Pick", "OTuiDPBtn", 0);
          
          		
	
		
		if (m_PutBtn.IsButtonChecked())
			pConfigFile->WriteInteger("Pick", "PutBtn", 1);
		else
			pConfigFile->WriteInteger("Pick", "PutBtn", 0);
		pConfigFile->WriteInteger("Pick", "FillterEdit", m_FillterEdit.GetIntNumber());
        pConfigFile->WriteInteger("Pick", "TPRoomNECK", 				m_TPNotEquipmentChecker.IsButtonChecked());
        pConfigFile->WriteInteger("Pick", "TPRoomNEV", 					m_nSelRoom);

		char szKeyName[10];
		int Count = 0;
		for (int i = 0; i < defMAX_AUTO_FILTERL; i++)
		{
			sprintf(szKeyName, "Name%d", i);
			pConfigFile->WriteString("FilterMagic", szKeyName, m_cFilterMagic[i]);
			sprintf(szKeyName, "Value%d", i);
			pConfigFile->WriteInteger("FilterMagic", szKeyName, m_FilterMagicV[i]);
		}
		pConfigFile->WriteInteger("Pick", "SelMagic", m_CurrentSelMagic);
	}
	g_UiBase.CloseAutoSettingFile(true);
}
/*******************************************************************
	Khac
*******************************************************************/
void KUiOther::Initialize()
{

}
void KUiOther::LoadScheme()
{
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO))
	{
		KWndPage::Init(&Ini, "MainOther");
	
	}	
}

int KUiOther::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:			
	break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}