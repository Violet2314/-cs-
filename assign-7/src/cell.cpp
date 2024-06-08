
#include "cell.h"
#include <string>
using namespace std;

Cell::Cell(string now) {
    l.col = now[0];
    l.row = stoi(now.substr(1));
    strdata = "";
    formula = "";
    numdata = 0;
}

Cell::~Cell(){

}

void Cell::adddepended(Cell* cell){
    depended[cell->getCellName()] = cell;
}

void Cell::removedepended(Cell* cell){
    depended.erase(cell->getCellName());
}

void Cell::adddepending(Cell* cell){
    depending[cell->getCellName()] = cell;
}

void Cell::removedepending(Cell* cell){
    depending.erase(cell->getCellName());
}

string Cell::getkey(std::unordered_map<std::string,Cell*> now) const{
    string ans = "";
    for(auto it : now){
        ans += it.first;
        ans += " ";
    }
    return ans;
}

string Cell::getdepend() const{
    string ans = formula;
    ans = ans + "\n" + "Cells that " + getCellName() + "directly depends on: "
          + getkey(depending);
    ans += "\n";
    ans = ans + "Cells that directly depend on " + getCellName() + " "
          + getkey(depended) + "\n";
    return ans;
}

string Cell::getCellName() const{
    return l.col + to_string(l.row);
}

string Cell::getstrdata() const{
    return strdata;
}

double Cell::getnumdata() const{
    return numdata;
}

string Cell::getformule() const{
    return formula;
}

void Cell::setstrdata(string s){
    strdata = s;
    numdata = 0;
}

void Cell::setnumdata(double d){
    strdata = to_string(d);
    numdata = d;
}

void Cell::setformula(string s){
    formula = s;
}
