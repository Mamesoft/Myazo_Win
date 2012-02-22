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
	Window(std::wstring ClassName,int ID,Window* ParentWindow);
	bool Create(std::wstring Caption,unsigned long WindowStyle,int X,int Y,int Width,int Height);
	bool Create(std::wstring Caption,unsigned long WindowStyle,unsigned long WindowStyleEx,int X,int Y,int Width,int Height);
	bool IsCreated(void)const;
	bool Destroy(void);
	bool Move(int X,int Y,int Width,int Height,bool Redraw=true);
	bool Show(int Flag);
	bool Update(void);
	long Message(unsigned int Message,WPARAM WParam,LPARAM LParam);
	HWND GetWindowHandle(void)const;
	int GetWindowID(void)const;
	Window* GetParentWindow(void)const;
	std::wstring GetWindowClassName(void)const;
	unsigned long GetWindowStyle(void)const;
	void SetWindowStyle(unsigned long Style);
	unsigned long GetWindowStyleEx(void)const;
	void SetWindowStyleEx(unsigned long StyleEx);
	bool AddControl(std::wstring ClassName,std::wstring Caption,unsigned long WindowStyle,int X,int Y,int Width,int Height,int ID);
	bool AddControl(std::wstring ClassName,std::wstring Caption,unsigned long WindowStyle,unsigned long WindowStyleEx,int X,int Y,int Width,int Height,int ID);
	bool AddControl(const Window& Control);
	bool AddControl(Window&& Control);
	bool RemoveControl(const Window& Control);
	bool RemoveControlByHandle(HWND WindowHandle);
	bool RemoveControlByID(int ID);
	Window& GetControl(int Index);
	Window& GetControlByHandle(HWND WindowHandle);
	Window& GetControlByID(int ControlID);
	bool IsExistControlByHandle(HWND WindowHandle);
	bool IsExistControlByID(int ControlID);
	Window& operator=(const Window& LeftRef);
	Window& operator=(Window&& RightRef);
	bool operator==(const Window& LeftRef)const;
	bool operator!=(const Window& LeftRef)const;
	
};

class DialogWindow : public Window
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
	HICON GetWindowIcon(void)const;
	HICON GetWindowSmallIcon(void)const;
	DialogWindow& operator=(const DialogWindow& LeftRef);
	DialogWindow& operator=(DialogWindow&& RightRef);
	bool operator==(const DialogWindow& LeftRef)const;
	bool operator!=(const DialogWindow& LeftRef)const;

};