#ifndef FILTER_H
#define FILTER_H

#include<cmath>
#include"FL/gl.h"

/**
 * \brief Extract Pixel from a given source and index;
 */

inline GLubyte* extractPixel(const GLubyte* source, int num_channel, int idx)
{
	GLubyte* pixel = new GLubyte[num_channel];
	memcpy(pixel, (const void*)source[idx*num_channel], num_channel);
	return pixel;
}

/**
 * \brief Convert RGB image to Intensity Grey Image
 */
inline GLubyte filterIntensity(GLubyte r, GLubyte g, GLubyte b)
{
	return (GLubyte)round(0.299 * (float)r + 0.587 * (float)g + 0.144 * (float)b);
}

inline GLubyte filterIntensity(GLubyte* pixel, int num_channel=3)
{
	return filterIntensity(pixel[0], pixel[1], pixel[2]);
}

/**
 * \brief Apply a Given Filter at a given spot
 * \param source The source image. Assume single color
 */
inline GLint applyFilter(const GLint* filter, int filterW, int filterH, const GLubyte* source, int sourceW, int sourceH, int x, int y)
{
	//the origin of the convolution on source
	int alignX = x - (filterW / 2);
	int alignY = y - (filterH / 2);

	int result = 0;

	for (int i = 0; i < filterW; ++i)
	{
		for (int j = 0; j < filterH; ++j)
		{
			// use reflection to do padding.
			// abs takes care of negative overflow
			// x y are in source coord space
			// i j are in filter coord space
			int x = (i + alignX < sourceW) ? abs(i + alignX) : 2 * (sourceW - 1) - (i + alignX);
			int y = (i + alignY < sourceH) ? abs(i + alignY) : 2 * (sourceH - 1) - (i + alignY);

			//convert coordinate to index in 1D
			int filterPos = j * filterW + i; //According to blue book, pixels are stored row-first
			int sourcePos = y * sourceW + x;

			result += filter[filterPos] * source[sourcePos];
		}
	}
	return result;
}


#endif //FILTER.H