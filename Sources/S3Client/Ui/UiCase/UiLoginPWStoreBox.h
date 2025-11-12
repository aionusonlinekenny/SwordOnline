// -------------------------------------------------------------------------
//	文件名		：	交易确认框
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-5-28
// -------------------------------------------------------------------------
#ifndef __UiLoginPWStoreBox_H__
#define __UiLoginPWStoreBox_H__

#include "../elem/wndtext.h"
#include "../elem/wndedit.h"
#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndlabeledbutton.h"
#include "../../../core/src/gamedatadef.h"



class KUiLoginPWStoreBox : protected KWndImage
{
public:
	//----界面面板统一的接口函数----
	static KUiLoginPWStoreBox*	OpenWindow();	
	static KUiLoginPWStoreBox*	GetIfVisible();				//如果窗口正被显示，则返回实例指针
	static void			CloseWindow(bool bDestroy);		//关闭窗口
private:
	KUiLoginPWStoreBox();
	~KUiLoginPWStoreBox() {}
	void 	Initialize();								//初始化
	void	Show();
	void	Hide();
	void	LoadScheme(const char* pScheme);			//载入界面方案
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();

private:
	static KUiLoginPWStoreBox*	m_pSelf;


	KWndEdit32    m_NumberEdit;

	KWndButton    m_Btn1,m_Btn2,m_Btn3,m_Btn4,m_Btn5,m_Btn6,m_Btn7,m_Btn8,m_Btn9,m_Btn0,m_Btndc;

    KWndButton  m_BtnDone, m_BtnCancel;

};


#endif // __UiTradeConfirm_H__