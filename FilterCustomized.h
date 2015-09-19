#ifndef FILTER_CUSTOM_H
#define FILTER_CUSTOM_H

#include "ImpBrush.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"


class FilterCustomized
{
public:
	FilterCustomized(const double* kernel, const int kernel_w, const int kernel_h);
	~FilterCustomized();

	double applyCustomizedFilter(GLubyte* image, const int x, const int y, const int img_w, const int img_h, int channel);
	
private:
	double* m_kernel;
	int m_kernel_w;
	int m_kernel_h;
};

#endif