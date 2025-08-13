#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <frameobject.h>

namespace compat
{
struct RawFrame
{
    const char* function_name;
    const char* filename;
    int lineno;
    bool is_entry_frame;

    auto operator==( const compat::RawFrame& other ) const -> bool
    {
        return ( function_name == other.function_name && filename == other.filename && lineno == other.lineno &&
                 is_entry_frame == other.is_entry_frame );
    }

    struct Hash
    {
        auto operator()( compat::RawFrame const& frame ) const noexcept -> std::size_t
        {
            // Keep this hashing fast and simple as this has a non trivial
            // performance impact on the tracing functionality. We don't hash
            // the contents of the strings because the interpreter will give us
            // the same char* for the same code object. Of course, we can have
            // some scenarios where two functions with the same function name have
            // two different char* but in that case we will end registering the
            // name twice, which is a good compromise given the speed that we
            // gain keeping this simple.

            auto the_func = std::hash<const char*>{}( frame.function_name );
            auto the_filename = std::hash<const char*>{}( frame.filename );
            auto lineno = std::hash<int>{}( frame.lineno );
            return the_func ^ the_filename ^ lineno ^ frame.is_entry_frame;
        }
    };
};

inline bool isEntryFrame( PyFrameObject* frame )
{
#if PY_VERSION_HEX >= 0x030B0000
    return _PyFrame_IsEntryFrame( frame );
#else
    (void)frame;
    return true;
#endif
}

inline PyFrameObject* threadStateGetFrame( PyThreadState* tstate )
{
#if PY_VERSION_HEX < 0x030B0000
    // Prior to Python 3.11 this was exposed.
    return tstate->frame;
#else
    // Return a borrowed reference.
    PyFrameObject* ret = PyThreadState_GetFrame( tstate );
    if( ret )
    {
        assert( Py_REFCNT( ret ) >= 2 );
        Py_DECREF( ret );
    }
    return ret;
#endif
}

inline PyCodeObject* frameGetCode( PyFrameObject* frame )
{
#if PY_VERSION_HEX < 0x030B0000
    // Prior to Python 3.11 this was exposed.
    return frame->f_code;
#else
    // Return a borrowed reference.
    PyCodeObject* ret = PyFrame_GetCode( frame );
    assert( Py_REFCNT( ret ) >= 2 );
    Py_DECREF( ret );
    return ret;
#endif
}

inline PyFrameObject* frameGetBack( PyFrameObject* frame )
{
#if PY_VERSION_HEX < 0x030B0000
    // Prior to Python 3.11 this was exposed.
    return frame->f_back;
#else
    // Return a borrowed reference.
    PyFrameObject* ret = PyFrame_GetBack( frame );
    if( ret )
    {
        assert( Py_REFCNT( ret ) >= 2 );
        Py_DECREF( ret );
    }
    return ret;
#endif
}

inline PyInterpreterState* threadStateGetInterpreter( PyThreadState* tstate )
{
#if PY_VERSION_HEX < 0x03090000
    return tstate->interp;
#else
    return PyThreadState_GetInterpreter( tstate );
#endif
}

void setprofileAllThreads( Py_tracefunc func, PyObject* arg )
{
    assert( PyGILState_Check() );

    PyThreadState* this_tstate = PyThreadState_Get();
    PyInterpreterState* interp = threadStateGetInterpreter( this_tstate );
    for( PyThreadState* tstate = PyInterpreterState_ThreadHead( interp ); tstate != nullptr;
         tstate = PyThreadState_Next( tstate ) )
    {
#if PY_VERSION_HEX >= 0x03090000
        if( _PyEval_SetProfile( tstate, func, arg ) < 0 )
        {
            _PyErr_WriteUnraisableMsg( "in PyEval_SetProfileAllThreads", NULL );
        }
#else
        // For 3.7 and 3.8, backport _PyEval_SetProfile from 3.9
        // https://github.com/python/cpython/blob/v3.9.13/Python/ceval.c#L4738-L4767
        PyObject* profileobj = tstate->c_profileobj;

        tstate->c_profilefunc = NULL;
        tstate->c_profileobj = NULL;
        /* Must make sure that tracing is not ignored if 'profileobj' is freed */
        tstate->use_tracing = tstate->c_tracefunc != NULL;
        Py_XDECREF( profileobj );

        Py_XINCREF( arg );
        tstate->c_profileobj = arg;
        tstate->c_profilefunc = func;

        /* Flag that tracing or profiling is turned on */
        tstate->use_tracing = ( func != NULL ) || ( tstate->c_tracefunc != NULL );
#endif
    }
}
} // namespace compat
