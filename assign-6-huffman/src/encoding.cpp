// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include "bitstream.h"
#include "filelib.h"
#include <queue>
using namespace std;
// TODO: include any other headers you need

Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    freqTable[PSEUDO_EOF]++;//用来标记结束
    while(true){
        if(input.eof()) break;// eof?
        else {
            int now = input.get();
            freqTable[now]++;
        }
    }
    //cout << freqTable['\0'] << "\n";
    freqTable.remove(-1);
    return freqTable;
}

struct CompareHuffmanNodePtr {//定义比较
    bool operator()(const HuffmanNode* lhs, const HuffmanNode* rhs) const {
        // 优先队列默认是最大堆，为了使其按照count的升序排列，我们使用大于号
        return lhs->count > rhs->count;
    }
};

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNodePtr> p;
    for(const auto it : freqTable){//char 最高就到255，PSEUDO_EOF是256会被char翻译为 \0 所以用auto才可以
        HuffmanNode* now = new HuffmanNode(it,freqTable[it],NULL,NULL);
        p.push(now);
        //cout << it <<  " " << freqTable[it] << "\n";
    }
    while(p.size() != 1){
        HuffmanNode* L = p.top();p.pop();
        cout << L->count << " ";
        HuffmanNode* R = p.top();p.pop();
        cout << R->count << "\n";
        HuffmanNode* now = new HuffmanNode(NOT_A_CHAR,L->count + R->count,L,R);
        p.push(now);
    }
    return p.top();
}

void dfs(HuffmanNode* now,
         string s,
         Map<int, string> &encodingMap){
    if(now == NULL) return ;
    if(now->character != NOT_A_CHAR){
        int a = now->character;
        encodingMap[a] = s;
    }
    dfs(now->zero,s + '0',encodingMap);
    dfs(now->one,s + '1',encodingMap);
    return ;
}

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;   // this is just a placeholder so it will compile
    dfs(encodingTree,"",encodingMap);
    return encodingMap;             // this is just a placeholder so it will compile
}


void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while(!input.eof()){
        int now = input.get();
        int len = encodingMap[now].size();
        string s = encodingMap[now];
        for(int i = 0; i < len; i++){
            output.writeBit(s[i] == '1'?1 : 0);
        }
    }
    int len = encodingMap[PSEUDO_EOF].size();
    string s = encodingMap[PSEUDO_EOF];
    for(int i = 0; i < len; i++){
        output.writeBit(s[i] == '1'?1 : 0);
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* root = encodingTree;

    while(true){
        if(input.fail()) break;
        int now = root->character;
        if(now != NOT_A_CHAR){
            if(now == PSEUDO_EOF) break;//表示到头了,结束了
            output.put(now);
            root = encodingTree;
            cout << " ";
            //cout << now << "\n";
        }
        int bit = input.readBit();
        cout << bit;
        if(bit == 1) root = root->one;
        else root = root->zero;
    }
}

void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* root = buildEncodingTree(freqTable);
    Map<int, string> map = buildEncodingMap(root);
    rewindStream(input);//重新改到开始的位置
    output << freqTable;//保存压缩的编码方式
    encodeData(input,map,output);
    freeTree(root);
}

void decompress(ibitstream& input, ostream& output) {
    Map<int,int> freqTable;
    input >> freqTable;
    for(auto it : freqTable){
        cout << it << " " << freqTable[it] << "\n";
    }
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input,encodingTree,output);
    freeTree(encodingTree);
}

void freeTree(HuffmanNode* node) {
    if(node != NULL){
        freeTree(node->one);
        freeTree(node->zero);
        delete node;
    }
}
