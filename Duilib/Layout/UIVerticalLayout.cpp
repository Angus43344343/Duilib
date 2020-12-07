#include "StdAfx.h"
#include "UIVerticalLayout.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CVerticalLayoutUI)
	CVerticalLayoutUI::CVerticalLayoutUI() : m_iSepHeight(0), m_uButtonState(0), m_bImmMode(false), m_dwSepColor(0xAA000000)//zm
	{
		ptLastMouse.x = ptLastMouse.y = 0;
		::ZeroMemory(&m_rcNewPos, sizeof(m_rcNewPos));
	}

	LPCTSTR CVerticalLayoutUI::GetClass() const
	{
		return _T("VerticalLayoutUI");
	}

	LPVOID CVerticalLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_VERTICALLAYOUT) == 0 ) return static_cast<CVerticalLayoutUI*>(this);
		return CContainerUI::GetInterface(pstrName);
	}

	UINT CVerticalLayoutUI::GetControlFlags() const
	{
		if( IsEnabled() && m_iSepHeight != 0 ) return UIFLAG_SETCURSOR;
		else return 0;
	}

	void CVerticalLayoutUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		rc = m_rcItem;

		// Adjust for inset
		RECT m_rcInset = CVerticalLayoutUI::m_rcInset;
		GetManager()->GetDPIObj()->Scale(&m_rcInset);
		rc.left += m_rcInset.left;
		rc.top += m_rcInset.top;
		rc.right -= m_rcInset.right;
		rc.bottom -= m_rcInset.bottom + m_iSepHeight;//zm
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
		if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

		if( m_items.GetSize() == 0) 
		{
			ProcessScrollBar(rc, 0, 0);
			return;
		}

		// Determine the minimum size
		SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
		if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
			szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
			szAvailable.cy += m_pVerticalScrollBar->GetScrollRange();

		int cxNeeded = 0;
		int nAdjustables = 0;
		int cyFixed = 0;
		int nEstimateNum = 0;
		SIZE szControlAvailable;
		int iControlMaxWidth = 0;
		int iControlMaxHeight = 0;

		for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
			if( !pControl->IsVisible() ) continue;
			if( pControl->IsFloat() ) continue;
			szControlAvailable = szAvailable;
			RECT rcPadding = pControl->GetPadding();//控件外边距
			szControlAvailable.cx -= rcPadding.left + rcPadding.right;
			iControlMaxWidth = pControl->GetFixedWidth();
			iControlMaxHeight = pControl->GetFixedHeight();
			if (iControlMaxWidth <= 0) iControlMaxWidth = pControl->GetMaxWidth(); 
			if (iControlMaxHeight <= 0) iControlMaxHeight = pControl->GetMaxHeight();
			if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
			if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;
			SIZE sz = pControl->EstimateSize(szControlAvailable);//计算控件DPI
			if( sz.cy == 0 ) 
			{
				nAdjustables++;
			}
			else 
			{
				if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
				if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
			}
			cyFixed += sz.cy + pControl->GetPadding().top + pControl->GetPadding().bottom;//垂直布局方向所有子控件的高度相加

			sz.cx = MAX(sz.cx, 0);
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
			cxNeeded = MAX(cxNeeded, sz.cx + rcPadding.left + rcPadding.right);//因为是竖向布局,所以水平方向的大小取决于容器内控件的最大宽度
			nEstimateNum++;
		}
		cyFixed += (nEstimateNum - 1) * m_iChildPadding;//垂直布局高度：所有子控件高度 + 子控件间距 * (子控件数 - 1)

		// Place elements
		int cyNeeded = 0;
		int cyExpand = 0;
		if( nAdjustables > 0 ) cyExpand = MAX(0, (szAvailable.cy - cyFixed) / nAdjustables);
		// Position the elements
		SIZE szRemaining = szAvailable;
		int iPosY = rc.top;
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
		{
			iPosY -= m_pVerticalScrollBar->GetScrollPos();
		}
		else 
		{
			// 子控件垂直对其方式
			if(nAdjustables <= 0) 
			{
				UINT iChildAlign = GetChildVAlign(); 
				if (iChildAlign == DT_VCENTER) //居中 
				{
					iPosY += (szAvailable.cy -cyFixed) / 2;
				}
				else if (iChildAlign == DT_BOTTOM) //向下对齐
				{
					iPosY += (szAvailable.cy - cyFixed);
				}
			}
		}

		int iEstimate = 0;
		int iAdjustable = 0;
		int cyFixedRemaining = cyFixed;
		for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
			if( !pControl->IsVisible() ) continue;
			if( pControl->IsFloat() ) 
			{
				SetFloatPos(it2);
				continue;
			}

			iEstimate += 1;
			RECT rcPadding = pControl->GetPadding();
			szRemaining.cy -= rcPadding.top;

			szControlAvailable = szRemaining;
			szControlAvailable.cx -= rcPadding.left + rcPadding.right;
			iControlMaxWidth = pControl->GetFixedWidth();
			iControlMaxHeight = pControl->GetFixedHeight();
			if (iControlMaxWidth <= 0) iControlMaxWidth = pControl->GetMaxWidth(); 
			if (iControlMaxHeight <= 0) iControlMaxHeight = pControl->GetMaxHeight();
			if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
			if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;
			cyFixedRemaining = cyFixedRemaining - (rcPadding.top + rcPadding.bottom);
			if (iEstimate > 1) cyFixedRemaining = cyFixedRemaining - m_iChildPadding;//下一个控件位置(top)
			SIZE sz = pControl->EstimateSize(szControlAvailable);//计算控件DPI
			if( sz.cy == 0 ) //Y
			{
				iAdjustable++;
				sz.cy = cyExpand;
				// Distribute remaining to last element (usually round-off left-overs)
				if( iAdjustable == nAdjustables ) 
				{
					sz.cy = MAX(0, szRemaining.cy - rcPadding.bottom - cyFixedRemaining);
				} 
				if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
				if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
			}
			else 
			{
				if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
				if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
				cyFixedRemaining -= sz.cy;//计算DPI后的控件位置
			}

			sz.cx = MAX(sz.cx, 0);//X
			if( sz.cx == 0 ) sz.cx = szAvailable.cx - rcPadding.left - rcPadding.right;
			if( sz.cx > szControlAvailable.cx ) sz.cx = szControlAvailable.cx;
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();

			UINT iChildAlign = GetChildAlign(); 
			if (iChildAlign == DT_CENTER) 
			{
				int iPosX = (rc.right + rc.left) / 2;
				if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
				{
					iPosX += m_pHorizontalScrollBar->GetScrollRange() / 2;
					iPosX -= m_pHorizontalScrollBar->GetScrollPos();
				}
				RECT rcCtrl = { iPosX - sz.cx/2, iPosY + rcPadding.top, iPosX + sz.cx - sz.cx/2, iPosY + sz.cy + rcPadding.top };
				pControl->SetPos(rcCtrl, false);
			}
			else if (iChildAlign == DT_RIGHT) 
			{
				int iPosX = rc.right;
				if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
				{
					iPosX += m_pHorizontalScrollBar->GetScrollRange();
					iPosX -= m_pHorizontalScrollBar->GetScrollPos();
				}
				RECT rcCtrl = { iPosX - rcPadding.right - sz.cx, iPosY + rcPadding.top, iPosX - rcPadding.right, iPosY + sz.cy + rcPadding.top };
				pControl->SetPos(rcCtrl, false);
			}
			else 
			{
				int iPosX = rc.left;
				if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
				{
					iPosX -= m_pHorizontalScrollBar->GetScrollPos();
				}
				RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + rcPadding.left + sz.cx, iPosY + sz.cy + rcPadding.top };
				pControl->SetPos(rcCtrl, false);//设置控件位置
			}

			iPosY += sz.cy + m_iChildPadding + rcPadding.top + rcPadding.bottom;
			cyNeeded += sz.cy + rcPadding.top + rcPadding.bottom;//滚动条位置
			szRemaining.cy -= sz.cy + m_iChildPadding + rcPadding.bottom;
		}
		cyNeeded += (nEstimateNum - 1) * m_iChildPadding;

		// Process the scrollbar
		ProcessScrollBar(rc, cxNeeded, cyNeeded);
	}

	void CVerticalLayoutUI::DoPostPaint(HDC hDC, const RECT& rcPaint)
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 && m_bImmMode ) 
		{
			RECT rcSeparator = GetThumbRect(true);
			CRenderEngine::DrawColor(hDC, rcSeparator, m_dwSepColor);
		}
	}

	bool CVerticalLayoutUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		bool bReturn = CContainerUI::DoPaint(hDC, rcPaint, pStopControl);

		if (0 != m_iSepHeight)
		{
			RECT rcSeparator = GetThumbRect(true);
			CRenderEngine::DrawColor(hDC, rcSeparator, m_dwSepColor);
		}
		return bReturn;
	}

	void CVerticalLayoutUI::SetSepHeight(int iHeight)
	{
		m_iSepHeight = iHeight;
	}

	int CVerticalLayoutUI::GetSepHeight() const
	{
		return m_iSepHeight;
	}

	void CVerticalLayoutUI::SetSepImmMode(bool bImmediately)
	{
		if( m_bImmMode == bImmediately ) return;
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode && m_pManager != NULL ) 
		{
			m_pManager->RemovePostPaint(this);
		}

		m_bImmMode = bImmediately;
	}

	bool CVerticalLayoutUI::IsSepImmMode() const
	{
		return m_bImmMode;
	}

	void CVerticalLayoutUI::SetSepColor(DWORD dwSepColor)
	{
		m_dwSepColor = dwSepColor;
	}

	DWORD CVerticalLayoutUI::GetSepColor(DWORD dwSepColor)
	{
		return m_dwSepColor;
	}

	void CVerticalLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("sepheight")) == 0 ) SetSepHeight(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("sepimm")) == 0 ) SetSepImmMode(_tcsicmp(pstrValue, _T("true")) == 0);
		else if (_tcsicmp(pstrName, _T("sepcolor")) == 0)
		{
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetSepColor(clrColor);
		}
		else CContainerUI::SetAttribute(pstrName, pstrValue);
	}

	void CVerticalLayoutUI::DoEvent(TEventUI& event)
	{
		if( m_iSepHeight != 0 ) 
		{
			if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() )
			{
				RECT rcSeparator = GetThumbRect(false);
				if( ::PtInRect(&rcSeparator, event.ptMouse) ) 
				{
					m_uButtonState |= UISTATE_CAPTURED;
					ptLastMouse = event.ptMouse;
					m_rcNewPos = m_rcItem;
					if( !m_bImmMode && m_pManager ) m_pManager->AddPostPaint(this);
					return;
				}
			}
			if( event.Type == UIEVENT_BUTTONUP )
			{
				if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
				{
					m_uButtonState &= ~UISTATE_CAPTURED;
					m_rcItem = m_rcNewPos;
					if( !m_bImmMode && m_pManager ) m_pManager->RemovePostPaint(this);
					NeedParentUpdate();
					return;
				}
			}
			if( event.Type == UIEVENT_MOUSEMOVE )
			{
				if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
				{
					LONG cy = event.ptMouse.y - ptLastMouse.y;
					ptLastMouse = event.ptMouse;
					RECT rc = m_rcNewPos;
					if( m_iSepHeight >= 0 ) 
					{
						if( cy > 0 && event.ptMouse.y < m_rcNewPos.bottom + m_iSepHeight ) return;
						if( cy < 0 && event.ptMouse.y > m_rcNewPos.bottom ) return;
						rc.bottom += cy;
						if( rc.bottom - rc.top <= GetMinHeight() ) 
						{
							if( m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight() ) return;
							rc.bottom = rc.top + GetMinHeight();
						}
						if( rc.bottom - rc.top >= GetMaxHeight() ) 
						{
							if( m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight() ) return;
							rc.bottom = rc.top + GetMaxHeight();
						}
					}
					else 
					{
						if( cy > 0 && event.ptMouse.y < m_rcNewPos.top ) return;
						if( cy < 0 && event.ptMouse.y > m_rcNewPos.top + m_iSepHeight ) return;
						rc.top += cy;
						if( rc.bottom - rc.top <= GetMinHeight() ) 
						{
							if( m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight() ) return;
							rc.top = rc.bottom - GetMinHeight();
						}
						if( rc.bottom - rc.top >= GetMaxHeight() ) 
						{
							if( m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight() ) return;
							rc.top = rc.bottom - GetMaxHeight();
						}
					}

					CDuiRect rcInvalidate = GetThumbRect(true);
					m_rcNewPos = rc;
					m_cxyFixed.cy = GetManager()->GetDPIObj()->Scale(m_rcNewPos.bottom - m_rcNewPos.top);

					if( m_bImmMode ) //容器之间使用相对布局才会有此效果
					{
						m_rcItem = m_rcNewPos;
						NeedParentUpdate();//更新窗口,由于是相对布局,当前容器大小确定后,整体容器跟随改变
					}
					else 
					{
						rcInvalidate.Join(GetThumbRect(true));
						rcInvalidate.Join(GetThumbRect(false));
						if( m_pManager ) m_pManager->Invalidate(rcInvalidate);
					}
					return;
				}
			}

			if( event.Type == UIEVENT_SETCURSOR )
			{
				RECT rcSeparator = GetThumbRect(false);
				if( IsEnabled() && ::PtInRect(&rcSeparator, event.ptMouse) ) 
				{
					::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
					return;
				}
			}
		}
		CContainerUI::DoEvent(event);
	}

	RECT CVerticalLayoutUI::GetThumbRect(bool bUseNew) const
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 && bUseNew) 
		{
			if( m_iSepHeight >= 0 ) 
				return CDuiRect(m_rcNewPos.left, MAX(m_rcNewPos.bottom - m_iSepHeight, m_rcNewPos.top), 
				m_rcNewPos.right, m_rcNewPos.bottom);
			else 
				return CDuiRect(m_rcNewPos.left, m_rcNewPos.top, m_rcNewPos.right, 
				MIN(m_rcNewPos.top - m_iSepHeight, m_rcNewPos.bottom));
		}
		else 
		{
			if( m_iSepHeight >= 0 ) 
				return CDuiRect(m_rcItem.left, MAX(m_rcItem.bottom - m_iSepHeight, m_rcItem.top), m_rcItem.right, 
				m_rcItem.bottom);
			else 
				return CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.right, 
				MIN(m_rcItem.top - m_iSepHeight, m_rcItem.bottom));

		}
	}
}
