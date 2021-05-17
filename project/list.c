#include "list.h"


//--------------------------------------------------------------
//создание листа задач
//--------------------------------------------------------------

List *createList()
{
	List * tmp = (List *)calloc(1, sizeof(List *));
	return tmp;
}




//--------------------------------------------------------------
//вставка задачи 
//--------------------------------------------------------------

void insertL(List *lst, Task * task)
{
	if( !lst -> head)
	{
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



//--------------------------------------------------------------
//удаление куска из листа памяти
//--------------------------------------------------------------

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
}

//--------------------------------------------------------------
//удаление листа памяти
//--------------------------------------------------------------

void deleteL(List *lst, Node * del)
{
	if(!del)
		return;
	if (del == lst -> head && del == lst -> tail)
	{
		lst -> head = 0;
		lst -> tail = 0;
		free(del);
		del = 0;
		return;
	}
	if ( del == lst -> head)
	{
		lst -> head = lst -> head -> next;
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
	Node * previous, * nextone;
	previous = del -> prev;
	nextone = del -> next;
	previous->next = nextone;
	nextone->prev = previous;
	free(del);
	del = 0;
}


/*//--------------------------------------------------------------
//создание листа памяти c allocpart-ами
//--------------------------------------------------------------

List2 *createList2()
{
	List2 * tmp = (List2 *)calloc(1, sizeof(List2 *));
	return tmp;
}

//--------------------------------------------------------------
//вставка куска памяти в лист памяти c allocpart-ами
//--------------------------------------------------------------

void insertL2(List2 *lst, AllocPart * task)
{
	if( !lst -> head){
		Node2 * newN =  (Node2 *)calloc(1, sizeof(Node2));
		newN -> task = task;
		newN -> prev = 0;
		newN -> next = 0;
		lst -> tail = newN;
		lst -> head = newN;
		return;
	}
	Node2 * p = (Node2 *)calloc(1, sizeof(Node2));
	p -> task = task;
	p -> next = 0;
	p -> prev = lst -> tail;
	lst -> tail -> next = p;
	lst -> tail = p;

}

//---------------------------------------------------------------------
//создание листа с отсортирорванными областями памяти
//и засовывания туда элементов отсортированного массива структур
//---------------------------------------------------------------------

List2* MemoryList (AllocPart* AllocTable, int Size)
{
	List2* MemoryList = createList2();
	for (int i = 0 ; i < Size ; i++)
		insertL2 (MemoryList, AllocTable + i);
	return MemoryList;
} */