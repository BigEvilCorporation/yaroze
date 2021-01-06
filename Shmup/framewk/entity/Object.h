#ifndef _OBJECT_H
#define _OBJECT_H

#include <libps.h>

typedef struct _object
{
    GsDOBJ2 gsObj;
    GsCOORDINATE2 gsTransform;
    VECTOR pos; 
    SVECTOR rot;
} Object;

void ObjectInit(Object* object);
void ObjectLinkTMD(Object* object, u_long* tmdData);
void ObjectUpdateGsTransform(Object* object);

#endif // _OBJECT_H
