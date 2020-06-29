/* 
 * producer-consumer.cpp: this file contains main func
 * This application implements Producer/Consumer problem
 * Using WINAPI thread functions
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
    // Set environment's default locale 
    // (to be able to use Cyrillic symbols in console)
    setlocale(0, "");

    std::cout << "\nThis program demonstrates producer/consumer problem solution\n";
    std::cout << "Press Enter to run next set of producers/consumers\n";
    std::cout << "Number of program calls: " << NUM_REPEATS << "\n";

    for (int n = 0; n < NUM_REPEATS; ++n) {

        std::cout << "\n\nCall # " << (n + 1) << "/" << NUM_REPEATS << "\n\n";
        // This buffer contains "tasks".
        // For now those are simple integer numbers.
        PCSharedBuffer buffer;

        std::vector<std::unique_ptr<PCProducer>> producers;
        std::vector<std::unique_ptr<PCConsumer>> consumers;

        // Producer worker threads that generate random integer numbers.
        for (int i = 0; i < NUM_PRODUCERS; i++) {
            auto p(std::make_unique<PCProducer>(buffer));
            producers.push_back(std::move(p));
        }

        // Consumer worker threads. Read integer numbers and print them to console.
        for (int i = 0; i < NUM_CONSUMERS; i++) {
            auto c(std::make_unique<PCConsumer>(buffer));
            consumers.push_back(std::move(c));
        }

        // Main thread waits for user input
        std::cin.get();

        // Send producer and consumer threads stop event
        for (auto p = producers.begin(); p != producers.end(); ++p) {
            p->get()->stop();
        }
        for (auto c = consumers.begin(); c != consumers.end(); ++c) {
            c->get()->stop();
        }

        // Wait threads to finalize their work
        for (auto p = producers.begin(); p != producers.end(); ++p) {
            p->get()->wait();
        }
        for (auto c = consumers.begin(); c != consumers.end(); ++c) {
            c->get()->wait();
        }
    }

    return 0;
}
