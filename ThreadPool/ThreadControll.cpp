#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <functional>
#include <condition_variable>
#include <dos.h>
#include "ThreadPool.cpp"
#include <windows.h>


void Test1()
{
    Sleep(2000);
    std::cerr << "hello home\n";
}
void Test2()
{
    Sleep(1000);
    std::cerr << "hello world\n";
}


int main()
{   
    int n;
    std::cin >> n;

    ThreadPool threadPool(n);

    threadPool.AddTask(Test1);
    threadPool.AddTask(Test2);
    //Sleep(4000);
    threadPool.AddTask(Test2);
    threadPool.AddTask(Test1);
   
    threadPool.StopThreads();

    return 0;
}