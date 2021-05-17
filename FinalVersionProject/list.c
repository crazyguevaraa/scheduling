#include "struct.h"
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


