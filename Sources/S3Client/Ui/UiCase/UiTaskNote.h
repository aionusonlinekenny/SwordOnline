/* 
 * File:     UiTaskNote.h
 * Desc:     任务记事窗口
 * Author:   flying
 * Creation: 2003/7/18
 * 即将第二次参加新人培训
 */
//-----------------------------------------------------------------------------
#pragma once

#if !defined _UITASKNOTE
#define _UITASKNOTE

#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndPage.h"
#include "../Elem/WndLabeledButton.h"

// "Edit" with 2048 byte buffer
class KTaskEdit : public KWndEdit
{
private:
	char			m_Text[2048];
public:
	KTaskEdit();
};


class KUiTaskNote_System : public KWndPage
{
public:	
	void	Initialize();								//初始化
	void	LoadScheme(const char* pScheme);			//载入界面方案
	void	UpdateView();
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnDelete(int nIndex);

	KScrollMessageListBox	m_RecordList;
	KWndLabeledButton		m_BtnDelete;
};

class KUiTaskNote_Personal : public KWndPage
{
public:
	void	Initialize();								//初始化
	void	LoadScheme(const char* pScheme);			//载入界面方案
	void	UpdateData();								//更新数据	
	void	UpdateView();
	void	Show();
	void	Hide();
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnSave();
	KTaskEdit				m_Editor;	
	KWndLabeledButton		m_BtnSave;
	KWndScrollBar			m_ScrollBar;
};

class KUiTaskNote_Event : public KWndPage
{
public:	
	void	Initialize();							
	void	LoadScheme(const char* pScheme);			
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

};

class KUiTaskNote_Bind : public KWndPage
{
public:	
	void	Initialize();								//初始化
	void	LoadScheme(const char* pScheme);			//载入界面方案
	void	UpdateView();
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnDelete(int nIndex);

	KScrollMessageListBox	m_RecordList;
	KWndLabeledButton		m_BtnDelete;
};

class KUiTaskNote : public KWndPageSet
{
public:
	static KUiTaskNote*		OpenWindow();
	//关闭窗口，同时可以选则是否删除对象实例
	static void				CloseWindow(bool bDestroy);
	static KUiTaskNote*		GetIfVisible();
	// 由游戏循环唤醒
	static void				WakeUp(BYTE btType, const char* pSystemRecord, int nLen, unsigned int uValue);
	//载入界面方案
	static void				LoadScheme(const char* pScheme);
private:
	KUiTaskNote();
	virtual ~KUiTaskNote();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	Initialize();
	void	OnClose();
	void	InitData();		//初始化相关存档数据
	void	SaveData();		//保存数据
private:
	// here goes data.
	static KUiTaskNote*		m_pSelf;

	KUiTaskNote_System		m_SystemPad;
	KUiTaskNote_Personal	m_PersonalPad;
	KUiTaskNote_Event		m_EventPad;
	KUiTaskNote_Bind		m_BindPad;
	
	KWndLabeledButton		m_BtnSystem;
	KWndLabeledButton		m_BtnPersonal;
	KWndLabeledButton		m_BtnEvent;
	KWndLabeledButton		m_BtnBind;	
	KWndButton				m_BtnClose;
};

#endif