#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

void handler(int sig) {
    // printf("SIGALRM received\n");
    const char *msg = "SIGALRM received\n";
    write(STDOUT_FILENO, msg, strlen(msg));
}

void set_interrupt(bool sa_restart=false) {
    // signal(SIGALRM, handler);
    // alarm(1);  // 一秒后发送 SIGALRM

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    if (sa_restart == true) {
        sa.sa_flags = SA_RESTART;   // 執行完handler之後，自动重启被中断的阻塞系统调用
    }
        if (sigaction(SIGALRM, &sa, nullptr) < 0) {
        printf("sigaction error");
    }

    struct itimerval tick, old_tick;
    memset(&tick, 0, sizeof(tick));
    tick.it_value.tv_sec = 1;
    /*
    The it_interval and it_value fields must be properly normalized, meaning that:

    tv_usec must be less than 1,000,000

    if either is zero, then timer becomes one-shot.
    */
    tick.it_value.tv_usec = 100000;  // 初始 100ms 后触发
    tick.it_interval.tv_sec = 1;
    tick.it_interval.tv_usec = 0;  // 标准要求 tv_usec < 1,000,000，否则行为未定义
    if (setitimer(ITIMER_REAL, &tick, &old_tick)) {
        printf("set timer failed");
    }
}
