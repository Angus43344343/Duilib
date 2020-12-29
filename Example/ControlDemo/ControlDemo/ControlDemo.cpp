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
	CWindowWnd::SetIcon(IDI_FRAME);//����Ӧ�ó���ͼ��
	CSkinManager::GetInstance()->AddReceiver(this);// Ƥ���ӿ�
	CResourceManager::GetInstance()->LoadLanguage(_T("chinese.xml"));//Ĭ�ϼ�������
	//CResourceManager::GetInstance()->SetTextQueryInterface(this);//���������л��ص�,ʹ�ûص�ʹ�ô�������̫��

	m_objPageBase.SetManager(&m_pm);
	m_objPageSenior.SetManager(&m_pm);
	m_objPageExtand.SetManager(&m_pm);
	m_objPageActiveX.SetManager(&m_pm);
	m_objPageRedraw.SetManager(&m_pm);

	m_trayIcon.CreateTrayIcon(m_pm.GetPaintWindow(), IDI_FRAME, _T("Duilib�ؼ���ʾ"));//��������ͼ��
}

void CControlDemo::OnFinalMessage(HWND hWnd)
{ 
	m_trayIcon.DeleteTrayIcon();//��������ͼ��

	PostQuitMessage(0); //���߳��˳�
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
			GetCursorPos(&stPoint);//����Menu��λ��,�����Լ����е���
			CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menu.xml"), stPoint, &m_pm, &m_MenuCheckInfo);
		}
	}

	__super::Notify(msg);
}

LRESULT CControlDemo::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;

	if (UIMSG_TRAYICON == uMsg)//����ͼ��
	{
		if (WM_LBUTTONUP == (UINT)lParam)
		{
			BOOL bVisible = ::IsWindowVisible(m_pm.GetPaintWindow());
			::ShowWindow(m_hWnd, !bVisible ? SW_SHOW : SW_HIDE);
		}
		else if (WM_RBUTTONUP == (UINT)lParam)//�Ҽ��˵�
		{
			POINT stPoint;
			GetCursorPos(&stPoint);//����Menu��λ��,�����Լ����е���
			stPoint.y -= 200;

			CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menu.xml"), stPoint, &m_pm, &m_MenuCheckInfo);
			if (nullptr != pMenu) pMenu->ResizeMenu();// ��̬��Ӻ��������ò˵��Ĵ�С
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

BOOL CControlDemo::Receive(SkinChangedParam param)//�����ӿ�
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

//��Ҫ��
LPCTSTR CControlDemo::QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType)
{
	return NULL;
}