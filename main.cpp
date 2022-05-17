#include <iostream>
#include <vector>
#include <memory>
#include <mutex>

//#define NO_HEAP_USAGE
#define DEBUG_

template<typename T>
struct Chunk {
    Chunk* next_available_chunk_ = nullptr;
    bool is_busy_ = false;
    T* data_ = nullptr;
};

template<typename T, size_t N>
class StackMemoryAllocator{
public:
    T* get(size_t request_size) {
        auto* chunk = &chunks_[0];
        std::lock_guard g(mtx_);
        if(chunk->data_ == nullptr)
            chunk->data_ = &reserved_memory_[0];
        size_t free_memory = 0;
        //arrange_free_mem();
        while(chunk) {
            std::cout << "Chunk: " << chunk - &chunks_[0] << std::endl;
            std::cout << "Size = ";
            if(chunk->next_available_chunk_ == nullptr) {
                std::cout << &reserved_memory_[N] - chunk->data_;
            } else {
                std::cout << chunk->next_available_chunk_ - chunk;
            }
            std::cout << " Busy = " << chunk->is_busy_;
            std::cout << std::endl;
            if(!chunk->is_busy_) {

                // if last chunk in stack
                if(chunk->next_available_chunk_ == nullptr) {
#ifdef DEBUG_
                    std::cout << "allocate in stack last element" << std::endl;
#endif
                    free_memory = &reserved_memory_[N] - chunk->data_;
                } else {
#ifdef DEBUG_
                    std::cout << "allocate in stack current chunk" << std::endl;
#endif
                    free_memory = chunk->next_available_chunk_ - chunk;
                }

                if(free_memory >= request_size) {
#ifdef DEBUG_
                    std::cout << "/allocate in stack>" << std::endl;
#endif

                    chunk->is_busy_ = true;
                    if(chunk->next_available_chunk_ == nullptr && free_memory == request_size)
                        return chunk->data_;

                    chunk->next_available_chunk_ = chunk + request_size;
                    chunk->next_available_chunk_->data_ =
                            &reserved_memory_[chunk->next_available_chunk_ - &chunks_[0]];

                    return chunk->data_;
                }
            }
            chunk = chunk->next_available_chunk_;
        }
        return nullptr;
    }
    bool release(T* pointer) {
        auto* chunk = &chunks_[0];
        std::lock_guard g(mtx_);
        while(chunk) {
            if(chunk->data_ == pointer) {
#ifdef DEBUG_
                std::cout << "Delete stack" << std::endl;
#endif
                chunk->is_busy_ = false;
                return true;
            }
            chunk = chunk->next_available_chunk_;
        }
        return false;
    }
private:
    void rearrange(){
        auto* chunk = &chunks_[0];
        auto* prev_chunk = &chunks_[0];
        while(chunk){
            if(!chunk->is_busy_ && !prev_chunk->is_busy_ && prev_chunk != chunk) {
                prev_chunk->next_available_chunk_ = chunk->next_available_chunk_;
            }
            if(!chunk->is_busy_)
                prev_chunk = chunk;
            chunk = chunk->next_available_chunk_;
        }
    }

    std::mutex mtx_;
    Chunk<T> chunks_[N] = {};
    T reserved_memory_[N] = {};
};

template<typename T, size_t N>
struct CustomAllocator {
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = CustomAllocator<U, N>;
    };

    CustomAllocator() = default;
    ~CustomAllocator() = default;

    CustomAllocator(const CustomAllocator&){

    };
    CustomAllocator& operator=(const CustomAllocator&) = delete;

    T *allocate(std::size_t n) {
#ifdef DEBUG_
        std::cout << "allocate: " << n << std::endl;
#endif
        T* allocated_block = mem_allocator_.get(n);
        if(allocated_block == nullptr)
        {
#ifdef NO_HEAP_USAGE
            throw std::bad_alloc();
#else
            #ifdef DEBUG_
            std::cout << "allocate in dynamic" << std::endl;
            #endif
            return new T[n];
#endif
        }
        return allocated_block;
    }

    void deallocate(T *p, std::size_t n) {
       if(!mem_allocator_.release(p)) {
#ifdef DEBUG_
           std::cout << "Delete Pointer" << std::endl;
#endif
           delete p;
       }
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {
        std::cout << "destroy" << std::endl;

        p->~T();
        mem_allocator_.release(p);
    }
    StackMemoryAllocator<T, N> mem_allocator_;
};

int main(int, char *[]) {
    auto v = std::vector<int, CustomAllocator<int, 6>>{};
//    v.reserve(100);
//
//    for (int i = 0; i < 100; ++i) {
//        v.push_back(i);
//    }
    {
        v.push_back(2);
        v.push_back(3);
        v.shrink_to_fit();
    }
    v.reserve(3);
    v.push_back(5);
//
//    std::cout << "\n\n\n\nAfter creation\n\n\n\n" << std::endl;
//    auto v2 = v;

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
