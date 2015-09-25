//
// WarpBrush.cpp
//
// The implementation of Rubber Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "WarpBrush.h"

extern float frand();

WarpBrush::WarpBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name), effectiveRadius(0)
{
}

void WarpBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	effectiveRadius = pDoc->getSize();
	lastPoint = target;
	BrushMove(source, target);
}

void WarpBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* pUI = pDoc->m_pUI;
	if (pDoc == NULL) {
		printf("WarpBrush::BrushMove  document is NULL\n");
		return;
	}
	
	//calculate the displacement
	Point displacement = target - lastPoint;
	//do nothing if displacement is zero
	if (displacement.x == 0 && displacement.y == 0) return;
	//calculate the color of every point
	GLubyte* imagePatch = new GLubyte[effectiveRadius*effectiveRadius * 4 * 3];
	int startX = target.x - effectiveRadius;
	int startY = target.y - effectiveRadius;
	//calculate color and store in a temporary array
	for (int j = 0; j < effectiveRadius * 2; ++j)
		for (int i = 0; i < effectiveRadius * 2; ++i)
		{
			memcpy(imagePatch + 3 * (i + j * 2 * effectiveRadius), pDoc->GetPaintPixel(warppedSourcePoint(target, startX + i, startY + j, displacement, effectiveRadius), ImpressionistDoc::EDGE_BLACK), 3);
		}
	//copy this temporary to the paint image
	for (int j = 0; j < effectiveRadius * 2; ++j)
		for (int i = 0; i < effectiveRadius * 2; ++i)
		{
			pDoc->SetPaintPixel(startX + i, startY + j, imagePatch + 3 * (j * 2 * effectiveRadius + i));
		}
	//printf("warpped! displacement: %d %d center: %d %d radius: %d\n", displacement.x, displacement.y, target.x, target.y, effectiveRadius);
	delete[] imagePatch;
	lastPoint = target;
	pUI->m_paintView->RestoreContent();
}

void WarpBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
/**
* \brief Calculate the original point given the target point, the displacement of the brush, and
* radius of the rubber brush
*/
Point WarpBrush::warppedSourcePoint(const Point center, int targetX, int targetY, const Point displacement, int radius)
{
	//this determines how much the center is moved relative to brush movement
	const double ratio = 0.9;
	const int diffX = targetX - center.x;
	const int diffY = targetY - center.y;
	//distance of givent point from the center of brush
	const double distance = sqrt(diffX*diffX + diffY*diffY);
	if (distance >= radius) return Point(targetX, targetY);
	//this is the displacementMagnitude of the brush relative to last position
	const double displacementMagnitude = sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
	//the offset of desired point should be the minimum of two values:
	//1. center equal displacementMagnitude, decrease linearly by slope with distance
	//2. edge equal 0, increase linearly by slope with (radius - distance)
	double offset = ratio * displacementMagnitude * (1 - pow(distance / radius, 2));
	//with the given offset and the direction of displacement, calculate the point to which target should be mapped
	Point warped = Point(targetX - (offset / displacementMagnitude) * displacement.x, targetY -(offset/displacementMagnitude) * displacement.y);
	//printf("disMag: %f diff: %d %d target: %d %d orig: %d %d\n", displacementMagnitude, diffX, diffY,targetX, targetY, warped.x, warped.y);
	return warped;
}
Point WarpBrush::warppedSourcePoint(const Point center, const Point target, const Point displacement, int radius)
{
	return warppedSourcePoint(center, target.x, target.y, displacement, radius);
}