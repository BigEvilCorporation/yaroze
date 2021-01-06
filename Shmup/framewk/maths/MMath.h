#ifndef MMATH_HEADER_FILE
#define MMATH_HEADER_FILE

/* MMath.h -- MMath library file
 *
 * Copyright (C) 2017 Zachary Wells
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#if defined PLATFORM_YAROZE
#include <libps.h>
#include "Maths.h"
#else
#include <math.h>
#include <memory.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

	#define MMATH_INLINE static inline
	#define MMATH_CONST static const

	//Types
	#pragma region Types
	#if defined(MMATH_DOUBLE)
	typedef double scalar;
	#else
	typedef float scalar;
	#endif

	//typedef struct vec2_s {
		typedef union {
			scalar data[2];
			struct { scalar x, y; } v;
			//struct { scalar g, a; };
		} vec2;
	//} vec2;
	//typedef struct vec3_s {
		typedef union {
			scalar data[3];
			struct { scalar x, y, z; } v;
			struct { scalar r, g, b; } c;
		} vec3;
	//} vec3;
	//typedef struct vec4_s {
		typedef union {
			scalar data[4];
			struct { scalar x, y, z, w; } v;
			struct { scalar r, g, b, a; } c;
		} vec4;
	//} vec4;

	//typedef struct quat_s {
		typedef union {
			scalar data[4];
			vec4 vec;
			vec3 axis;
			struct { float x, y, z, w; } v;
		} quat;
	//} quat;

	//typedef struct mat2_s {
		typedef union {
			scalar data[2 * 2];
			vec2 row[2];
			struct {
				scalar x0, y0;
				scalar x1, y1;
			} m;
			struct {
				vec2 r0;
				vec2 r1;
			} v;
		} mat2;
	//} mat2;
	//typedef struct mat3_s {
		typedef union {
			scalar data[3 * 3];
			vec3  row[3];
			struct {
				scalar x0, y0, z0;
				scalar x1, y1, z1;
				scalar x2, y2, z2;
			} m;
			struct {
				vec3 r0;
				vec3 r1;
				vec3 r2;
			} v;
		} mat3;
	//} mat3;
	//typedef struct mat4_s {
		typedef union {
			scalar data[4 * 4];
			vec4 row[4];
			struct {
				scalar x0, y0, z0, w0;
				scalar x1, y1, z1, w1;
				scalar x2, y2, z2, w2;
				scalar x3, y3, z3, w3;
			} m;
			struct {
				vec4 r0;
				vec4 r1;
				vec4 r2;
				vec4 r3;
			} v;
		} mat4;
	//} mat4;

	typedef struct transform_t {
		vec3 pos;
		vec3 scale;
		quat rot;
	} transform;
	#pragma endregion Types

	#pragma region Helper_Functions
	//Yaroze lib only declares double versions
	#if defined(MMATH_DOUBLE) || defined(PLATFORM_YAROZE)
	#define mm_sqrt(var) (sqrt(var))
	#define mm_acos(var) (acos(var))
	#define mm_asin(var) (asin(var))
	#define mm_atan(var) (atan(var))
	#define mm_cos(var)  (cos(var))
	#define mm_sin(var)  (sin(var))
	#define mm_tan(var)  (tan(var))
	#define mm_min(x, y) (fmin(x, y))
	#define mm_max(x, y) (fmax(x, y))
	#define mm_abs(var)  (fabs(var))
	#else
	#define mm_sqrt(var) (sqrtf(var))
	#define mm_acos(var) (acosf(var))
	#define mm_asin(var) (asinf(var))
	#define mm_atan(var) (atanf(var))
	#define mm_cos(var)  (cosf(var))
	#define mm_sin(var)  (sinf(var))
	#define mm_tan(var)  (tanf(var))
	#define mm_min(x, y) (fminf(x, y))
	#define mm_max(x, y) (fmaxf(x, y))
	#define mm_abs(var)  (fabsf(var))
	#endif

	//constants
	#define mm_dpi ((scalar)6.283185307179586) //double pi
	#define mm_pi  ((scalar)3.141592653589793) //pi
	#define mm_hpi ((scalar)1.570796326794896) //half pi

	//Functions
	MMATH_INLINE scalar radians(scalar degrees) {
		return degrees * (scalar)0.0174532925199432; //PI / 180
	}
	MMATH_INLINE scalar degrees(scalar radians) {
		return radians * (scalar)57.295779513082325; //180 / PI
	}
	#pragma endregion Helper_Functions

	//Vector Math
	#pragma region Vec_TempDef
	#define VEC_FOR_BEGIN(integer) { int i; for (i = 0; i < integer; i++)
	#define VEC_FOR_END }
	#define MMATH_GENFUNC_VECNEGATE(integer) \
	MMATH_INLINE void vec##integer##Negate(vec##integer *dest, const vec##integer *a) { \
		VEC_FOR_BEGIN(integer) { \
			dest->data[i] = -a->data[i]; \
		} \
		VEC_FOR_END \
	}
	#define MMATH_GENFUNC_VECABS(integer) \
	MMATH_INLINE void vec##integer##Abs(vec##integer *dest, const vec##integer *a) { \
		VEC_FOR_BEGIN(integer) { \
			dest->data[i] = mm_abs(a->data[i]); \
		} \
		VEC_FOR_END \
	}
	#define MMATH_GENFUNC_VECADDSCALAR(integer) \
	MMATH_INLINE void vec##integer##AddScalar(vec##integer *dest, const vec##integer *a, scalar b) { \
		VEC_FOR_BEGIN(integer) { \
			dest->data[i] = a->data[i] + b; \
		} \
		VEC_FOR_END \
	}
	#define MMATH_GENFUNC_VECSUBSCALAR(integer) \
	MMATH_INLINE void vec##integer##SubScalar(vec##integer *dest, const vec##integer *a, scalar b) { \
		VEC_FOR_BEGIN(integer) { \
			dest->data[i] = a->data[i] - b; \
		} \
		VEC_FOR_END \
	}
	#define MMATH_GENFUNC_VECMULSCALAR(integer) \
	MMATH_INLINE void vec##integer##MulScalar(vec##integer *dest, const vec##integer *a, scalar b) { \
		VEC_FOR_BEGIN(integer) { \
			dest->data[i] = a->data[i] * b; \
		} \
		VEC_FOR_END \
	}
	#define MMATH_GENFUNC_VECDIVSCALAR(integer) \
	MMATH_INLINE void vec##integer##DivScalar(vec##integer *dest, const vec##integer *a, scalar b) { \
		VEC_FOR_BEGIN(integer) { \
			dest->data[i] = a->data[i] / b; \
		} \
		VEC_FOR_END \
	}
	#define MMATH_GENFUNC_VECADD(integer) \
	MMATH_INLINE void vec##integer##Add(vec##integer *dest, const vec##integer *a, const vec##integer *b) { \
		VEC_FOR_BEGIN(integer) { \
			dest->data[i] = a->data[i] + b->data[i]; \
		} \
		VEC_FOR_END \
	}
	#define MMATH_GENFUNC_VECSUB(integer) \
	MMATH_INLINE void vec##integer##Sub(vec##integer *dest, const vec##integer *a, const vec##integer *b) { \
		VEC_FOR_BEGIN(integer) { \
			dest->data[i] = a->data[i] - b->data[i]; \
		} \
		VEC_FOR_END \
	}
	#define MMATH_GENFUNC_VECMUL(integer) \
	MMATH_INLINE void vec##integer##Mul(vec##integer *dest, const vec##integer *a, const vec##integer *b) { \
		VEC_FOR_BEGIN(integer) { \
			dest->data[i] = a->data[i] * b->data[i]; \
		} \
		VEC_FOR_END \
	}
	#define MMATH_GENFUNC_VECDIV(integer) \
	MMATH_INLINE void vec##integer##Div(vec##integer *dest, const vec##integer *a, const vec##integer *b) { \
		VEC_FOR_BEGIN(integer) { \
			dest->data[i] = a->data[i] / b->data[i]; \
		} \
		VEC_FOR_END \
	}
	#define MMATH_GENFUNC_VECDOT(integer) \
	MMATH_INLINE scalar vec##integer##Dot(const vec##integer *a, const vec##integer *b) { \
		scalar ret = 0; \
		VEC_FOR_BEGIN(integer) { \
			ret += a->data[i] * b->data[i]; \
		} \
		VEC_FOR_END \
		return ret; \
	}
	#define MMATH_GENFUNC_VECLEN(integer) \
	MMATH_INLINE scalar vec##integer##Length(const vec##integer *a) { \
		scalar sum = 0; \
		VEC_FOR_BEGIN(integer) { \
			sum += a->data[i] * a->data[i]; \
		} \
		VEC_FOR_END \
		return mm_sqrt(sum); \
	}
	#define MMATH_GENFUNC_VECDIST(integer) \
	MMATH_INLINE scalar vec##integer##Distance(const vec##integer *a, const vec##integer *b) { \
		vec##integer dir; \
		vec##integer##Sub(&dir, b, a); \
		return vec##integer##Length(&dir); \
	}
	#define MMATH_GENFUNC_VECNORM(integer) \
	MMATH_INLINE void vec##integer##Normalize(vec##integer *dest, const vec##integer *a) { \
		scalar len = vec##integer##Length(a); \
		if (len == 0) { \
			return; \
		} \
		len = 1.f / len; \
		VEC_FOR_BEGIN(integer) { \
			dest->data[i] = a->data[i] * len; \
		VEC_FOR_END \
		} \
	}
	#define MMATH_GENFUNC_VECLERP(integer) \
	MMATH_INLINE void vec##integer##Lerp(vec##integer *dest, const vec##integer *f, const vec##integer *l, scalar t) { \
		vec##integer temp1; \
		vec##integer temp2; \
		vec##integer##Sub(&temp1, l, f); \
		vec##integer##MulScalar(&temp2, &temp1, t); \
		vec##integer##Add(dest, &temp2, f); \
	}
	#define MMATH_GENFUNC_VECSTANDARD(integer) \
		MMATH_GENFUNC_VECADDSCALAR(integer) \
		MMATH_GENFUNC_VECSUBSCALAR(integer) \
		MMATH_GENFUNC_VECMULSCALAR(integer) \
		MMATH_GENFUNC_VECDIVSCALAR(integer) \
		MMATH_GENFUNC_VECADD(integer); \
		MMATH_GENFUNC_VECSUB(integer); \
		MMATH_GENFUNC_VECMUL(integer); \
		MMATH_GENFUNC_VECDIV(integer); \
		MMATH_GENFUNC_VECDOT(integer); \
		MMATH_GENFUNC_VECLEN(integer); \
		MMATH_GENFUNC_VECDIST(integer); \
		MMATH_GENFUNC_VECNORM(integer); \
		MMATH_GENFUNC_VECLERP(integer); \
		MMATH_GENFUNC_VECNEGATE(integer); \
		MMATH_GENFUNC_VECABS(integer);
	#pragma endregion Vec_TempDef

	#pragma region Vec2_Functions
	MMATH_GENFUNC_VECSTANDARD(2);
	MMATH_CONST vec2 vec2Zero     = { 0, 0 };
	MMATH_CONST vec2 vec2Identity = { 1, 1 };
	MMATH_INLINE void vec2ToVec3(vec3 *dest, const vec2 *a, scalar z) {
		dest->v.x = a->v.x;
		dest->v.y = a->v.y;
		dest->v.z = z;
	}
	MMATH_INLINE void vec2ToVec4(vec4 *dest, const vec2 *a, scalar z, scalar w) {
		dest->v.x = a->v.x;
		dest->v.y = a->v.y;
		dest->v.z = z;
		dest->v.w = w;
	}
	#pragma endregion Vec2_Functions

	#pragma region Vec3_Functions
	MMATH_GENFUNC_VECSTANDARD(3);
	MMATH_CONST vec3 vec3Zero     = { 0, 0, 0 };
	MMATH_CONST vec3 vec3Identity = { 1, 1, 1 };
	MMATH_CONST vec3 vec3XAxis    = { 1, 0, 0 };
	MMATH_CONST vec3 vec3YAxis    = { 0, 1, 0 };
	MMATH_CONST vec3 vec3ZAxis    = { 0, 0, 1 };
	MMATH_INLINE vec2 vec3ToVec2(vec2 *dest, const vec3 *a) {
		dest->v.x = a->v.x;
		dest->v.y = a->v.y;
	}
	MMATH_INLINE void vec3ToVec4(vec4 *dest, const vec3 *a, scalar w) {
		dest->v.x = a->v.x;
		dest->v.y = a->v.y;
		dest->v.z = a->v.z;
		dest->v.w = w;
	}
	MMATH_INLINE void vec3Cross(vec3 *dest, const vec3 *a, const vec3 *b) {
		dest->v.x = a->v.y * b->v.z - a->v.z * b->v.y;
		dest->v.y = a->v.z * b->v.x - a->v.x * b->v.z;
		dest->v.z = a->v.x * b->v.y - a->v.y * b->v.x;
	}
	#pragma endregion Vec3_Functions

	#pragma region Vec4_Functions
	MMATH_GENFUNC_VECSTANDARD(4);
	MMATH_CONST vec4 vec4Zero     = { 0, 0, 0, 0 };
	MMATH_CONST vec4 vec4Identity = { 1, 1, 1, 1 };
	MMATH_INLINE void vec4ToVec2(vec2 *dest, const vec4 *a) {
		dest->v.x = a->v.x;
		dest->v.y = a->v.y;
	}
	MMATH_INLINE void vec4ToVec3(vec3 *dest, const vec4 *a) {
		dest->v.x = a->v.x;
		dest->v.y = a->v.y;
		dest->v.z = a->v.z;
	}
	MMATH_INLINE void vec4DivW(vec3 *dest, const vec4 *a) {
		scalar w = (scalar)1.0 / a->v.w;
		dest->v.x = a->v.x * w;
		dest->v.y = a->v.y * w;
		dest->v.z = a->v.z * w;
	}
	#pragma endregion Vec4_Functions

	//Quaternion Math
	#pragma region Quaternion_Functions
	MMATH_CONST quat quatIndentity = { 0, 0, 0, 1 };
	MMATH_INLINE scalar quatLength(const quat *a) {
		return vec4Length((const vec4*)a);
	}
	MMATH_INLINE void quatNormalize(quat *dest, const quat *a) {
		vec4Normalize((vec4*)dest, (const vec4*)a);
	}
	MMATH_INLINE void quatAddScalar(quat *dest, const quat *a, scalar b) {
		vec4AddScalar((vec4*)dest, (const vec4*)a, b);
	}
	MMATH_INLINE void quatMulScalar(quat *dest, const quat *a, scalar b) {
		vec4MulScalar((vec4*)dest, (const vec4*)a, b);
	}
	MMATH_INLINE void quatAdd(quat *dest, const quat *a, const quat *b) {
		vec4Add((vec4*)dest, (const vec4*)a, (const vec4*)b);
	}
	MMATH_INLINE void quatMul(quat *dest, const quat *a, const quat *b) {
        vec3 BwAv;
        vec3 AwBv;
        vec3 abv;
        vec3 AxB;
        
		dest->v.w = a->v.w * b->v.w - vec3Dot(&a->axis, &b->axis);

		vec3MulScalar(&BwAv, &a->axis, b->v.w);
		vec3MulScalar(&AwBv, &b->axis, a->v.w);
		vec3Add(&abv, &BwAv, &AwBv);
		vec3Cross(&AxB, &a->axis, &b->axis);
		vec3Add(&dest->axis, &abv, &AxB);
	}
	MMATH_INLINE void quatMulVec3(vec3 *dest, const quat *a, const vec3 *b) {
		vec3 cross1;
		vec3 t;
		vec3 tw;
		vec3 cross2;
		vec3 twcross;
		
		vec3Cross(&cross1, &a->axis, b);
		vec3MulScalar(&t, &cross1, (scalar)2.0);
		vec3MulScalar(&tw, &t, a->v.w);
		vec3Cross(&cross2, &a->axis, &t);
		vec3Add(&twcross, &tw, &cross2);
		vec3Add(dest, b, &twcross);
	}
	MMATH_INLINE void quatNegate(quat *dest, const quat *a) {
		dest->v.x = -a->v.x;
		dest->v.y = -a->v.y;
		dest->v.z = -a->v.z;
		dest->v.w = -a->v.w;
	}
	MMATH_INLINE void quatConjugate(quat *dest, const quat *a) {
		dest->v.x = -a->v.x;
		dest->v.y = -a->v.y;
		dest->v.z = -a->v.z;
		dest->v.w =  a->v.w;
	}
	MMATH_INLINE void quatInverse(quat *dest, const quat *a) {
        scalar length;
		quatConjugate(dest, a);
		length = quatLength(a);
		if (length != (scalar)1.0) {
			scalar l2 = (scalar)1.0 / (length * length);
			dest->v.x *= l2;
			dest->v.y *= l2;
			dest->v.z *= l2;
			dest->v.w *= l2;
		}
	}
	//pitch X | yaw Y | roll Z
	MMATH_INLINE void quatEurler(quat *dest, const vec3 *e) {
		scalar cy = mm_cos(e->v.y * (scalar)0.5);
		scalar sy = mm_sin(e->v.y * (scalar)0.5);
		scalar cr = mm_cos(e->v.z * (scalar)0.5);
		scalar sr = mm_sin(e->v.z * (scalar)0.5);
		scalar cp = mm_cos(e->v.x * (scalar)0.5);
		scalar sp = mm_sin(e->v.x * (scalar)0.5);

		dest->v.x = cy * sr * cp - sy * cr * sp;
		dest->v.y = cy * cr * sp + sy * sr * cp;
		dest->v.z = sy * cr * cp - cy * sr * sp;
		dest->v.w = cy * cr * cp + sy * sr * sp;
	}
	//MMATH_INLINE void quatToEuler(vec3 *dest, const quat *q) {
		//TODO: make this
	//}
	MMATH_INLINE void quatAxisAngle(quat *dest, const vec3 *axis, scalar r) {
		scalar a2 = r * (scalar)0.5;
		vec3MulScalar(&dest->axis, axis, mm_sin(a2));
		dest->v.w = mm_cos(a2);
	}
	MMATH_INLINE void quatToMat3(mat3 *dest, const quat *a) {
		scalar x2 = a->v.x * a->v.x,
			   y2 = a->v.y * a->v.y,
			   z2 = a->v.z * a->v.z,
			   xy = a->v.x * a->v.y,
			   xz = a->v.x * a->v.z,
			   yz = a->v.y * a->v.z,
			   xw = a->v.x * a->v.w,
			   yw = a->v.y * a->v.w,
			   zw = a->v.z * a->v.w;
		mat3 ret = {
			1-(2*(y2+z2)), 2*(xy+zw),     2*(xz-yw),
			2*(xy-zw),     1-(2*(x2+z2)), 2*(yz+xw),
			2*(xz+yw),     2*(yz-xw),     1-(2*(x2+y2))
		};
		*dest = ret;
	}
	MMATH_INLINE void mat3ToMat4(mat4 *dest, const mat3 *a);
	MMATH_INLINE void quatToMat4(mat4 *dest, const quat *a) {
		mat3 ret;
		quatToMat3(&ret, a);
		mat3ToMat4(dest, &ret);
	}
	MMATH_INLINE void quatSlerp(quat *dest, const quat * f, const quat *l, scalar t) {
		scalar dot = vec4Dot((const vec4*)f, (const vec4*)l);

		if (dot < (scalar)0.0) {
			dot = -dot;
			quatInverse(dest, l);
		} else {
			*dest = *l;
		}

		if (dot < (float)0.95) {
            quat fp;
            quat temp;
			scalar angle = mm_acos(dot);
			quatMulScalar(&fp, f, mm_sin(angle * (1 - t)));

			quatMulScalar(&temp, dest, mm_sin(angle * t));
			quatAdd(dest, &fp, &temp);
			quatMulScalar(&temp, dest, (scalar)1.0 / mm_sin(angle));
			*dest = temp;
		} else {
			quat temp;
			vec4Lerp((vec4*)&temp, (const vec4*)f, (const vec4*)dest, t);
			*dest = temp;
		}
	}
	#pragma endregion Quaternion_Functions

	//Matrix Math
	#pragma region Mat_TempDef
	#define MAT_FOR_FLAT_BEGIN(integer) { int i; for (i = 0; i < integer * integer; i++)
	#define MAT_FOR_FLAT_END }
	#define MAT_FOR_BEGIN(integer) { int x,y; for (x = 0; x < integer; x++) for (y = 0; y < integer; y++)
	#define MAT_FOR_END }
	#define MMATH_GENFUNC_MATTRPOSE(integer) \
	MMATH_INLINE void mat##integer##Transpose(mat##integer * dest, const mat##integer *a) { \
		MAT_FOR_BEGIN(integer) { \
			dest->row[x].data[y] = a->row[y].data[x]; \
		} \
		MAT_FOR_END \
	}
	#define MMATH_GENFUNC_MATDIAG(integer) \
	MMATH_INLINE void mat##integer##Diagonal(mat##integer *dest, scalar f) { \
	    {\
            int i; \
    		memset(dest, 0x0, sizeof(mat##integer)); \
    		for (i = 0; i < integer * integer; i += integer + 1) { \
    			dest->data[i] = f; \
    		} \
        } \
	}
	#define MMATH_GENFUNC_MATADD(integer) \
	MMATH_INLINE void mat##integer##Add(mat##integer *dest, const mat##integer *a, const mat##integer *b) { \
		MAT_FOR_FLAT_BEGIN(integer) { \
			dest->data[i] = a->data[i] + b->data[i]; \
		} \
		MAT_FOR_FLAT_END \
	}
	#define MMATH_GENFUNC_MATSUB(integer) \
	MMATH_INLINE void mat##integer##Sub(mat##integer *dest, const mat##integer *a, const mat##integer *b) { \
		MAT_FOR_FLAT_BEGIN(integer) { \
			dest->data[i] = a->data[i] - b->data[i]; \
		} \
		MAT_FOR_FLAT_END \
	}
	#define MMATH_GENFUNC_MATMUL(integer) \
	MMATH_INLINE void mat##integer##Mul(mat##integer *dest, const mat##integer *a, const mat##integer *b) { \
		memset(dest, 0x0, sizeof(mat##integer)); \
		MAT_FOR_BEGIN(integer) { \
			VEC_FOR_BEGIN(integer) { \
				dest->row[x].data[y] += a->row[x].data[i] * b->row[i].data[y]; \
			} \
			VEC_FOR_END \
		} \
		MAT_FOR_END \
	}
	#define MMATH_GENFUNC_MATMULSCALAR(integer) \
	MMATH_INLINE void mat##integer##MulScalar(mat##integer *dest, const mat##integer *a, scalar b) { \
		MAT_FOR_FLAT_BEGIN(integer) { \
			dest->data[i] = a->data[i] * b; \
		} \
		MAT_FOR_FLAT_END \
	}
	#define MMATH_GENFUNC_MATMULVEC(integer) \
	MMATH_INLINE void mat##integer##MulVec##integer(vec##integer *dest, const mat##integer *a, const vec##integer *b) { \
	   { \
    	    int c; \
    		memset(dest, 0x0, sizeof(vec##integer)); \
    		VEC_FOR_BEGIN(integer) { \
    			for (c = 0; c < integer; c++) { \
    				dest->data[i] += a->row[c].data[i] * b->data[c]; \
    			} \
    		} \
    		VEC_FOR_END \
        } \
	}
	#define MMATH_GENFUNC_MATSTANDARD(integer) \
		MMATH_GENFUNC_MATTRPOSE(integer); \
		MMATH_GENFUNC_MATDIAG(integer); \
		MMATH_GENFUNC_MATADD(integer); \
		MMATH_GENFUNC_MATSUB(integer); \
		MMATH_GENFUNC_MATMUL(integer); \
		MMATH_GENFUNC_MATMULSCALAR(integer); \
		MMATH_GENFUNC_MATMULVEC(integer);
	#pragma endregion Mat_TempDef
	
	#pragma region Mat2_Functions
	MMATH_GENFUNC_MATSTANDARD(2);
	MMATH_CONST mat2 mat2Identity = {
		1, 0,
		0, 1
	};
	MMATH_INLINE void mat2ToMat3(mat3 *dest, const mat2 *a) {
		mat3 ret = {
			a->m.x0, a->m.y0, 0,
			a->m.x1, a->m.y1, 0,
			0,    0,      1
		};
		*dest = ret;
	}
	MMATH_INLINE void mat2ToMat4(mat4 *dest, const mat2 *a) {
		mat4 ret = {
			a->m.x0, a->m.y0, 0, 0,
			a->m.x1, a->m.y1, 0, 0,
			0,    0,      1, 0,
			0,    0,      0, 1
		};
		*dest = ret;
	}
	#pragma endregion Mat2_Functions

	#pragma region Mat3_Functions
	MMATH_GENFUNC_MATSTANDARD(3);
	MMATH_CONST mat3 mat3Identity = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	};
	MMATH_INLINE void mat3ToMat2(mat2 *dest, const mat3 *a) {
		mat2 ret = {
			a->m.x0, a->m.y0,
			a->m.x1, a->m.y1
		};
		*dest = ret;
	}
	MMATH_INLINE void mat3ToMat4(mat4 *dest, const mat3 *a) {
		mat4 ret = {
			a->m.x0, a->m.y0, a->m.z0, 0,
			a->m.x1, a->m.y1, a->m.z1, 0,
			a->m.x2, a->m.y2, a->m.z2, 0,
			0,     0,     0,     1
		};
		*dest = ret;
	}
	//MMATH_INLINE void mat3Inverse(mat3 *dest, const mat3 *a) {
	//	mat3 ret; TODO: also make this
	//}
	MMATH_INLINE void mat3RotateX(mat3 *dest, scalar r) {
		scalar c = mm_cos(r);
		scalar s = mm_sin(r);
		mat3 ret = {
			1, 0, 0,
			0, c, s,
			0,-s, c
		};
		*dest = ret;
	}
	MMATH_INLINE void mat3RotateY(mat3 *dest, scalar r) {
		scalar c = mm_cos(r);
		scalar s = mm_sin(r);
		mat3 ret = {
			c, 0,-s,
			0, 1, 0,
		    s, 0, c
		};
		*dest = ret;
	}
	MMATH_INLINE void mat3RotateZ(mat3 *dest, scalar r) {
		scalar c = mm_cos(r);
		scalar s = mm_sin(r);
		mat3 ret = {
			c, s, 0,
		   -s, c, 0,
			0, 0, 1
		};
		*dest = ret;
	}
	#pragma endregion Mat3_Functions

	#pragma region Mat4_Functions
	MMATH_GENFUNC_MATSTANDARD(4);
	MMATH_CONST mat4 mat4Identity = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	MMATH_INLINE void mat4Scale(mat4 *dest, const vec3 *s) {
		mat4 ret = {
			s->v.x, 0,      0,    0,
			0,      s->v.y, 0,    0,
			0,      0,      s->v.z, 0,
			0,      0,      0,    1
		};
		*dest = ret;
	}
	MMATH_INLINE void mat4Translate(mat4 *dest, const vec3 *t) {
		mat4 ret = {
			1,      0,      0,    0,
			0,      1,      0,    0,
			0,      0,      1,    0,
			t->v.x, t->v.y, t->v.z, 1
		};
		*dest = ret;
	}
	MMATH_INLINE void mat4Perspective(mat4 *dest, scalar aspect, scalar fovY, scalar zNear, scalar zFar) {
		scalar f   = (scalar)1.0 / mm_tan(fovY * (scalar)0.5);
		scalar nf  = (scalar)1.0 / (zNear - zFar);
		mat4 ret = {
			f/aspect, 0,  0,                0,
			0,        f,  0,                0,
			0,        0, (zFar+zNear)*nf,  -1,
			0,        0, (2*zFar*zNear)*nf, 0
		};
		*dest = ret;
	}
	MMATH_INLINE void mat4Ortho(mat4 *dest, scalar left, scalar right, scalar top, scalar bottom, scalar zNear, scalar zFar) {
		scalar tb = top - bottom;
		scalar rf = right - left;
		scalar fn = zFar - zNear;
		mat4 ret = {
			2/rf,               0,                  0,                      0,
			0,                  2/tb,               0,                      0,
			0,                  0,                  -2/fn,                  0,
			-(right + left)/rf, -(top + bottom)/tb, -(zFar + zNear)/fn,     1
		};
		*dest = ret;
	}
	MMATH_INLINE void mat4LookAt(mat4 *dest, const vec3 *eye, const vec3 *center, const vec3 *up) {
		vec3 temp;
		vec3 z;
		vec3 x;
		vec3 y;
		scalar ndotx;
		scalar ndoty;
		scalar ndotz;
		mat4 ret;
		
		vec3Sub(&temp, center, eye);
		vec3Normalize(&z, &temp);
		vec3Cross(&temp, up, &z);
		vec3Normalize(&x, &temp);
		vec3Cross(&temp, &z, &x);
		vec3Normalize(&y, &temp);

		ndotx = -vec3Dot(&x, eye);
		ndoty = -vec3Dot(&y, eye);
		ndotz = -vec3Dot(&z, eye);

		ret.m.x0 = x.v.x;
        ret.m.x1 = y.v.x;   
        ret.m.x2 = z.v.x;
        ret.m.x3 = 0;
		ret.m.y0 = x.v.y;   
        ret.m.y1 = y.v.y;   
        ret.m.y2 = z.v.y;
        ret.m.y3 = 0;
		ret.m.z0 = x.v.z;   
        ret.m.z1 = y.v.z;   
        ret.m.z2 = z.v.z;
        ret.m.z3 = 0;
		ret.m.w0 = ndotx;   
        ret.m.w1 = ndoty;   
        ret.m.w2 = ndotz;
        ret.m.w3 = 1;
        
		*dest = ret;
	}
	MMATH_INLINE void mat4ToMat2(mat2 *dest, const mat4 *a) {
		mat2 ret = {
			a->m.x0, a->m.y0,
			a->m.x1, a->m.y1
		};
		*dest = ret;
	}
	MMATH_INLINE void mat4ToMat3(mat3 *dest, const mat4 *a) {
		mat3 ret = {
			a->m.x0, a->m.y0, a->m.z0,
			a->m.x1, a->m.y1, a->m.z1,
			a->m.x2, a->m.y2, a->m.z2
		};
		*dest = ret;
	}
	#pragma endregion Mat4_Functions

	//Transformations
	#pragma region Transform_Functions
	MMATH_CONST transform transformIdentity = { {0,0,0}, {1,1,1}, {0,0,0,1} };
	MMATH_INLINE void transformToMat4(mat4 *dest, const transform *t) {
		mat4 ret = {
			t->scale.v.x, 0, 0, 0,
			0, t->scale.v.y, 0, 0,
			0, 0, t->scale.v.z, 0,
			t->pos.v.x, t->pos.v.y, t->pos.v.z, 1
		};
		mat4 rotate;
		quatToMat4(&rotate, &t->rot);
		mat4Mul(dest, &rotate, &ret);
	}
	//TODO: Solve transform inverse
	MMATH_INLINE void transformMul(transform *dest, const transform *a, const transform *b) {
		vec3 pos;
		vec3 posScale;
		
		quatMulVec3(&pos, &a->rot, &b->pos);
		vec3Mul(&posScale, &pos, &a->scale);
		vec3Add(&dest->pos, &posScale, &a->pos);
		vec3Mul(&dest->scale, &a->scale, &b->scale);
		quatMul(&dest->rot, &a->rot, &b->rot);
	}
	MMATH_INLINE void transformLerp(transform *dest, const transform *f, const transform *l, scalar t) {
		vec3Lerp(&dest->pos, &f->pos, &l->pos, t);
		vec3Lerp(&dest->scale, &f->scale, &l->scale, t);
		quatSlerp(&dest->rot, &f->rot, &l->rot, t);
	}
	#pragma endregion Transform_Functions

#if defined(__cplusplus)
}
#endif

#endif //MMATH_HEADER_FILE
