#ifndef _PAGEACTIVEX_H__
#define _PAGEACTIVEX_H__

#include "Duilib.h"
#define DUI_PAGE_ACTIVEX _T("pageActiveX")

class CPageActiveX : 
	public CNotifyPump, 
	public INotifyUI, 
	public IMessageFilterUI, 
	public CFlashEventHandler,
	public CWebBrowserEventHandler
{
public:
	void SetManager(CPaintManagerUI* pManager);
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

public:
	/*Flash接口*/
	virtual HRESULT OnReadyStateChange(long newState) { return S_OK; }
	virtual HRESULT OnProgress(long percentDone) { return S_OK; }
	virtual HRESULT FSCommand(LPCTSTR command, LPCTSTR args) { return S_OK; } 
	virtual HRESULT FlashCall(LPCTSTR request) { return S_OK; }

	/*WebBrowser接口*/
	virtual void BeforeNavigate2(CWebBrowserUI* pWeb, IDispatch *pDisp, VARIANT *&url, VARIANT *&Flags, VARIANT *&TargetFrameName, VARIANT *&PostData, VARIANT *&Headers, VARIANT_BOOL *&Cancel) {}
	virtual void NavigateError(CWebBrowserUI* pWeb, IDispatch *pDisp, VARIANT * &url, VARIANT *&TargetFrameName, VARIANT *&StatusCode, VARIANT_BOOL *&Cancel) {}
	virtual void NavigateComplete2(CWebBrowserUI* pWeb, IDispatch *pDisp, VARIANT *&url){}
	virtual void ProgressChange(CWebBrowserUI* pWeb, LONG nProgress, LONG nProgressMax) {}
	virtual void NewWindow3(CWebBrowserUI* pWeb, IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl);
	virtual void CommandStateChange(CWebBrowserUI* pWeb, long Command, VARIANT_BOOL Enable){};
	virtual void TitleChange(CWebBrowserUI* pWeb, BSTR bstrTitle){};
	virtual void DocumentComplete(CWebBrowserUI* pWeb, IDispatch *pDisp, VARIANT *&url){}

	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(CWebBrowserUI* pWeb,//右键菜单
		/* [in] */ DWORD dwID,
		/* [in] */ POINT __RPC_FAR *ppt,
		/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
		/* [in] */ IDispatch __RPC_FAR *pdispReserved)
	{
		return E_NOTIMPL;/*弹出右键菜单*/
	}
	virtual HRESULT STDMETHODCALLTYPE GetHostInfo(CWebBrowserUI* pWeb,//设置IE属性
		/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo)
	{
		//去边框和滚动条
		//if (pInfo != NULL) 
		//{
		//	pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO | DOCHOSTUIFLAG_NO3DBORDER;
		//}
		//return S_OK;

		return E_NOTIMPL;
	}
public:
	CPageActiveX();
	virtual ~CPageActiveX();
private:
	CPaintManagerUI* m_pManager;
};

#endif /*_PAGEACTIVEX_H__*/