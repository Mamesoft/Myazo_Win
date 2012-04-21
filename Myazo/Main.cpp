#include "ShareHeaders.h"
#include "Myazo.h"

int __stdcall wWinMain(HINSTANCE,HINSTANCE,LPWSTR,int)
{
	Myazo& Program=Myazo::GetInstance();
	if(Program.Init()) Program.EnterMessageLoop();
	return 0;
}