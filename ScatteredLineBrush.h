//
// ScatteredLineBrush.h
//
// Scatter Line Brush
//

#ifndef SCATTERLINEBRUSH_H
#define SCATTERLINEBRUSH_H

#include "ImpBrush.h"
#include <cmath>
#define PI 3.14159265358979

class ScatteredLineBrush : public ImpBrush
{
public:
	ScatteredLineBrush(ImpressionistDoc* pDoc, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
private:
	int lineLength;
	int lineAngle;
	int lineXProj; //X projection of line length
	int lineYProj; //Y projection of line length
};

#endif



