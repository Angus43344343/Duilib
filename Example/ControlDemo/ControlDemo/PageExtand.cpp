#include "StdAfx.h"
#include "PageExtand.h"

CPageExtand::CPageExtand() : m_pManager(nullptr)
{
	AddVirtualWnd(DUI_PAGE_EXTAND, this);
}

CPageExtand::~CPageExtand()
{
	RemoveVirtualWnd(DUI_PAGE_EXTAND);

	CMenuWnd::DestroyMenu();//zm 释放资源 
}

void CPageExtand::SetManager(CPaintManagerUI* pManager)
{
	_ASSERT(nullptr != pManager);

	m_pManager = pManager;
	m_pManager->AddNotifier(this);
	m_pManager->AddMessageFilter(this);
}

void CPageExtand::Notify(TNotifyUI& msg)
{
	_ASSERT(nullptr != m_pManager);

	if (msg.sType == DUI_MSGTYPE_ITEMCLICK)//tree + tab测试
	{
		CAnimationTabLayoutUI* pobjTabLyout = static_cast<CAnimationTabLayoutUI*>(m_pManager->FindControl(_T("aTreeTabLayout")));
		if (msg.pSender->GetName().Compare(_T("childtreeTest9")) == 0)
		{
			pobjTabLyout->SelectItem(0);
		}
		else if (msg.pSender->GetName().Compare(_T("childtreeTest10")) == 0)
		{
			pobjTabLyout->SelectItem(1);
		}
		else if (msg.pSender->GetName().Compare(_T("childtreeTest11")) == 0)
		{
			pobjTabLyout->SelectItem(2);
		}
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)//click + menu
	{
		if (msg.pSender->GetName().Compare(_T("btnMenu")) == 0)
		{

			POINT stPoint;
			GetCursorPos(&stPoint);//设置Menu的位置,可以自己进行调整
			CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menu.xml"), stPoint, m_pManager, &m_MenuCheckInfo);

			// 先获取到根项，然后就可以使用rootMenu插到到菜单内的任意子菜单项，然后做添加删除操作
			CMenuUI* rootMenu = pMenu->GetMenuUI();
			if (rootMenu != NULL)
			{
				CMenuElementUI* pNew = new CMenuElementUI;
				pNew->SetName(_T("Menu_Dynamic"));
				pNew->SetText(_T("lsAutoOne"));
				pNew->SetResourceText(true);//设置多语言接口
				pNew->SetShowExplandIcon(true);
				pNew->SetIcon(_T("WebSit.png"));
				pNew->SetIconSize(16, 16);


				CMenuElementUI* pSubNew = new CMenuElementUI;
				pSubNew->SetText(_T("动态二级菜单"));
				pSubNew->SetName(_T("Menu_Dynamic"));
				pSubNew->SetIcon(_T("Virus.png"));
				pSubNew->SetIconSize(16, 16);

				pNew->Add(pSubNew);
				rootMenu->Add(pNew);

				//添加菜单元素到指定位置
				CMenuElementUI* pNew2 = new CMenuElementUI;
				pNew2->SetName(_T("Menu_Dynamic"));
				pNew2->SetText(_T("动态定向一级菜单"));
				rootMenu->AddAt(pNew2, 2);
			}

			pMenu->ResizeMenu();
		}
		else if (msg.pSender->GetName().Compare(_T("Menu_btn")) == 0)//menu内嵌btn
		{
			DUI__Trace(_T("Menu内嵌按钮测试"));//在调试区的“输出”区域显示此信息
		}
	}
}

LRESULT CPageExtand::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	LRESULT lRes = 0;

	if (WM_MENUCLICK == uMsg)//点击Menu的某个元素产生此消息
	{
		MenuCmd* pMenuCmd = (MenuCmd*) wParam;
		CMenuElementUI* pMenuElement = (CMenuElementUI*)lParam;

		CDuiString strName = pMenuCmd->szName;

		if (strName.Compare(_T("Menu_Test2")) == 0)
		{
			DUI__Trace(pMenuCmd->szText);
		}
		else if (strName.Compare(_T("Menu_Test6")) == 0)
		{
			DUI__Trace(pMenuCmd->szText);
		}
	}

	return lRes;
}

