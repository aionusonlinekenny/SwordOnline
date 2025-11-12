// -------------------------------------------------------------------------
//	Cua so item kenh chat -- coded by Ken Nguyen
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiChatItem.h"
#include "../UiSoundSetting.h"
#include "../../../Engine/src/Text.h"
#include "../../../core/src/coreshell.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iCoreShell*		g_pCoreShell;
extern iRepresentShell*	g_pRepresentShell;

#define	SCHEME_INI 	"ChatItemInfo.ini"

KUiChatItem* KUiChatItem::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	Mo 1 cua so moi de hien thi thong tin cua item tren kenh chat
//--------------------------------------------------------------------------

KUiChatItem* KUiChatItem::OpenWindow(int nIdx)
{
	if (!nIdx || !g_pCoreShell) return NULL;
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiChatItem;
		if (m_pSelf)
		{
			m_pSelf->Initialize();
		}
	}
	if (m_pSelf)
	{
		char szImage[80];
		if(g_pCoreShell->GetGameData(GDI_CHAT_ITEM_IMAGE, (unsigned int)&szImage, nIdx))
		m_pSelf->m_ItemImage.SetImage(ISI_T_SPR,szImage,true);
		int nWidth, nHeight;
		m_pSelf->m_ItemImage.GetSize(&nWidth,  &nHeight);
		m_pSelf->m_ImageWidth = nWidth + 5;
		m_pSelf->m_ImageHeight = nHeight +5;
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->SetInfomation(nIdx);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

KUiChatItem::KUiChatItem()
{
	m_uTitleBgColor= 0;
	m_nFontSize  = 12;
	m_nTitleLen    = 0;
	ZeroMemory(m_ObjTitle,sizeof(m_ObjTitle));
	m_ImageWidth = 0;
	m_ImageHeight = 0;
	m_BorderPieces = 0;
}

KUiChatItem* KUiChatItem::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void KUiChatItem::CloseWindow()
{
	if (m_pSelf)
	{
			m_pSelf->Destroy();
			m_pSelf = NULL;
	}
}

void KUiChatItem::SetInfomation(int nIdx)
{
	char	szTitle[1024];
	szTitle[0] = 0;
	int	nLenTitle = 0;
	m_nMaxLineLen = 0;
	g_pCoreShell->GetGameData(GDI_CHAT_ITEM_DESC, (unsigned int)nIdx, (int)&szTitle);
	nLenTitle = TEncodeText(szTitle, strlen(szTitle));
	if (nLenTitle > 0 && szTitle[0] && nLenTitle <= 1024)
	{
		memcpy(m_ObjTitle, szTitle, nLenTitle);
		m_nTitleLen = nLenTitle;
	}
	else
	{
		CloseWindow();
		return;
	}
	m_nTitleLineNum = TGetEncodedTextLineCount(m_ObjTitle, m_nTitleLen, 0, m_nMaxLineLen, m_nFontSize);
	m_Width = m_nFontSize * m_nMaxLineLen / 2 + m_ImageWidth;
	if(m_Width%12 != 0) m_Width += 12 - m_Width%12; //+ vao cho du~ vien`
	m_BorderPieces = m_Width/12; //so diem~ vien` khung
	m_Height = (m_nFontSize + 1) * (m_nTitleLineNum + 3);
	if(m_ImageHeight > m_Height)
	m_Height = m_ImageHeight;
	m_CancelBtn.SetSize(m_Width,m_Height-10);
}

void KUiChatItem::PaintWindow()
{
	if (g_pRepresentShell == NULL)
		return;

	KOutputTextParam	Param;
	Param.BorderColor = 0;
	Param.nZ = TEXT_IN_SINGLE_PLANE_COORD;

	KRUShadow Shadow;
	GetAbsolutePos(&Shadow.oPosition.nX, &Shadow.oPosition.nY);//
	GetSize(&Shadow.oEndPos.nX, &Shadow.oEndPos.nY);
	Shadow.oEndPos.nX += Shadow.oPosition.nX;
	Shadow.oEndPos.nY += Shadow.oPosition.nY;
	if(m_nTitleLen > 0)
	{
		Shadow.Color.Color_dw = m_uTitleBgColor;
		g_pRepresentShell->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);

		Param.nSkipLine = 0;
		Param.nNumLine = 1;
		Param.Color = 0xffffffff;
		Param.nY = Shadow.oPosition.nY + 12;
		int nLineLen;
		while(true)
		{
			if (TGetEncodedTextLineCount(m_ObjTitle, m_nTitleLen, 0, nLineLen, m_nFontSize, Param.nSkipLine, 1) == 0)
				break;
			Param.nX = m_Left + (m_Width-m_ImageWidth) / 2 - (nLineLen * m_nFontSize) / 4 + m_ImageWidth;
			g_pRepresentShell->OutputRichText(m_nFontSize, &Param, m_ObjTitle, m_nTitleLen, 0);
			Param.nSkipLine ++;
			Param.nY += m_nFontSize + 1;
		};
	}
//-------------------------Ve~ vien` tren va duoi khung - Ken Nguyen---------------------------
	KRUImage* Border = new KRUImage[m_BorderPieces*2];
	for (int i = 0; i < m_BorderPieces ; i++)
	{
		Border[i].nType = ISI_T_SPR;
		Border[i].Color.Color_b.a = 255;
		Border[i].bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		Border[i].uImage = 0;
		Border[i].nISPosition = IMAGE_IS_POSITION_INIT;
		Border[i].bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
		strcpy(Border[i].szImage, "\\Spr\\linhtinh\\itemchatborder.spr");
		Border[i].nFrame = 0;
		Border[i].oPosition.nX = m_Left + i*12;
		Border[i].oPosition.nY = m_Top;
		Border[i].oPosition.nZ = 0;
		g_pRepresentShell->DrawPrimitives(1, &Border[i], RU_T_IMAGE, TRUE);
	}
	for (int j = m_BorderPieces; j < m_BorderPieces*2 ; j++)
	{
		Border[j].nType = ISI_T_SPR;
		Border[j].Color.Color_b.a = 255;
		Border[j].bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		Border[j].uImage = 0;
		Border[j].nISPosition = IMAGE_IS_POSITION_INIT;
		Border[j].bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
		strcpy(Border[j].szImage, "\\Spr\\linhtinh\\itemchatborder.spr");
		Border[j].nFrame = 0;
		Border[j].oPosition.nX = m_Left + (j-m_BorderPieces)*12;
		Border[j].oPosition.nY = m_Top + m_Height - 10;
		Border[j].oPosition.nZ = 0;
		g_pRepresentShell->DrawPrimitives(1, &Border[j], RU_T_IMAGE, TRUE);
	}
//-------------------------Bong' Item----------------------------------------
	KRUShadow ItemShadow;
	ItemShadow.oPosition.nX = m_Left;
	ItemShadow.oPosition.nY = m_Top + 10;
	ItemShadow.oEndPos.nX = ItemShadow.oPosition.nX + m_ImageWidth;
	ItemShadow.oEndPos.nY = ItemShadow.oPosition.nY + m_ImageHeight;
	ItemShadow.Color.Color_dw = ((GetColor("0,93,57") & 0xffffff) | 0x0a000000);
	g_pRepresentShell->DrawPrimitives(1, &ItemShadow, RU_T_SHADOW, true);
//-----------------------Khung item------------------------------------------
	KRURect	rect;
	rect.Color.Color_dw = 0xff00ffff;
	rect.oPosition.nX = ItemShadow.oPosition.nX;
	rect.oPosition.nY = ItemShadow.oPosition.nY;
	rect.oEndPos.nZ = rect.oPosition.nZ = 0;
	rect.oEndPos.nX = ItemShadow.oEndPos.nX;
	rect.oEndPos.nY = ItemShadow.oEndPos.nY;
	g_pRepresentShell->DrawPrimitives(1, &rect, RU_T_RECT, true);
	if (Border) delete [] Border;
//--------------------------------------------------------------------------------

}
int KUiChatItem::Initialize()
{
	AddChild(&m_ItemImage);
	AddChild(&m_CancelBtn);
	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	return true;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void KUiChatItem::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		m_ItemImage.Init(&Ini, "ItemImage");
		m_CancelBtn.Init(&Ini, "CancelBtn");
	}
	m_uTitleBgColor = ((GetColor("0,30,19") & 0xffffff) | 0x0a000000);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
int KUiChatItem::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
		CloseWindow();
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}
