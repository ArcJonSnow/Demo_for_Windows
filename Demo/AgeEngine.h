#pragma once
#include "stdafx.h"
#include "merror.h"
#include "arcsoft_fsdk_age_estimation.h"

#define WORKBUF_SIZE_AGE (20*1024*1024)
class AgeEngine
{
private:
	MByte *pWorkMem;
	MHandle hEngine;
public:
	AgeEngine();
	~AgeEngine();
	int init();
	int uninit();
	int AgeEstimationStaticImage(LPASVLOFFSCREEN pImgInfo, LPASAE_FSDK_AGEFACEINPUT pFaceRes, LPASAE_FSDK_AGERESULT pAgeRes);
	int AgeEstimationPreview(LPASVLOFFSCREEN pImgInfo, LPASAE_FSDK_AGEFACEINPUT pFaceRes, LPASAE_FSDK_AGERESULT pAgeRes);
};