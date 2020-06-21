/*
 * pc_producer - this is a source file for my implementation for producer threads
 * (c) 2020 Anna Chertova
 */

#include "pc_producer.h"
#include <iostream>

PCProducer::PCProducer(PCSharedBuffer& product_buffer): buffer(product_buffer), engine(rd())
{
	rnd = std::bind(unif, engine);
}

PCProducer::~PCProducer()
{
}

unsigned long PCProducer::start()
{
	std::cout << "PCProducer::start() id = " << get_id() << "\n";
	bool stop = is_stopped();
	while (!stop) {		
		int produced_item = generate_next();
		buffer.add_item(produced_item);
		std::cout << "Producer id = " << get_id() <<
			" item = " << produced_item <<
			" buffer size = " << buffer.size() << "\n";
		// check if we should stop the thread
		stop = is_stopped();
	}
	return 0;
}

int PCProducer::generate_next()
{
	return rnd();
}
