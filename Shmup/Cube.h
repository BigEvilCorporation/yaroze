#ifndef _CUBE_H
#define _CUBE_H

#include "TMD.h"

static TMDVertex geomCubeVerts[8] =
{
    { -20,-20,-20 },
    {  20,-20,-20 },
    {  20, 20,-20 },
    { -20, 20,-20 },
    { -20,-20, 20 },
    {  20,-20, 20 },
    {  20, 20, 20 },
    { -20, 20, 20 },
};

static TMDVertex geomCubeNorms[6] =
{
    { 0, 0,-ONE },
    { 0, 0, ONE },
    { ONE, 0, 0 },
    {-ONE, 0, 0 },
    { 0, -ONE, 0 },
    { 0, ONE, 0 },
};

typedef struct _TMDPrimData
{
    TMDPrimitive primHdr0;
    TMDPrim_04_03_00_20 prim0;
} TMDPrimData;

static TMDPrimData geomCubePrimData[12] =
{
    //Front
    {
        {
            4,      // primDataSizeWords
            3,      // packetDataSizeWords
            0x00,   // flags
            0x20    // mode
        },
        {
            255,    // colourR
            0,      // colourG
            0,      // colourB
            0x20,   // mode
            0,      // normIdx
            0,      // vertIdx0
            1,      // vertIdx1
            2,      // vertIdx2
        }
    },
    {
        {
            4,      // primDataSizeWords
            3,      // packetDataSizeWords
            0x00,   // flags
            0x20    // mode
        },
        {
            255,    // colourR
            0,      // colourG
            0,      // colourB
            0x20,   // mode
            0,      // normIdx
            0,      // vertIdx0
            2,      // vertIdx1
            3,      // vertIdx2
        }
    },
    //Back
    {
        {
            4,      // primDataSizeWords
            3,      // packetDataSizeWords
            0x00,   // flags
            0x20    // mode
        },
        {
            0,      // colourR
            255,    // colourG
            0,      // colourB
            0x20,   // mode
            1,      // normIdx
            5,      // vertIdx0
            4,      // vertIdx1
            7,      // vertIdx2
        }
    },
    {
        {
            4,      // primDataSizeWords
            3,      // packetDataSizeWords
            0x00,   // flags
            0x20    // mode
        },
        {
            0,      // colourR
            255,    // colourG
            0,      // colourB
            0x20,   // mode
            1,      // normIdx
            5,      // vertIdx0
            7,      // vertIdx1
            6,      // vertIdx2
        }
    },
    //Right
    {
        {
            4,      // primDataSizeWords
            3,      // packetDataSizeWords
            0x00,   // flags
            0x20    // mode
        },
        {
            0,      // colourR
            0,      // colourG
            255,    // colourB
            0x20,   // mode
            2,      // normIdx
            1,      // vertIdx0
            5,      // vertIdx1
            6,      // vertIdx2
        }
    },
    {
        {
            4,      // primDataSizeWords
            3,      // packetDataSizeWords
            0x00,   // flags
            0x20    // mode
        },
        {
            0,      // colourR
            0,      // colourG
            255,    // colourB
            0x20,   // mode
            2,      // normIdx
            1,      // vertIdx0
            6,      // vertIdx1
            2,      // vertIdx2
        }
    },
    //Left
    {
        {
            4,      // primDataSizeWords
            3,      // packetDataSizeWords
            0x00,   // flags
            0x20    // mode
        },
        {
            255,    // colourR
            0,      // colourG
            255,    // colourB
            0x20,   // mode
            3,      // normIdx
            4,      // vertIdx0
            0,      // vertIdx1
            3,      // vertIdx2
        }
    },
    {
        {
            4,      // primDataSizeWords
            3,      // packetDataSizeWords
            0x00,   // flags
            0x20    // mode
        },
        {
            255,    // colourR
            0,      // colourG
            255,    // colourB
            0x20,   // mode
            3,      // normIdx
            4,      // vertIdx0
            3,      // vertIdx1
            7,      // vertIdx2
        }
    },
    //Top
    {
        {
            4,      // primDataSizeWords
            3,      // packetDataSizeWords
            0x00,   // flags
            0x20    // mode
        },
        {
            0,      // colourR
            255,    // colourG
            255,    // colourB
            0x20,   // mode
            4,      // normIdx
            4,      // vertIdx0
            5,      // vertIdx1
            1,      // vertIdx2
        }
    },
    {
        {
            4,      // primDataSizeWords
            3,      // packetDataSizeWords
            0x00,   // flags
            0x20    // mode
        },
        {
            0,      // colourR
            255,    // colourG
            255,    // colourB
            0x20,   // mode
            4,      // normIdx
            4,      // vertIdx0
            1,      // vertIdx1
            0,      // vertIdx2
        }
    },
    //Bottom
    {
        {
            4,      // primDataSizeWords
            3,      // packetDataSizeWords
            0x00,   // flags
            0x20    // mode
        },
        {
            255,    // colourR
            255,    // colourG
            255,    // colourB
            0x20,   // mode
            5,      // normIdx
            6,      // vertIdx0
            7,      // vertIdx1
            3,      // vertIdx2
        }
    },
    {
        {
            4,      // primDataSizeWords
            3,      // packetDataSizeWords
            0x00,   // flags
            0x20    // mode
        },
        {
            255,    // colourR
            255,    // colourG
            255,    // colourB
            0x20,   // mode
            5,      // normIdx
            6,      // vertIdx0
            3,      // vertIdx1
            2,      // vertIdx2
        }
    },
};

typedef struct _TMDData
{
    TMDHeader hdr;
    TMDObject obj;
} TMDData;

static TMDData geomCube =
{
    {
        0,                      // id
        TMDFLAG_FIXP,           // flags
        1,                      // numObjects
    },
    {
        (u_long*)geomCubeVerts, // vertexTable
        8,                      // vertexCount
        (u_long*)geomCubeNorms, // normalTable
        6,                      // normalCount
        (u_long*)&geomCubePrimData, // primitiveTable
        12,                     // primitiveCount
        0                       // scale
    }
};

#endif // _CUBE_H
