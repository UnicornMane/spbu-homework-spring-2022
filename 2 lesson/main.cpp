#include <iostream>
#include <vector>
#include "Thread_Safe_Queue.h"
//#include "Sema's_queue.h"
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;


int n = 0;
int time_to_prepare = 0;
int time_to_complete = 0;

chrono::duration<float> boss_of_task_creation(int preparators, int completors);


int main()
{

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;


    cout << "enter the number of tasks: ";
    cin >> n;

    cout << "\nenter the time that preparation should take: ";
    cin >> time_to_prepare;

    cout << "\nenter the time that execution should take: ";
    cin >> time_to_complete;

    int num_of_threads = thread::hardware_concurrency();

    for (int i = 1; i < num_of_threads; ++i)
    {
        cout << setprecision(4) << fixed << boss_of_task_creation(i, num_of_threads - i).count()
        << " s with these parameters:\n number of generators: " << i
             << "\n number of executors: " << num_of_threads - i << "\n";

    }

    return 0;
}



chrono::duration<float> boss_of_task_creation(int preparators, int completors)
{
    chrono::time_point<std::chrono::high_resolution_clock> t1 = chrono::high_resolution_clock::now();
    atomic<size_t> counter_of_prepared_tasks;
    atomic<size_t> counter_of_completed_tasks;
    SafeQueue<bool> distribution_of_tasks;
    vector<thread> f1;
    vector<thread> f2;
    while (1)
    {
        if(f1.size() < preparators && counter_of_prepared_tasks < n)
        {
            f1.emplace_back([&](){
                while (counter_of_prepared_tasks < n)
                {
                    std::this_thread::sleep_for(chrono::seconds (time_to_prepare));
                    distribution_of_tasks.push(true);
                    counter_of_prepared_tasks.fetch_add(1);
                }
            });
        }
        if(f2.size() < completors && counter_of_completed_tasks < n)
        {
            f2.emplace_back([&]() {
                while (counter_of_completed_tasks < n)
                {
                    std::this_thread::sleep_for(chrono::seconds (time_to_complete));
                    while(!distribution_of_tasks.try_pop() && counter_of_completed_tasks < n)
                    {

                    }
                    counter_of_completed_tasks.fetch_add(1);
                }
            });
        }
        if (counter_of_completed_tasks >= n)
        {
            break;
        }
        if ((f1.size() >= preparators || counter_of_prepared_tasks >= n) && (f2.size() >= completors || counter_of_completed_tasks >= n))
        {
            break;
        }
    }

    while (counter_of_completed_tasks < n)
    {

    }
    chrono::time_point<std::chrono::high_resolution_clock> t2 = chrono::high_resolution_clock::now();

    for (auto & i : f1)
    {
        i.join();
    }
    for (auto & j : f2)
    {
        j.join();
    }
    return t2 - t1;
}