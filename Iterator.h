//
// Created by chensn on 2016/12/19.
//

#ifndef TINYSTL_ITERATOR_H
#define TINYSTL_ITERATOR_H

#include <stddef.h>

namespace TinySTL {
    // 五种迭代器类型
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag: public input_iterator_tag {};
    struct bidirectional_iterator_tag: public forward_iterator_tag {};
    struct random_access_iterator_tag: public bidirectional_iterator_tag{};

    template <class T, class Distance>
    struct input_iterator {
        typedef input_iterator_tag  iterator_category;
        typedef T                   value_type;
        typedef Distance            difference_type;
        typedef T*                  pointer;
        typedef T&                  reference;
    };

    struct output_iterator
    {
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;
    };

    template <class T, class Distance>
    struct forward_iterator
    {
        typedef forward_iterator_tag	iterator_category;
        typedef T						value_type;
        typedef Distance				difference_type;
        typedef T*						pointer;
        typedef T&						reference;
    };

    template <class T, class Distance>
    struct bidirectional_iterator
    {
        typedef bidirectional_iterator_tag	iterator_category;
        typedef T							value_type;
        typedef Distance					difference_type;
        typedef T*							pointer;
        typedef T&							reference;
    };

    template <class T, class Distance>
    struct random_access_iterator
    {
        typedef random_access_iterator_tag	iterator_category;
        typedef T							value_type;
        typedef Distance					difference_type;
        typedef T*							pointer;
        typedef T&							reference;
    };

    template <class Category, class T, class Distance = ptrdiff_t,
            class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef Category	iterator_category;
        typedef T			value_type;
        typedef Distance	difference_type;
        typedef Pointer		pointer;
        typedef Reference	reference;
    };

    // 萃取器
    template <class Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::value_type           value_type;
        typedef typename Iterator::difference_type      difference_type;
        typedef typename Iterator::pointer              pointer;
        typedef typename Iterator::reference            reference;
    };

    // 偏特化版
    template <class T>
    struct iterator_traits<T*>
    {
        typedef random_access_iterator_tag 	iterator_category;
        typedef T 							value_type;
        typedef ptrdiff_t 					difference_type;
        typedef T*							pointer;
        typedef T& 							reference;
    };

    template <class T>
    struct iterator_traits<const T*>
    {
        typedef random_access_iterator_tag 	iterator_category;
        typedef T 							value_type;
        typedef ptrdiff_t 					difference_type;
        typedef const T*					pointer;
        typedef const T& 					reference;
    };

    // 获取迭代器的category
    template <class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&) {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    // 获取迭代器的value type
    template <class Iterator>
    inline typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    // 获取迭代器的distance type
    template <class Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
    difference_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    // 计算迭代器之间的距离
    template <class InputIterator>
    inline iterator_traits<InputIterator>::difference_type
    _distance(InputIterator first, InputIterator last,
             input_iterator_tag) {
        iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last) {
            ++first;
            ++n;
        }
        return n;
    }

    template <class RandomAccessIterator>
    inline iterator_traits<RandomAccessIterator>::difference_type
    _distance(RandomAccessIterator first, RandomAccessIterator last,
             random_access_iterator_tag) {
        return last - first;
    }

    template <class InputIterator>
    inline iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last) {
//        typedef typename iterator_traits<InputIterator>::iterator_category category;
//        return _distance(first, last, category());
        return _distance(first, last, iterator_category(first));
    }

    // advance函数,向前几个步长
    template <class InputIterator, class Distance>
    inline void _advance(InputIterator &i, Distance n, input_iterator_tag) {
        while (n--)
            ++i;
    }

    template <class BidirectionalIterator, class Distance>
    inline void _advance(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag) {
        if (n >= 0)
            while (n--) ++i;
        else
            while (n++) --i;
    }

    template <class InputIterator, class Distance>
    inline void _advance(InputIterator &i, Distance n, random_access_iterator_tag) {
        i += n;
    }

    template <class InputIterator, class Distance>
    inline void advance(InputIterator &i, Distance n) {
        _advance(i, n, iterator_category(i));
    }
}

#endif //TINYSTL_ITERATOR_H
