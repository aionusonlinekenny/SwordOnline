
#ifndef COREUSENAMEDEF_H
#define COREUSENAMEDEF_H

#define		NPCNAME_KEY						"<npc>"
#define		PLAYERNAME_KEY					"<player>"
#define		SEXNAME_KEY						"<sex>"
#define		MAX_PLAYER_IN_ACCOUNT			3

#define		SETTING_PATH					"\\settings"

//---------------------------- npc res Ïà¹Ø ------------------------------
#define		RES_INI_FILE_PATH				"\\settings\\npcres"
#define		RES_NPC_STYLE_PATH				"\\spr\\npcres\\style"
#define		RES_SOUND_FILE_PATH				"sound"

#define		NPC_RES_KIND_FILE_NAME			"\\settings\\npcres\\ÈËÎïÀàÐÍ.txt"
#define		NPC_NORMAL_RES_FILE				"\\settings\\npcres\\ÆÕÍ¨npc×ÊÔ´.txt"
#define		NPC_NORMAL_SPRINFO_FILE			"\\settings\\npcres\\ÆÕÍ¨npc×ÊÔ´ÐÅÏ¢.txt"
#define		STATE_MAGIC_TABLE_NAME			"\\settings\\npcres\\×´Ì¬Í¼ÐÎ¶ÔÕÕ±í.txt"
#define		PLAYER_RES_SHADOW_FILE			"\\settings\\npcres\\Ö÷½Ç¶¯×÷ÒõÓ°¶ÔÓ¦±í.txt"
#define		NPC_RES_SHADOW_FILE				"ÆÕÍ¨npc¶¯×÷ÒõÓ°¶ÔÓ¦±í.txt"
#define		PLAYER_SOUND_FILE				"\\settings\\npcres\\Ö÷½Ç¶¯×÷ÉùÒô±í.txt"
#define		NPC_SOUND_FILE					"\\settings\\npcres\\npc¶¯×÷ÉùÒô±í.txt"

#define		NPC_ACTION_NAME					"npc¶¯×÷±í.txt"
#define		ACTION_FILE_NAME				"¶¯×÷±àºÅ±í.txt"

#define		PLAYER_MENU_STATE_RES_FILE		"\\settings\\npcres\\½çÃæ×´Ì¬ÓëÍ¼ÐÎ¶ÔÕÕ±í.txt"
#define		PLAYER_INSTANT_SPECIAL_FILE		"\\settings\\npcres\\Ë²¼äÌØÐ§.txt"

#define		SPR_INFO_NAME					"ÐÅÏ¢"
#define		KIND_NAME_SECT					"ÈËÎïÀàÐÍ"
#define		KIND_NAME_SPECIAL				"ÌØÊânpc"
#define		KIND_NAME_NORMAL				"ÆÕÍ¨npc"
#define		KIND_FILE_SECT1					"²¿¼þËµÃ÷ÎÄ¼þÃû"
#define		KIND_FILE_SECT2					"ÎäÆ÷ÐÐÎª¹ØÁª±í1"
#define		KIND_FILE_SECT3					"ÎäÆ÷ÐÐÎª¹ØÁª±í2"
#define		KIND_FILE_SECT4					"¶¯×÷ÌùÍ¼Ë³Ðò±í"
#define		KIND_FILE_SECT5					"×ÊÔ´ÎÄ¼þÂ·¾­"

//--------------------------- player ÃÅÅÉÏà¹Ø ------------------------------
// ×¢£ºÏÂÃæÕâ¸öÉè¶¨ÎÄ¼þ±ØÐë°´ ½ð ½ð Ä¾ Ä¾ Ë® Ë® »ð »ð ÍÁ ÍÁ µÄË³ÐòÅÅÁÐ
#define		FACTION_FILE					"\\settings\\faction\\Faction.ini"

//--------------------------- player PKÏà¹Ø ------------------------------
#define		defPK_PUNISH_FILE				"\\settings\\npc\\player\\PKPunish.txt"

//--------------------------- player °ï»áÏà¹Ø ------------------------------
#define		defPLAYER_TONG_PARAM_FILE		"\\settings\\tong\\TongSet.ini"


//----------------------------- ÁÄÌìÏà¹Ø ------------------------------
#define		CHAT_PATH						"\\chat"
#define		CHAT_TEAM_INFO_FILE_NAME		"Team.cht"
#define		CHAT_CHANNEL_INFO_FILE_NAME		"Channel.cht"

#define		CHAT_CHANNEL_NAME_ALL			"ThÕ giíi "
#define		CHAT_CHANNEL_NAME_SCREEN		"GÇn ®ã "
#define		CHAT_CHANNEL_NAME_SINGLE		"MËt "
#define		CHAT_CHANNEL_NAME_TEAM			"Tæ ®äi"
#define		CHAT_CHANNEL_NAME_FACTION		"M«n ph¸i "
#define		CHAT_CHANNEL_NAME_TONG			"Bang héi "
#define		CHAT_CHANNEL_NAME_SCREENSINGLE	""
#define		CHAT_CHANNEL_NAME_SYSTEM		"HÖ thèng"

#define		CHAT_TAKE_CHANNEL_NAME_TEAM			"Tæ ®éi "
#define		CHAT_TAKE_CHANNEL_NAME_FACTION		"M«n ph¸i "
#define		CHAT_TAKE_CHANNEL_NAME_TONG			"Bang héi "
#define		CHAT_TAKE_CHANNEL_NAME_SCREENSINGLE	"MËt "

//---------------------------- player ÊýÖµÏà¹Ø ------------------------------
#define		PLAYER_LEVEL_EXP_FILE			"\\settings\\npc\\player\\level_exp.txt"
#define		PLAYER_LEVEL_ADD_FILE			"\\settings\\npc\\player\\level_add.txt"
#define		PLAYER_LEVEL_LEAD_EXP_FILE		"\\settings\\npc\\player\\level_lead_exp.txt"
#define		BASE_ATTRIBUTE_FILE_NAME		"\\settings\\npc\\player\\NewPlayerBaseAttribute.ini"
#define		PLAYER_PK_RATE_FILE				"\\settings\\npc\\PKRate.ini"
#define		PLAYER_BASE_VALUE				"\\settings\\npc\\player\\BaseValue.ini"

// 00ÄÐ½ð 01Å®½ð 02ÄÐÄ¾ 03Å®Ä¾ 04ÄÐË® 05Å®Ë® 06ÄÐ»ð 07Å®»ð 08ÄÐÍÁ 09Å®ÍÁ
#define		NEW_PLAYER_INI_FILE_NAME		"\\settings\\npc\\player\\NewPlayerIni%02d.ini"

//---------------------------- player ½»Ò×Ïà¹Ø ------------------------------
#define		BUYSELL_FILE					"\\settings\\buysell.txt"
#define		GOODS_FILE						"\\settings\\goods.txt"

//----------------------------- ÎïÆ·Ïà¹Ø ------------------------------
#define		CHANGERES_MELEE_FILE			"\\settings\\item\\MeleeRes.txt"
#define		CHANGERES_RANGE_FILE			"\\settings\\item\\RangeRes.txt"
#define		CHANGERES_ARMOR_FILE			"\\settings\\item\\ArmorRes.txt"
#define		CHANGERES_HELM_FILE				"\\settings\\item\\HelmRes.txt"
#define		CHANGERES_HORSE_FILE			"\\settings\\item\\HorseRes.txt"
#define		ITEM_ABRADE_FILE				"\\settings\\item\\AbradeRate.ini"

//--------------- npc skill missles Éè¶¨ÎÄ¼þ£¬ÓÃÓÚÉú³ÉÄ£°å -------------
#define		SKILL_SETTING_FILE				"\\settings\\Skills.txt"
#define		MISSLES_SETTING_FILE			"\\settings\\Missles.txt"
#define		NPC_SETTING_FILE				"\\settings\\NpcS.txt"
#define		NPC_NAME_FILE					"\\settings\\NpcName.txt"
#define		NPC_GOLD_TEMPLATE_FILE			"\\settings\\npc\\NpcGoldTemplate.txt"

//---------------------------- object Ïà¹Ø ------------------------------
#define		OBJ_DATA_FILE_NAME		"\\settings\\obj\\ObjData.txt"
#define		MONEY_OBJ_FILE_NAME		"\\settings\\obj\\MoneyObj.txt"
#define		OBJ_NAME_COLOR_FILE		"\\settings\\obj\\ObjNameColor.ini"

//---------------------------- Ð¡µØÍ¼Ïà¹Ø -------------------------------
#define		defLITTLE_MAP_SET_FILE	"\\Ui\\Default\\Ð¡µØÍ¼ÑÕÉ«.ini"

//----------------------------- ÉùÒôÏà¹Ø --------------------------------
#define		defINSTANT_SOUND_FILE	"\\settings\\SoundList.txt"
#define		defMUSIC_SET_FILE		"\\settings\\music\\MusicSet.txt"
#define		defMUSIC_FIGHT_SET_FILE	"\\settings\\music\\MusicFightSet.ini"

//------------------------------------------------------------------------
#define NPC_LEVELSCRIPT_FILENAME		"\\script\\npclevelscript\\npclevelscript.lua"
#define NPC_TEMPLATE_BINFILEPATH		"\\settings"
#define NPC_TEMPLATE_BINFILE			"NpcTemplate.Bin"
#define WEAPON_PHYSICSSKILLFILE			"\\settings\\PhysicsSkillId.txt"				
#define WEAPON_PARTICULARTYPE			"ÏêÏ¸Àà±ð"
#define	WEAPON_DETAILTYPE				"¾ßÌåÀà±ð"
#define	WEAPON_SKILLID					"¶ÔÓ¦ÎïÀí¼¼ÄÜ±àºÅ"
//-----------------------------------------------------------------------
#define	WORLD_WAYPOINT_TABFILE			"\\settings\\WayPoint.txt"
#define WORLD_STATION_TABFILE			"\\settings\\Station.txt"
#define WORLD_STATIONPRICE_TABFILE		"\\settings\\StationPrice.txt"
#define WORLD_WAYPOINTPRICE_TABFILE		"\\settings\\WayPointPrice.txt"
#define WORLD_DOCK_TABFILE				"\\settings\\Wharf.txt"
#define WORLD_DOCKPRICE_TABFILE			"\\settings\\WharfPrice.txt"
#define STRINGRESOURSE_TABFILE			"\\settings\\StringResource.txt"
#define PLAYER_RANK_SETTING_TABFILE		"\\settings\\RankSetting.txt"	
#define QUESTITEM_TABFILE				"\\settings\\item\\questkey.txt"

//---------------------------------ÈÎÎñ--------------------------
#define TASK_MISSION_SETTING_TABFILE	"\\settings\\task\\missions.txt"



//---------------------------- ÏûÏ¢Ïà¹Ø ------------------------------
// ÌØ±ð×¢Òâ£ºÒÔÏÂ×Ö·û´®³¤¶È²»ÄÜ³¬¹ý32×Ö½Ú£¬°üÀ¨ %d %s µÈ½ÓÊÕ¾ßÌåÄÚÈÝÒÔºóµÄ³¤¶È
#define		MSG_GET_EXP						"B¹n nhËn ®­îc <color=green>%d <color>®iÓm kinh nghiÖm"
#define		MSG_GET_EXP_CS					"B¹n nhËn ®­îc <color=green>%d00 <color>®iÓm kinh nghiÖm"
#define		MSG_DEC_EXP						"B¹n tæn thÊt <color=green>%d <color>®iÓm kinh nghiÖm"
#define		MSG_LEVEL_UP					"Th¨ng cÊp nh©n vËt, ®¼ng cÊp hiÖn t¹i %d !"
#define		MSG_LEADER_LEVEL_UP				"Th¨ng cÊp l·nh ®¹o, ®¼ng cÊp hiÖn t¹i %d !"
#define		MSG_GET_ATTRIBUTE_POINT			"NhËn ®­îc %d ®iÓm tiÒm n¨ng !"
#define		MSG_GET_SKILL_POINT				"NhËn ®­îc %d ®iÓm kü n¨ng !"
#define		MSG_GET_ATTRIBUTE_SKILL_POINT	"NhËn ®­îc %d ®iÓm tiÒm n¨ng vµ %d ®iÓm kü n¨ng !"

#define		MSG_TEAM_IS_FULL				"Nhãm mµ b¹n xin phÐp gia nhËp ®· ®ñ sè l­îng"
#define		MSG_TEAM_OPEN					"Nhãm cña b¹n hiÖn t¹i cã thÓ tiÕp nhËn thµnh viªn míi"
#define		MSG_TEAM_CLOSE					"Nhãm cña b¹n hiÖn t¹i kh«ng thÓ tiÕp nhËn thµnh viªn míi"
#define		MSG_TEAM_MODEPICK_SELF			"Tù nhÆt"
#define		MSG_TEAM_MODEPICK_FREE			"Tù do nhÆt"
#define		MSG_TEAM_MODEPICK_CAPTAIN		"§éi tr­ëng nhÆt"
#define		MSG_TEAM_MODEPICK_ALTERNATE		"Lu©n phiªn nhÆt"
#define		MSG_TEAM_PKFOLLOW_OPEN			"B¹n më chÕ ®é PK theo nhãm"
#define		MSG_TEAM_PKFOLLOW_CLOSE			"B¹n ®ãng chÕ ®é PK theo nhãm"
#define		MSG_TEAM_AUTO_REFUSE_INVITE		"Tõ chèi tù ®éng mêi nhãm "
#define		MSG_TEAM_NOT_AUTO_REFUSE_INVITE	"Kh«ng tõ chèi mêi nhãm"
#define		MSG_TEAM_SEND_INVITE			"Göi lêi mêi tíi %s vµo nhãm !"
#define		MSG_TEAM_GET_INVITE				"%s mêi vµo nhãm !"
#define		MSG_TEAM_REFUSE_INVITE			"%s tõ chèi lêi mêi nhãm !"
#define		MSG_TEAM_CREATE					"T¹o tæ ®éi míi !"
#define		MSG_TEAM_CREATE_FAIL			"T¹o tæ ®éi míi thÊt b¹i !"
#define		MSG_TEAM_CANNOT_CREATE			"Kh«ng thÓ lËp tæ ®éi !"
#define		MSG_TEAM_TARGET_CANNOT_ADD_TEAM	"Kh«ng thÓ thªm thµnh viªn !"
#define		MSG_TEAM_ADD_MEMBER				"%s gia nhËp tæ ®éi !"
#define		MSG_TEAM_SELF_ADD				"Tham gia nhãm %s !"
#define		MSG_TEAM_DISMISS_CAPTAIN		"Gi¶ t¸n nhãm !"
#define		MSG_TEAM_DISMISS_MEMBER			"%s tho¸t ra khái tæ ®éi !"
#define		MSG_TEAM_KICK_ONE				"%s bÞ trôc xuÊt ra khái tæ ®éi !"
#define		MSG_TEAM_BE_KICKEN				"BÞ trôc xuÊt ra khái tæ ®éi !"
#define		MSG_TEAM_APPLY_ADD				"Xin ra nhËp tæ ®éi %s !"
#define		MSG_TEAM_APPLY_ADD_SELF_MSG		"Ng­êi ch¬i %s muèn gia nhËp nhãm !"
#define		MSG_TEAM_LEAVE					"%s rêi ®éi !"
#define		MSG_TEAM_LEAVE_SELF_MSG			"Rêi khái tæ ®éi %s !"
#define		MSG_TEAM_CHANGE_CAPTAIN_FAIL1	"Bæ nhiÖm ®éi tr­ëng kh«ng thµnh c«ng !"
#define		MSG_TEAM_CHANGE_CAPTAIN_FAIL2	"%s kh«ng ®ñ tµi l·nh ®¹o !"
#define		MSG_TEAM_CHANGE_CAPTAIN_FAIL3	"Tæ ®éi cã tr­ëng nhãm míi !"
#define		MSG_TEAM_CHANGE_CAPTAIN			"%s chë thµnh tr­ëng nhãm !"
#define		MSG_TEAM_CHANGE_CAPTAIN_SELF	"%s ®­îc bæ nhiÑm thµnh ®éi tr­ëng !"
#define		MSG_CHAT_APPLY_ADD_FRIEND		"Thªm %s vµo danh s¸ch b¹n bÌ !"
#define		MSG_CHAT_FRIEND_HAD_IN			"%s ®· ®­îc thªm vµo nh­ lµ b¹n bÌ !"
#define		MSG_CHAT_GET_FRIEND_APPLY		"%s ®ång ý thµnh b¹n bÌ !"
#define		MSG_CHAT_REFUSE_FRIEND			"%s tõ chèi trë thµnh b¹n bÌ !"
#define		MSG_CHAT_ADD_FRIEND_FAIL		"Thªm %s lµm h¶o h÷u thÊt b¹i"
#define		MSG_CHAT_ADD_FRIEND_SUCCESS		"B¹n vµ %s trë thµnh h¶o h÷u!"
#define		MSG_CHAT_CREATE_TEAM_FAIL1		"Thµnh lËp chia tæ h¶o h÷u thÊt b¹i."
#define		MSG_CHAT_CREATE_TEAM_FAIL2		"§· tån t¹i tæ ®éi cïng tªn."
#define		MSG_CHAT_CREATE_TEAM_FAIL3		"§· ®¹t ®Õn sè tèi ®a cña tæ ®éi."
#define		MSG_CHAT_RENAME_TEAM_FAIL		"Thay ®æi tªn tæ ®éi thÊt b¹i."
#define		MSG_CHAT_DELETE_TEAM_FAIL1		"Xãa tæ ®éi thÊt b¹i."
#define		MSG_CHAT_DELETE_TEAM_FAIL2		"Tæ ®éi nµy kh«ng ®­îc phÐp xãa."
#define		MSG_CHAT_FRIEND_ONLINE			"%s trªn m¹ng."
#define		MSG_CHAT_FRIEND_OFFLINE			"%s rêi m¹ng"
#define		MSG_CHAT_DELETE_FRIEND			"B¹n vµ %s hñy bá quan hÖ h¶o h÷u!"
#define		MSG_CHAT_DELETED_FRIEND			"%s vµ b¹n hñy bá quan hÖ h¶o h÷u!"
#define		MSG_CHAT_MSG_FROM_FRIEND		"%s göi mét tin tøc!"
#define		MSG_CHAT_FRIEND_NOT_NEAR		"%s kh«ng ë gÇn ®©y!"
#define		MSG_CHAT_FRIEND_NOT_ONLINE		"%s kh«ng cã trªn m¹ng"
#define		MSG_CHAT_TAR_REFUSE_SINGLE_TALK	"%s ch­a ®¨ng ký vµo kªnh trß chuyÖn riªng!"

#define		MSG_SHOP_NO_ROOM				"Kh«ng ®ñ kho¶ng trèng !"
#define		MSG_SHOP_NO_MONEY				"Kh«ng ®ñ ng©n l­îng !"

#define		MSG_NPC_NO_MANA					"Kh«ng ®ñ néi lùc !"
#define		MSG_NPC_NO_STAMINA				"Kh«ng ®ñ thÓ lùc !"
#define		MSG_NPC_NO_LIFE					"Kh«ng ®ñ sinh lùc !"
#define		MSG_NPC_DEATH					"%s ®· chÕt !"

#define		MSG_OBJ_CANNOT_PICKUP			"Kh«ng thÓ nhÆt trang bÞ cña ng­êi kh¸c !"
#define		MSG_MONEY_CANNOT_PICKUP			"Kh«ng thÓ nhÆt tiÒn cña ng­êi kh¸c !"
#define		MSG_OBJ_TOO_FAR					"VËt phÈm qu¸ xa kh«ng thÓ lÊy"
#define		MSG_DEC_MONEY					"B¹n bÞ mÊt <color=yellow>%d <color>l­îng b¹c"
#define		MSG_EARN_MONEY					"B¹n nhËn ®­îc <color=yellow>%d <color>l­îng b¹c"
#define		MSG_DEATH_LOSE_ITEM				"B¹n ®· bÞ mÊt <color=blue>%s<color>"
#define		MSG_ADD_ITEM					"B¹n nhËn ®­îc mét c¸i <color=blue>%s"
#define		MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE	"Thanh phÝm t¾t cã cïng mét lo¹i vËt phÈm!"
#define		MSG_CAN_NOT_VIEW_ITEM			"HiÖn t¹i kh«ng thÓ coi trang bÞ ng­êi kh¸c!"
#define		MSG_ITEM_DAMAGED				"Trang bÞ cña b¹n %s mÆc qu¸ l©u, ®· bÞ h­ h¹i hoµn toµn!"
#define		MSG_ITEM_NEARLY_DAMAGED			"Trang bÞ cña b¹n %s ®· gÇn h­ háng, cÇn söa ch÷a kÞp thêi!"

#define		MSG_TRADE_STATE_OPEN			"BËt tr¹ng th¸i giao dÞch !"
#define		MSG_TRADE_STATE_CLOSE			"§ãng tr¹ng th¸i giao dÞch !"
#define		MSG_TRADE_SELF_LOCK				"Khã giao dÞch !"
#define		MSG_TRADE_SELF_UNLOCK			"Më kho¸ giao dÞch !"
#define		MSG_TRADE_DEST_LOCK				"%s ®¸ kho¸ !"
#define		MSG_TRADE_DEST_UNLOCK			"%s më kho¸ !"
#define		MSG_TRADE_SUCCESS				"Giao dÞch víi %s thµnh c«ng !"
#define		MSG_TRADE_FAIL					"Giao dÞch víi %s thÊt b¹i !"
#define		MSG_TRADE_SELF_ROOM_FULL		"Kh«ng cã ®ñ kho¶ng trèng !"
#define		MSG_TRADE_DEST_ROOM_FULL		"%s kh«ng cã ®ñ kho¶ng trèng !"
#define		MSG_TRADE_SEND_APPLY			"Mêi %s giao dÞch !"
#define		MSG_TRADE_GET_APPLY				"%s ®ßng ý giao dÞch !"
#define		MSG_TRADE_REFUSE_APPLY			"%s tõ chèi giao dÞch !"
#define		MSG_TRADE_TASK_ITEM				"Kh«ng thÓ giao dÞch vËt phÈm nhiÖm vô !"

#define		MSG_PK_NORMAL_FLAG_OPEN			"Tr¹ng th¸i PK chiÕn ®Êu !"
#define		MSG_PK_NORMAL_FLAG_CLOSE		"Tr¹ng th¸i PK luyÖn c«ng !"
#define		MSG_PK_NORMAL_FLAG_OPEN_NEXT	"Tr¹ng th¸i PK ®å s¸t !"
#define		MSG_PK_NORMAL_FLAG_UNKNOWN		"Tr¹ng th¸i PK ë d¹ng thøc ch­a râ"
#define		MSG_PK_HIGHT_LOCK_NOT_SWITCH	"HiÖn giê PK cña b¹n >= %d, tr­íc khi PK xin h·y hñy bá tr¹ng th¸i khãa b¶o hiÓm."
#define		MSG_PKSTATE_NOT_OPEN			"Tr¹ng th¸i PK nµy ch­a më"
#define		MSG_PK_VALUE					"TrÞ sè PK hiÖn giê lµ %d !"
#define		MSG_PK_ERROR_1					"HiÖn t¹i b¹n ®ang ë tr¹ng th¸i phi chiÕn ®Êu!"
#define		MSG_PK_ERROR_2					"T©n thñ kh«ng thÓ cõu s¸t!"
#define		MSG_PK_ERROR_3					"B¹n ®ang cïng ng­êi kh¸c bµn luËn!"
#define		MSG_PK_ERROR_4					"B¹n ®ang cïng ng­êi kh¸c cõu s¸t!"
#define		MSG_PK_ERROR_5					"B¹n kh«ng thÓ cõu s¸t t©n thñ!!"
#define		MSG_PK_ERROR_6					"§èi ph­¬ng ®ang ë tr¹ng th¸i phi chiÕn ®Êu!"
#define		MSG_PK_ERROR_7					"§èi ph­¬ng ®ang cïng ng­êi kh¸c bµn luËn!"
#define		MSG_PK_ERROR_8					"B¹n hiÖn giê kh«ng thÓ cõu s¸t!"
#define		MSG_PK_ERROR_9					"Ng­êi nµy ®ang trong tr¹ng th¸i ®ãng cõu s¸t"
#define		MGS_PK_IS_LOCK					"PK tõ phi chiÕn ®Êu chuyÓn sang luyÖn c«ng ph¶i mÊt %d phót"
#define		MGS_PK_IS_LOCK1					"Tr¹ng th¸i ®Æc biÖt kh«ng thÓ chuyÓn kiÓu chiÕn ®Êu"
#define		MGS_PK_IS_LOCK2					"Thµnh viªn bang héi míi cã thÓ më chiÕn ®Êu bang héi"
#define		MGS_PK_IS_LOCK3					"ChÕ ®éi PK nhãm kh«ng thÓ tù ý chuyÓn tr¹ng th¸i chiÕn ®Êu"
#define		MGS_PK_IS_LOCK4					"Trong tæ ®éi kh«ng thÓ chuyÓn PK m« h×nh bang chiÕn"
#define		MGS_PK_IS_LOCK5					"§iÓm PK cña b¹n qu¸ nhiÒu, kh«ng thÓ h¹ thñ ®­îc!"
#define		MGS_PK_IS_LOCK6					"§iÓm kinh nghiÖm ©m cña b¹n qu¸ nhiÒu, kh«ng thÓ h¹ thñ ®­îc!"
#define		MGS_PK_IS_LOCK7					"PK chuyÓn gi÷a c¸c tr¹ng th¸i chiÕn ®Êu ph¶i mÊt %d gi©y"

#define		MSG_PK_ENMITY_SUCCESS_1			"%s cïng víi B¹n tiÕn hµnh cõu s¸t"
#define		MSG_PK_ENMITY_SUCCESS_2			"Sau 10 gi©y b¾t ®Çu tiÕn hµnh cõu s¸t"
#define		MSG_PK_ENMITY_CLOSE				"Tr¹ng th¸i cõu s¸t kÕt thóc"
#define		MSG_PK_ENMITY_OPEN				"B¾t ®Çu tiÕn hµnh cõu s¸t"

#define		MSG_TONG_CREATE_ERROR01			"Tªn bang héi qu¸ dµi!"
#define		MSG_TONG_CREATE_ERROR02			"Phe ph¸i bang héi sai!"
#define		MSG_TONG_CREATE_ERROR03			"Thµnh viªn cßn trong bang héi kh«ng thÓ t¹o bang!"
#define		MSG_TONG_CREATE_ERROR04			"Tªn ch÷ ®á míi cã thÓ thµnh lËp bang héi!"
#define		MSG_TONG_CREATE_ERROR05			"§¼ng cÊp thµnh lËp bang héi ch­a ®ñ!"
#define		MSG_TONG_CREATE_ERROR06			"§¼ng cÊp thèng so¸i thµnh lËp bang héi ch­a ®ñ!"
#define		MSG_TONG_CREATE_ERROR07			"Thµnh lËp bang héi cÇn %d tiÒn"
#define		MSG_TONG_CREATE_ERROR08			"Cßn trong tæ ®éi kh«ng thÓ thµnh lËp bang héi!"
#define		MSG_TONG_CREATE_ERROR09			"Tªn bang héi sai!"
#define		MSG_TONG_CREATE_ERROR10			"Kh«ng ®ñ tiÒn ®Ó thiÕt lËp bang héi!"
#define		MSG_TONG_CREATE_ERROR11			"Tªn bang héi kh«ng hîp lÖ hoÆc trïng víi tªn bang héi kh¸c!"
#define		MSG_TONG_APPLY_CREATE			"B¹n xin thµnh lËp bang héi!"
#define		MSG_TONG_CREATE_SUCCESS			"Thµnh lËp bang héi thµnh c«ng!"
#define		MSG_TONG_APPLY_ADD				"B¹n xin gia nhËp bang héi!"
#define		MSG_TONG_APPLY_ADD_ERROR1		"Thµnh viªn bang héi nµy kh«ng thÓ gia nhËp bang héi kh¸c!"
#define		MSG_TONG_APPLY_ADD_ERROR2		"Tªn ch÷ ®á míi cã thÓ gia nhËp bang héi!"
#define		MSG_TONG_APPLY_ADD_ERROR3		"Cßn trong tæ ®éi kh«ng thÓ gia nhËp bang héi!"
#define		MSG_TONG_REFUSE_ADD				"%s tõ chèi b¹n gia nhËp bang héi!"
#define		MSG_TONG_ADD_SUCCESS			"B¹n gia nhËp bang héi!"
#define		MSG_TONG_CANNOT_LEAVE1			"Bang chñ kh«ng thÓ rêi khái bang héi!"
#define		MSG_TONG_CANNOT_LEAVE2			"Tr­ëng l·o kh«ng thÓ rêi khái bang héi!"
#define		MSG_TONG_BE_KICKED				"B¹n bÞ ®uæi khái bang héi!"
#define		MSG_TONG_LEAVE_SUCCESS			"B¹n chñ ®éng rêi khái bang héi thµnh c«ng!"
#define		MSG_TONG_LEAVE_FAIL				"B¹n chñ ®éng rêi khái bang héi thÊt b¹i!"
#define		MSG_TONG_CHANGE_AS_MASTER		"B¹n ®­îc bæ nhiÖm lµm bang chñ!!"
#define		MSG_TONG_CHANGE_AS_MEMBER		"HiÖn t¹i b¹n trë thµnh bang chóng b×nh th­êng!"

#define		MSG_NEW_BLOCK		"B¹n ch­a më khãa r­¬ng, kh«ng thÓ chuyÓn tr¹ng th¸i!"
#define		MSG_NEW_CHANGESTATEPK10		"PK 10 kh«ng thÓ chuyÓn tr¹ng th¸i khi ë ngoµi thµnh"

#define		MSG_FUNCTION_FOBIDDENED			"Xin lçi, c«ng n¨ng nµy t¹m thêi ®· bÞ ®ãng!"
#define		MESSAGE_SYSTEM_ANNOUCE_HEAD		"C«ng bè "
#define		MESSAGE_TEAM_ANNOUCE_HEAD		"Tin ®éi"
#define		NORMAL_FUNCTION_NAME		"main"

enum enumMSG_ID
{
	enumMSG_ID_NONE = 0,
	enumMSG_ID_TEAM_KICK_One,
	enumMSG_ID_TEAM_DISMISS,
	enumMSG_ID_TEAM_LEAVE,
	enumMSG_ID_TEAM_REFUSE_INVITE,
	enumMSG_ID_TEAM_APPLY_INVITE_FAIL,
	enumMSG_ID_TEAM_APPLY_INVITE_FAIL2,
	enumMSG_ID_TEAM_SELF_ADD,
	enumMSG_ID_TEAM_ADDMEMBER_NOTVALID,
	enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL,
	enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL2,
	enumMSG_ID_OBJ_CANNOT_PICKUP,
	enumMSG_ID_OBJ_TOO_FAR,
	enumMSG_ID_DEC_MONEY,
	enumMSG_ID_TRADE_SELF_ROOM_FULL,
	enumMSG_ID_TRADE_DEST_ROOM_FULL,
	enumMSG_ID_TRADE_REFUSE_APPLY,
	enumMSG_ID_TRADE_TASK_ITEM,
	enumMSG_ID_GET_ITEM,
	enumMSG_ID_ITEM_DAMAGED,
	enumMSG_ID_MONEY_CANNOT_PICKUP,
	enumMSG_ID_CANNOT_ADD_TEAM,
	enumMSG_ID_TEAM_IS_FULL,
	enumMSG_ID_CANNOT_INVITE_TEAM,
	enumMSG_ID_TARGET_CANNOT_ADD_TEAM,
	enumMSG_ID_GET_INVITE_TEAM_REPLY,
	enumMSG_ID_PK_NORMAL_NOT_OPEN,
	enumMSG_ID_PK_HIGHT_LOCK_NOT_SWITCH,
	enumMSG_ID_PKSTATE_NOT_OPEN,
	enumMSG_ID_PK_ERROR_1,
	enumMSG_ID_PK_ERROR_2,
	enumMSG_ID_PK_ERROR_3,
	enumMSG_ID_PK_ERROR_4,
	enumMSG_ID_PK_ERROR_5,
	enumMSG_ID_PK_ERROR_6,
	enumMSG_ID_PK_ERROR_7,
	enumMSG_ID_PK_ERROR_8,
	enumMSG_ID_PK_ERROR_9,
	enumMSG_ID_PK_IS_LOCK,
	enumMSG_ID_PK_IS_LOCK1,
	enumMSG_ID_PK_IS_LOCK2,
	enumMSG_ID_PK_IS_LOCK3,
	enumMSG_ID_PK_IS_LOCK4,
	enumMSG_ID_PK_IS_LOCK5,
	enumMSG_ID_PK_IS_LOCK6,
	enumMSG_ID_PK_IS_LOCK7,
	enumMSG_ID_DEATH_LOSE_ITEM,
	enumMSG_ID_TONG_REFUSE_ADD,
	enumMSG_ID_TONG_BE_KICK,
	enumMSG_ID_TONG_LEAVE_SUCCESS,
	enumMSG_ID_TONG_LEAVE_FAIL,
	enumMSG_ID_TONG_CHANGE_AS_MASTER,
	enumMSG_ID_TONG_CHANGE_AS_MEMBER,

	enumMSG_ID_FIGHT_MODE_ERROR1,
	enumMSG_ID_FIGHT_MODE_ERROR2,
	enumMSG_ID_FIGHT_MODE_ERROR3,

    enumMSG_ID_ITEM_CANTREPAIR,

    enumMSG_ID_FUNCTION_FOBIDDENED,

    enumMSG_ID_ITEM_TOWNPORTAL,


	enumMSG_ID_NUM,
	enumMSG_ID_BLOCK_PK,
	enumMSG_ID_CHANGE_STATE_PK10,
};

//---------------------------- Êó±êÖ¸ÕëÏà¹Ø ------------------------------
#define		MOUSE_CURSOR_NORMAL				0
#define		MOUSE_CURSOR_FIGHT				1
#define		MOUSE_CURSOR_DIALOG				2
#define		MOUSE_CURSOR_PICK				3
#define		MOUSE_CURSOR_USE				8
#define		MOUSE_CURSOR_SHOP_ITEM          9
#define		MOUSE_CURSOR_TRADE				11
#endif
