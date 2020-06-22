/*
 * pc_consumer.cpp - this is a source file for my implementation for consumer thread
 * (c) 2020 Anna Chertova
 */

#include "pc_consumer.h"
#include <iostream>

PCConsumer::PCConsumer(PCSharedBuffer& consume_buffer): buffer(consume_buffer)
{
}

PCConsumer::~PCConsumer()
{
}

unsigned long PCConsumer::start()
{
	std::cout << "PCConsumer::start() id = " << get_id() << "\n";
	bool stop = is_stopped();
	while (!stop) {
		int next_item = buffer.get_item();
		std::cout << "Consumer id = " << get_id() <<
			" item = " << next_item <<
			" buffer size = " << buffer.size() << "\n";
		stop = is_stopped();
		Sleep(WAIT_TIME_MS);
	}
	return 0;
}
