#include <vector>
#include <pthread.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "p1_process.h"
#include "p1_threads.h"

using namespace std;

// This file implements the ParallelMergeSorter class definition found in p1_threads.h
// It is not required to use the defined classes/functions, but it may be helpful




// This struct is used to pass arguments into each thread (inside of thread_init)
// ctx is essentially the "this" keyword, but since we are in a static function, we cannot use "this"

// Feel free to modify this struct in any way
struct MergeSortArgs {
  int thread_index;
  ParallelMergeSorter * ctx;

  MergeSortArgs(ParallelMergeSorter * ctx, int thread_index) {
    this->ctx = ctx;
    this->thread_index = thread_index;
  }
};


// Class constructor
ParallelMergeSorter::ParallelMergeSorter(vector<student> &original_list, int num_threads) {
  this->threads = vector<pthread_t>(num_threads);
  this->sorted_list = vector<student>(original_list);
  this->num_threads = num_threads;
}

// This function will be called by each child process to perform multithreaded sorting
vector<student> ParallelMergeSorter::run_sort() {
  //already an instance of a vector named threads in the num_threads, am i overwriting it with this?
  //pthread_t threads[num_threads];
  
  //cout << "number of threads from run_sort(): " << num_threads << "\n";

  for (int i = 0; i < num_threads; i++) {
    // We have to use the heap for this otherwise args will be destructed in each iteration,
    // and the thread will not have the correct args struct
    MergeSortArgs * args = new MergeSortArgs(this, i);
    //create threads here, and check if they are created successfully

    if (pthread_create(&threads[i], NULL, thread_init, args) != 0){
      perror("Failed to create thread!");
    }
      
      //find a way to find the size of each list
      //printf("Thread %d of %d created\n",i,num_threads);
      // Your implementation goes here, you will need to implement:
      // Creating worker threads
      //  - Each worker thread will use ParallelMergeSorter::thread_init as its start routine
      //
      //  - Since the args are taken in as a void *, you will need to use
      //  the MergeSortArgs struct to pass multiple parameters (pass a pointer to the struct)
      //
      //  - Don't forget to make sure all threads are done before merging their sorted sublists


    }
    //waits for threads to finish
    for(int i = 0; i<num_threads; i++){
      if (pthread_join(threads[i], NULL) != 0){
        perror("Failed to join thread!");
      }
    }
    /*
     * Uncomment this code for testing sorting without threads
     *
     * thread_init((void *) args);
     */

    // Your implementation goes here, you will need to implement:
    // Creating worker threads
    //  - Each worker thread will use ParallelMergeSorter::thread_init as its start routine
    //
    //  - Since the args are taken in as a void *, you will need to use
    //  the MergeSortArgs struct to pass multiple parameters (pass a pointer to the struct)
    //
    //  - Don't forget to make sure all threads are done before merging their sorted sublists



  // Merge sorted sublists together
  this->merge_threads();
  return this->sorted_list;
}

// Standard merge sort implementation
void ParallelMergeSorter::merge_sort(int low, int high) {
  // Your implementation goes here, you will need to implement:
  // Top-down merge sort
  if (low >= high){
    return;
  }

  int mid = (low + high)/2;
  merge_sort(low, mid);
  merge_sort(mid +1, high);
  //merge the two arrays together
  merge(low,mid,high);
  }



// Standard merge implementation for merge sort
void ParallelMergeSorter::merge(int low, int mid, int high) {
  // Your implementation goes here, you will need to implement:
  // Merge for top-down merge sort
  //  - The merge results should go in temporary list, and once the merge is done, the values
  //  from the temporary list should be copied back into this->sorted_list

  //initialize temporary arrays
  
  int i; int j; int k; int n1; int n2;

  n1 = mid - low + 1; //length of A[low:mid]
  n2 = high - mid; // length of A[mid +1:high]

  //initialize temporary arrays
  //struct student L[n1];
  //struct student R[n2];
  std::vector<student> L(n1);
  std::vector<student> R(n2);
  //store the values into the 2 temp arrays
    for (i = 0; i < n1; i++){
      L[i] = sorted_list[low + i];
    }

    for (j = 0; j < n2; j++){
      R[j] = sorted_list[mid + 1 + j];
    }
    i = 0; // i indexes the smallest remaining element in L
    j = 0; // j indexes the smallest remaining element in R
    k = low; // k indexes the location in A to fill
    // As long as each of the arrays L and R contains an unmerged element,
    // copy the smallest unmerged element back into A[low:high]
    while (i < n1 && j < n2) {
    //change to L[i].grade <= R[j].grade to sort from least to greatest!
    if (L[i].grade >= R[j].grade) {
        sorted_list[k] = L[i];
        i++;
    } else {
        sorted_list[k] = R[j];
        j++;
    }
    k++;
  }
  while(i < n1){
    sorted_list[k] = L[i];
    i = i + 1;
    k = k + 1;
  }

  while(j < n2){
    sorted_list[k] = R[j];
    j = j + 1;
    k = k + 1;
  }

}

// This function will be used to merge the resulting sorted sublists together
void ParallelMergeSorter::merge_threads() {
  // Your implementation goes here, you will need to implement:
  // Merging the sorted sublists together
  //  - Each worker thread only sorts a subset of the entire list, therefore once all
  //  worker threads are done, we are left with multiple sorted sublists which then need to
  //  be merged once again to result in one total sorted list
  int mid, high, low;
  int work_per_thread = sorted_list.size() / num_threads;
  
  for(int i=1; i<num_threads; i++){
    high = ((i+1)* work_per_thread)-1;
    mid = ((i)* work_per_thread)-1;
    
    //assign the leftover students to be sorted in the last thread
    if(i == num_threads -1 ){
      high = sorted_list.size()- 1;
    }
    merge(0, mid, high);
  }
}

// This function is the start routine for the created threads, it should perform merge sort on its assigned sublist
// Since this function is static (pthread_create must take a static function), we cannot access "this" and must use ctx instead
void * ParallelMergeSorter::thread_init(void * args) {
  MergeSortArgs * sort_args = (MergeSortArgs *) args;
  int thread_index = sort_args->thread_index;
  ParallelMergeSorter * ctx = sort_args->ctx;

  int work_per_thread = ctx->sorted_list.size() / ctx->num_threads;
  
  // Your implementation goes here, you will need to implement:
  // Getting the sublist to sort based on the thread index
  //  - The lower bound is thread_index * work_per_thread, the upper bound is (thread_index + 1) * work_per_thread
  //
  //  - The range of merge sort is typically [lower, upper), lower inclusive, upper exclusive
  //
  //  - Remember to make sure all elements are included in the sort, integer division rounds down
  //
  // Running merge sort
  //  - The provided functions assume a top-down implementation
  //
  //  - Many implementations of merge sort are online, feel free to refer to them (wikipedia is good)
  //
  //  - It may make sense to equivalate this function as the non recursive "helper function" that merge sort normally has
  int low = thread_index * work_per_thread;
  int high = (thread_index + 1) * work_per_thread;
  cout << "Number of threads: " << ctx->num_threads << " Low: " << low << "High: " << high << "\n";
  //if its on last thread, then the higher variable is set to the size of array -1
  if (thread_index == ctx->num_threads - 1) {
        high = ctx->sorted_list.size();
    }
  ctx->merge_sort(low, high-1);

  // Free the heap allocation
  delete sort_args;
  return NULL;
}
