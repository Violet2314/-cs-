/**
 * File: ssmodel.cpp
 * ------------------
 * This file will contain the implementation of the SSModel class (once you
 * have finished it!
 */
 
#include "ssmodel.h"
#include <algorithm>
#include "exp.h"
#include "parser.h"
using namespace std;

SSModel::SSModel(int nRows, int nCols, SSView *ssview) {
    numrow = nRows;
    numcol = nCols;
    view = ssview;
}

SSModel::~SSModel() {}

bool SSModel::nameIsValid(const string& cellname) const {
    string str = "";
    int vis = 0;
    for(int i = 0; i < cellname.size(); i++){
        if(!isalpha(cellname[i])) {
            //cout << cellname[i] << " ";
            vis = i;
            break;
        }
        str += cellname[i];
    }
    //cout << str << "\n";
    if(str == ""){
        //cout << "beacuse " + str + " == 空";
        return false;
    }
    int sum = 0;
    int wok = 1;
    for(int i = str.size()-1; i >= 0; i--){
        sum += (str[i] - 'a' + 1) * wok;
        wok *= 26;
    }
    if(sum > numcol) return false;
    string number = cellname.substr(vis);
    for(int i = 0; i < number.size(); i++){
        if(!isdigit(number[i])){
            cout << number[i] << "\n";
            return false;
        }
    }
    wok = stoi(number);
    if(wok > numrow) return false;
    return true;
}

void SSModel::setCellFromScanner(const string& cellname, TokenScanner& scanner) {
    Cell* newcell = new Cell(cellname);
    string f = cellname + " = ";
    try{
        Expression* now = parseExp(scanner);
        if(now->getType() == TEXTSTRING){
            newcell->setstrdata(now->toString());
            f += "\"";
            f += now->toString();
            f += "\"";
        }else if(now->getType() == DOUBLE){
            //cout << now->toString() << "\n";
            newcell->setnumdata(stod(now->toString()));
            f += now->toString();
        }else if(now->getType() == IDENTIFIER){
            f += now->toString();
            string vis[8] = {"min","max","average","product","sum","median","stdev"};
            int can = 0;
            string str = now->toString();
            transform(str.begin(),str.end(),str.begin(),::tolower);
            for(int i = 0; i <= 6; i++){
                if(str == vis[i]) can = i;
            }
            if(can){
                double ans = 0;
                Vector<double> d;
                string wok = "";
                while(scanner.hasMoreTokens()){
                    wok = scanner.nextToken();
                    //cout << wok << " ";
                    if(!nameIsValid(wok)) {
                        if(wok.size() == 1) continue;
                        error("Invalid cell name " + wok);
                    }
                    if(table.count(wok)){
                        f += wok;
                        d.add(table[wok]->getnumdata());
                        newcell->adddepending(table[wok]);
                        table[wok]->adddepended(newcell);
                    }
                    wok = scanner.nextToken();
                }
                if(str == "min") ans = min(d);
                else if(str == "max") ans = max(d);
                else if(str == "average") ans = average(d);
                else if(str == "product") ans = product(d);
                else if(str == "sum") ans = sum(d);
                else if(str == "median") ans = median(d);
                else if(str == "stdev") ans = stdev(d);
                newcell->setnumdata(ans);
            }else{
                f += now->toString();
                string name = now->toString();
                //cout << name << " ";
                if(!nameIsValid(name)) {
                    error("Invalid cell name " + name);
                }
                newcell->adddepending(table[name]);
                table[name]->adddepended(newcell);
                newcell->setnumdata(table[name]->getnumdata());
            }
        }else if(now->getType() == COMPOUND){
            f += now->toString();
            now->eval(*this, newcell);
        }else{
            cout << "what can i sat" << "\n";
        }
        table[cellname] = newcell;
        formula.push_back(f);
        table[cellname]->setformula(f);
    }catch(ErrorException e){
        cout << e.getMessage() << "\n";
    }
}

void SSModel::printCellInformation(const string& cellname) {
    if(table.count(cellname)){
        cout << table[cellname]->getdepend();
    }else{
        cout << cellname << "is not content\n";
    }
}

void SSModel::writeToStream(ostream& outfile) const {
    for(auto it : formula){
        outfile << it.substr(2);
    }
}

void SSModel::readFromStream(istream& infile) {
    //ifstream
    string line;
    while(getline(infile,line)){
        TokenScanner scanner(line);
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.scanStrings();
        string cellname = scanner.nextToken();
        toLowerCaseInPlace(cellname);
        if (scanner.nextToken() != "=")
            error("= expected.");
        setCellFromScanner(cellname,scanner);
    }
    show();
    //infile.close();
}

void SSModel::clear(){
    for(auto it : table){
        it.second->~Cell();
    }
    table.clear();
    formula.clear();
    view->displayEmptySpreadsheet();
}

int SSModel::getrow() const{
    return numrow;
}

int SSModel::getcol() const{
    return numcol;
}

void SSModel::setrow(int i){
    numrow = i;
}

void SSModel::setcol(int i){
    numcol = i;
}

unordered_map<string,Cell*> SSModel::gettable() const{
    return table;
}

vector<string> SSModel::getformula() const{
    return formula;
}

void SSModel::addformula(string s){
    formula.push_back(s);
}

Cell* SSModel::getcell(string name) {
    if(table.count(name)){
        return table[name];
    }else{
        Cell* newcell = new Cell(name);
        table[name] = newcell;
        return  table[name];
    }
}

double SSModel::getnumValue(string name){
    if(nameIsValid(name)){
        return table[name]->getnumdata();
    }else{
        return 0;
    }
}

void SSModel::show(){
    for(auto wok : table){
        view->displayCell(wok.second);
        //cout << wok.second->getstrdata() << "\n";
    }
}

void SSModel::showcell(string cellname){
    view->displayCell(table[cellname]);
}


