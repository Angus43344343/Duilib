#include "StdAfx.h"
#include "PageActiveX.h"

CPageActiveX::CPageActiveX() : m_pManager(nullptr)
{
	AddVirtualWnd(DUI_PAGE_ACTIVEX, this);
}

CPageActiveX::~CPageActiveX()
{
	RemoveVirtualWnd(DUI_PAGE_ACTIVEX);
}

void CPageActiveX::SetManager(CPaintManagerUI* pManager)
{
	_ASSERT(nullptr != pManager);

	m_pManager = pManager;
	m_pManager->AddNotifier(this);
	m_pManager->AddMessageFilter(this);
}

void CPageActiveX::Notify(TNotifyUI& msg)
{
	_ASSERT(nullptr != m_pManager);

	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)//init
	{
		//WebBrowser初始化
		CWebBrowserUI* pWebBrowser = static_cast<CWebBrowserUI*>(m_pManager->FindControl(_T("webTest")));
		if (nullptr != pWebBrowser)
		{
			pWebBrowser->SetWebBrowserEventHandler(this);
			pWebBrowser->NavigateUrl(_T("www.baidu.com"));
		}
	}
	else if (msg.sType == DUI_MSGTYPE_SHOWACTIVEX)//active
	{
		if (msg.pSender->GetName().Compare(_T("flashTest")) == 0)
		{
			IShockwaveFlash* pFlash = NULL;
			CActiveXUI* pActiveX = static_cast<CActiveXUI*>(msg.pSender);
			pActiveX->GetControl(__uuidof(IShockwaveFlash), (void**)&pFlash);
			if (pFlash != NULL)  
			{
				pFlash->put_WMode(_bstr_t(_T("Transparent")));
				pFlash->put_Movie(_bstr_t(CPaintManagerUI::GetInstancePath() + _T("\\skin\\flash\\1.swf")));
				//pFlash->DisableLocalSecurity();
				//pFlash->put_AllowScriptAccess(L"always");
				//pFlash->Play();
				pFlash->Release();
			}
		}
		else if (msg.pSender->GetName().Compare(_T("flashTest1")) == 0)
		{
			IShockwaveFlash* pFlash = NULL;
			CActiveXUI* pActiveX = static_cast<CActiveXUI*>(msg.pSender);
			pActiveX->GetControl(__uuidof(IShockwaveFlash), (void**)&pFlash);
			if (pFlash != NULL)
			{
				pFlash->put_WMode(_bstr_t(_T("Transparent")));
				pFlash->put_Movie(_bstr_t(CPaintManagerUI::GetInstancePath() + _T("\\skin\\flash\\1.swf")));
				//pFlash->DisableLocalSecurity();
				//pFlash->put_AllowScriptAccess(L"always");
				//pFlash->Play();
				pFlash->Release();
			}
		}
		else if (msg.pSender->GetName().Compare(_T("flashTest2")) == 0)
		{

			IShockwaveFlash* pFlash = NULL;
			CFlashUI* pActiveX = static_cast<CFlashUI*>(msg.pSender);
			pActiveX->SetFlashEventHandler(this);
			pActiveX->GetControl(__uuidof(IShockwaveFlash), (void**)&pFlash);
			if (pFlash != NULL)
			{
				pFlash->put_WMode(_bstr_t(_T("Transparent")));
				pFlash->put_Movie(_bstr_t(CPaintManagerUI::GetInstancePath() + _T("\\skin\\flash\\3.swf")));

				pFlash->Release();
			}

			//pActiveX->InVoke(...)------>FlashCall()
		}
	}
}

LRESULT CPageActiveX::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	LRESULT lRes = 0;

	return lRes;
}

void CPageActiveX::NewWindow3(CWebBrowserUI* pWeb, IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl)
{
	*Cancel = 1;//默认弹出IE浏览器;设置为真,则不弹出
	CWebBrowserUI* pWebBrowser = static_cast<CWebBrowserUI*>(m_pManager->FindControl(_T("webTest")));
	if (nullptr != pWebBrowser)
	{
		pWebBrowser->NavigateUrl(bstrUrl);
	}
}