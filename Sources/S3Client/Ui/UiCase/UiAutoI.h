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

class KUiTreoMay : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme();
	void	Breathe();
	void	Active(BOOL bAct); //ham kich hoat

	KWndText32			m_tLifePoison;
	KWndText32			m_tManaPoison;
	KWndText32			m_tAllPoison;
	KWndText32			m_tAllPoisonMin;
	
	KWndEdit32			m_eLifePoison;
	KWndEdit32			m_eManaPoison;
	KWndEdit32			m_eAllPoison;
	KWndEdit32			m_eAllPoisonMin;
	
	KWndText32			m_tSuaTrangBi;
	KWndLabeledButton	m_tbSuaTrangBi;
	BOOL				m_bSuaTrangBi;
	
	KWndText32			m_tGuiTien;
	KWndLabeledButton	m_tbGuiTien;
	BOOL				m_bGuiTien;
	
	KWndText32			m_tBanTrangBi;
	KWndLabeledButton	m_tbBanTrangBi;
	BOOL				m_bBanTrangBi;
	
	KWndText32			m_tQuayLai;
	KWndLabeledButton	m_tbQuayLai;
	BOOL				m_bQuayLai;
	

BOOL				m_bIsActived;		//duoc kich hoat hay chua?
BOOL                m_Start;	

	
KWndLabeledButton   m_bStart;	
KWndLabeledButton   m_bClose;
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	static KUiTreoMay*		m_pTreoMaySelf;
private:

};


class KUiNhatItem : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme();
	void	Breathe();
	void	Active(BOOL bAct); //ham kich hoat
	
	
	KWndText32			m_tMoneyObj;
	KWndText32			m_tItemObj;
	KWndText32			m_tPropObj;
	KWndText32			m_tItemSelect;
	KWndText32			m_tNumSelect;
	KWndText32			m_tRingSelect;
	KWndText32			m_tWeaSelect;
	
	KWndEdit32			m_eNumSelect;
	
	KWndLabeledButton			m_tbMoneyObj;
	KWndLabeledButton			m_tbItemObj;
	KWndLabeledButton			m_tbPropObj;
	KWndLabeledButton			m_tbItemSelect;
	KWndLabeledButton			m_tbRingSelect;
	KWndLabeledButton			m_tbWeaSelect;
	
	
	BOOL				m_bMoneyObj;
	BOOL				m_bItemObj;
	BOOL				m_bPropObj;
	BOOL				m_bItemSelect;
	BOOL				m_bRingSelect;
	BOOL				m_bWeaSelect;
	
	KWndLabeledButton   m_bStart;	
	KWndLabeledButton   m_bClose;
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	static KUiNhatItem*		m_pNhatItemSelf;
private:





BOOL					m_bIsActived;		//duoc kich hoat hay chua?
BOOL                m_Start;	
};


class KUiFighting : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme();
	void	Breathe();
	void	Active(BOOL bAct); //ham kich hoat
KWndText32		    m_tBoQuaPK;
KWndText32		    m_tBoQuaPK2;
KWndText32		    m_tPhamVi;


KWndText32		    m_tLife;
KWndText32		    m_tMana;
KWndText32		    m_tReturn;
KWndText32		    m_tAtack;
KWndText32		    m_tRange;
KWndText32		    m_tNAtack;
KWndText32		    m_tNQuanhDiem;
KWndText32		    m_tNTheoSau;
KWndText32		    m_tNThongBao;
KWndText32		    m_tGuiTienNew;
KWndText32		    m_tGuiTienNew2;

KWndEdit32		    m_eBoQuaPK;
KWndEdit32			m_eLife;
KWndEdit32		    m_eMana;
KWndEdit32		    m_eReturn;
KWndEdit32		    m_eAtack;
KWndEdit32		    m_eRange;
KWndEdit32		    m_eNAtack;
KWndEdit32		    m_ePhamViTuVe;
KWndEdit32		    m_eGuiTienNew;

	KWndLabeledButton			m_tbQuanhDiem;
	KWndLabeledButton			m_tbTheoSau;
	BOOL				m_bQuanhDiem;
	BOOL				m_bTargetPlayer;
	BOOL				m_bTheoSau;
	
	KWndText32			m_tbHorse;
	BOOL				m_bHorse;
	KWndLabeledButton			m_tHorse;
	KWndText32			m_tbTuVe;
	KWndText32			m_tbPhamViTuVe;
	BOOL				m_bTuVe;
	KWndLabeledButton			m_tTuVe;
	
	
KWndLabeledButton   m_bStart;	
KWndLabeledButton   m_bClose;
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	static KUiFighting*		m_pFightSelf;
private:





BOOL					m_bIsActived;		//duoc kich hoat hay chua?
BOOL                m_Start;	
};

class KUiPickUp : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme();
	void	Breathe();
	void	Active(BOOL bAct); //ham kich hoat

	
	
	KWndText32			m_tTeam;
	KWndText32			m_tReturnPos;
	KWndText32			m_tTeamAuto;
	KWndText32			m_tInviteAuto;
	KWndText32			m_tAcceptAuto;
	KWndText32			m_tCancelAuto;
	

	KWndLabeledButton			m_tbInviteAuto;
	KWndLabeledButton			m_tbAcceptAuto;
	KWndLabeledButton			m_tbCancelAuto;


	BOOL				m_bInviteAuto;
	BOOL				m_bAcceptAuto;
	BOOL				m_bCancelAuto;
	
	KWndText32			m_tLuanChuyenNgaMy;
	KWndText32			m_tMongDiep;
	KWndText32			m_tLuuThuy;
	KWndText32			m_tPhatTam;
	KWndText32			m_tThanhAm;
	KWndText32			m_tPhoDo;
	KWndLabeledButton			m_tbMongDiep;
	KWndLabeledButton			m_tbLuuThuy;
	KWndLabeledButton			m_tbPhatTam;
	KWndLabeledButton			m_tbThanhAm;
	KWndLabeledButton			m_tbPhoDo;
	BOOL				m_bMongDiep;
	BOOL				m_bLuuThuy;
	BOOL				m_bPhatTam;
	BOOL				m_bThanhAm;
	BOOL				m_bPhoDo;
//	KWndEdit32			m_eMongDiep;
//	KWndEdit32			m_eLuuThuy;
//	KWndEdit32			m_ePhatTam;
//	KWndEdit32			m_eThanhAm;
//	KWndEdit32			m_ePhoDo;
	KWndText32		    m_tNgaMyBuff;
	KWndEdit32			m_eNgaMyBuff;
	
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	BOOL					m_bIsActived;		//duoc kich hoat hay chua?

	

};

class KUiCauHinh : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme();
	void	Breathe();
	void	Active(BOOL bAct); //ham kich hoat

	KWndText32			m_tAnNguoiChoi;
	KWndText32			m_tAnHieuUng;
	KWndText32			m_tAnGame;
	KWndText32			m_tPlayer;
	KWndText32			m_tPlayer1;
	KWndText32			m_tPlayer2;
	KWndText32			m_tPlayer3;
	KWndText32			m_tMissle;
	KWndText32			m_tMissle2;
	KWndText32			m_tGame1;
	KWndText32			m_tGame2;
	KWndText32			m_tGame3;
	KWndText32			m_tFPS;
	KWndLabeledButton			m_tHidePlayer;
	KWndLabeledButton			m_tHidePlayer1;
	KWndLabeledButton			m_tHidePlayer2;
	KWndEdit32						m_eHidePlayer3;
	KWndLabeledButton			m_tHideMissle;	
	KWndLabeledButton			m_tHideMissle2;	
	KWndLabeledButton			m_tHideGame1;	
	KWndLabeledButton			m_tHideGame2;	
	KWndLabeledButton			m_tHideGame3;	
	KWndEdit32			m_eFPS;
	BOOL				m_bHidePlayer;
	BOOL				m_bHidePlayer1;
	BOOL				m_bHidePlayer2;
	BOOL				m_bHideMissle;	
	BOOL				m_bHideMissle2;	
	BOOL				m_bHideGame1;	
	BOOL				m_bHideGame2;	
	BOOL				m_bHideGame3;	
	
	
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	BOOL					m_bIsActived;		//duoc kich hoat hay chua?

	

};
class KUiAutoI : public KWndPageSet
{
public:
	static KUiAutoI* OpenWindow();
	static KUiAutoI* ActiveAuto();		
	static KUiAutoI* GetIfVisible();	
	static BOOL GetIfActive();
	static void			CloseWindow(bool bDestroy);		//关闭窗口
	static void			BlockAuto();		//关闭窗口
	static void			LoadScheme();//载入界面方案
	void			Breathe();
	static KUiAutoI*			LoadPrivateSetting(KIniFile* pFile);
	static int			SavePrivateSetting(KIniFile* pFile);
	
	
private:
	KUiAutoI() {}
	~KUiAutoI() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	PaintWindow();
private:
	static KUiAutoI* m_pSelf;
private:
	KWndButton			m_Close;
	KWndLabeledButton			m_Active;
	KWndPureTextBtn		m_TActiveText;
	KWndPureTextBtn			m_FightText;
	KWndPureTextBtn			m_PickText;
	KWndPureTextBtn			m_NhatItemText;
	KWndPureTextBtn			m_TreoMayText;
	KWndPureTextBtn			m_CauHinhText;
	BOOL		m_ActiveText;
	KUiFighting			m_FightPad;	//Bang~ Chien' dau'
	KUiPickUp			m_PickPad;//Bang~ Ho Tro
	KUiNhatItem			m_NhatItemPad;//Bang~ nhat do`
	KUiTreoMay			m_TreoMayPad;//Bang~ treo may
	KUiCauHinh			m_CauHinhPad;//Bang~ giam cau hinh
	
	//--------------------------------------------------------
	KWndButton			m_FightingBtn;	//nut chien' dau'
	KWndButton			m_PickUpBtn;	//nut ho tro
	KWndButton			m_NhatItemBtn;	//nut nhat do`
	KWndButton			m_TreoMayBtn;	//nut nhat do`
	KWndButton			m_CauHinhBtn;	//nut nhat do`
	KWndList		m_NearbyList;
	KUiPlayerItem*	m_pNearbyPlayersList;
	KUiPlayerTeam	m_Info;
	//--------------------------------------------------------
	char				m_szTitle[24]; //ten auto
};
