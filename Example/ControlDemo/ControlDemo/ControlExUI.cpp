#include "stdafx.h"
#include "ControlExUI.h"

//===================================================================================
IMPLEMENT_DUICONTROL(CCardUI)
CCardUI::CCardUI() : m_pobjLblImage(nullptr),
m_pobjLblName(nullptr),
m_pobjLblAge(nullptr),
m_pobjLblSex(nullptr),
m_pobjLblId(nullptr),
m_pobjLblBirthday(nullptr),
m_pobjLblTel(nullptr),
m_pobjLblCreateTime(nullptr)
{

}
//
void CCardUI::SetImage(LPCTSTR pstrValue)
{
	m_strImage = pstrValue;
	Invalidate();
}

void CCardUI::SetName(LPCTSTR pstrValue)
{
	m_strName = pstrValue;
	Invalidate();
}

void CCardUI::SetAge(LPCTSTR pstrValue)
{
	m_strAge = pstrValue;
	Invalidate();
}

void CCardUI::SetSex(LPCTSTR pstrValue)
{
	m_strSex = pstrValue;
	Invalidate();
}

void CCardUI::SetID(LPCTSTR pstrValue)
{
	m_strId = pstrValue;
	Invalidate();
}

void CCardUI::SetBirthday(LPCTSTR pstrValue)
{
	m_strBirthday = pstrValue;
	Invalidate();
}

void CCardUI::SetTel(LPCTSTR pstrValue)
{
	m_strTel = pstrValue;
	Invalidate();
}

void CCardUI::SetCreateTime(LPCTSTR pstrValue)
{
	m_strCreateTime = pstrValue;
	Invalidate();
}
//
LPCTSTR CCardUI::GetClass() const
{
	return _T("CCardUI");
}

LPVOID CCardUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CTL_CARD) == 0) return static_cast<CCardUI*>(this);
	return CContainerUI::GetInterface(pstrName);
}

UINT CCardUI::GetControlFlags() const
{
	return CContainerUI::GetControlFlags();
}

void CCardUI::DoInit()
{
	CDialogBuilder builder;
	CVerticalLayoutUI* pLabelEx = static_cast<CVerticalLayoutUI*>(builder.Create(_T("card.xml"), (UINT)0, NULL, m_pManager));
	_ASSERT(nullptr != pLabelEx);
	this->Add(pLabelEx);

	m_pobjLblImage = static_cast<CLabelUI*>(m_pManager->FindControl(_T("lblImage")));
	m_pobjLblName = static_cast<CLabelUI*>(m_pManager->FindControl(_T("lblName")));
	m_pobjLblAge = static_cast<CLabelUI*>(m_pManager->FindControl(_T("lblAge")));
	m_pobjLblSex = static_cast<CLabelUI*>(m_pManager->FindControl(_T("lblSex")));
	m_pobjLblId = static_cast<CLabelUI*>(m_pManager->FindControl(_T("lblID")));
	m_pobjLblBirthday = static_cast<CLabelUI*>(m_pManager->FindControl(_T("lblBirthday")));
	m_pobjLblTel = static_cast<CLabelUI*>(m_pManager->FindControl(_T("lblTel")));
	m_pobjLblCreateTime = static_cast<CLabelUI*>(m_pManager->FindControl(_T("lblCreateTime")));
}

void CCardUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcsicmp(pstrName, _T("Image")) == 0) SetImage(pstrValue);
	else if (_tcsicmp(pstrName, _T("Name")) == 0) SetName(pstrValue);
	else if (_tcsicmp(pstrName, _T("Age")) == 0) SetAge(pstrValue);
	else if (_tcsicmp(pstrName, _T("Sex")) == 0) SetSex(pstrValue);
	else if (_tcsicmp(pstrName, _T("ID")) == 0) SetID(pstrValue);
	else if (_tcsicmp(pstrName, _T("Birthday")) == 0) SetBirthday(pstrValue);
	else if (_tcsicmp(pstrName, _T("Tel")) == 0) SetTel(pstrValue);
	else if (_tcsicmp(pstrName, _T("CreateTime")) == 0) SetCreateTime(pstrValue);
	else CContainerUI::SetAttribute(pstrName, pstrValue);
}

bool CCardUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	CContainerUI::DoPaint(hDC, rcPaint, pStopControl);

	m_pobjLblImage->SetBkImage(m_strImage);
	m_pobjLblName->SetText(m_strName);
	m_pobjLblAge->SetText(m_strAge);
	m_pobjLblSex->SetText(m_strSex);
	m_pobjLblId->SetText(m_strId);
	m_pobjLblBirthday->SetText(m_strBirthday);
	m_pobjLblTel->SetText(m_strTel);
	m_pobjLblCreateTime->SetText(m_strCreateTime);

	return true;
}


