cmake_minimum_required(VERSION 2.8)

set(PRJ send_pack)
project(${PRJ})

file(GLOB SOURCES
  *.cc
  )

include_directories(
  $ENV{DPDK_RTE_TARGET}/include
  src/
  )

link_directories($ENV{DPDK_RTE_TARGET}/lib)

set(DPDK_DRIVERS
  "-lrte_pmd_ixgbe -lrte_pmd_vmxnet3_uio -lrte_pmd_e1000"
  )

set(DPDK_LIBS
  "-Wl,--whole-archive"
  "-ldpdk"
  "-Wl,--no-whole-archive"
  )

set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -std=c++11 -Wall"
  )
add_definitions(-mssse3)
add_definitions(-msse4)

option(DPI_DEBUG "Debug mode (allow asserts, debug messages, etc)" OFF)
add_definitions(-DNDEBUG)
if(DPI_DEBUG)
  remove_definitions (-DNDEBUG)
endif(DPI_DEBUG)

add_executable(${PRJ} ${SOURCES})
#add_subdirectory(test)
target_link_libraries(${PRJ} ${DPDK_LIBS})
target_link_libraries(${PRJ} pthread dl m)
