/*
 * pc_consumer.cpp - this is a source file for my implementation for consumer thread
 * (c) 2020 Anna Chertova
 */

#include "pc_consumer.h"
#include "pc_shared_ostream.h"

PCConsumer::PCConsumer(PCSharedBuffer& consume_buffer): buffer(consume_buffer)
{
}

PCConsumer::~PCConsumer()
{
}

unsigned long PCConsumer::start()
{
	shared_cout << "PCConsumer::start() id = " << get_id() << "\n";
	bool stop = is_stopped();
	int cur_item(0);
	while (!stop) {
		Sleep(CONSUMER_DELAY_MS);
		if (buffer.try_get_item(cur_item)) {
			shared_cout << "Consumer id = " << get_id() <<
				"\titem = " << cur_item <<
				"\t\tbuffer size = " << buffer.size() << "\n";
		}
		stop = is_stopped();
		Sleep(SLEEP_TIME_MS);
	}
	return 0;
}
