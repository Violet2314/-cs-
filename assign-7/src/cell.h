#ifndef CELL_H
#define CELL_H
#include "ssutil.h"

class Cell
{
public:
    Cell(std::string now);
    ~Cell();

    std::string getstrdata() const;
    double getnumdata() const;
    std::string getformule() const;

    std::string getCellName() const;

    void setformula(std::string s);
    void setstrdata(std::string s);
    void setnumdata(double d);

    void adddepended(Cell* cell);
    void adddepending(Cell* cell);

    void removedepended(Cell* cell);
    void removedepending(Cell* cell);

    std::string getdepend() const;

    std::string getkey(std::unordered_map<std::string,Cell*> now) const;

private:
    location l;
    std::string strdata;
    std::string formula;
    double numdata;
    std::unordered_map<std::string,Cell*> depended,depending;
};

#endif // CELL_H
