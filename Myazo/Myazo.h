#include "ShareHeaders.h"
#include "Window.h"
#include "Json.h"
#include "PNGFile.h"

class Myazo
{
private:
	HINSTANCE Instance;
	bool UploadAsPrivate,UtilityMode,CaptureStarted;
	PNGFile ImageEncoder;
	DialogWindow MainWindow,LayerWindow,AuthWindow;
	RECT CaptureRect;
	std::shared_ptr<Gdiplus::Font> LayerWindowFont;
	const std::wstring AppName,UserAgent,UploadDomain,UploadScriptPath,TempFileNamePrefix;
	std::wstring UserID,PassWord;
	Json::Parser JsonParser;

	Myazo(const Myazo&);
	Myazo(void);
	static LRESULT __stdcall WndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam);
	static LRESULT __stdcall LayerWndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam);
	static LRESULT __stdcall AuthWndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam);
	bool InitWindow(void);
	void ProcessKeyMessage(void);
	void MoveLayerWindow(void);
	void DrawLayerWindowContent(void);
	void StartCapture(int X,int Y);
	void UpdatePosition(int X,int Y);
	void EndCapture(int X, int Y);
	std::shared_ptr<IStream> GetScreenShot(void);
	std::string CreateMessage(std::shared_ptr<IStream>& ImageFileStream);
	std::wstring Upload(const std::string& Message);
	void OpenUrl(const std::wstring& Url);
	bool IsImageFile(const std::wstring& FileName);
	int UploadImageFile(const std::wstring& FileName);
	Myazo& operator=(const Myazo&);
	Myazo& operator=(Myazo&&);

public:
	~Myazo(void);
	static Myazo& GetInstance(void);
	bool Init(void);
	void EnterMessageLoop(void);

};
