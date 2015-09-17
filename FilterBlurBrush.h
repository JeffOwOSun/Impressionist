//
// FilterBlurBrush.h
//
// The header file for Line Brush. 
//

#ifndef FILTERBLURBRUSH_H
#define FILTERBLURBRUSH_H

#include "ImpBrush.h"
#include <cmath>

class FilterBlurBrush : public ImpBrush
{
public:
	FilterBlurBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
	double applyBlur(GLdouble* filter, int filterW, int filterH, int x, int y, int channel);
private:
	
};

#endif