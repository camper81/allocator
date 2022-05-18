cmake_minimum_required(VERSION 3.20)
project(allocator)

set(BOOST_ROOT ENV{BOOST_INSTALL_DIR})
set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_CXX_STANDARD 17)
set(Boost_DEBUG ON)
find_package(Boost REQUIRED COMPONENTS
        unit_test_framework)

add_executable(allocator main.cpp)
target_link_libraries(allocator Boost::unit_test_framework)