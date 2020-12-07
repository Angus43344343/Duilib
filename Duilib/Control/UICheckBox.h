#ifndef _UICHECKBOX_H__
#define _UICHECKBOX_H__

namespace DuiLib
{
	class UILIB_API CCheckBoxUI : public COptionUI
	{
		DECLARE_DUICONTROL(CCheckBoxUI)
	public:
		CCheckBoxUI();

	public:
		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface(LPCTSTR pstrName);

		void SetCheck(bool bCheck);
		bool GetCheck() const;

	public:
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetAutoCheck(bool bEnable);
		virtual void DoEvent(TEventUI& event);
		virtual void Selected(bool bSelected, bool bMsg = true);

	protected:
		bool m_bAutoCheck;
	};

}//namespace DuiLib

#endif /*_UICHECKBOX_H__*/