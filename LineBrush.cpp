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
ImpBrush(pDoc, name)
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
	printf("%d\n", width);
	glLineWidth((float)width);
	
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

	int lineLength = pDoc->getSize();

	SetColor(source);
	glBegin(GL_LINES);
	glVertex2d(target.x - lineLength, target.y);
	glVertex2d(target.x + lineLength, target.y);

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