#ifndef DEBUG_H__
#define DEBUG_H__

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "oglconsole.h"

void fatal(const char *fmt, ...);
void warning(const char *fmt, ...);
void info(const char *fmt, ...);
void debug(const char *fmt, ...);
void checkOpenGL(const char *);
void checkOpenGL();

void assERT(bool condition, const char *fmt, ...);

#endif