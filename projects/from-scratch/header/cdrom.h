/**
 * Helper lib for streaming data from the cd rom
 * Created by wituz
 * 
 */

#ifndef cdrom_h
#define cdrom_h

#include <SYS/TYPES.H>
#include <LIBDS.H>
#include "base.h"

#define SECTOR 2048

void CdOpen();
void CdClose();
void CdReadFile(char* file_path, u_long** file);


#endif