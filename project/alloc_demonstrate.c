#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "list.h"
#include "task.h"
#include "print.h"
#include "sort.h"
#include "alloctable.h"





int main()


{
    int Memoryarray[3][20] =    {
                                {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1}, 
                                {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0}, 
                                {0, 0, 0, 0, 0, 1, 1, 1, 1, 1}
                                };

    int Memorysize [] = {20, 10, 10};

    int MemAmount = 3;

    int amountMemParts[] = {0, 0};

    for (int i = 0; i < MemAmount ; i++)
    {
        printf("Processing memory example number %d:\n", i);
        printMemory(Memoryarray[i], Memorysize[i]);

        AllocPart* AllocTableEmployed = create_AllocTableEmployed(Memorysize[i]);
        AllocPart* AllocTableFree = create_AllocTableFree(Memoryarray[i], Memorysize[i]);

        amountMemParts[0] = 0;
        amountMemParts[1] = 1;
        
        AllocTab(Memoryarray[i], Memorysize[i], AllocTableEmployed, AllocTableFree, amountMemParts);
        printf("\nmem parts taken = %d mem parts free = %d\n", amountMemParts[0], amountMemParts[1]);

        printf("AllocTableEmployed:\n");
        printAlloctable(AllocTableEmployed, amountMemParts[0]);

        printf("\nAllocTableFree:\n");
        printAlloctable(AllocTableFree, amountMemParts[1]);

        free (AllocTableEmployed);
        free (AllocTableFree);
        printf("\n");
    }

    printf("want to try out for yourself? type 1 if yes: ");

    int flag = 0;

    scanf("%d", &flag);

    while(flag)
    {
        int Memsize = 0;
        printf("Give memory size:");
        scanf("%d", &Memsize);

        int* Memory = (int*)calloc(1, Memsize * sizeof(int));

        printf ("just print zeros and one which are in memory:\n");

        for (int i = 0; i < Memsize; i++)
        {
            scanf ("%d", Memory + i);
        }

        printf("The memory you entered:\n");
        printMemory(Memory, Memsize);

        AllocPart* AllocTableEmployed = create_AllocTableEmployed(Memsize);
        AllocPart* AllocTableFree = create_AllocTableFree(Memory, Memsize);

        amountMemParts[0] = 0;
        amountMemParts[1] = 1;

        
        AllocTab(Memory, Memsize, AllocTableEmployed, AllocTableFree, amountMemParts);
        printf("\nmem parts taken = %d mem parts free = %d\n", amountMemParts[0], amountMemParts[1]);

        printf("AllocTableEmployed:\n");
        printAlloctable(AllocTableEmployed, amountMemParts[0]);

        printf("\nAllocTableFree:\n");
        printAlloctable(AllocTableFree, amountMemParts[1]);

        free (AllocTableEmployed);
        free (AllocTableFree);

        printf("Want to try more? print 1 if yes: ");
        scanf("%d", &flag);
    }
}