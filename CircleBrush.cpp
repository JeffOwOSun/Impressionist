//
// CircleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CircleBrush.h"

extern float frand();

CircleBrush::CircleBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void CircleBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	//Begin the Brush
	//Call BrushMove once to paint
	BrushMove(source, target);
}

void CircleBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "CircleBrush::BrushMove  document is NULL\n" );
		return;
	}
	//get the radius of the circle;
	float r = ((float) pDoc -> getSize()) / 2;
	//The brush is moved to a new place. I need to draw a filled circle there
	int num_segments = GetNumCircleSegments(r);

	float theta = 2 * 3.1415926 / float(num_segments);
	float tangetial_factor = tanf(theta);//calculate the tangential factor

	float radial_factor = cosf(theta);//calculate the radial factor
	
	//set the initial point coordinate
	float x = r;//we start at angle = 0
	float y = 0;

	glBegin(GL_TRIANGLE_FAN);
	//set color
	SetColor(source);
	//first point is the center point
	glVertex2f(target.x, target.y);
	//then loop to add the bounding points to the vertex map
	for (int ii = 0; ii < num_segments; ii++)
	{
		glVertex2f(x + target.x, y + target.y);//output vertex

		//calculate the tangential vector
		//remember, the radial vector is (x, y)
		//to get the tangential vector we flip those coordinates and negate one of them

		float tx = -y;
		float ty = x;

		//add the tangential vector

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		//correct using the radial factor

		x *= radial_factor;
		y *= radial_factor;
	}
	//last point should return to the first bounding point,
	//for GL_TRIANGLE_FAN doesn't close the loop altomatically
	glVertex2f(target.x + r, target.y);
	glEnd();
}

void CircleBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

int GetNumCircleSegments(float r)
{
	return 10 * sqrtf(r);//change the 10 to a smaller/bigger number as needed
}
