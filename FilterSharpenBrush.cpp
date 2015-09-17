

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "FilterSharpenBrush.h"

FilterSharpenBrush::FilterSharpenBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void FilterSharpenBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	BrushMove(source, target);
}

void FilterSharpenBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	// Use 7*7 as the blurred region
	double tempBuffer[9][3] = { { 0 } };
	// Therefore start from x-3, y+3
	int startX = target.x - 1;
	int startY = target.y + 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int channel = 0; channel < 3; channel++) {
				tempBuffer[i * 3 + j][channel] += applySharpen((GLdouble*)&Sharpen, 3, 3, startX + i, startY - j, channel);
			}
		}
	}
	glBegin(GL_POINTS);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			glColor3ub((GLubyte)tempBuffer[i * 3 + j][0], (GLubyte)tempBuffer[i * 3 + j][1], (GLubyte)tempBuffer[i * 3 + j][2]);
			glVertex2d(startX + i, startY - j);
		}
	}
	glEnd();
}

void FilterSharpenBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

double FilterSharpenBrush::applySharpen(GLdouble* filter, int filterW, int filterH, int x, int y, int channel)
{
	double res = 0.0;
	int cnt = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			res += (double)*(GetDocument()->GetOriginalPixel(x + i, y + j) + channel)*filter[cnt++];
		}
	}
	if (res > 255) res = 255;
	if (res < 0) res = 0;
	return res;
}