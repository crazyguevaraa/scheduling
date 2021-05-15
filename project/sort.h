int MemCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySet2);
void MemSwapAllocPart (AllocPart *MemoryArea_1, AllocPart *MemoryArea_2);
void MemSortAllocTable (AllocPart* AllocTable, int left, int right);
int TaskCompareAllocTable (Task* Task1, Task* Task2);
void TaskSwapAllocPart (Task* Task1, Task* Task2);
void TaskSortAllocTable (Task* taskarr, int left, int right);
void PrepforAddressSort (AllocPart* AllocTable, int Memsize);
int AddressCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySet2);
void AddressSortAllocTable (AllocPart* AllocTable, int left, int right);

//---------испр--------------------------------------
//компаратор для сортировки по размеру памяти
//--------------------------------------------------------------

int MemCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySet2)
{
    int Size1 =  MemorySet1 -> size;
    int Size2 =  MemorySet2 -> size;

    return (Size1 > Size2);
}

//-----испр-------------------------------------------------------------
//swap двух указанных элементов для сортировки по размеру памяти
//----------------------------------------------------------------------
void MemSwapAllocPart (AllocPart *MemoryArea_1, AllocPart *MemoryArea_2)
{
    AllocPart   swap_const    = *MemoryArea_1;
            	*MemoryArea_1 = *MemoryArea_2;
            	*MemoryArea_2 = swap_const;
}

//---------------------------------------------------------------------
// сортировкf по размеру памяти
//---------------------------------------------------------------------
void MemSortAllocTable (AllocPart* AllocTable, int left, int right)
{
    
    int i = left;
    int j = right;
    int pivot = (right + left) / 2;

    do
    {
        while(MemCompareAllocTable(AllocTable + i, AllocTable + pivot) && (i < right) )
            i++;
        while(MemCompareAllocTable(AllocTable + pivot, AllocTable + j) && (j > left) )
            j--;
        if (i <= j)
        {
            MemSwapAllocPart (AllocTable + i, AllocTable + j);
            i++;
            j--;
        }
    }
    while (i <= j);
    if (j > left)
        MemSortAllocTable (AllocTable, left, j);
    if (i < right)
    	MemSortAllocTable (AllocTable, i, right);
}

//--------------------------------------------------------------
//компаратор для таблицы аллокций
//--------------------------------------------------------------

int TaskCompareAllocTable (Task* Task1, Task* Task2)
{
    int queue_num1 =  Task1 -> time_wait;
    int queue_num2 =  Task2 -> time_wait;

    return (queue_num1 <= queue_num2);
}

//-----испр-------------------------------------------------------------
//swap двух указанных элементов для сортировки задач
//----------------------------------------------------------------------

void TaskSwapAllocPart (Task* Task1, Task* Task2)
{
    Task swap_const = *Task1;
        	 *Task1 = *Task2;
        	 *Task2 = swap_const;
}


//---------------------------------------------------------------------
// сортировка задач
//---------------------------------------------------------------------
void TaskSortAllocTable (Task* taskarr, int left, int right)
{
    
    int i = left;
    int j = right;
    int pivot = (right + left) / 2;
  
    do
    {
        while(TaskCompareAllocTable(taskarr + i, taskarr + pivot) && (i < right) )
            i++;
        while(TaskCompareAllocTable(taskarr + pivot, taskarr + j) && (j > left) )
            j--;
        if (i <= j)
        {
            TaskSwapAllocPart (taskarr + i, taskarr + j);
            i++;
            j--;
        }
    }
    while (i <= j);
    if (j > left)
        TaskSortAllocTable (taskarr, left, j);
    if (i < right)
    	TaskSortAllocTable (taskarr, i, right);
}

//-----------------------------------------------------------------------------
// Данная фигня должна в уже отсортированном по размеру массиве AllocTable
// отсортировать одинаковые по размеру куски по адресам от меньшего к большему
// Данная функция отбирает группы кусков с одинаковым размером и отсылает их
// на сортировку по адресу
//-----------------------------------------------------------------------------

void PrepforAddressSort (AllocPart* AllocTable, int Memsize)
{

    

    for (int i = 0; i < Memsize;)
	{
  
		int same_size = 1; 										// счетчик кусков с одинаковым размером, самый первый кусок сразу считаем

        int j = i;

		while ( (AllocTable[j].size == AllocTable[j + 1].size) && (i < Memsize) )  // смотрим, является ли следующий кусок одинаковым по размеру
		{
			same_size++;                                            // Если да, то увеличиваем счетчик		
            j++;					
		}
		if (same_size != 1)										// Если кроме данного куска еще хотя бы один имеет такой же размер, сортируем
		{  
			AddressSortAllocTable(AllocTable + i, 0, same_size - 1);
		}
        
        i += same_size;
	}
}

//-----------------------------------------------------------------------------
// Компаратар адресов
// Просто сравниваем адреса, на которые указывают allocpart'ы как числа
//-----------------------------------------------------------------------------

int AddressCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySet2)
{
    int* address1 =  (MemorySet1 -> point);
    int* address2 =  (MemorySet2 -> point);

    return (address1 < address2);
}

//---------------------------------------------------------------------
// сортировка по адресам
// Также как и по размеру, только компратор другой
//---------------------------------------------------------------------

void AddressSortAllocTable (AllocPart* AllocTable, int left, int right)
{
    int i = left;
    int j = right;
    int pivot = (right + left) / 2;


    do
    {
        
        while(AddressCompareAllocTable(AllocTable + i, AllocTable + pivot) && (i < right) )
        {
            i++;
        }
        while(AddressCompareAllocTable(AllocTable + pivot, AllocTable + j) && (j > left) )
        {
            j--;
        }
            
        if (i <= j)
        {
            MemSwapAllocPart (AllocTable + i, AllocTable + j);
            i++;
            j--;
        }
    }
    while (i <= j);
    if (j > left)
        AddressSortAllocTable (AllocTable, left, j);
    if (i < right)
    	AddressSortAllocTable (AllocTable, i, right);
}