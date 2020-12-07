#include "stdafx.h"
#include "UIListEx.h"

namespace DuiLib {

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	IMPLEMENT_DUICONTROL(CListExUI)

	CListExUI::CListExUI() : m_pEditUI(NULL), m_pComboBoxUI(NULL), m_bAddMessageFilter(FALSE),m_nRow(-1),m_nColum(-1),m_pXCallback(NULL)
	{
	}

	LPCTSTR CListExUI::GetClass() const
	{
		return _T("XListUI");
	}

	UINT CListExUI::GetControlFlags() const
	{
		return UIFLAG_TABSTOP;
	}

	LPVOID CListExUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("ListEx")) == 0 ) return static_cast<IListOwnerUI*>(this);
		return CListUI::GetInterface(pstrName);
	}

	BOOL CListExUI::CheckColumEditable(int nColum)
	{
		CListContainerHeaderItemUI* pHItem = static_cast<CListContainerHeaderItemUI*>(m_pHeader->GetItemAt(nColum));
		return pHItem != NULL? pHItem->GetColumeEditable() : FALSE;
	}

	void CListExUI::InitListCtrl()
	{
		if (!m_bAddMessageFilter)
		{
			GetManager()->AddNotifier(this);
			m_bAddMessageFilter = TRUE;
		}
	}

	CRichEditUI* CListExUI::GetEditUI()
	{
		if (m_pEditUI == NULL)
		{
			m_pEditUI = new CRichEditUI;
			m_pEditUI->SetName(_T("ListEx_Edit"));
			LPCTSTR pDefaultAttributes = GetManager()->GetDefaultAttributeList(_T("RichEdit"));
			if( pDefaultAttributes ) 
			{
				m_pEditUI->ApplyAttributeList(pDefaultAttributes);
			}
			m_pEditUI->SetBkColor(0xFFFFFFFF);
			m_pEditUI->SetRich(false);
			m_pEditUI->SetMultiLine(false);
			m_pEditUI->SetWantReturn(true);
			m_pEditUI->SetFloat(true);
			m_pEditUI->SetAttribute(_T("autohscroll"), _T("true"));
			Add(m_pEditUI);
		}

		if (m_pComboBoxUI)
		{
			RECT rc = {0,0,0,0};
			m_pComboBoxUI->SetPos(rc);
		}

		return m_pEditUI;
	}

	BOOL CListExUI::CheckColumComboBoxable(int nColum)
	{
		CListContainerHeaderItemUI* pHItem = static_cast<CListContainerHeaderItemUI*>(m_pHeader->GetItemAt(nColum));
		return pHItem != NULL? pHItem->GetColumeComboable() : FALSE;
	}

	CComboBoxUI* CListExUI::GetComboBoxUI()
	{
		if (m_pComboBoxUI == NULL)
		{
			m_pComboBoxUI = new CComboBoxUI;
			m_pComboBoxUI->SetName(_T("ListEx_Combo"));
			LPCTSTR pDefaultAttributes = GetManager()->GetDefaultAttributeList(_T("Combo"));
			if( pDefaultAttributes ) 
			{
				m_pComboBoxUI->ApplyAttributeList(pDefaultAttributes);
			}

			m_pComboBoxUI->SetFloat(true);//zm 


			Add(m_pComboBoxUI);
		}

		if (m_pEditUI)
		{
			RECT rc = {0,0,0,0};
			m_pEditUI->SetPos(rc);
		}

		return m_pComboBoxUI;
	}

	BOOL CListExUI::CheckColumCheckBoxable(int nColum)
	{
		CControlUI* p = m_pHeader->GetItemAt(nColum);
		_ASSERT(p != NULL);
		CListContainerHeaderItemUI* pHItem = static_cast<CListContainerHeaderItemUI*>(p->GetInterface(_T("ListContainerHeaderItem")));
		return pHItem != NULL? pHItem->GetColumeCheckable() : FALSE;
	}

	void CListExUI::Notify(TNotifyUI& msg)
	{	
		CDuiString strName = msg.pSender->GetName();

		//��ѡ��
		if(_tcsicmp(msg.sType, _T("listheaditemchecked")) == 0)
		{
			BOOL bCheck = (BOOL)msg.lParam;
			//�ж��Ƿ��Ǳ�LIST���͵�notify
			CListHeaderUI* pHeader = GetHeader();
			for (int i = 0; i < pHeader->GetCount(); i++)
			{
				if (pHeader->GetItemAt(i) == msg.pSender)
				{
					for (int i = 0; i < GetCount(); ++i) 
					{
						CControlUI* p = GetItemAt(i);
						CListTextExElementUI* pLItem = static_cast<CListTextExElementUI*>(p->GetInterface(_T("ListTextExElement")));
						if (pLItem != NULL) 
						{
							pLItem->SetCheck(bCheck);
						}
					}
					break;
				}
			}
		}
		else if (_tcsicmp(msg.sType, DUI_MSGTYPE_LISTITEMCHECKED) == 0)
		{
			for (int i = 0; i < GetCount(); ++i) 
			{
				CControlUI* p = GetItemAt(i);
				CListTextExElementUI* pLItem = static_cast<CListTextExElementUI*>(p->GetInterface(_T("ListTextExElement")));
				if (pLItem != NULL && pLItem == msg.pSender)
				{
					OnListItemChecked(LOWORD(msg.wParam), HIWORD(msg.wParam), msg.lParam);
					break;
				}
			}
		}

		//�༭����Ͽ�
		if (_tcsicmp(strName, _T("ListEx_Edit")) == 0 && m_pEditUI && m_nRow >= 0 && m_nColum >= 0)
		{
			if(_tcsicmp(msg.sType, DUI_MSGTYPE_SETFOCUS) == 0)
			{

			}
			else if(_tcsicmp(msg.sType, DUI_MSGTYPE_KILLFOCUS) == 0)
			{
				CDuiString sText = m_pEditUI->GetText();
				CListTextExElementUI* pRowCtrl = (CListTextExElementUI*)GetItemAt(m_nRow);
				if (pRowCtrl)
				{
					pRowCtrl->SetText(m_nColum, sText);
				}

				//���õ�ǰ����
				SetEditRowAndColum(-1, -1);

				//���ر༭��
				RECT rc = {0,0,0,0};
				m_pEditUI->SetPos(rc);
				m_pEditUI->SetVisible(false);
			}
		}
		else if (_tcsicmp(strName, _T("ListEx_Combo")) == 0 && m_pComboBoxUI && m_nRow >= 0 && m_nColum >= 0)
		{
			int  iCurSel, iOldSel;
			iCurSel = msg.wParam;
			iOldSel = msg.lParam;

			if(_tcsicmp(msg.sType, DUI_MSGTYPE_SETFOCUS) == 0)
			{

			}
			else if(_tcsicmp(msg.sType, DUI_MSGTYPE_KILLFOCUS) == 0)
			{
			}
			else if (_tcsicmp(msg.sType, DUI_MSGTYPE_ITEMSELECT) == 0 /*&& iOldSel >= 0*/)//zm
			{
				CListTextExElementUI* pRowCtrl = (CListTextExElementUI*)GetItemAt(m_nRow);
				if (pRowCtrl)
				{
					pRowCtrl->SetText(m_nColum, m_pComboBoxUI->GetText());
				}

				//������Ͽ�
				RECT rc = {0,0,0,0};
				m_pComboBoxUI->SetPos(rc);
			}
		}
		else if(_tcsicmp(msg.sType, _T("scroll")) == 0 && (m_pComboBoxUI || m_pEditUI) && m_nRow >= 0 && m_nColum >= 0)
		{
			HideEditAndComboCtrl();
		}
	}
	void CListExUI::HideEditAndComboCtrl()
	{
		//���ر༭��
		RECT rc = {0,0,0,0};
		if(m_pEditUI)
		{	
			m_pEditUI->SetPos(rc);

			m_pEditUI->SetVisible(false);
		}

		if(m_pComboBoxUI)
		{	
			m_pComboBoxUI->SetPos(rc);
		}
	}

	IListComboCallbackUI* CListExUI::GetTextArrayCallback() const
	{
		return m_pXCallback;
	}

	void CListExUI::SetTextArrayCallback(IListComboCallbackUI* pCallback)
	{
		m_pXCallback = pCallback;
	}

	void CListExUI::OnListItemClicked(int nIndex, int nColum, RECT* lpRCColum, LPCTSTR lpstrText)
	{
		RECT rc = {0,0,0,0};
		if (nColum < 0)
		{
			if (m_pEditUI)
			{
				m_pEditUI->SetPos(rc);

				m_pEditUI->SetVisible(false);
			}

			if (m_pComboBoxUI)
			{
				m_pComboBoxUI->SetPos(rc);
			}
		}
		else
		{
			if (CheckColumEditable(nColum) && GetEditUI())
			{
				//���浱ǰ����
				SetEditRowAndColum(nIndex, nColum);
				
				m_pEditUI->SetVisible(true);
				//�ƶ�λ��
				m_pEditUI->SetFixedWidth(lpRCColum->right - lpRCColum->left);
				m_pEditUI->SetFixedHeight(lpRCColum->bottom - lpRCColum->top);
				m_pEditUI->SetFixedXY(CDuiSize(lpRCColum->left,lpRCColum->top));
				SIZE szTextSize = CRenderEngine::GetTextSize(m_pManager->GetPaintDC(), m_pManager, _T("TTT"), m_ListInfo.nFont, DT_CALCRECT | DT_SINGLELINE);
				m_pEditUI->SetTextPadding(CDuiRect(2, (lpRCColum->bottom - lpRCColum->top - szTextSize.cy) / 2, 2, 0));
				//��������
				m_pEditUI->SetText(lpstrText);

				m_pEditUI->SetFocus();
			}
			else if(CheckColumComboBoxable(nColum) && GetComboBoxUI())
			{
				//������Ͽ�
				m_pComboBoxUI->RemoveAll();
				
				//���浱ǰ����
				SetEditRowAndColum(nIndex, nColum);

				//��������
				m_pComboBoxUI->SetText(lpstrText);

				//�ƶ�λ��
				//zm ����ComboBoxλ��,ֻ��ʹ��float����Element��
				m_pComboBoxUI->SetFixedWidth(lpRCColum->right - lpRCColum->left + 1);
				m_pComboBoxUI->SetFixedHeight(lpRCColum->bottom - lpRCColum->top);
				m_pComboBoxUI->SetFixedXY(CDuiSize(lpRCColum->left - 1, lpRCColum->top));
				//zm

				//����Combo��dropbox
				if (m_pXCallback)
				{
					m_pXCallback->GetItemComboTextArray(m_pComboBoxUI, nIndex, nColum);
				}

				m_pComboBoxUI->SetVisible(true);
			}
			else
			{
				if (m_pEditUI)
				{
					m_pEditUI->SetPos(rc);

					m_pEditUI->SetVisible(false);
				}
				if (m_pComboBoxUI)
				{
					m_pComboBoxUI->SetPos(rc);
				}
			}
		}
	}
	void CListExUI::OnListItemChecked(int nIndex, int nColum, BOOL bChecked)
	{
		CControlUI* p = m_pHeader->GetItemAt(nColum);
		CListContainerHeaderItemUI* pHItem = static_cast<CListContainerHeaderItemUI*>(p->GetInterface(_T("ListContainerHeaderItem")));
		if (pHItem == NULL)
		{
			return;
		}

		//���ѡ�У���ô����Ƿ�ȫ��������ѡ��״̬
		if (bChecked)
		{
			BOOL bCheckAll = TRUE;
			for(int i = 0; i < GetCount(); i++) 
			{
				CControlUI* p = GetItemAt(i);
				CListTextExElementUI* pLItem = static_cast<CListTextExElementUI*>(p->GetInterface(_T("ListTextExElement")));
				if( pLItem != NULL && !pLItem->GetCheck()) 
				{
					bCheckAll = FALSE;
					break;
				}
			}

			if (bCheckAll)
			{
				pHItem->SetCheck(TRUE);
			}
			else
			{
				pHItem->SetCheck(FALSE);
			}
		}
		else
		{
			pHItem->SetCheck(FALSE);
		}
	}

	void CListExUI::DoEvent(TEventUI& event)
	{
		if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_SCROLLWHEEL)
		{
			HideEditAndComboCtrl();
		}

		CListUI::DoEvent(event);
	}

	void CListExUI::SetColumItemColor(int nIndex, int nColum, DWORD iBKColor)
	{
		CControlUI* p = GetItemAt(nIndex);
		CListTextExElementUI* pLItem = static_cast<CListTextExElementUI*>(p->GetInterface(_T("ListTextExElement")));
		if( pLItem != NULL) 
		{
			DWORD iTextBkColor = iBKColor;
			pLItem->SetColumItemColor(nColum, iTextBkColor);
		}
	}

	BOOL CListExUI::GetColumItemColor(int nIndex, int nColum, DWORD& iBKColor)
	{
		CControlUI* p = GetItemAt(nIndex);
		CListTextExElementUI* pLItem = static_cast<CListTextExElementUI*>(p->GetInterface(_T("ListTextExElement")));
		if( pLItem == NULL) 
		{
			return FALSE;
		}
		pLItem->GetColumItemColor(nColum, iBKColor);
		return TRUE;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	IMPLEMENT_DUICONTROL(CListContainerHeaderItemUI)

	CListContainerHeaderItemUI::CListContainerHeaderItemUI() : m_bDragable(TRUE), m_uButtonState(0), m_iSepWidth(4),
		m_uTextStyle(DT_VCENTER | DT_CENTER | DT_SINGLELINE), m_dwTextColor(0), m_iFont(-1), m_bShowHtml(FALSE),
		m_bEditable(FALSE),m_bComboable(FALSE),m_bCheckBoxable(FALSE),m_uCheckBoxState(0),m_bChecked(FALSE),m_pOwner(NULL)
	{
		SetTextPadding(CDuiRect(2, 0, 2, 0));
		ptLastMouse.x = ptLastMouse.y = 0;
		SetMinWidth(16);
	}

	LPCTSTR CListContainerHeaderItemUI::GetClass() const
	{
		return _T("ListContainerHeaderItem");
	}

	LPVOID CListContainerHeaderItemUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("ListContainerHeaderItem")) == 0 ) return this;
		return CContainerUI::GetInterface(pstrName);
	}

	UINT CListContainerHeaderItemUI::GetControlFlags() const
	{
		if( IsEnabled() && m_iSepWidth != 0 ) return UIFLAG_SETCURSOR;
		else return 0;
	}

	void CListContainerHeaderItemUI::SetEnabled(BOOL bEnable)
	{
		CContainerUI::SetEnabled(bEnable);
		if( !IsEnabled() ) 
		{
			m_uButtonState = 0;
		}
	}

	BOOL CListContainerHeaderItemUI::IsDragable() const
	{
		return m_bDragable;
	}

	void CListContainerHeaderItemUI::SetDragable(BOOL bDragable)
	{
		m_bDragable = bDragable;
		if ( !m_bDragable ) m_uButtonState &= ~UISTATE_CAPTURED;
	}

	DWORD CListContainerHeaderItemUI::GetSepWidth() const
	{
		return m_iSepWidth;
	}

	void CListContainerHeaderItemUI::SetSepWidth(int iWidth)
	{
		m_iSepWidth = iWidth;
	}

	DWORD CListContainerHeaderItemUI::GetTextStyle() const
	{
		return m_uTextStyle;
	}

	void CListContainerHeaderItemUI::SetTextStyle(UINT uStyle)
	{
		m_uTextStyle = uStyle;
		Invalidate();
	}

	DWORD CListContainerHeaderItemUI::GetTextColor() const
	{
		return m_dwTextColor;
	}


	void CListContainerHeaderItemUI::SetTextColor(DWORD dwTextColor)
	{
		m_dwTextColor = dwTextColor;
	}

	RECT CListContainerHeaderItemUI::GetTextPadding() const
	{
		return m_rcTextPadding;
	}

	void CListContainerHeaderItemUI::SetTextPadding(RECT rc)
	{
		m_rcTextPadding = rc;
		Invalidate();
	}

	void CListContainerHeaderItemUI::SetFont(int index)
	{
		m_iFont = index;
	}

	BOOL CListContainerHeaderItemUI::IsShowHtml()
	{
		return m_bShowHtml;
	}

	void CListContainerHeaderItemUI::SetShowHtml(BOOL bShowHtml)
	{
		if( m_bShowHtml == bShowHtml ) return;

		m_bShowHtml = bShowHtml;
		Invalidate();
	}

	LPCTSTR CListContainerHeaderItemUI::GetNormalImage() const
	{
		return m_sNormalImage;
	}

	void CListContainerHeaderItemUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_sNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CListContainerHeaderItemUI::GetHotImage() const
	{
		return m_sHotImage;
	}

	void CListContainerHeaderItemUI::SetHotImage(LPCTSTR pStrImage)
	{
		m_sHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CListContainerHeaderItemUI::GetPushedImage() const
	{
		return m_sPushedImage;
	}

	void CListContainerHeaderItemUI::SetPushedImage(LPCTSTR pStrImage)
	{
		m_sPushedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CListContainerHeaderItemUI::GetFocusedImage() const
	{
		return m_sFocusedImage;
	}

	void CListContainerHeaderItemUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		m_sFocusedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CListContainerHeaderItemUI::GetSepImage() const
	{
		return m_sSepImage;
	}

	void CListContainerHeaderItemUI::SetSepImage(LPCTSTR pStrImage)
	{
		m_sSepImage = pStrImage;
		Invalidate();
	}

	void CListContainerHeaderItemUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("dragable")) == 0 ) SetDragable(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("sepwidth")) == 0 ) SetSepWidth(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("align")) == 0 ) 
		{
			if( _tcsstr(pstrValue, _T("left")) != NULL ) 
			{
				m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
				m_uTextStyle |= DT_LEFT;
			}
			if( _tcsstr(pstrValue, _T("center")) != NULL ) 
			{
				m_uTextStyle &= ~(DT_LEFT | DT_RIGHT);
				m_uTextStyle |= DT_CENTER;
			}
			if( _tcsstr(pstrValue, _T("right")) != NULL ) 
			{
				m_uTextStyle &= ~(DT_LEFT | DT_CENTER);
				m_uTextStyle |= DT_RIGHT;
			}
		}
		else if( _tcsicmp(pstrName, _T("endellipsis")) == 0 ) 
		{
			if( _tcsicmp(pstrValue, _T("true")) == 0 ) m_uTextStyle |= DT_END_ELLIPSIS;
			else m_uTextStyle &= ~DT_END_ELLIPSIS;
		}    
		else if( _tcsicmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("textcolor")) == 0 ) 
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("textpadding")) == 0 ) 
		{
			RECT rcTextPadding = { 0 };
			LPTSTR pstr = NULL;
			rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);
			rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);
			rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);
			rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10);
			SetTextPadding(rcTextPadding);
		}
		else if( _tcsicmp(pstrName, _T("showhtml")) == 0 ) SetShowHtml(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("pushedimage")) == 0 ) SetPushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("sepimage")) == 0 ) SetSepImage(pstrValue);

		else if( _tcsicmp(pstrName, _T("editable")) == 0 ) SetColumeEditable(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("comboable")) == 0 ) SetColumeComboable(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("checkable")) == 0 ) SetColumeCheckable(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("checkboxwidth")) == 0 ) SetCheckBoxWidth(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("checkboxheight")) == 0 ) SetCheckBoxHeight(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("checkboxnormalimage")) == 0 ) SetCheckBoxNormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("checkboxhotimage")) == 0 ) SetCheckBoxHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("checkboxpushedimage")) == 0 ) SetCheckBoxPushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("checkboxfocusedimage")) == 0 ) SetCheckBoxFocusedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("checkboxdisabledimage")) == 0 ) SetCheckBoxDisabledImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("checkboxselectedimage")) == 0 ) SetCheckBoxSelectedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("checkboxforeimage")) == 0 ) SetCheckBoxForeImage(pstrValue);

		else CContainerUI::SetAttribute(pstrName, pstrValue);
	}

	void CListContainerHeaderItemUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) 
		{
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CContainerUI::DoEvent(event);
			return;
		}

		//CheckBoxAble
		if (m_bCheckBoxable)
		{
			RECT rcCheckBox;
			GetCheckBoxRect(rcCheckBox);

			if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
			{
				if( ::PtInRect(&rcCheckBox, event.ptMouse)) 
				{
					m_uCheckBoxState |= UISTATE_PUSHED | UISTATE_CAPTURED;
					Invalidate();
				}
			}
			else if( event.Type == UIEVENT_MOUSEMOVE )
			{
				if( (m_uCheckBoxState & UISTATE_CAPTURED) != 0 ) 
				{
					if( ::PtInRect(&rcCheckBox, event.ptMouse) ) 
						m_uCheckBoxState |= UISTATE_PUSHED;
					else 
						m_uCheckBoxState &= ~UISTATE_PUSHED;
					Invalidate();
				}
				else if (::PtInRect(&rcCheckBox, event.ptMouse))
				{
					m_uCheckBoxState |= UISTATE_HOT;
					Invalidate();
				}
				else
				{
					m_uCheckBoxState &= ~UISTATE_HOT;
					Invalidate();
				}
			}
			else if( event.Type == UIEVENT_BUTTONUP )
			{
				if( (m_uCheckBoxState & UISTATE_CAPTURED) != 0 )
				{
					if( ::PtInRect(&rcCheckBox, event.ptMouse) ) 
					{
						SetCheck(!GetCheck());
						CContainerUI* pOwner = (CContainerUI*)m_pParent;
						if (pOwner)
						{
							m_pManager->SendNotify(this, DUI_MSGTYPE_LISTHEADITEMCHECKED, pOwner->GetItemIndex(this), m_bChecked);
						}
					}
					m_uCheckBoxState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
					Invalidate();
				}
				else if (::PtInRect(&rcCheckBox, event.ptMouse))
				{

				}
			}
			else if( event.Type == UIEVENT_MOUSEENTER )
			{
				if( ::PtInRect(&rcCheckBox, event.ptMouse) ) 
				{
					m_uCheckBoxState |= UISTATE_HOT;
					Invalidate();
				}
			}
			else if( event.Type == UIEVENT_MOUSELEAVE )
			{
				m_uCheckBoxState &= ~UISTATE_HOT;
				Invalidate();
			}
		}

		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
		{
			if( !IsEnabled() ) return;
			RECT rcSeparator = GetThumbRect();
			if (m_iSepWidth>=0)
				rcSeparator.left -= 4;
			else
				rcSeparator.right += 4;
			if( ::PtInRect(&rcSeparator, event.ptMouse) ) 
			{
				if( m_bDragable ) 
				{
					m_uButtonState |= UISTATE_CAPTURED;
					ptLastMouse = event.ptMouse;
				}
			}
			else 
			{
				m_uButtonState |= UISTATE_PUSHED;
				CContainerUI* pOwner = (CContainerUI*)m_pParent;
				if (pOwner)
				{
					m_pManager->SendNotify(this, DUI_MSGTYPE_LISTHEADERCLICK, 0, pOwner->GetItemIndex(this));//zm
				}
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
			{
				m_uButtonState &= ~UISTATE_CAPTURED;
				if( GetParent() ) 
					GetParent()->NeedParentUpdate();
			}
			else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) 
			{
				m_uButtonState &= ~UISTATE_PUSHED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
			{
				RECT rc = m_rcItem;
				if( m_iSepWidth >= 0 ) 
				{
					rc.right -= ptLastMouse.x - event.ptMouse.x;
				}
				else 
				{
					rc.left -= ptLastMouse.x - event.ptMouse.x;
				}

				if( rc.right - rc.left > GetMinWidth() ) 
				{
					m_cxyFixed.cx = rc.right - rc.left;
					ptLastMouse = event.ptMouse;
					if( GetParent() ) 
						GetParent()->NeedParentUpdate();
				}
			}
			return;
		}
		if( event.Type == UIEVENT_SETCURSOR )
		{
			RECT rcSeparator = GetThumbRect();
			if (m_iSepWidth>=0)
				rcSeparator.left-=4;
			else
				rcSeparator.right+=4;
			if( IsEnabled() && m_bDragable && ::PtInRect(&rcSeparator, event.ptMouse) ) 
			{
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
				return;
			}
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( IsEnabled() ) 
			{
				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) 
			{
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		CContainerUI::DoEvent(event);
	}

	SIZE CListContainerHeaderItemUI::EstimateSize(SIZE szAvailable)
	{
		if( m_cxyFixed.cy == 0 ) return CDuiSize(m_cxyFixed.cx, m_pManager->GetDefaultFontInfo()->tm.tmHeight + 14);
		return CContainerUI::EstimateSize(szAvailable);
	}

	RECT CListContainerHeaderItemUI::GetThumbRect() const
	{
		if( m_iSepWidth >= 0 ) return CDuiRect(m_rcItem.right - m_iSepWidth, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
		else return CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.left - m_iSepWidth, m_rcItem.bottom);
	}

	void CListContainerHeaderItemUI::PaintStatusImage(HDC hDC)
	{
		//HeadItem Bkgnd
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;

		if( (m_uButtonState & UISTATE_PUSHED) != 0 ) 
		{
			if( m_sPushedImage.IsEmpty() && !m_sNormalImage.IsEmpty() ) DrawImage(hDC, (LPCTSTR)m_sNormalImage);
			if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) ) {}
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) 
		{
			if( m_sHotImage.IsEmpty() && !m_sNormalImage.IsEmpty() ) DrawImage(hDC, (LPCTSTR)m_sNormalImage);
			if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) {}
		}
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) 
		{
			if( m_sFocusedImage.IsEmpty() && !m_sNormalImage.IsEmpty() ) DrawImage(hDC, (LPCTSTR)m_sNormalImage);
			if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) {}
		}
		else 
		{
			if( !m_sNormalImage.IsEmpty() ) 
			{
				if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) {}
			}
		}

		if( !m_sSepImage.IsEmpty() ) 
		{
			RECT rcThumb = GetThumbRect();
			rcThumb.left -= m_rcItem.left;
			rcThumb.top -= m_rcItem.top;
			rcThumb.right -= m_rcItem.left;
			rcThumb.bottom -= m_rcItem.top;

			m_sSepImageModify.Empty();
			m_sSepImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
			if( !DrawImage(hDC, (LPCTSTR)m_sSepImage, (LPCTSTR)m_sSepImageModify) ) {}
		}

		if(m_bCheckBoxable)
		{
			m_uCheckBoxState &= ~UISTATE_PUSHED;

			if( (m_uCheckBoxState & UISTATE_SELECTED) != 0 ) 
			{
				if( !m_sCheckBoxSelectedImage.IsEmpty() ) 
				{
					if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxSelectedImage) ) {}
					else goto Label_ForeImage;
				}
			}

			if( IsFocused() ) m_uCheckBoxState |= UISTATE_FOCUSED;
			else m_uCheckBoxState &= ~ UISTATE_FOCUSED;
			if( !IsEnabled() ) m_uCheckBoxState |= UISTATE_DISABLED;
			else m_uCheckBoxState &= ~ UISTATE_DISABLED;

			if( (m_uCheckBoxState & UISTATE_DISABLED) != 0 ) 
			{
				if( !m_sCheckBoxDisabledImage.IsEmpty() ) 
				{
					if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxDisabledImage) ) {}
					else return;
				}
			}
			else if( (m_uCheckBoxState & UISTATE_PUSHED) != 0 ) 
			{
				if( !m_sCheckBoxPushedImage.IsEmpty() ) 
				{
					if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxPushedImage) ) {}
					else return;
				}
			}
			else if( (m_uCheckBoxState & UISTATE_HOT) != 0 ) 
			{
				if( !m_sCheckBoxHotImage.IsEmpty() ) 
				{
					if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxHotImage) ) {}
					else return;
				}
			}
			else if( (m_uCheckBoxState & UISTATE_FOCUSED) != 0 ) 
			{
				if( !m_sCheckBoxFocusedImage.IsEmpty() ) 
				{
					if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxFocusedImage) ) {}
					else return;
				}
			}

			if( !m_sCheckBoxNormalImage.IsEmpty() ) 
			{
				if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxNormalImage) ) {}
				else return;
			}

Label_ForeImage:
			if( !m_sCheckBoxForeImage.IsEmpty() ) 
			{
				if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxForeImage) ) {}
			}
		}
	}

	void CListContainerHeaderItemUI::PaintText(HDC hDC)
	{
		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();

		RECT rcText = m_rcItem;
		rcText.left += m_rcTextPadding.left;
		rcText.top += m_rcTextPadding.top;
		rcText.right -= m_rcTextPadding.right;
		rcText.bottom -= m_rcTextPadding.bottom;
		if (m_bCheckBoxable) 
		{
			RECT rcCheck;
			GetCheckBoxRect(rcCheck);
			rcText.left += (rcCheck.right - rcCheck.left);
		}

		CDuiString sText = GetText();
		if( sText.IsEmpty() ) return;

		int nLinks = 0;
		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rcText, sText, m_dwTextColor, \
			NULL, NULL, nLinks, m_iFont, DT_SINGLELINE | m_uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rcText, sText, m_dwTextColor, \
			m_iFont, DT_SINGLELINE | m_uTextStyle);
	}

	BOOL CListContainerHeaderItemUI::GetColumeEditable()
	{
		return m_bEditable;
	}

	void CListContainerHeaderItemUI::SetColumeEditable(BOOL bEnable)
	{
		m_bEditable = bEnable;
	}

	BOOL CListContainerHeaderItemUI::GetColumeComboable()
	{
		return m_bComboable;
	}

	void CListContainerHeaderItemUI::SetColumeComboable(BOOL bEnable)
	{
		m_bComboable = bEnable;
	}

	BOOL CListContainerHeaderItemUI::GetColumeCheckable()
	{
		return m_bCheckBoxable;
	}
	void CListContainerHeaderItemUI::SetColumeCheckable(BOOL bEnable)
	{
		m_bCheckBoxable = bEnable;
	}
	void CListContainerHeaderItemUI::SetCheck(BOOL bCheck)
	{
		if( m_bChecked == bCheck ) return;
		m_bChecked = bCheck;
		if( m_bChecked ) m_uCheckBoxState |= UISTATE_SELECTED;
		else m_uCheckBoxState &= ~UISTATE_SELECTED;
		Invalidate();
	}

	BOOL CListContainerHeaderItemUI::GetCheck()
	{
		return m_bChecked;
	}
	BOOL CListContainerHeaderItemUI::DrawCheckBoxImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify)
	{
		RECT rcCheckBox;
		GetCheckBoxRect(rcCheckBox);
		return CRenderEngine::DrawImageString(hDC, m_pManager, rcCheckBox, m_rcPaint, pStrImage, pStrModify);
	}
	LPCTSTR CListContainerHeaderItemUI::GetCheckBoxNormalImage()
	{
		return m_sCheckBoxNormalImage;
	}

	void CListContainerHeaderItemUI::SetCheckBoxNormalImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxNormalImage = pStrImage;
	}

	LPCTSTR CListContainerHeaderItemUI::GetCheckBoxHotImage()
	{
		return m_sCheckBoxHotImage;
	}

	void CListContainerHeaderItemUI::SetCheckBoxHotImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxHotImage = pStrImage;
	}

	LPCTSTR CListContainerHeaderItemUI::GetCheckBoxPushedImage()
	{
		return m_sCheckBoxPushedImage;
	}

	void CListContainerHeaderItemUI::SetCheckBoxPushedImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxPushedImage = pStrImage;
	}

	LPCTSTR CListContainerHeaderItemUI::GetCheckBoxFocusedImage()
	{
		return m_sCheckBoxFocusedImage;
	}

	void CListContainerHeaderItemUI::SetCheckBoxFocusedImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxFocusedImage = pStrImage;
	}

	LPCTSTR CListContainerHeaderItemUI::GetCheckBoxDisabledImage()
	{
		return m_sCheckBoxDisabledImage;
	}

	void CListContainerHeaderItemUI::SetCheckBoxDisabledImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxDisabledImage = pStrImage;
	}
	LPCTSTR CListContainerHeaderItemUI::GetCheckBoxSelectedImage()
	{
		return m_sCheckBoxSelectedImage;
	}

	void CListContainerHeaderItemUI::SetCheckBoxSelectedImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxSelectedImage = pStrImage;
	}
	LPCTSTR CListContainerHeaderItemUI::GetCheckBoxForeImage()
	{
		return m_sCheckBoxForeImage;
	}

	void CListContainerHeaderItemUI::SetCheckBoxForeImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxForeImage = pStrImage;
	}
	int CListContainerHeaderItemUI::GetCheckBoxWidth() const
	{
		if (m_pManager) m_pManager->GetDPIObj()->Scale(m_cxyCheckBox.cx);//zm
		return m_cxyCheckBox.cx;
	}

	void CListContainerHeaderItemUI::SetCheckBoxWidth(int cx)
	{
		if( cx < 0 ) return; 
		m_cxyCheckBox.cx = cx;
	}

	int CListContainerHeaderItemUI::GetCheckBoxHeight()  const 
	{
		if (m_pManager) m_pManager->GetDPIObj()->Scale(m_cxyCheckBox.cy);//zm
		return m_cxyCheckBox.cy;
	}

	void CListContainerHeaderItemUI::SetCheckBoxHeight(int cy)
	{
		if( cy < 0 ) return; 
		m_cxyCheckBox.cy = cy;
	}

	void CListContainerHeaderItemUI::GetCheckBoxRect(RECT &rc)
	{
		memset(&rc, 0x00, sizeof(rc)); 
		int nItemHeight = m_rcItem.bottom - m_rcItem.top;
		rc.left = m_rcItem.left + 6;
		rc.top = m_rcItem.top + (nItemHeight - GetCheckBoxHeight()) / 2;
		rc.right = rc.left + GetCheckBoxWidth();
		rc.bottom = rc.top + GetCheckBoxHeight();
	}

	void CListContainerHeaderItemUI::SetOwner(CContainerUI* pOwner)
	{
		m_pOwner = pOwner;
	}
	CContainerUI* CListContainerHeaderItemUI::GetOwner()
	{
		return m_pOwner;
	}
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	IMPLEMENT_DUICONTROL(CListTextExElementUI)

	CListTextExElementUI::CListTextExElementUI() : 
	m_nLinks(0), m_nHoverLink(-1), m_pOwner(NULL),m_uCheckBoxState(0),m_bChecked(FALSE)
	{
		::ZeroMemory(&m_rcLinks, sizeof(m_rcLinks));
		m_cxyCheckBox.cx = m_cxyCheckBox.cy = 0;

		::ZeroMemory(&ColumCorlorArray, sizeof(ColumCorlorArray));
	}

	CListTextExElementUI::~CListTextExElementUI()
	{
		CDuiString* pText;
		for( int it = 0; it < m_aTexts.GetSize(); it++ ) 
		{
			pText = static_cast<CDuiString*>(m_aTexts[it]);
			if( pText ) delete pText;
		}
		m_aTexts.Empty();
	}

	LPCTSTR CListTextExElementUI::GetClass() const
	{
		return _T("ListTextExElementUI");
	}

	LPVOID CListTextExElementUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("ListTextExElement")) == 0 ) return static_cast<CListTextExElementUI*>(this);
		return CListLabelElementUI::GetInterface(pstrName);
	}

	UINT CListTextExElementUI::GetControlFlags() const
	{
		return UIFLAG_WANTRETURN | ( (IsEnabled() && m_nLinks > 0) ? UIFLAG_SETCURSOR : 0);
	}

	LPCTSTR CListTextExElementUI::GetText(int iIndex) const
	{
		CDuiString* pText = static_cast<CDuiString*>(m_aTexts.GetAt(iIndex));
		if( pText ) return pText->GetData();
		return NULL;
	}

	void CListTextExElementUI::SetText(int iIndex, LPCTSTR pstrText)
	{
		if( m_pOwner == NULL ) return;
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		if( iIndex < 0 || iIndex >= pInfo->nColumns ) return;
		while( m_aTexts.GetSize() < pInfo->nColumns ) { m_aTexts.Add(NULL); }

		CDuiString* pText = static_cast<CDuiString*>(m_aTexts[iIndex]);
		if( (pText == NULL && pstrText == NULL) || (pText && *pText == pstrText) ) return;

		if ( pText )
			pText->Assign(pstrText);
		else
			m_aTexts.SetAt(iIndex, new CDuiString(pstrText));
		Invalidate();
	}

	DWORD CListTextExElementUI::GetTextColor(int iIndex) const
	{
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		if (iIndex < 0 || iIndex >= pInfo->nColumns || m_aTextColors.GetSize() <= 0) return pInfo->dwTextColor;

		DWORD dwColor = (DWORD)m_aTextColors.GetAt(iIndex);
		return dwColor;
	}

	void CListTextExElementUI::SetTextColor(int iIndex, DWORD dwTextColor)
	{
		if (m_pOwner == NULL) return;

		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		if (iIndex < 0 || iIndex >= pInfo->nColumns) return;
		while (m_aTextColors.GetSize() < pInfo->nColumns) { m_aTextColors.Add((LPVOID)pInfo->dwTextColor); }
		m_aTextColors.SetAt(iIndex, (LPVOID)dwTextColor);

		Invalidate();
	}

	void CListTextExElementUI::SetOwner(CControlUI* pOwner)
	{
		CListElementUI::SetOwner(pOwner);
		m_pOwner = static_cast<CListUI*>(pOwner->GetInterface(_T("List")));
	}

	CDuiString* CListTextExElementUI::GetLinkContent(int iIndex)
	{
		if( iIndex >= 0 && iIndex < m_nLinks ) return &m_sLinks[iIndex];
		return NULL;
	}

	void CListTextExElementUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pOwner != NULL ) m_pOwner->DoEvent(event);
			else CListLabelElementUI::DoEvent(event);
			return;
		}

		// When you hover over a link
		if( event.Type == UIEVENT_SETCURSOR ) 
		{
			for( int i = 0; i < m_nLinks; i++ ) 
			{
				if( ::PtInRect(&m_rcLinks[i], event.ptMouse) ) 
				{
					::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
					return;
				}
			}      
		}
		if( event.Type == UIEVENT_BUTTONUP && IsEnabled() ) 
		{
			for( int i = 0; i < m_nLinks; i++ ) 
			{
				if( ::PtInRect(&m_rcLinks[i], event.ptMouse) ) 
				{
					m_pManager->SendNotify(this, DUI_MSGTYPE_LINK, i);
					break;
				}
			}
		}
		if( m_nLinks > 0 && event.Type == UIEVENT_MOUSEMOVE ) 
		{
			int nHoverLink = -1;
			for( int i = 0; i < m_nLinks; i++ ) 
			{
				if( ::PtInRect(&m_rcLinks[i], event.ptMouse) ) 
				{
					nHoverLink = i;
					break;
				}
			}

			if(m_nHoverLink != nHoverLink) 
			{
				Invalidate();
				m_nHoverLink = nHoverLink;
			}
		}
		if( m_nLinks > 0 && event.Type == UIEVENT_MOUSELEAVE ) 
		{
			if(m_nHoverLink != -1) 
			{
				Invalidate();
				m_nHoverLink = -1;
			}
		}

		//����Ƿ���Ҫ��ʾ�༭�������Ͽ�	
		CListExUI * pListCtrl = (CListExUI *)m_pOwner;
		int nColum = HitTestColum(event.ptMouse);
		if(event.Type == UIEVENT_BUTTONUP && m_pOwner->IsFocused())
		{
			RECT rc = {0,0,0,0};
			if (nColum >= 0)
			{
				GetColumRect(nColum, rc);
			}

			pListCtrl->OnListItemClicked(GetIndex(), nColum, &rc, GetText(nColum));
		}

		//����Ƿ���Ҫ��ʾCheckBox
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		for( int i = 0; i < pInfo->nColumns; i++ )
		{
			if (pListCtrl->CheckColumCheckBoxable(i))
			{
				RECT rcCheckBox;
				GetCheckBoxRect(i, rcCheckBox);

				if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
				{
					if( ::PtInRect(&rcCheckBox, event.ptMouse)) 
					{
						m_uCheckBoxState |= UISTATE_PUSHED | UISTATE_CAPTURED;
						Invalidate();
					}
				}
				else if( event.Type == UIEVENT_MOUSEMOVE )
				{
					if( (m_uCheckBoxState & UISTATE_CAPTURED) != 0 ) 
					{
						if( ::PtInRect(&rcCheckBox, event.ptMouse) ) 
							m_uCheckBoxState |= UISTATE_PUSHED;
						else 
							m_uCheckBoxState &= ~UISTATE_PUSHED;
						Invalidate();
					}
				}
				else if( event.Type == UIEVENT_BUTTONUP )
				{
					if( (m_uCheckBoxState & UISTATE_CAPTURED) != 0 )
					{
						if( ::PtInRect(&rcCheckBox, event.ptMouse) ) 
						{
							SetCheck(!GetCheck());
							if (m_pManager)
							{
								m_pManager->SendNotify(this, DUI_MSGTYPE_LISTITEMCHECKED, MAKEWPARAM(GetIndex(), 0), m_bChecked);
							}
						}
						m_uCheckBoxState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
						Invalidate();
					}
				}
				else if( event.Type == UIEVENT_MOUSEENTER )
				{
					if( ::PtInRect(&rcCheckBox, event.ptMouse) ) 
					{
						m_uCheckBoxState |= UISTATE_HOT;
						Invalidate();
					}
				}
				else if( event.Type == UIEVENT_MOUSELEAVE )
				{
					m_uCheckBoxState &= ~UISTATE_HOT;
					Invalidate();
				}
			}
		}

		CListLabelElementUI::DoEvent(event);
	}

	SIZE CListTextExElementUI::EstimateSize(SIZE szAvailable)
	{
		TListInfoUI* pInfo = NULL;
		if( m_pOwner ) pInfo = m_pOwner->GetListInfo();

		SIZE cXY = m_cxyFixed;
		if( cXY.cy == 0 && m_pManager != NULL && pInfo != NULL) 
		{
			cXY.cy = m_pManager->GetFontInfo(pInfo->nFont)->tm.tmHeight + 8;
			cXY.cy += pInfo->rcTextPadding.top + pInfo->rcTextPadding.bottom;
		}

		return cXY;
	}

	int CListTextExElementUI::GetItemState()
	{
		int iState = 0;

		if ((m_uButtonState & UISTATE_HOT) != 0)
		{
			iState = 1;
		}

		if (IsSelected())
		{
			iState = 2;
		}

		if (!IsEnabled())
		{
			iState = 3;
		}

		return iState;
	}

	void CListTextExElementUI::DrawItemText(HDC hDC, const RECT& rcItem)
	{
		if( m_pOwner == NULL ) return;
		CListExUI * pListCtrl = (CListExUI *)m_pOwner;
		TListInfoUI* pInfo = m_pOwner->GetListInfo();	
		IListCallbackUI* pCallback = m_pOwner->GetTextCallback();

		m_nLinks = 0;
		int nLinks = lengthof(m_rcLinks);
		for( int i = 0; i < pInfo->nColumns; i++ )
		{
			RECT rcItem = { pInfo->rcColumn[i].left, m_rcItem.top, pInfo->rcColumn[i].right, m_rcItem.bottom };
		
			DWORD iTextBkColor = 0;
			if (GetColumItemColor(i, iTextBkColor))
			{	
				RECT rcColor = rcItem;
				rcColor.right -= 1;
				rcColor.bottom -= 1;//zm Ԥ����������
				CRenderEngine::DrawColor(hDC, rcColor, iTextBkColor);
			}

			rcItem.left += pInfo->rcTextPadding.left;
			rcItem.right -= pInfo->rcTextPadding.right;
			rcItem.top += pInfo->rcTextPadding.top;
			rcItem.bottom -= pInfo->rcTextPadding.bottom;

			//����Ƿ���Ҫ��ʾCheckBox
			if (pListCtrl->CheckColumCheckBoxable(i))
			{
				RECT rcCheckBox;
				GetCheckBoxRect(i, rcCheckBox);
				rcItem.left += (rcCheckBox.right - rcCheckBox.left);
			}

			CDuiString strText;//��ʹ��LPCTSTR����������̫�� by cddjr 2011/10/20
			DWORD iTextColor = pInfo->dwTextColor;

			if (pCallback) iTextColor = pCallback->GetItemTextColor(this, m_iIndex, i, GetItemState());//zm
			if( pCallback ) strText = pCallback->GetItemText(this, m_iIndex, i);
			else strText.Assign(GetText(i));

			if( pInfo->bShowHtml )
				CRenderEngine::DrawHtmlText(hDC, m_pManager, rcItem, strText.GetData(), iTextColor, \
				&m_rcLinks[m_nLinks], &m_sLinks[m_nLinks], nLinks, pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
			else
				CRenderEngine::DrawText(hDC, m_pManager, rcItem, strText.GetData(), iTextColor, \
				pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);

			m_nLinks += nLinks;
			nLinks = lengthof(m_rcLinks) - m_nLinks; 
		}

		for( int i = m_nLinks; i < lengthof(m_rcLinks); i++ ) 
		{
			::ZeroMemory(m_rcLinks + i, sizeof(RECT));
			((CDuiString*)(m_sLinks + i))->Empty();
		}
	}

	void CListTextExElementUI::PaintStatusImage(HDC hDC)
	{
		CListExUI * pListCtrl = (CListExUI *)m_pOwner;
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		for( int i = 0; i < pInfo->nColumns; i++ )
		{
			if (pListCtrl->CheckColumCheckBoxable(i))
			{
				RECT rcCheckBox;
				GetCheckBoxRect(i, rcCheckBox);

				m_uCheckBoxState &= ~UISTATE_PUSHED;

				if( (m_uCheckBoxState & UISTATE_SELECTED) != 0 ) 
				{
					if( !m_sCheckBoxSelectedImage.IsEmpty() ) 
					{
						if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxSelectedImage, NULL, rcCheckBox) ) {}
						else goto Label_ForeImage;
					}
				}

				if( IsFocused() ) m_uCheckBoxState |= UISTATE_FOCUSED;
				else m_uCheckBoxState &= ~ UISTATE_FOCUSED;
				if( !IsEnabled() ) m_uCheckBoxState |= UISTATE_DISABLED;
				else m_uCheckBoxState &= ~ UISTATE_DISABLED;

				if( (m_uCheckBoxState & UISTATE_DISABLED) != 0 ) 
				{
					if( !m_sCheckBoxDisabledImage.IsEmpty() ) 
					{
						if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxDisabledImage, NULL, rcCheckBox) ) {}
						else return;
					}
				}
				else if( (m_uCheckBoxState & UISTATE_PUSHED) != 0 ) 
				{
					if( !m_sCheckBoxPushedImage.IsEmpty() ) 
					{
						if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxPushedImage, NULL, rcCheckBox) ) {}
						else return;
					}
				}
				else if( (m_uCheckBoxState & UISTATE_HOT) != 0 ) 
				{
					if( !m_sCheckBoxHotImage.IsEmpty() ) 
					{
						if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxHotImage, NULL, rcCheckBox) ) {}
						else return;
					}
				}
				else if( (m_uCheckBoxState & UISTATE_FOCUSED) != 0 ) 
				{
					if( !m_sCheckBoxFocusedImage.IsEmpty() ) 
					{
						if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxFocusedImage, NULL, rcCheckBox) ) {}
						else return;
					}
				}

				if( !m_sCheckBoxNormalImage.IsEmpty() ) 
				{
					if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxNormalImage, NULL, rcCheckBox) ) {}
					else return;
				}

Label_ForeImage:
				if( !m_sCheckBoxForeImage.IsEmpty() ) 
				{
					if( !DrawCheckBoxImage(hDC, (LPCTSTR)m_sCheckBoxForeImage, NULL, rcCheckBox) ) {}
				}
			}
		}
	}

	BOOL CListTextExElementUI::DrawCheckBoxImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify, RECT& rcCheckBox)
	{
		return CRenderEngine::DrawImageString(hDC, m_pManager, rcCheckBox, m_rcPaint, pStrImage, pStrModify);
	}

	void CListTextExElementUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("checkboxwidth")) == 0 ) SetCheckBoxWidth(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("checkboxheight")) == 0 ) SetCheckBoxHeight(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("checkboxnormalimage")) == 0 ) SetCheckBoxNormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("checkboxhotimage")) == 0 ) SetCheckBoxHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("checkboxpushedimage")) == 0 ) SetCheckBoxPushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("checkboxfocusedimage")) == 0 ) SetCheckBoxFocusedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("checkboxdisabledimage")) == 0 ) SetCheckBoxDisabledImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("checkboxselectedimage")) == 0 ) SetCheckBoxSelectedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("checkboxforeimage")) == 0 ) SetCheckBoxForeImage(pstrValue);
		else CListLabelElementUI::SetAttribute(pstrName, pstrValue);
	}
	LPCTSTR CListTextExElementUI::GetCheckBoxNormalImage()
	{
		return m_sCheckBoxNormalImage;
	}

	void CListTextExElementUI::SetCheckBoxNormalImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxNormalImage = pStrImage;
	}

	LPCTSTR CListTextExElementUI::GetCheckBoxHotImage()
	{
		return m_sCheckBoxHotImage;
	}

	void CListTextExElementUI::SetCheckBoxHotImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxHotImage = pStrImage;
	}

	LPCTSTR CListTextExElementUI::GetCheckBoxPushedImage()
	{
		return m_sCheckBoxPushedImage;
	}

	void CListTextExElementUI::SetCheckBoxPushedImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxPushedImage = pStrImage;
	}

	LPCTSTR CListTextExElementUI::GetCheckBoxFocusedImage()
	{
		return m_sCheckBoxFocusedImage;
	}

	void CListTextExElementUI::SetCheckBoxFocusedImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxFocusedImage = pStrImage;
	}

	LPCTSTR CListTextExElementUI::GetCheckBoxDisabledImage()
	{
		return m_sCheckBoxDisabledImage;
	}

	void CListTextExElementUI::SetCheckBoxDisabledImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxDisabledImage = pStrImage;
	}
	LPCTSTR CListTextExElementUI::GetCheckBoxSelectedImage()
	{
		return m_sCheckBoxSelectedImage;
	}

	void CListTextExElementUI::SetCheckBoxSelectedImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxSelectedImage = pStrImage;
	}
	LPCTSTR CListTextExElementUI::GetCheckBoxForeImage()
	{
		return m_sCheckBoxForeImage;
	}

	void CListTextExElementUI::SetCheckBoxForeImage(LPCTSTR pStrImage)
	{
		m_sCheckBoxForeImage = pStrImage;
	}

	bool CListTextExElementUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return true;
		DrawItemBk(hDC, m_rcItem);
		PaintStatusImage(hDC);
		DrawItemText(hDC, m_rcItem);
		return true;
	}
	void CListTextExElementUI::GetCheckBoxRect(int nIndex, RECT &rc)
	{
		memset(&rc, 0x00, sizeof(rc));
		int nItemHeight = m_rcItem.bottom - m_rcItem.top;
		rc.left = m_rcItem.left + 6;
		rc.top = m_rcItem.top + (nItemHeight - GetCheckBoxHeight()) / 2;
		rc.right = rc.left + GetCheckBoxWidth();
		rc.bottom = rc.top + GetCheckBoxHeight();
	}
	int CListTextExElementUI::GetCheckBoxWidth() const
	{
		return m_cxyCheckBox.cx;
	}

	void CListTextExElementUI::SetCheckBoxWidth(int cx)
	{
		if( cx < 0 ) return; 
		m_cxyCheckBox.cx = cx;
	}

	int CListTextExElementUI::GetCheckBoxHeight()  const 
	{
		return m_cxyCheckBox.cy;
	}

	void CListTextExElementUI::SetCheckBoxHeight(int cy)
	{
		if( cy < 0 ) return; 
		m_cxyCheckBox.cy = cy;
	}

	void CListTextExElementUI::SetCheck(BOOL bCheck)
	{
		if( m_bChecked == bCheck ) return;
		m_bChecked = bCheck;
		if( m_bChecked ) m_uCheckBoxState |= UISTATE_SELECTED;
		else m_uCheckBoxState &= ~UISTATE_SELECTED;
		Invalidate();
	}

	BOOL  CListTextExElementUI::GetCheck() const
	{
		return m_bChecked;
	}

	BOOL CListTextExElementUI::CheckColumEditable(int nColum)
	{
		return m_pOwner->CheckColumEditable(nColum);
	}

	void CListTextExElementUI::GetColumRect(int nColum, RECT &rc)
	{
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		RECT rcOwnerPos = m_pOwner->GetPos();

		rc.left = pInfo->rcColumn[nColum].left + 1;
		rc.top  = 1;
		rc.right = pInfo->rcColumn[nColum].right - 1;
		rc.bottom = m_rcItem.bottom - m_rcItem.top - 1;
		OffsetRect(&rc, -rcOwnerPos.left, m_rcItem.top - rcOwnerPos.top);
	}

	void CListTextExElementUI::SetColumItemColor(int nColum, DWORD iBKColor)
	{
		ColumCorlorArray[nColum].bEnable = TRUE;
		ColumCorlorArray[nColum].iBKColor = iBKColor;
		Invalidate();
	}

	BOOL CListTextExElementUI::GetColumItemColor(int nColum, DWORD& iBKColor)
	{
		if (!ColumCorlorArray[nColum].bEnable)
		{
			return FALSE;
		}
		iBKColor = ColumCorlorArray[nColum].iBKColor;
		return TRUE;
	}

} // namespace DuiLib

