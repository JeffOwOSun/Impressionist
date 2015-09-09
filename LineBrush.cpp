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

	int size = pDoc->getSize();

	glLineWidth((float)size);
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

	glColor3f(1.0f, 0.0f, 0.5f);
	glBegin(GL_LINES);
	SetColor(source);
	glVertex2d(target.x - 10, target.y);
	glVertex2d(target.x + 10, target.y);

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