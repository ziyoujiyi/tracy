# Install script for directory: /home/bwang/gsim_wks/app/3rdparty/tracy

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/mnt/l/f/share/cnliao/software/conda/envs/prod/bin/x86_64-conda-linux-gnu-objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTracyClient.so.0.11.1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTracyClient.so.0.11.1")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTracyClient.so.0.11.1"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/bwang/gsim_wks/app/3rdparty/tracy/build/libTracyClient.so.0.11.1")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTracyClient.so.0.11.1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTracyClient.so.0.11.1")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/mnt/l/f/share/cnliao/software/conda/envs/prod/bin/x86_64-conda-linux-gnu-strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTracyClient.so.0.11.1")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTracyClient.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTracyClient.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTracyClient.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/bwang/gsim_wks/app/3rdparty/tracy/build/libTracyClient.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTracyClient.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTracyClient.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/mnt/l/f/share/cnliao/software/conda/envs/prod/bin/x86_64-conda-linux-gnu-strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTracyClient.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/tracy" TYPE FILE FILES
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/tracy/TracyC.h"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/tracy/Tracy.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/tracy/TracyD3D11.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/tracy/TracyD3D12.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/tracy/TracyLua.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/tracy/TracyOpenCL.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/tracy/TracyOpenGL.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/tracy/TracyVulkan.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/client" TYPE FILE FILES
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/tracy_concurrentqueue.h"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/tracy_rpmalloc.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/tracy_SPSCQueue.h"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyKCore.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyArmCpuTable.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyCallstack.h"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyCallstack.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyCpuid.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyDebug.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyDxt1.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyFastVector.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyLock.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyProfiler.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyRingBuffer.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyScoped.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyStringHelpers.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracySysPower.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracySysTime.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracySysTrace.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyPyTrace.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/client/TracyThread.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/common" TYPE FILE FILES
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/tracy_lz4.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/tracy_lz4hc.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracyAlign.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracyAlloc.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracyApi.h"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracyColor.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracyForceInline.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracyMutex.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracyProtocol.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracyQueue.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracySocket.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracyStackFrames.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracySystem.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracyUwp.hpp"
    "/home/bwang/gsim_wks/app/3rdparty/tracy/public/common/TracyYield.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/Tracy/TracyTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/Tracy/TracyTargets.cmake"
         "/home/bwang/gsim_wks/app/3rdparty/tracy/build/CMakeFiles/Export/share/Tracy/TracyTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/Tracy/TracyTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/Tracy/TracyTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Tracy" TYPE FILE FILES "/home/bwang/gsim_wks/app/3rdparty/tracy/build/CMakeFiles/Export/share/Tracy/TracyTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Tracy" TYPE FILE FILES "/home/bwang/gsim_wks/app/3rdparty/tracy/build/CMakeFiles/Export/share/Tracy/TracyTargets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Tracy" TYPE FILE FILES "/home/bwang/gsim_wks/app/3rdparty/tracy/build/TracyConfig.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/bwang/gsim_wks/app/3rdparty/tracy/build/python/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/bwang/gsim_wks/app/3rdparty/tracy/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
