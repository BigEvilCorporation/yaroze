#include "Maths.h"

void MatrixInit(MATRIX* mtx)
{
    mtx->t[0] = 0;
    mtx->t[1] = 0;
    mtx->t[2] = 0;
    mtx->m[0][0] = ONE;
    mtx->m[0][1] = 0;
    mtx->m[0][2] = 0;
    mtx->m[1][0] = 0;
    mtx->m[1][1] = ONE;
    mtx->m[1][2] = 0;
    mtx->m[2][0] = 0;
    mtx->m[2][1] = 0;
    mtx->m[2][2] = ONE;
}
