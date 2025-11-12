/*****************************************************************************************
//	界面--小地图
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-21
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/Wnds.h"
#include "../Elem/WndMessage.h"
#include "UiMiniMap.h"
#include "UiFindPos.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "UiWorldMap.h"
#include "UiMapCave.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/gamedatadef.h"
#include "../../../Engine/src/KDebug.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../Elem/TextPic.h"
#include <time.h>

extern iRepresentShell*	g_pRepresentShell;
extern iCoreShell*			g_pCoreShell;
extern IInlinePicEngineSink *g_pIInlinePicSink;


#define	SCHEME_INI_SMALL	"UiMiniMapSmall.ini"
#define	SCHEME_INI_BIG		"UiMiniMapBig.ini"

//地图拖动后还原的时间(秒)
#define MAP_RECOVER_TIME		3000
#define	NOT_DRAGING_MAP			-1
#define	WAIT_TO_BE_SET_BACK		-2
#define	IS_DRAGING_MAP			(m_OldPos.x != NOT_DRAGING_MAP)
#define	IS_WAIT_TO_SET_BACK		(m_OldPos.x == WAIT_TO_BE_SET_BACK)

const char*	s_ModeName[MINIMAP_M_COUNT] = 
{
	"B秐  nh?", 
	"B秐  l韓",
	"B秐  th?g韎",
	"T総 b秐 "
};

KUiMiniMap*			KUiMiniMap::ms_pSelf = NULL;
static MINIMAP_MODE	s_eMapMode = MINIMAP_M_NONE;
static MINIMAP_MODE	s_eMapOldMode = MINIMAP_M_BRIEF_PIC;

KUiMiniMap::KUiMiniMap()
{
	m_OldPos.x = NOT_DRAGING_MAP;

	m_bCheckSearchMap = FALSE;
}

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiMiniMap* KUiMiniMap::OpenWindow()
{
	if (ms_pSelf == NULL)
	{
		ms_pSelf = new KUiMiniMap;
		if (ms_pSelf)
			ms_pSelf->Initialize();
	}
	if (ms_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		ms_pSelf->Show();
		ms_pSelf->BringToTop();
	}
	return ms_pSelf;
}

//--------------------------------------------------------------------------
//	功能：如果窗口正被显示，则返回实例指针
//--------------------------------------------------------------------------
KUiMiniMap* KUiMiniMap::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
		return ms_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
void KUiMiniMap::Initialize()
{
	AddChild(&m_Shadow);
	AddChild(&m_SceneName);

	AddChild(&m_TongMapInfo1);
    AddChild(&m_TongMapInfo2);


	AddChild(&m_SearchMapBtn);
	AddChild(&m_SwitchBtn);
	AddChild(&m_WorldMapBtn);
	AddChild(&m_CaveMapBtn);

	AddChild(&m_ScenePos);
	AddChild(&m_AutoMove);
	m_bCheckSearchMap = FALSE;
	m_bAutoMove = FALSE;
	m_Style &= ~WND_S_VISIBLE;

	char Scheme[128];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this, WL_NORMAL);
}

//--------------------------------------------------------------------------
//	功能：关闭窗口
//--------------------------------------------------------------------------
void KUiMiniMap::CloseWindow()
{
	if (ms_pSelf)
	{
		ms_pSelf->StopScrollMap();
		ms_pSelf->MapMoveBack();
		if (g_pCoreShell)
			g_pCoreShell->SceneMapOperation(GSMOI_IS_SCENE_MAP_SHOWING, SCENE_PLACE_MAP_ELEM_NONE, 0);
		ms_pSelf->Destroy();
		ms_pSelf = NULL;
	}
}

//--------------------------------------------------------------------------
//	功能：载入窗口的界面方案
//--------------------------------------------------------------------------
void KUiMiniMap::LoadScheme(const char* pScheme)
{
	if (ms_pSelf)
	{
		char		szBuf[128];
		KIniFile	Ini;
		sprintf(szBuf, "%s\\%s", pScheme,
			s_eMapMode == MINIMAP_M_BRIEF_PIC ? SCHEME_INI_SMALL : SCHEME_INI_BIG);
		if (Ini.Load(szBuf))
		{
			ms_pSelf->LoadScheme(&Ini);
		}
	}
}

void KUiMiniMap::LoadScheme(KIniFile* pIni)
{
	if (pIni == NULL)
		return;
	Init(pIni, "MiniMap");
	m_Shadow.Init(pIni, "NameShadow");
	m_SceneName.Init(pIni, "SceneName");


	m_TongMapInfo1.Init(pIni, "TongMapInfo1");
    m_TongMapInfo2.Init(pIni, "TongMapInfo2");

	m_ScenePos.Init(pIni, "ScenePos");
	m_AutoMove.Init(pIni, "AutoMove");
	m_SwitchBtn.Init(pIni, "SwitchBtn");
	m_WorldMapBtn.Init(pIni, "WorldMapBtn");

	m_SearchMapBtn.Init(pIni, "BtnFlag");
	m_CaveMapBtn.Init(pIni, "CaveMapBtn");

	pIni->GetInteger("MapRect", "Left",   0, (int*)&m_MapPos.x);
	pIni->GetInteger("MapRect", "Top",    0, (int*)&m_MapPos.y);
	pIni->GetInteger("MapRect", "Width",  0, (int*)&m_MapSize.cx);
	pIni->GetInteger("MapRect", "Height", 0, (int*)&m_MapSize.cy);

	if (IsVisible() && g_pCoreShell)
	{
		g_pCoreShell->SceneMapOperation(GSMOI_IS_SCENE_MAP_SHOWING,
			SCENE_PLACE_MAP_ELEM_PIC | SCENE_PLACE_MAP_ELEM_CHARACTER | SCENE_PLACE_MAP_ELEM_PARTNER,
			(m_MapSize.cx | (m_MapSize.cy << 16)));
	}
}

//--------------------------------------------------------------------------
//	功能：窗口函数
//--------------------------------------------------------------------------
int KUiMiniMap::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_SwitchBtn)
			MapSetMode(s_eMapMode == MINIMAP_M_BRIEF_PIC ? MINIMAP_M_BRIEF_PIC_BROWSE : MINIMAP_M_BRIEF_PIC);
		else if (uParam == (unsigned int)(KWndWindow*)&m_WorldMapBtn)
		{
			MapSetMode(MINIMAP_M_WORLD_MAP);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CaveMapBtn)
		{
			MapSetMode(MINIMAP_M_CAVE_MAP);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_SearchMapBtn)
		{
			/*g_pCoreShell->SceneMapOperation(GSMOI_SCENE_POST_SEARCH_MAP, 0,0);
			g_pCoreShell->SceneMapOperation(GSMOI_DELETE_DRAW_POS, 0, 0);
			if (m_bCheckSearchMap)
			m_bCheckSearchMap = FALSE;
			else
			m_bCheckSearchMap = TRUE;
			*/

			if (g_pCoreShell->GetPaintFindPos())
			{
				g_pCoreShell->SetPaintFindPos(FALSE);
				m_bCheckSearchMap = FALSE;
			}
			else
			{
				g_pCoreShell->SetPaintFindPos(TRUE);
				m_bCheckSearchMap = TRUE;
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_AutoMove)
		{
			if (!m_bAutoMove)
			{
				g_pCoreShell->SetAutoMove(true);			
				m_bAutoMove = TRUE;
			}
			else
			{
				g_pCoreShell->SetAutoMove(false);		
				m_bAutoMove = FALSE;
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ScenePos)
		{
			if (!KUiFindPos::GetIfVisible())
			{
				KUiFindPos::OpenWindow();
				
				
				int nPositionX, nPositionY;
				GetPosition(&nPositionX, &nPositionY);
				int nSizeX, nSizeY;
				GetSize(&nSizeX, &nSizeY);
				
				KUiFindPos::SetPositionOpen(nPositionX,nPositionY+nSizeY+20);
				g_pCoreShell->SceneMapOperation(GSMOI_DELETE_DRAW_POS, 0, 0);
			}
		}
		
		break;
	case WM_LBUTTONDOWN:
		/*{
				int nCursorX, nCursorY;
				Wnd_GetCursorPos(&nCursorX, &nCursorY);
				int nPositionX, nPositionY;
				GetPosition(&nPositionX, &nPositionY);
				int nSizeX, nSizeY;
				GetSize(&nSizeX, &nSizeY);
			g_DebugLog("%d - %d",nCursorX,nCursorY);
			if (nCursorX >= nPositionX && nCursorX <= nPositionX +  nSizeX &&
				nCursorY >= nPositionY && nCursorY <= nPositionY +  nSizeY
				)
			{


	KUiSceneTimeInfo	Spot;

	GetUpdateSceneTimeInfo(&Spot);
	g_pCoreShell->SceneMapOperation(GSMOI_SCENE_TIME_INFO, (unsigned int)&Spot, 0);
	// g_DebugLog("Pos: %s - %d - %d", Spot.szSceneName,Spot.nScenePos0/8,Spot.nScenePos1/8);
	g_pCoreShell->SceneMapOperation(GSMOI_FIND_POS, Spot.nScenePos0/8,Spot.nScenePos1/8);
		//g_pCoreShell->SceneMapOperation(GSMOI_SCENE_POST_SEARCH_MAP, Spot.nPostX,Spot.nPostY);
		m_bCheckSearchMap = FALSE;
}
else
{
		if (IS_DRAGING_MAP && ((uParam & MK_CONTROL) == 0) && g_pCoreShell)
			g_pCoreShell->SceneMapOperation(GSMOI_SCENE_FOLLOW_WITH_MAP, 0, 0);
		StopScrollMap();

		Wnd_TransmitInputToGameSpace(uMsg, uParam, nParam);
}

		}*/
		if (m_bCheckSearchMap && g_pCoreShell->GetPaintFindPos())
		{
			int nCursorX, nCursorY;
			Wnd_GetCursorPos(&nCursorX, &nCursorY);	
			g_pCoreShell->SetPaintFindPos(TRUE);
			m_bCheckSearchMap = FALSE;
			g_pCoreShell->SceneMapFindPosOperation(GSMOI_PAINT_SCENE_FIND_POS, nCursorX, nCursorY, true, true);

/*
			KUiSceneCalcPos	Spot;
			g_pCoreShell->SceneMapFindPosOperation(GSMOI_SCENE_CALCFLAG, (unsigned int)&Spot.nScenePos0, (unsigned int)&Spot.nScenePos1);

		int srcX = m_MapPos.x + m_nAbsoluteLeft;
		int srcY = m_MapPos.y + m_nAbsoluteTop;

			srcX *= 16;
			srcY *= 32;

			int destX = Spot.nScenePos0*16;
			int destY = Spot.nScenePos1*32;

			char szScenePos[32];
			sprintf(szScenePos,"srcX %d, xrcY %d  => destX %d,destY %d", srcX, srcY, destX, destY);
			MessageBox(NULL,szScenePos,"Info",MB_OK);
*/
		}

		break;


	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MOUSEHOVER:
		Wnd_TransmitInputToGameSpace(uMsg, uParam, nParam);
		break;
	case WND_N_CHILD_MOVE:
		if (uParam == (unsigned int)(KWndWindow*)&m_SceneName ||
			uParam == (unsigned int)(KWndWindow*)&m_ScenePos ||
			uParam == (unsigned int)(KWndWindow*)&m_Shadow)
		{
			KWndWindow* pWnd = (KWndWindow*)uParam;
			int nMoveOffsetX = (short)(nParam & 0xffff);
			int nMoveOffsetY = (short)((nParam >> 16) & 0xffff);

			int x, y;
			pWnd->GetPosition(&x, &y);
			x -= nMoveOffsetX;
			y -= nMoveOffsetY;
			pWnd->SetPosition(x, y);	//按钮跟着窗口变化，所以要先变回原样
			GetPosition(&x, &y);
			x += nMoveOffsetX;
			y += nMoveOffsetY;
			SetPosition(x, y);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			Wnd_SetCapture(this);
			Wnd_GetCursorPos((int *)&m_OldPos.x, (int *)&m_OldPos.y);
		}

		break;
	case WM_MOUSEMOVE:
		if(IS_DRAGING_MAP && (uParam & MK_RBUTTON))
		{
			int nCursorX, nCursorY;
			Wnd_GetCursorPos(&nCursorX, &nCursorY);
			m_nOffsetX = m_OldPos.x - nCursorX;
			m_nOffsetY = m_OldPos.y - nCursorY;
			m_OldPos.x   =  nCursorX;
			m_OldPos.y   =  nCursorY;
			MapScroll(uParam & MK_CONTROL);
		}
		else
		{
			Wnd_TransmitInputToGameSpace(uMsg, uParam, nParam);
		}
		break;
	case WM_RBUTTONUP:
		if (IS_DRAGING_MAP && ((uParam & MK_CONTROL) == 0) && g_pCoreShell)
			g_pCoreShell->SceneMapOperation(GSMOI_SCENE_FOLLOW_WITH_MAP, 0, 0);
		StopScrollMap();
		break;
	default:
		nRet = KWndWindow::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nRet;
}

//停止拖动地图
void KUiMiniMap::StopScrollMap()
{
	if (IS_DRAGING_MAP)
	{
		m_uLastScrollTime = IR_GetCurrentTime();
		Wnd_ReleaseCapture();
		m_OldPos.x = WAIT_TO_BE_SET_BACK;
	}
}

//地图还原原始偏移
void KUiMiniMap::MapMoveBack()
{
	m_OldPos.x = NOT_DRAGING_MAP;
	m_nOffsetX = 0;
	m_nOffsetY = 0;
	if (g_pCoreShell)
	{
		g_pCoreShell->SceneMapOperation(GSMOI_SCENE_FOLLOW_WITH_MAP, 0, 0);
		g_pCoreShell->SceneMapOperation(GSMOI_SCENE_MAP_FOCUS_OFFSET, 0, 0);
	}
}

//绘制窗口
void KUiMiniMap::PaintWindow()
{
	KWndImage::PaintWindow();
	if (g_pCoreShell)
	{
		int nX = m_MapPos.x + m_nAbsoluteLeft;
		int nY = m_MapPos.y + m_nAbsoluteTop;
		g_pCoreShell->SceneMapOperation(GSMOI_PAINT_SCENE_MAP, nX, nY);

		if (g_pRepresentShell)
		{
			KRURect	rect;
			rect.Color.Color_dw = 0xff000000;
			rect.oEndPos.nX = rect.oPosition.nX = nX - 1;
			rect.oEndPos.nY = rect.oPosition.nY = nY - 1;
			rect.oEndPos.nZ = rect.oPosition.nZ = 0;
			rect.oEndPos.nX += m_MapSize.cx;
			rect.oEndPos.nY += m_MapSize.cy;
			g_pRepresentShell->DrawPrimitives(1, &rect, RU_T_RECT, true);

			int nCursorX, nCursorY;
			Wnd_GetCursorPos(&nCursorX, &nCursorY);

			int nPositionX, nPositionY;

			GetPosition(&nPositionX, &nPositionY);

			int nSizeX, nSizeY;
			GetSize(&nSizeX, &nSizeY);



			if (m_bCheckSearchMap &&
				nCursorX >= nPositionX && nCursorX <= nPositionX +  nSizeX &&
				nCursorY >= nPositionY && nCursorY <= nPositionY +  nSizeY
				)
			{


	KRUImage RPostSearchImage;
	RPostSearchImage.nType = ISI_T_SPR;
	RPostSearchImage.Color.Color_b.a = 255;
	RPostSearchImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	RPostSearchImage.uImage = 0;
	RPostSearchImage.nISPosition = IMAGE_IS_POSITION_INIT;
	RPostSearchImage.bRenderFlag = 0;
    strcpy(RPostSearchImage.szImage,"\\Spr\\Ui3\\小地图\\地图小旗帜.spr");
	RPostSearchImage.oPosition.nX = nCursorX - 1;
	RPostSearchImage.oPosition.nY = nCursorY - 12;
	RPostSearchImage.oPosition.nZ = 0;
	RPostSearchImage.nFrame = 0;
    g_pRepresentShell->DrawPrimitives(1, &RPostSearchImage, RU_T_IMAGE, 1);



			}




		}
	}
}

void KUiMiniMap::Breathe()
{
	if (IS_WAIT_TO_SET_BACK && g_pCoreShell &&
		IR_IsTimePassed(MAP_RECOVER_TIME, m_uLastScrollTime))
	{
		MapMoveBack();
	}
		if (KUiFindPos::GetIfVisible())
			{
			int nPositionX, nPositionY;
			GetPosition(&nPositionX, &nPositionY);
			int nSizeX, nSizeY;
			GetSize(&nSizeX, &nSizeY);

			KUiFindPos::SetPositionOpen(nPositionX,nPositionY+nSizeY+20);

			}
			int nCursorX, nCursorY;	
		if (m_bCheckSearchMap)
		{
			Wnd_GetCursorPos(&nCursorX, &nCursorY);	
			g_pCoreShell->SceneMapFindPosOperation(GSMOI_PAINT_SCENE_FIND_POS, nCursorX, nCursorY, true, false);	
		}
}

//活动函数




void KUiMiniMap::StopAndMapMoveBack()
{

if (ms_pSelf)
{
	ms_pSelf->StopScrollMap();
	ms_pSelf->MapMoveBack();
}

}


void KUiMiniMap::GetUpdateSceneTimeInfo(KUiSceneTimeInfo* pInfo)
{

if (ms_pSelf)
{


		    int nCursorX, nCursorY;
			Wnd_GetCursorPos(&nCursorX, &nCursorY);
			int nPositionX, nPositionY;
			ms_pSelf->GetPosition(&nPositionX, &nPositionY);
			int nSizeX, nSizeY;
			ms_pSelf->GetSize(&nSizeX, &nSizeY);

if (nCursorX >= nPositionX && nCursorX <= nPositionX +  nSizeX &&
	nCursorY >= nPositionY && nCursorY <= nPositionY +  nSizeY
	)
{
pInfo->nScenePos0 = ms_pSelf->m_MapPos.x + ms_pSelf->m_nAbsoluteLeft;
pInfo->nScenePos1 = ms_pSelf->m_MapPos.y + ms_pSelf->m_nAbsoluteTop;
pInfo->nPostX = nCursorX;
pInfo->nPostY = nCursorY;
}

}

}

void KUiMiniMap::UpdateSceneTimeInfo(KUiSceneTimeInfo* pInfo)
{
	if (ms_pSelf && pInfo)
	{


		ms_pSelf->m_SceneName.SetText(pInfo->szSceneName);


		
		if (pInfo->szTongName[0] && s_eMapMode != MINIMAP_M_BRIEF_PIC)
		{
		char szinfo[256];
		sprintf(szinfo,"Thu?su蕋:%d%,Ch?s?v藅 gi?%d%",pInfo->szTongT,pInfo->szTongVG);
		ms_pSelf->m_TongMapInfo1.SetText(szinfo);
		sprintf(szinfo,"Th竔 th?%s,Bang h閕 chi誱 l躰h:%s",pInfo->szTongNameBC,pInfo->szTongName);
		ms_pSelf->m_TongMapInfo2.SetText(szinfo);
		}
		

		else
		{
		ms_pSelf->m_TongMapInfo1.SetText("");
		ms_pSelf->m_TongMapInfo2.SetText("");
		}







		    int nCursorX, nCursorY;
			Wnd_GetCursorPos(&nCursorX, &nCursorY);
			int nPositionX, nPositionY;
			ms_pSelf->GetPosition(&nPositionX, &nPositionY);
			int nSizeX, nSizeY;
			ms_pSelf->GetSize(&nSizeX, &nSizeY);



			char szScenePos[32];

			if (ms_pSelf->m_bCheckSearchMap && g_pCoreShell->GetPaintFindPos())
			{
				KUiSceneCalcPos	Spot;
				g_pCoreShell->SceneMapFindPosOperation(GSMOI_SCENE_CALCFLAG, (unsigned int)&Spot.nScenePos0, (unsigned int)&Spot.nScenePos1);
				if (Spot.nScenePos0 && Spot.nScenePos1)
					sprintf(szScenePos, "%d/%d", Spot.nScenePos0, Spot.nScenePos1);
				else
					sprintf(szScenePos, "%d/%d", pInfo->nScenePos0 / 8, pInfo->nScenePos1 / 8);
			}
			else
				sprintf(szScenePos, "%d/%d", pInfo->nScenePos0 / 8, pInfo->nScenePos1 / 8);


			ms_pSelf->m_ScenePos.SetLabel(szScenePos);


		
//		if (ms_pSelf->m_bAutoMove)
//		ms_pSelf->m_AutoMove.SetLabel("D鮪g L筰");
//		else
//		ms_pSelf->m_AutoMove.SetLabel("Di Chuy觧");
		

	//	ms_pSelf->m_ScenePos.Set2IntText(pInfo->nScenePos0 / 8, pInfo->nScenePos1 / 8, '/');
	}
}

//显示窗口
void KUiMiniMap::Show()
{
	KWndImage::Show();
	if (g_pCoreShell)
	{
		g_pCoreShell->SceneMapOperation(GSMOI_IS_SCENE_MAP_SHOWING,
			SCENE_PLACE_MAP_ELEM_PIC | SCENE_PLACE_MAP_ELEM_CHARACTER | SCENE_PLACE_MAP_ELEM_PARTNER,
			(ms_pSelf->m_MapSize.cx | (ms_pSelf->m_MapSize.cy << 16)));	
	}
}

//隐藏窗口
void KUiMiniMap::Hide()
{
	KWndImage::Hide();
	if (g_pCoreShell)
		g_pCoreShell->SceneMapOperation(GSMOI_IS_SCENE_MAP_SHOWING, SCENE_PLACE_MAP_ELEM_NONE, 0);
}

//地图卷动
void KUiMiniMap::MapScroll(int nbScrollScene)
{
	KSceneMapInfo MapInfo;
    if(g_pCoreShell && g_pCoreShell->SceneMapOperation(GSMOI_SCENE_MAP_INFO, (unsigned int)&MapInfo, 0))
	{
		int nX = MapInfo.nOrigFocusH + MapInfo.nFocusOffsetH + MapInfo.nScallH * m_nOffsetX;
		int nY = MapInfo.nOrigFocusV + MapInfo.nFocusOffsetV + MapInfo.nScallV * m_nOffsetY;
		if (nX < MapInfo.nFocusMinH)
			nX = MapInfo.nFocusMinH; 
		else if (nX > MapInfo.nFocusMaxH)
			nX = MapInfo.nFocusMaxH;
		if (nY < MapInfo.nFocusMinV)
			nY = MapInfo.nFocusMinV;
		else if (nY > MapInfo.nFocusMaxV)
			nY = MapInfo.nFocusMaxV;

		nX -= MapInfo.nOrigFocusH;
		nY -= MapInfo.nOrigFocusV;

		g_pCoreShell->SceneMapOperation(GSMOI_SCENE_FOLLOW_WITH_MAP, 0, nbScrollScene);
		g_pCoreShell->SceneMapOperation(GSMOI_SCENE_MAP_FOCUS_OFFSET, (unsigned int)nX, nY);
	}
}

void maptoggle()
{
}

void MapToggleStatus()
{
	if (s_eMapMode == MINIMAP_M_WORLD_MAP)
		MapSetMode(s_eMapOldMode);
	else if (s_eMapMode == MINIMAP_M_CAVE_MAP)
		MapSetMode(s_eMapOldMode);
	else if (s_eMapMode == MINIMAP_M_NONE)
		MapSetMode(MINIMAP_M_BRIEF_PIC);
	else if (s_eMapMode == MINIMAP_M_BRIEF_PIC)
		MapSetMode(MINIMAP_M_BRIEF_PIC_BROWSE);
	else
	{
		MapSetMode(MINIMAP_M_NONE);
		g_pCoreShell->SceneMapOperation(GSMOI_DELETE_DRAW_LINE, 0, 0);
		KUiFindPos::CloseWindow();
	}
}

//设置显示内容模式
void MapSetMode(MINIMAP_MODE eMode)
{
	if (eMode == s_eMapMode &&
		(eMode < MINIMAP_M_NONE || eMode > MINIMAP_M_CAVE_MAP))
		return;

	if (s_eMapMode != MINIMAP_M_WORLD_MAP && s_eMapMode != MINIMAP_M_CAVE_MAP)
	{
		s_eMapOldMode = s_eMapMode;
	}
	
	switch(s_eMapMode = eMode)
	{
	case MINIMAP_M_NONE:			//没有显示地图
		KUiMiniMap::CloseWindow();
		KUiWorldmap::CloseWindow();
		KUiMapCave::CloseWindow();
		break;
	case MINIMAP_M_BRIEF_PIC:		//缩略图
	case MINIMAP_M_BRIEF_PIC_BROWSE://缩略图浏览/放大模式
		KUiWorldmap::CloseWindow();
		KUiMapCave::CloseWindow();
		if (KUiMiniMap::GetIfVisible() == NULL)
			KUiMiniMap::OpenWindow();
		else
		{
			char Scheme[128];
			g_UiBase.GetCurSchemePath(Scheme, 256);
			KUiMiniMap::LoadScheme(Scheme);
		}
		break;
	case MINIMAP_M_WORLD_MAP:		//世界地图
		KUiMiniMap::CloseWindow();
		KUiMapCave::CloseWindow();
		KUiWorldmap::OpenWindow();
		break;
	case MINIMAP_M_CAVE_MAP:		//世界地图
		KUiMiniMap::CloseWindow();
		KUiWorldmap::CloseWindow();
		KUiMapCave::OpenWindow();
		break;
	}
}
