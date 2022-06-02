#define BOOST_TEST_MODULE test_exceptions

#include <boost/test/unit_test.hpp>
#include "custom_allocator.h"
#include "custom_vector.h"
#include <exception>

BOOST_AUTO_TEST_SUITE(test_exceptions)

BOOST_AUTO_TEST_CASE(exception_test) {
        auto func = [](){
            CustomVector<int, CustomAllocator<int, 3>> sv;
            for(size_t idx = 0; idx < 10; ++idx) {
                sv.push_back(idx);
            }
        };

        BOOST_CHECK_THROW(func(), std::bad_alloc);
}

BOOST_AUTO_TEST_SUITE_END()