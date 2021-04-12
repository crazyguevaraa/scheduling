#include <stdio.h>
#include <math.h>

unsigned char Rshl(unsigned char num,unsigned int id);
unsigned char Rshr(unsigned char num,unsigned int id);

int main ()
{
    unsigned char c;
    unsigned int n;
    scanf("%hhu", &c);
    scanf("%d", &n);
    unsigned char sus = Rshl(c, n);
    unsigned char sus_1 = Rshr(c, n);
    printf("\nВНИМАНИЕ\n");
    printf("%hhu\n", sus);
    printf("\nВНИМАНИЕ\n");
    printf("%hhu\n", sus_1);
}    
unsigned char Rshl (unsigned char num,unsigned int id)
{
    id = id % 8;
    return (num << id) | (num >> (8 - id));
}    
unsigned char Rshr (unsigned char num,unsigned int id)
{
    id = id % 8;
    return (num >> id) | (num << (8-id));
}

