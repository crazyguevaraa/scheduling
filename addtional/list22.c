#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10
 // |task1->|
 

typedef struct task_to_do
{
	unsigned int mem;
	unsigned int pid;
	unsigned int time_act;
	int 		 status;
	int 		 time_before_exec;

}Task;


typedef struct Nod
{
	Task * 		 task;
	struct Nod * next;
	struct Nod * prev; 
}Node;

typedef struct Lst
{
	Node *head;
	Node *tail;
}List;


List *createList(){
	List * tmp = (List *)calloc(1, sizeof(List *));
	return tmp;
};

void printList(List *lst){
	printf("print:\n");
	Node *p = lst->head;
	while( p ){
		if(p->next)
			printf("%hhu->", *(p->task));
		else
			printf("%hhu\n", *(p->task));
		p = p->next;
	}
	printf("end\n");
}

void insertL(List *lst, void * task){
	if( !lst->head){
		Node * newN =  (Node *)calloc(1, sizeof(Node));
		newN->task = (char *)task;
		newN->prev = 0;
		newN->next = 0;
		lst->tail = newN;
		lst->head = newN;
		return;
	}
	Node * p = (Node *)calloc(1, sizeof(Node));
	p->task = (char *)task;
	p->next = 0;
	p->prev = lst->tail;
	lst->tail->next = p;
	lst->tail = p;	
	
};



List *  destroyList(List * lst){
	Node *p = lst->head;
	Node * prev = 0;
	while( p )
	{
		prev = p;
		p = p->next;
		free(prev);
	}
	free(lst);
	return 0;
};

// Вставить элемет перед
void insertBefore(List * lst, Node * point, char * task)
{
    Node* pointer;

    while ( (pointer->next != point) & (pointer != lst->tail) )
    {
        pointer = pointer->next;
    }

    if (pointer == lst->tail)
    {
        printf("There is no node in list with such adress\n");
        return;
    }
    
    Node* new = (Node *)calloc(1, sizeof(Node *));
	
    new->task = task;

    new->prev = pointer->prev;

	new->next = pointer;
}

// Вставить перед.
void insertAfter(List *lst, Node * point, char * task)
{
    Node* pointer;

    while ( (pointer->next != point) & (pointer != lst->tail) )
    {
        pointer = pointer->next;
    }

    if (pointer == lst->tail)
    {
        printf("There is no node in list with such adress\n");
        return;
    }
    
    Node* new = (Node *)calloc(1, sizeof(Node *));
    new->task = task;

    new->next = point->next;

    point->next = new;
}

void deleteL(List *lst, Node * del){
//	printf("del: %p %hhu\n", del, *(del->task));
	if(!del)
		return;
	if(del == lst->head && del == lst->tail)
	{
		printf("head-tail\n");
		lst->head = 0;
		lst->tail = 0;
		free(del);
		del = 0;
		return;
	}
	if ( del == lst->head)
	{
		lst->head = lst->head->next;
		printf("head: %hhu\n", *(lst->head->task));
		lst->head->prev = 0;
		free(del);
		del = 0;
		return;
	}
	if (del == lst->tail)
	{
		lst->tail = lst->tail->prev;
		lst->tail->next = 0;
		free(del);
		del = 0;
		return;
	}
	Node * pred, * sled;
	pred = del->prev;
	sled = del->next;
	pred->next = sled;
	sled->prev = pred;
	free(del);
	del = 0;		
};
