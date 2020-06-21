/* 
 * producer-consumer.cpp: this file contains main func
 * (c) 2020 Anna Chertova
 */

#include <iostream>
#include <queue>
#include "pc_producer.h"
#include "pc_consumer.h"
#include "pc_shared_buffer.h"
#include "constants.h"

int main()
{
    PCSharedBuffer buffer;
    buffer.init();

    PCProducer my_producer(buffer);
    my_producer.init();
    
    PCConsumer my_consumer(buffer);
    my_consumer.init();

    char a;
    std::cin >> a;
    my_producer.stop();
    my_consumer.stop();

    my_producer.wait();
    my_consumer.wait();

    return 0;
}
