#pragma once
#include "pqueue.h"
#include <string>
#include "vector.h"

class BinomialHeapPQueue : public PQueue {
public:
	BinomialHeapPQueue();
	~BinomialHeapPQueue();
	
	static BinomialHeapPQueue *merge(BinomialHeapPQueue *one, BinomialHeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
	
private:
    struct Node{
        std::string val;
        int dagree;
        Vector<Node*> child;
        Node* parent;
        Node* next;
    };
    Vector<Node*> head;
    int len;
};

