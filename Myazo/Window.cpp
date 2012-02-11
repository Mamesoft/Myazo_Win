#include "Window.h"

Window::Window(void):ParentWindow(nullptr)
{
	WindowHandle=nullptr;
	WindowClassName=L"";
	IsCreated_Value=false;
	ID=0;
	return;
}

Window::Window(Window* ParentWindow):ParentWindow(ParentWindow)
{
	WindowHandle=nullptr;
	WindowClassName=L"";
	IsCreated_Value=false;
	ID=0;
	return;
}

Window::Window(const Window& LeftRef):ParentWindow(LeftRef.ParentWindow)
{
	WindowHandle=LeftRef.WindowHandle;
	((Window&)LeftRef).WindowHandle=nullptr;
	WindowClassName=std::move(LeftRef.WindowClassName);
	ChildControls=std::move(LeftRef.ChildControls);
	IsCreated_Value=LeftRef.IsCreated_Value;
	ID=LeftRef.ID;
	return;
}

Window::Window(Window&& RightRef):ParentWindow(RightRef.ParentWindow)
{
	WindowHandle=RightRef.WindowHandle;
	RightRef.WindowHandle=nullptr;
	WindowClassName=std::move(RightRef.WindowClassName);
	ChildControls=std::move(RightRef.ChildControls);
	IsCreated_Value=RightRef.IsCreated_Value;
	ID=RightRef.ID;
	return;
}

Window::Window(std::wstring ClassName,int ID,Window* ParentWindow)
{
	return;
}

Window::~Window(void)
{
	if(IsCreated_Value) Destroy();
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
	return IsWindow(WindowHandle)&&DestroyWindow(WindowHandle)?ChildControls.clear(),WindowHandle=nullptr,true:false;
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

Window& Window::GetParentWindow(void)const
{
	return *ParentWindow;
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

Window& Window::AddControl(std::wstring ClassName,std::wstring Caption,unsigned long WindowStyle,int X,int Y,int Width,int Height,int ID)
{
	Window Control(ClassName,ID,this);
	return Control.Create(Caption,WindowStyle,X,Y,Width,Height)?ChildControls.push_back(Control),Control:throw std::exception();
}

Window& Window::AddControl(std::wstring ClassName,std::wstring Caption,unsigned long WindowStyle,unsigned long WindowStyleEx,int X,int Y,int Width,int Height,int ID)
{
	Window Control(ClassName,ID,this);
	return Control.Create(Caption,WindowStyle,WindowStyleEx,X,Y,Width,Height)?ChildControls.push_back(Control),Control:throw std::exception();
}

Window& Window::AddControl(const Window& Control)
{
	if(Control.ParentWindow==this) ChildControls.push_back(Control);
	return ChildControls.back();
}

Window& Window::AddControl(Window&& Control)
{
	if(Control.ParentWindow==this) ChildControls.push_back(std::move(Control));
	return ChildControls.back();
}

bool Window::RemoveControl(const Window& Control)
{
	auto Iterator=std::find_if(ChildControls.cbegin(),ChildControls.cend(),[&](const Window& Item){return Item==Control?true:false;});
	if(Iterator==ChildControls.cend()) return false;
	ChildControls.erase(Iterator);
	return true;
}

bool Window::RemoveControlByHandle(HWND WindowHandle)
{
	auto Iterator=std::find_if(ChildControls.cbegin(),ChildControls.cend(),[&](const Window& Item){return Item.WindowHandle==WindowHandle?true:false;});
	if(Iterator==ChildControls.cend()) return false;
	ChildControls.erase(Iterator);
	return true;
}

bool Window::RemoveControlByID(int ID)
{
	auto Iterator=std::find_if(ChildControls.cbegin(),ChildControls.cend(),[&](const Window& Item){return Item.ID==ID?true:false;});
	if(Iterator==ChildControls.cend()) return false;
	ChildControls.erase(Iterator);
	return true;
}

Window& Window::GetControl(int Index)
{
	if(Index<0||Index>=ChildControls.size()) throw std::exception();
	return ChildControls[Index];
}

Window& Window::GetControlByHandle(HWND WindowHandle)
{
	auto Iterator=std::find_if(ChildControls.begin(),ChildControls.end(),[&](const Window& Item){return Item.WindowHandle==WindowHandle?true:false;});
	if(Iterator==ChildControls.end()) throw std::exception();
	return *Iterator;
}

Window& Window::GetControlByID(int ID)
{
	auto Iterator=std::find_if(ChildControls.begin(),ChildControls.end(),[&](const Window& Item){return Item.ID==ID?true:false;});
	if(Iterator==ChildControls.end()) throw std::exception();
	return *Iterator;
}

Window& Window::operator=(const Window& LeftRef)
{
	if(IsCreated_Value) Destroy();
	WindowHandle=LeftRef.WindowHandle;
	LeftRef.WindowHandle=nullptr;
	WindowClassName=std::move(LeftRef.WindowClassName);
	ChildControls=std::move(LeftRef.ChildControls);
	IsCreated_Value=LeftRef.IsCreated_Value;
	ID=LeftRef.ID;
	return *this;
}

Window& Window::operator=(Window&& RightRef)
{
	if(IsCreated_Value) Destroy();
	WindowHandle=RightRef.WindowHandle;
	RightRef.WindowHandle=nullptr;
	WindowClassName=std::move(RightRef.WindowClassName);
	ChildControls=std::move(RightRef.ChildControls);
	IsCreated_Value=RightRef.IsCreated_Value;
	ID=RightRef.ID;
	return *this;
}

DialogWindow::DialogWindow(const DialogWindow& LeftRef):Window((Window)LeftRef)
{
	WindowClass=LeftRef.WindowClass;
	ClassAtom=LeftRef.ClassAtom;
	IsRegistered_Value=LeftRef.IsRegistered_Value;
	LeftRef.IsRegistered_Value=false;
	return;
}

DialogWindow::DialogWindow(DialogWindow&& RightRef):Window(std::move((Window)RightRef))
{
	WindowClass=RightRef.WindowClass;
	ClassAtom=RightRef.ClassAtom;
	IsRegistered_Value=RightRef.IsRegistered_Value;
	RightRef.IsRegistered_Value=false;
	return;
}

DialogWindow::DialogWindow(std::wstring ClassName,WNDPROC Proc)
{
	WindowClassName=ClassName;
	InitWindowClass();
	WindowClass.lpfnWndProc=Proc;
	WindowClass.lpszClassName=ClassName.c_str();
	IsRegistered_Value=false;
	return;
}

DialogWindow::DialogWindow(std::wstring ClassName,WNDPROC Proc,Window* ParentWindow):Window(ParentWindow)
{
	WindowClassName=ClassName;
	InitWindowClass();
	WindowClass.lpszClassName=ClassName.c_str();
	WindowClass.lpfnWndProc=Proc;
	IsRegistered_Value=false;
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
	IsRegistered_Value=false;
	return;
}

DialogWindow::DialogWindow(std::wstring ClassName,WNDPROC Proc,HICON Icon,HICON SmallIcon,Window* ParentWindow):Window(ParentWindow)
{
	WindowClassName=ClassName;
	InitWindowClass();
	WindowClass.lpszClassName=ClassName.c_str();
	WindowClass.lpfnWndProc=Proc;
	WindowClass.hIcon=Icon;
	WindowClass.hIconSm=SmallIcon;
	IsRegistered_Value=false;
	return;
}

DialogWindow::DialogWindow(const WNDCLASSEX& WindowClass)
{
	memcpy(&this->WindowClass,&WindowClass,sizeof(this->WindowClass));
	if(WindowClass.lpszClassName) WindowClassName=WindowClass.lpszClassName;
	IsRegistered_Value=false;
	return;
}

DialogWindow::DialogWindow(const WNDCLASSEX& WindowClass,Window* ParentWindow):Window(ParentWindow)
{
	memcpy(&this->WindowClass,&WindowClass,sizeof(this->WindowClass));
	if(WindowClass.lpszClassName) WindowClassName=WindowClass.lpszClassName;
	IsRegistered_Value=false;
	return;
}

DialogWindow::~DialogWindow(void)
{
	if(IsRegistered_Value) Unregister();
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
	IsRegistered_Value=true;
	return true;
}

bool DialogWindow::Unregister(void)
{
	return UnregisterClass(WindowClassName.c_str(),GetModuleHandle(nullptr))?!(IsRegistered_Value=false):false;
}

DialogWindow& DialogWindow::operator=(const DialogWindow& LeftRef)
{
	if(IsCreated_Value) Destroy();
	WindowHandle=LeftRef.WindowHandle;
	LeftRef.WindowHandle=nullptr;
	WindowClassName=std::move(LeftRef.WindowClassName);
	ChildControls=std::move(LeftRef.ChildControls);
	IsCreated_Value=LeftRef.IsCreated_Value;
	ID=LeftRef.ID;
	if(IsRegistered_Value) Unregister();
	WindowClass=LeftRef.WindowClass;
	ClassAtom=LeftRef.ClassAtom;
	IsRegistered_Value=LeftRef.IsRegistered_Value;
	LeftRef.IsRegistered_Value=false;
	return *this;
}

DialogWindow& DialogWindow::operator=(DialogWindow&& RightRef)
{
	if(IsCreated_Value) Destroy();
	WindowHandle=RightRef.WindowHandle;
	RightRef.WindowHandle=nullptr;
	WindowClassName=std::move(RightRef.WindowClassName);
	ChildControls=std::move(RightRef.ChildControls);
	IsCreated_Value=RightRef.IsCreated_Value;
	ID=RightRef.ID;
	if(IsRegistered_Value) Unregister();
	WindowClass=RightRef.WindowClass;
	ClassAtom=RightRef.ClassAtom;
	IsRegistered_Value=RightRef.IsRegistered_Value;
	RightRef.IsRegistered_Value=false;
	return *this;
}
