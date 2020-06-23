#pragma once
/*
 * pc_shared_buffer.h - this is a header file for my implementation for shared buffer
 * (this a thread-safe buffer)
 * (c) 2020 Anna Chertova
 */

#include <queue>
#include <windows.h>
#include "constants.h"

class PCSharedBuffer {

public:
	PCSharedBuffer();
	~PCSharedBuffer();

	int init();

	bool try_add_item(int item);
	bool try_get_item(int &item);
	int size() const;
	bool is_full() const;

private:
	std::queue<int> buffer;
	mutable HANDLE mutex;		// mutex for protecting access to the buffer
	HANDLE read_event;			// event for signaling when buffer has data to read
	HANDLE write_event;			// event for siganling when buffer has space for writing data

	void cleanup();
};