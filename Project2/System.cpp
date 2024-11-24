//
// Created by Andrew on 11/23/2024.
//

#include "System.hpp"

#include <utility>

System::System(std::ifstream& input)
{
    //Each line of the file corresponds
    std::string currentLine;
    std::stringstream tokens;
    std::string currToken;
    char currentToken;
    std::vector<char> resourceNames;
    std::vector<std::string> processNames;
    //First line of the file format is names system resources
    std::getline(input, currentLine);
    tokens.str(currentLine);
    while (tokens >> currentToken)
    {
        //std::cout << currentToken << " ";
        resourceNames.push_back(currentToken);
    }

    //Second Line contains max available instances of each resource
    tokens.clear();
    std::getline(input, currentLine);
    tokens.str(currentLine);
    int i = 0;
    while (tokens >> currToken)
    {
        int value = std::stoi(currToken);
        instances.insert(std::pair<char, int>(resourceNames[i], value));
        //std::cout << currentToken << " ";
        //instances[resourceNames[i]] = (currentToken - '0');
        i++;
    }

    /*
     * Remaining Lines are in groups of three representing each process and
     * its currently allocated resources, and then it's maximum number of resources
     * respectively
     */
    while (!input.eof())
    {
        //Allocated Resources to the Process
        tokens.clear();
        std::getline(input, currentLine);
        std::string pName = currentLine;
        processNames.push_back(pName);
        auto processAllocated = new Process(pName);
        tokens.clear();
        std::getline(input, currentLine); //Currently Allocated Resources to Process
        tokens.str(currentLine);
        int j = 0;
        while (tokens >> currentToken)
        {
            processAllocated->
                    resources.insert(std::pair<char, int>(resourceNames[j], (currentToken - '0')));
            j++;
        }
        allocated.push_back(processAllocated);

        //Maximum Resources to the Process
        auto process_max = new Process(pName);
        tokens.clear();
        std::getline(input, currentLine); //Maximum Resources to the Process
        tokens.str(currentLine);
        int k = 0;
        while (tokens >> currentToken)
        {
            process_max->
                    resources.insert(std::pair<char, int>(resourceNames[k], (currentToken - '0')));
            k++;
        }
        maximum.push_back(process_max);
    }

    //Information from everything else is used to calculate the available system resources
    for (int l = 0; l < instances.size(); l++)
    {
        auto resourceName = resourceNames[l];
        int max = instances[resourceName];
        int resourceCount = 0;
        for (auto o : allocated)
        {
            resourceCount += o->resources[resourceName];
        }
        available.insert(std::pair<char,int>(resourceName, (max-resourceCount)));
    }

    //Information from allocated and maximum is used to calculate "need"
    for (int q = 0; q < processNames.size(); q++)
    {
        auto newProcess = new Process(processNames[q]);
        for (int b = 0; b < instances.size(); b++)
        {
            char resourceName = resourceNames[b];
            int maxResource = maximum[q]->resources[resourceName];
            int allocatedResource = allocated[q]->resources[resourceName];
            int neededResource = maxResource - allocatedResource;
            newProcess -> resources.insert(std::pair<char, int>(resourceName, neededResource));
        }
        need.push_back(newProcess);
    }
}

void System::debugPrint()
{
    std::cout << '\n' << "System Overview" << '\n' << '\n';
    std::cout << "Maximum Available Resources" << '\n';

    for (const auto& i : instances)
    {
        std::cout << i.first << " " << i.second << '\n';
    }
    std::cout << '\n';

    std::cout << "Resources Allocated to each process" << '\n';
    for (const auto& a : allocated)
    {
        a->printAllocated();
    }
    std::cout << '\n';

    std::cout << "Maximum Resources for each process" << '\n';
    for (const auto& m : maximum)
    {
        m->printMax();
    }
    std::cout << '\n';

    std::cout << "Available system Resources" << '\n';
    for (const auto& a : available)
    {
        std::cout << a.first << " " << a.second << '\n';
    }
    std::cout << '\n';

    std::cout << "Needed Resources (Used in Bankers Algorithm)\n";
    for (const auto& n : need)
    {
        n->printNeeded();
    }
}

std::pair<bool, std::vector<std::string>> System::isSafe()
{
    const size_t pCount = allocated.size();
    const size_t rCount = instances.size();
    std::vector<char> rNames;
    {
        for (auto names_ : instances)
        {
            rNames.push_back(names_.first);
        }
    }

    //work is initialized to be equal to available
    std::map<char,int> work;
    for (auto a : available)
    {
        work.insert(a);
    }

    //finished processes initialized to all false
    std::vector<bool> finished (pCount);
    for (auto && i : finished) i = false;

    //array to keep track of the safe sequence
    std::vector<std::string> safeSequence (pCount);

    //Implementation of algorithm
    int count = 0;
    while (count < pCount)
    {
        bool safe = false;
        for (int i = 0; i < pCount; i++)
        {
            bool temp = true;
            if(!finished[i])
            {
                auto needProcess = need[i];
                auto allocProcess = allocated[i];
                for (auto j : rNames)
                {
                    //Needed resources must be less than or equal to available resources
                    if((needProcess->resources[j]) > (work[j]))
                    { temp = false; }
                }
                if (temp)
                {
                    for (auto k : rNames)
                    {
                        work[k] = ((work[k]) + (allocProcess->resources[k]));
                    }
                    safeSequence[count] = allocProcess -> name; //Add process to the safe sequence
                    count++; //Update the count
                    finished[i] = true;
                    safe = true;
                }
            }
        }
        if(!safe)
        {
            return std::make_pair(false, safeSequence);
        }
    }
    //If all processes are finished print sequence and return true.
    return std::make_pair(true, safeSequence);
}

void System::print()
{
    if(isSafe().first)
    {
        std::cout << "\nSystem is safe, the safe sequence is\n";
        for(const auto& seq : isSafe().second)
        {
            std::cout << seq << std::endl;
        }
    }
    else
    {
        std::cout << "System is unsafe";
    }
    std::cout << '\n';
}

void Process::printMax()
{
    std::cout << "Process " << name << '\n';
    for (const auto& r : resources)
    {
        std::cout << "Has a maximum requirement of " << r.second << " of resource " << r.first << '\n';
    }
}

void Process::printAllocated()
{
    std::cout << "Process " << name << '\n';
    for (const auto& r : resources)
    {
        std::cout << "Has allocated " << r.second << " of resource " << r.first << '\n';
    }
}

void Process::printNeeded()
{
    std::cout << "Process " << name << '\n';
    for (const auto& r : resources)
    {
        std::cout << "May need up to " << r.second << " more of resource " << r.first << '\n';
    }
}

Process::Process(std::string name_)
{
    name = std::move(name_);
}
