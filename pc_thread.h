#pragma once
/*
 * pc_thread.h - this is a header file for basic threads implementation
 * (c) 2020 Anna Chertova
 */
#include <windows.h>

class PCThread {

public:

	// Constructs and runs thread
	// start() function is used as a thread function
	PCThread();
	// Cleans resources up
	virtual ~PCThread();

	// Wait for thread to finish its job
	int wait();
	// Sends thread stop event
	// To let the thread finish its job gracefully
	void stop();
	// Get thread id
	unsigned long get_id() const;

protected:

	// This functions is used as a thread func
	// Should be overriden in child threads to implement
	// their own functionality
	virtual unsigned long start();
	// Checks if the thread has received stop event
	bool is_stopped();

private:

	HANDLE handle;
	DWORD thread_id;
	HANDLE stop_event;
	
	// Initializes thread
	int init();
	// Cleans resources up
	void cleanup();

	static unsigned long __stdcall static_thread_start(void* param);
};