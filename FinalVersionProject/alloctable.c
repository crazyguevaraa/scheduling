
#include "sort.h"
#include "alloctable.h"


//--------------------------------------------------------------
// Функция, выделяющая память под AllocTableEmployed
// Так как изначально ничего не заполнено, то изначальная память не имеет заполненных кусков
// Принимает размер памяти
// Возвращает массив занятых Allopart - ов
//--------------------------------------------------------------
AllocPart* create_AllocTableEmployed (int Memsize)
{
	AllocPart* AllocTableEmployed = (AllocPart*)calloc(1, Memsize * sizeof(AllocPart)); // Выделяем память

	AllocTableEmployed -> size = 0;

	return AllocTableEmployed;
}

//--------------------------------------------------------------
// Функция, выделяющая память под AllocTableFree
// Так как изначально ничего не заполнено, то вся изначальная память - один большой кусок свободной памяти
// Принимает указатель на память, её размер
// Возвращает массив свободных Allopart - ов
//--------------------------------------------------------------
AllocPart* create_AllocTableFree (int* Memory, int Memsize)
{
	AllocPart* AllocTableFree = (AllocPart*)calloc(1, Memsize * sizeof(AllocPart)); // Выделяем память
	AllocTableFree -> point   = Memory;												// Начало свободной памяти - начало памяти
	AllocTableFree -> size    = Memsize;											// Размер свободного куска - вся память, так как она еще не занята
	return AllocTableFree;
}

//--------------------------------------------------------------
// Функция, освобождающая память от обоих AllocTable
// Принимает  Таблицы аллокаций
// Возвращает ничего
//--------------------------------------------------------------
void destroyBothAllocTables (AllocPart* AllocTableFree, AllocPart* AllocTableEmployed)
{
	free (AllocTableFree);
	free (AllocTableEmployed);
}

//---------------------------------------------------------------------
// занесение данных в таблицу аллокации занятых областей
// Принимает  указатель на память, указатель на таблицу аллокации занятых областей
// Возвращает размер занятой области (в ячейках)
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
//Принимает : указатель на память, указатель на таблицу аллокации пустых областей
//Возвращает размер пустой области (В ячейках)
//---------------------------------------------------------------------
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
//Принимает : указатель на память
//Возвращает единицу если ячейка заполнена и ноль если ячейка пуста
//--------------------------------------------------------------

int IsEmployed (int* Memory)
{
	if (*Memory == 1) return 1;				    //если память заполнена, возвращает единицу
	else return 0;						    //если память свободна , возращает ноль
}

//---------------------------------------------------------------------
//Создание таблицы аллокаций
//Принимает : указатель на память, размер памяти, указатели на таблицы аллокаций заполненных и пустых областей,
//			  массив, куда запишутся количества кусков свободной и занятой памятей
//Возвращает ничего
//--------------------------------------------------------------

void AllocTab (int* Memory, int SizeOfMemory, AllocPart* AllocTableEmployed, AllocPart* AllocTableFree, int* Amount_of_mem_parts)
{
	
	int FreeAreaNumber = 0;				// количество цельных кусков свободной памяти
	int EmployedAreaNumber = 0;			// количество цельных кусков занятой памяти

	for (int MemoryNumber = 0; MemoryNumber < SizeOfMemory;) // пробегаемся по памяти
	{
		
		if (IsEmployed (Memory + MemoryNumber)) // Если нашли занятую ячейку, то от нее отсчитываем сколько подряд занятых,
												// и записываем указатель на эту область памяти и ее размер в AllocTableEmployed 
		{	
			int EmployedArea = EnterValuesEmployed (Memory + MemoryNumber, SizeOfMemory - MemoryNumber ,AllocTableEmployed + EmployedAreaNumber);
            
			EmployedAreaNumber++;				// Записываем, что нашелся кусок занятой памяти
			MemoryNumber += EmployedArea;		// Весь кусок памяти от данного MemoryNumber до MemoryNumber + EmployedArea обработан, поэтому
												// мы его пропускаем далее в цикле
		}
		else									//Аналогично с найденной свободной ячейкой
		{    
			int FreeArea = EnterValuesFree (Memory + MemoryNumber, SizeOfMemory - MemoryNumber,AllocTableFree + FreeAreaNumber);
        
			FreeAreaNumber++;
			MemoryNumber += FreeArea;
		} 
			
	}

	Amount_of_mem_parts[0] = EmployedAreaNumber; // записываем в массив количество занятых и 
	Amount_of_mem_parts[1] = FreeAreaNumber;	 //	свободных кусков
}

//------------------------------------------------------------------------------
// Функция обрабатывает состояние памяти
// Принимает указатель на начало памяти и её размер, указатели на массив
// 			  Allocpart-ов, массив из количества цельных кусков свободной и занятой памяти
// Возвращает ничего 
//------------------------------------------------------------------------------
void processMemory(int* Memory, int SizeOfMemory, AllocPart* AllocTableEmployed, AllocPart* AllocTableFree, int* Amount_of_mem_parts)
{
    AllocTab (Memory, SizeOfMemory, AllocTableEmployed, AllocTableFree, Amount_of_mem_parts); // переформируем куски свободной и заянтой памяти

    MemSortAllocTable (AllocTableEmployed, 0, Amount_of_mem_parts[0]);  //пересортируем куски
    MemSortAllocTable (AllocTableFree, 0, Amount_of_mem_parts[1]);      //занятой и свободной памятей

	AddressSort(AllocTableFree, Amount_of_mem_parts[1]);

}