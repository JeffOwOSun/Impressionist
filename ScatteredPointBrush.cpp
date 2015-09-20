//
// ScatteredPointBrush.cpp
//
// The implementation of Scattered Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include <cstdlib>
#include <ctime>
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointBrush.h"

extern float frand();

ScatteredPointBrush::ScatteredPointBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{}

void ScatteredPointBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	//set the point size to 1
	glPointSize((float)1);

	srand((unsigned int)time(0));
	BrushMove( source, target );
}

void ScatteredPointBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "ScatteredPointBrush::BrushMove  document is NULL\n" );
		return;
	}
	//get the size of the scattered area
	int size = pDoc->getSize();
	//# of points
	int num_points = size;

	for (int i = 0; i < num_points; ++i) {
		int x = rand() % size - size / 2;
		int y = rand() % size - size / 2;
		glBegin(GL_POINTS);
		Point paintPoint = Point((target.x + x), (target.y + y));
		SetColor(paintPoint);
		glVertex2d(paintPoint.x, paintPoint.y);
		glEnd();
	}
}

void ScatteredPointBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}
