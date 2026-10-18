#include "Tape.h"

Tape::Tape(){
    tape.push_back(0);
    cur = tape.begin();
}

Tape::Tape(const Tape& to_copy) : tape(to_copy.tape){
    int dist = std::distance(to_copy.tape.cbegin(), std::list<int>::const_iterator(to_copy.cur));
    cur = tape.begin();
    std::advance(cur, dist);
}

void Tape::set_tape(const std::string& initial_state, int start_pos){
    tape.clear();
    for (char c : initial_state) {
        if (c == '1') tape.push_back(1);
        else if (c == '0' || c == ' ')  tape.push_back(0);
    }
    if (tape.empty())  tape.push_back(0);
    
    cur = tape.begin();
    for (int i = 0; i < start_pos && cur != std::prev(tape.end()); ++i) {
        ++cur;
    }
}

int Tape::get_symbol() const{
    return *cur;
}

void Tape::set_symbol(int symbol){
    *cur = symbol;
}

void Tape::move_right(){
    if(cur == std::prev(tape.end())) tape.push_back(0);
    ++cur;
}

void Tape::move_left(){
    if(cur == tape.begin()) tape.push_front(0);
    --cur;
}

Tape& Tape::operator=(const Tape& to_copy){    
    if(this == &to_copy) return *this;
    tape = to_copy.tape;
    int dist = std::distance(to_copy.tape.cbegin(), std::list<int>::const_iterator(to_copy.cur));

    cur = tape.begin();
    std::advance(cur, dist);
    
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Tape& tape){
    for(auto iter = tape.tape.begin(); iter != tape.tape.end(); ++iter){
        if(iter == tape.cur) os << '[' << *iter << ']';
        else os << ' ' << *iter << ' ';
    }
    return os;
}

std::istream& operator>>(std::istream& is, Tape& tape){
    std::string initial_state;
    is >> initial_state;
    tape.set_tape(initial_state, 0);
    return is;
}