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


#define 	SCHEME_INI_AUTO_SHEET			"\\Ui\\Ui3\\AutoFilterEquip.ini"
//--------------------------------------------------------------------------
//	Chuc nang: Tu chien dau
//--------------------------------------------------------------------------


void KUiFighting1::Initialize()
{
	AddChild(&m_Close);
	
	AddChild(&m_tText1);
	AddChild(&m_tText2);
	AddChild(&m_tText3);
	AddChild(&m_tText4);
	AddChild(&m_tText5);
	AddChild(&m_tText6);
	AddChild(&m_tText7);
	AddChild(&m_tText8);
	AddChild(&m_tText9);
	AddChild(&m_tText10);
	AddChild(&m_tText11);
	AddChild(&m_tText12);
	AddChild(&m_tText13);
	AddChild(&m_tText14);
	AddChild(&m_tText15);
	AddChild(&m_tText16);
	AddChild(&m_tText17);
	AddChild(&m_tText18);
	AddChild(&m_tText19);
	AddChild(&m_tText20);
	AddChild(&m_tText21);
	AddChild(&m_tText22);
	AddChild(&m_tText23);
	AddChild(&m_tText24);
	AddChild(&m_tText25);
	AddChild(&m_tText26);
	AddChild(&m_tText27);
	AddChild(&m_tText28);
	AddChild(&m_tText29);
	AddChild(&m_tText30);
	AddChild(&m_tText31);
	AddChild(&m_tText32);
	AddChild(&m_tText33);
	AddChild(&m_tText34);
	AddChild(&m_tText35);
	AddChild(&m_tText36);
	AddChild(&m_tText37);
	AddChild(&m_tText38);
	AddChild(&m_tText39);
	AddChild(&m_tText40);
	AddChild(&m_tText41);
	
	AddChild(&m_tEdit1);
	AddChild(&m_tEdit2);
	AddChild(&m_tEdit3);
	AddChild(&m_tEdit4);
	AddChild(&m_tEdit5);
	AddChild(&m_tEdit6);
	AddChild(&m_tEdit7);
	AddChild(&m_tEdit8);
	AddChild(&m_tEdit9);
	AddChild(&m_tEdit10);
	AddChild(&m_tEdit11);
	AddChild(&m_tEdit12);
	AddChild(&m_tEdit13);
	AddChild(&m_tEdit14);
	AddChild(&m_tEdit15);
	AddChild(&m_tEdit16);
	AddChild(&m_tEdit17);
	AddChild(&m_tEdit18);
	AddChild(&m_tEdit19);
	AddChild(&m_tEdit20);
	AddChild(&m_tEdit21);
	AddChild(&m_tEdit22);
	AddChild(&m_tEdit23);
	AddChild(&m_tEdit24);
	AddChild(&m_tEdit25);
	AddChild(&m_tEdit26);
	AddChild(&m_tEdit27);
	AddChild(&m_tEdit28);
	AddChild(&m_tEdit29);
	AddChild(&m_tEdit30);
	AddChild(&m_tEdit31);
	AddChild(&m_tEdit32);
	AddChild(&m_tEdit33);
	AddChild(&m_tEdit34);
	AddChild(&m_tEdit35);
	AddChild(&m_tEdit36);
	AddChild(&m_tEdit37);
	AddChild(&m_tEdit38);
	AddChild(&m_tEdit39);
	AddChild(&m_tEdit40);
	AddChild(&m_tEdit41);

	Wnd_AddWindow(this);
}
//Cai` dat. file .ini
void KUiFighting1::LoadScheme()
{
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO_SHEET))
	{
		KWndPage::Init(&Ini, "Fighting");
		
		m_Close.Init(&Ini, "CloseBtn");
		m_tText1.Init(&Ini, "T1");
		m_tText2.Init(&Ini, "T2");
		m_tText3.Init(&Ini, "T3");
		m_tText4.Init(&Ini, "T4");
		m_tText5.Init(&Ini, "T5");
		m_tText6.Init(&Ini, "T6");
		m_tText7.Init(&Ini, "T7");
		m_tText8.Init(&Ini, "T8");
		m_tText9.Init(&Ini, "T9");
		m_tText10.Init(&Ini, "T10");
		m_tText11.Init(&Ini, "T11");
		m_tText12.Init(&Ini, "T12");
		m_tText13.Init(&Ini, "T13");
		m_tText14.Init(&Ini, "T14");
		m_tText15.Init(&Ini, "T15");
		m_tText16.Init(&Ini, "T16");
		m_tText17.Init(&Ini, "T17");
		m_tText18.Init(&Ini, "T18");
		m_tText19.Init(&Ini, "T19");
		m_tText20.Init(&Ini, "T20");
		m_tText21.Init(&Ini, "T21");
		m_tText22.Init(&Ini, "T22");
		m_tText23.Init(&Ini, "T23");
		m_tText24.Init(&Ini, "T24");
		m_tText25.Init(&Ini, "T25");
		m_tText26.Init(&Ini, "T26");
		m_tText27.Init(&Ini, "T27");
		m_tText28.Init(&Ini, "T28");
		m_tText29.Init(&Ini, "T29");
		m_tText30.Init(&Ini, "T30");
		m_tText31.Init(&Ini, "T31");
		m_tText32.Init(&Ini, "T32");
		m_tText33.Init(&Ini, "T33");
		m_tText34.Init(&Ini, "T34");
		m_tText35.Init(&Ini, "T35");
		m_tText36.Init(&Ini, "T36");
		m_tText37.Init(&Ini, "T37");
		m_tText38.Init(&Ini, "T38");
		m_tText39.Init(&Ini, "T39");	
			
			
			
		m_tEdit1.Init(&Ini, "E1");
		m_tEdit2.Init(&Ini, "E2");
		m_tEdit3.Init(&Ini, "E3");
		m_tEdit4.Init(&Ini, "E4");
		m_tEdit5.Init(&Ini, "E5");
		m_tEdit6.Init(&Ini, "E6");
		m_tEdit7.Init(&Ini, "E7");
		m_tEdit8.Init(&Ini, "E8");
		m_tEdit9.Init(&Ini, "E9");
		m_tEdit10.Init(&Ini, "E10");
		m_tEdit11.Init(&Ini, "E11");
		m_tEdit12.Init(&Ini, "E12");
		m_tEdit13.Init(&Ini, "E13");
		m_tEdit14.Init(&Ini, "E14");
		m_tEdit15.Init(&Ini, "E15");
		m_tEdit16.Init(&Ini, "E16");
		m_tEdit17.Init(&Ini, "E17");
		m_tEdit18.Init(&Ini, "E18");
		m_tEdit19.Init(&Ini, "E19");
		m_tEdit20.Init(&Ini, "E20");		
		m_tEdit21.Init(&Ini, "E21");
		m_tEdit22.Init(&Ini, "E22");
		m_tEdit23.Init(&Ini, "E23");
		m_tEdit24.Init(&Ini, "E24");
		m_tEdit25.Init(&Ini, "E25");
		m_tEdit26.Init(&Ini, "E26");
		m_tEdit27.Init(&Ini, "E27");
		m_tEdit28.Init(&Ini, "E28");
		m_tEdit29.Init(&Ini, "E29");
		m_tEdit30.Init(&Ini, "E30");		
		m_tEdit31.Init(&Ini, "E31");
		m_tEdit32.Init(&Ini, "E32");
		m_tEdit33.Init(&Ini, "E33");
		m_tEdit34.Init(&Ini, "E34");
		m_tEdit35.Init(&Ini, "E35");
		m_tEdit36.Init(&Ini, "E36");
		m_tEdit37.Init(&Ini, "E37");
		m_tEdit38.Init(&Ini, "E38");
		m_tEdit39.Init(&Ini, "E39");
		
		
		
		
		m_tText1.SetText("Sinh lùc tèi ®a (200)        >=  ");
		m_tText2.SetText("Néi lùc tèi ®a (200)         >=  ");
		m_tText3.SetText("ThÓ lùc tèi ®a (200)         >=  ");
		m_tText4.SetText("Kh¸ng ®éc (25)               >=  ");
		m_tText5.SetText("Kh¸ng háa (25)               >=  ");
		m_tText6.SetText("Kh¸ng l«i (30)               >=  ");
		m_tText7.SetText("Phäng thñ vËt lý (25)        >= ");
		m_tText8.SetText("Kh¸ng b¨ng (25)              >=  ");
		m_tText9.SetText("Thêi gian lµm chËm (40)      >=  ");
		m_tText10.SetText("Thêi gian tróng ®éc (40)    >=  ");
		m_tText11.SetText("Thêi gian cho¸ng (40)       >=  ");
		m_tText12.SetText("Tèc ®é di chuyÓn (40)       >=  ");
		m_tText13.SetText("Thêi gian phôc håi (40)     >=  ");
		m_tText14.SetText("Kh¸ng tÊt c¶ (20)           >=  ");
		m_tText15.SetText("Søc m¹nh (20)               >=  ");
		m_tText16.SetText("Th©n ph¸p (20)              >=  ");
		m_tText17.SetText("Sinh khÝ (20)               >=  ");
		m_tText18.SetText("Bá qua nÐ tr¸nh             >=  ");
		m_tText19.SetText("§é chÝnh x¸c                >=  ");
		m_tText20.SetText("NÐ tr¸nh                    >=  ");
		
		m_tText21.SetText("Tèc ®é ®¸nh ngo¹i c«ng (30) >=  ");
		m_tText22.SetText("STVL ngo¹i «cng (®iÓm) (50) >=  ");
		m_tText23.SetText("Háa s¸t ngo¹i c«ng (100)    >=  ");
		m_tText24.SetText("B¨ng s¸t ngo¹i c«ng (100)   >=  ");
		m_tText25.SetText("L«i s¸t ngo¹i c«ng (100)    >=  ");
		m_tText26.SetText("§éc s¸t ngo¹i c«ng (50)    >=  ");
		m_tText27.SetText("STVL ngo¹i c«ng (%) (100)   >=  ");
		m_tText28.SetText("ChuyÓn hãa ST thµnh NL (15) >=  ");
		m_tText29.SetText("May m¾n (10)                >=  ");
		m_tText30.SetText("Hót sinh lùc (10)           >=  ");
		m_tText31.SetText("Hót néi lùc (10)            >=  ");
		m_tText32.SetText("STVL néi c«ng (®iÓm) (200)  >=  ");
		m_tText33.SetText("§éc s¸t néi c«ng (100)      >=  ");
		m_tText34.SetText("B¨ng s¸t néi c«ng (200)     >=  ");
		m_tText35.SetText("Háa s¸t néi c«ng (200)      >=  ");
		m_tText36.SetText("L«i s¸t néi c«ng (200)      >=  ");
		m_tText37.SetText("Phôc håi thÓ lùc (10)       >=  ");
		m_tText38.SetText("Phôc håi sinh lùc (10)      >=  ");
		m_tText39.SetText("Phôc håi néi lùc (10)       >=  ");
		
		
		
		m_tEdit1.SetIntText(0);
		m_tEdit2.SetIntText(0);
		m_tEdit3.SetIntText(0);
		m_tEdit4.SetIntText(0);
		m_tEdit5.SetIntText(0);
		m_tEdit6.SetIntText(0);
		m_tEdit7.SetIntText(0);
		m_tEdit8.SetIntText(0);
		m_tEdit9.SetIntText(0);
		m_tEdit10.SetIntText(0);
		m_tEdit11.SetIntText(0);
		m_tEdit12.SetIntText(0);
		m_tEdit13.SetIntText(0);
		m_tEdit14.SetIntText(0);
		m_tEdit15.SetIntText(0);
		m_tEdit16.SetIntText(0);
		m_tEdit17.SetIntText(0);
		m_tEdit18.SetIntText(0);
		m_tEdit19.SetIntText(0);
		m_tEdit20.SetIntText(0);			
		m_tEdit21.SetIntText(0);
		m_tEdit22.SetIntText(0);
		m_tEdit23.SetIntText(0);
		m_tEdit24.SetIntText(0);
		m_tEdit25.SetIntText(0);
		m_tEdit26.SetIntText(0);
		m_tEdit27.SetIntText(0);
		m_tEdit28.SetIntText(0);
		m_tEdit29.SetIntText(0);
		m_tEdit30.SetIntText(0);	
		m_tEdit31.SetIntText(0);
		m_tEdit32.SetIntText(0);
		m_tEdit33.SetIntText(0);
		m_tEdit34.SetIntText(0);
		m_tEdit35.SetIntText(0);
		m_tEdit36.SetIntText(0);
		m_tEdit37.SetIntText(0);
		m_tEdit38.SetIntText(0);
		m_tEdit39.SetIntText(0);
		
	}
	
}
//Ham` chay. lien tuc. (nhu hoi tho~)
void KUiFighting1::Breathe()
{
		
		
		
}
// k¡§ach hoat
void KUiFighting1::Active(BOOL bAct)
{
	m_bIsActived = bAct;
}

int KUiFighting1::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_Close)
		{
			// Send Du Lieu Cho Server
		
			KUiTeamAuto::CloseWindow(0);
		}
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

KUiTeamAuto* KUiTeamAuto::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	chuc nang: mo cua~ so~ sheet chinh'
//--------------------------------------------------------------------------
KUiTeamAuto* KUiTeamAuto::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTeamAuto;
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
		
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	chuc nang: kiem tra co' an~ hay khong
//--------------------------------------------------------------------------
KUiTeamAuto* KUiTeamAuto::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	chuc nang: dong lai
//--------------------------------------------------------------------------
void KUiTeamAuto::CloseWindow(bool bDestroy)
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
void KUiTeamAuto::Initialize()
{
	m_FightPad.Initialize();
	AddPage(&m_FightPad, &m_FightingBtn);
		
	Wnd_AddWindow(this);
	LoadScheme();
}

//--------------------------------------------------------------------------
//	chuc nang: khoi tao theo file .ini
//--------------------------------------------------------------------------
void KUiTeamAuto::LoadScheme()
{
	KIniFile	Ini;
	if (m_pSelf && Ini.Load(SCHEME_INI_AUTO_SHEET))
	{
		m_pSelf->Init(&Ini, "Main");

		m_pSelf->m_Active.Init(&Ini, "ActiveBtn");
		m_pSelf->m_TActiveText.Init(&Ini, "ActiveText");

		m_pSelf->m_FightingBtn.Init(&Ini, "FightingBtn");
		m_pSelf->m_FightText.Init(&Ini, "FightText");

		m_pSelf->m_Active.SetFrame(0);
		m_pSelf->m_Active.CheckButton(false);
		m_pSelf->m_ActiveText = false;

		m_pSelf->m_FightPad.LoadScheme();
		

	}
}
//Paint my love :)
//Ham` ve~
void KUiTeamAuto::PaintWindow()
{
	KWndPageSet::PaintWindow();
	if (g_pRepresentShell)
	{
		KRUShadow	bg;
		GetAbsolutePos(&bg.oPosition.nX, &bg.oPosition.nY);
		bg.oPosition.nX += 0;
		bg.oPosition.nY += 16;
		bg.Color.Color_dw = 0x16000000;
		GetSize(&bg.oEndPos.nX, &bg.oEndPos.nY);
		bg.oEndPos.nX += bg.oPosition.nX;
		bg.oEndPos.nY += bg.oPosition.nY;
		g_pRepresentShell->DrawPrimitives(1, &bg, RU_T_SHADOW, true);

		if(m_FightingBtn.IsButtonChecked())
		{
				KRURect	rect;
				rect.Color.Color_dw = 0xfffffcb2;
				rect.oPosition.nX = bg.oPosition.nX +240;
				rect.oPosition.nY = bg.oPosition.nY +10;
				rect.oEndPos.nZ = rect.oPosition.nZ = 0;
				rect.oEndPos.nX = rect.oPosition.nX + 40;
				rect.oEndPos.nY = rect.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect, RU_T_RECT, true);
		
				KRURect	rect1;
				rect1.Color.Color_dw = 0xfffffcb2;
				rect1.oPosition.nX = rect.oPosition.nX;
				rect1.oPosition.nY = rect.oEndPos.nY + 6;
				rect1.oEndPos.nZ = rect1.oPosition.nZ = 0;
				rect1.oEndPos.nX = rect.oEndPos.nX;
				rect1.oEndPos.nY = rect1.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect1, RU_T_RECT, true);
		
				KRURect	rect2;
				rect2.Color.Color_dw = 0xfffffcb2;
				rect2.oPosition.nX = rect1.oPosition.nX;
				rect2.oPosition.nY = rect1.oEndPos.nY + 6;
				rect2.oEndPos.nZ = rect2.oPosition.nZ = 0;
				rect2.oEndPos.nX = rect1.oEndPos.nX;
				rect2.oEndPos.nY = rect2.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect2, RU_T_RECT, true);
				
				KRURect	rect3;
				rect3.Color.Color_dw = 0xfffffcb2;
				rect3.oPosition.nX = rect2.oPosition.nX;
				rect3.oPosition.nY = rect2.oEndPos.nY + 6;
				rect3.oEndPos.nZ = rect3.oPosition.nZ = 0;
				rect3.oEndPos.nX = rect2.oEndPos.nX;
				rect3.oEndPos.nY = rect3.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect3, RU_T_RECT, true);
		
				KRURect	rect4;
				rect4.Color.Color_dw = 0xfffffcb2;
				rect4.oPosition.nX = rect3.oPosition.nX;
				rect4.oPosition.nY = rect3.oEndPos.nY + 6;
				rect4.oEndPos.nZ = rect4.oPosition.nZ = 0;
				rect4.oEndPos.nX = rect3.oEndPos.nX;
				rect4.oEndPos.nY = rect4.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect4, RU_T_RECT, true);
		
				KRURect	rect5;
				rect5.Color.Color_dw = 0xfffffcb2;
				rect5.oPosition.nX = rect4.oPosition.nX;
				rect5.oPosition.nY = rect4.oEndPos.nY + 6;
				rect5.oEndPos.nZ = rect5.oPosition.nZ = 0;
				rect5.oEndPos.nX = rect4.oEndPos.nX;
				rect5.oEndPos.nY = rect5.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect5, RU_T_RECT, true);
			
				KRURect	rect6;
				rect6.Color.Color_dw = 0xfffffcb2;
				rect6.oPosition.nX = rect5.oPosition.nX;
				rect6.oPosition.nY = rect5.oEndPos.nY + 6;
				rect6.oEndPos.nZ = rect6.oPosition.nZ = 0;
				rect6.oEndPos.nX = rect5.oEndPos.nX;
				rect6.oEndPos.nY = rect6.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect6, RU_T_RECT, true);
		
				KRURect	rect7;
				rect7.Color.Color_dw = 0xfffffcb2;
				rect7.oPosition.nX = rect6.oPosition.nX;
				rect7.oPosition.nY = rect6.oEndPos.nY + 6;
				rect7.oEndPos.nZ = rect7.oPosition.nZ = 0;
				rect7.oEndPos.nX = rect6.oEndPos.nX;
				rect7.oEndPos.nY = rect7.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect7, RU_T_RECT, true);
		
				KRURect	rect8;
				rect8.Color.Color_dw = 0xfffffcb2;
				rect8.oPosition.nX = rect7.oPosition.nX;
				rect8.oPosition.nY = rect7.oEndPos.nY + 6;
				rect8.oEndPos.nZ = rect8.oPosition.nZ = 0;
				rect8.oEndPos.nX = rect7.oEndPos.nX;
				rect8.oEndPos.nY = rect8.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect8, RU_T_RECT, true);
		
				KRURect	rect9;
				rect9.Color.Color_dw = 0xfffffcb2;
				rect9.oPosition.nX = rect8.oPosition.nX;
				rect9.oPosition.nY = rect8.oEndPos.nY + 6;
				rect9.oEndPos.nZ = rect9.oPosition.nZ = 0;
				rect9.oEndPos.nX = rect8.oEndPos.nX;
				rect9.oEndPos.nY = rect9.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect9, RU_T_RECT, true);
		
				KRURect	rect10;
				rect10.Color.Color_dw = 0xfffffcb2;
				rect10.oPosition.nX = rect9.oPosition.nX;
				rect10.oPosition.nY = rect9.oEndPos.nY + 6;
				rect10.oEndPos.nZ = rect10.oPosition.nZ = 0;
				rect10.oEndPos.nX = rect9.oEndPos.nX;
				rect10.oEndPos.nY = rect10.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect10, RU_T_RECT, true);
		
				KRURect	rect11;
				rect11.Color.Color_dw = 0xfffffcb2;
				rect11.oPosition.nX = rect10.oPosition.nX;
				rect11.oPosition.nY = rect10.oEndPos.nY + 6;
				rect11.oEndPos.nZ = rect11.oPosition.nZ = 0;
				rect11.oEndPos.nX = rect10.oEndPos.nX;
				rect11.oEndPos.nY = rect11.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect11, RU_T_RECT, true);
		
				KRURect	rect12;
				rect12.Color.Color_dw = 0xfffffcb2;
				rect12.oPosition.nX = rect11.oPosition.nX;
				rect12.oPosition.nY = rect11.oEndPos.nY + 6;
				rect12.oEndPos.nZ = rect12.oPosition.nZ = 0;
				rect12.oEndPos.nX = rect11.oEndPos.nX;
				rect12.oEndPos.nY = rect12.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect12, RU_T_RECT, true);
		
				KRURect	rect13;
				rect13.Color.Color_dw = 0xfffffcb2;
				rect13.oPosition.nX = rect12.oPosition.nX;
				rect13.oPosition.nY = rect12.oEndPos.nY + 6;
				rect13.oEndPos.nZ = rect13.oPosition.nZ = 0;
				rect13.oEndPos.nX = rect12.oEndPos.nX;
				rect13.oEndPos.nY = rect13.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect13, RU_T_RECT, true);
		
				KRURect	rect14;
				rect14.Color.Color_dw = 0xfffffcb2;
				rect14.oPosition.nX = rect13.oPosition.nX;
				rect14.oPosition.nY = rect13.oEndPos.nY + 6;
				rect14.oEndPos.nZ = rect14.oPosition.nZ = 0;
				rect14.oEndPos.nX = rect13.oEndPos.nX;
				rect14.oEndPos.nY = rect14.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect14, RU_T_RECT, true);
		
				KRURect	rect15;
				rect15.Color.Color_dw = 0xfffffcb2;
				rect15.oPosition.nX = rect14.oPosition.nX;
				rect15.oPosition.nY = rect14.oEndPos.nY + 6;
				rect15.oEndPos.nZ = rect15.oPosition.nZ = 0;
				rect15.oEndPos.nX = rect14.oEndPos.nX;
				rect15.oEndPos.nY = rect15.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect15, RU_T_RECT, true);
		
				KRURect	rect16;
				rect16.Color.Color_dw = 0xfffffcb2;
				rect16.oPosition.nX = rect15.oPosition.nX;
				rect16.oPosition.nY = rect15.oEndPos.nY + 6;
				rect16.oEndPos.nZ = rect16.oPosition.nZ = 0;
				rect16.oEndPos.nX = rect15.oEndPos.nX;
				rect16.oEndPos.nY = rect16.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect16, RU_T_RECT, true);
		
				KRURect	rect17;
				rect17.Color.Color_dw = 0xfffffcb2;
				rect17.oPosition.nX = rect16.oPosition.nX;
				rect17.oPosition.nY = rect16.oEndPos.nY + 6;
				rect17.oEndPos.nZ = rect17.oPosition.nZ = 0;
				rect17.oEndPos.nX = rect16.oEndPos.nX;
				rect17.oEndPos.nY = rect17.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect17, RU_T_RECT, true);
		
				KRURect	rect18;
				rect18.Color.Color_dw = 0xfffffcb2;
				rect18.oPosition.nX = rect17.oPosition.nX;
				rect18.oPosition.nY = rect17.oEndPos.nY + 6;
				rect18.oEndPos.nZ = rect18.oPosition.nZ = 0;
				rect18.oEndPos.nX = rect17.oEndPos.nX;
				rect18.oEndPos.nY = rect18.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect18, RU_T_RECT, true);
		
				KRURect	rect19;
				rect19.Color.Color_dw = 0xfffffcb2;
				rect19.oPosition.nX = rect18.oPosition.nX;
				rect19.oPosition.nY = rect18.oEndPos.nY + 6;
				rect19.oEndPos.nZ = rect19.oPosition.nZ = 0;
				rect19.oEndPos.nX = rect18.oEndPos.nX;
				rect19.oEndPos.nY = rect19.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect19, RU_T_RECT, true);
		
		
				KRURect	rect20;
				rect20.Color.Color_dw = 0xfffffcb2;
				rect20.oPosition.nX = bg.oPosition.nX +510;
				rect20.oPosition.nY = bg.oPosition.nY +10;
				rect20.oEndPos.nZ = rect20.oPosition.nZ = 0;
				rect20.oEndPos.nX = rect20.oPosition.nX + 40;
				rect20.oEndPos.nY = rect20.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect20, RU_T_RECT, true);
		
				KRURect	rect21;
				rect21.Color.Color_dw = 0xfffffcb2;
				rect21.oPosition.nX = rect20.oPosition.nX;
				rect21.oPosition.nY = rect20.oEndPos.nY + 6;
				rect21.oEndPos.nZ = rect21.oPosition.nZ = 0;
				rect21.oEndPos.nX = rect20.oEndPos.nX;
				rect21.oEndPos.nY = rect21.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect21, RU_T_RECT, true);
		
				KRURect	rect22;
				rect22.Color.Color_dw = 0xfffffcb2;
				rect22.oPosition.nX = rect21.oPosition.nX;
				rect22.oPosition.nY = rect21.oEndPos.nY + 6;
				rect22.oEndPos.nZ = rect22.oPosition.nZ = 0;
				rect22.oEndPos.nX = rect21.oEndPos.nX;
				rect22.oEndPos.nY = rect22.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect22, RU_T_RECT, true);
				
				KRURect	rect23;
				rect23.Color.Color_dw = 0xfffffcb2;
				rect23.oPosition.nX = rect22.oPosition.nX;
				rect23.oPosition.nY = rect22.oEndPos.nY + 6;
				rect23.oEndPos.nZ = rect23.oPosition.nZ = 0;
				rect23.oEndPos.nX = rect22.oEndPos.nX;
				rect23.oEndPos.nY = rect23.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect23, RU_T_RECT, true);
		
				KRURect	rect24;
				rect24.Color.Color_dw = 0xfffffcb2;
				rect24.oPosition.nX = rect23.oPosition.nX;
				rect24.oPosition.nY = rect23.oEndPos.nY + 6;
				rect24.oEndPos.nZ = rect24.oPosition.nZ = 0;
				rect24.oEndPos.nX = rect23.oEndPos.nX;
				rect24.oEndPos.nY = rect24.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect24, RU_T_RECT, true);
		
				KRURect	rect25;
				rect25.Color.Color_dw = 0xfffffcb2;
				rect25.oPosition.nX = rect24.oPosition.nX;
				rect25.oPosition.nY = rect24.oEndPos.nY + 6;
				rect25.oEndPos.nZ = rect25.oPosition.nZ = 0;
				rect25.oEndPos.nX = rect24.oEndPos.nX;
				rect25.oEndPos.nY = rect25.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect25, RU_T_RECT, true);
			
				KRURect	rect26;
				rect26.Color.Color_dw = 0xfffffcb2;
				rect26.oPosition.nX = rect25.oPosition.nX;
				rect26.oPosition.nY = rect25.oEndPos.nY + 6;
				rect26.oEndPos.nZ = rect26.oPosition.nZ = 0;
				rect26.oEndPos.nX = rect25.oEndPos.nX;
				rect26.oEndPos.nY = rect26.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect26, RU_T_RECT, true);
		
				KRURect	rect27;
				rect27.Color.Color_dw = 0xfffffcb2;
				rect27.oPosition.nX = rect26.oPosition.nX;
				rect27.oPosition.nY = rect26.oEndPos.nY + 6;
				rect27.oEndPos.nZ = rect27.oPosition.nZ = 0;
				rect27.oEndPos.nX = rect26.oEndPos.nX;
				rect27.oEndPos.nY = rect27.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect27, RU_T_RECT, true);
		
				KRURect	rect28;
				rect28.Color.Color_dw = 0xfffffcb2;
				rect28.oPosition.nX = rect27.oPosition.nX;
				rect28.oPosition.nY = rect27.oEndPos.nY + 6;
				rect28.oEndPos.nZ = rect28.oPosition.nZ = 0;
				rect28.oEndPos.nX = rect27.oEndPos.nX;
				rect28.oEndPos.nY = rect28.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect28, RU_T_RECT, true);
		
				KRURect	rect29;
				rect29.Color.Color_dw = 0xfffffcb2;
				rect29.oPosition.nX = rect28.oPosition.nX;
				rect29.oPosition.nY = rect28.oEndPos.nY + 6;
				rect29.oEndPos.nZ = rect29.oPosition.nZ = 0;
				rect29.oEndPos.nX = rect28.oEndPos.nX;
				rect29.oEndPos.nY = rect29.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect29, RU_T_RECT, true);
		
				KRURect	rect30;
				rect30.Color.Color_dw = 0xfffffcb2;
				rect30.oPosition.nX = rect29.oPosition.nX;
				rect30.oPosition.nY = rect29.oEndPos.nY + 6;
				rect30.oEndPos.nZ = rect30.oPosition.nZ = 0;
				rect30.oEndPos.nX = rect29.oEndPos.nX;
				rect30.oEndPos.nY = rect30.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect30, RU_T_RECT, true);
		
				KRURect	rect31;
				rect31.Color.Color_dw = 0xfffffcb2;
				rect31.oPosition.nX = rect30.oPosition.nX;
				rect31.oPosition.nY = rect30.oEndPos.nY + 6;
				rect31.oEndPos.nZ = rect31.oPosition.nZ = 0;
				rect31.oEndPos.nX = rect30.oEndPos.nX;
				rect31.oEndPos.nY = rect31.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect31, RU_T_RECT, true);
		
				KRURect	rect32;
				rect32.Color.Color_dw = 0xfffffcb2;
				rect32.oPosition.nX = rect31.oPosition.nX;
				rect32.oPosition.nY = rect31.oEndPos.nY + 6;
				rect32.oEndPos.nZ = rect32.oPosition.nZ = 0;
				rect32.oEndPos.nX = rect31.oEndPos.nX;
				rect32.oEndPos.nY = rect32.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect32, RU_T_RECT, true);
		
				KRURect	rect33;
				rect33.Color.Color_dw = 0xfffffcb2;
				rect33.oPosition.nX = rect32.oPosition.nX;
				rect33.oPosition.nY = rect32.oEndPos.nY + 6;
				rect33.oEndPos.nZ = rect33.oPosition.nZ = 0;
				rect33.oEndPos.nX = rect32.oEndPos.nX;
				rect33.oEndPos.nY = rect33.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect33, RU_T_RECT, true);
		
				KRURect	rect34;
				rect34.Color.Color_dw = 0xfffffcb2;
				rect34.oPosition.nX = rect33.oPosition.nX;
				rect34.oPosition.nY = rect33.oEndPos.nY + 6;
				rect34.oEndPos.nZ = rect34.oPosition.nZ = 0;
				rect34.oEndPos.nX = rect33.oEndPos.nX;
				rect34.oEndPos.nY = rect34.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect34, RU_T_RECT, true);
		
				KRURect	rect35;
				rect35.Color.Color_dw = 0xfffffcb2;
				rect35.oPosition.nX = rect34.oPosition.nX;
				rect35.oPosition.nY = rect34.oEndPos.nY + 6;
				rect35.oEndPos.nZ = rect35.oPosition.nZ = 0;
				rect35.oEndPos.nX = rect34.oEndPos.nX;
				rect35.oEndPos.nY = rect35.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect35, RU_T_RECT, true);
		
				KRURect	rect36;
				rect36.Color.Color_dw = 0xfffffcb2;
				rect36.oPosition.nX = rect35.oPosition.nX;
				rect36.oPosition.nY = rect35.oEndPos.nY + 6;
				rect36.oEndPos.nZ = rect36.oPosition.nZ = 0;
				rect36.oEndPos.nX = rect35.oEndPos.nX;
				rect36.oEndPos.nY = rect36.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect36, RU_T_RECT, true);
		
				KRURect	rect37;
				rect37.Color.Color_dw = 0xfffffcb2;
				rect37.oPosition.nX = rect36.oPosition.nX;
				rect37.oPosition.nY = rect36.oEndPos.nY + 6;
				rect37.oEndPos.nZ = rect37.oPosition.nZ = 0;
				rect37.oEndPos.nX = rect36.oEndPos.nX;
				rect37.oEndPos.nY = rect37.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect37, RU_T_RECT, true);
		
				KRURect	rect38;
				rect38.Color.Color_dw = 0xfffffcb2;
				rect38.oPosition.nX = rect37.oPosition.nX;
				rect38.oPosition.nY = rect37.oEndPos.nY + 6;
				rect38.oEndPos.nZ = rect38.oPosition.nZ = 0;
				rect38.oEndPos.nX = rect37.oEndPos.nX;
				rect38.oEndPos.nY = rect38.oPosition.nY + 14;
				g_pRepresentShell->DrawPrimitives(1, &rect38, RU_T_RECT, true);
		
		
		}
		
	}
}

int KUiTeamAuto::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
//-----------------------------------------------------------------------------------
		if(uParam == (unsigned int)(KWndWindow*)&m_FightText)
		{
		m_pSelf->ActivePage(0);	
		return 0;
		}
//-----------------------------------------------------------------------------------
		break;
	}
	return KWndPageSet::WndProc(uMsg, uParam, nParam);
}

KUiTeamAuto* KUiTeamAuto::LoadPrivateSetting(KIniFile* pFile)
{	
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTeamAuto;
		if (m_pSelf)
		{
			m_pSelf->Initialize();
			m_pSelf->m_FightPad.Show();
		}
	}
	if (pFile && m_pSelf)
	{
		int LocDo1 = 0;
		int LocDo2 = 0;
		int LocDo3 = 0;
		int LocDo4 = 0;
		int LocDo5 = 0;
		int LocDo6 = 0;
		int LocDo7 = 0;
		int LocDo8 = 0;
		int LocDo9 = 0;
		int LocDo10 = 0;
		int LocDo11 = 0;
		int LocDo12 = 0;
		int LocDo13 = 0;
		int LocDo14 = 0;
		int LocDo15 = 0;
		int LocDo16 = 0;
		int LocDo17 = 0;
		int LocDo18 = 0;
		int LocDo19 = 0;
		int LocDo20 = 0;
		int LocDo21 = 0;
		int LocDo22 = 0;
		int LocDo23 = 0;
		int LocDo24 = 0;
		int LocDo25 = 0;
		int LocDo26 = 0;
		int LocDo27 = 0;
		int LocDo28 = 0;
		int LocDo29 = 0;
		int LocDo30 = 0;
		int LocDo31 = 0;
		int LocDo32 = 0;
		int LocDo33 = 0;
		int LocDo34 = 0;
		int LocDo35 = 0;
		int LocDo36 = 0;
		int LocDo37 = 0;
		int LocDo38 = 0;
		int LocDo39 = 0;
		if ( pFile->GetInteger("AutoLocDo", "LocDo1", 0, (int*)(&LocDo1)))	
			m_pSelf->m_FightPad.m_tEdit1.SetIntText(LocDo1);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo2", 0, (int*)(&LocDo2)))	
			m_pSelf->m_FightPad.m_tEdit2.SetIntText(LocDo2);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo3", 0, (int*)(&LocDo3)))	
			m_pSelf->m_FightPad.m_tEdit3.SetIntText(LocDo3);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo4", 0, (int*)(&LocDo4)))	
			m_pSelf->m_FightPad.m_tEdit4.SetIntText(LocDo4);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo5", 0, (int*)(&LocDo5)))	
			m_pSelf->m_FightPad.m_tEdit5.SetIntText(LocDo5);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo6", 0, (int*)(&LocDo6)))	
			m_pSelf->m_FightPad.m_tEdit6.SetIntText(LocDo6);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo7", 0, (int*)(&LocDo7)))	
			m_pSelf->m_FightPad.m_tEdit7.SetIntText(LocDo7);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo8", 0, (int*)(&LocDo8)))	
			m_pSelf->m_FightPad.m_tEdit8.SetIntText(LocDo8);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo9", 0, (int*)(&LocDo9)))	
			m_pSelf->m_FightPad.m_tEdit9.SetIntText(LocDo9);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo10", 0, (int*)(&LocDo10)))	
			m_pSelf->m_FightPad.m_tEdit10.SetIntText(LocDo10);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo11", 0, (int*)(&LocDo11)))	
			m_pSelf->m_FightPad.m_tEdit11.SetIntText(LocDo11);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo12", 0, (int*)(&LocDo12)))	
			m_pSelf->m_FightPad.m_tEdit12.SetIntText(LocDo12);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo13", 0, (int*)(&LocDo13)))	
			m_pSelf->m_FightPad.m_tEdit13.SetIntText(LocDo13);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo14", 0, (int*)(&LocDo14)))	
			m_pSelf->m_FightPad.m_tEdit14.SetIntText(LocDo14);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo15", 0, (int*)(&LocDo15)))	
			m_pSelf->m_FightPad.m_tEdit15.SetIntText(LocDo15);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo16", 0, (int*)(&LocDo16)))	
			m_pSelf->m_FightPad.m_tEdit16.SetIntText(LocDo16);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo17", 0, (int*)(&LocDo17)))	
			m_pSelf->m_FightPad.m_tEdit17.SetIntText(LocDo17);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo18", 0, (int*)(&LocDo18)))	
			m_pSelf->m_FightPad.m_tEdit18.SetIntText(LocDo18);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo19", 0, (int*)(&LocDo19)))	
			m_pSelf->m_FightPad.m_tEdit19.SetIntText(LocDo19);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo20", 0, (int*)(&LocDo20)))	
			m_pSelf->m_FightPad.m_tEdit20.SetIntText(LocDo20);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo21", 0, (int*)(&LocDo21)))	
			m_pSelf->m_FightPad.m_tEdit21.SetIntText(LocDo21);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo22", 0, (int*)(&LocDo22)))	
			m_pSelf->m_FightPad.m_tEdit22.SetIntText(LocDo22);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo23", 0, (int*)(&LocDo23)))	
			m_pSelf->m_FightPad.m_tEdit23.SetIntText(LocDo23);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo24", 0, (int*)(&LocDo24)))	
			m_pSelf->m_FightPad.m_tEdit24.SetIntText(LocDo24);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo25", 0, (int*)(&LocDo25)))	
			m_pSelf->m_FightPad.m_tEdit25.SetIntText(LocDo25);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo26", 0, (int*)(&LocDo26)))	
			m_pSelf->m_FightPad.m_tEdit26.SetIntText(LocDo26);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo27", 0, (int*)(&LocDo27)))	
			m_pSelf->m_FightPad.m_tEdit27.SetIntText(LocDo27);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo28", 0, (int*)(&LocDo28)))	
			m_pSelf->m_FightPad.m_tEdit28.SetIntText(LocDo28);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo29", 0, (int*)(&LocDo29)))	
			m_pSelf->m_FightPad.m_tEdit29.SetIntText(LocDo29);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo30", 0, (int*)(&LocDo30)))	
			m_pSelf->m_FightPad.m_tEdit30.SetIntText(LocDo30);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo31", 0, (int*)(&LocDo31)))	
			m_pSelf->m_FightPad.m_tEdit31.SetIntText(LocDo31);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo32", 0, (int*)(&LocDo32)))	
			m_pSelf->m_FightPad.m_tEdit32.SetIntText(LocDo32);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo33", 0, (int*)(&LocDo33)))	
			m_pSelf->m_FightPad.m_tEdit33.SetIntText(LocDo33);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo34", 0, (int*)(&LocDo34)))	
			m_pSelf->m_FightPad.m_tEdit34.SetIntText(LocDo34);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo35", 0, (int*)(&LocDo35)))	
			m_pSelf->m_FightPad.m_tEdit35.SetIntText(LocDo35);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo36", 0, (int*)(&LocDo36)))	
			m_pSelf->m_FightPad.m_tEdit36.SetIntText(LocDo36);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo37", 0, (int*)(&LocDo37)))	
			m_pSelf->m_FightPad.m_tEdit37.SetIntText(LocDo37);	
		if ( pFile->GetInteger("AutoLocDo", "LocDo38", 0, (int*)(&LocDo38)))	
			m_pSelf->m_FightPad.m_tEdit38.SetIntText(LocDo38);
		if ( pFile->GetInteger("AutoLocDo", "LocDo39", 0, (int*)(&LocDo39)))	
			m_pSelf->m_FightPad.m_tEdit39.SetIntText(LocDo39);		
			
			
			
	}
	return m_pSelf;
}
int KUiTeamAuto::SavePrivateSetting(KIniFile* pFile)
{
		if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTeamAuto;
		if (m_pSelf)
		{
			m_pSelf->Initialize();
			m_pSelf->m_FightPad.Show();
		}
	}
	if (pFile && m_pSelf)
	{
		int LocDo1 = m_pSelf->m_FightPad.m_tEdit1.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo1", LocDo1);
		int LocDo2 = m_pSelf->m_FightPad.m_tEdit2.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo2", LocDo2);
		int LocDo3 = m_pSelf->m_FightPad.m_tEdit3.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo3", LocDo3);
		int LocDo4 = m_pSelf->m_FightPad.m_tEdit4.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo4", LocDo4);
		int LocDo5 = m_pSelf->m_FightPad.m_tEdit5.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo5", LocDo5);
		int LocDo6 = m_pSelf->m_FightPad.m_tEdit6.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo6", LocDo6);
		int LocDo7 = m_pSelf->m_FightPad.m_tEdit7.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo7", LocDo7);
		int LocDo8 = m_pSelf->m_FightPad.m_tEdit8.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo8", LocDo8);
		int LocDo9 = m_pSelf->m_FightPad.m_tEdit9.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo9", LocDo9);
		int LocDo10 = m_pSelf->m_FightPad.m_tEdit10.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo10", LocDo10);
		int LocDo11 = m_pSelf->m_FightPad.m_tEdit11.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo11", LocDo11);
		int LocDo12 = m_pSelf->m_FightPad.m_tEdit12.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo12", LocDo12);
		int LocDo13 = m_pSelf->m_FightPad.m_tEdit13.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo13", LocDo13);
		int LocDo14 = m_pSelf->m_FightPad.m_tEdit14.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo14", LocDo14);
		int LocDo15 = m_pSelf->m_FightPad.m_tEdit15.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo15", LocDo15);
		int LocDo16 = m_pSelf->m_FightPad.m_tEdit16.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo16", LocDo16);
		int LocDo17 = m_pSelf->m_FightPad.m_tEdit17.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo17", LocDo17);
		int LocDo18 = m_pSelf->m_FightPad.m_tEdit18.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo18", LocDo18);
		int LocDo19 = m_pSelf->m_FightPad.m_tEdit19.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo19", LocDo19);
		int LocDo20 = m_pSelf->m_FightPad.m_tEdit20.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo20", LocDo20);
		int LocDo21 = m_pSelf->m_FightPad.m_tEdit21.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo21", LocDo21);
		int LocDo22 = m_pSelf->m_FightPad.m_tEdit22.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo22", LocDo22);
		int LocDo23 = m_pSelf->m_FightPad.m_tEdit23.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo23", LocDo23);
		int LocDo24 = m_pSelf->m_FightPad.m_tEdit24.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo24", LocDo24);
		int LocDo25 = m_pSelf->m_FightPad.m_tEdit25.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo25", LocDo25);
		int LocDo26 = m_pSelf->m_FightPad.m_tEdit26.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo26", LocDo26);
		int LocDo27 = m_pSelf->m_FightPad.m_tEdit27.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo27", LocDo27);
		int LocDo28 = m_pSelf->m_FightPad.m_tEdit28.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo28", LocDo28);
		int LocDo29 = m_pSelf->m_FightPad.m_tEdit29.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo29", LocDo29);
		int LocDo30 = m_pSelf->m_FightPad.m_tEdit30.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo30", LocDo30);
		int LocDo31 = m_pSelf->m_FightPad.m_tEdit31.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo31", LocDo31);
		int LocDo32 = m_pSelf->m_FightPad.m_tEdit32.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo32", LocDo32);
		int LocDo33 = m_pSelf->m_FightPad.m_tEdit33.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo33", LocDo33);
		int LocDo34 = m_pSelf->m_FightPad.m_tEdit34.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo34", LocDo34);
		int LocDo35 = m_pSelf->m_FightPad.m_tEdit35.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo35", LocDo35);
		int LocDo36 = m_pSelf->m_FightPad.m_tEdit36.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo36", LocDo36);
		int LocDo37 = m_pSelf->m_FightPad.m_tEdit37.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo37", LocDo37);
		int LocDo38 = m_pSelf->m_FightPad.m_tEdit38.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo38", LocDo38);
		int LocDo39 = m_pSelf->m_FightPad.m_tEdit39.GetIntNumber();
		pFile->WriteInteger("AutoLocDo", "LocDo39", LocDo39);
		
		
	
	}
	return 1;
}
	
void KUiTeamAuto::Breathe()
{
	
	if(!g_pCoreShell) return;
	BOOL bAct = m_ActiveText;
	m_Active.CheckButton(bAct);
	m_FightPad.Active(bAct);
	m_FightText.BringToTop();
	
	
	
	if (m_FightPad.m_tEdit1.GetIntNumber() > 200)
		m_FightPad.m_tEdit1.SetIntText(200);
	if (m_FightPad.m_tEdit2.GetIntNumber() > 200)
		m_FightPad.m_tEdit2.SetIntText(200);
	if (m_FightPad.m_tEdit3.GetIntNumber() > 200)
		m_FightPad.m_tEdit3.SetIntText(200);
	if (m_FightPad.m_tEdit4.GetIntNumber() > 25)
		m_FightPad.m_tEdit4.SetIntText(25);
	if (m_FightPad.m_tEdit5.GetIntNumber() > 25)
		m_FightPad.m_tEdit5.SetIntText(25);
	if (m_FightPad.m_tEdit6.GetIntNumber() > 30)
		m_FightPad.m_tEdit6.SetIntText(30);
	if (m_FightPad.m_tEdit7.GetIntNumber() > 25)
		m_FightPad.m_tEdit7.SetIntText(25);
	if (m_FightPad.m_tEdit8.GetIntNumber() > 25)
		m_FightPad.m_tEdit8.SetIntText(25);
	if (m_FightPad.m_tEdit9.GetIntNumber() > 40)
		m_FightPad.m_tEdit9.SetIntText(40);
	if (m_FightPad.m_tEdit10.GetIntNumber() > 40)
		m_FightPad.m_tEdit10.SetIntText(40);
	if (m_FightPad.m_tEdit11.GetIntNumber() > 40)
		m_FightPad.m_tEdit11.SetIntText(40);
	if (m_FightPad.m_tEdit12.GetIntNumber() > 40)
		m_FightPad.m_tEdit12.SetIntText(40);
	if (m_FightPad.m_tEdit13.GetIntNumber() > 40)
		m_FightPad.m_tEdit13.SetIntText(40);
	if (m_FightPad.m_tEdit14.GetIntNumber() > 20)
		m_FightPad.m_tEdit14.SetIntText(20);
	if (m_FightPad.m_tEdit15.GetIntNumber() > 20)
		m_FightPad.m_tEdit15.SetIntText(20);
	if (m_FightPad.m_tEdit16.GetIntNumber() > 20)
		m_FightPad.m_tEdit16.SetIntText(20);
	if (m_FightPad.m_tEdit17.GetIntNumber() > 20)
		m_FightPad.m_tEdit17.SetIntText(20);
	if (m_FightPad.m_tEdit21.GetIntNumber() > 30)
		m_FightPad.m_tEdit21.SetIntText(30);
	if (m_FightPad.m_tEdit22.GetIntNumber() > 50)
		m_FightPad.m_tEdit22.SetIntText(50);
	if (m_FightPad.m_tEdit23.GetIntNumber() > 100)
		m_FightPad.m_tEdit23.SetIntText(100);
	if (m_FightPad.m_tEdit24.GetIntNumber() > 100)
		m_FightPad.m_tEdit24.SetIntText(100);
	if (m_FightPad.m_tEdit25.GetIntNumber() > 100)
		m_FightPad.m_tEdit25.SetIntText(100);
	if (m_FightPad.m_tEdit26.GetIntNumber() > 50)
		m_FightPad.m_tEdit26.SetIntText(50);
	if (m_FightPad.m_tEdit27.GetIntNumber() > 100)
		m_FightPad.m_tEdit27.SetIntText(100);
	if (m_FightPad.m_tEdit28.GetIntNumber() > 15)
		m_FightPad.m_tEdit28.SetIntText(15);
	if (m_FightPad.m_tEdit29.GetIntNumber() > 10)
		m_FightPad.m_tEdit29.SetIntText(10);
	if (m_FightPad.m_tEdit30.GetIntNumber() > 10)
		m_FightPad.m_tEdit30.SetIntText(10);
	if (m_FightPad.m_tEdit31.GetIntNumber() > 10)
		m_FightPad.m_tEdit31.SetIntText(10);
	if (m_FightPad.m_tEdit32.GetIntNumber() > 200)
		m_FightPad.m_tEdit32.SetIntText(200);
	if (m_FightPad.m_tEdit33.GetIntNumber() > 100)
		m_FightPad.m_tEdit33.SetIntText(100);
	if (m_FightPad.m_tEdit34.GetIntNumber() > 200)
		m_FightPad.m_tEdit34.SetIntText(200);
	if (m_FightPad.m_tEdit35.GetIntNumber() > 200)
		m_FightPad.m_tEdit35.SetIntText(200);
	if (m_FightPad.m_tEdit36.GetIntNumber() > 200)
		m_FightPad.m_tEdit36.SetIntText(200);
	if (m_FightPad.m_tEdit37.GetIntNumber() > 10)
		m_FightPad.m_tEdit37.SetIntText(10);
	if (m_FightPad.m_tEdit38.GetIntNumber() > 10)
		m_FightPad.m_tEdit38.SetIntText(10);
	if (m_FightPad.m_tEdit39.GetIntNumber() > 10)
		m_FightPad.m_tEdit39.SetIntText(10);
		
	
	KUiAuToLocDo pInfoTM;	
	pInfoTM.Option1 = m_FightPad.m_tEdit1.GetIntNumber();
	pInfoTM.Option2 = m_FightPad.m_tEdit2.GetIntNumber();
	pInfoTM.Option3 = m_FightPad.m_tEdit3.GetIntNumber();
	pInfoTM.Option4 = m_FightPad.m_tEdit4.GetIntNumber();
	pInfoTM.Option5 = m_FightPad.m_tEdit5.GetIntNumber();
	pInfoTM.Option6 = m_FightPad.m_tEdit6.GetIntNumber();
	pInfoTM.Option7 = m_FightPad.m_tEdit7.GetIntNumber();
	pInfoTM.Option8 = m_FightPad.m_tEdit8.GetIntNumber();
	pInfoTM.Option9 = m_FightPad.m_tEdit9.GetIntNumber();
	pInfoTM.Option10 = m_FightPad.m_tEdit10.GetIntNumber();
	pInfoTM.Option11 = m_FightPad.m_tEdit11.GetIntNumber();
	pInfoTM.Option12 = m_FightPad.m_tEdit12.GetIntNumber();
	pInfoTM.Option13 = m_FightPad.m_tEdit13.GetIntNumber();
	pInfoTM.Option14 = m_FightPad.m_tEdit14.GetIntNumber();
	pInfoTM.Option15 = m_FightPad.m_tEdit15.GetIntNumber();
	pInfoTM.Option16 = m_FightPad.m_tEdit16.GetIntNumber();
	pInfoTM.Option17 = m_FightPad.m_tEdit17.GetIntNumber();
	pInfoTM.Option18 = m_FightPad.m_tEdit18.GetIntNumber();
	pInfoTM.Option19 = m_FightPad.m_tEdit19.GetIntNumber();
	pInfoTM.Option20 = m_FightPad.m_tEdit20.GetIntNumber();
	pInfoTM.Option21 = m_FightPad.m_tEdit21.GetIntNumber();
	pInfoTM.Option22 = m_FightPad.m_tEdit22.GetIntNumber();
	pInfoTM.Option23 = m_FightPad.m_tEdit23.GetIntNumber();
	pInfoTM.Option24 = m_FightPad.m_tEdit24.GetIntNumber();
	pInfoTM.Option25 = m_FightPad.m_tEdit25.GetIntNumber();
	pInfoTM.Option26 = m_FightPad.m_tEdit26.GetIntNumber();
	pInfoTM.Option27 = m_FightPad.m_tEdit27.GetIntNumber();
	pInfoTM.Option28 = m_FightPad.m_tEdit28.GetIntNumber();
	pInfoTM.Option29 = m_FightPad.m_tEdit29.GetIntNumber();
	pInfoTM.Option30 = m_FightPad.m_tEdit30.GetIntNumber();
	pInfoTM.Option31 = m_FightPad.m_tEdit31.GetIntNumber();
	pInfoTM.Option32 = m_FightPad.m_tEdit32.GetIntNumber();
	pInfoTM.Option33 = m_FightPad.m_tEdit33.GetIntNumber();
	pInfoTM.Option34 = m_FightPad.m_tEdit34.GetIntNumber();
	pInfoTM.Option35 = m_FightPad.m_tEdit35.GetIntNumber();
	pInfoTM.Option36 = m_FightPad.m_tEdit36.GetIntNumber();
	pInfoTM.Option37 = m_FightPad.m_tEdit37.GetIntNumber();
	pInfoTM.Option38 = m_FightPad.m_tEdit38.GetIntNumber();
	pInfoTM.Option39 = m_FightPad.m_tEdit39.GetIntNumber();
	
	
	g_pCoreShell->OperationRequest(GOI_AUTOLOCDO, (int)&pInfoTM, 0);
		
}
