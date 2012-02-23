#include "Myazo.h"

Myazo::Myazo(const Myazo&)
{
	return;
}

Myazo::Myazo(void)
{
	Instance=nullptr;
	UploadAsPrivate=false;
	UtilityMode=false;
	OffsetX=OffsetY=0;
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

LRESULT __stdcall Myazo::AuthWndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam)
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
	int X,Y,Width,Height;
	Instance=GetModuleHandle(nullptr);
	if(!ImageEncoder.Init()) return false;
	X=GetSystemMetrics(SM_XVIRTUALSCREEN);
	Y=GetSystemMetrics(SM_YVIRTUALSCREEN);
	Width=GetSystemMetrics(SM_CXVIRTUALSCREEN);
	Height=GetSystemMetrics(SM_CYVIRTUALSCREEN);
	if(!InitWindow()) return false;
	if(!MainWindow.Create(L"",WS_POPUP,WS_EX_TRANSPARENT|WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,0,0,0,0)) return false;
	MainWindow.Move(X,Y,Width,Height,false);
	MainWindow.Show(SW_SHOW);
	MainWindow.Update();
	if(!LayerWindow.Create(L"Myazo",WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_LAYERED|WS_EX_NOACTIVATE,100,100,300,300)) return false;
	SetLayeredWindowAttributes(LayerWindow.GetWindowHandle(),0x000000FF,100,LWA_COLORKEY|LWA_ALPHA);
	return true;
}

bool Myazo::InitWindow(void)
{
	WNDCLASSEX WndClass;
	ZeroMemory(&WndClass,sizeof(WndClass));
	WndClass.lpfnWndProc=WndProc;
	WndClass.hInstance=GetModuleHandle(nullptr);
	WndClass.hIcon=(HICON)LoadImage(Instance,L"",IMAGE_ICON,32,32,0);
	WndClass.hIconSm=(HICON)LoadImage(Instance,L"",IMAGE_ICON,16,16,0);
	WndClass.hCursor=LoadCursor(nullptr,IDC_CROSS);
	WndClass.lpszClassName=L"MyazoMainWindow";	
	MainWindow=DialogWindow(WndClass);
	if(!MainWindow.Register()) return false;
	WndClass.style=CS_VREDRAW|CS_HREDRAW;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpfnWndProc=LayerWndProc;
	WndClass.lpszClassName=L"MyazoLayerWindow";
	LayerWindow=DialogWindow(WndClass);
	if(!LayerWindow.Register()) return false;
	WndClass.hbrBackground=(HBRUSH)COLOR_WINDOW;
	WndClass.lpfnWndProc=AuthWndProc;
	WndClass.hCursor=LoadCursor(nullptr,IDC_ARROW);
	WndClass.lpszClassName=L"MyazoAuthWindow";
	AuthWindow=DialogWindow(WndClass);
	if(!AuthWindow.Register()) return false;
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

void Myazo::ProcessKeyMessage(unsigned int KeyChar)
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
			(GetSystemMetrics(SM_CXVIRTUALSCREEN)-WindowWidth)/2,
			(GetSystemMetrics(SM_CYVIRTUALSCREEN)-WindowHeight)/2,WindowWidth,WindowHeight);
		break;
	}
	return;
}
