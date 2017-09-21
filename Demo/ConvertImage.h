#pragma once
#include <Windows.h>
#include <string>
#include <tchar.h>
#include <gdiplus.h>
#include <stdint.h>
using namespace std;
using namespace Gdiplus;

class CConvertImage
{
public:
	CConvertImage();
	~CConvertImage();

	static bool ConvertImageToBmp(string sInputImg, string & pOutBmpImg);
	static bool readBmp(string sInputImg, uint8_t ** imageData, int * pWidth, int * pHeight);
	static bool readBmpFromBuffer(const string & sOutBmp, uint8_t ** imageData, int * pWidth, int * pHeight);

};

class CGDIPlusInit {
public:
	CGDIPlusInit();
	~CGDIPlusInit();

	static CGDIPlusInit _init;
private:
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
};