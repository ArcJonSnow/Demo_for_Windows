#include "stdafx.h"
#include "AgeEngine.h"
AgeEngine::AgeEngine()
{
	pWorkMem = nullptr;
	hEngine = nullptr;
}
AgeEngine::~AgeEngine()
{
}
int AgeEngine::init()
{
	pWorkMem = (MByte *)malloc(WORKBUF_SIZE_AGE);
	if (pWorkMem == nullptr)
	{
		return -1;
	}
	int ret = ASAE_FSDK_InitAgeEngine(APPID, AGE_SDKKEY, pWorkMem, WORKBUF_SIZE_AGE, &hEngine);
	return ret;
}
int AgeEngine::uninit()
{
	int ret = ASAE_FSDK_UninitAgeEngine(hEngine);
	if (pWorkMem != nullptr)
	{
		free(pWorkMem);
	}
	return ret;
}
int AgeEngine::AgeEstimationStaticImage(LPASVLOFFSCREEN pImgInfo, LPASAE_FSDK_AGEFACEINPUT pFaceRes, LPASAE_FSDK_AGERESULT pAgeRes)
{
	int ret = ASAE_FSDK_AgeEstimation_StaticImage(hEngine, pImgInfo, pFaceRes, pAgeRes);
	return ret;
}
int AgeEngine::AgeEstimationPreview(LPASVLOFFSCREEN pImgInfo, LPASAE_FSDK_AGEFACEINPUT pFaceRes, LPASAE_FSDK_AGERESULT pAgeRes)
{
	int ret = ASAE_FSDK_AgeEstimation_Preview(hEngine, pImgInfo, pFaceRes, pAgeRes);
	return ret;
}