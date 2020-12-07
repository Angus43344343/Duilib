#ifndef _PAGECONTROL_H__
#define _PAGECONTROL_H__

#include "Duilib.h"
#define DUI_PAGE_BASE _T("pageBase")

class CPageBase : public CNotifyPump, public INotifyUI, public IMessageFilterUI
{
public:
	void SetManager(CPaintManagerUI* pManager);
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
private:
	void ShowRichEdit(CRichEditUI* pobjRichEdit, TCHAR* pcFileName);
public:
	CPageBase();
	virtual ~CPageBase();
private:
	CPaintManagerUI* m_pManager;
};

#endif /*_PAGECONTROL_H__*/