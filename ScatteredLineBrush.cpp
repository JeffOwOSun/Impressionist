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

	int width = pDoc->getWidth();
	glLineWidth((float)width);

	//get the size of the brush and save it
	lineLength = pDoc->getSize();

	//get the angle of the brush and save it
	lineAngle = pDoc->getAngle();

	//calculate the x and y projection of line length
	lineXProj = (int)lineLength * cos(((double)lineAngle) * PI / 360);
	lineYProj = (int)lineLength * sin(((double)lineAngle) * PI / 360);

	prevPoint = Point(target);

	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc == NULL) {
		printf("ScatteredLineBrush::BrushMove document is NULL \n");
		return;
	}

	ImpressionistUI* dlg = pDoc->m_pUI;

	int strokeDirection = pDoc->getStrokeDirectionType();
	switch (strokeDirection)
	{
		case DIR_SLIDER_OR_RIGHT_MOUSE:
			break;
		case DIR_BRUSH_DIRECTION:
		{
			int diffX = target.x - prevPoint.x;
			int diffY = target.y - prevPoint.y;
			//cout << atan2(diffY, diffX) << endl;
			lineAngle = (int)(atan2(diffY, diffX) / PI * 360); //rotate 90 degree counter clockwise
			// cout << lineAngle << endl;
			lineXProj = (int)lineLength * cos(((double)lineAngle) * PI / 360);
			lineYProj = (int)lineLength * sin(((double)lineAngle) * PI / 360);
			prevPoint.x = target.x;
			prevPoint.y = target.y;
			break;
		}
		case DIR_GRADIENT:
		{
			//get the gradient at given source pixel
			GLint gradientX = pDoc->GetGradientX(source);
			GLint gradientY = pDoc->GetGradientY(source);
			//calculate line Angle and projection
			lineAngle = (int)(atan2(gradientX, -gradientY) / PI * 360); //rotate 90 degree counter clockwise
			lineXProj = (int)lineLength * cos(((double)lineAngle) * PI / 360);
			lineYProj = (int)lineLength * sin(((double)lineAngle) * PI / 360);
			break;
		}
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
