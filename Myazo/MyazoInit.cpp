#include "Myazo.h"

Myazo::Myazo(const Myazo&)
{
	return;
}

Myazo::Myazo(void)
{
	Instance=GetModuleHandle(nullptr);
	UploadAsPrivate=false;
	UtilityMode=false;
	OffsetX=OffsetY=0;
	ScreenSizeX=0;
	ScreenSizeY=0;
	Window hoge=DialogWindow();
	MainWindow=DialogWindow(L"MyazoMainWindow",WndProc,
		(HICON)LoadImage(GetModuleHandle(nullptr),L"",IMAGE_ICON,32,32,0),
		(HICON)LoadImage(GetModuleHandle(nullptr),L"",IMAGE_ICON,16,16,0));
	return;
}

Myazo::~Myazo(void)
{
	return;
}

Myazo& Myazo::operator=(const Myazo&)
{
	return *this;
}

Myazo& Myazo::operator=(Myazo&&)
{
	return *this;
}

LRESULT __stdcall Myazo::WndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam)
{
	Myazo& Program=GetInstance();
	switch(Message){
	case WM_KEYDOWN:
		Program.ProcessKeyMessage(WParam);
		break;
	default:
		return DefWindowProc(WindowHandle,Message,WParam,LParam);
	}
	return 0;
}

LRESULT __stdcall Myazo::LayerWndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam)
{
	Myazo& Program=GetInstance();
	switch(Message){
	default:
		return DefWindowProc(WindowHandle,Message,WParam,LParam);
	}
	return 0;
}

Myazo& Myazo::GetInstance(void)
{
	static Myazo& Instance=Myazo();
	return Instance;
}

bool Myazo::Init(void)
{
	ScreenSizeX=GetSystemMetrics(SM_CXVIRTUALSCREEN);
	ScreenSizeY=GetSystemMetrics(SM_CYVIRTUALSCREEN);
	return true;
}
bool Myazo::InitWindow(void)
{

	return true;
}

void Myazo::EnterMessageLoop(void)
{
	MSG Msg;
	ZeroMemory(&Msg,sizeof(Msg));
	while(Msg.message!=WM_QUIT){
		if(PeekMessage(&Msg,0,0,0,PM_REMOVE)){
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
	return;
}

bool Myazo::Upload(void)
{
	return true;
}

void Myazo::ProcessKeyMessage(unsigned int KeyChar)throw
{
	switch(KeyChar){
	case 'P':
		UploadAsPrivate=!UploadAsPrivate;
		MessageBox(LayerWindow.GetWindowHandle(),
			UploadAsPrivate?L"非公開モードでアップロードされます。":L"公開モードでアップロードされます。",L"Myazo",MB_OK);
		break;
	case 'A':
		int WindowWidth=300,WindowHeight=150;
		AuthWindow.Create(L"アカウントの認証",WS_CAPTION|WS_SYSMENU,0,
			(ScreenSizeX-WindowWidth)/2,(ScreenSizeY-WindowHeight)/2,WindowWidth,WindowHeight);
		break;
	}
	return;
}
