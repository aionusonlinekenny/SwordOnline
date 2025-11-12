#ifndef _SERVER

#include "KCore.h"
#include <math.h>

#include "Kjxpathfinder.h"
KJXPathFinder g_JXPathFinder;

#include "ScenePlaceMapC.h"


#define USE_MAX_OBSTACLE 0

CONST BYTE			m_sCloseFlag = 10;
CONST BYTE			m_sOpenFlag = m_sCloseFlag - 1;

KJXPathFinder::KJXPathFinder(): m_nMapWidth(0), m_nMapHeight(0), 
								m_map(NULL)
{
}
KJXPathFinder::~KJXPathFinder()
{
	ClearMap();
}

VOID KJXPathFinder::ClearMap()
{
	for (INT i = 0; i < m_nMapWidth; i++)
		delete[] m_map[i];
	delete[] m_map;
	m_map = NULL;
	m_nMapWidth = 0;
	m_nMapHeight = 0;

	for (INT a = 0; a < m_OpenTable.GetSize(); a++)
		delete m_OpenTable.GetNode(a);

	m_OpenTable.Reset(0);
}

BOOL KJXPathFinder::CreateMap(INT w, INT h)
{
	ClearMap();

	m_map = new PathNode*[w];
	memset(m_map, 0, sizeof(PathNode*) * w);
	if (!m_map)
		return FALSE;

	for (INT i = 0; i < w; i++)
	{
		m_map[i] = new PathNode[h];
		if (!m_map[i])
		{
			ClearMap();
			return FALSE;
		}
		memset(m_map[i], 0, sizeof(PathNode) * h);
	}
	
	m_nMapWidth = w;
	m_nMapHeight = h;

	m_OpenTable.Reset(w * h);
	return TRUE;
}

BOOL KJXPathFinder::Init(RECT* rc,KScenePlaceMapC* mapper)
{
	m_bReady = LoadMap(rc, mapper);
	
	m_target.x = m_target.y = -1;
	
	return m_bReady;
}

//#include "../KRegion.h"
BOOL KJXPathFinder::LoadMap(RECT* rc, KScenePlaceMapC* mapper)
{
	m_LeftTop.left = rc->left * 16;
	m_LeftTop.top = rc->top * 32;
	m_LeftTop.right = rc->right * 16;
	m_LeftTop.bottom = rc->bottom * 32;


	//INT w = (rc->right - rc->left + 1) * 16;//dat文件
	//INT h = (rc->bottom - rc->top + 1) * 32;//文件夹
	
	int w,h,x,y;
	w  = (rc->right - rc->left + 1);
	h = (rc->bottom - rc->top + 1); 
	//m_vec.AddVector(); 

	if (!CreateMap(w*REGION_GRID_WIDTH,h*REGION_GRID_HEIGHT))
		return FALSE;

	BYTE** m_ppbtBarrier = mapper->GetbtBarrier();

	for ( y = 0; y < h; y++)	   //历遍所有文件夹
	{
		for ( x = 0; x < w; x++) //i*j 个区域	dat文件	历遍所有dat文件编号
		{   						
			BYTE* lpbtObstacle = m_ppbtBarrier[y*w + x];

			for (int i = 0; i < REGION_GRID_HEIGHT; ++i)
			{//512个点
				for (int j = 0; j < REGION_GRID_WIDTH; ++j)
				{ 
					BYTE obstacle = lpbtObstacle[i*REGION_GRID_WIDTH + j];

					m_map[x*REGION_GRID_WIDTH+j][y*REGION_GRID_HEIGHT+i].factor = 1.0f;

					if ((obstacle == 0) || ((2 <= obstacle) && (obstacle <= 5))) //不记录完整障碍？
						m_map[x*REGION_GRID_WIDTH+j][y*REGION_GRID_HEIGHT+i].canwoalk = walkable;  //记录障碍点
					else
						m_map[x*REGION_GRID_WIDTH+j][y*REGION_GRID_HEIGHT+i].canwoalk = unwalkable;
				}	   
			}
		}
	}

	CalcFactors();
	return TRUE;
}

CONST INT	cnFactorRadius = 5;
FLOAT CalcFactor(INT dx, INT dy)
{
	FLOAT dis = sqrt((FLOAT)(dx * dx + dy * dy));
	if (cnFactorRadius - dis <= 0)
		return 1.0f;

	return 2.0f * (cnFactorRadius - dis) / cnFactorRadius + 1.0f;
}

VOID KJXPathFinder::CalcFactors()
{
	INT i,j;
	FLOAT factor;
	for (INT a = 0; a < cnFactorRadius; a++)
	{
		for (i = 0; i < m_nMapHeight; i++)
		{
			factor = CalcFactor(a, i);

			if (m_map[a][i].canwoalk && factor > m_map[a][i].factor)
				m_map[a][i].factor = factor;

			factor = CalcFactor(m_nMapWidth-a-1, i);

			if (m_map[m_nMapWidth-a-1][i].canwoalk && factor > m_map[m_nMapWidth-a-1][i].factor)
				m_map[m_nMapWidth-a-1][i].factor = factor;
		}
		for (i = 0; i < m_nMapWidth; i++)
		{
			factor = CalcFactor(i, a);

			if (m_map[i][a].canwoalk && factor > m_map[i][a].factor)
				m_map[i][a].factor = factor;

			factor = CalcFactor(i, m_nMapHeight-a-1);

			if (m_map[i][m_nMapHeight-a-1].canwoalk && factor > m_map[i][m_nMapHeight-a-1].factor)
				m_map[i][m_nMapHeight-a-1].factor = factor;
		}
	}

	for (i = 0; i < m_nMapWidth; i++)//x
	{
		for (j = 0; j < m_nMapHeight; j++)//y
		{
			if (!m_map[i][j].canwoalk)
			{
				BOOL bCanCalc = FALSE;
				if (i - 1 >= 0 && m_map[i-1][j].canwoalk)		// 左
					bCanCalc = TRUE;
				else if (i + 1 < m_nMapWidth && m_map[i+1][j].canwoalk) // 右
					bCanCalc = TRUE;
				else if (j - 1 >= 0 && m_map[i][j-1].canwoalk)		// 上
					bCanCalc = TRUE;
				else if (j + 1 < m_nMapHeight && m_map[i][j+1].canwoalk) // 下
					bCanCalc = TRUE;
				if (bCanCalc)
				{
					INT x = i - cnFactorRadius + 1;
					INT y = j - cnFactorRadius + 1;

					for (INT a = x; a < x + cnFactorRadius*2-1; a++)
					for (INT b = y; b < y + cnFactorRadius*2-1; b++)
					if (a >= 0 && a < m_nMapWidth && b >= 0 && b < m_nMapHeight)
					{
						if (m_map[a][b].canwoalk)
						{
							FLOAT factor = CalcFactor(a-i, b - j);
							if (factor > m_map[a][b].factor)
								m_map[a][b].factor = factor;
						}
					}
				}
			}
		}
	}
}

void KJXPathFinder::FixStart(int& nowx,int& nowy)
{
	if (m_map[nowx][nowy].canwoalk != walkable)
	{
		for (int i = 1; i <= 2; i++)
		{
			if (nowx - i > 0)
			{
				if (m_map[nowx - i][nowy].canwoalk == walkable)
				{
					nowx -= i;
					return;
				}
			}
			if (nowx + i < m_nMapWidth)
			{
				if (m_map[nowx + i][nowy].canwoalk == walkable)
				{
					nowx += i;
					return;
				}
			}
			if (nowy - i > 0)
			{
				if (m_map[nowx][nowy - i].canwoalk == walkable)
				{
					nowy -= i;
					return;
				}
			}
			if (nowy + i < m_nMapHeight)
			{
				if (m_map[nowx][nowy + i].canwoalk == walkable)
				{
					nowy += i;
					return;
				}
			}
		}
	}
}
void KJXPathFinder::GetPath(int OldX,int OldY,std::vector<FindPathNode>& PathIts)
{
	int nowx = OldX/32 - m_LeftTop.left;
	int nowy = OldY/32 - m_LeftTop.top;

	FixStart(nowx,nowy);

	INT x = nowx, y = nowy;

	INT nLen = 0;
	while (x != m_target.x || y != m_target.y)
	{
		if (m_map[x][y].canwoalk != walkable)
			break;		

		register INT tempx = m_map[x][y].parent_x;
		register INT tempy = m_map[x][y].parent_y;
		x = tempx;
		y = tempy;
		nLen ++;
	}
	if (nLen)
	{
		x = nowx;
		y = nowy;

		bool data = false;
		while (true)
		{
			if (data)
			{
				FindPathNode get;
				get.x = (x + m_LeftTop.left) * 32;
				get.y = (y + m_LeftTop.top) * 32;
				PathIts.push_back(get);
			} 
			else
			{
				data = true;
			}

			if (x == m_target.x && y == m_target.y)
				break;

			if (m_map[x][y].canwoalk != walkable)
				break;		

			register INT tempx = m_map[x][y].parent_x;
			register INT tempy = m_map[x][y].parent_y;
			x = tempx;
			y = tempy;
		}
	}
}
VOID KJXPathFinder::ResetMap()
{
	for (INT a = 0; a < m_OpenTable.GetSize(); a++)
		delete m_OpenTable.GetNode(a);

	m_OpenTable.Reset(m_nMapHeight * m_nMapWidth);

	for (INT x = 0; x < m_nMapWidth; x++)
		for (INT y = 0; y < m_nMapHeight; y++)
		{
			PathNode& node = m_map[x][y];
			node.gconst = 0;
			node.parent_x = 0;
			node.parent_y = 0;
			node.pathflag = 0;
			//node.factor = 1.0f;
		}	
}
BOOL KJXPathFinder::FindPath(int OldX,int OldY,int nXpos,int nYpos)
{
	FindPathNode start,target;
	start.x = OldX/32 - m_LeftTop.left;
	start.y = OldY/32 - m_LeftTop.top;

	FixStart(start.x,start.y);

	target.x = nXpos/32 - m_LeftTop.left;
	target.y = nYpos/32 - m_LeftTop.top;

	FixStart(target.x,target.y);

	m_target.x = m_target.y = -1;
	return FindPath(start,target);
}
BOOL KJXPathFinder::FindPath(FindPathNode& start, FindPathNode& target)
{
	if (!m_bReady)
		return FALSE;

	if (target.x == m_target.x && target.y == m_target.y)
	{
		FindPathNode next;
		return GetNextStep(start, next) != emKNEXTSTEP_RESULT_NOANYWAY;
	}
	else
	{
		if (target.x < 0 || target.x >= m_nMapWidth || target.y < 0 || target.y >= m_nMapHeight)
			return FALSE;

		m_target.x = target.x;
		m_target.y = target.y;

		ResetMap();

		WORD hcost = 10*(abs(start.x - target.x) + abs(start.y - target.y));
		AddOpenNode(m_target.x, m_target.y, hcost, hcost, -1, -1);
		
		FindPathNode next;
		return GetNextStep(start, next) != emKNEXTSTEP_RESULT_NOANYWAY;
	}
	return TRUE;
}

KJXPathFinder::KE_NEXTSTEP_RESULT 
KJXPathFinder::GetNextStep(CONST FindPathNode& now, FindPathNode & nextstep)
{
	if (!m_bReady)
		return emKNEXTSTEP_RESULT_NOANYWAY;

	if (now.x == m_target.x && now.y == m_target.y)
		return emKNEXTSTEP_RESULT_ARRIVAL;

	if (now.x < 0 || now.x >= m_nMapWidth || now.y < 0 || now.y >= m_nMapHeight)
		return emKNEXTSTEP_RESULT_NOANYWAY;

	if (m_map[now.x][now.y].canwoalk != walkable)
		return emKNEXTSTEP_RESULT_ARRIVAL;

	if (m_map[m_target.x][m_target.y].canwoalk != walkable)
		return emKNEXTSTEP_RESULT_NOANYWAY;

	if ((m_map[now.x][now.y].pathflag == m_sOpenFlag) ||
		(m_map[now.x][now.y].pathflag == m_sCloseFlag))
	{
		nextstep.x = m_map[now.x][now.y].parent_x;
		nextstep.y = m_map[now.x][now.y].parent_y;
		return emKNEXTSTEP_RESULT_SUCCESS;
	}
	
	FixOpenTable(now);
	while (!m_OpenTable.empty())
	{
		OpenNode* pNode = m_OpenTable.PopNode();
		AddCloseNode(pNode, now.x, now.y);
		delete pNode;
		
		if ((m_map[now.x][now.y].pathflag == m_sOpenFlag))
		{
			nextstep.x = m_map[now.x][now.y].parent_x;
			nextstep.y = m_map[now.x][now.y].parent_y;	
			return emKNEXTSTEP_RESULT_SUCCESS;
		}
	}
	return emKNEXTSTEP_RESULT_NOANYWAY;
}

VOID KJXPathFinder::AddOpenNode(INT x, INT y, WORD fcost, WORD hcost, INT px, INT py)
{
	if (m_map[x][y].pathflag != 0 || m_map[x][y].canwoalk != walkable)
		return;

	OpenNode* pNode = new OpenNode;
	pNode->x = x;
	pNode->y = y;
	pNode->fcost = fcost;
	pNode->hcost = hcost;
	m_OpenTable.PushNode(pNode);

	m_map[x][y].gconst = fcost - hcost;
	m_map[x][y].parent_x = px;
	m_map[x][y].parent_y = py;
	m_map[x][y].pathflag = m_sOpenFlag;
}

VOID KJXPathFinder::AddCloseNode(OpenNode* pNode, INT tx, INT ty)
{
	m_map[pNode->x][pNode->y].pathflag = m_sCloseFlag;

	for (INT y = pNode->y - 1; y <= pNode->y + 1; y++)
	for (INT x = pNode->x - 1; x <= pNode->x + 1; x++)
	{
		if (x < 0 || x >= m_nMapWidth || y < 0 || y >= m_nMapHeight)
			continue;
		if (m_map[x][y].canwoalk != walkable)
			continue;
		if (m_map[x][y].pathflag == m_sCloseFlag)
			continue;

		BYTE corner = walkable;
		if (x == pNode->x-1)
		{
			if (y == pNode->y-1)
			{				
				if (m_map[pNode->x-1][pNode->y].canwoalk == unwalkable || m_map[pNode->x][pNode->y-1].canwoalk == unwalkable)
					corner = unwalkable;
			}
			else if (y == pNode->y+1)
			{
				if (m_map[pNode->x-1][pNode->y].canwoalk == unwalkable || m_map[pNode->x][pNode->y+1].canwoalk == unwalkable) 
					corner = unwalkable; 
			}
		}
		else if (x == pNode->x+1)
		{
			if (y == pNode->y-1) 
			{
				if (m_map[pNode->x+1][pNode->y].canwoalk == unwalkable || m_map[pNode->x][pNode->y-1].canwoalk == unwalkable) 
					corner = unwalkable;
			}
			else if (y == pNode->y+1)
			{
				if (m_map[pNode->x+1][pNode->y].canwoalk == unwalkable || m_map[pNode->x][pNode->y+1].canwoalk == unwalkable)
					corner = unwalkable; 
			}
		}
		if (corner != walkable)
			continue;

		FLOAT addCost = 0;
		if (abs(x-pNode->x) == 1 && abs(y-pNode->y) == 1)
		{
			addCost = 14.0f * m_map[pNode->x][pNode->y].factor;
		}
		else
		{
			addCost = 10.0f * m_map[pNode->x][pNode->y].factor;
		}

		WORD gcost = m_map[pNode->x][pNode->y].gconst + (WORD)addCost;

		if (m_map[x][y].pathflag != m_sOpenFlag)
		{
			WORD hcost = 10*(abs(x - tx) + abs(y - ty));
			AddOpenNode(x, y, gcost+hcost, hcost, pNode->x, pNode->y);
		}
		else
		{
			if (gcost < m_map[x][y].gconst)
			{
				m_map[x][y].gconst = gcost;
				m_map[x][y].parent_x = pNode->x;
				m_map[x][y].parent_y = pNode->y;
				for (INT i = 0; i < m_OpenTable.GetSize(); i++)
				{
					OpenNode* pOpenNode = m_OpenTable.GetNode(i);
					if (pOpenNode->x == x && pOpenNode->y == y)
					{
						pOpenNode->fcost = gcost + pOpenNode->hcost;
						m_OpenTable.MakeHeap();	
						break;
					}
				}
			}
		}
	}	
}

VOID KJXPathFinder::FixOpenTable(CONST FindPathNode& now)
{
	for (INT a = 0; a < m_OpenTable.GetSize(); a++)
	{
		OpenNode* pNode = m_OpenTable.GetNode(a);
		WORD oldfcost = pNode->fcost;
		WORD oldhcost = pNode->hcost;

		pNode->hcost = 10*(abs(pNode->x - now.x) + abs(pNode->y - now.y));
		pNode->fcost = pNode->hcost + (oldfcost - oldhcost);
	}
	m_OpenTable.MakeHeap();
}
