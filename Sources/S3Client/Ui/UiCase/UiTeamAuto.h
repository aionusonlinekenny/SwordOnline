/*****************************************************************************************
//	Chuc nang: Choi tu dong
//	Copyright : Ken Nguyen 2013
//	Author	:   Ken Nguyen (thienthanden2)
//	CreateTime:	12-05-2013
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../elem/wndpage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndtext.h"
#include "../../../core/src/gamedatadef.h"
#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndLabeledButton.h"
#include "../../../core/src/gamedatadef.h"
#include "../elem/WndList.h"



class KUiFighting1 : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme();
	void	Breathe();
	void	Active(BOOL bAct); //ham kich hoat

KWndText32		    m_tText1;
KWndText32		    m_tText2;
KWndText32		    m_tText3;
KWndText32		    m_tText4;
KWndText32		    m_tText5;
KWndText32		    m_tText6;
KWndText32		    m_tText7;
KWndText32		    m_tText8;
KWndText32		    m_tText9;
KWndText32		    m_tText10;
KWndText32		    m_tText11;
KWndText32		    m_tText12;
KWndText32		    m_tText13;
KWndText32		    m_tText14;
KWndText32		    m_tText15;
KWndText32		    m_tText16;
KWndText32		    m_tText17;
KWndText32		    m_tText18;
KWndText32		    m_tText19;
KWndText32		    m_tText20;
KWndText32		    m_tText21;
KWndText32		    m_tText22;
KWndText32		    m_tText23;
KWndText32		    m_tText24;
KWndText32		    m_tText25;
KWndText32		    m_tText26;
KWndText32		    m_tText27;
KWndText32		    m_tText28;
KWndText32		    m_tText29;
KWndText32		    m_tText30;
KWndText32		    m_tText31;
KWndText32		    m_tText32;
KWndText32		    m_tText33;
KWndText32		    m_tText34;
KWndText32		    m_tText35;
KWndText32		    m_tText36;
KWndText32		    m_tText37;
KWndText32		    m_tText38;
KWndText32		    m_tText39;
KWndText32		    m_tText40;
KWndText32		    m_tText41;

KWndEdit32			m_tEdit1;
KWndEdit32			m_tEdit2;
KWndEdit32			m_tEdit3;
KWndEdit32			m_tEdit4;
KWndEdit32			m_tEdit5;
KWndEdit32			m_tEdit6;
KWndEdit32			m_tEdit7;
KWndEdit32			m_tEdit8;
KWndEdit32			m_tEdit9;
KWndEdit32			m_tEdit10;
KWndEdit32			m_tEdit11;
KWndEdit32			m_tEdit12;
KWndEdit32			m_tEdit13;
KWndEdit32			m_tEdit14;
KWndEdit32			m_tEdit15;
KWndEdit32			m_tEdit16;
KWndEdit32			m_tEdit17;
KWndEdit32			m_tEdit18;
KWndEdit32			m_tEdit19;
KWndEdit32			m_tEdit20;
KWndEdit32			m_tEdit21;
KWndEdit32			m_tEdit22;
KWndEdit32			m_tEdit23;
KWndEdit32			m_tEdit24;
KWndEdit32			m_tEdit25;
KWndEdit32			m_tEdit26;
KWndEdit32			m_tEdit27;
KWndEdit32			m_tEdit28;
KWndEdit32			m_tEdit29;
KWndEdit32			m_tEdit30;
KWndEdit32			m_tEdit31;
KWndEdit32			m_tEdit32;
KWndEdit32			m_tEdit33;
KWndEdit32			m_tEdit34;
KWndEdit32			m_tEdit35;
KWndEdit32			m_tEdit36;
KWndEdit32			m_tEdit37;
KWndEdit32			m_tEdit38;
KWndEdit32			m_tEdit39;
KWndEdit32			m_tEdit40;
KWndEdit32			m_tEdit41;
KWndButton			m_Close;

private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	static KUiFighting1*		m_pFightSelf;
private:





BOOL					m_bIsActived;		//duoc kich hoat hay chua?
BOOL                m_Start;	
};



class KUiTeamAuto : public KWndPageSet
{
public:
	static KUiTeamAuto* OpenWindow();		
	static KUiTeamAuto* GetIfVisible();
		static void			CloseWindow(bool bDestroy);		//关闭窗口
	static void			LoadScheme();//载入界面方案
	void			Breathe();
	static KUiTeamAuto*			LoadPrivateSetting(KIniFile* pFile);
	static int			SavePrivateSetting(KIniFile* pFile);
	
private:
	KUiTeamAuto() {}
	~KUiTeamAuto() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	PaintWindow();
private:
	static KUiTeamAuto* m_pSelf;
private:
	KWndLabeledButton			m_Active;
	KWndPureTextBtn		m_TActiveText;
	BOOL		m_ActiveText;
	KWndPureTextBtn			m_FightText;
	KUiFighting1			m_FightPad;	//Bang~ Chien' dau'
	
	//--------------------------------------------------------
	KWndButton		m_FightingBtn;	//nut chien' dau'
	KWndList		m_NearbyList;
	KUiPlayerItem*	m_pNearbyPlayersList;
	KUiPlayerTeam	m_Info;
	//--------------------------------------------------------
	char				m_szTitle[24]; //ten auto
};
