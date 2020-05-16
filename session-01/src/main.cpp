#include <Arduino.h>

/*
void swap(uint8_t *A, uint8_t *B)
{
    uint8_t C = *A;
    *A = *B;
    *B = C;
}
*/

void swap(void *A, void *B, size_t num)
{
    uint8_t c[num];
    uint8_t *a = (uint8_t *)A;
    uint8_t *b = (uint8_t *)B;
    for(size_t i=0; i<num; i++)
    {
        c[i] = a[i];
        a[i] = b[i];
        b[i] = c[i];
    }
}

int comp2(void *A, void *B)
{
    uint32_t *a = (uint32_t*)A;
    uint32_t *b = (uint32_t*)B;
    return !(*a < *b);
}

int comp1(void *A, void *B)
{
    float *a = (float*)A;
    float *b = (float*)B;
    return !(*a < *b);
}

void sort(void *arr, size_t num, size_t size, int (*comp)(void *, void*))
{
    uint8_t *ARR = (uint8_t*)arr;
    for(size_t j=0; j<num-1; j++)
    {
        for(size_t i=j+1; i<num; i++)
        {
            if (comp(&ARR[i*size], &ARR[j*size]))
                swap(&ARR[i*size], &ARR[j*size], size);
        }
    }
}

void setup()
{

}

float arr1[]={5,10,20};
uint32_t arr2[]={5,10,20};

void loop()
{
    sort(arr1, sizeof(arr1)/sizeof(float), sizeof(float), comp1);
    sort(arr2, sizeof(arr2)/sizeof(uint32_t), sizeof(uint32_t), comp2);
    delay(1);
}