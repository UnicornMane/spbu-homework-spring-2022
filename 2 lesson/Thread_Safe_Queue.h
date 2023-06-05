#ifndef INC_2_LESSON_THREAD_SAFE_QUEUE_H
#define INC_2_LESSON_THREAD_SAFE_QUEUE_H

#include <mutex>
#include <queue>
#include <condition_variable>


template<typename T>
class SafeQueue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    void push(T &&new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }

    bool try_pop(T &value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
        {
            return false;
        }
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> res{std::make_shared<T>(std::move(data_queue.front()))};
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

#endif //INC_2_LESSON_THREAD_SAFE_QUEUE_H