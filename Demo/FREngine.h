#pragma once
#include "stdafx.h"
#include "merror.h"
#include "arcsoft_fsdk_face_recognition.h"

#define WORKBUF_SIZE_FR (40*1024*1024)
class FREngine
{
private:
	MByte *pWorkMem;
	MHandle hEngine;
public:
	FREngine();
	~FREngine();
	int init();
	int uninit();
	int ExtractFRFeature(LPASVLOFFSCREEN pImgData, LPAFR_FSDK_FACEINPUT pFaceRes, LPAFR_FSDK_FACEMODEL pFaceModels);
	int FacePairMatching(AFR_FSDK_FACEMODEL *reffeature, AFR_FSDK_FACEMODEL *probefeature, MFloat *pfSimilScore);
};