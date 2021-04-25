#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.c"


int main()
{
    int Memsize = 0; // M - количество ячеек памяти
    int TaskNum = 0; // n - общее количество задач
    int t = 0; // t - время, на протяжении которого выполняется симуляция(в условных единицах)

    int* Memory = (int *)calloc(1, Memsize * sizeof(char));  // макет памяти, 1 единица памяти в нашей ОС - 1 char
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

                AllocTab (Memory, Memsize, AllocTableEmployed, AllocTableFree); // переформируем куски свободной и заянтой памяти

                //======================================================================================================
                // Проблемка: я из вне не знаю какой размер у массивов AllocTableEmployed, AllocTableFree,
                // поэтому не могу отсортировать их используя Sort
                // Можно как - то использовать локальные переменные FreeAreaNumber и EmployedAreaNumber из AllocTab
                // и, мб, загонять их в некий статичный массив из двух интов
                //======================================================================================================

                // GaySortAllocTable (AllocTableEmployed, ???) 
                // GaySortAllocTable (AllocTableFree, ???)
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
    
    int pid = 0;
    
    while(1)
    {
        
        scanf("%d", &pid);

        // функция, которая выводит на экран параметры интересующей задачи 
        task_status(pid, TaskNum, StructArray);
    }

    return 0;