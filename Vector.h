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
        typedef Alloc                   dataAllocator;

    private:
        iterator start;             //表示目前使用空间的头
        iterator finish;            //使用空间的尾
        iterator end_of_storage;    //可用空间的尾

    public:
        vector() : start(0), finish(0), end_of_storage(0) {}
        vector(size_type n, const T& value) {
            allocateAndFillN(n, value);
        }
        vector(int n, const T& value) {
            allocateAndFillN(n, value);
        }
        vector(long n, const T& value) {
            allocateAndFillN(n, value);
        }
        explicit vector(size_type n) {
            allocateAndFillN(n, value_type());
        }
        template<class InputIterator>
        vector(InputIterator first, InputIterator last) {
            vector_aux(first, last, typename std::is_integral<InputIterator>::type());
        }
        vector(const vector& v) {
            allocateAndCopy(v.start, v.finish);
        }
        vector(vector&& v) {
            start = v.start;
            finish = v.finish;
            end_of_storage = v.end_of_storage;
            v.start = v.finish = v.end_of_storage = 0;
        }
        vector& operator = (const vector& v) {
            if (this != &v) {
                allocateAndCopy(v.start, v.finish);
            }
            return *this;
        }
        vector& operator = (vector&& v) {
            if (this != &v) {
                destroyAndDeallocateAll();
                start = v.start;
                finish = v.finish;
                end_of_storage = v.end_of_storage;
                v.start = v.finish = v.end_of_storage = 0;
            }
            return *this;
        }
        ~vector() { destroyAndDeallocateAll(); }

        //比较操作
        bool operator == (const vector& v) const {
            return size() == v.size() && std::equal(begin(), end(), v.begin());
        }
        bool operator != (const vector& v) const {
            return !(*this == v);
        }

        //插入操作
        iterator insert(iterator position, const value_type& val);
        void insert(iterator position, const size_type& n, const value_type& val);

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
        void resize(size_type n, value_type val = value_type()) {
            if (n < size())
                erase(begin()+n, end());
            else
                insert(end(), n - size(), val);
        }

        //与访问相关
        reference operator[](const size_type i){ return *(begin() + i); }
        reference front() { return *begin(); }
        reference back() { return *(end() - 1); }

        //修改容器相关的操作
        //清空容器，销毁容器中的所有对象并使容器的size为0，但不回收容器已有的空间
        void clear() {
            erase(begin(), end());
        }
        void swap(vector& v) {
            std::swap(start, v.start);
            std::swap(finish, v.finish);
            std::swap(end_of_storage, v.end_of_storage);
        }

        void push_back(const value_type& value) {
            if (finish != end_of_storage) {
                //还有空间
                construct(finish, value);
                ++finish;
            } else
                insert(end(), value);
        }
        void pop_back() {
            --finish;
            destroy(finish);
        }

        iterator erase(iterator position) {
            if (position + 1 != end())
                std::copy(position + 1, finish, position);
            --finish;
            destroy(finish);
            return position;
        }

        iterator erase(iterator first, iterator last) {
            iterator i = std::copy(last, finish, first);
            destroy(i, finish);
            finish = finish - (last - first);
            return first;
        }

        template <class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);

        //容器的空间配置器相关
//        Alloc get_allocator(){ return dataAllocator; }
    private:
        void destroyAndDeallocateAll();
        void allocateAndFillN(const size_type n, const value_type& value);
        template<class InputIterator>
        void allocateAndCopy(InputIterator first, InputIterator last);
        template<class InputIterator>
        void reallocateAndCopy(iterator position, InputIterator first, InputIterator last);
        void reallocateAndFillN(iterator position, const size_type& n, const value_type& val);
        size_type getNewCapacity(size_type len) const;

        template<class InputIterator>
        void vector_aux(InputIterator first, InputIterator last, std::false_type);
        template<class Integer>
        void vector_aux(Integer n, const value_type& value, std::true_type);
        template<class InputIterator>
        void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
        template<class Integer>
        void insert_aux(iterator position, Integer n, const value_type& value, std::true_type);

//    public:
//        template<class T, class Alloc>
//        friend bool operator == (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);
//        template<class T, class Alloc>
//        friend bool operator != (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);
    };

    /*************************************************************************************************************************************************/
    //比较操作
    template<class T, class Alloc>
    bool operator == (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2){
        //return v1 == v2;
        return v1.operator==(v2);
    }
    template<class T, class Alloc>
    bool operator != (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2){
        return !(v1 == v2);
    }

    //分配内存和释放内存函数集
    template <class T, class Alloc>
    inline void vector<T,Alloc>::destroyAndDeallocateAll() {
        if (start) { //如果start不为空
            destroy(start, finish);
            dataAllocator::deallocate(start, capacity());
        }
    }

    template <class T, class Alloc>
    inline void vector<T,Alloc>::allocateAndFillN(const size_type n, const value_type& value) {
        start = dataAllocator::allocate(n);
        TinySTL::uninitialized_fill_n(start, n, value);
        finish = end_of_storage = start + n;
    }

    template <class T, class Alloc>
    template<class InputIterator>
    inline void vector<T,Alloc>::allocateAndCopy(InputIterator first, InputIterator last) {
        start = dataAllocator::allocate(last - first);
        finish = TinySTL::uninitialized_copy(first, last, start);
        end_of_storage = finish;
    };

    //insert函数集
    template <class T, class Alloc>
    template <class InputIterator>
    void vector<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
        difference_type locationLeft = end_of_storage - finish; // the size of left storage
        difference_type locationNeed = distance(first, last);//last - first;

        if (locationLeft >= locationNeed){
            if (finish - position > locationNeed){
                TinySTL::uninitialized_copy(finish - locationNeed, finish, finish);
                std::copy_backward(position, finish - locationNeed, finish);
                std::copy(first, last, position);
            }
            else{
                iterator temp = TinySTL::uninitialized_copy(first + (finish - position), last, finish);
                TinySTL::uninitialized_copy(position, finish, temp);
                std::copy(first, first + (finish - position), position);
            }
            finish += locationNeed;
        }
        else{
            reallocateAndCopy(position, first, last);
        }
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
            reallocateAndFillN(position, n, value);
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
    template<class InputIterator>
    void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type){
        allocateAndCopy(first, last);
    }
    template<class T, class Alloc>
    template<class Integer>
    void vector<T, Alloc>::vector_aux(Integer n, const value_type& value, std::true_type){
        allocateAndFillN(n, value);
    }

    template<class T, class Alloc>
    template<class InputIterator>
    void vector<T, Alloc>::reallocateAndCopy(iterator position, InputIterator first, InputIterator last){
        difference_type newCapacity = getNewCapacity(last - first);

        T *newStart = dataAllocator::allocate(newCapacity);
        T *newEndOfStorage = newStart + newCapacity;
        T *newFinish = TinySTL::uninitialized_copy(begin(), position, newStart);
        newFinish = TinySTL::uninitialized_copy(first, last, newFinish);
        newFinish = TinySTL::uninitialized_copy(position, end(), newFinish);

        destroyAndDeallocateAll();
        start = newStart;
        finish = newFinish;
        end_of_storage = newEndOfStorage;
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::reallocateAndFillN(iterator position, const size_type& n, const value_type& val){
        difference_type newCapacity = getNewCapacity(n);

        T *newStart = dataAllocator::allocate(newCapacity);
        T *newEndOfStorage = newStart + newCapacity;
        T *newFinish = TinySTL::uninitialized_copy(begin(), position, newStart);
        newFinish = TinySTL::uninitialized_fill_n(newFinish, n, val);
        newFinish = TinySTL::uninitialized_copy(position, end(), newFinish);

        destroyAndDeallocateAll();
        start = newStart;
        finish = newFinish;
        end_of_storage = newEndOfStorage;
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::size_type vector<T, Alloc>::getNewCapacity(size_type len)const{
        size_type oldCapacity = end_of_storage - start;
        auto res = std::max(oldCapacity, len);
        size_type newCapacity = (oldCapacity != 0 ? (oldCapacity + res) : len);
        return newCapacity;
    }
}


#endif //TINYSTL_VECTOR_H
