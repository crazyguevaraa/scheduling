#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.c"


int main()
{
    int M, n, t = 0; // M - количество ячеек памяти, n - общее количество задач, t - время, на протяжении которого выполняется симуляция(в условных единицах)
   
    scanf("%d%d%d", &M, &n, &t);
    
    Task * StructArray = EnterTask(n);   

    int RAM[n] = {0};
    
    // функция создает лист на ожидание, пихает все задачи в него и возвращает его
    wait_list = wait_list_constructor(n, StructArray);

    // функция создает лист на выполнение и возвращает его
    todo_list = List *createList();
    
    while(1)
    {
        // проверяем задачи на возможность впихнуть их в лист на исполнение,
        // если пихаем задачу в ОП, то удаляем из листа на ожидание, и добавляем в лист на исполнение

        // исполняем одну задачу 
        execution (todo_list -> head -> task, todo_list, &t);

        // удаляем выполненную задачу из ОП и листа на исполнения
        void to_delete_a_task (todo_list -> head -> task, todo_list);
        
        // прерывание цикла если истекло время, либо если все задачи выполнены
        if(t <= 0)
            break;

        if(todo_list -> head = 0 && todo_list -> tail = 0)
            break;   
    }
    
    // функция полностью удаляет оба списка
    
    int pid = 0;
    
    while(1)
    {
        
        scanf("%d", &pid);

        // функция, которая выводит на экран параметры интересующей задачи 
        void task_status(int pid, int n, Task * StructArray);
    }

    return 0;
}
