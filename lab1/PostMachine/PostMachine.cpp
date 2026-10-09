#include "PostMachine.h"

Tape::Tape(){
    tape.push_back(0);
    cur=tape.begin();
}

int Tape::get_symbol() const{
    return *cur;
}

void Tape::set_symbol(int symbol){
    *cur=symbol;
}

void Tape::move_right(){
    if(cur==std::prev(tape.end())){
        tape.push_back(0);
    }
    ++cur;
}

void Tape::move_left(){
    if(cur==tape.begin()){
        tape.push_front(0);
    }
    --cur;
}