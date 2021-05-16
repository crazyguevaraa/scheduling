//--------------------------------------------------------------
//структура задачи
typedef struct {
	unsigned int pid; //идентификатор задачи
	unsigned int time_act; //время выполнения задачи
	unsigned int mem; // количество памяти, нужное для размещения задачи
//время ожидания
//статус




}Task;

//--------------------------------------------------------------
//структура области памяти

typedef struct 
{
	char * point;                                              //указатель на начало области
	int size;                                                  //размер области 
} AllocPart;

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

void AllocTab (int* Memory, int SizeOfMemory, AllocPart* AllocTableEmployed, AllocPart* AllocTableFree)
{
	for (int MemoryNumber = 0, int FreeAreaNumber = 0, int EmployedAreaNumber = 0; MemoryNumber < SizeOfMemory;)
	{
		if (IsEmployed (Memory [MemoryNumber])) 
		{	
			int EmployedArea = EnterValuesEmployed (Memory [MemoryNumber], AllocTableEmployed [EmployedAreaNumber]);
			EmployedAreaNumber++;
			MemoryNumber += EmployedArea;
		}
		else
		{
			int FreeArea = EnterValuesFree (Memory [MemoryNumber], AllocTableFree [FreeAreaNumber]);
			FreeAreaNumber++;
			MemoryNumber += FreeArea;
		} 
			
	}
}

//---------------------------------------------------------------------
//сортировка таблицы аллокаций
//ПРИНИМАЕТ : указатель на таблицу аллокаций
//ВОЗВРАЩАЕТ: отсортированная по размеру таблица аллокаций

 
//---------испр--------------------------------------
//компаратор для гей-сортировки
//НАХУЙ НЕ НУЖНА
//upd.: все таки нужна блинб(((

int GayCompareAllocTable (AllocPart* MemorySet1, AllocPart* MemorySize2)
{
    int Size1 =  MemorySet1 -> size;
    int Size2 =  MemorySet2 -> size;
 
    while (*Size1 == *Size2 && *Size1 != '\0' && *Size2 != '\0')
    {
      Size1 = (MemorySet1++) -> size;
      Size2 = (MemorySet2++) -> size;
    }
    return (Size1 > Size2) - (Size1 < Size2);
}





//-----испр-------------------------------------------------------------
//swap двух указанных элементов для гей-сортировки

void GaySwapAllocPart (AllocPart *MemoryArea_1, AllocPart *MemoryArea_2)
{
    AllocPart  swap_const = *MemoryArea_1;
            *MemoryArea_1 = *MemoryArea_2;
            *MemoryArea_2 = swap_const;
}





//---------------------------------------------------------------------
//гей-сортировка 
void GaySortAllocTable (AllocPart* AllocTable, int Size)
{
    int       i         = Size / 2;
    AllocPart ptr_left  = AllocTable[1];
    AllocPart ptr_right = AllocTable[size_arr_1 - 1];
    AllocPart ptr_pivot = AllocPart[i];
    
    for(unsigned long long int k = 0, j = Size - 1; k < j; i++, j--)
    {
        while(gay_compar_str(ptr_left, ptr_pivot))
            ptr_left++;
        while(gay_compar_str(ptr_pivot, ptr_right))
            ptr_right--;
        
        GaySwapAllocPart (ptr_left, ptr_right);
    }
    GaySortAllocTable (AllocTable, i);
    GaySortAllocTable (AllocTable + i, Size - i);
}

//---------------------------------------------------------------------
//создание листа с отсортирорванными областями памяти 
//и запихуивание туда элементов отсортированного массива структур

List* MemoryList (AllocPart* AllocTable, int Size)
{
	List* MemoryList = createList();
	for (int i = 0 ; i < Size ; i++)
		insertL (MemoryList, AllocTable + i);
	return MemoryList;
}














