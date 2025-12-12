#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>
#include <ctime>
#include <sstream>
#include "p1_process.h"
#include "p1_threads.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

// This file implements the multi-processing logic for the project


// This function should be called in each child process right after forking
// The input vector should be a subset of the original files vector
void process_classes(vector<string> classes, int num_threads) {


  printf("Child process is created. (pid: %d)\n", getpid());
  // Each process should use the sort function which you have defined
  // in the p1_threads.cpp for multithread sorting of the data.

  for (int i = 0; i < classes.size(); i++) {
    // get all the input/output file names here
    string class_name = classes[i];
    char buffer[40];
    sprintf(buffer, "input/%s.csv", class_name.c_str());
    string input_file_name(buffer);

    sprintf(buffer, "output/%s_sorted.csv", class_name.c_str());
    string output_sorted_file_name(buffer);

    sprintf(buffer, "output/%s_stats.csv", class_name.c_str());
    string output_stats_file_name(buffer);


    // Your implementation goes here, you will need to implement:

    // File I/O
    vector<student> students;
    // File pointer
    ifstream fin;
    // Open an existing file
    //printf("opening an input file, does it even get here??\n");
    fin.open(input_file_name.c_str(), ios::in);
    if (fin.is_open()) {
    std::cout << "File opened successfully" << std::endl;
    //int counter = 0;
    // You can now read or write to the file
    std::string line;
    unsigned long id;
    double grade;
    string tempString;

    int randPrint = 0;
    getline(fin, line);

    while (getline(fin, line)) {
      //if(counter!= 0){
        //std::getline("input/algorithm.csv", line);
        replace(line.begin(),line.end(),',',' ');
        stringstream inputString(line);
        inputString>>id>>grade;
        inputString>>grade;
        /*
        //put id into variable
        std::getline(inputString, tempString);
        id = atol(tempString.c_str());
        //put grade into variable
        std::getline(inputString, tempString);
        grade = atof(tempString.c_str()); */
        struct student tmpstudent(id,grade);
        students.push_back(tmpstudent);

        /*
        if(randPrint == 4){
          std::cout << "Student ID: " << id << " Grade: " << grade <<std::endl;
        }
        */
        //std::cout << line << std::endl;
        //randPrint++;
    //}
  }
    std::cout << "Done reading lines of: " << input_file_name.c_str() << std::endl;
    fin.close(); // Remember to close the file after you're done using it
      //return students;
    } else {
    // File could not be opened
    std::cout << "Failed to open the file." << std::endl;
    }

    // Run multi threaded sort
    ParallelMergeSorter sorter(students, num_threads);
    vector<student> sortedList = sorter.run_sort();

    ofstream fout;
    fout.open(output_sorted_file_name.c_str());
    if (!fout) {
        std::cerr << "Error opening the output file." << std::endl;
        //return;
    }
    fout<<"Rank,Student ID,Grade\n";
    for(int i = 0;i<students.size();i++){
      //int rank = i+1;
      //fout<<students[i].id<<','<<setprecision(13)<<students[i].grade<<"\n";
      fout<<i+1<<','<<sortedList[i].id<<','<<setprecision(13)<<sortedList[i].grade<<"\n";
    }
    
    std::cout << "Finished Outputting: " << input_file_name.c_str() << " by process: " << getpid() <<std::endl;
    fout.close();



    //  - This means reading the input file, and creating a list of students,


    //  see p1_process.h for the definition of the student struct
    //

    // Open an existing file
    //  - Also, once the sorting is done and the statistics are generated, this means
    //  creating the appropritate output files
    //
    // Multithreaded Sorting
    //  - See p1_thread.cpp and p1_thread.h
    //
    //  - The code to run the sorter has already been provided
    //
    // Generating Statistics
    //  - This can be done after sorting or during



    //make output stream - open output_sorted_file_name(id, grade)
    //for 2nd
    //process = do entire file separetly
    //mutlithread  = splits up the  file and works in each of them
    //process: make only child processs do work!!!
    //greater than 0 - parent acts -
    // last process: do remainder of work - last process gets 2 process
  }
  // child process done, exit the program
  printf("Child process is terminated. (pid: %d)\n", getpid());

  //uncomment below if the program breaks
  exit(0);
}





void create_processes_and_sort(vector<string> class_names, int num_processes, int num_threads) {
  vector<pid_t> child_pids;
  //push back the main pid
  //child_pids.push_back(getpid());
  //cout << "Pushed back main process: " << getpid() <<endl;
  int classes_per_process = max(class_names.size() / num_processes, 1ul);
  cout << "Processes: " << num_processes << " Threads: " << num_threads << " Classes per process:" << classes_per_process <<"\n";
  // This code is provided to you to test your sorter, this code does not use any child processes
  // Remove this later on
  //pid_t p = getpid();
  
  //create a vector of vectors
  vector<vector<string> >classVector;

  //splitting the input vector into sublists and then inserting them back into a 2d vector
  //doesnt work when u have 2 as the number of processes!. Need to fix.
  int i = 0;
  int k = 0;
  while (k < num_processes){
    vector<string> temp;
    while(temp.size() < classes_per_process && i < class_names.size()) {
      //cout << i;
      temp.push_back(class_names[i]);
      //cout << "Pushed back: " << class_names[i]<<endl;
      i++;
    }
    classVector.push_back(temp);
    cout << "Sublist "<< (k+1) << " created!" <<endl;
    k++;
  }

  //put in the rest of classes to the last sublist
  while (i < class_names.size()){
    classVector[num_processes-1].push_back(class_names[i]);
    //cout << "Pushed back: " << class_names[i]<<endl;
    i++;
  }

  cout << "Sublist size: " << classVector.size()<<endl;
  //cout << "Exiting!!" <<endl;
  //exit(0);
  //printing to see if vectors were created properly
  //for(int k = 0; k < classVector.size(); k++){
    //cout << "Vector: " << (k + 1) << "size: " << classVector[k].size();
  //}

  int j;
  //old forking code
  //new forking code is cleaner
  /*
  for(j = 0; j < classVector.size(); j++){
    pid_t p = getpid();  
    if (p < 0){
      perror("fork fail");
      exit(1);
    }
    else {
    child_pids.push_back(p);
    cout << "Pushed back: " << p <<endl;
    }
    if (fork() != 0){ //ensures that only the parent's forks do stuff
      process_classes(classVector[j], num_threads);
      //get the pid only if its not 0!
      
    }
    cout << "Fork called: " << (j + 1) << " times!\n ";
  }
  */
  for(j = 0; j < classVector.size(); j++){
    pid_t p = fork();
    if (p == 0){ //child trying to fork
      process_classes(classVector[j], num_threads);
    }
    else if (p > 0){ //parent created fork - we push back all the child pids - when they try to fork, they process classes
      child_pids.push_back(p);
      cout << "Pushed back: " << p <<endl;
    }
    else {
      perror("Fork Fail!");
      //exit(0);
    }
    cout << "Fork called: " << (j + 1) << " times!\n ";
  }

  cout << "do we get here..." << endl;
  
  //waiting for each process we created
  for (int i = 0; i < num_processes; ++i) {
    int status;
    int ret = waitpid(child_pids[i], &status, 0);
      if (ret == -1) {
        perror("Error waiting for child");
      } 
      else {
        cout << "Waiting..." << endl;
      }
  }
}
  // Your implementation goes here, you will need to implement:
  // Splitting up work
  //   - Split up the input vector of classes into num_processes sublists
  //
  //   - Make sure all classes are included, remember integer division rounds down
  //
  // Creating child processes 
  //   - Each child process will handle one of the sublists from above via process_classes
  //   
  // Waiting for all child processes to terminate

