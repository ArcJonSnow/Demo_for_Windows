#include "stdafx.h"
#include "ConvertImage.h"

#pragma comment(lib,"gdiplus")

CConvertImage::CConvertImage()
{
}


CConvertImage::~CConvertImage()
{
}

static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders  
	UINT  size = 0;         // size of the image encoder array in bytes  

	ImageCodecInfo* pImageCodecInfo = NULL;

	//2.获取GDI+支持的图像格式编码器种类数以及ImageCodecInfo数组的存放大小  
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure  

					//4.获取所有的图像编码器信息  
	GetImageEncoders(num, size, pImageCodecInfo);

	//5.查找符合的图像编码器的Clsid
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success  
		}
	}

	//6.释放步骤3分配的内存  
	free(pImageCodecInfo);
	return -1;  // Failure  
}

static string UnicodeToANSI(const wstring& str)
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

static string UnicodeToUTF8(const wstring& str)
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

static string UTF8ToGBK(const string& str)
{
	wchar_t*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), NULL, NULL);
	pElementText = new wchar_t[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(wchar_t) * (iTextLen + 1));
	::MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),
		str.size(),
		pElementText,
		iTextLen);
	wstring strText;
	strText = pElementText;
	delete[] pElementText;
	return UnicodeToANSI(strText);
}

static wstring GBKToUNICODE(const string& str)
{
	wstring strTo;
	int    iTextLen;
	// wide char to multi char
	iTextLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, NULL);
	if (iTextLen > 0)
	{
		strTo.resize(iTextLen);
		::MultiByteToWideChar(CP_ACP,
			0,
			str.c_str(),
			str.size(),
			(wchar_t*)strTo.c_str(),
			strTo.size());
	}
	return strTo;
}


static string GBKToUTF8(const string& str)
{
	return UnicodeToUTF8(GBKToUNICODE(str));
}

bool CConvertImage::ConvertImageToBmp(string sInputImg, string& pOutBmpImg)
{
	CLSID   encoderClsid;
	wstring wsInputImg = GBKToUNICODE(sInputImg);
	//7.创建Image对象并加载图片
	Image* image = new Image(wsInputImg.c_str());
	if (image == NULL || image->GetLastStatus() != 0)
	{
		return false;
	}
	// Get the CLSID of the PNG encoder.  
	if (GetEncoderClsid(L"image/bmp", &encoderClsid) == -1)
	{
		delete image;
		return false;
	}

	HGLOBAL  hImage = ::GlobalAlloc(GMEM_MOVEABLE, 0);
	if (hImage == NULL)
	{
		delete image;
		return false;
	}

	IStream* pStream = NULL;
	if (::CreateStreamOnHGlobal(hImage, FALSE, &pStream) != S_OK)
	{
		::GlobalFree(hImage);
		delete image;
		return false;
	}

	image->Save(pStream, &encoderClsid);

	int size = ::GlobalSize(hImage);

	pOutBmpImg.resize(size);

	char *srcBuffer = (char *)::GlobalLock(hImage);
	if (srcBuffer)
	{
		memcpy((void*)pOutBmpImg.c_str(), srcBuffer, size);
	}
	::GlobalUnlock(hImage);
	::GlobalFree(hImage);

	pStream->Release();
	delete image;
	return true;
}

bool CConvertImage::readBmp(string sInputImg, uint8_t ** imageData, int * pWidth, int * pHeight)
{
	string sOutBmp;
	if (ConvertImageToBmp(sInputImg, sOutBmp))
	{
		return readBmpFromBuffer(sOutBmp, imageData, pWidth, pHeight);
	}
	return false;
}


bool CConvertImage::readBmpFromBuffer(const string& sOutBmp, uint8_t **imageData, int *pWidth, int *pHeight)
{
	if (sOutBmp.empty() || imageData == NULL || pWidth == NULL || pHeight == NULL)
	{
		return false;
	}
	const char* pBuffer = sOutBmp.c_str();
	BITMAPINFOHEADER head;
	memcpy(&head, pBuffer + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
	*pWidth = head.biWidth;
	*pHeight = head.biHeight;
	int biBitCount = head.biBitCount;
	int lineByte = ((*pWidth) * biBitCount / 8 + 3) / 4 * 4;
	*imageData = (uint8_t *)malloc(lineByte * (*pHeight));
	for (int i = 0; i < *pHeight; i++)
	{
		memcpy(*imageData + i * (*pWidth) * 3, pBuffer + (*pHeight - 1 - i) * lineByte + 54, (*pWidth) * 3);
	}
	return true;
}

CGDIPlusInit CGDIPlusInit::_init;

CGDIPlusInit::CGDIPlusInit()
{
	gdiplusToken = 0;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

CGDIPlusInit::~CGDIPlusInit()
{
	if (gdiplusToken != NULL)
	{
		GdiplusShutdown(gdiplusToken);
	}
}
