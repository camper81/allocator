#include <iostream>
#include <vector>
#include <map>
#include "custom_allocator.h"

//#define NO_HEAP_USAGE
//#define DEBUG_

constexpr long long factorial(long long n)
{
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

void test1(){
    const int n = 10;

    std::map<int,int> m1;
    for(int i = 0; i < n; ++i) {
        m1[i] = factorial(i);
    }

    std::map<int, int,std::less<int>, CustomAllocator<std::pair<const int,int>, 5>> m2;
    for(int i = 0; i < n; ++i) {
        m2[i] = factorial(i);
    }

}

int main(int, char *[]) {

    test1();
    auto v = std::vector<int, CustomAllocator<int, 6>>{};

    {
        v.push_back(2);
        v.push_back(3);
        v.shrink_to_fit();
    }
    v.reserve(3);
    v.push_back(5);


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
