#include "ShareHeaders.h"

class PNGFile
{
private:
	CLSID PNGEncoderClassID;
	ULONG_PTR GdiplusToken;
	Gdiplus::GdiplusStartupInput GdiplusInput;

public:
	~PNGFile(void);
	bool Init(void);
	CLSID GetPNGEncoderClassID(void);
	bool IsWellHeader(std::wstring FileName);
	bool IsWellHeader(IStream* Stream);
	bool Save(Gdiplus::Bitmap* InputBitmap,std::wstring FileName);
	bool Save(Gdiplus::Bitmap* InputBitmap,IStream* Stream);
	bool Save(HBITMAP BitmapHandle,std::wstring FileName,HPALETTE Palette=(HPALETTE)GetStockObject(DEFAULT_PALETTE));
	bool Save(HBITMAP BitmapHandle,IStream* Stream,HPALETTE Palette=(HPALETTE)GetStockObject(DEFAULT_PALETTE));
	bool ConvertToPNG(std::wstring InputFileName,std::wstring OutputFileName);

};