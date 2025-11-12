// -------------------------------------------------------------------------
//	文件名		：	UiMsgSel.h
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-1-6
//	功能描述	：	带滚动条的消息选择
// -------------------------------------------------------------------------
#ifndef __UiMsgSel2_H__
#define __UiMsgSel2_H__

#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndText.h"

struct KUiQuestionAndAnswer;
struct KUiNpcSpr;

class KUiMsgSel2 : protected KWndShowAnimate
{
public:
	//----界面面板统一的接口函数----
	static KUiMsgSel2*	OpenWindow(KUiQuestionAndAnswer* pContent, KUiNpcSpr* pImage); 		//打开窗口，返回唯一的一个类对象实例
	static KUiMsgSel2*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//载入界面方案
	static void			CloseWindow(bool bDestroy);		//关闭窗口
private:
	KUiMsgSel2() {}
	~KUiMsgSel2() {}
	void	Show(KUiQuestionAndAnswer* pContent);
	int		Initialize();								//初始化
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickMsg(int nMsg);			//响应点击消息
	void	ChangeCurSel(bool bNext);
	virtual void	Breathe();

private:
	static KUiMsgSel2*	m_pSelf;
	KScrollMessageListBox	m_MsgScrollList;
	KWndMessageListBox		m_InfoText;
	KWndImage          	 	m_NpcSpr;  
	KWndScrollBar			m_InfoScroll;
	
	bool m_bAutoUp;
	bool m_bAutoDown;
	unsigned int	m_uLastScrollTime;
};


#endif // __UiMsgSel2_H__