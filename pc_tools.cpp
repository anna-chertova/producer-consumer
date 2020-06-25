/*
 * pc_tools.cpp - this is a source file for utility functions
 * (c) 2020 Anna Chertova
 */

#include <windows.h>
#include <iostream>
#include "pc_tools.h"
#include "pc_shared_ostream.h"

void PCTools::print_error()
{
	DWORD err_msg_id = GetLastError();
	LPSTR message_buf = nullptr;
	size_t size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		err_msg_id,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&message_buf,
		0,
		NULL);

	std::string message(message_buf, size);
	shared_cerr << message;
	//Free the buffer.
	LocalFree(message_buf);
}