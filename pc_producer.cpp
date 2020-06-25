/*
 * pc_producer - this is a source file for my implementation for producer threads
 * (c) 2020 Anna Chertova
 */

#include "pc_producer.h"
#include "pc_shared_ostream.h"

PCProducer::PCProducer(PCSharedBuffer& product_buffer): buffer(product_buffer), engine(rd())
{
	rnd = std::bind(unif, engine);
}

PCProducer::~PCProducer()
{
}

unsigned long PCProducer::start()
{
	shared_cout << "PCProducer::start() id = " << get_id() << "\n";
	bool stop = is_stopped();
	bool success = false;
	int cur_item = generate_next();
	while (!stop) {
		// if previously produced item was queued then generate next
		if (success) {
			cur_item = generate_next();
		}
		success = buffer.try_add_item(cur_item);
		if (success) {
			shared_cout << "Producer id = " << get_id() <<
				"\titem = " << cur_item <<
				"\t\tbuffer size = " << buffer.size() << "\n";
		}
		// check if we should stop the thread
		stop = is_stopped();
		Sleep(SLEEP_TIME_MS);
	}
	return 0;
}

int PCProducer::generate_next()
{
	return rnd();
}
