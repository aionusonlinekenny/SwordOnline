/*****************************************************************************************
//	Chuc nang: Choi tu dong
//	Copyright : Ken Nguyen 2013
//	Author	:   Ken Nguyen (thienthanden2)
//	CreateTime:	12-05-2013
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiAutoI.h"
#include "UiTeamManage.h"
#include "UiTeamAuto.h"
#include "../../../core/src/GameDataDef.h"
#include <crtdbg.h>
#include "../ShortcutKey.h"
#include "../../../core/src/coreshell.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"

extern iRepresentShell*	g_pRepresentShell;

extern iCoreShell*		g_pCoreShell;


#define 	SCHEME_INI_AUTO_SHEET			"\\Ui\\Ui3\\AutoPlay.ini"
//#define 	SCHEME_INI_AUTO_SHEET			"\\Ui\\Ui3\\AutoPlay1024.ini"

//--------------------------------------------------------------------------
//	Chuc nang: Tu chien dau
//--------------------------------------------------------------------------

void KUiTreoMay::Initialize()
{
	Wnd_AddWindow(this);
	
	AddChild(&m_tLifePoison);
	AddChild(&m_tManaPoison);
	AddChild(&m_tAllPoison);
	AddChild(&m_tAllPoisonMin);
	AddChild(&m_eLifePoison);
	AddChild(&m_eManaPoison);
	AddChild(&m_eAllPoison);
	AddChild(&m_eAllPoisonMin);
	AddChild(&m_tSuaTrangBi);
	AddChild(&m_tbSuaTrangBi);
	AddChild(&m_tGuiTien);
	AddChild(&m_tbGuiTien);
	AddChild(&m_tBanTrangBi);
	AddChild(&m_tbBanTrangBi);
	AddChild(&m_tQuayLai);
	AddChild(&m_tbQuayLai);
}
//Cai` dat. file .ini
void KUiTreoMay::LoadScheme()
{
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO_SHEET))
	{
		KWndPage::Init(&Ini, "TreoMay");
		m_tLifePoison.SetText("Cöu ChuyÓn (10):");
		m_tManaPoison.SetText("Thñ ¤ (10):");
		m_tAllPoison.SetText("Ng­ng ThÇn §an (20):");
		m_tAllPoisonMin.SetText("Thõa Tiªn MËt (20):");
		m_tSuaTrangBi.SetText("Tù Söa Trang BÞ");
		m_tGuiTien.SetText("Tù Sö Dông TTL, NNH");
		m_tBanTrangBi.SetText("TDP Khi BÞ §å S¸t");
		m_tQuayLai.SetText("Tù §éng Trë L¹i");

		
		
		m_eLifePoison.Init(&Ini, "EAuto1");
		m_eManaPoison.Init(&Ini, "EAuto2");
		m_eAllPoison.Init(&Ini, "EAuto3");
		m_eAllPoisonMin.Init(&Ini, "EAuto4");
		
		m_tLifePoison.Init(&Ini, "TAuto1");
		m_tManaPoison.Init(&Ini, "TAuto2");
		m_tAllPoison.Init(&Ini, "TAuto3");
		m_tAllPoisonMin.Init(&Ini, "TAuto4");
		m_tSuaTrangBi.Init(&Ini, "TAuto5");
		m_tGuiTien.Init(&Ini, "TAuto6");
		m_tBanTrangBi.Init(&Ini, "TAuto7");
		m_tQuayLai.Init(&Ini, "TAuto8");
		
		m_tbSuaTrangBi.Init(&Ini, "BAuto5");
		m_tbSuaTrangBi.SetFrame(1);
		m_tbSuaTrangBi.CheckButton(true);
		m_bSuaTrangBi = true;
		
		m_tbGuiTien.Init(&Ini, "BAuto6");
		m_tbGuiTien.SetFrame(0);
		m_tbGuiTien.CheckButton(false);
		m_bGuiTien = false;
		
		m_tbBanTrangBi.Init(&Ini, "BAuto7");
		m_tbBanTrangBi.SetFrame(0);
		m_tbBanTrangBi.CheckButton(false);
		m_bBanTrangBi = false;
		
		m_tbQuayLai.Init(&Ini, "BAuto8");
		m_tbQuayLai.SetFrame(0);
		m_tbQuayLai.CheckButton(false);
		m_bQuayLai = false;
		
		m_eLifePoison.SetIntText(0);
		m_eManaPoison.SetIntText(0);
		m_eAllPoison.SetIntText(0);
		m_eAllPoisonMin.SetIntText(0);
		
		
	}
	
}
//Ham` chay. lien tuc. (nhu hoi tho~)
void KUiTreoMay::Breathe()
{
		if(!g_pCoreShell) return;
}
// k¨ªch hoat
void KUiTreoMay::Active(BOOL bAct)
{
	m_bIsActived = bAct;
}

int KUiTreoMay::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_tbSuaTrangBi)
		{
			if (!m_bSuaTrangBi)
			{
				m_bSuaTrangBi = TRUE;
			    m_tbSuaTrangBi.CheckButton(true);
				m_tbSuaTrangBi.SetFrame(1);

			}
		    else
			{
				m_bSuaTrangBi = FALSE;
			    m_tbSuaTrangBi.CheckButton(false);
			    m_tbSuaTrangBi.SetFrame(0);
			}
			
			
		}
	else if (uParam == (unsigned int)(KWndWindow*)&m_tbGuiTien)
		{
			if (!m_bGuiTien)
			{
				m_bGuiTien = TRUE;
			    m_tbGuiTien.CheckButton(true);
				m_tbGuiTien.SetFrame(1);

			}
		    else
			{
				m_bGuiTien = FALSE;
			    m_tbGuiTien.CheckButton(false);
			    m_tbGuiTien.SetFrame(0);
			}
			
			
		}
	else if (uParam == (unsigned int)(KWndWindow*)&m_tbBanTrangBi)
		{
			if (!m_bBanTrangBi)
			{
				
				m_bBanTrangBi = TRUE;
			    m_tbBanTrangBi.CheckButton(true);
				m_tbBanTrangBi.SetFrame(1);
			}
		    else
			{
				m_bBanTrangBi = FALSE;
			    m_tbBanTrangBi.CheckButton(false);
			    m_tbBanTrangBi.SetFrame(0);
			}
		
		
		}
	else if (uParam == (unsigned int)(KWndWindow*)&m_tbQuayLai)
		{
			
			if (!m_bQuayLai)
			{
				m_bQuayLai = TRUE;
			    m_tbQuayLai.CheckButton(true);
				m_tbQuayLai.SetFrame(1);

			}
		    else
			{
				m_bQuayLai = FALSE;
			    m_tbQuayLai.CheckButton(false);
			    m_tbQuayLai.SetFrame(0);
			}
			
	
		}
	
	
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}


void KUiNhatItem::Initialize()
{
	
	Wnd_AddWindow(this);
	
	AddChild(&m_tMoneyObj);
	AddChild(&m_tbMoneyObj);
	AddChild(&m_tItemObj);
	AddChild(&m_tbItemObj);
	AddChild(&m_tPropObj);
	AddChild(&m_tbPropObj);
	
	AddChild(&m_tItemSelect);
	AddChild(&m_tbItemSelect);
	
	AddChild(&m_tNumSelect);
	AddChild(&m_eNumSelect);
	
	AddChild(&m_tbRingSelect);
	AddChild(&m_tRingSelect);
	AddChild(&m_tbWeaSelect);
	AddChild(&m_tWeaSelect);
	
	
	
	
}
//Cai` dat. file .ini
void KUiNhatItem::LoadScheme()
{
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO_SHEET))
	{
		KWndPage::Init(&Ini, "NhatItem");
		
		m_tMoneyObj.SetText("Tù nhÆt tiÒn");
		m_tItemObj.SetText("NhÆt tÊt c¶ TB");
		m_tPropObj.SetText("Tù nhÆt nhiÖm vô");
		m_tItemSelect.SetText("NhÆt ®å chän läc");
		m_tNumSelect.SetText("§óng ®iÒu kiÖn [1-6]:     dßng");
		m_tRingSelect.SetText("ChØ nhÆt trang søc");
		m_tWeaSelect.SetText("ChØ nhÆt vò khÝ ");
		
		

		
		
		m_eNumSelect.Init(&Ini, "ENumSelect");
		m_eNumSelect.SetIntText(1);
		
		m_tMoneyObj.Init(&Ini, "TMoneyObj");
		m_tItemObj.Init(&Ini, "TItemObj");
		m_tPropObj.Init(&Ini, "TPropObj");
		m_tItemSelect.Init(&Ini, "TItemSelect");
		m_tNumSelect.Init(&Ini, "TNumSelect");
		m_tRingSelect.Init(&Ini, "TRingSelect");
		m_tWeaSelect.Init(&Ini, "TWeaSelect");
		
		m_tbRingSelect.Init(&Ini, "BRingSelect");
		m_tbRingSelect.SetFrame(0);
		m_tbRingSelect.CheckButton(false);
		m_bRingSelect = false;	
		
		m_tbWeaSelect.Init(&Ini, "BWeaSelect");
		m_tbWeaSelect.SetFrame(0);
		m_tbWeaSelect.CheckButton(false);
		m_bWeaSelect = false;	
		
		m_tbMoneyObj.Init(&Ini, "BMoneyObj");
		m_tbMoneyObj.SetFrame(0);
		m_tbMoneyObj.CheckButton(true);
		m_bMoneyObj = true;	

		m_tbItemObj.Init(&Ini, "BItemObj");
		m_tbItemObj.SetFrame(0);
		m_tbItemObj.CheckButton(false);
		m_bItemObj = false;

		m_tbPropObj.Init(&Ini, "BPropObj");
		m_tbPropObj.SetFrame(0);
		m_tbPropObj.CheckButton(true);
		m_bPropObj = true;
		
		
		m_tbItemSelect.Init(&Ini, "BItemSelect");
		m_tbItemSelect.SetFrame(0);
		m_tbItemSelect.CheckButton(false);
		m_bItemSelect = false;
	}
}
//Ham` chay. lien tuc. (nhu hoi tho~)
void KUiNhatItem::Breathe()
{
		if(!g_pCoreShell) return;
		
		
	

}
// k¨ªch hoat
void KUiNhatItem::Active(BOOL bAct)
{
	m_bIsActived = bAct;
}

int KUiNhatItem::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		 if (uParam == (unsigned int)(KWndWindow*)&m_tbMoneyObj)
		{
			if (!m_bMoneyObj)
			{
				m_bMoneyObj = TRUE;
			    m_tbMoneyObj.CheckButton(true);
				m_tbMoneyObj.SetFrame(1);

			}
		    else
			{
				m_bMoneyObj = FALSE;
			    m_tbMoneyObj.CheckButton(false);
			    m_tbMoneyObj.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbItemObj)
		{
			if (!m_bItemObj)
			{
				m_bItemObj = TRUE;
			    m_tbItemObj.CheckButton(true);
				m_tbItemObj.SetFrame(1);
				if (KUiTeamAuto::GetIfVisible())
				KUiTeamAuto::CloseWindow(0);
				m_bItemSelect = FALSE;
			    m_tbItemSelect.CheckButton(false);
			    m_tbItemSelect.SetFrame(0);
			}
		    else
			{
				m_bItemObj = FALSE;
			    m_tbItemObj.CheckButton(false);
			    m_tbItemObj.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbPropObj)
		{
			if (!m_bPropObj)
			{
				m_bPropObj = TRUE;
			    m_tbPropObj.CheckButton(true);
				m_tbPropObj.SetFrame(1);

			}
		    else
			{
				m_bPropObj = FALSE;
			    m_tbPropObj.CheckButton(false);
			    m_tbPropObj.SetFrame(0);
			}
		}
	
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbItemSelect)
		{
			if (!m_bItemSelect)
			{
				m_bItemSelect = TRUE;
			    m_tbItemSelect.CheckButton(true);
				m_tbItemSelect.SetFrame(1);
				KUiTeamAuto::OpenWindow();
				m_bItemObj = FALSE;
			    m_tbItemObj.CheckButton(false);
			    m_tbItemObj.SetFrame(0);
			}
		    else
			{
				if (KUiTeamAuto::GetIfVisible())
				KUiTeamAuto::CloseWindow(0);
				m_bItemSelect = FALSE;
			    m_tbItemSelect.CheckButton(false);
			    m_tbItemSelect.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbRingSelect)
		{
			if (!m_bRingSelect && m_bItemSelect)
			{
				m_bRingSelect = TRUE;
			    m_tbRingSelect.CheckButton(true);
				m_tbRingSelect.SetFrame(1);
			}
		    else
			{
				m_bRingSelect = FALSE;
			    m_tbRingSelect.CheckButton(false);
			    m_tbRingSelect.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbWeaSelect)
		{
			if (!m_bWeaSelect && m_bItemSelect)
			{
				m_bWeaSelect = TRUE;
			    m_tbWeaSelect.CheckButton(true);
				m_tbWeaSelect.SetFrame(1);
			}
		    else
			{
				m_bWeaSelect = FALSE;
			    m_tbWeaSelect.CheckButton(false);
			    m_tbWeaSelect.SetFrame(0);
			}
		}
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}





void KUiFighting::Initialize()
{
	
	AddChild(&m_tLife);
 	AddChild(&m_tMana);
	AddChild(&m_tReturn);
	AddChild(&m_tAtack);
	AddChild(&m_tRange);
	AddChild(&m_tNAtack);
	
	
	AddChild(&m_tNQuanhDiem);
	AddChild(&m_tbQuanhDiem);
	AddChild(&m_tNTheoSau);
	AddChild(&m_tbTheoSau);
	AddChild(&m_tNThongBao);

	AddChild(&m_eLife);
 	AddChild(&m_eMana);
	AddChild(&m_eReturn);
	AddChild(&m_eAtack);
	AddChild(&m_eRange);
	AddChild(&m_eNAtack);
	AddChild(&m_ePhamViTuVe);
	
	AddChild(&m_tHorse);
	AddChild(&m_tbHorse);
	AddChild(&m_tTuVe);
	AddChild(&m_tbTuVe);
	AddChild(&m_tbPhamViTuVe);
	
	AddChild(&m_tBoQuaPK);
	AddChild(&m_tBoQuaPK2);
	AddChild(&m_eBoQuaPK);
	
	AddChild(&m_tGuiTienNew);
	AddChild(&m_tGuiTienNew2);
	AddChild(&m_eGuiTienNew);
	
	Wnd_AddWindow(this);
}
//Cai` dat. file .ini
void KUiFighting::LoadScheme()
{
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO_SHEET))
	{
		KWndPage::Init(&Ini, "Fighting");
		m_tLife.Init(&Ini, "TLife");
 		m_tMana.Init(&Ini, "TMana");
		m_tReturn.Init(&Ini, "TReturn");
		m_tBoQuaPK.Init(&Ini, "TBoQuaPK");
		m_tBoQuaPK2.Init(&Ini, "TBoQuaPK2");
		m_tAtack.Init(&Ini, "TAtack");
	    m_tRange.Init(&Ini, "TRange");
	
		m_tNAtack.Init(&Ini, "TNAtack");
		m_eLife.Init(&Ini, "ELife");
		
		
		
 	    m_eMana.Init(&Ini, "EMana");
		m_eReturn.Init(&Ini, "EReturn");
		m_eBoQuaPK.Init(&Ini, "EBoQuaPK");
		m_eAtack.Init(&Ini, "EAtack");
	    m_eRange.Init(&Ini, "ERange");
	    m_eNAtack.Init(&Ini, "ENAtack");
	    m_ePhamViTuVe.Init(&Ini, "EPhamViTuVe");

		
		m_tNQuanhDiem.Init(&Ini, "TNQuanhDiem");
		m_tNTheoSau.Init(&Ini, "TNTargetPlayer");
		
		m_tNThongBao.Init(&Ini, "TNThongBao");
		m_tbQuanhDiem.Init(&Ini, "TQuanhDiem");
		m_tbQuanhDiem.SetFrame(0);
		m_tbQuanhDiem.CheckButton(false);
		m_bQuanhDiem = false;
		
		m_tbTheoSau.Init(&Ini, "TTargetPlayer");
		m_tbTheoSau.SetFrame(0);
		m_tbTheoSau.CheckButton(false);
		m_bTheoSau = false;
		
		m_bTargetPlayer = false;
		m_tLife.SetText("Håi Sinh Lùc");
		
 	    m_tMana.SetText("Håi Néi Lùc");
	    m_tReturn.SetText("Thæ §Þa Phï");
		
		
	    m_tBoQuaPK.SetText("Thêi gian ®æi ng­êi");
	    m_tBoQuaPK2.SetText("gi©y");
		m_tAtack.SetText("Ph¹m vi tÊn c«ng");
		m_tRange.SetText("Kho¶ng c¸ch xuÊt chiªu");
		m_tNAtack.SetText("Tù Bá Qua");
		m_tNQuanhDiem.SetText("Quanh §iÓm");
		m_tNTheoSau.SetText("Theo Sau");
		m_tNThongBao.SetText("ChÕ §é: LuyÖn C«ng (SPACE)");

		m_eLife.SetIntText(500);
 	    m_eMana.SetIntText(200);
		m_eReturn.SetIntText(0);
		m_eBoQuaPK.SetIntText(5);
		m_eAtack.SetIntText(500);
	    m_eRange.SetIntText(75);
	    m_eNAtack.SetIntText(5000);
		m_ePhamViTuVe.SetIntText(500);
		
		m_tbHorse.Init(&Ini, "THorse");
		m_tbHorse.SetText("Xuèng ngùa");
		m_tHorse.Init(&Ini, "BHorse");
		m_tHorse.SetFrame(0);
		m_tHorse.CheckButton(false);
		m_bHorse = false;
		 
		m_tbTuVe.Init(&Ini, "TTuVe");
		m_tbTuVe.SetText("Tù VÖ");
		m_tbPhamViTuVe.Init(&Ini, "TPhamViTuVe");
		m_tbPhamViTuVe.SetText("Ph¹m vi");
		
		m_tTuVe.Init(&Ini, "BTuVe");
		m_tTuVe.SetFrame(0);
		m_tTuVe.CheckButton(false);
		m_bTuVe = false;
		
		
		
		m_tGuiTienNew.Init(&Ini, "TGuiTienNew");
	    m_tGuiTienNew.SetText("TDP khi ng©n l­îng > ");
		m_tGuiTienNew2.Init(&Ini, "TGuiTienNew2");
	    m_tGuiTienNew2.SetText("v¹n");
		m_eGuiTienNew.Init(&Ini, "EGuiTienNew");
		m_eGuiTienNew.SetIntText(9999); // them goi tien 
	}
	
}
//Ham` chay. lien tuc. (nhu hoi tho~)
void KUiFighting::Breathe()
{

}
// k¨ªch hoat
void KUiFighting::Active(BOOL bAct)
{
	m_bIsActived = bAct;
}

int KUiFighting::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{

	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_tbTheoSau)
		{
			if (!m_bTheoSau && !m_bTargetPlayer) 
			{
				m_bTheoSau = TRUE;
			    m_tbTheoSau.CheckButton(true);
				m_tbTheoSau.SetFrame(1);
				
				m_bTuVe = FALSE;
			    m_tTuVe.CheckButton(false);
				m_tTuVe.SetFrame(0);
			}
		    else
			{
				m_bTheoSau = FALSE;
			    m_tbTheoSau.CheckButton(false);
			    m_tbTheoSau.SetFrame(0);
				
				m_bTuVe = TRUE;
			    m_tTuVe.CheckButton(true);
				m_tTuVe.SetFrame(1);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbQuanhDiem)
		{
			if (!m_bQuanhDiem)
			{
				m_bQuanhDiem = TRUE;
			    m_tbQuanhDiem.CheckButton(true);
				m_tbQuanhDiem.SetFrame(1);
			}
		    else
			{
				m_bQuanhDiem = FALSE;
			    m_tbQuanhDiem.CheckButton(false);
			    m_tbQuanhDiem.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tHorse)
		{
			if (!m_bHorse)
			{
				m_bHorse = TRUE;
			    m_tHorse.CheckButton(true);
				m_tHorse.SetFrame(1);

			}
		    else
			{
				m_bHorse = FALSE;
			    m_tHorse.CheckButton(false);
			    m_tHorse.SetFrame(0);
			}

		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tTuVe)
		{
			if (!m_bTuVe)
			{
				m_bTuVe = TRUE;
			    m_tTuVe.CheckButton(true);
				m_tTuVe.SetFrame(1);
				
				m_bTheoSau = FALSE;
			    m_tbTheoSau.CheckButton(false);
				m_tbTheoSau.SetFrame(0);

			}
		    else
			{
				m_bTuVe = FALSE;
			    m_tTuVe.CheckButton(false);
			    m_tTuVe.SetFrame(0);
				
				m_bTheoSau = TRUE;
			    m_tbTheoSau.CheckButton(true);
				m_tbTheoSau.SetFrame(1);
			}
		}
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

//--------------------------------------------------------------------------
//	Chuc nang: Tu nhat do
//--------------------------------------------------------------------------
void KUiPickUp::Initialize()
{

	
	
	AddChild(&m_tTeamAuto);
	AddChild(&m_tInviteAuto);
	AddChild(&m_tbInviteAuto);
	AddChild(&m_tAcceptAuto);
	AddChild(&m_tbAcceptAuto);
	AddChild(&m_tCancelAuto);
	AddChild(&m_tbCancelAuto);
	
	AddChild(&m_tbMongDiep);
	AddChild(&m_tbLuuThuy);
	AddChild(&m_tbPhatTam);
	AddChild(&m_tbThanhAm);
	AddChild(&m_tbPhoDo);
//	AddChild(&m_eMongDiep);
//	AddChild(&m_eLuuThuy);
//	AddChild(&m_ePhatTam);
//	AddChild(&m_eThanhAm);
//	AddChild(&m_ePhoDo);
	AddChild(&m_tLuanChuyenNgaMy);
	AddChild(&m_tMongDiep);
	AddChild(&m_tLuuThuy);
	AddChild(&m_tPhatTam);
	AddChild(&m_tThanhAm);
	AddChild(&m_tPhoDo);
	
	AddChild(&m_tNgaMyBuff);
	AddChild(&m_eNgaMyBuff);
}

void KUiPickUp::LoadScheme()
{
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO_SHEET))
	{
		KWndPage::Init(&Ini, "PickUp");

		

		
		
		m_tTeamAuto.Init(&Ini, "TTeamAuto");
		m_tInviteAuto.Init(&Ini, "TInviteAuto");
		m_tAcceptAuto.Init(&Ini, "TAcceptAuto");
		m_tCancelAuto.Init(&Ini, "TCancelAuto");
		
		m_tTeamAuto.SetText("---- Chøc N¨ng Tæ §éi ----");
		
		m_tInviteAuto.SetText("Mêi");
		m_tAcceptAuto.SetText("§ång ý");
		m_tCancelAuto.SetText("Tõ Chèi");
		
	
		
		m_tbInviteAuto.Init(&Ini, "BInviteAuto");
		m_tbInviteAuto.SetFrame(0);
		m_tbInviteAuto.CheckButton(false);
		m_bInviteAuto = false;
		
		m_tbAcceptAuto.Init(&Ini, "BAcceptAuto");
		m_tbAcceptAuto.SetFrame(0);
		m_tbAcceptAuto.CheckButton(false);
		m_bAcceptAuto = false;
		
		m_tbCancelAuto.Init(&Ini, "BCancelAuto");
		m_tbCancelAuto.SetFrame(0);
		m_tbCancelAuto.CheckButton(false);
		m_bCancelAuto = false;
		
		
		
		m_tLuanChuyenNgaMy.Init(&Ini, "TLuanChuyenNgaMy");
		m_tLuanChuyenNgaMy.SetText("---- Lu©n ChuyÓn Nga My ----");	
		m_tMongDiep.Init(&Ini, "TMongDiep");
		m_tMongDiep.SetText("Méng §iÖp");
		m_tLuuThuy.Init(&Ini, "TLuuThuy");
		m_tLuuThuy.SetText("L­u Thñy");	
		m_tPhatTam.Init(&Ini, "TPhatTam");
		m_tPhatTam.SetText("PhËt T©m Tõ H÷u");	
		m_tThanhAm.Init(&Ini, "TThanhAm");
		m_tThanhAm.SetText("Thanh ¢m Ph¹n X­íng");	
		m_tPhoDo.Init(&Ini, "TPhoDo");
		m_tPhoDo.SetText("Phæ §é Chóng Sinh");		
//		m_eMongDiep.Init(&Ini, "EMongDiep");
//		m_eLuuThuy.Init(&Ini, "ELuuThuy");
//		m_ePhatTam.Init(&Ini, "EPhatTam");
//		m_eThanhAm.Init(&Ini, "EThanhAm");
//		m_ePhoDo.Init(&Ini, "EPhoDo");

//		m_eMongDiep.SetIntText(0);
//		m_eLuuThuy.SetIntText(0);
//		m_ePhatTam.SetIntText(0);
//		m_eThanhAm.SetIntText(0);
//		m_ePhoDo.SetIntText(0);
		
		m_tNgaMyBuff.SetText("Buff Nga My (%)");
		m_tNgaMyBuff.Init(&Ini, "TNgaMyBuff");
		m_eNgaMyBuff.Init(&Ini, "ENgaMyBuff");
		m_eNgaMyBuff.SetIntText(50);
		
		
		m_tbMongDiep.Init(&Ini, "TBMongDiep");
		m_tbMongDiep.SetFrame(0);
		m_tbMongDiep.CheckButton(false);
		m_bMongDiep = false;
		
		m_tbLuuThuy.Init(&Ini, "TBLuuThuy");
		m_tbLuuThuy.SetFrame(0);
		m_tbLuuThuy.CheckButton(false);
		m_bLuuThuy = false;

		m_tbPhatTam.Init(&Ini, "TBPhatTam");
		m_tbPhatTam.SetFrame(0);
		m_tbPhatTam.CheckButton(false);
		m_bPhatTam = false;		
		
		m_tbThanhAm.Init(&Ini, "TBThanhAm");
		m_tbThanhAm.SetFrame(0);
		m_tbThanhAm.CheckButton(false);
		m_bThanhAm = false;		
		
		m_tbPhoDo.Init(&Ini, "TBPhoDo");
		m_tbPhoDo.SetFrame(0);
		m_tbPhoDo.CheckButton(false);
		m_bPhoDo = false;		
	}
}
//Ham` thuc. thi cua sheet nhat. do`
void KUiPickUp::Breathe()
{
	if(!g_pCoreShell) return;


}



int KUiPickUp::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_tbInviteAuto)
		{
			if (!m_bInviteAuto)
			{
				m_bInviteAuto = TRUE;
			    m_tbInviteAuto.CheckButton(true);
				m_tbInviteAuto.SetFrame(1);
				m_bAcceptAuto = FALSE;
			    m_tbAcceptAuto.CheckButton(false);
			    m_tbAcceptAuto.SetFrame(0);
				m_bCancelAuto = FALSE;
			    m_tbCancelAuto.CheckButton(false);
			    m_tbCancelAuto.SetFrame(0);
			}
		    else
			{
				m_bInviteAuto = FALSE;
			    m_tbInviteAuto.CheckButton(false);
			    m_tbInviteAuto.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbAcceptAuto)
		{
			if (!m_bAcceptAuto)
			{
				m_bAcceptAuto = TRUE;
			    m_tbAcceptAuto.CheckButton(true);
				m_tbAcceptAuto.SetFrame(1);
				m_bInviteAuto = FALSE;
			    m_tbInviteAuto.CheckButton(false);
			    m_tbInviteAuto.SetFrame(0);
				m_bCancelAuto = FALSE;
			    m_tbCancelAuto.CheckButton(false);
			    m_tbCancelAuto.SetFrame(0);

			}
		    else
			{
				m_bAcceptAuto = FALSE;
			    m_tbAcceptAuto.CheckButton(false);
			    m_tbAcceptAuto.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbCancelAuto)
		{
			if (!m_bCancelAuto)
			{
				m_bCancelAuto = TRUE;
			    m_tbCancelAuto.CheckButton(true);
				m_tbCancelAuto.SetFrame(1);
				m_bInviteAuto = FALSE;
			    m_tbInviteAuto.CheckButton(false);
			    m_tbInviteAuto.SetFrame(0);
				m_bAcceptAuto = FALSE;
			    m_tbAcceptAuto.CheckButton(false);
			    m_tbAcceptAuto.SetFrame(0);

			}
		    else
			{
				m_bCancelAuto = FALSE;
			    m_tbCancelAuto.CheckButton(false);
			    m_tbCancelAuto.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbMongDiep)
		{
			if (!m_bMongDiep)
			{
				m_bMongDiep = TRUE;
			    m_tbMongDiep.CheckButton(true);
				m_tbMongDiep.SetFrame(1);

			}
		    else
			{
				m_bMongDiep = FALSE;
			    m_tbMongDiep.CheckButton(false);
			    m_tbMongDiep.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbLuuThuy)
		{
			if (!m_bLuuThuy)
			{
				m_bLuuThuy = TRUE;
			    m_tbLuuThuy.CheckButton(true);
				m_tbLuuThuy.SetFrame(1);

			}
		    else
			{
				m_bLuuThuy = FALSE;
			    m_tbLuuThuy.CheckButton(false);
			    m_tbLuuThuy.SetFrame(0);
			}
		}		
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbPhatTam)
		{
			if (!m_bPhatTam)
			{
				m_bPhatTam = TRUE;
			    m_tbPhatTam.CheckButton(true);
				m_tbPhatTam.SetFrame(1);

			}
		    else
			{
				m_bPhatTam = FALSE;
			    m_tbPhatTam.CheckButton(false);
			    m_tbPhatTam.SetFrame(0);
			}
		}		
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbThanhAm)
		{
			if (!m_bThanhAm)
			{
				m_bThanhAm = TRUE;
			    m_tbThanhAm.CheckButton(true);
				m_tbThanhAm.SetFrame(1);

			}
		    else
			{
				m_bThanhAm = FALSE;
			    m_tbThanhAm.CheckButton(false);
			    m_tbThanhAm.SetFrame(0);
			}
		}			
		else if (uParam == (unsigned int)(KWndWindow*)&m_tbPhoDo)
		{
			if (!m_bPhoDo)
			{
				m_bPhoDo = TRUE;
			    m_tbPhoDo.CheckButton(true);
				m_tbPhoDo.SetFrame(1);

			}
		    else
			{
				m_bPhoDo = FALSE;
			    m_tbPhoDo.CheckButton(false);
			    m_tbPhoDo.SetFrame(0);
			}
		}				
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}


void KUiCauHinh::Initialize()
{
	AddChild(&m_tPlayer);
	AddChild(&m_tPlayer1);
	AddChild(&m_tPlayer2);
	AddChild(&m_tPlayer3);
	AddChild(&m_tMissle);
	AddChild(&m_tMissle2);
	AddChild(&m_tGame1);
	AddChild(&m_tGame2);
	AddChild(&m_tGame3);
	AddChild(&m_tHidePlayer);
	AddChild(&m_tHidePlayer1);
	AddChild(&m_tHidePlayer2);
	AddChild(&m_eHidePlayer3);
	AddChild(&m_tHideMissle);
	AddChild(&m_tHideMissle2);
	AddChild(&m_tHideGame1);
	AddChild(&m_tHideGame2);
	AddChild(&m_tHideGame3);
	AddChild(&m_tAnNguoiChoi);
	AddChild(&m_tAnHieuUng);
	AddChild(&m_tAnGame);
	AddChild(&m_eFPS);
	AddChild(&m_tFPS);

	
}
void KUiCauHinh::LoadScheme()
{
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO_SHEET))
	{
		KWndPage::Init(&Ini, "CauHinh");
		m_tFPS.SetText("Gi¶m FPS [1-200]");
		m_tFPS.Init(&Ini, "TFPS");
		m_eFPS.Init(&Ini, "EFPS");
		m_eFPS.SetIntText(0);
		m_tAnNguoiChoi.Init(&Ini, "TAnNguoiChoi");
		m_tAnHieuUng.Init(&Ini, "TAnHieuUng");
		m_tAnGame.Init(&Ini, "TAnGame");
		m_tPlayer.Init(&Ini, "TPlayer");
		m_tPlayer1.Init(&Ini, "TPlayer1");
		m_tPlayer2.Init(&Ini, "TPlayer2");
		m_tPlayer3.Init(&Ini, "TPlayer3");
		m_tMissle.Init(&Ini, "TMissle");
		m_tMissle2.Init(&Ini, "TMissle2");
		m_tGame1.Init(&Ini, "TGame1");
		m_tGame2.Init(&Ini, "TGame2");
		m_tGame3.Init(&Ini, "TGame3");
		m_tAnNguoiChoi.SetText("------- Èn ng­êi ch¬i -------");
		m_tAnHieuUng.SetText("-------- Èn hiÖu øng --------");
		m_tAnGame.SetText("----- HiÓn thÞ h×nh ¶nh -----");
		m_tPlayer.SetText("TÊt C¶");
		m_tPlayer1.SetText("Cïng Mµu");
		m_tPlayer2.SetText("Kh¸c Mµu");
		m_tPlayer3.SetText("Ph¹m Vi");
		m_tMissle.SetText("Kü N¨ng ");
		m_tMissle2.SetText("Bïa Chó ");
		m_tGame1.SetText("Ng­êi ch¬i ");
		m_tGame2.SetText("NPC ");
		m_tGame3.SetText("B¶n §å ");
		m_tHidePlayer.Init(&Ini, "THidePlayer");
		m_tHidePlayer.SetFrame(0);
		m_tHidePlayer.CheckButton(false);
		m_bHidePlayer = false;
		m_tHidePlayer1.Init(&Ini, "THidePlayer1");
		m_tHidePlayer1.SetFrame(0);
		m_tHidePlayer1.CheckButton(false);
		m_bHidePlayer1 = false;
		m_tHidePlayer2.Init(&Ini, "THidePlayer2");
		m_tHidePlayer2.SetFrame(0);
		m_tHidePlayer2.CheckButton(false);
		m_bHidePlayer2 = false;
		m_eHidePlayer3.Init(&Ini, "EHidePlayer3");
		m_eHidePlayer3.SetIntText(0);
		m_tHideMissle.Init(&Ini, "THideMissle");
		m_tHideMissle.SetFrame(0);
		m_tHideMissle.CheckButton(false);
		m_bHideMissle = false;
		m_tHideMissle2.Init(&Ini, "THideMissle2");
		m_tHideMissle2.SetFrame(0);
		m_tHideMissle2.CheckButton(false);
		m_bHideMissle2 = false;
		m_tHideGame1.Init(&Ini, "THideGame1");
		m_tHideGame1.SetFrame(0);
		m_tHideGame1.CheckButton(false);
		m_bHideGame1 = false;
		m_tHideGame2.Init(&Ini, "THideGame2");
		m_tHideGame2.SetFrame(0);
		m_tHideGame2.CheckButton(false);
		m_bHideGame2 = false;
		m_tHideGame3.Init(&Ini, "THideGame3");
		m_tHideGame3.SetFrame(0);
		m_tHideGame3.CheckButton(false);
		m_bHideGame3 = false;
	}
}
//Ham` thuc. thi cua sheet nhat. do`
void KUiCauHinh::Breathe()
{
	if(!g_pCoreShell) return;
}

int KUiCauHinh::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_tHidePlayer)
		{
			//KUiTeamAuto::OpenWindow();
			if (!m_bHidePlayer)
			{
				m_bHidePlayer = TRUE;
			    m_tHidePlayer.CheckButton(true);
				m_tHidePlayer.SetFrame(1);
		
				m_bHidePlayer2 = FALSE;
			    m_tHidePlayer2.CheckButton(false);
			    m_tHidePlayer2.SetFrame(0);
				m_bHidePlayer1 = FALSE;
			    m_tHidePlayer1.CheckButton(false);
			    m_tHidePlayer1.SetFrame(0);
			}
		    else
			{
				m_bHidePlayer = FALSE;
			    m_tHidePlayer.CheckButton(false);
			    m_tHidePlayer.SetFrame(0);
			}

		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tHidePlayer1)
		{
			//KUiTeamAuto::OpenWindow();
			if (!m_bHidePlayer1)
			{
				m_bHidePlayer1 = TRUE;
			    m_tHidePlayer1.CheckButton(true);
				m_tHidePlayer1.SetFrame(1);
				
				m_bHidePlayer = FALSE;
			    m_tHidePlayer.CheckButton(false);
			    m_tHidePlayer.SetFrame(0);
				m_bHidePlayer2 = FALSE;
			    m_tHidePlayer2.CheckButton(false);
			    m_tHidePlayer2.SetFrame(0);

			}
		    else
			{
				m_bHidePlayer1 = FALSE;
			    m_tHidePlayer1.CheckButton(false);
			    m_tHidePlayer1.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tHidePlayer2)
		{
			//KUiTeamAuto::OpenWindow();
			if (!m_bHidePlayer2)
			{
				m_bHidePlayer2 = TRUE;
			    m_tHidePlayer2.CheckButton(true);
				m_tHidePlayer2.SetFrame(1);
				
				m_bHidePlayer = FALSE;
			    m_tHidePlayer.CheckButton(false);
			    m_tHidePlayer.SetFrame(0);
				m_bHidePlayer1 = FALSE;
			    m_tHidePlayer1.CheckButton(false);
			    m_tHidePlayer1.SetFrame(0);
			}
		    else
			{
				m_bHidePlayer2 = FALSE;
			    m_tHidePlayer2.CheckButton(false);
			    m_tHidePlayer2.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tHideMissle)
		{
			if (!m_bHideMissle)
			{
				m_bHideMissle = TRUE;
			    m_tHideMissle.CheckButton(true);
				m_tHideMissle.SetFrame(1);

			}
		    else
			{
				m_bHideMissle = FALSE;
			    m_tHideMissle.CheckButton(false);
			    m_tHideMissle.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tHideMissle2)
		{
			if (!m_bHideMissle2)
			{
				m_bHideMissle2 = TRUE;
			    m_tHideMissle2.CheckButton(true);
				m_tHideMissle2.SetFrame(1);

			}
		    else
			{
				m_bHideMissle2 = FALSE;
			    m_tHideMissle2.CheckButton(false);
			    m_tHideMissle2.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tHideGame1)
		{
			if (!m_bHideGame1)
			{
				m_bHideGame1 = TRUE;
			    m_tHideGame1.CheckButton(true);
				m_tHideGame1.SetFrame(1);

			}
		    else
			{
				m_bHideGame1 = FALSE;
			    m_tHideGame1.CheckButton(false);
			    m_tHideGame1.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tHideGame2)
		{
			if (!m_bHideGame2)
			{
				m_bHideGame2 = TRUE;
			    m_tHideGame2.CheckButton(true);
				m_tHideGame2.SetFrame(1);

			}
		    else
			{
				m_bHideGame2 = FALSE;
			    m_tHideGame2.CheckButton(false);
			    m_tHideGame2.SetFrame(0);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_tHideGame3)
		{
			if (!m_bHideGame3)
			{
				m_bHideGame3 = TRUE;
			    m_tHideGame3.CheckButton(true);
				m_tHideGame3.SetFrame(1);

			}
		    else
			{
				m_bHideGame3 = FALSE;
			    m_tHideGame3.CheckButton(false);
			    m_tHideGame3.SetFrame(0);
			}
		}
	break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}



KUiAutoI* KUiAutoI::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	chuc nang: mo cua~ so~ sheet chinh'
//--------------------------------------------------------------------------
KUiAutoI* KUiAutoI::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiAutoI;
		if (m_pSelf)
		{
			m_pSelf->Initialize();
			m_pSelf->m_FightPad.Show();
		}
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
		//m_pSelf->m_ActiveText = FALSE;
	}

	return m_pSelf;
}

//--------------------------------------------------------------------------
//	chuc nang: kiem tra co' an~ hay khong
//--------------------------------------------------------------------------
KUiAutoI* KUiAutoI::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	chuc nang: dong lai
//--------------------------------------------------------------------------
void KUiAutoI::CloseWindow(bool bDestroy)
{
	m_pSelf->Hide();
	
//	if (m_pSelf)
//	{
//		if (bDestroy == false)
//			m_pSelf->Hide();
//		else
//		{
//			m_pSelf->Destroy();
//			m_pSelf = NULL;
//		}
//	}
}

//--------------------------------------------------------------------------
//	chuc nang: khoi tao
//--------------------------------------------------------------------------
void KUiAutoI::Initialize()
{
	m_FightPad.Initialize();
	m_PickPad.Initialize();
	m_NhatItemPad.Initialize();
	m_TreoMayPad.Initialize();
	m_CauHinhPad.Initialize();
	
	AddPage(&m_FightPad, &m_FightingBtn);
	AddPage(&m_PickPad, &m_PickUpBtn);
	AddPage(&m_NhatItemPad, &m_NhatItemBtn);
	AddPage(&m_TreoMayPad, &m_TreoMayBtn);
	AddPage(&m_CauHinhPad, &m_CauHinhBtn);
	
	
	
	AddChild(&m_Close);
	AddChild(&m_Active);
	AddChild(&m_TActiveText);
	AddChild(&m_FightText);
	AddChild(&m_PickText);
	AddChild(&m_NhatItemText);
	AddChild(&m_TreoMayText);
	AddChild(&m_CauHinhText);
	
	Wnd_AddWindow(this);
	memset(m_szTitle,0,sizeof(m_szTitle));
	strcpy(m_szTitle,"Tù §éng ChiÕn §Êu");
	LoadScheme();
}

//--------------------------------------------------------------------------
//	chuc nang: khoi tao theo file .ini
//--------------------------------------------------------------------------
void KUiAutoI::LoadScheme()
{
	KIniFile	Ini;
	if (m_pSelf && Ini.Load(SCHEME_INI_AUTO_SHEET))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_Close.Init(&Ini, "CloseBtn");

		m_pSelf->m_Active.Init(&Ini, "ActiveBtn");
		m_pSelf->m_TActiveText.Init(&Ini, "ActiveText");

		m_pSelf->m_FightingBtn.Init(&Ini, "FightingBtn");
		m_pSelf->m_PickUpBtn.Init(&Ini, "PickUpBtn");
		m_pSelf->m_NhatItemBtn.Init(&Ini, "NhatItemBtn");
		m_pSelf->m_TreoMayBtn.Init(&Ini, "TreoMayBtn");
		m_pSelf->m_CauHinhBtn.Init(&Ini, "CauHinhBtn");

		m_pSelf->m_FightText.Init(&Ini, "FightText");
		m_pSelf->m_PickText.Init(&Ini, "PickText");
		m_pSelf->m_NhatItemText.Init(&Ini, "NhatItemText");
		m_pSelf->m_TreoMayText.Init(&Ini, "TreoMayText");
		m_pSelf->m_CauHinhText.Init(&Ini, "CauHinhText");
		
		m_pSelf->m_FightText.SetText("ChiÕn ®Êu");
		m_pSelf->m_PickText.SetText("Hç Trî ");
		m_pSelf->m_NhatItemText.SetText("NhÆt §å ");
		m_pSelf->m_TreoMayText.SetText("Treo M¸y");
		m_pSelf->m_CauHinhText.SetText("Tèi ¦u");
		m_pSelf->m_TActiveText.SetText("KÝch ho¹t");

		m_pSelf->m_Active.SetFrame(0);
		m_pSelf->m_Active.CheckButton(false);
		m_pSelf->m_ActiveText = false;

		m_pSelf->m_FightPad.LoadScheme();
		m_pSelf->m_PickPad.LoadScheme();
		m_pSelf->m_NhatItemPad.LoadScheme();
		m_pSelf->m_TreoMayPad.LoadScheme();
		m_pSelf->m_CauHinhPad.LoadScheme();

	}
}
//Paint my love :)
//Ham` ve~
void KUiAutoI::PaintWindow()
{
	KWndPageSet::PaintWindow();
	if (g_pRepresentShell)
	{
		KRUShadow	bg;
		GetAbsolutePos(&bg.oPosition.nX, &bg.oPosition.nY);
		bg.oPosition.nX += 5;
		bg.oPosition.nY += 82;
		bg.Color.Color_dw = 0x16000000;
		GetSize(&bg.oEndPos.nX, &bg.oEndPos.nY);
		bg.oEndPos.nX += bg.oPosition.nX - 10;
		bg.oEndPos.nY += bg.oPosition.nY - 102;
		g_pRepresentShell->DrawPrimitives(1, &bg, RU_T_SHADOW, true);

		KRUImage RUIconImage;
		RUIconImage.nType = ISI_T_SPR;
		RUIconImage.Color.Color_b.a = 255;
		RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		RUIconImage.uImage = 0;
		RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
		RUIconImage.bRenderFlag = 0;
//		strcpy(RUIconImage.szImage, "\\spr\\Auto\\thanhdai.spr");
		RUIconImage.oPosition.nX = m_Left + 28;
		RUIconImage.oPosition.nY = m_Top + 6;
		RUIconImage.oPosition.nZ = 0;
		RUIconImage.nFrame = 0;
		g_pRepresentShell->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 1);
		
			
		if(m_szTitle[0])
		{
		KOutputTextParam param;
		param.Color = 0xfffffcb2;
		param.BorderColor = 0xff000000;
		param.nX = m_Left + 34;
		param.nY = m_Top + 10;
		param.nZ = TEXT_IN_SINGLE_PLANE_COORD;
		param.nSkipLine = 0;
		param.nNumLine = 1;
		g_pRepresentShell->OutputRichText(16, &param, m_szTitle, strlen(m_szTitle));
		}
		if (m_NhatItemBtn.IsButtonChecked())
		{
			KRURect	rect;
			rect.Color.Color_dw = 0xfffffcb2;
			rect.oPosition.nX = bg.oPosition.nX +138;
			rect.oPosition.nY = bg.oPosition.nY +119;
			rect.oEndPos.nZ = rect.oPosition.nZ = 0;
			rect.oEndPos.nX = rect.oPosition.nX + 23;
			rect.oEndPos.nY = rect.oPosition.nY + 14;
			g_pRepresentShell->DrawPrimitives(1, &rect, RU_T_RECT, true);
			
			
		}
		
		if (m_PickUpBtn.IsButtonChecked())
		{
	
			KRURect	rect5;
			rect5.Color.Color_dw = 0xfffffcb2;
			rect5.oPosition.nX = bg.oPosition.nX +120;
			rect5.oPosition.nY = bg.oPosition.nY + 165;
			rect5.oEndPos.nZ = rect5.oPosition.nZ = 0;
			rect5.oEndPos.nX = rect5.oPosition.nX + 60;
			rect5.oEndPos.nY = rect5.oPosition.nY + 14;
			g_pRepresentShell->DrawPrimitives(1, &rect5, RU_T_RECT, true);
		
		}
		if (m_CauHinhBtn.IsButtonChecked())
		{
			KRURect	rectpv;
			rectpv.Color.Color_dw = 0xfffffcb2;
			rectpv.oPosition.nX = bg.oPosition.nX +148;
			rectpv.oPosition.nY = bg.oPosition.nY +25;
			rectpv.oEndPos.nZ = rectpv.oPosition.nZ = 0;
			rectpv.oEndPos.nX = rectpv.oPosition.nX + 40;
			rectpv.oEndPos.nY = rectpv.oPosition.nY + 14;
			g_pRepresentShell->DrawPrimitives(1, &rectpv, RU_T_RECT, true);
			
			KRURect	rect5;
			rect5.Color.Color_dw = 0xfffffcb2;
			rect5.oPosition.nX = bg.oPosition.nX +123;
			rect5.oPosition.nY = bg.oPosition.nY + 165;
			rect5.oEndPos.nZ = rect5.oPosition.nZ = 0;
			rect5.oEndPos.nX = rect5.oPosition.nX + 40;
			rect5.oEndPos.nY = rect5.oPosition.nY + 14;
			g_pRepresentShell->DrawPrimitives(1, &rect5, RU_T_RECT, true);
		}
		
		
		if (m_TreoMayBtn.IsButtonChecked())
		{
		KRURect	rect;
		rect.Color.Color_dw = 0xfffffcb2;
		rect.oPosition.nX = bg.oPosition.nX +140;
		rect.oPosition.nY = bg.oPosition.nY +7;
		rect.oEndPos.nZ = rect.oPosition.nZ = 0;
		rect.oEndPos.nX = bg.oEndPos.nX - 13;
		rect.oEndPos.nY = rect.oPosition.nY + 14;
		g_pRepresentShell->DrawPrimitives(1, &rect, RU_T_RECT, true);
		
		KRURect	rect1;
		rect1.Color.Color_dw = 0xfffffcb2;
		rect1.oPosition.nX = rect.oPosition.nX;
		rect1.oPosition.nY = rect.oEndPos.nY +14;
		rect1.oEndPos.nZ = rect1.oPosition.nZ = 0;
		rect1.oEndPos.nX = rect.oEndPos.nX;
		rect1.oEndPos.nY = rect1.oPosition.nY + 14;
		g_pRepresentShell->DrawPrimitives(1, &rect1, RU_T_RECT, true);
		
		KRURect	rect2;
		rect2.Color.Color_dw = 0xfffffcb2;
		rect2.oPosition.nX = rect1.oPosition.nX;
		rect2.oPosition.nY = rect1.oEndPos.nY +14;
		rect2.oEndPos.nZ = rect2.oPosition.nZ = 0;
		rect2.oEndPos.nX = rect1.oEndPos.nX;
		rect2.oEndPos.nY = rect2.oPosition.nY + 14;
		g_pRepresentShell->DrawPrimitives(1, &rect2, RU_T_RECT, true);
		
		KRURect	rect3;
		rect3.Color.Color_dw = 0xfffffcb2;
		rect3.oPosition.nX = rect2.oPosition.nX;
		rect3.oPosition.nY = rect2.oEndPos.nY +14;
		rect3.oEndPos.nZ = rect3.oPosition.nZ = 0;
		rect3.oEndPos.nX = rect2.oEndPos.nX;
		rect3.oEndPos.nY = rect3.oPosition.nY + 14;
		g_pRepresentShell->DrawPrimitives(1, &rect3, RU_T_RECT, true);
		
		}
		
		if(m_FightingBtn.IsButtonChecked())
		{
		KRURect	rect;
		rect.Color.Color_dw = 0xfffffcb2;
		rect.oPosition.nX = bg.oPosition.nX +120;
		rect.oPosition.nY = bg.oPosition.nY +7;
		rect.oEndPos.nZ = rect.oPosition.nZ = 0;
		rect.oEndPos.nX = bg.oEndPos.nX - 13;
		rect.oEndPos.nY = rect.oPosition.nY + 14;
		g_pRepresentShell->DrawPrimitives(1, &rect, RU_T_RECT, true);
		
		KRURect	rect1;
		rect1.Color.Color_dw = 0xfffffcb2;
		rect1.oPosition.nX = rect.oPosition.nX;
		rect1.oPosition.nY = rect.oEndPos.nY +6;
		rect1.oEndPos.nZ = rect1.oPosition.nZ = 0;
		rect1.oEndPos.nX = rect.oEndPos.nX;
		rect1.oEndPos.nY = rect1.oPosition.nY + 14;
		g_pRepresentShell->DrawPrimitives(1, &rect1, RU_T_RECT, true);
			KRURect	rect2;
		if (!m_FightPad.m_bTargetPlayer)
		{
			rect2.Color.Color_dw = 0xfffffcb2;
			rect2.oPosition.nX = rect1.oPosition.nX;
			rect2.oPosition.nY = rect1.oEndPos.nY +6;
			rect2.oEndPos.nZ = rect2.oPosition.nZ = 0;
			rect2.oEndPos.nX = rect1.oEndPos.nX;
			rect2.oEndPos.nY = rect2.oPosition.nY + 14;
			g_pRepresentShell->DrawPrimitives(1, &rect2, RU_T_RECT, true);
		}
		else
		{
			rect2.Color.Color_dw = 0xfffffcb2;
			rect2.oPosition.nX = rect1.oPosition.nX + 10;
			rect2.oPosition.nY = rect1.oEndPos.nY + 6;
			rect2.oEndPos.nZ = rect2.oPosition.nZ = 0;
			rect2.oEndPos.nX = rect1.oEndPos.nX - 30;
			rect2.oEndPos.nY = rect2.oPosition.nY + 14;
			g_pRepresentShell->DrawPrimitives(1, &rect2, RU_T_RECT, true);
		}
		KRURect	rect3;
		if (!m_FightPad.m_bTargetPlayer)
		{
			rect3.Color.Color_dw = 0xfffffcb2;
			rect3.oPosition.nX = rect2.oPosition.nX;
			rect3.oPosition.nY = rect2.oEndPos.nY +6;
			rect3.oEndPos.nZ = rect3.oPosition.nZ = 0;
			rect3.oEndPos.nX = rect2.oEndPos.nX;
			rect3.oEndPos.nY = rect3.oPosition.nY + 14;
			g_pRepresentShell->DrawPrimitives(1, &rect3, RU_T_RECT, true);
		}
		else
		{
			rect3.Color.Color_dw = 0xfffffcb2;
			rect3.oPosition.nX = rect1.oPosition.nX;
			rect3.oPosition.nY = rect2.oEndPos.nY + 6;
			rect3.oEndPos.nZ = rect3.oPosition.nZ = 0;
			rect3.oEndPos.nX = rect1.oEndPos.nX;
			rect3.oEndPos.nY = rect3.oPosition.nY + 14;
			g_pRepresentShell->DrawPrimitives(1, &rect3, RU_T_RECT, true);

		}
			KRURect	rect4;
			rect4.Color.Color_dw = 0xfffffcb2;
			rect4.oPosition.nX = rect3.oPosition.nX + 30;
			rect4.oPosition.nY = rect3.oEndPos.nY + 6 ;
			rect4.oEndPos.nZ = rect4.oPosition.nZ = 0;
			rect4.oEndPos.nX = rect3.oEndPos.nX;
			rect4.oEndPos.nY = rect4.oPosition.nY + 14;
			g_pRepresentShell->DrawPrimitives(1, &rect4, RU_T_RECT, true);
			

		if (!m_FightPad.m_bTargetPlayer)
		{
		
			KRURect	rect5;
			rect5.Color.Color_dw = 0xfffffcb2;
			rect5.oPosition.nX = rect3.oPosition.nX;
			rect5.oPosition.nY = rect4.oEndPos.nY + 6;
			rect5.oEndPos.nZ = rect5.oPosition.nZ = 0;
			rect5.oEndPos.nX = rect3.oEndPos.nX;
			rect5.oEndPos.nY = rect5.oPosition.nY + 14;
			g_pRepresentShell->DrawPrimitives(1, &rect5, RU_T_RECT, true);
					
			KRURect	rect6;
			rect6.Color.Color_dw = 0xfffffcb2;
			rect6.oPosition.nX = rect5.oPosition.nX;
			rect6.oPosition.nY = rect5.oEndPos.nY + 46;
			rect6.oEndPos.nZ = rect6.oPosition.nZ = 0;
			rect6.oEndPos.nX = rect5.oEndPos.nX;
			rect6.oEndPos.nY = rect6.oPosition.nY + 14;
			g_pRepresentShell->DrawPrimitives(1, &rect6, RU_T_RECT, true);		
					
			KRURect	rect7;
			rect7.Color.Color_dw = 0xfffffcb2;
			rect7.oPosition.nX = rect6.oPosition.nX;
			rect7.oPosition.nY = rect6.oEndPos.nY + 6;
			rect7.oEndPos.nZ = rect7.oPosition.nZ = 0;
			rect7.oEndPos.nX = rect6.oEndPos.nX - 15;
			rect7.oEndPos.nY = rect7.oPosition.nY + 14;
			g_pRepresentShell->DrawPrimitives(1, &rect7, RU_T_RECT, true);		
		}
		
		}
	}
}

int KUiAutoI::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WM_KEYDOWN:
		if 	(uParam == VK_SPACE)
		{
			if (m_FightPad.m_bTargetPlayer)
			{	
				m_FightPad.m_bTargetPlayer = FALSE;
				m_FightPad.m_tNThongBao.SetText("Mode: LuyÖn C«ng (SPACE)");
			}	
			else
			{
				m_FightPad.m_bTargetPlayer = TRUE;
				m_FightPad.m_tbTheoSau.SetFrame(0);
				m_FightPad.m_tbTheoSau.CheckButton(false);
				m_FightPad.m_bTheoSau = false;
				m_FightPad.m_tNThongBao.SetText("Mode: PK (SPACE)");
			}
		}
		break;
	case WND_N_BUTTON_CLICK:
//-----------------------------------------------------------------------------------
		if(uParam == (unsigned int)(KWndWindow*)&m_Close)
		{
		CloseWindow(false);
		return 0;
		}
//-----------------------------------------------------------------------------------
		else if(uParam == (unsigned int)(KWndWindow*)&m_Active || uParam == (unsigned int)(KWndWindow*)&m_TActiveText)
		{
		if (m_ActiveText)
			{
			m_ActiveText = FALSE;
			m_pSelf->m_Active.CheckButton(false);
			m_pSelf->m_Active.SetFrame(0);
			}
			else
			{
			m_ActiveText = TRUE;
			m_pSelf->m_Active.CheckButton(true);
			m_pSelf->m_Active.SetFrame(1);
			}
		return 0;
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_FightText)
		{
		m_pSelf->ActivePage(0);	
		return 0;
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_PickText)
		{
		m_pSelf->ActivePage(1);		
		return 0;
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_NhatItemText)
		{
		m_pSelf->ActivePage(2);		
		return 0;
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_TreoMayText)
		{
		m_pSelf->ActivePage(3);		
		return 0;
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_CauHinhText)
		{
		m_pSelf->ActivePage(4);		
		return 0;
		}

//-----------------------------------------------------------------------------------
		break;
	}
	return KWndPageSet::WndProc(uMsg, uParam, nParam);
}
BOOL KUiAutoI::GetIfActive()
{
	return m_pSelf->m_ActiveText;
}
KUiAutoI* KUiAutoI::ActiveAuto()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiAutoI;
		if (m_pSelf)
		{
			m_pSelf->Initialize();
			m_pSelf->m_FightPad.Show();
		//	m_pSelf->m_Active.CheckButton(TRUE);
		}
	}
	if (m_pSelf)
	{

		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	
		BOOL bAct = m_pSelf->m_ActiveText;
		if (bAct)
		{
			m_pSelf->m_Active.CheckButton(FALSE);
			m_pSelf->m_ActiveText = FALSE;
			//m_pSelf->Hide();
		}
		else
		{
			m_pSelf->m_Active.CheckButton(TRUE);
			m_pSelf->m_ActiveText = TRUE;
		}
		
	}

	return m_pSelf;
}

void KUiAutoI::BlockAuto()
{

		m_pSelf->Hide();
		m_pSelf->m_Active.CheckButton(FALSE);

}

KUiAutoI* KUiAutoI::LoadPrivateSetting(KIniFile* pFile)
{	
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiAutoI;
		if (m_pSelf)
		{
			m_pSelf->Initialize();
			m_pSelf->m_FightPad.Show();
		}
	}
	if (pFile && m_pSelf)
	{
		int HoiMau = 0;
		int HoiMana = 0;
		int ThoDiaPhu = 0;
		int PhamVi = 0;
		int KhoangCach = 0;
		int TuBoQua = 0;
		int QuanhDiem = 0;
		if ( pFile->GetInteger("AutoPlay", "HoiMau", 0, (int*)(&HoiMau)))
			m_pSelf->m_FightPad.m_eLife.SetIntText(HoiMau);
		if ( pFile->GetInteger("AutoPlay", "HoiMana", 0, (int*)(&HoiMana)))
			m_pSelf->m_FightPad.m_eMana.SetIntText(HoiMana);	
		if ( pFile->GetInteger("AutoPlay", "ThoDiaPhu", 0, (int*)(&ThoDiaPhu)))
		{
			if (ThoDiaPhu >= 500) 
			m_pSelf->m_FightPad.m_eReturn.SetIntText(500);	
			else
			m_pSelf->m_FightPad.m_eReturn.SetIntText(ThoDiaPhu);	
			
		}
		if ( pFile->GetInteger("AutoPlay", "PhamVi", 0, (int*)(&PhamVi)))
			m_pSelf->m_FightPad.m_eAtack.SetIntText(PhamVi);	
		if ( pFile->GetInteger("AutoPlay", "KhoangCach", 0, (int*)(&KhoangCach)))
			m_pSelf->m_FightPad.m_eRange.SetIntText(KhoangCach);	
		if ( pFile->GetInteger("AutoPlay", "TuBoQua", 0, (int*)(&TuBoQua)))
			m_pSelf->m_FightPad.m_eNAtack.SetIntText(TuBoQua);	
		if ( pFile->GetInteger("AutoPlay", "QuanhDiem", 0, (int*)(&QuanhDiem)))
			if (QuanhDiem == 1)
			{
				m_pSelf->m_FightPad.m_bQuanhDiem = TRUE;
			    m_pSelf->m_FightPad.m_tbQuanhDiem.CheckButton(true);
				m_pSelf->m_FightPad.m_tbQuanhDiem.SetFrame(1);
				}
		
		int LifePoison=0;
		if ( pFile->GetInteger("AutoPlay", "LifePoison", 0, (int*)(&LifePoison)))
			m_pSelf->m_TreoMayPad.m_eLifePoison.SetIntText(LifePoison);	
		int ManaPoison=0;
		if ( pFile->GetInteger("AutoPlay", "ManaPoison", 0, (int*)(&ManaPoison)))
			m_pSelf->m_TreoMayPad.m_eManaPoison.SetIntText(ManaPoison);	
		int AllPoison=0;
		if ( pFile->GetInteger("AutoPlay", "AllPoison", 0, (int*)(&AllPoison)))
			m_pSelf->m_TreoMayPad.m_eAllPoison.SetIntText(AllPoison);	
		int AllPoisonMin=0;
		if ( pFile->GetInteger("AutoPlay", "AllPoisonMin", 0, (int*)(&AllPoisonMin)))
			m_pSelf->m_TreoMayPad.m_eAllPoisonMin.SetIntText(AllPoisonMin);
		
		int NumSelect =0;
		if ( pFile->GetInteger("AutoPlay", "NumSelect", 0, (int*)(&NumSelect)))
			m_pSelf->m_NhatItemPad.m_eNumSelect.SetIntText(NumSelect);
			
	}
	return m_pSelf;
}

int KUiAutoI::SavePrivateSetting(KIniFile* pFile)
{

	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiAutoI;
		if (m_pSelf)
		{
			m_pSelf->Initialize();
			m_pSelf->m_FightPad.Show();
		}
	}
	if (pFile && m_pSelf)
	{
		int HoiMau = m_pSelf->m_FightPad.m_eLife.GetIntNumber();
		pFile->WriteInteger("AutoPlay", "HoiMau", HoiMau);
		int HoiMana = m_pSelf->m_FightPad.m_eMana.GetIntNumber();
		pFile->WriteInteger("AutoPlay", "HoiMana", HoiMana);
		int ThoDiaPhu = m_pSelf->m_FightPad.m_eReturn.GetIntNumber();
		pFile->WriteInteger("AutoPlay", "ThoDiaPhu", ThoDiaPhu);
		int PhamVi = m_pSelf->m_FightPad.m_eAtack.GetIntNumber();
		pFile->WriteInteger("AutoPlay", "PhamVi", PhamVi);
		int KhoangCach = m_pSelf->m_FightPad.m_eRange.GetIntNumber();
		pFile->WriteInteger("AutoPlay", "KhoangCach", KhoangCach);
		int TuBoQua = m_pSelf->m_FightPad.m_eNAtack.GetIntNumber();
		pFile->WriteInteger("AutoPlay", "TuBoQua", TuBoQua);
		int QuanhDiem = 0;
		if (m_pSelf->m_FightPad.m_bQuanhDiem)
		QuanhDiem = 1;
		pFile->WriteInteger("AutoPlay", "QuanhDiem", QuanhDiem);	
		int TheoSau = 0;
		
		
		
		// UiTreoMay
		
			int LifePoison = m_pSelf->m_TreoMayPad.m_eLifePoison.GetIntNumber();
			pFile->WriteInteger("AutoPlay", "LifePoison", LifePoison);
			int ManaPoison = m_pSelf->m_TreoMayPad.m_eManaPoison.GetIntNumber();
			pFile->WriteInteger("AutoPlay", "ManaPoison", ManaPoison);
			int AllPoison = m_pSelf->m_TreoMayPad.m_eAllPoison.GetIntNumber();
			pFile->WriteInteger("AutoPlay", "AllPoison", AllPoison);
			int AllPoisonMin = m_pSelf->m_TreoMayPad.m_eAllPoisonMin.GetIntNumber();
			pFile->WriteInteger("AutoPlay", "AllPoisonMin", AllPoisonMin);
			
			
			// UiNhatItem
			int NumSelect = m_pSelf->m_NhatItemPad.m_eNumSelect.GetIntNumber();
			pFile->WriteInteger("AutoPlay", "NumSelect", NumSelect);
			
	}
	return 1;
}



void KUiAutoI::Breathe()
{
	
	if(!g_pCoreShell) return;
	BOOL bAct = m_ActiveText;
	m_Active.CheckButton(bAct);
	m_FightPad.Active(bAct);
	m_FightText.BringToTop();
	m_PickText.BringToTop();
	m_NhatItemText.BringToTop();
	m_TreoMayText.BringToTop();
	m_CauHinhText.BringToTop();
//_--------------------------------------------//



	//---------------------------------------------
	KUiAuToRegion nAuto;
	nAuto.nLife = m_FightPad.m_eLife.GetIntNumber();
	nAuto.nMana = m_FightPad.m_eMana.GetIntNumber();
	nAuto.nReturn = m_FightPad.m_eReturn.GetIntNumber();
	nAuto.nAtack = m_FightPad.m_eAtack.GetIntNumber();
	nAuto.nRange = m_FightPad.m_eRange.GetIntNumber();
	nAuto.nNAtack = m_FightPad.m_eNAtack.GetIntNumber();
	nAuto.nBoQuaPK = m_FightPad.m_eBoQuaPK.GetIntNumber();
	nAuto.bStart = bAct;
	nAuto.bQuanhDiem = m_FightPad.m_bQuanhDiem;
	nAuto.bTargetPlayer = m_FightPad.m_bTargetPlayer;
	nAuto.bTheoSau = m_FightPad.m_bTheoSau;
	nAuto.bHorse = m_FightPad.m_bHorse;
	nAuto.bTuVe = m_FightPad.m_bTuVe;
	nAuto.nPhamViTuVe = m_FightPad.m_ePhamViTuVe.GetIntNumber();
	nAuto.nGuiTienNew = m_FightPad.m_eGuiTienNew.GetIntNumber();
	if (m_FightPad.m_bTargetPlayer)
	{
		m_FightPad.m_tReturn.Hide();
	//	m_FightPad.m_tAtack.Hide();
	//	m_FightPad.m_tRange.Hide();
		m_FightPad.m_tNAtack.Hide();
		m_FightPad.m_tNQuanhDiem.Hide();
		m_FightPad.m_tbQuanhDiem.Hide();
		m_FightPad.m_tNTheoSau.Hide();
		m_FightPad.m_tbTheoSau.Hide();
		m_FightPad.m_eReturn.Hide();
		m_FightPad.m_eBoQuaPK.Show();
		m_FightPad.m_tBoQuaPK.Show();
		m_FightPad.m_tBoQuaPK2.Show();
	//m_FightPad.m_eAtack.Hide();
	//	m_FightPad.m_eRange.Hide();
		m_FightPad.m_eNAtack.Hide();
		m_FightPad.m_tHorse.Hide();
		m_FightPad.m_tbHorse.Hide();
		m_FightPad.m_ePhamViTuVe.Hide();
		m_FightPad.m_tbPhamViTuVe.Hide();
		m_FightPad.m_tbTuVe.Hide();
		m_FightPad.m_tTuVe.Hide();
		m_FightPad.m_tbHorse.Hide();
	}
	else
	{
		m_FightPad.m_tReturn.Show();
		m_FightPad.m_eBoQuaPK.Hide();
		m_FightPad.m_tBoQuaPK.Hide();
		m_FightPad.m_tBoQuaPK2.Hide();
		
		
	//	m_FightPad.m_tAtack.Show();
	//	m_FightPad.m_tRange.Show();
		m_FightPad.m_tNAtack.Show();
		m_FightPad.m_tNQuanhDiem.Show();
		m_FightPad.m_tbQuanhDiem.Show();
		m_FightPad.m_tNTheoSau.Show();
		m_FightPad.m_tbTheoSau.Show();
		m_FightPad.m_eReturn.Show();
	//	m_FightPad.m_eAtack.Show();
	//	m_FightPad.m_eRange.Show();
		m_FightPad.m_eNAtack.Show();
		m_FightPad.m_tHorse.Show();
		m_FightPad.m_tbHorse.Show();
		m_FightPad.m_ePhamViTuVe.Show();
		m_FightPad.m_tbTuVe.Show();
		m_FightPad.m_tTuVe.Show();
		m_FightPad.m_tbPhamViTuVe.Show();
	}
		
		
	// Ho Tro
	nAuto.nNgaMyBuff = m_PickPad.m_eNgaMyBuff.GetIntNumber();
	nAuto.bInviteAuto = m_PickPad.m_bInviteAuto;
	nAuto.bAcceptAuto = m_PickPad.m_bAcceptAuto;
	nAuto.bCancelAuto = m_PickPad.m_bCancelAuto;
	
	

	
	
	nAuto.bUseTDP = m_TreoMayPad.m_bBanTrangBi;
	
	int num = 0;
	if (m_PickPad.m_bMongDiep)
	num++;
	if (m_PickPad.m_bLuuThuy)
	num++;
	if (m_PickPad.m_bPhatTam)
	num++;
	if (m_PickPad.m_bThanhAm)
	num++;
	if (m_PickPad.m_bPhoDo)
	num++;
	
	if (num > 2)
	{
		m_PickPad.m_tbMongDiep.SetFrame(0);
		m_PickPad.m_tbMongDiep.CheckButton(false);
		m_PickPad.m_bMongDiep = false;
		m_PickPad.m_tbLuuThuy.SetFrame(0);
		m_PickPad.m_tbLuuThuy.CheckButton(false);
		m_PickPad.m_bLuuThuy = false;
		m_PickPad.m_tbPhatTam.SetFrame(0);
		m_PickPad.m_tbPhatTam.CheckButton(false);
		m_PickPad.m_bPhatTam = false;
		m_PickPad.m_tbThanhAm.SetFrame(0);
		m_PickPad.m_tbThanhAm.CheckButton(false);
		m_PickPad.m_bThanhAm = false;
		m_PickPad.m_tbPhoDo.SetFrame(0);
		m_PickPad.m_tbPhoDo.CheckButton(false);
		m_PickPad.m_bPhoDo = false;
	}
	
	nAuto.bNgaMy1 = m_PickPad.m_bMongDiep;
	nAuto.bNgaMy2 = m_PickPad.m_bLuuThuy;
	nAuto.bNgaMy3 = m_PickPad.m_bPhatTam;
	nAuto.bNgaMy4 = m_PickPad.m_bThanhAm;
	nAuto.bNgaMy5 = m_PickPad.m_bPhoDo;
	
	nAuto.bHideMissle = m_CauHinhPad.m_bHideMissle;
	nAuto.bHidePlayer = m_CauHinhPad.m_bHidePlayer;
	nAuto.bHidePlayer1 = m_CauHinhPad.m_bHidePlayer1;
	nAuto.bHidePlayer2 = m_CauHinhPad.m_bHidePlayer2;
	nAuto.eHidePlayer3 = m_CauHinhPad.m_eHidePlayer3.GetIntNumber();
	
	nAuto.bHideMissle2 = m_CauHinhPad.m_bHideMissle2;
	nAuto.bHideGame1 = m_CauHinhPad.m_bHideGame1;
	nAuto.bHideGame2 = m_CauHinhPad.m_bHideGame2;
	nAuto.bHideGame3 = m_CauHinhPad.m_bHideGame3;
	
	
	// Nhat Item
	nAuto.bMoneyObj = m_NhatItemPad.m_bMoneyObj;
	nAuto.bItemObj = m_NhatItemPad.m_bItemObj;
	nAuto.bPropObj = m_NhatItemPad.m_bPropObj;
	nAuto.bItemSelect = m_NhatItemPad.m_bItemSelect;
	
	g_pCoreShell->GetGameData(GDI_ATI_SETTING, 0, (int)&nAuto);
	
			KUiAuToTreoMay pInfoTM;
			
			if (m_TreoMayPad.m_eLifePoison.GetIntNumber() > 10)
			m_TreoMayPad.m_eLifePoison.SetIntText(10);
			if (m_TreoMayPad.m_eManaPoison.GetIntNumber() > 10)
			m_TreoMayPad.m_eManaPoison.SetIntText(10);
			if (m_TreoMayPad.m_eAllPoison.GetIntNumber() > 20)
			m_TreoMayPad.m_eAllPoison.SetIntText(20);
			if (m_TreoMayPad.m_eAllPoisonMin.GetIntNumber() > 20)
			m_TreoMayPad.m_eAllPoisonMin.SetIntText(20);
		
		
			pInfoTM.LifePoison = m_TreoMayPad.m_eLifePoison.GetIntNumber();
			pInfoTM.ManaPoison = m_TreoMayPad.m_eManaPoison.GetIntNumber();
			pInfoTM.AllPoison = m_TreoMayPad.m_eAllPoison.GetIntNumber();
			pInfoTM.AllPoisonMin = m_TreoMayPad.m_eAllPoisonMin.GetIntNumber();
			pInfoTM.bSuaTrangBi = m_TreoMayPad.m_bSuaTrangBi;
			pInfoTM.bUseTTL = m_TreoMayPad.m_bGuiTien;
			pInfoTM.bQuayLai = m_TreoMayPad.m_bQuayLai;
			pInfoTM.bNhatDoChonLoc = m_NhatItemPad.m_bItemSelect;
			pInfoTM.NumSelect = m_NhatItemPad.m_eNumSelect.GetIntNumber();
			pInfoTM.RingSelect = m_NhatItemPad.m_bRingSelect;
			pInfoTM.WeaSelect = m_NhatItemPad.m_bWeaSelect;
	
	
			g_pCoreShell->OperationRequest(GOI_AUTOQUAYLAI, (int)&pInfoTM, 0);
		if (m_CauHinhPad.m_eFPS.GetIntNumber() >= 200)
			m_CauHinhPad.m_eFPS.SetIntText(200);
		if (m_CauHinhPad.m_eFPS.GetIntNumber() > 0 && m_CauHinhPad.m_eFPS.GetIntNumber() <= 200)
			g_pRepresentShell->FPSDelay = m_CauHinhPad.m_eFPS.GetIntNumber();

}
