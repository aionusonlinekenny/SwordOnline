#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndLabeledButton.h"

struct KUiObjAtRegion;
class KUiGetString3 : protected KWndImage
{
public:
	//----界面面板统一的接口函数----
	static KUiGetString3* OpenWindow(int nPoint, int nMaxPoint, const char* pszTitle, KWndWindow* pRequester, unsigned int uParam);
	static KUiGetString3*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//载入界面方案
	static void			CloseWindow(bool bDestroy);		//关闭窗口
private:
	KUiGetString3();
	~KUiGetString3() {}
	void	Initialize();								//初始化
	void	Show();
	void	Hide();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();
	void	OnCheckInput();
	void 	OnIncrease();
	void 	OnDecrease();	
private:
	static KUiGetString3*	m_pSelf;
	KWndEdit512				m_StringEdit;
	KWndLabeledButton		m_OkBtn;
	KWndLabeledButton		m_CancelBtn;
	KWndButton				m_Increase;
	KWndButton				m_Decrease;	
	unsigned int			m_uRequesterParam;
	KWndWindow*				m_pRequester;	
	KWndText32				m_Title;
	char					szTitle[16];
	int						m_nMaxPoint;	
};

