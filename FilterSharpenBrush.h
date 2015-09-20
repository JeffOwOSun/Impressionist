//
// FilterSharpenBrush.h
//
// The header file for Line Brush. 
//

#ifndef FILTERSHARPENBRUSH_H
#define FILTERSHARPENBRUSH_H

#include "ImpBrush.h"
#include <cmath>

class FilterSharpenBrush : public ImpBrush
{
public:
	FilterSharpenBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
	double applySharpen(GLdouble* filter, int filterW, int filterH, int x, int y, int channel);
};

#endif