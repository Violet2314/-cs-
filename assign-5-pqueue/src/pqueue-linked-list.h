#pragma once
#include "pqueue.h"
#include <string>

class LinkedListPQueue : public PQueue {
public:
	LinkedListPQueue();
	~LinkedListPQueue();
	
	static LinkedListPQueue *merge(LinkedListPQueue *one, LinkedListPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
	
private:
    struct cell{
        std::string val;
        cell* next;
        cell* before;
    };
    cell* head;//要记得分配他指向哪里
};
