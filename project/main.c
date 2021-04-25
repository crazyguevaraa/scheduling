#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.c"


int main()
{
    int Memsize = 0; // M - количество ячеек памяти
    int TaskNum = 0; // n - общее количество задач
    int t = 0; // t - время, на протяжении которого выполняется симуляция(в условных единицах)

    int* Memory = (int *)calloc(1, Memsize * sizeof(char));  // макет памяти, 1 единица памяти в нашей ОС - 1 int
    scanf("%d%d%d", &Memsize, &TaskNum, &t);
    
    Task * StructArray = EnterTask(TaskNum);   

    int RAM[TaskNum] = {0};
    
    // функция создает лист на ожидание, пихает все задачи в него и возвращает его
    List * wait_list = wait_list_constructor(TaskNum, StructArray, Memsize, t);

    // функция создает лист на выполнение и возвращает его
    List * todo_list = createList();

    // создаем массивы указателей на соответсвенно занятые и свободные куски памяти
    AllocPart* AllocTableEmployed = create_AllocTableEmployed(Memsize);
    AllocPart* AllocTableFree = create_AllocTableFree(Memory, Memsize);

    int Amount_of_mem_parts [2] = { 0 }; // 1 элемент - количество кусков занятой памяти, 2 - количество кусков свободной памяти

    while(1)
    {
        Task * another_one = wait_list -> head;
        for (int i = 0; i < TaskNum; i++)
        {
            if (another_one -> mem > AllocTableFree -> size)
            {
                to_add_to_execution (another_one, todo_list, wait_list);
                *(AllocTableFree -> point) = 1;
                another_one -> status = 1;

                AllocTab (Memory, Memsize, AllocTableEmployed, AllocTableFree, Amount_of_mem_parts); // переформируем куски свободной и заянтой памяти

                GaySortAllocTable (AllocTableEmployed, Amount_of_memory_chunks[0]); //пересортируем куски
                GaySortAllocTable (AllocTableFree, Amount_of_memory_chunks[1]);     // занятой и свободной памятей
            }
        }

        // исполняем одну задачу 
        execution (todo_list -> head -> task, todo_list, &t);

        // удаляем выполненную задачу из ОП и листа на исполнения
        to_delete_a_task (todo_list -> head -> task, todo_list);
        
        // прерывание цикла если истекло время, либо если все задачи выполнены
        if(t <= 0)
            break;

        if(todo_list -> head == 0 && todo_list -> tail == 0)
            break;   
    }
    
    // функции полностью удаляет оба списка
    destroyList(wait_list);
    destroyList(todo_list);
    destroyBothAllocTables(AllocTableFree, AllocTableEmployed);
    
    int pid = 0;
    
    while(1)
    {
        
        scanf("%d", &pid);

        // функция, которая выводит на экран параметры интересующей задачи 
        task_status(pid, TaskNum, StructArray);
    }

    return 0;