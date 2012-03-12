#include "PNGFile.h"

PNGFile::~PNGFile(void)
{
	Gdiplus::GdiplusShutdown(GdiplusToken);
	return;
}

bool PNGFile::Init(void)
{
	Gdiplus::GdiplusStartup(&GdiplusToken,&GdiplusInput,nullptr);
	std::wstring TargetMimeType=L"image/png";
	unsigned Size,EncoderCount;
	Gdiplus::GetImageEncodersSize(&EncoderCount,&Size);
	auto Codecs=std::vector<unsigned char>(Size,0);
	Gdiplus::GetImageEncoders(EncoderCount,Size,(Gdiplus::ImageCodecInfo*)Codecs.data());
	auto Codec=std::find_if((Gdiplus::ImageCodecInfo*)Codecs.data(),(Gdiplus::ImageCodecInfo*)Codecs.data()+EncoderCount,
		[&](Gdiplus::ImageCodecInfo& Obj){return !TargetMimeType.compare(Obj.MimeType)?true:false;});
	PNGEncoderClassID=(*Codec).Clsid;
	return true;
}

CLSID PNGFile::GetPNGEncoderClassID(void)
{
	return PNGEncoderClassID;
}

bool PNGFile::IsWellHeader(std::wstring FileName)
{
	std::array<unsigned char,8> WellHeader={0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A},Header;
	std::ifstream File(FileName,std::ios::in|std::ios::binary);
	File.read((char*)Header.data(),Header.size());
	File.close();
	return Header==WellHeader;
}

bool PNGFile::IsWellHeader(IStream* Stream)
{
	std::array<unsigned char,8> WellHeader={0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A},Header;
	unsigned long WrittenSize;
	LARGE_INTEGER Position;
	Position.QuadPart=0;
	Stream->Seek(Position,STREAM_SEEK_SET,nullptr);
	Stream->Read(Header.data(),8,&WrittenSize);
	return Header==WellHeader;
}

bool PNGFile::Save(Gdiplus::Bitmap* InputBitmap,std::wstring FileName)
{
	return InputBitmap->Save(FileName.c_str(),&PNGEncoderClassID)==Gdiplus::Status::Ok?true:false;
}

bool PNGFile::Save(Gdiplus::Bitmap* InputBitmap,IStream* Stream)
{
	return InputBitmap->Save(Stream,&PNGEncoderClassID)==Gdiplus::Status::Ok?true:false;
}

bool PNGFile::Save(HBITMAP BitmapHandle,std::wstring FileName,HPALETTE Palette)
{
	Gdiplus::Bitmap InputBitmap(BitmapHandle,Palette);
	return InputBitmap.Save(FileName.c_str(),&PNGEncoderClassID)==Gdiplus::Status::Ok?true:false;
}

bool PNGFile::Save(HBITMAP BitmapHandle,IStream* Stream,HPALETTE Palette)
{
	Gdiplus::Bitmap InputBitmap(BitmapHandle,Palette);
	return InputBitmap.Save(Stream,&PNGEncoderClassID)==Gdiplus::Status::Ok?true:false;
}

bool PNGFile::ConvertToPNG(std::wstring InputFileName,std::wstring OutputFileName)
{
	Gdiplus::Bitmap InputBitmap(InputFileName.c_str());
	return InputBitmap.Save(OutputFileName.c_str(),&PNGEncoderClassID)==Gdiplus::Status::Ok?true:false;
}