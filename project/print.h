

void printMemory (int* Memory, int size);
void printAlloctable(AllocPart *Alloctable, int size);


void printMemory (int* Memory, int size)
{
    int i = 0;
	printf("[");
	for (i = 0; i < size - 1; i++)
	    printf("%d, ", *(Memory + i) );


	printf("%d]", *(Memory + i) );
}

void printAlloctable(AllocPart *Alloctable, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("size of %d part is %d\n",i, (Alloctable + i)->size );
    }
}