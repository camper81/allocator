#define BOOST_TEST_MODULE test

#include <sstream>
#include <boost/test/unit_test.hpp>
#include "custom_allocator.h"
#include "custom_vector.h"
#include "factorial.h"
#include <exception>

template <typename T>
void print_map(T m, std::ostream& os){
    for(auto&[key, value] : m) {
        os << key << " " << value << std::endl;
    }
}

template <typename T>
void print_vec(T m, std::ostream& os){
    for(auto& value : m) {
        os << value << std::endl;
    }
}


template <typename T,typename U, size_t Size = 10>
void assign_map(T& m, U (*func)(U a)){
    for(size_t key = 0 ; key < Size; ++key ) {
        m[key] = func(key);
    }
}

template <typename T,typename U, size_t Size = 10>
void assign_vec(T& v, U (*func)(U a)){
    for(size_t key = 0 ; key < Size; ++key ) {
        v.push_back(func(key));
    }
}

template<typename T>
void test(T m, std::ostream& os = std::cout){
    assign(m, &factorial);
    print(m, os);
}

BOOST_AUTO_TEST_SUITE(test_map)

BOOST_AUTO_TEST_CASE(testMapAllocatorByFactorial) {
    std::stringstream sstr1;
    std::map<int, int> m1;
    assign_map(m1, &factorial);
    print_map(m1, sstr1);

    std::stringstream sstr2;
    std::map<int, int, std::less<int>,
            CustomAllocator<std::pair<const int, int>, 10>> m2;
    assign_map(m2, &factorial);
    print_map(m2, sstr2);

    BOOST_CHECK_EQUAL(sstr1.str(), sstr2.str());
}

BOOST_AUTO_TEST_CASE(testVectorAllocatorByFactorial) {
    std::stringstream sstr1;
    std::vector<int> vec1;
    assign_vec(vec1, &factorial);
    print_vec(vec1, sstr1);

    std::stringstream sstr2;
    std::vector<int, CustomAllocator<int, 10>> vec2;
    assign_vec(vec2, &factorial);
    print_vec(vec2, sstr2);

    BOOST_CHECK_EQUAL(sstr1.str(), sstr2.str());
}

BOOST_AUTO_TEST_CASE(testBlankContainer) {
    std::stringstream sstr1;
    std::map<int, int> m1;
    print_map(m1, sstr1);

    std::stringstream sstr2;
    std::map<int, int, std::less<int>,
            CustomAllocator<std::pair<const int, int>, 0>> m2;
    print_map(m2, sstr2);

    BOOST_CHECK_EQUAL(sstr1.str(), sstr2.str());
}

BOOST_AUTO_TEST_CASE(testBigMapElements) {
    std::stringstream sstr1;
    std::map<int, int> m1;
    assign_map(m1, &factorial);
    print_map(m1, sstr1);

    std::stringstream sstr2;
    std::map<int, int, std::less<int>,
            CustomAllocator<std::pair<const int, int>, 10000>> m2;
    assign_map(m2, &factorial);
    print_map(m2, sstr2);

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

BOOST_AUTO_TEST_SUITE_END()