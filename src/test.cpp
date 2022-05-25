#define BOOST_TEST_MODULE test

#include <sstream>
#include <boost/test/unit_test.hpp>
#include "custom_allocator.h"
#include "factorial.h"

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