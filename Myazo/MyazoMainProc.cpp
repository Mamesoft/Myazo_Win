#include "Myazo.h"

LRESULT __stdcall Myazo::WndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam)
{
	Myazo& Program=GetInstance();
	switch(Message){
	case WM_LBUTTONDOWN:
		Program.StartCapture(LOWORD(LParam),HIWORD(LParam));
		break;
	case WM_LBUTTONUP:
		{
			Program.EndCapture(LOWORD(LParam),HIWORD(LParam));
			auto ResultString=Program.Upload(Program.CreateMessage(Program.GetScreenShot()));
			if(std::regex_match(ResultString,std::wregex(L"http://myazo.net/(./)?[0-9a-f]{32}\\.png"))) Program.OpenUrl(ResultString);
			else MessageBox(nullptr,ResultString.c_str(),Program.AppName.c_str(),MB_OK|MB_ICONERROR);
		}
	case WM_RBUTTONDOWN:
		Program.LayerWindow.Destroy();
		Program.MainWindow.Destroy();
		return DefWindowProc(WindowHandle,Message,WParam,LParam);
	case WM_MOUSEMOVE:
		if(Program.CaptureStarted) Program.UpdatePosition(LOWORD(LParam),HIWORD(LParam));
		break;
	case WM_TIMER:
		if(WParam==100) Program.ProcessKeyMessage();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(WindowHandle,Message,WParam,LParam);
	}
	return 0;
}

LRESULT __stdcall Myazo::LayerWndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam)
{
	Myazo& Program=GetInstance();
	switch(Message){
	case WM_PAINT:
		PAINTSTRUCT PaintStruct;
		BeginPaint(WindowHandle,&PaintStruct);
		Program.DrawLayerWindowContent();
		EndPaint(WindowHandle,&PaintStruct);
		break;
	default:
		return DefWindowProc(WindowHandle,Message,WParam,LParam);
	}
	return 0;
}

LRESULT __stdcall Myazo::AuthWndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam)
{
	Myazo& Program=GetInstance();
	switch(Message){
	default:
		return DefWindowProc(WindowHandle,Message,WParam,LParam);
	}
	return 0;
}

void Myazo::ProcessKeyMessage(void)
{
	if(GetKeyState(VK_ESCAPE)&0x8000) MainWindow.Destroy();
	else if(GetKeyState('P')&0x8000){
		UploadAsPrivate=!UploadAsPrivate;
		MessageBox(nullptr,
			UploadAsPrivate?L"非公開モードでアップロードされます。":L"公開モードでアップロードされます。",AppName.c_str(),MB_OK);
	}else if(GetKeyState('A')&0x8000){
		int WindowWidth=300,WindowHeight=150;
		AuthWindow.Create(L"アカウントの認証",WS_CAPTION|WS_SYSMENU,0,
			(GetSystemMetrics(SM_CXVIRTUALSCREEN)-WindowWidth)/2,
			(GetSystemMetrics(SM_CYVIRTUALSCREEN)-WindowHeight)/2,WindowWidth,WindowHeight);
	}
	return;
}

void Myazo::MoveLayerWindow(void)
{
	LayerWindow.Move(CaptureRect.left<CaptureRect.right?CaptureRect.left:CaptureRect.right,
		CaptureRect.top<CaptureRect.bottom?CaptureRect.top:CaptureRect.bottom,
		std::abs(CaptureRect.right-CaptureRect.left)+1,std::abs(CaptureRect.bottom-CaptureRect.top)+1,true);
	return;
}

void Myazo::DrawLayerWindowContent(void)
{
	std::shared_ptr<Gdiplus::Graphics> LayerWindowGraphics(new Gdiplus::Graphics(LayerWindow.GetWindowHandle()));
	std::shared_ptr<Gdiplus::StringFormat> Layout(new Gdiplus::StringFormat(Gdiplus::StringFormatFlagsNoWrap));
	std::shared_ptr<Gdiplus::SolidBrush> BlackBrush(new Gdiplus::SolidBrush(Gdiplus::Color::Black)),WhiteBrush(new Gdiplus::SolidBrush(Gdiplus::Color::White));
	Layout->SetAlignment(Gdiplus::StringAlignment::StringAlignmentFar);
	Layout->SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentFar);
	Layout->SetTrimming(Gdiplus::StringTrimmingNone);
	RECT ClientRect;
	GetClientRect(LayerWindow.GetWindowHandle(),&ClientRect);
	Gdiplus::RectF LayoutRect(ClientRect.left,ClientRect.top,ClientRect.right-5,ClientRect.bottom-5);
	std::vector<wchar_t> WindowSize(64);
	_itow(ClientRect.right,WindowSize.data(),10);
	auto Null=std::find(WindowSize.begin(),WindowSize.end(),0);
	*Null++=L'\n';
	_itow(ClientRect.bottom,Null._Ptr,10);
	int Count=std::wcslen(WindowSize.data());
	LayerWindowGraphics->DrawString(WindowSize.data(),Count,&*LayerWindowFont,LayoutRect,&*Layout,&*BlackBrush);
	LayoutRect.Inflate(-1,-1);
	LayerWindowGraphics->DrawString(WindowSize.data(),Count,&*LayerWindowFont,LayoutRect,&*Layout,&*WhiteBrush);
	return;
}

void Myazo::StartCapture(int X,int Y)
{
	CaptureStarted=true;
	SetCapture(MainWindow.GetWindowHandle());
	CaptureRect.left=X;
	CaptureRect.top=Y;
	LayerWindow.ShowAndUpdate(SW_SHOW);
	return;
}

void Myazo::UpdatePosition(int X,int Y)
{
	CaptureRect.right=X;
	CaptureRect.bottom=Y;
	MoveLayerWindow();
	return;
}

void Myazo::EndCapture(int X,int Y)
{
	CaptureStarted=false;
	ReleaseCapture();
	CaptureRect.right=X;
	CaptureRect.bottom=Y;
	LayerWindow.Show(SW_HIDE);
	return;
}

std::shared_ptr<IStream> Myazo::GetScreenShot(void)
{
	HDC DesktopDC=GetDC(nullptr),CaptureDC=CreateCompatibleDC(DesktopDC);
	unsigned int X=CaptureRect.left<CaptureRect.right?CaptureRect.left:CaptureRect.right,
		Y=CaptureRect.top<CaptureRect.bottom?CaptureRect.top:CaptureRect.bottom,
		Width=std::abs(CaptureRect.right-CaptureRect.left)+1,
		Height=std::abs(CaptureRect.bottom-CaptureRect.top)+1;
	HBITMAP CaptureBitmapHandle=CreateCompatibleBitmap(DesktopDC,Width,Height),PrevBitmapHandle=(HBITMAP)SelectObject(CaptureDC,CaptureBitmapHandle);
	BitBlt(CaptureDC,0,0,Width,Height,DesktopDC,X,Y,SRCCOPY);
	MainWindow.Show(SW_HIDE);
	IStream* Stream;
	CreateStreamOnHGlobal(nullptr,true,&Stream);
	std::shared_ptr<IStream> ImageFileStream(Stream,[](IStream* Obj){if(Obj) Obj->Release();});
	ImageEncoder.Save(CaptureBitmapHandle,&*ImageFileStream);
	SelectObject(CaptureDC,PrevBitmapHandle);
	DeleteObject(CaptureBitmapHandle);
	DeleteDC(CaptureDC);
	ReleaseDC(nullptr,DesktopDC);
	return ImageFileStream;
}

std::string Myazo::CreateMessage(std::shared_ptr<IStream>& ImageFileStream)
{
	std::string Boundary="h-o-g-e-p-i-y-o",NewLine="\r\n";
	unsigned long WrittenSize;
	STATSTG State;
	ImageFileStream->Stat(&State,STATFLAG_NONAME);
	std::vector<char> ImageFileData(State.cbSize.LowPart),AsciiString(128);
	LARGE_INTEGER Position;
	Position.QuadPart=0;
	ImageFileStream->Seek(Position,STREAM_SEEK_SET,nullptr);
	ImageFileStream->Read(ImageFileData.data(),State.cbSize.LowPart,&WrittenSize);
	std::ostringstream Message;
	std::string Template="--"+Boundary+NewLine+"Content-Disposition: form-data; name=";
	std::wcstombs(AsciiString.data(),UploadDomain.c_str(),UploadDomain.length()+1);
	Message<<Template<<"\"imagedata\"; filename=\""<<AsciiString.data()<<"\""<<NewLine<<"Content-Type: image/png"<<NewLine<<NewLine;
	Message.write(ImageFileData.data(),ImageFileData.size())<<NewLine;
	std::wcstombs(AsciiString.data(),UserID.c_str(),UserID.length()+1);
	Message<<Template<<"\"id\""<<NewLine<<NewLine<<AsciiString.data()<<NewLine;
	std::wcstombs(AsciiString.data(),PassWord.c_str(),PassWord.length()+1);
	Message<<Template<<"\"pass\""<<NewLine<<NewLine<<AsciiString.data()<<NewLine;
	Message<<Template<<"\"private\""<<NewLine<<NewLine<<(UploadAsPrivate?"true":"false")<<NewLine;
	Message<<"--"<<Boundary<<"--"<<NewLine;
	return Message.str();
}

std::wstring Myazo::Upload(const std::string& Message)
{
	std::wstring Header=L"Content-Type: multipart/form-data; boundary=h-o-g-e-p-i-y-o";
	auto DestroyFunction=[](void* Obj){if(Obj) InternetCloseHandle(Obj);};
	std::shared_ptr<void> SessionHandle,ConnectionHandle,RequestHandle;
	SessionHandle.reset(InternetOpen(AppName.c_str(),INTERNET_OPEN_TYPE_PRECONFIG,nullptr,nullptr,0),DestroyFunction);
	ConnectionHandle.reset(InternetConnect(SessionHandle.get(),UploadDomain.c_str(),80,nullptr,nullptr,INTERNET_SERVICE_HTTP,0,0),DestroyFunction);
	RequestHandle.reset(HttpOpenRequest(ConnectionHandle.get(),L"POST",UploadScriptPath.c_str(),nullptr,nullptr,nullptr,INTERNET_FLAG_NO_CACHE_WRITE,0),DestroyFunction);
	HttpAddRequestHeaders(RequestHandle.get(),(L"User-Agent: "+UserAgent+L"\r\n").c_str(),std::wcslen(L"User-Agent: Myazo_win/1.00\r\n"),HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE);
	DWORD WrittenSize;
	INTERNET_BUFFERS Buffer;
	ZeroMemory(&Buffer,sizeof(Buffer));
	Buffer.dwStructSize=sizeof(INTERNET_BUFFERS);
	Buffer.dwBufferTotal=Message.length();
	Buffer.lpcszHeader=Header.c_str();
	Buffer.dwHeadersLength=Header.length();
	if(HttpSendRequestEx(RequestHandle.get(),&Buffer,nullptr,0,0)&&InternetWriteFile(RequestHandle.get(),Message.c_str(),Message.length(),&WrittenSize)){
		HttpEndRequest(RequestHandle.get(),nullptr,0,0);
		std::vector<char> Temp;
		Temp.resize(1024,0);
		std::vector<wchar_t> TempUnicode(Temp.size());
		InternetReadFile(RequestHandle.get(),Temp.data(),Temp.size(),&WrittenSize);
		std::mbstowcs(TempUnicode.data(),Temp.data(),WrittenSize+1);
		auto Result=JsonParser.Parse(std::wstring(TempUnicode.data()));
		return !Result(L"error")?Result(L"imgurl"):Result(L"errormessage");
	}
	return L"";
}

void Myazo::OpenUrl(const std::wstring& Url)
{
	SHELLEXECUTEINFO ExecuteInfo;
	ZeroMemory(&ExecuteInfo,sizeof(ExecuteInfo));
	ExecuteInfo.cbSize=sizeof(ExecuteInfo);
	ExecuteInfo.lpFile=Url.c_str();
	ShellExecuteExW(&ExecuteInfo);
	return;
}

bool Myazo::IsImageFile(const std::wstring& FileName)
{
	return std::regex_match(FileName,std::wregex(L".+\\.(bmp|png|gif|jpg|jpeg|jfif|tif|tiff|ico)",std::regex_constants::ECMAScript|std::regex_constants::icase));
}

int Myazo::UploadImageFile(const std::wstring& FileName)
{
	auto ReleaseFunction=[](IStream* Obj){if(Obj) Obj->Release();};
	IStream* StreamPtr;
	std::shared_ptr<IStream> ImageFileStream;
	if(std::regex_match(__wargv[1],std::wregex(L".+\\.png",std::regex_constants::ECMAScript|std::regex_constants::icase))){
		if(ImageEncoder.IsWellHeader(FileName)){
			SHCreateStreamOnFile(__wargv[1],STGM_READWRITE,&StreamPtr);
			ImageFileStream.reset(StreamPtr,ReleaseFunction);
		}else return MessageBox(nullptr,L"ファイルフォーマットが不正です。",AppName.c_str(),MB_OK|MB_ICONERROR),1;
	}else{
		Gdiplus::Bitmap Image(FileName.c_str());
		CreateStreamOnHGlobal(nullptr,true,&StreamPtr);
		if(!ImageEncoder.Save(&Image,StreamPtr)) return MessageBox(nullptr,L"PNG形式に変換出来ませんでした。",AppName.c_str(),MB_OK|MB_ICONERROR),1;
		ImageFileStream.reset(StreamPtr,ReleaseFunction);
	}
	auto ResultString=Upload(CreateMessage(ImageFileStream));
	return std::regex_match(ResultString,std::wregex(L"http://myazo.net/(./)?[0-9a-f]{32}\\.png"))?OpenUrl(ResultString),0:
		MessageBox(nullptr,ResultString.c_str(),AppName.c_str(),MB_OK|MB_ICONERROR),1;
}