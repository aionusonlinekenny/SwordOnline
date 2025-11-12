/*****************************************************************************************
//	界面窗口体系结构--切换显示状态时带移动控制的图形的窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-17
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "WndRect.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
extern iRepresentShell*	g_pRepresentShell;

void KWndRect::Clone(KWndRect* pCopy)
{
	if (pCopy)
	{
		KWndWindow::Clone(pCopy);
		pCopy->m_uColor = m_uColor;
	}
}

//初始化
int KWndRect::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		char	Buff[16];
		pIniFile->GetString(pSection, "RectColor", "", Buff, sizeof(Buff));
		m_uColor = (GetColor(Buff) & 0xffffff);
		int nAlpha;
		pIniFile->GetInteger(pSection, "RectColorAlpha", 128, &nAlpha);
		if (nAlpha > 255 || nAlpha < 0)
			m_uColor |= 0x16000000;
		else
			m_uColor |= (((255 - nAlpha) << 21) & 0xff000000);
		return true;
	}
	return false;
}

//窗体绘制
void KWndRect::PaintWindow()
{
	KWndWindow::PaintWindow();
	if (m_uColor && g_pRepresentShell)
	{


KRURect		Rect;
Rect.Color.Color_dw = m_uColor;
Rect.oPosition.nX = m_nAbsoluteLeft;
Rect.oPosition.nY = m_nAbsoluteTop;
Rect.oEndPos.nX = m_nAbsoluteLeft + m_Width;
Rect.oEndPos.nY = m_nAbsoluteTop + m_Height;
g_pRepresentShell->DrawPrimitives(1, &Rect,RU_T_RECT, true);
	}
}

//判断一个点是否在窗口范围内,传入的是绝对坐标
int KWndRect::PtInWindow(int x, int y)
{
	return 0;
}

void KWndRect::SetColor(unsigned int uColor)
{
	m_uColor = uColor;
}