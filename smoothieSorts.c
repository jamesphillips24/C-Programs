#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// James Phillips
// 7/20/24
// Program to take current location, shop locations, and
// smoothie locations. Sorts distance from shops using merge
// and insertion sorts and then validates queries using binary
// search.

// Global variables
int x, y;

typedef struct Point{
  int x;
  int y;
  double distance;
} Point;

// Take input
Point* ReadData(int* n, int* s, int* t, Point** searchPoints){
  scanf("%d %d %d %d %d", &x, &y, n, s, t);

  // Dynamically allocate shop points and calculate distances
  Point* shopPoints = malloc(sizeof(Point) * *n);
  for(int i = 0; i < *n; i++){
    scanf("%d %d", &shopPoints[i].x, &shopPoints[i].y);
    shopPoints[i].distance = sqrt(pow((x - shopPoints[i].x), 2) + pow((y - shopPoints[i].y), 2));
  }

  // Dynamically allocate search points and calculate distances
  // Query distances are calculated for binary search use
  *searchPoints = malloc(sizeof(Point) * *s);
  for(int i = 0; i < *s; i++){
    scanf("%d %d", &(*searchPoints)[i].x, &(*searchPoints)[i].y);
      (*searchPoints)[i].distance = sqrt(pow((x - (*searchPoints)[i].x), 2) + pow((y - (*searchPoints)[i].y), 2));
  }
  
  return shopPoints;
}

// Return int indicating closer point based on distance,
// x value, and then y value
int compareTo(Point first, Point second){
  if(first.distance < second.distance)
    return -1;
  else if(first.distance > second.distance)
    return 1;
  else if(first.x < second.x)
      return -1;
  else if(first.x > second.x)
    return 1;
  else if(first.y < second.y)
    return -1;
  else if(first.y > second.y)
    return 1;
  else
    return 0;
}

// Merge part of merge sort
// Use temp arrays indicating subarrays of main point list
// to insert values in ascending order.
void merge(Point* points, int left, int mid, int right){
  int a = 0, b = 0, c = left;
  int n1 = mid - left + 1;
  int n2 = right - mid;

  Point* tempL = malloc(sizeof(Point) * n1);
  Point* tempR = malloc(sizeof(Point) * n2);

  // Fill temp arrays
  for(int i = 0; i < n1; i++)
    tempL[i] = points[left + i];
  for(int j = 0; j < n2; j++)
    tempR[j] = points[mid + 1 + j];

  // Go until reaching the end of one temp array
  // Replace actual array with ascending elements
  while(a < n1 && b < n2){
    if(compareTo(tempL[a], tempR[b]) <= 0){
      points[c] = tempL[a];
      a++;
    }
    else{
      points[c] = tempR[b];
      b++;
    }
    c++;
  }

  // Insert remaining elements
  while(a < n1){
    points[c] = tempL[a];
    a++;
    c++;
  }

  // Insert remaining elements
  while(b < n2){
    points[c] = tempR[b];
    b++;
    c++;
  }

  free(tempL);
  free(tempR);
}

// Insertion sort for smaller arrays
// Move over elements until they are smaller than
// the given element. Then insert element.
void insertionSort(Point* points, int left, int right){
  Point temp;
  int j;
  for(int i = left + 1; i <= right; i++){
    temp = points[i];

    for(j = i - 1; j >= left; j--){
      if(compareTo(points[j], temp) > 0)
        points[j+1] = points[j];
      else
        break;
    }

    points[j+1] = temp;
  }
}

// Merge sort funciton
// If the sub array is t or smaller in length, call insertion
// sort. Otherwise, make recursive calls and merge.
void mergeSort(Point* points, int left, int right, int t){
  if(right > left){
    if(right - left + 1 <= t)
      insertionSort(points, left, right);
    else{
      int mid = (left + right)/2;
      mergeSort(points, left, mid, t);
      mergeSort(points, mid + 1, right, t);
      merge(points, left, mid, right);
    }
  }
}

// Wrapper function for sort
void sortWrapper(Point* points, int n, int t){
  mergeSort(points, 0, n-1, t);
}

// Binary search to find if queries are in the list
// of given points.
int binarySearch(Point* points, int n, Point point){
  int left = 0, right = n-1, mid;

  while(left <= right){
    mid = (left + right)/2;
    if(compareTo(points[mid], point) < 0)
      left = mid + 1;
    else if(compareTo(points[mid], point) > 0)
      right = mid - 1;
    else
      return mid;
  }
  return -1;
}

int main(void) {
  Point* searchPoints;
  int n, s, t;

  // Input
  Point* shopPoints = ReadData(&n, &s, &t, &searchPoints);

  // Sort
  sortWrapper(shopPoints, n, t);
  for(int i = 0; i < n; i++){
    printf("%d %d\n", shopPoints[i].x, shopPoints[i].y);
  }

  // Search and print
  for(int i = 0; i < s; i++){
    int num = binarySearch(shopPoints, n, searchPoints[i]);
    if(num == -1)
      printf("%d %d not found\n", searchPoints[i].x, searchPoints[i].y);
    else
      printf("%d %d found at rank %d\n", searchPoints[i].x, searchPoints[i].y, num+1);
  }

  // Free memory
  free(shopPoints);
  free(searchPoints);
}