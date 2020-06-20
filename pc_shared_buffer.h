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

private:
	std::queue<int> buffer;
};