//
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "ScatteredPointBrush.h"
#include "CircleBrush.h"
#include "LineBrush.h"
#include "ScatteredLineBrush.h"
#include "ScatteredCircleBrush.h"

#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() :
m_ucBitmap(NULL), m_ucPainting(NULL), m_ucIntensity(NULL),
m_iGradientX(NULL), m_iGradientY(NULL)
{
	// Set NULL image name as init.
	m_imageName[0]	='\0';

	m_nWidth		= -1;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]
		= new ScatteredPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]
		= new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]
		= new ScatteredCircleBrush( this, "Scattered Circles" );

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui)
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName()
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

void ImpressionistDoc::setStrokeDirectionType(int type)
{
	m_pUI->setStrokeDirection(type);
	m_pCurrentDirection = type;
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

int ImpressionistDoc::getWidth()
{
	return m_pUI->getWidth();
}

int ImpressionistDoc::getAngle()
{
	return m_pUI->getAngle();
}

int ImpressionistDoc::getStrokeDirectionType()
{
	return m_pUI->getStrokeDirection();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,
					height;

	if ( (data=readBMP(iname, width, height))==NULL )
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;
	if ( m_ucIntensity ) delete[] m_ucIntensity;
	if ( m_iGradientX ) delete[] m_iGradientX;
	if ( m_iGradientY ) delete[] m_iGradientY;

	m_ucBitmap		= data;

	// allocate and calculate the Intensity map of the original Image
	m_ucIntensity = new GLubyte[width*height];
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
			m_ucIntensity[j * width + i] = filterIntensity(m_ucBitmap + 3 * (j * width + i));

	//DEBUG//saveMatrix<unsigned char>("m_ucIntensity.txt", m_ucIntensity, width, height);

	// allocate and calculate the gradient map of the original Image
	// TODO: Wrap these filters in custom classes
	m_iGradientX = new GLint[width*height];
	m_iGradientY = new GLint[width*height];

	//Apply gaussian filter
	GLubyte* blurred = new GLubyte[width*height];
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			blurred[j * width + i] = applyFilter((GLdouble*)&Gaussian, 3, 3, m_ucIntensity, width, height, i, j);
		}
	//DEBUG//saveMatrix<unsigned char>("m_blurred.txt", blurred, width, height);
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			m_iGradientX[j * width + i] = applyFilter((GLint*)&SobelX, 3, 3, blurred, width, height, i, j);
			m_iGradientY[j * width + i] = applyFilter((GLint*)&SobelY, 3, 3, blurred, width, height, i, j);
		}
	//DEBUG//saveMatrix<int>("m_iGradientX.txt", m_iGradientX, width, height);
	//DEBUG//saveMatrix<int>("m_iGradientY.txt", m_iGradientY, width, height);

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
								m_pUI->m_mainWindow->y(),
								width*2,
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname)
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas()
{

	// Release old storage
	if ( m_ucPainting )
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well
		m_pUI->m_paintView->refresh();
	}

	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 )
		x = 0;
	else if ( x >= m_nWidth )
		x = m_nWidth-1;

	if ( y < 0 )
		y = 0;
	else if ( y >= m_nHeight )
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

//------------------------------------------------------------------
// Get the Intensity of original image coord x and y
//------------------------------------------------------------------
GLubyte ImpressionistDoc::GetIntensity(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return m_ucIntensity[y*m_nWidth + x];
}
GLubyte ImpressionistDoc::GetIntensity(Point point)
{
	return GetIntensity(point.x, point.y);
}
//------------------------------------------------------------------
// Get the Gradient of original image coord x and y
//------------------------------------------------------------------
GLint ImpressionistDoc::GetGradientX(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return m_iGradientX[y*m_nWidth + x];
}

GLint ImpressionistDoc::GetGradientY(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return m_iGradientY[y*m_nWidth + x];
}

GLint ImpressionistDoc::GetGradientX(Point point)
{
	return GetGradientX(point.x, point.y);
}
GLint ImpressionistDoc::GetGradientY(Point point)
{
	return GetGradientY(point.x, point.y);
}

