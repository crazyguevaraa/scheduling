//----------------------------------------------------------------------
// Функция, печатающая память, как массив
//----------------------------------------------------------------------
void fprintMemory (int* Memory, int size, FILE* output);

//----------------------------------------------------------------------
// Функция, печатающая куски памяти из Alloctable
//----------------------------------------------------------------------
void fprintAlloctable(AllocPart *Alloctable, int size, FILE* output);

void fprintStateAfterexecution (int* Memory, int Memsize, int time, unsigned int pid, AllocPart* AllocTableFree, int Amount_of_mem_parts, FILE* output);

void fprintfAfterSimulation (int* Memory, int Memsize, Task* StructArray, int TaskNum, FILE* output);

//----------------------------------------------------------------------
// Функция, печатающая память, как массив
//----------------------------------------------------------------------
void printMemory (int* Memory, int size);

//----------------------------------------------------------------------
// Функция, печатающая куски памяти из Alloctable
//----------------------------------------------------------------------
void printAlloctable(AllocPart *Alloctable, int size);