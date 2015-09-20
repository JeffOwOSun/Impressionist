//
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>
#include <string.h>
#include <algorithm>

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
#include "FilterBlurBrush.h"
#include "FilterSharpenBrush.h"
#include "FilterCustomized.h"
#include "AlphaMappedBrush.h"

#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() :
m_nPaintWidth(300), m_nPaintHeight(275),
m_ucBitmap(NULL), m_ucPainting(NULL),
m_iGradient(NULL), m_ucPainting_Undo(NULL), m_uiGradientMod(NULL), m_ucEdge(NULL),
m_ucAnother(NULL), m_iReferenceGradient(NULL), m_uiReferenceGradientMod(NULL),
m_ucDissolve(NULL),
m_ucAlphaBrush(NULL)
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
	ImpBrush::c_pBrushes[BRUSH_BLUR_FILTER]
		= new FilterBlurBrush(this, "Blur Filter");
	ImpBrush::c_pBrushes[BRUSH_SHARPEN_FILTER]
		= new FilterSharpenBrush(this, "Sharpen Filter");
	ImpBrush::c_pBrushes[BRUSH_ALPHA_MAPPED]
		= new AlphaMappedBrush(this, "Alpha Mapped");

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
	if (type != BRUSH_LINES && type != BRUSH_SCATTERED_LINES) {
		m_pUI->m_StrokeDirectionChoice->deactivate();
		m_pUI->m_BrushLineWidthSlider->deactivate();
		m_pUI->m_BrushLineAngleSlider->deactivate();
	}
	else {
		m_pUI->m_StrokeDirectionChoice->activate();
		m_pUI->m_BrushLineWidthSlider->activate();
		m_pUI->m_BrushLineAngleSlider->activate();
	}
	if (type == BRUSH_BLUR_FILTER || type == BRUSH_SHARPEN_FILTER)
	{
		m_pUI->m_BrushSizeSlider->deactivate();
		m_pUI->m_BrushAlphaSlider->deactivate();
	} 
	else
	{
		m_pUI->m_BrushSizeSlider->activate();
		m_pUI->m_BrushAlphaSlider->activate();
	}
	
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

int ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}

double ImpressionistDoc::getColorR()
{
	return m_pUI->getColorR();
}

double ImpressionistDoc::getColorG()
{
	return m_pUI->getColorG();
}

double ImpressionistDoc::getColorB()
{
	return m_pUI->getColorB();
}
bool ImpressionistDoc::getEdgeClipping()
{
	return m_pUI->getEdgeClipping();
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

	// assuming resolution changed. release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;
	if ( m_ucPainting_Undo ) delete[] m_ucPainting_Undo;
	if ( m_iGradient) delete[] m_iGradient;
	if ( m_uiGradientMod) delete[] m_uiGradientMod;
	if ( m_ucEdge) delete[] m_ucEdge;
	if (m_ucDissolve) delete[] m_ucDissolve;
	if (m_ucAnother) {
		delete[] m_ucAnother;
		m_ucAnother = NULL;
	}
	if (m_iReferenceGradient) {
		delete[] m_iReferenceGradient;
		m_iReferenceGradient = NULL;
	}
	if (m_uiReferenceGradientMod) {
		delete[] m_uiReferenceGradientMod;
		m_uiReferenceGradientMod = NULL;
	}

	m_ucBitmap		= data;

	// allocate the gradient map of the original Image
	// TODO: Wrap these filters in custom classes
	m_iGradient = new GLint[2*width*height];
	m_uiGradientMod = new GLuint[width*height];

	//calculate gradient
	CalculateGradient(m_ucBitmap, m_iGradient, m_uiGradientMod);

	// allocate space for edge
	m_ucEdge = new GLubyte[3*width*height];
	CalculateEdgeMap(m_pUI->getEdgeThreshold());

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	// allocate space for undo painting
	m_ucPainting_Undo = new unsigned char[width*height * 3];
	memset(m_ucPainting_Undo, 0, width*height * 3);

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
// Load edge image
//----------------------------------------------------------------
int ImpressionistDoc::loadEdgeImage(char* iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	//reject different dimension
	if (width != m_nPaintWidth || height != m_nPaintHeight)
	{
		fl_alert("The dimension of edge image doesn't match source image");
		return 0;
	}
	
	//load the edge map
	if (m_ucEdge) delete[] m_ucEdge;
	m_ucEdge = data;

	//update original view if it's displaying edge
	if (m_pUI->m_origView->viewMode == OriginalView::EDGE_MODE)
		m_pUI->m_origView->refresh();

	return 1;
}

//----------------------------------------------------------------
// Load mural image
//----------------------------------------------------------------
int ImpressionistDoc::loadMuralImage(char* iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	//reject different dimension
	if (width != m_nPaintWidth || height != m_nPaintHeight)
	{
		fl_alert("The dimension of edge image doesn't match source image");
		return 0;
	}

	//load the bit map
	if (m_ucBitmap) delete[] m_ucBitmap;
	m_ucBitmap = data;

	//calculate gradient
	CalculateGradient(m_ucBitmap, m_iGradient, m_uiGradientMod);

	//calculate new edge
	CalculateEdgeMap(m_pUI->getEdgeThreshold());

	//refresh display
	m_pUI->m_origView->refresh();

	return 1;
}

//----------------------------------------------------------------
// Load another image
//----------------------------------------------------------------
int ImpressionistDoc::loadAnother(char* iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	//reject different dimension
	if (width != m_nPaintWidth || height != m_nPaintHeight)
	{
		fl_alert("The dimension of edge image doesn't match source image");
		return 0;
	}

	//load the bit map
	if (m_ucAnother) delete[] m_ucAnother;
	m_ucAnother = data;

	if (!m_iReferenceGradient) //if never loaded another image before
	{
		//allocate gradient space
		m_iReferenceGradient = new GLint[2 * width*height];
		m_uiReferenceGradientMod = new GLuint[width*height];
	}
	//calculate gradient
	CalculateGradient(m_ucAnother, m_iReferenceGradient, m_uiReferenceGradientMod);

	//refresh display
	m_pUI->m_origView->refresh();

	return 1;
}


int ImpressionistDoc::loadDissolveImage(char* iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width, height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	//reject different dimension
	if (width != m_nPaintWidth || height != m_nPaintHeight)
	{
		fl_alert("The dimension of edge image doesn't match source image");
		return 0;
	}

	if (m_ucDissolve) delete[] m_ucDissolve;
	m_ucDissolve = data;
	
	double dissolveRate = 0.5;
	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			for (int channel = 0; channel < 3; ++channel)
			{
				int pixel = (row*width + col) * 3 + channel;
				m_ucBitmap[pixel] = ((double)m_ucBitmap[pixel] * (1 - dissolveRate)) + ((double)m_ucDissolve[pixel] * dissolveRate);
			}
		}
	}

	//refresh display
	m_pUI->m_origView->refresh();

	return 1;
}

//----------------------------------------------------------------
// Load alpha brush
//----------------------------------------------------------------
int ImpressionistDoc::loadAlphaBrush(char* iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	m_nAlphaBrushWidth = width;
	m_nAlphaBrushHeight = height;

	//load the bit map
	if (m_ucAlphaBrush) delete[] m_ucAlphaBrush;
	m_ucAlphaBrush = data;

	return 1;
}
//----------------------------------------------------------------
//Calculate gradients
//----------------------------------------------------------------
void ImpressionistDoc::CalculateGradient(const GLubyte *const source, GLint * const targetGradient, GLuint * const targetMod)
{
	int width = m_nPaintWidth, height = m_nPaintHeight;

	//calculate the intensity
	GLubyte * intensity = new GLubyte[width * height];
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
			intensity[j * width + i] = filterIntensity(source + 3 * (j * width + i));

	//Apply gaussian filter
	GLubyte* blurred = new GLubyte[width*height];
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			blurred[j * width + i] = applyFilter((GLdouble*)&Gaussian, 3, 3, intensity, width, height, i, j);
		}
	
	//Calculate the gradient
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			int pos = j * width + i;
			targetGradient[2 * pos] = applyFilter((GLint*)&SobelX, 3, 3, blurred, width, height, i, j);
			targetGradient[2 * pos + 1] = applyFilter((GLint*)&SobelY, 3, 3, blurred, width, height, i, j);
			targetMod[pos] = sqrt(pow(targetGradient[2 * pos], 2) + pow(targetGradient[2 * pos + 1], 2));
		}
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
// Save the edge image
// This is called by the UI when the save edge image menu button is
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveEdgeImage(char *iname)
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucEdge);

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

/*
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
*/

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

	return m_iGradient[2*(y*m_nWidth + x)];
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

	return m_iGradient[2 * (y*m_nWidth + x) + 1];
}

GLint ImpressionistDoc::GetReferenceGradientX(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return m_iReferenceGradient[2 * (y*m_nWidth + x)];
}

GLint ImpressionistDoc::GetReferenceGradientY(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return m_iReferenceGradient[2 * (y*m_nWidth + x) + 1];
}

GLuint ImpressionistDoc::GetGradientMod(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return m_uiGradientMod[y*m_nWidth + x];
}
GLboolean ImpressionistDoc::GetEdge(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return m_ucEdge[3*(y*m_nWidth + x)] > 0 ? true: false;
}


GLint ImpressionistDoc::GetGradientX(Point point)
{
	return GetGradientX(point.x, point.y);
}
GLint ImpressionistDoc::GetGradientY(Point point)
{
	return GetGradientY(point.x, point.y);
}
GLuint ImpressionistDoc::GetGradientMod(Point point)
{
	return GetGradientMod(point.x, point.y);
}
GLint ImpressionistDoc::GetReferenceGradientX(Point point)
{
	return GetReferenceGradientX(point.x, point.y);
}
GLint ImpressionistDoc::GetReferenceGradientY(Point point)
{
	return GetReferenceGradientY(point.x, point.y);
}
GLboolean ImpressionistDoc::GetEdge(Point point)
{
	return GetEdge(point.x, point.y);
}


// Calculate edge with given threshold, and store it.
GLubyte* ImpressionistDoc::CalculateEdgeMap(int threshold)
{
	if (!m_ucEdge) return NULL;

	int pos = 0;
	for (int i = 0; i < m_nPaintWidth; ++i)
		for (int j = 0; j < m_nPaintHeight; ++j)
		{
			pos = (j * m_nPaintWidth + i);
			m_ucEdge[3*pos] = m_ucEdge[3*pos+1] = m_ucEdge[3*pos+2] = m_uiGradientMod[pos] > threshold ? 255 : 0;
		}
	//return the calculated Edge
	return m_ucEdge;
}

void ImpressionistDoc::applyCustomFilter(double* kernel, int w, int h)
{
	m_pUI->m_paintView->refresh();
	FilterCustomized customizedFilter(kernel, w, h);
	GLubyte* tar = new GLubyte[m_nWidth * m_nHeight * 3];
	memset(tar, 0, sizeof(tar));
	for (int i = 0; i < m_nHeight; ++i)
	{
		for (int j = 0; j < m_nWidth; ++j)
		{
			int pixelPos = (i * m_nWidth + j) * 3;
			tar[pixelPos] = (GLubyte)customizedFilter.applyCustomizedFilter(m_ucBitmap, j, i, m_nWidth, m_nHeight, 0);
			tar[pixelPos + 1] = (GLubyte)customizedFilter.applyCustomizedFilter(m_ucBitmap, j, i, m_nWidth, m_nHeight, 1);
			tar[pixelPos + 2] = (GLubyte)customizedFilter.applyCustomizedFilter(m_ucBitmap, j, i, m_nWidth, m_nHeight, 2);
		}
	}
	m_ucPainting = tar;
}

void ImpressionistDoc::applyAutoPaint(ImpBrush* brush, int space, bool vary)
{
	if (!vary)
	{
		for (int j = 0; j < m_nHeight; j += space)
		{
			for (int i = 0; i < m_nWidth; i += space)
			{
				Point p(i, j);
				if (i == 0 && j == 0)
					brush->BrushBegin(p, p);
				else
					brush->BrushMove(p, p);
			}
			Point p(m_nWidth, m_nHeight);
			brush->BrushEnd(p, p);
		}
	} 
	else
	{
		std::vector<Point> order;
		int range = (int)((m_pUI->getSize()));
		for (int j = 0; j < m_nHeight; j += space)
		{
			for (int i = 0; i < m_nWidth; i += space)
			{
				int randomPos = (rand() % range) - (range / 2);
				int randomPos2 = (rand() % range) - (range / 2);
				Point p(i+randomPos, j+randomPos2);
				order.push_back(p);
			}
		}
		std::random_shuffle(order.begin(), order.end());

		brush->BrushBegin(order[0], order[0]);
		for (int i = 1; i < order.size(); ++i)
		{
			brush->BrushMove(order[i], order[i]);
		}
		brush->BrushEnd(order.back(), order.back());
	}
}
