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

Command Command::parser(const std::string& line){
   std::stringstream stream(line);
   char symbol='!';
   int number = -1,fdigit = -1,sdigit = -1;
   if (!(stream >> number >> symbol)) {
        return Command(-1);
    }
   if(symbol=='V' || symbol=='X' || symbol=='>' || symbol=='<' ) stream>>fdigit;
   else if(symbol=='?') stream>>fdigit>>sdigit;
   else if(symbol=='!') return Command(number,symbol);
   else return Command(-1);

   return Command(number,symbol,fdigit,sdigit);
}

Tape& Tape::operator=(const Tape& to_copy){    
    if(this==&to_copy) return *this;
    tape=to_copy.tape;
    int dist=std::distance(to_copy.tape.cbegin(),std::list<int>::const_iterator(to_copy.cur));

    cur = tape.begin();
    std::advance(cur, dist);
    
    return *this;
}

Tape::Tape(const Tape& to_copy) : tape(to_copy.tape){
    int dist=std::distance(to_copy.tape.cbegin(),std::list<int>::const_iterator(to_copy.cur));
    cur=tape.begin();
    std::advance(cur, dist);
}

std::ostream& operator<<(std::ostream& os,const Tape& tape){
    std::list<int>::const_iterator iter=tape.tape.begin();
    for(;iter!=tape.tape.end();++iter){
        if(iter==tape.cur) os <<'['<< *iter <<']';
        else os <<' '<< *iter <<' ';
    }
    return os;
}

std::istream& operator>>(std::istream& is,Tape& tape){
    std::string initial_state;
    is>>initial_state;
    tape.set_tape(initial_state,0);
    return is;
}

bool PostMachine::step(){
    if(command.find(current_line)==command.end()){
        throw std::out_of_range("Ошибка: Команды с номером " + std::to_string(current_line) + " не существует!");
    }
    Command cmd=command[current_line];
    char symbol=cmd.get_symbol();

    if(symbol=='!') return false;
    else if(symbol=='?'){
        current_line = (tape.get_symbol() == 0) ? cmd.get_fdigit() : cmd.get_sdigit();
        return true;
    }
    else if (symbol == 'V')  tape.set_symbol(1);
    else if (symbol == 'X')  tape.set_symbol(0);
    else if (symbol == '>')  tape.move_right();
    else if (symbol == '<')  tape.move_left();

    current_line = cmd.get_fdigit();
    
    return true;
}
