# include "tmp.h"
# include <linux/perf_event.h>

struct perf_event_attr attr = {
    .type = PERF_TYPE_HARDWARE,
    .config = PERF_COUNT_HW_INSTRUCTIONS,
    .sample_period = 1000000,
    .sample_type = PERF_SAMPLE_IP,
    .disabled = 1,
    .exclude_kernel = 1,
    .exclude_hv = 1,
    .wakeup_events = 1,
    .sigtrap = 1, // 需要内核支持
};
int fd = perf_event_open(&attr, pid, cpu, group_fd, flags);

// 启用异步 signal
fcntl(fd, F_SETFL, O_RDWR | O_NONBLOCK | O_ASYNC);
fcntl(fd, F_SETSIG, SIGIO);
fcntl(fd, F_SETOWN, getpid());
