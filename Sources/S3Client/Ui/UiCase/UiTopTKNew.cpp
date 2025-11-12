/*****************************************************************************************
//	ΩÁ√Ê--–°µÿÕº
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-21
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/Wnds.h"
#include "../Elem/WndMessage.h"
#include "UiTopTKNew.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/gamedatadef.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../Elem/TextPic.h"
#include <time.h>

extern iRepresentShell*	g_pRepresentShell;
extern iCoreShell*			g_pCoreShell;
extern IInlinePicEngineSink *g_pIInlinePicSink;


#define	SCHEME_INI_TOP_TK_NEW	"UiTopTK.ini"


KUiTopTKNew*			KUiTopTKNew::ms_pSelf = NULL;

KUiTopTKNew::KUiTopTKNew()
{
	
}

//--------------------------------------------------------------------------
//	π¶ƒ‹£∫¥Úø™¥∞ø⁄£¨∑µªÿŒ®“ªµƒ“ª∏ˆ¿‡∂‘œÛ µ¿˝
//--------------------------------------------------------------------------
KUiTopTKNew* KUiTopTKNew::OpenWindow()
{
	if (ms_pSelf == NULL)
	{
		ms_pSelf = new KUiTopTKNew;
		if (ms_pSelf)
			ms_pSelf->Initialize();
	}
	if (ms_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		ms_pSelf->Show();
		ms_pSelf->BringToTop();
	}
	return ms_pSelf;
}

//--------------------------------------------------------------------------
//	π¶ƒ‹£∫»Áπ˚¥∞ø⁄’˝±ªœ‘ æ£¨‘Ú∑µªÿ µ¿˝÷∏’Î
//--------------------------------------------------------------------------
KUiTopTKNew* KUiTopTKNew::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
		return ms_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	π¶ƒ‹£∫≥ı ºªØ
//--------------------------------------------------------------------------
void KUiTopTKNew::Initialize()
{
	AddChild(&m_Shadow);
	AddChild(&m_PNumberMain);
	AddChild(&m_PTimeMain);



	AddChild(&m_PTextW1);
	AddChild(&m_PTextW2);
	AddChild(&m_PTextW3);
	AddChild(&m_PTextH1);
	AddChild(&m_PTextH2);
	AddChild(&m_PTextH3);
	AddChild(&m_PTextH4);
	AddChild(&m_PText21);
	AddChild(&m_PText22);
	AddChild(&m_PText23);
	AddChild(&m_PText24);
	AddChild(&m_PText31);
	AddChild(&m_PText32);
	AddChild(&m_PText33);
	AddChild(&m_PText34);
	AddChild(&m_PText1);
	AddChild(&m_PText2);
	AddChild(&m_PText3);
	AddChild(&m_PText4);



	AddChild(&m_PTopW1);
	AddChild(&m_PTopW2);
	AddChild(&m_PTopW3);
	AddChild(&m_PTopW4);
	AddChild(&m_PTopW5);
	AddChild(&m_PTopW6);
	AddChild(&m_PTopW7);
	AddChild(&m_PTopH1);
	AddChild(&m_PTopH2);
	AddChild(&m_PTopH3);
	AddChild(&m_PTopH4);
	AddChild(&m_PTopH5);
	AddChild(&m_PTopH6);
	AddChild(&m_PTopH7);
	AddChild(&m_PTopH8);
	AddChild(&m_PTopH9);
	AddChild(&m_PTopH10);

	AddChild(&m_PTop11);
	AddChild(&m_PTop12);
	AddChild(&m_PTop13);
	AddChild(&m_PTop14);
	AddChild(&m_PTop15);
	AddChild(&m_PTop16);
	AddChild(&m_PTop17);
	AddChild(&m_PTop18);
	AddChild(&m_PTop19);
	AddChild(&m_PTop110);
	AddChild(&m_PTop21);
	AddChild(&m_PTop22);
	AddChild(&m_PTop23);
	AddChild(&m_PTop24);
	AddChild(&m_PTop25);
	AddChild(&m_PTop26);
	AddChild(&m_PTop27);
	AddChild(&m_PTop28);
	AddChild(&m_PTop29);
	AddChild(&m_PTop210);
	AddChild(&m_PTop31);
	AddChild(&m_PTop32);
	AddChild(&m_PTop33);
	AddChild(&m_PTop34);
	AddChild(&m_PTop35);
	AddChild(&m_PTop36);
	AddChild(&m_PTop37);
	AddChild(&m_PTop38);
	AddChild(&m_PTop39);
	AddChild(&m_PTop310);
	AddChild(&m_PTop41);
	AddChild(&m_PTop42);
	AddChild(&m_PTop43);
	AddChild(&m_PTop44);
	AddChild(&m_PTop45);
	AddChild(&m_PTop46);
	AddChild(&m_PTop47);
	AddChild(&m_PTop48);
	AddChild(&m_PTop49);
	AddChild(&m_PTop410);
	AddChild(&m_PTop51);
	AddChild(&m_PTop52);
	AddChild(&m_PTop53);
	AddChild(&m_PTop54);
	AddChild(&m_PTop55);
	AddChild(&m_PTop56);
	AddChild(&m_PTop57);
	AddChild(&m_PTop58);
	AddChild(&m_PTop59);
	AddChild(&m_PTop510);
	AddChild(&m_PTop61);
	AddChild(&m_PTop62);
	AddChild(&m_PTop63);
	AddChild(&m_PTop64);
	AddChild(&m_PTop65);
	AddChild(&m_PTop66);
	AddChild(&m_PTop67);
	AddChild(&m_PTop68);
	AddChild(&m_PTop69);
	AddChild(&m_PTop610);
	AddChild(&m_PTop71);
	AddChild(&m_PTop72);
	AddChild(&m_PTop73);
	AddChild(&m_PTop74);
	AddChild(&m_PTop75);
	AddChild(&m_PTop76);
	AddChild(&m_PTop77);
	AddChild(&m_PTop78);
	AddChild(&m_PTop79);
	AddChild(&m_PTop710);




	char Scheme[128];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	π¶ƒ‹£∫πÿ±’¥∞ø⁄
//--------------------------------------------------------------------------
void KUiTopTKNew::CloseWindow()
{
	if (ms_pSelf)
	{
		ms_pSelf->Destroy();
		ms_pSelf = NULL;
	}
}


void KUiTopTKNew::UpdateDataTop(KUiTopTKNewData* pDataTop)
{
	if (ms_pSelf)
	{




	char szBufferText[32] = "";

	if (((int)pDataTop->m_Phe) == 1)
	{
	sprintf(szBufferText,"TËng %d ng≠Íi",(int)pDataTop->m_Soluong);
	}
	else if (((int)pDataTop->m_Phe) == 2)
	{
	sprintf(szBufferText,"Kim %d ng≠Íi",(int)pDataTop->m_Soluong);
	}
	else 
	{
	sprintf(szBufferText,"TËng 0 ng≠Íi : Kim 0 ng≠Íi");
	}

	ms_pSelf->m_PNumberMain.SetText(szBufferText);

	sprintf(szBufferText,"Cﬂn %d phÛt",(int)pDataTop->m_ThoiGian);

	ms_pSelf->m_PTimeMain.SetText(szBufferText);

	ms_pSelf->m_PText21.SetIntText((int)pDataTop->m_NguoiChoi);
	ms_pSelf->m_PText31.SetIntText((int)pDataTop->m_TNguoiChoi);
	ms_pSelf->m_PText22.SetIntText((int)pDataTop->m_NPC);
	ms_pSelf->m_PText32.SetIntText((int)pDataTop->m_TNPC);
	ms_pSelf->m_PText23.SetIntText((int)pDataTop->m_LienTramHT);
	ms_pSelf->m_PText33.SetIntText((int)pDataTop->m_TLienTramHT);
	ms_pSelf->m_PText24.SetIntText((int)pDataTop->m_BauVat);
	ms_pSelf->m_PText34.SetIntText((int)pDataTop->m_TBauVat);

	sprintf(szBufferText,"ßi”m t›ch lÚy: %d",(int)pDataTop->m_TichLuy);
	ms_pSelf->m_PText1.SetText(szBufferText);
	sprintf(szBufferText,"Tˆ vong: %d",(int)pDataTop->m_TuVong);
	ms_pSelf->m_PText2.SetText(szBufferText);
	sprintf(szBufferText,"Li™n tr∂m: %d",(int)pDataTop->m_LienTram);
	ms_pSelf->m_PText3.SetText(szBufferText);


	

	if (((int)pDataTop->m_Top1Phe) == 1)
	{
	sprintf(szBufferText,"1:TËng");
	ms_pSelf->m_PTopH1.SetText(szBufferText);
	ms_pSelf->m_PTopH1.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop11.SetText(pDataTop->m_szTop1Name);
	ms_pSelf->m_PTop11.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop21.SetIntText((int)pDataTop->m_Top1TichLuy);
	ms_pSelf->m_PTop21.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop31.SetIntText((int)pDataTop->m_Top1NguoiChoi);
	ms_pSelf->m_PTop31.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop41.SetIntText((int)pDataTop->m_Top1NPC);
	ms_pSelf->m_PTop41.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop51.SetIntText((int)pDataTop->m_Top1TuVong);
	ms_pSelf->m_PTop51.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop61.SetIntText((int)pDataTop->m_Top1LienTram);
	ms_pSelf->m_PTop61.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop71.SetIntText((int)pDataTop->m_Top1BaoVat);
	ms_pSelf->m_PTop71.SetTextColor(RGB(244, 164, 96));
	}
	else if (((int)pDataTop->m_Top1Phe) == 2)
	{
	sprintf(szBufferText,"1:Kim");
	ms_pSelf->m_PTopH1.SetText(szBufferText);
	ms_pSelf->m_PTopH1.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop11.SetText(pDataTop->m_szTop1Name);
	ms_pSelf->m_PTop11.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop21.SetIntText((int)pDataTop->m_Top1TichLuy);
	ms_pSelf->m_PTop21.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop31.SetIntText((int)pDataTop->m_Top1NguoiChoi);
	ms_pSelf->m_PTop31.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop41.SetIntText((int)pDataTop->m_Top1NPC);
	ms_pSelf->m_PTop41.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop51.SetIntText((int)pDataTop->m_Top1TuVong);
	ms_pSelf->m_PTop51.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop61.SetIntText((int)pDataTop->m_Top1LienTram);
	ms_pSelf->m_PTop61.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop71.SetIntText((int)pDataTop->m_Top1BaoVat);
	ms_pSelf->m_PTop71.SetTextColor(RGB(238 ,233, 233));
	}

	else

	{
	ms_pSelf->m_PTopH1.SetText("");
	ms_pSelf->m_PTop11.SetText("");
	ms_pSelf->m_PTop21.SetText("");
	ms_pSelf->m_PTop31.SetText("");
	ms_pSelf->m_PTop41.SetText("");
	ms_pSelf->m_PTop51.SetText("");
	ms_pSelf->m_PTop61.SetText("");
	ms_pSelf->m_PTop71.SetText("");
	}

		








	if (((int)pDataTop->m_Top2Phe) == 1)
	{
	sprintf(szBufferText,"2:TËng");
	ms_pSelf->m_PTopH2.SetText(szBufferText);
	ms_pSelf->m_PTopH2.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop12.SetText(pDataTop->m_szTop2Name);
	ms_pSelf->m_PTop12.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop22.SetIntText((int)pDataTop->m_Top2TichLuy);
	ms_pSelf->m_PTop22.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop32.SetIntText((int)pDataTop->m_Top2NguoiChoi);
	ms_pSelf->m_PTop32.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop42.SetIntText((int)pDataTop->m_Top2NPC);
	ms_pSelf->m_PTop42.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop52.SetIntText((int)pDataTop->m_Top2TuVong);
	ms_pSelf->m_PTop52.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop62.SetIntText((int)pDataTop->m_Top2LienTram);
	ms_pSelf->m_PTop62.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop72.SetIntText((int)pDataTop->m_Top2BaoVat);
	ms_pSelf->m_PTop72.SetTextColor(RGB(244, 164, 96));
	}
	else if (((int)pDataTop->m_Top2Phe) == 2)
	{
	sprintf(szBufferText,"2:Kim");
	ms_pSelf->m_PTopH2.SetText(szBufferText);
	ms_pSelf->m_PTopH2.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop12.SetText(pDataTop->m_szTop2Name);
	ms_pSelf->m_PTop12.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop22.SetIntText((int)pDataTop->m_Top2TichLuy);
	ms_pSelf->m_PTop22.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop32.SetIntText((int)pDataTop->m_Top2NguoiChoi);
	ms_pSelf->m_PTop32.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop42.SetIntText((int)pDataTop->m_Top2NPC);
	ms_pSelf->m_PTop42.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop52.SetIntText((int)pDataTop->m_Top2TuVong);
	ms_pSelf->m_PTop52.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop62.SetIntText((int)pDataTop->m_Top2LienTram);
	ms_pSelf->m_PTop62.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop72.SetIntText((int)pDataTop->m_Top2BaoVat);
	ms_pSelf->m_PTop72.SetTextColor(RGB(238 ,233, 233));
	}

	else

	{
	ms_pSelf->m_PTopH2.SetText("");
	ms_pSelf->m_PTop12.SetText("");
	ms_pSelf->m_PTop22.SetText("");
	ms_pSelf->m_PTop32.SetText("");
	ms_pSelf->m_PTop42.SetText("");
	ms_pSelf->m_PTop52.SetText("");
	ms_pSelf->m_PTop62.SetText("");
	ms_pSelf->m_PTop72.SetText("");
	}


	if (((int)pDataTop->m_Top3Phe) == 1)
	{
	sprintf(szBufferText,"3:TËng");
	ms_pSelf->m_PTopH3.SetText(szBufferText);
	ms_pSelf->m_PTopH3.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop13.SetText(pDataTop->m_szTop3Name);
	ms_pSelf->m_PTop13.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop23.SetIntText((int)pDataTop->m_Top3TichLuy);
	ms_pSelf->m_PTop23.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop33.SetIntText((int)pDataTop->m_Top3NguoiChoi);
	ms_pSelf->m_PTop33.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop43.SetIntText((int)pDataTop->m_Top3NPC);
	ms_pSelf->m_PTop43.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop53.SetIntText((int)pDataTop->m_Top3TuVong);
	ms_pSelf->m_PTop53.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop63.SetIntText((int)pDataTop->m_Top3LienTram);
	ms_pSelf->m_PTop63.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop73.SetIntText((int)pDataTop->m_Top3BaoVat);
	ms_pSelf->m_PTop73.SetTextColor(RGB(244, 164, 96));
	}
	else if (((int)pDataTop->m_Top3Phe) == 2)
	{
	sprintf(szBufferText,"3:Kim");
	ms_pSelf->m_PTopH3.SetText(szBufferText);
	ms_pSelf->m_PTopH3.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop13.SetText(pDataTop->m_szTop3Name);
	ms_pSelf->m_PTop13.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop23.SetIntText((int)pDataTop->m_Top3TichLuy);
	ms_pSelf->m_PTop23.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop33.SetIntText((int)pDataTop->m_Top3NguoiChoi);
	ms_pSelf->m_PTop33.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop43.SetIntText((int)pDataTop->m_Top3NPC);
	ms_pSelf->m_PTop43.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop53.SetIntText((int)pDataTop->m_Top3TuVong);
	ms_pSelf->m_PTop53.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop63.SetIntText((int)pDataTop->m_Top3LienTram);
	ms_pSelf->m_PTop63.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop73.SetIntText((int)pDataTop->m_Top3BaoVat);
	ms_pSelf->m_PTop73.SetTextColor(RGB(238 ,233, 233));
	}

	else

	{
	ms_pSelf->m_PTopH3.SetText("");
	ms_pSelf->m_PTop13.SetText("");
	ms_pSelf->m_PTop23.SetText("");
	ms_pSelf->m_PTop33.SetText("");
	ms_pSelf->m_PTop43.SetText("");
	ms_pSelf->m_PTop53.SetText("");
	ms_pSelf->m_PTop63.SetText("");
	ms_pSelf->m_PTop73.SetText("");
	}


	if (((int)pDataTop->m_Top4Phe) == 1)
	{
	sprintf(szBufferText,"4:TËng");
	ms_pSelf->m_PTopH4.SetText(szBufferText);
	ms_pSelf->m_PTopH4.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop14.SetText(pDataTop->m_szTop4Name);
	ms_pSelf->m_PTop14.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop24.SetIntText((int)pDataTop->m_Top4TichLuy);
	ms_pSelf->m_PTop24.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop34.SetIntText((int)pDataTop->m_Top4NguoiChoi);
	ms_pSelf->m_PTop34.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop44.SetIntText((int)pDataTop->m_Top4NPC);
	ms_pSelf->m_PTop44.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop54.SetIntText((int)pDataTop->m_Top4TuVong);
	ms_pSelf->m_PTop54.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop64.SetIntText((int)pDataTop->m_Top4LienTram);
	ms_pSelf->m_PTop64.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop74.SetIntText((int)pDataTop->m_Top4BaoVat);
	ms_pSelf->m_PTop74.SetTextColor(RGB(244, 164, 96));
	}
	else if (((int)pDataTop->m_Top4Phe) == 2)
	{
	sprintf(szBufferText,"4:Kim");
	ms_pSelf->m_PTopH4.SetText(szBufferText);
	ms_pSelf->m_PTopH4.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop14.SetText(pDataTop->m_szTop4Name);
	ms_pSelf->m_PTop14.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop24.SetIntText((int)pDataTop->m_Top4TichLuy);
	ms_pSelf->m_PTop24.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop34.SetIntText((int)pDataTop->m_Top4NguoiChoi);
	ms_pSelf->m_PTop34.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop44.SetIntText((int)pDataTop->m_Top4NPC);
	ms_pSelf->m_PTop44.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop54.SetIntText((int)pDataTop->m_Top4TuVong);
	ms_pSelf->m_PTop54.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop64.SetIntText((int)pDataTop->m_Top4LienTram);
	ms_pSelf->m_PTop64.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop74.SetIntText((int)pDataTop->m_Top4BaoVat);
	ms_pSelf->m_PTop74.SetTextColor(RGB(238 ,233, 233));
	}

	else

	{
	ms_pSelf->m_PTopH4.SetText("");
	ms_pSelf->m_PTop14.SetText("");
	ms_pSelf->m_PTop24.SetText("");
	ms_pSelf->m_PTop34.SetText("");
	ms_pSelf->m_PTop44.SetText("");
	ms_pSelf->m_PTop54.SetText("");
	ms_pSelf->m_PTop64.SetText("");
	ms_pSelf->m_PTop74.SetText("");
	}


	if (((int)pDataTop->m_Top5Phe) == 1)
	{
	sprintf(szBufferText,"5:TËng");
	ms_pSelf->m_PTopH5.SetText(szBufferText);
	ms_pSelf->m_PTopH5.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop15.SetText(pDataTop->m_szTop5Name);
	ms_pSelf->m_PTop15.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop25.SetIntText((int)pDataTop->m_Top5TichLuy);
	ms_pSelf->m_PTop25.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop35.SetIntText((int)pDataTop->m_Top5NguoiChoi);
	ms_pSelf->m_PTop35.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop45.SetIntText((int)pDataTop->m_Top5NPC);
	ms_pSelf->m_PTop45.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop55.SetIntText((int)pDataTop->m_Top5TuVong);
	ms_pSelf->m_PTop55.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop65.SetIntText((int)pDataTop->m_Top5LienTram);
	ms_pSelf->m_PTop65.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop75.SetIntText((int)pDataTop->m_Top5BaoVat);
	ms_pSelf->m_PTop75.SetTextColor(RGB(244, 164, 96));
	}
	else if (((int)pDataTop->m_Top5Phe) == 2)
	{
	sprintf(szBufferText,"5:Kim");
	ms_pSelf->m_PTopH5.SetText(szBufferText);
	ms_pSelf->m_PTopH5.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop15.SetText(pDataTop->m_szTop5Name);
	ms_pSelf->m_PTop15.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop25.SetIntText((int)pDataTop->m_Top5TichLuy);
	ms_pSelf->m_PTop25.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop35.SetIntText((int)pDataTop->m_Top5NguoiChoi);
	ms_pSelf->m_PTop35.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop45.SetIntText((int)pDataTop->m_Top5NPC);
	ms_pSelf->m_PTop45.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop55.SetIntText((int)pDataTop->m_Top5TuVong);
	ms_pSelf->m_PTop55.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop65.SetIntText((int)pDataTop->m_Top5LienTram);
	ms_pSelf->m_PTop65.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop75.SetIntText((int)pDataTop->m_Top5BaoVat);
	ms_pSelf->m_PTop75.SetTextColor(RGB(238 ,233, 233));
	}

	else

	{
	ms_pSelf->m_PTopH5.SetText("");
	ms_pSelf->m_PTop15.SetText("");
	ms_pSelf->m_PTop25.SetText("");
	ms_pSelf->m_PTop35.SetText("");
	ms_pSelf->m_PTop45.SetText("");
	ms_pSelf->m_PTop55.SetText("");
	ms_pSelf->m_PTop65.SetText("");
	ms_pSelf->m_PTop75.SetText("");
	}


	if (((int)pDataTop->m_Top6Phe) == 1)
	{
	sprintf(szBufferText,"6:TËng");
	ms_pSelf->m_PTopH6.SetText(szBufferText);
	ms_pSelf->m_PTopH6.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop16.SetText(pDataTop->m_szTop6Name);
	ms_pSelf->m_PTop16.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop26.SetIntText((int)pDataTop->m_Top6TichLuy);
	ms_pSelf->m_PTop26.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop36.SetIntText((int)pDataTop->m_Top6NguoiChoi);
	ms_pSelf->m_PTop36.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop46.SetIntText((int)pDataTop->m_Top6NPC);
	ms_pSelf->m_PTop46.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop56.SetIntText((int)pDataTop->m_Top6TuVong);
	ms_pSelf->m_PTop56.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop66.SetIntText((int)pDataTop->m_Top6LienTram);
	ms_pSelf->m_PTop66.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop76.SetIntText((int)pDataTop->m_Top6BaoVat);
	ms_pSelf->m_PTop76.SetTextColor(RGB(244, 164, 96));
	}
	else if (((int)pDataTop->m_Top6Phe) == 2)
	{
	sprintf(szBufferText,"6:Kim");
	ms_pSelf->m_PTopH6.SetText(szBufferText);
	ms_pSelf->m_PTopH6.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop16.SetText(pDataTop->m_szTop6Name);
	ms_pSelf->m_PTop16.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop26.SetIntText((int)pDataTop->m_Top6TichLuy);
	ms_pSelf->m_PTop26.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop36.SetIntText((int)pDataTop->m_Top6NguoiChoi);
	ms_pSelf->m_PTop36.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop46.SetIntText((int)pDataTop->m_Top6NPC);
	ms_pSelf->m_PTop46.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop56.SetIntText((int)pDataTop->m_Top6TuVong);
	ms_pSelf->m_PTop56.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop66.SetIntText((int)pDataTop->m_Top6LienTram);
	ms_pSelf->m_PTop66.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop76.SetIntText((int)pDataTop->m_Top6BaoVat);
	ms_pSelf->m_PTop76.SetTextColor(RGB(238 ,233, 233));
	}

	else

	{
	ms_pSelf->m_PTopH6.SetText("");
	ms_pSelf->m_PTop16.SetText("");
	ms_pSelf->m_PTop26.SetText("");
	ms_pSelf->m_PTop36.SetText("");
	ms_pSelf->m_PTop46.SetText("");
	ms_pSelf->m_PTop56.SetText("");
	ms_pSelf->m_PTop66.SetText("");
	ms_pSelf->m_PTop76.SetText("");
	}


	if (((int)pDataTop->m_Top7Phe) == 1)
	{
	sprintf(szBufferText,"7:TËng");
	ms_pSelf->m_PTopH7.SetText(szBufferText);
	ms_pSelf->m_PTopH7.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop17.SetText(pDataTop->m_szTop7Name);
	ms_pSelf->m_PTop17.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop27.SetIntText((int)pDataTop->m_Top7TichLuy);
	ms_pSelf->m_PTop27.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop37.SetIntText((int)pDataTop->m_Top7NguoiChoi);
	ms_pSelf->m_PTop37.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop47.SetIntText((int)pDataTop->m_Top7NPC);
	ms_pSelf->m_PTop47.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop57.SetIntText((int)pDataTop->m_Top7TuVong);
	ms_pSelf->m_PTop57.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop67.SetIntText((int)pDataTop->m_Top7LienTram);
	ms_pSelf->m_PTop67.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop77.SetIntText((int)pDataTop->m_Top7BaoVat);
	ms_pSelf->m_PTop77.SetTextColor(RGB(244, 164, 96));
	}
	else if (((int)pDataTop->m_Top7Phe) == 2)
	{
	sprintf(szBufferText,"7:Kim");
	ms_pSelf->m_PTopH7.SetText(szBufferText);
	ms_pSelf->m_PTopH7.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop17.SetText(pDataTop->m_szTop7Name);
	ms_pSelf->m_PTop17.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop27.SetIntText((int)pDataTop->m_Top7TichLuy);
	ms_pSelf->m_PTop27.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop37.SetIntText((int)pDataTop->m_Top7NguoiChoi);
	ms_pSelf->m_PTop37.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop47.SetIntText((int)pDataTop->m_Top7NPC);
	ms_pSelf->m_PTop47.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop57.SetIntText((int)pDataTop->m_Top7TuVong);
	ms_pSelf->m_PTop57.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop67.SetIntText((int)pDataTop->m_Top7LienTram);
	ms_pSelf->m_PTop67.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop77.SetIntText((int)pDataTop->m_Top7BaoVat);
	ms_pSelf->m_PTop77.SetTextColor(RGB(238 ,233, 233));
	}

	else

	{
	ms_pSelf->m_PTopH7.SetText("");
	ms_pSelf->m_PTop17.SetText("");
	ms_pSelf->m_PTop27.SetText("");
	ms_pSelf->m_PTop37.SetText("");
	ms_pSelf->m_PTop47.SetText("");
	ms_pSelf->m_PTop57.SetText("");
	ms_pSelf->m_PTop67.SetText("");
	ms_pSelf->m_PTop77.SetText("");
	}


	if (((int)pDataTop->m_Top8Phe) == 1)
	{
	sprintf(szBufferText,"8:TËng");
	ms_pSelf->m_PTopH8.SetText(szBufferText);
	ms_pSelf->m_PTopH8.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop18.SetText(pDataTop->m_szTop8Name);
	ms_pSelf->m_PTop18.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop28.SetIntText((int)pDataTop->m_Top8TichLuy);
	ms_pSelf->m_PTop28.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop38.SetIntText((int)pDataTop->m_Top8NguoiChoi);
	ms_pSelf->m_PTop38.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop48.SetIntText((int)pDataTop->m_Top8NPC);
	ms_pSelf->m_PTop48.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop58.SetIntText((int)pDataTop->m_Top8TuVong);
	ms_pSelf->m_PTop58.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop68.SetIntText((int)pDataTop->m_Top8LienTram);
	ms_pSelf->m_PTop68.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop78.SetIntText((int)pDataTop->m_Top8BaoVat);
	ms_pSelf->m_PTop78.SetTextColor(RGB(244, 164, 96));
	}
	else if (((int)pDataTop->m_Top8Phe) == 2)
	{
	sprintf(szBufferText,"8:Kim");
	ms_pSelf->m_PTopH8.SetText(szBufferText);
	ms_pSelf->m_PTopH8.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop18.SetText(pDataTop->m_szTop8Name);
	ms_pSelf->m_PTop18.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop28.SetIntText((int)pDataTop->m_Top8TichLuy);
	ms_pSelf->m_PTop28.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop38.SetIntText((int)pDataTop->m_Top8NguoiChoi);
	ms_pSelf->m_PTop38.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop48.SetIntText((int)pDataTop->m_Top8NPC);
	ms_pSelf->m_PTop48.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop58.SetIntText((int)pDataTop->m_Top8TuVong);
	ms_pSelf->m_PTop58.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop68.SetIntText((int)pDataTop->m_Top8LienTram);
	ms_pSelf->m_PTop68.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop78.SetIntText((int)pDataTop->m_Top8BaoVat);
	ms_pSelf->m_PTop78.SetTextColor(RGB(238 ,233, 233));
	}

	else

	{
	ms_pSelf->m_PTopH8.SetText("");
	ms_pSelf->m_PTop18.SetText("");
	ms_pSelf->m_PTop28.SetText("");
	ms_pSelf->m_PTop38.SetText("");
	ms_pSelf->m_PTop48.SetText("");
	ms_pSelf->m_PTop58.SetText("");
	ms_pSelf->m_PTop68.SetText("");
	ms_pSelf->m_PTop78.SetText("");
	}


	if (((int)pDataTop->m_Top9Phe) == 2)
	{
	sprintf(szBufferText,"9:Kim");
	ms_pSelf->m_PTopH9.SetText(szBufferText);
	ms_pSelf->m_PTopH9.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop19.SetText(pDataTop->m_szTop9Name);
	ms_pSelf->m_PTop19.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop29.SetIntText((int)pDataTop->m_Top9TichLuy);
	ms_pSelf->m_PTop29.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop39.SetIntText((int)pDataTop->m_Top9NguoiChoi);
	ms_pSelf->m_PTop39.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop49.SetIntText((int)pDataTop->m_Top9NPC);
	ms_pSelf->m_PTop49.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop59.SetIntText((int)pDataTop->m_Top9TuVong);
	ms_pSelf->m_PTop59.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop69.SetIntText((int)pDataTop->m_Top9LienTram);
	ms_pSelf->m_PTop69.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop79.SetIntText((int)pDataTop->m_Top9BaoVat);
	ms_pSelf->m_PTop79.SetTextColor(RGB(238 ,233, 233));
	}
	else if (((int)pDataTop->m_Top9Phe) == 1)
	{
	sprintf(szBufferText,"9:TËng");
	ms_pSelf->m_PTopH9.SetText(szBufferText);
	ms_pSelf->m_PTopH9.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop19.SetText(pDataTop->m_szTop9Name);
	ms_pSelf->m_PTop19.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop29.SetIntText((int)pDataTop->m_Top9TichLuy);
	ms_pSelf->m_PTop29.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop39.SetIntText((int)pDataTop->m_Top9NguoiChoi);
	ms_pSelf->m_PTop39.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop49.SetIntText((int)pDataTop->m_Top9NPC);
	ms_pSelf->m_PTop49.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop59.SetIntText((int)pDataTop->m_Top9TuVong);
	ms_pSelf->m_PTop59.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop69.SetIntText((int)pDataTop->m_Top9LienTram);
	ms_pSelf->m_PTop69.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop79.SetIntText((int)pDataTop->m_Top9BaoVat);
	ms_pSelf->m_PTop79.SetTextColor(RGB(244, 164, 96));
	}

	else

	{
	ms_pSelf->m_PTopH9.SetText("");
	ms_pSelf->m_PTop19.SetText("");
	ms_pSelf->m_PTop29.SetText("");
	ms_pSelf->m_PTop39.SetText("");
	ms_pSelf->m_PTop49.SetText("");
	ms_pSelf->m_PTop59.SetText("");
	ms_pSelf->m_PTop69.SetText("");
	ms_pSelf->m_PTop79.SetText("");
	}


	if (((int)pDataTop->m_Top10Phe) == 1)
	{
	sprintf(szBufferText,"10:TËng");
	ms_pSelf->m_PTopH10.SetText(szBufferText);
	ms_pSelf->m_PTopH10.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop110.SetText(pDataTop->m_szTop10Name);
	ms_pSelf->m_PTop110.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop210.SetIntText((int)pDataTop->m_Top10TichLuy);
	ms_pSelf->m_PTop210.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop310.SetIntText((int)pDataTop->m_Top10NguoiChoi);
	ms_pSelf->m_PTop310.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop410.SetIntText((int)pDataTop->m_Top10NPC);
	ms_pSelf->m_PTop410.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop510.SetIntText((int)pDataTop->m_Top10TuVong);
	ms_pSelf->m_PTop510.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop610.SetIntText((int)pDataTop->m_Top10LienTram);
	ms_pSelf->m_PTop610.SetTextColor(RGB(244, 164, 96));
	ms_pSelf->m_PTop710.SetIntText((int)pDataTop->m_Top10BaoVat);
	ms_pSelf->m_PTop710.SetTextColor(RGB(244, 164, 96));
	}
	else if (((int)pDataTop->m_Top10Phe) == 2)
	{
	sprintf(szBufferText,"10:Kim");
	ms_pSelf->m_PTopH10.SetText(szBufferText);
	ms_pSelf->m_PTopH10.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop110.SetText(pDataTop->m_szTop10Name);
	ms_pSelf->m_PTop110.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop210.SetIntText((int)pDataTop->m_Top10TichLuy);
	ms_pSelf->m_PTop210.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop310.SetIntText((int)pDataTop->m_Top10NguoiChoi);
	ms_pSelf->m_PTop310.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop410.SetIntText((int)pDataTop->m_Top10NPC);
	ms_pSelf->m_PTop410.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop510.SetIntText((int)pDataTop->m_Top10TuVong);
	ms_pSelf->m_PTop510.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop610.SetIntText((int)pDataTop->m_Top10LienTram);
	ms_pSelf->m_PTop610.SetTextColor(RGB(238 ,233, 233));
	ms_pSelf->m_PTop710.SetIntText((int)pDataTop->m_Top10BaoVat);
	ms_pSelf->m_PTop710.SetTextColor(RGB(238 ,233, 233));
	}

	else

	{
	ms_pSelf->m_PTopH10.SetText("");
	ms_pSelf->m_PTop110.SetText("");
	ms_pSelf->m_PTop210.SetText("");
	ms_pSelf->m_PTop310.SetText("");
	ms_pSelf->m_PTop410.SetText("");
	ms_pSelf->m_PTop510.SetText("");
	ms_pSelf->m_PTop610.SetText("");
	ms_pSelf->m_PTop710.SetText("");
	}











	}
}



//--------------------------------------------------------------------------
//	π¶ƒ‹£∫‘ÿ»Î¥∞ø⁄µƒΩÁ√Ê∑Ω∞∏
//--------------------------------------------------------------------------
void KUiTopTKNew::LoadScheme(const char* pScheme)
{
	if (ms_pSelf)
	{
		char		szBuf[128];
		KIniFile	Ini;
		sprintf(szBuf, "%s\\%s", pScheme,SCHEME_INI_TOP_TK_NEW);
		if (Ini.Load(szBuf))
		{
			ms_pSelf->LoadScheme(&Ini);
		}
	}
}

void KUiTopTKNew::LoadScheme(KIniFile* pIni)
{
	if (pIni == NULL)
		return;
	Init(pIni, "Main");
	m_Shadow.Init(pIni, "Shadow");
	m_PNumberMain.Init(pIni, "PNumberMain");
	m_PTimeMain.Init(pIni, "PTimeMain");


	m_PTextW1.Init(pIni, "PTextW1");
	m_PTextW2.Init(pIni, "PTextW2");
	m_PTextW3.Init(pIni, "PTextW3");
	m_PTextH1.Init(pIni, "PTextH1");
	m_PTextH2.Init(pIni, "PTextH2");
	m_PTextH3.Init(pIni, "PTextH3");
	m_PTextH4.Init(pIni, "PTextH4");
	m_PText21.Init(pIni, "PText21");
	m_PText22.Init(pIni, "PText22");
	m_PText23.Init(pIni, "PText23");
	m_PText24.Init(pIni, "PText24");
	m_PText31.Init(pIni, "PText31");
	m_PText32.Init(pIni, "PText32");
	m_PText33.Init(pIni, "PText33");
	m_PText34.Init(pIni, "PText34");
	m_PText1.Init(pIni, "PText1");
	m_PText2.Init(pIni, "PText2");
	m_PText3.Init(pIni, "PText3");
	m_PText4.Init(pIni, "PText4");



	m_PTopW1.Init(pIni, "PTopW1");
	m_PTopW2.Init(pIni, "PTopW2");
	m_PTopW3.Init(pIni, "PTopW3");
	m_PTopW4.Init(pIni, "PTopW4");
	m_PTopW5.Init(pIni, "PTopW5");
	m_PTopW6.Init(pIni, "PTopW6");
	m_PTopW7.Init(pIni, "PTopW7");
	m_PTopH1.Init(pIni, "PTopH1");
	m_PTopH2.Init(pIni, "PTopH2");
	m_PTopH3.Init(pIni, "PTopH3");
	m_PTopH4.Init(pIni, "PTopH4");
	m_PTopH5.Init(pIni, "PTopH5");
	m_PTopH6.Init(pIni, "PTopH6");
	m_PTopH7.Init(pIni, "PTopH7");
	m_PTopH8.Init(pIni, "PTopH8");
	m_PTopH9.Init(pIni, "PTopH9");
	m_PTopH10.Init(pIni, "PTopH10");



	m_PTop11.Init(pIni, "PTop11");
	m_PTop12.Init(pIni, "PTop12");
	m_PTop13.Init(pIni, "PTop13");
	m_PTop14.Init(pIni, "PTop14");
	m_PTop15.Init(pIni, "PTop15");
	m_PTop16.Init(pIni, "PTop16");
	m_PTop17.Init(pIni, "PTop17");
	m_PTop18.Init(pIni, "PTop18");
	m_PTop19.Init(pIni, "PTop19");
	m_PTop110.Init(pIni, "PTop110");
	m_PTop21.Init(pIni, "PTop21");
	m_PTop22.Init(pIni, "PTop22");
	m_PTop23.Init(pIni, "PTop23");
	m_PTop24.Init(pIni, "PTop24");
	m_PTop25.Init(pIni, "PTop25");
	m_PTop26.Init(pIni, "PTop26");
	m_PTop27.Init(pIni, "PTop27");
	m_PTop28.Init(pIni, "PTop28");
	m_PTop29.Init(pIni, "PTop29");
	m_PTop210.Init(pIni, "PTop210");
	m_PTop31.Init(pIni, "PTop31");
	m_PTop32.Init(pIni, "PTop32");
	m_PTop33.Init(pIni, "PTop33");
	m_PTop34.Init(pIni, "PTop34");
	m_PTop35.Init(pIni, "PTop35");
	m_PTop36.Init(pIni, "PTop36");
	m_PTop37.Init(pIni, "PTop37");
	m_PTop38.Init(pIni, "PTop38");
	m_PTop39.Init(pIni, "PTop39");
	m_PTop310.Init(pIni, "PTop310");
	m_PTop41.Init(pIni, "PTop41");
	m_PTop42.Init(pIni, "PTop42");
	m_PTop43.Init(pIni, "PTop43");
	m_PTop44.Init(pIni, "PTop44");
	m_PTop45.Init(pIni, "PTop45");
	m_PTop46.Init(pIni, "PTop46");
	m_PTop47.Init(pIni, "PTop47");
	m_PTop48.Init(pIni, "PTop48");
	m_PTop49.Init(pIni, "PTop49");
	m_PTop410.Init(pIni, "PTop410");
	m_PTop51.Init(pIni, "PTop51");
	m_PTop52.Init(pIni, "PTop52");
	m_PTop53.Init(pIni, "PTop53");
	m_PTop54.Init(pIni, "PTop54");
	m_PTop55.Init(pIni, "PTop55");
	m_PTop56.Init(pIni, "PTop56");
	m_PTop57.Init(pIni, "PTop57");
	m_PTop58.Init(pIni, "PTop58");
	m_PTop59.Init(pIni, "PTop59");
	m_PTop510.Init(pIni, "PTop510");
	m_PTop61.Init(pIni, "PTop61");
	m_PTop62.Init(pIni, "PTop62");
	m_PTop63.Init(pIni, "PTop63");
	m_PTop64.Init(pIni, "PTop64");
	m_PTop65.Init(pIni, "PTop65");
	m_PTop66.Init(pIni, "PTop66");
	m_PTop67.Init(pIni, "PTop67");
	m_PTop68.Init(pIni, "PTop68");
	m_PTop69.Init(pIni, "PTop69");
	m_PTop610.Init(pIni, "PTop610");
	m_PTop71.Init(pIni, "PTop71");
	m_PTop72.Init(pIni, "PTop72");
	m_PTop73.Init(pIni, "PTop73");
	m_PTop74.Init(pIni, "PTop74");
	m_PTop75.Init(pIni, "PTop75");
	m_PTop76.Init(pIni, "PTop76");
	m_PTop77.Init(pIni, "PTop77");
	m_PTop78.Init(pIni, "PTop78");
	m_PTop79.Init(pIni, "PTop79");
	m_PTop710.Init(pIni, "PTop710");

}



int KUiTopTKNew::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	return KWndWindow::WndProc(uMsg, uParam, nParam);;
}




void KUiTopTKNew::PaintWindow()
{
KWndImage::PaintWindow();
	if (g_pCoreShell)
	{
		if (g_pRepresentShell)
		{
		KRULine line[2];
		line[0].Color.Color_dw = 0xcca0ffff;
		line[0].oPosition.nX = 150;
		line[0].oPosition.nY = 167;
		line[0].oEndPos.nX = 730;
		line[0].oEndPos.nY = 167;

		line[1].Color.Color_dw = 0xcca0ffff;
		line[1].oPosition.nX = 150;
		line[1].oPosition.nY = 287;
		line[1].oEndPos.nX = 730;
		line[1].oEndPos.nY = 287;

		

		g_pRepresentShell->DrawPrimitives(2, line, RU_T_LINE, true);
		}
	}
}



void KUiTopTKNew::Show()
{
	KWndImage::Show();

}


void KUiTopTKNew::Hide()
{

	KWndImage::Hide();

}


