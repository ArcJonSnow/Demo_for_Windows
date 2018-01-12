#include "stdafx.h"
#include "GenderEngine.h"
GenderEngine::GenderEngine()
{
	pWorkMem = nullptr;
	hEngine = nullptr;
}
GenderEngine::~GenderEngine()
{
}
int GenderEngine::init()
{
	pWorkMem = (MByte *)malloc(WORKBUF_SIZE_GENDER);
	if (pWorkMem == nullptr)
	{
		return -1;
	}
	int ret = ASGE_FSDK_InitGenderEngine(APPID, GENDER_SDKKEY, pWorkMem, WORKBUF_SIZE_GENDER, &hEngine);
	return ret;
}
int GenderEngine::uninit()
{
	int ret = ASGE_FSDK_UninitGenderEngine(hEngine);
	if (pWorkMem != nullptr)
	{
		free(pWorkMem);
	}
	return ret;
}
int GenderEngine::GenderEstimationStaticImage(LPASVLOFFSCREEN pImgInfo, LPASGE_FSDK_GENDERFACEINPUT pFaceRes, LPASGE_FSDK_GENDERRESULT pGenderRes)
{
	int ret = ASGE_FSDK_GenderEstimation_StaticImage(hEngine, pImgInfo, pFaceRes, pGenderRes);
	return ret;
}
int GenderEngine::GenderEstimationPreview(LPASVLOFFSCREEN pImgInfo, LPASGE_FSDK_GENDERFACEINPUT pFaceRes, LPASGE_FSDK_GENDERRESULT pGenderRes)
{
	int ret = ASGE_FSDK_GenderEstimation_Preview(hEngine, pImgInfo, pFaceRes, pGenderRes);
	return ret;
}