/*
 * pc_shared_buffer.cpp - this is a source file for my implementation for shared buffer
 * (this a thread-safe buffer)
 * (c) 2020 Anna Chertova
 */

#include <iostream>
#include "pc_shared_buffer.h"
#include "pc_tools.h"

PCSharedBuffer::PCSharedBuffer(): mutex(nullptr), read_event(nullptr), write_event(nullptr)
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

	if (read_event) {
		CloseHandle(read_event);
		read_event = nullptr;
	}

	if (write_event) {
		CloseHandle(write_event);
		write_event = nullptr;
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

	read_event = CreateEvent(NULL,	// default security attributes
		FALSE,						// auto reset event
		FALSE,						// initial state is nonsignaled
		TEXT("PCReadyEvent")		// name
	);

	if (!read_event) {
		std::cerr << "Error creating read event for shared buffer\n";
		PCTools::print_error();
		return 1;
	}

	write_event = CreateEvent(NULL,
		FALSE,
		FALSE,
		TEXT("PCWriteEvent")
	);

	if (!write_event) {
		std::cerr << "Error creating write event for shared buffer\n";
		PCTools::print_error();
		return 1;
	}

	return 0;
}

bool PCSharedBuffer::try_add_item(int item)
{
	// lock mutex before adding item
	DWORD wait_result = WaitForSingleObject(mutex, INFINITE);
	if (wait_result != WAIT_OBJECT_0) {
		std::cerr << "Error adding item: could not lock buffer\n";
		PCTools::print_error();
		return false;
	}

	// if buffer is full, wait
	if (buffer.size() == MAX_BUFFER_SIZE) {
		ReleaseMutex(mutex);

		DWORD wait_event_result = WaitForSingleObject(write_event, WAIT_TIME_MS);
		if (wait_event_result == WAIT_FAILED) {
			std::cerr << "Error adding item: could not wait for write event\n";
			PCTools::print_error();
			return false;
		}
		if (wait_event_result == WAIT_TIMEOUT) {
			// time is out, try next time
			return false;
		}
		wait_result = WaitForSingleObject(mutex, INFINITE);
		if (wait_result != WAIT_OBJECT_0) {
			std::cerr << "Error adding item: could not lock buffer\n";
			PCTools::print_error();
			return false;
		}
	}

	buffer.push(item);
	if (buffer.size() == 1) { // if buffer was empty signal that buffer is ready to be used
		if (!SetEvent(read_event)) {
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
		DWORD wait_event_result = WaitForSingleObject(read_event, INFINITE);
		if (wait_event_result != WAIT_OBJECT_0) {
			std::cerr << "Error getting item: could not wait for read event\n";
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

	// avoid copying (in future this could be an object)
	int item = std::move(buffer.front()); 
	buffer.pop();

	// if buffer was full signal that buffer is ready to be used
	if (buffer.size() < MAX_BUFFER_SIZE) { 
		if (!SetEvent(write_event)) {
			std::cerr << "Error signaling that buffer is not full\n";
			PCTools::print_error();
		}
	}

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

bool PCSharedBuffer::is_full() const
{
	DWORD wait_result = WaitForSingleObject(mutex, INFINITE);
	if (wait_result != WAIT_OBJECT_0) {
		std::cerr << "Error checking if buffer is full: could not lock buffer\n";
		PCTools::print_error();
		return true;
	}
	bool is_full = (buffer.size() == MAX_BUFFER_SIZE);
	ReleaseMutex(mutex);
	return is_full;
}
