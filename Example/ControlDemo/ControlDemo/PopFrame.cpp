#include "stdafx.h"
#include "PopFrame.h"

LPCTSTR CPopFrame::GetWindowClassName(void) const
{
	return _T("CPopFrame");
}

CDuiString CPopFrame::GetSkinFile()
{
	return _T("popWnd.xml");
}

void CPopFrame::InitWindow()
{

}

void CPopFrame::OnFinalMessage(HWND hWnd)
{
	delete this;
}