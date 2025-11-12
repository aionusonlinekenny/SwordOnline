/*****************************************************************************************
//	界面--储物箱界面
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-21
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../elem/WndButton.h"
#include "../elem/WndText.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"

struct KUiObjAtRegion;

class KUiSlectBoxCheck : protected KWndShowAnimate
{
public:
	static KUiSlectBoxCheck* OpenWindow(int nId1,int nId2,int nId3);		//打开窗口，返回唯一的一个类对象实例
	static KUiSlectBoxCheck* GetIfVisible();		//如果窗口正被显示，则返回实例指针
	static void			CloseWindow();		//关闭窗口
	static void			LoadScheme(const char* pScheme);//载入界面方案
private:
	KUiSlectBoxCheck() {}
	~KUiSlectBoxCheck() {}
	void	Initialize();							//初始化
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//窗口函数

private:
	static KUiSlectBoxCheck*		m_pSelf;
private:

	KWndButton			m_Button1Btn;
	KWndButton			m_Button2Btn;
	KWndButton			m_Button3Btn;
	char                m_szImage[6][128];
};