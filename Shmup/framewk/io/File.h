#ifndef _FILE_H
#define _FILE_H

#include <libps.h>

#define FILE_READ_SUCCESS 0
#define FILE_READ_ERROR   -1

int FileGetSize(char* filename);
int FileReadSync(char* filename, int sizeBytes, u_long* destination);

#endif // _FILE_H
