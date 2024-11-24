//
// Created by Andrew on 11/23/2024.
//

#ifndef KENT_STATE_OS_RESOURCES_H
#define KENT_STATE_OS_RESOURCES_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <array>

struct Process
{
    Process(std::string);
    void print() {std::cout << name;};
    void printMax();
    void printAllocated();
    void printNeeded();
    std::string name;
    std::map<char, int> resources; //Name of resource along with number allocated/max
};

class System
{
public:
    //Constructors
    System() = default;
    System(std::ifstream&);

    //Prints out safe order if system is safe
    //Otherwise prints out "unsafe order"
    void print();

    //Prints system overview to ensure file is being read correctly
    void debugPrint();
private:
    //Evaluates if order is safe, this is where Banker's
    //Algorithm Safety Part is implemented.
    std::pair<bool, std::vector<std::string>> isSafe();

    //std::vector of allocated, and max resources;
    std::vector<Process*> allocated;
    std::vector<Process*> maximum;

    //Needed for Bankers algorithm; max number of resources each process may need.
    std::vector<Process*> need;

    //std::map of maximum/available number of instances of each resource keyed by resource name
    std::map<char, int> instances;
    std::map<char, int> available;
};


#endif //KENT_STATE_OS_RESOURCES_H
