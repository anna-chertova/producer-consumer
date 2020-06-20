/*
 * pc_thread.h - this is a header file for my implementation for threads
 * (c) 2020 Anna Chertova
 */
#pragma once
#include <windows.h>

class PCThread {

public:

	/// TODO: think about copy, move constructor and operator =
	/// What should we do: forbid using them, define them or can leave
	/// default versions to live on
	PCThread();

	virtual ~PCThread();

	int init();
	int wait();
	unsigned long get_id() const;

protected:

	virtual unsigned long start();

private:

	HANDLE handle;
	DWORD thread_id;
	
	void cleanup();

	static unsigned long __stdcall static_thread_start(void* param);
};