/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

#include <iostream>
#include <fstream>
using namespace std;

#include "console.h"
#include "simpio.h"   // for getLine
#include "strlib.h"   // for toLowerCase, trim

unordered_map<string,vector<string>> sentence;
static const string kGrammarsDirectory = "res/grammars/";
static const string kGrammarFileExtension = ".g";

void getSentence(string filename){
    ifstream file;
    string line;
    string top;
    file.open(kGrammarsDirectory + filename);
    int num;
    while(getline(file,line)){
        if(line[0] == '<'){
            top = line;
            getline(file,line);
            num = stoi(line);
            for(int i = 1; i <= num; i++){
                getline(file,line);
                sentence[top].push_back(line);
            }
        }
    }
}

string becomeSentence(string s){
    string ans = "";
    int num = sentence[s].size();
    int now = rand() % num;
    string ss = sentence[s][now];
    num = ss.size();
    string check;
    for(int i = 0; i < num; i++){
        if(ss[i] == '<'){
            check = "";
            for(int j = i; j < num; j++){
                if(ss[j] == '>'){
                    check += ss[j];
                    i = j;
                    break;
                }
                check += ss[j];
            }
            ans += becomeSentence(check);
            continue;
        }
        ans += ss[i];
    }
    return ans;
}


static string getNormalizedFilename(string filename) {
    string normalizedFileName = kGrammarsDirectory + filename;
    if (!endsWith(normalizedFileName, kGrammarFileExtension))
        normalizedFileName += kGrammarFileExtension;
    return normalizedFileName;
}

static bool isValidGrammarFilename(string filename) {
    string normalizedFileName = getNormalizedFilename(filename);
    ifstream infile(normalizedFileName.c_str());
    return !infile.fail();
}

static string getFileName() {
    while (true) {
        string filename = trim(getLine("Name of grammar file? [<return> to quit]: "));
        if (filename.empty() || isValidGrammarFilename(filename)) return filename;
        cout << "Failed to open the grammar file named \"" << filename << "\". Please try again...." << endl;

    }
}

int main() {
    while (true) {
        srand(time(0));
        sentence.clear();
        string filename = getFileName();
        if (filename.empty()) break;
        cout << "Here's where you read in the \"" << filename << "\" grammar "
             << "and generate three random sentences." << endl;
        getSentence(filename);
        string ans = becomeSentence("<start>");
        cout << "\n";
        cout << ans << "\n" << "\n";
    }
    
    cout << "Thanks for playing!" << endl;
    return 0;
}
