#ifndef _logger_h_
#define _logger_h_

#include "STDARG.H"
#include "base.h"

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_DEBUG 2
#define LOG_LEVEL_TRACE 3

/*
 * Log a message at specified level
 * @param level log level (0=info, 1=debug, 2=trace)
 * @param msg the message to print
 */
void log(unsigned char level,char *msg, ...);

#endif