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
	return;
}

