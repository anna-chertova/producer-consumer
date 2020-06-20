/*
 * pc_producer - this is a source file for my implementation for producer threads
 * (c) 2020 Anna Chertova
 */

#include "pc_producer.h"
#include <iostream>

PCProducer::PCProducer(PCSharedBuffer& product_buffer): buffer(product_buffer)
{
}

PCProducer::~PCProducer()
{
}

unsigned long PCProducer::start()
{
	std::cout << "PCProducer::start() id = " << get_id() << "\n";
	bool success = true;
	while (success) {
		int produced_item = generate_next();
		success = buffer.add_item(produced_item);
		std::cout << "Producer id = " << get_id() <<
			" item = " << produced_item <<
			" buffer size = " << buffer.size() << "\n";
	}
	return 0;
}

int PCProducer::generate_next()
{
	return 0;
}
