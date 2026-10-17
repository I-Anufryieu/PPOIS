#include "PostMachine.h"

Tape::Tape(){
    tape.push_back(0);
    cur = tape.begin();
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
    
void PostMachine::add_command(const Command& cmd){
    auto unique_key = command.insert({cmd.get_number(), cmd});
    if (!unique_key.second) {
        throw std::invalid_argument("Ошибка: Команда с номером " + std::to_string(cmd.get_number()) + " уже существует!");
    }
}

std::ostream& operator<<(std::ostream& os, const Command& cmd) {
    char symbol = cmd.get_symbol();
    os << cmd.get_number() << " " << symbol;
    
    if (symbol == Command::SET1 || symbol == Command::SET0 || 
        symbol == Command::RIGHT || symbol == Command::LEFT) {
        os << " " << cmd.get_fdigit();
    } 
    else if (symbol == Command::BRANCH) {
        os << " " << cmd.get_fdigit() << " " << cmd.get_sdigit();
    }
    
    return os;
}

std::istream& operator>>(std::istream& is, Command& cmd) {
    int number = Command::INVALID_NUM;
    int fdigit = Command::INVALID_NUM;
    int sdigit = Command::INVALID_NUM;
    char symbol = Command::STOP;

    if (!(is >> number >> symbol)) {
        cmd = Command(Command::INVALID_NUM);
        return is;
    }
    if (symbol == Command::SET1 || symbol == Command::SET0 || symbol == Command::RIGHT || symbol == Command::LEFT) is >> fdigit;
    else if (symbol == Command::BRANCH)  is >> fdigit >> sdigit;
    else if (symbol != Command::STOP) {
        cmd = Command(Command::INVALID_NUM);
        is.setstate(std::ios::failbit);
        return is;
    }
    cmd = Command(number, symbol, fdigit, sdigit);
    return is;
}

Tape& Tape::operator=(const Tape& to_copy){    
    if(this == &to_copy) return *this;
    tape = to_copy.tape;
    int dist = std::distance(to_copy.tape.cbegin(), std::list<int>::const_iterator(to_copy.cur));

    cur = tape.begin();
    std::advance(cur, dist);
    
    return *this;
}

Tape::Tape(const Tape& to_copy) : tape(to_copy.tape){
    int dist = std::distance(to_copy.tape.cbegin(), std::list<int>::const_iterator(to_copy.cur));
    cur = tape.begin();
    std::advance(cur, dist);
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

bool PostMachine::step(){
    auto cmd_iterator = command.find(current_line);
    if(cmd_iterator == command.end()){
        throw std::out_of_range("Ошибка: Команды с номером " + std::to_string(current_line) + " не существует!");
    }
    const Command& cmd = cmd_iterator->second;
    char symbol = cmd.get_symbol();

    if(symbol == Command::STOP) return false;
    else if(symbol == Command::BRANCH){
        current_line = (tape.get_symbol() == 0) ? cmd.get_fdigit() : cmd.get_sdigit();
        return true;
    }
    else if (symbol == Command::SET1)  tape.set_symbol(1);
    else if (symbol == Command::SET0)  tape.set_symbol(0);
    else if (symbol == Command::RIGHT)  tape.move_right();
    else if (symbol == Command::LEFT)   tape.move_left();

    current_line = cmd.get_fdigit();
    
    return true;
}