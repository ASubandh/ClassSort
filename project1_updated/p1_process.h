#ifndef __P1_PROCESS
#define __P1_PROCESS

#include <vector>

// Student struct
struct student {
  unsigned long id;
  double grade;

  student(unsigned long id, double grade) {
    this->id = id;
    this->grade = grade;
  }
  student(){
    this->id = 0;
    this->grade = 0;
  }
  //test function for helping me see if this went through correctly

  void display() {
    printf("Student ID: %l", id);
    printf("First Name: %f",grade);
    //std::cout << "   First Name: " << grade << std:: end;
    //std::cout << endl;
  }

};

void create_processes_and_sort(std::vector<std::string>, int, int);
void process_classes(std::vector<std::string> classes, int num_threads);
//std::vector<student> parseCSV(const std::string& filename);
#endif
