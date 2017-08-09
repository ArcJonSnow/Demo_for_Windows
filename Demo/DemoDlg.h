
// DemoDlg.h : header file
//

#pragma once
#include <string>
#include <vector>
#include <cv.h>
#include <highgui.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "merror.h"
#include "utils.h"
#include "CvvImage.h"
#include "FDEngine.h"
#include "FTEngine.h"
#include "FREngine.h"

#define MAX_FACEMODELS_NUM 12
#define MAX_INPUT_FACES_NUM 12
typedef struct {
	CImage image;
	CRect rcFace;
	CString * name;
	AFR_FSDK_FACEMODEL faceModel;
} FACE;
typedef struct{
	int nFaceId;
	float fCompareResult;
} CompareResult;
// CDemoDlg dialog
class CDemoDlg : public CDialogEx
{
// Construction
public:
	CDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON					m_hIcon;
	FDEngine*				mFDEngine;
	FTEngine*				mFTEngine;
	FREngine*				mFREngine;
	CWnd*					pInputImageWnd;
	CWnd*					pFaceImageWnd[MAX_FACEMODELS_NUM];
	CRect					mInputPictureFaceRect[MAX_INPUT_FACES_NUM];
	CRect					mInputVideoFaceRect[MAX_INPUT_FACES_NUM];
	CompareResult			mCompareResult[MAX_INPUT_FACES_NUM];
	int						mInputPictureFaceNum;
	int						mInputVideoFaceNum;
	CImage					mInputImage;
	std::vector<FACE *>		mFaceModels;
	int						mFaceModelsNum;
	CString					mFaceName[MAX_FACEMODELS_NUM];
	bool					bCompare;
	bool					bPicture;
	int						mVideoWidth;
	int						mVideoHeight;
	HANDLE					mFRThread;
	HANDLE					hMutex;
	AFT_FSDK_FACERES		mFTFaceResult;
	CvCapture*				m_pCapture;
	IplImage*				pFrame;
	int						mFps;
	bool					bPlayflag;
	
	static DWORD WINAPI FRThread(LPVOID lpThreadParameter);
	void FRThreadImp();
	void DrawFaceRectOfInputVideo();
	void ShowImage(IplImage* img, UINT ID);
	void Play();
	void Stop();
	void DoClose();
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedAddToFaceLib();
	afx_msg void OnBnClickedFRPicture();
	afx_msg void OnBnClickedFRVideo();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
