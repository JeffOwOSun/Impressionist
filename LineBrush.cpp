//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"

extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name), lineLength(10), lineAngle(0), lineXProj(10), lineYProj(0)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
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
	glLineWidth((float)width);

	//get the size of the brush and save it
	lineLength = pDoc->getSize();

	//get the angle of the brush and save it
	lineAngle = pDoc->getAngle();

	//calculate the x and y projection of line length
	lineXProj = (int) lineLength * cos(lineAngle);
	lineYProj = (int) lineLength * sin(lineAngle);
	
	BrushMove(source, target);

}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove document is NULL \n");
		return;
	}

	//calculate the start and end point of the line to draw
	int x1 = target.x - lineXProj / 2;
	int y1 = target.y - lineYProj / 2;
	int x2 = target.x + lineXProj / 2;
	int y2 = target.y + lineYProj / 2;

	SetColor(source);
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);

	GLenum error_flag;
	error_flag = glGetError();
	if (error_flag != GL_NO_ERROR) {
		printf("Error: %1s (%i) in %1s.\n", gluErrorString(error_flag), error_flag, "method name");
	}
	glEnd();
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}