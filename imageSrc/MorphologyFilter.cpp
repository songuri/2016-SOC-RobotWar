#include "stdafx.h"
#include "MorphologyFilter.h"

void DilateG(const CByteImage& imageIn, const CByteImage& mask, CByteImage& imageOut)
{
	// �Է� ���� �� ����ũ ������ ũ�� ����
	int nWidth  = imageIn.GetWidth();
	int nHeight = imageIn.GetHeight();
	int nMaskW = mask.GetWidth();
	int nMaskH = mask.GetHeight();
	int hW = nMaskW/2;
	int hH = nMaskH/2;

	for (int r=hH ; r<nHeight ; r++) // �Է� ������ ���� ����
	{
		BYTE* pIn  = imageIn .GetPtr(r);
		BYTE* pOut = imageOut.GetPtr(r);

		for (int c=hW ; c<nWidth ; c++) // �Է� ������ ���� ����
		{
			int maxVal = 0;
			for (int y=0 ; y<nMaskH ; y++) // ����ũ�� ���� ����
			{
				BYTE* pMask = mask.GetPtr(y);
				BYTE* pInM = imageIn.GetPtr(r-hH+y, c-hW); // �Է� ���󿡼� ����ũ ���� �� ��

				for (int x=0 ; x<nMaskW ; x++) // ����ũ�� ���� ����
				{
					int sum = pInM[x] + pMask[x];
					if (sum > maxVal) // �ִ밪 Ž�� 
					{
						maxVal = sum;
					}
				}
			}

			pOut[c] = CLIP(maxVal); // ��� ���� [0, 255] ������ ������ ����
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