#include "utilities/logger.h"

#include <atlstr.h>

c_logger::c_logger()
{

}

c_logger::~c_logger()
{

}

void c_logger::error(const char *format, ...)
{
	CString text;
	va_list args;
	va_start(args, format);
	text.FormatV(format, args);
	va_end(args);
	text.Append("\n");

	OutputDebugString((LPCTSTR)text);
}