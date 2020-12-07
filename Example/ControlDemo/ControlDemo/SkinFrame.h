#ifndef _SKINFRAME_H__
#define _SKINFRAME_H__

#include "Duilib.h"

class CSkinFrame : public WindowImplBase
{
public:
	LPCTSTR GetWindowClassName(void) const;
	CDuiString GetSkinFile();

	virtual void InitWindow() override;
	virtual void OnFinalMessage(HWND hWnd) override;

	virtual void Notify(TNotifyUI& msg) override;
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
public:
	CSkinFrame(HWND hParent, CControlUI *pControl) : m_hParentWnd(hParent), m_pobjControl(pControl){};
	~CSkinFrame() = default;
private:
	HWND m_hParentWnd;
	CControlUI* m_pobjControl;

};

#endif /*_SKINFRAME_H__*/