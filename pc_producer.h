/*
 * pc_producer - this is a header file for my implementation for producer threads
 * (c) 2020 Anna Chertova
 */

#pragma once
#include "pc_thread.h"
#include "pc_shared_buffer.h"

class PCProducer : public PCThread {

public:

	PCProducer(PCSharedBuffer &product_buffer);
	virtual ~PCProducer();

private:

	virtual unsigned long start();

	int generate_next();

	PCSharedBuffer buffer;
};