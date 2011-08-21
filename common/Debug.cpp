#include "Debug.h"

#include "Sys.h"

#include "oglconsole.h"


void fatal(const char *msg, ...) 
{
	va_list list;
	va_start(list, msg);
	char res[200];
	vsnprintf(res, sizeof(res), msg, list);
	va_end(list);

	OGLCONSOLE_Print(res);
	OGLCONSOLE_Print("\n");

#ifdef WIN32
	MessageBoxA(0, res, "Fatel error!", MB_OK|MB_ICONERROR|MB_APPLMODAL);
	//SDL_Quit();
	exit(0);
#endif
}

void warning(const char *fmt, ...)
{
	va_list list;
	va_start(list, fmt);
	OGLCONSOLE_Print("\n\nWARNING:\n");
	OGLCONSOLE_Print(fmt, va_pass(list));
	OGLCONSOLE_Print("\n");
	va_end(list);
}

void info(const char *fmt, ...)
{
	va_list list;
	va_start(list, fmt);
	OGLCONSOLE_Print(fmt, va_pass(list));
	OGLCONSOLE_Print("\n");
	va_end(list);
}

void debug(const char *fmt, ...)
{
	va_list list;
	va_start(list, fmt);
	OGLCONSOLE_Print(fmt, va_pass(list));
	OGLCONSOLE_Print("\n");
	va_end(list);
}

void assERT(bool condition, const char *fmt, ...)
{
	va_list list;
	if (!condition)
	{
		va_start(list, fmt);
		fatal(fmt, va_pass(list));
		va_end(list);	
	}
}

