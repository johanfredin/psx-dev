#include "header/cdrom.h"

u_char didInitDs = 0;

void CdOpen() {
    if (!didInitDs) {
        didInitDs = 1;
        DsInit();
    }
}

void CdClose() {
    if (didInitDs) {
        didInitDs = 0;
        DsClose();
    }
}

void CdReadFile(char *file_path, u_long **file, u_char *filesRead) {
    u_char *file_path_raw;
    int *sectors_size;
    DslFILE *temp_file_info;
    sectors_size = MALLOC(int);
    temp_file_info = MALLOC(DslFILE);

    // Exit if libDs isn't initialized
    if (!didInitDs) {
        printf("LIBDS not initialized, run cdOpen() first\n");
        return;
    }

    // Get raw file path
    file_path_raw = (char *)malloc3(4 + strlen(file_path));
    strcpy(file_path_raw, "\\");
    strcat(file_path_raw, file_path);
    strcat(file_path_raw, ";1");
    printf("Loading file from CD: %s\n", file_path_raw);

    // Search for file on disc
    DsSearchFile(temp_file_info, file_path_raw);

    // Read the file if it was found
    if (temp_file_info->size > 0) {
        printf("...file found\n");
        printf("...file size: %lu\n", temp_file_info->size);
        *sectors_size = temp_file_info->size + (SECTOR % temp_file_info->size);
        printf("...file buffer size needed: %d\n", *sectors_size);
        printf("...sectors needed: %d\n", (*sectors_size + SECTOR - 1) / SECTOR);
        *file = (u_long *)malloc3(*sectors_size + SECTOR);

        DsRead(&temp_file_info->pos, (*sectors_size + SECTOR - 1) / SECTOR, *file, DslModeSpeed);
        while (DsReadSync(NULL))
            ;
        printf("...file loaded!\n");
    } else {
        printf("...file not found\n");
    }

    // Clean up
    FREE(file_path_raw);
    FREE(sectors_size);
    FREE(temp_file_info);

    if (filesRead != NULL) {
        *filesRead++;
    }
    printf("File %s loaded into memory addr=%p\n", file_path, *file);
}