#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "list.h"
#include "task.h"
#include "print.h"
#include "sort.h"
#include "alloctable.h"
#include "processor.h"


int main(int argc, char** argv)
{
    int Memsize = 0; // M - количество ячеек памяти
    int TaskNum = 0; // n - общее количество задач
    int time = 0; // t - время, на протяжении которого выполняется симуляция(в условных единицах)

    FILE* input = fopen(argv[1], "r");


    fscanf(input, "%d %d %d", &Memsize, &TaskNum, &time);

	int* Memory = (int *)calloc(1, Memsize * sizeof(int));  // макет памяти, 1 единица памяти в нашей ОС - 1 int

    Task * StructArray = EnterTask(input, TaskNum);               // создаем массив задач

    fclose(input);

	TaskSortAllocTable(StructArray, 0, TaskNum - 1);        // сортируем его по времени поступления в Процессор (т. е. по параметру time_wait)

    // функция создает лист на ожидание, заполняет его задачами и возвращает его
    List * wait_list = wait_list_constructor(TaskNum, StructArray, Memsize);

    // функция создает лист на выполнение и возвращает его
    List * todo_list = createList();

    // создаем массивы указателей на соответсвенно занятые и свободные куски памяти
    AllocPart* AllocTableEmployed = create_AllocTableEmployed(Memsize);
    AllocPart* AllocTableFree = create_AllocTableFree(Memory, Memsize);

    //открываем файл на запись, если был дан, то открываем его, если нет, то создаем output.dat
    FILE* output;
    if (argc > 2)
        output = fopen(argv[2], "w");
    else
        output = fopen("output.dat", "w");

    // главная часть программы
    int flag = processor(Memory, Memsize, TaskNum, time, AllocTableEmployed, AllocTableFree, wait_list, todo_list, output);

    fprintfAfterSimulation(Memory, Memsize, StructArray, TaskNum, output); // печатаем состояние памяти и задач после симуляции

    if( !flag )
        printf("\nEverything is alright, output written in file you gave, or in output.dat if you did not give a file for output\n");

    fclose(output);

    // функции полностью удаляют оба списка и таблицы и освобождают память
    destroyList(wait_list);
    destroyList(todo_list);
    destroyBothAllocTables(AllocTableFree, AllocTableEmployed);
    free(Memory);
    free(StructArray);

    return 0;
}