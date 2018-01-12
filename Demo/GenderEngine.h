#pragma once
#include "stdafx.h"
#include "merror.h"
#include "arcsoft_fsdk_gender_estimation.h"

#define WORKBUF_SIZE_GENDER (20*1024*1024)
class GenderEngine
{
private:
	MByte *pWorkMem;
	MHandle hEngine;
public:
	GenderEngine();
	~GenderEngine();
	int init();
	int uninit();
	int GenderEstimationStaticImage(LPASVLOFFSCREEN pImgInfo, LPASGE_FSDK_GENDERFACEINPUT pFaceRes, LPASGE_FSDK_GENDERRESULT pAgeRes);
	int GenderEstimationPreview(LPASVLOFFSCREEN pImgInfo, LPASGE_FSDK_GENDERFACEINPUT pFaceRes, LPASGE_FSDK_GENDERRESULT pAgeRes);
};