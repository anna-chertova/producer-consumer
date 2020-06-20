/*
 * pc_producer - this is a header file for my implementation for producer threads
 * (c) 2020 Anna Chertova
 */

#include "pc_thread.h"

class PCProducer : public PCThread {

public:

	virtual ~PCProducer();

private:

	virtual unsigned long start();
};