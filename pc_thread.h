#pragma once
/*
 * pc_thread.h - this is a header file for my implementation for threads
 * (c) 2020 Anna Chertova
 */
#include <windows.h>

class PCThread {

public:

	PCThread();
	virtual ~PCThread();

	int init();
	int wait();
	unsigned long get_id() const;
	void stop();

protected:

	virtual unsigned long start();
	bool is_stopped();

private:

	HANDLE handle;
	DWORD thread_id;
	HANDLE stop_event;
	
	void cleanup();

	static unsigned long __stdcall static_thread_start(void* param);
};