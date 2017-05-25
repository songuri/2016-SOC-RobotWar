#pragma once
#include "MyImage.h"

#define M_PI      3.14159265358979323846
void DrawLine(CByteImage& canvas, int x1, int y1, int x2, int y2, BYTE R, BYTE G, BYTE B);
CByteImage CannyEdge(const CByteImage& imageIn, CByteImage& imageOut, int nThresholdHi, int nThresholdLo);
int  HarrisCorner(const CByteImage& imageIn,  double dThreshold, double paramK, int nMaxCorner, double* posX, double* posY);
void drawRectangle(const CByteImage& imagein, int x, int y, int xx, int yy, int color);
void Drawpredict(CByteImage& imagein, int(&arrx)[5] , int (&arry)[5]);
void FindCenter(double(&posX)[50], double(&posY)[50], double &centerX, double &centerY);
int pixelCount(CByteImage& imageIn, int x, int y) ;
void Tracking(int &x, int &y , int &xx, int &yy, int average_x, int average_y, CByteImage &result) ;