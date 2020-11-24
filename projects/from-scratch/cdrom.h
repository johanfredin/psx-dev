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


void CdOpen() {
	if(!didInitDs) {
		didInitDs = 1;
		DsInit();
	}
}

void CdClose() {
	if(didInitDs) {
		didInitDs = 0;
		DsClose();
    }
}

void CdReadFile(unsigned char* file_path, u_long** file) {
	u_char* file_path_raw;
	int* sectors_size;
	DslFILE* temp_file_info;
	sectors_size = (int*)malloc3(sizeof(int));
	temp_file_info = malloc3(sizeof(DslFILE));

	// Exit if libDs isn't initialized
	if(!didInitDs) {
		printf("LIBDS not initialized, run cdOpen() first\n");	
		return;
	}

	// Get raw file path
	file_path_raw = (u_char*)malloc3(4 + strlen(file_path));
	strcpy(file_path_raw, "\\");
	strcat(file_path_raw, file_path);
	strcat(file_path_raw, ";1");
	printf("Loading file from CD: %s\n", file_path_raw);

	// Search for file on disc
	DsSearchFile(temp_file_info, file_path_raw);

	// Read the file if it was found
	if(temp_file_info->size > 0) {
		printf("...file found\n");
		printf("...file size: %lu\n", temp_file_info->size);
		*sectors_size = temp_file_info->size + (SECTOR % temp_file_info->size);
		printf("...file buffer size needed: %d\n", *sectors_size);
		printf("...sectors needed: %d\n", (*sectors_size + SECTOR - 1) / SECTOR);
		*file = (u_long*)malloc3(*sectors_size + SECTOR);
		
		DsRead(&temp_file_info->pos, (*sectors_size + SECTOR - 1) / SECTOR, *file, DslModeSpeed);
		while(DsReadSync(NULL));
		printf("...file loaded!\n");
	} else {
		printf("...file not found\n");
	}

	// Clean up
	free3(file_path_raw);
	free3(sectors_size);
	free3(temp_file_info);

	printf("File %s loaded into memory addr=%p\n", file_path, *file);
}

#endif