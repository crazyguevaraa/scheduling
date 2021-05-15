#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "print.h"
#include "list.h"
#include "task.h"
#include "sort.h"
#include "alloctable.h"
#include "processor.h"


int main()
{
    int Memsize = 0; // M - количество ячеек памяти
    int TaskNum = 0; // n - общее количество задач
    int time = 0; // t - время, на протяжении которого выполняется симуляция(в условных единицах)

    scanf("%d %d %d", &Memsize, &TaskNum, &time);

	int* Memory = (int *)calloc(1, Memsize * sizeof(int));  // макет памяти, 1 единица памяти в нашей ОС - 1 int

    Task * StructArray = EnterTask(TaskNum);                // создаем массив задач

	TaskSortAllocTable(StructArray, 0, TaskNum - 1);        // сортируем его по времени поступления в Процессор (т. е. по параметру time_wait)

    // функция создает лист на ожидание, заполняет его задачами и возвращает его
    List * wait_list = wait_list_constructor(TaskNum, StructArray, Memsize);

    // функция создает лист на выполнение и возвращает его
    List * todo_list = createList();

    // создаем массивы указателей на соответсвенно занятые и свободные куски памяти
    AllocPart* AllocTableEmployed = create_AllocTableEmployed(Memsize);
    AllocPart* AllocTableFree = create_AllocTableFree(Memory, Memsize);


    // главная часть программы
    processor(Memory, Memsize, TaskNum, time, AllocTableEmployed, AllocTableFree, wait_list, todo_list);

    printf("\nMemory state after simulation:\n");

    printMemory(Memory, Memsize);

    printf("Task status:\n");

    task_status(TaskNum, StructArray);


    // функции полностью удаляют оба списка и таблицы и освобождают память
    destroyList(wait_list);
    destroyList(todo_list);
    destroyBothAllocTables(AllocTableFree, AllocTableEmployed);
    free(Memory);
    free(StructArray);

    return 0;
}