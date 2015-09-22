// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"
#include "utility\Filter.h"
#include "utility\SaveMatrix.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image
	int		loadEdgeImage(char* iname);		// called by the UI to load Edge Image
	int		saveEdgeImage(char* iname);		// called by the UI to save Edge Image
	int		loadMuralImage(char *iname);	// called by the UI to load mural image
	int		loadAnother(char *iname);		// called by the UI to load gradient reference
	int		loadDissolveImage(char* iname); // called by the UI to load dissolved image
	int		loadAlphaBrush(char *iname);	// called by the UI to load alpha Brush

	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType

	void	setPaintlyStyle(int type);
	int		getPaintlyStyle();

	void	setPaintlyStroke(int type);
	int		getPaintlyStroke();

	void	setPaintlyThreshold(int val);
	int		getPaintlyThreshold();

	int		getPaintlyMaxBrush();
	void	setPaintlyMaxBrush(int val);

	int		getPaintlyMinBrush();
	void	setPaintlyMinBrush(int val);
	
	double	getPaintlyCurvature();
	void	setPaintlyCurvature(double val);

	double	getPaintlyBlur();
	void	setPaintlyBlur(double val);

	int		getPaintlyMaxStroke();
	void	setPaintlyMaxStroke(int val);

	int		getPaintlyMinStroke();
	void	setPaintlyMinStroke(int val);

	double	getPaintlyAlpha();
	void	setPaintlyAlpha(double val);

	int		getPaintlyLayers();
	void	setPaintlyLayers(int val);

	double	getPaintlyJr();
	void	setPaintlyJr(double val);
	double	getPaintlyJg();
	void	setPaintlyJg(double val);
	double	getPaintlyJb();
	void	setPaintlyJb(double val);
	double	getPaintlyJh();
	void	setPaintlyJh(double val);
	double	getPaintlyJs();
	void	setPaintlyJs(double val);
	double	getPaintlyJv();
	void	setPaintlyJv(double val);

	int		getStrokeDirectionType();
	int		getSize();						// get the UI size
	int		getWidth();
	int		getAngle();						// get the brush angle
	int		getAlpha();
	double	getColorR();
	double	getColorG();
	double	getColorB();
	bool	getEdgeClipping();
	
	void	setStrokeDirectionType(int type); // called by the UI to set the brushType
	void	setSize(int size);				// set the UI size
	void	setWidth(int width);
	void	setAngle(int angle);			// set the brush angle
	char*	getImageName();					// get the current image name

	//GLubyte GetIntensity(int x, int y);
	GLint GetGradientX(int x, int y);
	GLint GetGradientY(int x, int y);
	GLuint GetGradientMod(int x, int y);
	GLint GetReferenceGradientX(int x, int y);
	GLint GetReferenceGradientY(int x, int y);
	GLboolean GetEdge(int x, int y);
	//GLubyte GetIntensity(Point point);
	GLint GetGradientX(Point point);
	GLint GetGradientY(Point point);
	GLuint GetGradientMod(Point point);
	GLint GetReferenceGradientX(Point point);
	GLint GetReferenceGradientY(Point point);
	GLboolean GetEdge(Point point);
	void applyCustomFilter(double* kernel, int w, int h);
	void applyAutoPaint(ImpBrush* brush, int space, bool vary);
	void applyPaintlyPaint();

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Dimensions of the alpha mapped brush
	int				m_nAlphaBrushWidth,
					m_nAlphaBrushHeight;

	int				m_nPaintlyStyleType,
					m_nPaintlyStrokeType,
					m_nPaintlyThreshold,
					m_nPaintlyMaxBrush,
					m_nPaintlyMinBrush,
					m_nPaintlyMaxStroke,
					m_nPaintlyMinStroke,
					m_nPaintlyLayers;
	
	double			m_dPaintlyCurvature,
					m_dPaintlyBlur,
					m_dPaintlyAlpha,
					m_dPaintlyJr,
					m_dPaintlyJg,
					m_dPaintlyJb,
					m_dPaintlyJh,
					m_dPaintlyJs,
					m_dPaintlyJv;

	// Bitmaps for original image and painting.
	GLubyte*	m_ucBitmap;
	GLubyte*	m_ucPainting;
	// Another Image
	GLubyte *	m_ucAnother;
	// Alpha Brush
	GLubyte*	m_ucAlphaBrush;
	
	// Store painting to these array
	// Use unsigned char at now
	unsigned char* m_ucPainting_Undo;
	
	// Gradient map for intensity of original image.
	GLint *		m_iGradient;
	// Gradient modulo
	GLuint *	m_uiGradientMod;

	// Gradient map for intensity of reference image.
	GLint *		m_iReferenceGradient;
	// Gradient modulo
	GLuint *	m_uiReferenceGradientMod;
	
	// Edge map
	GLubyte * m_ucEdge;
	GLubyte * m_ucDissolve;


	// The current active brush.
	ImpBrush*			m_pCurrentBrush;
	int				m_pCurrentDirection;
	// Size of the brush.
	int m_nSize;							

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  
	// Calculate the edge map using gradient mod and threshold
	GLubyte* CalculateEdgeMap(int EdgeThreshold);
	// calculate the gradient
	void CalculateGradient(const GLubyte * const source, GLint * const targetGradient, GLuint* const targetMod);

private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
