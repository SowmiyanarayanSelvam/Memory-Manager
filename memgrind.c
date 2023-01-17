// Hittishi Kurlagunda					[Net ID: hk919]
//Sowmiyanarayan Selvam					[Net ID: ss4370]

#include <stdio.h>
#include <time.h>
#include "umalloc.h"

int * prev_ptr [MEM_SIZE];
int len = 0; 

void consistency()
{
 // 0.Consistency 
    printf("FIRST\n");
    void *p = (void *) malloc(sizeof(char));
    printf("%d\n",p);
    char *temp = (char*) p;
    temp = 'a';
    free(p);
    traverse_blocks();
    printf("SECOND\n");
    void *p2 = (void *) malloc(sizeof(char));
    printf("%d\n",p2);
    char *temp2 = (char*) p2;
    temp2 = 'b';
    free(p2);
    traverse_blocks();
}

int maximisation()
{
 //1.maximisation
    void *p;
    int ctr = 1;
    int max_alloc = 0;
    while(1)
    {
        void *p = (void *)malloc(ctr * sizeof(char));
        ctr*=2;
        if(p == NULL)
        {
            ctr/=4;
            printf("%d\n",ctr);
            void *p = (void *)malloc(ctr * sizeof(char));
            break;
        }
        free(p);
        printf("%d,%d - %d\n",ctr,p,p + ctr * sizeof(char));
    }
    max_alloc = ctr;
    // printf("midpoint\n");
    // printf("mid_max:%d\n",max_alloc);
    ctr/=2;
    while(1)
    {
        void *p2 = (void *)malloc(ctr * sizeof(char));
        if(p2 == NULL)
        {
            //  printf("%d,%d\n",ctr,p);
            ctr/=2;
        }
        else if(p2 != NULL||ctr==1)
        {
            free(p2);
            max_alloc+=ctr;
            break;
        }
    }
    free(p);
    printf("max:%d\n",max_alloc);
    return max_alloc;
}

void basic_coalescence(int alloc)
{
//2.basic coalescence
printf("%d\n",alloc);
void *arr2 = (void*)malloc(alloc/2 * sizeof(char));
printf("1/2:%d - %d\n",arr2,arr2 + alloc/2 * sizeof(char));
void *arr4 = (void*)malloc(alloc/4 * sizeof(char));
printf("1/4:%d - %d\n",arr4,arr4 + alloc/4 * sizeof(char));
free(arr2);
free(arr4);
// printf("-:%d\n",arr2+ alloc);
void *arr = (void*)malloc(alloc * sizeof(char));
printf("1:%d - %d\n",arr,arr + alloc * sizeof(char));
free(arr);
}

void* saturation()
{
   //3.saturation
//    void *arr = (void *)malloc(3686 * sizeof(char));
//    int ctr = 3686;
  
   void *arr = (void *)malloc(9216 * 1024 * sizeof(char));
   prev_ptr[len] = arr;
   len+=1;
   void *p,*toret;
   int ctr = 9216 * 1024;
   while(1)
   {
    void *p = (void *)malloc(sizeof(char));
    
    if(p == NULL)
    {
        // printf("last:%d\n", p);
        break;
    }
    toret = p;
    prev_ptr[len] = p;
    len+=1;
    ctr+=1;
   }
   printf("saturation:%d\n", ctr);
   printf("last:%d\n", toret);
   return toret;
}

void* time_overload(void *arr)
{
    free(arr);
    clock_t begin = clock();
    arr = malloc(sizeof(char));
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Maximum time overhead:%f\n", time_spent);
    return arr;
}

void intermediate_coalescence(int alloc)
{
    // printf("%d\n",&mem[0]);
    // free(arr);
    // free(arr);
    // printf("%d\n",arr);
     while(len>=0)
    {
        free(prev_ptr[len]);
        len-=1;
    }
    traverse_blocks();
    // printf()
    void *p = (void*)malloc(alloc * sizeof(char));
    printf("1:%d - %d\n",p,p + alloc * sizeof(char));
    free(p);
}

int main()
{
    // printf("0.Consistency\n");
    consistency();
    // printf("\n");
    int c = maximisation();
    // printf("%d\n", c);
    // traverse_blocks();
    basic_coalescence(c);
    // traverse_blocks();
    void *arr;
    // saturation();
    arr = saturation();
    // printf("%d\n", arr);
    arr = time_overload(arr);
    // printf("%d\n", arr);
    intermediate_coalescence(c);
    printf("Finished");
}