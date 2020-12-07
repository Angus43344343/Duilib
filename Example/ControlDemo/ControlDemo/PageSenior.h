#ifndef _PAGESENIOR_H__
#define _PAGESENIOR_H__

#include "Duilib.h"
#define DUI_PAGE_SENIOR _T("pageSenior")

class CPageSenior : public CNotifyPump, public INotifyUI, public IMessageFilterUI, public IListCallbackUI, public IListComboCallbackUI
{
public:
	void SetManager(CPaintManagerUI* pManager);

	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	//list 回调
	static int CALLBACK CompareItemHigh(UINT_PTR item1Addr, UINT_PTR item2Addr, UINT_PTR CompareData);
	static int CALLBACK CompareItemLow(UINT_PTR item1Addr, UINT_PTR item2Addr, UINT_PTR CompareData);

	virtual LPCTSTR GetItemText(CControlUI* pList, int iItem, int iSubItem);
	virtual DWORD GetItemTextColor(CControlUI* pList, int iItem, int iSubItem, int iState);// iState：0-正常、1-激活、2-选择、3-禁用

	virtual void GetItemComboTextArray(CControlUI* pCtrl, int iItem, int iSubItem);
	//list 回调
public:
	CPageSenior();
	virtual ~CPageSenior();
private:
	CPaintManagerUI* m_pManager;
};

#endif /*_PAGESENIOR_H__*/