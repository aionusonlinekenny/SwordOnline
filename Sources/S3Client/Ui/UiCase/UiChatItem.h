// -------------------------------------------------------------------------
//	Cua so item kenh chat -- coded by Ken Nguyen
// -------------------------------------------------------------------------
#ifndef __UiChatItem_H__
#define __UiChatItem_H__

#include "../Elem/WndImage.h"
#include "../Elem/WndButton.h"
class KUiChatItem : protected KWndImage
{
public:
	static KUiChatItem*	OpenWindow(int nIdx);	
	static KUiChatItem*	GetIfVisible();				
	static void			CloseWindow();	
private:
	KUiChatItem();
	~KUiChatItem() {}
	int		Initialize();							
	void	LoadScheme(const char* pScheme);		
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	PaintWindow();
	void	SetInfomation(int nIdx);
private:
	static KUiChatItem*	m_pSelf;
	unsigned int m_uTitleBgColor;
	int		m_nFontSize;
	int		m_nTitleLen;
	int     m_nMaxLineLen;
	int     m_nTitleLineNum;
	char    m_ObjTitle[1024];
	int		m_ImageWidth;
	int		m_ImageHeight;
	int		m_BorderPieces; //vien` khung
	KWndImage	m_ItemImage;
	KWndButton	m_CancelBtn;
};


#endif // __UiChatItem_H__