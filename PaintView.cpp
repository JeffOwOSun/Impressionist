//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include "CircleBrush.h"
#include <cmath>
#include <string.h>
#include <vector>
#include <algorithm>
#include <fstream>
#define PI 3.14159265358979


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6
#define AUTO_DRAW			7
#define PAINTLY				8


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );

		// Enable Alpha
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	//DEBUGONLY
	//printf("WindowW: %d WindowH: %d drawW: %d drawH: %d WidgetX: %d WidgetY: %d WidgetW: %d WidgetH: %d\n", m_nWindowWidth, m_nWindowHeight, m_nDrawWidth, m_nDrawHeight, x(), y(), w(), h());

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();
	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		/* printf("=====================\nsource: x-%d y-%d\ntarget: x-%d y-%d\nm_nStartRow: %d m_nEndRow: %d\nm_nStartCol: %d m_nEndCol: %d\n", 
			   source.x, source.y, target.x, target.y, m_nStartRow, m_nEndRow, m_nStartCol, m_nEndCol);
		*/
		

		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			SaveUndoOnBrush();
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			break;
		case LEFT_MOUSE_DRAG:
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );
			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			//Handle right mouse down event
			//remember the point for the beginning of the red line
			m_ptLastPoint = Point(target);
			break;
		case RIGHT_MOUSE_DRAG:
			//Handle right mouse drag event
			//restorecontent to clear any previously drawn red line
			RestoreContent();
			//then draw a new red line
			glBegin(GL_LINES);
				glColor3ub(255, 0, 0); //set the color to red
				glVertex2d(m_ptLastPoint.x, m_ptLastPoint.y);
				glVertex2d(target.x, target.y);
			glEnd();
			break;
		case RIGHT_MOUSE_UP:
			//Restorecontent to clear red line
			RestoreContent();
			//set the angle and line length, somehow
			//calculate the line length and angle
			//use the calculated value to update UI elements
			if (m_pDoc->m_pCurrentBrush == ImpBrush::c_pBrushes[BRUSH_LINES] ||
				m_pDoc->m_pCurrentBrush == ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES])
			{
				m_pUI->setSize((int)(sqrt(pow(m_ptLastPoint.x - target.x, 2) + pow(m_ptLastPoint.y - target.y, 2))));
				m_pUI->setAngle((int)(atan2((double)target.y - m_ptLastPoint.y, (double)target.x - m_ptLastPoint.x) / PI * 360));
			}
			break;
		case AUTO_DRAW:
			m_pDoc->applyAutoPaint(m_pDoc->m_pCurrentBrush, m_pUI->getAutoPaintSpace(), m_pUI->getAutoVary());
			SaveCurrentContent();
			RestoreContent();
			break;
		case PAINTLY:
			m_pDoc->applyPaintlyPaint();
			SaveCurrentContent();
			RestoreContent();
			break;
		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{

	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		// call draw Marker from mouse drag event
		m_pUI->drawMarker(coord);
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		//float_marker();
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		// call draw marker from Mouse move event
		m_pUI->drawMarker(coord);
		break;
	default:
		return 0;
		break;

	}

	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	Fl_Gl_Window::resize(x(), y(), width, height);
}

void PaintView::resize(int x, int y, int width, int height)
{
	Fl_Gl_Window::resize(x, y, w(), h());
}

void PaintView::SaveCurrentContent()
{
	//printf("Saving\n");
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT_AND_BACK);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, //only three bytes for one image
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	//printf("Restoring\n");
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

void PaintView::SaveUndoOnBrush()
{
	int dimension = m_pDoc->m_nPaintHeight * m_pDoc->m_nPaintWidth * 3;
	delete[] m_pDoc->m_ucPainting_Undo;
	m_pDoc->m_ucPainting_Undo = new unsigned char[dimension];
	memcpy(m_pDoc->m_ucPainting_Undo, m_pDoc->m_ucPainting, dimension);
}

void PaintView::TriggerAutoPaint()
{
	isAnEvent = 1;
	eventToDo = AUTO_DRAW;
	redraw();
}

void PaintView::TriggerPaintly()
{
	isAnEvent = 1;
	eventToDo = PAINTLY;
	redraw();
}

void PaintView::paintlyBlur(unsigned char* source, unsigned char* reference, int brushSize, int width, int height)
{
	/*cout << endl;
	cout << brushSize << endl;
	cout << endl;
	double blurRate = brushSize * m_pDoc->getPaintlyBlur();
	double* kernel = new double[9];*/
	double kernel[] = { 0, 1, 2, 1, 0, 1, 5, 9, 5, 1, 2, 9, 15, 9, 2, 1, 5, 9, 5, 1, 0, 1, 2, 1, 0 };
	int kernelSize = 5;
	double Weight = 87;
	// Initialize kernel
	/*for (int i = 0; i < kernelSize; i++)
	{
		for (int j = 0; j < kernelSize; j++)
		{
			int offsetX = brushSize*abs(i-kernelSize/2); int offsetY = brushSize*abs(j-kernelSize/2);
			kernel[i * kernelSize + j] = pow((2.71828183), (-(offsetX*offsetX + offsetY*offsetY) / (2 * blurRate*blurRate)));
			Weight += kernel[i * kernelSize + j];
		}
	}*/

	/*for (int i = 0; i < kernelSize; i++)
	{
		for (int j = 0; j < kernelSize; j++)
		{
			cout << kernel[i*kernelSize + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
*/

	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			int referencePosition = i * width + j;
			double sum[3] = { 0, 0, 0 };
			for (int _i = 0; _i < kernelSize; _i++)
			{
				for (int _j = 0; _j < kernelSize; _j++)
				{
					int curY = i + _i - kernelSize / 2;
					int curX = j + _j - kernelSize / 2;
					if (curY < 0 || curY > height - 1 || curX < 0 || curX > width - 1) continue;
					sum[0] += (double)source[(curY*width + curX) * 3] * kernel[_i*kernelSize + _j];
					sum[1] += (double)source[(curY*width + curX) * 3 + 1] * kernel[_i*kernelSize + _j];
					sum[2] += (double)source[(curY*width + curX) * 3 + 2] * kernel[_i*kernelSize + _j];
				}
			}
			for (int k = 0; k < 3; k++) {
				double res = sum[k] / Weight;
				if (res < 0) res = 0;
				if (res > 255) res = 255;
				reference[referencePosition * 3 + k] = (unsigned char)res;
			}
		}
	}
}

void PaintView::paintlyDiff(unsigned char* canvas, unsigned char* reference, unsigned char* diff, int width, int height)
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			int pos = i * width + j;
			diff[pos] = (reference[pos * 3] - canvas[pos * 3]) * (reference[pos * 3] - canvas[pos * 3])
				+ (reference[pos * 3 + 1] - canvas[pos * 3 + 1]) * (reference[pos * 3 + 1] - canvas[pos * 3 + 1])
				+ (reference[pos * 3 + 2] - canvas[pos * 3 + 2]) * (reference[pos * 3 + 2] - canvas[pos * 3 + 2]);
			diff[pos] = (unsigned char)sqrt((double)diff[pos]);
			
			//cout << (int)diff[pos] << " ";
		}
	}
	
}

void PaintView::paintlyLayer(unsigned char* canvas, unsigned char* diff, double gridRate, int brushSize,
	int threshold, int width, int height, unsigned char* reference)
{
	int gridSize = (int)(gridRate * brushSize);
	vector<Point> vec;
	for (int i = 0; i < height; i += gridSize)
	{
		for (int j = 0; j < width; j += gridSize)
		{
			double sumError = 0;
			double maxError = -10.0;
			int maxErrorIndX = 0;
			int maxErrorIndY = 0;

			for (int ni = -gridSize / 2; ni < gridSize / 2; ni++)
			{
				for (int nj = -gridSize / 2; nj < gridSize / 2; nj++)
				{
					int curY = i + ni, curX = j + nj;
					
					if (curX < 0 || curY < 0 || curY > height - 1 || curX > width - 1) continue;
					
					sumError += ((double)diff[curY * width + curX]);// / (gridSize*gridSize);
					
					if (diff[curY * width + curX] > maxError)
					{
						maxError = diff[curY * width + curX];
						maxErrorIndX = curX; maxErrorIndY = curY;
					}
				}
			}

			if (sumError > threshold)
			{
				vec.push_back(Point(maxErrorIndX, maxErrorIndY));
			}
		}
	}
	std::random_shuffle(vec.begin(), vec.end());
	m_pUI->setSize(brushSize);
	
	for (int i = 0; i < vec.size(); ++i)
	{
		if (m_pDoc->getPaintlyStroke() == STROKE_CURVEDBRUSH)
		{
			vector<Point> vp;
			vector<int> vr;
			vector<int> vg;
			vector<int> vb;
			makeCurved(vec[i], reference, brushSize, canvas, vp, vr, vg, vb);
			for (int i = 0; i < vp.size(); i++) 
			{
				CircleBrush* a = (CircleBrush*)m_pDoc->m_pCurrentBrush;
				a->DrawCircle(vp[0], vp[i], (((float)(brushSize*2))/3.0));
			}
		}
		else
		{
			m_pDoc->setBrushType(BRUSH_CIRCLES);
			m_pDoc->m_pCurrentBrush->BrushMove(vec[i], vec[i]);
		}
	}
}

void PaintView::paintlyPaint()
{
	int width = m_pDoc->m_nWidth;
	int height = m_pDoc->m_nHeight;
	
	unsigned char* canvas = m_pDoc->m_ucPainting;
	unsigned char* reference = new unsigned char[width*height*3];
	unsigned char* diff = new unsigned char[width*height];

	for (int i = 0; i < width*height*3; i++)
		canvas[i] = 0;
	
	int maxbrushSize = m_pDoc->getPaintlyMaxBrush();
	int minbrushSize = m_pDoc->getPaintlyMinBrush();
	double gridRate = m_pDoc->getPaintlyGrid();
	int threshold = m_pDoc->getPaintlyThreshold();
	int layers = m_pDoc->getPaintlyLayers();
	int brushSizeDecay = (maxbrushSize - minbrushSize) / layers;

	brushSizeDecay = brushSizeDecay > 1 ? brushSizeDecay : 1;

	for (int i = maxbrushSize; i >= minbrushSize; i-=brushSizeDecay) {
		paintlyBlur(m_pDoc->m_ucBitmap, reference, i, width, height);
		paintlyDiff(canvas, reference, diff, width, height);
		paintlyLayer(canvas, diff, gridRate, i, threshold, width, height, reference);
	}
	glFlush();
	m_pDoc->m_pUI->m_paintView->refresh();
}

void PaintView::makeCurved(const Point& start, unsigned char* reference, int brushSize, unsigned char* canvas, 
	vector<Point>& vP, vector<int>& vR, vector<int>& vG, vector<int>& vB)
{
	int startX = start.x;
	int startY = start.y;
	unsigned char strokeColorR = reference[(startY*m_nDrawWidth + startX) * 3];
	unsigned char strokeColorG = reference[(startY*m_nDrawWidth + startX) * 3 + 1];
	unsigned char strokeColorB = reference[(startY*m_nDrawWidth + startX) * 3 + 2];
	vP.push_back(Point(startX, startY));
	vR.push_back(strokeColorR);
	vG.push_back(strokeColorG);
	vB.push_back(strokeColorB);
	int x = startX, y = startY;
	int lastDx = 0, lastDy = 0;

	for (int i = 1; i <= m_pDoc->getPaintlyMaxStroke(); i++)
	{
		if (i > m_pDoc->getPaintlyMinStroke())
		{
			int diff1 = reference[(y*m_nDrawWidth + x) * 3] - canvas[(y*m_nDrawWidth + x) * 3]
				+ reference[(y*m_nDrawWidth + x) * 3 + 1] - canvas[(y*m_nDrawWidth + x) * 3 + 1]
				+ reference[(y*m_nDrawWidth + x) * 3 + 2] - canvas[(y*m_nDrawWidth + x) * 3 + 2];
			int diff2 = reference[(y*m_nDrawWidth + x) * 3] - strokeColorR
				+ reference[(y*m_nDrawWidth + x) * 3 + 1] - strokeColorG
				+ reference[(y*m_nDrawWidth + x) * 3 + 2] - strokeColorB;
			if (abs(diff1) < abs(diff2)) return;
		}

		int gradientX = (int)m_pDoc->GetGradientX(x, y);
		int gradientY = (int)m_pDoc->GetGradientY(x, y);

		int dx = -gradientY;
		int dy = gradientX;

		if (lastDx * dx + lastDy * dy < 0)
		{
			dx = -dx;
			dy = -dy;
		}
		if (dx == 0 && dy == 0) return;

		dx = m_pDoc->getPaintlyCurvature()*dx + lastDx*(1 - m_pDoc->getPaintlyCurvature());
		dy = m_pDoc->getPaintlyCurvature()*dy + lastDy*(1 - m_pDoc->getPaintlyCurvature());
		double tempX = (dx) / (sqrt((double)(dx*dx + dy*dy)));
		double tempY = (dy) / (sqrt((double)(dx*dx + dy*dy)));
		x = x + brushSize*tempX;
		y = y + brushSize*tempY;
		lastDx = dx;
		lastDy = dy;
		if (x < 0 || y < 0 || x > m_nDrawWidth-1 || y > m_nDrawHeight-1) return;
 		vP.push_back(Point(x, y));
		vR.push_back(strokeColorR);
		vG.push_back(strokeColorG);
		vB.push_back(strokeColorB);
	}
}