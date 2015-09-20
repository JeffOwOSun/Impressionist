//
// AlphaMappedBrush.cpp
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "AlphaMappedBrush.h"

extern float frand();

AlphaMappedBrush::AlphaMappedBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void AlphaMappedBrush::BrushBegin(const Point source, const Point target)
{
	BrushMove(source, target);
}

void AlphaMappedBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	if (pDoc->m_ucAlphaBrush == NULL) {
		fl_alert("No alpha brush loaded");
		return;
	}

	glBegin(GL_POINTS);
	int beginX = target.x - pDoc->m_nAlphaBrushWidth / 2;
	int beginY = target.y - pDoc->m_nAlphaBrushHeight / 2;
	for (int i = 0; i < pDoc->m_nAlphaBrushWidth; ++i)
	{
		for (int j = 0; j < pDoc->m_nAlphaBrushHeight; ++j)
		{
			//get alpha value of the brush
			int alpha = pDoc->m_ucAlphaBrush[3 * (j * pDoc->m_nAlphaBrushWidth + i)];
			//set the color
			SetColor(source, alpha);
			//draw the point
			glVertex2d(beginX + i, beginY + j);
		}
	}
	glEnd();
}

void AlphaMappedBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

