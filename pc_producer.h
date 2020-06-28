/*
 * pc_producer - this is a header file for my implementation for producer threads
 * (c) 2020 Anna Chertova
 */

#pragma once
#include <random>
#include <functional>
#include "pc_thread.h"
#include "pc_shared_buffer.h"

class PCProducer : public PCThread {

public:

	PCProducer(PCSharedBuffer &product_buffer);
	virtual ~PCProducer();

private:

	virtual unsigned long start() override;

	int generate_next();

	PCSharedBuffer& buffer;

	// for producing random numbers
	std::uniform_int_distribution<int> unif;
	std::random_device rd;
	std::mt19937 engine;
	std::function<int()> rnd;
};