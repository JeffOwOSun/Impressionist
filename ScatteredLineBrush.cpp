//
// ScatteredLineBrush.cpp
//
//

#include "ImpressionistDoc.h"
#include "ImpressionistUI.h"
#include "ScatteredLineBrush.h"
#include <math.h>

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name), lineLength(10), lineAngle(0), lineXProj(10), lineYProj(0)
{

}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	// The following lines is a pseudo implementation.
	// We should control the width of line brush here and the
	// control panel should called "line width".
	// Which means the actual implementation should be like
	/*
	int width = pDoc->getLineWidth();
	glLineWidth((float)size);
	*/
	int width = pDoc->getWidth();
	printf("%d\n", width);
	glLineWidth((float)width);

	//get the size of the brush and save it
	lineLength = pDoc->getSize();

	//get the angle of the brush and save it
	lineAngle = pDoc->getAngle();

	//calculate the x and y projection of line length
	lineXProj = (int)lineLength * cos(((double)lineAngle) * PI / 360);
	lineYProj = (int)lineLength * sin(((double)lineAngle) * PI / 360);

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
	
	int upperX = target.x + lineXProj / 2;
	int lowerX = target.x - lineXProj / 2;
	int upperY = target.y + lineYProj / 2;
	int lowerY = target.y - lineYProj / 2;


	for (int i = 0; i < 4; i++) {
		glBegin(GL_LINES);

		int offsetX = rand() % (2 * lineLength) - lineLength;
		int offsetY = rand() % (2 * lineLength) - lineLength;
		SetColor(Point(source.x + offsetX, source.y + offsetY));
		glVertex2d(upperX + offsetX, upperY + offsetY);
		glVertex2d(lowerX + offsetX, lowerY + offsetY);

		glEnd();
	}
	
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
