#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

// bool for first time allocating memory.
static int first = 1;

// ram memory given by the OS
static char *start_memory = NULL;
static char *end_memory = NULL;


//Header of an allocated piece of memory
typedef struct Block
{
    // 8 bytes on a 64bit OS
    size_t size;
    int free;
    struct Block *next;
}Block;

static Block *free_header_list = NULL; 

char* my_alloc(int size);
int main()
{
   
    Block *freeBlock;
    char *ptr = my_alloc(16);
    char *ptr2 = my_alloc(16);


    *ptr = 'E';
    *ptr2 = 'B';
    printf("%c%c\n", *ptr, *ptr2);

    return 0;
}
char*  my_alloc(int size)
{
    void *memory = NULL;
    // check to see if its teh first time allocating memory.
    if(first == 1)
    {
        first = 0;
        // Asks the OS for memory to use in the program
        memory = mmap(NULL, (sizeof(size_t) * 200), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if(memory == MAP_FAILED)
        {
            printf("Error, could not reserve space from heap.");
            return 1;
        }
        start_memory = memory;
        end_memory = memory + (sizeof(size_t) * 200);

        printf("%p\n", start_memory);
        printf("%p\n", end_memory);

        free_header_list = memory;

    }
    char *return_address = (char*)free_header_list + sizeof(Block);
    free_header_list->size = size;
    free_header_list->free = 0;

    if(start_memory < end_memory)
    {

        Block *next = (Block*)((char*)free_header_list + sizeof(Block) + size);
        // free_header_list->next = next;
        start_memory = (char*)next;
        free_header_list = next;
        
    }
    return return_address;
}