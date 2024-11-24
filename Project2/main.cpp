//
// Created by Andrew on 11/23/2024.
//

#include "System.hpp"

int main()
{
    std::ifstream systemState("systemSnapshot.txt");
    System OS(systemState);
    //OS.debugPrint();
    OS.print();
}