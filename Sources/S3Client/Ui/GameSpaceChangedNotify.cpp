#include "KWin32.h"

#include "Windows.h"
#include "../../core/src/coreshell.h"
#include "../../core/src/GameDataDef.h"
#include "../../core/src/CoreObjGenreDef.h"
#include "Windows.h"
#include "KEngine.h"
#include "Elem/Wnds.h"
#include "Elem/MouseHover.h"
#include "UiCase/UiPlayerBar.h"
#include "UiCase/UiStatus.h"
#include "UiCase/UiTrade.h"
#include "UiCase/UiSkills.h"
#include "UiCase/UiItem.h"
#include "UiCase/UiShop.h"
#include "UiCase/UiShopGold.h"
#include "UiCase/UiNewsMessage1.h"
#include "UiCase/UiMsgSel2.h"
#include "UiCase/UiMsgSel3.h"
#include "UiCase/UiMsgSel.h"
#include "UiCase/UiMsgCentrePad.h"
#include "UiCase/UiInformation2.h"
#include "UiCase/UiSysMsgCentre.h"
#include "UiCase/UiTeamManage.h"
#include "UiCase/UiSelPlayerNearby.h"
#include "UiCase/UiChatCentre.h"
#include "UiCase/UiStoreBox.h"

#include "UiCase/UiNewPWStoreBox.h"

#include "UiCase/UiRPNewBox.h"

#include "UiCase/UiTopTKNew.h"

#include "UiCase/UiCheckItemBox.h"
#include "UiCase/UiBuyShop.h"
#include "UiCase/UiSlectBoxCheck.h"
#include "UiCase/UiPGBoxItem.h"
#include "UiCase/UiPGBoxItem.h"
#include "UiCase/UiTaskNote.h"
#include "UiCase/UiNewsMessage.h"
#include "UiCase/UiStrengthRank.h"
#include "UiCase/UiTongManager.h"
#include "UiCase/UiTongCreateSheet.h"
#include "UiCase/UiStringBox.h"
#include "../Login/Login.h"
#include "UiCase/UiParadeItem.h"
#include "KTongProtocol.h"
#include "UiCase/UiTimeBox.h"
#include "UiCase/UiInit.h"

#include "../S3Client.h"
#include "UiShell.h"

#include "../../Engine/Src/Text.h"

bool UiCloseWndsInGame(bool bAll);

extern iCoreShell*		g_pCoreShell;

void GameWorldTips(unsigned int uParam, int nParam);

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º½ÓÊÜÓÎÏ·ÊÀ½çÊý¾Ý¸Ä±äÍ¨ÖªµÄº¯Êý
//--------------------------------------------------------------------------
void CoreDataChangedCallback(unsigned int uDataId, unsigned int uParam, int nParam)
{
	KUiTrade* pTradeBar = NULL;
	switch(uDataId)
	{
	case GDCNI_HOLD_OBJECT:
		Wnd_DragFinished();
		if (uParam && ((KUiObjAtRegion*)uParam)->Obj.uGenre != CGOG_NOTHING)
		{
			KUiDraggedObject Obj;
			Obj.uGenre = ((KUiObjAtRegion*)uParam)->Obj.uGenre;
			Obj.uId = ((KUiObjAtRegion*)uParam)->Obj.uId;
			Obj.DataW = ((KUiObjAtRegion*)uParam)->Region.Width;
			Obj.DataH = ((KUiObjAtRegion*)uParam)->Region.Height;			
			Wnd_DragBegin(&Obj, DrawDraggingGameObjFunc);
		}
		break;
	case GDCNI_PLAYER_BASE_INFO:	//Ö÷½ÇµÄÒ»Ð©²»Ò×±äµÄÊý¾Ý
		{
			KUiStatus* pBar = KUiStatus::GetIfVisible();
			if (pBar)
				pBar->UpdateBaseData();
			
			KUiPlayerBaseInfo	Info;
			memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
			g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (int)&Info, 0);
			if (Info.nCurFaction >= 0)
				KUiMsgCentrePad::QueryAllChannel();
			else
			{
				KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Faction);
				if (Info.nCurTong != 0)
				{
					KUiMsgCentrePad::QueryAllChannel();
				}
				else
				{
					KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Tong);
				}
			}
		}
		break;
	case GDCNI_PLAYER_RT_ATTRIBUTE:
		{
			KUiStatus* pBar = KUiStatus::GetIfVisible();
			if (pBar)
				pBar->UpdateData();
		}
		break;
	case GDCNI_PLAYER_IMMED_ITEMSKILL://Ö÷½ÇµÄÁ¢¼´Ê¹ÓÃÎïÆ·ÓëÎä¹¦
		if (uParam)
		{
			KUiGameObject* pObj = (KUiGameObject*)uParam;
			KUiPlayerBar* pBar = KUiPlayerBar::GetIfVisible();
			if (pBar)
			{
				if (nParam >= 0)
					pBar->UpdateItem(nParam, pObj->uGenre, pObj->uId);
				else
					pBar->UpdateSkill(-nParam - 1, pObj->uGenre, pObj->uId);
			}
		}
		break;

	case GDCNI_OBJECT_CHANGED:
		if (uParam)
		{
			KUiObjAtContRegion* pObject = (KUiObjAtContRegion*)uParam;
			if (pObject->eContainer == UOC_ITEM_TAKE_WITH)
			{
				pTradeBar = KUiTrade::GetIfVisible();
				if (pTradeBar)
				{
					pTradeBar->OnChangedTakewithItem((KUiObjAtRegion*)uParam, nParam);
				}
				else
				{
					KUiItem* pItemsBar = KUiItem::GetIfVisible();
					if (pItemsBar)
						pItemsBar->UpdateItem((KUiObjAtRegion*)uParam, nParam);
				}
			}
			else if (pObject->eContainer == UOC_EQUIPTMENT)
			{
				KUiStatus* pEquips = KUiStatus::GetIfVisible();
				if (pEquips)
					pEquips->UpdateEquip((KUiObjAtRegion*)uParam, nParam);
			}
			else if (pObject->eContainer == UOC_PG_BOX)
			{
				KUiPGBoxItem* pEquips = KUiPGBoxItem::GetIfVisible();
				if (pEquips)
					pEquips->UpdateEquip((KUiObjAtRegion*)uParam, nParam);
			}
			else if (pObject->eContainer == UOC_STORE_BOX)
			{
				KUiStoreBox* pStoreBox = KUiStoreBox::GetIfVisible();
				if (pStoreBox)
					pStoreBox->UpdateItem((KUiObjAtRegion*)uParam, nParam);
			}
			else if (pObject->eContainer == UOC_RPNEW_BOX)
			{
				KUiRPNewBox* pRPNewBox = KUiRPNewBox::GetIfVisible();
				if (pRPNewBox)
					pRPNewBox->UpdateItem((KUiObjAtRegion*)uParam, nParam);
			}
			else if (pObject->eContainer == UOC_CHECK_BOX)
			{
				KUiCheckItemBox* pCheckBox = KUiCheckItemBox::GetIfVisible();
				if (pCheckBox)
					pCheckBox->UpdateItem((KUiObjAtRegion*)uParam, nParam);
			}
			else if (pObject->eContainer == UOC_TO_BE_TRADE)
			{
				KUiTrade* pTrade = KUiTrade::GetIfVisible();
				if (pTrade)
					pTrade->OnSelfChangedItem(pObject, nParam);
			}
			else if (pObject->eContainer == UOC_IN_HAND)
			{
				Wnd_DragFinished();
				if (pObject->Obj.uGenre != CGOG_NOTHING)
				{
					KUiDraggedObject Obj;
					Obj.uGenre = pObject->Obj.uGenre;
					Obj.uId = pObject->Obj.uId;
					Obj.DataW = pObject->Region.Width;
					Obj.DataH = pObject->Region.Height;			
					Wnd_DragBegin(&Obj, DrawDraggingGameObjFunc);
				}
			}
			else if (pObject->eContainer == UOC_IMMEDIA_ITEM)
			{
				KUiPlayerBar* pBar = KUiPlayerBar::GetIfVisible();
				if (pBar)
				{
					if (nParam)
						pBar->UpdateItem(pObject->Region.h, pObject->Obj.uGenre, pObject->Obj.uId);
					else
						pBar->UpdateItem(pObject->Region.h, CGOG_NOTHING, 0);
				}
			}
			else if (pObject->eContainer == UOC_IMMEDIA_SKILL)
			{
				KUiPlayerBar* pBar = KUiPlayerBar::GetIfVisible();
				if (pBar)
				{
					if (nParam)
						pBar->UpdateSkill(pObject->Region.h, pObject->Obj.uGenre, pObject->Obj.uId);
					else
						pBar->UpdateSkill(pObject->Region.h, CGOG_NOTHING, 0);
				}
			}
		}
		break;
	case GDCNI_LIVE_SKILL_BASE:		//Éú»î¼¼ÄÜÊýÖµ±ä»¯
		{
			KUiSkills* pPad = KUiSkills::GetIfVisible();
			if (pPad)
				pPad->UpdateLiveBaseData();
		}
		break;
	case GDCNI_FIGHT_SKILL_POINT:	//Ê£ÓàÕ½¶·¼¼ÄÜµãÊý±ä»¯
		{
			KUiSkills* pPad = KUiSkills::GetIfVisible();
			if (pPad)
			{
				pPad->UpdateFightRemainPoint(nParam);
			}
		}
		break;
	case GDCNI_SKILL_CHANGE:			//ÐÂÕÆÎÕÁËÒ»¸ö¼¼ÄÜ
		if (uParam)
		{
			KUiSkills::UpdateSkill((KUiSkillData*)uParam, nParam);
		}
		break;
/*	case GDCNI_PLAYER_LEADERSHIP:	//Ö÷½ÇÍ³Ë§ÄÜÁ¦Ïà¹ØµÄÊý¾Ý·¢Éú±ä»¯
		{
			KUiManage* pBar = KUiManage::GetIfVisible();
			if (pBar)
				pBar->UpdateLeaderData();
		}
		break;	
*/
	case GDCNI_TRADE_START:
		if (uParam)
		{
			UiCloseWndsInGame(false);
			KUiTrade::OpenWindow((KUiPlayerItem*)uParam);
		}
		break;
	case GDCNI_TRADE_DESIRE_ITEM:	//¶Ô·½Ôö¼õÏë½»Ò×µÄÎïÆ·
		pTradeBar = KUiTrade::GetIfVisible();
		if (pTradeBar)
			pTradeBar->OnOppositeChangedItem((KUiObjAtRegion*)uParam, nParam);
		break;


	case GDCNI_BUY_SHOP_DESIRE_ITEM:

		{

       KUiBuyShop* pBuyShop = KUiBuyShop::GetIfVisible();

		if (pBuyShop)
			pBuyShop->UpdateItem((KUiObjAtRegion*)uParam, nParam);

		break;

		}

	case GDCNI_TRADE_OPER_DATA:		//½»Ò××´Ì¬±ä»¯
		pTradeBar = KUiTrade::GetIfVisible();
		if (pTradeBar)
			pTradeBar->UpdateOperData();
		break;
	case GDCNI_TRADE_END:			//½»Ò×½áÊø
		pTradeBar = KUiTrade::GetIfVisible();
		if (pTradeBar)
			pTradeBar->UpdateTradeEnd(nParam);
		break;
	case GDCNI__TRADE:
		if (uParam == 1)
		{

		KUiShop::CancelTrade();

		if (nParam)
			KUiShopGold::OpenWindow();
		else
			KUiShopGold::CloseWindow();

		}

		else if (uParam == 2)
		{

		KUiShopGold::CancelTrade();

	    if (nParam)
		KUiShop::OpenWindow(2);
		else
		KUiShop::CloseWindow();

		}

		else if (uParam == 3)
		{

		KUiShopGold::CancelTrade();

	    if (nParam)
		KUiShop::OpenWindow(3);
		else
		KUiShop::CloseWindow();

		}

		else

		{
	
		KUiShopGold::CancelTrade();

	    if (nParam)
		KUiShop::OpenWindow(0);
		else
		KUiShop::CloseWindow();

		}

		break;
		
	case GDCNI__TRADE_ITEM:
		{
			KUiShop* pShop = KUiShop::GetIfVisible();
			if (pShop)
				pShop->UpdateData();
		}
		break;
	case GDCNI_QUESTION_CHOOSE:
		KUiMsgSel::OpenWindow((KUiQuestionAndAnswer*)uParam);
		break;
	case GDCNI_QUESTION_CHOOSE_1: // say new
		KUiMsgSel2::OpenWindow((KUiQuestionAndAnswer*)uParam, (KUiNpcSpr*)nParam); 
		break;
	case GDCNI_QUESTION_CHOOSE_2: // say new
		KUiMsgSel3::OpenWindow((KUiQuestionAndAnswer*) uParam, nParam);
		break;
	case GDCNI_GAME_START:
		{
			g_LoginLogic.NotifyToStartGame();
			Wnd_GameSpaceHandleInput(true);
			KUiMsgCentrePad::ReleaseActivateChannelAll();
			KUiMsgCentrePad::QueryAllChannel();
		}
		break;
	case GDCNI_SPEAK_WORDS:			//Ëµ»°ÄÚÈÝ
	//uParam = (KUiInformationParam*) pWordDataList Ö¸ÏòKUiInformationParamÊý×é
	//nParam = pWordDataList°üº¬KUiInformationParamÔªËØµÄÊýÄ¿
		if (uParam && nParam)
			g_UiInformation2.SpeakWords((KUiInformationParam*)uParam, nParam);
		break;
	case GDCNI_INFORMATION:
		if (uParam)
		{
			KUiInformationParam* pInformation = (KUiInformationParam*)uParam;
			KWndWindow*	pCaller = pInformation->bNeedConfirmNotify ? ((KWndWindow*)WND_GAMESPACE) : 0;
			UIMessageBox2(pInformation->sInformation, pInformation->nInforLen,
				pInformation->sConfirmText, pCaller, 0);
		}
		break;
	case GDCNI_CHAT_GROUP:
		KUiChatCentre::UpdateData(UICC_U_ALL, 0, 0);
		break;
	case GDCNI_CHAT_FRIEND:		//ÁÄÌìºÃÓÑ·¢Éú±ä»¯
		KUiChatCentre::UpdateData(UICC_U_GROUP, 0, nParam);
		break;
	case GDCNI_CHAT_FRIEND_STATUS:	//ÁÄÌìºÃÓÑ×´Ì¬·¢Éú±ä»¯
		KUiChatCentre::UpdateData(UICC_U_FRIEND, uParam, nParam);
		break;
	case GDCNI_TEAM:
		{
			KUiTeamManage* pPad = KUiTeamManage::GetIfVisible();
			if (pPad)
				pPad->UpdateData((KUiPlayerTeam*)uParam);
			if (uParam)
				KUiMsgCentrePad::QueryAllChannel();
			else
				KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Team);
		}
		break;
	case GDCNI_QUERY_TONGKIM:
		{
			if (uParam) {
				//g_DebugLog("cap nhat lai danh sach kenh chat");
				KUiMsgCentrePad::QueryAllChannel();
			}
			else
				KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Kim);
		}
		break;
//	case GDCNI_TEAM_NEARBY_LIST:
//		KUiTeamManage::UpdateNearbyTeams((KUiTeamItem*)uParam, nParam);
//		break;
	case GDCNI_SWITCH_CURSOR:
		Wnd_SwitchCursor(nParam);
		break;
	case GDCNI_SYSTEM_MESSAGE:
		if (uParam)
			KUiSysMsgCentre::AMessageArrival((KSystemMessage*)uParam, (void*)nParam);
		break;
	case GDCNI_OPEN_STORE_BOX:
		KUiStoreBox::OpenWindow();
		KUiItem::OpenWindow();
		break;
	case GDCNI_SWITCHING_SCENEPLACE:
		break;
	// flying add this branch to deal with the task record from server.
	case GDCNI_MISSION_RECORD:
		if (uParam)
		{
			KMissionRecord* pRecord = (KMissionRecord*)uParam;
			KUiTaskNote::WakeUp(nParam, pRecord->sContent, pRecord->nContentLen, pRecord->uValue);
		}
		break;
	case GDCNI_PK_SETTING:
		break;
	case GDCNI_VIEW_PLAYERITEM:
		KUiParadeItem::OpenWindow((KUiPlayerItem*)uParam);
		break;
	case GDCNI_PLAYER_BRIEF_PROP:
		GameWorldTips(uParam, nParam);
		break;
	case GDCNI_NEWS_MESSAGE:		//ÐÂÎÅÏûÏ¢
		if (uParam)
			KUiNewsMessage::MessageArrival((KNewsMessage*)uParam, (SYSTEMTIME*)nParam);
		break;
    case GDCNI_NEWS_MESSAGE_1:		//ÐÂÎÅÏûÏ¢
		if (uParam)
			KUiNewsMessage1::MessageArrival1((KNewsMessage1*)uParam, (SYSTEMTIME*)nParam);
		break;
	case GDCNII_RANK_INDEX_LIST_ARRIVE:
		KUiStrengthRank::OpenWindow();
		KUiStrengthRank::NewIndexArrive(uParam, (KRankIndex *)nParam);
		break;
	case GDCNII_RANK_INFORMATION_ARRIVE:
		KUiStrengthRank::NewRankArrive(uParam, (KRankMessage *)nParam);
		break;
	case GDCNI_TONG_INFO:
		KUiTongManager::TongInfoArrive((KUiPlayerRelationWithOther *)uParam, (KTongInfo *)nParam);
		break;
	case GDCNI_TONG_MEMBER_LIST:
		KUiTongManager::NewDataArrive((KUiGameObjectWithName *)uParam, (KTongMemberItem *)nParam);
		break;

	case GDCNI_SHOP_OPEN_CHECK:
		KUiItem::UpdateButton(uParam);
		break;

    case GDCNI_OPEN_TIME_BOX:
		{
			KUiTimeBoxInfo* pInfo = (KUiTimeBoxInfo*)uParam;
			KUiTimeBox::OpenWindow(pInfo->szTitle, pInfo->nTime, pInfo->szAction);
		}
		break;
	case GDCNI_TONG_ACTION_RESULT:
		KUiTongManager::ResponseResult((KUiGameObjectWithName *)uParam, nParam);
		break;
	case GDCNI_OPEN_TONG_CREATE_SHEET:
		if(uParam)
			KUiTongCreateSheet::OpenWindow();
		else
			KUiTongCreateSheet::CloseWindow();
		break;
	case GDCNI_OPEN_STRING_BOX:
		KUiStringBox::OpenWindow((int)uParam,(char*) nParam);
		break;

	case GDCNI_OPEN_CHECK_ITEM_BOX:
		KUiCheckItemBox::OpenWindow((char*)uParam,(char*) nParam);
		break;
	case GDCNI_OPEN_SLECT_CHECK_BOX:
		KUiSlectBoxCheck::OpenWindow(((char*)uParam)[0],((char*)uParam)[1],((char*)uParam)[2]);
		break;
	case GDCNI_OPEN_PG_BOX_ITEM:
		KUiPGBoxItem::OpenWindow();
		break;




	case GDCNI_OPEN_NEW_PW_BOX_ITEM:
		KUiNewPWStoreBox::OpenWindow();
		break;



   case GDCNI_SEND_TOP_TK_NEW_ITEM:

	   if (!KUiTopTKNew::GetIfVisible())
	   {
		KUiTopTKNew::OpenWindow();
	   }

	   if (KUiTopTKNew::GetIfVisible())
	   {
		KUiTopTKNew::UpdateDataTop((KUiTopTKNewData*)uParam);
	   }

		break;
	

	case GDCNI_OPEN_BUY_SHOP_ITEM:
		KUiBuyShop::OpenWindow((char*)uParam);
		break;

    case GDCNI_S2C_EXIT_GAME:
		g_pCoreShell->OperationRequest(GOI_EXIT_GAME, 0, 0);		
		g_LoginLogic.ReturnToIdle();
		UiEndGame();
		KUiInit::OpenWindow(true, false);
		break;
	case GDCNI_OPEN_LOCK_BOX_ITEM:

		if (KUiStoreBox::GetIfVisible())
		KUiStoreBox::UpdateClientDT();

		if (KUiStatus::GetIfVisible())
		KUiStatus::UpdateClientDT();

		
		break;

/*	case GDCNII_SET_MATE_NAME:
		{
			KUiParadeItem* pCom = KUiParadeItem::GetIfVisible();
			if (pCom)
			{
				KUiParadeItem::UpdateMateName((char *)uParam);
			}
			break;
		}*/
	}
}

//////////////////////////////////////////////////////

void KClientCallback::CoreDataChanged(unsigned int uDataId, unsigned int uParam, int nParam)
{
	CoreDataChangedCallback(uDataId, uParam, nParam);
}

typedef std::map<std::string, std::string> BLACKLIST;
BLACKLIST g_BlackListUserNames;

#define BLACKLIST_UNITNAME	 "Sæ ®en"

struct BlacklistNotify : public AddinNotify
{
	virtual int	RenameUnitGroup(const char* Unit, const char* Name, const char* NewName, const STRINGLIST& friends);
	virtual int	MoveUnitGroup(const char* Unit, const char* Name, const char* Name2, const STRINGLIST& friends);
	virtual int	DeleteUnitGroup(const char* Unit, const char* Name, const STRINGLIST& friends);
	virtual int	DeleteUnitMember(const char* Unit, const char* Name);
	virtual int	MoveUnitMember(const char* Unit, const char* Name, const char* Group);
	void	SendNotifyGroupFriend(const char* Unit, const std::string& group, const STRINGLIST& friends);
	void	SendNotifyDeleteFriend(const char* Unit, const char* Name);
};

BlacklistNotify s_LNotify;

void ClearBlackList()
{
	g_BlackListUserNames.clear();
}

void CreateBlackListUnit()
{
	ClearBlackList();
	KUiChatCentre::AddAddinUnit(BLACKLIST_UNITNAME, &s_LNotify);
}

void AddBlackList(const char* strName, const char* strGroup)
{
	if (strName && strName[0] != 0 && strGroup)
	{
		g_BlackListUserNames[strName] = strGroup;

		if (g_BlackListUserNames.size() > 0)
		{
			int nUnit = KUiChatCentre::FindUnitIndex(BLACKLIST_UNITNAME);
			if (nUnit >= 0)
			{
				KUiChatCentre::AddFriendInfo(nUnit, (char*)strName, (char*)strGroup);
			}
		}
	}
}

void RemoveBlackList(char* strName)
{
	if (strName && strName[0] != 0)
	{
		BLACKLIST::iterator i = g_BlackListUserNames.find(strName);
		if (i != g_BlackListUserNames.end())
		{
			g_BlackListUserNames.erase(i);

			int nUnit = KUiChatCentre::FindUnitIndex(BLACKLIST_UNITNAME);
			if (nUnit >= 0)
			{
				KUiChatCentre::DeleteFriend(nUnit, strName, true);
			}
		}
	}
}

bool IsInBlackName(char* strName)
{
	if (strName && strName[0] != 0)
	{
		return g_BlackListUserNames.find(strName) != g_BlackListUserNames.end();
	}

	return false;
}

int	SaveBlackListPrivateSetting(KIniFile* pFile, LPCSTR lpSection, int nStart)
{
	int i = nStart;
	char szKey[10];
	char szLine[256];
	BLACKLIST::iterator iU = g_BlackListUserNames.begin();
	while (iU != g_BlackListUserNames.end())
	{
		sprintf(szKey, "%d", i);
		sprintf(szLine, "AddBlackList(\"%s\", \"%s\")", iU->first.c_str(), iU->second.c_str());
		pFile->WriteString(lpSection, szKey, szLine);
		i++;
		iU++;
	}
	return i;
}

int	BlacklistNotify::RenameUnitGroup(const char* Unit, const char* Name, const char* NewName, const STRINGLIST& friends)
{
	AddinNotify::RenameUnitGroup(Unit, Name, NewName, friends);

	if (NewName && NewName[0])
		SendNotifyGroupFriend(Unit, std::string(NewName), friends);

	return 0;
}

int	BlacklistNotify::MoveUnitGroup(const char* Unit, const char* Name, const char* Name2, const STRINGLIST& friends)
{
	AddinNotify::MoveUnitGroup(Unit, Name, Name2, friends);

	if (Name2 && Name2[0])
		SendNotifyGroupFriend(Unit, std::string(Name2), friends);

	return 0;
}

int	BlacklistNotify::DeleteUnitGroup(const char* Unit, const char* Name, const STRINGLIST& friends)
{
	AddinNotify::DeleteUnitGroup(Unit, Name, friends);

	if (Name && Name[0])
		SendNotifyGroupFriend(Unit, "", friends);

	return 0;
}

int	BlacklistNotify::DeleteUnitMember(const char* Unit, const char* Name)
{
	AddinNotify::DeleteUnitMember(Unit, Name);

	SendNotifyDeleteFriend(Unit, Name);

	return 0;
}

int	BlacklistNotify::MoveUnitMember(const char* Unit, const char* Name, const char* Group)
{
	AddinNotify::MoveUnitMember(Unit, Name, Group);

	if (Name && Name[0])
	{
		STRINGLIST friends;
		friends.push_back(Name);
		SendNotifyGroupFriend(Unit, Group, friends);
	}

	return 0;
}

void BlacklistNotify::SendNotifyGroupFriend(const char* Unit, const std::string& group, const STRINGLIST& friends)
{
	if (strcmp(Unit, BLACKLIST_UNITNAME) == 0)
	{
		STRINGLIST::const_iterator i = friends.begin();
		while (i != friends.end())
		{
			g_BlackListUserNames[(*i)] = group;
			i++;
		}
	}
}

void BlacklistNotify::SendNotifyDeleteFriend(const char* Unit, const char* Name)
{
	if (strcmp(Unit, BLACKLIST_UNITNAME) == 0)
	{
		RemoveBlackList((char*)Name);
	}
}

///////////////////////////////////////////////////////////////////////////////
void KClientCallback::ChannelMessageArrival(int figure, DWORD nChannelID, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, bool bSucc)
{
	if (!bSucc)
	{
		char szInfo[256];
		int n = sprintf(szInfo, "Xin thø lçi! TÇn sè vÉn ch­a më, kh«ng thÓ chuyÓn tin tøc!");
		KUiMsgCentrePad::SystemMessageArrival(szInfo, n);
		return;
	}

	int nIndex = -1;
	if (nChannelID == -1)	//gm alias ID
	{
		//ÕÒµ½ÏµÍ³ÆµµÀ
		int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
		for (int n = 0; n < nChannelDataCount; n++)
		{
			if (KUiMsgCentrePad::IsChannelType(n, KUiMsgCentrePad::ch_GM))
			{
				nIndex = n;
				break;
			}
		}
		if (nIndex < 0)
			return;
		nChannelID = KUiMsgCentrePad::GetChannelID(nIndex);
		if (nChannelID == -1)
			return;
	}
	else
	{
		nIndex = KUiMsgCentrePad::GetChannelIndex(nChannelID);
		if (nIndex < 0)
			return;

		if (IsInBlackName(szSendName))
			return;
	}

	KUiMsgCentrePad::NewChannelMessageArrival(figure, nChannelID, szSendName, pMsgBuff, nMsgLength);

	if (KUiMsgCentrePad::GetChannelSubscribe(nIndex) && KUiMsgCentrePad::IsChannelType(nIndex, KUiMsgCentrePad::ch_Screen))
	{
		KUiPlayerItem SelectPlayer;
		int nKind = -1;
		if (g_pCoreShell->FindSpecialNPC(szSendName, &SelectPlayer, nKind) && nKind == kind_player)
		{
			int nPos = 0;  
			char * pszCheck1 = NULL;
			char * pszCheck2 = NULL;
			int nCount = 0;
			char szNum[16];
			int nLeng = 0;
			unsigned int uMsgLen1 = 0;
			unsigned int uMsgLen2 = 0;
			int i = 0;
			int j = 0;
			int nIdx = 0;
			BOOL bOk = FALSE;
			ChatItem CItem;
			CItem.nItemGenre = -1;
			pszCheck1 = (char*)pMsgBuff;
			while(i < nMsgLength)
			{
				uMsgLen2 = 0;
				if(*pszCheck1 == '[')
				{
					pszCheck2 = (char*)(pMsgBuff + i);
					nCount = 0;
					j = i;
					while(j < nMsgLength)
					{
						uMsgLen2++;
						if(*pszCheck2 == ']' && nCount == 30)
						{
							bOk = TRUE;
							break;
						}

						if(*pszCheck2 == ',')
						{
							nCount++;
						}

						pszCheck2++;
						j++;
					}
				}

				pszCheck1++;
				if(bOk)
				{
					uMsgLen1 = i;
					break;
				}

				i++;
			}

			if(bOk)
			{
				bOk = FALSE;
				if(uMsgLen1 > 200)
				{
					return;
				}

				ZeroMemory(szNum, sizeof(szNum));
				while(1)
				{
					if(*pszCheck1 == ',') break;
					if(nLeng < 5) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.nKind = atoi(szNum);
				ZeroMemory(szNum, sizeof(szNum));
				nLeng = 0;
				pszCheck1++;
				while(1)
				{
					if(*pszCheck1 == ',') break;
					if(nLeng < 5) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.nItemGenre = atoi(szNum);
				ZeroMemory(szNum, sizeof(szNum));
				nLeng = 0;
				pszCheck1++;
				while(1)
				{
					if(*pszCheck1 == ',') break;
					if(nLeng < 5) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.nDetailType = atoi(szNum);
				ZeroMemory(szNum, sizeof(szNum));
				nLeng = 0;
				pszCheck1++;
				while(1)
				{
					if(*pszCheck1 == ',') break;
					if(nLeng < 5) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.nParticularType = atoi(szNum);
				ZeroMemory(szNum, sizeof(szNum));
				nLeng = 0;
				pszCheck1++;
				while(1)
				{
					if(*pszCheck1 == ',') break;
					if(nLeng < 5) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.nSeries = atoi(szNum);
				ZeroMemory(szNum, sizeof(szNum));
				nLeng = 0;
				pszCheck1++;
				while(1)
				{
					if(*pszCheck1 == ',') break;
					if(nLeng < 5) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.nLevel = atoi(szNum);
				ZeroMemory(szNum, sizeof(szNum));
				nLeng = 0;
				pszCheck1++;
				while(1)
				{
					if(*pszCheck1 == ',') break;
					if(nLeng < 5) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.nLuck = atoi(szNum);

				for ( i = 0; i < 6; i++)
				{
					ZeroMemory(szNum, sizeof(szNum));
					nLeng = 0;
					pszCheck1++;
					while(1)
					{
						if(*pszCheck1 == ',') break;
						if(nLeng < 10) szNum[nLeng] = (*pszCheck1);
						pszCheck1++;
						nLeng++;
					}

					CItem.nParamX[i] = atoi(szNum);
				}

				for ( i = 0; i < 12; i++)
				{
					ZeroMemory(szNum, sizeof(szNum));
					nLeng = 0;
					pszCheck1++;
					while(1)
					{
						if(*pszCheck1 == ',') break;
						if(nLeng < 10) szNum[nLeng] = (*pszCheck1);
						pszCheck1++;
						nLeng++;
					}

					CItem.pnMagicParam[i] = atoi(szNum);
				}

				ZeroMemory(szNum, sizeof(szNum));
				nLeng = 0;
				pszCheck1++;
				while(1)
				{
					if(*pszCheck1 == ',') break;
					if(nLeng < 5) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.m_Time.bYear = atoi(szNum);
				ZeroMemory(szNum, sizeof(szNum));
				nLeng = 0;
				pszCheck1++;
				while(1)
				{
					if(*pszCheck1 == ',') break;
					if(nLeng < 5) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.m_Time.bMonth = atoi(szNum);
				ZeroMemory(szNum, sizeof(szNum));
				nLeng = 0;
				pszCheck1++;
				while(1)
				{
					if(*pszCheck1 == ',') break;
					if(nLeng < 5) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.m_Time.bDay = atoi(szNum);
				ZeroMemory(szNum, sizeof(szNum));
				nLeng = 0;
				pszCheck1++;
				while(1)
				{
					if(*pszCheck1 == ',') break;
					if(nLeng < 5) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.m_Time.bHour = atoi(szNum);
				ZeroMemory(szNum, sizeof(szNum));
				nLeng = 0;
				pszCheck1++;
				
				while(1)
				{
					if(*pszCheck1 == ',') break;
					if(nLeng < 8) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.iNgoaiTrang = atoi(szNum);
				ZeroMemory(szNum, sizeof(szNum));
				nLeng = 0;
				pszCheck1++;
				
				while(1)
				{
					if(*pszCheck1 == ','  || *pszCheck1 == ']') break;
					if(nLeng < 5) szNum[nLeng] = (*pszCheck1);
					pszCheck1++;
					nLeng++;
				}

				CItem.nDurability = atoi(szNum);
				nIdx = g_pCoreShell->GetGameData(GDI_ITEM_CHAT, true, (int)&CItem);
				if(nIdx)
				{
					int nOffset = 0;
					char Buffer[560] = {0};
					ZeroMemory(Buffer, sizeof(Buffer));
					if(uMsgLen1 > 0) 
					{
						memcpy(&Buffer[nOffset], pMsgBuff, uMsgLen1);
						nOffset += uMsgLen1;
					}
					
					Buffer[nOffset] = '<';
					nOffset++;

					char szName[64] = {0};
					g_pCoreShell->GetGameData(GDI_ITEM_NAME, (unsigned int)&szName, nIdx);
					int nItemLen = strlen(szName);
					for (i=0; i<nItemLen; i++)
					{
						Buffer[nOffset] = szName[i];
						nOffset++;
					}
					
					Buffer[nOffset] = '>';
					nOffset++;

					pszCheck2++;
					memcpy(&Buffer[nOffset], pszCheck2, nMsgLength - (uMsgLen1 + uMsgLen2));
					nOffset += nMsgLength - (uMsgLen1 + uMsgLen2);
					Buffer[nOffset] = 0;
					if(nOffset)
					{
						g_pCoreShell->ChatSpecialPlayer(&SelectPlayer, (const char*)Buffer, nOffset);
					}
					
					g_pCoreShell->GetGameData(GDI_ITEM_CHAT, false, nIdx);
				}
			}
			else
			{
				g_pCoreShell->ChatSpecialPlayer(&SelectPlayer, pMsgBuff, nMsgLength);
			}
		}
	}
}

void KClientCallback::MSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, bool bSucc)
{
	if (!bSucc)
	{
		char szInfo[256];
		int n = sprintf(szInfo, "Ng­êi ch¬i, %s kh«ng cã trªn m¹ng!", szSendName);
		KUiMsgCentrePad::SystemMessageArrival(szInfo, n);
		return;
	}

	if (IsInBlackName(szSourceName))
		return;

	KUiMsgCentrePad::NewMSNMessageArrival(szSourceName, szSendName, pMsgBuff, nMsgLength);
	
	//if (!KUiPlayerBar::IsSelfName(szSourceName))	//²»ÊÇ×Ô¼ºËµµÄ·Åµ½Í·¶¥
	//{
	//	KUiPlayerItem SelectPlayer;
	//	int nKind = -1;
	//	if (g_pCoreShell->FindSpecial((char*)KUiPlayerBar::SelfName(), &SelectPlayer, nKind) && nKind == kind_player)
	//	{
	//		strncpy(SelectPlayer.Name, szSourceName, 32);	//ÎªÁËÏÔÊ¾±ðÈËµÄÃû×Ö
	//		g_pCoreShell->ChatSpecialPlayer(&SelectPlayer, pMsgBuff, nMsgLength);
	//	}
	//}
}

void KClientCallback::NotifyChannelID(char* ChannelName, DWORD channelid, BYTE cost)
{
	KUiMsgCentrePad::OpenChannel(ChannelName, channelid, cost);
}

void KClientCallback::FriendInvite(char* roleName)
{
	if (roleName && roleName[0] != 0)
	{
		// Í¨Öª½çÃæÓÐÈËÉêÇëÌí¼ÓÁÄÌìºÃÓÑ
		KSystemMessage	sMsg;
		KUiPlayerItem	sPlayer;

		strcpy(sPlayer.Name, roleName);
		sPlayer.nIndex = -1;
		sPlayer.uId = 0;
		sPlayer.nData = 0;

		sprintf(sMsg.szMessage, MSG_CHAT_GET_FRIEND_APPLY, roleName);
		sMsg.eType = SMT_FRIEND;
		sMsg.byConfirmType = SMCT_UI_FRIEND_INVITE;
		sMsg.byPriority = 3;
		sMsg.byParamSize = sizeof(KUiPlayerItem);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&sPlayer);
	}
}

void KClientCallback::AddFriend(char* roleName, BYTE answer)
{
	if (roleName && roleName[0] != 0)
	{
		if (answer == answerAgree)
		{
			KSystemMessage	sMsg;
			KUiPlayerItem	sPlayer;

			memset(&sPlayer, 0, sizeof(KUiPlayerItem));
			strcpy(sPlayer.Name, roleName);
			sPlayer.uId = 0;
			sPlayer.nIndex = -1;

			sprintf(sMsg.szMessage, MSG_CHAT_ADD_FRIEND_SUCCESS, roleName);
			sMsg.eType = SMT_FRIEND;
			sMsg.byConfirmType = SMCT_UI_INTERVIEW;
			sMsg.byPriority = 2;
			sMsg.byParamSize = sizeof(KUiPlayerItem);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&sPlayer);

			int nUnit = KUiChatCentre::FindUnitIndexByRoleNameAtServerUnit(roleName);
			if (nUnit >= 0)
			{
				KUiChatCentre::DeleteFriend(nUnit, roleName, false);
			}

			KUiChatCentre::AddFriendInfo(0, roleName, "");
			KUiChatCentre::FriendStatus(0, roleName, stateOnline);
		}
		else if (answer == answerDisagree)
		{
			// Í¨Öª½çÃæpRefuse->m_szName¾Ü¾øÁËËûµÄ½»ÓÑÉêÇë
			KSystemMessage	sMsg;

			sprintf(sMsg.szMessage, MSG_CHAT_REFUSE_FRIEND, roleName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		else if (answer == answerUnable)
		{
			// Í¨Öª½çÃæÌí¼ÓÄ³ÈËÎªºÃÓÑ²Ù×÷Ê§°Ü
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_CHAT_ADD_FRIEND_FAIL, roleName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
	}
}

void KClientCallback::FriendStatus(char* roleName, BYTE state)
{
	if (roleName && roleName[0] != 0)
	{
		int nUnit = KUiChatCentre::FindUnitIndexByRoleNameAtServerUnit(roleName);
		if (nUnit >= 0 && KUiChatCentre::FriendStatus(nUnit, roleName, state))
		{
			// Í¨Öª½çÃæÓÐºÃÓÑÉÏÏß
			KSystemMessage	sMsg;
			if (state == stateOnline)
				sprintf(sMsg.szMessage, MSG_CHAT_FRIEND_ONLINE, roleName);
			else if (state == stateOffline)
				sprintf(sMsg.szMessage, MSG_CHAT_FRIEND_OFFLINE, roleName);
			else
				return;

			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
	}
}

void KClientCallback::FriendInfo(char* roleName, char* unitName, char* groupName, BYTE state)
{
	if (roleName && roleName[0] != 0 && unitName && groupName)
	{
		int nUnit = 0;
		if (unitName[0])
			nUnit = KUiChatCentre::FindUnitIndex(unitName);
		if (nUnit >= 0)
		{
			int nUnitOld = KUiChatCentre::FindUnitIndexByRoleNameAtServerUnit(roleName);
			if (nUnitOld >= 0 && nUnitOld != nUnit)
			{
				KUiChatCentre::DeleteFriend(nUnitOld, roleName, false);
			}

			KUiChatCentre::AddFriendInfo(nUnit, roleName, groupName);
			KUiChatCentre::FriendStatus(nUnit, roleName, state);
		}
	}
}

void KClientCallback::AddPeople(char* unitName, char* roleName)
{
	if (roleName && roleName[0] != 0 && unitName)
	{
		int nUnit = KUiChatCentre::FindUnitIndex(unitName);
		if (nUnit >= 0)
		{
			int nUnitOld = KUiChatCentre::FindUnitIndexByRoleNameAtServerUnit(roleName);
			if (nUnitOld >= 0 && nUnitOld != nUnit)
			{
				KUiChatCentre::DeleteFriend(nUnitOld, roleName, false);
			}
			
			KUiChatCentre::AddFriendInfo(nUnit, roleName, "");
			KUiChatCentre::FriendStatus(nUnit, roleName, stateOnline);
		}
	}
}

#define LEVEL_TIPS_INI "\\Ui\\ÔÓ»â.ini"
//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºµÈ¼¶µÄÌùÊ¿
//--------------------------------------------------------------------------
void LevelTips(int nNewLevel)
{
	char szMsg[256], szBuf[16];
	KIniFile Ini;
	int nMsgLen;

	if(Ini.Load(LEVEL_TIPS_INI))
	{
		Ini.GetString("LevelUpTips", itoa(nNewLevel, szBuf, 10), "", szMsg, sizeof(szMsg));

		nMsgLen = TEncodeText(szMsg, strlen(szMsg));

		if(nMsgLen)
		{
		    KUiMsgCentrePad::SystemMessageArrival(szMsg, nMsgLen);
		}
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÃÅÅÉµÄÌùÊ¿
//--------------------------------------------------------------------------
void FactionTips(int nbJoin)
{
	char szMsg[256] = "";
	KIniFile Ini;
	int nMsgLen;

	if(Ini.Load(LEVEL_TIPS_INI))
	{
		if (nbJoin)
			Ini.GetString("FactionTips", "Join", "", szMsg, sizeof(szMsg));

		nMsgLen = TEncodeText(szMsg, strlen(szMsg));

		if(nMsgLen)
		{
		    KUiMsgCentrePad::SystemMessageArrival(szMsg, nMsgLen);
		}
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÌùÊ¿ÀÏ´ó
//--------------------------------------------------------------------------
void GameWorldTips(unsigned int uParam, int nParam)
{
	if(uParam == PBP_LEVEL)
		LevelTips(nParam);
	else if(uParam == PBP_FACTION)
		FactionTips(nParam);
}
