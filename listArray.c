#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #define MAX 4
// head <-0|0|0|0|0|0|0|0|0|0->   <-0000000000-><-0000000000-><-0000000000->

 typedef struct Node{

// Указатель на следующий элемент буфера    
    Node  *next, *prev;
//		size_t size;
		int num;
		// Объекты, помещаемые в буфер 
//		char * array;
//        char * array[0];
		char array[4]; 
}NodeArray;

// Класс-контейнер для элементов Obj
typedef struct LA{
	Node *head
	Node *tail;
}ListArray;



//-----------------------------ListArray----------------
ListArray * createLA()
{
    ListArray * tmp = calloc (1 , sizeof(ListArray));
    return tmp;
}


//-----------------------------setV---------------------
// Выделяем необходимое количество узлов и записываем в 
// нужную ячейку
void setV(ListArray * lst, char z, int n);
{
    
    

     
}




// Получить Элемент с номером n
char getV(ListArray *lst, int n);
// Обнулить все узлы
void clearLA(ListArray * lst);
// освободить всю память
ListArray * destroyLA(ListArray * lst);
// печать всего массива
void printAr(const ListArray * ar);

x = x^x;

