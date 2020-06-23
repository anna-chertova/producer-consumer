/*
 * pc_consumer.h - this is a header file for my implementation for consumer thread
 * (c) 2020 Anna Chertova
 */

#pragma once
#include "pc_thread.h"
#include "pc_shared_buffer.h"

class PCConsumer : public PCThread {

public:
	PCConsumer(PCSharedBuffer &consume_buffer);
	virtual ~PCConsumer();

private:
	virtual unsigned long start();
	PCSharedBuffer& buffer;
};
