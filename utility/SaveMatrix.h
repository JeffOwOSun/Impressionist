#ifndef SAVEMATRIX_H
#define SAVEMATRIX_H

#include<string>
#include<iostream>
#include<fstream>
using namespace std;

template <typename ele>
int saveMatrix(const char* name, const ele* matrix, int width, int height)
{
	ofstream myFS;
	myFS.open(name);
	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			int idx = j * width + i;
			myFS << (int)matrix[idx];
			if (i < width - 1) myFS << " ";
		}
		myFS << "\n";
	}
	myFS.close();
	return 0;
}

#endif SAVEMATRIX_H