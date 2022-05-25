template <typename T, class Allocator = std::allocator<T>>
class SimpleVector {
public:
    SimpleVector() = default;

    template <typename Tp, class Al = std::allocator<T>>
    SimpleVector(const SimpleVector<Tp, Al>&){
    }

    T& operator[](size_t index)
    {
        return data[index];
    }

    T* begin()
    {
        return data;
    }
    T* end()
    {
        return data + size;
    }

    size_t Size() const
    {
        return size;
    }
    size_t Capacity() const
    {
        return capacity;
    }


    // принимаем объект по значению
    void push_back(T value)
    {
        if(size == capacity) ExpandIfNeeded();

        data[size++] = value;
    }

private:
    T* data = nullptr;
    size_t size = 0;
    size_t capacity = 0;
    Allocator allocator;
    // expand vector
    void ExpandIfNeeded()
    {
        T* olddata = data;
        std::size_t oldcap = capacity;

        auto new_cap = (capacity == 0 ? 1 : 2 * capacity);
        capacity = new_cap;
        data = allocator.allocate(capacity);
        //move
        {
            T* p = olddata;
            T* np = data;
            for(size_t i = 0; i < size; ++i){
                allocator.construct(np, *p);
                ++np;
                ++p;
            }

        }
        //destruct
        {
            T* p = olddata;
            for(size_t i = 0; i < size; ++i){
                allocator.destroy(p);
                ++p;
            }
        }
        //deallocate
        if(olddata != nullptr)
            allocator.deallocate(olddata, oldcap);

    }
};
