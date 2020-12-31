/**
 * Helper lib for streaming data from the cd rom
 * Created by wituz
 * 
 */

#ifndef cdrom_h
#define cdrom_h

// #include <LIBCD.H>
#include <SYS/TYPES.H>
#include <LIBDS.H>

#define SECTOR 2048

void CdOpen();
void CdClose();
void CdReadFile(u_char* file_path, u_long** file);


#endif