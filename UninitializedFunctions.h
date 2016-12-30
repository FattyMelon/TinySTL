//
// Created by chensn on 2016/12/27.
//

#ifndef TINYSTL_UNINITIALIZEDFUNCTIONS_H
#define TINYSTL_UNINITIALIZEDFUNCTIONS_H

#include <algorithm>
#include "TypeTraits.h"

#include "Allocator.h"
//#include "Algorithm.h"
#include "Iterator.h"
//#include "ReverseIterator.h"
#include "UninitializedFunctions.h"

namespace TinySTL {
    /**********************************************************/
    /*
    * copy
    * */
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
                                            ForwardIterator result, _true_type);
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
                                            ForwardIterator result, _false_type);

    template<class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
        typedef typename _type_traits<typename iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
        return _uninitialized_copy_aux(first, last, result, isPODType());
    }
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
                                            ForwardIterator result, _true_type){
        memcpy(result, first, (last - first) * sizeof(*first));
        return result + (last - first);
    }
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
                                            ForwardIterator result, _false_type){
        int i = 0;
        for (; first != last; ++first, ++i){
            construct(&*(result + i), *first);  //迭代器(不能保证是指针),应该用*运算符后再取地址。
        }
        return (result + i);
    }

    /**********************************************************/
    /*
    * fill
    * */
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                 const T& value, _true_type);
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                 const T& value, _false_type);

    template<class ForwardIterator, class T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value){
        typedef typename _type_traits<T>::is_POD_type isPODType;
        _uninitialized_fill_aux(first, last, value, isPODType());
    }
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                 const T& value, _true_type){
        std::fill(first, last, value);
    }
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                 const T& value, _false_type){
        for (; first != last; ++first){
            construct(&*first, value);
        }
    }

    /**********************************************************/
    /*
    * fill_n
    * */
    template <class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, _true_type);

    template<class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, _false_type);

    template <class ForwardIterator, class Size, class T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x) {
        typedef typename _type_traits<T>::is_POD_type isPODType;
        return _uninitialized_fill_n_aux(first, n, x, isPODType());
    }

    template <class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, _true_type) {
        return std::fill_n(first, n, x);
    }

    template <class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, _false_type) {
        int i = 0;
        for (; i != n; ++i) {
            construct((T*)(first + i), x);
        }
        return (first + i);
    }
}

#endif //TINYSTL_UNINITIALIZEDFUNCTIONS_H

