// -------------------------------------------------------------------------
//	文件名		：	交易确认框
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-5-28
// -------------------------------------------------------------------------
#ifndef __UiShopName_H__
#define __UiShopName_H__

#include "../elem/wndtext.h"
#include "../elem/wndedit.h"
#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndlabeledbutton.h"
#include "../../../core/src/gamedatadef.h"



class KUiShopName : protected KWndImage
{
public:
	//----界面面板统一的接口函数----
	static KUiShopName*	OpenWindow();	
	static KUiShopName*	GetIfVisible();				//如果窗口正被显示，则返回实例指针
	static void			CloseWindow(bool bDestroy);		//关闭窗口
	static int			SavePrivateSetting(KIniFile* pFile);
	static KUiShopName*			LoadPrivateSetting(KIniFile* pFile);
private:
	KUiShopName();
	~KUiShopName() {}
	void 	Initialize();								//初始化
	void	Show();
	void	Hide();
	void	LoadScheme(const char* pScheme);			//载入界面方案
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();

private:
	static KUiShopName*	m_pSelf;
	
	KWndEdit32    m_StringEdit;            

    KWndLabeledButton  m_BtnDone, m_BtnCancel;


};


#endif // __UiTradeConfirm_H__