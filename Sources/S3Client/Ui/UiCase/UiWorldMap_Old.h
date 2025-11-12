
#pragma once

#include "../Elem/WndImage.h"
#include "../Elem/UiImage.h"

class KUiWorldmap : protected KWndImage
{
public:
	static KUiWorldmap* OpenWindow();		
	static void			CloseWindow();		
	static KUiWorldmap*	GetIfVisible();
	void				LoadScheme(const char* pScheme);	

private:
	KUiWorldmap() {}
	~KUiWorldmap() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	Breathe();			
	void	UpdateData();
private:
	static KUiWorldmap* m_pSelf;
	KWndImage			m_Sign;
};