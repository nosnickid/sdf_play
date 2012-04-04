#include "Debug.h"

#include "Sys.h"

#include "oglconsole.h"

void _debug_print(const char *msg, va_list varg) 
{
    char res[500];
    vsnprintf(res, sizeof(res), msg, varg);

#ifndef WIN32
    printf(res);
    printf("\n");
#endif
    OGLCONSOLE_Print(res);
    OGLCONSOLE_Print("\n");
}

void fatal(const char *fmt, ...)
{
    va_list list;
    va_start(list, fmt);
    fatal(fmt, list);
    va_end(list);
}

void fatal(const char *fmt, va_list list)
{
    _debug_print(fmt, list);

#ifdef WIN32
	char res[500];
	vsnprintf(res, sizeof(res), fmt, list);
	MessageBoxA(0, res, "Fatel error!", MB_OK|MB_ICONERROR|MB_APPLMODAL);
	//SDL_Quit();
	exit(0);
#else
    OGLCONSOLE_SetVisibility(1);
#endif
}

void warning(const char *fmt, ...)
{
	va_list list;
	va_start(list, fmt);

	OGLCONSOLE_Print("\n\nWARNING:\n");
    _debug_print(fmt, list);
    va_end(list);
    OGLCONSOLE_SetVisibility(1);
	va_end(list);
}

void info(const char *fmt, ...)
{
	va_list list;
	va_start(list, fmt);
    _debug_print(fmt, list);
    va_end(list);
	va_end(list);
}

void debug(const char *fmt, ...)
{
	va_list list;
	va_start(list, fmt);
    _debug_print(fmt, list);
	va_end(list);
}

void checkOpenGL() { checkOpenGL("unspecified"); }

void checkOpenGL(const char *prefix)
{
    int i = glGetError();
    
    while (i != GL_NO_ERROR) {
        fatal("checkOpenGL(%s): %x %s\n", prefix, i, gluErrorString(i));
        i = glGetError();
    }
}

void assERT(bool condition, const char *fmt, ...)
{
	va_list list;
	if (!condition)
	{
		va_start(list, fmt);
		fatal(fmt, list);
		va_end(list);	
	}
}

