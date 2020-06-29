/*
 * pc_shared_buffer.cpp - this is a source file for shared (synchronized)
 * buffer implementation
 * (c) 2020 Anna Chertova
 */

#include <iostream>
#include <cassert>
#include "pc_shared_buffer.h"
#include "pc_tools.h"
#include "pc_shared_ostream.h"

PCSharedBuffer::PCSharedBuffer(): mutex(nullptr), read_event(nullptr),
	write_event(nullptr)
{
	init();
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
	mutex = CreateMutex(NULL,	// default security attributes
		false,					// initially not owned
		NULL					// unnamed
	);

	if (!mutex) {
		shared_cerr << "Error creating mutex for shared buffer\n";
		PCTools::print_error();
		return 1;
	}

	read_event = CreateEvent(NULL,	// default security attributes
		FALSE,						// auto reset event
		FALSE,						// initial state is nonsignaled
		TEXT("PCReadyEvent")		// name
	);

	if (!read_event) {
		shared_cerr << "Error creating read event for shared buffer\n";
		PCTools::print_error();
		return 1;
	}

	write_event = CreateEvent(NULL,
		FALSE,
		FALSE,
		TEXT("PCWriteEvent")
	);

	if (!write_event) {
		shared_cerr << "Error creating write event for shared buffer\n";
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
		shared_cerr << "Error adding item: could not lock buffer\n";
		PCTools::print_error();
		return false;
	}

  // if buffer is full, wait
	while (buffer.size() == MAX_BUFFER_SIZE) {
		DWORD wait_event_result = WaitForSingleObject(write_event, WAIT_TIME_MS);
		if (wait_event_result == WAIT_FAILED) {
			shared_cerr <<
				"Error adding item: could not wait for write event\n";
			PCTools::print_error();
			ReleaseMutex(mutex);
			return false;
		}
		if (wait_event_result == WAIT_TIMEOUT) {
			// time is out, try next time
			ReleaseMutex(mutex);
			return false;
		}
	}

	assert(buffer.size() < MAX_BUFFER_SIZE);

	buffer.push(item);
	// if buffer was empty signal that buffer is ready to be used
	if (buffer.size() == 1) {
		if (!SetEvent(read_event)) {
			shared_cerr << "Error signaling that buffer is not empty\n";
			PCTools::print_error();
		}
	}
	// release mutex after add is finished
	ReleaseMutex(mutex);
	return true;
}

bool PCSharedBuffer::try_get_item(int& item)
{
	// lock before getting item
	DWORD wait_result = WaitForSingleObject(mutex, INFINITE);
	if (wait_result != WAIT_OBJECT_0) {
		shared_cerr << "Error getting item: could not lock buffer\n";
		PCTools::print_error();
		return false;
	}

	// if buffer is empty wait
		while (buffer.empty()) {
		DWORD wait_event_result = WaitForSingleObject(read_event, WAIT_TIME_MS);
		if (wait_event_result == WAIT_FAILED) {
			shared_cerr << "Error getting item: could not wait for read event\n";
			PCTools::print_error();
			ReleaseMutex(mutex);
			return false;
		}
		if (wait_event_result == WAIT_TIMEOUT) {
			// time is out, try next time
			ReleaseMutex(mutex);
			return false;
		}
	}

	assert(!buffer.empty());
	bool signal_write_event = (buffer.size() == MAX_BUFFER_SIZE);
	assert(buffer.size() <= MAX_BUFFER_SIZE);

	// avoid copying (in future this could be an object)
	item = std::move(buffer.front()); 
	buffer.pop();

	assert(buffer.size() < MAX_BUFFER_SIZE);

	// if buffer was full signal that buffer is ready to be used
	if (signal_write_event) { 
		if (!SetEvent(write_event)) {
			shared_cerr << "Error signaling that buffer is not full\n";
			PCTools::print_error();
		}
	}

	ReleaseMutex(mutex);
	return true;
}

int PCSharedBuffer::size() const
{
	DWORD wait_result = WaitForSingleObject(mutex, INFINITE);
	if (wait_result != WAIT_OBJECT_0) {
		shared_cerr << "Error getting size: could not lock buffer\n";
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
		shared_cerr << "Error checking if buffer is full: could not lock buffer\n";
		PCTools::print_error();
		return true;
	}
	bool is_full = (buffer.size() == MAX_BUFFER_SIZE);
	ReleaseMutex(mutex);
	return is_full;
}
