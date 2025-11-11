#include <stdio.h>

float add(float a, float b) {
    return a + b;
}

int main() {
    float flt1;
    float flt2; 
    float result;

    flt1 = flt2 = 0; // initialize

    if (scanf("%f %f", &flt1, &flt2) == 2) {
        result = add((float)flt1, (float)flt2);
        printf("Sum = %.2f", result);
        return 0;
    }
    else return -1;
}