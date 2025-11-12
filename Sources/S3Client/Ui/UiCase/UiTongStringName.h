/*******************************************************************************
File        : UiTongCreateSheet.h
Creator     : Fyt(Fan Zhanpeng)
create data : 08-29-2003(mm-dd-yyyy)
Description : 创建帮会的表单
********************************************************************************/


#if !defined(AFX_KUITONGCREATESHEET_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_)
#define AFX_KUITONGCREATESHEET_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_

/*---------------------------*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*------------------------------------------*/
#include "../elem/wndtext.h"
#include "../elem/wndedit.h"
#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndlabeledbutton.h"

/*---------------------------------------------------------*/
class KUiTongStringName : KWndImage
{
public:
	KUiTongStringName();
	virtual ~KUiTongStringName();

	static        KUiTongStringName* OpenWindow(int nSelect,char *nString);  //打开窗口
	static        KUiTongStringName* GetIfVisible();//如果窗口正被显示，则返回实例指针
	static void   CloseWindow(bool bDestory = TRUE); //关闭窗口，同时可以选则是否删除对象实例
	static void   LoadScheme(const char* pScheme);   //载入界面方案

public:

	static void   LinkToMainWindow(                 //建立这个界面和主界面的关联
		                      KWndWindow *pMain);
private:
	static        KUiTongStringName *ms_pSelf;


private:
	void          Initialize();               //初始化
	                      /*------------------------------------------------*/
	                                          //窗口函数
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

private:
	void          AlignmentButtonCheck(       //正中邪CheckBox的管理函数
		                               NPCCAMP eSide);

	void          OnDone();                   //响应确定按钮被按下

private:

	KWndEdit32    m_EditTongName;  

	KWndText32	  m_StringBoxName;
	KWndEdit32    m_EditTongNumber;

    KWndLabeledButton
		          m_BtnDone, m_BtnCancel; 
	int           m_nSelectTong; 
	char          m_nStringTong[32];
	KWndWindow              *m_pMain;
	
};


#endif // !defined(AFX_KUITONGCREATESHEET_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_)
