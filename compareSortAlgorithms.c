#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t *ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	//printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void *ptr)
{
	size_t *pSz = (size_t *)ptr - 1;
	extraMemoryAllocated -= *pSz;
	//printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t *)ptr - 1);
}

size_t Size(void *ptr)
{
	return ((size_t *)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int i, int n)
{
    // Build Max Heap
    for (int j = (n - 1) / 2; j >= i; j--) {
        // Percolate down from the last non-leaf node to the first element
        int parent = j;
        int leftChild = 2 * parent + 1;
        int rightChild = 2 * parent + 2;

        while (leftChild < n + 1) { 
            int maxChild = leftChild;
            if (rightChild < n + 1 && arr[rightChild] > arr[leftChild])
                maxChild = rightChild;

            if (arr[parent] < arr[maxChild]) {
                // Swap parent and its max child if parent is smaller
                int temp = arr[parent];
                arr[parent] = arr[maxChild];
                arr[maxChild] = temp;

                parent = maxChild;
                leftChild = 2 * parent + 1;
                rightChild = 2 * parent + 2;
            } else {
                break;  // Heap property established
            }
        }
    }

    // Sort the heap
    for (int k = n; k >= i; k--) { // Adjusted the loop condition here
        // Swap the root of the heap with the last element
        int temp = arr[i];
        arr[i] = arr[k];
        arr[k] = temp;

        // Restore the heap property for the remaining elements
        int parent = i;
        int leftChild = 2 * parent + 1;
        int rightChild = 2 * parent + 2;

        while (leftChild < k) {
            int maxChild = leftChild;
            if (rightChild < k && arr[rightChild] > arr[leftChild])
                maxChild = rightChild;

            if (arr[parent] < arr[maxChild]) {
                // Swap parent and maximum child 
                int temp = arr[parent];
                arr[parent] = arr[maxChild];
                arr[maxChild] = temp;

                parent = maxChild;
                leftChild = 2 * parent + 1;
                rightChild = 2 * parent + 2;
            } else {
                break;  // Heap property established
            }
        }
    }
}




// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r)
	{
		// Find the midpoint
		int m = (l + r) / 2;
		// Sort first and second halves
		mergeSort(pData, l, m);
		mergeSort(pData, m + 1, r);

		int i, j, k;
		int n1 = m - l + 1;
		int n2 = r - m;

		// Create temp arrays
		int *L = Alloc(n1 * sizeof(int));
		int *R = Alloc(n2 * sizeof(int));

		// Copy data to temp arrays L[] and R[]
		for (i = 0; i < n1; i++)
			L[i] = pData[l + i];
		for (j = 0; j < n2; j++)
			R[j] = pData[m + 1 + j];

		// Merge the temp arrays back into pData[l..r]
		i = 0; // Initial index of first subarray
		j = 0; // Initial index of second subarray
		k = l; // Initial index of merged subarray
		while (i < n1 && j < n2)
		{
			if (L[i] <= R[j])
			{
				pData[k] = L[i];
				i++;
			}
			else
			{
				pData[k] = R[j];
				j++;
			}
			k++;
		}

		// Copy the remaining elements of L[], if there are any
		while (i < n1)
		{
			pData[k] = L[i];
			i++;
			k++;
		}
		// Copy the remaining elements of R[], if there are any
		while (j < n2)
		{
			pData[k] = R[j];
			j++;
			k++;
		}

		// Deallocate memory for temp arrays
		DeAlloc(L);
		DeAlloc(R);
	}
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int *pData, int n)
{
	int i, item, j;
	for (i = 1; i < n; i++)
	{
		item = pData[i];
		/* Move elements of arr[0..i-1], that are
		greater than key, to one position ahead
		of their current position */
		for (j = i - 1; j >= 0; j--)
		{
			if (pData[j] > item)
				pData[j + 1] = pData[j];
			else
				break;
		}
		pData[j + 1] = item;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int *pData, int n)
{
	// printf("\nUsing Bubble sort\n\n");
	int i, j, temp;
	for (i = 0; i < n - 1; i++)
	{
		// printf("Iteration# %d\n", i + 1);
		for (j = 0; j < n - i - 1; j++)
		{
			if (pData[j] > pData[j + 1])
			{ // then swap
				temp = pData[j];
				pData[j] = pData[j + 1];
				pData[j + 1] = temp;
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int *pData, int n)
{
	int i, j, min_idx, temp;

	// One by one move boundary of unsorted subarray  
	for (i = 0; i < n - 1; i++)

	{

		// Find the minimum element in unsorted array  

		min_idx = i;

		for (j = i + 1; j < n; j++)

			if (pData[j] < pData[min_idx])
				min_idx = j;

		// Swap the found minimum element with the first element  
		temp = pData[i];

		pData[i] = pData[min_idx];
		pData[min_idx] = temp;
		// Not super efficient because of the swapping
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE *inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			fscanf(inFile, "%d ", &n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i = 0; i < 100; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\t");

	for (i = sz; i < dataSz; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
	double cpu_time_used;
	char *fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};

	for (i = 0; i < 3; ++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);

		if (dataSz <= 0)
			continue;

		pDataCopy = (int *)Alloc(sizeof(int) * dataSz);

		printf("---------------------------\n");
		printf("Dataset Size : %d\n", dataSz);
		printf("---------------------------\n");

		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
}
