//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "originalview.h"
#include <algorithm>

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif

OriginalView::OriginalView(int			x, 
						   int			y, 
						   int			w, 
						   int			h, 
						   const char*	l)
							: viewMode(ORIG_MODE), Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}

void OriginalView::draw()
{
	if(!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer( GL_FRONT );
		ortho();

	}

	glClear( GL_COLOR_BUFFER_BIT );

	if ( m_pDoc->m_ucBitmap ) 
	{
		// note that both OpenGL pixel storage and the Windows BMP format
		// store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
		// around with startrow.

		m_nWindowWidth=w();
		m_nWindowHeight=h();

		int drawWidth, drawHeight;
		GLvoid* bitstart;

		// we are not using a scrollable window, so ignore it
		Point scrollpos;	// = GetScrollPosition();
		scrollpos.x=scrollpos.y=0;

		drawWidth	= min( m_nWindowWidth, m_pDoc->m_nWidth );
		drawHeight	= min( m_nWindowHeight, m_pDoc->m_nHeight );

		int	startrow	= m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
		if ( startrow < 0 ) 
			startrow = 0;

		


		

		// just copy image to GLwindow conceptually
		glRasterPos2i( 0, m_nWindowHeight - drawHeight );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth );
		glDrawBuffer( GL_BACK );
		// decide which image to use
		switch (viewMode)
		{
		case ORIG_MODE:
			bitstart = m_pDoc->m_ucBitmap +3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);
			glDrawPixels(drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart);
			break;
		case EDGE_MODE:
			bitstart = m_pDoc->m_ucEdge + ((m_pDoc->m_nWidth * startrow) + scrollpos.x);
			glDrawPixels(drawWidth, drawHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, bitstart);
		}
		drawMarker();
	}
			
	glFlush();
}

void OriginalView::refresh()
{
	redraw();
}

void OriginalView::resizeWindow(int width, int height)
{
	Fl_Gl_Window::resize(x(), y(), width, height);
}

void OriginalView::resize(int x, int y, int width, int height)
{
	Fl_Gl_Window::resize(x, y, w(), h());
}

void OriginalView::setDrawMarker(const Point& p)
{
	markerPoint = p;
	redraw();
}

void OriginalView::drawMarker()
{
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0);
	glVertex2d(markerPoint.x - 5, m_nWindowHeight - markerPoint.y);
	glVertex2d(markerPoint.x + 5, m_nWindowHeight - markerPoint.y);
	glVertex2d(markerPoint.x, m_nWindowHeight - markerPoint.y - 4);
	glVertex2d(markerPoint.x, m_nWindowHeight - markerPoint.y + 6);
	glEnd();
}
