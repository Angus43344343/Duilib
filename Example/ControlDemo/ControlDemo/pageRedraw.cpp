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
	m_pManager->AddNotifier(this);//添加notify响应
	m_pManager->AddMessageFilter(this);//添加MessageHandler响应
}

void CPageRedraw::Notify(TNotifyUI& msg)
{


}

LRESULT CPageRedraw::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	LRESULT lRes = 0;

	return lRes;
}