#ifndef _PHYSDEFS_H
#define _PHYSDEFS_H

#include "..\\maths\\Maths.h"
#include "..\\maths\\MMath.h"

#define VISCO_INLINE static inline

#define VISCO_MAX_CONTACTS 4

#ifdef VISCO_DLL

//DLL
#ifdef VISCOSITY_EXPORTS
#define VISOC_API __declspec(dllexport)
#else

#define VISCO_API __declspec(dllimport)
#endif //Export

#else

//Static Library
#define VISCO_API

#endif //DLL

#endif // _PHYSDEFS_H
