#pragma once
#include "stdafx.h"
#include "merror.h"
#include "arcsoft_fsdk_face_tracking.h"

#define WORKBUF_SIZE_FT (40*1024*1024)
class FTEngine
{
private:
	MInt32 nScale;
	MInt32 nMaxFace;
	MByte *pWorkMem;
	MHandle hEngine;
public:
	FTEngine();
	~FTEngine();
	int init();
	int uninit();
	int FaceTracking(LPASVLOFFSCREEN pImgData, LPAFT_FSDK_FACERES *pFaceRes);
};