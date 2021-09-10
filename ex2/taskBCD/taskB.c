#include "array.h"
#include <stdio.h>

int main(void){
    Array a = array_new(2);

    for (int i = 0; i<100; i++) {
        array_insertBack(&a, (long)i);
    }
    array_destroy(a);
}