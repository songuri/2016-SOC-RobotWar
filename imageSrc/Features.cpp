#include "stdafx.h"
#include "Features.h"
#include "MyImageFunc.h"
#include <math.h>

#define MAX 30 // 추적 알고리즘 시 확인할 범위
int RANGE = 25; // 사각형이 그려질 크기를 나타낼 변수
bool OK = false; // Tracking의 유무를 판단할 변수
int arr[9][2] = { { 0, -MAX }, { -MAX, -MAX }, { -MAX, 0 }, { -MAX, +MAX }, { 0, +MAX }, { +MAX, +MAX }, { +MAX, 0 }, { +MAX, -MAX }, { 0, 0 } }; // 추적 알고리즘 확인 방향

//사각형 그리기
void drawRectangle(const CByteImage& imagein, int x, int y, int xx, int yy, int color) {

   ASSERT(imagein.GetChannel() == 3);

   int nWidth = imagein.GetWidth();
   int nHeight = imagein.GetHeight();

   for (int i = 0; i < nWidth; i++) {

      BYTE *mask = imagein.GetPtr(i);
      if (color == 1){
         for (int j = 0; j < nHeight; j++) {

            if (xx >= nWidth - 5)
               xx = nWidth - 10;

            if (yy >= nHeight - 5)
               yy = nHeight - 10;

            if ((((i == x) || (i == xx)) && ((j >= y) && (j <= yy))) || (((j == y) || (j == yy)) && ((i >= x) && (i <= xx)))) {

               imagein.GetAt(i, j, 0) = 0;
               imagein.GetAt(i, j, 1) = 0;
               imagein.GetAt(i, j, 2) = 255;
            }
         }
      }
      else if (color == 2){
         for (int j = 0; j < nHeight; j++) {

            if (xx >= nWidth - 5)
               xx = nWidth - 10;

            if (yy >= nHeight - 5)
               yy = nHeight - 10;

            if ((((i == x) || (i == xx)) && ((j >= y) && (j <= yy))) || (((j == y) || (j == yy)) && ((i >= x) && (i <= xx)))) {

               imagein.GetAt(i, j, 0) = 0;
               imagein.GetAt(i, j, 1) = 255;
               imagein.GetAt(i, j, 2) = 255;
            }
         }

      }
      else if (color == 3){
         for (int j = 0; j < nHeight; j++) {

            if (xx >= nWidth - 5)
               xx = nWidth - 10;

            if (yy >= nHeight - 5)
               yy = nHeight - 10;

            if ((((i == x) || (i == xx)) && ((j >= y) && (j <= yy))) || (((j == y) || (j == yy)) && ((i >= x) && (i <= xx)))) {

               imagein.GetAt(i, j, 0) = 0;
               imagein.GetAt(i, j, 1) = 255;
               imagein.GetAt(i, j, 2) = 0;
            }
         }

      }
      else if (color == 4){
         for (int j = 0; j < nHeight; j++) {

            if (xx >= nWidth - 5)
               xx = nWidth - 10;

            if (yy >= nHeight - 5)
               yy = nHeight - 10;

            if ((((i == x) || (i == xx)) && ((j >= y) && (j <= yy))) || (((j == y) || (j == yy)) && ((i >= x) && (i <= xx)))) {

               imagein.GetAt(i, j, 0) = 255;
               imagein.GetAt(i, j, 1) = 0;
               imagein.GetAt(i, j, 2) = 0;
            }
         }

      }
	  else{ //color가 0일경우 리턴시켜버리기
		  return;
	  }
   }
}

void DrawLine(CByteImage& canvas, int x1, int y1, int x2, int y2, BYTE R, BYTE G, BYTE B)
{
   ASSERT(canvas.GetChannel()==3);

   int xs, ys, xe, ye;
   if (x1 == x2) // 수직선
   {
      if (y1 < y2) {ys = y1; ye = y2;}
      else       {ys = y2; ye = y1;}
      for (int r=ys ; r<=ye ; r++)
      {
         canvas.GetAt(x1, r, 0) = B;
         canvas.GetAt(x1, r, 1) = G;
         canvas.GetAt(x1, r, 2) = R;
      }
      return;
   }

   double a = (double)(y2-y1)/(x2-x1); // 기울기
   int nHeight = canvas.GetHeight();

   if ((a>-1) && (a<1)) // 가로축에 가까움
   {
      if (x1 < x2) {xs = x1; xe = x2; ys = y1; ye = y2;}
      else       {xs = x2; xe = x1; ys = y2; ye = y1;}
      for (int c=xs ; c<=xe ; c++)
      {
         int r = (int)(a*(c-xs) + ys + 0.5);
         if (r<0 || r>=nHeight)
            continue;
         canvas.GetAt(c, r, 0) = B;
         canvas.GetAt(c, r, 1) = G;
         canvas.GetAt(c, r, 2) = R;
      }
   }
   else // 세로축에 가까움
   {
      double invA = 1.0 / a;
      if (y1 < y2) {ys = y1; ye = y2; xs = x1; xe = x2; }
      else       {ys = y2; ye = y1; xs = x2; xe = x1; }
      for (int r=ys ; r<=ye ; r++)
      {
         int c = (int)(invA*(r-ys) + xs + 0.5);
         if (r<0 || r>=nHeight)
            continue;
         canvas.GetAt(c, r, 0) = B;
         canvas.GetAt(c, r, 1) = G;
         canvas.GetAt(c, r, 2) = R;
      }
   }
}

CDoubleImage _Gaussian5x5(const CIntImage& imageIn)
{
   int nWidth   = imageIn.GetWidth();
   int nHeight = imageIn.GetHeight();
   int nWStep = imageIn.GetWStep();

   CDoubleImage imageGss(nWidth, nHeight);
   imageGss.SetConstValue(0);
   int nWStepG = imageGss.GetWStep();

   int nWStep2 = 2*nWStep;

   for (int r=2 ; r<nHeight-2 ; r++)
   {
      double* pGss = imageGss.GetPtr(r);
      int*    pIn  = imageIn.GetPtr(r);
      for (int c=2 ; c<nWidth-2 ; c++)
      {
         pGss[c] = (2*(pIn[c-nWStep2-2] + pIn[c-nWStep2+2] + pIn[c+nWStep2-2] + pIn[c+nWStep2+2]) + 
                 4*(pIn[c-nWStep2-1] + pIn[c-nWStep2+1] + pIn[c-nWStep-2] + pIn[c-nWStep+2] +
                   pIn[c+nWStep-2] + pIn[c+nWStep+2] + pIn[c+nWStep2-1] + pIn[c+nWStep2+1]) + 
                 5*(pIn[c-nWStep2] + pIn[c-2] + pIn[c+2] + pIn[c+nWStep2]) + 
                 9*(pIn[c-nWStep-1] + pIn[c-nWStep+1] + pIn[c+nWStep-1] + pIn[c+nWStep+1]) + 
                 12*(pIn[c-nWStep] + pIn[c+nWStep] + pIn[c-1] + pIn[c+1]) + 
                 15*(pIn[c]))/159.0;
      }
   }
   
   return imageGss;
}

CByteImage CannyEdge(const CByteImage& imageIn, CByteImage& imageOut, int nThresholdHi, int nThresholdLo)
{
   int nWidth = imageIn.GetWidth();
   int nHeight = imageIn.GetHeight();
   int nWStep = imageIn.GetWStep();

   // 가우시안 마스크
   CDoubleImage  imageGss(nWidth, nHeight);
   imageGss = _Gaussian5x5(imageIn);

   // 소벨 마스크
   int Gx[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 2 };
   int Gy[9] = { 1, 1, 1, 0, 0, 0, -1, -1, -1 };

   CDoubleImage  imageMag(nWidth, nHeight);
   CByteImage imageAng(nWidth, nHeight);
   int nWStepG = imageGss.GetWStep();
   int nWStepA = imageAng.GetWStep();

   double* pGss = imageGss.GetPtr();

   // 미분 구하기
   for (int r=0 ; r<nHeight ; r++)
   {
   double* pMag = imageMag.GetPtr(r);
   BYTE*   pAng = imageAng.GetPtr(r);
   for (int c=0 ; c<nWidth ; c++)
   {
      double sumX = 0.0;
      double sumY = 0.0;
      for (int y=0 ; y<3 ; y++)
      {
         for (int x=0 ; x<3 ; x++)
         {
            int py = r-1+y;
            int px = c-1+x;
            if (px>=0 && px<nWidth && py>=0 && py<nHeight)
            {
               sumX += Gx[y*3+x]*pGss[py*nWStepG+px];
               sumY += Gy[y*3+x]*pGss[py*nWStepG+px];
            }
         }
      }

      pMag[c] = sqrt(sumX*sumX + sumY*sumY); // 경계선의 세기
      double theta;                // 경계선의 수직 방향
      if (pMag[c] == 0)
      {
         if(sumY == 0)
         {
            theta = 0;
         }
         else
         {
            theta = 90;
         }
      }
      else
      {
         theta = atan2((float)sumY, (float)sumX)*180.0/M_PI;
      }

      if ((theta > -22.5 && theta < 22.5) || theta > 157.5 || theta < -157.5)
      {
         pAng[c] = 0;
      }
      else if ((theta >= 22.5 && theta < 67.5) || (theta >= -157.5 && theta < -112.5))
      {
         pAng[c] = 45;
      }
      else if ((theta >= 67.5 && theta <= 112.5) || (theta >= -112.5 && theta <= -67.5))
      {
         pAng[c] = 90;
      }
      else
      {
         pAng[c] = 135;
      }
   } // 열 이동 끝
   } // 행 이동 끝

   // 비 최대값 억제
   CByteImage imageCand(nWidth, nHeight);
   imageCand.SetConstValue(0);

   for (int r=1 ; r<nHeight-1 ; r++)
   {
   BYTE*   pCand = imageCand.GetPtr(r);
   double* pMag  = imageMag.GetPtr(r);
   BYTE*   pAng  = imageAng.GetPtr(r);
   for (int c=1 ; c<nWidth-1 ; c++)
   {
      switch (pAng[c])
      {
      case 0:      // 0도 방향 비교
         if (pMag[c] > pMag[c-1] && pMag[c] > pMag[c+1])
         {
            pCand[c] = 255;
         }
         break;
      case 45:   // 45도 방향 비교
         if (pMag[c] > pMag[c-nWStepG+1] && pMag[c] > pMag[c+nWStepG-1])
         {
            pCand[c] = 255;
         }
         break;
      case 90:      // 90도 방향 비교
         if (pMag[c] > pMag[c-nWStepG] && pMag[c] > pMag[c+nWStepG])
         {
            pCand[c] = 255;
         }
         break;
      case 135:   // 135도 방향 비교
         if (pMag[c] > pMag[c-nWStepG-1] && pMag[c] > pMag[c+nWStepG+1])
         {
            pCand[c] = 255;
         }
         break;
      }
   } // 열 이동 끝
   } // 행 이동 끝

   imageCand.SaveImage("Cand.bmp");
   
   // 문턱값 검사
   imageOut = CByteImage(nWidth, nHeight);
   imageOut.SetConstValue(0);
   for (int r=1 ; r<nHeight-1 ; r++)
   {
   BYTE*   pOut  = imageOut.GetPtr(r);
   BYTE*   pCand = imageCand.GetPtr(r);
   double* pMag  = imageMag.GetPtr(r);
   BYTE*   pAng  = imageAng.GetPtr(r);
   for (int c=1 ; c<nWidth-1 ; c++)
   {
      if (pCand[c])
      {
         if (pMag[c] > nThresholdHi)
         {
            pOut[c] = 255;
         }
         else if (pMag[c] > nThresholdLo) // 연결된 픽셀 검사
         {
            bool bIsEdge = true;
            switch (pAng[c])
            {
            case 0:      // 90도 방향 검사
               if ((pMag[c-nWStepG] > nThresholdHi) || 
                  (pMag[c+nWStepG] > nThresholdHi))
               {
                  pOut[c - 1] = 255;
                  pOut[c] = 255;
                  pOut[c + 1] = 255;
               }
               break;
            case 45:   // 135도 방향 검사
               if ((pMag[c-nWStepG-1] > nThresholdHi) || 
                  (pMag[c+nWStepG+1] > nThresholdHi))
               {
                  pOut[c - 1] = 255;
                  pOut[c] = 255;
                  pOut[c + 1] = 255;
               }
               break;
            case 90:      // 0도 방향 검사
               if ((pMag[c-1] > nThresholdHi) || 
                  (pMag[c+1] > nThresholdHi))
               {
                  pOut[c - 1] = 255;
                  pOut[c] = 255;
                  pOut[c + 1] = 255;
               }
               break;
            case 135:   // 45도 방향 검사
               if ((pMag[c-nWStepG+1] > nThresholdHi) || 
                  (pMag[c+nWStepG-1] > nThresholdHi))
               {
                  pOut[c - 1] = 255;
                  pOut[c] = 255;
                  pOut[c + 1] = 255;
               }
               break;
            }
         }
      }
   } // 열 이동 끝
   } // 행 이동 끝

   return imageCand;
}

int HarrisCorner(const CByteImage& imageIn,  double dThreshold, double paramK, int nMaxCorner, double* posX, double* posY)
{
   int nWidth   = imageIn.GetWidth();
   int nHeight = imageIn.GetHeight();
   int nWStep = imageIn.GetWStep();

   CIntImage imageDxx(nWidth, nHeight); imageDxx.SetConstValue(0);
   CIntImage imageDxy(nWidth, nHeight); imageDxy.SetConstValue(0);
   CIntImage imageDyy(nWidth, nHeight); imageDyy.SetConstValue(0);
   int nWStepD = imageDxx.GetWStep();

   // 미분 계산
   int dx, dy;

   for (int r=1 ; r<nHeight-1 ; r++)
   {
      BYTE* pIn = imageIn.GetPtr(r);
      int* pDxx = imageDxx.GetPtr(r);
      int* pDxy = imageDxy.GetPtr(r);
      int* pDyy = imageDyy.GetPtr(r);
      for (int c=1 ; c<nWidth-1 ; c++)
      {
         dx = pIn[c-nWStep-1] + 2*pIn[c-1] + pIn[c+nWStep-1] 
            - pIn[c-nWStep+1] - 2*pIn[c+1] - pIn[c+nWStep+1];
         dy = pIn[c-nWStep-1] + 2*pIn[c-nWStep] + pIn[c-nWStep+1] 
            - pIn[c+nWStep-1] - 2*pIn[c+nWStep] - pIn[c+nWStep+1];
         pDxx[c] = dx*dx;
         pDxy[c] = dx*dy;
         pDyy[c] = dy*dy;
      }
   }

   // 가우시안 필터링
   CDoubleImage imageGDxx = _Gaussian5x5(imageDxx);
   CDoubleImage imageGDxy = _Gaussian5x5(imageDxy);
   CDoubleImage imageGDyy = _Gaussian5x5(imageDyy);

   // 코너 응답 함수
   CDoubleImage imageCornerScore(nWidth, nHeight);

   for (int r=2 ; r<nHeight-2 ; r++)
   {
      double *pScore = imageCornerScore.GetPtr(r);
      double* pGDxx = imageGDxx.GetPtr(r);
      double* pGDxy = imageGDxy.GetPtr(r);
      double* pGDyy = imageGDyy.GetPtr(r);
      for (int c=2 ; c<nWidth-2 ; c++)
      {
         pScore[c] = (pGDxx[c]*pGDyy[c] - pGDxy[c]*pGDxy[c]) 
            - paramK*(pGDxx[c]+pGDyy[c])*(pGDxx[c]+pGDyy[c]);
      }
   }

   // 지역 최대값 추출
   int numCorner = 0;
   for (int r=2 ; r<nHeight-2 ; r++)
   {
      double *pScore = imageCornerScore.GetPtr(r);
      for (int c=2 ; c<nWidth-2 ; c++)
      {
         if (pScore[c] > dThreshold)
         {
            if (pScore[c] > pScore[c-nWStepD] &&
               pScore[c] > pScore[c+nWStepD] &&
               pScore[c] > pScore[c-1] &&
               pScore[c] > pScore[c+1])
            {
               posX[numCorner] = c;
               posY[numCorner] = r;
               numCorner++;

               if (numCorner >= nMaxCorner)
                  return nMaxCorner;
            }
         }
      }
   }

   return numCorner;
}

//방향 예측 
void Drawpredict(CByteImage& imagein, int(&arrx)[5], int(&arry)[5]) {
   
   ASSERT(imagein.GetChannel() == 3);
   
   float a, b;

   int nWidth = imagein.GetWidth();
   int nHeight = imagein.GetHeight();

   int sum_x = 0, sum_xy = 0, sum_x2 = 0, sum_y = 0;

   for (int i = 0; i < 10; i++){
      sum_x += arrx[i];
      sum_y += arry[i];
      sum_xy += arrx[i] * arry[i];
      sum_x2 += pow(arrx[i], 2.0);
   }
   // 최소제곱법
   b = (5 * sum_xy - sum_x*sum_y) / (5 * sum_x2 - pow(sum_x, 2.0));
   a = (sum_y - b*sum_x) / 5;

   for(int i = 0; i < 4; i++) {
      int temp;

      for(int j = i; j < 4; j++) {

         if(arrx[j] > arrx[j + 1]) {
            temp = arrx[j];
            arrx[j] = arrx[j + 1];
            arrx[j + 1] = temp;
            temp = arry[j];
            arry[j] = arry[j + 1];
            arry[j + 1] = temp;
         }
      }
   }

   if(arry[0] < 0)
      arry[0] = 0;
   if(arry[4] < 0)
      arry[4] = 0;

   if(arry[0] >= nHeight - 2)
      arry[0] = nHeight - 3;
   if(arry[4] >= nHeight - 2)
      arry[4] = nHeight - 3;

   if(arrx[4] < 0)
      arrx[4] = 0;
   if(arrx[0] < 0)
      arrx[0] = 0;

   if(arrx[4] >= nWidth)
      arrx[4] = nWidth - 1;
   if(arrx[0] >= nWidth)
      arrx[0] = nWidth - 1;

   DrawLine(imagein, arrx[0], arry[0], arrx[4], arry[4], 255, 255, 255);
   DrawLine(imagein, arrx[0], arry[0] + 1, arrx[4], arry[4] + 1, 255,255, 255);
   DrawLine(imagein, arrx[0], arry[0] + 2, arrx[4], arry[4] + 2, 255, 255, 255);
}

// 사각형 중심 추정
void FindCenter(double(&posX)[50], double(&posY)[50], double &centerX, double &centerY){

	int tempSize = 50;


	for (int i = 0; i < 50; i++){

		if (posX[i] == 0 || posY[i] == 0) {
			tempSize--;
		}
		else {
			centerX += posX[i];
			centerY += posY[i];
		}
	}
	centerX /= tempSize;
	centerY /= tempSize;

}


int pixelCount(CByteImage& imageIn, int x, int y) {

   int count = 0;
   int xR = x + RANGE;
   int yR = y + RANGE;
   if (xR > 640)
	   xR = 639;
   if (yR > 480)
	   yR = 479;

   //인설진 시간에 추가한 코드
   for (int i = y; i < yR; i++) {

      BYTE *temp = imageIn.GetPtr(i); //NULL값들어갈수도 있다.
									//이게 	
      for (int j = x; j < xR; j++) {
		  //if (temp != NULL)
         int value = temp[j];

         if(value == 255)
            count++;
      }
   }

   return count;
}
void Tracking(int &x, int &y , int &xx, int &yy, int average_x , int average_y, CByteImage &result)
{
	
		   if (!OK) {
			   x = average_x - RANGE;
			   y = average_y - RANGE;
			   xx = x + RANGE;
			   yy = y + RANGE;
			   OK = true;
		   }

		   int tempX, tempY;
		   int temp = 0;
		   int Tcount = 0;

		   for (int i = 0; i <= 8; i++) {
			   int XX = x + arr[i][0];
			   int YY = y + arr[i][1];
			   
			   int count = pixelCount(result, XX, YY);//이함수에서 오류가 자꾸떠
			   Tcount = count;
			   count /= 7; //기현이 마음대로 한거 ㅋ

			   if (count >= temp) {
				   temp = count;
				   tempX = arr[i][0];
				   tempY = arr[i][1];
			   }
		   }
		   if (Tcount > RANGE * RANGE * 0.9)
			   RANGE += 9;
		   else if (Tcount < RANGE * RANGE * 0.7 && RANGE > 10)
			   RANGE -= 9;

		   if (x + tempX > 0 && y + tempY > 0 && x + tempX + RANGE < 625 && y + tempY + RANGE < 460){
			   x += tempX;
			   y += tempY;

			   xx = x + RANGE;
			   yy = y + RANGE;
		   }
}