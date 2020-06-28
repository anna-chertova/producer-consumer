/* 
 * producer-consumer.cpp: this file contains main func
 * (c) 2020 Anna Chertova
 */

#include <iostream>
#include <vector>
#include <memory>
#include "pc_producer.h"
#include "pc_consumer.h"
#include "pc_shared_buffer.h"
#include "constants.h"
#include "pc_shared_ostream.h"

int main()
{
    // allow usage of Cyrillic symbols in console
    setlocale(0, "");

    std::cout << "\nThis program demonstrates producer/consumer problem solution\n";
    std::cout << "Enter any key to stop the program\n";
    std::cout << "Number of program calls: " << NUM_REPEATS << "\n\n";

    for (int n = 0; n < NUM_REPEATS; ++n) {

        std::cout << "\n\nCall # " << (n + 1) << "/" << NUM_REPEATS << "\n\n";
        // This buffer contains "tasks".
        // For now those are simple integer numbers.
        PCSharedBuffer buffer;
        buffer.init();

        std::vector<std::unique_ptr<PCProducer>> producers;
        std::vector<std::unique_ptr<PCConsumer>> consumers;

        // Producer worker threads that generate random integer numbers.
        for (int i = 0; i < NUM_PRODUCERS; i++) {
            auto p(std::make_unique<PCProducer>(buffer));
            p->init();
            producers.push_back(std::move(p));
        }

        // Consumer worker threads. Read integer numbers and print them to console.
        for (int i = 0; i < NUM_CONSUMERS; i++) {
            auto c(std::make_unique<PCConsumer>(buffer));
            c->init();
            consumers.push_back(std::move(c));
        }

        // Main thread waits for user input
        char a;
        std::cin >> a;

        // Send producer and consumer threads stop command
        for (auto p = producers.begin(); p != producers.end(); ++p) {
            p->get()->stop();
        }
        for (auto c = consumers.begin(); c != consumers.end(); ++c) {
            c->get()->stop();
        }

        // Wait threads to finalyze their work
        for (auto p = producers.begin(); p != producers.end(); ++p) {
            p->get()->wait();
        }
        for (auto c = consumers.begin(); c != consumers.end(); ++c) {
            c->get()->wait();
        }
    }

    return 0;
}
