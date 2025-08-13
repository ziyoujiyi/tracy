#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include "tmp.h"

int main() {
    set_interrupt();

    while (1) {
        // sched_yield();  // 主动让出 CPU 给其它线程
        
    }
}
