//
// ScatteredLineBrush.cpp
//
//

#include "ImpressionistDoc.h"
#include "ImpressionistUI.h"
#include "ScatteredLineBrush.h"
#include <math.h>

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{

}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();
	glLineWidth((float)1);
	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredLineBrush::BrushMove document is NULL \n");
		return;
	}

	int lineLength = pDoc->getSize();
	
	int upperX = target.x + lineLength;
	int lowerX = target.x - lineLength;
	int upperY = target.y + lineLength;
	int lowerY = target.y - lineLength;


	for (int i = 0; i < 4; i++) {
		glBegin(GL_LINES);
		int offset = rand() % (2 * lineLength);
		int offset2 = rand() % (2 * lineLength);
		Point tmp_point;
		tmp_point.x = target.x - lineLength + offset;
		tmp_point.y = target.y - lineLength + offset2;
		SetColor(tmp_point);
		printf("%d%d\n", offset, offset2);
		glVertex2d(tmp_point.x - lineLength, tmp_point.y);
		glVertex2d(tmp_point.x + lineLength, tmp_point.y);
		glEnd();
	}
	
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
