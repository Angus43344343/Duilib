#include "stdafx.h"
#include "UIRollText.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CRollTextUI)

	CRollTextUI::CRollTextUI(void)
	{
		m_nScrollPos = 0;
		m_nText_W_H = 0;
		m_nStep = 5;
		m_bUseRoll = FALSE;
		m_nRollDirection = ROLLTEXT_LEFT;

		m_nRollState = 0;
		m_uiTimeSpan = ROLLTEXT_TIMERID_SPAN;
		m_uiTimeEndSpan = ROLLTEXT_ROLL_END_SPAN;
	}

	CRollTextUI::~CRollTextUI(void)
	{
		EndRoll();//zm
		//m_pManager->KillTimer(this, ROLLTEXT_ROLL_END);
		//m_pManager->KillTimer(this, ROLLTEXT_TIMERID);
	}

	LPCTSTR CRollTextUI::GetClass() const
	{
		return _T("RollTextUI");
	}

	LPVOID CRollTextUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("RollText")) == 0 ) return static_cast<CRollTextUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	BOOL CRollTextUI::IsUseRoll()
	{
		return m_bUseRoll;
	}

	void CRollTextUI::SetUseRoll(BOOL bUseRoll)
	{
		if (m_bUseRoll == bUseRoll) return;
		m_bUseRoll = bUseRoll;
		BeginRoll();
	}

	int CRollTextUI::GetRollDirection()
	{
		return m_nRollDirection;
	}

	void CRollTextUI::SetRollDirection(int iRollDirction)
	{
		m_nRollDirection = iRollDirction;
		Invalidate();
	}

	UINT CRollTextUI::GetTimeSpan()
	{
		return m_uiTimeSpan;
	}

	void CRollTextUI::SetTimeSpan(UINT uiTimeSpan)
	{
		m_nRollState |= ROLLTEXT_TIMERID_STATE;
		KillTimer(ROLLTEXT_TIMERID);
		SetTimer(ROLLTEXT_TIMERID, uiTimeSpan);
	}

	UINT CRollTextUI::GetTimeEndSpan()
	{
		return m_uiTimeEndSpan;
	}

	void CRollTextUI::SetTimeEndSpan(UINT uiTimeEndSpan)
	{
		m_nRollState |= ROLLTEXT_ROLL_END_STATE;
		KillTimer(ROLLTEXT_ROLL_END);
		SetTimer(ROLLTEXT_ROLL_END, uiTimeEndSpan);
	}

	void CRollTextUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcsicmp(pstrName, _T("roll")) == 0)
		{
			if (_tcsicmp(pstrValue, _T("true")) == 0) SetUseRoll(TRUE);
		}
		else if (_tcsicmp(pstrName, _T("direction")) == 0)
		{
			if (_tcsstr(pstrValue, _T("left")) != NULL)
			{
				SetRollDirection(ROLLTEXT_LEFT);
			}
			else if (_tcsstr(pstrValue, _T("right")) != NULL)
			{
				SetRollDirection(ROLLTEXT_RIGHT);
			}
			else if (_tcsstr(pstrValue, _T("top")) != NULL)
			{
				SetRollDirection(ROLLTEXT_UP);
			}
			else if (_tcsstr(pstrValue, _T("bottom")) != NULL)
			{
				SetRollDirection(ROLLTEXT_DOWN);
			}
		}
		else if (_tcsicmp(pstrName, _T("timespan")) == 0)
		{
			SetTimeSpan(_ttoi(pstrValue));
		}
		else if (_tcsicmp(pstrName, _T("timeendspan")) == 0)
		{
			SetTimeEndSpan(_ttoi(pstrValue));
		}
		else __super::SetAttribute(pstrName, pstrValue);
	}

	void CRollTextUI::BeginRoll(LONG lTimeSpan, LONG lMaxTimeLimited)
	{
		if (m_bUseRoll)
		{
			m_nText_W_H = 0;

			if (0 == (m_nRollState & ROLLTEXT_TIMERID_STATE))
			{
				SetTimer(ROLLTEXT_TIMERID, lTimeSpan);
			}

			if (0 == (m_nRollState & ROLLTEXT_ROLL_END_STATE))
			{
				SetTimer(ROLLTEXT_ROLL_END, lMaxTimeLimited);
			}
		}
		//m_nRollDirection = nDirect;
		//if (m_bUseRoll)
		//{
		//	EndRoll();
		//}
		//m_nText_W_H = 0;
	
		//m_pManager->KillTimer(this, ROLLTEXT_TIMERID);
		//m_pManager->SetTimer(this, ROLLTEXT_TIMERID, lTimeSpan);
	
		//m_pManager->KillTimer(this, ROLLTEXT_ROLL_END);
		//m_pManager->SetTimer(this, ROLLTEXT_ROLL_END, lMaxTimeLimited*1000);

		//m_bUseRoll = TRUE;
	}

	void CRollTextUI::EndRoll()
	{
		if (!m_bUseRoll) return;

		KillTimer(ROLLTEXT_ROLL_END);
		KillTimer(ROLLTEXT_TIMERID);
		
		m_bUseRoll = FALSE;
		Invalidate();
	}

	void CRollTextUI::SetPos(RECT rc)
	{
		CLabelUI::SetPos(rc);
		m_nText_W_H = 0;			//布局变化重新计算
	}

	void CRollTextUI::SetText( LPCTSTR pstrText )
	{
		CLabelUI::SetText(pstrText);
		m_nText_W_H = 0;			//文本变化重新计算
	}

	void CRollTextUI::DoEvent(TEventUI& event)
	{
		if (event.Type == UIEVENT_TIMER && event.wParam == ROLLTEXT_ROLL_END)
		{
			KillTimer(ROLLTEXT_ROLL_END);
			m_pManager->SendNotify(this, DUI_MSGTYPE_TEXTROLLEND);
		}
		else if( event.Type == UIEVENT_TIMER && event.wParam == ROLLTEXT_TIMERID ) 
		{
			Invalidate();
			return;
		}
		CLabelUI::DoEvent(event);
	}

	void CRollTextUI::PaintText(HDC hDC)
	{
		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();
		DWORD dwTextColor = IsEnabled() ? m_dwTextColor : m_dwDisabledTextColor;
		CDuiString sText = GetText();
		if( sText.IsEmpty() ) return;

		RECT rcTextPadding = GetTextPadding();
		CDuiRect  rcClient;
		rcClient = m_rcItem;
		rcClient.left += rcTextPadding.left;
		rcClient.right -= rcTextPadding.right;
		rcClient.top += rcTextPadding.top;
		rcClient.bottom -= rcTextPadding.bottom;

		RECT rc = rcClient;
		UINT uTextStyle = DT_WORDBREAK | DT_EDITCONTROL;

		if (m_bUseRoll)
		{
			if (m_nText_W_H > 0)
			{
				int nScrollRange = 0;

				if (m_nRollDirection == ROLLTEXT_LEFT || m_nRollDirection == ROLLTEXT_RIGHT)
				{	//左面移动
					nScrollRange = m_nText_W_H + rcClient.GetWidth();

					rcClient.Offset((m_nRollDirection == ROLLTEXT_LEFT ? rcClient.GetWidth() : -rcClient.GetWidth()), 0);
					rcClient.Offset((m_nRollDirection == ROLLTEXT_LEFT ? -m_nScrollPos : m_nScrollPos), 0);
					rcClient.right += (m_nText_W_H - rcClient.GetWidth());
				}
				else
				{	//上下移动
					nScrollRange = m_nText_W_H + rcClient.GetHeight();
					rcClient.Offset(0, (m_nRollDirection == ROLLTEXT_UP ? rcClient.GetHeight() : -rcClient.GetHeight()));
					rcClient.Offset(0, (m_nRollDirection == ROLLTEXT_UP ? -m_nScrollPos : m_nScrollPos));
					rcClient.bottom += (m_nText_W_H - rcClient.GetHeight());
				}

				m_nScrollPos += m_nStep;
				if (m_nScrollPos > nScrollRange)
				{
					m_nScrollPos = 0;
				}
			}

			rc = rcClient;
			//UINT uTextStyle = DT_WORDBREAK | DT_EDITCONTROL;
			if (m_nText_W_H == 0)
			{
				uTextStyle |= DT_CALCRECT;				//第一次计算文本宽度或高度
				if (m_nRollDirection == ROLLTEXT_LEFT || m_nRollDirection == ROLLTEXT_RIGHT)
				{	//左右移动
					rc.right += 10000;
				}
				else
				{	//上下移动
					rc.bottom += 10000;
				}
			}
		}
		
		if (m_bShowHtml)
		{
			int nLinks = 0;
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText, dwTextColor, NULL, NULL, nLinks, m_iFont, uTextStyle);
		}
		else
		{
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, dwTextColor, m_iFont, uTextStyle);
		}

		if ((m_bUseRoll) && (m_nText_W_H == 0))
		{
			m_nText_W_H = (m_nRollDirection == ROLLTEXT_LEFT || m_nRollDirection == ROLLTEXT_RIGHT) ? (rc.right - rc.left) : (rc.bottom - rc.top);//计算文本宽度或高度
		}
	}
}