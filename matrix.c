#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h> 

// Value depend on System core 
#define CORE 4

#define MAX 5

// Maximum threads is equal to total core of system 
// pthread_t thread[CORE * 3]; 
// pthread_t thread[4]; 

pthread_t thread[MAX * 3];
// int MAX = 0;

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

// int MAX;
int step_i = 0;

void fillMatrix(int matrix[MAX][MAX]) {
  for(int i = 0; i<MAX; i++) {
    for(int j = 0; j<MAX; j++) {
      matrix[i][j] = rand()%10+1;
    }
  }
}

void printMatrix(int matrix[MAX][MAX]) {
  for(int i = 0; i<MAX; i++) {
    for(int j = 0; j<MAX; j++) {
      printf("%5d", matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
  int x, y;
  intptr_t core = (intptr_t)args;

  // Each thread computes 1/4th of matrix addition
  for (x = core * MAX / 4; x < (core + 1) * MAX / 4; x++) {
    for (y = 0; y < MAX; y++) {
      matSumResult[x][y] = matA[x][y] + matB[x][y];
    }
  }  
  // free(args);
  return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
  int x, y;
  intptr_t core = (intptr_t)args;

  // Each thread computes 1/4th of matrix difference
  for (x = core * MAX / 4; x < (core + 1) * MAX / 4; x++) {
    for (y = 0; y < MAX; y++) {
      matDiffResult[x][y] = matA[x][y] - matB[x][y];
    }
  }  
  // free(args);
  return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
  int j, k, i = step_i++; //i denotes row number of resultant matC
   
  for (j = 0; j < MAX; j++) {
    for (k = 0; k < MAX; k++) {
      matProductResult[i][j] += matA[i][k] * matB[k][j];
    }
  }
      
  return NULL;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main(char **args) {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    int i, step = 0;

    // 0. Get the matrix size from the command line and assign it to MAX
    // MAX = atoi(args[1]);
    
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    
    // 4. Create a thread for each cell of each matrix operation.

    for (i = 0; i < MAX; i++) {
      if (pthread_create(&thread[i], NULL, &computeSum, (void *)(intptr_t)step) != 0) {
        perror("Failed to create thread!");
      }
      if (pthread_create(&thread[i + MAX], NULL, &computeDiff, (void *)(intptr_t)step) != 0) {
        perror("Failed to create thread!");
      }
      if (pthread_create(&thread[i + MAX * 2], NULL, &computeProduct, (void *)(intptr_t)step) != 0) {
        perror("Failed to create thread!");
      }
      step++;
    }

    // 
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute.
    // 
    // One way to do this is to malloc memory for the thread number i, populate the coordinates
    // into that space, and pass that address to the thread. The thread will use that number to calcuate 
    // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
    
    // 5. Wait for all threads to finish.
    // pthread_exit(NULL);
    for (i = 0; i < MAX * 3; i++) {
      pthread_join(thread[i], NULL);
    }
    
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
  
}