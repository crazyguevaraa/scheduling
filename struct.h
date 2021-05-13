typedef struct
{
	int * point;                                              //указатель на начало области
	int size;                                                  //размер области
} AllocPart;

//--------------------------------------------------------------
//структура задачи
//--------------------------------------------------------------
typedef struct  task_to_do{
    unsigned int pid;        //идентификатор задачи
    unsigned int time_act;   //время выполнения задачи
    unsigned int mem;        //количество памяти, нужное для размещения задачи
    unsigned int time_wait;  //время ожидания
    unsigned int status;     //статус
	int *	 taken_mem;  //указатель на начало памяти, которое заняло данный task 
}Task;

typedef struct Nod
{
	Task * 		 task;
	struct Nod * next;
	struct Nod * prev;
}Node;

//--------------------------------------------------------------
//структура области памяти
//--------------------------------------------------------------

typedef struct Lst
{
	Node *head;
	Node *tail;
}List;

//--------------------------------------------------------------
//структура листа памяти
//--------------------------------------------------------------

typedef struct nod
{
	AllocPart   *	task;
	struct nod * next;
	struct nod * prev;
}Node2;

typedef struct lst
{
	Node2 *head;
	Node2 *tail;
}List2;

