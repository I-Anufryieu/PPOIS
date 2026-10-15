#include "TuringMachine.h"

Tape::Tape() {
    tape.push_back(BLANK_SYMBOL);
    cur = tape.begin();
}

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

std::istream &operator>>(std::istream& is,Tape& tape){
    tape.tape.clear();
    std::string initial_state;
    is>>initial_state;
    for(char c : initial_state){
        tape.tape.push_back(c);
    }
    tape.cur = tape.tape.begin();
    return is;
}

bool Program::get_transition(const std::string& state,char symbol,Transition& next_transition) const{
    auto find_state = state_table.find({state,symbol});
    if(find_state!=state_table.end()){
        next_transition=find_state->second;
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

std::string Parser::parse_and_set_tape(const std::string& line, Tape& tape) const{
    if (line.empty()) return "Ошибка: строка ленты не может быть пустой";
    for (char c : line) {
        if (!tape_alphabet.contains(c))  return "Ошибка: символ '" + std::string(1, c) + "' отсутствует в алфавите ленты";
    }
    std::istringstream ss(line);
    ss >> tape;
    return "";
}
