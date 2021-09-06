#ifndef _CONTROLEXUI_H__
#define _CONTROLEXUI_H__

#include "Duilib.h"
#define DUI_CTL_CARD		_T("Card")

namespace DuiLib
{
	//===========================================================
	class CCardUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CCardUI)
	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		virtual void DoInit() override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl) override;
	private:
		void SetImage(LPCTSTR pstrValue);
		void SetName(LPCTSTR pstrValue);
		void SetAge(LPCTSTR pstrValue);
		void SetSex(LPCTSTR pstrValue);
		void SetID(LPCTSTR pstrValue);
		void SetBirthday(LPCTSTR pstrValue);
		void SetTel(LPCTSTR pstrValue);
		void SetCreateTime(LPCTSTR pstrValue);
	public:
		CCardUI();
		~CCardUI() = default;
	private:
		CDuiString m_strImage;
		CDuiString m_strName;
		CDuiString m_strAge; 
		CDuiString m_strSex;
		CDuiString m_strId;
		CDuiString m_strBirthday;
		CDuiString m_strTel;
		CDuiString m_strCreateTime;

		CLabelUI* m_pobjLblImage;
		CLabelUI* m_pobjLblName;
		CLabelUI* m_pobjLblAge;
		CLabelUI* m_pobjLblSex;
		CLabelUI* m_pobjLblId;
		CLabelUI* m_pobjLblBirthday;
		CLabelUI* m_pobjLblTel;
		CLabelUI* m_pobjLblCreateTime;
	};


}
#endif/*_CONTROLEXUI_H__*/