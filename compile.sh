#!/usr/bin/env bash
set -e

TRACY_ROOT=/home/bwang/gsim_wks/tracy

cd "$TRACY_ROOT/build"
make clean
cmake -DTRACY_STATIC=OFF -DTRACY_CLIENT_PYTHON=ON -DTRACY_NO_STATISTICS=OFF -DTRACY_VERBOSE=ON -DTRACY_FIBERS=ON -DTRACY_CALLSTACK=OFF -DTRACY_SYMBOL_OFFLINE_RESOLVE=ON -DTRACY_LIBBACKTRACE_ELF_DYNLOAD_SUPPORT=OFF -DCMAKE_BUILD_TYPE=Debug ../
make -j 10

cd "$TRACY_ROOT/python"
/home/bwang/envs/envs/py38/bin/python setup.py bdist_wheel

/home/bwang/envs/envs/py38/bin/pip uninstall -y tracy_client || true
/home/bwang/envs/envs/py38/bin/pip install "$TRACY_ROOT/python/dist/"*.whl