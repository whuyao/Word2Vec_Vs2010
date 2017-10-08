#include "aligned_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include <math.h>
#include <errno.h>

/*
void* aligned_malloc(size_t size, long long align)
{
	void* raw_malloc_ptr;		//初始分配的地址
	void* aligned_ptr;			//最终我们获得的alignment地址

	if( align & (align - 1) )	//如果alignment不是2的n次方，返回
	{
		errno = EINVAL;
		return ( (void*)0 );
	}

	if( 0 == size )
	{
		return ( (void*)0 );
	} 

	//将alignment置为至少为2*sizeof(void*),一种优化做法。
	if( align < 2*sizeof(void*) )
	{
		align = 2 * sizeof(void*);
	}

	raw_malloc_ptr = malloc(size + align);
	if( !raw_malloc_ptr )
	{
		return ( (void*)0 );
	}

	//Align  We have at least sizeof (void *) space below malloc'd ptr. 
	aligned_ptr = (void*) ( ((size_t)raw_malloc_ptr + align) & ~((size_t)align - 1));

	( (void**)aligned_ptr )[-1] = raw_malloc_ptr;

	return aligned_ptr;
}

void* aligned_free(void * aligned_ptr)
{
	if( aligned_ptr )
	{
		free( ((void**)aligned_ptr)[-1] );
	}

	return NULL;
}
*/

void* aligned_malloc(size_t size, long long alignment)  
{  
	// 分配足够的内存, 这里的算法很经典, 早期的STL中使用的就是这个算法  

	// 首先是维护FreeBlock指针占用的内存大小  
	const int pointerSize = sizeof (void*);  

	// alignment - 1 + pointerSize这个是FreeBlock内存对齐需要的内存大小  
	// 前面的例子sizeof(T) = 20, __alignof(T) = 16,  
	// g_MaxNumberOfObjectsInPool = 1000  
	// 那么调用本函数就是alignedMalloc(1000 * 20, 16)  
	// 那么alignment - 1 + pointSize = 19  
	const long long requestedSize = size + alignment - 1 + pointerSize;  

	// 分配的实际大小就是20000 + 19 = 20019  
	void* raw = malloc(requestedSize);  

	// 这里实Pool真正为对象实例分配的内存地址  
	uintptr_t start = (uintptr_t) raw + pointerSize;  
	// 向上舍入操作  
	// 解释一下, __ALIGN - 1指明的是实际内存对齐的粒度  
	// 例如__ALIGN = 8时, 我们只需要7就可以实际表示8个数(0~7)  
	// 那么~(__ALIGN - 1)就是进行舍入的粒度  
	// 我们将(bytes) + __ALIGN-1)就是先进行进位, 然后截断  
	// 这就保证了我是向上舍入的  
	// 例如byte = 100, __ALIGN = 8的情况  
	// ~(__ALIGN - 1) = (1 000)B  
	// ((bytes) + __ALIGN-1) = (1 101 011)B  
	// (((bytes) + __ALIGN-1) & ~(__ALIGN - 1)) = (1 101 000 )B = (104)D  
	// 104 / 8 = 13, 这就实现了向上舍入  
	// 对于byte刚好满足内存对齐的情况下, 结果保持byte大小不变  
	// 记得《Hacker's Delight》上面有相关的计算  
	// 这个表达式与下面给出的等价  
	// ((((bytes) + _ALIGN - 1) * _ALIGN) / _ALIGN)  
	// 但是SGI STL使用的方法效率非常高   
	void* aligned = (void*) ((start + alignment - 1) & ~(alignment - 1));  

	// 这里维护一个指向malloc()真正分配的内存  
	*(void**) ((uintptr_t) aligned - pointerSize) = raw;  

	// 返回实例对象真正的地址  
	return aligned;  
}  


// 这里是内部维护的内存情况  
//                   这里满足内存对齐要求  
//                             |  
// ----------------------------------------------------------------------  
// | 内存对齐填充 | 维护的指针 | 对象1 | 对象2 | 对象3 | ...... | 对象n |  
// ----------------------------------------------------------------------  
// ^                     | 指向malloc()分配的地址起点  
// |                     |  
// -----------------------  
void aligned_free(void* aligned)  
{  
	// 释放操作很简单了, 参见上图  
	void* raw = *(void**) ((uintptr_t) aligned - sizeof (void*));  
	free(raw);  
}  

bool isAligned(void* data, long long alignment)  
{  
	// 又是一个经典算法, 参见<Hacker's Delight>  
	return ((uintptr_t) data & (alignment - 1)) == 0;  
}  