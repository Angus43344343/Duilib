#ifndef __UIRESOURCEMANAGER_H__
#define __UIRESOURCEMANAGER_H__
#pragma once

enum UILIB_RESTYPE
{
	UILIB_FILE = 1,		// 来自磁盘文件
	UILIB_ZIP,			// 来自磁盘zip压缩包
	UILIB_RESOURCE,		// 来自资源
	UILIB_ZIPRESOURCE,	// 来自资源的zip压缩包
};

namespace DuiLib {
	// 控件文字查询接口
	class UILIB_API IQueryControlText
	{
	public:
		virtual LPCTSTR QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType) = 0;
	};

	class UILIB_API CResourceManager
	{
	private:
		CResourceManager(void);
		~CResourceManager(void);

	public:
		static CResourceManager* GetInstance()
		{
			static CResourceManager * p = new CResourceManager;
			return p;
		};	
		void Release(void) { delete this; }

	public:
		BOOL LoadResource(STRINGorID xml, LPCTSTR type = NULL);
		BOOL LoadResource(CMarkupNode Root);
		void ResetResourceMap();
		LPCTSTR GetImagePath(LPCTSTR lpstrId);
		LPCTSTR GetXmlPath(LPCTSTR lpstrId);

	public:
		//切换语言:思路是使用xml里使用text及resourcetext,捕获text文本,添加lpstrId到map,再通过QueryControlText切换到想要的语言
		void SetLanguage(LPCTSTR pstrLanguage) { m_sLauguage = pstrLanguage; }
		LPCTSTR GetLanguage() { return m_sLauguage; }
		BOOL LoadLanguage(LPCTSTR pstrXml);
		
		void SetTextQueryInterface(IQueryControlText* pInterface) { m_pQuerypInterface = pInterface; }
		CDuiString GetText(LPCTSTR lpstrId, LPCTSTR lpstrType = NULL);
		void ReloadText();
		void ResetTextMap();

	private:
		CStdStringPtrMap m_mTextResourceHashMap;
		IQueryControlText*	m_pQuerypInterface;
		CStdStringPtrMap m_mImageHashMap;
		CStdStringPtrMap m_mXmlHashMap;
		CMarkup m_xml;
		CDuiString m_sLauguage;
		CStdStringPtrMap m_mTextHashMap;
	};

} // namespace DuiLib

#endif // __UIRESOURCEMANAGER_H__