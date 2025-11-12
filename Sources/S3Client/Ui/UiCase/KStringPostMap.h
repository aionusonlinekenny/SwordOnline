// -------------------------------------------------------------------------
//	文件名		：	交易确认框
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-5-28
// -------------------------------------------------------------------------
#ifndef __KStringPostMap_H__
#define __KStringPostMap_H__

#include "../elem/wndtext.h"
#include "../elem/wndedit.h"
#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndlabeledbutton.h"
#include "../../../core/src/gamedatadef.h"



class KUiStringPostMap : protected KWndImage
{
public:
	//----界面面板统一的接口函数----
	
	static KUiStringPostMap*	OpenWindow();	
	static KUiStringPostMap*	GetIfVisible();				
	static void			CloseWindow(bool bDestroy);		
	static void			SetPositionOpen(int nX,int nY);
private:
	KUiStringPostMap();
	~KUiStringPostMap() {}
	

	
	void 	Initialize();								//初始化
	void	Show();
	void	Hide();
	void	LoadScheme(const char* pScheme);			//载入界面方案
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();

private:
	static KUiStringPostMap*	m_pSelf;

	KWndText32	  m_StringBoxName;
	
	KWndEdit32    m_StringEdit;  

	KWndImage     m_IMGEdit;

    KWndLabeledButton  m_BtnDone, m_BtnCancel;

};


#endif 