#ifndef _PHYSICS_H
#define _PHYSICS_H

//Viscosity Physics Engine
#include "physdefs.h"

#define VISCO_VERSION_0_1 0x0001

#if  defined(__cplusplus)
extern "C" {
#endif
	
	#include "world.h"
	
	VISCO_API int viscoGetVersion(void);
	
#ifdef __cplusplus
};
#endif

#endif // _PHYSICS_H
