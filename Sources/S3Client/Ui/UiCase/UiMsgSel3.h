#ifndef __UiSayNew_H__
#define __UiSayNew_H__

#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndText.h"


struct KUiQuestionAndAnswer;
class KUiMsgSel3 : protected KWndShowAnimate
{
public:
	//----界面面板统一的接口函数----
	static KUiMsgSel3*	OpenWindow(KUiQuestionAndAnswer* pContent, int nImage);		//打开窗口，返回唯一的一个类对象实例
	static KUiMsgSel3*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//载入界面方案
	static void			CloseWindow(bool bDestroy);		//关闭窗口
private:
	KUiMsgSel3() {}
	~KUiMsgSel3() {}
	void	Show(KUiQuestionAndAnswer* pContent, int nImage);
	int		Initialize();								//初始化
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickMsg(int nMsg);		
	void	ChangeCurSel(bool bNext);
	virtual void	Breathe();
	int nValue;


private:
	static KUiMsgSel3*	m_pSelf;
	KWndScrollBar			m_InfoScroll;	
	KWndMessageListBox		m_InfoText;
	KScrollMessageListBox	m_MsgScrollList;
	KWndImage				m_Image;

	bool m_bAutoUp;
	bool m_bAutoDown;
	unsigned int	m_uLastScrollTime;
	
};
#endif // __UiMsgSel_H__