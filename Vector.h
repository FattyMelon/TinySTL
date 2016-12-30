//
// Created by chensn on 2016/12/27.
//

#ifndef TINYSTL_VECTOR_H
#define TINYSTL_VECTOR_H

#include <assert.h>
#include "Allocator.h"
#include "Iterator.h"
#include "UninitializedFunctions.h"
namespace TinySTL {
    template <class T, class Alloc = allocator<T>>
    class vector {
    public:
        typedef T                       value_type;
        typedef value_type *            iterator;   //vector的迭代器是普通指针
        typedef const value_type *		const_iterator;
        typedef value_type *            pointer;
        typedef value_type &            reference;
        typedef size_t					size_type;
        typedef ptrdiff_t	            difference_type;

    private:
        iterator start;             //表示目前使用空间的头
        iterator finish;            //使用空间的尾
        iterator end_of_storage;    //可用空间的尾

    public:
        vector() : start(0), finish(0), end_of_storage(0) {}
        vector(size_type n, const T& value);
        vector(int n, const T& value);
        vector(long n, const T& value);
        explicit vector(size_type n);
        template<class InputIterator>
        vector(InputIterator first, InputIterator last);
        vector(const vector& v);
        vector(vector&& v);
        vector& operator = (const vector& v);
        vector& operator = (vector&& v);
        ~vector();

        //比较操作
        bool operator == (const vector& v) const;
        bool operator != (const vector& v) const;

        //迭代器相关
        iterator begin() { return start; }
        const_iterator begin() const { return start; }    //const vector优先调用这个
        const_iterator cbegin() const { return start; }
        iterator end() { return finish; }
        const_iterator end() const { return finish; }          //const vector优先调用这个
        const_iterator cend() const { return finish; }

        //与容器相关
        size_type size() const { return size_type(finish - start); }
        size_type capacity() const { return size_type(end_of_storage - start); }
        bool empty() const { return start == finish; }
        void resize(size_type n, value_type val = value_type());

        //与访问相关
        reference operator[](const size_type i){ return *(begin() + i); }
        reference front() { return *begin(); }
        reference back() { return *(end() - 1); }

        //修改容器相关的操作
        //清空容器，销毁容器中的所有对象并使容器的size为0，但不回收容器已有的空间
        void clear();
        void swap(vector& v);
        void push_back(const value_type& value);
        void pop_back();
        iterator insert(iterator position, const value_type& val);
        void insert(iterator position, const size_type& n, const value_type& val);
        template <class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);

    private:
        template<class InputIterator>
        void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
        template <class Integer>
        void insert_aux(iterator position, Integer n, const value_type& value, std::true_type);
    };

    template <class T, class Alloc>
    template <class InputIterator>
    void vector<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {

    }

    template <class T, class Alloc>
    template <class Integer>
    void vector<T, Alloc>::insert_aux(iterator position, Integer n, const value_type& value, std::true_type) {
        assert(n != 0);
        difference_type locationLeft = end_of_storage - finish;
        difference_type locationNeed = n;

        if (locationLeft >= locationNeed) {
            auto tempPtr = end() - 1;
            for (; tempPtr - position >= 0; --tempPtr) {
                construct(tempPtr + locationNeed, *tempPtr);
            }
            TinySTL::uninitialized_fill_n(position, n, value);
            finish += locationNeed;
        } else {
//            reallocateAndFillN(position, n, value);
        }
    };

    template<class T, class Alloc>
    template<class InputIterator>
    void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last){
        insert_aux(position, first, last, typename std::is_integral<InputIterator>::type());
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::insert(iterator position, const size_type& n, const value_type& val){
        insert_aux(position, n, val, typename std::is_integral<size_type>::type());
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type& val){
        const auto index = position - begin();
        insert(position, 1, val);
        return begin() + index;
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::push_back(const value_type& value){
        insert(end(), value);
    }
}


#endif //TINYSTL_VECTOR_H
