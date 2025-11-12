/*****************************************************************************************
//	Hop thoai dung nhap toa do
//	Copyright : PTTK
//	Author	:   Zin (DVK)
//	CreateTime:	2013/30/05
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../elem/WndButton.h"
#include "../elem/WndText.h"
#include "../elem/WndEdit.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"

#define CHEST_PW_MAX_VALUE	999999
#define CHEST_PW_MIN_VALUE	100000


struct KUiObjAtRegion;

class KUiFindPos : protected KWndShowAnimate
{
public:
	static KUiFindPos* OpenWindow();					//Mo hop thoai nhap toa do
	static KUiFindPos* GetIfVisible();				//Kiem tra xem co dang mo hop thoai hay khong
	static void			CloseWindow();					//Dong hop thoai nhap toa do
	static void			LoadScheme(const char* pScheme);//Load layout cua hop thoai nhap toa do
	static void	SetPositionOpen(int x, int y);

private:
	KUiFindPos() {}
	~KUiFindPos() {}
	void	Initialize();								//Khoi tao hop thoai
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//Xu ly trong qua trinh hop thoai dang chay
	void	OnOK();										//Gui goi tin mo khoa toi Server
	void	OnCheckInput();
	void	Breathe();
	void	UpdateData1();


private:
	static KUiFindPos*		m_pSelf;
private:
	KWndText80			m_Text;
	KWndEdit32			m_PosXInput;
	KWndEdit32			m_PosYInput;
	KWndText80			m_InfoText;
	KWndButton			m_OKBtn;
	KWndButton			m_CancelBtn;
};