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
#include "mem.h"

#define SECTOR 2048

void CdOpen();
void CdClose();
/**
 * Read contents of the cd
 * @param file_path location of file on cd
 * @param file where we will store the cd content
 * @param filesRead used when we want to keep track of how many assets we have loaded (pass in NULL if not needed) 
 */
void CdReadFile(char* file_path, u_long** file, u_char* filesRead);


#endif