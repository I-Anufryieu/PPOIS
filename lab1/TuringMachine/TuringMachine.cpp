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

void Parser::parse_tape_alphabet(const std::string& line){
    std::istringstream ss(line);
    char symbol;
    while(ss>>symbol){
        tape_alphabet.insert(symbol);
    }
}

void Parser::parse_tape_states(const std::string& line){
    std::istringstream ss(line);
    std::string state;
    while(ss>>state){
        tape_states.insert(state);
    }
}

std::string Parser::parse_and_add_transition(const std::string rule){
    std::istringstream ss(rule);
    std::string current_state, next_state;
    char read_symbol, write_symbol, move_direction;
    if (!(ss >> current_state >> read_symbol >> write_symbol >> move_direction >> next_state)) return "Ошибка: неверный формат правила";

    if (tape_states.find(current_state) == tape_states.end() || tape_states.find(next_state) == tape_states.end())
        return "Ошибка: одно из состояний не найдено";
    
    if (tape_alphabet.find(read_symbol) == tape_alphabet.end() || tape_alphabet.find(write_symbol) == tape_alphabet.end())
       return "Ошибка: один из символов отсутствует в алфавите";  

    if(move_direction!='R' && move_direction!='L' && move_direction != 'S') return "Ошибка: написано неверное направление (R, L, S)";

    Transition transition{write_symbol, move_direction, next_state};
    program.add_transition(current_state, read_symbol, transition);

    return "";
}
