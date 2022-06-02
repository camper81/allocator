cmake_minimum_required(VERSION 3.12)
project(allocator)

#set(BOOST_ROOT ENV{BOOST_INSTALL_DIR})
#set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_CXX_STANDARD 17)
#set(Boost_DEBUG ON)
#find_package(Boost REQUIRED COMPONENTS
#        unit_test_framework)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

add_subdirectory(src)

#target_link_libraries(allocator_ Boost::unit_test_framework)