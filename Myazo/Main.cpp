#include "ShareHeaders.h"
#include "Myazo.h"

int __stdcall wWinMain(HINSTANCE,HINSTANCE,wchar_t*,int)
{
	Myazo& Program=Myazo::GetInstance();
	if(Program.Init()) Program.EnterMessageLoop();
	else MessageBox(nullptr,L"�A�v���P�[�V�����̏������Ɏ��s���܂����B",L"Myazo",MB_OK);
	return 0;
}