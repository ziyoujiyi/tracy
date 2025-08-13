#include <iostream>
#include <thread>
#include <unistd.h>

#include "/home/bwang/gsim_wks/tracy/public/tracy/Tracy.hpp"
#include "/home/bwang/gsim_wks/tracy/public/tracy/TracyC.h"

// const char* fiber = "job1";
// TracyCZoneCtx zone;

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
    std::cout << "do work_2" << std::endl;
    do_work_1();
}

void f()
{
    tracy::GetProfiler();

    std::thread t1(
        []
        {
            // TracyFiberEnter( fiber );
            // TracyCZone( ctx, 1 );
            // zone = ctx;
            while( true )
            {
                do_work_2();
            }
            // TracyFiberLeave;
        } );

    std::thread t2(
        []
        {
            // TracyFiberEnter( fiber );
            while( true )
            {
                do_work_3();
            }
            // TracyCZoneEnd( zone );
            // TracyFiberLeave;
        } );

    t1.join();
    t2.join();
}
