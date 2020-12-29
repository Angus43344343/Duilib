#include "stdafx.h"
#include "ControlDemo.h"

CControlDemo::CControlDemo()
{

}

CControlDemo::~CControlDemo()
{
}

LPCTSTR CControlDemo::GetWindowClassName(void) const 
{
	return _T("UIControlDemo"); 
}

CDuiString CControlDemo::GetSkinFile() 
{ 
	return _T("duilib.xml"); 
}

void CControlDemo::InitWindow()
{
	CWindowWnd::SetIcon(IDI_FRAME);//设置应用程序图标
	CSkinManager::GetInstance()->AddReceiver(this);// 皮肤接口
	CResourceManager::GetInstance()->LoadLanguage(_T("chinese.xml"));//默认加载中文
	//CResourceManager::GetInstance()->SetTextQueryInterface(this);//设置语言切换回调,使用回调使用代码冗余太多

	m_objPageBase.SetManager(&m_pm);
	m_objPageSenior.SetManager(&m_pm);
	m_objPageExtand.SetManager(&m_pm);
	m_objPageActiveX.SetManager(&m_pm);
	m_objPageRedraw.SetManager(&m_pm);

	m_trayIcon.CreateTrayIcon(m_pm.GetPaintWindow(), IDI_FRAME, _T("Duilib控件演示"));//创建托盘图标
}

void CControlDemo::OnFinalMessage(HWND hWnd)
{ 
	m_trayIcon.DeleteTrayIcon();//销毁托盘图标

	PostQuitMessage(0); //主线程退出
}

void CControlDemo::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)//option + animationtablayout
	{
		CAnimationTabLayoutUI* pobjTabLyout = static_cast<CAnimationTabLayoutUI*>(m_pm.FindControl(_T("aTabLayout")));
		if (msg.pSender->GetName().Compare(_T("optionControl")) == 0)
		{
			pobjTabLyout->SelectItem(0);
		}
		else if (msg.pSender->GetName().Compare(_T("optionList")) == 0)
		{
			pobjTabLyout->SelectItem(1);
		}
		else if (msg.pSender->GetName().Compare(_T("optionExtand")) == 0)
		{
			pobjTabLyout->SelectItem(2);
		}
		else if (msg.pSender->GetName().Compare(_T("optionActiveX")) == 0)
		{
			pobjTabLyout->SelectItem(3);
		}
		else if (msg.pSender->GetName().Compare(_T("optionRedraw")) == 0)
		{
			pobjTabLyout->SelectItem(4);
		}
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName().Compare(_T("btnSkin")) == 0)//skin manager
		{
			CFadeButtonUI* pobjBtnSkin = static_cast<CFadeButtonUI*>(m_pm.FindControl(_T("btnSkin")));
			if (nullptr != pobjBtnSkin)
			{
				CSkinFrame* pobjSkinFrame = new CSkinFrame(m_hWnd, pobjBtnSkin);
				pobjSkinFrame->Create(NULL, _T("skinframe"), WS_POPUP | WS_VISIBLE, WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
			}
		}
		else if (msg.pSender->GetName().Compare(_T("btnMemu")) == 0)//Menu
		{
			POINT stPoint;
			GetCursorPos(&stPoint);//设置Menu的位置,可以自己进行调整
			CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menu.xml"), stPoint, &m_pm, &m_MenuCheckInfo);
		}
	}

	__super::Notify(msg);
}

LRESULT CControlDemo::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;

	if (UIMSG_TRAYICON == uMsg)//托盘图标
	{
		if (WM_LBUTTONUP == (UINT)lParam)
		{
			BOOL bVisible = ::IsWindowVisible(m_pm.GetPaintWindow());
			::ShowWindow(m_hWnd, !bVisible ? SW_SHOW : SW_HIDE);
		}
		else if (WM_RBUTTONUP == (UINT)lParam)//右键菜单
		{
			POINT stPoint;
			GetCursorPos(&stPoint);//设置Menu的位置,可以自己进行调整
			stPoint.y -= 200;

			CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menu.xml"), stPoint, &m_pm, &m_MenuCheckInfo);
			if (nullptr != pMenu) pMenu->ResizeMenu();// 动态添加后重新设置菜单的大小
		}
	}
	else if (WM_MENUCLICK == uMsg)//Menu button
	{
		MenuCmd* pMenuCmd = (MenuCmd*)wParam;
		//CMenuElementUI* pMenuElement = (CMenuElementUI*)lParam;
		CDuiString strName = pMenuCmd->szName;

		if (strName.Compare(_T("mbtnCh")) == 0)
		{
			CResourceManager::GetInstance()->LoadLanguage(_T("chinese.xml"));
		}
		else if (strName.Compare(_T("mbtnEn")) == 0)
		{
			CResourceManager::GetInstance()->LoadLanguage(_T("english.xml"));
		}

		InvalidateRect(m_hWnd, NULL, TRUE);
		m_pm.NeedUpdate();
	}
	
	return lRes;
}

BOOL CControlDemo::Receive(SkinChangedParam param)//换肤接口
{
	CControlUI* pRoot = m_pm.FindControl(_T("vltRoot"));
	if (pRoot != NULL) 
	{
		if (param.bColor) 
		{
			pRoot->SetBkColor(param.bkcolor);
			pRoot->SetBkImage(_T(""));
		}
		else 
		{
			pRoot->SetBkColor(0);
			pRoot->SetBkImage(param.bgimage);
		}
	}

	return TRUE;
}

//不要用
LPCTSTR CControlDemo::QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType)
{
	return NULL;
}