/*****************************************************************************************
//	界面--游戏显示玩家信息主界面
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-16
*****************************************************************************************/
#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndText.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndValueImage.h"
#include "../Elem/WndObjContainer.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndList2.h"

#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../Core/src/GameDataDef.h"
#include "UiMsgCentrePad.h"


#include "../Elem/ComWindow.h"
#define	INVISIBLE_POS	-300,100
//时间
class GameWorld_DateTime : public KWndLabeledButton
{
public:
	DECLARE_COMCLASS(GameWorld_DateTime)
	GameWorld_DateTime();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);	//初始化
	void            UpdateData();

private:
	int             m_nSmoothPing, m_nCrowdPing, m_nBlockPing;
	bool            m_bNetStatus;
	bool			m_bRegionName;
	bool			m_bServerName;
	bool			m_bLogo;
	char            m_szRegionName[32], m_szServerName[32], m_szSmoothMsg[32], m_szCrowdMsg[32], m_szBlockMsg[32];
	unsigned int	m_uLastSwitchTime, m_uDefaultColor, m_uSmoothColor, m_uCrowdColor, m_uBlockColor;



private:
	void OnButtonClick();
	void UpdateNetStatus();
};

class KImmediaItem : public KWndObjectBox
{
public:
	int m_nIndex;
	DWORD m_TextColor;

	KImmediaItem() :m_nIndex(-1), m_TextColor(0xFFFFFF00){}
	void PaintWindow();
	int	Init(KIniFile* pIniFile, const char* pSection);

	bool isImmedia(){ return m_Object.uGenre != CGOG_SKILL; }
	bool CheckBox(int x,int y);

	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
};

struct KUiDraggedObject;
struct KPopupMenuData;
class KIniFile;

#include <vector>

class KUiPlayerBar : protected KWndImage
{
public:
	void DropSkill(int x,int y);

	//----界面面板统一的接口函数----
	static KUiPlayerBar* OpenWindow();					//打开窗口，返回唯一的一个类对象实例
	static KUiPlayerBar* GetIfVisible();				//如果窗口正被显示，则返回实例指针
	static void			CloseWindow(bool bDestroy);		//关闭窗口，同时可以选则是否删除对象实例
	static void			LoadScheme(const char* pScheme);//载入界面方案

	static void			InputNameMsg(char bChannel, const char* szName, bool bFocus);
	static int			FindRecentPlayer(const char* szName);
	static int          AddRecentPlayer(const char* szName);
	static char*        GetRecentPlayerName(int nIndex);//根据index取得某密聊频道的目标玩家名

	static const char*	SelfName()
	{
		if (m_pSelf)
			return m_pSelf->m_szSelfName;
		return NULL;
	}
	static BOOL	IsSelfName(char* szName)
	{
		if (m_pSelf && szName)
		{
			return (strcmpi(m_pSelf->m_szSelfName, szName) == 0);
		}
		return FALSE;
	}
	static void	OnUseItem(int nIndex);				//使用快捷物品
	static BOOL	IsCanSendMessage(const char* Buffer, int nLen, char* szDestName, int nChannelID);				//能发送吗?
	static void	OnSendChannelMessage(DWORD nChannelID, const char* Buffer, int nLen);				//发送频道聊天到服务器
	static void	OnSendSomeoneMessage(const char* Name, const char* Buffer, int nLen);				//发送someone聊天到服务器

	void	UpdateItem(int nIndex, unsigned int uGenre, unsigned int uId);	//变更物品摆换
	void	UpdateSkill(int nIndex, unsigned int uGenre, unsigned int uId);	//变更快捷技能

	static	bool		GetExp(int& nFull, int& nCurrLevelExp, int& nCurrentExp);

	static BOOL			LoadPrivateSetting(KIniFile* pFile);
	static int			SavePrivateSetting(KIniFile* pFile);

	static void         SetCurrentChannel(int nIndex);
	static void         SwitchChannel(BOOL bUp = FALSE);  //切换到下一个频道，到底了就切换到第一个，循环
	                                                      //bUp是true就上升，否则下降
	                                                      //e...下标是0是最顶部的概念
    static int          GetCurChannel();                  //得到PlayerBar里当前的频道
    static void			Clear();
private:
	KUiPlayerBar();
	~KUiPlayerBar();
	void	Breathe();									//活动函数
	void	Initialize();								//初始化
	void	PaintWindow();

	void	UpdateData();								//更新数据
	void	UpdateXXXNumber(int& nMana, int& nFullMana);							//更新常变的那些数值数据
	void	UpdateRuntimeAttribute(int& nMoney, int& nLevel);

	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//窗口函数
	void	OnObjPickedDropped(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);//响应界面操作取起放下东西
	void	LoadScheme(KIniFile* pIni);//载入界面方案

	void	OnSend(BOOL bDirectSend);
	void	OnDirectSendChannelMessage(DWORD nChannelID, BYTE cost, const char* Buffer, int nLen);				//发送频道聊天到服务器
	void	PopupChannelMenu(int x, int y);
	void	PopupPhraseMenu(int x, int y, bool bFirstItem);
	void	InputRecentMsg(bool bPrior);
	void	OnSwitchSize();			//切换大小
 
	int GetChannelIndex(const char* pTitle);
	//void SetCurrentChannel(int nIndex);
	void InputCurrentChannel(int nIndex, bool bFocus);
	//返回在Channel中的索引, < 0 失败
	//int AddRecentPlayer(const char* szName);
	void ReplaceSpecialName(char* szDest, size_t nDestSize, char* szSrc);
	
	int IsHasCost(BYTE cost, int nMoney, int nLevel, int nMana, int nFullMana, int& nUseMoney, int& nUseLevel, int& nUseMana);

private:
	static KUiPlayerBar* m_pSelf;
private:
	KWndButton		m_Face;		//表情
	KWndButton		m_Friend;	//好友	
	KWndButton		m_EscDlg;	//系统选项

	KWndButton	m_CheckParty;

	void HideParty();
	std::vector<unsigned int> m_Party;

	KWndImage m_BgImg1;
	KWndImage m_BgImg2;
	KWndImage m_BgImg3;
	KWndImage m_BgImg4;
	KWndImage m_BgImg5;
	KWndImage m_BgImg6;
	KWndImage m_BgImg7;
	KWndImage m_BgImg8;

	void ShowIt(unsigned int uId, int x, KWndImage* m_AvatarImg);
	KWndImage m_AvatarImg1;
	KWndImage m_AvatarImg2;
	KWndImage m_AvatarImg3;
	KWndImage m_AvatarImg4;
	KWndImage m_AvatarImg5;
	KWndImage m_AvatarImg6;
	KWndImage m_AvatarImg7;
	KWndImage m_AvatarImg8;

	KWndText32 m_NameImg1;
	KWndText32 m_NameImg2;
	KWndText32 m_NameImg3;
	KWndText32 m_NameImg4;
	KWndText32 m_NameImg5;
	KWndText32 m_NameImg6;
	KWndText32 m_NameImg7;
	KWndText32 m_NameImg8;

	KWndImagePart m_LifeImg1;
	KWndImagePart m_LifeImg2;
	KWndImagePart m_LifeImg3;
	KWndImagePart m_LifeImg4;
	KWndImagePart m_LifeImg5;
	KWndImagePart m_LifeImg6;
	KWndImagePart m_LifeImg7;
	KWndImagePart m_LifeImg8;

	GameWorld_DateTime		m_DateTime;		//时间

#define	UPB_IMMEDIA_ITEM_COUNT	5
	KImmediaItem	m_ImmediaItem[UPB_IMMEDIA_ITEM_COUNT];
	KWndObjectBox	m_ImmediaSkill[2];
	KWndButton	m_SendBtn;	//发送
	KWndEdit512	m_InputEdit;	//输入框
	KWndPureTextBtn	m_ChannelSwitchBtn;	//切换频道
	KWndButton	m_ChannelOpenBtn;	//频道打开
    KWndButton		m_Auto;
	KWndButton		m_MarketBtn;

	KWndButton	m_SwitchBtn;	//切换大小

	char		m_cPreMsgCounter;
	char		m_cLatestMsgIndex;


#define MAX_ITEMBUTTON 120

public:
	static void			SetChatItem(ChatItem CItem, unsigned int uId);//ghi nho 1 chat item
	static void			SetItemBtnInfo(int nBtnNo, ChatItem * pItem); // dat id item rao ban
	static void			ClearItemBtn(); //xoa het button
	static void			SetItemBtnPos(int nBtnNo,int X, int Y); // dat vi tri button chat
private:
	KWndPureTextBtn		m_ItemBtn[MAX_ITEMBUTTON];//Item rao ban
	char			m_ChatItemInfo[128]; //thong tin item se duoc gui di
	char			m_ItemName[64]; // ten item


#define	MAX_RECENT_MSG_COUNT	8
	char		m_RecentMsg[MAX_RECENT_MSG_COUNT][512];
	int m_nCurChannel;
#define	MAX_RECENTPLAYER_COUNT	10
	char m_RecentPlayerName[MAX_RECENTPLAYER_COUNT][32];
	int m_nRecentPlayerName;

	int				m_bMiniMode;
	
	char			m_szSelfName[32];	//自己的名字

	int				m_nExperienceFull;
	int				m_nCurLevelExperience;
	int				m_nExperience;
};
