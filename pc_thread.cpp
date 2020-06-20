/*
 * pc_thread.cpp - this is an implementation file for my threads
 * (c) 2020 Anna Chertova
 */
#include <iostream>

#include "pc_thread.h"
#include "pc_tools.h"

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
		PCTools::print_error();		
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