#include <iostream>
#include <atomic>


#ifndef SharedPtr_hpp
#define SharedPtr_hpp


namespace cs540 {

std::mutex mtx;

class Helper {
    template <typename T> friend class SharedPtr;
public:
    int ref_count{};
    
    Helper() {};
    virtual ~Helper() {};
};

template <typename U> class Helper2 : public Helper {
private:
    U *ptr;
    using Helper::ref_count;
public:
    Helper2(U *p) {
        ptr = p;
        ref_count = 1;
    };
    ~Helper2() {
        delete ptr;
        ptr = nullptr;
    };
};


template <typename T> class SharedPtr {
public:
    T *ptr;
    Helper *h;
    
public:
    
    void inc(){
        if (h != nullptr) {
            std::unique_lock<std::mutex> lock(mtx);
            h->ref_count++;
        }
    };
    
    void dec(){
        if (h != nullptr) {
            std::unique_lock<std::mutex> lock(mtx);
            h->ref_count--;
            if (h->ref_count == 0){
                delete h;
                ptr = nullptr;
                h = nullptr;
            }
        }
    };
    
    SharedPtr() : ptr(nullptr), h(nullptr) {};
    
    template <typename U> explicit SharedPtr(U *p){
        ptr = p;
        h = new Helper2<U>(p);
    };
    
    SharedPtr(const SharedPtr &p) {
        ptr = p.ptr;
        h = p.h;
        if (h != nullptr){
            inc();
        }
    };
    template <typename U> SharedPtr(const SharedPtr<U> &p) {
        ptr = static_cast<T*>(p.ptr);
        h = p.h;
        if (ptr != nullptr){
            inc();
        }
    };
    
    SharedPtr(SharedPtr &&p) {
        ptr = p.ptr;
        h = p.h;
        p.ptr = nullptr;
        p.h = nullptr;
    };
    template <typename U> SharedPtr(SharedPtr<U> &&p){
        ptr = p.ptr;
        h = p.h;
        p.ptr = nullptr;
        p.h = nullptr;
    };
        
    SharedPtr &operator=(const SharedPtr &p){
        if (*this != p){
            dec();
            ptr = p.ptr;
            h = p.h;
            inc();
        }
        return *this;
    };
    
    template <typename U> SharedPtr<T> &operator=(const SharedPtr<U> &p){
        if (*this != p){
            dec();
            ptr = p.ptr;
            h = p.h;
            inc();
        }
        return *this;
    };
    
    SharedPtr &operator=(SharedPtr &&p){
        if (*this != p){
            dec();
            ptr = p.ptr;
            h = p.h;
            p.ptr = nullptr;
            p.h = nullptr;
        }
        return *this;
    };
    
    template <typename U> SharedPtr &operator=(SharedPtr<U> &&p){
        if (*this != p){
            dec();
            ptr = p.ptr;
            h = p.h;
            p.ptr = nullptr;
            p.h = nullptr;
        }
        return *this;
    };
    
    ~SharedPtr(){
        dec();
    };
    
    template <typename U> SharedPtr(const SharedPtr<U> &sp, T *p) {
        ptr = p;
        h = sp.h;
        if (ptr) {
            inc();
        }else{
            h = nullptr;
        }
    }
    
    //Modifiers
    void reset(){
        dec();
        ptr = nullptr;
        h = nullptr;
    };
    
    template <typename U> void reset(U *p){
        dec();
        ptr = p;
        h = new Helper2<U>(p);
    };
    
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
template <typename T1, typename T2> bool operator==(const SharedPtr<T1> &p1, const SharedPtr<T2> &p2){
    return p1.get() == p2.get() || (p1 == nullptr && p2 == nullptr);
};

template <typename T> bool operator==(const SharedPtr<T> &p, std::nullptr_t n){
    return !p;
};

template <typename T> bool operator==(std::nullptr_t n, const SharedPtr<T> &p){
    return !p;
};

template <typename T1, typename T2> bool operator!=(const SharedPtr<T1> &p1, const SharedPtr<T2> &p2){
    return !(p1 == p2);
};

template <typename T> bool operator!=(const SharedPtr<T> &p, std::nullptr_t n){
    return !(p == n);
};
template <typename T> bool operator!=(std::nullptr_t n, const SharedPtr<T> &p){
    return !(p == n);
};

template <typename T, typename U> SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp){
    return SharedPtr<T>(sp);
};

template <typename T, typename U> SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp){
    auto p = dynamic_cast<T*>(sp.get());
    return SharedPtr<T>(sp, p);
};

}

#endif /* SharedPtr_h */
