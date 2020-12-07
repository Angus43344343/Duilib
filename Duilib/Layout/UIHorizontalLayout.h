#ifndef __UIHORIZONTALLAYOUT_H__
#define __UIHORIZONTALLAYOUT_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CHorizontalLayoutUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CHorizontalLayoutUI)
	public:
		CHorizontalLayoutUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetSepWidth(int iWidth);
		int GetSepWidth() const;
		void SetSepImmMode(bool bImmediately);
		bool IsSepImmMode() const;

		void SetSepColor(DWORD dwSepColor);
		DWORD GetSepColor(DWORD dwSepColor);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoEvent(TEventUI& event);

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void DoPostPaint(HDC hDC, const RECT& rcPaint);
		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);//zm

		RECT GetThumbRect(bool bUseNew = false) const;

	protected:
		int m_iSepWidth;
		UINT m_uButtonState;
		POINT ptLastMouse;
		RECT m_rcNewPos;
		bool m_bImmMode;

		DWORD m_dwSepColor;
	};
}
#endif // __UIHORIZONTALLAYOUT_H__
