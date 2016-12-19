//
// Created by chensn on 2016/12/19.
//

#include <cstdlib>
#include "Alloc.h"

namespace TinySTL {
    char *alloc::start_free = 0;
    char *alloc::end_free = 0;
    size_t alloc::heap_size = 0;

    alloc::obj *alloc::free_list[alloc::ENFreeLists::NFREELISTS] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    void *alloc::allocate(size_t bytes) {
        // 大于128就调用第一级配置器
        if (bytes > EMaxBytes::MAXBYTES) {
            return malloc(bytes);
        }
        size_t index = FREELIST_INDEX(bytes);
        obj *list = free_list[index];
        if (list) { // list不为空,说明还有空间
            free_list[index] = list->next;
            return list;
        } else { // 说明list没有足够空间,需要从内存池中取空间
            return refill(ROUND_UP(bytes));
        }
    }

    void alloc::deallocate(void *ptr, size_t bytes) {
        if (bytes > EMaxBytes::MAXBYTES) {
            free(ptr);
            return;
        }
        size_t index = FREELIST_INDEX(bytes);
        obj *node = (obj *)(ptr);
        // 头插法
        node->next = free_list[index];
        free_list[index] = node;
    }

    // 返回一个大小为n的对象
    // 其中的n已经上调为8的倍数
    void *alloc::refill(size_t n) {
        size_t nobjs = ENObjs::NOBJS;
        // 从内存池中取
        char *chunk = chunk_alloc(n, nobjs);
        obj **my_free_list = 0;
        obj *result = 0;
        obj *current_obj = 0, *next_obj = 0;
        int i;

        // 如果取出的只有一个区块,就将这个区块分配给调用者,free list无新节点
        if (nobjs == 1) return chunk;
        // 否则准备调整free list
        my_free_list = free_list + FREELIST_INDEX(n);
        result = (obj *)(chunk);
        *my_free_list = next_obj = (obj *)(chunk + n);
        // 以下将free list的各节点串联起来
        for (i = 1; ; ++i) {
            current_obj = next_obj;
            next_obj = (obj *)((char *)next_obj + n);
            if (nobjs - 1 == i) {
                current_obj->next = 0;
                break;
            } else {
                current_obj->next = next_obj;
            }
        }
        return result;
    }
    //假设bytes已经上调为8的倍数
    char *alloc::chunk_alloc(size_t size, size_t &nobjs) {
        char *result;
        size_t total_bytes = size * nobjs;
        size_t bytes_left = end_free - start_free;  // 内存池剩余空间

        if (bytes_left >= total_bytes) {    // 完全满足需要
            result = start_free;
            start_free = start_free + total_bytes;
            return result;
        } else if (bytes_left >= size) {    //只满足一个或以上
            nobjs = bytes_left / size;
            total_bytes = size * nobjs;
            result = start_free;
            start_free += total_bytes;
            return result;
        } else {    // 内存池剩余空间连一个区块的大小都无法提供
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
            if (bytes_left > 0) {
                // 内存池内还有一些零头
                obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
                ((obj *)start_free)->next = *my_free_list;  // ???
                *my_free_list = (obj *)start_free;
            }
            start_free = (char *)malloc(bytes_to_get);
            if (0 == start_free) { // 如果申请动态内存失败
                obj **my_free_list = 0, *p = 0;
                for (int i = 0; i <= EMaxBytes::MAXBYTES; i+=EAlign::ALIGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if (p != 0) {
                        *my_free_list = p->next;
                        start_free = (char *)p;
                        end_free = start_free + i;
                        return chunk_alloc(size, nobjs);
                    }
                }
                end_free = 0;
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            // 递归调用自身,为了修正nobjs
            return chunk_alloc(size, nobjs);
        }
    }
}

