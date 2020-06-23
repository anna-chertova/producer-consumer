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
    // allow usage of Cyrillic symbols in console
    setlocale(0, "");

    std::cout << "\nThis program demonstrates producer/consumer problem solution\n";
    std::cout << "Enter any key to stop the program\n\n";

    // This buffer contains "tasks".
    // For now those are simple integer numbers.
    PCSharedBuffer buffer;
    buffer.init();

    // Producer worker thread. Generates random integer numbers
    PCProducer my_producer(buffer);
    my_producer.init();
    
    // Consumer worker thread. Reads integer numbers and prints them.
    PCConsumer my_consumer(buffer);
    my_consumer.init();

    // Main thread waits for user input
    char a;
    std::cin >> a;

    // Send producer and consumer threads stop command
    my_producer.stop();
    my_consumer.stop();

    // Wait threads to finalyze their work
    my_producer.wait();
    my_consumer.wait();

    return 0;
}
