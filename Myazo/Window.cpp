#include "Window.h"

Window::Window(void)
{
	Init();
	return;
}

Window::Window(Window* ParentWindow)
{
	Init();
	this->ParentWindow=ParentWindow;
	return;
}

Window::Window(const Window& LeftRef)
{
	WindowHandle=LeftRef.WindowHandle;
	ID=LeftRef.ID;
	ChildControls=LeftRef.ChildControls;
	WindowClassName=LeftRef.WindowClassName;
	ParentWindow=LeftRef.ParentWindow;
	return;
}

Window::Window(Window&& RightRef)
{
	WindowHandle=std::move(RightRef.WindowHandle);
	ID=std::move(RightRef.ID);
	ChildControls=std::move(RightRef.ChildControls);
	WindowClassName=std::move(RightRef.WindowClassName);
	ParentWindow=RightRef.ParentWindow;
	return;
}

Window::Window(std::wstring ClassName,int ID,Window* ParentWindow)
{
	Init();
	*this->ID=ID;
	WindowClassName=ClassName;
	this->ParentWindow=ParentWindow;
	return;
}

void Window::Init(void)
{
	WindowHandle.reset(new HWND(),[](HWND Obj){if(Obj&&IsWindow(Obj)) DestroyWindow(Obj);});
	ID=std::make_shared<int>();
	ChildControls=std::make_shared<std::vector<Window>>();
	*WindowHandle=nullptr;
	*ID=0;
	ParentWindow=nullptr;
	return;
}

bool Window::Create(std::wstring Caption,unsigned long WindowStyle,int X,int Y,int Width,int Height)
{
	if(!*WindowHandle){
		*WindowHandle=CreateWindowEx(0,WindowClassName.c_str(),Caption.c_str(),WindowStyle,
			X,Y,Width,Height,ParentWindow?ParentWindow->GetWindowHandle():nullptr,(HMENU)*ID,GetModuleHandle(nullptr),nullptr);
	}else return false;
	return *WindowHandle?true:false;
}

bool Window::Create(std::wstring Caption,unsigned long WindowStyle,unsigned long WindowStyleEx,int X,int Y,int Width,int Height)
{
	if(!*WindowHandle){
		*WindowHandle=CreateWindowEx(WindowStyleEx,WindowClassName.c_str(),Caption.c_str(),WindowStyle,
			X,Y,Width,Height,ParentWindow?ParentWindow->GetWindowHandle():nullptr,(HMENU)*ID,GetModuleHandle(nullptr),nullptr);
	}else return false;
	return *WindowHandle?true:false;
}

bool Window::IsCreated(void)const
{
	return *WindowHandle?true:false;
}

bool Window::Destroy(void)
{
	return IsWindow(*WindowHandle)&&DestroyWindow(*WindowHandle)?ChildControls->clear(),*WindowHandle=nullptr,true:false;
}

bool Window::Move(int X,int Y,int Width,int Height,bool Redraw=true)
{
	return IsWindow(*WindowHandle)&&MoveWindow(*WindowHandle,X,Y,Width,Height,Redraw)?true:false;
}

bool Window::Show(int Flag)
{
	return IsWindow(*WindowHandle)?ShowWindow(*WindowHandle,Flag),true:false;
}

bool Window::Update(void)
{
	if(IsWindow(*WindowHandle)&&UpdateWindow(*WindowHandle)) return true;
	else return false;
}

long Window::SendMessage(unsigned int Message,unsigned int WParam,long LParam)
{
	return IsWindow(*WindowHandle)?SendMessageW(*WindowHandle,Message,WParam,LParam):0;
}

HWND Window::GetWindowHandle(void)const
{
	return *WindowHandle;
}

int Window::GetWindowID(void)const
{
	return *ID;
}

Window* Window::GetParentWindow(void)const
{
	return ParentWindow;
}

unsigned long Window::GetWindowStyle(void)const
{
	return *WindowHandle&&IsWindow(*WindowHandle)?GetWindowLong(*WindowHandle,GWL_STYLE):0;
}

void Window::SetWindowStyle(unsigned long Style)
{
	if(*WindowHandle&&IsWindow(*WindowHandle)) SetWindowLong(*WindowHandle,GWL_STYLE,Style);
	return;
}

unsigned long Window::GetWindowStyleEx(void)const
{
	return *WindowHandle&&IsWindow(*WindowHandle)?GetWindowLong(*WindowHandle,GWL_EXSTYLE):0;
}

void Window::SetWindowStyleEx(unsigned long StyleEx)
{
	if(*WindowHandle&&IsWindow(*WindowHandle)) SetWindowLong(*WindowHandle,GWL_EXSTYLE,StyleEx);
	return;
}

bool Window::AddControl(std::wstring ClassName,std::wstring Caption,unsigned long WindowStyle,int X,int Y,int Width,int Height,int ID)
{
	Window Control(ClassName,ID,this);
	return Control.Create(Caption,WindowStyle,X,Y,Width,Height)?ChildControls->push_back(Control),true:false;
}

bool Window::AddControl(std::wstring ClassName,std::wstring Caption,unsigned long WindowStyle,unsigned long WindowStyleEx,int X,int Y,int Width,int Height,int ID)
{
	Window Control(ClassName,ID,this);
	return Control.Create(Caption,WindowStyle,WindowStyleEx,X,Y,Width,Height)?ChildControls->push_back(std::move(Control)),true:false;
}

bool Window::AddControl(const Window& Control)
{
	return Control==*this?ChildControls->push_back(Control),true:false;
}

bool Window::AddControl(Window&& Control)
{
	return Control==*this?ChildControls->push_back(std::move(Control)),true:false;
}

bool Window::RemoveControl(const Window& Control)
{
	auto Iterator=std::find_if(ChildControls->cbegin(),ChildControls->cend(),[&](const Window& Item){return Item==Control?true:false;});
	if(Iterator==ChildControls->cend()) return false;
	ChildControls->erase(Iterator);
	return true;
}

bool Window::RemoveControlByHandle(HWND WindowHandle)
{
	auto Iterator=std::find_if(ChildControls->cbegin(),ChildControls->cend(),[&](const Window& Item){return *Item.WindowHandle==WindowHandle?true:false;});
	if(Iterator==ChildControls->cend()) return false;
	ChildControls->erase(Iterator);
	return true;
}

bool Window::RemoveControlByID(int ID)
{
	auto Iterator=std::find_if(ChildControls->cbegin(),ChildControls->cend(),[&](const Window& Item){return *Item.ID==ID?true:false;});
	if(Iterator==ChildControls->cend()) return false;
	ChildControls->erase(Iterator);
	return true;
}

Window& Window::GetControl(int Index)
{
	if(Index<0||Index>=ChildControls->size()) throw std::exception();
	return ChildControls->at(Index);
}

Window& Window::GetControlByHandle(HWND WindowHandle)
{
	auto Iterator=std::find_if(ChildControls->begin(),ChildControls->end(),[&](const Window& Item){return *Item.WindowHandle==WindowHandle?true:false;});
	if(Iterator==ChildControls->end()) throw std::exception();
	return *Iterator;
}

Window& Window::GetControlByID(int ID)
{
	auto Iterator=std::find_if(ChildControls->begin(),ChildControls->end(),[&](const Window& Item){return *Item.ID==ID?true:false;});
	if(Iterator==ChildControls->end()) throw std::exception();
	return *Iterator;
}

Window& Window::operator=(const Window& LeftRef)
{
	if(*WindowHandle) Destroy();
	WindowHandle=LeftRef.WindowHandle;
	ID=LeftRef.ID;
	ChildControls=std::move(LeftRef.ChildControls);
	WindowClassName=std::move(LeftRef.WindowClassName);
	ParentWindow=LeftRef.ParentWindow;
	return *this;
}

Window& Window::operator=(Window&& RightRef)
{
	if(*WindowHandle) Destroy();
	WindowHandle=std::move(RightRef.WindowHandle);
	ID=std::move(RightRef.ID);
	ChildControls=std::move(RightRef.ChildControls);
	WindowClassName=std::move(RightRef.WindowClassName);
	ParentWindow=RightRef.ParentWindow;
	return *this;
}

bool Window::operator==(const Window& LeftRef)const
{
	return *WindowHandle==*LeftRef.WindowHandle&&*ID==*LeftRef.ID?true:false;
}

bool Window::operator!=(const Window& LeftRef)const
{
	return !operator==(LeftRef);
}

DialogWindow::DialogWindow(const DialogWindow& LeftRef):Window((Window)LeftRef)
{
	WindowClass=LeftRef.WindowClass;
	ClassAtom=LeftRef.ClassAtom;
	return;
}

DialogWindow::DialogWindow(DialogWindow&& RightRef):Window(std::move((Window)RightRef))
{
	WindowClass=std::move(RightRef.WindowClass);
	ClassAtom=std::move(RightRef.ClassAtom);
	return;
}

DialogWindow::DialogWindow(std::wstring ClassName,WNDPROC Proc)
{
	WindowClassName=ClassName;
	Init();
	WindowClass->lpfnWndProc=Proc;
	WindowClass->lpszClassName=ClassName.c_str();
	return;
}

DialogWindow::DialogWindow(std::wstring ClassName,WNDPROC Proc,Window* ParentWindow):Window(ParentWindow)
{
	WindowClassName=ClassName;
	Init();
	WindowClass->lpszClassName=ClassName.c_str();
	WindowClass->lpfnWndProc=Proc;
	return;
}

DialogWindow::DialogWindow(std::wstring ClassName,WNDPROC Proc,HICON Icon,HICON SmallIcon)
{
	WindowClassName=ClassName;
	Init();
	WindowClass->lpszClassName=ClassName.c_str();
	WindowClass->lpfnWndProc=Proc;
	WindowClass->hIcon=Icon;
	WindowClass->hIconSm=SmallIcon;
	return;
}

DialogWindow::DialogWindow(std::wstring ClassName,WNDPROC Proc,HICON Icon,HICON SmallIcon,Window* ParentWindow):Window(ParentWindow)
{
	WindowClassName=ClassName;
	Init();
	WindowClass->lpszClassName=ClassName.c_str();
	WindowClass->lpfnWndProc=Proc;
	WindowClass->hIcon=Icon;
	WindowClass->hIconSm=SmallIcon;
	return;
}

DialogWindow::DialogWindow(const WNDCLASSEX& WindowClass)
{
	Init();
	*this->WindowClass=WindowClass;
	if(this->WindowClass->lpszClassName) WindowClassName=WindowClass.lpszClassName;
	return;
}

DialogWindow::DialogWindow(const WNDCLASSEX& WindowClass,Window* ParentWindow):Window(ParentWindow)
{
	Init();
	*this->WindowClass=WindowClass;
	if(this->WindowClass->lpszClassName) WindowClassName=WindowClass.lpszClassName;
	return;
}

DialogWindow::~DialogWindow(void)
{
	if(ClassAtom) Unregister();
	return;
}

void DialogWindow::Init(void)
{
	ZeroMemory(WindowClass.get(),sizeof(*WindowClass));
	WindowClass->cbSize=sizeof(WindowClass);
	WindowClass->hbrBackground=(HBRUSH)COLOR_WINDOW;
	WindowClass->hCursor=LoadCursor(nullptr,IDC_ARROW);
	WindowClass->hInstance=GetModuleHandle(nullptr);
	WindowClass->lpszClassName=WindowClassName.c_str();
	WindowClass->style=CS_HREDRAW|CS_VREDRAW;
	WindowClass->lpszMenuName=nullptr;
	*ClassAtom=0;
	return;
}

bool DialogWindow::Register(void)
{
	return !*ClassAtom&&(*ClassAtom=RegisterClassEx(WindowClass.get()))&&!*ClassAtom?true:false;
}

bool DialogWindow::Unregister(void)
{
	return *ClassAtom&&UnregisterClass(WindowClassName.c_str(),GetModuleHandle(nullptr))?*ClassAtom=0,true:false;
}

DialogWindow& DialogWindow::operator=(const DialogWindow& LeftRef)
{
	if(*WindowHandle) Destroy();
	WindowHandle=LeftRef.WindowHandle;
	ID=LeftRef.ID;
	ChildControls=LeftRef.ChildControls;
	WindowClassName=LeftRef.WindowClassName;
	if(*ClassAtom) Unregister();
	WindowClass=LeftRef.WindowClass;
	ClassAtom=LeftRef.ClassAtom;
	return *this;
}

DialogWindow& DialogWindow::operator=(DialogWindow&& RightRef)
{
	if(*WindowHandle) Destroy();
	WindowHandle=std::move(RightRef.WindowHandle);
	ID=std::move(RightRef.ID);
	ChildControls=std::move(RightRef.ChildControls);
	WindowClassName=std::move(RightRef.WindowClassName);
	if(*ClassAtom) Unregister();
	WindowClass=std::move(RightRef.WindowClass);
	ClassAtom=std::move(RightRef.ClassAtom);
	return *this;
}
