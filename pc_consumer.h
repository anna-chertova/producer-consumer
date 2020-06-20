/*
 * pc_consumer.h - this is a header file for my implementation for consumer thread
 * (c) 2020 Anna Chertova
 */

#pragma once
#include "pc_thread.h"

class PCConsumer : public PCThread {

public:
	virtual ~PCConsumer();

private:
	virtual unsigned long start();
};
