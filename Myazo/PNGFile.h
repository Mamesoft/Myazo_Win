#include "ShareHeaders.h"

class PNGFile
{
private:
	CLSID PNGEncoderClassID;

public:
	PNGFile(void);
	~PNGFile(void);
	CLSID GetPNGEncoderClassID(void);
	bool IsWellHeader(std::wstring FileName);
	bool Save(Gdiplus::Bitmap& InputImage,std::wstring FileName);
	bool ConvertToPNG(std::wstring InputFileName,std::wstring OutputFileName);

};