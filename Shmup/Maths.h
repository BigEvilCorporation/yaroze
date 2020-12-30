#ifndef _MATHS_H
#define _MATHS_H

static u_long min(u_long value1, u_long value2)
{
    return value1 < value2 ? value1 : value2;
}

static u_long max(u_long value1, u_long value2)
{
    return value1 > value2 ? value1 : value2;
}

static int RandRanged(int minVal, int maxVal)
{
    return minVal + (rand() % (maxVal - minVal));
}

#endif // _MATHS_H
