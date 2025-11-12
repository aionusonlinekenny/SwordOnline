/*****************************************************************************************
//	界面--小地图
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-21
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndImage.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndShadow.h"
#include "../Elem/PopupMenu.h"
#include "../../Core/Src/GameDataDef.h"




class KUiTopTKNew : public KWndImage
{
public:
	
	static KUiTopTKNew*	OpenWindow();					
	static KUiTopTKNew*	GetIfVisible();					
	static void			CloseWindow();	
	
	static void			UpdateDataTop(KUiTopTKNewData* pDataTop);


	static void			LoadScheme(const char* pScheme);

	void	Show();										
	void	Hide();										

private:
	KUiTopTKNew();
	~KUiTopTKNew() {}
	void	Initialize();
	void	PaintWindow();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	LoadScheme(KIniFile* pIni);

private:
	static KUiTopTKNew*	ms_pSelf;
private:

	KWndText32		m_PNumberMain,m_PTimeMain;
	KWndText32		m_PTextW1,m_PTextW2,m_PTextW3;
	KWndText32		m_PTextH1,m_PTextH2,m_PTextH3,m_PTextH4;
	KWndText32		m_PText21,m_PText22,m_PText23,m_PText24;
	KWndText32		m_PText31,m_PText32,m_PText33,m_PText34;
	KWndText32		m_PText1,m_PText2,m_PText3,m_PText4;

	KWndText32		m_PTopW1,m_PTopW2,m_PTopW3,m_PTopW4,m_PTopW5,m_PTopW6,m_PTopW7;
	KWndText32		m_PTopH1,m_PTopH2,m_PTopH3,m_PTopH4,m_PTopH5,m_PTopH6,m_PTopH7,m_PTopH8,m_PTopH9,m_PTopH10;

	KWndText32		m_PTop11,m_PTop12,m_PTop13,m_PTop14,m_PTop15,m_PTop16,m_PTop17,m_PTop18,m_PTop19,m_PTop110;
	KWndText32		m_PTop21,m_PTop22,m_PTop23,m_PTop24,m_PTop25,m_PTop26,m_PTop27,m_PTop28,m_PTop29,m_PTop210;
	KWndText32		m_PTop31,m_PTop32,m_PTop33,m_PTop34,m_PTop35,m_PTop36,m_PTop37,m_PTop38,m_PTop39,m_PTop310;
	KWndText32		m_PTop41,m_PTop42,m_PTop43,m_PTop44,m_PTop45,m_PTop46,m_PTop47,m_PTop48,m_PTop49,m_PTop410;
	KWndText32		m_PTop51,m_PTop52,m_PTop53,m_PTop54,m_PTop55,m_PTop56,m_PTop57,m_PTop58,m_PTop59,m_PTop510;
	KWndText32		m_PTop61,m_PTop62,m_PTop63,m_PTop64,m_PTop65,m_PTop66,m_PTop67,m_PTop68,m_PTop69,m_PTop610;
	KWndText32		m_PTop71,m_PTop72,m_PTop73,m_PTop74,m_PTop75,m_PTop76,m_PTop77,m_PTop78,m_PTop79,m_PTop710;
	

	KWndShadow		m_Shadow;
};

