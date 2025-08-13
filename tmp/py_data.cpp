#include <atomic>      // std::atomic
#include <cstdint>     // uint32_t, uint64_t
#include <cstring>     // memcpy
#include <string.h>
#include <vector>

struct PythonFrame {
    uint64_t code_address;   // 可映射 code object / co_filename
    uint32_t line_no;        // 当前执行行号
    uint32_t name_id;        // 可选: 函数名字符串 ID
};

#define MAX_STACK_DEPTH 128
struct PythonStackSnapshot {
    std::atomic<uint64_t> version;    // version tag，用于 ABA 检测
    uint32_t frame_count;
    PythonFrame frames[MAX_STACK_DEPTH];
};

std::vector<PythonFrame> python_stack;

// ring slot count = 2; double buffering
PythonStackSnapshot snapshot[2];       // 用户态写一个，NMI 读一个
std::atomic<uint32_t> write_index;     // 当前写入 slot（0 或 1）

// 写入 snapshot
uint32_t slot = write_index.load(std::memory_order_relaxed);
uint32_t next_slot = 1 - slot;

auto& snap = snapshot[next_slot];
snap.version.store(0, std::memory_order_relaxed);  // invalidate
snap.frame_count = python_stack.size();            // 写 frame
memcpy(snap.frames, python_stack.data(), snap.frame_count * sizeof(PythonFrame));
snap.version.store(monotonic_counter++, std::memory_order_release);  // commit

write_index.store(next_slot, std::memory_order_release);

uint32_t slot = write_index.load(std::memory_order_acquire);
auto& snap = snapshot[slot];

#define MAX_STACK 128
uint64_t v1 = snap.version.load(std::memory_order_acquire);
// 读 frame_count 和内容
uint32_t n = snap.frame_count;
PythonFrame tmp[MAX_STACK];
memcpy(tmp, snap.frames, n * sizeof(PythonFrame));
uint64_t v2 = snap.version.load(std::memory_order_acquire);

if (v1 == v2 && v1 != 0) {
    // 成功读取一致快照
    process_python_stack(tmp, n);
}

thread_local PythonStackRing* g_py_stack;

void update_python_stack_snapshot(PyFrameObject* top_frame) {
    auto& ring = *g_py_stack;
    int next = 1 - ring.write_index.load(std::memory_order_relaxed);
    auto& slot = ring.slots[next];

    slot.version.store(0, std::memory_order_relaxed);  // invalidate

    std::vector<PythonFrame> frames;
    for (auto* f = top_frame; f != nullptr; f = f->f_back) {
        PythonFrame frame;
        frame.code_addr = reinterpret_cast<uint64_t>(f->f_code); // 伪地址
        frame.line_no = PyFrame_GetLineNumber(f);
        frame.name_hash = hash(f->f_code->co_name);
        frames.push_back(frame);
    }

    std::reverse(frames.begin(), frames.end());
    slot.frame_count = frames.size();
    memcpy(slot.frames, frames.data(), frames.size() * sizeof(PythonFrame));
    slot.version.store(monotonic_counter++, std::memory_order_release);

    ring.write_index.store(next, std::memory_order_release);
}

PythonStackSnapshot TryReadPythonStack(int tid) {
    auto* ring = LookupPythonRing(tid); // 每线程注册
    int idx = ring->write_index.load(std::memory_order_acquire);
    auto& slot = ring->slots[idx];

    uint64_t v1 = slot.version.load(std::memory_order_acquire);
    PythonStackSnapshot snapshot;
    memcpy(&snapshot, &slot, sizeof(PythonStackSnapshot));
    uint64_t v2 = slot.version.load(std::memory_order_acquire);

    if (v1 == v2 && v1 != 0) {
        return snapshot;
    } else {
        return {}; // 空栈
    }
}