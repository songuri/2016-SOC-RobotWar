#include "stdafx.h"
#include "MorphologyFilter.h"

void DilateG(const CByteImage& imageIn, const CByteImage& mask, CByteImage& imageOut)
{
	// 입력 영상 및 마스크 영상의 크기 정보
	int nWidth  = imageIn.GetWidth();
	int nHeight = imageIn.GetHeight();
	int nMaskW = mask.GetWidth();
	int nMaskH = mask.GetHeight();
	int hW = nMaskW/2;
	int hH = nMaskH/2;

	for (int r=hH ; r<nHeight ; r++) // 입력 영상의 세로 방향
	{
		BYTE* pIn  = imageIn .GetPtr(r);
		BYTE* pOut = imageOut.GetPtr(r);

		for (int c=hW ; c<nWidth ; c++) // 입력 영상의 가로 방향
		{
			int maxVal = 0;
			for (int y=0 ; y<nMaskH ; y++) // 마스크의 세로 방향
			{
				BYTE* pMask = mask.GetPtr(y);
				BYTE* pInM = imageIn.GetPtr(r-hH+y, c-hW); // 입력 영상에서 마스크 내의 각 행

				for (int x=0 ; x<nMaskW ; x++) // 마스크의 가로 방향
				{
					int sum = pInM[x] + pMask[x];
					if (sum > maxVal) // 최대값 탐색 
					{
						maxVal = sum;
					}
				}
			}

			pOut[c] = CLIP(maxVal); // 결과 값이 [0, 255] 구간에 오도록 조절
		}
	}
	for (int i = 0; i < hH; i++) {

		BYTE* pOut = imageOut.GetPtr(i);

		for (int j = 0; j < nWidth; j++) {

			pOut[j] = 255;

		}
	}

	for (int i = 0; i < nHeight; i++) {

		BYTE* pOut = imageOut.GetPtr(i);

		for (int j = 0; j < hW; j++) {

			pOut[j] = 255;

		}
	}
}