#include <list>
#include <iterator>
#include <sstream>
#include <string>

class Tape{
private:
    std::list<int> tape;
    std::list<int>::iterator cur;

public:
    Tape();
    int get_symbol() const;
    void set_symbol(int val);
    void move_right();
    void move_left();
    
};

class Command{
private:
    int number;
    char symbol;
    int fdigit;
    int sdigit;

public:
    Command(int number=-1,char symbol='!',int fdigit=-1,int sdigit=-1);
    static Command parser(const std::string& str);
    int get_number() const { return number; }
    char get_symbol() const { return symbol;}
    int get_fdigit() const {return fdigit;}
    int get_sdigit() const { return sdigit;}
    
};