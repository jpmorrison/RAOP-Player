/*
 *  Squeezelite - lightweight headless squeezebox emulator
 *
 *  (c) Adrian Smith 2012-2015, triode1@btinternet.com
 *  (c) Philippe, philippe_44@outlook.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "platform.h"
#include "log_util.h"

// for printf format errors that cause exceptions in MSVC
#define WINDEBUG 1  

#if WINDEBUG
#include <crtdbg.h>  // For _CrtSetReportMode

void log_utilInvalidParameterHandler(const wchar_t* expression,
	const wchar_t* function,
	const wchar_t* file,
	unsigned int line,
	uintptr_t pReserved)
{
	//     wprintf(L"Invalid parameter detected in function %s."
	//             L" File: %s Line: %d\n", function, file, line);
	//     wprintf(L"Expression: %s\n", expression);
	//     abort();
}


#endif

extern log_level 	util_loglevel;
//static log_level 	*loglevel = &util_loglevel;

// logging functions
const char *logtime(void) {
	static char buf[1000];
#if WIN
	SYSTEMTIME lt;
	GetLocalTime(&lt);
	sprintf_s(buf, sizeof(buf), "[%02d:%02d:%02d.%03d]", lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	strftime(buf, sizeof(buf), "[%T.", localtime(&tv.tv_sec));
	sprintf(buf+strlen(buf), "%06ld]", (long)tv.tv_usec);
#endif
	return buf;
}

/*---------------------------------------------------------------------------*/
void logprint(const char *fmt, ...) {
	va_list args;

#if WINDEBUG
	_invalid_parameter_handler oldHandler, newHandler;
	newHandler = log_utilInvalidParameterHandler;
	oldHandler = _set_invalid_parameter_handler(newHandler);

	// Disable the message box for assertions.
	_CrtSetReportMode(_CRT_ASSERT, 0);

#endif
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fflush(stderr);
#if WINDEBUG
	_CrtSetReportMode(_CRT_ASSERT, 1);
	_set_invalid_parameter_handler(oldHandler);
#endif
}

/*---------------------------------------------------------------------------*/
log_level debug2level(char *level)
{
	if (!strcmp(level, "error")) return lERROR;
	if (!strcmp(level, "warn")) return lWARN;
	if (!strcmp(level, "info")) return lINFO;
	if (!strcmp(level, "debug")) return lDEBUG;
	if (!strcmp(level, "sdebug")) return lSDEBUG;
	return lWARN;
}

/*---------------------------------------------------------------------------*/
char *level2debug(log_level level)
{
	switch (level) {
	case lERROR: return "error";
	case lWARN: return "warn";
	case lINFO: return "info";
	case lDEBUG: return "debug";
	case lSDEBUG: return "debug";
	default: return "warn";
	}
}
