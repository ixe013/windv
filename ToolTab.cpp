// ToolTab.cpp : implementation file
//

#include "stdafx.h"
#include "WinDV.h"
#include "ToolTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTab

CToolTab::CToolTab()
{
}

CToolTab::~CToolTab()
{
}


void CToolTab::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	int s = SaveDC(lpDrawItemStruct->hDC);
	lpDrawItemStruct->rcItem.top += ::GetSystemMetrics(SM_CYEDGE)+1;
	TCITEM item;
	char buf[256];
	item.mask = TCIF_TEXT;
	item.cchTextMax = sizeof buf / sizeof buf[0];
	item.pszText = buf;
	GetItem(lpDrawItemStruct->itemID, &item);
	HBRUSH hbr = (HBRUSH)GetParent()->SendMessage(WM_CTLCOLORDLG, (WPARAM)(lpDrawItemStruct->hDC), (LPARAM)m_hWnd);
	FillRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, hbr);
	DrawText(lpDrawItemStruct->hDC, buf, -1, &lpDrawItemStruct->rcItem, DT_CENTER);
	RestoreDC(lpDrawItemStruct->hDC, s);
}
