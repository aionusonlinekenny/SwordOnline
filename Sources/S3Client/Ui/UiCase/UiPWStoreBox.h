// -------------------------------------------------------------------------
//	文件名		：	交易确认框
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-5-28
// -------------------------------------------------------------------------
#ifndef __UiPWStoreBox_H__
#define __UiPWStoreBox_H__

#include "../elem/wndtext.h"
#include "../elem/wndedit.h"
#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndlabeledbutton.h"
#include "../../../core/src/gamedatadef.h"



class KUiPWStoreBox : protected KWndImage
{
public:
	//----界面面板统一的接口函数----
	static KUiPWStoreBox*	OpenWindow();	
	static KUiPWStoreBox*	GetIfVisible();				//如果窗口正被显示，则返回实例指针
	static void			CloseWindow(bool bDestroy);		//关闭窗口
private:
	KUiPWStoreBox();
	~KUiPWStoreBox() {}
	void 	Initialize();								//初始化
	void	Show();
	void	Hide();
	void	LoadScheme(const char* pScheme);			//载入界面方案
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();

private:
	static KUiPWStoreBox*	m_pSelf;


	KWndEdit32    m_NumberPW;
	KWndEdit32    m_NumberEdit;
	KWndEdit32    m_NumberEditTrue;

    KWndLabeledButton  m_BtnDone, m_BtnCancel;


};


#endif // __UiTradeConfirm_H__