#include "Myazo.h"

LRESULT __stdcall Myazo::WndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam)
{
	Myazo& Program=GetInstance();
	switch(Message){
	case WM_LBUTTONDOWN:
		Program.StartCapture(LOWORD(LParam),HIWORD(LParam));
		break;
	case WM_LBUTTONUP:
		Program.EndCapture(LOWORD(LParam),HIWORD(LParam));
	case WM_RBUTTONDOWN:
		Program.MainWindow.Destroy();
		Program.LayerWindow.Destroy();
		return DefWindowProc(WindowHandle,Message,WParam,LParam);
	case WM_MOUSEMOVE:
		if(Program.CaptureStarted) Program.UpdatePosition(LOWORD(LParam),HIWORD(LParam));
		break;
	case WM_KEYDOWN:
		Program.ProcessKeyMessage(WParam);
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
	case WM_COMMAND:
		Program.ProcessCommandMessage(LOWORD(WParam));
		break;
	case WM_DESTROY:
		Program.MainWindow.Show(SW_SHOW);
		break;
	default:
		return DefWindowProc(WindowHandle,Message,WParam,LParam);
	}
	return 0;
}

void Myazo::ProcessKeyMessage(int VirtualKeyChar)
{
	switch(VirtualKeyChar){
	case VK_ESCAPE:
		MainWindow.Destroy();
		LayerWindow.Destroy();
		break;
	case 'P':
		UploadAsPrivate=!UploadAsPrivate;
		MainWindow.Show(SW_HIDE);
		MessageBox(nullptr,
			UploadAsPrivate?L"非公開モードでアップロードされます。":L"公開モードでアップロードされます。",AppName.c_str(),MB_OK);
		MainWindow.Show(SW_SHOW);
		MainWindow.Focus();
		break;
	case 'A':
		MainWindow.Show(SW_HIDE);
		int WindowWidth=300,WindowHeight=150;
		AuthWindow.Create(L"アカウントの認証",WS_CAPTION|WS_SYSMENU,0,
			(GetSystemMetrics(SM_CXVIRTUALSCREEN)-WindowWidth)/2,
			(GetSystemMetrics(SM_CYVIRTUALSCREEN)-WindowHeight)/2,WindowWidth,WindowHeight);
		InitAuthWindow();
		AuthWindow.ShowAndUpdate(SW_SHOW);
		AuthWindow.Focus();
	}
	return;
}

void Myazo::ProcessCommandMessage(int ID)
{
	if(AuthWindow.IsExistControl(ID)){
		auto Control=AuthWindow(ID);
		if(Control==Controls[5]) OpenUrl(L"http://myazo.net/start/");
		else if(Control==Controls[6]){
			auto Result=Authenticate(Controls[2].GetCaption(),Controls[4].GetCaption());
			if(!Result(L"error")){
				MessageBox(AuthWindow.GetWindowHandle(),L"認証に成功しました。",AppName.c_str(),MB_OK|MB_ICONINFORMATION);
				UserID=Controls[2].GetCaption();
				PassWord=Controls[4].GetCaption();
				AuthWindow.Message(WM_CLOSE,0,0);
			}else{
				MessageBox(AuthWindow.GetWindowHandle(),Result(L"errormessage").String().c_str(),AppName.c_str(),MB_OK|MB_ICONERROR);
				if(Result.Hash().find(L"printurl")!=Result.Hash().end()) OpenUrl(Result(L"printurl"));
			}
		}else if(Control==Controls[7]) AuthWindow.Message(WM_CLOSE,0,0);
	}
	return;
}

void Myazo::Encrypt(std::vector<wchar_t>& Data)
{
	const unsigned Size=Data.size();
	for(unsigned i=0;i<Size;i++)
		if(!(i%2)) Data[i]=~(Data[i]+Size-i);
		else Data[i]=~(Data[i]+i);
	return;
}

void Myazo::Decrypt(std::vector<wchar_t>& Data)
{
	const unsigned Size=Data.size();
	for(unsigned i=0;i<Size;i++)
		if(!(i%2)) Data[i]=~Data[i]-Size+i;
		else Data[i]=~Data[i]-i;
	return;
}

std::wstring Myazo::ReadSettingFile(void)
{
	const wchar_t* EmptySettingString=L"{\"userid\":\"\",\"password\":\"\"}";
	const auto& Path=SettingDirectory+(*--SettingDirectory.end()==L'\\'?L"":L"\\")+SettingFileName;
	if(PathIsDirectory(SettingDirectory.c_str())&&PathFileExists(Path.c_str())){
		std::ifstream SettingFile(Path,std::ios::in|std::ios::binary);
		SettingFile.seekg(0,std::ios::end);
		int Size=SettingFile.tellg();
		SettingFile.clear();
		Size=(Size-SettingFile.seekg(0,std::ios::beg).tellg())/2;
		if(Size<2) return EmptySettingString;
		std::vector<wchar_t> Data(Size,0);
		SettingFile.read((char*)Data.data(),sizeof(wchar_t)*Data.size());
		SettingFile.close();
		Decrypt(Data);
		return std::wstring(Data.data(),Data.size());
	}else{
		CreateDirectory(SettingDirectory.c_str(),nullptr);
		return EmptySettingString;
	}
}

void Myazo::WriteSettingFile(const std::wstring& FileContent)
{
	std::ofstream SettingFile(SettingDirectory+(*--SettingDirectory.end()==L'\\'?L"":L"\\")+SettingFileName,
		std::ios::out|std::ios::binary|std::ios::trunc);
	std::vector<wchar_t> Data(FileContent.length(),0);
	std::memcpy(Data.data(),FileContent.c_str(),sizeof(wchar_t)*FileContent.length());
	Encrypt(Data);
	SettingFile.write((char*)Data.data(),sizeof(wchar_t)*FileContent.length());
	return;
}

Json::Item Myazo::Authenticate(const std::wstring& UserID,const std::wstring& PassWord)
{
	std::ostringstream Message;
	std::string Boundary=ToMultiByte(this->Boundary),NewLine="\r\n",Template="--"+Boundary+NewLine+"Content-Disposition: form-data; name=";
	Message<<Template<<"\"id\""<<NewLine<<NewLine<<ToMultiByte(UserID)<<NewLine;
	Message<<Template<<"\"pass\""<<NewLine<<NewLine<<ToMultiByte(PassWord)<<NewLine;
	Message<<"--"<<Boundary<<"--"<<NewLine;
	return Upload(Message.str(),DefaultHeader,UploadDomain,L"auth.php");
}

void Myazo::SetClipboardText(const std::wstring& Text)
{
	int Size=(Text.length()+1)*sizeof(wchar_t);
	HGLOBAL GlobalHandle=GlobalAlloc(GMEM_DDESHARE|GMEM_MOVEABLE,Size);
	auto Pointer=GlobalLock(GlobalHandle);
	std::memcpy(Pointer,Text.c_str(),Size);
	GlobalUnlock(GlobalHandle);
	OpenClipboard(nullptr);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT,GlobalHandle);
	CloseClipboard();
	GlobalFree(GlobalHandle);
	return;
}

void Myazo::MoveLayerWindow(void)
{
	LayerWindow.Move(CaptureRect.left<CaptureRect.right?CaptureRect.left:CaptureRect.right,
		CaptureRect.top<CaptureRect.bottom?CaptureRect.top:CaptureRect.bottom,
		std::abs(CaptureRect.right-CaptureRect.left+1),std::abs(CaptureRect.bottom-CaptureRect.top+1),true);
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
	std::vector<wchar_t> WindowSize(64,0);
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
	MainWindow.Show(SW_HIDE);
	LayerWindow.Show(SW_HIDE);
	auto Result=Upload(CreateMessage(GetScreenShot()),DefaultHeader,UploadDomain,UploadScriptPath);
	if(!Result(L"error")&&IsValidImageUrl(Result(L"imgurl"))){
		OpenUrl(Result(L"printurl"));
		SetClipboardText(Result(L"imgurl"));
	}else{
		MessageBox(nullptr,Result(L"errormessage").String().c_str(),AppName.c_str(),MB_OK|MB_ICONERROR);
		if(Result.Hash().find(L"printurl")!=Result.Hash().end()) OpenUrl(Result(L"printurl"));
	}
	return;
}

std::shared_ptr<IStream> Myazo::GetScreenShot(void)
{
	HDC DesktopDC=GetDC(nullptr),CaptureDC=CreateCompatibleDC(DesktopDC);
	unsigned int X=CaptureRect.left<CaptureRect.right?CaptureRect.left:CaptureRect.right,
		Y=CaptureRect.top<CaptureRect.bottom?CaptureRect.top:CaptureRect.bottom,
		Width=std::abs(CaptureRect.right-CaptureRect.left+1),
		Height=std::abs(CaptureRect.bottom-CaptureRect.top+1);
	HBITMAP CaptureBitmapHandle=CreateCompatibleBitmap(DesktopDC,Width,Height),
		PrevBitmapHandle=(HBITMAP)SelectObject(CaptureDC,CaptureBitmapHandle);
	BitBlt(CaptureDC,0,0,Width,Height,DesktopDC,X,Y,SRCCOPY);
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
	unsigned long WrittenSize;
	STATSTG State;
	ImageFileStream->Stat(&State,STATFLAG_NONAME);
	std::vector<char> ImageFileData(State.cbSize.LowPart,0);
	LARGE_INTEGER Position;
	Position.QuadPart=0;
	ImageFileStream->Seek(Position,STREAM_SEEK_SET,nullptr);
	ImageFileStream->Read(ImageFileData.data(),State.cbSize.LowPart,&WrittenSize);
	std::ostringstream Message;
	std::string Boundary=ToMultiByte(this->Boundary),NewLine="\r\n",Template="--"+Boundary+NewLine+"Content-Disposition: form-data; name=";
	Message<<Template<<"\"imagedata\"; filename=\""<<ToMultiByte(UploadDomain)<<"\""<<NewLine<<"Content-Type: image/png"<<NewLine<<NewLine;
	Message.write(ImageFileData.data(),ImageFileData.size())<<NewLine;
	if(!UserID.empty()&&!PassWord.empty()){
		Message<<Template<<"\"id\""<<NewLine<<NewLine<<ToMultiByte(UserID)<<NewLine;
		Message<<Template<<"\"pass\""<<NewLine<<NewLine<<ToMultiByte(PassWord)<<NewLine;
		Message<<Template<<"\"private\""<<NewLine<<NewLine<<(UploadAsPrivate?"true":"false")<<NewLine;
	}
	Message<<"--"<<Boundary<<"--"<<NewLine;
	return Message.str();
}

Json::Item Myazo::Upload(const std::string& Message,const std::wstring& Header,const std::wstring& Domain,const std::wstring& ScriptPath)
{
	auto DestroyFunction=[](void* Obj){if(Obj) InternetCloseHandle(Obj);};
	std::shared_ptr<void> SessionHandle,ConnectionHandle,RequestHandle;
	SessionHandle.reset(InternetOpen(AppName.c_str(),INTERNET_OPEN_TYPE_PRECONFIG,nullptr,nullptr,0),DestroyFunction);
	ConnectionHandle.reset(InternetConnect(SessionHandle.get(),Domain.c_str(),80,nullptr,nullptr,INTERNET_SERVICE_HTTP,0,0),DestroyFunction);
	RequestHandle.reset(HttpOpenRequest(ConnectionHandle.get(),L"POST",ScriptPath.c_str(),nullptr,nullptr,nullptr,INTERNET_FLAG_NO_CACHE_WRITE,0),DestroyFunction);
	auto UserAgentHeader=L"User-Agent: "+UserAgent+L"\r\n";
	HttpAddRequestHeaders(RequestHandle.get(),UserAgentHeader.c_str(),UserAgentHeader.length(),HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE);
	DWORD WrittenSize;
	INTERNET_BUFFERS Buffer;
	ZeroMemory(&Buffer,sizeof(Buffer));
	Buffer.dwStructSize=sizeof(INTERNET_BUFFERS);
	Buffer.dwBufferTotal=Message.length();
	Buffer.lpcszHeader=Header.c_str();
	Buffer.dwHeadersLength=Header.length();
	if(HttpSendRequestEx(RequestHandle.get(),&Buffer,nullptr,0,0)&&InternetWriteFile(RequestHandle.get(),Message.c_str(),Message.length(),&WrittenSize)){
		HttpEndRequest(RequestHandle.get(),nullptr,0,0);
		std::vector<char> Temp(1024,0);
		InternetReadFile(RequestHandle.get(),Temp.data(),Temp.size(),&WrittenSize);
		return JsonParser.Parse(ToUnicode(Temp));
	}
	return Json::Item(Json::Type::Null);
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

bool Myazo::IsValidImageUrl(const std::wstring& Url)
{
	return std::regex_match(Url,std::wregex(L"http://myazo.net/[0-9a-fA-F]{32}\\.png"));
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
	auto Result=Upload(CreateMessage(ImageFileStream),DefaultHeader,UploadDomain,UploadScriptPath);
	if(Result(L"error")&&IsValidImageUrl(Result(L"imgurl"))){
		OpenUrl(Result(L"printurl"));
		SetClipboardText(Result(L"imgurl"));
		return 0;
	}else{
		MessageBox(nullptr,Result(L"errormessage").String().c_str(),AppName.c_str(),MB_OK|MB_ICONERROR);
		if(Result.Hash().find(L"printurl")!=Result.Hash().end()) OpenUrl(Result(L"printurl"));
		return 1;
	}
}

std::wstring Myazo::ToUnicode(const std::string& MultiByteString)
{
	std::vector<wchar_t> TempBuffer(MultiByteString.length()+1,0);
	std::mbstowcs(TempBuffer.data(),MultiByteString.c_str(),MultiByteString.length());
	return std::wstring(TempBuffer.data());
}

std::wstring Myazo::ToUnicode(const std::vector<char>& MultiByteBuffer)
{
	std::vector<wchar_t> TempBuffer(MultiByteBuffer.size()+1,0);
	std::mbstowcs(TempBuffer.data(),MultiByteBuffer.data(),MultiByteBuffer.size());
	return std::wstring(TempBuffer.data());
}

std::string Myazo::ToMultiByte(const std::wstring& UnicodeString)
{
	std::vector<char> TempBuffer(UnicodeString.length()+1,0);
	std::wcstombs(TempBuffer.data(),UnicodeString.c_str(),UnicodeString.length());
	return std::string(TempBuffer.data());
}

std::string Myazo::ToMultiByte(const std::vector<wchar_t>& UnicodeBuffer)
{
	std::vector<char> TempBuffer(UnicodeBuffer.size()+1,0);
	std::wcstombs(TempBuffer.data(),UnicodeBuffer.data(),UnicodeBuffer.size());
	return std::string(TempBuffer.data());
}