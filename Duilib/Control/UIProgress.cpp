#include "StdAfx.h"
#include "UIProgress.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CProgressUI)

		CProgressUI::CProgressUI() : m_bShowText(false), m_bHorizontal(true), m_nMin(0), m_nMax(100), m_nValue(0), m_bStretchForeImage(true), m_bCircle(false), m_iCircleWidth(10)
	{
		m_uTextStyle = DT_SINGLELINE | DT_CENTER;
		SetFixedHeight(12);
	}

	LPCTSTR CProgressUI::GetClass() const
	{
		return _T("ProgressUI");
	}

	LPVOID CProgressUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_PROGRESS) == 0 ) return static_cast<CProgressUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}
	
	bool CProgressUI::IsShowText()
	{
		return m_bShowText;
	}

	void CProgressUI::SetShowText(bool bShowText)
	{
		if( m_bShowText == bShowText ) return;
		m_bShowText = bShowText;
		if (!m_bShowText) SetText(_T(""));
		else UpdateText();//zm
	}

	bool CProgressUI::IsHorizontal()
	{
		return m_bHorizontal;
	}

	void CProgressUI::SetHorizontal(bool bHorizontal)
	{
		if( m_bHorizontal == bHorizontal ) return;

		m_bHorizontal = bHorizontal;
		Invalidate();
	}

	int CProgressUI::GetMinValue() const
	{
		return m_nMin;
	}

	void CProgressUI::SetMinValue(int nMin)
	{
		m_nMin = nMin;
		Invalidate();
	}

	int CProgressUI::GetMaxValue() const
	{
		return m_nMax;
	}

	void CProgressUI::SetMaxValue(int nMax)
	{
		m_nMax = nMax;
		Invalidate();
	}

	int CProgressUI::GetValue() const
	{
		return m_nValue;
	}

	void CProgressUI::SetValue(int nValue)
	{
		if(nValue == m_nValue || nValue<m_nMin || nValue > m_nMax) 
		{
			return;
		}
		m_nValue = nValue;
		Invalidate();
		UpdateText();
	}

	void CProgressUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("hor")) == 0 ) SetHorizontal(_tcsicmp(pstrValue, _T("true")) == 0);
		if (_tcsicmp(pstrName, _T("circle")) == 0) SetCircle(_tcsicmp(pstrValue, _T("true")) == 0);//zm
		else if( _tcsicmp(pstrName, _T("min")) == 0 ) SetMinValue(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("max")) == 0 ) SetMaxValue(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("value")) == 0 ) SetValue(_ttoi(pstrValue));
		else if (_tcsicmp(pstrName, _T("circlewidth")) == 0) SetCircleWidth(_ttoi(pstrValue));//zm
		else if (_tcsicmp(pstrName, _T("showtext")) == 0) SetShowText(_tcsicmp(pstrValue, _T("true")) == 0);//zm
		else if( _tcsicmp(pstrName, _T("isstretchfore"))==0) SetStretchForeImage(_tcsicmp(pstrValue, _T("true")) == 0? true : false);
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CProgressUI::PaintForeColor(HDC hDC)
	{
		if(m_dwForeColor == 0) return;

		if( m_nMax <= m_nMin ) m_nMax = m_nMin + 1;
		if( m_nValue > m_nMax ) m_nValue = m_nMax;
		if( m_nValue < m_nMin ) m_nValue = m_nMin;

		RECT rc = m_rcItem;
		if( m_bHorizontal ) 
		{
			rc.right = m_rcItem.left + (m_nValue - m_nMin) * (m_rcItem.right - m_rcItem.left) / (m_nMax - m_nMin);
		}
		else if (m_bCircle)//zm
		{
			Gdiplus::Graphics g(hDC);
			g.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);//¿¹¾â³Ý

			RECT rcPos = GetPos();
			Gdiplus::SolidBrush brush(m_dwForeColor);
			Gdiplus::REAL nStartDegree = 90.0;
			Gdiplus::REAL nSweepDegree = 360.0f * (m_nValue * 1.0f / (m_nMax - m_nMin));
			g.FillPie(&brush, rcPos.left, rcPos.top, rcPos.right - rcPos.left, rcPos.bottom - rcPos.top, nStartDegree, nSweepDegree);

			DWORD dwBkColor = 0xFFFFFFFF;
			if (m_pManager != NULL) dwBkColor = m_pManager->GetRoot()->GetBkColor();
			Gdiplus::SolidBrush brushbk(dwBkColor);
			g.FillPie(&brushbk, rcPos.left + m_iCircleWidth, rcPos.top + m_iCircleWidth, rcPos.right - rcPos.left - m_iCircleWidth * 2, rcPos.bottom - rcPos.top - m_iCircleWidth * 2, 0, 360);

			g.ReleaseHDC(hDC);
		}
		else 
		{
			rc.bottom = m_rcItem.top + (m_rcItem.bottom - m_rcItem.top) * (m_nMax - m_nValue) / (m_nMax - m_nMin);
		
		}
		
		if (!m_bCircle) CRenderEngine::DrawColor(hDC, rc, GetAdjustColor(m_dwForeColor));//zm
	}

	void CProgressUI::PaintForeImage(HDC hDC)
	{
		if (m_bCircle) return;//zm

		if( m_nMax <= m_nMin ) m_nMax = m_nMin + 1;
		if( m_nValue > m_nMax ) m_nValue = m_nMax;
		if( m_nValue < m_nMin ) m_nValue = m_nMin;

		RECT rc = {0};
		if( m_bHorizontal ) 
		{
			rc.right = (LONG)((m_nValue - m_nMin) * (m_rcItem.right - m_rcItem.left) * 1.0f / (m_nMax - m_nMin));
			rc.bottom = m_rcItem.bottom - m_rcItem.top;
		}
		else 
		{
			rc.top = (LONG)((m_rcItem.bottom - m_rcItem.top) * (m_nMax - m_nValue) * 1.0f / (m_nMax - m_nMin));
			rc.right = m_rcItem.right - m_rcItem.left;
			rc.bottom = m_rcItem.bottom - m_rcItem.top;
		}

		if( !m_sForeImage.IsEmpty() ) 
		{
			m_sForeImageModify.Empty();
			int sw = MulDiv(rc.right - rc.left, 100, GetManager()->GetDPIObj()->GetScale());
			int sh = MulDiv(rc.bottom - rc.top, 100, GetManager()->GetDPIObj()->GetScale());
			rc.left = MulDiv(rc.left, 100, GetManager()->GetDPIObj()->GetScale());
			rc.top = MulDiv(rc.top, 100, GetManager()->GetDPIObj()->GetScale());
			rc.right = rc.left + sw;
			rc.bottom = rc.top + sh;
			if (m_bStretchForeImage) 
			{
				m_sForeImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rc.left, rc.top, rc.right, rc.bottom);
			}
			else 
			{
				m_sForeImageModify.SmallFormat(_T("dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), rc.left, rc.top, rc.right, rc.bottom, rc.left, rc.top, rc.right, rc.bottom);
			}

			if( !DrawImage(hDC, (LPCTSTR)m_sForeImage, (LPCTSTR)m_sForeImageModify) ) {}
			else return;
		}
	}

	bool CProgressUI::IsStretchForeImage()
	{
		return m_bStretchForeImage;
	}

	void CProgressUI::SetStretchForeImage( bool bStretchForeImage /*= true*/ )
	{
		if (m_bStretchForeImage==bStretchForeImage)		return;
		m_bStretchForeImage=bStretchForeImage;
		Invalidate();
	}

	void CProgressUI::UpdateText()
	{
		if(m_bShowText) 
		{
			CDuiString sText;
			sText.Format(_T("%.0f%%"), (m_nValue - m_nMin) * 100.0f / (m_nMax - m_nMin));
			SetText(sText);
		}
	}

	void CProgressUI::SetCircle(bool bCircle)
	{
		if (m_bCircle == bCircle) return;

		m_bCircle = bCircle;
		if (m_bCircle) m_bHorizontal = false;

		Invalidate();
	}

	bool CProgressUI::IsCircle()
	{
		return m_bCircle;
	}

	void CProgressUI::SetCircleWidth(int iWidth)
	{
		m_iCircleWidth = iWidth;
	}

	int CProgressUI::GetCircleWidth()
	{
		return m_iCircleWidth;
	}

	void CProgressUI::PaintBkColor(HDC hDC)
	{
		if (m_bCircle)
		{
			Gdiplus::Graphics g(hDC);
			g.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);//¿¹¾â³Ý

			RECT rcPos = GetPos();
			Gdiplus::SolidBrush brush(m_dwBackColor);
			g.FillPie(&brush, rcPos.left, rcPos.top, rcPos.right - rcPos.left, rcPos.bottom - rcPos.top, 0, 360);
			g.ReleaseHDC(hDC);
		}
		else
		{
			CControlUI::PaintBkColor(hDC);
		}
	}
}
