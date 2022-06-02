#define BOOST_TEST_MODULE test

#include <sstream>
#include <boost/test/unit_test.hpp>
#include "custom_allocator.h"
#include "custom_vector.h"
#include "factorial.h"
#include <exception>

template <typename T>
void print(T m, std::ostream& os){
    for(auto&[key, value] : m) {
        os << key << " " << value << std::endl;
    }
}

template <typename T,typename U, size_t Size = 10>
void assign(T& m, U (*func)(U a)){
    for(size_t key = 0 ; key < Size; ++key ) {
        m[key] = func(key);
    }
}

template<typename T>
void test(T m, std::ostream& os = std::cout){
    assign(m, &factorial);
    print(m, os);
}

BOOST_AUTO_TEST_SUITE(test_map)

BOOST_AUTO_TEST_CASE(testCompare) {
    std::stringstream sstr1;
    std::map<int, int> m1;
    assign(m1, &factorial);
    print(m1, sstr1);

    std::stringstream sstr2;
    std::map<int, int, std::less<int>,
        CustomAllocator<std::pair<const int, int>, 10>> m2;
    assign(m2, &factorial);
    print(m2, sstr2);

    BOOST_CHECK_EQUAL(sstr1.str(), sstr2.str());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_my_container)

    BOOST_AUTO_TEST_CASE(equality_test) {
        CustomVector<int> sv1;
        for(size_t idx = 0; idx < 10; ++idx) {
            sv1.push_back(idx);
        }

        CustomVector<int, CustomAllocator<int, 10>> sv2;
        for(size_t idx = 0; idx < 10; ++idx) {
            sv2.push_back(idx);
        }

        BOOST_CHECK_EQUAL_COLLECTIONS(sv1.begin(), sv1.end(), sv2.begin(), sv2.end());
    }


    BOOST_AUTO_TEST_CASE(exception_test) {
//        auto func = [](){
//            CustomVector<int, CustomAllocator<int, 3>> sv;
//            for(size_t idx = 0; idx < 10; ++idx) {
//                sv.push_back(idx);
//            }
//        };
//
//        BOOST_CHECK_THROW(func(), std::bad_alloc);
    }


BOOST_AUTO_TEST_SUITE_END()