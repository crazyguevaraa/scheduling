int GayCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySet2);
void GaySwapAllocPart (AllocPart *MemoryArea_1, AllocPart *MemoryArea_2);
void GaySortAllocTable (AllocPart* AllocTable, int left, int right);
int TaskCompareAllocTable (Task* Task1, Task* Task2);
void TaskSwapAllocPart (Task* Task1, Task* Task2);
void TaskSortAllocTable (Task* taskarr, int left, int right);
void PrepforAddressSort (AllocPart* AllocTable, int Memsize);
int AddressCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySet2);
void AddressSortAllocTable (AllocPart* AllocTable, int Size);

//---------испр--------------------------------------
//компаратор для гей-сортировки
//НАХУЙ НЕ НУЖНА
//upd.: все таки нужна блинб(((
//--------------------------------------------------------------

int GayCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySet2)
{
    int Size1 =  MemorySet1 -> size;
    int Size2 =  MemorySet2 -> size;

    return (Size1 > Size2);
}

//-----испр-------------------------------------------------------------
//swap двух указанных элементов для гей-сортировки
//----------------------------------------------------------------------
void GaySwapAllocPart (AllocPart *MemoryArea_1, AllocPart *MemoryArea_2)
{
    AllocPart   swap_const    = *MemoryArea_1;
            	*MemoryArea_1 = *MemoryArea_2;
            	*MemoryArea_2 = swap_const;
}

//---------------------------------------------------------------------
//гей-сортировка
//---------------------------------------------------------------------
void GaySortAllocTable (AllocPart* AllocTable, int left, int right)
{
    
    int i = left;
    int j = right;
    int pivot = (right + left) / 2;

    //printf("pivot = %d\n", pivot);
    //printf("at start: i = %d, j = %d\n", i, j);

    do
    {
        //printf("\nI am here\n");
        //printAlloctable(AllocTable, Size);
        while(GayCompareAllocTable(AllocTable + i, AllocTable + pivot) && (i < right) )
            i++;
        while(GayCompareAllocTable(AllocTable + pivot, AllocTable + j) && (j > left) )
            j--;
        if (i <= j)
        {
            //printf("i = %d, j = %d\n", i, j);
            GaySwapAllocPart (AllocTable + i, AllocTable + j);
            printAlloctable(AllocTable, right - left + 1);
            //printf("\n\n");
            i++;
            j--;
        }
    }
    while (i <= j);
    if (j > left)
        GaySortAllocTable (AllocTable, left, j);
    if (i < right)
    	GaySortAllocTable (AllocTable, i, right);
}

//--------------------------------------------------------------
//компаратор для таблицы аллокций
//--------------------------------------------------------------

int TaskCompareAllocTable (Task* Task1, Task* Task2)
{
    int queue_num1 =  Task1 -> time_wait;
    int queue_num2 =  Task2 -> time_wait;

    return (queue_num1 < queue_num2);
}

//-----испр-------------------------------------------------------------
//swap двух указанных элементов для гей-сортировки
//----------------------------------------------------------------------

void TaskSwapAllocPart (Task* Task1, Task* Task2)
{
    Task swap_const = *Task1;
        	 *Task1 = *Task2;
        	 *Task2 = swap_const;
}


//---------------------------------------------------------------------
//гей-сортировка
//---------------------------------------------------------------------
void TaskSortAllocTable (Task* taskarr, int left, int right)
{
    
    int i = left;
    int j = right;
    int pivot = (right + left) / 2;

    printf("pivot = %d\n", pivot);
    //printf("at start: i = %d, j = %d\n", i, j);

    do
    {
        //printf("\nI am here\n");
        //printAlloctable(AllocTable, Size);
        while(TaskCompareAllocTable(taskarr + i, taskarr + pivot) && (i < right) )
            i++;
        while(TaskCompareAllocTable(taskarr + pivot, taskarr + j) && (j > left) )
            j--;
        if (i <= j)
        {
            //printf("i = %d, j = %d\n", i, j);
            TaskSwapAllocPart (taskarr + i, taskarr + j);
            //printf("\n\n");
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
    for (int i = 0; i < Memsize; i++)
	{
		int same_size = 1; 										// счетчик кусков с одинаковым размером, самый первый кусок сразу считаем

		while ( AllocTable[i].size == AllocTable[i + 1].size)  // смотрим, является ли следующий кусок одинаковым по размеру
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
    int address1 =  *MemorySet1 -> point;
    int address2 =  *MemorySet2 -> point;

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