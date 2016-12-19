//
// Created by chensn on 2016/12/19.
//

#ifndef TINYSTL_ALLOC_H
#define TINYSTL_ALLOC_H

#include <cstdio>

namespace TinySTL {
    /*
	**空间配置器，以字节数为单位分配
	**内部使用
	*/
    class alloc {
    private:

//  二级配置器
    private:
        enum EAlign { ALIGN = 8 };  // 小型区块的上调边界
        enum EMaxBytes { MAXBYTES = 128 };  // 小型区块的上限
        enum ENFreeLists { NFREELISTS = (EMaxBytes::MAXBYTES / EAlign::ALIGN) };    // free-lists个数
        enum ENObjs{ NOBJS = 20};   // 每次增加的节点数
    private:
        // free-lists节点构造
        union obj {
            union obj *next;
            char client[1];
        };
        static obj *free_list[ENFreeLists::NFREELISTS]; // 16个free-lists节点
    private:
        static char *start_free; // 内存池起始位置
        static char *end_free;  // 内存池结束位置
        static size_t heap_size;
    private:
        // 将bytes上调到8的倍数
        static size_t ROUND_UP(size_t bytes) {
            return ((bytes + EAlign::ALIGN - 1) & ~(EAlign::ALIGN - 1));
        }
        // 根据区块大小,决定使用第n号free-list, n从0开始
        static size_t FREELIST_INDEX(size_t bytes) {
            return ((bytes + EAlign::ALIGN - 1) / EAlign::ALIGN - 1);
        }
        // 返回一个大小为n的对象
        static void *refill(size_t n);
        static char *chunk_alloc(size_t size, size_t &nobjs);

    public:
        static void *allocate(size_t bytes);
        static void deallocate(void *ptr, size_t bytes);
        static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
    };
}

#endif //TINYSTL_ALLOC_H
