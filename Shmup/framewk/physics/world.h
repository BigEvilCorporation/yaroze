#ifndef _WORLD_H
#define _WORLD_H

#include "physdefs.h"
#include "shape.h"

typedef size_t bodyID;
typedef size_t jointID;

typedef enum jointType {
	JOINT_DELETE = 0,
	JOINT_CONTACT
} jointType;

typedef struct joint {
	jointType type;
	bodyID a, b;
} joint;

typedef struct contact_joint {
	joint j;
	contact contact;
} contact_joint, joint_max;

typedef enum bodyType {
	BODY_DELETE = 0,
	BODY_STATIC,
	BODY_DYNAMIC,
	BODY_KINEMATIC
} bodyType;

typedef struct accumulator {
	vec3 vel, avel;
} accumulator;

typedef struct world {
	vec3 gravity;
	
	size_t body_size;
	size_t body_cap;

	size_t* body_empty;
	size_t body_empty_size;

	bodyType *body_type;
	vec3 *body_pos;  //3
	vec3 *body_vel;  //3
	quat *body_rot;  //4
	vec3 *body_avel; //3
	accumulator *body_accum; // 6
	aabb *body_aabb; //6
	shape **body_shape; //array of pointers, shapes are stored separately from worlds

	size_t joint_size;
	size_t joint_cap;

	size_t *joint_empty;
	size_t joint_empty_size;

	joint_max* joints;

} world;

VISCO_API world* worldCreate(void);
VISCO_API void   worldDestroy(world *wld);

VISCO_API void worldStep(world **wld, scalar delta);

VISCO_API bodyID bodyCreate(world **wld);
VISCO_API void   bodyDestroy(world *wld, bodyID body);

VISCO_API void     bodySetType(world *wld, bodyID body, bodyType type);
VISCO_API bodyType bodyGetType(world *wld, bodyID body);

VISCO_API void bodyGetPosition(vec3 *dest, world *world, bodyID body);
VISCO_API void bodySetPosition(world *wld, bodyID body, const vec3 *position);

VISCO_API void bodyGetOrientation(quat *dest, world *world, bodyID body);
VISCO_API void bodySetOrientation(world *wld, bodyID body, const quat *rot);

VISCO_API void bodyGetTransform(transform *dest, world *world, bodyID body);
VISCO_API void bodyGetMat4(mat4 *dest, world *wld, bodyID body);

VISCO_API void bodySetShape(world *wld, bodyID body, shape *shape);

VISCO_API void bodyApplyForce(world *wld, bodyID body, const vec3 *pos, const vec3 *force);
VISCO_API void bodyApplyForceAtCenter(world *wld, bodyID body, const vec3 *force);
VISCO_API void bodyApplyTorque(world *wld, bodyID body, const vec3 *torque);

VISCO_API void bodyGetVelocityAtPoint(vec3 *dest, world *wld, bodyID body, const vec3 *pos);

//VISCO_API void jointDestroy(world *world, jointID joint);

#endif // _WORLD_H
