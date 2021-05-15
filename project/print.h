

void printMemory (int* Memory, int size);
void printAlloctable(AllocPart *Alloctable, int size);


//----------------------------------------------------------------------
// Функция, печатающая память, как массив
//----------------------------------------------------------------------
void printMemory (int* Memory, int size)
{
    int i = 0;
	printf("\n[");                        // Скобочки для красоты восприятия
	for (i = 0; i < size - 1; i++)
	    printf("%d, ", *(Memory + i) );


	printf("%d]\n\n", *(Memory + i) );     // Скобочки для красоты восприятия
}

//----------------------------------------------------------------------
// Функция, печатающая куски памяти из Alloctable
//----------------------------------------------------------------------
void printAlloctable(AllocPart *Alloctable, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("\nsize of %d part is %d\n\n",i, (Alloctable + i)->size );
    }
}