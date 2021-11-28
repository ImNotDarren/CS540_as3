#include <iostream>
#include <atomic>


#ifndef SharedPtr_hpp
#define SharedPtr_hpp


namespace cs540 {


template <typename T> class SharedPtr {
public:
    T *ptr;
    
    SharedPtr() : ptr(nullptr) {};
    
    template <typename U> explicit SharedPtr(U *p){
        ptr = static_cast<T*>(p);
    };
    
    SharedPtr(const SharedPtr &p);
    template <typename U> SharedPtr(const SharedPtr<U> &p);
    
    SharedPtr(SharedPtr &&p);
    template <typename U> SharedPtr(SharedPtr<U> &&p);
    
    SharedPtr &operator=(const SharedPtr &);
    template <typename U> SharedPtr<T> &operator=(const SharedPtr<U> &);
    
    SharedPtr &operator=(SharedPtr &&p);
    template <typename U> SharedPtr &operator=(SharedPtr<U> &&p);
    
    ~SharedPtr();
    
    //Modifiers
    void reset();
    
    template <typename U> void reset(U *p);
    
    //Observers
    T *get() const {
        return ptr;
    };
    
    T &operator*() const {
        return *ptr;
    };
    
    T *operator->() const {
        return ptr;
    };
    
    explicit operator bool() const {
        return (ptr != nullptr);
    };
};

//Free functions
template <typename T1, typename T2>
bool operator==(const SharedPtr<T1> &, const SharedPtr<T2> &);

template <typename T>
bool operator==(const SharedPtr<T> &, std::nullptr_t);
template <typename T>
bool operator==(std::nullptr_t, const SharedPtr<T> &);

template <typename T1, typename T2>
bool operator!=(const SharedPtr<T1>&, const SharedPtr<T2> &);

template <typename T>
bool operator!=(const SharedPtr<T> &, std::nullptr_t);
template <typename T>
bool operator!=(std::nullptr_t, const SharedPtr<T> &);

template <typename T, typename U>
SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp);

template <typename T, typename U>
SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp);

}

#endif /* SharedPtr_h */
