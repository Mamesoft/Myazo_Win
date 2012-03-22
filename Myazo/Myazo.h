#include "ShareHeaders.h"
#include "Window.h"
#include "Json.h"
#include "PNGFile.h"

class Myazo
{
private:
	typedef std::shared_ptr<void> Handle;

	HINSTANCE Instance;
	bool UploadAsPrivate,UtilityMode,CaptureStarted,FirstBoot;
	PNGFile ImageEncoder;
	DialogWindow MainWindow,LayerWindow,AuthWindow;
	std::array<Window,10> Controls;
	RECT CaptureRect;
	std::shared_ptr<Gdiplus::Font> LayerWindowFont,UIFont;
	const std::wstring AppName,UserAgent,Boundary,DefaultHeader,UploadDomain,UploadScriptPath,SettingFileName;
	std::wstring UserID,PassWord,SettingFileDirectory;
	Json::Parser JsonParser;

	Myazo(const Myazo&);
	Myazo(void);
	static LRESULT __stdcall WndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam);
	static LRESULT __stdcall LayerWndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam);
	static LRESULT __stdcall AuthWndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam);
	bool InitWindowClass(void);
	bool InitWindow(void);
	bool InitAuthWindow(void);
	void InitSetting(void);
	void SaveSetting(void);
	void ProcessKeyMessage(int VirtualKeyChar);
	void ProcessCommandMessage(int ID);
	void Encrypt(std::vector<wchar_t>& Data);
	void Decrypt(std::vector<wchar_t>& Data);
	std::wstring ReadSettingFile(void);
	void WriteSettingFile(const std::wstring& FileContent);
	Json::Item Authenticate(const std::wstring& UserID,const std::wstring& PassWord);
	void OpenAuthWindow(void);
	void SetClipboardText(const std::wstring& Test);
	void MoveLayerWindow(void);
	void DrawLayerWindowContent(void);
	void StartCapture(int X,int Y);
	void UpdatePosition(int X,int Y);
	void EndCapture(int X, int Y);
	std::shared_ptr<IStream> GetScreenShot(void);
	std::string CreateMessage(std::shared_ptr<IStream>& ImageFileStream);
	Json::Item Upload(const std::string& Message,const std::wstring& Header,const std::wstring& Domain,const std::wstring& ScriptPath);
	void OpenUrl(const std::wstring& Url);
	bool IsImageFile(const std::wstring& FileName);
	bool IsValidImageUrl(const std::wstring& Url);
	bool UploadImageFile(const std::wstring& FileName);
	void CheckArgumentOrUpload(void);
	std::wstring ToUnicode(const std::string& MultiByteString);
	std::wstring ToUnicode(const std::vector<char>& MultiByteBuffer);
	std::string ToMultiByte(const std::wstring& UnicodeString);
	std::string ToMultiByte(const std::vector<wchar_t>& UnicodeBuffer);
	Myazo& operator=(const Myazo&);
	Myazo& operator=(Myazo&&);

public:
	~Myazo(void);
	static Myazo& GetInstance(void);
	bool Init(void);
	void EnterMessageLoop(void);

};
