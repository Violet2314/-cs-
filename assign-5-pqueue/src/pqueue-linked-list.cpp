#include "pqueue-linked-list.h"
using namespace std;

LinkedListPQueue::LinkedListPQueue() {
    head = new cell();////要记得分配他指向哪里
}
LinkedListPQueue::~LinkedListPQueue() {
    cell* now;
    cell* now2 = head->next;//这里有问题，应改在extractMin()弹出的时候就要删的
    while(now2 != NULL){
        now = now2->next;
        delete now2;
        now2 = now;
    }
    delete head;
}

const string& LinkedListPQueue::peek() const {
    if(logSize == 0){
        throw "链表长度为0";
    }
    return head->next->val;
}

string LinkedListPQueue::extractMin() {
    if(logSize == 0){
        throw "链表长度为0";
    }
    string ans = head->next->val;
    head = head->next;
    logSize--;
    return ans;
}

void LinkedListPQueue::enqueue(const string& elem) {
    cell* now = new cell();
    now->val = elem;
    cell* p1 = head;
    cell* p2 = head->next;
    logSize++;
    while(p2 != NULL){
        if(p2->val > elem){
            p1->next = now;
            now->before = p1;
            now->next = p2;
            p2->before = now;
            return ;
        }
        p1 = p2;
        p2 = p2->next;
    }
    p1->next = now;
    now->before = p1;
}

LinkedListPQueue *LinkedListPQueue::merge(LinkedListPQueue * one, LinkedListPQueue * two) {
    LinkedListPQueue* now = new LinkedListPQueue;

    cell* p1 = one->head->next;
    while(p1 != NULL){
        now->enqueue(p1->val);
        p1 = p1->next;
    }
    p1 = two->head->next;
    while(p1 != NULL){
        now->enqueue(p1->val);
        p1 = p1->next;
    }

    one->~LinkedListPQueue();
    two->~LinkedListPQueue();

    return now;
}

