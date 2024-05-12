/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
#include <queue>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"

static string getWord(const Lexicon& english, const string& prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "你的输入要是英文才可以，请重新输入" << endl;
    }
}

static void generateLadder(const Lexicon& english, const string& start, const string& end) {
    queue<vector<string>> q;
    unordered_map<string,int> mp;
    vector<string> vc1;vc1.push_back(start);
    string now;
    q.push(vc1);
    vector<string> check;
    vector<string> ans;
    bool have = false;
    while(q.size()){
        check = q.front();
        q.pop();
        now = check.back();
        if(mp[now]) continue;
        mp[now] = 1;
        for(int i = 0; i < now.size(); i++){
            for(int j = 0; j <= 25; j++){
                if(now[i] == 'a'+ j) continue;
                string change = now;change[i] = 'a' + j;
                if(change == end){
                    ans = check;
                    have = true;
                    break;
                }
                if(english.contains(change) && have == false){
                    vector<string> newCheck = check;
                    newCheck.push_back(change);
                    q.push(newCheck);
                }
            }
            if(have) break;
        }
    }
    if(have){
        cout << "Here's where you'll search for a word ladder connecting ";
        cout << start << "->";
        for(int i = 1; i < ans.size(); i++){
            cout << ans[i] << "->";
        }
        cout << end << endl;
    }else{
        cout << "不存在" << endl;
    }
}

static const string kEnglishLanguageDatafile = "res/dictionary.txt";

static void playWordLadder() {
    Lexicon english(kEnglishLanguageDatafile);
    while (true) {
        string start = getWord(english, "请输入开始的英语单词[空的表示退出]");
        if (start.empty()) break;
        string end = getWord(english, "请输入结束的英语单词[空的表示退出]");
        if (end.empty()) break;
        generateLadder(english, start, end);
    }
}

int main() {
    cout << "欢迎来到单词接龙" << endl << endl;
    while(true){
        playWordLadder();
        cout << "是否继续游玩" << endl;
        string s;getline(cin,s);
        if(s == "no") break;
    }
    cout << "感谢你的游玩" << endl;
    return 0;
}
