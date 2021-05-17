#include "print.h"


//----------------------------------------------------------------------
// Функция, печатающая память, как массив
//----------------------------------------------------------------------
void fprintMemory (int* Memory, int size, FILE* output)
{
    int i = 0;
	fprintf(output, "\n[");                        // Скобочки для красоты восприятия
	for (i = 0; i < size - 1; i++)
	    fprintf(output, "%d, ", *(Memory + i) );


	fprintf(output, "%d]\n\n", *(Memory + i) );     // Скобочки для красоты восприятия
}

//----------------------------------------------------------------------
// Функция, печатающая куски памяти из Alloctable
//----------------------------------------------------------------------
void fprintAlloctable(AllocPart *Alloctable, int size, FILE* output)
{
    for (int i = 0; i < size; i++)
    {
        fprintf(output, "\nsize of %d part is %d\n",i, (Alloctable + i)->size );
    }
    fprintf(output, "\n");
}

void fprintStateAfterexecution (int* Memory, int Memsize, int time, unsigned int pid, AllocPart* AllocTableFree, int Amount_of_mem_parts, FILE* output)
{
    fprintf(output, "After execution of %u: time from start = %d, memory state\n", pid, time + 1);

    fprintMemory(Memory, Memsize, output);   // смотрим, правильно ли освободиласть память

    fprintf(output, "State of alloctablefree:\n");

    fprintAlloctable(AllocTableFree, Amount_of_mem_parts, output); // смотрим на массив свободных кусков, и можем сравнить его с состоянием памяти, напечатанным выше
}

void fprintfAfterSimulation (int* Memory, int Memsize, Task* StructArray, int TaskNum, FILE* output)
{
    fprintf(output, "\nMemory state after simulation:\n");

    fprintMemory(Memory, Memsize, output);

    fprintf(output, "Task status:\n");

    ftask_status(TaskNum, StructArray, output);
}

//----------------------------------------------------------------------
// Функция, печатающая память, как массив
//----------------------------------------------------------------------
void printMemory (int* Memory, int size)
{
    int i = 0;
	printf("\n[");                        // Скобочки для красоты восприятия
	for (i = 0; i < size - 1; i++)
	    printf("%d, ", *(Memory + i) );


	printf("%d]\n\n", *(Memory + i) );     // Скобочки для красоты восприятия
}

//----------------------------------------------------------------------
// Функция, печатающая куски памяти из Alloctable
//----------------------------------------------------------------------
void printAlloctable(AllocPart *Alloctable, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("size of %d part is %d\n",i, (Alloctable + i)->size );
    }
}