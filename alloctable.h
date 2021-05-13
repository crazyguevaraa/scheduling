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
AllocPart* create_AllocTableFree (int* Memory, int Memsize)
{
	AllocPart* AllocTableFree = (AllocPart*)calloc(1, Memsize * sizeof(AllocPart));
	AllocTableFree -> point   = Memory;
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
//--------------------------------------------------------------

int EnterValuesEmployed (int* Memory, int allowedMem ,AllocPart* AllocTableEmployed)
{
	int EmployedArea = 0;
	AllocTableEmployed -> point = Memory;                      //запись в элемент массив структур указателя на начало новой занятой области
	for (EmployedArea = 0; EmployedArea < allowedMem ;)
	{
		if (Memory [EmployedArea] == 1)
		{
			EmployedArea++; //если область все еще занята, то продолжаем двигаться правее
		}
		else
			break;
	}
	AllocTableEmployed -> size = EmployedArea; //запись в элемент массива структур размера области
			return EmployedArea;                       //возврат размера занятой области
}

//---------------------------------------------------------------------
//занесение данных в таблицу аллокации пустых областей
//ПРИНИМАЕТ : указатель на память, указатель на таблицу аллокации пустых областей
//ВОЗВРАЩАЕТ: размер пустой области (В ячейках)

int EnterValuesFree (int* Memory, int allowedMem ,AllocPart* AllocTableFree)
{
	int FreeArea = 0;
	AllocTableFree     -> point = Memory;                      //запись в элемент массива структур указателя на начало новой свободной области
	for (FreeArea = 0; FreeArea < allowedMem ;)
	{
		if (Memory [FreeArea] == 0)
		{
			FreeArea++;         //если область все еще свободна, то продолжаем двигаться правее
		}
		else
			break;
	}
			AllocTableFree -> size = FreeArea;     //запись в элемент массива структур размера свободной области
			return FreeArea;                           //возврат размера свободной области
	
}

//---------------------------------------------------------------------
//проверка на заполненность области
//ПРИНИМАЕТ : указатель на память
//ВОЗВРАЩАЕТ: единицу если ячейка заполнена и ноль если ячейка пуста
//--------------------------------------------------------------

int IsEmployed (int* Memory)
{
	if (*Memory == 1) return 1;				    //если память заполнена, возвращает единицу
	else return 0;						    //если память свободна , возращает ноль
}

//---------------------------------------------------------------------
//Создание таблицы аллокаций
//ПРИНИМАЕТ : указатель на память, размер памяти, указатели на таблицы аллокаций заполненных и пустых областей
//ВОЗВРАЩАЕТ: НИ-ХУ-Я
//--------------------------------------------------------------

void AllocTab (int* Memory, int SizeOfMemory, AllocPart* AllocTableEmployed, AllocPart* AllocTableFree, int* Amount_of_mem_parts)
{
	
	int FreeAreaNumber = 0;
	int EmployedAreaNumber = 0;

	for (int MemoryNumber = 0; MemoryNumber < SizeOfMemory;)
	{
		
		if (IsEmployed (Memory + MemoryNumber)) 
		{	
			int EmployedArea = EnterValuesEmployed (Memory + MemoryNumber, SizeOfMemory - MemoryNumber ,AllocTableEmployed + EmployedAreaNumber);
            
			EmployedAreaNumber++;
			MemoryNumber += EmployedArea;
		}
		else
		{    
			int FreeArea = EnterValuesFree (Memory + MemoryNumber, SizeOfMemory - MemoryNumber,AllocTableFree + FreeAreaNumber);
        
			FreeAreaNumber++;
			MemoryNumber += FreeArea;
		} 
			
	}

	Amount_of_mem_parts[0] = EmployedAreaNumber;
	Amount_of_mem_parts[1] = FreeAreaNumber;
}
