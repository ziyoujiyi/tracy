#pragma once

#include "compat.hpp"

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <thread>
#include <unordered_set>

#include <unwind.h>

#if defined( __GLIBC__ )
#    define FAST_TLS __attribute__( ( tls_model( "initial-exec" ) ) )
#else
#    define FAST_TLS __attribute__( ( tls_model( "local-dynamic" ) ) )
#endif

// #define TRACY_PY_TRACING

#ifdef TRACY_PY_TRACING
namespace tracy
{

class PythonStackGuard
{
  public:
    static inline void lock() { isActive = true; }
    static inline void unlock() { isActive = false; }
    static inline bool isLocked() { return isActive; }
    static std::atomic<bool> isActive;
};

struct RecursionGuard
{
    RecursionGuard()
        : wasLocked( isActive )
    {
        isActive = true;
    }

    ~RecursionGuard() { isActive = wasLocked; }

    const bool wasLocked;
    FAST_TLS static thread_local bool isActive;
};

// Tracker interface

// This class must have a trivial destructor (and therefore all its instance
// attributes must be POD). This is required because the libc implementation
// can perform allocations even after the thread local variables for a thread
// have been destroyed. If a TLS variable that is not trivially destructable is
// accessed after that point by our allocation hooks, it will be resurrected,
// and then it will be freed when the thread dies, but its destructor either
// won't be called at all, or will be called on freed memory when some
// arbitrary future thread is destroyed (if the pthread struct is reused for
// another thread).
class PythonStackTracker
{
  private:
    PythonStackTracker() {}
    struct Frame
    {
        PyFrameObject* frame;
        compat::RawFrame raw_frame_record;
    };

  public:
    static void installProfileHooks();
    static void removeProfileHooks();

    void clear();

    static PythonStackTracker& get();
    int pushPythonFrame( PyFrameObject* frame );
    void popPythonFrame();

  public:
    // Fetch the thread-local stack tracker without checking if its stack needs to be reloaded.
    static PythonStackTracker& getUnsafe();

  private:
    static std::vector<Frame> pythonFrameToStack( PyFrameObject* current_frame );
    static void recordAllStacks();
    void reloadStackIfTrackerChanged();

    void pushFrame( const Frame& frame );

    static std::mutex s_mutex;
    static std::unordered_map<PyThreadState*, std::vector<Frame>> s_initial_stack_by_thread;
    static std::atomic<unsigned int> s_tracker_generation;

    uint32_t d_tracker_generation{};

  public:
    std::vector<Frame>* d_stack{};
    uint32_t d_num_pending_pops = 0;
};

// Trace function interface

/**
 * Trace function to be installed in all Python treads to track function calls
 *
 * This trace function's sole purpose is to give a thread-safe, GIL-synchronized view of the Python
 * stack. To retrieve the Python stack using the C-API forces the caller to have the GIL held. Requiring
 * the GIL in the allocator function has too much impact on performance and can deadlock extension
 * modules that have native locks that are not synchronized themselves with the GIL. For this reason we
 * need a way to record and store the Python call frame information in a way that we can read without the
 * need to use the C-API. This trace function writes to disk the PUSH and POP operations so the Python
 *stack at any point can be reconstructed later.
 *
 **/
int PyTraceFunction( PyObject* obj, PyFrameObject* frame, int what, PyObject* arg );

/**
 * Trampoline that serves as the initial profiling function for each thread.
 *
 * This performs some one-time setup, then installs PyTraceFunction.
 */
int PyTraceTrampoline( PyObject* obj, PyFrameObject* frame, int what, PyObject* arg );

/**
 * Drop any references to frames on this thread's stack.
 *
 * This should be called when either the thread is dying or our profile
 * function is being uninstalled from it.
 */
void forget_python_stack();

void PyTraceWorker( void* ptr );
} // namespace tracy

#endif