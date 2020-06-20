/*
 * pc_shared_buffer.cpp - this is a source file for my implementation for shared buffer
 * (this a thread-safe buffer)
 * (c) 2020 Anna Chertova
 */

#include <iostream>
#include "pc_shared_buffer.h"
#include "pc_tools.h"

PCSharedBuffer::PCSharedBuffer(): mutex(nullptr), ready_event(nullptr)
{
}

PCSharedBuffer::~PCSharedBuffer()
{
	if (mutex) {
		ReleaseMutex(mutex);
		CloseHandle(mutex);
		mutex = nullptr;
	}

	if (ready_event) {
		CloseHandle(ready_event);
		ready_event = nullptr;
	}
}

int PCSharedBuffer::init()
{
	if (mutex) {
		CloseHandle(mutex);
		mutex = nullptr;
	}

	mutex = CreateMutex(NULL,	// default security attributes
		false,					// initially not owned
		NULL					// unnamed
	);

	if (!mutex) {
		std::cerr << "Error creating mutex for shared buffer\n";
		PCTools::print_error();
		return 1;
	}

	if (ready_event) {
		CloseHandle(ready_event);
		ready_event = nullptr;
	}

	ready_event = CreateEvent(NULL,		// default security attributes
		TRUE,						// manually reset event
		FALSE,						// initital state is nonsignaled
		TEXT("PCReadyEvent")		// name
	);

	if (!ready_event) {
		std::cerr << "Error creating ready event for shared buffer\n";
		PCTools::print_error();
		return 1;
	}

	return 0;
}

void PCSharedBuffer::add_item(int item)
{
	// lock mutex before adding item
	DWORD wait_result = WaitForSingleObject(mutex, INFINITE);
	if (wait_result != WAIT_OBJECT_0) {
		std::cerr << "Error adding item: could not lock buffer\n";
		PCTools::print_error();
		return;
	}
	buffer.push(item);
	// release mutex after add is finished
	ReleaseMutex(mutex);
}

int PCSharedBuffer::get_item()
{
	DWORD wait_result = WaitForSingleObject(mutex, INFINITE);
	if (wait_result != WAIT_OBJECT_0) {
		std::cerr << "Error getting item: could not lock buffer\n";
		PCTools::print_error();
		return 0;
	}
	int item = buffer.front();
	buffer.pop();
	ReleaseMutex(mutex);
	return item;
}
