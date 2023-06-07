#ifndef INC_4_LESSON_LOCKFREESTAC_H
#define INC_4_LESSON_LOCKFREESTAC_H

#include <atomic>


template<typename T>
class LockfreeStack
{
private:
    struct Node
    {
        T data;
        Node *next;

        Node(T const &data_) : data(data_)
        {}
    };

    std::atomic<Node*> head;

public:
    void push(T const &data)
    {
        Node *const new_node = new Node(data);
        new_node->next = head.load();
        while (!head.compare_exchange_strong(new_node->next, new_node));
    }

    bool pop(T &result)
    {
        Node *old_head = head.load();
        while (true)
        {
            if (!head)
            {
                return false;
            }

            if(head.compare_exchange_strong(old_head, old_head->next))
            {
                result = old_head->data;
                return true;
            }
        }
    }
};


#endif //INC_4_LESSON_LOCKFREESTAC_H
