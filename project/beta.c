#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "print.h"
#include "list.h"
#include "task.h"
#include "sort.h"
#include "alloctable.h"


int main(int argc, char** argv)
{
    int Memsize = 0; // M - количество ячеек памяти
    int TaskNum = 0; // n - общее количество задач
    int time = 0; // t - время, на протяжении которого выполняется симуляция(в условных единицах)
    int timefromstart = 0;
    int if_executed = 0;

    FILE* input = fopen(argv[1], "r");


    fscanf(input, "%d %d %d", &Memsize, &TaskNum, &time);

	int* Memory = (int *)calloc(1, Memsize * sizeof(int));  // макет памяти, 1 единица памяти в нашей ОС - 1 int

    Task * StructArray = EnterTask(input, TaskNum);                // создаем массив задач

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

    int Amount_of_mem_parts [2] = { 0, 1 }; // 1 элемент - количество кусков занятой памяти, 2 - количество кусков свободной памяти

	Node* newnode_with_task = wait_list -> head;   // Создаем ноду с очередной задачей

	int Taskwaiting = TaskNum;                     // количество задач, не поступивших в список на исполнение

    int TaskPut = 0;                               // Количество задач, положенных в память и в список на выполнение в данной итерации цикла

    int counter = 0;

    while(1)
        {
            fprintf(output, "Starting new tick:\n\n");

            if (wait_list -> head)                     // При первом прогоне это просто ничего не меняет        
            {                                          // Когда уже некоторые задачи были положены в to_do list, они удаляются из wait_list
                newnode_with_task = wait_list -> head; // поэтому newnode смотрит на нулевой указатель. Поэтому, переставляем его на первую ноду из 
            }                                          // wait_list

            for (int i = 0; i < Taskwaiting; )       // Пробегаемся по листу wait_list и смотрим, можем ли оставшиеся задачи положить в память
            {                                           // и в to_do list
                for (counter = 0; (counter < Amount_of_mem_parts[1]) && (counter < Taskwaiting); ) // Будем обрабатывать задачи пока есть цельные куски свободной памяти 
                {                                                                            // потом будем переформировать память
                    if (timefromstart < newnode_with_task -> task -> time_wait) // Если с начала симуляции прошло меньше времени, чем когда 
                        break;                                                  // должна быть загружена задача, то мы её и последующие не грузим,
                                                    

                    if ( newnode_with_task -> task -> mem > Memsize ) // проверка может ли в целом задача поместиться в память
                    {
                        newnode_with_task -> task -> status = 5;      // если нет - выставляем статус - отклонена
                        to_delete_a_task(newnode_with_task -> task, wait_list); // удаляем ее из списка на ожидания

                        TaskPut++;     // тогда, задач в списке ожидания на 1 меньше

                        if (newnode_with_task -> next)      // выставляем следующую задачу, если она есть, на обработку
                            newnode_with_task = newnode_with_task -> next;
                    }
                    else
                    {
                        if ( (newnode_with_task -> task -> mem) <= (AllocTableFree[counter].size) )   // Смотрим, можно ли задачу положить в память,
                        {                                                                       // так как AllocTableFree впоследствии сортируется, достаточно посмотреть на первый его элемент
                            
                            Task * another_one = newnode_with_task -> task;                     // Если положить можно, то вытаскиваем из ноды указатель на задачу

                            to_add_to_execution (another_one, todo_list, wait_list, AllocTableFree, counter);            // Добавляем в список на исполнение 

                            fprintf(output, "Added task pid = %d to the memory:\n", another_one -> pid); 

                            fprintMemory(Memory, Memsize, output);                                       // Печатаем память после добавления задачи

                            counter++; // увеличим счетчик добавленных зада
                                
                            TaskPut++; // мы положили одну задачу в список на исполнение, поэтому свободных кусков в Alloctable уменьшилось на 1
                            
                        }
                        else
                        {
                            if (!(newnode_with_task -> next))   // Если нет задач, после той, которая пока не помещается в память, идем на исполнение задач
                                break;
                        }

                        if (newnode_with_task -> next)  // если есть еще задачи, сразу внутри данного цикла попробуем их положить в память
                                {
                                    newnode_with_task = newnode_with_task -> next;
                                }
                    }
                    
                    
                }
                
                if (counter) // Если что - то положили в память то переформируем куски свободной и заянтой памяти
                {
                    processMemory (Memory, Memsize, AllocTableEmployed, AllocTableFree, Amount_of_mem_parts);
                    i += counter;       // Также сдвинем счетчик на количество задач, которые выполнили 
                }
                else
                {
                    i++;    //Если не смогли добавить, то сдвинем счетчик просто на 1
                }

                if (timefromstart < newnode_with_task -> task -> time_wait) // Если с начала симуляции прошло меньше времени, чем когда 
                        break;                                              // должна быть загружена следующая задача, то мы её и последующие не грузим,
            }

            Taskwaiting -= TaskPut; // мы положили одну задачу в список на исполнение, поэтому ожидает на 1 меньше

            TaskPut = 0;            // обнулим количство задач, положенных в цикле


            if (todo_list -> head)
                if_executed = execution (todo_list -> head -> task, todo_list, &time, &timefromstart); // исполняем одну задачу


            if (if_executed) // Если исполнили некую задачу
            {
                processMemory (Memory, Memsize, AllocTableEmployed, AllocTableFree, Amount_of_mem_parts); // переформируем куски свободной и заянтой памяти

                fprintStateAfterexecution (Memory, Memsize, AllocTableFree, Amount_of_mem_parts[1], output); //Печать состояния
            }
            


            timefromstart++;
            time--;


            // прерывание цикла если истекло время, либо если все задачи выполнены
            if(time <= 0)
            {
                fprintf(output, "\nTime expired\n");
                break;
            }
                

            if(wait_list -> head == 0 && wait_list -> tail == 0 && todo_list -> head ==  0 && todo_list -> tail == 0)
            {
                fprintf(output, "\nAll tasks done\n");
                break;
            }

            fprintf(output, "Another tick passed: timeleft = %d, time from start = %d, Memory state:\n", time, timefromstart); // Печать состояния после очередного тика
            fprintMemory(Memory, Memsize, output);

            
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