#ifndef _logger_h_
#define _logger_h_

#include "STDARG.H"
#include "base.h"


#define ERROR   0
#define INFO    1
#define DEBUG   2
#define TRACE   3

/*
 * Log a message at specified level
 * @param level log level (0=err, 1=info, 2=debug, 3=trace)
 * @param msg the message to print
 */
void log(unsigned char level, char *msg, ...);

#endif