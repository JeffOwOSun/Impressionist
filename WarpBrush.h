//
// WarpBrush.h
//
// The header file for Rubber Brush. 
//

#ifndef WARPBRUSH_H
#define WARPBRUSH_H	

#include "ImpBrush.h"
#include <cmath>
#define PI 3.14159265358979

class WarpBrush : public ImpBrush
{
public:
	WarpBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
private:
	int effectiveRadius;
	Point lastPoint;
	Point warppedSourcePoint(const Point center, int targetX, int targetY, const Point displacement, int radius);
	Point warppedSourcePoint(const Point center, const Point target, const Point displacement, int radius);
};

#endif