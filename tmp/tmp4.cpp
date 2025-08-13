#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "tmp.h"

int main() {
    set_interrupt(true);  
    // char c;
    // read(STDIN_FILENO, &c, 1);  // 阻塞 syscall
    // printf("Exiting main\n");
    char buf[10];
    while (1) {
        ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
        if (n < 0) {
            if (errno == EINTR) {
                write(STDOUT_FILENO, "read interrupted by signal\n", 28);
            } else {
                perror("read");
            }
        } else {
            write(STDOUT_FILENO, buf, n);
        }
    }
    return 0;
}
