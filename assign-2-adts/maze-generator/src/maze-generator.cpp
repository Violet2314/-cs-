/**
 * File: maze-generator.cpp
 * ------------------------
 * Presents an adaptation of Kruskal's algorithm to generate mazes.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "simpio.h"
#include "maze-graphics.h"

vector<wall> walls;
int dimension;
int root[10000];


int find(int x){
    if(root[x] == x) return x;
    else return root[x] = find(root[x]);
}

void add(int a,int b){
    root[find(root[b])] = root[a];
}

void removethewall(MazeGeneratorView &M){
    int sum = dimension * dimension;
    while(true){
        int now = rand() % sum + 1;
        int row = now/dimension - 1;
        int col = now % dimension;
        if(col == 0){
            col = dimension-1;
        }else{
            row+=1;
            col -= 1;
        }
        int fan = rand() % 4;
        int near;
        wall nowwall;cell a1,a2;
        if(fan == 0){
            if(row == dimension-1) continue;
            near = now+10;
            a1.row = row,a1.col = col;//表示删去row和row+1之间的墙啊，不是点
            a2.row = row+1,a2.col = col;
        }
        else if(fan == 1){
            if(col <= 0) continue;
            near = now-1;
            a1.row = row,a1.col = col-1;
            a2.row = row,a2.col = col;
        }
        else if(fan == 2){
            if(row <= 0) continue;
            near = now-10;
            a1.row = row-1,a1.col = col;
            a2.row = row,a2.col = col;
        }
        else {
            if(col == dimension-1) continue;
            near = now+1;
            a1.row = row,a1.col = col;
            a2.row = row,a2.col = col+1;
        }

        if(find(root[near]) != find(root[now])){
            add(root[now],root[near]);
            nowwall.one = a1;
            nowwall.two = a2;
            M.removeWall(nowwall);
            break;
        }
    }
}

void getAll();

static int getMazeDimension(string prompt,
                            int minDimension = 7, int maxDimension = 50) {
    while (true) {
        int response = getInteger(prompt);
        if (response == 0) return response;
        if (response >= minDimension && response <= maxDimension) return response;
        cout << "Please enter a number between "
             << minDimension << " and "
             << maxDimension << ", inclusive." << endl;
    }
}

int main() {
    MazeGeneratorView M;
    srand(time(0));
    while (true) {
        dimension = getMazeDimension("你的迷宫应该是什么尺寸 [0 to exit]? ");
        if (dimension == 0) break;
        for(int i = 0; i <= dimension * dimension; i++) root[i] = i;
        M.setDimension(dimension);
        getAll();
        M.drawBorder();
        M.addAllWalls(walls);
        while(find(root[1]) != find(root[dimension * dimension])){
            removethewall(M);
        }
        M.repaint();
        cout << "尺寸是 " << dimension << "." << endl;
    }

    return 0;
}

void getAll(){
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            cell center,down,rigth;
            center.row = i,center.col = j;
            down.row = i+1,down.col = j;
            rigth.row = i,rigth.col = j+1;

            wall now1,now2;
            now1.one = center;
            now1.two = down;
            now2.one = center;
            now2.two = rigth;

            if(down.row < dimension){
                walls.push_back(now1);
            }
            if(rigth.col < dimension){
                walls.push_back(now2);
            }
        }
    }

}
