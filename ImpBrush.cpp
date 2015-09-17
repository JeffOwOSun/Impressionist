//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"
#include <iostream>
#include <algorithm>

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc,
				   char*				name) :
					m_pDoc(pDoc),
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();

	GLubyte color[4];

	memcpy(color, pDoc->GetOriginalPixel(source), 3);
	double chosen_R = pDoc->getColorR();
	double chosen_G = pDoc->getColorG();
	double chosen_B = pDoc->getColorB();
	
	int R = (int)(chosen_R * 255);
	int G = (int)(chosen_G * 255);
	int B = (int)(chosen_B * 255);

	// std::cout << "a" << (int)color[0] << " " << (int)color[1] << " " << (int)color[2] << std::endl;
	// std::cout << "RGB: " << pDoc->getColorR() << " " << G << " " << B << std::endl;
	color[0] = ((int)color[0] * 0.7) + R * 0.3;
	color[1] = ((int)color[1] * 0.7) + G * 0.3;
	color[2] = ((int)color[2] * 0.7) + B * 0.3;
	// std::cout << (int)color[0] << " " << (int)color[1] << " " << (int)color[2] << std::endl;
	color[3] = pDoc->getAlpha();
	glColor4ubv(color);
}