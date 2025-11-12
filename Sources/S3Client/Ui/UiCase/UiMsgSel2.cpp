// -------------------------------------------------------------------------
//	文件名		：	UiMsgSel.h
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-1-6
//	功能描述	：	带滚动条的消息选择
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "UiMsgSel2.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI	"UiMsgSel2.ini"

KUiMsgSel2* KUiMsgSel2::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiMsgSel2* KUiMsgSel2::OpenWindow(KUiQuestionAndAnswer* pContent, KUiNpcSpr *pImage) 
{ 
    if (m_pSelf == NULL) 
    { 
        m_pSelf = new KUiMsgSel2; 
        if (m_pSelf) 
        { 
            m_pSelf->Initialize(); 
            m_pSelf->m_NpcSpr.SetImage(ISI_T_SPR, pImage->ImageFile, true); 
            int nWidth, nHeight; 
            m_pSelf->m_NpcSpr.GetSize(&nWidth,  &nHeight); 
            m_pSelf->m_NpcSpr.SetPosition(80 - nWidth/2, 80 - nHeight/2); 
            m_pSelf->m_NpcSpr.SetMaxFrame(pImage->MaxFrame); 
        } 
    } 
    if (m_pSelf) 
    {   m_pSelf->m_NpcSpr.SetImage(ISI_T_SPR,pImage->ImageFile,true); 
        int nWidth, nHeight; 
        m_pSelf->m_NpcSpr.GetSize(&nWidth,  &nHeight); 
        m_pSelf->m_NpcSpr.SetPosition(80 -nWidth/2, 80 -nHeight/2); 
        m_pSelf->m_NpcSpr.SetMaxFrame(pImage->MaxFrame); 
        UiSoundPlay(UI_SI_WND_OPENCLOSE); 
        m_pSelf->BringToTop(); 
        m_pSelf->Show(pContent); 
    } 
    return m_pSelf; 
}
//--------------------------------------------------------------------------
//	功能：关闭销毁窗口
//--------------------------------------------------------------------------
void KUiMsgSel2::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->m_bAutoUp = false;
		m_pSelf->m_bAutoDown = false;

		Wnd_ReleaseExclusive((KWndWindow*)m_pSelf);
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();		
	}
}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
int KUiMsgSel2::Initialize()
{
	AddChild(&m_NpcSpr); // d騨g th阭 
	AddChild(&m_MsgScrollList);
	AddChild(&m_InfoText);
	AddChild(&m_InfoScroll);	
	m_InfoText.SetScrollbar(&m_InfoScroll);
	
	m_Style &= ~WND_S_VISIBLE;
	Wnd_AddWindow(this, WL_TOPMOST);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	return true;
}

//--------------------------------------------------------------------------
//	功能：载入窗口的界面方案
//--------------------------------------------------------------------------
void KUiMsgSel2::LoadScheme(const char* pScheme) 
{ 
    if (m_pSelf == NULL) 
        return; 
    char        Buff[128]; 
    KIniFile    Ini; 
    sprintf(Buff, "%s\\"SCHEME_INI, pScheme); 
    if (Ini.Load(Buff)) 
    { 
        m_pSelf->Init(&Ini, "Main"); 
        m_pSelf->m_MsgScrollList.Init(&Ini, "Select"); 
        m_pSelf->m_InfoText.Init(&Ini, "InfoText"); 
        m_pSelf->m_NpcSpr.Init(&Ini, "NpcSpr");
		m_pSelf->m_InfoScroll.Init(&Ini, "InfoScroll");		
    } 
}  

//--------------------------------------------------------------------------
//	功能：窗口消息函数
//--------------------------------------------------------------------------
int KUiMsgSel2::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			int x = LOWORD(nParam) - m_nAbsoluteLeft;
			int y = HIWORD(nParam) - m_nAbsoluteTop;
			if (x >= 0 && x <= m_Width &&
				y >= 0 && y <= m_Height)
			{
				int nMLeft = 0;
				int nMTop = 0;
				m_MsgScrollList.GetMessageListBox()->GetPosition(&nMLeft, &nMTop);
				int nMWidth = 0;
				int nMHeight = 0;
				m_MsgScrollList.GetMessageListBox()->GetSize(&nMWidth, &nMHeight);
				if (x >= nMLeft && x <= nMLeft + nMWidth)
				{
					if (y <= nMTop && y >= nMTop - 50)
						m_bAutoUp = true;
					else
						m_bAutoUp = false;

					if (y >= nMTop + nMHeight && y <= nMTop + nMHeight + 50)
						m_bAutoDown = true;
					else
						m_bAutoDown = false;
				}
			}
		}
		break;
	case WND_N_LIST_ITEM_HIGHLIGHT:
		m_bAutoUp = false;
		m_bAutoDown = false;
		if (nParam >= 0)
			m_MsgScrollList.GetMessageListBox()->SetCurSel(nParam);
		else
		{
			ChangeCurSel(true);	//经实际测试在nParam < 0 时是向下
			nRet = 1;
		}
		break;
	case WND_N_LIST_ITEM_ACTIVE:
		m_bAutoUp = false;
		m_bAutoDown = false;
		if (nParam >= 0)
			OnClickMsg(nParam);
		break;
	case WND_N_SCORLLBAR_POS_CHANGED:
		if (uParam == (unsigned int)(KWndWindow*)&m_InfoScroll)
			m_InfoText.SetFirstShowLine(nParam);
		break;		
	case WM_KEYDOWN:
		m_bAutoUp = false;
		m_bAutoDown = false;
		if (uParam == VK_RETURN)
		{
			nRet = m_MsgScrollList.GetMessageListBox()->GetCurSel();
			if (nRet > 0)
				OnClickMsg(nParam);
			nRet = 1;
		}
		else if (uParam == VK_UP)
		{
			ChangeCurSel(false);
			nRet = 1;
		}
		else if (uParam == VK_DOWN)
		{
			ChangeCurSel(true);
			nRet = 1;
		}
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
		break;
	}
	return 0;
}

//--------------------------------------------------------------------------
//	功能：响应点击按钮
//--------------------------------------------------------------------------
void KUiMsgSel2::OnClickMsg(int nMsg)
{
	CloseWindow(false);
	m_MsgScrollList.GetMessageListBox()->Clear();
	g_pCoreShell->OperationRequest(GOI_QUESTION_CHOOSE, 0, nMsg);
}

void KUiMsgSel2::ChangeCurSel(bool bNext)
{
	int nRet = m_MsgScrollList.GetMessageListBox()->GetCurSel();
	if (bNext)
	{
		if (nRet < m_MsgScrollList.GetMessageListBox()->GetMsgCount() - 1)
			m_MsgScrollList.GetMessageListBox()->SetCurSel(nRet + 1);
	}
	else
	{
		if (nRet  > 0)
			m_MsgScrollList.GetMessageListBox()->SetCurSel(nRet - 1);
	}
}

//--------------------------------------------------------------------------
//	功能：显示窗口
//--------------------------------------------------------------------------
void KUiMsgSel2::Show(KUiQuestionAndAnswer* pContent)
{
	if (pContent && pContent->AnswerCount >= 0)
	{
		m_MsgScrollList.GetMessageListBox()->Clear();
		m_InfoText.Clear();
		m_InfoText.AddOneMessage(pContent->Question, pContent->QuestionLen);
		int i = 0;
		if (pContent->AnswerCount <= 0)
		{
			m_MsgScrollList.GetMessageListBox()->AddOneMessage("K誸 th骳 i tho筰.", -1);
		}
		else
		{
			for (i = 0; i < pContent->AnswerCount ; i++)
			{
				if (!m_MsgScrollList.GetMessageListBox()->AddOneMessage(pContent->Answer[i].AnswerText, pContent->Answer[i].AnswerLen))
					m_MsgScrollList.GetMessageListBox()->AddOneMessage("[Error]", -1);
			}				
		}
	}
	KWndShowAnimate::Show();
	Wnd_SetExclusive((KWndWindow*)this);

	m_uLastScrollTime = IR_GetCurrentTime();
	m_bAutoUp = false;
	m_bAutoDown = false;
}

void KUiMsgSel2::Breathe()
{
	if (m_NpcSpr.IsVisible()) 
        m_NpcSpr.NextFrame();  
	if (m_bAutoUp)
	{
		if (IR_IsTimePassed(200, m_uLastScrollTime))
		{
			ChangeCurSel(false);
			m_uLastScrollTime = IR_GetCurrentTime();
		}
	}

	if (m_bAutoDown)
	{
		if (IR_IsTimePassed(200, m_uLastScrollTime))
		{
			ChangeCurSel(true);
			m_uLastScrollTime = IR_GetCurrentTime();
		}
	}
}