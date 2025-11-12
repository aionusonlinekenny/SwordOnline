#include "../Elem/WndImage.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndPureTextBtn.h"
#include "KTabFile.h"

#define MAX_BUFF_ROLE			20

struct KStateControl;
class KUiPlayerControlBar : KWndImage
{
public:

	static        KUiPlayerControlBar* OpenWindow(); 
	static        KUiPlayerControlBar* GetIfVisible();
	static void   CloseWindow(bool bDestory = TRUE); 
	static void   LoadScheme(const char* pScheme);
private:
	KUiPlayerControlBar();
	~KUiPlayerControlBar();
private:
	static        KUiPlayerControlBar *m_pSelf;

private:
	void    Initialize();       
	void	Breathe();
	void	Add(int nSkillPos, KStateControl* pStateSkill);
	void	Release();

private:
	KWndButton 	m_BuffImage[MAX_BUFF_ROLE];
	KWndText32 	m_txtBuffTime[MAX_BUFF_ROLE];

	KIniFile		Ini;
	int				m_BuffListCount;
	int				m_nListCount;
	KStateControl*	m_pStateSkill;
};
