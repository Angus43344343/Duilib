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

		//容器添加子控件
		CControlUI* GetItemAt(int iIndex) const;
		int GetItemIndex(CControlUI* pControl) const;
		bool SetItemIndex(CControlUI* pControl, int iIndex);
		int GetCount() const;
		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);
		bool Remove(CControlUI* pControl);
		bool RemoveAt(int iIndex);
		void RemoveAll();

		//设置容器属性
		void SetVisible(bool bVisible = true);//容器可见
		void SetInternVisible(bool bVisible = true);//容器内子控件可见
		void SetEnabled(bool bEnabled);//容器使能
		void SetMouseEnabled(bool bEnable = true);//鼠标使能
		virtual bool IsMouseChildEnabled() const;
		virtual void SetMouseChildEnabled(bool bEnable = true);//容器内的子控件是否响应鼠标消息

		//容器布局
		virtual RECT GetInset() const;
		virtual void SetInset(RECT rcInset); // 设置内边距，相当于设置客户区
		virtual int GetChildPadding() const;
		virtual void SetChildPadding(int iPadding);//子控件距离
		virtual UINT GetChildAlign() const;
		virtual void SetChildAlign(UINT iAlign);//子控件水平方向位置
		virtual UINT GetChildVAlign() const;
		virtual void SetChildVAlign(UINT iVAlign);//子控件垂直方向位置

		//容器位置和绘制相关
		RECT GetClientPos() const;
		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);//滚动条与界面同步移动
		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		//延迟销毁
		virtual bool IsAutoDestroy() const;
		virtual void SetAutoDestroy(bool bAuto);
		virtual bool IsDelayedDestroy() const;
		virtual void SetDelayedDestroy(bool bDelayed);

		//关联绘制管理UIManager
		void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
		CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

		//事件消息处理
		void DoEvent(TEventUI& event);

		//处理list/combo/ect 
		virtual int FindSelectable(int iIndex, bool bForward = true) const;

		//滚动条
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

		//子类化控件属性设置
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
