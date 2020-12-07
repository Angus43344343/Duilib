#ifndef __UICONTAINER_H__
#define __UICONTAINER_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class IContainerUI
	{
	public:
		virtual CControlUI* GetItemAt(int iIndex) const = 0;
		virtual int GetItemIndex(CControlUI* pControl) const  = 0;
		virtual bool SetItemIndex(CControlUI* pControl, int iIndex)  = 0;
		virtual int GetCount() const = 0;
		virtual bool Add(CControlUI* pControl) = 0;
		virtual bool AddAt(CControlUI* pControl, int iIndex)  = 0;
		virtual bool Remove(CControlUI* pControl) = 0;
		virtual bool RemoveAt(int iIndex)  = 0;
		virtual void RemoveAll() = 0;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//
	class CScrollBarUI;

	class UILIB_API CContainerUI : public CControlUI, public IContainerUI
	{
		DECLARE_DUICONTROL(CContainerUI)

	public:
		CContainerUI();
		virtual ~CContainerUI();

	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		//��������ӿؼ�
		CControlUI* GetItemAt(int iIndex) const;
		int GetItemIndex(CControlUI* pControl) const;
		bool SetItemIndex(CControlUI* pControl, int iIndex);
		int GetCount() const;
		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);
		bool Remove(CControlUI* pControl);
		bool RemoveAt(int iIndex);
		void RemoveAll();

		//������������
		void SetVisible(bool bVisible = true);//�����ɼ�
		void SetInternVisible(bool bVisible = true);//�������ӿؼ��ɼ�
		void SetEnabled(bool bEnabled);//����ʹ��
		void SetMouseEnabled(bool bEnable = true);//���ʹ��
		virtual bool IsMouseChildEnabled() const;
		virtual void SetMouseChildEnabled(bool bEnable = true);//�����ڵ��ӿؼ��Ƿ���Ӧ�����Ϣ

		//��������
		virtual RECT GetInset() const;
		virtual void SetInset(RECT rcInset); // �����ڱ߾࣬�൱�����ÿͻ���
		virtual int GetChildPadding() const;
		virtual void SetChildPadding(int iPadding);//�ӿؼ�����
		virtual UINT GetChildAlign() const;
		virtual void SetChildAlign(UINT iAlign);//�ӿؼ�ˮƽ����λ��
		virtual UINT GetChildVAlign() const;
		virtual void SetChildVAlign(UINT iVAlign);//�ӿؼ���ֱ����λ��

		//����λ�úͻ������
		RECT GetClientPos() const;
		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);//�����������ͬ���ƶ�
		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		//�ӳ�����
		virtual bool IsAutoDestroy() const;
		virtual void SetAutoDestroy(bool bAuto);
		virtual bool IsDelayedDestroy() const;
		virtual void SetDelayedDestroy(bool bDelayed);

		//�������ƹ���UIManager
		void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
		CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

		//�¼���Ϣ����
		void DoEvent(TEventUI& event);

		//����list/combo/ect 
		virtual int FindSelectable(int iIndex, bool bForward = true) const;

		//������
		virtual SIZE GetScrollPos() const;
		virtual SIZE GetScrollRange() const;
		virtual void SetScrollPos(SIZE szPos, bool bMsg = true);
		virtual void SetScrollStepSize(int nSize);
		virtual int GetScrollStepSize() const;
		virtual void LineUp();
		virtual void LineDown();
		virtual void PageUp();
		virtual void PageDown();
		virtual void HomeUp();
		virtual void EndDown();
		virtual void LineLeft();
		virtual void LineRight();
		virtual void PageLeft();
		virtual void PageRight();
		virtual void HomeLeft();
		virtual void EndRight();
		virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
		virtual CScrollBarUI* GetVerticalScrollBar() const;
		virtual CScrollBarUI* GetHorizontalScrollBar() const;

		//���໯�ؼ���������
		bool SetSubControlText(LPCTSTR pstrSubControlName, LPCTSTR pstrText);
		bool SetSubControlFixedHeight(LPCTSTR pstrSubControlName, int cy);
		bool SetSubControlFixedWdith(LPCTSTR pstrSubControlName, int cx);
		bool SetSubControlUserData(LPCTSTR pstrSubControlName, LPCTSTR pstrText);
		CDuiString GetSubControlText(LPCTSTR pstrSubControlName);
		int GetSubControlFixedHeight(LPCTSTR pstrSubControlName);
		int GetSubControlFixedWdith(LPCTSTR pstrSubControlName);
		const CDuiString GetSubControlUserData(LPCTSTR pstrSubControlName);
		CControlUI* FindSubControl(LPCTSTR pstrSubControlName);
	protected:
		virtual void SetFloatPos(int iIndex);
		virtual void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired);

	protected:
		CStdPtrArray m_items;
		RECT m_rcInset;
		int m_iChildPadding;
		UINT m_iChildAlign;
		UINT m_iChildVAlign;
		bool m_bAutoDestroy;
		bool m_bDelayedDestroy;
		bool m_bMouseChildEnabled;
		int	 m_nScrollStepSize;

		CScrollBarUI* m_pVerticalScrollBar;
		CScrollBarUI* m_pHorizontalScrollBar;
		CDuiString	m_sVerticalScrollBarStyle;
		CDuiString	m_sHorizontalScrollBarStyle;
	};

} // namespace DuiLib

#endif // __UICONTAINER_H__
