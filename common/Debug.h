#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "oglconsole.h"

template<char count>
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
#define va_pass(valist) CVaPassNext(valist).svapassnext


void fatal(const char *fmt, ...);
void warning(const char *fmt, ...);
void info(const char *fmt, ...);
void debug(const char *fmt, ...);

void assERT(bool condition, const char *fmt, ...);


