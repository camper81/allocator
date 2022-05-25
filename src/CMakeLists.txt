cmake_minimum_required(VERSION 3.2)

add_executable(allocator main.cpp)

target_include_directories(allocator
        PUBLIC ${INCLUDE_DIR}
        )

#if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(test test.cpp)

    set_target_properties(test PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            )

    set_target_properties(test PROPERTIES
            COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
            INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
            )

    target_include_directories(test
            PUBLIC ${INCLUDE_DIR}
            )

    target_link_libraries(test
            ${Boost_LIBRARIES}
            )
#endif()


if (MSVC)
    target_compile_options(allocator PRIVATE
            /W4
            )
else ()
    target_compile_options(allocator PRIVATE
            -Wall -Wextra -pedantic -Werror
            )
endif()

#if(WITH_BOOST_TEST)
    enable_testing()
    add_test(test_map test)
#endif()