//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>
#include <vector>

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;
								
	PaintView*			m_paintView;
	OriginalView*		m_origView;

// for brush dialog
	Fl_Window*			m_brushDialog;
	Fl_Window*			m_colorWindow;
	Fl_Window*			m_filterSizeWindow;
	Fl_Window*			m_filterEntryWindow;
	Fl_Int_Input*		m_filterWidth;
	Fl_Int_Input*		m_filterHeight;
	Fl_Button*			m_filterSizeApply;
	Fl_Button*			m_filterEntryApply;

	// ------ For auto painting
	Fl_Slider*			m_autoPaintSpaceSlider;
	Fl_Light_Button*	m_autoPaintRandSize;
	Fl_Button*			m_autoPaintApply;
	// ------

	Fl_Choice*			m_BrushTypeChoice;
	Fl_Color_Chooser*	m_colorChooser;

	// implement four ways to contol the stroke direction
	// 1. Use slider value to control angle of lines
	// 2. Use Right Mouse-dragged lines to indicate the angle
	// 3. Use the direction of mouse moving
	// 4. Use the gradient to control Stroke direction
	Fl_Choice*			m_StrokeDirectionChoice;
	
	Fl_Slider*			m_BrushSizeSlider;
	Fl_Slider*			m_BrushLineWidthSlider;
	Fl_Slider*			m_BrushLineAngleSlider;
	Fl_Slider*			m_BrushAlphaSlider;
	Fl_Button*          m_ClearCanvasButton;
	Fl_Light_Button*	m_EdgeClipping;
	Fl_Slider*			m_EdgeThreshold;
	Fl_Button*			m_EdgeExtraction;
	std::vector<Fl_Float_Input*> m_EntryInputs;

	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);

	// Interface to get attribute
	int					getStrokeDirection();
	void				setStrokeDirection(int type);

	int					getSize();
	void				setSize(int size);

	int					getWidth();
	void				setWidth(int width);

	int					getAngle();
	void				setAngle(int angle);

	int					getAlpha();
	void				setAlpha(int alpha);

	double				getColorR();
	void				setColorR(double R);
	double				getColorG();
	void				setColorG(double G);
	double				getColorB();
	void				setColorB(double B);
	void				drawMarker(const Point& p);
	bool				getEdgeClipping();
	void				setEdgeClipping(bool clipping);
	int					getAutoPaintSpace();
	bool				getAutoVary();

	int					getEdgeThreshold();
	void				setEdgeThreshold(int edgeThreshold);

	void				ShowFilterEntry(int w, int h);

private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

	// All attributes here
	int		m_nStrokeDirection;
	int		m_nSize;
	int		m_nWidth;
	int		m_nAngle;
	int		m_nAlpha;
	double	m_nColorR;
	double  m_nColorG;
	double  m_nColorB;
	bool	m_bEdgeClipping;
	int		m_nEdgeThreshold;
	int		m_nKernelH;
	int		m_nKernelW;
	int		m_nAutoSpace;
	bool	m_bAutoSizeVary;

	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE + 1];
	static Fl_Menu_Item		strokeDirectionMenu[NUM_STROKE_DIR + 1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void cb_color_window(Fl_Menu_* o, void* v);
	static void cb_color_chooser(Fl_Widget* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_widthSlides(Fl_Widget* o, void* v);
	static void cb_angleSlides(Fl_Widget* o, void* v);
	static void cb_alphaSlides(Fl_Widget* o, void* v);
	static void cb_strokeDirectionChoice(Fl_Widget* o, void* v);
	static void cb_undo_canvas(Fl_Menu_* o, void* v);
	static void cb_EdgeClipping(Fl_Widget* o, void* v);
	static void cb_EdgeThreshold(Fl_Widget* o, void* v);
	static void cb_EdgeExtraction(Fl_Widget* o, void* v);
	static void cb_orig_view(Fl_Menu_* o, void* v);
	static void cb_edge_view(Fl_Menu_* o, void* v);
	static void cb_swap_view(Fl_Menu_* o, void* v);
	static void cb_filter_size(Fl_Menu_* o, void* v);
	static void cb_filter_size_check(Fl_Widget* o, void* v);
	static void cb_applyFilter(Fl_Widget* o, void* v);
	static void cb_load_edge_image(Fl_Menu_ * o, void * v);
	static void cb_save_edge_image(Fl_Menu_ * o, void * v);
	static void cb_load_mural_image(Fl_Menu_ * o, void * v);
	static void cb_autoPaintSlides(Fl_Widget* o, void* v);
	static void cb_autoPaintSize(Fl_Widget*o, void* v);
	static void cb_autoPaintApply(Fl_Widget* o, void* v);
	static void cb_another_view(Fl_Menu_* o, void* v);
	static void cb_load_another(Fl_Menu_ *o, void * v);
	static void cb_load_dissolve(Fl_Menu_ *o, void* v);
	static void cb_load_alpha_brush(Fl_Menu_ *o, void * v);
	static void cb_start_collage(Fl_Menu_ *o, void * v);
};

#endif
