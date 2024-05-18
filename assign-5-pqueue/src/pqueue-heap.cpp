#include "pqueue-heap.h"
using namespace std;

HeapPQueue::HeapPQueue() {
    capacity = 8;
    array = new std::string[capacity + 1];
}
HeapPQueue::~HeapPQueue() {
    delete[] array;
}

const string& HeapPQueue::peek() const {
    if(logSize == 0){
        throw "是空的";
    }
    return array[1];
}

string HeapPQueue::extractMin() {
    if(logSize == 0){
        throw "是空的";
    }
    string ans = array[1];
    string wei = array[logSize];
    array[1] = wei;
    logSize--;
    int now1 = 1;
    int now2 = 1;
    while(now1 < logSize){
        string ans1 = now2 * 2 + 1 <= logSize ? array[now2*2+1] : "";
        string ans2 = now2 * 2 <= logSize ? array[now2*2] : "";
        if(ans1 == "" && ans2 == ""){
            break;
        }else if(ans1 == ""){
            now2 = now2 * 2;
        }else{
            now2 = ans1 > ans2 ? now2 * 2 : now2 * 2 + 1;
        }

        if(now2 <= logSize && array[now1] > array[now2]){
            wei = array[now1];
            array[now1] = array[now2];
            array[now2] = wei;
        }else{
            break;
        }
        now1 = now2;
    }

    return ans;
}

void HeapPQueue::enqueue(const string&  elem) {
    if(logSize >= capacity){
        capacity *= 2;
        std::string* big = new std::string[capacity + 1];
        for(int i = 1; i <= logSize; i++){
            big[i] = array[i];
        }
        delete[] array;
        array = big;
    }
    logSize += 1;
    array[logSize] = elem;
    int now = logSize;
    int now1 = logSize;
    std::string first;
    while(now > 1){
        now /= 2;
        if(array[now1] < array[now]){
            first = array[now];
            array[now] = array[now1];
            array[now1] = first;
        }else{
            break;
        }
        now1 = now;
    }
}

HeapPQueue *HeapPQueue::merge(HeapPQueue *  one , HeapPQueue *  two) {
    HeapPQueue* ans = new HeapPQueue();

    for(int i = 1; i <= one->logSize; i++){
        ans->enqueue(one->array[i]);
    }
    for(int i = 1; i <= two->logSize; i++){
        ans->enqueue(two->array[i]);
    }

    one->~HeapPQueue();
    two->~HeapPQueue();

    return ans;
}
