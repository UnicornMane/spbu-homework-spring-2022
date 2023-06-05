#include <iostream>
#include <thread>
#include <future>

using namespace std;

int a = 0;
int b = 1;

void addOne()
{
    a += 1;
    b += 1;
}

void swapp()
{
    int c = b;
    b = a;
    a = c;
}


int main()
{
    std::packaged_task<void()> t1(addOne);
    std::packaged_task<void()> t2(swapp);
    std::future<void> fu1 = t1.get_future();
    std::future<void> fu2 = t2.get_future();

    t1();
    t2();

    fu1.get();
    fu2.get();

    std::cout << a << " " << b;

    return 0;
}
