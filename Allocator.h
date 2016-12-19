//
// Created by chensn on 2016/12/19.
//

#ifndef TINYSTL_ALLOCATOR_H
#define TINYSTL_ALLOCATOR_H


#include "Alloc.h"

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
//        typedef ptrdiff_t   difference_type;
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
        return ;
    }

    template <class T>
    T *allocator<T>::allocate(size_t n) {
        if (n == 0) return nullptr;
        return static_cast<T *>(alloc::allocate(sizeof(T) * n));
    }
}

#endif //TINYSTL_ALLOCATOR_H
