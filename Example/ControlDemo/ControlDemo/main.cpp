#include "stdafx.h"
#include "ControlDemo.h"

#include "ControlExUI.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	//�ڴ���
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//�����˳����Զ���ӡ�ڴ�й©�Ŀ���Ϣ
	//_CrtDumpMemoryLeaks();//û�о�̬��ȫ���ڴ��Ͷ��߳��˳�����ʱʹ��
	//_CrtSetBreakAlloc(1450);//��ָ�����ڴ������ʱ�ϵ�

	//COM
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	// OLE
	HRESULT hRes = ::OleInitialize(NULL);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	//ע���Ի�ؼ�
	REGIST_DUICONTROL(CCardUI);
	
	CControlDemo *pobjFrameWnd = new CControlDemo();
	assert(nullptr != pobjFrameWnd);

#ifdef _UNICODE
	pobjFrameWnd->EnableUnicode();//�����unicode
#endif

	pobjFrameWnd->Create(NULL, _T("Duilib��������"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 357, 320);
	pobjFrameWnd->CenterWindow();
	pobjFrameWnd->ShowModal();
	
	// ��Ϣѭ��
	CPaintManagerUI::MessageLoop();

	// ���ٴ���
	delete pobjFrameWnd;
	pobjFrameWnd = NULL;

	// ������Դ
	CPaintManagerUI::Term();

	// OLE
	OleUninitialize();
	//COM
	::CoUninitialize();

	return 0;
}