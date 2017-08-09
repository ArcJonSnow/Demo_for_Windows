#pragma once
#include "stdafx.h"
#include "merror.h"
#include "arcsoft_fsdk_face_detection.h"

#define WORKBUF_SIZE_FD (40*1024*1024)
class FDEngine
{
private:
	MInt32 nScale;
	MInt32 nMaxFace;
	MByte *pWorkMem;
	MHandle hEngine;
public:
	FDEngine();
	~FDEngine();
	int init();
	int uninit();
	int FaceDetection(LPASVLOFFSCREEN pImgData, LPAFD_FSDK_FACERES *pFaceRes);
};