#include <iostream>
#include <cstdio>
#include <new>
#include <iterator>

template<typename T>
void copyArr(const T* from, T* to, int sz) {
    for (int i = 0; i < sz; ++i)
        to[i] = from[i];
}

template<typename T>
class Deque {
private:
    const int EXPAND_KOEF = 2;
    const int SHRINK_KOEF = 2;
    const int MAX_CARRY_KOEF = 4;
    
    template <class Type = T, class Pointer = T*, class Reference = T&>
    class DequeIterator : public std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t, Pointer, Reference> {
    private:
        Pointer arr;
        Pointer begPtr;
        int capacity;
        int itemIndex;       
        
    public:
        
        DequeIterator(Pointer _arr, Pointer _begPtr, int _capacity, int _itemIndex):  
            arr(_arr), begPtr(_begPtr), capacity(_capacity), itemIndex(_itemIndex) {}

    
        DequeIterator(const DequeIterator& iter) {
            itemIndex = iter.itemIndex;
            begPtr = iter.begPtr;
            arr = iter.arr;
            capacity = iter.capacity;
        }
    
        ~DequeIterator() {}
    
        DequeIterator operator=(const DequeIterator& other) {
            itemIndex = other.itemIndex;
            begPtr = other.begPtr;
            arr = other.arr;
            capacity = other.capacity;
            return *this;
        }
    
        const DequeIterator& operator++(int) {
            DequeIterator res = DequeIterator(*this);
            ++itemIndex;
            return res;
        }
        
        DequeIterator& operator++() {
            ++itemIndex;
            return *this;
        }
        
        const DequeIterator& operator--(int) {
            DequeIterator res = DequeIterator(*this);
            --itemIndex;
            return res;
        }
        
        DequeIterator& operator--() {
            --itemIndex;
            return *this;
        }
        
        DequeIterator& operator+=(int x) {
            itemIndex += x;
            return *this; 
        }
        
        DequeIterator& operator-=(int x) {
            itemIndex -= x;
            return *this;
        }
        
        DequeIterator operator+(int x) const {
            DequeIterator iter = *this;
            return iter += x;
        }
        
        DequeIterator operator-(int x) const {
            DequeIterator iter = *this;
            return iter -= x;
        }
        
        int operator-(const DequeIterator& other) const {
            return itemIndex - other.itemIndex;
        }
        
        Reference operator*() {
            int rlen = arr + (capacity - 1) - begPtr + 1;
            if (itemIndex <= rlen) 
                return *(begPtr + (itemIndex - 1));
            return arr[itemIndex - rlen - 1];
        }
        
        Pointer operator->() {
            int rlen = arr + (capacity - 1) - begPtr + 1;
            if (itemIndex <= rlen) 
                return begPtr + (itemIndex - 1);
            return &arr[itemIndex - rlen - 1];
        }
        
        bool operator==(const DequeIterator& iter) const {
            return itemIndex == iter.itemIndex;
        }
        
        bool operator!=(const DequeIterator& iter) const {
            return !(*this == iter);
        }
        
        bool operator<(const DequeIterator& iter) const {
            return itemIndex < iter.itemIndex;
        }
        
        bool operator<=(const DequeIterator& iter) const {
            return itemIndex <= iter.itemIndex;
        }
        
        bool operator>(const DequeIterator& iter) const {
            return itemIndex > iter.itemIndex;
        }
        
        bool operator>=(const DequeIterator& iter) const {
            return itemIndex >= iter.itemIndex;
        }
    };
    
    T* arr;
    int capacity;
    int begPtr, endPtr;
    int cnt;
    
    
    void expand() {
        T* buffer = new T[capacity * EXPAND_KOEF];
        for (int i = begPtr, curInd = capacity; ; ++i, ++curInd) {
            if (i == capacity)
                i = 0;
            buffer[curInd] = arr[i];
            if (i == endPtr)
                break;
        }
        delete[] arr;
        capacity *= EXPAND_KOEF;
        arr = new T[capacity];
        copyArr(buffer, arr, capacity);
        begPtr = capacity / 2;
        endPtr = capacity / 2 + cnt - 1;
        delete[] buffer;
    }
    
    void shrink() {
        T* buffer = new T[capacity / SHRINK_KOEF];
        for (int i = begPtr, curInd = capacity / SHRINK_KOEF / 2; ; ++i, ++curInd) {
            if (i == capacity)
                i = 0;
            buffer[curInd] = arr[i];
            if (i == endPtr)
                break;
        }
        delete[] arr;
        capacity /= SHRINK_KOEF;
        arr = new T[capacity];
        copyArr(buffer, arr, capacity);
        begPtr = capacity / 2;
        endPtr = capacity / 2 + cnt - 1;
        delete[] buffer;
    }
    
public:
    
    typedef DequeIterator<> iterator;
    typedef DequeIterator<const T, const T*, const T&> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    
    Deque() {
        arr = new T[1];
        capacity = 2;
        cnt = 0;
        begPtr = endPtr = 1;
    }
    
    Deque(const Deque& deq) {
        capacity = deq.capacity;
        begPtr = deq.begPtr;
        endPtr = deq.endPtr;
        cnt = deq.cnt;
        arr = new T[capacity];
        copyArr(deq.arr, arr, capacity);
    }
    
    Deque(Deque&& deq) {
        arr = deq.arr;
        capacity = deq.capacity;
        begPtr = deq.begPtr;
        endPtr = deq.endPtr;
        cnt = deq.cnt;
    }
    
    ~Deque() {
        delete[] arr;
    }
    
    const T& back() const {
        return arr[endPtr];
    }
    
    T& back() {
        return arr[endPtr];
    }
    
    const T& front() const {
        return arr[begPtr];
    }
    
    T& front() {
        return arr[begPtr];
    }
    
    void pop_back() {
        --endPtr;
        if (endPtr == -1)
            endPtr = capacity - 1;
        --cnt;
        if (cnt > 0 && MAX_CARRY_KOEF * cnt <= capacity)
            shrink();
    }
    
    void pop_front() {
        ++begPtr;
        if (begPtr == capacity)
            begPtr = 0;
        --cnt;
        if (cnt > 0 && MAX_CARRY_KOEF * cnt <= capacity)
            shrink();
    }
    
    void push_back(const T& val) {
        if (cnt + 1 >= capacity)
            expand();
        ++cnt;
        if (cnt == 1) {
            begPtr = endPtr = capacity / 2;
            arr[begPtr] = val;
            return;
        }
        ++endPtr;
        if (endPtr == capacity)
            endPtr = 0;
        arr[endPtr] = val;
    }
    
    void push_front(const T& val) {
        if (cnt + 1 >= capacity)
            expand();
        ++cnt;
        if (cnt == 1) {
            begPtr = endPtr = capacity / 2;
            arr[begPtr] = val;
            return;
        }
        --begPtr;
        if (begPtr == -1)
            begPtr = capacity - 1;
        arr[begPtr] = val;
    }
    
    bool empty() const {
        return !cnt;
    }
    
    int size() const {
        return cnt;
    }
    
    T& operator[](int pos) const {
        int id = begPtr + pos;
        if (id >= capacity)
            id -= capacity;
        return arr[id];
    }
    
    bool operator==(const Deque& other) const {
        if (capacity != other.capacity || cnt != other.cnt || begPtr != other.begPtr || endPtr != other.endPtr)
            return false;
        for (int i = 0; i < capacity; ++i)
            if (arr[i] != other.arr[i])
                return false;
        return true;
    }
    
    Deque operator=(const Deque& other) {
        if (*this == other)
            return *this;
        delete[] arr;
        capacity = other.capacity;
        cnt = other.cnt;
        begPtr = other.begPtr;
        endPtr = other.endPtr;
        arr = new T[capacity];
        copyArr(other.arr, arr, capacity);
        return *this;
    }
    
    void clear() {
        while (cnt > 0)
            pop_back();
    }
    
    iterator begin() {
        return iterator(arr, arr + begPtr, capacity, 1);
    }
    
    iterator end() {
        return iterator(arr, arr + begPtr, capacity, cnt + 1);
    }
    
    const const_iterator cbegin() const {
        return const_iterator(arr, arr + begPtr, capacity, 1);
    }
    
    const const_iterator cend() const {
        return const_iterator(arr, arr + begPtr, capacity, cnt + 1);
    }
    
    const const_iterator begin() const {
        return cbegin();
    }
    
    const const_iterator end() const {
        return cend();
    }
    
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    
    const const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }
    
    const const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }
    
    const const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }
    
    const const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }
};