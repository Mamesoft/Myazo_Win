#include "ShareHeaders.h"
#include "Myazo.h"

int __stdcall wWinMain(HINSTANCE,HINSTANCE,wchar_t*,int)
{
	Myazo Program;
	if(Program.Init()) Program.EnterMessageLoop();
	else MessageBox(nullptr,L"アプリケーションの初期化に失敗しました。",L"Myazo",MB_OK);
	return 0;
}
