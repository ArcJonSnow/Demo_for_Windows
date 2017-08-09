#include "stdafx.h"
#include "FDEngine.h"

FDEngine::FDEngine()
{
	nScale = 16;
	nMaxFace = 12;
	pWorkMem = nullptr;
	hEngine = nullptr;
}
FDEngine::~FDEngine()
{
}
int FDEngine::init()
{
	pWorkMem = (MByte *)malloc(WORKBUF_SIZE_FD);
	if (pWorkMem == nullptr)
	{
		return -1;
	}
	int ret = AFD_FSDK_InitialFaceEngine(APPID, FD_SDKKEY, pWorkMem, WORKBUF_SIZE_FD, &hEngine, AFD_FSDK_OPF_0_HIGHER_EXT, nScale, nMaxFace);
	return ret;
}
int FDEngine::uninit()
{
	int ret = AFD_FSDK_UninitialFaceEngine(hEngine);
	free(pWorkMem);
	return ret;
}
int FDEngine::FaceDetection(LPASVLOFFSCREEN pImgData, LPAFD_FSDK_FACERES *pFaceRes)
{
	int ret = AFD_FSDK_StillImageFaceDetection(hEngine, pImgData, pFaceRes);
	return ret;
}