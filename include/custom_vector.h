template <typename T, class Allocator = std::allocator<T>>
class CustomVector {
public:
    CustomVector() = default;

    template <typename Tp, class Al = std::allocator<T>>
    CustomVector(const CustomVector<Tp, Al>&){
    }

    T& operator[](size_t index)
    {
        return data_[index];
    }

    T* begin()
    {
        return data_;
    }
    T* end()
    {
        return data_ + size_;
    }

    size_t size() const
    {
        return size_;
    }
    size_t capacity() const
    {
        return capacity_;
    }

    // принимаем объект по значению
    void push_back(T value)
    {
        if(size_ == capacity_)
            expand();

        data_[size_++] = value;
    }

private:
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    Allocator allocator_;
    // expand vector
    void expand()
    {
        T* old_data = data_;
        std::size_t oldcap = capacity_;

        auto new_cap = (capacity_ == 0 ? 1 : 2 * capacity_);
        capacity_ = new_cap;
        data_ = allocator_.allocate(capacity_);
        //move
        {
            T* p = old_data;
            T* np = data_;
            for(size_t i = 0; i < size_; ++i){
                allocator_.construct(np, *p);
                ++np;
                ++p;
            }

        }
        //destruct
        {
            T* p = old_data;
            for(size_t i = 0; i < size_; ++i){
                allocator_.destroy(p);
                ++p;
            }
        }
        //deallocate
        if(old_data != nullptr)
            allocator_.deallocate(old_data, oldcap);

    }
};
