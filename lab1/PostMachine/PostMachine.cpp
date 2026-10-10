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

