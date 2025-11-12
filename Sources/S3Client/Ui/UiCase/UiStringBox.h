// -------------------------------------------------------------------------
//	文件名		：	交易确认框
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-5-28
// -------------------------------------------------------------------------
#ifndef __UiStringBox_H__
#define __UiStringBox_H__

#include "../elem/wndtext.h"
#include "../elem/wndedit.h"
#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndlabeledbutton.h"
#include "../../../core/src/gamedatadef.h"



class KUiStringBox : protected KWndImage
{
public:
	//----界面面板统一的接口函数----
	static KUiStringBox*	OpenWindow(int nId,char* szTitle);	
	static KUiStringBox*	GetIfVisible();				//如果窗口正被显示，则返回实例指针
	static void			CloseWindow(bool bDestroy);		//关闭窗口
private:
	KUiStringBox();
	~KUiStringBox() {}
	void 	Initialize();								//初始化
	void	Show();
	void	Hide();
	void	LoadScheme(const char* pScheme);			//载入界面方案
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();

private:
	static KUiStringBox*	m_pSelf;

	KWndText32	  m_StringBoxName;
	
	KWndEdit32    m_StringEdit;  
	KWndEdit32    m_NumberEdit;

    KWndLabeledButton  m_BtnDone, m_BtnCancel;

	int           m_nID;
	char          m_szTitle[32];

};


#endif // __UiTradeConfirm_H__