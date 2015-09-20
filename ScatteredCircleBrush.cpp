//
// ScatteredCircleBrush.cpp
//
// The implementation of Scattered Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include <cstdlib>
#include <ctime>
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCircleBrush.h"

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush( ImpressionistDoc* pDoc, char* name ) :
	CircleBrush(pDoc,name)
{}

void ScatteredCircleBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;
  //set the brush size for this stroke
  size = pDoc->getSize();
  //fix the point nubmer to be 3
  num_points = 3;
	srand((unsigned int)(time(0)));
	BrushMove( source, target );
}

void ScatteredCircleBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "ScatteredCircleBrush::BrushMove  document is NULL\n" );
		return;
	}

	for (int i = 0; i < num_points; ++i) {
		int x = rand() % size - size / 2;
		int y = rand() % size - size / 2;
		Point paintPoint = Point((target.x + x), (target.y + y));
		DrawCircle(paintPoint, paintPoint, (float)size / (float)2.0);
	}
}

void ScatteredCircleBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}
