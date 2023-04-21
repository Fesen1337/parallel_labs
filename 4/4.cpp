#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <omp.h>

#define N 13

void print_vector(char* vec, size_t size, const char* start_text) {
    printf("%s", start_text);
    for (size_t i = 0; i < size; i++) printf("%d ", vec[i]);
        printf("\n\n");
}
int main() {
    char a[N];
    size_t i;
    int max = 0;
    srand(time(NULL));
    for (i = 0; i < N; i++) {
        a[i] = rand() % 10 + 1;
    }
    print_vector(a, N, "vector a: ");
    #pragma omp parallel for schedule(static, 1) shared(a) private(i)
    for (i = 0; i < N; i++)
        if (max < a[i])
            max = a[i];
    printf("max = %d\n", max);
}