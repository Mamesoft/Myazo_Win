#include "Myazo.h"

Myazo::Myazo(const Myazo&)
{
	return;
}

Myazo::Myazo(void):TempFileNamePrefix(L"myazo")
{
	Instance=nullptr;
	UploadAsPrivate=UtilityMode=CaptureStarted=false;
	ZeroMemory(&CaptureRect,sizeof(CaptureRect));
	return;
}

Myazo::~Myazo(void)
{
	KillTimer(MainWindow.GetWindowHandle(),100);
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
	MainWindow.ShowAndUpdate(SW_SHOW);
	if(!LayerWindow.Create(L"Myazo",WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_LAYERED|WS_EX_NOACTIVATE,100,100,300,300)) return false;
	SetLayeredWindowAttributes(LayerWindow.GetWindowHandle(),0x000000FF,100,LWA_COLORKEY|LWA_ALPHA);
	SetTimer(MainWindow.GetWindowHandle(),100,100,nullptr);
	LayerWindowGraphics.reset(new Gdiplus::Graphics(LayerWindow.GetWindowHandle()));
	LayerWindowFont.reset(new Gdiplus::Font(L"Tahoma",8));
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