#include "ShareHeaders.h"
#include "Myazo.h"

int __stdcall wWinMain(HINSTANCE,HINSTANCE,LPWSTR,int)
{
	Myazo& Program=Myazo::GetInstance();
	if(Program.Init()) Program.EnterMessageLoop();
	else MessageBox(nullptr,L"�A�v���P�[�V�����̏������Ɏ��s���܂����B",L"Myazo",MB_OK|MB_ICONERROR);
	return 0;
}