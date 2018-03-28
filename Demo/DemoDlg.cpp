
// DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Demo.h"
#include "DemoDlg.h"
#include "afxdialogex.h"
#include "ConvertImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDemoDlg::CDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDemoDlg::IDD, pParent)
	, mInputPictureFaceNum(0)
	, mInputVideoFaceNum(0)
	, mFaceModelsNum(0)
	, mLastFRTime(0)
	, bCompare(false)
	, bPicture(false)
	, bPlayflag(false)
	, pFrame(nullptr)
	, m_pCapture(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME1, mFaceName[0]);
	DDX_Text(pDX, IDC_NAME2, mFaceName[1]);
	DDX_Text(pDX, IDC_NAME3, mFaceName[2]);
	DDX_Text(pDX, IDC_NAME4, mFaceName[3]);
	DDX_Text(pDX, IDC_NAME5, mFaceName[4]);
	DDX_Text(pDX, IDC_NAME6, mFaceName[5]);
	DDX_Text(pDX, IDC_NAME7, mFaceName[6]);
	DDX_Text(pDX, IDC_NAME8, mFaceName[7]);
	DDX_Text(pDX, IDC_NAME9, mFaceName[8]);
	DDX_Text(pDX, IDC_NAME10, mFaceName[9]);
	DDX_Text(pDX, IDC_NAME11, mFaceName[10]);
	DDX_Text(pDX, IDC_NAME12, mFaceName[11]);
	DDX_Control(pDX, BUTTON_ADDTOFACELIB, mAddToFaceLib);
	DDX_Control(pDX, BUTTON_FRCAMERA, mFRCamera);
	DDX_Control(pDX, BUTTON_FRPICTURE, mFRPicture);
	DDX_Control(pDX, BUTTON_FRVIDEO, mFRVideo);
	DDX_Control(pDX, BUTTON_STOP, mStopButton);
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BUTTON_ADDTOFACELIB, &CDemoDlg::OnBnClickedAddToFaceLib)
	ON_BN_CLICKED(BUTTON_FRPICTURE, &CDemoDlg::OnBnClickedFRPicture)
	ON_BN_CLICKED(BUTTON_FRVIDEO, &CDemoDlg::OnBnClickedFRVideo)
	ON_BN_CLICKED(BUTTON_FRCAMERA, &CDemoDlg::OnBnClickedFrcamera)
	ON_BN_CLICKED(BUTTON_STOP, &CDemoDlg::OnBnClickedStop)
END_MESSAGE_MAP()

// CDemoDlg message handlers

BOOL CDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 初始化变量
	for (int i = 0; i < MAX_FACEMODELS_NUM; i++)
	{
		mFaceName[i].Format(_T("%d号"), i);
	}
	for (int i = 0; i < MAX_INPUT_FACES_NUM; i++)
	{
		mCompareResult[i].nFaceId = -1;
		mCompareResult[i].fCompareResult = 0.0f;
		mCompareResult[i].nAge = 0;
		mCompareResult[i].nGender = -1;
	}
	mFTFaceResult.rcFace = new MRECT[MAX_INPUT_FACES_NUM];
	hMutex = CreateMutex(NULL, FALSE, _T("frame"));
	// 初始化引擎
	int ret = 0;
	mFDEngine = new FDEngine();
	ret = mFDEngine->init();
	if (ret != MOK)
	{
		CString str;
		str.Format(_T("初始化FD引擎失败,错误码: %d, 程序将关闭！"), ret);
		MessageBox(str);
		DoClose();
		exit(0);
		return false;
	}
	mFREngine = new FREngine();
	ret = mFREngine->init();
	if (ret != MOK)
	{
		CString str;
		str.Format(_T("初始化FR引擎失败,错误码: %d, 程序将关闭!"), ret);
		MessageBox(str);
		DoClose();
		exit(0);
		return false;
	}
	mAgeEngine = new AgeEngine();
	ret = mAgeEngine->init();
	if (ret != MOK)
	{
		CString str;
		str.Format(_T("初始化Age引擎失败,错误码: %d, 程序将关闭!"), ret);
		MessageBox(str);
		DoClose();
		exit(0);
		return false;
	}
	mGenderEngine = new GenderEngine();
	ret = mGenderEngine->init();
	if (ret != MOK)
	{
		CString str;
		str.Format(_T("初始化Gender引擎失败,错误码: %d, 程序将关闭!"), ret);
		MessageBox(str);
		DoClose();
		exit(0);
		return false;
	}
	// 绑定PIC控件
	pInputImageWnd = GetDlgItem(PIC_INPUTIMAGE);
	pFaceImageWnd[0] = GetDlgItem(PIC_FACE1);
	pFaceImageWnd[1] = GetDlgItem(PIC_FACE2);
	pFaceImageWnd[2] = GetDlgItem(PIC_FACE3);
	pFaceImageWnd[3] = GetDlgItem(PIC_FACE4);
	pFaceImageWnd[4] = GetDlgItem(PIC_FACE5);
	pFaceImageWnd[5] = GetDlgItem(PIC_FACE6);
	pFaceImageWnd[6] = GetDlgItem(PIC_FACE7);
	pFaceImageWnd[7] = GetDlgItem(PIC_FACE8);
	pFaceImageWnd[8] = GetDlgItem(PIC_FACE9);
	pFaceImageWnd[9] = GetDlgItem(PIC_FACE10);
	pFaceImageWnd[10] = GetDlgItem(PIC_FACE11);
	pFaceImageWnd[11] = GetDlgItem(PIC_FACE12);
	UpdateData(FALSE);
	// 设置定时器
	SetTimer(0, 1000, NULL);
	mStopButton.EnableWindow(0);
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CDemoDlg::DoClose()
{
	// 停止运行并释放内存和引擎
	Stop();
	if (mFDEngine != nullptr)
	{
		mFDEngine->uninit();
		delete mFDEngine;
		mFDEngine = nullptr;
	}
	if (mFREngine != nullptr)
	{
		mFREngine->uninit();
		delete mFREngine;
		mFREngine = nullptr;
	}
	if (mAgeEngine != nullptr)
	{
		mAgeEngine->uninit();
		delete mAgeEngine;
		mAgeEngine = nullptr;
	}
	if (mGenderEngine != nullptr)
	{
		mGenderEngine->uninit();
		delete mGenderEngine;
		mGenderEngine = nullptr;
	}
	for (std::vector<FACE*>::iterator iter = mFaceModels.begin(); iter != mFaceModels.end();)
	{
		(*iter)->image.Destroy();
		delete[] (*iter)->faceModel.pbFeature;
		delete (*iter);
		iter = mFaceModels.erase(iter);
	}
	if (mFTFaceResult.rcFace != nullptr)
	{
		delete[] mFTFaceResult.rcFace;
		mFTFaceResult.rcFace = nullptr;
	}
	CloseHandle(hMutex);
}
void CDemoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	DoClose();
	CDialogEx::OnClose();
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		
		if (bPicture)	//判断当前需要显示的是否为图片,如果是图片则进行图片渲染
		{
			if (!mInputImage.IsNull())	//判断渲染图片是否为NULL
			{
				CRect rc;	//定义图片显示控件PIC_INPUTIMAGE的矩形框
				CRect rcScale;	//定义图片缩放后对应的矩形框
				pInputImageWnd->GetClientRect(rc);
				int height = mInputImage.GetHeight();
				int width = mInputImage.GetWidth();
				CDC* pDc;
				pDc = pInputImageWnd->GetDC();
				SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
				CRect *face = new CRect[mInputPictureFaceNum];

				float xScale, yScale, ScaleIndex;	//计算图片缩放比例
				if (width <= rc.Width() && height <= rc.Height())
				{
					ScaleIndex = 1;
				}
				else
				{
					xScale = (float)rc.Width() / (float)width;
					yScale = (float)rc.Height() / (float)height;
					ScaleIndex = (xScale >= yScale ? yScale : xScale);
				}
				rcScale = CRect(rc.TopLeft(), CSize((int)width * ScaleIndex, (int)height * ScaleIndex));
				for (int i = 0; i < mInputPictureFaceNum; i++)
				{
					face[i].left = (int)mInputPictureFaceRect[i].left * ScaleIndex;
					face[i].right = (int)mInputPictureFaceRect[i].right * ScaleIndex;
					face[i].top = (int)mInputPictureFaceRect[i].top * ScaleIndex;
					face[i].bottom = (int)mInputPictureFaceRect[i].bottom * ScaleIndex;
				}
				mInputImage.StretchBlt(pDc->m_hDC, rcScale, SRCCOPY);	//将图片按缩放比例显示在PIC_INPUTIMAGE控件
				
				CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
				CClientDC dc(pInputImageWnd);
				dc.SelectObject(pen);
				dc.SelectStockObject(NULL_BRUSH);
				for (int i = 0; i < mInputPictureFaceNum; i++)
				{
					dc.Rectangle(face[i].left, face[i].top, face[i].right, face[i].bottom);		//绘制人脸框
					if (bCompare)		//判断是否为人脸识别对比，如果是则将名字等信息显示在人脸框
					{
						dc.SetBkMode(TRANSPARENT);
						dc.SetTextColor(RGB(255, 255, 255));
						
						CString str;
						if (mCompareResult[i].nFaceId != -1)
						{
							str.AppendFormat(*mFaceModels[mCompareResult[i].nFaceId]->name + _T(", %.3f\n"), mCompareResult[i].fCompareResult);
						}
						if (mCompareResult[i].nAge != 0)
						{
							str.AppendFormat(_T("%d岁 "), mCompareResult[i].nAge);
						}
						if (mCompareResult[i].nGender != -1)
						{
							str.Append(mCompareResult[i].nGender ? _T("女") : _T("男"));
						}
						dc.DrawTextEx(str, face[i], DT_TOP | DT_CENTER | DT_NOCLIP, NULL);
					}
				}
				ReleaseDC(pDc);
				delete[] face;
			}
		}
		for (int i = 0; i < mFaceModelsNum; i++)	//绘制右侧人脸库中的人脸图片
		{
			CDC* pFaceDC;
			pFaceDC = pFaceImageWnd[i]->GetDC();
			CRect face;
			pFaceImageWnd[i]->GetClientRect(face);
			int faceWidth = mFaceModels[i]->rcFace.right - mFaceModels[i]->rcFace.left;
			int faceHeight = mFaceModels[i]->rcFace.bottom - mFaceModels[i]->rcFace.top;
			float xScale = (float)face.Width() / (float)faceWidth;
			float yScale = (float)face.Height() / (float)faceHeight;
			float ScaleIndex = (xScale >= yScale ? yScale : xScale);
			face = CRect(face.TopLeft(), CSize((int)faceWidth*ScaleIndex, (int)faceHeight*ScaleIndex));
			CRect faceRect = CRect(mFaceModels[i]->rcFace.left, mFaceModels[i]->rcFace.top, mFaceModels[i]->rcFace.right, mFaceModels[i]->rcFace.bottom);
			SetStretchBltMode(pFaceDC->m_hDC, STRETCH_HALFTONE);
			mFaceModels[i]->image.StretchBlt(pFaceDC->m_hDC, face, faceRect, SRCCOPY);
			ReleaseDC(pFaceDC);
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDemoDlg::OnBnClickedAddToFaceLib()
{
	// TODO: Add your control notification handler code here
	// 创建对话框读取图片路径
	CFileDialog fileDlg(TRUE, _T("bmp"), NULL, 0, _T("Picture Files|*.*||"), this);
	fileDlg.DoModal();
	CString strFilePath;
	strFilePath = fileDlg.GetPathName();
	if (strFilePath == _T(""))
	{
		return;
	}
	// 读取图片数据信息存入ASVLOFFSCREEN结构体
	ASVLOFFSCREEN offInput = { 0 };
	offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
	offInput.ppu8Plane[0] = nullptr;
	char path[MAX_PATH];
	int len = WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, path, len, NULL, NULL);
	bool ret = CConvertImage::readBmp(path, (uint8_t**)&offInput.ppu8Plane[0], &offInput.i32Width, &offInput.i32Height);
	if (!offInput.ppu8Plane[0] || !ret)
	{
		MessageBox(_T("读取文件失败！"));
		return;
	}
	offInput.pi32Pitch[0] = offInput.i32Width * 3;//因为在读取图片的时候将补齐字节的部分去掉了，因此每一行的字节数是宽X3。如果直接使用bmp图片读取的是4字节对齐数据，需要将pi32Pitch设置成补齐后每一行的字节数

	//进行FD获取人脸框位置
	LPAFD_FSDK_FACERES faceRes;
	int mRet = mFDEngine->FaceDetection(&offInput, &faceRes);
	if (faceRes->nFace <= 0 || mRet != MOK)
	{
		MessageBox(_T("图片中没有检测到人脸！"));
	}
	mInputImage.Destroy();
	mInputImage.Load(strFilePath); 
	mInputPictureFaceNum = faceRes->nFace;
	for (int i = 0; i < faceRes->nFace; i++)
	{
		mInputPictureFaceRect[i].SetRect(faceRes->rcFace[i].left, faceRes->rcFace[i].top, faceRes->rcFace[i].right, faceRes->rcFace[i].bottom);
	}
	
	bPicture = true;
	bCompare = false;
	Invalidate(TRUE);
	UpdateWindow();
	// 对检测到的人脸进行FR人脸特征提取，将提取的人脸特征信息存入人脸库
	for (int i = 0; i < faceRes->nFace; i++)
	{
		if (mFaceModelsNum > MAX_FACEMODELS_NUM - 1)
		{
			Invalidate(FALSE);
			continue;
		}
		FACE *face = new FACE;
		face->image.Load(strFilePath);
		face->rcFace.SetRect(faceRes->rcFace[i].left, faceRes->rcFace[i].top, faceRes->rcFace[i].right, faceRes->rcFace[i].bottom);
		AFR_FSDK_FACEMODEL LocalFaceModels;
		AFR_FSDK_FACEINPUT faceLocation;
		faceLocation.rcFace = faceRes->rcFace[i];
		faceLocation.lOrient = faceRes->lfaceOrient[i];
		mRet = mFREngine->ExtractFRFeature(&offInput, &faceLocation, &LocalFaceModels);
		if (mRet != MOK)
		{
			MessageBox(_T("提取特征失败!"));
		}
		face->faceModel.lFeatureSize = LocalFaceModels.lFeatureSize;
		face->faceModel.pbFeature = new MByte[face->faceModel.lFeatureSize];
		memcpy(face->faceModel.pbFeature, LocalFaceModels.pbFeature, face->faceModel.lFeatureSize);//LocalFaceModels.pbFeature指向人脸特征信息，将其数据拷贝到人脸库，如果不拷贝出来，用相同的引擎进行特征提取时会覆盖上一次得到的人脸特征信息
		face->name = &mFaceName[mFaceModelsNum];
		mFaceModels.push_back(face);
		mFaceModelsNum++;
	}
	bCompare = false;
	Invalidate(FALSE);
	if (mFaceModelsNum > MAX_FACEMODELS_NUM - 1)
	{
		MessageBox(_T("人脸库已经满了！"));
	}
	free(offInput.ppu8Plane[0]);
}

void CDemoDlg::OnBnClickedFRPicture()
{
	// TODO: Add your control notification handler code here
	// 创建对话框读取图片路径
	CFileDialog fileDlg(TRUE, _T("bmp"), NULL, 0, _T("Picture Files|*.*||"), this);
	fileDlg.DoModal();
	CString strFilePath;
	strFilePath = fileDlg.GetPathName();
	if (strFilePath == _T(""))
	{
		return;
	}
	// 读取图片数据信息存入ASVLOFFSCREEN结构体
	ASVLOFFSCREEN offInput = { 0 };
	offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
	offInput.ppu8Plane[0] = nullptr;
	char path[MAX_PATH];
	int len = WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, path, len, NULL, NULL);
	bool ret = CConvertImage::readBmp(path, (uint8_t**)&offInput.ppu8Plane[0], &offInput.i32Width, &offInput.i32Height);
	if (!offInput.ppu8Plane[0] || !ret)
	{
		MessageBox(_T("读取文件失败！"));
		return;
	}
	offInput.pi32Pitch[0] = offInput.i32Width * 3;//因为在读取图片的时候将补齐字节的部分去掉了，因此每一行的字节数是宽X3。如果直接使用bmp图片读取的是4字节对齐数据，需要将pi32Pitch设置成补齐后每一行的字节数
	//进行FD获取人脸框位置
	LPAFD_FSDK_FACERES faceRes;
	int mRet = mFDEngine->FaceDetection(&offInput, &faceRes);
	if (faceRes->nFace <= 0 || mRet != MOK)
	{
		MessageBox(_T("图片中没有检测到人脸！"));
	}
	mInputImage.Destroy();
	mInputImage.Load(strFilePath);
	mInputPictureFaceNum = faceRes->nFace;
	for (int i = 0; i < faceRes->nFace; i++)
	{
		mInputPictureFaceRect[i].SetRect(faceRes->rcFace[i].left, faceRes->rcFace[i].top, faceRes->rcFace[i].right, faceRes->rcFace[i].bottom);
	}
	bPicture = true;
	bCompare = false;
	Invalidate(TRUE);
	UpdateWindow();

	// 进行AgeEstimation
	ASAE_FSDK_AGEFACEINPUT ageInputFaceRes;
	ASAE_FSDK_AGERESULT ageRes;
	ageInputFaceRes.lFaceNumber = faceRes->nFace;
	ageInputFaceRes.pFaceRectArray = new MRECT[ageInputFaceRes.lFaceNumber];
	ageInputFaceRes.pFaceOrientArray = new int[ageInputFaceRes.lFaceNumber];
	for (int i = 0; i < ageInputFaceRes.lFaceNumber; i++)
	{
		ageInputFaceRes.pFaceRectArray[i].left = faceRes->rcFace[i].left;
		ageInputFaceRes.pFaceRectArray[i].top = faceRes->rcFace[i].top;
		ageInputFaceRes.pFaceRectArray[i].right = faceRes->rcFace[i].right;
		ageInputFaceRes.pFaceRectArray[i].bottom = faceRes->rcFace[i].bottom;
		ageInputFaceRes.pFaceOrientArray[i] = faceRes->lfaceOrient[i];
	}
	mAgeEngine->AgeEstimationStaticImage(&offInput, &ageInputFaceRes, &ageRes);
	delete[] ageInputFaceRes.pFaceRectArray;
	delete[] ageInputFaceRes.pFaceOrientArray;
	// 进行GenderEstimation
	ASGE_FSDK_GENDERFACEINPUT genderInputFaceRes;
	ASGE_FSDK_GENDERRESULT genderRes;
	genderInputFaceRes.lFaceNumber = faceRes->nFace;
	genderInputFaceRes.pFaceRectArray = new MRECT[genderInputFaceRes.lFaceNumber];
	genderInputFaceRes.pFaceOrientArray = new int[genderInputFaceRes.lFaceNumber];
	for (int i = 0; i < genderInputFaceRes.lFaceNumber; i++)
	{
		genderInputFaceRes.pFaceRectArray[i].left = faceRes->rcFace[i].left;
		genderInputFaceRes.pFaceRectArray[i].top = faceRes->rcFace[i].top;
		genderInputFaceRes.pFaceRectArray[i].right = faceRes->rcFace[i].right;
		genderInputFaceRes.pFaceRectArray[i].bottom = faceRes->rcFace[i].bottom;
		genderInputFaceRes.pFaceOrientArray[i] = faceRes->lfaceOrient[i];
	}
	mGenderEngine->GenderEstimationStaticImage(&offInput, &genderInputFaceRes, &genderRes);
	delete[] genderInputFaceRes.pFaceRectArray;
	delete[] genderInputFaceRes.pFaceOrientArray;
	// 将年龄和性别的结果存入mCompareResult，用来进行显示
	for (int i = 0; i < faceRes->nFace; i++)
	{
		mCompareResult[i].nAge = ageRes.pAgeResultArray[i];
		mCompareResult[i].nGender = genderRes.pGenderResultArray[i];
	}
	//进行人脸比对，将结果存储在mCompareResult
	for (int i = 0; i < faceRes->nFace; i++)
	{
		AFR_FSDK_FACEINPUT faceResult;
		AFR_FSDK_FACEMODEL faceModel;
		faceResult.rcFace = faceRes->rcFace[i];
		faceResult.lOrient = faceRes->lfaceOrient[i];
		mRet = mFREngine->ExtractFRFeature(&offInput, &faceResult, &faceModel);
		if (mRet != MOK)
		{
			MessageBox(_T("提取特征失败!"));
		}
		float maxScore = 0;
		for (int j = 0; j < mFaceModelsNum; j++)
		{
			float fSimilScore = 0.0f;
			mFREngine->FacePairMatching(&(mFaceModels[j]->faceModel), &faceModel, &fSimilScore);
			if (fSimilScore >= maxScore)
			{
				maxScore = fSimilScore;
				if (maxScore > 0.55)
				{
					mCompareResult[i].nFaceId = j;
					mCompareResult[i].fCompareResult = maxScore;
				}
				else
				{
					mCompareResult[i].nFaceId = -1;
					mCompareResult[i].fCompareResult = 0.0f;
				}
			}
		}
	}
	if (faceRes->nFace == 0)
	{
		bCompare = false;
	}
	else
	{
		bCompare = true;
	}
	Invalidate(FALSE);
	free(offInput.ppu8Plane[0]);
}

void CDemoDlg::OnBnClickedFRVideo()
{
	// TODO: Add your control notification handler code here
	if (!bPlayflag)
	{
		// 创建对话框读取视频路径
		CFileDialog  fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
			_T("*.mp4|*.mp4|*.avi|*.avi||"), NULL);
		fileDlg.m_ofn.lpstrTitle = _T("选取视频文件");
		fileDlg.DoModal();
		CString strFilePath;
		strFilePath = fileDlg.GetPathName();
		if (strFilePath == _T(""))
		{
			return;
		}
		else
		{
			char path[MAX_PATH];
			int len = WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, path, len, NULL, NULL);
			if (!(m_pCapture = cvCreateFileCapture(path)))
			{
				MessageBox(_T("打开视频文件失败!"));
				return;
			}
		}
		// 初始化FT引擎
		int ret = 0;
		if (mFTEngine == nullptr)
		{
			mFTEngine = new FTEngine();
			ret = mFTEngine->init();
			if (ret != MOK)
			{
				CString str;
				str.Format(_T("初始化FT引擎失败,错误码: %d, 程序将关闭!"), ret);
				MessageBox(str);
				DoClose();
				exit(0);
			}
		}
		// 初始化AgeForVideo引擎
		if (mAgeEngineForVideo == nullptr)
		{
			mAgeEngineForVideo = new AgeEngine();
			ret = mAgeEngineForVideo->init();
			if (ret != MOK)
			{
				CString str;
				str.Format(_T("初始化AgeForVideo引擎失败,错误码: %d, 程序将关闭!"), ret);
				MessageBox(str);
				DoClose();
				exit(0);
			}
		}
		// 初始化GenderForVideo引擎
		if (mGenderEngineForVideo == nullptr)
		{
			mGenderEngineForVideo = new GenderEngine();
			ret = mGenderEngineForVideo->init();
			if (ret != MOK)
			{
				CString str;
				str.Format(_T("初始化AgeForVideo引擎失败,错误码: %d, 程序将关闭!"), ret);
				MessageBox(str);
				DoClose();
				exit(0);
			}
		}
		// 获取视频帧数据
		WaitForSingleObject(hMutex, INFINITE);
		pFrame = cvQueryFrame(m_pCapture);
		mVideoWidth = pFrame->width;
		mVideoHeight = pFrame->height;
		ReleaseMutex(hMutex);
		bPicture = false;
		Play();
		//创建FR线程
		mFRThread = CreateThread(NULL, 0, FRThread, this, 0, NULL);
	}
}
void CDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (0 == nIDEvent)
	{
		UpdateData();
		CDialogEx::OnTimer(nIDEvent);
	}
	if (1 == nIDEvent)
	{
		WaitForSingleObject(hMutex, INFINITE);
		// 获取视频帧数据
		pFrame = cvQueryFrame(m_pCapture);
		if (!pFrame)
		{
			Stop();
			return;
		}
		else
		{
			// 读取视频帧数据信息存入ASVLOFFSCREEN结构体
			ASVLOFFSCREEN offInput = { 0 };
			offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
			offInput.i32Width = pFrame->width;
			offInput.i32Height = pFrame->height;
			offInput.ppu8Plane[0] = (uint8_t *)pFrame->imageData;
			offInput.pi32Pitch[0] = offInput.i32Width * 3;

			// 进行FT，获取人脸框信息
			LPAFT_FSDK_FACERES faceRes;
			mFTEngine->FaceTracking(&offInput, &faceRes);
			ShowImage(pFrame, PIC_INPUTIMAGE);
			mInputVideoFaceNum = faceRes->nFace;
			mFTFaceResult.nFace = faceRes->nFace;
			for (int i = 0; i < faceRes->nFace; i++)
			{
				mInputVideoFaceRect[i].SetRect(faceRes->rcFace[i].left, faceRes->rcFace[i].top, faceRes->rcFace[i].right, faceRes->rcFace[i].bottom);
				mFTFaceResult.lfaceOrient = faceRes->lfaceOrient;
				memcpy(&mFTFaceResult.rcFace[i], &faceRes->rcFace[i], sizeof(faceRes->rcFace[i]));
			}
			ReleaseMutex(hMutex);

			ASAE_FSDK_AGEFACEINPUT ageInputFaceRes;
			ASAE_FSDK_AGERESULT ageRes;
			ageInputFaceRes.lFaceNumber = faceRes->nFace;
			ageInputFaceRes.pFaceRectArray = new MRECT[ageInputFaceRes.lFaceNumber];
			ageInputFaceRes.pFaceOrientArray = new int[ageInputFaceRes.lFaceNumber];
			for (int i = 0; i < ageInputFaceRes.lFaceNumber; i++)
			{
				ageInputFaceRes.pFaceRectArray[i].left = faceRes->rcFace[i].left;
				ageInputFaceRes.pFaceRectArray[i].top = faceRes->rcFace[i].top;
				ageInputFaceRes.pFaceRectArray[i].right = faceRes->rcFace[i].right;
				ageInputFaceRes.pFaceRectArray[i].bottom = faceRes->rcFace[i].bottom;
				ageInputFaceRes.pFaceOrientArray[i] = faceRes->lfaceOrient;
			}
			mAgeEngineForVideo->AgeEstimationPreview(&offInput, &ageInputFaceRes, &ageRes);
			delete[] ageInputFaceRes.pFaceRectArray;
			delete[] ageInputFaceRes.pFaceOrientArray;
			// 进行GenderEstimation
			ASGE_FSDK_GENDERFACEINPUT genderInputFaceRes;
			ASGE_FSDK_GENDERRESULT genderRes;
			genderInputFaceRes.lFaceNumber = faceRes->nFace;
			genderInputFaceRes.pFaceRectArray = new MRECT[genderInputFaceRes.lFaceNumber];
			genderInputFaceRes.pFaceOrientArray = new int[genderInputFaceRes.lFaceNumber];
			for (int i = 0; i < genderInputFaceRes.lFaceNumber; i++)
			{
				genderInputFaceRes.pFaceRectArray[i].left = faceRes->rcFace[i].left;
				genderInputFaceRes.pFaceRectArray[i].top = faceRes->rcFace[i].top;
				genderInputFaceRes.pFaceRectArray[i].right = faceRes->rcFace[i].right;
				genderInputFaceRes.pFaceRectArray[i].bottom = faceRes->rcFace[i].bottom;
				genderInputFaceRes.pFaceOrientArray[i] = faceRes->lfaceOrient;
			}
			mGenderEngineForVideo->GenderEstimationPreview(&offInput, &genderInputFaceRes, &genderRes);
			delete[] genderInputFaceRes.pFaceRectArray;
			delete[] genderInputFaceRes.pFaceOrientArray;
			// 将年龄和性别的结果存入mCompareResult，用来进行显示
			for (int i = 0; i < faceRes->nFace; i++)
			{
				mCompareResult[i].nAge = ageRes.pAgeResultArray[i];
				mCompareResult[i].nGender = genderRes.pGenderResultArray[i];
			}

			bPicture = false;
			DrawFaceRectOfInputVideo();
		}
		CDialogEx::OnTimer(nIDEvent);
	}
}
DWORD WINAPI CDemoDlg::FRThread(LPVOID lpThreadParameter)
{	
	((CDemoDlg*)lpThreadParameter)->FRThreadImp();
	return NULL;
}
void CDemoDlg::FRThreadImp()
{
	while (bPlayflag)
	{
		clock_t duration = clock() - mLastFRTime;// 设置时间判定，防止长时间占用内存
		if (duration < 30)
		{
			Sleep(1);
			continue;
		}
		mLastFRTime = clock();
		WaitForSingleObject(hMutex, INFINITE);
		if (!bPlayflag)
		{
			ReleaseMutex(hMutex);
			break;
		}
		// 读取视频帧数据信息存入ASVLOFFSCREEN结构体
		ASVLOFFSCREEN offInput = { 0 };
		offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
		offInput.i32Width = pFrame->width;
		offInput.i32Height = pFrame->height;
		uint8_t * frameData = new uint8_t[offInput.i32Width * offInput.i32Height * 3];
		memcpy(frameData, (uint8_t *)pFrame->imageData, offInput.i32Width * offInput.i32Height * 3);
		offInput.ppu8Plane[0] = frameData;
		offInput.pi32Pitch[0] = offInput.i32Width * 3;
		// 拷贝FT获得的人脸框信息
		int FTFaceNum = mFTFaceResult.nFace;
		LPAFR_FSDK_FACEINPUT faceResult = new AFR_FSDK_FACEINPUT[FTFaceNum];
		for (int i = 0; i < FTFaceNum; i++)
		{
			faceResult[i].rcFace = mFTFaceResult.rcFace[i];
			faceResult[i].lOrient = mFTFaceResult.lfaceOrient;
		}
		ReleaseMutex(hMutex);
		// 根据FT获得的人脸框信息，进行人脸特征提取和匹配
		for (int i = 0; i < mFTFaceResult.nFace; i++)
		{
			AFR_FSDK_FACEMODEL faceModel;
			int mRet = mFREngine->ExtractFRFeature(&offInput, &faceResult[i], &faceModel);
			if (mRet == MOK)
			{
				float maxScore = 0;
				for (int j = 0; j < mFaceModelsNum; j++)
				{
					float fSimilScore;
					mFREngine->FacePairMatching(&(mFaceModels[j]->faceModel), &faceModel, &fSimilScore);
					if (fSimilScore >= maxScore)
					{
						maxScore = fSimilScore;
						if (maxScore > 0.55)
						{
							mCompareResult[i].nFaceId = j;
							mCompareResult[i].fCompareResult = maxScore;
						}
						else
						{
							mCompareResult[i].nFaceId = -1;
							mCompareResult[i].fCompareResult = 0.0f;
						}
					}
				}
			}
		}
		if (mFTFaceResult.nFace == 0)
		{
			bCompare = false;
		}
		else
		{
			bCompare = true;
		}
		delete[] frameData;
		delete[] faceResult;
	}
}
// 进行人脸框绘制的函数
void CDemoDlg::DrawFaceRectOfInputVideo()
{
	//计算缩放比例并进行人脸框的缩放
	CRect rc;
	pInputImageWnd->GetClientRect(rc);
	CDC* pDc;
	pDc = pInputImageWnd->GetDC();
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	CRect *face = new CRect[mInputVideoFaceNum];
	float xScale = (float)rc.Width() / (float)mVideoWidth;
	float yScale = (float)rc.Height() / (float)mVideoHeight;
	for (int i = 0; i < mInputVideoFaceNum; i++)
	{
		face[i].left = (int)mInputVideoFaceRect[i].left * xScale;
		face[i].right = (int)mInputVideoFaceRect[i].right * xScale;
		face[i].top = (int)mInputVideoFaceRect[i].top * yScale;
		face[i].bottom = (int)mInputVideoFaceRect[i].bottom * yScale;
	}

	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CClientDC dc(pInputImageWnd);
	dc.SelectObject(pen);
	dc.SelectStockObject(NULL_BRUSH);
	for (int i = 0; i < mInputVideoFaceNum; i++)
	{
		dc.Rectangle(face[i].left, face[i].top, face[i].right, face[i].bottom);
		if (bCompare)// 判断是否进行了人脸匹配，如果进行人脸匹配，在人脸框中显示姓名等信息
		{
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(RGB(255, 255, 255));

			CString str;
			if (mCompareResult[i].nFaceId != -1)
			{
				str.AppendFormat(*mFaceModels[mCompareResult[i].nFaceId]->name + _T(", %.3f\n"), mCompareResult[i].fCompareResult);
			}
			if (mCompareResult[i].nAge != 0)
			{
				str.AppendFormat(_T("%d岁 "), mCompareResult[i].nAge);
			}
			if (mCompareResult[i].nGender != -1)
			{
				str.Append(mCompareResult[i].nGender ? _T("女") : _T("男"));
			}
			dc.DrawText(str, face[i], DT_TOP | DT_CENTER | DT_NOCLIP);
		}
	}
	ReleaseDC(pDc);
	delete[] face;
}
//在PIC控件中显示图片/视频信息
void CDemoDlg::ShowImage(IplImage* img, UINT ID)
{ 
	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);     
	cimg.Destroy();
	ReleaseDC(pDC);
}
//进入视频播放模式
void CDemoDlg::Play()
{
	bPlayflag = true;
	mAddToFaceLib.EnableWindow(0);
	mFRPicture.EnableWindow(0);
	mFRCamera.EnableWindow(0);
	mFRVideo.EnableWindow(0);
	mStopButton.EnableWindow(1);
	mFps = (int)cvGetCaptureProperty(m_pCapture, CV_CAP_PROP_FPS);
	if (mFps != 0)
	{
		SetTimer(1, 1000 / mFps, NULL); 
	}
	else
	{
		SetTimer(1, 0, NULL);
	}
}
// 停止视频播放
void CDemoDlg::Stop()
{
	bPlayflag = false;
	mInputVideoFaceNum = 0;
	mAddToFaceLib.EnableWindow(1);
	mFRPicture.EnableWindow(1);
	mFRCamera.EnableWindow(1);
	mFRVideo.EnableWindow(1);
	mStopButton.EnableWindow(0);
	cvReleaseCapture(&m_pCapture);
	if (mFTEngine != nullptr)
	{
		mFTEngine->uninit();
		delete mFTEngine;
		mFTEngine = nullptr;
	}
	if (mAgeEngineForVideo != nullptr)
	{
		mAgeEngineForVideo->uninit();
		delete mAgeEngineForVideo;
		mAgeEngineForVideo = nullptr;
	}
	if (mGenderEngineForVideo != nullptr)
	{
		mGenderEngineForVideo->uninit();
		delete mGenderEngineForVideo;
		mGenderEngineForVideo = nullptr;
	}
	ReleaseMutex(hMutex);
	WaitForSingleObject(mFRThread, INFINITE);
	KillTimer(1);
}
// 回车键刷新数据
BOOL CDemoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		Invalidate(FALSE);
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CDemoDlg::OnBnClickedFrcamera()
{
	// TODO: Add your control notification handler code here
	if (!bPlayflag)
	{
		// 打开摄像头
		if (!(m_pCapture = cvCreateCameraCapture(0)))
		{
			MessageBox(_T("打开摄像头失败!"));
			return;
		}
		// 初始化FT引擎
		int ret = 0;
		if (mFTEngine == nullptr)
		{
			mFTEngine = new FTEngine();
			ret = mFTEngine->init();
			if (ret != MOK)
			{
				CString str;
				str.Format(_T("初始化FT引擎失败,错误码: %d, 程序将关闭!"), ret);
				MessageBox(str);
				DoClose();
				exit(0);
			}
		}
		// 初始化AgeForVideo引擎
		if (mAgeEngineForVideo == nullptr)
		{
			mAgeEngineForVideo = new AgeEngine();
			ret = mAgeEngineForVideo->init();
			if (ret != MOK)
			{
				CString str;
				str.Format(_T("初始化AgeForVideo引擎失败,错误码: %d, 程序将关闭!"), ret);
				MessageBox(str);
				DoClose();
				exit(0);
			}
		}
		// 初始化GenderForVideo引擎
		if (mGenderEngineForVideo == nullptr)
		{
			mGenderEngineForVideo = new GenderEngine();
			ret = mGenderEngineForVideo->init();
			if (ret != MOK)
			{
				CString str;
				str.Format(_T("初始化AgeForVideo引擎失败,错误码: %d, 程序将关闭!"), ret);
				MessageBox(str);
				DoClose();
				exit(0);
			}
		}
		// 获取视频帧数据
		WaitForSingleObject(hMutex, INFINITE);
		pFrame = cvQueryFrame(m_pCapture);
		mVideoWidth = pFrame->width;
		mVideoHeight = pFrame->height;
		ReleaseMutex(hMutex);
		bPicture = false;
		Play();
		//创建FR线程
		mFRThread = CreateThread(NULL, 0, FRThread, this, 0, NULL);
	}
}

void CDemoDlg::OnBnClickedStop()
{
	// TODO: Add your control notification handler code here
	if (bPlayflag)
	{
		Stop();
	}
}
