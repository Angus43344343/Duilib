#ifndef _CONTROLDEMO_H__
#define _CONTROLDEMO_H__

#include "PageControl.h"
#include "PageSenior.h"
#include "PageExtand.h"
#include "PageActiveX.h"

#include "SkinFrame.h"


class CControlDemo : public WindowImplBase, public SkinChangedReceiver
{
public:
	LPCTSTR GetWindowClassName(void) const ;
	CDuiString GetSkinFile();

	virtual void InitWindow() override;
	virtual void OnFinalMessage(HWND hWnd) override;

	virtual void Notify(TNotifyUI& msg) override;
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual BOOL Receive(SkinChangedParam param);//Obsever----Receiver
	virtual LPCTSTR QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType);
private:
	void ShowRichEdit(CRichEditUI* pobjRichEdit, TCHAR* pcFileName);
public:
	CControlDemo();
	~CControlDemo();
private:
	CTrayIcon m_trayIcon;
	CStdStringPtrMap m_MenuCheckInfo; //保存菜单的单选复选信息
	
	CPageBase m_objPageBase;
	CPageSenior m_objPageSenior;
	CPageExtand m_objPageExtand;
	CPageActiveX m_objPageActiveX;
};

#endif /*_CONTROLDEMO_H__*/


