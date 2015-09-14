#ifndef FILTER_H
#define FILTER_H

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
	return GLubyte(0.299 * (float)r + 0.587 * (float)g + 0.144 * (float)b);
}

inline GLubyte filterIntensity(GLubyte* pixel, int num_channel=3)
{
	return filterIntensity(pixel[0], pixel[1], pixel[2]);
}

/**
 * \brief Apply a Given Filter
 */
inline GLubyte applyFilter(const GLubyte* filter, int filterW, int filterH, const GLubyte* source, int sourceW, int sourceH, int x, int y)
{
	int alignX = x - (filterW / 2);
	int alignY = y - (filterH / 2);

	int result = 0;
	for (int i = 0; i < filterW; ++i)
	{
		for (int j = 0; j < filterH; ++j)
		{
			//convert coordinate to pos to beginning
			int filterPos = j * filterW + i;
			int sourcePos = (j + alignY) * sourceW + (i + alignX);
			result += filter[filterPos] * source[sourcePos];
		}
	}

	return (GLubyte)result;
}


#endif //FILTER.H