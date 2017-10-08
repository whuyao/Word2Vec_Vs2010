#ifndef ALIGNED_MEMORY_FUNCTIONS
#define ALIGNED_MEMORY_FUNCTIONS

/*
void* aligned_malloc(size_t size, long long align);
void* aligned_free(void * aligned_ptr);
*/
void* aligned_malloc(size_t size, long long alignment);
void aligned_free(void* aligned);
bool isAligned(void* data, long long alignment);

#endif