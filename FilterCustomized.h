#ifndef FILTER_CUSTOM_H
#define FILTER_CUSTOM_H

class FilterCustomized
{
public:
	FilterCustomized(const double* kernel, const int kernel_w, const int kernel_h);
	~FilterCustomized();

	double applyCustomizedFilter(const int* image, const int x, const int y, const int img_w, const int img_h, int channel);

private:
	double* m_kernel;
	int m_kernel_w;
	int m_kernel_h;
};

#endif