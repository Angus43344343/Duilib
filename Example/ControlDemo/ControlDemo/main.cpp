#include "stdafx.h"
#include "ControlDemo.h"

#include "ControlExUI.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	//内存检测
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出后自动打印内存泄漏的块信息
	//_CrtDumpMemoryLeaks();//没有静态及全局内存块和多线程退出机制时使用
	//_CrtSetBreakAlloc(1450);//在指定的内存申请点时断点

	//COM
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	// OLE
	HRESULT hRes = ::OleInitialize(NULL);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	//注册自绘控件
	REGIST_DUICONTROL(CCardUI);
	
	CControlDemo *pobjFrameWnd = new CControlDemo();
	assert(nullptr != pobjFrameWnd);

#ifdef _UNICODE
	pobjFrameWnd->EnableUnicode();//如果是unicode
#endif

	pobjFrameWnd->Create(NULL, _T("Duilib测试用例"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 357, 320);
	pobjFrameWnd->CenterWindow();
	pobjFrameWnd->ShowModal();
	
	// 消息循环
	CPaintManagerUI::MessageLoop();

	// 销毁窗口
	delete pobjFrameWnd;
	pobjFrameWnd = NULL;

	// 清理资源
	CPaintManagerUI::Term();

	// OLE
	OleUninitialize();
	//COM
	::CoUninitialize();

	return 0;
}