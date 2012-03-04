#include "ShareHeaders.h"
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
		Program.GetScreenShot();
		break;
	case WM_RBUTTONDOWN:
		Program.MainWindow.Destroy();
		return DefWindowProc(WindowHandle,Message,WParam,LParam);
	case WM_MOUSEMOVE:
		if(Program.CaptureStarted) Program.UpdatePosition(LOWORD(LParam),HIWORD(LParam));
		break;
	case WM_TIMER:
		if(WParam==100) Program.ProcessKeyMessage();
		break;
	case WM_DESTROY:
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
	case WM_ERASEBKGND:
		return Program.DrawLayerWindowContent();
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
		MessageBox(LayerWindow.GetWindowHandle(),
			UploadAsPrivate?L"非公開モードでアップロードされます。":L"公開モードでアップロードされます。",L"Myazo",MB_OK);
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
		std::abs(CaptureRect.right-CaptureRect.left),std::abs(CaptureRect.bottom-CaptureRect.top),true);
	return;
}

bool Myazo::DrawLayerWindowContent(void)
{
	std::shared_ptr<Gdiplus::StringFormat> Layout(new Gdiplus::StringFormat());
	std::shared_ptr<Gdiplus::SolidBrush> BlackBrush(new Gdiplus::SolidBrush(Gdiplus::Color::Black)),WhiteBrush(new Gdiplus::SolidBrush(Gdiplus::Color::White));
	Layout->SetAlignment(Gdiplus::StringAlignment::StringAlignmentFar);
	Layout->SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentFar);
	auto LayoutRect=Gdiplus::RectF(
		CaptureRect.left<CaptureRect.right?CaptureRect.left:CaptureRect.right,
		CaptureRect.top<CaptureRect.bottom?CaptureRect.top:CaptureRect.bottom,
		std::abs(CaptureRect.right-CaptureRect.left)-10,
		std::abs(CaptureRect.bottom-CaptureRect.top)-10);
	LayerWindowGraphics->Clear(Gdiplus::Color(100,100,100));
	LayerWindowGraphics->DrawString(L"",0,&*LayerWindowFont,LayoutRect,&*Layout,&*BlackBrush);
	LayoutRect.Width--;
	LayoutRect.Height--;
	LayerWindowGraphics->DrawString(L"",0,&*LayerWindowFont,LayoutRect,&*Layout,&*WhiteBrush);
	return true;
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

void Myazo::GetScreenShot(void)
{
	HDC DesktopDC=GetDC(nullptr),CaptureDC=CreateCompatibleDC(DesktopDC);
	unsigned int X=CaptureRect.left<CaptureRect.right?CaptureRect.left:CaptureRect.right,
		Y=CaptureRect.top<CaptureRect.bottom?CaptureRect.top:CaptureRect.bottom,
		Width=std::abs(CaptureRect.right-CaptureRect.left),
		Height=std::abs(CaptureRect.bottom-CaptureRect.top);
	HBITMAP CaptureBitmapHandle=CreateCompatibleBitmap(DesktopDC,Width,Height),PrevBitmapHandle=(HBITMAP)SelectObject(CaptureDC,CaptureBitmapHandle);
	BitBlt(CaptureDC,0,0,Width,Height,DesktopDC,X,Y,SRCCOPY);
	MainWindow.Show(SW_HIDE);
	std::vector<wchar_t> TempFileName;
	TempFileName.reserve(MAX_PATH*2);
	GetTempPath(MAX_PATH,TempFileName.data());
	GetTempFileName(TempFileName.data(),TempFileNamePrefix.c_str(),0,TempFileName.data());
	ImageEncoder.Save(CaptureBitmapHandle,TempFileName.data());
	SelectObject(CaptureDC,PrevBitmapHandle);
	DeleteObject(CaptureBitmapHandle);
	DeleteDC(CaptureDC);
	ReleaseDC(nullptr,DesktopDC);
	return;
}

bool Myazo::Upload(void)
{
	std::wstring UploadDomein=L"myazo.net",
		UploadScriptPath=L"upload.php",
		Boundary=L"----BOUNDARYBOUNDARY----",
		NewLine=L"\x000D\x000A",
		Header=L"Content-Type: multipart/form-data; boundary="+Boundary;

	return true;
}