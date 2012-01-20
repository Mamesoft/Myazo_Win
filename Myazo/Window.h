#include "ShareHeaders.h"
#undef SendMessage

class ControlWindow;

class Window
{
private:
	Window(const Window&);

protected:
	HWND WindowHandle;
	Window* ParentWindow;
	std::wstring WindowClassName;
	std::vector<ControlWindow*> ChildControls;
	bool IsCreated_Value;
	int ID;

	Window(void);

public:
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
	Window* GetParentWindow(void)const;
	const std::wstring GetWindowClassName(void)const;
	unsigned long GetWindowStyle(void)const;
	void SetWindowStyle(unsigned long Style);
	unsigned long GetWindowStyleEx(void)const;
	void SetWindowStyleEx(unsigned long StyleEx);
	ControlWindow* AddControl(std::wstring ClassName,std::wstring Caption,unsigned long WindowStyle,int X,int Y,int Width,int Height,int ID);
	ControlWindow* AddControl(std::wstring ClassName,std::wstring Caption,unsigned long WindowStyle,unsigned long WindowStyleEx,int X,int Y,int Width,int Height,int ID);
	ControlWindow* AddControl(ControlWindow* const Control);
	bool RemoveControl(ControlWindow* const Control);
	bool RemoveControlByHandle(HWND WindowHandle);
	bool RemoveControlByID(int ID);
	ControlWindow* GetControl(int Index);
	ControlWindow* GetControlByHandle(HWND WindowHandle);
	ControlWindow* GetControlByID(int ControlID);
	
};

class DialogWindow : public Window
{
private:
	DialogWindow(const DialogWindow&);

protected:
	WNDCLASSEX WindowClass;
	unsigned short ClassAtom;
	bool IsRegistered_Value;

	void InitWindowClass(void);

public:
	DialogWindow(std::wstring ClassName,WNDPROC Proc);
	DialogWindow(std::wstring ClassName,WNDPROC Proc,HICON Icon,HICON SmallIcon);
	DialogWindow(std::wstring ClassName,WNDPROC Proc,HICON Icon,HICON SmallIcon,Window* const ParentWindow);
	DialogWindow(const WNDCLASSEX& WindowClass);
	DialogWindow(const WNDCLASSEX& WindowClass,Window* ParentWindow);
	~DialogWindow(void);
	bool Register(void);
	bool IsRegistered(void)const;
	unsigned short GetWindowClassAtom(void)const;
	WNDPROC GetWindowProc(void)const;
	HICON GetWindowIcon(void)const;
	HICON GetWindowSmallIcon(void)const;

};

class ControlWindow : public Window
{
private:
	ControlWindow(const ControlWindow&);

protected:


public:
	ControlWindow(std::wstring ClassName,int ID,Window* ParentWindow);
	~ControlWindow(void);

};