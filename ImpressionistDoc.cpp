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
#include "WarpBrush.h"

#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() :
m_nPaintWidth(300), m_nPaintHeight(275),
m_ucBitmap(NULL), m_ucPainting(NULL),
m_iGradient(NULL), m_ucPainting_Undo(NULL), m_uiGradientMod(NULL), m_ucEdge(NULL),
m_ucAnother(NULL), m_iReferenceGradient(NULL), m_uiReferenceGradientMod(NULL),
m_ucDissolve(NULL),
m_ucAlphaBrush(NULL)
{
	memset(m_blackColor, 0, 3);
	// Set NULL image name as init.
	m_imageName[0]	='\0';

	m_nWidth		= -1;
	//default impressionist values
	m_nPaintlyStyleType = 0;
	m_nPaintlyStrokeType = 1;
	m_nPaintlyThreshold = 30;
	m_nPaintlyMaxBrush = 8;
	m_nPaintlyMinBrush = 2;
	m_nPaintlyMaxStroke = 8;
	m_nPaintlyMinStroke = 4;
	m_nPaintlyLayers = 2;

	m_dPaintlyGrid = 1.0;
	m_dPaintlyCurvature = 1.0;
	m_dPaintlyBlur = 0.8;
	m_dPaintlyAlpha = 0.85;
	m_dPaintlyJr = 1.0;
	m_dPaintlyJg = 1.0;
	m_dPaintlyJb = 1.0;
	m_dPaintlyJh = 1.0;
	m_dPaintlyJs = 1.0;
	m_dPaintlyJv = 1.0;

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
	ImpBrush::c_pBrushes[BRUSH_WARP]
		= new WarpBrush(this, "Warp Brush");

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
/////////////////////////././././././/////////////////////////
void ImpressionistDoc::setPaintlyStyle(int type)
{
	if (type != STYLE_CUSTOMIZE)
	{
		m_pUI->m_paintlyStrokeChoice->deactivate();
		m_pUI->m_paintlyThresholdSlider->deactivate();
		m_pUI->m_paintlyMaxBrushSizeSlider->deactivate();
		m_pUI->m_paintlyMinBrushSizeSlider->deactivate();
		m_pUI->m_paintlyGridSlider->deactivate();
		m_pUI->m_paintlyCurvatureSlider->deactivate();
		m_pUI->m_paintlyBlurSlider->deactivate();
		m_pUI->m_paintlyMaxStrokeLengthSlider->deactivate();
		m_pUI->m_paintlyMinStrokeLengthSlider->deactivate();
		m_pUI->m_paintlyAlphaSlider->deactivate();
		m_pUI->m_paintlyLayersSlider->deactivate();
	}
	else
	{
		setPaintlyStroke(0);
		m_pUI->m_paintlyStrokeChoice->activate();
		m_pUI->m_paintlyThresholdSlider->activate();
		m_pUI->m_paintlyMaxBrushSizeSlider->activate();
		m_pUI->m_paintlyMinBrushSizeSlider->activate();
		m_pUI->m_paintlyGridSlider->activate();
		m_pUI->m_paintlyCurvatureSlider->activate();
		m_pUI->m_paintlyBlurSlider->activate();
		m_pUI->m_paintlyMaxStrokeLengthSlider->activate();
		m_pUI->m_paintlyMinStrokeLengthSlider->activate();
		m_pUI->m_paintlyAlphaSlider->activate();
		m_pUI->m_paintlyLayersSlider->activate();
	}

	if (type == STYLE_IMPRESSIONIST)
	{
		setPaintlyStroke(1);
		setPaintlyThreshold(30);
		m_nPaintlyMaxBrush = 8;
		m_nPaintlyMinBrush = 2;
		m_dPaintlyGrid = 1.0;
		m_dPaintlyCurvature = 1.0;
		m_dPaintlyBlur = 0.8;
		m_nPaintlyMaxStroke = 8;
		m_nPaintlyMinStroke = 4;
		m_dPaintlyAlpha = 0.85;
		m_nPaintlyLayers = 2;
	}

	if (type == STYLE_EXPRESSIONIST)
	{
		m_nPaintlyStrokeType = 1;
		m_nPaintlyThreshold = 10;
		m_nPaintlyMaxBrush = 7;
		m_nPaintlyMinBrush = 3;
		m_dPaintlyGrid = 1.0;
		m_dPaintlyCurvature = 0.45;
		m_dPaintlyBlur = 0.5;
		m_nPaintlyMaxStroke = 20;
		m_nPaintlyMinStroke = 12;
		m_dPaintlyAlpha = 1.0;
		m_nPaintlyLayers = 3;
	}

	m_nPaintlyStyleType = type;
}

int ImpressionistDoc::getPaintlyStyle()
{
	return m_nPaintlyStyleType;
}

void ImpressionistDoc::setPaintlyStroke(int type)
{
	m_nPaintlyStrokeType = type;
}

int ImpressionistDoc::getPaintlyStroke()
{
	return m_nPaintlyStrokeType;
}

void ImpressionistDoc::setPaintlyThreshold(int val)
{
	m_nPaintlyThreshold = val;
}

int ImpressionistDoc::getPaintlyThreshold()
{
	return m_nPaintlyThreshold;
}


int ImpressionistDoc::getPaintlyMaxBrush()
{
	return m_nPaintlyMaxBrush;
}

void ImpressionistDoc::setPaintlyMaxBrush(int val)
{
	m_nPaintlyMaxBrush = val;
}

int ImpressionistDoc::getPaintlyMinBrush()
{
	return m_nPaintlyMinBrush;
}

void ImpressionistDoc::setPaintlyMinBrush(int val)
{
	m_nPaintlyMinBrush = val;
}

double ImpressionistDoc::getPaintlyGrid()
{
	return m_dPaintlyGrid;
}
void ImpressionistDoc::setPaintlyGrid(double val)
{
	m_dPaintlyGrid = val;
}

double ImpressionistDoc::getPaintlyCurvature()
{
	return m_dPaintlyCurvature;
}

void ImpressionistDoc::setPaintlyCurvature(double val)
{
	m_dPaintlyCurvature = val;
}

double ImpressionistDoc::getPaintlyBlur()
{
	return m_dPaintlyBlur;
}

void ImpressionistDoc::setPaintlyBlur(double val)
{
	m_dPaintlyBlur = val;
}

int ImpressionistDoc::getPaintlyMaxStroke()
{
	return m_nPaintlyMaxStroke;
}

void ImpressionistDoc::setPaintlyMaxStroke(int val)
{
	m_nPaintlyMaxStroke = val;
}

int ImpressionistDoc::getPaintlyMinStroke()
{
	return m_nPaintlyMinStroke;
}

void ImpressionistDoc::setPaintlyMinStroke(int val)
{
	m_nPaintlyMinStroke = val;
}

double ImpressionistDoc::getPaintlyAlpha()
{
	return m_dPaintlyAlpha;
}

void ImpressionistDoc::setPaintlyAlpha(double val)
{
	m_dPaintlyAlpha = val;
}

int ImpressionistDoc::getPaintlyLayers()
{
	return m_nPaintlyLayers;
}

void ImpressionistDoc::setPaintlyLayers(int val)
{
	m_nPaintlyLayers = val;
}

double ImpressionistDoc::getPaintlyJr()
{
	return m_dPaintlyJr;
}

void ImpressionistDoc::setPaintlyJr(double val)
{
	m_dPaintlyJr = val;
}

double ImpressionistDoc::getPaintlyJg()
{
	return m_dPaintlyJg;
}

void ImpressionistDoc::setPaintlyJg(double val)
{
	m_dPaintlyJg = val;
}

double ImpressionistDoc::getPaintlyJb()
{
	return m_dPaintlyJb;
}

void ImpressionistDoc::setPaintlyJb(double val)
{
	m_dPaintlyJb = val;
}

double ImpressionistDoc::getPaintlyJh()
{
	return m_dPaintlyJr;
}

void ImpressionistDoc::setPaintlyJh(double val)
{
	m_dPaintlyJr = val;
}

double ImpressionistDoc::getPaintlyJs()
{
	return m_dPaintlyJg;
}

void ImpressionistDoc::setPaintlyJs(double val)
{
	m_dPaintlyJg = val;
}

double ImpressionistDoc::getPaintlyJv()
{
	return m_dPaintlyJb;
}

void ImpressionistDoc::setPaintlyJv(double val)
{
	m_dPaintlyJb = val;
}


///////////////////////////////////////////////////////////////
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
	
	/*double dissolveRate = 0.5;
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
	}*/


	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			int p = rand() % 10;
			int pixel = (row*width + col) * 3;
			if (p > 5)
			{
				m_ucBitmap[pixel] = m_ucDissolve[pixel];
				m_ucBitmap[pixel + 1] = m_ucDissolve[pixel + 1];
				m_ucBitmap[pixel + 2] = m_ucDissolve[pixel + 2];
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

//------------------------------------------------------------------
// Get the color of the pixel in the paint image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetPaintPixel(int x, int y, EdgeMode mode = ImpressionistDoc::EDGE_CONFINE)
{
	switch (mode)
	{
	case ImpressionistDoc::EDGE_CONFINE:
		if (x < 0)
			x = 0;
		else if (x >= m_nWidth)
			x = m_nWidth - 1;

		if (y < 0)
			y = 0;
		else if (y >= m_nHeight)
			y = m_nHeight - 1;
		return (GLubyte*)(m_ucPainting + 3 * (y*m_nWidth + x));
		break;
	case ImpressionistDoc::EDGE_REFLECTION:
		x = (x < m_nWidth) ? abs(x) : 2 * (m_nWidth - 1) - x;
		y = (y < m_nHeight) ? abs(y) : 2 * (m_nHeight - 1) - y;
		return (GLubyte*)(m_ucPainting + 3 * (y*m_nWidth + x));
		break;
	case ImpressionistDoc::EDGE_BLACK:
	{
		if (x < m_nWidth && y < m_nHeight && x > 0 && y > 0)
			return (GLubyte*)(m_ucPainting + 3 * (y*m_nWidth + x));
		else
		{
			return m_blackColor;
		}		
		break;
	}
		
	}
	
}

//----------------------------------------------------------------
// Get the color of the pixel in the paint image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetPaintPixel(const Point p, EdgeMode mode =	ImpressionistDoc::EDGE_CONFINE)
{
	return GetPaintPixel(p.x, p.y, mode);
}

GLubyte* ImpressionistDoc::GetPaintPixel(double x, double y, EdgeMode mode = ImpressionistDoc::EDGE_CONFINE)
{
	int ceilx = ceil(x);
	int ceily = ceil(y);
	int floorx = floor(x);
	int floory = floor(y);
	if (ceilx == floorx && ceily == floory) return GetPaintPixel((int)x, (int)y, mode);
	if (ceilx == floorx || ceily == floory)
	{
		GLubyte* pixel1 = GetPaintPixel(ceilx, ceily, mode);
		GLubyte* pixel2 = GetPaintPixel(floorx, floory, mode);
		//take average of the two pixels
		GLubyte* pixel = new GLubyte[3];
		for (int i = 0; i < 3; ++i)
		{
			pixel[i] = (pixel1[i] + pixel2[i]) / 2;
		}
		return pixel;
	}
	GLubyte* pixel1 = GetPaintPixel(ceilx, ceily, mode);
	GLubyte* pixel2 = GetPaintPixel(ceilx, floory, mode);
	GLubyte* pixel3 = GetPaintPixel(floorx, ceily, mode);
	GLubyte* pixel4 = GetPaintPixel(floorx, floory, mode);
	//take average of the two pixels
	GLubyte* pixel = new GLubyte[3];
	for (int i = 0; i < 3; ++i)
	{
		pixel[i] = (pixel1[i] + pixel2[i] + pixel3[i] + pixel4[i]) / 4;
	}
	return pixel;
}

//------------------------------------------------------------------
// SET the color of the pixel in the paint image at coord x and y
//------------------------------------------------------------------
void ImpressionistDoc::SetPaintPixel(int x, int y, const GLubyte* color)
{
	//directly ignore outbound situations
	if (x < m_nWidth && y < m_nHeight && x > 0 && y > 0)
		memcpy(m_ucPainting + 3 * (y * m_nWidth + x), color, 3);
}

//----------------------------------------------------------------
// Set the color of the pixel in the paint image at point p
//----------------------------------------------------------------
void ImpressionistDoc::SetPaintPixel(const Point p, const GLubyte* color)
{
	SetPaintPixel(p.x, p.y, color);
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


void ImpressionistDoc::applyPaintlyPaint()
{
	m_pUI->m_paintView->paintlyPaint();
}

// Generate Thumbnail Collage
void ImpressionistDoc::StartCollage()
{
	const int numPatchHeight = ceil((double)m_nPaintHeight / 32.0);
	const int numPatchWidth = ceil((double)m_nPaintWidth / 32.0);
	const int patchHeight = 32;
	const int patchWidth = 32;
	const int channelSize = 1024;
	const int numFiles = 6;
	const string fileNames[] = {
		"data_batch_1.bin",
		"data_batch_2.bin",
		"data_batch_3.bin",
		"data_batch_4.bin",
		"data_batch_5.bin",
		"test_batch.bin",
	};
	const int imageSize = 3073; //1 class byte (useless here) + 3 * 1024 bytes for each channel
	const int imageOffset = 1;
	struct Patch {
		GLubyte* data;
		int startX;
		int startY;
		int distance;
		Patch(GLubyte* data = NULL, int x = 0, int y = 0): distance(INT_MAX), data(data), startX(x), startY(y){}
	};

	//1. slice the original image
	//printf("slicing original image\n");
	vector<Patch> originalPatches = vector<Patch>();
	for (int y = 0; y < m_nPaintHeight; y += patchHeight)
	{
		for (int x = 0; x < m_nPaintWidth; x += patchWidth)
		{
			// array to store pixels of this patch. First 1024 bytes R, second G, third B.
			GLubyte* thisPatch = new GLubyte[patchWidth * patchHeight * 3];
			//copy this 32*32 patch out to the vector
#pragma loop(hint_parallel(8))
			for (int j = 0; j < patchHeight; ++j)
				for (int i = 0; i < patchWidth; ++i)
				{
					const GLubyte* origPixel = GetOriginalPixel(x + i, y + j);
					int pos = j * patchWidth + i;
					//shuffle the RGB format
					thisPatch[pos] = origPixel[0]; //R
					thisPatch[pos + channelSize] = origPixel[1]; //G
					thisPatch[pos + 2 * channelSize] = origPixel[2]; //B
				}

			originalPatches.push_back(Patch(thisPatch, x, y));
		}
	}

	
	//count of thumbnails
	int count = 0;
	//for each bin file
	for (int fileIdx = 0; fileIdx < numFiles; ++fileIdx)
	{
		//open the file
		string fileName = fileNames[fileIdx];

		ifstream inFile(fileName, ios::in | ios::binary);

		//buffer contains an external image
		GLubyte buffer[imageSize-imageOffset]; //for the real image
		GLubyte reverseBuffer[imageSize]; //the image read directly
		
		while (inFile.read((char*)reverseBuffer, imageSize))
		{
			++count;
			if (count % 1000 == 0) printf("processing thumbnail %d\n", count);
			//reverse row sequence in the buffer and remove the first byte
#pragma loop(hint_parallel(8))
			for (int i = 0; i < patchHeight; ++i){
				memcpy(buffer + i * patchWidth, imageOffset + reverseBuffer + (patchHeight - 1 - i) * patchWidth, patchWidth);
				memcpy(buffer + channelSize + i * patchWidth, imageOffset + channelSize + reverseBuffer + (patchHeight - 1 - i) * patchWidth, patchWidth);
				memcpy(buffer + channelSize * 2 + i * patchWidth, imageOffset + channelSize * 2 + reverseBuffer + (patchHeight - 1 - i) * patchWidth, patchWidth);
			}

			//for every image patch
			for (vector<Patch>::iterator patch = originalPatches.begin(); patch != originalPatches.end(); ++patch)
			{
				// distance of the patch to the thumbnail
				int distance = 0;
#pragma loop(hint_parallel(8))
				for (int i = 0; i < 3 * channelSize; ++i)
					distance += abs(buffer[i] - patch->data[i]);

				//2. compare the distance of images
				if (distance < patch->distance) {
					patch->distance = distance;
					// i j are coordinates in patch
					// x y are coordinates in ucpainting
					for (int j = 0; j < patchHeight; ++j)
						for (int i = 0; i < patchWidth; ++i)
						{
							int x = i + patch->startX;
							int y = j + patch->startY;
							if (x >= m_nPaintWidth || y >= m_nPaintHeight)
								continue;
							int bufferPos = j * patchWidth + i;
							memcpy(m_ucPainting + 3 * (y * m_nPaintWidth + x), buffer + bufferPos, 1); //R
							memcpy(m_ucPainting + 3 * (y * m_nPaintWidth + x) + 1, buffer + channelSize + bufferPos, 1); //G
							memcpy(m_ucPainting + 3 * (y * m_nPaintWidth + x) + 2, buffer + channelSize * 2 + bufferPos, 1); //B
						}
				}
			}
		}

		inFile.close();
	}
	
}
