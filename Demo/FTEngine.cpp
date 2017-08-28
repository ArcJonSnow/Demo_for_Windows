#include "stdafx.h"
#include "FTEngine.h"

FTEngine::FTEngine()
{
	nScale = 16;
	nMaxFace = 12;
	pWorkMem = nullptr;
	hEngine = nullptr;
}
FTEngine::~FTEngine()
{
}
int FTEngine::init()
{
	pWorkMem = (MByte *)malloc(WORKBUF_SIZE_FT);
	if (pWorkMem == nullptr)
	{
		return -1;
	}
	int ret = 0;
	ret = AFT_FSDK_InitialFaceEngine(APPID, FT_SDKKEY, pWorkMem, WORKBUF_SIZE_FT, &hEngine, AFT_FSDK_OPF_0_HIGHER_EXT, nScale, nMaxFace);
	return ret;
}
int FTEngine::uninit()
{
	int ret = 0;
	ret = AFT_FSDK_UninitialFaceEngine(hEngine);
	if (pWorkMem != nullptr)
	{
		free(pWorkMem);
	}
	return ret;
}
int FTEngine::FaceTracking(LPASVLOFFSCREEN pImgData, LPAFT_FSDK_FACERES *pFaceRes)
{
	int ret;
	ret = AFT_FSDK_FaceFeatureDetect(hEngine, pImgData, pFaceRes);
	return ret;
}