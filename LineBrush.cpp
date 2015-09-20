//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"
#include <iostream>
using namespace std;

extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name), lineLength(10), lineAngle(0), lineXProj(10), lineYProj(0)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
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

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove document is NULL \n");
		return;
	}

	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove document is NULL \n");
		return;
	}

	//set stroke direction
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
			printf("gradientX: %d gradientY: %d lineAngle: %d\n", gradientX, gradientY, lineAngle);
			lineXProj = (int)lineLength * cos(((double)lineAngle) * PI / 360);
			lineYProj = (int)lineLength * sin(((double)lineAngle) * PI / 360);
			break;
		}
		case DIR_ANOTHER_GRADIENT:
		{
			//get the gradient at given source pixel
			GLint gradientX = pDoc->GetReferenceGradientX(source);
			GLint gradientY = pDoc->GetReferenceGradientY(source);
			//calculate line Angle and projection
			lineAngle = (int)(atan2(gradientX, -gradientY) / PI * 360); //rotate 90 degree counter clockwise
			printf("gradientX: %d gradientY: %d lineAngle: %d\n", gradientX, gradientY, lineAngle);
			lineXProj = (int)lineLength * cos(((double)lineAngle) * PI / 360);
			lineYProj = (int)lineLength * sin(((double)lineAngle) * PI / 360);
			break;
		}
	}

	//calculate the start and end point of the line to draw
	int x1 = target.x - lineXProj / 2;
	int y1 = target.y - lineYProj / 2;
	int x2 = target.x + lineXProj / 2;
	int y2 = target.y + lineYProj / 2;

	//do edge clipping
	if (pDoc->getEdgeClipping())
	{
		double cos_val = cos(((double)lineAngle) * PI / 360);
		double sin_val = sin(((double)lineAngle) * PI / 360);
		//sprintf("Edge clipping!\n");
		for (int distance = lineLength / 2; distance > 0; --distance)
		{
			//calculate the pixel
			int lineX = (int)distance * (int)cos_val;
			int lineY = (int)distance * (int)sin_val;
			//tell if the pixel is an edge
			int xx1 = target.x - lineX;
			int yy1 = target.y - lineY;
			int xx2 = target.x + lineX;
			int yy2 = target.y + lineY;
			if (pDoc->GetEdge(xx1, yy1))
			{
				//printf("updating point1! %d\n", pDoc->GetGradientMod(xx1, yy1));
				x1 = xx1;
				y1 = yy1;
			}
			if (pDoc->GetEdge(xx2, yy2))
			{
				//printf("updating point2! %d\n", pDoc->GetGradientMod(xx2, yy2));
				x2 = xx2;
				y2 = yy2;
			}
		}
	}
	
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