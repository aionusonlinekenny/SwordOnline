 /* 
 * File:     UiTaskNote.cpp
 * Desc:     任务记事窗口
 * Author:   flying
 * Creation: 2003/7/18
 * 好多好多的mm哦，咔咔，以后每次都要去
 */
//-----------------------------------------------------------------------------
#pragma once

#include "KWin32.h"
#include "KIniFile.h"
#include <time.h>
#include "../Elem/wnds.h"
#include "../Elem/WndMessage.h"
#include "../UiSoundSetting.h"
#include "UiTaskNote.h"
#include "UiTaskDataFile.h"
#include "../../Represent/iRepresent/iRepresentShell.h"
#include "../../../core/src/CoreShell.h"
#include "../../../core/src/gamedatadef.h"
#include "../../../Engine/src/Text.h"
#include "../../../Engine/src/KDebug.h"
#include "../UiBase.h"

extern iRepresentShell*	g_pRepresentShell;

#define 	SCHEME_INI_TASKNOTE			"UiTaskNote.ini"
#define 	SCHEME_INI_SYSTEM			"UiTaskNote_System.ini"
#define 	SCHEME_INI_PERSONAL			"UiTaskNote_Personal.ini"
#define 	SCHEME_INI_EVENT			"UiTaskNote_Event.ini"
#define 	SCHEME_INI_BIND				"UiTaskNote_Bind.ini"

//////////////////////////////////////////////////////////////////////////
// Editor
//////////////////////////////////////////////////////////////////////////
KTaskEdit::KTaskEdit()
{
	m_Text[0] = m_Text[sizeof(m_Text) - 1] = 0;
	SetTextPtr(m_Text, sizeof(m_Text));
}

//////////////////////////////////////////////////////////////////////////
// Page: Personal
//////////////////////////////////////////////////////////////////////////

void KUiTaskNote_Personal::Initialize()
{
	AddChild(&m_BtnSave);
	AddChild(&m_Editor);
	AddChild(&m_ScrollBar);
}

void KUiTaskNote_Personal::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_PERSONAL);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
		m_BtnSave.Init(&Ini, "SaveBtn");
		m_Editor.Init(&Ini, "Editor");
		m_ScrollBar.Init(&Ini, "EditorScroll");
	}
}

void KUiTaskNote_Personal::UpdateView()
{
	char	cBuffer[2048];
	if (KTaskDataFile::GetPersonalRecord(cBuffer, sizeof(cBuffer)))
		m_Editor.SetText(cBuffer);
	else
		m_Editor.ClearText(false);
}

void KUiTaskNote_Personal::UpdateData()
{
	OnSave();
}

int KUiTaskNote_Personal::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nResult = false;
	if (uMsg ==  WND_N_BUTTON_CLICK &&
		uParam == (unsigned int)(KWndWindow*)&m_BtnSave)
	{
		OnSave();
		nResult = true;
	}
	else
	{
		nResult = KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return nResult;
}

void KUiTaskNote_Personal::Show()
{
	Wnd_SetFocusWnd(&m_Editor);
	KWndPage::Show();		
}

void KUiTaskNote_Personal::Hide()
{
	Wnd_SetFocusWnd(NULL);
	KWndPage::Hide();
}

// Save information to file.
void KUiTaskNote_Personal::OnSave()
{
	char szRecord[2048];
	int nLen = m_Editor.GetText(szRecord, sizeof(szRecord), false);
	KTaskDataFile::SetPersonalRecord(szRecord, nLen);
}

//////////////////////////////////////////////////////////////////////////
// Page: System
//////////////////////////////////////////////////////////////////////////
// constructor

void KUiTaskNote_System::Initialize()
{
	AddChild(&m_BtnDelete);
	AddChild(&m_RecordList);
}

void KUiTaskNote_System::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_SYSTEM);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
		m_BtnDelete.Init(&Ini, "DeleteBtn");
		m_RecordList.Init(&Ini, "List");
	}
}

int KUiTaskNote_System::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nResult = 0;
	switch (uMsg)
	{
	case WND_N_LIST_ITEM_SEL:
		if (uParam == (unsigned int)(KWndWindow*)&m_RecordList)
			m_BtnDelete.Enable(nParam >= 0);
		nResult = 1;
		break;
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_BtnDelete)
		{
			OnDelete(m_RecordList.GetMessageListBox()->GetCurSel());
			nResult = 1;
		}
		break;
	default:
		nResult = KWndPage::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nResult;
}

void KUiTaskNote_System::OnDelete(int nIndex)
{
	if (KTaskDataFile::RemoveSystemRecord(nIndex))
	{
		m_RecordList.GetMessageListBox()->RemoveAMessage(nIndex);
		if (nIndex >= m_RecordList.GetMessageListBox()->GetMsgCount())
		{
			nIndex--;
		}
		m_RecordList.GetMessageListBox()->SetCurSel(nIndex);
	}
}

void KUiTaskNote_System::UpdateView()
{
	m_RecordList.GetMessageListBox()->Clear();
	int nCount = KTaskDataFile::GetSystemRecordCount();
	if (nCount > 0)
	{
		m_RecordList.GetMessageListBox()->SetCapability(nCount);	//+4是多留些空间
		for (int i = 0; i < nCount; i++)
		{
			const TASK_SYSTEM_RECORD* pRecord = KTaskDataFile::GetSystemRecord(i);
			if (pRecord == NULL)
				break;
			m_RecordList.GetMessageListBox()->AddOneMessage(pRecord->cBuffer, pRecord->nContentLen);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Page: Event
//////////////////////////////////////////////////////////////////////////
// constructor

void KUiTaskNote_Event::Initialize()
{

}

void KUiTaskNote_Event::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_EVENT);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
	}
}

int KUiTaskNote_Event::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nResult = 0;
	switch (uMsg)
	{
	case WND_N_LIST_ITEM_SEL:
		break;
	case WND_N_BUTTON_CLICK:
		break;
	default:
		nResult = KWndPage::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nResult;
}


//////////////////////////////////////////////////////////////////////////
// Page: Bind
//////////////////////////////////////////////////////////////////////////
// constructor
void KUiTaskNote_Bind::Initialize()
{
	AddChild(&m_BtnDelete);
	AddChild(&m_RecordList);
}

void KUiTaskNote_Bind::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_SYSTEM);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
		m_BtnDelete.Init(&Ini, "DeleteBtn");
		m_RecordList.Init(&Ini, "List");
	}
}

int KUiTaskNote_Bind::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nResult = 0;
	switch (uMsg)
	{
	case WND_N_LIST_ITEM_SEL:
		if (uParam == (unsigned int)(KWndWindow*)&m_RecordList)
			m_BtnDelete.Enable(nParam >= 0);
		nResult = 1;
		break;
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_BtnDelete)
		{
			OnDelete(m_RecordList.GetMessageListBox()->GetCurSel());
			nResult = 1;
		}
		break;
	default:
		nResult = KWndPage::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nResult;
}

void KUiTaskNote_Bind::OnDelete(int nIndex)
{
	if (KTaskDataFile::RemoveBindRecord(nIndex))
	{
		m_RecordList.GetMessageListBox()->RemoveAMessage(nIndex);
		if (nIndex >= m_RecordList.GetMessageListBox()->GetMsgCount())
		{
			nIndex--;
		}
		m_RecordList.GetMessageListBox()->SetCurSel(nIndex);
	}
}

void KUiTaskNote_Bind::UpdateView()
{
	m_RecordList.GetMessageListBox()->Clear();
	int nCount = KTaskDataFile::GetBindRecordCount();
	if (nCount > 0)
	{
		m_RecordList.GetMessageListBox()->SetCapability(nCount);	//+4是多留些空间
		for (int i = 0; i < nCount; i++)
		{
			const TASK_BIND_RECORD* pRecord = KTaskDataFile::GetBindRecord(i);
			if (pRecord == NULL)
				break;
			m_RecordList.GetMessageListBox()->AddOneMessage(pRecord->cBuffer, pRecord->nContentLen);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//  The main class of this file: KUiTaskNote.
//////////////////////////////////////////////////////////////////////////
KUiTaskNote*	KUiTaskNote::m_pSelf = NULL;

// constructor. 
KUiTaskNote::KUiTaskNote()
{
}

// destructor
KUiTaskNote::~KUiTaskNote()
{
}

// open
KUiTaskNote* KUiTaskNote::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTaskNote;
		if (m_pSelf)
		{
			m_pSelf->Initialize();			
			m_pSelf->InitData();
		}
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

// ....
void KUiTaskNote::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->Hide();
		if (bDestroy)
		{
			m_pSelf->SaveData();
			KTaskDataFile::ClearAll();
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}

// is it visible?
KUiTaskNote* KUiTaskNote::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

void KUiTaskNote::InitData()
{
	KTaskDataFile::LoadData();
	m_PersonalPad.UpdateView();
	m_SystemPad.UpdateView();
	m_BindPad.UpdateView();	
}

//保存数据
void KUiTaskNote::SaveData()
{
	m_PersonalPad.UpdateData();
	KTaskDataFile::SaveData();
}

// Load the face
void KUiTaskNote::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_TASKNOTE);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_BtnSystem.Init(&Ini, "SystemBtn");
			m_pSelf->m_BtnPersonal.Init(&Ini, "PersonalBtn");
			m_pSelf->m_BtnEvent.Init(&Ini, "BulletinBtn");
			m_pSelf->m_BtnBind.Init(&Ini, "ItemBindBtn");			
			m_pSelf->m_BtnClose.Init(&Ini, "CloseBtn");

			m_pSelf->m_SystemPad.LoadScheme(pScheme);
			m_pSelf->m_PersonalPad.LoadScheme(pScheme);
			m_pSelf->m_EventPad.LoadScheme(pScheme);
			m_pSelf->m_BindPad.LoadScheme(pScheme);			
		}
	}
}

// Initialize
void KUiTaskNote::Initialize()
{
	m_EventPad.Initialize();
	AddPage(&m_EventPad, &m_BtnEvent);
	m_SystemPad.Initialize();
	AddPage(&m_SystemPad, &m_BtnSystem);
	m_PersonalPad.Initialize();
	AddPage(&m_PersonalPad, &m_BtnPersonal);
	m_BindPad.Initialize();
	AddPage(&m_BindPad, &m_BtnBind);
	
	AddChild(&m_BtnClose);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, sizeof(Scheme));
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

// Deal with the window messages.
int KUiTaskNote::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nResult = false;
	if (uMsg == WND_N_BUTTON_CLICK)
	{
		if (uParam == (unsigned int)(KWndWindow*)&m_BtnClose)
		{
			OnClose();
			nResult = true;
		}
		else
		{
			if (uParam == (unsigned int)(KWndWindow*)&m_BtnEvent)
				m_EventPad.Show();
			nResult = KWndPageSet::WndProc(uMsg, uParam, nParam);
		}
	}
	else
	{
		nResult = KWndPageSet::WndProc(uMsg, uParam, nParam);
	}
	return nResult;
}

void KUiTaskNote::OnClose()
{
	CloseWindow(true);
}

void KUiTaskNote::WakeUp(BYTE btType, const char* pRecord, int nLen, unsigned int uValue)
{
	if (nLen > 0)
	{
		if (m_pSelf == NULL)
			KTaskDataFile::LoadData();
		
		bool bsInserted	= false;
		bool bbInserted	= false;
		switch (btType)
		{
		case 1:
			TASK_SYSTEM_RECORD sRecord;
			sRecord.tTime = time(&sRecord.tTime);
			sRecord.uReserved = uValue;
			sRecord.nContentLen = nLen;
			sRecord.pBuffer = pRecord;
			bsInserted = KTaskDataFile::InsertSystemRecord(&sRecord);		
			break;
		case 2:
			TASK_BIND_RECORD bRecord;
			bRecord.tTime = time(&bRecord.tTime);
			bRecord.uReserved = uValue;
			bRecord.nContentLen = nLen;
			bRecord.pBuffer = pRecord;
			bbInserted = KTaskDataFile::InsertBindRecord(&bRecord);		
			break;
		default:
			break;
		}
		
		if (m_pSelf == NULL)
		{
			if (bsInserted || bbInserted)
				KTaskDataFile::SaveData();
			KTaskDataFile::ClearAll();
		}
		else if (bsInserted)
		{
			m_pSelf->m_SystemPad.UpdateView();
		}
		else if (bbInserted)
		{
			m_pSelf->m_BindPad.UpdateView();
		}		
	}
}