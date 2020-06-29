/*
 * pc_thread.cpp - this is an implementation file for my threads
 * (c) 2020 Anna Chertova
 */
#include <iostream>

#include "pc_thread.h"
#include "pc_tools.h"
#include "constants.h"
#include "pc_shared_ostream.h"

PCThread::PCThread(): handle(nullptr), thread_id(0L), stop_event(nullptr)
{
	init();
}

PCThread::~PCThread()
{
	cleanup();
}

unsigned long PCThread::get_id() const
{
	return thread_id;
}

void PCThread::cleanup()
{
	if (handle) {
		CloseHandle(handle);
		handle = nullptr;
		
	}

	if (stop_event) {
		CloseHandle(stop_event);
		stop_event = nullptr;
	}

	thread_id = 0L;
}

int PCThread::init()
{
	handle = CreateThread(
		nullptr,				// default security attributes
		0,						// default stack size
		static_thread_start,	// thread function name
		(void*)this,			// arguments to thread function
		0,						// default creation flags
		&thread_id				// return thread id
	);

	if (handle == nullptr) {
		// Error creating thread
		shared_cerr << "Error creating thread\n";
		PCTools::print_error();
		return 1;
	}

	stop_event = CreateEvent(
		NULL,				// default security attributes
		TRUE,				// manual reset event
		FALSE,				// initial state is nonsignaled
		NULL				// unnamed
	);

	if (stop_event == nullptr) {
		// Error creating stop evetn
		shared_cerr << "Error creating stop event\n";
		PCTools::print_error();
		return 1;
	}

	return 0;
}

int PCThread::wait()
{
	if (handle == nullptr)
	{
		shared_cerr << "Thread is not initialized\n";
		return 1;
	}

	DWORD result = WaitForSingleObject(handle, INFINITE);
	if (result != WAIT_OBJECT_0) {
		shared_cerr << "Wait failed\n";
		return 1;
	}
	return 0;
}

unsigned long PCThread::start()
{
	return 0;
}

void PCThread::stop()
{
	if (!stop_event)
		return;

	if (!SetEvent(stop_event)) {
		shared_cerr << "Error stopping thread = " << thread_id << "\n";
	}
}

bool PCThread::is_stopped()
{
	DWORD wait_result = WaitForSingleObject(stop_event, WAIT_TIME_MS);
	if (wait_result == WAIT_OBJECT_0) {
		return true;
	}
	if (wait_result == WAIT_FAILED) {
		shared_cerr << "Error waiting on stop event\n";
		PCTools::print_error();
	}
	return false;
}

DWORD __stdcall PCThread::static_thread_start(void* param)
{
	PCThread* cur_thread = (PCThread*)param;
	return cur_thread->start();
}