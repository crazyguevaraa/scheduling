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
        Task * another_one = wait_list -> head -> task;
        for (int i = 0; i < TaskNum; i++)
        {
            if (another_one -> mem > AllocTableFree -> size)
            {
                to_add_to_execution (another_one, todo_list, wait_list);
                *(AllocTableFree -> point) = 1;
                another_one -> status = 1;

                AllocTab (Memory, Memsize, AllocTableEmployed, AllocTableFree, Amount_of_mem_parts); // переформируем куски свободной и заянтой памяти

                GaySortAllocTable (AllocTableEmployed, Amount_of_mem_parts[0]); //пересортируем куски
                GaySortAllocTable (AllocTableFree, Amount_of_mem_parts[1]);     // занятой и свободной памятей
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



    int pid = 0;
    
    while(1)
    { 
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






#define SIZE 10

#include <stdio.h>
#include <stdlib.h>

//int AddressCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySet2);
//void AddressSortAllocTable (AllocPart AllocTable, int Size);

//--------------------------------------------------------------
//структура задачи
//--------------------------------------------------------------
typedef struct  task_to_do{
    unsigned int pid;        //идентификатор задачи
    unsigned int time_act;   //время выполнения задачи
    unsigned int mem;        //количество памяти, нужное для размещения задачи
    unsigned int time_wait;  //время ожидания
    unsigned int status;     //статус
}Task;


typedef struct Nod
{
	Task * 		 task;
	struct Nod * next;
	struct Nod * prev; 
}Node;

typedef struct Lst
{
	Node *head;
	Node *tail;
}List;

//int AddressCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySet2);
//void AddressSortAllocTable (AllocPart* AllocTable, int Size);


List *createList(){
	List * tmp = (List *)calloc(1, sizeof(List *));
	return tmp;
};



void insertL(List *lst, Task * task)
{
	if( !lst -> head){
		Node * newN =  (Node *)calloc(1, sizeof(Node));
		newN -> task = task;
		newN -> prev = 0;
		newN -> next = 0;
		lst -> tail = newN;
		lst -> head = newN;
		return;
	}
	Node * p = (Node *)calloc(1, sizeof(Node));
	p -> task = task;
	p -> next = 0;
	p -> prev = lst -> tail;
	lst -> tail -> next = p;
	lst -> tail = p;	
	
};



List *  destroyList(List * lst)
{
	Node *p = lst -> head;
	Node * prev = 0;
	while( p )
	{
		prev = p;
		p = p -> next;
		free(prev);
	}
	free(lst);
	return 0;
};





void deleteL(List *lst, Node * del)
{
//	printf("del: %p %hhu\n", del, *(del->task));
	if(!del)
		return;
	if(del == lst -> head && del == lst -> tail)
	{
		printf("head-tail\n");
		lst -> head = 0;
		lst -> tail = 0;
		free(del);
		del = 0;
		return;
	}
	if ( del == lst -> head)
	{
		lst -> head = lst -> head -> next;
		printf("head: %hhu\n", *(lst -> head -> task));
		lst -> head -> prev = 0;
		free(del);
		del = 0;
		return;
	}
	if (del == lst->tail)
	{
		lst -> tail = lst -> tail -> prev;
		lst -> tail -> next = 0;
		free(del);
		del = 0;
		return;
	}
	Node * pred, * sled;
	pred = del -> prev;
	sled = del -> next;
	pred->next = sled;
	sled->prev = pred;
	free(del);
	del = 0;		
};



//--------------------------------------------------------------
//отправка задачи в список очереди
//--------------------------------------------------------------
void to_add_to_queue (Task* newone, List* queue)
{
	insertL(queue, newone);
	newone -> status = 1;    // попала в очередь на ожидание
};


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
        to_do -> status = 4;
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
	Task * structArray = (Task * )calloc(n, sizeof(Task));

    for(int i = 0; i < n; i++)
    {
	    printf ("Введите параметры задачи %d:\n", i);
        scanf("%d %d %d %d", &structArray[i].pid, &structArray[i].mem, &structArray[i].time_act, &structArray[i].time_wait);
    }

    return structArray;
};



// Создает лист на ожидание
// Сразу выставляет статус 0 задачам, априоре невыполнимым
List * wait_list_constructor(int n, Task * StructArray, int Memsize, int time)
{
    List * tmp = createList();

    for(int i = 0; i < n; i++)
    {
		if (StructArray[i].mem > Memsize)
		{
			StructArray[i].status = 0;
		}
		else if (StructArray[i].time_act > time)
			 {
			 	 StructArray[i].status = 0;
			 }
			 else
        	 	to_add_to_queue(StructArray + i, tmp);
    }

    return tmp;
};






//--------------------------------------------------------------
//структура области памяти
//--------------------------------------------------------------
typedef struct 
{
	char * point;                                              //указатель на начало области
	int size;                                                  //размер области 
} AllocPart;




int AddressCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySet2);
void AddressSortAllocTable (AllocPart* AllocTable, int Size);







//--------------------------------------------------------------
// Функция, выделяющая память под AllocTableEmployed
// Так как изначально ничего не заполнено, то изначальная память не имеет заполненных кусков
//--------------------------------------------------------------
AllocPart* create_AllocTableEmployed (int Memsize)
{
	AllocPart* AllocTableEmployed = (AllocPart*)calloc(1, Memsize * sizeof(AllocPart));

	AllocTableEmployed -> size = 0;

	return AllocTableEmployed;
}

//--------------------------------------------------------------
// Функция, выделяющая память под AllocTableFree
// Так как изначально ничего не заполнено, то вся изначальная память - один большой кусок свободной памяти
//--------------------------------------------------------------
AllocPart* create_AllocTableFree (char* Memory, int Memsize)
{
	AllocPart* AllocTableFree = (AllocPart*)calloc(1, Memsize * sizeof(AllocPart));
	AllocTableFree -> point   = (char*)Memory;
	AllocTableFree -> size    = Memsize;
	return AllocTableFree;
}

//--------------------------------------------------------------
// Функция, освобождающая память от обоих AllocTable
//--------------------------------------------------------------
void destroyBothAllocTables (AllocPart* AllocTableFree, AllocPart* AllocTableEmployed)
{
	free (AllocTableFree);
	free (AllocTableEmployed);
}

//---------------------------------------------------------------------
//занесение данных в таблицу аллокации занятых областей 
//ПРИНИМАЕТ:  указатель на память, указатель на таблицу аллокации занятых областей
//ВОЗВРАЩАЕТ: размер занятой области (в ячейках)

int EnterValuesEmployed (int* Memory, AllocPart* AllocTableEmployed)
{
	AllocTableEmployed -> point = Memory;                      //запись в элемент массив структур указателя на начало новой занятой области 
	for (int EmployedArea = 0;;)
	{
		if (Memory [EmployedArea + 1] == 1) EmployedArea++; //если область все еще занята, то продолжаем двигаться правее
		else 
		{
			AllocTableEmployed -> size = EmployedArea; //запись в элемент массива структур размера области
			return EmployedArea;                       //возврат размера занятой области
		}
	}
}

//---------------------------------------------------------------------
//занесение данных в таблицу аллокации пустых областей
//ПРИНИМАЕТ : указатель на память, указатель на таблицу аллокации пустых областей
//ВОЗВРАЩАЕТ: размер пустой области (В ячейках)

int EnterValuesFree (int* Memory, AllocPart* AllocTableFree)
{
	AllocTableFree     -> point = Memory;                      //запись в элемент массива структур указателя на начало новой свободной области 
	for (int FreeArea = 0;;)
	{
		if (Memory [FreeArea + 1] == 0) FreeArea++;         //если область все еще свободна, то продолжаем двигаться правее
		else
		{
			AllocTableFree     -> size = FreeArea;     //запись в элемент массива структур размера свободной области
			return FreeArea;                           //возврат размера свободной области
		}
	}
}

//---------------------------------------------------------------------
//проверка на заполненность области
//ПРИНИМАЕТ : указатель на память
//ВОЗВРАЩАЕТ: единицу если ячейка заполнена и ноль если ячейка пуста

int IsEmployed (int* Memory)
{
	if (Memory == 1) return 1;				    //если память заполнена, возвращает единицу
	else return 0;						    //если память свободна , возращает ноль	
}


//---------------------------------------------------------------------
//Создание таблицы аллокаций
//ПРИНИМАЕТ : указатель на память, размер памяти, указатели на таблицы аллокаций заполненных и пустых областей
//ВОЗВРАЩАЕТ: НИ-ХУ-Я

void AllocTab (int* Memory, int SizeOfMemory, AllocPart* AllocTableEmployed, AllocPart* AllocTableFree, int* Amount_of_mem_parts)
{
	int FreeAreaNumber = 0;
	int EmployedAreaNumber = 0;

	for (int MemoryNumber = 0; MemoryNumber < SizeOfMemory;)
	{
		if (IsEmployed (Memory [MemoryNumber])) 
		{	
			int EmployedArea = EnterValuesEmployed (Memory [MemoryNumber], AllocTableEmployed + EmployedAreaNumber);
			EmployedAreaNumber++;
			MemoryNumber += EmployedArea;
		}
		else
		{
			int FreeArea = EnterValuesFree (Memory [MemoryNumber], AllocTableFree + FreeAreaNumber);
			FreeAreaNumber++;
			MemoryNumber += FreeArea;
		} 
			
	}

	Amount_of_mem_parts[0] = EmployedAreaNumber;
	Amount_of_mem_parts[1] = FreeAreaNumber;
}

//---------------------------------------------------------------------
//сортировка таблицы аллокаций
//ПРИНИМАЕТ : указатель на таблицу аллокаций
//ВОЗВРАЩАЕТ: отсортированная по размеру таблица аллокаций

 
//---------испр--------------------------------------
//компаратор для гей-сортировки
//НАХУЙ НЕ НУЖНА
//upd.: все таки нужна блинб(((

int GayCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySet2)
{
    int Size1 =  MemorySet1 -> size;
    int Size2 =  MemorySet2 -> size;
    
    return (Size1 > Size2) - (Size1 < Size2);
}


//-----испр-------------------------------------------------------------
//swap двух указанных элементов для гей-сортировки
//----------------------------------------------------------------------
void GaySwapAllocPart (AllocPart *MemoryArea_1, AllocPart *MemoryArea_2)
{
    AllocPart*  swap_const = MemoryArea_1;
            	MemoryArea_1 = MemoryArea_2;
            	MemoryArea_2 = swap_const;
}


//---------------------------------------------------------------------
//гей-сортировка
//---------------------------------------------------------------------
void GaySortAllocTable (AllocPart* AllocTable, int Size)
{
    int       i          = Size / 2;
    AllocPart* ptr_left  = AllocTable + 1;
    AllocPart* ptr_right = AllocTable + Size - 1;
    AllocPart* ptr_pivot = AllocTable + i;
    
    for(unsigned long long int k = 0, j = Size - 1; k < j; i++, j--)
    {
        while(GayCompareAllocTable(ptr_left, ptr_pivot))
            ptr_left++;
        while(GayCompareAllocTable(ptr_pivot, ptr_right))
            ptr_right--;
        
        GaySwapAllocPart (ptr_left, ptr_right);
    }
    GaySortAllocTable (AllocTable, i);
    GaySortAllocTable (AllocTable + i, Size - i);
}

//-----------------------------------------------------------------------------
// Данная фигня должна в уже отсортированном по размеру массиве AllocTable
// отсортировать одинаковые по размеру куски по адресам от меньшего к большему
// Данная функция отбирает группы кусков с одинаковым размером и отсылает их
// на сортировку по адресу 
//-----------------------------------------------------------------------------
void PrepforAddressSort (AllocPart* AllocTable, int Memsize)
{
    for (int i = 0; i < Memsize; i++)
	{
		int same_size = 1; 										// счетчик кусков с одинаковым размером, самый первый кусок сразу считаем

		while ( (AllocTable[i].size == AllocTable[i + 1].size))  // смотрим, является ли следующий кусок одинаковым по размеру
		{
			same_size++;										// Если да, то увеличиваем счетчик
		}
		if (same_size != 1)										// Если кроме данного куска еще хотя бы один имеет такой же размер, сортируем
		{
			AddressSortAllocTable(AllocTable + i, same_size);
		}
		i += same_size;											// все куски от данного до отстоящего на same_size уже отсортировали, второй раз по ним прогонять нет смысла
	}
}

//-----------------------------------------------------------------------------
// Компаратар адресов
// Просто сравниваем адреса, на которые указывают allocpart'ы как числа
//-----------------------------------------------------------------------------
int AddressCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySet2)
{
    int address1 =  MemorySet1 -> point;
    int address2 =  MemorySet2 -> point;
    
    return (address1 > address2) - (address1 < address2);
}


//---------------------------------------------------------------------
// сортировка по адресам
// Также как и по размеру, только компратор другой
//---------------------------------------------------------------------
void AddressSortAllocTable (AllocPart* AllocTable, int Size)
{
    int       i          = Size / 2;
    AllocPart* ptr_left  = AllocTable + 1;
    AllocPart* ptr_right = AllocTable + Size - 1;
    AllocPart* ptr_pivot = AllocTable + i;
    
    for(unsigned long long int k = 0, j = Size - 1; k < j; i++, j--)
    {
        while(AddressCompareAllocTable(ptr_left, ptr_pivot))
            ptr_left++;
        while(AddressCompareAllocTable(ptr_pivot, ptr_right))
            ptr_right--;
        
        GaySwapAllocPart (ptr_left, ptr_right);
    }

    AddressSortAllocTable (AllocTable, i);
    AddressSortAllocTable (AllocTable + i, Size - i);
}

//---------------------------------------------------------------------
//создание листа с отсортирорванными областями памяти 
//и запихуивание туда элементов отсортированного массива структур
//---------------------------------------------------------------------
List* MemoryList (AllocPart* AllocTable, int Size)
{
	List* MemoryList = createList();
	for (int i = 0 ; i < Size ; i++)
		insertL (MemoryList, AllocTable + i);
	return MemoryList;
}

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
