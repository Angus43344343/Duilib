#ifndef _POPFRAME_H__
#define _POPFRAME_H__

#include "Duilib.h"

class CPopFrame : public WindowImplBase
{
public:
	LPCTSTR GetWindowClassName(void) const;
	CDuiString GetSkinFile();

	virtual void InitWindow() override;
	virtual void OnFinalMessage(HWND hWnd) override;
public:
	CPopFrame() = default;
	~CPopFrame() = default;
};

#endif /*_POPFRAME_H_-*/