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

   /*  нахуй не нужно
   	asm ("push {r7, lr}");
    asm ("ldr r6, [pc, #8]");
    asm ("sub r6, #1");
    asm ("cmp r6, #0");
    asm ("bne execution+0x4");
    asm ("pop {r7, pc}");
    asm (".word 0xea60"); //60000  (here has to be time_act)
    */

    to_delete_a_task(to_do, execution);
};

//--------------------------------------------------------------
//ввод задачи
//--------------------------------------------------------------

Task * EnterTask (int n)  // Принимает на вход количество задач, возвращает массив структур Task
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

void task_status(int pid, int n, Task * StructArray)
{
	int i = 0;
	Task * toshow = 0;

    for(i = 0; i < n; i++)
	{
        if(pid == StructArray[i].pid)
        {
            toshow = &StructArray[i];
            break;
        }
    }

	if (toshow == 0)
		printf("Нет задачи с введенным вами идентификационным номером");
	else
		switch ( toshow -> status )
		{
		case 0:
			printf("не поступила в очередь\n");
			break;
		case 1:
			printf("в очереди\n");
			break;

		case 2:
			printf("поступила на исполнение\n");
			break;

		case 3:
			printf("исполняется, %d\n", StructArray[i].time_act);
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