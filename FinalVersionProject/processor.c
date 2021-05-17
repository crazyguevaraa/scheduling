#include "processor.h"

int processor(int* Memory, int Memsize, int TaskNum, int time, AllocPart* AllocTableEmployed, AllocPart* AllocTableFree, List * wait_list, List * todo_list, FILE* output)
{
    

    int timefromstart = 0; // время с начала симуляции
    int pid_of_executed = 0;   // флаг, что закончилось исполнение очередной задачи


    Node* newnode_with_task = wait_list -> head;   // Создаем ноду с очередной задачей

    int Taskwaiting = TaskNum;                     // количество задач, не поступивших в список на исполнение

    int TaskPut = 0;                               // Количество задач, положенных в память и в список на выполнение в данной итерации цикла

    int Amount_of_mem_parts [2] = { 0, 1 }; // 1 элемент - количество кусков занятой памяти, 2 - количество кусков свободной памяти

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
                        fprintf (output, "\nTask with pid %d declined: takes too much memory\n", newnode_with_task -> task -> pid);
                        newnode_with_task -> task -> status = 5;      // если нет - выставляем статус - отклонена
                        to_delete_a_task(newnode_with_task -> task, wait_list); // удаляем ее из списка на ожидания

                        Taskwaiting--;     // тогда, задач в списке ожидания на 1 меньше

                        if (newnode_with_task -> next)      // выставляем следующую задачу, если она есть, на обработку
                            newnode_with_task = newnode_with_task -> next;
                    }
                    else
                    {
                        if ( (newnode_with_task -> task -> mem) <= (AllocTableFree[counter].size) )   // Смотрим, можно ли задачу положить в память,
                        {                                                                       // так как AllocTableFree впоследствии сортируется, достаточно посмотреть на первый его элемент
                            
                            Task * another_one = newnode_with_task -> task;                     // Если положить можно, то вытаскиваем из ноды указатель на задачу

                            to_add_to_execution (another_one, todo_list, wait_list, AllocTableFree, counter);            // Добавляем в список на исполнение 

                            fprintf(output, "Added task pid = %d to the memory, time from start = %d, memory state:\n", another_one -> pid, timefromstart); 

                            fprintMemory(Memory, Memsize, output);                                       // Печатаем память после добавления задачи

                            counter++; // увеличим счетчик добавленных зада
                                
                            TaskPut++; // мы положили одну задачу в список на исполнение, поэтому свободных кусков в Alloctable уменьшилось на 1
                            
                        }
                        else
                        {
                            i++; // Если не удалось положить данную задачу, но в теории она помещается, переключаемся дальше

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

                if (timefromstart < newnode_with_task -> task -> time_wait) // Если с начала симуляции прошло меньше времени, чем когда 
                        break;                                              // должна быть загружена следующая задача, то мы её и последующие не грузим,

                if ( !Amount_of_mem_parts[1] )
                {
                    break;
                }
            }

            Taskwaiting -= TaskPut; // мы положили одну задачу в список на исполнение, поэтому ожидает на 1 меньше

            TaskPut = 0;            // обнулим количство задач, положенных в цикле


            if (todo_list -> head)
                pid_of_executed = execution (todo_list -> head -> task, todo_list, &time); // исполняем одну задачу


            if ( pid_of_executed ) // Если исполнили некую задачу
            {
                processMemory (Memory, Memsize, AllocTableEmployed, AllocTableFree, Amount_of_mem_parts); // переформируем куски свободной и заянтой памяти

                fprintStateAfterexecution (Memory, Memsize, timefromstart, pid_of_executed, AllocTableFree, Amount_of_mem_parts[1], output); //Печать состояния
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

    return 0;
}
