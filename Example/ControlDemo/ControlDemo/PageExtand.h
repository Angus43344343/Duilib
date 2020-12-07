#ifndef _PAGEEXTAND_H__
#define _PAGEEXTAND_H__

#include "Duilib.h"
#define DUI_PAGE_EXTAND _T("pageExtand")

class CPageExtand : public CNotifyPump, public INotifyUI, public IMessageFilterUI
{
public:
	void SetManager(CPaintManagerUI* pManager);
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
public:
	CPageExtand();
	virtual ~CPageExtand();
private:
	CPaintManagerUI* m_pManager;
	CStdStringPtrMap m_MenuCheckInfo; //保存菜单的单选复选信息
};



#endif /*_PAGEEXTAND_H__*/