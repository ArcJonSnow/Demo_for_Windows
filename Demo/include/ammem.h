/*----------------------------------------------------------------------------------------------
*
* This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary and 		
* confidential information. 
* 
* The information and code contained in this file is only for authorized ArcSoft employees 
* to design, create, modify, or review.
* 
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER AUTHORIZATION.
* 
* If you are not an intended recipient of this file, you must not copy, distribute, modify, 
* or take any action in reliance on it. 
* 
* If you have received this file in error, please immediately notify ArcSoft and 
* permanently delete the original and any copy of any file and any printout thereof.
*
*-------------------------------------------------------------------------------------------------*/

#ifndef __AMMEM_H__
#define __AMMEM_H__

#include "amcomdef.h"

typedef struct	__tag_mem_info
{
	MDWord	dwTotalMemUsed;
	MDWord	dwTotalMemFree;
}MMEMINFO, *LPMMEMINFO;


#ifdef __cplusplus
extern "C" {
#endif


MVoid*	MMemAlloc(MHandle hContext, MLong lSize);
MVoid*	MMemRealloc(MHandle hContext, MVoid* pMem, MLong lSize);
MVoid	MMemFree(MHandle hContext, MVoid* pMem);
MVoid	MMemSet(MVoid* pMem, MByte byVal, MLong lSize);
MVoid	MMemCpy(MVoid* pDst, const MVoid* pSrc, MLong lSize);
MVoid	MMemMove(MVoid* pDst, MVoid* pSrc, MLong lSize);
MLong	MMemCmp(MVoid* pBuf1, MVoid* pBuf2, MLong lSize);

MHandle MMemMgrCreate(MVoid* pMem, MLong lMemSize);
MVoid	MMemMgrDestroy(MHandle hMemMgr);	

MLong   GetMaxAllocMemSize(MHandle hContext);


typedef struct
{
	    MLong lCodebase;             	// Codebase version number 
	    MLong lMajor;                	// major version number 
	    MLong lMinor;                	// minor version number
	    MLong lBuild;                	// Build version number, increasable only
	    const MChar *Version;        	// version in string form
	    const MChar *BuildDate;      	// latest build Date
	    const MChar *CopyRight;      	// copyright 
}MPBASE_Version;

const MPBASE_Version* Mpbase_GetVersion();

#ifdef __cplusplus
}
#endif

#endif

