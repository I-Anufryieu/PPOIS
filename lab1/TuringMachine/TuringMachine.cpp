#include "TuringMachine.h"

void Tape::move_right(){
    if(cur==std::prev(tape.end())) tape.push_back('_');
    ++cur;
}

void Tape::move_left(){
    if(cur==tape.begin()) tape.push_front('_');
    --cur;
}

std::ostream& operator<<(std::ostream& os,const Tape& tape){
    for(auto iter=tape.tape.begin();iter!=tape.tape.end();++iter){
        if(iter==tape.cur) os <<'['<< *iter <<']';
        else os << ' '<< *iter <<' ';
    }
    return os;
}

bool Program::get_transition(const std::string& state,char symbol,Transition& next_trans) const{
    auto find_state = state_table.find({state,symbol});
    if(find_state!=state_table.end()){
        next_trans=find_state->second;
        return true;
    }
    return false;
}