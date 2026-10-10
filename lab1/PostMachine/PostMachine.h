#include <list>
#include <iterator>
#include <sstream>
#include <string>
#include <map>

class Tape{
private:
    std::list<int> tape;
    std::list<int>::iterator cur;

public:
    Tape();
    Tape(const Tape &to_copy);
    int get_symbol() const;
    void set_symbol(int val);
    void move_right();
    void move_left();

    Tape& operator=(const Tape& to_copy);

};

class Command{
private:
    int number;
    char symbol;
    int fdigit;
    int sdigit;
public:
    Command(int number = -1, char symbol = '!', int fdigit = -1, int sdigit = -1)
        : number(number), symbol(symbol), fdigit(fdigit), sdigit(sdigit) {}
    static Command parser(const std::string& line);
    int get_number() const { return number; }
    char get_symbol() const { return symbol;}
    int get_fdigit() const {return fdigit;}
    int get_sdigit() const { return sdigit;} 
};

