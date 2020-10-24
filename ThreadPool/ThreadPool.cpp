#include <thread>
#include <mutex>
#include <list>
#include <functional>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <wtypes.h>
#include "ThreadPool.h"

#define MAX_THREADS 255
#define BUF_SIZE 255

    std::list<std::function<void()>> ThreadPool::tasks;
    std::mutex ThreadPool::mutex;
    bool ThreadPool::exitFlag = false;
    int maxThreads;
    int ThreadPool::currentNumOfThreads = 0;
    HANDLE hThreadArray[MAX_THREADS];

    void ThreadPool::WriteLog(std::string mess)
    {
        std::ofstream out("Log.txt", std::ios::app);
        if (out.is_open())
        {
            out << mess << std::endl;
        }
        out.close();
    }

    unsigned int ThreadPool::Task(void* args)
    {
        std::function<void()> task;
        while (!exitFlag)
        {        
            if (!tasks.empty())
            {
                mutex.lock();
                if (!tasks.empty())
                {
                    task = tasks.front();
                    tasks.pop_front();
                }
                else
                {
                    mutex.unlock();
                    continue;
                }
                mutex.unlock();   

                try
                {
                    task();
                }
                catch (const std::exception& e)
                {
                    WriteLog(e.what());
                }              
                currentNumOfThreads--;
            } 
        }
        return 0;
    }
   
    ThreadPool::ThreadPool(int n)
    {
        unsigned int   dwThreadIdArray[MAX_THREADS];
        maxThreads = n;

        for (int i = 0; i < maxThreads; i++)
        {

            hThreadArray[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadPool::Task, this, 0, &dwThreadIdArray[i]);

            std::cerr << "Thread " << hThreadArray[i] << " was create\n";

            std::ofstream out("Log.txt", std::ios::app);
            if (out.is_open())
            {
                out << "Thread " << hThreadArray[i] << " was create" << std::endl;
            }
            out.close();

        } 
            
    }
    void ThreadPool::AddTask(std::function<void()> task)
    {
        if (currentNumOfThreads < maxThreads)
        {
            tasks.push_back(task);
            currentNumOfThreads++;
            std::cerr << "Add new task\n";
            WriteLog("Add new task");
        }
        else
        {
            std::cerr << "Exceeding the maximum allowed number of running threads\n";
            WriteLog("Exceeding the maximum allowed number of running threads");
        }
    }

    void ThreadPool::StopThreads()
    {
        exitFlag = true;
        WaitForMultipleObjects(maxThreads, hThreadArray, TRUE, INFINITE);

        for (int i = 0; i < maxThreads; i++)
        {
            CloseHandle(hThreadArray[i]);
        }  
    }


