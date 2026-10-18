#include "../include/Tape.h"

Tape::Tape() {
    tape.push_back(BLANK_SYMBOL);
    cur = tape.begin();
}

Tape& Tape::operator=(const Tape& to_copy) {    
    if (this == &to_copy) return *this;
    tape = to_copy.tape;
    int dist = std::distance(to_copy.tape.cbegin(), std::list<char>::const_iterator(to_copy.cur));

    cur = tape.begin();
    std::advance(cur, dist);
    
    return *this;
}

Tape::Tape(const Tape& to_copy) : tape(to_copy.tape) {
    int dist = std::distance(to_copy.tape.cbegin(), std::list<char>::const_iterator(to_copy.cur));
    cur = tape.begin();
    std::advance(cur, dist);
}

void Tape::move_right() {
    if (cur == std::prev(tape.end())) tape.push_back('_');
    ++cur;
}

void Tape::move_left() {
    if (cur == tape.begin()) tape.push_front('_');
    --cur;
}

std::ostream& operator<<(std::ostream& os, const Tape& tape) {
    for (auto iter = tape.tape.begin(); iter != tape.tape.end(); ++iter) {
        if (iter == tape.cur) os << '[' << *iter << ']';
        else os << ' ' << *iter << ' ';
    }
    return os;
}

std::istream &operator>>(std::istream& is, Tape& tape) {
    tape.tape.clear();
    std::string initial_state;
    is >> initial_state;
    for (char c : initial_state) {
        tape.tape.push_back(c);
    }
    tape.cur = tape.tape.begin();
    return is;
}