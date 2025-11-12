/*****************************************************************************************
//	界面窗口体系结构--显示图形局部的窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-25
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "WndImagePart.h"
#include "KSprite.h"

#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

//--------------------------------------------------------------------------
//	功能：构造函数
//--------------------------------------------------------------------------
KWndImagePart::KWndImagePart()
{
	m_Text[0] = 0;
	IR_InitUiImagePartRef(m_Image);
}

void KWndImagePart::Clone(KWndImagePart* pCopy)
{
	if (pCopy)
	{
		KWndWindow::Clone(pCopy);
		pCopy->m_Image	= m_Image;
		pCopy->SetPart(0, 100);
	}
}

//--------------------------------------------------------------------------
//	功能：初始化窗口
//--------------------------------------------------------------------------
int KWndImagePart::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "PartType", 0, &m_Image.nDivideFashion);
		if (m_Image.nDivideFashion < IDF_LEFT_TO_RIGHT || m_Image.nDivideFashion > IDF_BOTTOM_TO_TOP)
			m_Image.nDivideFashion = IDF_LEFT_TO_RIGHT;
		m_Image.uImage = 0;
		m_Image.nISPosition = 0;
		int nValue;
		pIniFile->GetInteger(pSection, "ImgType", 0, &nValue);
		if (nValue == 1)
		{
			m_Image.nType = ISI_T_BITMAP16;
			m_Image.bRenderStyle = IMAGE_RENDER_STYLE_OPACITY;
		}
		else
		{
			m_Image.nType = ISI_T_SPR;
			m_Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		}
		pIniFile->GetString(pSection, "Image", "" , m_Image.szImage, sizeof(m_Image.szImage));
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
//	功能：窗体绘制
//--------------------------------------------------------------------------
void KWndImagePart::PaintWindow()
{
	KWndWindow::PaintWindow();
	if (g_pRepresentShell)
	{
		m_Image.oPosition.nX = m_nAbsoluteLeft;
		m_Image.oPosition.nY = m_nAbsoluteTop;
		g_pRepresentShell->DrawPrimitives(1, &m_Image, RU_T_IMAGE_PART, true);
		
		if (m_Text[0])
		{
	unsigned char m_nFontSize = 12;
	int m_nTextLen = strlen(m_Text);

	KOutputTextParam	Param;
	Param.nX = m_nAbsoluteLeft;
	Param.nY = m_nAbsoluteTop;
	Param.nZ = TEXT_IN_SINGLE_PLANE_COORD;

	Param.nX += (m_Width - m_nTextLen * m_nFontSize / 2) / 2;
	if (Param.nX < m_nAbsoluteLeft)
		Param.nX = m_nAbsoluteLeft;

	Param.nY += (m_Height - m_nFontSize - 1) / 2;

	Param.nNumLine = 1;
	Param.Color = 0xFFFFFFFF;
	Param.BorderColor = 0xFF000000;
	g_pRepresentShell->OutputRichText(m_nFontSize, &Param, m_Text, m_nTextLen, m_Width);

			//g_pRepresentShell->OutputText(12, m_Text, -1, m_nAbsoluteLeft + 1, m_nAbsoluteTop + 1, 0xFF000000);
			//g_pRepresentShell->OutputText(12, m_Text, -1, m_nAbsoluteLeft, m_nAbsoluteTop, 0xFFFFFFFF);
		}
	}
}

//--------------------------------------------------------------------------
//	功能：根据部分值与全值的比例关系设置绘制图的哪部分区域
//--------------------------------------------------------------------------
void KWndImagePart::SetPart(int nPartValue, int nFullValue, bool nDraw)
{
	if (nDraw)
	 sprintf(m_Text,"%d/%d", nPartValue, nFullValue);
	else
	 m_Text[0] = 0;

	if (nFullValue)
		IR_UpdateImagePart(m_Image, nPartValue, nFullValue);
}
