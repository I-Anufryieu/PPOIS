#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <iostream>
#include <list>
#include <iterator>
#include <string>
#include <map>
#include <set>
#include <sstream>

constexpr char BLANK_SYMBOL = '_';
constexpr int MAX_STEPS = 1000;

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
    Tape(const Tape &to_copy);
    ~Tape() = default;
    void move_right();
    void move_left();

    char get_symbol() const{ return *cur;}
    void set_symbol(char symbol){ *cur = symbol; }
    void set_head_position(int position){
        cur=tape.begin();
        std::advance(cur,position);
    }

    Tape& operator=(const Tape& to_copy);

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

    bool operator==(const Program& other) const {
        return state_table == other.state_table;
    }

    bool operator!=(const Program& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Program& program);
    friend std::istream& operator>>(std::istream& is, Program& program);
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
    std::string parse_and_add_transition(const std::string& rule);

    bool operator==(const Parser& other) const {
        return tape_alphabet == other.tape_alphabet &&
               tape_states == other.tape_states &&
               program == other.program;
    }

    bool operator!=(const Parser& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Parser& parser);
};

class TuringMachine{
private:
    Tape tape;
    Program program;
    std::set<char> alphabet;
    std::string current_state;
public:
    bool step();
    const Tape& get_tape() const{ return tape; }
    std::string get_current_state() const{ return current_state; }
    void set_program(const Program& p) { program = p; }
    void set_alphabet(const std::set<char>& alph) { alphabet = alph; }
    void set_initial_state(const std::string& state) { current_state = state; }

    bool is_configured() const { return !alphabet.empty(); }
    std::string set_tape(const std::string& line);
    void set_head_position(int position) {tape.set_head_position(position);}

    bool operator==(const TuringMachine& other) const {
        return tape == other.tape &&
               program == other.program &&
               alphabet == other.alphabet &&
               current_state == other.current_state;
    }

    bool operator!=(const TuringMachine& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const TuringMachine& turing_machine);
    friend std::istream& operator>>(std::istream& is, TuringMachine& turing_machine);
};

#endif