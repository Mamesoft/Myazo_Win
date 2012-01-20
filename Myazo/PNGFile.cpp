#include "PNGFile.h"
#include "PNGFile.h"

PNGFile::PNGFile(void)
{
	std::wstring TargetMimeType=L"image/png";
	unsigned Size,EncoderCount;
	Gdiplus::GetImageEncodersSize(&EncoderCount,&Size);
	auto Codecs=(Gdiplus::ImageCodecInfo*)new char[Size];
	Gdiplus::GetImageEncoders(EncoderCount,Size,Codecs);
	for(int i=0;i<EncoderCount;i++){
		if(!TargetMimeType.compare(Codecs[i].MimeType)){
			PNGEncoderClassID=Codecs[i].Clsid;
			break;
		}
	}
	delete[] Codecs;
	return;
}

PNGFile::~PNGFile(void)
{
	return;
}

CLSID PNGFile::GetPNGEncoderClassID(void)
{
	return PNGEncoderClassID;
}

bool PNGFile::IsWellHeader(std::wstring FileName)
{
	unsigned long long PNGFileHeader=0x89504E470D0A1A0A,TargetFileHeader;
	std::ifstream File(FileName.c_str(),std::ios::binary);
	File>>TargetFileHeader;
	File.close();
	if(PNGFileHeader!=TargetFileHeader) return false;
	return true;
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