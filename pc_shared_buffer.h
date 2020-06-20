#pragma once
/*
 * pc_shared_buffer.h - this is a header file for my implementation for shared buffer
 * (this a thread-safe buffer)
 * (c) 2020 Anna Chertova
 */

#include <queue>
#include <windows.h>

class PCSharedBuffer {

public:
	PCSharedBuffer();
	~PCSharedBuffer();

	int init();

private:
	std::queue<int> buffer;
	HANDLE mutex; // mutex for protecting access to the buffer
	HANDLE ready_event; // event for signaling when buffer is ready
};