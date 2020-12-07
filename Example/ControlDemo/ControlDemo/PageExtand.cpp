#include "StdAfx.h"
#include "PageExtand.h"

CPageExtand::CPageExtand() : m_pManager(nullptr)
{
	AddVirtualWnd(DUI_PAGE_EXTAND, this);
}

CPageExtand::~CPageExtand()
{
	RemoveVirtualWnd(DUI_PAGE_EXTAND);

	CMenuWnd::DestroyMenu();//zm �ͷ���Դ 
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

	if (msg.sType == DUI_MSGTYPE_ITEMCLICK)//tree + tab����
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
			GetCursorPos(&stPoint);//����Menu��λ��,�����Լ����е���
			CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menu.xml"), stPoint, m_pManager, &m_MenuCheckInfo);

			// �Ȼ�ȡ�����Ȼ��Ϳ���ʹ��rootMenu�嵽���˵��ڵ������Ӳ˵��Ȼ�������ɾ������
			CMenuUI* rootMenu = pMenu->GetMenuUI();
			if (rootMenu != NULL)
			{
				CMenuElementUI* pNew = new CMenuElementUI;
				pNew->SetName(_T("Menu_Dynamic"));
				pNew->SetText(_T("lsAutoOne"));
				pNew->SetResourceText(true);//���ö����Խӿ�
				pNew->SetShowExplandIcon(true);
				pNew->SetIcon(_T("WebSit.png"));
				pNew->SetIconSize(16, 16);


				CMenuElementUI* pSubNew = new CMenuElementUI;
				pSubNew->SetText(_T("��̬�����˵�"));
				pSubNew->SetName(_T("Menu_Dynamic"));
				pSubNew->SetIcon(_T("Virus.png"));
				pSubNew->SetIconSize(16, 16);

				pNew->Add(pSubNew);
				rootMenu->Add(pNew);

				//��Ӳ˵�Ԫ�ص�ָ��λ��
				CMenuElementUI* pNew2 = new CMenuElementUI;
				pNew2->SetName(_T("Menu_Dynamic"));
				pNew2->SetText(_T("��̬����һ���˵�"));
				rootMenu->AddAt(pNew2, 2);
			}

			pMenu->ResizeMenu();
		}
		else if (msg.pSender->GetName().Compare(_T("Menu_btn")) == 0)//menu��Ƕbtn
		{
			DUI__Trace(_T("Menu��Ƕ��ť����"));//�ڵ������ġ������������ʾ����Ϣ
		}
	}
}

LRESULT CPageExtand::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	LRESULT lRes = 0;

	if (WM_MENUCLICK == uMsg)//���Menu��ĳ��Ԫ�ز�������Ϣ
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

