// Hittishi Kurlagunda					[Net ID: hk919]
//Sowmiyanarayan Selvam					[Net ID: ss4370]
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "umalloc.h"

static char mem[MEM_SIZE];

bool init = false;

bool is_block_free(block *block) //Returns the status of the block whether it is free or not.
{
    return (block->is_free);
}

void * umalloc(size_t size, char *file, int line) //Checks whether the requested memory is available along with the metadata size for storing that particular data in user memory.
{
    char *free_block;
    // printf("umalloc\n");
    if (size > MEM_SIZE-sizeof(block)-1) 
    {
        printf("Error: %s in line %d not enough memory.\n", file, line);
        return NULL;
    }
    
    if (init == false)
    {
        ((block *)&mem[0])->is_free = true;
        ((block *)&mem[0])->block_size = MEM_SIZE - (sizeof(block)) - 1;
        init = true;
        printf("initialized\n");
    }

    free_block = find_first_block(size);

    if (free_block == NULL) 
    {
        printf("Error:%s in line %d, requested more memory than available.\n", file, line);
        return NULL;
    } 

    return break_block(free_block, size);
    
}

char * next(char *curr_block) //Checks if there is any next block and if available return the pointer to that block.
{
    char *next_block = curr_block + ((block *)curr_block)->block_size + sizeof(block) + 1;
    // printf("next\n");
    if (next_block >= &mem[MEM_SIZE]) 
    {
        return NULL;
    } 
    
    else 
    {
        return next_block;
    }
}

char * find_first_block(size_t size) //Modification of the first fit algorithm. It finds the first block with the same or greater memory required for allocation.
{
    char *curr_block = &mem[0];
    size_t size_required = sizeof(block) + size + 1;
    // printf("find_first_block\n");
    while (curr_block < &mem[MEM_SIZE] && curr_block != NULL) 
    {
        if (is_block_free(((block *)curr_block)))
            {
                if(((block *)curr_block)->block_size >= size_required) 
                {
                    return curr_block;
                }
            } 
        curr_block = next(curr_block);
    }

    return NULL;
}

void * break_block(char *curr_block, size_t size) //When there is excess memory in the block that has been assigned, the block is broken into 2 one in which the allocation will be done. The other would be a new free block with an updated size for further allocations.
{
    size_t size_required = sizeof(block) + size + 1;
    size_t free_block_size = ((block *)curr_block)->block_size;
    size_t remaining_block_size = free_block_size - size_required;
    // printf("break_block\n");
    
    
    char *remaining_block = curr_block + size_required;

    ((block *)curr_block)->is_free = false;
    ((block *)curr_block)->block_size = size;

    ((block *)remaining_block)->is_free = true;
    ((block *)remaining_block)->block_size = remaining_block_size;
    

    return (void *)(curr_block + sizeof(block) + 1);
}

void merge_free_blocks() //Traverses throughout the user memory and merges adjacent free blocks for future allocations.
{
    size_t newSize = 0;
    char *curr_block = &mem[0];
    char *next_block = NULL;
    // printf("merge_free_blocks\n");
    
    while (curr_block < &mem[MEM_SIZE] && curr_block != NULL) 
    {
        if (is_block_free(((block *)curr_block)))
        {
            next_block = next(curr_block);
            
            if (next_block >= &mem[MEM_SIZE] || next_block == NULL)
            {
                return;
            } 
                
            
            newSize += sizeof(block) + ((block*)next_block)->block_size + 1;

            if (is_block_free(((block *)next_block))) 
            {
                ((block *)curr_block)->block_size += newSize;
                newSize = 0;
                next_block = next(next_block);
            } 
            else 
            {
                curr_block = next(curr_block);
            }
        } 
        else 
        {
            curr_block = next(curr_block);
        }
    }
}

void ufree(void *to_free, char *file, int line) //Frees the blocks located at the pointer provided. If already freed it returns an error. Calls merge free blocks after for smooth memory management.
{
    char *curr_block;
    char *block_pos;
    // printf("ufree\n");
    
    if (to_free == NULL) 
        {
            return;
        }
    
    if (mem + sizeof(block) + 1 > (char*) to_free || (char*) to_free > &mem[MEM_SIZE] - sizeof(block) - 1)
    {
        printf("Error:%s in line %d specified address is invalid and not within range.\n", file, line);
        return;
    }
    
    curr_block = &mem[0];
    block_pos =  (char *)((char *)to_free - sizeof(block) - 1);
    
    while (curr_block < &mem[MEM_SIZE]) 
    {
        if (curr_block == block_pos) 
        {
            if (is_block_free(((block *)curr_block))) 
            {
                printf("Error: %s in line %d specified address was already freed.\n", file, line);
                return;
            }
            ((block *)curr_block)->is_free = true;
            merge_free_blocks();
            return;
        }

        curr_block = next(curr_block);
    }
    printf("Error:%s in line %d, memory hasn't been allocated.\n", file, line);
    return;
}

//Traverses the memory and pretty prints it. Used predominantly while debugging the code.
void traverse_blocks()
{
    char *curr_block = &mem[0];
    int ctr = 0;
    while (curr_block < &mem[MEM_SIZE]&& curr_block!=NULL) 
    {   
        ctr+=1;
        if(is_block_free(((block *)curr_block)))
        {   
            printf("%d:%d free.\n",ctr,curr_block,((block*)curr_block)->block_size);
            printf("*******\n");
        }
        else
        {
            printf("%d:%d in use.\n",ctr,curr_block,((block*)curr_block)->block_size);
            printf("*******\n");
        }
        
        curr_block = next(curr_block);
    }
}


// int main()
// {
// //    printf("Allocating\n");
//   int *ptr1 = (int*)malloc(sizeof(int));
// //   printf("%d",(int)ptr);
// //   printf("Deallocating\n");
// //   free(ptr);
//     printf("%d\n",(int)ptr1);

// //   printf("Allocating\n");
//    char *ptr2 = (char*)malloc(sizeof(char));
//     char *ptr3 = (char*)malloc(sizeof(char));
//     free(ptr1);
//     char *ptr4 = (char*)malloc(sizeof(char));
//     int *ptr5 = (int*)malloc(sizeof(int));
//   printf("%d\n",(int)ptr2);
//   printf("%d\n",(int)ptr3);
//   printf("%d\n",(int)ptr4);
//   printf("%d\n",(int)ptr5);
//   traverse_blocks();

// //   printf("Deallocating\n");
// //   free(ptr2);
// }
