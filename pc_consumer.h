/*
 * pc_consumer.h - this is a header file consumer thread implementation
 * (c) 2020 Anna Chertova
 */

#pragma once
#include "pc_thread.h"
#include "pc_shared_buffer.h"

// Consumer thread takes numbers from queue
// And outputs them to console
// Delay before each step can be set by CONSUMER_DELAY_MS
class PCConsumer : public PCThread {

public:
	PCConsumer(PCSharedBuffer &consume_buffer);
	virtual ~PCConsumer();

private:
	virtual unsigned long start() override;
	PCSharedBuffer& buffer;
};
