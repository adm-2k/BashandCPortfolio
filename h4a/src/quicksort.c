#include <stdio.h>
#include <string.h>
#include "quicksort.h"

/* Static (private to this file) function prototypes. */
static void swap(void *a, void *b, size_t size);
static int lomuto(void *array, int left, int right, size_t elem_sz,
                  int (*cmp)(const void *, const void *));
static void quicksort_helper(void *array, int left, int right, size_t elem_sz,
                             int (*cmp)(const void *, const void *));

/**
 * Swaps the values in two pointers.
 *
 * Casts the void pointers to type (char *) and works with them as char pointers
 * for the remainder of the function. Swaps one byte at a time, until all 'size'
 * bytes have been swapped. For example, if ints are passed in, size will be 4
 * and this function will swap 4 bytes starting at a and b pointers.
 */
static void swap(void *a, void *b, size_t size)
{
    char temp;            //
    char *pa = (char *)a; // cast a to char pointer
    char *pb = (char *)b; // cast b to char pointer

    // iterate through the size of the array, swapping elements
    for (size_t i = 0; i < size; i++)
    {
        temp = pa[i];  // store element at a in temp
        pa[i] = pb[i]; // swap elements at a and b
        pb[i] = temp;  // store element in temp at b (temp = a)
    }
}

/**
 * Partitions array around a pivot, utilizing the swap function. Each time the
 * function runs, the pivot is placed into the correct index of the array in
 * sorted order. All elements less than the pivot should be to its left, and all
 * elements greater than or equal to the pivot should be to its right.
 */
static int lomuto(void *array, int left, int right, size_t elem_sz,
                  int (*cmp)(const void *, const void *))
{
    // cast array to char pointer to do pointer arithmetic
    char *arr = (char *)array; // char array to access elements
    // pivot becomes reference point for partitioning the array
    void *pivot = arr + left * elem_sz;
    int s = left; // s tracks position where next element less than pivot will go

    // iterate through array, swapping elements less than pivot to the left
    // starts from left + 1 because pivot is already in the leftmost position
    for (int i = left + 1; i <= right; i++)
    {
        if (cmp(arr + i * elem_sz, pivot) < 0) // compare current element i with pivot
        {
            s++;
            // ensures that all elements less than pivot are to the left of pivot
            swap(arr + s * elem_sz, arr + i * elem_sz, elem_sz); // swap elements s and i
        }
    }
    swap(arr + left * elem_sz, arr + s * elem_sz, elem_sz);
    return s;
}

/**
 * Sorts with lomuto partitioning, with recursive calls on each side of the
 * pivot.
 * This is the function that does the work, since it takes in both left and
 * right index values.
 * Takes in a void pointer to the array to be sorted (*array), the left and right indices (left and right)
 * of the array, the size of each element in the array (elem_sz), and a comparison function (int (*cmp) ...) .
 */
static void quicksort_helper(void *array, int left, int right, size_t elem_sz,
                             int (*cmp)(const void *, const void *))
{
    if (left < right)
    {
        int s = lomuto(array, left, right, elem_sz, cmp);
        quicksort_helper(array, left, s - 1, elem_sz, cmp);
        quicksort_helper(array, s + 1, right, elem_sz, cmp);
    }
}

/**
 * Quicksort function exposed to the user.
 * Calls quicksort_helper with left = 0 and right = len - 1.
 */
void quicksort(void *array, size_t len, size_t elem_sz,
               int (*cmp)(const void *, const void *))
{
    quicksort_helper(array, 0, len - 1, elem_sz, cmp);
}

/**
 * Comparison function for integers.
 */
int int_cmp(const void *a, const void *b)
{
    int int_a = *((int *)a);
    int int_b = *((int *)b);

    if (int_a == int_b)
        return 0;
    else if (int_a < int_b)
        return -1;
    else
        return 1;
}

/**
 * Comparison function for doubles.
 */
int dbl_cmp(const void *a, const void *b)
{
    double dbl_a = *((double *)a);
    double dbl_b = *((double *)b);

    if (dbl_a == dbl_b)
        return 0;
    else if (dbl_a < dbl_b)
        return -1;
    else
        return 1;
}

/**
 * Comparison function for strings.
 * Refers to the strcmp function to compare two strings.
 */
int str_cmp(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}
