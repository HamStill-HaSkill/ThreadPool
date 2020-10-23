#include <thread>
#include <mutex>
#include <list>
#include <functional>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class ThreadPool
{
private:
    std::vector<std::thread> ths;
    std::list<std::function<void()>> tasks;
    std::mutex mutex;
    bool exit = false;
    int maxThreads;
    int currentNumOfThreads = 0;

    void Task()
    {
        std::function<void()> task;
        while (!exit)
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
    }

    void WriteLog(std::string mess)
    {
        std::ofstream out("Log.txt", std::ios::app);
        if (out.is_open())
        {
            out << mess << std::endl;
        }
        out.close();
    }

public:    
    ThreadPool(int n)
    {
        maxThreads = n;
        for (int i = 0; i < n; ++i)
        {
            ths.push_back(std::thread(&ThreadPool::Task, this));
            std::cerr << "Thread " << ths[i].get_id() << " was create\n";

            std::ofstream out("Log.txt", std::ios::app);
            if (out.is_open())
            {
                out << "Thread " << ths[i].get_id() << " was create" << std::endl;
            }
            out.close();
        }
            
    }
    void AddTask(std::function<void()> task)
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

    void StopThreads()
    {
        exit = true;
        for (auto& th : ths)
            th.join();       
    }
};