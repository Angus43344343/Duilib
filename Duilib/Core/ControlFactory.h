#pragma once
#include <map>
namespace DuiLib 
{
	typedef CControlUI* (*CreateClass)();
	typedef std::map<CDuiString, CreateClass> MAP_DUI_CTRATECLASS;

	class UILIB_API CControlFactory
	{
	public:
		CControlUI* CreateControl(CDuiString strClassName);
		void RegistControl(CDuiString strClassName, CreateClass pFunc);

		static CControlFactory* GetInstance();
		void Release();

	private:	
		CControlFactory();
		virtual ~CControlFactory();

	private:
		MAP_DUI_CTRATECLASS m_mapControl;
	};

/*类比静态成员变量的申明和定义,所以每个Control类要先申明DECLARE_DUICONTROL(),然后在.cpp里IMPLEMENT_DUICONTROL定义*/
//申明
#define DECLARE_DUICONTROL(class_name)\
public:\
	static CControlUI* CreateControl();

//定义
#define IMPLEMENT_DUICONTROL(class_name)\
	CControlUI* class_name::CreateControl()\
	{ return new class_name; }

#define REGIST_DUICONTROL(class_name)\
	CControlFactory::GetInstance()->RegistControl(_T(#class_name), (CreateClass)class_name::CreateControl);

//注册：添加所有类型的控件,创建时根据类型表进行创建
#define INNER_REGISTER_DUICONTROL(class_name)\
	RegistControl(_T(#class_name), (CreateClass)class_name::CreateControl);
}