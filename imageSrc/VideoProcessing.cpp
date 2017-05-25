#include "stdafx.h"
#include "VideoProcessing.h"

///////////////////////////////////////////////////////////////////////////////
// ī�޶� ĸ�� Ŭ���� 
///////////////////////////////////////////////////////////////////////////////
CWebCam::CWebCam() :
	m_hCam(NULL)
{

}

CWebCam::~CWebCam()
{
	if (m_hCam)
		StopCam();
}

bool CWebCam::StartCam(CWnd* wnd, int nWidth, int nHeight, BITMAPINFO& bmpInfo)
{
	// ĸ�� ������ ����� �������� ũ�� �� ��ġ ����
	m_hCam = capCreateCaptureWindow("Cam Capture", WS_CHILD|WS_VISIBLE, 
									0, 0, nWidth, nHeight, wnd->m_hWnd, NULL);

	if (!capDriverConnect(m_hCam, 0))
	{
		AfxMessageBox("����� ī�޶� ã�� �� �����ϴ�.");
		return false;
	}

//	capDlgVideoFormat(m_hCam);
	capGetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO));

	// ĸ�� ������ ũ�⸦ ��� ������ ����
	bmpInfo.bmiHeader.biWidth		= nWidth;
	bmpInfo.bmiHeader.biHeight		= nHeight;
	bmpInfo.bmiHeader.biSizeImage	= nWidth*nHeight*bmpInfo.bmiHeader.biBitCount/8;
	if (!capSetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO)))
	{
		// ������ �����ϸ� ���� ������ �ǵ���
		capGetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO));
	}

	// ĸ�� ������ 24��Ʈ RGB�� �� �� �ֵ��� ����
	bmpInfo.bmiHeader.biBitCount	= 24;
	bmpInfo.bmiHeader.biCompression	= BI_RGB;
	if (!capSetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO)))
	{
		// ������ �����ϸ� ���� ������ �ǵ���
		capGetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO));
	}

	capOverlay	  (m_hCam,TRUE);
	capPreviewRate(m_hCam, 30);
	capPreview	  (m_hCam, TRUE);
	
	return true;
}

void CWebCam::StopCam()
{
	capCaptureAbort(m_hCam);
	capDriverDisconnect(m_hCam);
	m_hCam = NULL;
}

BOOL CWebCam::StartRecord(const char* filename)
{
	capFileSetCaptureFile(m_hCam, filename);
	return capCaptureSequence(m_hCam);
}

BOOL CWebCam::StopRecord()
{
	capCaptureStop(m_hCam);

	return capPreview(m_hCam, TRUE);
}

BOOL CWebCam::SetCallBackOnFrame(LRESULT (*fpProc)(HWND, LPVIDEOHDR))
{
	return capSetCallbackOnFrame(m_hCam, fpProc);
}

///////////////////////////////////////////////////////////////////////////////
// AVI ���� ��� Ŭ����
///////////////////////////////////////////////////////////////////////////////
CAviPlayer::CAviPlayer() :
	m_pAviFile(NULL),
	m_pGetFrame(NULL),
	m_nWidth(0),
	m_nHeight(0)
{
	AVIFileInit();
}

CAviPlayer::~CAviPlayer()
{
	if (m_pAviFile)
		CloseAvi();

	AVIFileExit();
}

bool CAviPlayer::LoadAvi(const char* filename, int& nFrame, int& nFrameRate, int& nWidth, int& nHeight)
{
	PAVISTREAM		pAviStream = NULL;
	AVISTREAMINFO	aviStreamInfo;
	
	if (!AVIFileOpen(&m_pAviFile, filename, OF_READ, NULL))
	{
		if (!AVIFileGetStream(m_pAviFile, &pAviStream, streamtypeVIDEO, 0))
		{
			AVIStreamInfo(pAviStream, &aviStreamInfo, sizeof(AVISTREAMINFO));
		}
	}
	
	if (!m_pAviFile || !pAviStream)
	{
		AfxMessageBox("AVI ���� �б⿡ �����߽��ϴ�.");
		return false;
	}

	nFrame = aviStreamInfo.dwLength;
	nFrameRate = aviStreamInfo.dwRate/aviStreamInfo.dwScale;

	RECT rcFrame = aviStreamInfo.rcFrame;
	m_nWidth	= nWidth  = rcFrame.right - rcFrame.left;
	m_nHeight	= nHeight = rcFrame.bottom - rcFrame.top;

	// ������ ���� ����
	BITMAPINFOHEADER bmiHeader;
	LONG bmiSize = sizeof(BITMAPINFOHEADER);
	memset(&bmiHeader, 0, bmiSize);
	bmiHeader.biSize	= bmiSize;
	bmiHeader.biWidth	= m_nWidth;
	bmiHeader.biHeight	= -m_nHeight;
	bmiHeader.biPlanes	= 1;
	bmiHeader.biBitCount = 24;
	bmiHeader.biCompression = BI_RGB;;
	bmiHeader.biSizeImage = m_nWidth*m_nHeight*3;

	m_pGetFrame = AVIStreamGetFrameOpen(pAviStream, &bmiHeader);

	AVIStreamClose(pAviStream);

	return true;
}

void CAviPlayer::CloseAvi()
{
	if (m_pGetFrame)
	{
		AVIStreamGetFrameClose(m_pGetFrame);
		m_pGetFrame = NULL;
	}
	if (m_pAviFile)
	{
		AVIFileClose(m_pAviFile);
		m_pAviFile = NULL;
	}
}

bool CAviPlayer::ShowFrame(CWnd* wnd, int idxFrame)
{
	// ������ ������ ������
	BYTE* pData = (BYTE*)AVIStreamGetFrame(m_pGetFrame, idxFrame);

	if (!pData)
		return false;

	// ��Ʈ�� ��� ����
	LPBITMAPINFO pBmpInfo = (LPBITMAPINFO)pData;
	
	// ��Ʈ�� ȭ�� ���
	SetStretchBltMode(wnd->GetDC()->m_hDC, COLORONCOLOR);
	::StretchDIBits(
		wnd->GetDC()->m_hDC,
		0, 0, m_nWidth, m_nHeight,
		0, 0, m_nWidth, m_nHeight,
		pData+sizeof(BITMAPINFOHEADER),
		pBmpInfo,
		DIB_RGB_COLORS,
		SRCCOPY);

	return true;
}

bool CAviPlayer::GetFrame(CByteImage& image, int idxFrame)
{
	BYTE* pData = (BYTE*)AVIStreamGetFrame(m_pGetFrame, idxFrame) + sizeof(BITMAPINFOHEADER);

	if (!pData)
		return false;

	memcpy(image.GetPtr(), pData, m_nWidth*m_nHeight*3);

	return true;
}

void YUY2ToRGB24(int nWidth, int nHeight, const BYTE* pYUY2, BYTE* pRGB24)
{
	int nSize = nHeight * (nWidth/2);

	int idxSrc = 0;
	int idxDst = 0;

	BYTE y1, u, y2, v;
	BYTE r, g, b;

	for (int i=0 ; i<nSize ; i++)
	{
		y1 = pYUY2[idxSrc  ];
		u  = pYUY2[idxSrc+1];
		y2 = pYUY2[idxSrc+2];
		v  = pYUY2[idxSrc+3];

		// ù ��° �ȼ�
		b = CLIP((76284*(y1-16) + 132252*(u-128)) >> 16);
		g = CLIP((76284*(y1-16) - 53281*(v-128) -  25625*(u-128) ) >> 16);
		r = CLIP((76284*(y1-16) + 104595*(v-128)) >> 16);

		pRGB24[idxDst  ] = b;
		pRGB24[idxDst+1] = g;
		pRGB24[idxDst+2] = r;

		idxDst += 3;

		// �� ��° �ȼ�
		b = CLIP((76284*(y2-16) + 132252*(u-128)) >> 16);
		g = CLIP((76284*(y2-16) -  53281*(v-128) -  25625*(u-128)) >> 16);
		r = CLIP((76284*(y2-16) + 104595*(v-128)) >> 16);

		pRGB24[idxDst  ] = b;
		pRGB24[idxDst+1] = g;
		pRGB24[idxDst+2] = r;

		idxDst += 3;
		idxSrc += 4;
	}
}