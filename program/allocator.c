#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

void my_alloc(int first, size_t size);
int main()
{
   

    my_alloc(1, 16);


    return 0;
}
void my_alloc(int first, size_t size)
{
    void *memory = NULL;
    if(first == 1)
    {
        // Asks the OS for memory to use in the program
        memory = mmap(NULL, (sizeof(size_t) * 200), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if(memory == MAP_FAILED)
        {
            printf("Error, could not reserve space from heap.");
        }
    }
    
    printf("%p\n", memory);
}