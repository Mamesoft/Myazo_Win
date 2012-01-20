#include "Window.h"

Window::Window(const Window&)
{
	return;
}

Window::Window(void)
{
	WindowHandle=nullptr;
	ParentWindow=nullptr;
	WindowClassName=L"";
	IsCreated_Value=false;
	ID=0;
	return;
}

Window::~Window(void)
{
	if(WindowHandle&&IsWindow(WindowHandle)) DestroyWindow(WindowHandle);
	return;
}

bool Window::Create(std::wstring Caption,unsigned long WindowStyle,int X,int Y,int Width,int Height)
{
	if(!IsCreated_Value){
		IsCreated_Value=true;
		WindowHandle=CreateWindowEx(0,WindowClassName.c_str(),Caption.c_str(),WindowStyle,
			X,Y,Width,Height,ParentWindow?ParentWindow->GetWindowHandle():nullptr,(HMENU)ID,GetModuleHandle(nullptr),nullptr);
		if(!WindowHandle) return false;
	}else return false;
	return true;
}

bool Window::Create(std::wstring Caption,unsigned long WindowStyle,unsigned long WindowStyleEx,int X,int Y,int Width,int Height)
{
	if(!IsCreated_Value){
		WindowHandle=CreateWindowEx(WindowStyleEx,WindowClassName.c_str(),Caption.c_str(),WindowStyle,
			X,Y,Width,Height,ParentWindow?ParentWindow->GetWindowHandle():nullptr,(HMENU)ID,GetModuleHandle(nullptr),nullptr);
	}else return false;
	return WindowHandle?IsCreated_Value=true:false;
}

bool Window::IsCreated(void)const
{
	return IsCreated_Value;
}

bool Window::Destroy(void)
{
	return IsWindow(WindowHandle)&&DestroyWindow(WindowHandle)?WindowHandle=nullptr,true:false;
}

bool Window::Move(int X,int Y,int Width,int Height,bool Redraw=true)
{
	return IsWindow(WindowHandle)&&MoveWindow(WindowHandle,X,Y,Width,Height,Redraw)?true:false;
}

bool Window::Show(int Flag)
{
	return IsWindow(WindowHandle)?ShowWindow(WindowHandle,Flag),true:false;
}

bool Window::Update(void)
{
	if(IsWindow(WindowHandle)&&UpdateWindow(WindowHandle)) return true;
	else return false;
}

long Window::SendMessage(unsigned int Message,unsigned int WParam,long LParam)
{
	return IsWindow(WindowHandle)?SendMessageW(WindowHandle,Message,WParam,LParam):0;
}

HWND Window::GetWindowHandle(void)const
{
	return WindowHandle;
}

int Window::GetWindowID(void)const
{
	return ID;
}

Window* Window::GetParentWindow(void)const
{
	return ParentWindow;
}

unsigned long Window::GetWindowStyle(void)const
{
	return IsCreated_Value&&IsWindow(WindowHandle)?GetWindowLong(WindowHandle,GWL_STYLE):0;
}

void Window::SetWindowStyle(unsigned long Style)
{
	if(IsCreated_Value&&IsWindow(WindowHandle)) SetWindowLong(WindowHandle,GWL_STYLE,Style);
	return;
}

unsigned long Window::GetWindowStyleEx(void)const
{
	return IsCreated_Value&&IsWindow(WindowHandle)?GetWindowLong(WindowHandle,GWL_EXSTYLE):0;
}

void Window::SetWindowStyleEx(unsigned long StyleEx)
{
	if(IsCreated_Value&&IsWindow(WindowHandle)) SetWindowLong(WindowHandle,GWL_EXSTYLE,StyleEx);
	return;
}

ControlWindow* Window::AddControl(std::wstring ClassName,std::wstring Caption,unsigned long WindowStyle,int X,int Y,int Width,int Height,int ID)
{
	auto Control=std::unique_ptr<ControlWindow>(new ControlWindow(ClassName,ID,this));
	return Control->Create(Caption,WindowStyle,X,Y,Width,Height)?ChildControls.push_back(Control.get()),Control.release():nullptr;
}

ControlWindow* Window::AddControl(std::wstring ClassName,std::wstring Caption,unsigned long WindowStyle,unsigned long WindowStyleEx,int X,int Y,int Width,int Height,int ID)
{
	auto Control=std::unique_ptr<ControlWindow>(new ControlWindow(ClassName,ID,this));
	if(!Control->Create(Caption,WindowStyle,WindowStyleEx,X,Y,Width,Height)) return nullptr;
	ChildControls.push_back(Control.get());
	return Control.release();
}

ControlWindow* Window::AddControl(ControlWindow* const Control)
{
	if(Control->GetParentWindow()==this) ChildControls.push_back(Control);
	return 0;
}

bool Window::RemoveControl(ControlWindow* const Control)
{
	auto Iterator=std::find_if(ChildControls.begin(),ChildControls.end(),[&](ControlWindow* Item){return Item==Control?true:false;});
	if(Iterator==ChildControls.end()) return false;
	ChildControls.erase(Iterator);
	return true;
}

bool Window::RemoveControlByHandle(HWND WindowHandle)
{
	auto Iterator=std::find_if(ChildControls.begin(),ChildControls.end(),[&](ControlWindow* Item){return Item->GetWindowHandle()==WindowHandle?true:false;});
	if(Iterator==ChildControls.end()) return false;
	ChildControls.erase(Iterator);
	return true;
}

bool Window::RemoveControlByID(int ID)
{
	auto Iterator=std::find_if(ChildControls.begin(),ChildControls.end(),[&](ControlWindow* Item){return Item->GetWindowID()==ID?true:false;});
	if(Iterator==ChildControls.end()) return false;
	ChildControls.erase(Iterator);
	return true;
}

ControlWindow* Window::GetControl(int Index)
{
	if(Index<0||Index>=ChildControls.size()) return nullptr;
	return ChildControls[Index];
}

ControlWindow* Window::GetControlByHandle(HWND WindowHandle)
{
	auto Iterator=std::find_if(ChildControls.begin(),ChildControls.end(),[&](ControlWindow* Item){return Item->GetWindowHandle()==WindowHandle?true:false;});
	if(Iterator==ChildControls.end()) return nullptr;
	return *Iterator;
}

ControlWindow* Window::GetControlByID(int ID)
{
	auto Iterator=std::find_if(ChildControls.begin(),ChildControls.end(),[&](ControlWindow* Item){return Item->GetWindowID()==ID?true:false;});
	if(Iterator==ChildControls.end()) return nullptr;
	return *Iterator;
}

DialogWindow::DialogWindow(const DialogWindow&)
{
	return;
}

DialogWindow::DialogWindow(std::wstring ClassName,WNDPROC Proc)
{
	WindowClassName=ClassName;
	InitWindowClass();
	WindowClass.lpfnWndProc=Proc;
	WindowClass.lpszClassName=ClassName.c_str();
	return;
}

DialogWindow::DialogWindow(std::wstring ClassName,WNDPROC Proc,HICON Icon,HICON SmallIcon)
{
	WindowClassName=ClassName;
	InitWindowClass();
	WindowClass.lpszClassName=ClassName.c_str();
	WindowClass.lpfnWndProc=Proc;
	WindowClass.hIcon=Icon;
	WindowClass.hIconSm=SmallIcon;
	return;
}

DialogWindow::DialogWindow(std::wstring ClassName,WNDPROC Proc,HICON Icon,HICON SmallIcon,Window* ParentWindow)
{
	WindowClassName=ClassName;
	this->ParentWindow=ParentWindow;
	InitWindowClass();
	WindowClass.lpszClassName=ClassName.c_str();
	WindowClass.lpfnWndProc=Proc;
	WindowClass.hIcon=Icon;
	WindowClass.hIconSm=SmallIcon;
	return;
}

DialogWindow::DialogWindow(const WNDCLASSEX& WindowClass)
{
	memcpy(&this->WindowClass,&WindowClass,sizeof(this->WindowClass));
	if(WindowClass.lpszClassName) WindowClassName=WindowClass.lpszClassName;
	return;
}

DialogWindow::DialogWindow(const WNDCLASSEX& WindowClass,Window* ParentWindow)
{
	memcpy(&this->WindowClass,&WindowClass,sizeof(this->WindowClass));
	if(WindowClass.lpszClassName) WindowClassName=WindowClass.lpszClassName;
	this->ParentWindow=ParentWindow;
	return;
}

DialogWindow::~DialogWindow(void)
{
	if(IsRegistered_Value) UnregisterClass(WindowClassName.c_str(),GetModuleHandle(nullptr));
	return;
}

void DialogWindow::InitWindowClass(void)
{
	ZeroMemory(&WindowClass,sizeof(WindowClass));
	WindowClass.cbSize=sizeof(WindowClass);
	WindowClass.hbrBackground=(HBRUSH)COLOR_WINDOW;
	WindowClass.hCursor=LoadCursor(nullptr,IDC_ARROW);
	WindowClass.hInstance=GetModuleHandle(nullptr);
	WindowClass.lpszClassName=WindowClassName.c_str();
	WindowClass.style=CS_HREDRAW|CS_VREDRAW;
	WindowClass.lpszMenuName=nullptr;
	return;
}

bool DialogWindow::Register(void)
{
	ClassAtom=RegisterClassEx(&WindowClass);
	if(!ClassAtom) return false;
	return true;
}

ControlWindow::ControlWindow(const ControlWindow&)
{
	return;
}

ControlWindow::ControlWindow(std::wstring ClassName,int ID,Window* ParentWindow)
{
	WindowClassName=ClassName;
	this->ID=ID;
	this->ParentWindow=ParentWindow;
	return;
}