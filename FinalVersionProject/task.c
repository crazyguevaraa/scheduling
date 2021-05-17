#include "task.h"

//--------------------------------------------------------------
// Создает лист на ожидание
// Сразу выставляет статус 0 задачам, априоре невыполнимым
//--------------------------------------------------------------

List * wait_list_constructor(int n, Task * StructArray, int Memsize)
{
    List * tmp = createList();

    for(int i = 0; i < n; i++)
    {
        to_add_to_queue(StructArray + i, tmp);
    }

    return tmp;
}


//--------------------------------------------------------------
//отправка задачи в список очереди
//--------------------------------------------------------------
void to_add_to_queue (Task* newone, List* queue)
{
	insertL(queue, newone);
	newone -> status = 1;    // попала в очередь на ожидание
}

//--------------------------------------------------------------
//удаление задачи из списка
//--------------------------------------------------------------
void to_delete_a_task (Task* to_delete, List* lst)
{
	Node * Node_to_delete;
	Node * p = lst -> head;

	while(1)
	{
        if(p -> task == to_delete)
		{
			Node_to_delete = p;
			break;
		}
		else
		{
			if (p != lst -> tail)
			    p = p -> next;
			else
			    break;
		}
	}


	deleteL(lst, Node_to_delete);
}

//--------------------------------------------------------------
// отправка задачи в список на выполнение
//--------------------------------------------------------------
void to_add_to_execution (Task* to_execute, List* lst, List* queue, AllocPart* AllocTableFree, int AlloctabPlacement)
{
    insertL(lst, to_execute);
    to_execute -> status = 1;    // попала в очередь исполнение
	for (int j = 0; j < (to_execute -> mem) ; j++)  // заполняем память данной задачей
		{
			AllocTableFree[AlloctabPlacement].point[j] = 1;
		}
	to_execute -> taken_mem = AllocTableFree[AlloctabPlacement].point; // записываем в структуру задачи, куда именно в памяти она была записана, чтобы впоследствии освободить её
    to_delete_a_task(to_execute, queue);
}

//--------------------------------------------------------------
//выполнение задачи
//--------------------------------------------------------------

unsigned int execution (Task* to_do, List* execution, int *time)
{
	unsigned int pid = 0;
	if ( (to_do -> time_act) > 1 )						// Если до выполнения задачи осталось меньше секунды, то она не будет выполнена за этот тик процессора
	{
		(to_do -> time_act) = (to_do -> time_act) - 1;  // Задача исполнялась еще один тик

		if (*time - 1 == 0)								// Если это последний тик процессора в симуляции
		{
			to_do -> status = 3;						// То выставляем статус задачи - в процессе исполнения
			to_delete_a_task(to_do, execution);			// Удаляем ее из списка на исполнение
		}

		return 0;
	}
	else							    			// Если задачу можно сделать за этот тик
	{
			to_do -> status = 4;					// то выставляем ей статус выполнена
			pid = to_do -> pid;
			
			for (int i = 0; i < to_do->mem; i++)	// Освобождаем память, занятой задачей
				(to_do -> taken_mem)[i] = 0;
			to_delete_a_task(to_do, execution);		// Удаляем ее из списка на исполнение
			
			return pid;
	}
        


    
}

//--------------------------------------------------------------
// ввод задачи
// Принимает на вход количество задач, возвращает массив структур Task
//--------------------------------------------------------------

Task * EnterTask (FILE* input, int n)
{
	Task * structArray = (Task * )calloc(1, n * sizeof(Task));

    for(int i = 0; i < n; i++)
    {
        fscanf(input, "%d %d %d %d", &structArray[i].pid, &structArray[i].mem, &structArray[i].time_act, &structArray[i].time_wait);
    }

    return structArray;
}

//--------------------------------------------------------------
//статус задачи
//--------------------------------------------------------------

void ftask_status(int n, Task * StructArray, FILE* output)
{
	Task * toshow = 0;

    for(int i = 0; i < n; i++)
	{

        toshow = &StructArray[i];

		fprintf(output, "Статус задачи с номером = %d: ", toshow -> pid);
    
		switch ( toshow -> status )
		{
		case 0:
			fprintf(output, "в очереди ожидания\n");
			break;
		case 1:
			fprintf(output, "в очереди на исполнение\n");
			break;

		case 2:
			fprintf(output, "поступила на исполнение\n");
			break;

		case 3:
			fprintf(output, "исполняется, %d у.е. времени осталось\n", StructArray[i].time_act);
			break;

		case 4:
			fprintf(output, "выполнена\n");
			break;

		case 5:
			fprintf(output, "отклонена\n");
			break;

		default:
			break;
		}
	}
}