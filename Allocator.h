//
// Created by chensn on 2016/12/19.
//

#ifndef TINYSTL_ALLOCATOR_H
#define TINYSTL_ALLOCATOR_H


#include "Alloc.h"
#include "Construct.h"

namespace TinySTL {
//    空间配置器
    template <class T>
    class allocator {
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
    public:
        static T *allocate();
        static T *allocate(size_t n);
        static void deallocate(T *ptr);
        static void deallocate(T *ptr, size_t n);

        static void construct(T *ptr);
        static void construct(T *ptr, const T& value);
        static void destroy(T *ptr);
        static void destroy(T *first, T *last);
    };

    template <class T>
    T *allocator<T>::allocate() {
        return (T *)(alloc::allocate(sizeof(T)));
    }

    template <class T>
    T *allocator<T>::allocate(size_t n) {
        if (n == 0) return nullptr;
        return (T *)(alloc::allocate(sizeof(T) * n));
    }

    template<class T>
    void allocator<T>::deallocate(T *ptr){
        alloc::deallocate((void *)(ptr), sizeof(T));
    }
    template<class T>
    void allocator<T>::deallocate(T *ptr, size_t n){
        if (n == 0) return;
        alloc::deallocate((void *)(ptr), sizeof(T)* n);
    }

    template <class T>
    void allocator<T>::construct(T *ptr) {
        new(ptr)T(); // place new, 在ptr指针处覆盖内存, 再进行T类型的初始化
    }

    template <class T>
    void allocator<T>::construct(T *ptr, const T &value) {
        new(ptr)T(value);
    }

    template<class T>
    void allocator<T>::destroy(T *ptr){
        ptr->~T();
    }

    template<class T>
    void allocator<T>::destroy(T *first, T *last){
        for (; first != last; ++first){
            first->~T();
        }
    }
}

#endif //TINYSTL_ALLOCATOR_H
