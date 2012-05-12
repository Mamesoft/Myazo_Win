#include "Myazo.h"

Myazo::Myazo(const Myazo&)
{
	return;
}

Myazo::Myazo(void):
AppName(L"Myazo"),
UserAgent(AppName+L"_win/1.02"),
Boundary(L"h-o-g-e-p-i-y-o"),
DefaultHeader(L"Content-Type: multipart/form-data; boundary="+Boundary),
UploadDomain(L"myazo.net"),
UploadScriptPath(L"upload.php"),
SettingFileName(L"Setting")
{
	Instance=nullptr;
	UploadAsPrivate=true;
	UtilityMode=CaptureStarted=FirstBoot=false;
	ZeroMemory(&CaptureRect,sizeof(CaptureRect));
	return;
}

Myazo::~Myazo(void)
{
	SaveSetting();
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
	Instance=GetModuleHandle(nullptr);
	std::vector<wchar_t> Temp(MAX_PATH,0);
	SHGetSpecialFolderPath(nullptr,Temp.data(),CSIDL_APPDATA,false);
	SettingFileDirectory=Temp.data();
	SettingFileDirectory+=(*--SettingFileDirectory.end()==L'\\'?L"":L"\\")+AppName;
	if(!ImageEncoder.Init()) return false;
	if(!InitWindowClass()) return false;
	InitSetting();
	if(FirstBoot) OpenAuthWindow();
	else if(CheckArgumentOrUpload()||!InitWindow()) return false;
	LayerWindowFont.reset(new Gdiplus::Font(L"Tahoma",8));
	UIFont.reset(new Gdiplus::Font(L"MS UI Gothic",9.75f));
	return true;
}

bool Myazo::InitWindowClass(void)
{
	Handle Cursor(LoadCursor(Instance,MAKEINTRESOURCE(IDC_CURSOR1)),[](void* Obj){if(Obj) DestroyCursor((HCURSOR)Obj);}),
		Icon(LoadImage(Instance,MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,32,32,0),[](void* Obj){if(Obj) DeleteObject(Obj);}),
		IconSmall(LoadImage(Instance,MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,16,16,0),[](void* Obj){if(Obj) DeleteObject(Obj);});
	WNDCLASSEX WndClass;
	ZeroMemory(&WndClass,sizeof(WndClass));
	WndClass.cbSize=sizeof(WndClass);
	WndClass.lpfnWndProc=WndProc;
	WndClass.hInstance=GetModuleHandle(nullptr);
	WndClass.hIcon=(HICON)Icon.get();
	WndClass.hIconSm=(HICON)IconSmall.get();
	WndClass.hCursor=(HCURSOR)Cursor.get();
	WndClass.lpszClassName=L"MyazoMainWindow";	
	MainWindow=DialogWindow(WndClass);
	if(!MainWindow.Register()) return false;
	WndClass.style=CS_VREDRAW|CS_HREDRAW;
	WndClass.hbrBackground=CreateSolidBrush(RGB(100,100,100));
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

bool Myazo::InitWindow(void)
{
	int X,Y,Width,Height;
	FirstBoot=false;
	X=GetSystemMetrics(SM_XVIRTUALSCREEN);
	Y=GetSystemMetrics(SM_YVIRTUALSCREEN);
	Width=GetSystemMetrics(SM_CXVIRTUALSCREEN);
	Height=GetSystemMetrics(SM_CYVIRTUALSCREEN);
	if(!MainWindow.Create(AppName.c_str(),WS_POPUP,WS_EX_TRANSPARENT|WS_EX_TOOLWINDOW|WS_EX_TOPMOST,0,0,0,0)) return false;
	MainWindow.Move(X,Y,Width,Height,false);
	MainWindow.ShowAndUpdate(SW_SHOW);
	MainWindow.Focus();
	if(!LayerWindow.Create(AppName.c_str(),WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_LAYERED|WS_EX_TOPMOST|WS_EX_NOACTIVATE,0,0,0,0)) return false;
	SetLayeredWindowAttributes(LayerWindow.GetWindowHandle(),RGB(255,0,0),100,LWA_COLORKEY|LWA_ALPHA);
	return true;
}

bool Myazo::InitAuthWindow(void)
{
	int ID=1000;
	std::wstring ClassNames[]={L"Static",L"Static",L"Edit",L"Static",L"Edit",L"Button",L"Button",L"Button"},
		Captions[]={L"下のテキストボックスにIDとパスワードを入力して、\n「認証」ボタンを押して下さい。",
			L"ID",L"",L"パスワード",L"",L"新規登録をする",L"認証",L"キャンセル"};
	int DefaultStyle=WS_CHILD|WS_VISIBLE,
		Styles[]={DefaultStyle|SS_LEFT,DefaultStyle|SS_LEFT,DefaultStyle|ES_LEFT|ES_AUTOHSCROLL,DefaultStyle|SS_LEFT,
			DefaultStyle|ES_LEFT|ES_PASSWORD|ES_AUTOHSCROLL,DefaultStyle|BS_CENTER,DefaultStyle|BS_CENTER,DefaultStyle|BS_CENTER},
		ExStyles[]={0,0,WS_EX_CLIENTEDGE,0,WS_EX_CLIENTEDGE,0,0,0},
		X[]={10,10,65,10,65,30,135,215},Y[]={10,48,45,73,70,94,94,94},
		Width[]={250,50,200,50,200,100,75,75},Height[]={24,12,19,12,19,23,23,23};
	for(int i=0;i<sizeof(ClassNames)/sizeof(std::wstring);i++){
		AuthWindow.AddControl(Controls[i]=Window(ClassNames[i],ID+i,&AuthWindow));
		Controls[i].Create(Captions[i],Styles[i],ExStyles[i],X[i],Y[i],Width[i],Height[i]);
		Controls[i].Message(WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(true,0));
	}
	return true;
}

void Myazo::InitSetting(void)
{
	auto Setting=Json::Parse(ReadSettingFile());
	UserID=Setting(L"userid");
	PassWord=Setting(L"password");
	return;
}

void Myazo::SaveSetting(void)
{
	Json::Item Setting(Json::Type::Hash);
	Setting.Hash().insert(Json::JsonHashPair(L"userid",UserID));
	Setting.Hash().insert(Json::JsonHashPair(L"password",PassWord));
	WriteSettingFile(Json::ToString(Setting));
	return;
}

void Myazo::EnterMessageLoop(void)
{
	MSG Msg;
	ZeroMemory(&Msg,sizeof(Msg));
	while(GetMessage(&Msg,nullptr,0,0)){
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return;
}