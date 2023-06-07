#include <iostream>
#include <functional>
#include <type_traits>
#include <thread>
using namespace std;

mutex mtx;

template <typename T, typename... Ts>
class logger
{
private:
    function<T(Ts...)> f;

public:
    logger(function<T(Ts...)> f)
    {
        this->f = f;
    }

    void operator() (Ts... args) requires (is_same_v<T, void>)
            {
                lock_guard<mutex> lock(mtx);
//                    while(!mtx.try_lock()) {}
                    {int dummy[sizeof...(Ts)] = {(std::cout << args << "; ", 0)...};}
                    cout << endl;
                    f(args...);
                    cout << "thread's ID:\t" << this_thread::get_id() << endl;
                    mtx.unlock();
            }

    T operator() (Ts... args)
    {
        lock_guard<mutex> lock(mtx);
        //while(!mtx.try_lock()) {}
        {int dummy[sizeof...(Ts)] = {(std::cout << args << "; ", 0)...};}
        cout << endl;
        cout << "thread's ID:\t" << this_thread::get_id() << endl;
        //mtx.unlock();
        return f(args...);
    }
};


double fs(long long t, double k)
{
    return t/k;
}

void v_fs(long long t, double k)
{
    cout << t/k << endl;
}

int main()
{
    long long t = 6;
    double k = 0.1;
    logger <double, long long, double> F(fs);
    thread th1([&]()
               {
                   cout << F(t, k) << endl;
               });

    logger <void, long long, double> v_F(v_fs);
    thread th2([&]()
               {
                   v_F(t, k);
               });

    th1.join();
    th2.join();
}