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
	auto Codecs=std::vector<Gdiplus::ImageCodecInfo>(EncoderCount);
	Gdiplus::GetImageEncoders(EncoderCount,Size,Codecs.data());
	auto Codec=std::find_if(Codecs.begin(),Codecs.end(),
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
	std::wifstream File(FileName,std::ios_base::in|std::ios_base::binary);
	File.read((wchar_t*)Header.data(),sizeof(Header[0])*Header.size());
	File.close();
	return Header==WellHeader?true:false;
}

bool PNGFile::Save(Gdiplus::Bitmap& InputImage,std::wstring FileName)
{
	if(InputImage.Save(FileName.c_str(),&PNGEncoderClassID)!=Gdiplus::Status::Ok) return false;
	return true;
}

bool PNGFile::ConvertToPNG(std::wstring InputFileName,std::wstring OutputFileName)
{
	Gdiplus::Bitmap InputImage(InputFileName.c_str());
	if(InputImage.Save(OutputFileName.c_str(),&PNGEncoderClassID)!=Gdiplus::Status::Ok) return false;
	return true;
}