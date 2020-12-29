#ifndef _PAGEREDRAW_H__
#define _PAGEREDRAW_H__

#include "Duilib.h"
#define DUI_PAGE_REDRAW _T("pageRedraw")

class CPageRedraw : public CNotifyPump, public INotifyUI, public IMessageFilterUI
{
public:
	void SetManager(CPaintManagerUI* pManager);
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

public:
	CPageRedraw();
	virtual ~CPageRedraw();
private:
	CPaintManagerUI* m_pManager;
};

#endif /*_PAGEREDRAW_H__*/