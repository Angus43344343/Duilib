#include "stdafx.h"
#include "SkinFrame.h"

LPCTSTR CSkinFrame::GetWindowClassName(void) const
{
	return _T("CSkinFrame");
}

CDuiString CSkinFrame::GetSkinFile()
{
	return _T("skin.xml");
}

void CSkinFrame::InitWindow()
{
	RECT rcBtnSkin = m_pobjControl->GetPos();
	SIZE szWnd = m_pm.GetInitSize();
	POINT ptWnd = { 0 };
	ptWnd.x = rcBtnSkin.right - szWnd.cx;
	ptWnd.y = rcBtnSkin.bottom;
	::ClientToScreen(m_hParentWnd, &ptWnd);

	SetWindowPos(m_hWnd, NULL, ptWnd.x, ptWnd.y, szWnd.cx, szWnd.cy, SWP_NOSIZE | SWP_NOACTIVATE);
}

void CSkinFrame::OnFinalMessage(HWND hWnd)
{
	delete this;
}

CSkinManager CSkinManager::m_objSkinManager;
void CSkinFrame::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)//option + animationtablayout
	{
		CAnimationTabLayoutUI* pobjTabLyout = static_cast<CAnimationTabLayoutUI*>(m_pm.FindControl(_T("aTabSkinLayout")));
		if (msg.pSender->GetName().Compare(_T("optBkImage")) == 0)
		{
			pobjTabLyout->SelectItem(0);
		}
		else if (msg.pSender->GetName().Compare(_T("optBkColor")) == 0)
		{
			pobjTabLyout->SelectItem(1);
		}
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName().Compare(_T("fbtnImage")) == 0)
		{
			SkinChangedParam skin;
			skin.bColor = false;
			skin.bgimage = msg.pSender->GetUserData();
			CSkinManager::GetInstance()->Broadcast(skin);
		}
		else if (msg.pSender->GetName().Compare(_T("fbtnColor")) == 0)
		{
			SkinChangedParam skin;
			skin.bColor = true;
			skin.bkcolor = msg.pSender->GetBkColor();
			CSkinManager::GetInstance()->Broadcast(skin);
		}
	}

	__super::Notify(msg);
}

LRESULT CSkinFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;

	return lRes;
}

LRESULT CSkinFrame::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	Close(0);
	return 0;
}