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
#define UIFLAG_SETCURSOR     0x00000002//���
#define UIFLAG_WANTRETURN    0x00000004//Enter

	// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_UPDATETEST    0x00000008
#define UIFIND_TOP_FIRST     0x00000010//�����ϲ�Ŀؼ���ʼ����;�п��ܶ���ؼ���λ���и��ǵ����,���ϲ�Ŀؼ����������ƵĿؼ�.
#define UIFIND_ME_FIRST      0x80000000//�ӵ�ǰ�ؼ���ʼ����

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
	//����
	typedef struct UILIB_API tagTFontInfo
	{
		HFONT hFont;
		CDuiString sFontName;
		int iSize;
		bool bBold;//�Ӵ�
		bool bUnderline;
		bool bItalic;//б��
		TEXTMETRIC tm;//ϵͳ����ṹ
	} TFontInfo;

	//ͼƬ
	typedef struct UILIB_API tagTImageInfo
	{
		HBITMAP hBitmap;
		LPBYTE pBits;
		LPBYTE pSrcBits;
		int nX;
		int nY;
		bool bAlpha;//͸��ͨ��
		bool bUseHSL;//HSL
		CDuiString sResType;
		DWORD dwMask;//����͸����
	} TImageInfo;

	//������Ϣ����ͼ����
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

	//��������
	typedef struct UILIB_API tagTPercentInfo
	{
		double left;
		double top;
		double right;
		double bottom;
	} TPercentInfo;

	//��Դ��Ϣ
	typedef struct UILIB_API tagTResInfo
	{
		DWORD m_dwDefaultDisabledColor;
		DWORD m_dwDefaultFontColor;
		DWORD m_dwDefaultLinkFontColor;
		DWORD m_dwDefaultLinkHoverFontColor;
		DWORD m_dwDefaultSelectedBkColor;
		TFontInfo m_DefaultFontInfo;
		CStdStringPtrMap m_CustomFonts;	//��������
		CStdStringPtrMap m_ImageHash;	//ͼƬ�����
		CStdStringPtrMap m_AttrHash;	//���Թ����
		CStdStringPtrMap m_StyleHash;	//��ʽ�����
		CStdStringPtrMap m_DrawInfoHash;//������Ϣ�����
	} TResInfo;

	// Structure for notifications from the system
	// to the control implementation.
	//�¼���Ϣ
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

	//������ټ�ֵ 
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
//���ƹ���===============================================================================
		//��ʼ��������Ϣ
		void Init(HWND hWnd, LPCTSTR pstrName = NULL);
		bool IsUpdateNeeded() const;
		void NeedUpdate();
		void Invalidate();
		void Invalidate(RECT& rcItem);//ʹ����������Ч,����WM_PAINT���»���

		LPCTSTR GetName() const;//���ƹ�����,�ɲ�����
		HDC GetPaintDC() const;
		HWND GetPaintWindow() const;//����ƴ��ھ��
		HWND GetTooltipWindow() const;//��ͣ����
		CShadowUI* GetShadow();//��ȡ��Ӱ����

		//��Դ����
		static HINSTANCE GetInstance();
		static void SetInstance(HINSTANCE hInst);//������
		static CDuiString GetInstancePath();
		static CDuiString GetCurrentPath();
		static void SetCurrentPath(LPCTSTR pStrPath);//��ǰ·��
		static HINSTANCE GetResourceDll();
		static void SetResourceDll(HINSTANCE hInst);//��Դ���
		static const CDuiString& GetResourcePath();
		static void SetResourcePath(LPCTSTR pStrPath);//��Դ·��
		static const CDuiString& GetResourceZip();
		static const CDuiString& GetResourceZipPwd();//ZIP��Դ,��������ZIP
		static void SetResourceZip(LPVOID pVoid, unsigned int len, LPCTSTR password = NULL);
		static void SetResourceZip(LPCTSTR pstrZip, bool bCachedResourceZip = false, LPCTSTR password = NULL);
		static bool IsCachedResourceZip();
		static HANDLE GetResourceZipHandle();//ZIP�ļ����
		static void SetResourceType(int nType);
		static int GetResourceType();//��Դ����
		static void ReloadSkin();//������Դ
		static CPaintManagerUI* GetPaintManager(LPCTSTR pstrName);
		static CStdPtrArray* GetPaintManagers();//��ȡ
		static bool LoadPlugin(LPCTSTR pstrModuleName);
		static CStdPtrArray* GetPlugins();//��̬���Ӻ����ӿ�
		bool IsForceUseSharedRes() const;
		void SetForceUseSharedRes(bool bForce);//������Դ

		//window:���崰����Ϣ����  Window
		int GetHoverTime() const;
		void SetHoverTime(int iTime);//������ͣʱ��,����ͣ���ڶ�Ӧ
		SIZE GetClientSize() const;
		SIZE GetInitSize();
		void SetInitSize(int cx, int cy);//��ʼ�����С
		RECT& GetSizeBox();
		void SetSizeBox(RECT& rcSizeBox);//�������촰��ļ�ͷ����
		RECT& GetCaptionRect();
		void SetCaptionRect(RECT& rcCaption);//���ñ�����
		SIZE GetRoundCorner() const;
		void SetRoundCorner(int cx, int cy);//����Բ��
		SIZE GetMinInfo() const;
		void SetMinInfo(int cx, int cy);//��С����
		SIZE GetMaxInfo() const;
		void SetMaxInfo(int cx, int cy);//�����
		bool IsShowUpdateRect() const;
		void SetShowUpdateRect(bool show);//������ʾ����(��������)
		void SetUseGdiplusText(bool bUse);
		bool IsUseGdiplusText() const;//GDI+��������,Ĭ�Ͽ���,ȱ����Ч�ʵ�
		void SetGdiplusTextRenderingHint(int trh);
		int GetGdiplusTextRenderingHint() const;//GDI+�����Եƽ������:�ر�������Ŵ���־��
		bool IsNoActivate();
		void SetNoActivate(bool bNoActivate);//

		//�ֲ㴰�ڣ�����������͸������,�ý�����ſ�
		BYTE GetOpacity() const;
		void SetOpacity(BYTE nOpacity);//����͸����
		bool IsLayered();
		void SetLayered(bool bLayered);//���÷ֲ㴰��
		RECT& GetLayeredInset();
		void SetLayeredInset(RECT& rcLayeredInset);//�ֲ㴰�ڵ��ڱ߾�
		BYTE GetLayeredOpacity();
		void SetLayeredOpacity(BYTE nOpacity);//�ֲ㴰�ڵ�͸����
		LPCTSTR GetLayeredImage();
		void SetLayeredImage(LPCTSTR pstrImage);//�ֲ㴰�ڵı���ͼ

		//�������:m_SharedResInfo.m_CustomFonts��m_ResInfo.m_CustomFonts  Font
		DWORD GetDefaultDisabledColor() const;
		void SetDefaultDisabledColor(DWORD dwColor, bool bShared = false);//Ĭ�Ͻ���������ɫ
		DWORD GetDefaultFontColor() const;
		void SetDefaultFontColor(DWORD dwColor, bool bShared = false);//Ĭ��������ɫ
		DWORD GetDefaultLinkFontColor() const;
		void SetDefaultLinkFontColor(DWORD dwColor, bool bShared = false);//Ĭ�ϳ�����������ɫ 
		DWORD GetDefaultLinkHoverFontColor() const;
		void SetDefaultLinkHoverFontColor(DWORD dwColor, bool bShared = false);//Ĭ�������ͣ�ڳ�����������ɫ 
		DWORD GetDefaultSelectedBkColor() const;
		void SetDefaultSelectedBkColor(DWORD dwColor, bool bShared = false);//Ĭ��ѡ�����屳��ɫ
		TFontInfo* GetDefaultFontInfo();//Ĭ��������Ϣ
		void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
		DWORD GetCustomFontCount(bool bShared = false) const;//�Զ���������
		void AddFontArray(LPCTSTR pstrPath);//�������
		HFONT AddFont(int id, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
		HFONT GetFont(int id);//��ȡ�����б��е�����
		HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
		int GetFontIndex(HFONT hFont, bool bShared = false);//��ȡ�����������б����
		int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
		void RemoveFont(HFONT hFont, bool bShared = false);//�Ƴ�����
		void RemoveFont(int id, bool bShared = false);
		void RemoveAllFonts(bool bShared = false);
		TFontInfo* GetFontInfo(int id);
		TFontInfo* GetFontInfo(HFONT hFont);//��ȡ������Ϣ
		void RebuildFont(TFontInfo* pFontInfo);

		//ͼƬ����:m_ResInfo.m_ImageHash��m_SharedResInfo.m_ImageHash Image
		static bool GetHSL(short* H, short* S, short* L);//û��ʹ�ô˹���
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

		//������Ϣ����:m_ResInfo.m_DrawInfoHash��m_SharedResInfo.m_DrawInfoHash,ͼƬ����Ƭ����
		bool IsPainting();
		void SetPainting(bool bIsPainting);
		const TDrawInfo* GetDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify);
		void RemoveDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify);
		void RemoveAllDrawInfos(bool bShared = false);

		//���Ĭ�������б�:m_SharedResInfo.m_AttrHash��m_ResInfo.m_AttrHash  Default
		void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList, bool bShared = false);
		LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
		bool RemoveDefaultAttributeList(LPCTSTR pStrControlName, bool bShared = false);
		void RemoveAllDefaultAttributeList(bool bShared = false);

		//����Զ��������б�:m_mWindowCustomAttrHash
		void AddWindowCustomAttribute(LPCTSTR pstrName, LPCTSTR pstrAttr);
		LPCTSTR GetWindowCustomAttribute(LPCTSTR pstrName) const;
		bool RemoveWindowCustomAttribute(LPCTSTR pstrName);
		void RemoveAllWindowCustomAttribute();

		// ��ʽ����:m_SharedResInfo.m_StyleHash��m_ResInfo.m_StyleHash    Style
		void AddStyle(LPCTSTR pName, LPCTSTR pStyle, bool bShared = false);
		LPCTSTR GetStyle(LPCTSTR pName) const;
		BOOL RemoveStyle(LPCTSTR pName, bool bShared = false);
		const CStdStringPtrMap& GetStyles(bool bShared = false) const;
		void RemoveAllStyle(bool bShared = false);

		//Duilib�Ի棺�ؼ����໯ʱ��дDoPostPaint
		int GetPostPaintCount() const;//�Ի�ؼ���
		bool IsPostPaint(CControlUI* pControl);
		bool AddPostPaint(CControlUI* pControl);
		bool RemovePostPaint(CControlUI* pControl);
		bool SetPostPaintIndex(CControlUI* pControl, int iIndex);

		//ѡ�������
		bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
		CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
		void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
		void RemoveAllOptionGroups();

		//DPI����
		CDPI* GetDPIObj();
		void SetDPI(int iDPI);
		static void SetAllDPI(int iDPI);
		void ResetDPIAssets();

		//��ʱ������
		bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
		bool KillTimer(CControlUI* pControl, UINT nTimerID);
		void KillTimer(CControlUI* pControl);
		void RemoveAllTimers();

		//���
		POINT GetMousePos() const;//��ȡ���λ��
		void SetCapture();
		void ReleaseCapture();
		bool IsCaptured();//��겶׽

		// ��ʼ����ק
		bool EnableDragDrop(bool bEnable);
		virtual bool OnDrop(FORMATETC* pFmtEtc, STGMEDIUM& medium,DWORD *pdwEffect);

//���ڼ�������Ϣ����=============================================================================
		bool AttachDialog(CControlUI* pControl);//���ؼ�������������
		bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);//ע��ؼ����ҽӿ�
		void ReapObjects(CControlUI* pControl);

		//��������
		CControlUI* GetFocus() const;
		void SetFocus(CControlUI* pControl);
		void SetFocusNeeded(CControlUI* pControl);
		bool SetNextTabControl(bool bForward = true);

		//Duilib�ڲ���Ϣ
		bool AddNotifier(INotifyUI* pControl);
		bool RemoveNotifier(INotifyUI* pControl);   
		void SendNotify(TNotifyUI& Msg, bool bAsync = false);
		void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);

		//��ϢԤ���������
		bool AddPreMessageFilter(IMessageFilterUI* pFilter);
		bool RemovePreMessageFilter(IMessageFilterUI* pFilter);

		//��Ϣ������
		bool AddMessageFilter(IMessageFilterUI* pFilter);
		bool RemoveMessageFilter(IMessageFilterUI* pFilter);

		//�ṩ���ھ���ʹ��ڹ��̵ĵͼ���װ
		int GetNativeWindowCount() const;
		RECT GetNativeWindowRect(HWND hChildWnd);
		bool AddNativeWindow(CControlUI* pControl, HWND hChildWnd);
		bool RemoveNativeWindow(HWND hChildWnd);

		//�ӳ�����
		void AddDelayedCleanup(CControlUI* pControl);
		void AddMouseLeaveNeeded(CControlUI* pControl);
		bool RemoveMouseLeaveNeeded(CControlUI* pControl);

		//���ټ�����
		bool AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
		bool RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
		bool TranslateAccelerator(LPMSG pMsg);//������ټ�

		//���ҿؼ�
		CControlUI* GetRoot() const;
		CControlUI* FindControl(POINT pt) const;
		CControlUI* FindControl(LPCTSTR pstrName) const;
		CControlUI* FindSubControlByPoint(CControlUI* pParent, POINT pt) const;
		CControlUI* FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const;
		CControlUI* FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex = 0);
		CStdPtrArray* FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass);

		//���ڹ�����Ϣ����
		static bool TranslateMessage(const LPMSG pMsg);
		bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);//��Ϣ���ڹ���
		bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);//Ԥ������Ϣ���ڹ���
		void UsedVirtualWnd(bool bUsed);//���ⴰ��

		//��Ϣѭ��
		static void MessageLoop();
		
		//�ͷŹ�����Դ
		static void Term();

		// ע�⣺ֻ֧�ּ�����ָ�룬��Ϊֻ�ͷ��ڴ棬���������������������
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
		HWND m_hWndPaint;	//�����ӵĴ���ľ��
		HDC m_hDcPaint;
		HDC m_hDcOffscreen;//��Ļ����DC
		HDC m_hDcBackground;//����DC
		HBITMAP m_hbmpOffscreen;//��ĻDIB
		BYTE* m_pOffscreenBits;
		HBITMAP m_hbmpBackground;//����DIB
		COLORREF* m_pBackgroundBits;

		// ��ʾ��Ϣ
		HWND m_hwndTooltip;
		TOOLINFO m_ToolTip;
		int m_iHoverTime;
		bool m_bNoActivate;
		bool m_bShowUpdateRect;

		//
		CControlUI* m_pRoot;
		CControlUI* m_pFocus;//��ý���ؼ�ָ��
		CControlUI* m_pEventHover; //���ͣ���ؼ�ָ��
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
		bool m_bFirstLayout;//�Ƿ�Ϊ��һ�㣨���㴰�ڣ�
		bool m_bUpdateNeeded;
		bool m_bFocusNeeded;
		bool m_bOffscreenPaint;//�Ƿ����m_hDcOffscreen
		
		BYTE m_nOpacity;
		bool m_bLayered;
		RECT m_rcLayeredInset;
		bool m_bLayeredChanged;
		RECT m_rcLayeredUpdate;
		TDrawInfo m_diLayered;

		bool m_bMouseTracking; //�Ƿ�׷�����
		bool m_bMouseCapture;//�Ƿ񲶻����
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
		
		// ������Ӱ
		CShadowUI m_shadow;
		
		// DPI������
		CDPI* m_pDPI;
		// �Ƿ���Gdiplus
		bool m_bUseGdiplusText;
		int m_trh;
		ULONG_PTR m_gdiplusToken;
		Gdiplus::GdiplusStartupInput *m_pGdiplusStartupInput;

		// ��ק
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
