/************************************************
*   programme test pour provoquer une violation
*   de la segmentation mémoire
*************************************************/
#include <stdio.h>
#include <stdlib.h>


int main()
{

    char buffer[10];
    int k;
    printf("test erreur de la segmentation mémoire \n");
    for(k=0; ; k++){
	buffer[k] = 'A';
    }
    return 0;
}
