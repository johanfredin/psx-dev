/**
 * Helper lib for streaming data from the cd rom
 * Created by wituz
 * 
 */

#ifndef cdrom_h
#define cdrom_h

#include <LIBDS.H>
#include <SYS/TYPES.H>

#define SECTOR 2048
#define NULL 0

u_char didInitDs = 0;

void CdOpen();
void CdClose();
void CdReadFile(u_char* file_path, u_long** file);


#endif