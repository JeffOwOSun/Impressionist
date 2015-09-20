//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>
#include <FL/Fl_Color_Chooser.H>

#include <math.h>
#include <iostream>
#include <stdlib.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen edge image
// This is called by the UI when the load edge image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_edge_image(Fl_Menu_ * o, void * v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadEdgeImage(newfile);
	}
}

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load mural image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_mural_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadMuralImage(newfile);
	}
}

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load gradient reference menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_another(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadAnother(newfile);
	}
}


//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//------------------------------------------------------------------
// Brings up a file chooser and then saves the chosen edge image
// This is called by the UI when the save edge image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_edge_image(Fl_Menu_ * o, void * v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->saveEdgeImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

//-------------------------------------------------------------
// Brings up the color chooser dialog
// This is called by the UI when the color chooser item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_color_window(Fl_Menu_* o, void* v)
{
	whoami(o)->m_colorWindow->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();

}



//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version for CS341, Spring 2002");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(int)v;
	pDoc->setBrushType(type);
}

void ImpressionistUI::cb_strokeDirectionChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;
	pDoc->setStrokeDirectionType(type);
}

void ImpressionistUI::cb_undo_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();
	unsigned char* m_tmp = pDoc->m_ucPainting;
	pDoc->m_ucPainting = pDoc->m_ucPainting_Undo;
	pDoc->m_ucPainting_Undo = m_tmp;
	pDoc->m_pUI->m_paintView->refresh();
	glFlush();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}


//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}

void ImpressionistUI::cb_widthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nWidth = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_angleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAngle = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	double value = ((Fl_Slider *)o)->value();
	int res = (int)(value * 255);
	((ImpressionistUI*)(o->user_data()))->m_nAlpha = res;
}

void ImpressionistUI::cb_color_chooser(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nColorR = ((Fl_Color_Chooser*)o)->r();
	((ImpressionistUI*)(o->user_data()))->m_nColorG = ((Fl_Color_Chooser*)o)->g();
	((ImpressionistUI*)(o->user_data()))->m_nColorB = ((Fl_Color_Chooser*)o)->b();
}
//---The light button callback for edge clipping---------------------
void ImpressionistUI::cb_EdgeClipping(Fl_Widget* o, void* v)
{
	ImpressionistUI *pUI = ((ImpressionistUI*)(o->user_data()));

	if (pUI->m_bEdgeClipping == TRUE) pUI->m_bEdgeClipping = FALSE;
	else {
		pUI->m_bEdgeClipping = TRUE;
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->CalculateEdgeMap(pUI->m_nEdgeThreshold);
	}
}
//-----------------------------------------------------------
// Called by UI to set the edge threshold when slider changed
//-----------------------------------------------------------
void ImpressionistUI::cb_EdgeThreshold(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nEdgeThreshold = int(((Fl_Slider *)o)->value());
}

//------------------------------------------------------------
// Extract edge
// Called by the UI when the EdgeExtraction button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_EdgeExtraction(Fl_Widget* o, void* v)
{
	ImpressionistUI *pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	//calculate the edge map according to the threshold
	pDoc->CalculateEdgeMap(pUI->m_nEdgeThreshold);
	pUI->m_origView->viewMode = OriginalView::EDGE_MODE;
	pUI->m_origView->refresh();
}

//------------------------------------------------------------
// Orig view
// Called by the UI when the Original View button is pressed
//------------------------------------------------------------
void ImpressionistUI::cb_orig_view(Fl_Menu_* o, void* v)
{
	ImpressionistUI *pUI = whoami(o);

	//display the orig image
	pUI->m_origView->viewMode = OriginalView::ORIG_MODE;
	pUI->m_origView->refresh();
}

void ImpressionistUI::cb_edge_view(Fl_Menu_* o, void* v)
{
	ImpressionistUI *pUI = whoami(o);

	pUI->m_origView->viewMode = OriginalView::EDGE_MODE;
	pUI->m_origView->refresh();
}

void ImpressionistUI::cb_swap_view(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();
	unsigned char* tmp = pDoc->m_ucBitmap;
	pDoc->m_ucBitmap = pDoc->m_ucPainting;
	pDoc->m_ucPainting = tmp;

	whoami(o)->m_origView->refresh();
	whoami(o)->m_paintView->refresh();
}

void ImpressionistUI::cb_filter_size(Fl_Menu_* o, void* v)
{
	whoami(o)->m_filterSizeWindow->show();
}

void ImpressionistUI::cb_filter_size_check(Fl_Widget* o, void* v)
{
	ImpressionistUI* db = (ImpressionistUI*)o->user_data();
	int w = atoi((db->m_filterWidth->value()));
	int h = atoi((db->m_filterHeight->value()));
	w = w > 1 ? w : 1;
	h = h > 1 ? h : 1;
	db->ShowFilterEntry(w, h);
	db->m_filterSizeWindow->hide();
}

void ImpressionistUI::cb_applyFilter(Fl_Widget* o, void* v)
{
	ImpressionistUI* db = (ImpressionistUI*)o->user_data();
	double* kernel = new double[db->m_nKernelH * db->m_nKernelW];
	for (int i = 0; i < db->m_nKernelH; ++i)
	{
		for (int j = 0; j < db->m_nKernelW; ++j)
		{
			int pixel = i * db->m_nKernelW + j;
			double weight = atof(db->m_EntryInputs[pixel]->value());
			//cout << weight << endl;
			kernel[pixel] = weight;
		}
	}
	
	db->getDocument()->applyCustomFilter(kernel, db->m_nKernelW, db->m_nKernelH);
	db->m_paintView->refresh();
}

void ImpressionistUI::ShowFilterEntry(int w, int h)
{
	int dialogWidth = w * 30 + (w + 1) * 10 + 20;
	int dialogHeight = h * 20 + (h + 1) * 10 + 40;
	m_filterEntryWindow = new Fl_Window(dialogWidth, dialogHeight, "Filter Kernel Entry");
	m_filterEntryWindow->user_data((void*)(this));
	for (int i = 1; i <= h; ++i)
	{
		for (int j = 1; j <= w; ++j)
		{
			Fl_Float_Input *input = new Fl_Float_Input(j * 10 + (j - 1) * 30, i * 10 + (i - 1) * 20, 30, 20, "");
			input->value("1.0");
			m_EntryInputs.push_back(input);
		}
	}
	

	m_filterEntryApply = new Fl_Button(dialogWidth / 2 - 30, dialogHeight - 30, 40, 20, "Apply");
	m_filterEntryApply->user_data((void*)(this));
	m_filterEntryApply->callback(cb_applyFilter);
	m_filterEntryWindow->end();
	m_filterEntryWindow->show();
	m_nKernelH = h;
	m_nKernelW = w;
}


void ImpressionistUI::cb_autoPaintSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAutoSpace = int(((Fl_Slider *)o)->value());
	//cout << int(((Fl_Slider *)o)->value()) << endl;
}

void ImpressionistUI::cb_autoPaintSize(Fl_Widget* o, void* v)
{
	ImpressionistUI *pUI = ((ImpressionistUI*)(o->user_data()));
	pUI->m_bAutoSizeVary = bool(((Fl_Light_Button *)o)->value());
}

void ImpressionistUI::cb_autoPaintApply(Fl_Widget* o, void* v)
{
	ImpressionistUI *pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc *pDoc = pUI->getDocument();
	pUI->m_paintView->TriggerAutoPaint();
}


void ImpressionistUI::cb_another_view(Fl_Menu_* o, void* v)
{
	ImpressionistUI *pUI = whoami(o);
	ImpressionistDoc *pDoc = pUI->getDocument();
	if (pDoc->m_ucAnother)
		pUI->m_origView->viewMode = OriginalView::ANOTHER_MODE;
	pUI->m_origView->refresh();
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
bool ImpressionistUI::getAutoVary()
{
	return m_bAutoSizeVary;
}

ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getStrokeDirection()
{
	return m_nStrokeDirection;
}

int ImpressionistUI::getSize()
{
	return m_nSize;
}

int ImpressionistUI::getWidth()
{
	return m_nWidth;
}

int ImpressionistUI::getAngle()
{
	return m_nAngle;
}

int ImpressionistUI::getAlpha()
{
	return m_nAlpha;
}

double ImpressionistUI::getColorR()
{
	return m_nColorR;
}

double ImpressionistUI::getColorG()
{
	return m_nColorG;
}

double ImpressionistUI::getColorB()
{
	return m_nColorB;
}

bool ImpressionistUI::getEdgeClipping()
{
	return m_bEdgeClipping;
}

int ImpressionistUI::getEdgeThreshold()
{
	return m_nEdgeThreshold;
}

int ImpressionistUI::getAutoPaintSpace()
{
	return m_nAutoSpace;
}
//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;

	if (size<=40) 
		m_BrushSizeSlider->value(m_nSize);
}

void ImpressionistUI::setWidth(int width)
{
	m_nWidth = width;
	if (width <= 40)
		m_BrushLineWidthSlider->value(m_nWidth);
}

void ImpressionistUI::setAngle(int angle)
{
	m_nAngle = angle;
	if (angle <= 359)
		m_BrushLineAngleSlider->value(m_nAngle);
}

void ImpressionistUI::setStrokeDirection(int type)
{
	m_nStrokeDirection = type;
}

void ImpressionistUI::setAlpha(int alpha) {
	m_nAlpha = alpha;
}

void ImpressionistUI::drawMarker(const Point& p) {
	m_origView->setDrawMarker(p);
}

void ImpressionistUI::setColorR(double R)
{
	m_nColorR = R;
}

void ImpressionistUI::setColorG(double G)
{
	m_nColorG = G;
}

void ImpressionistUI::setColorB(double B)
{
	m_nColorB = B;
}

void ImpressionistUI::setEdgeClipping(bool clipping) 
{
	m_bEdgeClipping = clipping;
}

void ImpressionistUI::setEdgeThreshold(int threshold) 
{
	m_nEdgeThreshold = threshold;
}

// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes }, 
		{ "&Clear Canvas",	FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
		{ "&Swap Views", FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cb_swap_view},
		{ "&Undo", FL_ALT + 'z', (Fl_Callback *)ImpressionistUI::cb_undo_canvas},
		{ "&Color palatte", FL_ALT + 'k', (Fl_Callback *)ImpressionistUI::cb_color_window, 0, FL_MENU_DIVIDER },
		{ "&Define Filter", FL_ALT + 'e', (Fl_Callback *)ImpressionistUI::cb_filter_size, 0, FL_MENU_DIVIDER},
		{ "Load Edge Image...", 0, (Fl_Callback *)ImpressionistUI::cb_load_edge_image },
		{ "Save Edge Image...", 0, (Fl_Callback *)ImpressionistUI::cb_save_edge_image, 0, FL_MENU_DIVIDER },
		{ "Load Mural Image...", 0, (Fl_Callback *)ImpressionistUI::cb_load_mural_image, 0, FL_MENU_DIVIDER },
		{ "Load Another Image...", 0, (Fl_Callback *)ImpressionistUI::cb_load_another, 0, FL_MENU_DIVIDER },
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },
	{ "&View", 0, 0, 0, FL_SUBMENU }, // change the view
		{ "&Original View", FL_ALT + 'o', (Fl_Callback *)ImpressionistUI::cb_orig_view },
		{ "&Edge View", FL_ALT + 'e', (Fl_Callback *)ImpressionistUI::cb_edge_view },
		{ "&Another View", FL_ALT + 'e', (Fl_Callback *)ImpressionistUI::cb_another_view },
		{ 0 },
	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "About", 0, (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {"Blur Filter", FL_ALT+'b', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_BLUR_FILTER},
  {"Sharpen Filter", FL_ALT+'s', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SHARPEN_FILTER},
  {0}
};

Fl_Menu_Item ImpressionistUI::strokeDirectionMenu[NUM_STROKE_DIR + 1] = {
		{ "&Slider/Right Mouse", FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_strokeDirectionChoice, (void*)DIR_SLIDER_OR_RIGHT_MOUSE },
		{ "&Gradient", FL_ALT+'g', (Fl_Callback*)ImpressionistUI::cb_strokeDirectionChoice, (void*)DIR_GRADIENT},
		{ "&Another Gradient", FL_ALT + 'a', (Fl_Callback*)ImpressionistUI::cb_strokeDirectionChoice, (void*)DIR_ANOTHER_GRADIENT },
		{ "&Brush Direction", FL_ALT + 'b', (Fl_Callback*)ImpressionistUI::cb_strokeDirectionChoice, (void*)DIR_BRUSH_DIRECTION },
		{0}
};



//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);
			m_paintView->m_pUI = this; //add UI pointer to paintView so that paintView can update attributes in UI

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// init values

	m_nSize = 10;
	m_nWidth = 1;
	m_nAngle = 0;
	m_nStrokeDirection = DIR_SLIDER_OR_RIGHT_MOUSE;
	m_nAlpha = 255;
	m_nColorR = 1.0;
	m_nColorG = 1.0;
	m_nColorB = 1.0;
	m_bEdgeClipping = false;
	m_nEdgeThreshold = 100;
	m_bAutoSizeVary = 0;
	m_nAutoSpace = 4;

	m_colorWindow = new Fl_Window(400, 325, "Color Dialog");
		m_colorChooser = new Fl_Color_Chooser(50, 20, 150, 150, "&Color Chooser");
		m_colorChooser->user_data((void*)(this));
		m_colorChooser->rgb(m_nColorR, m_nColorG, m_nColorB);
		m_colorChooser->callback(cb_color_chooser);
	m_colorWindow->end();
	
	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		// Accroding to the order, add stroke direction controller
		m_StrokeDirectionChoice = new Fl_Choice(114,40,150,25,"&Stroke Direction");
		m_StrokeDirectionChoice->user_data((void*)(this)); // record self to be used by static callback functions
		m_StrokeDirectionChoice->menu(strokeDirectionMenu);
		m_StrokeDirectionChoice->callback(cb_strokeDirectionChoice);
		m_StrokeDirectionChoice->deactivate();

		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);

		// Add brush size slider to the dialog
		m_BrushLineWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width");
		m_BrushLineWidthSlider->user_data((void*)(this));
		m_BrushLineWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushLineWidthSlider->labelfont(FL_COURIER);
		m_BrushLineWidthSlider->labelsize(12);
		m_BrushLineWidthSlider->minimum(1);
		m_BrushLineWidthSlider->maximum(40);
		m_BrushLineWidthSlider->step(1);
		m_BrushLineWidthSlider->value(m_nWidth);
		m_BrushLineWidthSlider->align(FL_ALIGN_RIGHT);
		m_BrushLineWidthSlider->callback(cb_widthSlides);
		m_BrushLineWidthSlider->deactivate();

		// Add brush angle slider to the dialog
		m_BrushLineAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle");
		m_BrushLineAngleSlider->user_data((void*)(this));
		m_BrushLineAngleSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushLineAngleSlider->labelfont(FL_COURIER);
		m_BrushLineAngleSlider->labelsize(12);
		m_BrushLineAngleSlider->minimum(0);
		m_BrushLineAngleSlider->maximum(359);
		m_BrushLineAngleSlider->step(1);
		m_BrushLineAngleSlider->value(m_nAngle);
		m_BrushLineAngleSlider->align(FL_ALIGN_RIGHT);
		m_BrushLineAngleSlider->callback(cb_angleSlides);
		m_BrushLineAngleSlider->deactivate();

		m_BrushAlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha");
		m_BrushAlphaSlider->user_data((void*)(this));
		m_BrushAlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAlphaSlider->labelfont(FL_COURIER);
		m_BrushAlphaSlider->labelsize(12);
		m_BrushAlphaSlider->minimum(0);
		m_BrushAlphaSlider->maximum(1.00);
		m_BrushAlphaSlider->step(0.01);
		m_BrushAlphaSlider->value(1); //not m_nAlpha because scale difference
		m_BrushAlphaSlider->align(FL_ALIGN_RIGHT);
		m_BrushAlphaSlider->callback(cb_alphaSlides);

		//------Button for auto painting
		m_autoPaintSpaceSlider = new Fl_Value_Slider(10, 240, 120, 20, "Space Size");
		m_autoPaintSpaceSlider->user_data((void*)(this));
		m_autoPaintSpaceSlider->type(FL_HOR_NICE_SLIDER);
		m_autoPaintSpaceSlider->labelfont(FL_COURIER);
		m_autoPaintSpaceSlider->labelsize(12);
		m_autoPaintSpaceSlider->minimum(1);
		m_autoPaintSpaceSlider->maximum(15);
		m_autoPaintSpaceSlider->step(1);
		m_autoPaintSpaceSlider->value(4);
		m_autoPaintSpaceSlider->align(FL_ALIGN_RIGHT);
		m_autoPaintSpaceSlider->callback(cb_autoPaintSlides);


		m_autoPaintRandSize = new Fl_Light_Button(210, 240, 100, 20, "Randomly");
		m_autoPaintRandSize->user_data((void*)(this));   // record self to be used by static callback functions
		m_autoPaintRandSize->callback(cb_autoPaintSize);

		m_autoPaintApply = new Fl_Button(320, 240, 60, 20, "Paint");
		m_autoPaintApply->user_data((void*)(this));
		m_autoPaintApply->callback(cb_autoPaintApply);

		//------


		//---To install a light button for edge clipping---------------------
		m_EdgeClipping = new Fl_Light_Button(10, 200, 150, 25, "Edge Clipping");
		m_EdgeClipping->user_data((void*)(this));   // record self to be used by static callback functions
		m_EdgeClipping->callback(cb_EdgeClipping);

		//Slider for edge Threshold
		m_EdgeThreshold = new Fl_Value_Slider(10, 280, 200, 20, "Edge Threshold");
		m_EdgeThreshold->user_data((void*)(this));
		m_EdgeThreshold->type(FL_HOR_NICE_SLIDER);
		m_EdgeThreshold->labelfont(FL_COURIER);
		m_EdgeThreshold->labelsize(12);
		m_EdgeThreshold->minimum(0);
		m_EdgeThreshold->maximum(500);
		m_EdgeThreshold->step(1);
		m_EdgeThreshold->value(m_nEdgeThreshold); //not m_nAlpha because scale difference
		m_EdgeThreshold->align(FL_ALIGN_RIGHT);
		m_EdgeThreshold->callback(cb_EdgeThreshold);

		//Button for extrating edge
		m_ClearCanvasButton = new Fl_Button(320, 280, 60, 20, "Do it!");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_EdgeExtraction);

    m_brushDialog->end();	


	m_filterSizeWindow = new Fl_Window(300, 80, "Filter Size");
		m_filterWidth = new Fl_Int_Input(80, 10, 50, 20, "Width");
			m_filterWidth->labelfont(FL_COURIER);
			m_filterWidth->labelsize(12);
			m_filterWidth->value("1");
		m_filterHeight = new Fl_Int_Input(180, 10, 60, 20, "Height");
			m_filterHeight->labelfont(FL_COURIER);
			m_filterHeight->labelsize(12);
			m_filterHeight->value("1");
		m_filterSizeApply = new Fl_Button(120, 40, 80, 20, "&OK");
			m_filterSizeApply->user_data((void*)(this));
			m_filterSizeApply->callback(cb_filter_size_check);
	m_filterSizeWindow->end();

}
