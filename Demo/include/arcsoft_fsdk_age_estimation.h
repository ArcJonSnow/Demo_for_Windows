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
#ifndef _ARCSOFT_FSDK_AGE_ESTIMATION_H_
#define _ARCSOFT_FSDK_AGE_ESTIMATION_H_

#include "amcomdef.h"
#include "asvloffscreen.h"

#ifdef __cplusplus
extern "C" {
#endif

// This enumeration defines the orientation of the face in anti-clockwise sequence. 
enum ASAE_FSDK_AgeOrientCode{
	ASAE_FSDK_FOC_Age_0			= 0x1,		// 0 degree 
	ASAE_FSDK_FOC_Age_90		= 0x2,		// 90 degree 
	ASAE_FSDK_FOC_Age_270		= 0x3,		// 270 degree 
	ASAE_FSDK_FOC_Age_180		= 0x4,		// 180 degree 
	ASAE_FSDK_FOC_Age_30		= 0x5,		// 30 degree 
	ASAE_FSDK_FOC_Age_60		= 0x6,		// 60 degree 
	ASAE_FSDK_FOC_Age_120		= 0x7,		// 120 degree 
	ASAE_FSDK_FOC_Age_150		= 0x8,		// 150 degree 
	ASAE_FSDK_FOC_Age_210		= 0x9,		// 210 degree 
	ASAE_FSDK_FOC_Age_240		= 0xa,		// 240 degree 
	ASAE_FSDK_FOC_Age_300		= 0xb,		// 300 degree 
	ASAE_FSDK_FOC_Age_330		= 0xc		// 330 degree 
};

// This structure defines the input face information. 
typedef struct{
	MRECT 		 *pFaceRectArray;		// bounding boxes of input faces. 
	MInt32		 *pFaceOrientArray;		// orientations of input faces. It must be set as one item of "ASAE_FSDK_AgeOrientCode". 
	MInt32		 lFaceNumber;			// number of input faces, and it must be greater than or euqual to 0. 
} ASAE_FSDK_AGEFACEINPUT, *LPASAE_FSDK_AGEFACEINPUT;

// This structure defines the age estimation results. 
typedef struct{
	MInt32 			*pAgeResultArray;	// the age result array with length of "lFaceNumber"
										//the value will be equal to or greater than 0. 
										//specifically, "0" represents unknown, and the value greater than 0 represents valid age. 
	MInt32			lFaceNumber;		// It is the same as "lFaceNumber" in "ASAE_FSDK_AGEFACEINPUT". 
} ASAE_FSDK_AGERESULT, *LPASAE_FSDK_AGERESULT;

// This structure describes the version information of the age estimation library. 
typedef struct
{
	MInt32 lCodebase;	// code base version number 
	MInt32 lMajor;		// major version number 
	MInt32 lMinor;		// minor version number 
	MInt32 lBuild;		// build version number 
	MPChar Version; 	// version in string form 
	MPChar BuildDate;	// latest build date 
	MPChar CopyRight;	// copyright information 
} ASAE_FSDK_Version;

// *********************************************************************
//  This function is used to initialize the age estimation engine
// ********************************************************************
MRESULT ASAE_FSDK_InitAgeEngine(
	MPChar						AppId,		// [in]  APPID
	MPChar						SDKKey,		// [in]  SDKKEY
	MByte						*pMem,		// [in]	 User allocated memory for the engine
	MInt32						lMemSize,	// [in]	 allocated memory size
	MHandle						*pEngine	// [out] pointer to the engine 
);

// **************************************************************************
//  This function is used to estimate age in static image mode automatically. 
// **************************************************************************
MRESULT ASAE_FSDK_AgeEstimation_StaticImage(	
	MHandle						hEngine,	// [in] age estimation engine 
	LPASVLOFFSCREEN				pImgInfo,	// [in] the original image information 
	LPASAE_FSDK_AGEFACEINPUT	pFaceRes,	// [in] the face rectangles information. 
	LPASAE_FSDK_AGERESULT		pAgeRes		// [out] the results of age estimation 
);

// *********************************************************************
//  This function is used to estimate age in preview mode automatically. 
// *********************************************************************
MRESULT ASAE_FSDK_AgeEstimation_Preview(
	MHandle						hEngine,	// [in] age estimation engine 
	LPASVLOFFSCREEN				pImgInfo,	// [in] the original image information 
	LPASAE_FSDK_AGEFACEINPUT	pFaceRes,	// [in] the face rectangles information 
	LPASAE_FSDK_AGERESULT		pAgeRes		// [out] the results of age estimation 
);

// ***********************************************************************
//  This function is used to release the age estimation engine. 
// ***********************************************************************
MRESULT ASAE_FSDK_UninitAgeEngine(
	MHandle	hEngine		// [in]  pointer to the engine
);

// *********************************************************************
//  This function is used to get the version information of the library. 
// *********************************************************************
const ASAE_FSDK_Version * ASAE_FSDK_GetVersion(
	MHandle	hEngine		// [in]  pointer to the engine
);

#ifdef __cplusplus
}
#endif

#endif
