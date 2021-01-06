#ifndef _MATHS_H
#define _MATHS_H

#include <libps.h>

#define INFINITY 1e+300

static MATRIX MtxIdentity;

void MatrixInit(MATRIX* mtx);

static u_long min(u_long value1, u_long value2)
{
    return value1 < value2 ? value1 : value2;
}

static u_long max(u_long value1, u_long value2)
{
    return value1 > value2 ? value1 : value2;
}

static float fmin(float value1, float value2)
{
    return value1 < value2 ? value1 : value2;
}

static float fmax(float value1, float value2)
{
    return value1 > value2 ? value1 : value2;
}

static int RandRanged(int minVal, int maxVal)
{
    return minVal + (rand() % (maxVal - minVal));
}

#endif // _MATHS_H
