// g++ test1.cpp -I./include -L./lib -lTracyClient -o test1.out

#include <iostream>
#include <thread>
#include <unistd.h>

#include "../include/Tracy.hpp"
#include "../include/TracyC.h"

void do_work_1()
{
    std::cout << "do work_1" << std::endl;
    for( volatile int j = 0; j < 10000000; ++j )
    {
    }
    sleep( 1 );
}

void do_work_2()
{
    std::cout << "do work_2" << std::endl;
    do_work_1();
}

void do_work_3()
{
    std::cout << "do work_3" << std::endl;
    do_work_1();
}

void f()
{
    std::thread t1(
        []
        {
            while( true )
            {
                do_work_2();
            }
        } );

    std::thread t2(
        []
        {
            while( true )
            {
                do_work_3();
            }
        } );

    t1.join();
    t2.join();
}

int main()
{
    tracy::GetProfiler();
    f();
}