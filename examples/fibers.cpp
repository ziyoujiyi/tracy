/*
g++ -g fibers.cpp ../public/TracyClient.cpp -I ../public/tracy -I
/mnt/l/f/share/cnliao/software/conda/envs/prod/include/python3.7m
/mnt/l/f/share/cnliao/software/conda/envs/prod/lib/libpython3.7m.so -DTRACY_ENABLE -DTRACY_FIBERS
-DTRACY_VERBOSE -lpthread -ldl

export LD_LIBRARY_PATH=/mnt/l/f/share/cnliao/software/conda/envs/prod/lib:$LD_LIBRARY_PATH
*/
// ./a.out

#include "fibers.hpp"

int main() { f(); }
