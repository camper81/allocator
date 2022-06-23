#define BOOST_TEST_MODULE test_exceptions

#include <boost/test/unit_test.hpp>
#include "custom_allocator.h"
#include "custom_vector.h"
#include <exception>

BOOST_AUTO_TEST_SUITE(test_exceptions)

BOOST_AUTO_TEST_CASE(catchExceptionMyVector) {
        auto func = [](){
            CustomVector<int, CustomAllocator<int, 3>> sv;
            for(size_t idx = 0; idx < 10; ++idx) {
                sv.push_back(idx);
            }
        };

        BOOST_CHECK_THROW(func(), std::bad_alloc);
}

BOOST_AUTO_TEST_CASE(catchExceptionVector) {
    auto func = [](){

        std::vector<int, CustomAllocator<int, 3>> sv;
        for(size_t idx = 0; idx < 10; ++idx) {
            sv.push_back(idx);
        }
    };

    BOOST_CHECK_THROW(func(), std::bad_alloc);
}

BOOST_AUTO_TEST_CASE(catchExceptionMap) {
    auto func = [](){

        std::map<int, int, std::less<int>,
                CustomAllocator<std::pair<const int, int>, 3>> mp;
        for(size_t idx = 0; idx < 10; ++idx) {
            mp[idx] = idx;
        }
    };

    BOOST_CHECK_THROW(func(), std::bad_alloc);
}

BOOST_AUTO_TEST_SUITE_END()