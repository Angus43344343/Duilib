#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#pragma once
namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CControlUI;
	class CRichEditUI;

	// Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP       0x00000001//TAB
#define UIFLAG_SETCURSOR     0x00000002//光标
#define UIFLAG_WANTRETURN    0x00000004//Enter

	// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_UPDATETEST    0x00000008
#define UIFIND_TOP_FIRST     0x00000010//从最上层的控件开始查找;有可能多个控件的位置有覆盖的情况,最上层的控件即是最后绘制的控件.
#define UIFIND_ME_FIRST      0x80000000//从当前控件开始查找

	// Flags used for controlling the paint
#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//字体
	typedef struct UILIB_API tagTFontInfo
	{
		HFONT hFont;
		CDuiString sFontName;
		int iSize;
		bool bBold;//加粗
		bool bUnderline;
		bool bItalic;//斜体
		TEXTMETRIC tm;//系统字体结构
	} TFontInfo;

	//图片
	typedef struct UILIB_API tagTImageInfo
	{
		HBITMAP hBitmap;
		LPBYTE pBits;
		LPBYTE pSrcBits;
		int nX;
		int nY;
		bool bAlpha;//透明通道
		bool bUseHSL;//HSL
		CDuiString sResType;
		DWORD dwMask;//设置透明度
	} TImageInfo;

	//绘制信息：贴图描述
	typedef struct UILIB_API tagTDrawInfo
	{
		tagTDrawInfo();
		void Parse(LPCTSTR pStrImage, LPCTSTR pStrModify, CPaintManagerUI *pManager);
		void Clear();

		CDuiString sDrawString;
		CDuiString sDrawModify;
		CDuiString sImageName;
		CDuiString sResType;
		RECT rcDest;
		RECT rcSource;
		RECT rcCorner;
		DWORD dwMask;
		BYTE uFade;
		bool bHole;
		bool bTiledX;
		bool bTiledY;
		bool bHSL;
		SIZE szIcon;
		CDuiString sIconAlign;
	} TDrawInfo;

	//绘制区域
	typedef struct UILIB_API tagTPercentInfo
	{
		double left;
		double top;
		double right;
		double bottom;
	} TPercentInfo;

	//资源信息
	typedef struct UILIB_API tagTResInfo
	{
		DWORD m_dwDefaultDisabledColor;
		DWORD m_dwDefaultFontColor;
		DWORD m_dwDefaultLinkFontColor;
		DWORD m_dwDefaultLinkHoverFontColor;
		DWORD m_dwDefaultSelectedBkColor;
		TFontInfo m_DefaultFontInfo;
		CStdStringPtrMap m_CustomFonts;	//字体管理表
		CStdStringPtrMap m_ImageHash;	//图片管理表
		CStdStringPtrMap m_AttrHash;	//属性管理表
		CStdStringPtrMap m_StyleHash;	//样式管理表
		CStdStringPtrMap m_DrawInfoHash;//绘制信息管理表
	} TResInfo;

	// Structure for notifications from the system
	// to the control implementation.
	//事件消息
	typedef struct UILIB_API tagTEventUI
	{
		int Type;
		CControlUI* pSender;
		DWORD dwTimestamp;
		POINT ptMouse;
		TCHAR chKey;
		WORD wKeyState;
		WPARAM wParam;
		LPARAM lParam;
	} TEventUI;

	// Drag&Drop control
	const TCHAR* const CF_MOVECONTROL = _T("CF_MOVECONTROL");

	typedef struct UILIB_API tagTCFMoveUI
	{
		CControlUI* pControl;
	} TCFMoveUI;

	// Listener interface
	class INotifyUI
	{
	public:
		virtual void Notify(TNotifyUI& msg) = 0;
	};

	// MessageFilter interface
	class IMessageFilterUI
	{
	public:
		virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
	};

	//处理加速键值 
	class ITranslateAccelerator
	{
	public:
		virtual LRESULT TranslateAccelerator(MSG *pMsg) = 0;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//
	typedef CControlUI* (*LPCREATECONTROL)(LPCTSTR pstrType);

	class UILIB_API CPaintManagerUI : public CIDropTarget 
	{
	public:
		CPaintManagerUI();
		~CPaintManagerUI();

	public:
//绘制管理===============================================================================
		//初始化绘制信息
		void Init(HWND hWnd, LPCTSTR pstrName = NULL);
		bool IsUpdateNeeded() const;
		void NeedUpdate();
		void Invalidate();
		void Invalidate(RECT& rcItem);//使绘制区域无效,发送WM_PAINT重新绘制

		LPCTSTR GetName() const;//绘制管理名,可不设置
		HDC GetPaintDC() const;
		HWND GetPaintWindow() const;//需绘制窗口句柄
		HWND GetTooltipWindow() const;//悬停窗口
		CShadowUI* GetShadow();//获取阴影窗口

		//资源加载
		static HINSTANCE GetInstance();
		static void SetInstance(HINSTANCE hInst);//程序句柄
		static CDuiString GetInstancePath();
		static CDuiString GetCurrentPath();
		static void SetCurrentPath(LPCTSTR pStrPath);//当前路径
		static HINSTANCE GetResourceDll();
		static void SetResourceDll(HINSTANCE hInst);//资源插件
		static const CDuiString& GetResourcePath();
		static void SetResourcePath(LPCTSTR pStrPath);//资源路径
		static const CDuiString& GetResourceZip();
		static const CDuiString& GetResourceZipPwd();//ZIP资源,包含加密ZIP
		static void SetResourceZip(LPVOID pVoid, unsigned int len, LPCTSTR password = NULL);
		static void SetResourceZip(LPCTSTR pstrZip, bool bCachedResourceZip = false, LPCTSTR password = NULL);
		static bool IsCachedResourceZip();
		static HANDLE GetResourceZipHandle();//ZIP文件句柄
		static void SetResourceType(int nType);
		static int GetResourceType();//资源类型
		static void ReloadSkin();//重载资源
		static CPaintManagerUI* GetPaintManager(LPCTSTR pstrName);
		static CStdPtrArray* GetPaintManagers();//获取
		static bool LoadPlugin(LPCTSTR pstrModuleName);
		static CStdPtrArray* GetPlugins();//动态链接函数接口
		bool IsForceUseSharedRes() const;
		void SetForceUseSharedRes(bool bForce);//共享资源

		//window:整体窗体信息设置  Window
		int GetHoverTime() const;
		void SetHoverTime(int iTime);//设置悬停时间,与悬停窗口对应
		SIZE GetClientSize() const;
		SIZE GetInitSize();
		void SetInitSize(int cx, int cy);//初始窗体大小
		RECT& GetSizeBox();
		void SetSizeBox(RECT& rcSizeBox);//用来拉伸窗体的箭头区域
		RECT& GetCaptionRect();
		void SetCaptionRect(RECT& rcCaption);//设置标题栏
		SIZE GetRoundCorner() const;
		void SetRoundCorner(int cx, int cy);//窗口圆角
		SIZE GetMinInfo() const;
		void SetMinInfo(int cx, int cy);//最小窗体
		SIZE GetMaxInfo() const;
		void SetMaxInfo(int cx, int cy);//最大窗体
		bool IsShowUpdateRect() const;
		void SetShowUpdateRect(bool show);//设置显示脏区(更新区域)
		void SetUseGdiplusText(bool bUse);
		bool IsUseGdiplusText() const;//GDI+绘制字体,默认开启,缺点是效率低
		void SetGdiplusTextRenderingHint(int trh);
		int GetGdiplusTextRenderingHint() const;//GDI+字体边缘平滑处理:特别是字体放大出现锯齿
		bool IsNoActivate();
		void SetNoActivate(bool bNoActivate);//

		//分层窗口：用来做异形透明窗口,让界面更炫酷
		BYTE GetOpacity() const;
		void SetOpacity(BYTE nOpacity);//设置透明度
		bool IsLayered();
		void SetLayered(bool bLayered);//设置分层窗口
		RECT& GetLayeredInset();
		void SetLayeredInset(RECT& rcLayeredInset);//分层窗口的内边矩
		BYTE GetLayeredOpacity();
		void SetLayeredOpacity(BYTE nOpacity);//分层窗口的透明度
		LPCTSTR GetLayeredImage();
		void SetLayeredImage(LPCTSTR pstrImage);//分层窗口的背景图

		//字体管理:m_SharedResInfo.m_CustomFonts和m_ResInfo.m_CustomFonts  Font
		DWORD GetDefaultDisabledColor() const;
		void SetDefaultDisabledColor(DWORD dwColor, bool bShared = false);//默认禁用字体颜色
		DWORD GetDefaultFontColor() const;
		void SetDefaultFontColor(DWORD dwColor, bool bShared = false);//默认字体颜色
		DWORD GetDefaultLinkFontColor() const;
		void SetDefaultLinkFontColor(DWORD dwColor, bool bShared = false);//默认超链接字体颜色 
		DWORD GetDefaultLinkHoverFontColor() const;
		void SetDefaultLinkHoverFontColor(DWORD dwColor, bool bShared = false);//默认鼠标悬停在超链接字体颜色 
		DWORD GetDefaultSelectedBkColor() const;
		void SetDefaultSelectedBkColor(DWORD dwColor, bool bShared = false);//默认选中字体背景色
		TFontInfo* GetDefaultFontInfo();//默认字体信息
		void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
		DWORD GetCustomFontCount(bool bShared = false) const;//自定义字体数
		void AddFontArray(LPCTSTR pstrPath);//添加字体
		HFONT AddFont(int id, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
		HFONT GetFont(int id);//获取字体列表中的字体
		HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
		int GetFontIndex(HFONT hFont, bool bShared = false);//获取字体在字体列表序号
		int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
		void RemoveFont(HFONT hFont, bool bShared = false);//移除字体
		void RemoveFont(int id, bool bShared = false);
		void RemoveAllFonts(bool bShared = false);
		TFontInfo* GetFontInfo(int id);
		TFontInfo* GetFontInfo(HFONT hFont);//获取字体信息
		void RebuildFont(TFontInfo* pFontInfo);

		//图片管理:m_ResInfo.m_ImageHash和m_SharedResInfo.m_ImageHash Image
		static bool GetHSL(short* H, short* S, short* L);//没有使用此功能
		static void SetHSL(bool bUseHSL, short H, short S, short L); // H:0~360, S:0~200, L:0~200 
		const TImageInfo* GetImage(LPCTSTR bitmap);
		const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0, bool bUseHSL = false, HINSTANCE instance = NULL);
		const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0, bool bUseHSL = false, bool bShared = false, HINSTANCE instance = NULL);
		const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha, bool bShared = false);
		void RemoveImage(LPCTSTR bitmap, bool bShared = false);
		void RemoveAllImages(bool bShared = false);
		static void ReloadSharedImages();
		void ReloadImages();
		const TImageInfo* GetImageString(LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);

		//绘制信息管理:m_ResInfo.m_DrawInfoHash和m_SharedResInfo.m_DrawInfoHash,图片的贴片描述
		bool IsPainting();
		void SetPainting(bool bIsPainting);
		const TDrawInfo* GetDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify);
		void RemoveDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify);
		void RemoveAllDrawInfos(bool bShared = false);

		//添加默认属性列表:m_SharedResInfo.m_AttrHash和m_ResInfo.m_AttrHash  Default
		void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList, bool bShared = false);
		LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
		bool RemoveDefaultAttributeList(LPCTSTR pStrControlName, bool bShared = false);
		void RemoveAllDefaultAttributeList(bool bShared = false);

		//添加自定义属性列表:m_mWindowCustomAttrHash
		void AddWindowCustomAttribute(LPCTSTR pstrName, LPCTSTR pstrAttr);
		LPCTSTR GetWindowCustomAttribute(LPCTSTR pstrName) const;
		bool RemoveWindowCustomAttribute(LPCTSTR pstrName);
		void RemoveAllWindowCustomAttribute();

		// 样式管理:m_SharedResInfo.m_StyleHash和m_ResInfo.m_StyleHash    Style
		void AddStyle(LPCTSTR pName, LPCTSTR pStyle, bool bShared = false);
		LPCTSTR GetStyle(LPCTSTR pName) const;
		BOOL RemoveStyle(LPCTSTR pName, bool bShared = false);
		const CStdStringPtrMap& GetStyles(bool bShared = false) const;
		void RemoveAllStyle(bool bShared = false);

		//Duilib自绘：控件子类化时重写DoPostPaint
		int GetPostPaintCount() const;//自绘控件数
		bool IsPostPaint(CControlUI* pControl);
		bool AddPostPaint(CControlUI* pControl);
		bool RemovePostPaint(CControlUI* pControl);
		bool SetPostPaintIndex(CControlUI* pControl, int iIndex);

		//选项组管理
		bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
		CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
		void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
		void RemoveAllOptionGroups();

		//DPI处理
		CDPI* GetDPIObj();
		void SetDPI(int iDPI);
		static void SetAllDPI(int iDPI);
		void ResetDPIAssets();

		//定时器管理
		bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
		bool KillTimer(CControlUI* pControl, UINT nTimerID);
		void KillTimer(CControlUI* pControl);
		void RemoveAllTimers();

		//鼠标
		POINT GetMousePos() const;//获取鼠标位置
		void SetCapture();
		void ReleaseCapture();
		bool IsCaptured();//鼠标捕捉

		// 初始化拖拽
		bool EnableDragDrop(bool bEnable);
		virtual bool OnDrop(FORMATETC* pFmtEtc, STGMEDIUM& medium,DWORD *pdwEffect);

//窗口及窗口消息管理=============================================================================
		bool AttachDialog(CControlUI* pControl);//将控件树关联到窗口
		bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);//注册控件查找接口
		void ReapObjects(CControlUI* pControl);

		//焦点设置
		CControlUI* GetFocus() const;
		void SetFocus(CControlUI* pControl);
		void SetFocusNeeded(CControlUI* pControl);
		bool SetNextTabControl(bool bForward = true);

		//Duilib内部消息
		bool AddNotifier(INotifyUI* pControl);
		bool RemoveNotifier(INotifyUI* pControl);   
		void SendNotify(TNotifyUI& Msg, bool bAsync = false);
		void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);

		//消息预处理过滤器
		bool AddPreMessageFilter(IMessageFilterUI* pFilter);
		bool RemovePreMessageFilter(IMessageFilterUI* pFilter);

		//消息过滤器
		bool AddMessageFilter(IMessageFilterUI* pFilter);
		bool RemoveMessageFilter(IMessageFilterUI* pFilter);

		//提供窗口句柄和窗口过程的低级封装
		int GetNativeWindowCount() const;
		RECT GetNativeWindowRect(HWND hChildWnd);
		bool AddNativeWindow(CControlUI* pControl, HWND hChildWnd);
		bool RemoveNativeWindow(HWND hChildWnd);

		//延迟清理
		void AddDelayedCleanup(CControlUI* pControl);
		void AddMouseLeaveNeeded(CControlUI* pControl);
		bool RemoveMouseLeaveNeeded(CControlUI* pControl);

		//加速键处理
		bool AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
		bool RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
		bool TranslateAccelerator(LPMSG pMsg);//翻译加速键

		//查找控件
		CControlUI* GetRoot() const;
		CControlUI* FindControl(POINT pt) const;
		CControlUI* FindControl(LPCTSTR pstrName) const;
		CControlUI* FindSubControlByPoint(CControlUI* pParent, POINT pt) const;
		CControlUI* FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const;
		CControlUI* FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex = 0);
		CStdPtrArray* FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass);

		//窗口过程消息处理
		static bool TranslateMessage(const LPMSG pMsg);
		bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);//消息窗口过程
		bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);//预处理消息窗口过程
		void UsedVirtualWnd(bool bUsed);//虚拟窗口

		//消息循环
		static void MessageLoop();
		
		//释放公有资源
		static void Term();

		// 注意：只支持简单类型指针，因为只释放内存，不会调用类对象的析构函数
		void DeletePtr(void* ptr);
	private:
		CStdPtrArray* GetFoundControls();
		static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);
		static CControlUI* CALLBACK __FindControlsFromUpdate(CControlUI* pThis, LPVOID pData);

		static void AdjustSharedImagesHSL();
		void AdjustImagesHSL();
		void PostAsyncNotify();
	private:
		CDuiString m_sName; 
		HWND m_hWndPaint;	//所附加的窗体的句柄
		HDC m_hDcPaint;
		HDC m_hDcOffscreen;//屏幕兼容DC
		HDC m_hDcBackground;//背景DC
		HBITMAP m_hbmpOffscreen;//屏幕DIB
		BYTE* m_pOffscreenBits;
		HBITMAP m_hbmpBackground;//背景DIB
		COLORREF* m_pBackgroundBits;

		// 提示信息
		HWND m_hwndTooltip;
		TOOLINFO m_ToolTip;
		int m_iHoverTime;
		bool m_bNoActivate;
		bool m_bShowUpdateRect;

		//
		CControlUI* m_pRoot;
		CControlUI* m_pFocus;//获得焦点控件指针
		CControlUI* m_pEventHover; //鼠标停留控件指针
		CControlUI* m_pEventClick;
		CControlUI* m_pEventKey;
		CControlUI* m_pLastToolTip;
		//
		POINT m_ptLastMousePos;
		SIZE m_szMinWindow;
		SIZE m_szMaxWindow;
		SIZE m_szInitWindowSize;
		RECT m_rcSizeBox;
		SIZE m_szRoundCorner;
		RECT m_rcCaption;
		UINT m_uTimerID;
		bool m_bFirstLayout;//是否为第一层（顶层窗口）
		bool m_bUpdateNeeded;
		bool m_bFocusNeeded;
		bool m_bOffscreenPaint;//是否绘制m_hDcOffscreen
		
		BYTE m_nOpacity;
		bool m_bLayered;
		RECT m_rcLayeredInset;
		bool m_bLayeredChanged;
		RECT m_rcLayeredUpdate;
		TDrawInfo m_diLayered;

		bool m_bMouseTracking; //是否追踪鼠标
		bool m_bMouseCapture;//是否捕获鼠标
		bool m_bIsPainting;
		bool m_bUsedVirtualWnd;
		bool m_bAsyncNotifyPosted;

		//
		CStdPtrArray m_aNotifiers;
		CStdPtrArray m_aTimers;
		CStdPtrArray m_aTranslateAccelerator;
		CStdPtrArray m_aPreMessageFilters;
		CStdPtrArray m_aMessageFilters;
		CStdPtrArray m_aPostPaintControls;
		CStdPtrArray m_aNativeWindow;
		CStdPtrArray m_aNativeWindowControl;
		CStdPtrArray m_aDelayedCleanup;
		CStdPtrArray m_aAsyncNotify;
		CStdPtrArray m_aFoundControls;
		CStdPtrArray m_aFonts;
		CStdPtrArray m_aNeedMouseLeaveNeeded;
		CStdStringPtrMap m_mNameHash;
		CStdStringPtrMap m_mWindowCustomAttrHash;
		CStdStringPtrMap m_mOptionGroup;
		
		bool m_bForceUseSharedRes;
		TResInfo m_ResInfo;
		
		// 窗口阴影
		CShadowUI m_shadow;
		
		// DPI管理器
		CDPI* m_pDPI;
		// 是否开启Gdiplus
		bool m_bUseGdiplusText;
		int m_trh;
		ULONG_PTR m_gdiplusToken;
		Gdiplus::GdiplusStartupInput *m_pGdiplusStartupInput;

		// 拖拽
		bool m_bDragDrop;
		bool m_bDragMode;
		HBITMAP m_hDragBitmap;
		
		//
		static HINSTANCE m_hInstance;
		static HINSTANCE m_hResourceInstance;
		static CDuiString m_pStrResourcePath;
		static CDuiString m_pStrResourceZip;
		static CDuiString m_pStrResourceZipPwd;
		static HANDLE m_hResourceZip;
		static bool m_bCachedResourceZip;
		static int m_nResType;
		static TResInfo m_SharedResInfo;
		static bool m_bUseHSL;
		static short m_H;
		static short m_S;
		static short m_L;
		static CStdPtrArray m_aPreMessages;
		static CStdPtrArray m_aPlugins;
	};

} // namespace DuiLib

#endif // __UIMANAGER_H__
