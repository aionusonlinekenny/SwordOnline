// -------------------------------------------------------------------------
//	文件名		：	UiToolsControlBar.h
//	创建者		：	万里
//	创建时间	：	2003-7-24 19:39:54
//	功能描述	：	
//
// -------------------------------------------------------------------------
#ifndef __UITOOLSCONTROLBAR_H__
#define __UITOOLSCONTROLBAR_H__

#pragma once
#include "../Elem/WndToolBar.h"
#include "../Elem/WndButton.h"

class KUiToolsControlBar : public KWndToolBar
{
public:
	//----界面面板统一的接口函数----
	static KUiToolsControlBar* OpenWindow();	//打开窗口，返回唯一的一个类对象实例
	static void				CloseWindow();		//关闭窗口
	static void				LoadScheme(const char* pScheme);//载入界面方案
	static void				DefaultScheme(const char* pScheme);//重新初始化界面
	static KUiToolsControlBar* GetSelf()	{return m_pSelf;}
//	static void	SwitchPKPopup();
//	static void	SwitchFastPK();

private:
	~KUiToolsControlBar() {}
	void	Initialize();							//初始化
	void	Breathe();
//	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	SwitchPKProcess(int nAction);

private:
	static KUiToolsControlBar*	m_pSelf;
	//KWndButton	m_Friend;
	//KWndButton	m_Options;
	enum CHOICE_PK
	{
		Exercises = 0,
		Fighting,
		Murder,
		Tongwar,
	};
};
#endif // __UITOOLSCONTROLBAR_H__