#include "header/logger.h"
#include <STRINGS.H>

void log_i(char *msg, ...) {
    log(LOG_LEVEL_INFO, msg);
}

void log_d(char *msg, ...) {
    log(LOG_LEVEL_DEBUG, msg);
}

void log_tr(char *msg, ...) {
    if (LOG_LEVEL >= LOG_LEVEL_TRACE) {
        printf(msg);
        printf("\n");
    }
}

void log_err(char *msg, ...) {
    if (LOG_LEVEL >= LOG_LEVEL_ERROR) {
        printf(msg);
        printf("\n");
    }
}

void log(unsigned char level, char* c, ...) {
    va_list lst;
    va_start(lst, c);
    if (LOG_LEVEL >= level) {
        while (*c != '\0') {
            if (*c != '%') {
                putchar(*c);
                c++;
                continue;
            }
            ++c;
            switch (*c) {
            case 's':
                printf("%s", va_arg(lst, char*));
                break;
            case 'd':
                printf("%d", va_arg(lst, int));
                break;
            }
            c++;
        }
        putchar('\n');
    }
}