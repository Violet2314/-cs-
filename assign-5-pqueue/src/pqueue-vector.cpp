#include "pqueue-vector.h"
using namespace std;

VectorPQueue::VectorPQueue() {}
VectorPQueue::~VectorPQueue() {}

const string& VectorPQueue::peek() const {
    if(data.isEmpty()){
        cout << "队列是空的" << endl;
    }
    return data[data.size() - 1];
}

string VectorPQueue::extractMin() {
    if(data.isEmpty()){
        cout << "队列是空的" << endl;
    }
    int idx = 0;
    for(int i = 0; i < logSize; i++){
        idx = data[i] < data[idx] ? i : idx;
    }
    string ans = data[idx];
    data.remove(idx);
    logSize = data.size();
    return ans;
}

void VectorPQueue::enqueue(const string& elem ) {
    data.add(elem);
    logSize = data.size();
}

VectorPQueue *VectorPQueue::merge(VectorPQueue *  one , VectorPQueue *  two ) {
    VectorPQueue *now = new VectorPQueue;

    now->data = one->data + two->data;
    now->logSize = now->data.size();

    one->~VectorPQueue();
    two->~VectorPQueue();

    return now;
}
