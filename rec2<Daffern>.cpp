/* CSE 310: Recitation 2
   Kaytlyn Daffern
   1221263225
   05/30/2023
   Compare performances of running time between Insertion Sort, Selection Sort, Quick Sort, and Merge Sort.
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
using namespace std;

//code mimicking the pseudocode for insertion sort
void InsertionSort(double A[], int n) {
    //iterates through the array comparing
    for (int j = 1; j < n; j++) {
        double key = A[j];
        int i = j - 1;
        while (i >= 0 && A[i] > key) {
            A[i + 1] = A[i];
            i = i - 1;
        }
        A[i + 1] = key;
    }
}

//code mimicking the pseudocode for selection sort
void SelectionSort(double A[], int n) {
    //iterates through the array comparing and if the next location is smaller they switch
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (A[j] < A[minIndex])
                minIndex = j;
        }
        swap(A[minIndex], A[i]);
    }
}
//helper method used for quick sort
int Partition(double A[], int p, int r) {
    double x = A[r];
    int i = p - 1;
    //iterates through the array swapping as needed
    for (int j = p; j <= r - 1; j++) {
        if (A[j] <= x) {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i + 1], A[r]);
    return i + 1;
}

//mimicking the pseudocode for quick sort
void QuickSort(double A[], int p, int r) {
    //checks if p or 0 is < than n - 1 or r and partitions through the array
    if (p < r) {
        int q = Partition(A, p, r);
        QuickSort(A, p, q - 1);
        QuickSort(A, q + 1, r);
    }
}

//helper method for merge sort
void Merge(double A[], int p, int q, int r) {
    //instantiates two new arrays to separate the list and merge again
    int n1 = q - p + 1;
    int n2 = r - q;
     // Dynamic allocated arrays
    double* L = new double[n1 + 1];
    double* R = new double[n2 + 1];
    for (int i = 0; i < n1; i++)
        L[i] = A[p + i];
    for (int j = 0; j < n2; j++)
        R[j] = A[q + 1 + j];
    L[n1] = INFINITY;
    R[n2] = INFINITY;
    int i = 0, j = 0;
    for (int k = p; k <= r; k++) {
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i++;
        } else {
            A[k] = R[j];
            j++;
        }
    }
    // Free the dynamically allocated memory
    delete[] L;
    delete[] R;
}

//mimicking the pseudocode for merge sort
void MergeSort(double A[], int p, int r) {
    //separates the array into smaller parts until they can be added back together in the correct order
    if (p < r) {
        int q = (p + r) / 2;
        MergeSort(A, p, q);
        MergeSort(A, q + 1, r);
        Merge(A, p, q, r);
    }
}

int main() {
    //used to check the run times of each sorting method on the arrays
    srand(time(0));

    // instantiates the array sizes to be compared with the sorting arrays
    int sizes[] = {10000, 25000, 50000, 100000, 150000, 200000};
    //instantiates the number of sizes in the sizes array
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    
    //runs through the different array sizes for each sorting algorithm
    for (int i = 0; i < numSizes; i++) {
        int n = sizes[i];

        // instantiates the array with random values
        double* A = new double[n];
        for (int j = 0; j < n; j++)
            A[j] = (rand() / static_cast<double>(RAND_MAX)) * 900.0 + 100.0;

        //Using different instantiated arrays copying A to avoid running the same array multiple times
        
        //instantiates the array B and fills it with array A
        double* B = new double[n];
        copy(A, A + n, B);
        //starts the time for Insertion Sort and runs the array through its algorithm
        clock_t startTime = clock();
        InsertionSort(B, n);
        //ends the time
        clock_t endTime = clock();
        //creates the difference between the end and start to get our insertion time for this array
        double InsertionTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

        //instantiates the array C and fills it with array A
        double* C = new double[n];
        copy(A, A + n, C);
        //starts the time for Selection Sort and runs the array through its algorithm
        startTime = clock();
        SelectionSort(C, n);
        //ends the time
        endTime = clock();
        //creates the difference between the end and start to get our selection time for this array
        double SelectionTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

        //instantiates the array D and fills it with array A
        double* D = new double[n];
        copy(A, A + n, D);
        //starts the time for Quick Sort and runs the array through its algorithm
        startTime = clock();
        QuickSort(D, 0, n - 1);
        //ends the time
        endTime = clock();
        //creates the difference between the end and start to get our quick time for this array
        double QuickTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

        //instantiates the array E and fills it with array A
        double* E = new double[n];
        copy(A, A + n, E);
        //starts the time for Quick Sort and runs the array through its algorithm
        startTime = clock();
        MergeSort(E, 0, n - 1);
        //ends the time
        endTime = clock();
        //creates the difference between the end and start to get our merge time for this array
        double MergeTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

        // Output the execution times to compare between array sizes and sorting algorithms
        cout << "The array size being ran: " << n << endl;
        cout << "Insertion-Sort time (seconds): " << InsertionTime << endl;
        cout << "Selection-Sort time (seconds): " << SelectionTime << endl;
        cout << "Quick-Sort time (seconds): " << QuickTime << endl;
        cout << "Merge-Sort time (seconds): " << MergeTime << endl;
        cout << endl;

        // removes the arrays before the next loop through for a new array size
        delete[] A;
        delete[] B;
        delete[] C;
        delete[] D;
        delete[] E;
    }

    return 0;
}
