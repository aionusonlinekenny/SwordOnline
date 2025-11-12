/*****************************************************************************************
//	界面--游戏显示玩家信息主界面
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-16
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"
#include <crtdbg.h>
#include "UiPlayerBar.h"
#include "UiStatus.h"
#include "UiSkillTree.h"
#include "UiEscDlg.h"
//#include "UiSelColor.h"
#include "UiMsgCentrePad.h"
#include "UiFaceSelector.h"
#include "UiMiniMap.h"
#include "UiHelper.h"
#include "UiAuto.h"
#include "../UiCase/UiShopGold.h"
#include "../ShortcutKey.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/gamedatadef.h"
#include "../../../core/src/MsgGenreDef.h"
#include "../../../core/src/CoreObjGenreDef.h"
#include "../../../core/src/coreshell.h"
#include "../../TextCtrlCmd/TextCtrlCmd.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Engine/src/Text.h"
#include "../../../Engine/src/KDebug.h"
#include "../../../Headers/KRelayProtocol.h"
#include "../../../Headers/KProtocolDef.h"
#include "../../../Headers/KProtocol.h"
#include "malloc.h"
#include "UiChatCentre.h"
#include "../UiChatPhrase.h"
#include "../../Login/Login.h"
#include "../ChatFilter.h"

extern CChatFilter g_ChatFilter;
extern KUiChatPhrase g_UiChatPhrase;

extern iCoreShell*		g_pCoreShell;
extern iRepresentShell*	g_pRepresentShell;

#define	SCHEME_INI			"UiPlayerBar.ini"
#define	SCHEME_INI_MINI		"玩家信息主界面最小化.ini"
#define GAME_LOGO			"Vo Lam Truyen Ky"
#define	SWITCH_LOGO_INTERVAL	5000

#define		SEL_CHANNEL_MENU		1
#define		SEL_PHRASE_MENU			2




#include "time.h"

int GetFormatedTimeString(struct tm* ptm, char* pString)
{
	return sprintf(pString, "%02d:%02d | %02d-%02d-%d",
			ptm->tm_hour,
			ptm->tm_min,
			ptm->tm_mday,
			ptm->tm_mon + 1,
			ptm->tm_year + 1900);
}

GameWorld_DateTime::GameWorld_DateTime()
{
	m_bRegionName   = false;
	m_bServerName   = false;
	m_bLogo    		= false;
	m_bNetStatus    = false;
	m_uLastSwitchTime = IR_GetCurrentTime();
}

IMPLEMENT_COMCLASS(GameWorld_DateTime)
void GameWorld_DateTime::OnButtonClick()
{
}

int	GameWorld_DateTime::Init(KIniFile* pIniFile, const char* pSection)
{
	if(KWndLabeledButton::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "SmoothPing", 0, &m_nSmoothPing);
		pIniFile->GetInteger(pSection, "CrowdPing", 0, &m_nCrowdPing);
		pIniFile->GetInteger(pSection, "BlockPing", 0, &m_nBlockPing);

		pIniFile->GetString(pSection, "SmoothMsg", "", m_szSmoothMsg, sizeof(m_szSmoothMsg));
		pIniFile->GetString(pSection, "CrowdMsg", "", m_szCrowdMsg, sizeof(m_szCrowdMsg));
		pIniFile->GetString(pSection, "BlockMsg", "", m_szBlockMsg, sizeof(m_szBlockMsg));
		g_LoginLogic.GetRegionServer(m_szRegionName, m_szServerName);

		char Buff[16];
		pIniFile->GetString(pSection, "Color", "0,0,0", Buff, sizeof(Buff));
		m_uDefaultColor = GetColor(Buff);
		pIniFile->GetString(pSection, "SmoothColor", "0,0,0", Buff, sizeof(Buff));
		m_uSmoothColor = GetColor(Buff);
		pIniFile->GetString(pSection, "CrowdColor", "0,0,0", Buff, sizeof(Buff));
		m_uCrowdColor = GetColor(Buff);
		pIniFile->GetString(pSection, "BlockColor", "0,0,0", Buff, sizeof(Buff));
		m_uBlockColor = GetColor(Buff);
	}
	return 1;
}

void GameWorld_DateTime::UpdateData()
{
	char szTime[64];
	if(!m_bRegionName && !m_bServerName && !m_bLogo && !m_bNetStatus)
	{	
		time_t curtm = ::time(NULL);
		struct tm* ptm = localtime(&curtm);
		int nLen = GetFormatedTimeString(ptm, szTime);
		SetLabelColor(m_uDefaultColor);
		SetLabel(szTime);
	}

	if (IR_IsTimePassed(SWITCH_LOGO_INTERVAL, m_uLastSwitchTime))
	{
		if(m_bRegionName)
		{
			m_bServerName = true;
			SetLabelColor(m_uDefaultColor);
			SetLabel(m_szServerName);
			m_bRegionName = false;
		}
		else if(m_bServerName)
		{
			m_bServerName = false;
			UpdateNetStatus();
			m_bNetStatus = true;
		}
		else if(m_bNetStatus)
		{
			m_bNetStatus = false;
		}
		else
		{
			SetLabelColor(m_uDefaultColor);
			SetLabel(m_szRegionName);
			m_bRegionName = true;
		}
	}

	if (m_Flag & WNDBTN_F_OVER)
	{
		m_bLogo = false;
		m_bNetStatus = false;
		m_uLastSwitchTime = IR_GetCurrentTime();
	}

	NextFrame();
}

void GameWorld_DateTime::UpdateNetStatus()
{
	int i, nPing;
	char Number[32], Buff[32];
	unsigned int uDefaultColor;

	if(g_pCoreShell) nPing = g_pCoreShell->GetPing();
	itoa(nPing, Number, 10);

	if(nPing >= 0 && nPing <= m_nSmoothPing)
	{
		i = strlen(m_szSmoothMsg);
		memcpy(Buff, m_szSmoothMsg, i);
		Buff[i] = ' ';
		memcpy(Buff + i + 1, Number, strlen(Number) + 1);
		uDefaultColor = m_uSmoothColor;
	}
	else if(nPing > m_nSmoothPing && nPing <= m_nCrowdPing)
	{
		i = strlen(m_szCrowdMsg);
		memcpy(Buff, m_szCrowdMsg, i);
		Buff[i] = ' ';
		memcpy(Buff + i + 1, Number, strlen(Number) + 1);
		uDefaultColor = m_uCrowdColor;
	}
	else
	{
		i = strlen(m_szBlockMsg);
		memcpy(Buff, m_szBlockMsg, i);
		Buff[i] = ' ';
		memcpy(Buff + i + 1, Number, strlen(Number) + 1);
		uDefaultColor = m_uBlockColor;
	}
	SetLabelColor(uDefaultColor);
	SetLabel(Buff);
}

int	KImmediaItem::Init(KIniFile* pIniFile, const char* pSection)
{
	int nRet = KWndObjectBox::Init(pIniFile, pSection);
	if (nRet)
	{
		char ChName[64];
		pIniFile->GetString(pSection, "TextColor", "255,255,0", ChName, sizeof(ChName));
		m_TextColor = GetColor(ChName);
		return true;
	}
	return nRet;
}

void KImmediaItem::PaintWindow()
{
	KWndObjectBox::PaintWindow();
	if (g_pCoreShell && isImmedia())
	{
		int nNum = g_pCoreShell->GetGameData(GDI_IMMEDIATEITEM_NUM, m_nIndex, 0);
		if (nNum > 1 && nNum < 1000)
		{
			int nFontSize = 12;
			char szNum[4];
			int nLen = sprintf(szNum, "%d", nNum);
			szNum[3] = 0;
			g_pRepresentShell->OutputText(nFontSize, szNum, KRF_ZERO_END,
				m_nAbsoluteLeft + m_Width - nLen * (nFontSize + 1) / 2,
				m_nAbsoluteTop + m_Height - nFontSize - 1, m_TextColor);
		}
	}
}

bool KImmediaItem::CheckBox(int x,int y)
{
	return (x >= m_nAbsoluteLeft && y >= m_nAbsoluteTop &&
				x < m_nAbsoluteLeft + m_Width && y < m_nAbsoluteTop + m_Height);
}

int KImmediaItem::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if ((uMsg == WM_RBUTTONDOWN) && !isImmedia())
	{
		m_Object.uGenre = CGOG_NOTHING;
		return 0;
	}
	return KWndObjectBox::WndProc(uMsg,uParam,nParam);
}

//--------------------------------------------------------------------------

KUiPlayerBar* KUiPlayerBar::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	功能：构造函数
//--------------------------------------------------------------------------
KUiPlayerBar::KUiPlayerBar()
{
	m_cPreMsgCounter = 0;
	m_cLatestMsgIndex= 0;
	m_bMiniMode = false;
	for (int i = 0; i < MAX_RECENT_MSG_COUNT; i++)
		m_RecentMsg[i][0] = 0;
	m_szSelfName[0] = 0;

	m_nCurChannel = -1;
	m_nRecentPlayerName = 0;

	memset(m_RecentPlayerName, 0, sizeof(m_RecentPlayerName));

	m_nExperienceFull = 0;
	m_nCurLevelExperience = 0;
	m_nExperience = 0;

	ZeroMemory(m_ChatItemInfo,sizeof(m_ChatItemInfo));//
	ZeroMemory(m_ItemName,sizeof(m_ItemName));//
}

KUiPlayerBar::~KUiPlayerBar()
{
}
extern unsigned int sk_drag;
void KUiPlayerBar::DropSkill(int x,int y)
{
	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		if (m_ImmediaItem[i].CheckBox(x,y))
		{
			m_ImmediaItem[i].SetContainerId((int)UOC_IMMEDIA_SKILL);
			m_ImmediaItem[i].HoldObject(CGOG_SKILL, sk_drag, 0, 0);
			break;
		}
	}
}
//--------------------------------------------------------------------------
//	功能：如果窗口正被显示，则返回实例指针
//--------------------------------------------------------------------------
KUiPlayerBar* KUiPlayerBar::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiPlayerBar* KUiPlayerBar::OpenWindow()
{
	KUiChatCentre::OpenWindow(false);

	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiPlayerBar;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->UpdateData();
		m_pSelf->Show();
	}

	m_pSelf->m_nCurChannel = -1;
	m_pSelf->m_nRecentPlayerName = 0;

	return m_pSelf;
}

//切换大小
void KUiPlayerBar::OnSwitchSize()
{
	m_bMiniMode = !m_bMiniMode;
	char Scheme[128];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

//--------------------------------------------------------------------------
//	功能：更新数据
//--------------------------------------------------------------------------
void KUiPlayerBar::UpdateData()
{
	//_ASSERT(g_pCoreShell);
	KUiPlayerBaseInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
	g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (int)&Info, 0);
	strcpy(m_szSelfName, Info.Name);

	int nA;
	int nA2;
	UpdateXXXNumber(nA, nA2);

	KUiPlayerImmedItemSkill immedItemSkillInfo;
	memset(&immedItemSkillInfo, 0, sizeof(KUiPlayerImmedItemSkill));
	g_pCoreShell->GetGameData(GDI_PLAYER_IMMED_ITEMSKILL, (int)&immedItemSkillInfo, 0);
	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		m_ImmediaItem[i].HoldObject(immedItemSkillInfo.ImmediaItem[i].uGenre,
			immedItemSkillInfo.ImmediaItem[i].uId, 0, 0);
	}
	
	m_ImmediaSkill[0].HoldObject(immedItemSkillInfo.IMmediaSkill[0].uGenre,
		immedItemSkillInfo.IMmediaSkill[0].uId, 0, 0);
	m_ImmediaSkill[1].HoldObject(immedItemSkillInfo.IMmediaSkill[1].uGenre,
		immedItemSkillInfo.IMmediaSkill[1].uId, 0, 0);
			
	int nM;
	int nL;
	UpdateRuntimeAttribute(nM, nL);
}

//--------------------------------------------------------------------------
//	功能：关闭窗口，同时可以选则是否删除对象实例
//--------------------------------------------------------------------------
void KUiPlayerBar::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->m_cLatestMsgIndex = 0;
		m_pSelf->m_cPreMsgCounter = 0;
		for(int i = 0; i < MAX_RECENT_MSG_COUNT; i++)
			m_pSelf->m_RecentMsg[i][0] = 0;

		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}

//--------------------------------------------------------------------------
//	功能：载入界面方案
//--------------------------------------------------------------------------
void KUiPlayerBar::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s" , pScheme, m_pSelf->m_bMiniMode ? SCHEME_INI_MINI : SCHEME_INI);
		if (Ini.Load(Buff))
			m_pSelf->LoadScheme(&Ini);
	}
}

unsigned int s_CanFocusColor = 0;
unsigned int s_NotCanFocusColor = 0;

void KUiPlayerBar::LoadScheme(KIniFile* pIni)
{
//	_ASSERT(pIni);
	Init(pIni, $Main);

	m_EscDlg  .Init(pIni, "Options");
	m_Face    .Init(pIni, "Face");
	m_Friend  .Init(pIni, "Friend");

	m_DateTime.Init(pIni, "DateTime");
	m_MarketBtn.Init(pIni, "Market");
	
	char	szBuffer[16];
	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		sprintf(szBuffer, "Item_%d", i);
		m_pSelf->m_ImmediaItem[i].Init(pIni, szBuffer);
		m_pSelf->m_ImmediaItem[i].m_nIndex = i;
	}
	m_ImmediaSkill[0].Init(pIni, "ImediaLeftSkill");
	m_ImmediaSkill[1].Init(pIni, "ImediaRightSkill");
	m_ImmediaSkill[0].EnablePickPut(false);
	m_ImmediaSkill[1].EnablePickPut(false);

	m_InputEdit	.Init(pIni, "InputEdit");
	s_CanFocusColor = m_InputEdit.GetFocusBkColor();

	if (pIni->GetString("InputEdit", "FocusNoCanBKColor", "0,0,0", szBuffer, sizeof(szBuffer)))
	{
		s_NotCanFocusColor = (GetColor(szBuffer) & 0xffffff);
		int	nValue;
		if (pIni->GetInteger("InputEdit", "FocusNoCanBKColorAlpha", 128, &nValue))
		{
			if (nValue >= 255 || nValue < 0)
			{
				s_NotCanFocusColor |= 0x16000000;
			}
			else
				s_NotCanFocusColor |= (((255 - nValue) << 21) & 0xff000000);
		}
	}
	else
	{
		KRColor nColor;
		nColor.Color_dw = s_CanFocusColor;
		nColor.Color_b.a = 12;
		nColor.Color_b.r = 108;
		s_NotCanFocusColor = nColor.Color_dw;
	}

	m_SendBtn	.Init(pIni, "SendBtn");
	m_ChannelSwitchBtn.Init(pIni, "ChannelBtn");
	m_ChannelOpenBtn.Init(pIni, "OpenChannelBtn");
    m_Auto.Init(pIni, "Auto");
	m_SwitchBtn .Init(pIni, "SwitchSizeBtn");

	for (int j =0; j < MAX_ITEMBUTTON; j++)
	{
		m_ItemBtn[j].Init(pIni, "ChatItem");
		m_ItemBtn[j].SetPosition(INVISIBLE_POS);
	}

	m_CheckParty.Init(pIni, "Button");

	m_Party.clear();

	m_BgImg1.Init(pIni,"Bg_Image1");
	m_BgImg2.Init(pIni,"Bg_Image2");
	m_BgImg3.Init(pIni,"Bg_Image3");
	m_BgImg4.Init(pIni,"Bg_Image4");
	m_BgImg5.Init(pIni,"Bg_Image5");
	m_BgImg6.Init(pIni,"Bg_Image6");
	m_BgImg7.Init(pIni,"Bg_Image7");
	m_BgImg8.Init(pIni,"Bg_Image8");
	
	m_AvatarImg1.Init(pIni,"Avatar_Image1");
	m_AvatarImg2.Init(pIni,"Avatar_Image2");
	m_AvatarImg3.Init(pIni,"Avatar_Image3");
	m_AvatarImg4.Init(pIni,"Avatar_Image4");
	m_AvatarImg5.Init(pIni,"Avatar_Image5");
	m_AvatarImg6.Init(pIni,"Avatar_Image6");
	m_AvatarImg7.Init(pIni,"Avatar_Image7");
	m_AvatarImg8.Init(pIni,"Avatar_Image8");

	m_NameImg1.Init(pIni,"Name_Image1");
	m_NameImg2.Init(pIni,"Name_Image2");
	m_NameImg3.Init(pIni,"Name_Image3");
	m_NameImg4.Init(pIni,"Name_Image4");
	m_NameImg5.Init(pIni,"Name_Image5");
	m_NameImg6.Init(pIni,"Name_Image6");
	m_NameImg7.Init(pIni,"Name_Image7");
	m_NameImg8.Init(pIni,"Name_Image8");

	m_LifeImg1.Init(pIni,"Life_Image1");
	m_LifeImg2.Init(pIni,"Life_Image2");
	m_LifeImg3.Init(pIni,"Life_Image3");
	m_LifeImg4.Init(pIni,"Life_Image4");
	m_LifeImg5.Init(pIni,"Life_Image5");
	m_LifeImg6.Init(pIni,"Life_Image6");
	m_LifeImg7.Init(pIni,"Life_Image7");
	m_LifeImg8.Init(pIni,"Life_Image8");

	HideParty();
			if (g_pCoreShell)
				g_pCoreShell->OperationRequest(GOI_OPEN_SHOP_GOLD, 0,0);

}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
void KUiPlayerBar::Initialize()
{
	AddChild(&m_DateTime);
	AddChild(&m_Face);
	AddChild(&m_EscDlg);
	AddChild(&m_Friend);
	AddChild(&m_ChannelOpenBtn);

	for (int j =0; j < MAX_ITEMBUTTON; j++)
	{
		AddChild(&m_ItemBtn[j]);
	}

	/*AddChild(&m_DateTime);
	AddChild(&m_Face);
	AddChild(&m_EscDlg);
	AddChild(&m_Friend);*/

	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		m_ImmediaItem[i].SetObjectGenre(CGOG_ITEM);
		m_ImmediaItem[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
		AddChild(&m_ImmediaItem[i]);
		m_ImmediaItem[i].SetContainerId((int)UOC_IMMEDIA_ITEM);
	}

	AddChild(&m_ImmediaSkill[0]);
	AddChild(&m_ImmediaSkill[1]);
	m_ImmediaSkill[0].SetContainerId((int)UOC_IMMEDIA_SKILL);
	m_ImmediaSkill[1].SetContainerId((int)UOC_IMMEDIA_SKILL);
	
	AddChild(&m_InputEdit);
	AddChild(&m_ChannelSwitchBtn);
	AddChild(&m_SendBtn);
	AddChild(&m_SwitchBtn);
	AddChild(&m_ChannelOpenBtn);
    AddChild(&m_Auto);

	AddChild(&m_CheckParty);
	m_CheckParty.CheckButton(true);

	AddChild(&m_BgImg1);
	AddChild(&m_BgImg2);
	AddChild(&m_BgImg3);
	AddChild(&m_BgImg4);
	AddChild(&m_BgImg5);
	AddChild(&m_BgImg6);
	AddChild(&m_BgImg7);
	AddChild(&m_BgImg8);

	AddChild(&m_AvatarImg1);
	AddChild(&m_AvatarImg2);
	AddChild(&m_AvatarImg3);
	AddChild(&m_AvatarImg4);
	AddChild(&m_AvatarImg5);
	AddChild(&m_AvatarImg6);
	AddChild(&m_AvatarImg7);
	AddChild(&m_AvatarImg8);

	AddChild(&m_NameImg1);
	AddChild(&m_NameImg2);
	AddChild(&m_NameImg3);
	AddChild(&m_NameImg4);
	AddChild(&m_NameImg5);
	AddChild(&m_NameImg6);
	AddChild(&m_NameImg7);
	AddChild(&m_NameImg8);

	AddChild(&m_LifeImg1);
	AddChild(&m_LifeImg2);
	AddChild(&m_LifeImg3);
	AddChild(&m_LifeImg4);
	AddChild(&m_LifeImg5);
	AddChild(&m_LifeImg6);
	AddChild(&m_LifeImg7);
	AddChild(&m_LifeImg8);
	AddChild(&m_MarketBtn);


	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	m_pSelf->LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	功能：窗口函数
//--------------------------------------------------------------------------
int KUiPlayerBar::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_M_POPUPMENU:
		if (uParam == (unsigned int)(KWndWindow*)(&m_InputEdit))
		{
			PopupPhraseMenu(LOWORD(nParam), HIWORD(nParam), true);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChannelSwitchBtn)
		{
			int x, y;
			m_ChannelSwitchBtn.GetAbsolutePos(&x, &y);
			PopupChannelMenu(x, y);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChannelOpenBtn)
		{
		}
		break;
	case WND_N_ITEM_PICKDROP:
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM))
			OnObjPickedDropped((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_Friend)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_FRIEND);
		else if(uParam == (unsigned int)(KWndWindow*)&m_MarketBtn)
		{
			if (g_pCoreShell)
			{
				g_pCoreShell->OperationRequest(GOI_OPEN_SHOP_GOLD, 0,0);
				if (!KUiShopGold::GetIfVisible())
						KUiShopGold::OpenWindow();
				else
				KUiShopGold::CloseWindow();
			}
			//KUiShopGold::OpenWindow();
		}
        else if (uParam == (unsigned int)(KWndWindow*)&m_Auto)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_AUTO);
		else if (uParam == (unsigned int)(KWndWindow*)&m_EscDlg)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SYSTEM);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Face)
		{
			KUiFaceSelector::OpenWindow(this, 0);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_SendBtn)
			OnSend(false);
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChannelSwitchBtn)
		{
			int x, y;
			m_ChannelSwitchBtn.GetAbsolutePos(&x, &y);
			PopupChannelMenu(x, y);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChannelOpenBtn)
		{
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_SwitchBtn)
			OnSwitchSize();
		break;
	case WND_N_LEFT_CLICK_ITEM:
		if (nParam == (int)(KWndWindow*)&m_ImmediaSkill[0])
			KUiSkillTree::OpenWindow(true);
		else if (nParam == (int)(KWndWindow*)&m_ImmediaSkill[1])
			KUiSkillTree::OpenWindow(false);
		break;
	case WND_N_RIGHT_CLICK_ITEM:
		if (g_UiBase.IsOperationEnable(UIS_O_USE_ITEM))
		{
			for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
				if (nParam == (int)(KWndWindow*)&m_ImmediaItem[i])
					OnUseItem(i);
		}
		break;
	case WND_N_EDIT_SPECIAL_KEY_DOWN:
		if (nParam == VK_RETURN)
		{
			OnSend((GetKeyState(VK_CONTROL) & 0x8000));
			Wnd_SetFocusWnd(NULL);
			nRet = 1;
		}
		else if (nParam == VK_UP || nParam == VK_DOWN)
		{
			if (GetKeyState(VK_CONTROL) & 0x8000)
			{
				int x, y;
				m_InputEdit.GetAbsolutePos(&x, &y);
				int nCX = 0;
				m_InputEdit.GetCaretPosition(&nCX, NULL);
				PopupPhraseMenu(x + nCX, y, nParam == VK_DOWN);
			}
			else
			{
				InputRecentMsg(nParam == VK_UP);
				nRet = 1;
			}
		}
		else if (nParam == VK_PRIOR || nParam == VK_NEXT)
		{
			if (m_nCurChannel >= 0)
			{
				int nToChannel;
				if (nParam == VK_PRIOR)
					nToChannel = (m_nCurChannel - 1 >= 0) ? m_nCurChannel - 1 : KUiMsgCentrePad::GetChannelCount() + m_nRecentPlayerName - 1;
				else
					nToChannel = (DWORD(m_nCurChannel + 1) < (KUiMsgCentrePad::GetChannelCount() + m_nRecentPlayerName)) ? m_nCurChannel + 1 : 0;

				if (nToChannel != m_nCurChannel)
				{
					InputCurrentChannel(nToChannel, true);
				}
			}				
			nRet = 1;
		}
		else if (nParam == VK_ESCAPE)
		{
			Wnd_SetFocusWnd(NULL);
			nRet = 1;
		}
		break;
	case WND_N_EDIT_CHANGE:
		m_cPreMsgCounter = 0;
		break;
	case WND_M_MENUITEM_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			if (HIWORD(nParam) == SEL_CHANNEL_MENU)
			{
				if (short(LOWORD(nParam)) >= 0)
				{
					InputCurrentChannel(LOWORD(nParam), true);
				}

				KPopupMenu::Cancel();
			}
			else if (HIWORD(nParam) == SEL_PHRASE_MENU)
			{
				if (short(LOWORD(nParam)) >= 0)
				{
					char szPhrase[64];
					int nLen = g_UiChatPhrase.GetPhrase(LOWORD(nParam), szPhrase);
					m_InputEdit.InsertString(szPhrase, nLen);
				}
			}
		}
		break;
	case WND_M_MENUITEMRIGHT_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			if (HIWORD(nParam) == SEL_CHANNEL_MENU)
			{
				if (short(LOWORD(nParam)) >= 0)
				{
					if (KUiMsgCentrePad::CheckChannel(LOWORD(nParam), !KUiMsgCentrePad::GetChannelSubscribe(LOWORD(nParam))))
					{
						KPopupMenuData* p = KPopupMenu::GetMenuData();
						if (p)
						{
							PopupChannelMenu(p->nXX, p->nYY);
						}
					}
					else
						KPopupMenu::Cancel();
				}
				else
					KPopupMenu::Cancel();
			}
		}
		break;
	case WND_M_OTHER_WORK_RESULT:
		if (nParam)
		{
			m_InputEdit.InsertString((const char*)nParam, -1);
			Wnd_SetFocusWnd(&m_InputEdit);
		}
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

void KUiPlayerBar::PaintWindow()
{
	KWndImage::PaintWindow();

	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount() + m_nRecentPlayerName;
	if (m_nCurChannel < 0 || m_nCurChannel >= nChannelDataCount)
	{
		SetCurrentChannel(0);
	}
}

void KUiPlayerBar::PopupChannelMenu(int x, int y)
{
	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
	if (nChannelDataCount + m_nRecentPlayerName <= 0)
		return;

	KPopupMenuData* pMenuData = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nChannelDataCount + m_nRecentPlayerName));
	if (pMenuData == NULL)
	{
		return;
	}

	KPopupMenu::InitMenuData(pMenuData, nChannelDataCount + m_nRecentPlayerName);
	pMenuData->usMenuFlag &= ~PM_F_HAVE_HEAD_TAIL_IMG;
	pMenuData->usMenuFlag |= (PM_F_AUTO_DEL_WHEN_HIDE | PM_F_CANCEL_BY_CALLER);

	pMenuData->nItemTitleIndent = 0;
	pMenuData->byItemTitleUpSpace = 0;

	int i;
	pMenuData->nItemHeight = 0;
	WORD nHei = 0;
	char szText[64];
	for (i = 0; i < nChannelDataCount; i++)
	{
		WORD nPicIndex = -1;
		int nOffset = 0;
		KRColor uColor;
		KRColor uBKColor;
		short nCheckPicIndex = -1;
		if (KUiMsgCentrePad::GetChannelMenuinfo(i, &nPicIndex, &nHei, &uColor, &uBKColor, szText, &nCheckPicIndex))
		{
			pMenuData->Items[i].szData[0] = KTC_INLINE_PIC;
			*((WORD*)(pMenuData->Items[i].szData + 1)) = nPicIndex;
			pMenuData->Items[i].szData[3] = uColor.Color_b.a;
			pMenuData->Items[i].szData[4] = uColor.Color_b.r;
			pMenuData->Items[i].szData[5] = uColor.Color_b.g;
			pMenuData->Items[i].szData[6] = uColor.Color_b.b;
			nOffset = 7;
			if (nHei > pMenuData->nItemHeight)
				pMenuData->nItemHeight = nHei;
			strcpy(pMenuData->Items[i].szData + nOffset, szText);
			pMenuData->Items[i].uDataLen = strlen(szText) + nOffset;
			
			pMenuData->Items[i].szData[pMenuData->Items[i].uDataLen++] = KTC_TAB;

			if (nCheckPicIndex != -1)
			{
				pMenuData->Items[i].szData[pMenuData->Items[i].uDataLen] = KTC_INLINE_PIC;
				*((WORD*)(pMenuData->Items[i].szData + pMenuData->Items[i].uDataLen + 1)) = nCheckPicIndex;
				pMenuData->Items[i].uDataLen += 3;
			}
		}

		pMenuData->Items[i].uBgColor = uBKColor.Color_dw;
		pMenuData->usMenuFlag |= PM_F_TAB_SPLIT_ITEM_TEXT;
	}

	if (nHei > 0)
        pMenuData->nItemRightWidth = MENU_ITEM_DEFAULT_RIGHT_WIDTH;
	else
		pMenuData->nItemRightWidth = 0;

	for (; i < nChannelDataCount + m_nRecentPlayerName; i++)
	{
		if (m_RecentPlayerName[i - nChannelDataCount][0] != 0)
		{
			WORD nPicIndex = -1;
			int nOffset = 0;
			KRColor uColor;
			KRColor uBKColor;
			if (KUiMsgCentrePad::GetPeopleMenuinfo(m_RecentPlayerName[i - nChannelDataCount], &nPicIndex, &nHei, &uColor, &uBKColor))
			{
				pMenuData->Items[i].szData[0] = KTC_INLINE_PIC;
				*((WORD*)(pMenuData->Items[i].szData + 1)) = nPicIndex;
				pMenuData->Items[i].szData[3] = uColor.Color_b.a;
				pMenuData->Items[i].szData[4] = uColor.Color_b.r;
				pMenuData->Items[i].szData[5] = uColor.Color_b.g;
				pMenuData->Items[i].szData[6] = uColor.Color_b.b;
				nOffset = 7;
				if (nHei > pMenuData->nItemHeight)
					pMenuData->nItemHeight = nHei;
			}

			strcpy(pMenuData->Items[i].szData + nOffset, m_RecentPlayerName[i - nChannelDataCount]);
			pMenuData->Items[i].uDataLen = strlen(m_RecentPlayerName[i - nChannelDataCount]) + nOffset;
			pMenuData->Items[i].uBgColor = uBKColor.Color_dw;
		}
		else
		{
			break;
		}
	}

	pMenuData->nNumItem = i;

	pMenuData->nX = x;
	pMenuData->nY = y;
	pMenuData->nSelectedItem = m_nCurChannel;
	KPopupMenu::Popup(pMenuData, (KWndWindow*)this, SEL_CHANNEL_MENU);
}

void KUiPlayerBar::PopupPhraseMenu(int x, int y, bool bFirstItem)
{
	int nDataCount = g_UiChatPhrase.GetPhraseCount();
	if (nDataCount <= 0)
		return;

	KPopupMenuData* pMenuData = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nDataCount));
	if (pMenuData == NULL)
	{
		return;
	}

	KPopupMenu::InitMenuData(pMenuData, nDataCount);
	pMenuData->usMenuFlag &= ~PM_F_HAVE_HEAD_TAIL_IMG;
	pMenuData->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	pMenuData->nItemTitleIndent = 0;
	pMenuData->byItemTitleUpSpace = 0;

	int i;
	for (i = 0; i < nDataCount; i++)
	{
		pMenuData->Items[i].uDataLen = g_UiChatPhrase.GetPhrase(i, pMenuData->Items[i].szData);
	}

	pMenuData->nNumItem = nDataCount;

	pMenuData->nX = x;
	pMenuData->nY = y;
	pMenuData->nSelectedItem = bFirstItem ? 0 : nDataCount - 1;
	KPopupMenu::Popup(pMenuData, (KWndWindow*)this, SEL_PHRASE_MENU);
}

void KUiPlayerBar::OnSend(BOOL bDirectSend)
{
	char szBuffer[512] = {0};
	ZeroMemory(szBuffer, sizeof(szBuffer));
	int nName = 0;
	int nMsgLength = m_InputEdit.GetText(szBuffer, sizeof(szBuffer), true);
	if (nMsgLength <= 0 || nMsgLength >= 80)
	{
		m_InputEdit.ClearText();
		return;
	}

	//====保存到最近发送消息记录里====
	memcpy(m_RecentMsg[m_cLatestMsgIndex], szBuffer, nMsgLength);
	m_RecentMsg[m_cLatestMsgIndex][nMsgLength] = 0;
	m_cLatestMsgIndex = (m_cLatestMsgIndex + 1) % MAX_RECENT_MSG_COUNT;
	m_cPreMsgCounter = 0;

	if (!bDirectSend)
	{
		if (TextMsgFilter(szBuffer, nMsgLength))	//客户端控制命令执行过滤
		{
			m_InputEdit.ClearText();
			return;
		}
	}

	if (g_pCoreShell == NULL)
	{
		return;
	}

	//====获取输入子串前端可能指定有的消息传送目标====
	int bChannel = false;
	int nDestChannel = -1;
	char Name[32];
	Name[0] = 0;
	if (szBuffer[nName] == TEXT_CTRL_CHAT_PREFIX || szBuffer[nName] == TEXT_CTRL_CHANNEL_PREFIX)
	{
		bChannel = (szBuffer[nName] == TEXT_CTRL_CHANNEL_PREFIX);
		while (nName < nMsgLength)
		{
			if (szBuffer[nName] == ' ')
			{
				szBuffer[nName] = 0;
				nName++;
				break;
			}

			nName++;
		}

		if (bChannel)
		{
			ReplaceSpecialName(Name, 31, szBuffer + 1);
			nDestChannel = GetChannelIndex(Name);
		}
		else
		{
			strncpy(Name, szBuffer + 1, 31);
		}

		Name[31] = 0;
	}

	if (Name[0] == 0)	//不指定名字
	{
		bChannel = true;
		int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
		if (m_nCurChannel >= 0 && m_nCurChannel < nChannelDataCount)
		{
			nDestChannel = m_nCurChannel;
		}
		else
		{
			//是附近玩家
			for (int n = 0; n < nChannelDataCount; n++)
			{
				if (KUiMsgCentrePad::IsChannelType(n, KUiMsgCentrePad::ch_Screen))
				{
					nDestChannel = n;
					break;
				}
			}
		}

		if (nDestChannel >= 0)
		{
			strncpy(Name, KUiMsgCentrePad::GetChannelTitle(nDestChannel), 31);
		}
		
		Name[31] = 0;
	}
	else	//指定名字的恢复Buffer数据
	{
		szBuffer[nName - 1] = ' ';
	}

	nMsgLength -= nName;
	
	if (bChannel && !KUiMsgCentrePad::IsChannelType(nDestChannel, KUiMsgCentrePad::ch_GM))	//在非GM频道中输入GM指令,不发送出去,以免泄密
	{
		if (nMsgLength > 3 && szBuffer[nName] == '?' && (szBuffer[nName + 1] == 'g' || szBuffer[nName + 1] == 'G') && (szBuffer[nName + 2] == 'm' || szBuffer[nName + 2] == 'M'))
		{
			return;
		}
	}

	if (!g_ChatFilter.IsTextPass(szBuffer + nName))
	{
		char szWarning[] = "Vui l遪g kh玭g s?d鬾g t?ng?kh玭g th輈h h頿 trong giao ti誴!";
		KUiMsgCentrePad::SystemMessageArrival(szWarning, sizeof(szWarning));
		return;
	}

	char szEvent[1024] = {0};
	ZeroMemory(szEvent, sizeof(szEvent));
	if (bChannel)
	{
		sprintf(szEvent, APP_CHAT, Name, szBuffer + nName);
	}
	else
	{
		sprintf(szEvent, APP_SAY, Name, szBuffer + nName);
	}

	szEvent[1023] = 0;

	if (g_UiBase.NotifyEvent(szEvent) == 0)
	{
		return;
	}

	char szBuffer2[1536] = {0};
	ZeroMemory(szBuffer2, sizeof(szBuffer2));
   	nMsgLength = KUiFaceSelector::ConvertFaceText(szBuffer2, szBuffer + nName, nMsgLength);

	if(m_ItemName[0])
	{
		char * pInsert = strstr(szBuffer2, m_ItemName);
		char * pInsert2 = strstr(szBuffer2, m_ItemName);
		int nLenChatItemInfo = strlen(m_ChatItemInfo);
		if(pInsert)
		{
			int nL = 0;
			while(1)
			{
				if(*pInsert == '>')
				{
					break;
				}

				nL++;
				pInsert++;
			}

			nL++;
			pInsert++;
			//int nLenChatItemInfo = strlen(m_ChatItemInfo);
			int nMove = nLenChatItemInfo - nL;
			if(nMove + strlen(szBuffer2) > 1535)
			{
				return; //gioi han pham vi chuoi~
			}

			int nAfterLen = strlen(pInsert);
			if(nMove > 0)
			{
				memmove(pInsert + nMove, pInsert, nAfterLen);
			}
			
			memcpy(pInsert2, m_ChatItemInfo, nLenChatItemInfo);
			nMsgLength += nMove;
		}
	}
	
	nMsgLength = TEncodeText(szBuffer2, nMsgLength);

	if (bChannel)
	{
		DWORD nChannelID = KUiMsgCentrePad::GetChannelID(nDestChannel);
		if (nChannelID != -1)
		{
			KUiMsgCentrePad::CheckChannel(nDestChannel, true);
			OnSendChannelMessage(nChannelID, szBuffer2, nMsgLength);
			m_InputEdit.ClearText();
			InputCurrentChannel(nDestChannel, true);
		}
	}
	else if (!IsSelfName(Name))
	{
		OnSendSomeoneMessage(Name, szBuffer2, nMsgLength);
		
		int nAdd = AddRecentPlayer(Name);
		if (nAdd >= 0)
		{
			m_InputEdit.ClearText();
			InputCurrentChannel(nAdd, true);
		}
	}
}

BOOL KUiPlayerBar::IsCanSendMessage(const char* Buffer, int nLen, char* szDestName, int nChannelID)
{
	if (m_pSelf)
	{
		if (Buffer && nLen > 0 && !g_ChatFilter.IsTextPass(Buffer))
			return FALSE;

		char szEvent[1024];
		if (nChannelID >= 0)
			sprintf(szEvent, APP_CHAT, szDestName, Buffer);
		else
			sprintf(szEvent, APP_SAY, szDestName, Buffer);

		szEvent[1023] = 0;

		if (g_UiBase.NotifyEvent(szEvent) == 0)
			return FALSE;
	}
	return TRUE;
}

void KUiPlayerBar::OnSendChannelMessage(DWORD nChannelID, const char* Buffer, int nLen)				//发送频道聊天到服务器
{
	if (nChannelID != -1 && Buffer && nLen > 0)
	{
		int nLeft = KUiMsgCentrePad::PushChannelData(nChannelID, Buffer, nLen);
		if (nLeft < 0)
		{
			char szWarning[] = "Thi猲 l?truy襫 ﹎ kh玭g c竎h n祇 s?d鬾g li猲 t鬰, m阨 b筺 l蕐 h琲 ch鑓 l竧!";
			KUiMsgCentrePad::SystemMessageArrival(szWarning, sizeof(szWarning));
		}
		else if (nLeft > 0)
		{
			char szWarning[64];
			sprintf(szWarning, "B筺 c莕 ngh?ng琲 sau [%d] gi﹜ m韎 c?th?ti誴 t鬰 truy襫 tin", (nLeft + 999 ) / 1000);
			KUiMsgCentrePad::SystemMessageArrival(szWarning, strlen(szWarning) + 1);
		}
	}
}

void KUiPlayerBar::OnDirectSendChannelMessage(DWORD nChannelID, BYTE cost, const char* Buffer, int nLen)				//发送频道聊天到服务器
{
	if (g_pCoreShell && nChannelID != -1 && Buffer && nLen > 0)
	{
		size_t chatsize = sizeof(CHAT_CHANNELCHAT_CMD) + nLen;
		size_t pckgsize = sizeof(tagExtendProtoHeader) + chatsize;

		tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_alloca(pckgsize);
		pExHeader->ProtocolType = c2s_extendchat;
		pExHeader->wLength = pckgsize - 1;

		CHAT_CHANNELCHAT_CMD* pCccCmd = (CHAT_CHANNELCHAT_CMD*)(pExHeader + 1);
		pCccCmd->ProtocolType = chat_channelchat;
		pCccCmd->wSize = chatsize - 1;
		pCccCmd->packageID = -1;
		pCccCmd->channelid = nChannelID;
		pCccCmd->cost = cost;
		pCccCmd->sentlen = nLen;
		pCccCmd->figure = g_pCoreShell->GetTongFigure();
		memcpy(pCccCmd + 1, Buffer, nLen);

		g_pCoreShell->SendNewDataToServer(pExHeader, pckgsize);
	}
}

void KUiPlayerBar::OnSendSomeoneMessage(const char* Name, const char* Buffer, int nLen)				//发送someone聊天到服务器
{
	if (g_pCoreShell && Name && Name[0] != 0 && Buffer && nLen > 0 && m_pSelf)
	{
		size_t chatsize = sizeof(CHAT_SOMEONECHAT_CMD) + nLen;
		size_t pckgsize = sizeof(tagExtendProtoHeader) + chatsize;

		tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_alloca(pckgsize);
		pExHeader->ProtocolType = c2s_extendchat;
		pExHeader->wLength = pckgsize - 1;

		CHAT_SOMEONECHAT_CMD* pCscCmd = (CHAT_SOMEONECHAT_CMD*)(pExHeader + 1);
		pCscCmd->ProtocolType = chat_someonechat;
		pCscCmd->wSize = chatsize - 1;
		pCscCmd->packageID = -1;
		strcpy(pCscCmd->someone, Name);
		pCscCmd->sentlen = nLen;
		memcpy(pCscCmd + 1, Buffer, nLen);

		g_pCoreShell->SendNewDataToServer(pExHeader, pckgsize);
	}
}


void KUiPlayerBar::InputRecentMsg(bool bPrior)
{
	int nCounter;
	if (bPrior)
		nCounter = m_cPreMsgCounter - 1;
	else
		nCounter = m_cPreMsgCounter + 1;
	if (nCounter < 0 && nCounter >= - MAX_RECENT_MSG_COUNT)
	{
		int nIndex = m_cLatestMsgIndex + nCounter;
		if (nIndex < 0)
			nIndex += 8;
		if (m_RecentMsg[nIndex][0])
		{
			m_InputEdit.SetText(m_RecentMsg[nIndex]);
			m_cPreMsgCounter = nCounter;
		}
	}
	else if (nCounter >= 0)
	{
		m_InputEdit.ClearText();
		m_cPreMsgCounter = 0;
	}
}

//--------------------------------------------------------------------------
//	功能：响应界面操作取起\放下东西
//--------------------------------------------------------------------------
void KUiPlayerBar::OnObjPickedDropped(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{

	KUiObjAtContRegion	Pick, Drop;
	KUiDraggedObject	Obj;
	KWndWindow*			pWnd = NULL;

	if (pPickPos)
	{
		_ASSERT(pPickPos->pWnd);
		((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.v = 0;
		Pick.eContainer = UOC_IMMEDIA_ITEM;
		pWnd = pPickPos->pWnd;
	}
	else if (pDropPos)
	{
		pWnd = pDropPos->pWnd;
	}
	else
		return;

	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.v = 0;
		Drop.eContainer = UOC_IMMEDIA_ITEM;
	}

	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		if (pWnd == (KWndWindow*)&m_ImmediaItem[i])
		{
			Drop.Region.h =	Pick.Region.h = i;
			break;
		}
	}
	_ASSERT(i < UPB_IMMEDIA_ITEM_COUNT);
	
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT, 
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}

//--------------------------------------------------------------------------
//	功能：使用物品
//--------------------------------------------------------------------------
void KUiPlayerBar::OnUseItem(int nIndex)
{
	if (m_pSelf && nIndex >= 0 && nIndex < UPB_IMMEDIA_ITEM_COUNT)
	{
		KUiDraggedObject Obj;
		m_pSelf->m_ImmediaItem[nIndex].GetObject(Obj);

		if (m_pSelf->m_ImmediaItem[nIndex].isImmedia())
		{
			KUiObjAtRegion Info;
			{
				Info.Obj.uGenre = Obj.uGenre;
				Info.Obj.uId = Obj.uId;
				Info.Region.h = nIndex;
				Info.Region.v = 0;
				Info.Region.Width = Info.Region.Height = 0;
			}
			g_pCoreShell->OperationRequest(GOI_USE_ITEM, (int)&Info, UOC_IMMEDIA_ITEM);
		}
		else
		{
			int	x, y;
			Wnd_GetCursorPos(&x, &y);

			g_pCoreShell->UseSkill(x, y, Obj.uId);
		}
	}
}


int KUiPlayerBar::GetChannelIndex(const char* pTitle)
{
	if (pTitle)
	{
		int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
		int n = KUiMsgCentrePad::GetChannelIndex((char*)pTitle);
		if (n >= 0 && n < nChannelDataCount)
			return n;
		
		n = nChannelDataCount;
		for (; n < nChannelDataCount + m_nRecentPlayerName; n++)
		{
			if (m_RecentPlayerName[n - nChannelDataCount][0] == 0)
				break;
			if (strcmp(m_RecentPlayerName[n - nChannelDataCount], pTitle) == 0)
			{
				return n;
			}
		}
	}
	return -1;
}

void KUiPlayerBar::SetCurrentChannel(int nIndex)
{
	if (!m_pSelf)
		return;
	if (nIndex < 0)
		return;

	if (nIndex == m_pSelf->m_nCurChannel)
		return;
	
	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
	if (nIndex < nChannelDataCount)
	{
		m_pSelf->m_nCurChannel = nIndex;
		WORD nPicIndex = -1;
		if (KUiMsgCentrePad::GetChannelMenuinfo(m_pSelf->m_nCurChannel, &nPicIndex))
		{
			char buffer[3];
			buffer[0] = KTC_INLINE_PIC;
			*((WORD*)(buffer + 1)) = nPicIndex;
			m_pSelf->m_ChannelSwitchBtn.SetText(buffer, 3);
		}
	}
	else if (nIndex < nChannelDataCount + m_pSelf->m_nRecentPlayerName)
	{
		m_pSelf->m_nCurChannel = nIndex;
		WORD nPicIndex = -1;
		if (KUiMsgCentrePad::GetPeopleMenuinfo(m_pSelf->m_RecentPlayerName[m_pSelf->m_nCurChannel - nChannelDataCount], &nPicIndex))
		{
			char buffer[3];
			buffer[0] = KTC_INLINE_PIC;
			*((WORD*)(buffer + 1)) = nPicIndex;
			m_pSelf->m_ChannelSwitchBtn.SetText(buffer, 3);
		}
		else
			m_pSelf->m_ChannelSwitchBtn.SetText(m_pSelf->m_RecentPlayerName[m_pSelf->m_nCurChannel - nChannelDataCount]);
	}
}

void KUiPlayerBar::InputCurrentChannel(int nIndex, bool bFocus)
{
	if (nIndex < 0)
		return;
	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
	if (nIndex < nChannelDataCount)
	{
		InputNameMsg(true, KUiMsgCentrePad::GetChannelTitle(nIndex), bFocus);
		SetCurrentChannel(nIndex);
	}
	else if (nIndex < nChannelDataCount + m_nRecentPlayerName)
	{
		InputNameMsg(false, m_RecentPlayerName[nIndex - nChannelDataCount], bFocus);
		SetCurrentChannel(nIndex);
	}
}

//--------------------------------------------------------------------------
//	功能：切换到下一个频道
//--------------------------------------------------------------------------
void KUiPlayerBar::SwitchChannel(BOOL bUp)
{
	if(m_pSelf)
	{
		int nNewChannelID;
		int nCount = KUiMsgCentrePad::GetChannelCount();
		if(bUp)
		{
			if(m_pSelf->m_nCurChannel < 1)
		    {
			    nNewChannelID = nCount - 1;
		    }
		    else
		    {
    			nNewChannelID = m_pSelf->m_nCurChannel - 1;
		    }
		}
		else
		{
		    if(m_pSelf->m_nCurChannel > nCount - 2)
		    {
			    nNewChannelID = 0;
		    }
		    else
		    {
    			nNewChannelID = m_pSelf->m_nCurChannel + 1;
		    }
		}
		m_pSelf->InputCurrentChannel(nNewChannelID, FALSE);
	}
}


//--------------------------------------------------------------------------
//  功能：得到PlayerBar里当前的频道
//--------------------------------------------------------------------------
int KUiPlayerBar::GetCurChannel()
{
	if(m_pSelf)
	    return m_pSelf->m_nCurChannel;
	else
		return -1;
}

//--------------------------------------------------------------------------
//	功能：更新常变的那些数值数据
//--------------------------------------------------------------------------
void KUiPlayerBar::UpdateXXXNumber(int& nMana, int& nFullMana)
{
	KUiPlayerRuntimeInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));

	g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);

	m_nExperienceFull = Info.nExperienceFull;
	m_nCurLevelExperience = Info.nCurLevelExperience;
	m_nExperience = Info.nExperience;

	KUiStatus* pStatus = KUiStatus::GetIfVisible();
	if (pStatus)
		pStatus->UpdateRuntimeInfo(&Info);

	KUiSceneTimeInfo	Spot;
	g_pCoreShell->SceneMapOperation(GSMOI_SCENE_TIME_INFO, (unsigned int)&Spot, 0);
	KUiMiniMap::UpdateSceneTimeInfo(&Spot);
	KUiAuto::UpdateSceneTimeInfo(&Spot);

	nMana = max(Info.nMana, 0);
	nFullMana = max(Info.nManaFull, 0);
}

void KUiPlayerBar::UpdateRuntimeAttribute(int& nMoney, int& nLevel)
{
	KUiPlayerAttribute	Info;
	memset(&Info, 0, sizeof(KUiPlayerAttribute));
	g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (unsigned int)&Info, 0);
	KUiStatus* pStatus = KUiStatus::GetIfVisible();
	
	if (pStatus)
		pStatus->UpdateRuntimeAttribute(&Info);

	nMoney = Info.nMoney;
	nLevel = Info.nLevel;
}

//--------------------------------------------------------------------------
//	功能：变更物品摆换
//--------------------------------------------------------------------------
void KUiPlayerBar::UpdateItem(int nIndex, unsigned int uGenre, unsigned int uId)
{
	if (nIndex >= 0 && nIndex < UPB_IMMEDIA_ITEM_COUNT)
	{
		UiSoundPlay(UI_SI_PICKPUT_ITEM);
		m_ImmediaItem[nIndex].HoldObject(uGenre, uId, 0, 0);
	}
}

//--------------------------------------------------------------------------
//	功能：变更快捷技能
//--------------------------------------------------------------------------
void KUiPlayerBar::UpdateSkill(int nIndex, unsigned int uGenre, unsigned int uId)
{
	if (nIndex == 0 || nIndex == 1)
	{
		m_ImmediaSkill[nIndex].HoldObject(uGenre, uId, 0, 0);
	}
}

//--------------------------------------------------------------------------
//	功能：我要呼吸
//--------------------------------------------------------------------------
void KUiPlayerBar::HideParty()
{

	m_BgImg1.Hide();
	m_BgImg2.Hide();
	m_BgImg3.Hide();
	m_BgImg4.Hide();
	m_BgImg5.Hide();
	m_BgImg6.Hide();
	m_BgImg7.Hide();
	m_BgImg8.Hide();

	m_AvatarImg1.Hide();
	m_AvatarImg2.Hide();
	m_AvatarImg3.Hide();
	m_AvatarImg4.Hide();
	m_AvatarImg5.Hide();
	m_AvatarImg6.Hide();
	m_AvatarImg7.Hide();
	m_AvatarImg8.Hide();

	m_NameImg1.Hide();
	m_NameImg2.Hide();
	m_NameImg3.Hide();
	m_NameImg4.Hide();
	m_NameImg5.Hide();
	m_NameImg6.Hide();
	m_NameImg7.Hide();
	m_NameImg8.Hide();

	m_LifeImg1.Hide();
	m_LifeImg2.Hide();
	m_LifeImg3.Hide();
	m_LifeImg4.Hide();
	m_LifeImg5.Hide();
	m_LifeImg6.Hide();
	m_LifeImg7.Hide();
	m_LifeImg8.Hide();
}
#define IMAGE_PLAYER	"\\Ui\\Ui3\\AvatarPlayer.ini"
void KUiPlayerBar::ShowIt(unsigned int uId, int x, KWndImage* m_AvatarImg)
{
	KIniFile Ini;
	char sz_Temp[30];
	char ImageNpc[100];
	
	if (Ini.Load(IMAGE_PLAYER))
	{
		sprintf(sz_Temp, "%d_Image", x);
		if (g_pCoreShell->GetGameData(GDI_PLAYER_IS_MALE, 0, uId))
		{
			Ini.GetString("AvatarMale", sz_Temp, "", ImageNpc, sizeof(ImageNpc));
		}
		else
		{	
			Ini.GetString("AvatarFemale", sz_Temp, "", ImageNpc, sizeof(ImageNpc));
		}
	}
	char* str = strstr(ImageNpc,".spr");
	if (str) strcpy(str,"Team.spr");

	m_AvatarImg->SetImage(ISI_T_SPR, ImageNpc);
	m_AvatarImg->Show();
}
void KUiPlayerBar::Breathe()
{
	if (g_LoginLogic.GetStatus() != LL_S_IN_GAME)
		return;

	m_DateTime.UpdateData();
	int nMana;
	int nFullMana;
	UpdateXXXNumber(nMana, nFullMana);
	int nMoney;
	int nLevel;
	UpdateRuntimeAttribute(nMoney, nLevel);

	DWORD dwID;
	BYTE cost;
	char* Buffer;
	int nLen;
	int nUseMana = 0;
	int nUseMoney = 0;
	int nUseLevel = 0;

	while(KUiMsgCentrePad::GetChannelData(dwID, cost, Buffer, nLen))
	{
		int nRet = IsHasCost(cost, nMoney, nLevel, nMana, nFullMana,
			nUseMoney, nUseLevel, nUseMana);

		bool bSend = !nRet;
		char szSystem[256];
		szSystem[0] = 0;

		if (!bSend)
		{
			if (nRet == 2 && nUseLevel > 0)
				sprintf(szSystem, "Ъng c蕄 c馻 b筺 kh玭g  [%d], kh玭g th?s?d鬾g [Thi猲 l?truy襫 ﹎ c玭g]  chuy觧 ph竧 tin t鴆", nUseLevel);

			if (nRet == 1 && nUseMoney > 0)
				sprintf(szSystem, "Money c馻 b筺 kh玭g  [%d]  s?d鬾g Thi猲 l?truy襫 ﹎ c玭g", nUseMoney);

			if (nRet == 3 && nUseMana > 0)
				sprintf(szSystem, "N閕 l鵦 c馻 b筺 kh玭g  [%d]  s?d鬾g Thi猲 l?truy襫 ﹎ c玭g", nUseMana);
		}
		else
		{
			if (nUseMoney > 0)
				sprintf(szSystem, "B筺  s?d鬾g [Thi猲 l?truy襫 ﹎ c玭g] chuy觧 ph竧 l祄 ti猽 hao [%d] money", nUseMoney);
			if (nUseMana > 0)
				sprintf(szSystem, "B筺  s?d鬾g [Thi猲 l?truy襫 ﹎ c玭g] chuy觧 ph竧 l祄 ti猽 hao [%d] n閕 l鵦", nUseMana);
		}

		if (bSend)
			OnDirectSendChannelMessage(dwID, cost, Buffer, nLen);

		KUiMsgCentrePad::PopChannelData(dwID);
		
		if (szSystem[0])
			KUiMsgCentrePad::SystemMessageArrival(szSystem, strlen(szSystem));
	}
	
	if (m_nCurChannel >= 0 && m_nCurChannel < (int)KUiMsgCentrePad::GetChannelCount() &&
		Wnd_GetFocusWnd() == (KWndWindow*)&m_InputEdit)
	{
		cost = KUiMsgCentrePad::GetChannelCost(KUiMsgCentrePad::GetChannelID(m_nCurChannel));
		if (IsHasCost(cost, nMoney, nLevel, nMana, nFullMana,
			nUseMoney, nUseLevel, nUseMana) != 0)
			m_InputEdit.SetFocusBkColor(s_NotCanFocusColor);
		else
			m_InputEdit.SetFocusBkColor(s_CanFocusColor);
	}

	if (!m_CheckParty.IsButtonChecked())
	{
		m_Party.clear();
		HideParty();
		return;
	}

	KUiPlayerTeam	m_Info;
	memset(&m_Info, 0, sizeof(m_Info));

	if (g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (unsigned int)&m_Info, 0))
	{
		if (m_Info.cNumMember > 0)
		{
			KUiPlayerItem* m_pPlayersList = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * (m_Info.cNumMember));
			if (m_pPlayersList)
			{
				int j,i;
				
				g_pCoreShell->TeamOperation(TEAM_OI_GD_MEMBER_LIST, (unsigned int)m_pPlayersList, m_Info.cNumMember);

				for (i = 0; i < m_Info.cNumMember; i++)
				{
					for (j = 0; j < m_Party.size(); j++)// kiem tra ten co' chua
					{
						if (m_Party[j] == m_pPlayersList[i].uId)
							break;
					}

					if (j >= m_Party.size())// ten chua co' thi them vo
					{
						m_Party.push_back(m_pPlayersList[i].uId);

					}
					if (!m_pPlayersList[i].nIndex || !m_pPlayersList[i].nData)
						continue;

					if (j == 0)
					{
						m_BgImg1.Show();
						m_NameImg1.Show();m_NameImg1.SetText(m_pPlayersList[i].Name);
						ShowIt(m_pPlayersList[i].uId,m_pPlayersList[i].headImage,&m_AvatarImg1);

						m_LifeImg1.Show();
						m_LifeImg1.SetPart(m_pPlayersList[i].nIndex,m_pPlayersList[i].nData, true);
					}
					else
					if (j == 1)
					{
						m_BgImg2.Show();
						m_NameImg2.Show();m_NameImg2.SetText(m_pPlayersList[i].Name);
						ShowIt(m_pPlayersList[i].uId,m_pPlayersList[i].headImage,&m_AvatarImg2);

						m_LifeImg2.Show();
						m_LifeImg2.SetPart(m_pPlayersList[i].nIndex,m_pPlayersList[i].nData, true);
					}
					else
					if (j == 2)
					{
						m_BgImg3.Show();
						m_NameImg3.Show();m_NameImg3.SetText(m_pPlayersList[i].Name);
						ShowIt(m_pPlayersList[i].uId,m_pPlayersList[i].headImage,&m_AvatarImg3);

						m_LifeImg3.Show();
						m_LifeImg3.SetPart(m_pPlayersList[i].nIndex,m_pPlayersList[i].nData, true);
					}
					else
					if (j == 3)
					{
						m_BgImg4.Show();
						m_NameImg4.Show();m_NameImg4.SetText(m_pPlayersList[i].Name);
						ShowIt(m_pPlayersList[i].uId,m_pPlayersList[i].headImage,&m_AvatarImg4);


						m_LifeImg4.Show();
						m_LifeImg4.SetPart(m_pPlayersList[i].nIndex,m_pPlayersList[i].nData, true);
					}
					else
					if (j == 4)
					{
						m_BgImg5.Show();
						m_NameImg5.Show();m_NameImg5.SetText(m_pPlayersList[i].Name);
						ShowIt(m_pPlayersList[i].uId,m_pPlayersList[i].headImage,&m_AvatarImg5);

						m_LifeImg5.Show();
						m_LifeImg5.SetPart(m_pPlayersList[i].nIndex,m_pPlayersList[i].nData, true);
					}
					else
					if (j == 5)
					{
						m_BgImg6.Show();
						m_NameImg6.Show();m_NameImg6.SetText(m_pPlayersList[i].Name);
						ShowIt(m_pPlayersList[i].uId,m_pPlayersList[i].headImage,&m_AvatarImg6);

						m_LifeImg6.Show();
						m_LifeImg6.SetPart(m_pPlayersList[i].nIndex,m_pPlayersList[i].nData, true);
					}
					else
					if (j == 6)
					{
						m_BgImg7.Show();
						m_NameImg7.Show();m_NameImg7.SetText(m_pPlayersList[i].Name);
						ShowIt(m_pPlayersList[i].uId,m_pPlayersList[i].headImage,&m_AvatarImg7);

						m_LifeImg7.Show();
						m_LifeImg7.SetPart(m_pPlayersList[i].nIndex,m_pPlayersList[i].nData, true);
					}
					else
					{
						m_BgImg8.Show();
						m_NameImg8.Show();m_NameImg8.SetText(m_pPlayersList[i].Name);
						ShowIt(m_pPlayersList[i].uId,m_pPlayersList[i].headImage,&m_AvatarImg8);

						m_LifeImg8.Show();
						m_LifeImg8.SetPart(m_pPlayersList[i].nIndex,m_pPlayersList[i].nData, true);
					}
				}
				while (m_Party.size() > m_Info.cNumMember)
				{
					m_Party.erase (m_Party.begin()+m_Party.size()-1);
					//m_Party.removeAt.DeleteString(m_Party.size()-1);
				}
				for (i = m_Info.cNumMember; i < 8; i++)
				{
					if (i == 0)
					{
						m_BgImg1.Hide();
						m_AvatarImg1.Hide();
						m_LifeImg1.Hide();
					}
					else
					if (i == 1)
					{
						m_BgImg2.Hide();
						m_AvatarImg2.Hide();
						m_LifeImg2.Hide();
					}
					else
					if (i == 2)
					{
						m_BgImg3.Hide();
						m_AvatarImg3.Hide();
						m_LifeImg3.Hide();
					}
					else
					if (i == 3)
					{
						m_BgImg4.Hide();
						m_AvatarImg4.Hide();
						m_LifeImg4.Hide();
					}
					else
					if (i == 4)
					{
						m_BgImg5.Hide();
						m_AvatarImg5.Hide();
						m_LifeImg5.Hide();
					}
					else
					if (i == 5)
					{
						m_BgImg6.Hide();
						m_AvatarImg6.Hide();
						m_LifeImg6.Hide();
					}
					else
					if (i == 6)
					{
						m_BgImg7.Hide();
						m_AvatarImg7.Hide();
						m_LifeImg7.Hide();
					}
					else
					{
						m_BgImg8.Hide();
						m_AvatarImg8.Hide();
						m_LifeImg8.Hide();
					}
				}
				free(m_pPlayersList);
			}
		}
		else
		{
			if (m_Party.size())
			{
				m_Party.clear();
				HideParty();
			}
		}
	}
	else
	{
		if (m_Party.size())
		{
			m_Party.clear();
			HideParty();
		}
	}
}

//0 成功, 返回消耗的银两和内力
//1 因为nUseMoney,钱不足
//2 因为nUseLevel,等级不足
//3 因为nUseMana,内力不足
int KUiPlayerBar::IsHasCost(BYTE cost, int nMoney, int nLevel, int nMana, int nFullMana, int& nUseMoney, int& nUseLevel, int& nUseMana)
{
	if (cost == 0)//免费
	{
		nUseMoney = 0;
		nUseLevel = 0;
		nUseMana = 0;
	}
	else if (cost == 1)//10元每句
	{
		if (nMoney < 100)
		{
			nUseMoney = 100;
			return 1;
		}
		nUseMoney = 100;
		nUseLevel = 0;
		nUseMana = 0;
	}
	else if (cost == 2)//2: <10Lv ? 不能说 : MaxMana/2/句
	{
		if (nLevel < 20)
		{
			nUseLevel = 20;
			return 2;
		}
		if (nMoney < 1000)
		{
			nUseMoney = 1000;
			return 1;
		}
		if (nMana < nFullMana / 2)
		{
			nUseMana = nFullMana / 2;
			return 3;
		}
		nUseMoney = 1000;
		nUseLevel = 20;
		nUseMana = nFullMana / 2;
	}
	else if (cost == 3)//3: MaxMana/10/句
	{
		if (nLevel < 10)
		{
			nUseLevel = 10;
			return 2;
		}
		if (nMoney < 500)
		{
			nUseMoney = 500;
			return 1;
		}
		if (nMana < nFullMana / 10)
		{
			nUseMana = nFullMana / 10;
			return 3;
		}
		nUseMoney = 500;
		nUseLevel = 10;
		nUseMana = nFullMana / 10;
	}
	else if (cost == 4)//4: <20Lv ? 不能说 : MaxMana*4/5/句
	{
		if (nLevel < 30)
		{
			nUseLevel = 30;
			return 2;
		}
		if (nMoney < 5000)
		{
			nUseMoney = 5000;
			return 1;
		}
		if (nMana < nFullMana * 4 / 5)
		{
			nUseMana = nFullMana * 4 / 5;
			return 3;
		}
		nUseMoney = 5000;
		nUseLevel = 30;
		nUseMana = nFullMana * 4 / 5;
	}
	else
	{
		nUseMoney = 0;
		nUseLevel = 0;
		nUseMana = 0;
	}
	return 0;
}

//--------------------------------------------------------------------------
//	功能：往输入框入姓名字符串
//--------------------------------------------------------------------------

void KUiPlayerBar::InputNameMsg(char bChannel, const char* szName, bool bFocus)
{
	if (m_pSelf == NULL)
		return;

	if (bFocus)
		Wnd_SetFocusWnd(&m_pSelf->m_InputEdit);
	
	if (szName == NULL || szName[0] == 0)	//名字输入为空返回
		return;

	if (bChannel)	//当输入为频道时,什么都不做,因为用户不需要此功能,但是后面的代码难得改了
		return;

	char Buffer[512];
	int nLen = m_pSelf->m_InputEdit.GetText(Buffer, sizeof(Buffer), false);
	if (nLen < 0)
		nLen = 0;
	Buffer[nLen] = 0;

	int nName = 0;
	if (Buffer[nName] == TEXT_CTRL_CHAT_PREFIX)
	{
		while (nName <= nLen)
		{
			if (Buffer[nName] == ' ' ||
				Buffer[nName] == 0)
			{
				Buffer[nName] = 0;
				break;
			}
			nName++;
		}

		if (nName > nLen)	//原输入中没有名字
			nName = 0;
	}
	
	if (bChannel)
	{
		m_pSelf->m_InputEdit.SetText("");
		//m_pSelf->m_InputEdit.SetText("&");
		//m_pSelf->m_InputEdit.InsertString(szName, strlen(szName));
		//m_pSelf->m_InputEdit.InsertString(" ", 1);
		m_pSelf->m_InputEdit.InsertString(Buffer + nName, nLen - nName);
	}
	else
	{
		if (nName > 0 &&
			(KShortcutKeyCentre::FindCommand(Buffer + 1) >= 0 ||
			KShortcutKeyCentre::ms_FunsMap.find(Buffer + 1) != KShortcutKeyCentre::ms_FunsMap.end()
			))	//当原输入串中有"/"且是函数关键字时
		{	//只追加" name"
			m_pSelf->m_InputEdit.InsertString(" ", 1);
			m_pSelf->m_InputEdit.InsertString(szName, strlen(szName));
		}
		else
		{
			m_pSelf->m_InputEdit.SetText("/");
			m_pSelf->m_InputEdit.InsertString(szName, strlen(szName));
			m_pSelf->m_InputEdit.InsertString(" ", 1);
			m_pSelf->m_InputEdit.InsertString(Buffer + nName, nLen - nName);
		}
	}
}

int	KUiPlayerBar::FindRecentPlayer(const char* szName)
{
	if (szName == NULL || szName[0] == 0)
		return -1;
	for (int n = 0; n < m_pSelf->m_nRecentPlayerName; n++)
	{
		if (m_pSelf->m_RecentPlayerName[n][0] == 0)
		{
			break;
		}
		if (strcmp(m_pSelf->m_RecentPlayerName[n], szName) == 0)
		{
			return n;
		}
	}

	return -1;
}

//--------------------------------------------------------------------------
//	功能：根据index取得某密聊频道的目标玩家名
//--------------------------------------------------------------------------
char* KUiPlayerBar::GetRecentPlayerName(int nIndex)
{
	int nChannelCount = KUiMsgCentrePad::GetChannelCount();
	if(m_pSelf && nIndex >= nChannelCount && nIndex < m_pSelf->m_nRecentPlayerName + nChannelCount)
	{
		return m_pSelf->m_RecentPlayerName[nIndex - nChannelCount];
	}
	else
	{
		return "";
	}
}
int KUiPlayerBar::AddRecentPlayer(const char* szName)
{
	if (!m_pSelf)
		return -1;
	if (szName == NULL || szName[0] == 0)
		return -1;
	if (strcmp(m_pSelf->m_szSelfName, szName) == 0)
		return -1;
	int n = m_pSelf->GetChannelIndex(szName);
	if (n >= 0)
		return n;
	n = 0;
	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
	for (; n < MAX_RECENTPLAYER_COUNT; n++)
	{
		if (m_pSelf->m_RecentPlayerName[n][0] == 0)
		{
			strncpy(m_pSelf->m_RecentPlayerName[n], szName, 32);
			m_pSelf->m_nRecentPlayerName++;
			return n + nChannelDataCount;
		}
	}

	if (n >= MAX_RECENTPLAYER_COUNT)
	{
		memmove(m_pSelf->m_RecentPlayerName[0], m_pSelf->m_RecentPlayerName[1], sizeof(m_pSelf->m_RecentPlayerName) - 32);
		n = MAX_RECENTPLAYER_COUNT - 1;
		strncpy(m_pSelf->m_RecentPlayerName[n], szName, 32);
		return n + nChannelDataCount;
	}

	return -1;
}

void KUiPlayerBar::ReplaceSpecialName(char* szDest, size_t nDestSize, char* szSrc)
{
	KUiMsgCentrePad::ReplaceChannelName(szDest, nDestSize, szSrc);
}

bool KUiPlayerBar::GetExp(int& nFull, int& nCurrLevelExp, int& nCurrentExp)
{
	if (m_pSelf)
	{
		nFull = m_pSelf->m_nExperienceFull;
		nCurrLevelExp = m_pSelf->m_nCurLevelExperience;
		nCurrentExp = m_pSelf->m_nExperience;
		return true;
	}

	return false;
}

BOOL KUiPlayerBar::LoadPrivateSetting(KIniFile* pFile)
{
	if (pFile && g_pCoreShell)
	{
		int nName = 0;
		int nLife = 0;
		int nPK = 0;

		if (pFile->GetInteger("Player", "ShowLife", 0, (int*)(&nLife)))
			g_pCoreShell->OperationRequest(GOI_SHOW_PLAYERS_LIFE, 0, nLife);
		if (pFile->GetInteger("Player", "ShowName", 0, (int*)(&nName)))
			g_pCoreShell->OperationRequest(GOI_SHOW_PLAYERS_NAME, 0, nName);
		if (pFile->GetInteger("Player", "PK", 0, (int*)(&nPK)))
			g_pCoreShell->OperationRequest(GOI_PK_SETTING, 0, nPK);
	}

	return TRUE;
}

int	KUiPlayerBar::SavePrivateSetting(KIniFile* pFile)
{
	if (pFile && g_pCoreShell)
	{
		int nLife = g_pCoreShell->GetGameData(GDI_SHOW_PLAYERS_LIFE, 0, 0);
		pFile->WriteInteger("Player", "ShowLife", nLife);
		int nName = g_pCoreShell->GetGameData(GDI_SHOW_PLAYERS_NAME, 0, 0);
		pFile->WriteInteger("Player", "ShowName", nName);
		int nPK = g_pCoreShell->GetGameData(GDI_PK_SETTING, 0, 0);
		pFile->WriteInteger("Player", "PK", nPK);
	}

	return 1;
}

void KUiPlayerBar::SetChatItem(ChatItem CItem, unsigned int uId)
{
	if(!uId) return;
	if (m_pSelf)
	{
		ZeroMemory(m_pSelf->m_ChatItemInfo,sizeof(m_pSelf->m_ChatItemInfo));
		ZeroMemory(m_pSelf->m_ItemName,sizeof(m_pSelf->m_ItemName));
		char Buffer[16];
		int nOffset = 0;
		m_pSelf->m_ChatItemInfo[nOffset] = '[';
		nOffset++;
		itoa(CItem.nKind, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ',';
		nOffset++;
		itoa(CItem.nItemGenre, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ',';
		nOffset++;
		itoa(CItem.nDetailType, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ',';
		nOffset++;
		itoa(CItem.nParticularType, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ',';
		nOffset++;
		itoa(CItem.nSeries, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ',';
		nOffset++;
		itoa(CItem.nLevel, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ',';
		nOffset++;
		itoa(CItem.nLuck, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ',';
		nOffset++;
		for ( int j = 0; j<6 ; j++)
		{
			sprintf(Buffer, "%d", CItem.nParamX[j]);
			//itoa(CItem.pnMagicParam[i], Buffer, 10);
			strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
			nOffset += strlen(Buffer);
			// if (j == 5) break;
			m_pSelf->m_ChatItemInfo[nOffset] = ',';
			nOffset++;
		}

		for ( int i = 0; i<12 ; i++)
		{
			sprintf(Buffer, "%d", CItem.pnMagicParam[i]);
			//itoa(CItem.pnMagicParam[i], Buffer, 10);
			strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
			nOffset += strlen(Buffer);
			//if (i == 11) break;
			m_pSelf->m_ChatItemInfo[nOffset] = ',';
			nOffset++;
		}

		itoa(CItem.m_Time.bYear, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ',';
		nOffset++;
		itoa(CItem.m_Time.bMonth, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ',';
		nOffset++;
		itoa(CItem.m_Time.bDay, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ',';
		nOffset++;
		itoa(CItem.m_Time.bHour, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ',';
		nOffset++;
		
		itoa(CItem.iNgoaiTrang, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ',';
		nOffset++;
		
		itoa(CItem.nDurability, Buffer, 10);
		strcat(&m_pSelf->m_ChatItemInfo[nOffset],Buffer);
		nOffset += strlen(Buffer);
		m_pSelf->m_ChatItemInfo[nOffset] = ']';
		nOffset++;
		
		m_pSelf->m_ChatItemInfo[nOffset] = '\0';
		char szName[64];
		char szName2[64];
		g_pCoreShell->GetGameData(GDI_ITEM_NAME, (unsigned int)&szName, uId);
		int nLen = strlen(szName);
		szName2[0] = '<';
		strncpy(szName2+1,szName,nLen);
		strncpy(szName2+1+nLen,">",1);
		nLen += 2;
		szName2[nLen] = '\0';
		memcpy(m_pSelf->m_ItemName,szName2,64);
		m_pSelf->m_InputEdit.InsertString(szName2, nLen);
	}
}

void KUiPlayerBar::SetItemBtnInfo(int nBtnNo, ChatItem * pItem)
{
	if(nBtnNo >=0 && nBtnNo < MAX_ITEMBUTTON && g_pCoreShell && m_pSelf && pItem)
	{
		m_pSelf->m_ItemBtn[nBtnNo].SetItemInfo(pItem);
		int nIdx = g_pCoreShell->GetGameData(GDI_ITEM_CHAT, true, (int)&m_pSelf->m_ItemBtn[nBtnNo].m_Item);
		if(!nIdx)
			return;

		char szName[64];
		char szName2[64];
		int nKind = g_pCoreShell->GetKind(nIdx);
		if(nKind == 2)
			m_pSelf->m_ItemBtn[nBtnNo].SetColor(0xffcc33ff,0xff000000,0xff00ffff,0xff000000,0xffff6699,0xff000000);
		else if(nKind == 1)
			m_pSelf->m_ItemBtn[nBtnNo].SetColor(0xffffd94e,0xff000000,0xff00ffff,0xff000000,0xffff6699,0xff000000);
		else if(g_pCoreShell->IsBlueItem(nIdx) && g_pCoreShell->GetGenre(nIdx) == 0)
			m_pSelf->m_ItemBtn[nBtnNo].SetColor(0xff6464ff,0xff000000,0xff00ffff,0xff000000,0xffff6699,0xff000000);
		else
			m_pSelf->m_ItemBtn[nBtnNo].SetColor(0xffffffff,0xff000000,0xff00ffff,0xff000000,0xffff6699,0xff000000);
		g_pCoreShell->GetGameData(GDI_ITEM_NAME, (unsigned int)&szName, nIdx);
		int nLen = strlen(szName);
		szName2[0] = '<';
		strncpy(szName2+1,szName,nLen);
		strncpy(szName2+1+nLen,">",1);
		nLen += 2;
		szName2[nLen] = '\0';
		m_pSelf->m_ItemBtn[nBtnNo].SetSize(nLen*6,14);
		m_pSelf->m_ItemBtn[nBtnNo].SetText(szName2,nLen);
		m_pSelf->m_ItemBtn[nBtnNo].m_bHaveItem = true;
		g_pCoreShell->GetGameData(GDI_ITEM_CHAT, false, nIdx);
	}
}

void KUiPlayerBar::ClearItemBtn()
{
	if(m_pSelf)
	{
		for (int j =0; j < MAX_ITEMBUTTON; j++)
		{
			if(m_pSelf->m_ItemBtn[j].m_X == -300 && m_pSelf->m_ItemBtn[j].m_Y == 100)
				continue;
			m_pSelf->m_ItemBtn[j].SetPosition(INVISIBLE_POS);
			m_pSelf->m_ItemBtn[j].Hide();
		}
	}
}

void KUiPlayerBar::SetItemBtnPos(int nBtnNo,int X, int Y)
{
	if(nBtnNo >=0 && nBtnNo < MAX_ITEMBUTTON)
	{
		if(m_pSelf)
		{
			if(m_pSelf->m_ItemBtn[nBtnNo].m_bHaveItem == false)
				return;

			if(m_pSelf->m_ItemBtn[nBtnNo].m_X == -300 == X && m_pSelf->m_ItemBtn[nBtnNo].m_Y == 100 == Y)
				return;

			m_pSelf->m_ItemBtn[nBtnNo].SetPosition(X,Y);
			m_pSelf->m_ItemBtn[nBtnNo].m_X = X;
			m_pSelf->m_ItemBtn[nBtnNo].m_Y = Y;
			if(m_pSelf->m_ItemBtn[nBtnNo].m_X == -300 && m_pSelf->m_ItemBtn[nBtnNo].m_Y == 100)
				m_pSelf->m_ItemBtn[nBtnNo].Hide();
			else
				m_pSelf->m_ItemBtn[nBtnNo].Show();
		}
	}
}
void KUiPlayerBar::Clear()
{
	if (m_pSelf)
		m_pSelf->CloseWindow(true);
///	if (ms_pStateList)
//	{
//		free (ms_pStateList);
//		ms_pStateList = NULL;
//	}
//	ms_nNumState = 0;
}