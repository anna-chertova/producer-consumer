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
	std::cout << "PCProducer::start()\n";
	return 0;
}
