#include "ShareHeaders.h"

class Window
{
protected:
	std::shared_ptr<HWND> WindowHandle;
	std::shared_ptr<int> ID;
	std::shared_ptr<std::vector<Window>> ChildControls;
	std::wstring WindowClassName;
	Window* ParentWindow;

	Window(Window* ParentWindow);
	void Init(void);

public:
	Window(void);
	Window(const Window& LeftRef);
	Window(Window&& RightRef);
	Window(const std::wstring& ClassName,int ID,Window* const ParentWindow);
	bool Create(const std::wstring& Caption,unsigned long WindowStyle,int X,int Y,int Width,int Height);
	bool Create(const std::wstring& Caption,unsigned long WindowStyle,unsigned long WindowStyleEx,int X,int Y,int Width,int Height);
	bool IsCreated(void)const;
	bool Destroy(void);
	bool Move(int X,int Y,int Width,int Height,bool Redraw=true);
	bool Show(int Flag);
	bool Update(void);
	bool ShowAndUpdate(int Flag);
	void Focus(void);
	long Message(unsigned int Message,WPARAM WParam,LPARAM LParam);
	std::wstring GetCaption(void)const;
	void SetCaption(const std::wstring& Caption);
	HWND GetWindowHandle(void)const;
	int GetWindowID(void)const;
	Window* GetParentWindow(void)const;
	std::wstring GetWindowClassName(void)const;
	unsigned long GetWindowStyle(void)const;
	void SetWindowStyle(unsigned long Style);
	unsigned long GetWindowStyleEx(void)const;
	void SetWindowStyleEx(unsigned long StyleEx);
	bool AddControl(const Window& Control);
	bool AddControl(Window&& Control);
	bool RemoveControl(const Window& Control);
	bool RemoveControl(const HWND &WindowHandle);
	bool RemoveControl(const int& ID);
	Window& GetControl(const HWND& WindowHandle);
	Window& GetControl(const int& ID);
	const Window& GetControl(const HWND& WindowHandle)const;
	const Window& GetControl(const int& ID)const;
	bool IsExistControl(const HWND& WindowHandle)const;
	bool IsExistControl(const int& ID)const;
	Window& operator=(const Window& LeftRef);
	Window& operator=(Window&& RightRef);
	Window& operator()(const HWND& WindowHandle);
	Window& operator()(const int& ID);
	const Window& operator()(const HWND& WindowHandle)const;
	const Window& operator()(const int& ID)const;
	
};

class DialogWindow:public Window
{
protected:
	std::shared_ptr<WNDCLASSEX> WindowClass;
	std::shared_ptr<unsigned short> ClassAtom;

	void Init(void);

public:
	DialogWindow(void);
	DialogWindow(const DialogWindow& LeftRef);
	DialogWindow(DialogWindow&& RightRef);
	DialogWindow(std::wstring ClassName,WNDPROC Proc);
	DialogWindow(std::wstring ClassName,WNDPROC Proc,Window* ParentWindow);
	DialogWindow(std::wstring ClassName,WNDPROC Proc,HICON Icon,HICON SmallIcon);
	DialogWindow(std::wstring ClassName,WNDPROC Proc,HICON Icon,HICON SmallIcon,Window* ParentWindow);
	DialogWindow(const WNDCLASSEX& WindowClass);
	DialogWindow(const WNDCLASSEX& WindowClass,Window* ParentWindow);
	~DialogWindow(void);
	bool Register(void);
	bool Unregister(void);
	bool IsRegistered(void)const;
	unsigned short GetWindowClassAtom(void)const;
	WNDPROC GetWindowProc(void)const;
	void SetWindowProc(WNDPROC& Proc);
	HICON GetWindowIcon(void)const;
	void SetWindowIcon(HICON& Icon);
	HICON GetWindowSmallIcon(void)const;
	void SetWindowSmallIcon(HICON& SmallIcon);
	DialogWindow& operator=(const DialogWindow& LeftRef);
	DialogWindow& operator=(DialogWindow&& RightRef);

};

bool operator==(const Window& Left,const Window& Right);
bool operator==(const Window& Left,const HWND& Right);
bool operator!=(const Window& Left,const Window& Right);
bool operator!=(const Window& Left,const HWND& Right);
bool operator==(const DialogWindow& Left,const DialogWindow& Right);
bool operator==(const DialogWindow& Left,const HWND& Right);
bool operator!=(const DialogWindow& Left,const DialogWindow& Right);
bool operator!=(const DialogWindow& Left,const HWND& Right);