#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Definição do tipo de função de ordenação
typedef int* (*SortingFunction)(int*, int);

// Função auxiliar para criar cópias de arrays
int* copyArray(const int* src, int size) {
    int* dest = (int*)malloc(size * sizeof(int));
    memcpy(dest, src, size * sizeof(int));
    return dest;
}

// Função de ordenação Bubble Sort
int* bubbleSort(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return arr;
}

// Função de ordenação Counting Sort
int* countingSort(int* arr, int n) {
    // Encontrar o valor máximo no array
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    
    // Criar um array de contagem
    int* count = (int*)calloc(max + 1, sizeof(int));

    // Contar as ocorrências de cada elemento
    for (int i = 0; i < n; i++) {
        count[arr[i]]++;
    }

    // Reconstruir o array ordenado
    int* sortedArray = (int*)malloc(n * sizeof(int));
    int index = 0;
    for (int i = 0; i <= max; i++) {
        while (count[i] > 0) {
            sortedArray[index++] = i;
            count[i]--;
        }
    }
    
    free(count);
    memcpy(arr, sortedArray, n * sizeof(int));
    free(sortedArray);
    return arr;
}

// Função de ordenação Insertion Sort
int* insertionSort(int* arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && key < arr[j]) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
    return arr;
}

// Função auxiliar para merge
void merge(int* arr, int* left, int leftSize, int* right, int rightSize) {
    int i = 0, j = 0, k = 0;
    while (i < leftSize && j < rightSize) {
        if (left[i] < right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < leftSize) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < rightSize) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

// Função de ordenação Merge Sort
int* mergeSort(int* arr, int n) {
    if (n > 1) {
        int mid = n / 2;
        int* left = copyArray(arr, mid);
        int* right = copyArray(arr + mid, n - mid);

        left = mergeSort(left, mid);
        right = mergeSort(right, n - mid);

        merge(arr, left, mid, right, n - mid);
        
        free(left);
        free(right);
    }
    return arr;
}

// Função auxiliar para particionamento do QuickSort
int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return i + 1;
}

// Função principal do QuickSort
void quickSortHelper(int* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortHelper(arr, low, pi - 1);
        quickSortHelper(arr, pi + 1, high);
    }
}

// Função de ordenação Quick Sort
int* quickSort(int* arr, int n) {
    quickSortHelper(arr, 0, n - 1);
    return arr;
}

// Função de ordenação Selection Sort
int* selectionSort(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
    return arr;
}

// Estrutura para representar um balde
typedef struct {
    int* elements;
    int count;
    int capacity;
} Bucket;

// Inicializar um balde
void initBucket(Bucket* bucket, int initialCapacity) {
    bucket->elements = (int*)malloc(initialCapacity * sizeof(int));
    bucket->count = 0;
    bucket->capacity = initialCapacity;
}

// Adicionar elemento a um balde
void addToBucket(Bucket* bucket, int value) {
    if (bucket->count == bucket->capacity) {
        bucket->capacity *= 2;
        bucket->elements = (int*)realloc(bucket->elements, bucket->capacity * sizeof(int));
    }
    bucket->elements[bucket->count++] = value;
}

// Liberar memória de um balde
void freeBucket(Bucket* bucket) {
    free(bucket->elements);
    bucket->count = 0;
    bucket->capacity = 0;
}

// Função de ordenação Bucket Sort
int* bucketSort(int* arr, int n, SortingFunction internalSort) {
    // Encontrar o valor máximo no array
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    // Definir o número de baldes
    int numBuckets = 10;
    Bucket* buckets = (Bucket*)malloc(numBuckets * sizeof(Bucket));
    for (int i = 0; i < numBuckets; i++) {
        initBucket(&buckets[i], 10);
    }

    // Distribuir os elementos nos baldes
    for (int i = 0; i < n; i++) {
        int bucketIndex = (arr[i] * numBuckets) / (max + 1);
        addToBucket(&buckets[bucketIndex], arr[i]);
    }

    // Ordenar cada balde usando o algoritmo interno
    for (int i = 0; i < numBuckets; i++) {
        internalSort(buckets[i].elements, buckets[i].count);
    }

    // Juntar os baldes ordenados
    int index = 0;
    for (int i = 0; i < numBuckets; i++) {
        for (int j = 0; j < buckets[i].count; j++) {
            arr[index++] = buckets[i].elements[j];
        }
        freeBucket(&buckets[i]);
    }
    
    free(buckets);
    return arr;
}

// Função para medir o tempo de execução de cada algoritmo
long measureTime(SortingFunction function, int* arr, int size) {
    int* arrCopy = copyArray(arr, size);
    
    clock_t start = clock();
    function(arrCopy, size);
    clock_t end = clock();
    
    long timeTaken = (long)((end - start) * 1000 / CLOCKS_PER_SEC);
    printf("%ld milisegundos\n", timeTaken);
    
    free(arrCopy);
    return timeTaken;
}

// Função para gerar um array de números aleatórios
int* generateRandomArray(int size) {
    int* dataSet = (int*)malloc(size * sizeof(int));
    srand(time(NULL));

    for (int i = 0; i < size; i++) {
        dataSet[i] = rand() % (size + 1);
    }

    return dataSet;
}

// Função para encontrar o algoritmo mais rápido
const char* findFastestAlgorithm(long* times, int count) {
    long minTime = times[0];
    int minIndex = 0;

    const char* algorithms[] = {
        "Bubble Sort", "Counting Sort", "Insertion Sort", 
        "Merge Sort", "Quick Sort", "Selection Sort"
    };

    for (int i = 1; i < count; i++) {
        if (times[i] < minTime) {
            minTime = times[i];
            minIndex = i;
        }
    }

    return algorithms[minIndex];
}

int main() {
    int size = 200000;
    int* randomArray = generateRandomArray(size);

    // Tamanhos dos subarrays a serem testados
    int sizes[] = {100, 500, 1000, 5000, 30000, 80000, 100000, 150000, 200000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    // Testa cada algoritmo para cada tamanho de subarray
    for (int s = 0; s < numSizes; s++) {
        int currentSize = sizes[s];
        int* subarray = copyArray(randomArray, currentSize);
        
        printf("\nTamanho do Subarray: %d\n", currentSize);
        
        long times[6];
        
        // Bucket Sort com Bubble Sort interno
        printf("Bucket Sort (Bubble Sort): ");
        times[0] = measureTime(bubbleSort, subarray, currentSize);

        // Bucket Sort com Counting Sort interno
        printf("Bucket Sort (Counting Sort): ");
        times[1] = measureTime(countingSort, subarray, currentSize);

        // Bucket Sort com Insertion Sort interno
        printf("Bucket Sort (Insertion Sort): ");
        times[2] = measureTime(insertionSort, subarray, currentSize);

        // Bucket Sort com Merge Sort interno
        printf("Bucket Sort (Merge Sort): ");
        times[3] = measureTime(mergeSort, subarray, currentSize);

        // Bucket Sort com Quick Sort interno
        printf("Bucket Sort (Quick Sort): ");
        times[4] = measureTime(quickSort, subarray, currentSize);

        // Bucket Sort com Selection Sort interno
        printf("Bucket Sort (Selection Sort): ");
        times[5] = measureTime(selectionSort, subarray, currentSize);

        // Comparar os tempos e identificar o algoritmo mais rápido
        const char* fastestAlgorithm = findFastestAlgorithm(times, 6);
        printf("O algoritmo interno mais rápido para Bucket Sort foi: %s\n", fastestAlgorithm);
        
        free(subarray);
    }
    
    free(randomArray);
    return 0;
}