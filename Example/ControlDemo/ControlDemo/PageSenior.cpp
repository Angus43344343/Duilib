#include "StdAfx.h"
#include "PageSenior.h"

CDuiString g_astrList[][4] = { { _T("0"), _T("����"), _T("��"), _T("65") }, { _T("1"), _T("����"), _T("Ů"), _T("58") },
							   { _T("2"), _T("����"), _T("��"), _T("78") }, { _T("3"), _T("����"), _T("��"), _T("84") },
							   { _T("4"), _T("����"), _T("Ů"), _T("82") }, { _T("5"), _T("����"), _T("��"), _T("100") },
							   { _T("6"), _T("�پ�"), _T("��"), _T("90") }, { _T("7"), _T("��ʮ"), _T("Ů"), _T("65") },
							   { _T("8"), _T("�پ�"), _T("��"), _T("90") }, { _T("9"), _T("��ʮ"), _T("Ů"), _T("65") }, 
							   { _T("10"), _T("�پ�"), _T("��"), _T("90") }, { _T("11"), _T("��ʮ"), _T("Ů"), _T("65") }, 
							   { _T("12"), _T("�پ�"), _T("��"), _T("90") }, { _T("13"), _T("��ʮ"), _T("Ů"), _T("65") }, 
							   { _T("14"), _T("�پ�"), _T("��"), _T("90") }, { _T("15"), _T("��ʮ"), _T("Ů"), _T("65") }, 
							   { _T("16"), _T("�پ�"), _T("��"), _T("90") }, { _T("17"), _T("{a}www.baidu.com{/a} {a}www.douyu.com{/a} {a}www.sojson.com/rgb.html{/a}"), _T("Ů"), _T("65") } };

CPageSenior::CPageSenior() : m_pManager(nullptr)
{
	AddVirtualWnd(DUI_PAGE_SENIOR, this);
}

CPageSenior::~CPageSenior()
{
	RemoveVirtualWnd(DUI_PAGE_SENIOR);

	//CDuiString����һ��MAX_LOCAL_STRING_LEN:63�ĳ�Ա����,����ʱ������·����ڴ�ռ�,���������ͷ�
	//�����ֶ��ͷ�����Ϊȫ�ֵ��ڴ�ռ��ͷ��ڳ��������,_CrtDumpMemoryLeaks��ⲻ��,Ϊ�˱����������ڴ�й©�ĸ���,�������ֶ��ͷ�
	for (int iRow = 0; iRow < sizeof (g_astrList) / sizeof (g_astrList[0]); iRow++)
	{
		for (int iColumn = 0; iColumn < sizeof (g_astrList[0]) / sizeof (g_astrList[0][0]); iColumn++)
		{
			g_astrList[iRow][iColumn].Empty();
		}
	}
}

void CPageSenior::SetManager(CPaintManagerUI* pManager)
{
	_ASSERT(nullptr != pManager);

	m_pManager = pManager;
	m_pManager->AddNotifier(this);//���Notify��Ӧ
	m_pManager->AddMessageFilter(this);//���MessageHandler��Ӧ
}

void CPageSenior::Notify(TNotifyUI& msg)
{
	_ASSERT(nullptr != m_pManager);

	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)//�ؼ���ʼ��
	{
		CListUI* pobjList = static_cast<CListUI*>(m_pManager->FindControl(_T("listTest")));
		if (nullptr != pobjList)
		{
			for (int iRow = 0; iRow < sizeof (g_astrList) / sizeof (g_astrList[0]); iRow++)
			{
				CListTextElementUI* pobjListItem = new CListTextElementUI();
				pobjList->Add(pobjListItem);
				for (int iColumn = 0; iColumn < sizeof (g_astrList[0]) / sizeof (g_astrList[0][0]); iColumn++)
				{
					pobjListItem->SetText(iColumn, g_astrList[iRow][iColumn]);
				}
			}
		}

		CListUI* pobjList1 = static_cast<CListUI*>(m_pManager->FindControl(_T("listTest1")));
		if (nullptr != pobjList1)
		{
			for (int iRow = 0; iRow < sizeof (g_astrList) / sizeof (g_astrList[0]); iRow++)
			{
				CListTextElementUI* pobjListItem = new CListTextElementUI();
				pobjList1->Add(pobjListItem);
				for (int iColumn = 0; iColumn < sizeof (g_astrList[0]) / sizeof (g_astrList[0][0]); iColumn++)
				{
					pobjListItem->SetText(iColumn, g_astrList[iRow][iColumn]);

					if ((17 == iRow) && (1 == iColumn))//����ָ����LINKʹ��
					{
						pobjListItem->SetName(_T("linkItem"));
					}
				}
			}

			pobjList1->SetTextCallback(this);//���ListTextElement�ص�
		}

		CListExUI* pobjListEx1 = static_cast<CListExUI*>(m_pManager->FindControl(_T("listExTest1")));
		pobjListEx1->InitListCtrl();//��Ҫ��ʼ���ؼ�����Ϣ��Ϣ
		if (nullptr != pobjListEx1)
		{
			for (int iRow = 0; iRow < sizeof (g_astrList) / sizeof (g_astrList[0]); iRow++)
			{
				CListTextExElementUI* pobjListExItem = new CListTextExElementUI();
				pobjListEx1->Add(pobjListExItem);
				pobjListExItem->SetAttribute(_T("style"), _T("styleListCheck"));
				for (int iColumn = 0; iColumn < sizeof (g_astrList[0]) / sizeof (g_astrList[0][0]); iColumn++)
				{
					pobjListExItem->SetText(iColumn, g_astrList[iRow][iColumn]);
				}
			}

			pobjListEx1->SetTextCallback(this);//���ListTextElement�ص�
			pobjListEx1->SetTextArrayCallback(this);//���Combo�ص�,��������ComboԪ����Ϣ
		}

	}
	else if (msg.sType == DUI_MSGTYPE_ITEMCLICK)//list/listEx ����
	{
		if (msg.pSender->GetParent()->GetParent()->GetName().Compare(_T("listTest1")) == 0)
		{
			CListTextElementUI* pElement = static_cast<CListTextElementUI*>(msg.pSender->GetInterface(DUI_CTR_LISTTEXTELEMENT));
			if (NULL != pElement)
			{
				if ((0 < (int)msg.wParam) && (4 == (int)msg.wParam))
				{
					pElement->SetTextColor((int)msg.lParam, 0xFFFF1493);
				}
			}
		}
		else if (msg.pSender->GetParent()->GetParent()->GetName().Compare(_T("listExTest1")) == 0)
		{
			CListTextExElementUI* pElement = static_cast<CListTextExElementUI*>(msg.pSender->GetInterface(DUI_CTR_LISTTEXTELEMENT_EX));
			if ((nullptr != pElement) && (2 == (int)msg.lParam)) pElement->SetTextColor((int)msg.lParam, 0xFFFF1493);
		}
		else if (msg.pSender->GetParent()->GetParent()->GetName().Compare(_T("listExTest2")) == 0)
		{
			CListContainerElementUI* pobjElement = static_cast<CListContainerElementUI*>(msg.pSender->GetInterface(DUI_CTR_LISTCONTAINERELEMENT));
			if (nullptr != pobjElement)
			{
				CControlUI* pControl = pobjElement->FindSubControl(_T("lblList"));
				if (nullptr != pControl)
				{
					CLabelUI* pobjLabel = static_cast<CLabelUI*>(pControl->GetInterface(DUI_CTR_LABEL));
					if (nullptr != pobjLabel)
					{
						pobjLabel->SetText(_T("��Ҫһ����"));
					}
				}
			}
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMACTIVATE)//list/listEx ˫�������"�س�"��
	{
		if (msg.pSender->GetParent()->GetParent()->GetName().Compare(_T("listTest1")) == 0)
		{
			CListTextElementUI* pElement = static_cast<CListTextElementUI*>(msg.pSender->GetInterface(DUI_CTR_LISTTEXTELEMENT));
			if (nullptr != pElement)
			{
				pElement->SetItemColor((int)msg.lParam, 0xFFBBFFFF);
			}
		}
		else if (msg.pSender->GetParent()->GetParent()->GetName().Compare(_T("listExTest1")) == 0)
		{
			CListTextExElementUI* pElement = static_cast<CListTextExElementUI*>(msg.pSender->GetInterface(DUI_CTR_LISTTEXTELEMENT_EX));
			if ((nullptr != pElement) && (0 != (int)msg.lParam))
			{
				pElement->SetColumItemColor((int)msg.lParam, 0xFFBBFFFF);
			}
		}
	}
	else if (msg.sType == DUI_MSGTYPE_LISTHEADERCLICK) //ListHeader/listExHeader ���� 
	{
		static bool s_bCompare = false;
		if (msg.pSender->GetParent()->GetName().Compare(_T("lheaderTest1")) == 0)
		{
			CListUI* pobjList1 = static_cast<CListUI*>(m_pManager->FindControl(_T("listTest1")));
			if (nullptr != pobjList1)
			{
				s_bCompare = !s_bCompare;

				if (s_bCompare)
					pobjList1->SortItems(CompareItemHigh, (int)(msg.lParam));//������ͷʵ��������
				else
					pobjList1->SortItems(CompareItemLow, (int)(msg.lParam));
			}
		}
		else if (msg.pSender->GetParent()->GetName().Compare(_T("lheaderTest2")) == 0)
		{
			CListExUI* pobjListEx1 = static_cast<CListExUI*>(m_pManager->FindControl(_T("listExTest1")));
			if ((nullptr != pobjListEx1) && (0 != (int)msg.lParam))
			{
				s_bCompare = !s_bCompare;

				if (s_bCompare)
					pobjListEx1->SortItems(CompareItemHigh, (int)(msg.lParam));
				else
					pobjListEx1->SortItems(CompareItemLow, (int)(msg.lParam));
			}
		}
	}
	else if (msg.sType == DUI_MSGTYPE_LINK)//ListTextElement
	{
		if (msg.sVirtualWnd.Compare(DUI_PAGE_SENIOR) != 0) return;

		CDuiString strName = msg.pSender->GetName();
		if (strName.Compare(_T("linkItem")) == 0)//��ʼ��ָ���ض�ITEM����
		{
			if (0 == (int)msg.wParam)
			{
				::ShellExecute(NULL, _T("open"), _T("www.baidu.com"), NULL, NULL, SW_SHOW);
			}
			else if (1 == (int)msg.wParam)
			{
				::ShellExecute(NULL, _T("open"), _T("www.douyu.com"), NULL, NULL, SW_SHOW);
			}
			else if (2 == (int)msg.wParam)
			{
				::ShellExecute(NULL, _T("open"), _T("https://www.sojson.com/rgb.html"), NULL, NULL, SW_SHOW);
			}
		}
	}

}

LRESULT CPageSenior::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	LRESULT lRes = 0;


	return lRes;
}

LPCTSTR CPageSenior::GetItemText(CControlUI* pList, int iItem, int iSubItem)
{
	_ASSERT(nullptr != pList);

	if (pList->GetParent()->GetParent()->GetName().Compare(_T("listTest1")) == 0)
	{
		CListTextElementUI* pobjTextItem = (CListTextElementUI*)pList;
		if (0 == iSubItem) //iItem��Ӧ"��", iSubItem��Ӧ"��" 
		{
			/*�����������д���,��Ϊ��������ʱ,����ߵ�����ܱ��ֲ���*/
			CDuiString strIndex;
			strIndex.Format(_T("%d"), iItem);
			pobjTextItem->SetText(iSubItem, strIndex);
		}

		return pobjTextItem->GetText(iSubItem);
	}
	else if (pList->GetParent()->GetParent()->GetName().Compare(_T("listExTest1")) == 0)
	{
		CListTextExElementUI* pobjTextExItem = (CListTextExElementUI*)pList;
		return pobjTextExItem->GetText(iSubItem);
	}

	return NULL;
}

DWORD CPageSenior::GetItemTextColor(CControlUI* pList, int iItem, int iSubItem, int iState)
{
	_ASSERT(nullptr != pList);

	DWORD dwTextColor = 0;
	if (pList->GetParent()->GetParent()->GetName().Compare(_T("listTest1")) == 0)
	{
		CListTextElementUI* pobjTextItem = (CListTextElementUI*)pList;
		dwTextColor = ((0 == iState) && (1 == iSubItem)) ? 0xFFE8E8E8 : pobjTextItem->GetTextColor(iSubItem);
	}
	else if (pList->GetParent()->GetParent()->GetName().Compare(_T("listExTest1")) == 0)
	{
		CListTextExElementUI* pobjTextExItem = (CListTextExElementUI*)pList;
		dwTextColor = (3 == iSubItem) ? 0xFF90EE90 : pobjTextExItem->GetTextColor(iSubItem);
	}

	return dwTextColor;
}

/*�ȶԺ���:���ַ����ȶԾ���,�û����Ը�������д�Լ��ıȶԺ���*/
int CALLBACK CPageSenior::CompareItemHigh(UINT_PTR item1Addr, UINT_PTR item2Addr, UINT_PTR CompareData)//����
{
	CControlUI* pobjControl1 = (CControlUI*)(item1Addr);
	CControlUI* pobjControl2 = (CControlUI*)(item2Addr);
	CListTextElementUI* pobjItem1 = static_cast<CListTextElementUI*>(pobjControl1->GetInterface(DUI_CTR_LISTELEMENT));
	CListTextElementUI* pobjItem2 = static_cast<CListTextElementUI*>(pobjControl2->GetInterface(DUI_CTR_LISTELEMENT));

	_ASSERT((NULL != pobjItem1) && (NULL != pobjItem2));

	return _tcsicmp(pobjItem1->GetText(CompareData), pobjItem2->GetText(CompareData));
}

int CALLBACK CPageSenior::CompareItemLow(UINT_PTR item1Addr, UINT_PTR item2Addr, UINT_PTR CompareData)//����
{
	CControlUI* pobjControl1 = (CControlUI*)(item1Addr);
	CControlUI* pobjControl2 = (CControlUI*)(item2Addr);
	CListTextElementUI* pobjItem1 = static_cast<CListTextElementUI*>(pobjControl1->GetInterface(DUI_CTR_LISTELEMENT));
	CListTextElementUI* pobjItem2 = static_cast<CListTextElementUI*>(pobjControl2->GetInterface(DUI_CTR_LISTELEMENT));

	_ASSERT((NULL != pobjItem1) && (NULL != pobjItem2));

	return -_tcsicmp(pobjItem1->GetText(CompareData), pobjItem2->GetText(CompareData));
}
 
void CPageSenior::GetItemComboTextArray(CControlUI* pCtrl, int iItem, int iSubItem)
{
	CDuiString astrInfo[] = {_T("����"), _T("����"), _T("����")};
	CComboBoxUI* pobjComboBox = (CComboBoxUI*)pCtrl;

	pobjComboBox->SetAttribute(_T("style"), _T("styleListCombo"));

	for (int iCount = 0; iCount < sizeof (astrInfo) / sizeof (astrInfo[0]); iCount++)
	{
		CListLabelElementUI* pobjElement = new CListLabelElementUI();
		
		pobjElement->SetText(astrInfo[iCount]);

		pobjComboBox->Add(pobjElement);
	}
}
