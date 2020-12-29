#include "StdAfx.h"
#include "PageRedraw.h"

CPageRedraw::CPageRedraw() : m_pManager(nullptr)
{
	AddVirtualWnd(DUI_PAGE_REDRAW, this);
}

CPageRedraw::~CPageRedraw()
{
	RemoveVirtualWnd(DUI_PAGE_REDRAW);
}

void CPageRedraw::SetManager(CPaintManagerUI* pManager)
{
	_ASSERT(nullptr != pManager);

	m_pManager = pManager;
	m_pManager->AddNotifier(this);//���notify��Ӧ
	m_pManager->AddMessageFilter(this);//���MessageHandler��Ӧ
}

void CPageRedraw::Notify(TNotifyUI& msg)
{


}

LRESULT CPageRedraw::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	LRESULT lRes = 0;

	return lRes;
}