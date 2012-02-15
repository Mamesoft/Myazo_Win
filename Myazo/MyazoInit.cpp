#include "Myazo.h"

Myazo::Myazo(const Myazo&)
{
	return;
}

Myazo::Myazo(void)
{
	Instance=GetModuleHandle(nullptr);
	UploadAsPrivate=false;
	UtilityMode=false;
	OffsetX=OffsetY=0;
	return;
}

Myazo::~Myazo(void)
{
	return;
}

bool Myazo::Init(void)
{
	return true;
}

bool Myazo::InitWindow(void)
{

	return true;
}

void Myazo::EnterMessageLoop(void)
{
	MSG Msg;
	ZeroMemory(&Msg,sizeof(Msg));
	while(Msg.message!=WM_QUIT){
		if(PeekMessage(&Msg,0,0,0,PM_REMOVE)){
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
	return;
}

LRESULT __stdcall WndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam)
{
	switch(Message){
	default:
		return DefWindowProc(WindowHandle,Message,WParam,LParam);
	}
	return 0;
}

LRESULT __stdcall LayerWndProc(HWND WindowHandle,unsigned int Message,WPARAM WParam,LPARAM LParam)
{
	switch(Message){
	default:
		return DefWindowProc(WindowHandle,Message,WParam,LParam);
	}
	return 0;
}
