#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"

struct KUiTimeBoxInfo;

class KUiTimeBox : protected KWndImage
{
public:
	static KUiTimeBox* OpenWindow(const char* pszTitle, int nTime, char*pszAction);		// gui vao 3 bien so
	static KUiTimeBox*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	
	static void			CloseWindow(bool bDestroy);
	static void			OnOk();	
private:
	KUiTimeBox() {};
	~KUiTimeBox() {}
	void	Initialize();	
	void	Show();	
	void	Breathe();	
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
private:	
	static KUiTimeBox*	m_pSelf;
	KWndText32			m_Title;
	char 				szAction[32];	
	KWndText32 			m_Time;
	int 				m_nTime;
	int					nRunTime;
	KWndImageTextButton	m_Image;	
};

