#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include "custom_allocator.h"
#include "cvector.h"


constexpr long long factorial(const long long n)
{
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

template <typename T>
void print(T m){
    for(auto&[key, value] : m) {
        std::cout << key << " " << value << std::endl;
    }
}

template <typename T,typename U, size_t Size = 10>
void assign(T& m, U (*func)(U a)){
    for(U key = 0 ; key < Size; ++key ) {
        m[key] = func(key);
    }
}

template<typename T>
void test(T m){
        assign(m, &factorial);
        print(m);
}

void test0() {
    std::map<int, int> m1;
    test(m1);
}

void test1() {
    std::map<int, int, std::less<int>,
            CustomAllocator<std::pair<const int, int>, 10>> m2;
    test(m2);
}

void test2() {
    SimpleVector<int, CustomAllocator<int, 10>> sv;
    for(size_t idx = 0; idx < 10; ++idx) {
        sv.push_back(idx);
    }
    for(auto& val : sv){
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int main(int, char *[]) {
    test0();
    test1();
    test2();
//    auto v = std::vector<int, CustomAllocator<int, 6>>{};
//
//    {
//        v.push_back(2);
//        v.push_back(3);
//        v.shrink_to_fit();
//    }
//    v.reserve(3);
//    v.push_back(5);

//    SimpleVector<int, CustomAllocator<int, 3>> sv;
//    sv.push_back(1);
//    sv.push_back(2);
//    sv.push_back(3);

//    auto m = std::map<
//            int,
//            float,
//            std::less<int>,
//            CustomAllocator<
//                    std::pair<
//                            const int, float
//                    >, 5
//            >
//    >{};
//    std::pair<const int, float> s;
//    m[1] = {1};
//    m[2] = {2};
//    m[3] = {3};
//    m[10] = {4};
//    m[20] = {1};
//    m[21] = {1};
//
//    std::allocator<std::pair<int,std::string>> a;

//    for (int i = 0; i < 2; ++i) {
//        m[i] = static_cast<float>(i);
//    }

    return 0;
}
