#pragma once

#include <mutex>

//#define DEBUG_
//#define NO_HEAP_USAGE

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
        std::lock_guard g(mtx_);
        auto* ptr_chunk = &chunk_storage_[0];

        if(ptr_chunk->data_ == nullptr)
            ptr_chunk->data_ = &pre_allocated_[0];

        size_t free_memory = 0;

        //rearrange_free_spaces();

        while(ptr_chunk) {
            if(!ptr_chunk->is_busy_) {

                // if last ptr_chunk in stack
                if(ptr_chunk->next_available_chunk_ == nullptr) {
#ifdef DEBUG_
                    std::cout << "allocate in stack last element" << std::endl;
#endif
                    free_memory = &pre_allocated_[N] - ptr_chunk->data_;
                } else {
#ifdef DEBUG_
                    std::cout << "allocate in stack current ptr_chunk" << std::endl;
#endif
                    free_memory = ptr_chunk->next_available_chunk_ - ptr_chunk;
                }

                if(free_memory >= request_size) {
#ifdef DEBUG_
                    std::cout << "allocate in stack>" << std::endl;
#endif

                    ptr_chunk->is_busy_ = true;
                    if(ptr_chunk->next_available_chunk_ == nullptr && free_memory == request_size)
                        return ptr_chunk->data_;

                    ptr_chunk->next_available_chunk_ = ptr_chunk + request_size;
                    ptr_chunk->next_available_chunk_->data_ =
                            &pre_allocated_[ptr_chunk->next_available_chunk_ - &chunk_storage_[0]];

                    return ptr_chunk->data_;
                }
            }
            ptr_chunk = ptr_chunk->next_available_chunk_;
        }
        return nullptr;
    }
    bool release(T* pointer) {
        std::lock_guard g(mtx_);
        auto* chunk = &chunk_storage_[0];
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
    [[maybe_unused]]
    void rearrange_free_spaces(){
        auto* chunk = &chunk_storage_[0];
        auto* prev_chunk = &chunk_storage_[0];
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
    Chunk<T> chunk_storage_[N] = {};
    T pre_allocated_[N] = {};
};

template<class T, size_t N>
struct CustomAllocator {
    using size_type = size_t;
    using difference_type = ptrdiff_t;


    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = CustomAllocator<U, N>;
    };

    constexpr CustomAllocator() noexcept {};
    ~CustomAllocator() = default;

    constexpr CustomAllocator(const CustomAllocator&) noexcept {};
    CustomAllocator& operator=(const CustomAllocator&) = default;

    pointer allocate(std::size_t n) {
#ifdef DEBUG_
        std::cout << "allocate: " << n << std::endl;
#endif
        pointer allocated_block = mem_allocator_.get(n);
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

    void deallocate(pointer p, [[maybe_unused]]std::size_t n) {
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
    }

    void destroy(pointer p) {
        p->~T();
        mem_allocator_.release(p);
    }
    StackMemoryAllocator<T, N> mem_allocator_;
};