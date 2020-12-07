#ifndef __UITEXTSCROLLH__
#define __UITEXTSCROLLH__

#pragma once

namespace DuiLib
{
	#define ROLLTEXT_LEFT		0
	#define ROLLTEXT_RIGHT		1
	#define ROLLTEXT_UP 		2
	#define ROLLTEXT_DOWN		3


	#define ROLLTEXT_TIMERID			20
	#define ROLLTEXT_TIMERID_SPAN		500U
	#define ROLLTEXT_TIMERID_STATE		1

	#define ROLLTEXT_ROLL_END			21
	#define ROLLTEXT_ROLL_END_SPAN		1000*6U
	#define ROLLTEXT_ROLL_END_STATE		2

	class UILIB_API CRollTextUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CRollTextUI)
	public:
		CRollTextUI(void);
		~CRollTextUI(void);

	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

	public:
		//zm
		BOOL IsUseRoll();
		void SetUseRoll(BOOL bUseRoll);
		int GetRollDirection();
		void SetRollDirection(int iRollDirction);
		UINT GetTimeSpan();
		void SetTimeSpan(UINT uiTimeSpan);
		UINT GetTimeEndSpan();
		void SetTimeEndSpan(UINT uiTimeEndSpan);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		//zm

		virtual void PaintText(HDC hDC);
		virtual void DoEvent(TEventUI& event);
		virtual void SetPos(RECT rc);
		virtual void SetText(LPCTSTR pstrText);

	public:
		void BeginRoll(LONG lTimeSpan = ROLLTEXT_TIMERID_SPAN, LONG lMaxTimeLimited = ROLLTEXT_ROLL_END_SPAN);
		void EndRoll();
		
	private:    
		int m_nStep;
		int m_nScrollPos;
		BOOL m_bUseRoll; 
		int m_nText_W_H;

		UINT m_uiTimeSpan;
		UINT m_uiTimeEndSpan;

		int m_nRollState; 
		int m_nRollDirection;
	};

}	// namespace DuiLib

#endif // __UITEXTSCROLLH__