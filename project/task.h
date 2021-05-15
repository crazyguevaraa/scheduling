
void to_add_to_queue (Task* newone, List* queue);
void to_delete_a_task (Task* to_delete, List* lst);
void to_add_to_execution (Task* to_execute, List* lst, List* queue);
void execution (Task* to_do, List* execution, int *time);
Task * EnterTask (int n);
void task_status(int n, Task * StructArray);
List * wait_list_constructor(int n, Task * StructArray, int Memsize);



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
};


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
};

//--------------------------------------------------------------
// отправка задачи в список на выполнение
//--------------------------------------------------------------
void to_add_to_execution (Task* to_execute, List* lst, List* queue)
{
    insertL(lst, to_execute);
    to_execute -> status = 2;    // попала в очередь исполнение
    to_delete_a_task(to_execute, queue);
};

//--------------------------------------------------------------
//выполнение задачи
//--------------------------------------------------------------

void execution (Task* to_do, List* execution, int *time)
{
	*time -= to_do -> time_act;

    if(time >= 0)
	{
        to_do -> status = 4;
		for (int i = 0; i < to_do->mem; i++)
			(to_do -> taken_mem)[i] = 0;
	}
    else
    {
        to_do -> status = 3;
        to_do -> time_act = 0 - *time;
    }

    to_delete_a_task(to_do, execution);
};

//--------------------------------------------------------------
// ввод задачи
// Принимает на вход количество задач, возвращает массив структур Task
//--------------------------------------------------------------

Task * EnterTask (int n)
{
	Task * structArray = (Task * )calloc(1, n * sizeof(Task));

    for(int i = 0; i < n; i++)
    {
	    printf ("Введите параметры задачи %d:\n", i);
        scanf("%d %d %d %d", &structArray[i].pid, &structArray[i].mem, &structArray[i].time_act, &structArray[i].time_wait);
    }

    return structArray;
};

//--------------------------------------------------------------
//статус задачи
//--------------------------------------------------------------

void task_status(int n, Task * StructArray)
{
	Task * toshow = 0;

    for(int i = 0; i < n; i++)
	{

        toshow = &StructArray[i];

		printf("Статус задачи с номером = %d: ", toshow -> pid);
    
		switch ( toshow -> status )
		{
		case 0:
			printf("не поступила в очередь на исполнение\n");
			break;
		case 1:
			printf("в очереди на исполнение\n");
			break;

		case 2:
			printf("поступила на исполнение\n");
			break;

		case 3:
			printf("исполняется, %d у.е. времени осталось\n", StructArray[i].time_act);
			break;

		case 4:
			printf("выполнена\n");
			break;

		case 5:
			printf("отклонена\n");
			break;

		default:
			break;
		}
	}
}