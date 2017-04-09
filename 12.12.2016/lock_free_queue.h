#include <iostream>
#include <atomic>
#include <vector>


template <typename T>
class lock_free_queue
{
public:
//----------NODE CLASS------------------
    class Node
    {
    public:
        Node()
        {
            setNextNode(nullptr);
        }
        T getData()
        {
            return data;
        }
        void setData(T& data)
        {
            (*this).data = data;
        }
        std::atomic<Node*>& getNextNode()
        {
            return next;
        }
        void setNextNode(Node* next)
        {
            (*this).next.store(next);
        }
    private:
        std::atomic<Node*> next;
        T data;
    };
//----------END OF NODE CLASS----------

    lock_free_queue();
    ~lock_free_queue();
    void enqueue(T item);
    bool dequeue(T& item);
private:
    void collectGarbage(std::atomic<bool>& isUsedInDestructor);

    std::atomic<size_t> threadsInQueue;
    std::atomic<Node*> head;
    std::atomic<Node*> previousHead;
    std::atomic<Node*> tail;
};

/**
 * previousHead - pointer to start of collecting garbage
 */

template <typename T>
lock_free_queue<T>::lock_free_queue ()
{
    threadsInQueue.store(0);
    head.store(new Node());
    previousHead.store(head.load());
    tail.store(head.load());
};

/**
 * We have to delete all elements from previousHead (it means current head)
 * to tail->child (tail->child === nullptr)
 */

template <typename T>
lock_free_queue<T>::~lock_free_queue()
{
    head.store(nullptr);
    std::atomic<bool> isUsedInDestructor;
    isUsedInDestructor.store(true);
    collectGarbage(isUsedInDestructor);
}

/**
 * Source from original article
 */

template <typename T>
void lock_free_queue<T>::enqueue (T item)
{
    ++threadsInQueue;
    Node* node = new Node();
    node->setData(item);
    node->setNextNode(nullptr);
    Node* localTail;
    Node* nextTailNode;
    while (true){
        localTail = tail.load();
        nextTailNode = localTail->getNextNode();
        if (localTail == tail){
            if (nextTailNode == nullptr){
                if (localTail->getNextNode().compare_exchange_weak(nextTailNode, node)){
                    break;
                }
            }
            else{
                tail.compare_exchange_weak(localTail, nextTailNode);
            }
        }
    }
    tail.compare_exchange_strong(localTail, node);
    --threadsInQueue;
}

/**
 * Source from original article
 */

template <typename T>
bool lock_free_queue<T>::dequeue (T& item)
{
    ++threadsInQueue;
    while (true){
        Node* localHead = head.load();
        Node* localTail = tail.load();
        Node* nextHeadNode = localHead->getNextNode();
        if (localHead == head.load()){
            if (localHead == localTail){
                if (nextHeadNode == nullptr){
                    return false;
                }
                tail.compare_exchange_weak(localTail, nextHeadNode);
            }
            else{
                item = nextHeadNode->getData();
                if (head.compare_exchange_weak(localHead, nextHeadNode)){
                    break;
                }
            }
        }
    }
    std::atomic<bool> isUsedInDestructor;
    isUsedInDestructor.store(false);
    collectGarbage(isUsedInDestructor);
    --threadsInQueue;
    return true;
}

/**
 * Deleting vertexes from previousHead to current head
 */

template <typename T>
void lock_free_queue<T>::collectGarbage(std::atomic<bool>& isUsedInDestructor)
{
    if (threadsInQueue.load() != 1 && !isUsedInDestructor.load()){
        return;
    }
    Node* localHead = head.load();
    while (localHead != previousHead.load()){
        Node* nextToDelete = previousHead.load()->getNextNode();
        delete(previousHead);
        previousHead.store(nextToDelete);
    }
}
