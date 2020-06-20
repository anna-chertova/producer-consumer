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
	std::cout << "PCConsumer::start()\n";
	return 0;
}
