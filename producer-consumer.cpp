/* 
 * producer-consumer.cpp: this file contains main func
 * (c) 2020 Anna Chertova
 */

#include <iostream>
#include <queue>
#include "pc_producer.h"
#include "pc_consumer.h"
#include "pc_shared_buffer.h"

// These constants to be used in a final solution
#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 1
#define NUM_REPEATS 1
#define WAIT_TIME 0

int main()
{
    PCSharedBuffer buffer;
    buffer.init();

    PCProducer my_producer(buffer);
    my_producer.init();
    
    PCConsumer my_consumer(buffer);
    my_consumer.init();

    my_producer.wait();
    my_consumer.wait();

    return 0;
}
