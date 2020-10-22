#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <functional>
#include <condition_variable>
#include <dos.h>


std::list<std::function<void()>> tasks;
std::condition_variable cv;
std::mutex cv_m;

int i = 0;

void task()
{
    std::cerr << "Task \n";
}

void foo()
{
    std::unique_lock<std::mutex> lk(cv_m);
    if (tasks.empty())
    {
        cv.wait(lk);
    }
    else
    {
        tasks.front()();
    }

}

int main()
{
    std::vector<std::thread> ths;
    std::size_t n;
    std::cin >> n;


    for (std::size_t i = 0; i < n; ++i)
        ths.push_back(std::thread(foo));

    tasks.push_back(task);

    cv.notify_all();

    for (auto& th : ths)
        th.join();

    return 0;
}