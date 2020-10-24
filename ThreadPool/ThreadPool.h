#pragma once
#include <ostream>
#include <ostream>
#include <map>
#include <vector>
#include <list>
#include <mutex>
#include <functional>
#include <wtypes.h>

typedef void (*Procedure)(void*);
class ThreadPool {
public:


	static void WriteLog(std::string mess);
    std::vector<std::thread> ths;
    static std::list<std::function<void()>> tasks;
    static std::mutex mutex;
    static bool exitFlag;
    int maxThreads;
    static int currentNumOfThreads;
    HANDLE  hThreadArray[];
    static unsigned int _stdcall Task(void* args);
    ThreadPool(int n);
    void AddTask(std::function<void()> task);
    void StopThreads();
};