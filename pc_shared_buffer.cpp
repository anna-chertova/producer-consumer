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
	cleanup();
}

void PCSharedBuffer::cleanup()
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
	// to avoid leaks in case of re-initialization
	cleanup();
	
	mutex = CreateMutex(NULL,	// default security attributes
		false,					// initially not owned
		NULL					// unnamed
	);

	if (!mutex) {
		std::cerr << "Error creating mutex for shared buffer\n";
		PCTools::print_error();
		return 1;
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

bool PCSharedBuffer::add_item(int item)
{
	// lock mutex before adding item
	DWORD wait_result = WaitForSingleObject(mutex, INFINITE);
	if (wait_result != WAIT_OBJECT_0) {
		std::cerr << "Error adding item: could not lock buffer\n";
		PCTools::print_error();
		return false;
	}
	buffer.push(item);
	if (buffer.size() == 1) { // if buffer was empty signal that buffer is ready to be used
		if (!SetEvent(ready_event)) {
			std::cerr << "Error signaling that buffer is not empty\n";
			PCTools::print_error();
		}
	}
	// release mutex after add is finished
	ReleaseMutex(mutex);
	return true;
}

int PCSharedBuffer::get_item()
{
	DWORD wait_result = WaitForSingleObject(mutex, INFINITE);
	if (wait_result != WAIT_OBJECT_0) {
		std::cerr << "Error getting item: could not lock buffer\n";
		PCTools::print_error();
		return 0;
	}

	if (buffer.size() == 0) {
		ReleaseMutex(mutex);
		DWORD wait_event_result = WaitForSingleObject(ready_event, INFINITE);
		if (wait_event_result != WAIT_OBJECT_0) {
			std::cerr << "Error getting item: could not wait for ready event\n";
			PCTools::print_error();
			return 0;
		}
		wait_result = WaitForSingleObject(mutex, INFINITE);
		if (wait_result != WAIT_OBJECT_0) {
			std::cerr << "Error getting item: could not lock buffer\n";
			PCTools::print_error();
			return 0;
		}
	}

	int item = buffer.front();
	buffer.pop();
	ReleaseMutex(mutex);
	return item;
}

int PCSharedBuffer::size() const
{
	DWORD wait_result = WaitForSingleObject(mutex, INFINITE);
	if (wait_result != WAIT_OBJECT_0) {
		std::cerr << "Error getting size: could not lock buffer\n";
		PCTools::print_error();
		return -1;
	}
	int cur_size = buffer.size();
	ReleaseMutex(mutex);
	return cur_size;
}
