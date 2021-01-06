#include "TMD.h"

void TMDLoad(u_long* tmdData)
{
	//Load model data
	tmdData++;    //Skip TMD header
	GsMapModelingData(tmdData);
}

void TMDDebugPrint(u_long* tmdData)
{
    int i;
    int j;
    TMDHeader* tmdHdr;
    TMDObject* tmdObj;
    TMDPrimitive* tmdPrim;
	u_long tmdObjBase;
	char* tmdVertTbl;
    char* tmdNormTbl;
    char* tmdPrimTbl;
    TMDPrimReader tmdPrimReader;
    	
    //Read TMD header
	tmdHdr = (TMDHeader*)tmdData;
	tmdData += sizeof(TMDHeader);
	printf("TMD id: %d\n", tmdHdr->id);
	printf("TMD flags: %d\n", tmdHdr->flags);
	printf("TMD numObj: %d\n", tmdHdr->numObjects);
	
	//Read all objects
	for(i = 0; i < tmdHdr->numObjects; i++)
	{
       printf("Object %d:\n", i);
       
       tmdObjBase = (u_long)tmdData;
       
       //Read object header
       tmdObj = (TMDObject*)tmdData;
       tmdData += sizeof(TMDObject);
       
       //Get vert/norm/index tables
       if(tmdHdr->flags & TMDFLAG_FIXP)
       {
           //Table addrs are pointers
           printf(" Using table pointers\n");
	       tmdVertTbl = (char*)tmdObj->vertexTable;
	       tmdNormTbl = (char*)tmdObj->normalTable;
	       tmdPrimTbl = (char*)tmdObj->primitiveTable;
       }
       else
       {
           //Table addrs are offsets
           printf(" Using table offsets\n");
	       tmdVertTbl = (char*)(((char*)tmdObjBase) + (u_long)tmdObj->vertexTable);
	       tmdNormTbl = (char*)(((char*)tmdObjBase) + (u_long)tmdObj->normalTable);
	       tmdPrimTbl = (char*)(((char*)tmdObjBase) + (u_long)tmdObj->primitiveTable);
       }
       
       printf(" Vertices:   %d\n", tmdObj->vertexCount);
       printf(" Normals:    %d\n", tmdObj->normalCount);
       printf(" Primitives: %d\n", tmdObj->primitiveCount);
       printf(" Scale:      %d\n", tmdObj->scale);
       
       //Read all primitives
       for(j = 0; j < tmdObj->primitiveCount; j++)
       {
            tmdPrim = (TMDPrimitive*)tmdPrimTbl;
            tmdPrimTbl += sizeof(TMDPrimitive);
            
            printf(" Prim %d:\n", j);
            printf("  Prim sz:   %d\n", (int)tmdPrim->primDataSizeWords);
            printf("  Packet sz: %d\n", (int)tmdPrim->packetDataSizeWords);
            printf("  Flags:     %d\n", (int)tmdPrim->flags);
            printf("  Mode:      %d\n", (int)tmdPrim->mode);
            
            tmdPrimReader = TMDGetPrimReader(tmdPrim);
            if(tmdPrimReader)
            {
                tmdPrimTbl += (*tmdPrimReader)(tmdPrim, (TMDVertex*)tmdVertTbl, (TMDVertex*)tmdNormTbl);
            }
       }
    }
}

void TMDDebugPrintVerts(u_long* tmdData, int maxVerts)
{
    int i;
    int j;
    int vertCount = 0;
    TMDHeader* tmdHdr;
    TMDObject* tmdObj;
	u_long tmdObjBase;
	char* tmdVertTbl;
    	
    //Read TMD header
	tmdHdr = (TMDHeader*)tmdData;
	tmdData += sizeof(TMDHeader);
	
	//Read all objects
	for(i = 0; i < tmdHdr->numObjects && vertCount < maxVerts; i++)
	{
       printf("Object %d:\n", i);
       
       tmdObjBase = (u_long)tmdData;
       
       //Read object header
       tmdObj = (TMDObject*)tmdData;
       tmdData += sizeof(TMDObject);
       
       //Get vert/norm/index tables
       if(tmdHdr->flags & TMDFLAG_FIXP)
       {
	       tmdVertTbl = (char*)tmdObj->vertexTable;
       }
       else
       {
	       tmdVertTbl = (char*)(((char*)tmdObjBase) + (u_long)tmdObj->vertexTable);
       }
       
       printf(" Vertices:   %d\n", tmdObj->vertexCount);
       
       for(j = 0; j < tmdObj->vertexCount && vertCount < maxVerts; j++, vertCount++)
       {
            TMDVertex* vert = &((TMDVertex*)tmdVertTbl)[j];
            printf("  %d,%d,%d\n", vert->x, vert->y, vert->z);
       }
    }
}

TMDPrimReader TMDGetPrimReader(TMDPrimitive* prim)
{
    if(prim->primDataSizeWords == 4
    && prim->packetDataSizeWords == 3
    && prim->flags == 0x00
    && prim->mode == 0x20)
    {
        return &TMDPrimReader_04_03_00_20;
    }
    
    printf("Error: TMDPrimReader not found for format %d-%d-%02x-%02x\n", 
        prim->primDataSizeWords, 
        prim->packetDataSizeWords,
        prim->flags,
        prim->mode);
        
    return NULL;
}

u_long TMDPrimReader_04_03_00_20(TMDPrimitive* prim, TMDVertex* verts, TMDVertex* norms)
{
    TMDPrim_04_03_00_20* primPtr = (TMDPrim_04_03_00_20*)prim;
    printf("   Read a 04_03_00_20 primitive:\n");
    printf("    R:      %d, G: %d, B: %d\n", (int)primPtr->colourR, (int)primPtr->colourG, (int)primPtr->colourB);
    printf("    Mode:   %d\n", (int)primPtr->mode);
    printf("    Normal: %d\n", (int)primPtr->normIdx);
    printf("    Vert0:  %d (%d,%d,%d)\n",
        (int)primPtr->vertIdx0,
        (int)verts[primPtr->vertIdx0].x,
        (int)verts[primPtr->vertIdx0].y,
        (int)verts[primPtr->vertIdx0].z);
    printf("    Vert1:  %d (%d,%d,%d)\n",
        (int)primPtr->vertIdx1,
        (int)verts[primPtr->vertIdx1].x,
        (int)verts[primPtr->vertIdx1].y,
        (int)verts[primPtr->vertIdx1].z);
    printf("    Vert2:  %d (%d,%d,%d)\n",
        (int)primPtr->vertIdx2,
        (int)verts[primPtr->vertIdx2].x,
        (int)verts[primPtr->vertIdx2].y,
        (int)verts[primPtr->vertIdx2].z);
    
    return sizeof(TMDPrim_04_03_00_20);
}
