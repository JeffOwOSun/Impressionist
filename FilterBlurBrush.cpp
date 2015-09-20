

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "FilterBlurBrush.h"


FilterBlurBrush::FilterBlurBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void FilterBlurBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	BrushMove(source, target);
}

void FilterBlurBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	// Use 7*7 as the blurred region
	double tempBuffer[25][3] = { { 0 } };
	// Therefore start from x-3, y+3
	int startX = target.x - 2;
	int startY = target.y + 2;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			for (int channel = 0; channel < 3; channel++) {
				tempBuffer[i * 5 + j][channel] += applyBlur((GLdouble*)&Gaussian, 3, 3, startX + i, startY - j, channel);
			}
		}
	}
	glBegin(GL_POINTS);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			glColor3ub((GLubyte)tempBuffer[i * 5 + j][0], (GLubyte)tempBuffer[i * 5 + j][1], (GLubyte)tempBuffer[i * 5 + j][2]);
			glVertex2d(startX + i, startY - j);
		}
	}
	glEnd();
}

void FilterBlurBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

double FilterBlurBrush::applyBlur(GLdouble* filter, int filterW, int filterH, int x, int y, int channel)
{
	double res = 0.0;
	int cnt = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			res += (double)*(GetDocument()->GetOriginalPixel(x + i, y + j) + channel)*filter[cnt++];
		}
	}
	return res;
}