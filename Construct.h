//
// Created by chensn on 2016/12/19.
//

#ifndef TINYSTL_CONSTRUCT_H
#define TINYSTL_CONSTRUCT_H

#include <new>
#include "TypeTraits.h"

namespace TinySTL {
    template <class T1, class T2>
    inline void construct(T1 *ptr1, const T2& value) {
        new (ptr1) T1(value);
    };

    template <class T>
    inline void destroy(T *ptr) {
        ptr->~T();
    }

    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last){
        typedef typename _type_traits<ForwardIterator>::has_trivial_destructor trivial_destructor;
        _destroy(first, last, trivial_destructor());
    }

    template<class ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIterator last, _true_type){}

    template<class ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type){
        for (; first != last; ++first){
            destroy(&*first);
        }
    }
}

#endif //TINYSTL_CONSTRUCT_H
