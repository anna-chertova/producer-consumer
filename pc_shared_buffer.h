#pragma once
/*
 * pc_shared_buffer.h - this is a header file for shared (synchronized)
 * buffer implementation
 * (c) 2020 Anna Chertova
 */

#include <queue>
#include <windows.h>
#include "constants.h"

class PCSharedBuffer {

public:

	// Initializes all necessary synchronization primitives
	PCSharedBuffer();
	// Cleans up used resources
	~PCSharedBuffer();

	// Tries to add item into queue
	// Returns true on success, false on failure
	// If queue is full (determined by MAX_BUFFER_SIZE) then
	// waits for empty space in buffer
	// Uses WAIT_TIME_MS as timeout (to avoid blocking forever)
	bool try_add_item(int item);

	// Tries to get item from queue
	// Returns true on success, false on failure
	// If queue is empty then waits for items to be enqueued
	// Uses WAIT_TIME_MS as timeout (to avoid blcoking forever)
	bool try_get_item(int &item);

	// Returns current queue size
	// (thread-safe)
	int size() const;

	// Checks if there is empty space in queue
	// (thread-safe)
	bool is_full() const;

private:

	std::queue<int> buffer;
	// mutex for protecting access to the buffer
	mutable HANDLE mutex;
	// event for signaling when buffer has data to read
	HANDLE read_event;
	// event for siganling when buffer has space for writing data
	HANDLE write_event;

	int init();
	void cleanup();
};