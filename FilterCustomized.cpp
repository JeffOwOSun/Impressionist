#include "FilterCustomized.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <iostream>
using namespace std;

FilterCustomized::FilterCustomized(const double* kernel, const int kernel_w, const int kernel_h)
{
	m_kernel = new double[kernel_w * kernel_h];
	m_kernel_w = kernel_w;
	m_kernel_h = kernel_h;
	memcpy(m_kernel, kernel, kernel_w*kernel_h*sizeof(double));
}

FilterCustomized::~FilterCustomized()
{
	delete[] m_kernel;
}

double FilterCustomized::applyCustomizedFilter(GLubyte* image, const int x, const int y, const int img_w, const int img_h, int channel)
{
	// Paint from left lower corner
	int startX = x - m_kernel_w / 2;
	int startY = y - m_kernel_h / 2;
	double res = 0.0;
	double sum_weight = 0.0;
	for (int i = 0; i < m_kernel_h; ++i)
	{
		const int currentY = i + startY;
		if (currentY < 0 || currentY >= img_h) continue;
		
		for (int j = 0; j < m_kernel_w; ++j)
		{
			const int currentX = j + startX;
			if (currentX < 0 || currentX >= img_w) continue;
			res += (double)(image[(currentY*img_w + currentX) * 3 + channel]) * m_kernel[i*m_kernel_w + j];
			sum_weight += m_kernel[i*m_kernel_w + j];
			//cout << "So result is " << res << "And sum weight is " << sum_weight << endl;
		}
	}

	if (sum_weight == 0) sum_weight = 1;
	if (sum_weight < 0) sum_weight = -sum_weight;

	return (res/sum_weight);
}