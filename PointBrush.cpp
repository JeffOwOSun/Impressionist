//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "pointbrush.h"

#include <iostream>
using namespace std;

extern float frand();

PointBrush::PointBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void PointBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;
	
	int size = pDoc->getSize();

	glPointSize( (float)size );

	BrushMove( source, target );
}

void PointBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}
	cout << "aaaaaaaaaaaa" << target.x << " " << target.y << endl;
	// Start work on Blur filter
	double tempBuffer[49][3] = { { 0 } };
	// Currently assume not out of bound
	int startX = target.x - 3;
	int startY = target.y + 3;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			int currentX = startX + i, currentY = startY - j;
			for (int channel = 0; channel < 3; channel++) {
				tempBuffer[i * 7 + j][channel] += applyFilterRGB((GLdouble*)&Gaussian, 3, 3, currentX, currentY, channel);
			}
		}
	}
	glBegin(GL_POINTS);
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			glColor3ub((GLubyte)tempBuffer[i * 7 + j][0], (GLubyte)tempBuffer[i * 7 + j][1], (GLubyte)tempBuffer[i * 7 + j][2]);
			glVertex2d(startX + i, startY - j);
		}
	}
	glEnd();



	// end work

	/*glBegin( GL_POINTS );
		SetColor( source );

		glVertex2d( target.x, target.y );

	glEnd();*/
}

void PointBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

double PointBrush::applyFilterRGB(GLdouble* filter, int filterW, int filterH, int x, int y, int channel)
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