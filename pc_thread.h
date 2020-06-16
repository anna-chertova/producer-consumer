/*
 * pc_thread.h - this is a header file for my implementation for threads
 * (c) 2020 Anna Chertova
 */
#include <windows.h>

class PCThread {

public:

	/// TODO: think about copy, move constructor and operator =
	/// What should we do: forbid using them, define them or can leave
	/// default versions to live on
	PCThread();

	/// TODO: think if this should be virtual? Do we want to use this class for
	/// inheritance
	virtual ~PCThread();

	int init();
	int wait();

private:

	HANDLE handle;

	unsigned long start();

	static unsigned long __stdcall static_thread_start(void* param);
	static void print_error();
};