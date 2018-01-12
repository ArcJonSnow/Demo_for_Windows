/*******************************************************************************
Copyright(c) ArcSoft, All right reserved.

This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary 
and confidential information. 

DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER 
AUTHORIZATION.

If you are not an intended recipient of this file, you must not copy, 
distribute, modify, or take any action in reliance on it. 

If you have received this file in error, please immediately notify ArcSoft and 
permanently delete the original and any copy of any file and any printout 
thereof.
*******************************************************************************/

#ifndef _ARCSOFT_FSDK_GENDERESTIMATION_H_
#define _ARCSOFT_FSDK_GENDERESTIMATION_H_

#include "amcomdef.h"
#include "asvloffscreen.h"

#ifdef __cplusplus
extern "C" {
#endif

// This enumeration defines the orientation of the face in anti-clockwise sequence. 
enum ASGE_FSDK_GenderOrientCode{
	ASGE_FSDK_FOC_Gender_0			= 0x1,		// 0 degree 
	ASGE_FSDK_FOC_Gender_90			= 0x2,		// 90 degree 
	ASGE_FSDK_FOC_Gender_270		= 0x3,		// 270 degree 
	ASGE_FSDK_FOC_Gender_180        = 0x4,      // 180 degree 
	ASGE_FSDK_FOC_Gender_30			= 0x5,		// 30 degree 
	ASGE_FSDK_FOC_Gender_60			= 0x6,		// 60 degree 
	ASGE_FSDK_FOC_Gender_120		= 0x7,		// 120 degree 
	ASGE_FSDK_FOC_Gender_150		= 0x8,		// 150 degree 
	ASGE_FSDK_FOC_Gender_210		= 0x9,		// 210 degree 
	ASGE_FSDK_FOC_Gender_240		= 0xa,		// 240 degree 
	ASGE_FSDK_FOC_Gender_300		= 0xb,		// 300 degree 
	ASGE_FSDK_FOC_Gender_330		= 0xc		// 330 degree 
};

typedef struct{
	MRECT 		 *pFaceRectArray;		// Bounding boxes of input faces. 
	MInt32		 *pFaceOrientArray;		// Orientations of input faces. Can be set as one item of "ASGE_FSDK_GenderOrientCode". 
	MInt32		 lFaceNumber;			// Number of input faces, and it must be greater than or euqual to 0. 
}ASGE_FSDK_GENDERFACEINPUT, *LPASGE_FSDK_GENDERFACEINPUT;

typedef struct{
	MInt32 			*pGenderResultArray; // The gender result array with length of "lFaceNumber". 
										 // The value will be equal to one of below: 0, 1 and -1. 
										 // "1" represents female, "0" represents male, and "-1" represents unknown.*/

	MInt32			lFaceNumber;		// It is the same as "lFaceNumber" in "ASGE_FSDK_GENDERFACEINPUT". 
}ASGE_FSDK_GENDERRESULT, *LPASGE_FSDK_GENDERRESULT;

typedef struct
{
	MInt32 lCodebase;	// code base version number 
	MInt32 lMajor;		// major version number 
	MInt32 lMinor;		// minor version number 
	MInt32 lBuild;		// build version number, increasable only 
	MPChar Version; 	// version in string form 
	MPChar BuildDate;	// latest build date 
	MPChar CopyRight;	// copyright 
} ASGE_FSDK_Version;

//**********************************************************************
// The function is used to initialize the gender estimation module. 
//**********************************************************************
MRESULT ASGE_FSDK_InitGenderEngine(	
	MPChar						AppId,		// [in]  APPID
	MPChar						SDKKey,		// [in]  SDKKEY
	MByte						*pMem,		// [in]	 User allocated memory for the engine
	MInt32						lMemSize,	// [in]	 User allocated memory size
	MHandle						*pEngine	// [out] Pointer pointing to the gender estimation engine
);

// *****************************************************************************
//  The function is used to estimate gender on static image mode automatically. 
// *****************************************************************************
MRESULT ASGE_FSDK_GenderEstimation_StaticImage(	
	MHandle								hEngine,		// [in]  The gender estimation engine
	LPASVLOFFSCREEN						pImgInfo,		// [in]  The original image data
	LPASGE_FSDK_GENDERFACEINPUT			pFaceRes,		// [in]  The face position
	LPASGE_FSDK_GENDERRESULT		    pGenderRes//,	// [out] The detection result
);

// ************************************************************************
//  The function is used to estimate gender on preview mode automatically. 
// ************************************************************************
MRESULT ASGE_FSDK_GenderEstimation_Preview(		
	MHandle							hEngine,		// [in]  The gender estimation engine
	LPASVLOFFSCREEN					pImgInfo,		// [in]  The original image data
	LPASGE_FSDK_GENDERFACEINPUT		pFaceRes,		// [in]  The face position
	LPASGE_FSDK_GENDERRESULT		pGenderRes//,	// [out] The detection result
);

// ***********************************************************************
//  The function is used to release the gender estimation module. 
// ***********************************************************************
MRESULT ASGE_FSDK_UninitGenderEngine(
	MHandle        hEngine		// [in]  Pointer pointing to the gender estimation engine
);

// ***************************************************************************
//  The function used to get version information of gender estimation library. 
// ***************************************************************************
const ASGE_FSDK_Version * ASGE_FSDK_GetVersion(
	MHandle        hEngine	// [in]  Pointer pointing to the gender estimation engine
);


#ifdef __cplusplus
}
#endif

#endif
