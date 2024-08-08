#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// James Phillips
// 7/25/2024
// Lab Sorting Experiment
// Program to compare processing time of 6 different sorting methods

const int LENGTH = 100000;
const int MAXVAL = 100000000;

void fillArr(int arr[], int length){
  for(int i = 0; i < length; i++){
    arr[i] = rand() % MAXVAL + 1;
  }
}

void selectionSort(int arr[], int length){
    int index, temp;
    
    for (int i = 0; i < length - 1; i++){
      index = i;
      
      for (int j = i + 1; j < length; j++){
        if (arr[j] < arr[index])
          index = j;
      }

      temp = arr[i];
      arr[i] = arr[index];
      arr[index] = temp;
    }
}

void bubbleSort(int arr[], int length)
{
  int i, j, temp;
  
  for (i = 0; i < length-1; i++){
    for (j = 0; j < length-i-1; j++){
      if (arr[j] > arr[j+1]){
        temp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = temp;
      }
    }
  }
}

void insertionSort(int arr[], int left, int right)
{
  int i, item, j;
  
  for (i = left + 1; i <= right; i++) {
    item = arr[i];
    
    for(j = i - 1; j >= left; j--){
      if(arr[j]>item)
        arr[j+1] = arr[j];
      else
        break;
    }
  arr[j+1] = item;
  }
}

void merge(int arr[], int left, int mid, int right){
  int a = 0, b = 0, c = left;
  int n1 = mid - left + 1;
  int n2 = right - mid;

  int* tempL = malloc(sizeof(int) * n1);
  int* tempR = malloc(sizeof(int) * n2);

  for(int i = 0; i < n1; i++)
    tempL[i] = arr[left + i];
  for(int j = 0; j < n2; j++)
    tempR[j] = arr[mid + 1 + j];

  while(a < n1 && b < n2){
    if(tempL[a] <= tempR[b]){
      arr[c] = tempL[a];
      a++;
    }
    else{
      arr[c] = tempR[b];
      b++;
    }
    c++;
  }

  while(a < n1){
    arr[c] = tempL[a];
    a++;
    c++;
  }

  while(b < n2){
    arr[c] = tempR[b];
    b++;
    c++;
  }

  free(tempL);
  free(tempR);
}

void mergeSort(int arr[], int left, int right){
  if(right > left){
    int mid = (left + right)/2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }
}

void mergeSortInsertion(int arr[], int left, int right){
  if(right > left){
    if(right - left + 1 <= 25)
      insertionSort(arr, left, right);
    else{
      int mid = (left + right)/2;
      mergeSortInsertion(arr, left, mid);
      mergeSortInsertion(arr, mid + 1, right);
      merge(arr, left, mid, right);
    }
  }
}

// Quick Sort Partition
int partition(int arr[], int low, int high){
  int temp, index = low, i = low + rand()%(high - low + 1);

  temp = arr[low];
  arr[low] = arr[i];
  arr[i] = temp;

  index++;

  while(low <= high){
    while(low <= high && arr[low] <= arr[index])
      low++;

    while(high >= low && arr[high] > arr[index])
      high--;

    if(low < high){
      temp = arr[low];
      arr[low] = arr[high];
      arr[high] = temp;
    }
  }

  temp = arr[index];
  arr[index] = arr[high];
  arr[high] = temp;

  return high;
}

void quickSort(int arr[], int low, int high){
  if(low < high){
    int split = partition(arr, low, high);
    quickSort(arr, low, split - 1);
    quickSort(arr, split + 1, high);
  }
}

// Time calculation
long timediff(clock_t t1, clock_t t2){
  return ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;
}

int main(void) {
  srand(time(0));
  clock_t start, end;
  int sizes[5] = {10000, 20000, 30000, 50000, 100000};
  char names[6][100] = {"Selection", "Bubble", "Insertion", "Merge", "Merge + Insertion", "Quick"}; // Easier printing
  int arr[LENGTH];

  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 6; j++){
      fillArr(arr, LENGTH); // Refresh random numbers
      start = clock();
  
      if(j == 0)
        selectionSort(arr, sizes[i]);
      else if(j == 1)
        bubbleSort(arr, sizes[i]);
      else if(j == 2)
        insertionSort(arr, 0, sizes[i] - 1);
      else if(j == 3)
        mergeSort(arr, 0, sizes[i] - 1);
      else if(j == 4)
        mergeSortInsertion(arr, 0, sizes[i] - 1);
      else
        quickSort(arr, 0, sizes[i] - 1);

      end = clock();
      
      printf("\nSorting %d values takes %ld milliseconds for %s sort!", sizes[i], timediff(start, end), names[j]);
    }
  }
}