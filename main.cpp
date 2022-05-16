#include <iostream>
#include <vector>
#include <map>

template<typename T, size_t N>
struct stack_allocator {
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = stack_allocator<U, N>;
    };

    stack_allocator() = default;
    ~stack_allocator() = default;

    stack_allocator(const stack_allocator&) = delete;
    stack_allocator& operator=(const stack_allocator&) = delete;

    template<typename U>
    stack_allocator(const stack_allocator<U, N>&) {

    }

    T *allocate(std::size_t n) {
        if(available_stack_blocks_ < n)
        {
#ifdef NO_HEAP_USAGE
            throw std::bad_alloc();
#else
            return static_cast<T*>(malloc(n*sizeof(T)));
#endif
        }

        first_available_+= allocated_;
        allocated_ += n;
        available_stack_blocks_ -= n;

        return reinterpret_cast<T*>(&reserved_memory_[first_available_]);
    }

    void deallocate(T *p, std::size_t n) {
        if(p > reserved_memory_ && p < (reserved_memory_[allocated_])){
            allocated_ -= n;
            first_available_ -= n;

        } else {
          delete p;
        }
        std::cout << "Clean Memory" << std::endl;
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {
        p->~T();
    }

    size_t  available_stack_blocks_ = N;
    size_t  first_available_ = 0;
    size_t  allocated_ = 0;
    size_t  deallocated_ = 0;
    T reserved_memory_[N];
    T* heap_memory_ = nullptr;
};

int main(int, char *[]) {
//    auto v = std::vector<int, stack_allocator<int, 5>>{};
//    v.reserve(5);
//    for (int i = 0; i < 5; ++i) {
//        std::cout << "vector size = " << v.size() << std::endl;
//        v.emplace_back(i);
//        std::cout << std::endl;
//    }
//
//    std::cout << "\n\n\n\nAfter creation\n\n\n\n" << std::endl;
//    auto v2 = v;

    auto m = std::map<
            int,
            float,
            std::less<int>,
            stack_allocator<
                    std::pair<
                            const int, float
                    >, 5
            >
    >{};

    m[1] = {1};
    m[2] = {2};
    m[3] = {3};
    m[10] = {4};
//    m[20] = 1;
//
//    std::allocator<std::pair<int,std::string>> a;

//    for (int i = 0; i < 2; ++i) {
//        m[i] = static_cast<float>(i);
//    }

    return 0;
}
