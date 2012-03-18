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

Window::Window(const std::wstring& ClassName,int ID,Window* const ParentWindow)
{
	Init();
	*this->ID=ID;
	WindowClassName=ClassName;
	this->ParentWindow=ParentWindow;
	return;
}

void Window::Init(void)
{
	WindowHandle.reset(new HWND(),[](HWND* Obj){if(Obj&&IsWindow(*Obj)) DestroyWindow(*Obj);delete Obj;});
	ID=std::make_shared<int>();
	ChildControls=std::make_shared<std::vector<Window>>();
	*WindowHandle=nullptr;
	*ID=0;
	ParentWindow=nullptr;
	return;
}

bool Window::Create(const std::wstring& Caption,unsigned long WindowStyle,int X,int Y,int Width,int Height)
{
	if(!*WindowHandle){
		*WindowHandle=CreateWindowEx(0,WindowClassName.c_str(),Caption.c_str(),WindowStyle,
			X,Y,Width,Height,ParentWindow?ParentWindow->GetWindowHandle():nullptr,(HMENU)*ID,GetModuleHandle(nullptr),nullptr);
	}else if(!IsWindow(*WindowHandle)){
		*WindowHandle=nullptr;
		ChildControls->clear();
		return Create(Caption,WindowStyle,X,Y,Width,Height);
	}else return false;
	return *WindowHandle?true:false;
}

bool Window::Create(const std::wstring& Caption,unsigned long WindowStyle,unsigned long WindowStyleEx,int X,int Y,int Width,int Height)
{
	if(!*WindowHandle){
		*WindowHandle=CreateWindowEx(WindowStyleEx,WindowClassName.c_str(),Caption.c_str(),WindowStyle,
			X,Y,Width,Height,ParentWindow?ParentWindow->GetWindowHandle():nullptr,(HMENU)*ID,GetModuleHandle(nullptr),nullptr);
	}else if(!IsWindow(*WindowHandle)){
		*WindowHandle=nullptr;
		ChildControls->clear();
		return Create(Caption,WindowStyle,WindowStyleEx,X,Y,Width,Height);
	}else return false;
	return *WindowHandle?true:false;
}

bool Window::IsCreated(void)const
{
	return *WindowHandle?true:false;
}

bool Window::Destroy(void)
{
	if(IsWindow(*WindowHandle)){
		return DestroyWindow(*WindowHandle)?ChildControls->clear(),*WindowHandle=nullptr,true:false;
	}else if(*WindowHandle){
		ChildControls->clear();
		*WindowHandle=nullptr;
		return true;
	}else return false;
}

bool Window::Move(int X,int Y,int Width,int Height,bool Redraw)
{
	return IsWindow(*WindowHandle)&&MoveWindow(*WindowHandle,X,Y,Width,Height,Redraw)?true:false;
}

bool Window::Show(int Flag)
{
	return IsWindow(*WindowHandle)?ShowWindow(*WindowHandle,Flag),true:false;
}

bool Window::Update(void)
{
	return IsWindow(*WindowHandle)&&UpdateWindow(*WindowHandle)?true:false;
}

bool Window::ShowAndUpdate(int Flag)
{
	return IsWindow(*WindowHandle)&&ShowWindow(*WindowHandle,Flag)&&UpdateWindow(*WindowHandle)?true:false;
}

void Window::Focus(void)
{
	SetForegroundWindow(*WindowHandle);
	return;
}

long Window::Message(unsigned int Message,unsigned int WParam,long LParam)
{
	return IsWindow(*WindowHandle)?SendMessage(*WindowHandle,Message,WParam,LParam):0;
}

std::wstring Window::GetCaption(void)const
{
	if(!IsWindow(*WindowHandle)) return L"";
	std::vector<wchar_t> Buffer(512,0);
	GetWindowText(*WindowHandle,Buffer.data(),Buffer.size());
	return std::wstring(Buffer.data());
}

void Window::SetCaption(const std::wstring& Caption)
{
	if(!IsWindow(*WindowHandle)) return;
	SetWindowText(*WindowHandle,Caption.c_str());
	return;
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

std::wstring Window::GetWindowClassName(void)const
{
	return std::wstring(WindowClassName);
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

bool Window::AddControl(const Window& Control)
{
	return Control.ParentWindow==this?ChildControls->push_back(Control),true:false;
}

bool Window::AddControl(Window&& Control)
{
	return Control.ParentWindow==this?ChildControls->push_back(std::move(Control)),true:false;
}

bool Window::RemoveControl(const Window& Control)
{
	auto Iterator=std::find_if(ChildControls->cbegin(),ChildControls->cend(),[&](const Window& Item){return Item==Control;});
	if(Iterator==ChildControls->cend()) return false;
	ChildControls->erase(Iterator);
	return true;
}

bool Window::RemoveControl(const HWND& WindowHandle)
{
	auto Iterator=std::find_if(ChildControls->cbegin(),ChildControls->cend(),[&](const Window& Item){return *Item.WindowHandle==WindowHandle;});
	if(Iterator==ChildControls->cend()) return false;
	ChildControls->erase(Iterator);
	return true;
}

bool Window::RemoveControl(const int& ID)
{
	auto Iterator=std::find_if(ChildControls->cbegin(),ChildControls->cend(),[&](const Window& Item){return *Item.ID==ID;});
	if(Iterator==ChildControls->cend()) return false;
	ChildControls->erase(Iterator);
	return true;
}

Window& Window::GetControl(const HWND& WindowHandle)
{
	auto Iterator=std::find_if(ChildControls->begin(),ChildControls->end(),[&](const Window& Item){return *Item.WindowHandle==WindowHandle;});
	if(Iterator==ChildControls->end()) throw std::exception();
	return *Iterator;
}

Window& Window::GetControl(const int& ID)
{
	auto Iterator=std::find_if(ChildControls->begin(),ChildControls->end(),[&](const Window& Item){return *Item.ID==ID;});
	if(Iterator==ChildControls->end()) throw std::exception();
	return *Iterator;
}

const Window& Window::GetControl(const HWND& WindowHandle)const
{
	auto Iterator=std::find_if(ChildControls->cbegin(),ChildControls->cend(),[&](const Window& Item){return *Item.WindowHandle==WindowHandle;});
	if(Iterator==ChildControls->cend()) throw std::exception();
	return *Iterator;
}

const Window& Window::GetControl(const int& ID)const
{
	auto Iterator=std::find_if(ChildControls->cbegin(),ChildControls->cend(),[&](const Window& Item){return *Item.ID==ID;});
	if(Iterator==ChildControls->cend()) throw std::exception();
	return *Iterator;
}

bool Window::IsExistControl(const HWND& WindowHandle)const
{
	auto Iterator=std::find_if(ChildControls->cbegin(),ChildControls->cend(),[&](const Window& Item){return *Item.WindowHandle==WindowHandle;});
	return !(Iterator==ChildControls->cend());
}

bool Window::IsExistControl(const int& ID)const
{
	auto Iterator=std::find_if(ChildControls->cbegin(),ChildControls->cend(),[&](const Window& Item){return *Item.ID==ID;});
	return !(Iterator==ChildControls->cend());
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

Window& Window::operator()(const HWND& WindowHandle)
{
	auto Iterator=std::find_if(ChildControls->begin(),ChildControls->end(),[&](const Window& Item){return *Item.WindowHandle==WindowHandle;});
	if(Iterator==ChildControls->end()) throw std::exception();
	return *Iterator;
}

Window& Window::operator()(const int& ID)
{
	auto Iterator=std::find_if(ChildControls->begin(),ChildControls->end(),[&](const Window& Item){return *Item.ID==ID;});
	if(Iterator==ChildControls->end()) throw std::exception();
	return *Iterator;
}

const Window& Window::operator()(const HWND& WindowHandle)const
{
	auto Iterator=std::find_if(ChildControls->cbegin(),ChildControls->cend(),[&](const Window& Item){return *Item.WindowHandle==WindowHandle;});
	if(Iterator==ChildControls->cend()) throw std::exception();
	return *Iterator;
}

const Window& Window::operator()(const int& ID)const
{
	auto Iterator=std::find_if(ChildControls->cbegin(),ChildControls->cend(),[&](const Window& Item){return *Item.ID==ID;});
	if(Iterator==ChildControls->cend()) throw std::exception();
	return *Iterator;
}

DialogWindow::DialogWindow(void)
{
	Init();
	return;
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
	WindowClassName=WindowClass.lpszClassName;
	std::memcpy(&*this->WindowClass,&WindowClass,sizeof(WindowClass));
	this->WindowClass->lpszClassName=WindowClassName.c_str();
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
	return;
}

void DialogWindow::Init(void)
{
	WindowClass=std::make_shared<WNDCLASSEX>();
	ClassAtom.reset(new unsigned short,
		[](unsigned short* Obj){if(*Obj) UnregisterClass((wchar_t*)*Obj,GetModuleHandle(nullptr));delete Obj;});
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
	return !*ClassAtom&&(*ClassAtom=RegisterClassEx(WindowClass.get()))&&*ClassAtom?true:false;
}

bool DialogWindow::Unregister(void)
{
	return *ClassAtom&&UnregisterClass(WindowClassName.c_str(),GetModuleHandle(nullptr))?*ClassAtom=0,true:false;
}

bool DialogWindow::IsRegistered(void)const
{
	return *ClassAtom?true:false;
}

unsigned short DialogWindow::GetWindowClassAtom(void)const
{
	return *ClassAtom;
}

WNDPROC DialogWindow::GetWindowProc(void)const
{
	if(!IsWindow(*WindowHandle)) return nullptr;
	return (WNDPROC)GetWindowLong(*WindowHandle,GWL_WNDPROC);
}

void DialogWindow::SetWindowProc(WNDPROC& Proc)
{
	if(IsWindow(*WindowHandle)) return;
	SetWindowLong(*WindowHandle,GWL_WNDPROC,(long)Proc);
	return;
}

HICON DialogWindow::GetWindowIcon(void)const
{
	if(!IsWindow(*WindowHandle)) return nullptr;
	return (HICON)SendMessage(*WindowHandle,WM_GETICON,ICON_BIG,0);
}

void DialogWindow::SetWindowIcon(HICON& Icon)
{
	if(!IsWindow(*WindowHandle)) return;
	SendMessage(*WindowHandle,WM_SETICON,ICON_BIG,(LPARAM)Icon);
	return;
}

HICON DialogWindow::GetWindowSmallIcon(void)const
{
	if(!IsWindow(*WindowHandle)) return nullptr;
	return (HICON)SendMessage(*WindowHandle,WM_GETICON,ICON_SMALL,0);
}

void DialogWindow::SetWindowSmallIcon(HICON& SmallIcon)
{
	if(!IsWindow(*WindowHandle)) return;
	SendMessage(*WindowHandle,WM_SETICON,ICON_SMALL,(LPARAM)SmallIcon);
	return;
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

bool operator==(const Window& Left,const Window& Right)
{
	return Left.GetWindowHandle()==Right.GetWindowHandle()&&
		Left.GetWindowID()==Right.GetWindowID()&&
		Left.GetWindowClassName()==Right.GetWindowClassName()?true:false;;
}

bool operator==(const Window& Left,const HWND& Right)
{
	return Left.GetWindowHandle()==Right?true:false;
}

bool operator!=(const Window& Left,const Window& Right)
{
	return !(Left==Right);
}

bool operator!=(const Window& Left,const HWND& Right)
{
	return !(Left==Right);
}

bool operator==(const DialogWindow& Left,const DialogWindow& Right)
{
	return Left.GetWindowClassAtom()==Right.GetWindowClassAtom()&&Left.GetWindowClassName()==Right.GetWindowClassName()?true:false;
}

bool operator==(const DialogWindow& Left,const HWND& Right)
{
	return Left.GetWindowHandle()==Right?true:false;
}

bool operator!=(const DialogWindow& Left,const DialogWindow& Right)
{
	return !(Left==Right);
}

bool operator!=(const DialogWindow& Left,const HWND& Right)
{
	return !(Left==Right);
}