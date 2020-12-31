#ifndef _TMD_H
#define _TMD_H

#include <libps.h>

#define TMDFLAG_FIXP         1

#define TMD_PRIM_TYPE_POLY   1 // Polygon
#define TMD_PRIM_TYPE_LINE   2 // Line strip
#define TMD_PRIM_TYPE_SPRT   3 // Sprite

#define TMD_PRIM_FLAG_LGT    1 // 0 = Lighting on, 1 = lighting off
#define TMD_PRIM_FLAG_FCE    2 // 0 = single sided, 1 = double sided
#define TMD_PRIM_FLAG_GRD    4 // 0 = single colour, 1 = colour per vertex

typedef struct _TMDHeader
{
    int id;
    int flags;
    int numObjects;
} TMDHeader;

typedef struct _TMDObject
{
    //If FIXP:  table addrs are pointers
    //If !FIXP: table addrs are offsets from TMDObject
    
    u_long* vertexTable;
    u_long  vertexCount;
    u_long* normalTable;
    u_long  normalCount;
    u_long* primitiveTable;
    u_long  primitiveCount;
    long    scale;
} TMDObject;

typedef struct _TMDPrimitive
{
    u_char primDataSizeWords;
    u_char packetDataSizeWords;
    u_char flags;
    u_char mode;
} TMDPrimitive;

typedef struct _TMDVertex
{
    short x;
    short y;
    short z;
    short pad;
} TMDVertex;

////////////////////////////////////////////
//Primitive packet readers
////////////////////////////////////////////

typedef u_long (*TMDPrimReader)(TMDPrimitive* prim, TMDVertex* verts, TMDVertex* norms);

//3-point polygon, flat shading, no lighting
typedef struct _TMDPrim_04_03_00_20
{
    u_char  colourR;
    u_char  colourG;
    u_char  colourB;
    u_char  mode;
    u_short normIdx;
    u_short vertIdx0;
    u_short vertIdx1;
    u_short vertIdx2;
} TMDPrim_04_03_00_20;

void TMDLoad(u_long* tmdData);
void TMDDebugPrint(u_long* tmdData);
void TMDDebugPrintVerts(u_long* tmdData, int maxVerts);
TMDPrimReader TMDGetPrimReader(TMDPrimitive* prim);
u_long TMDPrimReader_04_03_00_20(TMDPrimitive* prim, TMDVertex* verts, TMDVertex* norms);

#endif // _TMD_H
