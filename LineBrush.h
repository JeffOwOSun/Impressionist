//
// LineBrush.h
//
// The header file for Line Brush. 
//

#ifndef LINEBRUSH_H
#define LINEBRUSH_H

#include "ImpBrush.h"
#include <cmath>
#define PI 3.14159265358979

class LineBrush : public ImpBrush
{
public:
	LineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
private:
	int lineLength;
	int lineAngle;
	int lineXProj; //X projection of line length
	int lineYProj; //Y projection of line length
	int prevMouseX;
	int prevMouseY;
};

#endif