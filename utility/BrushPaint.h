#ifndef BrushPaint
#define BrushPaint

#include "../ImpBrush.h"

class BrushPaint
{
public:
	void PointPaint();
	void LinePaint();
	void ScatteredLinePaint(const Point source, const Point target);
private:
	int NumOfLines;
}

#endif