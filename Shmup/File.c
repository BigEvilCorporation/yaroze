#include "File.h"

int FileGetSize(char* filename)
{
    CdlFILE cdlFile;
    if (CdSearchFile(&cdlFile, filename) != 0)
    {
        return cdlFile.size;
    }
    
    return 0;
}

int FileReadSync(char* filename, int sizeBytes, u_long* destination)
{
    int result = 0;
    CdReadFile(filename, destination, sizeBytes);
    
    while((result = CdReadSync(1, 0)) > 0)
    {
        
    }
    
    return result;
}
