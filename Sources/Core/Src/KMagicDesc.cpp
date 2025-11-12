#include "KCore.h"

#include "KEngine.h"
#include "KMagicAttrib.h"
#include "KMagicDesc.h"
#include "GameDataDef.h"
#include "KFaction.h"

#define		MAGICDESC_FILE		"\\settings\\MagicDesc.Ini"
extern const char * g_MagicID2String(int nAttrib);
const char MAGIC_ATTRIB_STRING[][100] = 
{
//¸Ä±ä¼¼ÄÜµÄÊôÐÔ
	"skill_begin",
	"skill_cost_v",							// ÏûºÄMANA
	"skill_costtype_v",		//	
	"skill_mintimepercast_v",		// Ã¿´Î·¢Ä§·¨µÄ¼ä¸ôÊ±¼ä
	"skill_misslenum_v",		// 
	"skill_misslesform_v",		
	"skill_param1_v",		
	"skill_param2_v",
	"skill_reserve1",
	"skill_reserve2",
	"skill_reserve3",
	"skill_reserve4",
	"skill_eventskilllevel",
	"skill_end",

//¸Ä±ä·¢³ö×Óµ¯µÄÊôÐÔ
	"missle_begin",
	"missle_movekind_v",
	"missle_speed_v",
	"missle_lifetime_v",
	"missle_height_v",
	"missle_damagerange_v",
	"missle_radius_v",
	"missle_reserve1",
	"missle_reserve2",
	"missle_reserve3",
	"missle_reserve4",
	"missle_reserve5",
	"missle_end",

//×°±¸ÊôÐÔ	
	"item_begin",
	"weapondamagemin_v",
	"weapondamagemax_v",
	"armordefense_v",
	"durability_v",
	"requirestr",
	"requiredex",
	"requirevit",
	"requireeng",
	"requirelevel",
	"requireseries",
	"requiresex",
	"requiremenpai",
	"weapondamageenhance_p",
	"armordefenseenhance_p",
	"requirementreduce_p",
	"indestructible_b",
	"item_reserve1",
	"item_reserve2",
	"item_reserve3",
	"item_reserve4",
	"item_reserve5",
	"item_reserve6",
	"item_reserve7",
	"item_reserve8",
	"item_reserve9",
	"item_reserve10",
	"item_end",

	"damage_begin",		
	"attackrating_v",
	"attackrating_p",
	"ignoredefense_p",  // 59
	"physicsdamage_v",
	"colddamage_v",
	"firedamage_v",
	"lightingdamage_v",
	"poisondamage_v",
	"magicdamage_v",
	"physicsenhance_p",
	"steallife_p",
	"stealmana_p",
	"stealstamina_p",
	"knockback_p", // 70
	"deadlystrike_p",
	"fatallystrike_p",
	"stun_p",
	"damage_reserve1",
	"damage_reserve2",
	"damage_reserve3",
	"damage_reserve4",
	"damage_reserve5",
	"damage_reserve6",
	"damage_reserve7",
	"damage_reserve8",
	"damage_reserve9",
	"damage_reserve10",
	"damage_end",

	"normal_begin",  //85
	"lifemax_v",
	"lifemax_p",
	"life_v",
	"lifereplenish_v", 
	"manamax_v",  //90
	"manamax_p",
	"mana_v",
	"manareplenish_v",
	"staminamax_v",
	"staminamax_p",
	"stamina_v",
	"staminareplenish_v",
	"strength_v",
	"dexterity_v",
	"vitality_v",  //100
	"energy_v",
	"poisonres_p",
	"fireres_p",
	"lightingres_p",
	"physicsres_p",
	"coldres_p",
	"freezetimereduce_p",
	"burntimereduce_p",
	"poisontimereduce_p",
	"poisondamagereduce_v",
	"stuntimereduce_p",
	"fastwalkrun_p",
	"visionradius_p",
	"fasthitrecover_v",
	"allres_p",
	"attackspeed_v",
	"castspeed_v",
	"meleedamagereturn_v",
	"meleedamagereturn_p",
	"rangedamagereturn_v",  //120
	"rangedamagereturn_p",
	"addphysicsdamage_v",
	"addfiredamage_v",
	"addcolddamage_v",
	"addlightingdamage_v",
	"addpoisondamage_v",
	"addphysicsdamage_p",
	"slowmissle_b",
	"changecamp_b",
	"physicsarmor_v",
	"coldarmor_v",
	"firearmor_v",
	"poisonarmor_v",
	"lightingarmor_v",
	"damagetomana_p",
	"lucky_v",
	"steallifeenhance_p",
	"stealmanaenhance_p",
	"stealstaminaenhance_p",
	"allskill_v",
	"metalskill_v",
	"woodskill_v",  //142
	"waterskill_v",
	"fireskill_v",
	"earthskill_v",
	"knockbackenhance_p",
	"deadlystrikeenhance_p",
	"stunenhance_p",
	"badstatustimereduce_v",
	"manashield_p",
	"adddefense_v",
	"adddefense_p",
	"fatallystrikeenhance_p",
	"lifepotion_v",
	"manapotion_v",
	"physicsresmax_p",
	"coldresmax_p",
	"fireresmax_p",
	"lightingresmax_p",
	"poisonresmax_p",
	"allresmax_p",
	"coldenhance_p",
	"fireenhance_p",
	"lightingenhance_p",
	"poisonenhance_p",
	"magicenhance_p",
	"attackratingenhance_v",
	"attackratingenhance_p",
	"normal_end",

	"dame_new_begin",
	"seriesdame",
	"addskilldame1",
	"addskilldame2",
	"addskilldame3",
	"addskilldame4",
	"addskilldame5",
	"addskilldame6",
	"skilllevelexp",
	"skilladdmissles",
	"nstatus_p",
	"invisibility_p",
	"madness_p",
	"dame_new_end",

	"pass_new_begin",
    "stateskilltime",
	"manashieldpoint_p",
	"poisondamagereturn_p",
	"buffemtyreturn_p",
	"returndoskillemty_p",
	"lifereplenishadd_p",
	"buffemtyres_p",
	"timecastskilleffec_v",
	"doskilleffec_p",
	"manafromdame_p",
	"doskillbuff1",
	"doskillbuff2",
	"doskillbuff3",
	"doskillbuff4",
	"doskillbuff5",
	"doskilleffec1_p",
	"timecastskilleffec1_v",
	"doskilleffec2_p",
	"timecastskilleffec2_v",
	"manashieldex_v",
	"addskilllevel_v",
	"addallskillslevel_v",
	"damagereturnresist_p",

	"itempinknomagic",

	"pass_new_end",
};



KMagicDesc	g_MagicDesc;
KMagicDesc::KMagicDesc()
{
	m_szDesc[0] = 0;
}

KMagicDesc::~KMagicDesc()
{
}

BOOL KMagicDesc::Init()
{
//	g_SetFilePath("\\");
	return (m_IniFile.Load(MAGICDESC_FILE));
}

const char* KMagicDesc::GetDesc(void *pData)
{
	
	char	szTempDesc[128];
	char*	pTempDesc = szTempDesc;
	int		i = 0;

	ZeroMemory(m_szDesc, 128);
	
	if (!pData)
		return NULL;

	KMagicAttrib* pAttrib = (KMagicAttrib *)pData;

	const char	*pszKeyName = g_MagicID2String(pAttrib->nAttribType);
	m_IniFile.GetString("Descript", pszKeyName, "", szTempDesc, 128);
	while(*pTempDesc)
	{
		if (*pTempDesc == '#')
		{
			int	nDescAddType = 0;
			switch(*(pTempDesc + 3))
			{
			case '+':
				nDescAddType = 1;
				break;
			case '~':
				nDescAddType = 2;
				break;
			default:
				nDescAddType = 0;
				break;
			}
			int nValue = 0;
			
			switch(*(pTempDesc + 2))
			{
			case 'A':
				nValue = (int)(pAttrib->nValue[0] / 256);
				break;			
			case '1':
				nValue = pAttrib->nValue[0];
				break;
			case '2':
				nValue = pAttrib->nValue[1];
				break;
			case '3':
				nValue = pAttrib->nValue[2];
				break;
			case '6':
				nValue = (int)(pAttrib->nValue[2] / 256);
				break;
			case '7':
				nValue = (int)(pAttrib->nValue[0] % 256);
				break;
			case '9':
				nValue = (int)(pAttrib->nValue[2] % 256);
				break;
			default:
				nValue = pAttrib->nValue[0];
				break;
			}
			switch(*(pTempDesc+1))
			{
			case 'm':		// ÃÅÅÉ
				strcat(m_szDesc, g_Faction.m_sAttribute[nValue].m_szName);
				i += strlen(g_Faction.m_sAttribute[nValue].m_szName);
				break;
			case 's':		// ÎåÐÐ
				switch(nValue)
				{
				case series_metal:
					strcat(m_szDesc, "HÖ Kim");
					break;
				case series_wood:
					strcat(m_szDesc, "HÖ Méc");
					break;
				case series_water:
					strcat(m_szDesc, "HÖ Thñy");
					break;
				case series_fire:
					strcat(m_szDesc, "HÖ Háa");
					break;
				case series_earth:
					strcat(m_szDesc, "HÖ Thæ ");
					break;
				default:
					strcat(m_szDesc, "V« HÖ ");
					break;
				}
				i += 4;
				break;
			case 'k':		// ÏûºÄÀàÐÍ
				switch(nValue)
				{
				case 0:
					strcat(m_szDesc, "Néi Lùc\n");
					break;
				case 1:
					strcat(m_szDesc, "Sinh Lùc\n");
					break;
				case 2:
					strcat(m_szDesc, "ThÓ Lùc\n");
					break;
				case 3:
					strcat(m_szDesc, "TiÒn\n");
					break;
				default:
					strcat(m_szDesc, "Néi Lùc\n");
					break;
				}
				i += 4;
				break;
			case 'd':		// Êý×Ö
				{
					switch(nDescAddType)
					{
					case 1:
						if (nValue > 0)
						{
							strcat(m_szDesc, "+");
							i += 1;
						}
						else
						{
							nValue = -nValue;
							strcat(m_szDesc, "-");
							i += 1;
						}
						break;
					case 2:
						if (nValue > 0)
						{
							strcat(m_szDesc, "-");
							i += 1;
						}
						break;
					default:
						break;
					}
					char	szMsg[16];
					sprintf(szMsg, "%d", nValue);
					strcat(m_szDesc, szMsg);
					i += strlen(szMsg);
				}
				break;
            case 'f':		// Êý×Ö
					char szMsg[80];
					sprintf(szMsg, "%d", (int)(nValue/18));
					strcat(m_szDesc,szMsg);
					i += strlen(szMsg);
				break;
			case 'x':		// ÐÔ±ð
				if (nValue)
					strcat(m_szDesc, "N÷ ");
				else
					strcat(m_szDesc, "Nam ");
				i += 4;
				break;
			case 'l':
				{
					char	szMsg[60];
					if (nValue > 0)
					{
						ISkill* pSkill =  g_SkillManager.GetSkill(nValue, 1);
						sprintf(szMsg, "Kü n¨ng [ %s ]: +", pSkill->GetSkillName());
					}
					else
						sprintf(szMsg, "Kü n¨ng vâ c«ng vèn cã: +", "");
					strcat(m_szDesc, szMsg);
					i += strlen(szMsg);
				}
				break;
             case 'w':
				{
					char szMsg[32];
					switch(nValue)
					{
					case 9:
						strcpy(szMsg, "tµn ¶nh kÐo dµi");
						break;
					default:
						strcpy(szMsg, "tµn ¶nh");
						break;
					}
					strcat(m_szDesc,szMsg);
					i += strlen(szMsg);
				}
				break;
			default:
				break;
			}
			pTempDesc += 4;
		}
		else
		{
			m_szDesc[i] = *pTempDesc;
			pTempDesc++;
			i++;
		}
	}
	return m_szDesc;
}













const char* KMagicDesc::GetDesc2(int nType)
{
	
	char	szTempDesc[128];
	char*	pTempDesc = szTempDesc;
	int		i = 0;

	ZeroMemory(m_szDesc, 128);
	
	if (!nType)
		return NULL;

	const char	*pszKeyName = g_MagicID2String(nType);
	m_IniFile.GetString("Descript", pszKeyName, "", szTempDesc, 128);
	while(*pTempDesc)
	{
		if (*pTempDesc == ':')
		{
	
			m_szDesc[i] = 0;
			break;
		}
		else
		{
			m_szDesc[i] = *pTempDesc;
			pTempDesc++;
			i++;
		}
	}
	return m_szDesc;
}













const char * g_MagicID2String(int nAttrib)
{
	if ((nAttrib < 0) || nAttrib >= magic_pass_new_end) return MAGIC_ATTRIB_STRING[magic_pass_new_end];
	return 	MAGIC_ATTRIB_STRING[nAttrib];
}

int	g_String2MagicID(char * szMagicAttribName)
{
	if ((!szMagicAttribName) || (!szMagicAttribName[0])) return -1;

	//nValue2 µ±ÖµÎª-1Ê±ÎªÓÀ¾ÃÐÔ×´Ì¬£¬0Îª·Ç×´Ì¬£¬ÆäËüÖµÎªÓÐÊ±Ð§ÐÔ×´Ì¬Ä§·¨Ð§¹û
	//ÐèÒª½«×´Ì¬Êý¾ÝÓë·Ç×´Ì¬Êý¾Ý·ÖÀë³öÀ´£¬·ÅÈëÏàÓ¦µÄÊý×éÄÚ£¬²¢¼ÇÂ¼×ÜÊýÁ¿
	
	for (int i  = 0 ; i <= magic_pass_new_end; i ++)
	{
		if (!strcmp(szMagicAttribName, g_MagicID2String(i)))
			return i;
	}
	return -1;
}

int	KMagicDesc::String2MagicID(char * szMagicAttribName)
{
	if ((!szMagicAttribName) || (!szMagicAttribName[0])) return -1;

	//nValue2 µ±ÖµÎª-1Ê±ÎªÓÀ¾ÃÐÔ×´Ì¬£¬0Îª·Ç×´Ì¬£¬ÆäËüÖµÎªÓÐÊ±Ð§ÐÔ×´Ì¬Ä§·¨Ð§¹û
	//ÐèÒª½«×´Ì¬Êý¾ÝÓë·Ç×´Ì¬Êý¾Ý·ÖÀë³öÀ´£¬·ÅÈëÏàÓ¦µÄÊý×éÄÚ£¬²¢¼ÇÂ¼×ÜÊýÁ¿
	
	for (int i  = 0 ; i <= magic_normal_end; i ++)
	{
		if (!strcmp(szMagicAttribName, g_MagicID2String(i)))
			return i;
	}
	return -1;
}


