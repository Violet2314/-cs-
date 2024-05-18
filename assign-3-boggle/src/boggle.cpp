/**
 * File: boggle.cpp
 * ----------------
 * Implements the game of Boggle.
 */

#include <cctype>
#include <iostream>
#include <ctype.h>
#include "console.h"
#include "gboggle.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "random.h"
#include "grid.h"
#include "map.h"
#include "set.h"
#include "lexicon.h"
#include "console.h"
using namespace std;

Lexicon english("res/dictionary.txt");
char chars[10][10];
unordered_map<string,int> mp;
unordered_map<string,int> mp2;
vector<string> allans;
vector<string> userans;
int vis[100];
int dx[8] = {1,1,1,-1,-1,-1,0,0};
int dy[8] = {1,0,-1,1,0,-1,1,-1};

static const string kStandardCubes[16] = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

static const string kBigBoggleCubes[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

static const int kMinLength = 4;
static const double kDelayBetweenHighlights = 100;
static const double kDelayAfterAllHighlights = 500;
int dimension;

char getrandchar(){
    int now1 = rand() % 25;
    int now2 = rand() % 6;
    return kBigBoggleCubes[now1][now2];
}

void dfs(string s,int x,int y){
    vis[x*10 + y] = 1;
    if(s.size() >= 4){
        if(english.contains(s) && mp[s] != 1){
            allans.push_back(s);
            mp[s] = 1;
        }
    }
    for(int i = 0; i <= 7; i++){
        int xx = x + dx[i];
        int yy = y + dy[i];
        if(vis[xx * 10 + yy] == 1 || xx < 0 || xx >= dimension || yy < 0 || yy >= dimension) continue;
        string ss =s + chars[xx][yy];
        dfs(ss,xx,yy);
    }
    vis[x*10 + y] = 0;
}

void getallans(){
    for(int i = 0; i <= dimension; i++){
        for(int j = 0; j <= dimension; j++){
            string s = "";s+=chars[i][j];
            dfs(s,i,j);
        }
    }
}

/**
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */
static void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl;
    cout << endl;
}

/**
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */
static void giveInstructions() {
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl;
    cout << endl;
    cout << "Hit return when you're ready...";
    getLine(); // ignore return value
}

/**
 * Function: getPreferredBoardSize
 * -------------------------------
 * Repeatedly prompts the user until he or she responds with one
 * of the two supported Boggle board dimensions.
 */

static int getPreferredBoardSize() {
    cout << "You can choose standard Boggle (4x4 grid) or Big Boggle (5x5 grid)." << endl;
    return getIntegerBetween("Which dimension would you prefer: 4 or 5?", 4, 5);
}

/**
 * Function: playBoggle
 * --------------------
 * Manages all details needed for the user to play one
 * or more games of Boggle.
 */
static void playBoggle() {
    dimension = getPreferredBoardSize();
    drawBoard(dimension, dimension);
    cout << "This is where you'd play the game of Boggle" << endl;
}

/**
 * Function: main
 * --------------
 * Serves as the entry point to the entire program.
 */
int main() {
    GWindow gw(kBoggleWindowWidth, kBoggleWindowHeight);
    srand(time(0));
    //setConsoleSize(1000, 800);
    //setConsoleLocation(1100, 100);
    initGBoggle(gw);
    welcome();
    if (getYesOrNo("Do you need instructions?")) giveInstructions();
    do {
        playBoggle();
        for(int i = 0; i < dimension; i++){
            for(int j = 0; j < dimension; j++){
                char now = getrandchar();
                labelCube(i,j,now);
                chars[i][j] = now;
            }
        }
        getallans();
        cout << "\n";

        string get;
        while(true){
            cout << "请输入可以形成的字符[输入q退出自己的输入环节]: ";
            getline(cin,get);
            if(get == "q"){
                cout << "用户输入结束" << endl;
                break;
            }else{
                if(mp2[get] == 1){
                    cout << "输入的单词已经存在了" << endl;
                    continue;
                }
                if(english.contains(get)){
                    cout << "成功存入" << endl;
                    userans.push_back(get);
                    recordWordForPlayer(get,HUMAN);
                    mp2[get] = 1;
                }else{
                    cout << "输入的单词在字典中不存在" << endl;
                }
            }
        }
        for(int i = 0; i < allans.size(); i++){
            if(mp2[allans[i]] == 1) continue;
            recordWordForPlayer(allans[i],COMPUTER);
        }
        int score1 = getscore(HUMAN);
        int socre2 = getscore(COMPUTER);
        if(score1 < socre2){
            cout << "电脑赢了" << endl;
        }else{
            cout << "你赢了" << endl;
        }
    } while (getYesOrNo("Would you like to play again?"));

    cout << "Thank you for playing!" << endl;
    shutdownGBoggle();
    return 0;
}
