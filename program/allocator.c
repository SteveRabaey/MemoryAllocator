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
    //next free block
    struct Block *next;

    //prev block on the heap
    struct Block *prev
}Block;

static Block *free_allocator_list = NULL; 

char* my_alloc(int size);
void free_mem(char *or_ntr);
int main()
{
   
    Block *freeBlock;
    char *ptr = my_alloc(16);
    char *ptr2 = my_alloc(16);

   
    *ptr = 'E';
    *ptr2 = 'B';

     free_mem(ptr);
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

        free_allocator_list = memory + sizeof(size_t);
        

    }
   
    char *return_address = NULL;
    if(start_memory < end_memory)
    {
        // check size
        //todo: need to implement prev
        if((end_memory - start_memory) >= (size + sizeof(Block)))
        {
            return_address = (char*)free_allocator_list;
            free_allocator_list->size = size;
            free_allocator_list->free = 0;
        }else
        {
            // need to check the freelist for free spots that are big enough
            //set this in a method to reuse under this if. also if no size is big enough request now space.
            //allocateFromList()
        }
        
        Block *next = (Block*)((char*)free_allocator_list + sizeof(Block) + size);
        // free_allocator_list->next = next;
        start_memory = (char*)next;
        next->prev = free_allocator_list;
        free_allocator_list = next;
        
    }
    // allocateFromList()
    return return_address;
}
//allocateFromList() check linked list for free
void free_mem(char *or_ptr)
{
    Block *or_block = or_ptr - sizeof(Block);
    // look for left neightboor + also update header
    while(1)
    {
        //Search pointer of the left header
        Block *left_block = or_block->prev - sizeof(Block);
        //segmentation fault ????
        if(left_block->free == 1)
        {
            left_block->size += (or_block->size + sizeof(Block));
            or_block = left_block;
        }else
        {
            or_block->free = 1;
            return 0;
        }

    }

    //search riht neighboor if flag is free merge. loop this.
    //dont forget to update linked list every time

 

    //set the header on free
}