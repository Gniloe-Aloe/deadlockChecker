#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <assert.h>

void LockMutexes(std::mutex& mutexOne, std::mutex& mutexTwo)
{
    std::lock_guard<std::mutex> guardOne(mutexOne);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::lock_guard<std::mutex> guardTwo(mutexTwo);
}

void GenerateDeadlock()
{
    std::mutex mutexOne, mutexTwo;

    std::thread threadDeadlock(
        [&mutexOne, &mutexTwo]()
        {LockMutexes(mutexOne, mutexTwo);}
        );

    std::lock_guard<std::mutex> guardTwo(mutexTwo);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::lock_guard<std::mutex> guardOne(mutexOne);

    threadDeadlock.join();
}

void EmulateSomeWork()
{
    std::vector<int> numbersContainer;
    for (int i = 0; i < 1000; ++i)
    {
        numbersContainer.push_back(i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

inline void DropWrongParametersError()
{
    std::cout << "Wrong parameters! Call just with:" << '\n'
        << "0 - don't generate a deadlock" << '\n' 
        << "1 - generate a deadlock" << std::endl;
    exit(0);  
}

// return false - don't generate a deadlock
// return true  - generate a deadlock
bool ValidateParameters(int parametersCounter, char* pointerInputParameter)
{
    if (parametersCounter != 2)
    {
        DropWrongParametersError();
    }

    // Error if we have more then one simbhol in the  parameter
    if (*(pointerInputParameter + 1) != '\0')
    {
        DropWrongParametersError();
    }

    char inputParametr = *pointerInputParameter;

    if (inputParametr == '0')
    {
        return false;
    }
    else if (inputParametr == '1')
    {
        return true;
    }

    DropWrongParametersError();
    assert(false && "unreachable line");
    return false; // Keep the compiler quiet
}

// return false - don't generate a deadlock
// return true  - generate a deadlock
bool RequestParameters()
{
    int inputValue;

    for (;;)
    {
        std::cout << "Enter the number:" << '\n' 
            << "0 - don't generate a deadlock" << '\n' 
            << "1 - generate a deadlock" << std::endl;

        std::cin >> inputValue;

        if (inputValue == 0 || inputValue == 1) 
        {
            break;
        }
        else
        {
            std::cout << "Wrong input! Try again." << std::endl;
        }
    }
    
    return inputValue;
}

int main(int argc, char *argv[])
{
    bool runDeadlockScenario;

    // Process user input
    if (argc == 1)
    {
        // Need to request parameters
        runDeadlockScenario = RequestParameters();
    }
    else
    {
        // We were launched with CLI parameters
        runDeadlockScenario = ValidateParameters(argc, argv[1]);
    }

    // Run the selected (deadlock/non deadlock) scenario
    if (runDeadlockScenario)
    {
        std::cout << "Deadlock scenario start" << std::endl;
        GenerateDeadlock();
    }
    else
    {
        std::cout << "Non deadlock scenario start" << std::endl;
        EmulateSomeWork();
    }

    return 0;
}