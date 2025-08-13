#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "tmp.h"

int main() {
    set_interrupt(false);  
    while(1) {
        getpid();
    }
    return 0;
}
