#include <iostream>
#include <list>
#include <iterator>
#include <string>
#include <map>
#include <set>
#include <sstream>

constexpr char BLANK_SYMBOL = '_';

struct Transition{
    char next_symbol;
    char move_direction;
    std::string next_state;

    bool operator==(const Transition& other) const {
        return next_symbol == other.next_symbol &&
               move_direction == other.move_direction &&
               next_state == other.next_state;
    }

    bool operator!=(const Transition& other) const {
        return !(*this == other);
    }
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
    bool operator==(const Tape& other) const{
        return (this->tape == other.tape) && 
               (std::distance(this->tape.cbegin(), std::list<char>::const_iterator(this->cur)) == 
                std::distance(other.tape.cbegin(), std::list<char>::const_iterator(other.cur)));
    }
    bool operator!=(const Tape& other) const{ return !(*this==other); }
    friend std::ostream &operator<<(std::ostream& os,const Tape& tape);
    friend std::istream &operator>>(std::istream& is,Tape& tape);
};

class Program{
private:
    std::map<std::pair<std::string,char>,Transition> state_table;

public:
    void add_transition(const std::string& state, char symbol, const Transition& transition){
        state_table[{state,symbol}] = transition;
    }
    bool get_transition(const std::string& state, char symbol, Transition& next_transition) const;


};

class Parser{
private:
    std::set<char> tape_alphabet;
    std::set<std::string> tape_states;
    Program program;

    
public:
    Program get_program() const{ return program;}
    void parse_tape_alphabet(const std::string& line);
    void parse_tape_states(const std::string& line);
    const std::set<char>& get_tape_alphabet() const{ return tape_alphabet;}
    const std::set<std::string>& get_tape_states() const{ return tape_states;}
    std::string parse_and_add_transition(const std::string rule);

    std::string parse_and_set_tape(const std::string& line, Tape& tape) const;

};

class TuringMachine{
private:
    Tape tape;
    Program program;
    std::string current_state;
public:
    
};
