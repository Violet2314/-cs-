#include "pqueue-binomial-heap.h"
using namespace std;

BinomialHeapPQueue::BinomialHeapPQueue() {
    len = 0;
}
BinomialHeapPQueue::~BinomialHeapPQueue() {
    ////这里有问题，应改在extractMin()弹出的时候就要删的
    for(int i = 0; i < len; i++){
        Node* now = head[i];
        while(now != NULL){
            Node* now2 = now->next;
            delete now;
            now = now2;
        }        
    }
}

const string& BinomialHeapPQueue::peek() const {
    if(logSize == 0){
        throw "空的";
    }
    int now = 0;
    for(int i = 0; i < len; i++){
        if(head[i] != NULL){
            now = i;
            break;
        }
    }
    for(int i = 1; i < len; i++){
        if(head[i]->val < head[now]->val){
            now = i;
        }
    }
    return head[now]->val;
}

string BinomialHeapPQueue::extractMin() {
    if(logSize == 0){
        throw "空的";
    }
    logSize--;
    int now = 1e9;
    for(int i = 0; i < len; i++){
        if(head[i] != NULL){
            now = i;
            break;
        }
    }
    for(int i = 0; i < len; i++){
        if(head[i] == NULL) continue;
        if(head[i]->val < head[now]->val){
            now = i;
        }
    }
    string ans = head[now]->val;
    BinomialHeapPQueue* jia = new BinomialHeapPQueue;
    for(int i = 0; i < now; i++){
        jia->head.add(NULL);
        jia->head[i] = head[now]->child[i];
        jia->len++;
    }
    head[now] = NULL;
    BinomialHeapPQueue* wok = merge(this,jia);
    head = wok->head;
    len = wok->len;
    return ans;
}

void BinomialHeapPQueue::enqueue(const string& elem) {
    Node* now = new Node;
    now->val = elem;
    now->dagree = 0;
    logSize++;
    for(int i = 0; i < len; i++){
        if(head[i] == NULL){
            head[i] = now;
            now = NULL;
            break;
        }
        if(head[i]->val > now->val){
            now->child.add(head[i]);
            head[i]->parent = now;
            now->dagree++;
        }else{
            now->parent = head[i];
            head[i]->child.add(now);
            now = head[i];
            now->dagree++;
        }
        head[i] = NULL;
    }
    if(now != NULL){
        head.add(now);
        len++;
    }
}

BinomialHeapPQueue *BinomialHeapPQueue::merge(BinomialHeapPQueue * one , BinomialHeapPQueue *  two ) {
    BinomialHeapPQueue *ans = new BinomialHeapPQueue;

    int minn = max(one->len,two->len);

    for(int i = 0; i < minn; i++){
        Node* now = NULL;
        bool check = false;
        Node* now1 = one->len > i ? one->head[i] : NULL;
        Node* now2 = two->len > i ? two->head[i] : NULL;
        if(now1 != NULL && now2 != NULL){
            check = true;
            if(now1->val > now2->val){
                now1->parent = now2;
                now2->child.add(now1);
                now2->dagree++;
                now = now2;
            }else{
                now2->parent = now1;
                now1->child.add(now2);
                now1->dagree++;
                now = now1;
            }
        }else if(now1 != NULL){
            now = now1;
        }else if(now2 != NULL){
            now = now2;
        }
        if(check == true){
            if(ans->len < i+1){
                ans->head.add(NULL);
                ans->len++;
            }
            ans->head.add(now);
            ans->len++;
        }else{
            if(ans->len == i+1){
                now2 = ans->head[i];
                now1 = now;
                if(now1 != NULL){
                    if(now1->val > now2->val){
                        now1->parent = now2;
                        now2->child.add(now1);
                        now2->dagree++;
                        now = now2;
                    }else{
                        now2->parent = now1;
                        now1->child.add(now2);
                        now1->dagree++;
                        now = now1;
                    }
                    ans->head.add(now);
                    ans->head[i] = NULL;
                    ans->len++;
                }
            }else{
                ans->head.add(now);
                ans->len++;
            }
        }
    }
    ans->logSize = one->logSize + two->logSize;
    return ans;
}
