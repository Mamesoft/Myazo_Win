#include "ShareHeaders.h"
#include "Window.h"
#include "PNGFile.h"

class Myazo
{
private:
	HINSTANCE Instance;
	bool UploadAsPrivate,UtilityMode;
	PNGFile ImageEncoder;
	DialogWindow MainWindow,LayerWindow,AuthWindow;
	int ScreenSizeX,ScreenSizeY,OffsetX,OffsetY;

	Myazo(const Myazo&);
	Myazo(void);
	static LRESULT __stdcall WndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam);
	static LRESULT __stdcall LayerWndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam);
	bool InitWindow(void);
	bool Upload(void);
	void ProcessKeyMessage(unsigned int KeyChar);
	Myazo& operator=(const Myazo&);
	Myazo& operator=(Myazo&&);

public:
	~Myazo(void);
	static Myazo& GetInstance(void);
	bool Init(void);
	void EnterMessageLoop(void);

};