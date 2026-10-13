#include <iostream>
#include <list>
#include <iterator>
#include <string>
#include <map>
#include <set>

struct Transition{
    char next_symbol;
    char move_to;
    std::string next_state;
};

class Tape{
private:
    std::list<char> tape;
    std::list<char>::iterator cur;
public:
    Tape();

    void move_right();
    void move_left();
    char get_symbol() const{ return *cur;};
    void set_symbol(char symbol){ *cur = symbol; };
    friend std::ostream &operator<<(std::ostream& os,const Tape& tape);
};

class Program{
private:
    std::map<std::pair<std::string,char>,Transition> state_table;

public:
    void add_transition(const std::string& state, char symbol, const Transition& transition){
        state_table[{state,symbol}] = transition;
    }
    bool get_transition(const std::string& state, char symbol, Transition& next_trans) const;


};

class Parser{
private:
    std::string input_alphabet;
    std::string input_state;
    std::set<char> tape_alphabet;
    std::set<std::string> tape_state;
    Program program;

    
    

public:
    Program get_program() const{ return program;}

};