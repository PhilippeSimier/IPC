/************************************************
*   programme test pour provoquer un l'émision
*   d'un signal suite à une erreur de calcul
*************************************************/
#include <stdio.h>
#include <stdlib.h>


int main()
{

    int a,b;
    int res;
    a = 5;
    b = 0;
    printf("test erreur division par zéro \n");
    res = a/b;

    return 0;
}
