#include "ShareHeaders.h"
#undef SendMessage

class Window
{
protected:
	HWND WindowHandle;
	Window* const ParentWindow;
	std::wstring WindowClassName;
	std::vector<Window> ChildControls;
	bool IsCreated_Value;
	int ID;

	Window(void);
	Window(Window* ParentWindow);

public:
	Window(const Window& LeftRef);
	Window(Window&& RightRef);
	Window(std::wstring ClassName,int ID,Window* ParentWindow);
	~Window(void);
	bool Create(std::wstring Caption,unsigned long WindowStyle,int X,int Y,int Width,int Height);
	bool Create(std::wstring Caption,unsigned long WindowStyle,unsigned long WindowStyleEx,int X,int Y,int Width,int Height);
	bool IsCreated(void)const;
	bool Destroy(void);
	bool Move(int X,int Y,int Width,int Height,bool Redraw=true);
	bool Show(int Flag);
	bool Update(void);
	long SendMessage(unsigned int Message,WPARAM WParam,LPARAM LParam);
	HWND GetWindowHandle(void)const;
	int GetWindowID(void)const;
	Window& GetParentWindow(void)const;
	const std::wstring GetWindowClassName(void)const;
	unsigned long GetWindowStyle(void)const;
	void SetWindowStyle(unsigned long Style);
	unsigned long GetWindowStyleEx(void)const;
	void SetWindowStyleEx(unsigned long StyleEx);
	Window& AddControl(std::wstring ClassName,std::wstring Caption,unsigned long WindowStyle,int X,int Y,int Width,int Height,int ID);
	Window& AddControl(std::wstring ClassName,std::wstring Caption,unsigned long WindowStyle,unsigned long WindowStyleEx,int X,int Y,int Width,int Height,int ID);
	Window& AddControl(const Window& Control);
	Window& AddControl(Window&& Control);
	bool RemoveControl(const Window& Control);
	bool RemoveControlByHandle(HWND WindowHandle);
	bool RemoveControlByID(int ID);
	Window& GetControl(int Index);
	Window& GetControlByHandle(HWND WindowHandle);
	Window& GetControlByID(int ControlID);
	Window& operator=(const Window& LeftRef);
	Window& operator=(Window&& RightRef);
	
};

class DialogWindow : public Window
{
protected:
	WNDCLASSEX WindowClass;
	unsigned short ClassAtom;
	bool IsRegistered_Value;

	void InitWindowClass(void);

public:
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
	HICON GetWindowIcon(void)const;
	HICON GetWindowSmallIcon(void)const;
	DialogWindow& operator=(const DialogWindow& LeftRef);
	DialogWindow& operator=(DialogWindow&& RightRef);

};
