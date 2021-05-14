#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "print.h"
#include "list.h"
#include "task.h"
#include "alloctable.h"
#include "sort.h"




int main()
{
    int Memsize = 0; // M - количество ячеек памяти
    int TaskNum = 0; // n - общее количество задач
    int t = 0; // t - время, на протяжении которого выполняется симуляция(в условных единицах)

    scanf("%d %d %d", &Memsize, &TaskNum, &t);

	int* Memory = (int *)calloc(1, Memsize * sizeof(int));  // макет памяти, 1 единица памяти в нашей ОС - 1 int

    Task * StructArray = EnterTask(TaskNum);

	printMemory(Memory, Memsize);

	TaskSortAllocTable(StructArray, 0, TaskNum - 1);

    // функция создает лист на ожидание, пихает все задачи в него и возвращает его
    List * wait_list = wait_list_constructor(TaskNum, StructArray, Memsize, t);

    // функция создает лист на выполнение и возвращает его
    List * todo_list = createList();

    // создаем массивы указателей на соответсвенно занятые и свободные куски памяти
    AllocPart* AllocTableEmployed = create_AllocTableEmployed(Memsize);
    AllocPart* AllocTableFree = create_AllocTableFree(Memory, Memsize);

    int Amount_of_mem_parts [2] = { 0 }; // 1 элемент - количество кусков занятой памяти, 2 - количество кусков свободной памяти

	Node* new = wait_list -> head;

	int Taskleft = TaskNum;

    while(1)
    {
		int i = 0;

        
        for (i = 0; i < Taskleft; i++)
        {
			Task * another_one = new -> task;
            if ( (another_one -> mem) < (AllocTableFree -> size) )
            {
	
                to_add_to_execution (another_one, todo_list, wait_list);

				printMemory(Memory, Memsize);

				printf("mem of another one = %d\n", another_one -> mem);

				for (int j = 0; j < (another_one -> mem) ; j++)
				{
					//printf("\nin cycle\n");
					AllocTableFree[0].point[j] = 1;
					//printf("adding a bit\n");
				}
                	
				another_one->taken_mem = AllocTableFree->point;


                another_one -> status = 1;

				printAlloctable(AllocTableFree, Amount_of_mem_parts[1]);
				
                AllocTab (Memory, Memsize, AllocTableEmployed, AllocTableFree, Amount_of_mem_parts); // переформируем куски свободной и заянтой памяти

				printAlloctable(AllocTableFree, Amount_of_mem_parts[1]);

                GaySortAllocTable (AllocTableEmployed, 0, Amount_of_mem_parts[0]);  //пересортируем куски
                GaySortAllocTable (AllocTableFree, 0, Amount_of_mem_parts[1]);      //занятой и свободной памятей
            }

			if (new -> next)
				new = new -> next;
        }

		Taskleft -= i;


        // исполняем одну задачу
        execution (todo_list -> head -> task, todo_list, &t);

		printMemory(Memory, Memsize);

        // удаляем выполненную задачу из ОП и листа на исполнения
        //to_delete_a_task (todo_list -> head -> task, todo_list);



        // прерывание цикла если истекло время, либо если все задачи выполнены
        if(t <= 0)
            break;

        if(wait_list -> head == 0 && wait_list -> tail == 0 && todo_list -> head ==  0 && todo_list -> tail == 0)
            break;
    }



    int pid = 0;

    while(1)
    {
		printf("\nВведите pid интересующей вас задачи или 0 в качестве отказа\n");
        scanf("%d", &pid);

        if(pid == 0)
            break;

        // функция, которая выводит на экран параметры интересующей задачи
        task_status(pid, TaskNum, StructArray);
    }

    // функции полностью удаляют оба списка и таблицы
    destroyList(wait_list);
    destroyList(todo_list);
    destroyBothAllocTables(AllocTableFree, AllocTableEmployed);

    return 0;
}