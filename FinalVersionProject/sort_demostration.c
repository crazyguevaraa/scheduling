#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "list.h"
#include "task.h"
#include "sort.h"
#include "alloctable.h"
#include "print.h"


int main()


{
    int Memory[][20] =  {
                        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1}, 
                        {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
					    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
					    {0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1}
                        };

    int Memsize[] = {10, 10, 20, 19};

    AllocPart* AllocTableEmployed;
    AllocPart* AllocTableFree;
    int amountMemParts[] = {0, 0};

    for (int i = 0; i < 4; i++)
    {
        printf("Proccessing memory example number %d:\n", i);
        printMemory(Memory[i], 20);

        AllocTableEmployed = create_AllocTableEmployed(Memsize[i]);
        AllocTableFree = create_AllocTableFree(Memory[i], Memsize[i]);

        
        amountMemParts[0] = 0;
        amountMemParts[1] = 1;
        

        
        AllocTab(Memory[i], Memsize[i], AllocTableEmployed, AllocTableFree, amountMemParts);
        printf("\nmem0 = %d mem1 = %d\n", amountMemParts[0], amountMemParts[1]);
        //printAlloctable(AllocTableEmployed, amountMemParts[0]);
        //printAlloctable(AllocTableFree, amountMemParts[1]);

        printMemory (Memory[i], Memsize[i]);

        printf("\n");
        printf("Before sorting:\n");
        printAlloctable(AllocTableFree, amountMemParts[1]);
        MemSortAllocTable(AllocTableFree, 0, amountMemParts[1] - 1);

        printf("\nAfter sorting:\n");
        printAlloctable(AllocTableFree, amountMemParts[1]);

        if ( (i - 3) )
            destroyBothAllocTables(AllocTableFree, AllocTableEmployed);
    }

    printf ("До этого сортировали только по памяти\nПосмотрим, правильно ли сортирует по адрессам сорт:\n");
    printf("возьмем последний массив в качетсве примера:");

    printMemory(Memory[3], Memsize[3]);

    printf("положим в начале каждого Allocpart-а 1:\n");
    for (int i = 0; i < amountMemParts[1]; i++)
    {
        * (AllocTableFree[i].point) = 1;
        printf("\n");
        printMemory(Memory[3], Memsize[3]);
    }

    printf("Уберем все 1 и отсортируем по адресам:\n");
    for (int i = 0; i < amountMemParts[1]; i++)
    {
        * (AllocTableFree[i].point) = 0;
        printf("\n");
        printMemory(Memory[3], Memsize[3]);
    }

    AddressSort(AllocTableFree, amountMemParts[1]);

    printf("Теперь опять положим в начале каждого Allocpart-а 1:\n");
    for (int i = 0; i < amountMemParts[1]; i++)
    {
        * (AllocTableFree[i].point) = 1;
        printf("\n");
        printMemory(Memory[3], Memsize[3]);
    }
    
}