#include"Filter.h"

GLint SobelX[9] = { -1, 0, 1,
-2, 0, 2,
-1, 0, 1 }; //NOTE: Y from bottom to top

GLint SobelY[9] = { -1, -2, -1,
0, 0, 0,
1, 2, 1 };

GLdouble Gaussian[9] = { 0.0625, 0.125, 0.0625,
0.125, 0.250, 0.125,
0.0625, 0.125, 0.0625 };
