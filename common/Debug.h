#ifndef DEBUG_H__
#define DEBUG_H__

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "oglconsole.h"

/*template<char count>
struct SVaPassNext{
	SVaPassNext<count-1> big;
	unsigned long dw;
};
template<> struct SVaPassNext<0>{};
//SVaPassNext - is generator of structure of any size at compile time.

class CVaPassNext{
public:
	SVaPassNext<50> svapassnext;
	CVaPassNext(va_list & args){
		try{//to avoid access violation
			memcpy(&svapassnext, args, sizeof(svapassnext));
		} catch (...) {}
	}
};
HASHdefine va_pass(valist) CVaPassNext(valist).svapassnext
*/


void fatal(const char *fmt, ...);
void warning(const char *fmt, ...);
void info(const char *fmt, ...);
void debug(const char *fmt, ...);
void checkOpenGL(const char *);
void checkOpenGL();


void fatal(const char *fmt, va_list varg);
void warning(const char *fmt, va_list varg);
void info(const char *fmt, va_list varg);
void debug(const char *fmt, va_list varg);


void assERT(bool condition, const char *fmt, ...);

#endif