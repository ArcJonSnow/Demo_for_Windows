/*******************************************************************************
* Copyright(c) ArcSoft, All right reserved.
*
* This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary
* and confidential information.
*
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER
* AUTHORIZATION.
*
* If you are not an intended recipient of this file, you must not copy,
* distribute, modify, or take any action in reliance on it.
*
* If you have received this file in error, please immediately notify ArcSoft and
* permanently delete the original and any copy of any file and any printout
* thereof.
*********************************************************************************/

#ifndef _ARCSOFT_FSDK_FACEDETECTION_H_
#define _ARCSOFT_FSDK_FACEDETECTION_H_

#include "amcomdef.h"
#include "asvloffscreen.h"


#ifdef __cplusplus
extern "C" {
#endif

    typedef MInt32 AFD_FSDK_OrientPriority;
    typedef MInt32 AFD_FSDK_OrientCode;

    /*******************************************************************************************
     FaceDetection Orientation Priority
     *******************************************************************************************/
    enum _AFD_FSDK_OrientPriority {
        AFD_FSDK_OPF_0_ONLY = 0x1,       // 0, 0, ...
        AFD_FSDK_OPF_90_ONLY = 0x2,      // 90, 90, ...
        AFD_FSDK_OPF_270_ONLY = 0x3,     // 270, 270, ...		
        AFD_FSDK_OPF_180_ONLY = 0x4,     // 180, 180, ...
        AFD_FSDK_OPF_0_HIGHER_EXT = 0x5, // 0, 90, 270, 180, 0, 90, 270, 180, ...
    };

    /*******************************************************************************************
     FaceDetection Face Orientation
     *******************************************************************************************/
    enum _AFD_FSDK_OrientCode {
        AFD_FSDK_FOC_0 = 0x1,   // 0 degree
        AFD_FSDK_FOC_90 = 0x2,  // 90 degree
        AFD_FSDK_FOC_270 = 0x3, // 270 degree
        AFD_FSDK_FOC_180 = 0x4, // 180 degree
        AFD_FSDK_FOC_30 = 0x5,  // 30 degree
        AFD_FSDK_FOC_60 = 0x6,  // 60 degree
        AFD_FSDK_FOC_120 = 0x7, // 120 degree
        AFD_FSDK_FOC_150 = 0x8, // 150 degree
        AFD_FSDK_FOC_210 = 0x9, // 210 degree
        AFD_FSDK_FOC_240 = 0xa, // 240 degree
        AFD_FSDK_FOC_300 = 0xb, // 300 degree
        AFD_FSDK_FOC_330 = 0xc  // 330 degree
    };

    /*******************************************************************************************
     FaceDetection Face Information
     *******************************************************************************************/

    typedef struct {
        MInt32 nFace;                     // number of faces detected
        MRECT *rcFace;                    // The bounding box of face
        AFD_FSDK_OrientCode *lfaceOrient; // the angle of each face
    } AFD_FSDK_FACERES, *LPAFD_FSDK_FACERES;


    /*******************************************************************************************
     FaceDetection Version Information
     *******************************************************************************************/
    typedef struct {
        MInt32 lCodebase; // Codebase version number
        MInt32 lMajor;    // Major version number
        MInt32 lMinor;    // Minor version number
        MInt32 lBuild;    // Build version number, increasable only
        MPChar Version;   // Version in string form
        MPChar BuildDate; // Latest build Date
        MPChar CopyRight; // Copyright
    } AFD_FSDK_Version;

    /************************************************************************
     * The function used to Initialize the face detection engine.
     ************************************************************************/
    MRESULT AFD_FSDK_InitialFaceEngine(
        MPChar AppId,					            // [in]  APPID
        MPChar SDKKey,						        // [in]  SDKKEY
        MByte *pMem,		                        // [in]	 User allocated memory for the engine
        MInt32 lMemSize,	                        // [in]	 User allocated memory size
        MHandle *phEngine,                          // [out] Pointing to the detection engine.
        AFD_FSDK_OrientPriority iOrientPriority,    // [in]  Defining the priority of face orientation
        MInt32 nScale,                              // [in]  An integer defining the minimal face to detect relative to the maximum of image width and height.
        MInt32 nMaxFaceNum                          // [in]  An integer defining the number of max faces to detection
        );

    /************************************************************************
     * The function used to detect face in the input image.
     ************************************************************************/

    MRESULT AFD_FSDK_StillImageFaceDetection(
        MHandle hEngine,                   // [in]  The face detection engine
        LPASVLOFFSCREEN pImgData,          // [in]  The original image data
        LPAFD_FSDK_FACERES *pFaceRes       // [out] The detection result
        );

    /************************************************************************
     * The function used to Uninitialize the detection module.
     ************************************************************************/
    MRESULT AFD_FSDK_UninitialFaceEngine(
        MHandle hEngine        // [in]  The face detection engine
        );

    /************************************************************************
     * The function used to get version information of face detection library.
     ************************************************************************/
    const AFD_FSDK_Version *AFD_FSDK_GetVersion(MHandle hEngine);

#ifdef __cplusplus
}
#endif

#endif //_ARC_FACETRACKING_H_