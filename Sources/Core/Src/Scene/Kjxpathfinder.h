#pragma once

#ifndef _SERVER

#include <vector>
#include <algorithm>

struct FindPathNode
{
	INT		x;
	INT		y;
};

//-------------------------------------------------------------------------------

CONST INT		notfinished = 0;
CONST INT		NearObstacle = 10;
CONST INT		MaxListNum = 0XFFFF;
//CONST INT		RegionWidth = 16;
//CONST INT		RegionHeight = 32;
CONST INT		notStarted = 0, found = 1, nonexistent = 2;
CONST BYTE		unwalkable = 0, walkable = 1;

#pragma pack(push, 1)
class OpenNode
{
public:
	INT		x;
	INT		y;
	WORD		fcost;
	WORD		hcost;
};

struct PathNode
{
	BYTE		canwoalk;
	BYTE		pathflag;
	WORD	gconst;
	INT		parent_x;
	INT		parent_y;
	FLOAT		factor;
};
#pragma pack(pop)

class OpenNodePtr
{
public:
	OpenNodePtr(): m_pNode(NULL) {}
	OpenNodePtr(OpenNode* pNode): m_pNode(pNode) {}
	VOID operator=(OpenNode* pNode)
	{
		m_pNode = pNode;
	}
	bool operator<(const OpenNodePtr& p)const
	{
		return m_pNode->fcost > p.m_pNode->fcost;
	}
	OpenNode*	operator->()
	{
		return m_pNode;
	}
	OpenNode*	GetPtr()
	{
		return m_pNode;
	}
	OpenNode*	detch()
	{
		OpenNode* res = m_pNode;
		m_pNode = NULL;
		return res;
	}
private:
	OpenNode*		m_pNode;
};

class OpenTable
{
public:
	OpenTable(): m_pOpenTable(NULL), m_nSize(0), m_nCurSize(0)
	{
	}
	~OpenTable()
	{
		if (m_pOpenTable)
			delete [] m_pOpenTable;
		m_pOpenTable = NULL;
		m_nSize = 0;
		m_nCurSize = 0;
	}

	bool			empty()			{ return m_nCurSize == 0; }
	INT				GetSize()		{ return m_nCurSize; }
	OpenNode*		GetNode(INT i)  { return m_pOpenTable[i].GetPtr(); }

	VOID			Reset(INT nSize)
	{
		if (m_pOpenTable)
		{
			delete [] m_pOpenTable;
			m_pOpenTable = NULL;
		}
		if (nSize)
		{
			m_nSize = nSize;
			m_pOpenTable = new OpenNodePtr[m_nSize];
		}
		m_nCurSize = 0;
	}

	OpenNode*		PopNode()
	{
		if (empty())
			return NULL;
		std::pop_heap(&m_pOpenTable[0], &m_pOpenTable[m_nCurSize]);
		m_nCurSize--;
		return m_pOpenTable[m_nCurSize].detch();
	}
	VOID			PushNode(OpenNode* pNode)
	{
		if (!m_nCurSize)
		{
			m_pOpenTable[0] = pNode;
			m_nCurSize++;
			return;
		}
		else
		{
			m_pOpenTable[m_nCurSize] = pNode;
			m_nCurSize++;
			std::push_heap(&m_pOpenTable[0], &m_pOpenTable[m_nCurSize]);
		}
	}
	VOID			MakeHeap()
	{
		if (empty()) return;
		std::make_heap(&m_pOpenTable[0], &m_pOpenTable[m_nCurSize]);
	}

private:
	OpenNodePtr*	m_pOpenTable;
	INT				m_nSize;
	INT				m_nCurSize;
};

//------------------------------------------------------------------------

class KScenePlaceMapC;
class KJXPathFinder
{
public:

	KJXPathFinder();
	~KJXPathFinder();

	enum KE_NEXTSTEP_RESULT
	{
		emKNEXTSTEP_RESULT_SUCCESS,
		emKNEXTSTEP_RESULT_NOANYWAY,
		emKNEXTSTEP_RESULT_ARRIVAL,
	};

	void FixStart(int& nowx,int& nowy);
	BOOL FindPath(int,int,int,int);
	void GetPath(int,int,std::vector<FindPathNode>&);

	BOOL				Init(RECT*,KScenePlaceMapC*);

	BOOL				FindPath(FindPathNode& start, FindPathNode& target);

	INT					GetMapWidth()			{ return m_nMapWidth; }
	INT					GetMapHeight()			{ return m_nMapHeight; }

	INT					CanWalk(INT x, INT y)	{ return m_map[x][y].canwoalk == walkable; }
	FLOAT				GetFactor(INT x, INT y)	{ return m_map[x][y].factor; }

	BOOL				IsReady()
	{
		return m_bReady;
	};
	RECT*	GetLeftTop()
	{
		return &m_LeftTop;
	}

private:
	KE_NEXTSTEP_RESULT	GetNextStep(CONST FindPathNode& now, FindPathNode & nextstep);
	BOOL				CreateMap(INT w, INT h);
	VOID				ClearMap();
	BOOL				LoadMap(RECT*,KScenePlaceMapC*);
	VOID				ResetMap();
	VOID				AddOpenNode(INT x, INT y, WORD fcost, WORD hcost, INT px, INT py);
	VOID				FixOpenTable(CONST FindPathNode& now);
	VOID				AddCloseNode(OpenNode* pNode, INT tx, INT ty);
	VOID				CalcFactors();
private:
	FindPathNode		m_target;				

	INT					m_nMapWidth;
	INT					m_nMapHeight;

	OpenTable			m_OpenTable;

	PathNode**			m_map;

	BOOL				m_bReady;
	RECT				m_LeftTop;
};

extern KJXPathFinder g_JXPathFinder;

#endif

