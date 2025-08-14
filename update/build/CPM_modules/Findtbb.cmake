include("/home/bwang/gsim_wks/app/3rdparty/tracy/cmake/CPM.cmake")
CPMAddPackage("NAME;tbb;GITHUB_REPOSITORY;oneapi-src/oneTBB;GIT_TAG;v2020.3.3;OPTIONS;TBB_TEST OFF")
set(tbb_FOUND TRUE)