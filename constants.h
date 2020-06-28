#pragma once
/*
 * constants.h - this header file contains constants used throughout the program
 * (c) 2020 Anna Chertova
 */

 // These constants to be used in a final solution
// number of producer threads
#define NUM_PRODUCERS 2
// number of consumer threads
#define NUM_CONSUMERS 2
// number of program calls
#define NUM_REPEATS 5
// ms passed to Sleep call in the end of each cycle of worker thread
// (in order to yield time slice to others)
#define SLEEP_TIME_MS 100
// time out used by worker threads to wait until read/write event
// (to avoid blocking forever)
// used to be able to stop/interrupt threads gracefully
#define WAIT_TIME_MS 20
// maximum size of shared buffer
// (contains "tasks" produced by producers
// in order to be consumed by consumers)
#define MAX_BUFFER_SIZE 1000
