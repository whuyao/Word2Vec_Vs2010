#include "aligned_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include <math.h>
#include <errno.h>

/*
void* aligned_malloc(size_t size, long long align)
{
	void* raw_malloc_ptr;		//��ʼ����ĵ�ַ
	void* aligned_ptr;			//�������ǻ�õ�alignment��ַ

	if( align & (align - 1) )	//���alignment����2��n�η�������
	{
		errno = EINVAL;
		return ( (void*)0 );
	}

	if( 0 == size )
	{
		return ( (void*)0 );
	} 

	//��alignment��Ϊ����Ϊ2*sizeof(void*),һ���Ż�������
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
	// �����㹻���ڴ�, ������㷨�ܾ���, ���ڵ�STL��ʹ�õľ�������㷨  

	// ������ά��FreeBlockָ��ռ�õ��ڴ��С  
	const int pointerSize = sizeof (void*);  

	// alignment - 1 + pointerSize�����FreeBlock�ڴ������Ҫ���ڴ��С  
	// ǰ�������sizeof(T) = 20, __alignof(T) = 16,  
	// g_MaxNumberOfObjectsInPool = 1000  
	// ��ô���ñ���������alignedMalloc(1000 * 20, 16)  
	// ��ôalignment - 1 + pointSize = 19  
	const long long requestedSize = size + alignment - 1 + pointerSize;  

	// �����ʵ�ʴ�С����20000 + 19 = 20019  
	void* raw = malloc(requestedSize);  

	// ����ʵPool����Ϊ����ʵ��������ڴ��ַ  
	uintptr_t start = (uintptr_t) raw + pointerSize;  
	// �����������  
	// ����һ��, __ALIGN - 1ָ������ʵ���ڴ���������  
	// ����__ALIGN = 8ʱ, ����ֻ��Ҫ7�Ϳ���ʵ�ʱ�ʾ8����(0~7)  
	// ��ô~(__ALIGN - 1)���ǽ������������  
	// ���ǽ�(bytes) + __ALIGN-1)�����Ƚ��н�λ, Ȼ��ض�  
	// ��ͱ�֤���������������  
	// ����byte = 100, __ALIGN = 8�����  
	// ~(__ALIGN - 1) = (1 000)B  
	// ((bytes) + __ALIGN-1) = (1 101 011)B  
	// (((bytes) + __ALIGN-1) & ~(__ALIGN - 1)) = (1 101 000 )B = (104)D  
	// 104 / 8 = 13, ���ʵ������������  
	// ����byte�պ������ڴ����������, �������byte��С����  
	// �ǵá�Hacker's Delight����������صļ���  
	// ������ʽ����������ĵȼ�  
	// ((((bytes) + _ALIGN - 1) * _ALIGN) / _ALIGN)  
	// ����SGI STLʹ�õķ���Ч�ʷǳ���   
	void* aligned = (void*) ((start + alignment - 1) & ~(alignment - 1));  

	// ����ά��һ��ָ��malloc()����������ڴ�  
	*(void**) ((uintptr_t) aligned - pointerSize) = raw;  

	// ����ʵ�����������ĵ�ַ  
	return aligned;  
}  


// �������ڲ�ά�����ڴ����  
//                   ���������ڴ����Ҫ��  
//                             |  
// ----------------------------------------------------------------------  
// | �ڴ������� | ά����ָ�� | ����1 | ����2 | ����3 | ...... | ����n |  
// ----------------------------------------------------------------------  
// ^                     | ָ��malloc()����ĵ�ַ���  
// |                     |  
// -----------------------  
void aligned_free(void* aligned)  
{  
	// �ͷŲ����ܼ���, �μ���ͼ  
	void* raw = *(void**) ((uintptr_t) aligned - sizeof (void*));  
	free(raw);  
}  

bool isAligned(void* data, long long alignment)  
{  
	// ����һ�������㷨, �μ�<Hacker's Delight>  
	return ((uintptr_t) data & (alignment - 1)) == 0;  
}  