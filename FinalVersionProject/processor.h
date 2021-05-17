#include "struct.h"
#include "task.h"
#include "alloctable.h"
#include "print.h"
#include "list.h"

int processor(int* Memory, int Memsize, int TaskNum, int time, AllocPart* AllocTableEmployed, AllocPart* AllocTableFree, List * wait_list, List * todo_list, FILE* output);

