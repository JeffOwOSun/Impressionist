// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include <cmath>
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
	// Determine how to treat outbound points
	enum EdgeMode{
		EDGE_CONFINE, //confine the given coordinate to projection onto the viable margin
		EDGE_REFLECTION, //reflect the point as if there were more images out of the bound
		EDGE_BLACK //return black color if out of bound
	};
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p ); 
	// Get the color of the painted picture at the specified point
	GLubyte* GetPaintPixel(int x, int y, EdgeMode mode);
	GLubyte* GetPaintPixel(double x, double y, EdgeMode mode);
	GLubyte* GetPaintPixel(const Point p, EdgeMode mode);
	void SetPaintPixel(int x, int y, const GLubyte* color);
	void SetPaintPixel(const Point p, const GLubyte* color);
	// Calculate the edge map using gradient mod and threshold
	GLubyte* CalculateEdgeMap(int EdgeThreshold);
	// calculate the gradient
	void CalculateGradient(const GLubyte * const source, GLint * const targetGradient, GLuint* const targetMod);
	// Thumbnail Collage
	void StartCollage();
	//void StopCollage(); //Not sure if this will work when startgothic is in process. May need subprocess

private:
	char			m_imageName[256];
	//bool			m_bGothicStopSignal; //this bit will be turned on by StopCollage()
	GLubyte			m_blackColor[3];

};

extern void MessageBox(char *message);

#endif
