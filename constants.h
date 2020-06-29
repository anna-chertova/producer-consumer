#pragma once
/*
 * constants.h - this header file contains constants used throughout the program
 * (c) 2020 Anna Chertova
 */

 // These constants to be used in a final solution
// number of producer threads
#define NUM_PRODUCERS 20
// number of consumer threads
#define NUM_CONSUMERS 10
// number of program calls
#define NUM_REPEATS 1
// ms passed to Sleep call in the end of each cycle of worker thread
// (in order to yield time slice to others)
#define SLEEP_TIME_MS 100
// time out used by worker threads to wait until read/write event
// (to avoid blocking forever)
// used to be able to stop/interrupt threads gracefully
#define WAIT_TIME_MS 20
// Delay before producing new task in ms
#define PRODUCER_DELAY_MS 300
// Delay before consuming new task in ms
#define CONSUMER_DELAY_MS 200
// maximum size of shared buffer
// (contains "tasks" produced by producers
// in order to be consumed by consumers)
#define MAX_BUFFER_SIZE 1000
