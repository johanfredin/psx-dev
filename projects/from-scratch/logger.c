#include "header/logger.h"

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