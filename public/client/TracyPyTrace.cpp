#include <cassert>
#include <limits.h>

#ifdef __linux__
#    include <link.h>
#elif defined( __APPLE__ )
#    include "macho_utils.h"
#    include <mach/mach.h>
#    include <mach/task.h>
#endif

#include "TracyDebug.hpp"
#include "TracyPyTrace.hpp"
#include "compat.hpp"
#include <algorithm>
#include <iostream>
#include <mutex>
#include <type_traits>
#include <unistd.h>
#include <utility>

using namespace std::chrono_literals;

#ifdef TRACY_PY_TRACING
namespace tracy
{
std::atomic<bool> PythonStackGuard::isActive = false;
FAST_TLS thread_local bool RecursionGuard::isActive = false;

using thread_id_t = unsigned long;
static inline thread_id_t thread_id() { return reinterpret_cast<thread_id_t>( pthread_self() ); };

std::mutex PythonStackTracker::s_mutex;
std::unordered_map<PyThreadState*, std::vector<PythonStackTracker::Frame>>
    PythonStackTracker::s_initial_stack_by_thread;
std::atomic<unsigned int> PythonStackTracker::s_tracker_generation;

PythonStackTracker& PythonStackTracker::get()
{
    PythonStackTracker& ret = getUnsafe();
    ret.reloadStackIfTrackerChanged();
    return ret;
}

PythonStackTracker& PythonStackTracker::getUnsafe()
{
    // See giant comment above.
    static_assert( std::is_trivially_destructible<PythonStackTracker>::value );
    FAST_TLS thread_local PythonStackTracker t_python_stack_tracker; // static type
    return t_python_stack_tracker;
}

void PythonStackTracker::reloadStackIfTrackerChanged()
{
    // Note: this function does not require the GIL.
    if( d_tracker_generation == s_tracker_generation )
    {
        return;
    }
    // If we reach this point, a new Tracker was installed by another thread,
    // which also captured our Python stack. Trust it, ignoring any stack we
    // already hold (since the stack we hold could be incorrect if tracking
    // stopped and later restarted underneath our still-running thread).
    if( d_stack )
    {
        d_stack->clear();
    }
    d_num_pending_pops = 0;

    std::vector<Frame> correct_stack;

    {
        std::unique_lock<std::mutex> lock( s_mutex );
        d_tracker_generation = s_tracker_generation;

        auto it = s_initial_stack_by_thread.find( PyGILState_GetThisThreadState() );
        if( it != s_initial_stack_by_thread.end() )
        {
            it->second.swap( correct_stack );
            s_initial_stack_by_thread.erase( it );
        }
    }
    // Iterate in reverse so that we push the most recent call last
    for( auto frame_it = correct_stack.rbegin(); frame_it != correct_stack.rend(); ++frame_it )
    {
        pushFrame( *frame_it );
    }
}

int PythonStackTracker::pushPythonFrame( PyFrameObject* frame )
{
    PyCodeObject* code = compat::frameGetCode( frame );
    const char* function = PyUnicode_AsUTF8( code->co_name );
    if( function == nullptr )
    {
        return -1;
    }

    const char* filename = PyUnicode_AsUTF8( code->co_filename );
    if( filename == nullptr )
    {
        return -1;
    }

    bool is_entry_frame = compat::isEntryFrame( frame );
    pushFrame( { frame, { function, filename, 0, is_entry_frame } } );
    return 0;
}

void PythonStackTracker::pushFrame( const Frame& frame )
{
    PythonStackGuard::lock();
    // Note: this function does not require the GIL.
    if( !d_stack )
    {
        d_stack = new std::vector<Frame>;
        d_stack->reserve( 1024 );
    }
    d_stack->push_back( frame );
    PythonStackGuard::unlock();
}

void PythonStackTracker::popPythonFrame()
{
    PythonStackGuard::lock();

    if( !d_stack || d_stack->empty() )
    {
        PythonStackGuard::unlock();
        return;
    }

    d_num_pending_pops += 1;

    d_stack->pop_back();
    PythonStackGuard::unlock();
}

std::vector<PythonStackTracker::Frame> PythonStackTracker::pythonFrameToStack( PyFrameObject* current_frame )
{
    std::vector<Frame> stack;

    while( current_frame )
    {
        PyCodeObject* code = compat::frameGetCode( current_frame );

        const char* function = PyUnicode_AsUTF8( code->co_name );
        if( function == nullptr )
        {
            return {};
        }

        const char* filename = PyUnicode_AsUTF8( code->co_filename );
        if( filename == nullptr )
        {
            return {};
        }

        stack.push_back( { current_frame, { function, filename, 0 } } );
        current_frame = compat::frameGetBack( current_frame );
    }

    return stack;
}

void PythonStackTracker::recordAllStacks()
{
    assert( PyGILState_Check() );

    // Record the current Python stack of every thread
    std::unordered_map<PyThreadState*, std::vector<Frame>> stack_by_thread;
    for( PyThreadState* tstate =
             PyInterpreterState_ThreadHead( compat::threadStateGetInterpreter( PyThreadState_Get() ) );
         tstate != nullptr; tstate = PyThreadState_Next( tstate ) )
    {
        PyFrameObject* frame = compat::threadStateGetFrame( tstate );
        if( !frame )
        {
            continue;
        }

        stack_by_thread[tstate] = pythonFrameToStack( frame );
        if( PyErr_Occurred() )
        {
            throw std::runtime_error( "Failed to capture a thread's Python stack" );
        }
    }

    // Throw away all but the most recent frame for this thread.
    // We ignore every stack frame above `Tracker.__enter__`.
    PyThreadState* tstate = PyThreadState_Get();
    assert( stack_by_thread[tstate].size() >= 1 );
    stack_by_thread[tstate].resize( 1 );

    std::unique_lock<std::mutex> lock( s_mutex );
    s_initial_stack_by_thread.swap( stack_by_thread );

    // Register that tracking has begun (again?), telling threads to sync their
    // TLS from these captured stacks. Update this atomically with the map, or
    // a thread that's 2 generations behind could grab the new stacks with the
    // previous generation number and immediately think they're out of date.
    s_tracker_generation++;
}

void PythonStackTracker::installProfileHooks()
{
    assert( PyGILState_Check() );

    // Uninstall any existing profile function in all threads. Do this before
    // installing ours, since we could lose the GIL if the existing profile arg
    // has a __del__ that gets called. We must hold the GIL for the entire time
    // we capture threads' stacks and install our trace function into them, so
    // their stacks can't change after we've captured them and before we've
    // installed our profile function that utilizes the captured stacks, and so
    // they can't start profiling before we capture their stack and miss it.
    compat::setprofileAllThreads( nullptr, nullptr );

    // Find and record the Python stack for all existing threads.
    recordAllStacks();

    // Install our profile trampoline in all existing threads.
    compat::setprofileAllThreads( PyTraceTrampoline, nullptr );
}

void PythonStackTracker::clear()
{
    if( !d_stack )
    {
        return;
    }

    while( !d_stack->empty() )
    {
        d_num_pending_pops += 1;
        d_stack->pop_back();
    }
    delete d_stack;
    d_stack = nullptr;
}

void forget_python_stack()
{
    RecursionGuard guard;
    PythonStackTracker::get().clear();
}

void PythonStackTracker::removeProfileHooks()
{
    forget_python_stack();

    assert( PyGILState_Check() );
    compat::setprofileAllThreads( nullptr, nullptr );
    std::unique_lock<std::mutex> lock( s_mutex );
    s_initial_stack_by_thread.clear();
}

// Called when profiling is initially enabled in each thread.
int PyTraceTrampoline( PyObject* obj, PyFrameObject* frame, int what, [[maybe_unused]] PyObject* arg )
{
    assert( PyGILState_Check() );
    RecursionGuard guard;

    PyEval_SetProfile( PyTraceFunction, nullptr );

    return PyTraceFunction( obj, frame, what, nullptr );
}

int PyTraceFunction( [[maybe_unused]] PyObject* obj, PyFrameObject* frame, int what, [[maybe_unused]] PyObject* arg )
{
    RecursionGuard guard;

    if( frame != PyEval_GetFrame() )
    {
        return 0;
    }

    switch( what )
    {
    case PyTrace_CALL:
    {
        PythonStackTracker::get().pushPythonFrame( frame );
        break;
    }
    case PyTrace_RETURN:
    {
        PythonStackTracker::get().popPythonFrame();
        break;
    }
    default:
        break;
    }
    return 0;
}

void PyTraceWorker( void* ptr )
{
    while( true )
    {
        std::this_thread::sleep_for( std::chrono::seconds( 10 ) );
        auto& tracker = PythonStackTracker::get();
        size_t stack_size = tracker.d_stack ? tracker.d_stack->size() : 0;
        MyDebug( "d_stack: %d, d_num_pending_pops: %d", stack_size, tracker.d_num_pending_pops );
    }
}
} // namespace tracy

#endif