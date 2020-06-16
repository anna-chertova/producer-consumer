/* 
 * producer-consumer.cpp: this file contains main func
 * (c) 2020 Anna Chertova
 */

#include <iostream>
#include "pc_thread.h"

int main()
{
    PCThread my_thread;
    my_thread.init();
    my_thread.wait();
    return 0;
}
