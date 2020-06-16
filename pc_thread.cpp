/*
 * pc_thread.cpp - this is an implementation file for my threads
 * (c) 2020 Anna Chertova
 */
#include "pc_thread.h"
#include <iostream>

PCThread::PCThread(): handle(nullptr)
{
	
}

PCThread::~PCThread()
{
	if (handle) {
		CloseHandle(handle);
		handle = nullptr;
	}
}

int PCThread::init()
{
	unsigned long thread_id(0L);
	//DWORD thread_id(0);

	handle = CreateThread(
		nullptr,				// default security attributes
		0,					// default stack size
		static_thread_start,// thread function name
		(void*)this,		// arguments to thread function
		0,					// default creation flags
		&thread_id			// return thread id
		);
	
	if (handle == nullptr) {
		// Error creating thread
		std::cerr << "Error creating thread\n";
		print_error();		
		return 1;
	}	
	return 0;
}

int PCThread::wait()
{
	if (handle == nullptr)
	{
		std::cerr << "Thread is not initialized\n";
		return 1;
	}
	DWORD result = WaitForSingleObject(handle, INFINITE);
	if (result != WAIT_OBJECT_0) {
		std::cerr << "Wait failed\n";
		return 1;
	}
	return 0;
}

unsigned long PCThread::start()
{
	std::cout << "PCThread::start()\n";
	return 0;
}

DWORD __stdcall PCThread::static_thread_start(void* param)
{
	PCThread* cur_thread = (PCThread*)param;
	return cur_thread->start();
}

void PCThread::print_error()
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

	//Free the buffer.
	LocalFree(message_buf);

	std::cerr << message;
}
