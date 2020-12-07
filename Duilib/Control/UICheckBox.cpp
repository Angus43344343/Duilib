#include "StdAfx.h"
#include "UICheckBox.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CCheckBoxUI)

	CCheckBoxUI::CCheckBoxUI() : m_bAutoCheck(false)
	{

	}

	LPCTSTR CCheckBoxUI::GetClass() const
	{
		return _T("CheckBoxUI");
	}
	LPVOID CCheckBoxUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, DUI_CTR_CHECKBOX) == 0) return static_cast<CCheckBoxUI*>(this);
		return COptionUI::GetInterface(pstrName);
	}

	void CCheckBoxUI::SetCheck(bool bCheck)
	{
		Selected(bCheck);
	}

	bool  CCheckBoxUI::GetCheck() const
	{
		return IsSelected();
	}

	void CCheckBoxUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcsicmp(pstrName, _T("EnableAutoCheck")) == 0) SetAutoCheck(_tcsicmp(pstrValue, _T("true")) == 0);

		COptionUI::SetAttribute(pstrName, pstrValue);
	}

	void CCheckBoxUI::SetAutoCheck(bool bEnable)
	{
		m_bAutoCheck = bEnable;
	}

	void CCheckBoxUI::DoEvent(TEventUI& event)
	{
		if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
		{
			if (m_pParent != NULL) m_pParent->DoEvent(event);
			else COptionUI::DoEvent(event);
			return;
		}

		if (m_bAutoCheck && (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK))
		{
			if (::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled())
			{
				SetCheck(!GetCheck());
				m_pManager->SendNotify(this, DUI_MSGTYPE_CHECKCLICK, 0, 0);
				Invalidate();
			}
			return;
		}

		COptionUI::DoEvent(event);
	}

	void CCheckBoxUI::Selected(bool bSelected, bool bMsg/* = true*/)
	{
		if (m_bSelected == bSelected) return;

		m_bSelected = bSelected;
		if (m_bSelected) m_uButtonState |= UISTATE_SELECTED;
		else m_uButtonState &= ~UISTATE_SELECTED;

		if (m_pManager != NULL)
		{
			if (!m_sGroupName.IsEmpty())
			{
				if (m_bSelected)
				{
					CStdPtrArray* aOptionGroup = m_pManager->GetOptionGroup(m_sGroupName);
					for (int i = 0; i < aOptionGroup->GetSize(); i++)
					{
						COptionUI* pControl = static_cast<COptionUI*>(aOptionGroup->GetAt(i));
						if (pControl != this)
						{
							pControl->Selected(false, bMsg);
						}
					}

					if (bMsg)
					{
						m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED, m_bSelected, 0);
					}
				}
			}
			else
			{
				if (bMsg)
				{
					m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED, m_bSelected, 0);
				}
			}
		}

		Invalidate();
	}
}//namespace DuiLib