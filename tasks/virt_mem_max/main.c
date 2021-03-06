#include<sys/mman.h>
#include<stdio.h>

int main()
{
	size_t result = 0;
    	size_t bytes_to_allocate = 1;
	void* res = NULL;
	while (res != MAP_FAILED) 
	{
		void* addr = mmap(NULL, bytes_to_allocate, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
		bytes_to_allocate *= 2;
		result+= bytes_to_allocate;
		res = addr;
	}

	size_t bytes_to_allocate1 = bytes_to_allocate / 2;
	result += bytes_to_allocate1;
	
	while (res != MAP_FAILED)
	{
		void* addr = mmap(NULL, bytes_to_allocate1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
		bytes_to_allocate1 /= 2;
		result += bytes_to_allocate1;
		res = addr;
	}

	printf("Allocated: %zu bytes of memory\n", result);
	return 0;
}
