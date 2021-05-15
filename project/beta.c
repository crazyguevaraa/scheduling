#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "print.h"
#include "list.h"
#include "task.h"
#include "sort.h"
#include "alloctable.h"

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

    int Amount_of_mem_parts [2] = { 0, 1 }; // 1 элемент - количество кусков занятой памяти, 2 - количество кусков свободной памяти

	Node* newnode_with_task = wait_list -> head;   // Создаем ноду с очередной задачей

	int Taskwaiting = TaskNum;                     // количество задач, не поступивших в список на исполнение

    int TaskPut = 0;                               // Количество задач, положенных в память и в список на выполнение в данной итерации цикла

    while(1)
    {
        if (wait_list -> head)                     // При первом прогоне это просто ничего не меняет        
        {                                          // Когда уже некоторые задачи были положены в to_do list, они удаляются из wait_list
            newnode_with_task = wait_list -> head; // поэтому newnode смотрит на нулевой указатель. Поэтому, переставляем его на первую ноду из 
        }                                          // wait_list    
            
        
        for (int i = 0; i < Taskwaiting; i++)       // Пробегаемся по листу wait_list и смотрим, можем ли оставшиеся задачи положить в память
        {                                           // и в to_do list
            for (int k = 0; k < Amount_of_mem_parts[1]; ) // будем обрабатывать задачи пока есть цельные куски свободной памяти потом
            {                                             // будем переформировать память

                if ( newnode_with_task -> task -> mem > Memsize )
                {
                    newnode_with_task -> task -> status = 5;
                    to_delete_a_task(newnode_with_task -> task, wait_list);

                    Taskwaiting--;

                    if (newnode_with_task -> next)
                        newnode_with_task = newnode_with_task -> next;
                }
                else
                {
                    if ( (newnode_with_task -> task -> mem) <= (AllocTableFree[k].size) )   // Смотрим, можно ли задачу положить в память,
                    {                                                                       // так как AllocTableFree впоследствии сортируется, достаточно посмотреть на первый его элемент
                        
                        Task * another_one = newnode_with_task -> task;                     // Если положить можно, то вытаскиваем из ноды указатель на задачу

                        to_add_to_execution (another_one, todo_list, wait_list);            // Добавляем в список на исполнение 

                        for (int j = 0; j < (another_one -> mem) ; j++)  // заполняем память данной задачей
                        {
                            AllocTableFree[k].point[j] = 1;
                        }

                        printf("Added task pid = %d to the memory:\n", another_one -> pid);

                        printMemory(Memory, Memsize);                                       // Печатаем память после добавления
                            
                        another_one->taken_mem = AllocTableFree[k].point; // записываем в структуру задачи, куда именно в памяти она была записана, чтобы впоследствии освободить её


                        another_one -> status = 1;                      // записываем, что положили задачу в список на исполнение

                        //printAlloctable(AllocTableFree, Amount_of_mem_parts[1]);
                        
                        

                        //printAlloctable(AllocTableFree, Amount_of_mem_parts[1]);

                        TaskPut += 1; // мы положили одну задачу в список на исполнение - запишем это
                        
                        
                    
                        k++; //если удалось положить задачу, то 

                        //printf("\nk = %d, amountparts = %d\n", k, Amount_of_mem_parts[1]);
                        
                    }
                    else
                    {
                        if (!(newnode_with_task -> next))
                            break;
                    }

                    if (newnode_with_task -> next)  // если есть еще задачи, сразу внутри данного цикла попробуем их положить в память
                            {
                                newnode_with_task = newnode_with_task -> next;
                                //printf("pid of new task = %d\n", newnode_with_task->task->pid);
                            }
                }
                
                
            }

            processMemory (Memory, Memsize, AllocTableEmployed, AllocTableFree, Amount_of_mem_parts); // переформируем куски свободной и заянтой памяти
        }

        Taskwaiting -= TaskPut; // количество задач, ожидающих того, что их положут в очередь на исполнение, уменьшилось на TaskPut

        TaskPut = 0;            // обнулим количство задач, положенных в цикле


        

        if (todo_list -> head)
            execution (todo_list -> head -> task, todo_list, &time); // исполняем одну задачу

        //printf("Executed\n");

        printf("After execution:\n");

        printMemory(Memory, Memsize);   // смотрим, освободиласть ли память

        processMemory (Memory, Memsize, AllocTableEmployed, AllocTableFree, Amount_of_mem_parts); // переформируем куски свободной и заянтой памяти

        printf("State of alloctablefree:\n");

        printAlloctable(AllocTableFree, Amount_of_mem_parts[1]); // смотрим на массив свободных кусков, и можем сравнить его с состоянием памяти, напечатанным выше


        // прерывание цикла если истекло время, либо если все задачи выполнены
        if(time <= 0)
        {
            printf("time expired");
            break;
        }
            

        if(wait_list -> head == 0 && wait_list -> tail == 0 && todo_list -> head ==  0 && todo_list -> tail == 0)
        {
            printf("All tasks done");
            break;
        }
            

    }




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