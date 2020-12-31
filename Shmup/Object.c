#include "Object.h"

void ObjectInit(Object* object)
{
    //Init transform
	object->pos.vx = 0;
	object->pos.vy = 0;
	object->pos.vz = 0;
	object->rot.vx = 0;
	object->rot.vy = 0;
	object->rot.vz = 0;
    GsInitCoordinate2(WORLD, &object->gsTransform);
	object->gsObj.coord2 = &object->gsTransform;
	ObjectUpdateGsTransform(object);
}

void ObjectLinkTMD(Object* object, u_long* tmdData)
{
	//Link pre-mapped TMD data
	tmdData += 3;    //Skip TMD header and metadata
	GsLinkObject4((u_long)tmdData, &object->gsObj, 0);
}

void ObjectUpdateGsTransform(Object* object)
{
    //Build pos/rot matrix
    MATRIX tempMtx;   
    tempMtx.t[0] = object->pos.vx;   
    tempMtx.t[1] = object->pos.vy;   
    tempMtx.t[2] = object->pos.vz;   
    RotMatrix(&object->rot, &tempMtx); 
    object->gsTransform.coord = tempMtx;
    object->gsTransform.flg = 0;
}
