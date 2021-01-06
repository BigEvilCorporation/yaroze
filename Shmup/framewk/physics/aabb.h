#ifndef _AABB_H
#define _AABB_H

#include "PhysDefs.h"

typedef struct aabb {
	vec3 min, max;
} aabb;

static const aabb aabbInfinity = {
	{-INFINITY, -INFINITY, -INFINITY},
	{ INFINITY,  INFINITY,  INFINITY}
};

VISCO_INLINE void aabbAddVec3(aabb *dest, const aabb *a, const vec3 *b) {
	vec3Add(&dest->min, &a->min, b);
	vec3Add(&dest->max, &a->max, b);
}
VISCO_INLINE void aabbSubVec3(aabb *dest, const aabb *a, const vec3 *b) {
	vec3Sub(&dest->min, &a->min, b);
	vec3Sub(&dest->max, &a->max, b);
}
VISCO_INLINE void aabbAdd(aabb *dest, const aabb *a, const aabb *b) {
	*dest = (aabb){
		{ mm_min(a->min.v.x, b->min.v.x), mm_min(a->min.v.y, b->min.v.y), mm_min(a->min.v.z, b->min.v.z) },
		{ mm_max(a->max.v.x, b->max.v.x), mm_max(a->max.v.y, b->max.v.y), mm_max(a->max.v.z, b->max.v.z) }
	};
}

VISCO_INLINE void aabbCenter(vec3 *dest, const aabb *a) {
	vec3 add;
	vec3Add(&add, &a->max, &a->min);
	vec3MulScalar(dest, &add, 0.5f);
}
VISCO_INLINE void aabbClosestPoint(vec3 *dest, const aabb *a, const vec3 *b) {
    int i;
	for (i = 0; i < 3; i++) {
		if (b->data[i] > a->max.data[i]) {
			dest->data[i] = a->max.data[i];
		} else if (b->data[i] < a->min.data[i]) {
			dest->data[i] = a->min.data[i];
		} else {
			dest->data[i] = b->data[i];
		}
	}
}

VISCO_INLINE int aabbCollidePoint(const aabb *a, const vec3 *b) {
	return (b->v.x >= a->min.v.x && b->v.x <= a->max.v.x) &&
		   (b->v.y >= a->min.v.y && b->v.y <= a->max.v.y) &&
		   (b->v.z >= a->min.v.z && b->v.z <= a->max.v.z);
}
VISCO_INLINE int aabbCollideAabb(const aabb *a, const aabb *b) {
	return (a->min.v.x <= b->max.v.x && a->max.v.x >= b->min.v.x) &&
		   (a->min.v.y <= b->max.v.y && a->max.v.y >= b->min.v.y) &&
		   (a->min.v.z <= b->max.v.z && a->max.v.z >= b->min.v.z);
}

#endif // _AABB_H
