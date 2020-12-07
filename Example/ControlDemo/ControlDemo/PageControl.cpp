#include "StdAfx.h"
#include "PageControl.h"
#include "PopFrame.h"

CPageBase::CPageBase() : m_pManager(nullptr)
{
	AddVirtualWnd(DUI_PAGE_BASE, this);
}

CPageBase::~CPageBase()
{
	RemoveVirtualWnd(DUI_PAGE_BASE);
}

void CPageBase::SetManager(CPaintManagerUI* pManager)
{
	_ASSERT(nullptr != pManager);

	m_pManager = pManager;
	m_pManager->AddNotifier(this);//添加notify响应
	m_pManager->AddMessageFilter(this);//添加MessageHandler响应
}

void CPageBase::Notify(TNotifyUI& msg)
{
	_ASSERT(nullptr != m_pManager);

	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)//initwindow在此进行初始设置
	{
		CRichEditUI* pobjRichEdit = static_cast<CRichEditUI*>(m_pManager->FindControl(_T("reditTest5")));//CRichEdit文本追加测试
		if (pobjRichEdit != NULL)
		{
			pobjRichEdit->AppendText(_T("这是一个怎样的故事"));
			pobjRichEdit->AppendText(_T("这是一个怎样的故事\n"));
			pobjRichEdit->AppendText(_T("这是一个怎样的故事\n"));
			pobjRichEdit->AppendText(_T("这是一个怎样的故事\n"));
		}
	}
	else if (msg.sType == DUI_MSGTYPE_LINK)//UIText
	{
		if (msg.sVirtualWnd.Compare(DUI_PAGE_BASE) != 0) return;

		if (msg.pSender->GetName().Compare(_T("textTest")) == 0)
		{
			::ShellExecute(NULL, _T("open"), (0 == msg.wParam) ? _T("www.baidu.com") : _T("www.douyu.com"), NULL, NULL, SW_SHOW);
		}
	}
	else if (msg.sType == DUI_MSGTYPE_TEXTROLLEND)//RollText
	{
		CRollTextUI* pRollText = static_cast<CRollTextUI*>(m_pManager->FindControl(_T("rolltextTest")));
		if (nullptr != pRollText)
		{
			pRollText->EndRoll();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_TEXTCHANGED)//Edit
	{
		CEditUI* pobjEditUI = static_cast<CEditUI*>(m_pManager->FindControl(_T("EditTest5")));
		if (nullptr != pobjEditUI)
		{
			//DWORD dwTextLong = pobjEditUI->GetSel();
			//pobjEditUI->SetSel(0, dwTextLong);
			//pobjEditUI->SetReplaceSel(_T("追加测试"));
			return;
		}
	}
	else if (msg.sType == DUI_MSGTYPE_RETURN)//Edit 捕抓"回车键"
	{
		CEditUI* pobjEditUI = static_cast<CEditUI*>(m_pManager->FindControl(_T("EditTest5")));
		if (nullptr != pobjEditUI)
		{
			pobjEditUI->SetText(_T(""));
		}
	}
	else if (msg.sType == DUI_MSGTYPE_DROP)//drop 功能控件都通用,只是用label和RichEdit做测试
	{
		CDuiString strName = msg.pSender->GetName();
		if (strName.Compare(_T("lblTest")) == 0)
		{
			CLabelUI* pobjLabel = static_cast<CLabelUI*>(m_pManager->FindControl(_T("lblTest")));
			if (nullptr != pobjLabel)
			{
				HDROP hDrop = (HDROP)(msg.lParam);
				TCHAR szFileName[MAX_PATH] = { 0 };
				UINT cFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
				if (cFiles > 0)
				{
					DragQueryFile(hDrop, 0, szFileName, sizeof(szFileName)); //获取拖放文件的文件名
					pobjLabel->SetBkImage(szFileName);
				}
			}
		}
		else if (strName.Compare(_T("reditTest")) == 0)
		{
			CRichEditUI* pobjRichEdit = static_cast<CRichEditUI*>(m_pManager->FindControl(_T("reditTest")));
			if (nullptr != pobjRichEdit)
			{
				HDROP hDrop = (HDROP)(msg.lParam);
				TCHAR szFileName[MAX_PATH] = { 0 };
				UINT cFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
				if (cFiles > 0)
				{
					DragQueryFile(hDrop, 0, szFileName, sizeof(szFileName)); //获取拖放文件的文件名
					ShowRichEdit(pobjRichEdit, szFileName);
				}
			}
		}
	}
	else if (msg.sType == DUI_MSGTYPE_VALUECHANGED)//Slider
	{
		CSliderUI* pobjSlider = static_cast<CSliderUI*>(m_pManager->FindControl(_T("sliderTest")));
		CProgressUI* pobjProgress = static_cast<CProgressUI*>(m_pManager->FindControl(_T("prosTest2")));
		if ((nullptr != pobjSlider) && (nullptr != pobjProgress))
		{
			pobjProgress->SetValue(pobjSlider->GetValue());
		}
	}
	else if (msg.sType == DUI_MSGTYPE_VALUECHANGED_MOVE)//Slider
	{
		CSliderUI* pobjSlider = static_cast<CSliderUI*>(m_pManager->FindControl(_T("sliderTest1")));
		CProgressUI* pobjProgress = static_cast<CProgressUI*>(m_pManager->FindControl(_T("prosTest3")));
		if ((nullptr != pobjSlider) && (nullptr != pobjProgress))
		{
			pobjProgress->SetValue(pobjSlider->GetValue());
		}
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)//Button或FadeButton
	{
		if (_tcsicmp(msg.pSender->GetName(), _T("btnTest")) == 0)
		{
			CButtonUI* pobjButton = static_cast<CButtonUI*>(m_pManager->FindControl(_T("btnTest")));
			if (nullptr != pobjButton)
			{
				pobjButton->SetText(_T("点击测试"));
			}
		}
		else if (msg.pSender->GetName().Compare(_T("fbtnTest1")) == 0)
		{
			CPopFrame* pobjPopFrame = new CPopFrame();
			ASSERT(nullptr != pobjPopFrame);

			pobjPopFrame->Create(m_pManager->GetPaintWindow(), _T("弹出窗口测试"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, 0, 0, 300, 260);
			pobjPopFrame->CenterWindow();

		}
		else if (msg.pSender->GetName().Compare(_T("fbtnTest2")) == 0)
		{
			CPopFrame* pobjPopFrame = new CPopFrame();
			ASSERT(nullptr != pobjPopFrame);

			pobjPopFrame->Create(m_pManager->GetPaintWindow(), _T("弹出窗口测试"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, 0, 0, 300, 260);
			pobjPopFrame->CenterWindow();
			pobjPopFrame->ShowModal();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_MENU)//Button
	{
		//创建右键菜单
	}
	else if (msg.sType == DUI_MSGTYPE_COLORCHANGED)//ColorPalette
	{
		CControlUI* pRoot = m_pManager->GetRoot();
		CColorPaletteUI* pobjColorPalete = static_cast<CColorPaletteUI*>(m_pManager->FindControl(_T("paletteTest")));
		if ((nullptr != pRoot) && (nullptr != pobjColorPalete))
		{
			pRoot->SetBkColor(pobjColorPalete->GetSelectColor());
		}
	}
	else if (msg.sType == DUI_MSGTYPE_CHECKCLICK)//CheckBox
	{
		CCheckBoxUI* pobjCheckBox = static_cast<CCheckBoxUI*>(m_pManager->FindControl(_T("cboxTest")));
		if (nullptr != pobjCheckBox)
		{
			if (pobjCheckBox->GetCheck())
			{
				pobjCheckBox->SetText(_T("check测试"));
			}
		}
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)//CheckBox和Option
	{
		if (msg.pSender->GetInterface(DUI_CTR_CHECKBOX))
		{
			CStdPtrArray* pobjGroup = static_cast<CStdPtrArray*>(m_pManager->GetOptionGroup(_T("gRadio")));
			if (nullptr != pobjGroup)
			{
				for (int iCount = 0; iCount < pobjGroup->GetSize(); iCount++)
				{
					CCheckBoxUI* pobjRadio = static_cast<CCheckBoxUI*>(pobjGroup->GetAt(iCount));
					if ((nullptr != pobjRadio) && (pobjRadio->IsSelected()))
					{
						pobjRadio->SetText(_T("Radio测试"));
					}
				}
			}
		}
		else if (msg.pSender->GetInterface(DUI_CTR_OPTION))
		{
			CAnimationTabLayoutUI* pobjaTabLayout = static_cast<CAnimationTabLayoutUI*>(m_pManager->FindControl(_T("aTabLayout")));
			if (nullptr != pobjaTabLayout)
			{
				if (_tcsicmp(msg.pSender->GetName(), _T("optionTest")) == 0) pobjaTabLayout->SelectItem(0);
				if (_tcsicmp(msg.pSender->GetName(), _T("optionTest1")) == 0) pobjaTabLayout->SelectItem(1);
			}
		}
	}	
	else if ((msg.sType == DUI_MSGTYPE_PREDROPDOWN) || (msg.sType == DUI_MSGTYPE_DROPDOWN))//Combo
	{
		CComboUI* pobjCombo = static_cast<CComboUI*>(m_pManager->FindControl(_T("comboTest")));
		if (nullptr != pobjCombo)
		{
			CDuiString strText = pobjCombo->GetText();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)//Combo
	{
		CComboUI* pobjCombo = static_cast<CComboUI*>(m_pManager->FindControl(_T("comboTest")));
		if (nullptr != pobjCombo)
		{
			CDuiString strText = pobjCombo->GetText();
			int iIndex = msg.wParam;
		}
	}

}

LRESULT CPageBase::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	LRESULT lRes = 0;

	if (WM_HOTKEY == uMsg)//HotKeyUI
	{
		UINT uiVirtKey = (UINT)HIWORD(lParam);
		if ((0 < (GetKeyState(VK_CONTROL) & 0x8000)) && (0 < (GetKeyState(VK_SHIFT) & 0x8000)) &&
			(uiVirtKey == 'F'))//没有小写
		{
			if (nullptr == m_pManager) return lRes;
			CControlUI* pobjControl = m_pManager->GetRoot();
			if (nullptr != pobjControl)
			{
				pobjControl->SetBkColor(0xffffffff);
			}
		}
	}

	return lRes;
}

void CPageBase::ShowRichEdit(CRichEditUI* pobjRichEdit, TCHAR* pcFileName)
{
	FILE* pFile = nullptr;
	char acFileBuf[1024] = { 0 };

	errno_t err = fopen_s(&pFile, w2a(pcFileName), "rb+");
	if ((0 == err) && (nullptr != pFile))
	{
		fseek(pFile, 0L, SEEK_END);
		int iFileLen = ftell(pFile);
		fseek(pFile, 0L, SEEK_SET);

		int iMaxFileLen = CLAMP(sizeof (acFileBuf), 0, iFileLen);
		int iReadLen = fread(acFileBuf, 1, iMaxFileLen, pFile);//如果文档存在中文,请将文档保存为ASCI类型;否则文档乱码
		fclose(pFile);

		if (0 < iReadLen)
		{
			pobjRichEdit->SetText(a2w(acFileBuf));
		}
	}
}